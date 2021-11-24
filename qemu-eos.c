#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef QEMU_DIR
	#define QEMU_DIR "~/qemu-eos"
#endif

int main(int argc, char *argv[]) {
	if (argc < 2) {
		puts("Usage: ./qemu-eos <ACTION>");
		puts("Actions: run, mount");
		return -1;
	}

	char cmd[1024];
	if (!strcmp(argv[1], "emu")) {
		if (argc < 3) {
			puts("Usage: ./qemu-eos <ACTION> <MODEL>");
			return -1;
		}

		snprintf(cmd, sizeof(cmd),
		"cd %s; ./run_canon_fw.sh %s,firmware=\"boot=1\"", QEMU_DIR, argv[2]);

		system(cmd);
	} else if (!strcmp(argv[1], "mount")) {
		system("sudo kpartx -av sd.img");
		puts("Do copy files, press enter to unmount.");
		fgetc(stdin);
		while (system("sudo kpartx -dv sd.img")) {
			puts("Drive is mounted. Please unmount and try again.");
			fgetc(stdin);
		}
	}
}
