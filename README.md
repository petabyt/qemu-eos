# Simplified qemu-eos
Modifications:  
- Added `Makefile`
- Added `qemu-eos.c`
- Added a .gitignore

## Usage
```
# Pull qemu, patch, and compile
make
# copy ROM files
qemu-eos run 1300D
```

If you would like to install the `qemu-eos` CLI program  
with an existing installation of QEMU, run this:  
```
sudo gcc -DQEMU_DIR='"<DIRECTORY OF QEMU-EOS>"' qemu-eos.c -o /bin/qemu-eos
```
