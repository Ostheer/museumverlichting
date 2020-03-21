# Compiler

* compiler (new GCC > 4.8 for c++14) ripped from
https://github.com/abhiTronix/raspberry-pi-cross-compilers

* has link time optimization flags:
`-march=armv6 -mfloat-abi=hard -mfpu=vfp`

* exact link
https://iweb.dl.sourceforge.net/project/raspberry-pi-cross-compilers/Raspberry%20Pi%20GCC%20Cross-Compiler%20Toolchains/Stretch/GCC%209.2.0/Raspberry%20Pi%201%2C%20Zero/cross-gcc-9.2.0-pi_0-1.tar.gz

# Environment
Install any libraries that are necessary for the final program on the Pi before this

* Create the folder `/home/ostheer/rpi-chain/rootfs/`
* Copy the folders `/usr`, `/lib` and `/opt` from the Raspberry Pi into it

# Library version hell

Sometimes the binary is compiled against a library in the toolchain which is of an incompatible version with the one natively present on the Raspberry Pi:

```
pi@raspberrypi:~ $ sudo TOKEN=MYTOKEN ./main 
./main: /usr/lib/arm-linux-gnueabihf/libstdc++.so.6: version `CXXABI_1.3.11' not found (required by ./main)
```

Possible solutions:
* You can try to update the local library (on the Pi)
* You can try to downgrade the version in the toolchain (e.g. copy the older version from the Pi to the toolchain folder)
* You can copy the object from the toolchain folder (such as `/home/ostheer/rpi-chain/cross-pi-gcc-9.2.0-0/arm-linux-gnueabihf/lib/libstdc++.so.6`) to the corresponding location on the Raspberry Pi
