#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef QEMU_DIR
	#define QEMU_DIR "~/qemu-eos"
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
			puts("Usage: ./qemu-eos <ACTION> <MODEL>");
			return -1;
		}

		char output[64];
		findDebug("1300D/debugmsg.gdb", output);

		snprintf(cmd, sizeof(cmd),
			// Send the debug 
			"cd %s; env QEMU_EOS_DEBUGMSG=\"%s\"" \
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

		system(cmd);
	} else if (!strcmp(argv[1], "mount")) {
		if (system("which kpartx")) {
			puts("Can't find kpartx. Try installing util-linux in your package manager.");
			return -1;
		}

		puts("Will mount and unmount with sudo.");

		system("sudo kpartx -av sd.img");
		puts("Do copy files, press enter to unmount.");
		fgetc(stdin);
		while (system("sudo kpartx -dv sd.img")) {
			puts("Drive is mounted. Please unmount and try again.");
			fgetc(stdin);
		}
	}
}
