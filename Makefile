# Semi replacement for install.sh - 
# Mainly to understand how it works

QEMU_DIR=qemu-eos
QEMU_NAME=qemu-4.2.1
DIR=../qemu-eos

all: clean init patch qemu-eos compile

init:
	-@mkdir $(DIR)
	cd $(DIR); wget -nc https://download.qemu.org/$(QEMU_NAME).tar.bz2
	cd $(DIR); tar jxf $(QEMU_NAME).tar.bz2

PATCH_DIR=$(DIR)/$(QEMU_NAME)
CODEBASE_URL=https://raw.githubusercontent.com/reticulatedpines/magiclantern_simplified/dev
patch:
	-@mkdir -p $(PATCH_DIR)/hw/eos
	-@mkdir -p $(PATCH_DIR)/hw/eos/dbi

	# Copy scripts
	cp -r scripts/* $(DIR)
	chmod +x $(DIR)/*.sh

	# Setup QEMU images
	cp -v sd.img.xz $(DIR)/
	cd $(DIR); unxz -v sd.img.xz
	cd $(DIR); cp -v sd.img cf.img

	# Patch remotely from magiclantern_simplified
	cd $(PATCH_DIR)/hw/eos/dbi; wget -4 $(CODEBASE_URL)/src/backtrace.c
	cd $(PATCH_DIR)/hw/eos/dbi; wget -4 $(CODEBASE_URL)/src/backtrace.h

	# Main QEMU patches
	patch -N -p1 --directory=$(PATCH_DIR) < $(QEMU_NAME)-gcc78.patch # GCC 7+
	patch -N -p1 --directory=$(PATCH_DIR) < $(QEMU_NAME).patch
	cp -r eos/* $(PATCH_DIR)/hw/eos

# Compile qemu-eos CLI program - Inserts the
# qemu directory into the program as a constant
# (it needs to compile the "../qemu-eos" string)
qemu-eos:
	sudo gcc -DQEMU_DIR='"$(shell cd $(DIR); pwd)"' qemu-eos.c -o /bin/qemu-eos

# Compile QEMU
CPU=$(shell grep -c processor /proc/cpuinfo 2> /dev/null || sysctl -n hw.ncpu 2> /dev/null || echo 1)
compile:
	cd $(PATCH_DIR); ../configure_eos.sh
	cd $(PATCH_DIR); make -j$(CPU)
	
clean:
	rm -rf *.tar.bz2*

	# Remove everything except qemu tar
	cd $(DIR); ls | grep -v $(QEMU_NAME).tar.bz2 | xargs rm -rf
