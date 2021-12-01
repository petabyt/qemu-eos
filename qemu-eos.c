#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// To keep linter happy
#ifndef QEMU_DIR
	#define QEMU_DIR ""
#endif

// Get DebugMsg_log address
int findDebug(char address[], char buffer[]) {
	FILE *f = fopen(address, "r");
	if (f == NULL) {
		return -1;
	}

	/* Go through the file, and find the "0xC14" around this:
	# b *0xC14
	# DebugMsg_log
	*/

	char line[128];
	while (fgets(line, 128, f) != NULL) {
		if (!strncmp(line, "# b *", 5)) {
			char line2[128];
			fgets(line2, 128, f);
			if (!strncmp(line2, "# DebugMsg_log", 14)) {
				// Increment past "# b *", and strip newline
				strcpy(buffer,
					strtok(line + 5, "\n")
				);
			}
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		puts("Usage: ./qemu-eos <ACTION>");
		puts("Actions: emu, mount");
		return -1;
	}

	char cmd[1024];
	if (!strcmp(argv[1], "emu")) {
		if (argc < 3) {
			puts("Usage: ./qemu-eos emu <MODEL>");
			return 1;
		}

		char output[64];
		findDebug("1300D/debugmsg.gdb", output);

		snprintf(cmd, sizeof(cmd),
			// Send the debug address
			"cd %s; env QEMU_EOS_DEBUGMSG=\"%s\"" \

			// Start qemu patched qemu
			" qemu-*/arm-softmmu/qemu-system-arm" \
			" -drive if=sd,format=raw,file=sd.img" \
			" -drive if=ide,format=raw,file=cf.img" \
			" -chardev socket,server,nowait,path=qemu.monitor$QEMU_JOB_ID,id=monsock" \
			" -mon chardev=monsock,mode=readline" \
			" -name %s" \
			" -M %s,firmware=\"boot=1\"",
			QEMU_DIR,
			output,
			argv[2], argv[2]
		);

		printf("Will execute: %s\n", cmd);

		system(cmd);
	} else if (!strcmp(argv[1], "mount")) {
		snprintf(cmd, sizeof(cmd), "kpartx -av %s/sd.img", QEMU_DIR);
		if (system(cmd)) {
			puts("\nFailed to run kpartx, command not found or you're not running as superuser.");
			puts("If command not found, install 'util-linux' in your package manager.");
			return 1;
		}

		puts("sd.img mounted. Copy your files, and press enter to unmount.");
		fgetc(stdin);

		snprintf(cmd, sizeof(cmd), "kpartx -dv %s/sd.img", QEMU_DIR);
		while (system(cmd)) {
			puts("Drive is mounted. Please unmount and try again.");
			fgetc(stdin);
		}
	}

	return 0;
}
