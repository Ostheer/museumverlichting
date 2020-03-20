# Compiler

* compiler (new GCC > 4.8 for c++14) ripped from
https://github.com/abhiTronix/raspberry-pi-cross-compilers

* has link time optimization flags:
`-march=armv6 -mfloat-abi=hard -mfpu=vfp`

* exact link
https://iweb.dl.sourceforge.net/project/raspberry-pi-cross-compilers/Raspberry%20Pi%20GCC%20Cross-Compiler%20Toolchains/Stretch/GCC%209.2.0/Raspberry%20Pi%201%2C%20Zero/cross-gcc-9.2.0-pi_0-1.tar.gz

# Environment

* Create the folder `/home/ostheer/rpi/rootfs/`
* Copy the folders `/usr`, `/lib` and `/opt` from the Raspberry Pi into it
