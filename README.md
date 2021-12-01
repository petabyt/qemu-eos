# Simplified qemu-eos
Modifications:  
- Added `Makefile`
- Added `qemu-eos.c`
- Added a .gitignore
- Removed old info (doc, HACKING.rst, README.rst, tests)

## Usage
```
# Pull qemu, patch, and compile
make
```

```
# Mount sd.img:
qemu-eos mount
# Press any key to remove the device when you unmount it
```

```
# Emulate something
qemu-eos emu 5D3
```

If you would like to install the `qemu-eos` CLI program  
with an existing installation of QEMU, run this:  
```
sudo gcc -DQEMU_DIR='"<DIRECTORY OF QEMU-EOS>"' qemu-eos.c -o /bin/qemu-eos
```
