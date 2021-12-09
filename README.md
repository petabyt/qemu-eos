# Simplified qemu-eos
Experiments and stuff to understand ML qemu  

Modifications:  
- Added `Makefile`
- Added `qemu-eos.c`
- Added `.gitignore`
- Removed some info to declutter (doc, HACKING.rst, README.rst, tests)

## Usage
```
# Pull qemu, patch, and compile
make
```

```
# Mount sd.img:
qemu-eos mount
# Press any key to remove the device once you unmount it
```

```
# Start the emulator
qemu-eos emu 5D3
```

To install `qemu-eos` into an existing installation:
```
sudo gcc -DQEMU_DIR='"<DIRECTORY OF QEMU-EOS>"' qemu-eos.c -o /bin/qemu-eos
```
