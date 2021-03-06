export ARCH:=arm
export CROSS_COMPILE:=arm-linux-gnueabihf

## Setting library locations
#Compiling with these works, but then the shared objects can't be found on the rpi. Solved by placing the .so in the /usr/lib/arm... folder on the RPI
#BOOST_HEADERS = /home/ostheer/rpi-chain/boost_1_67_0 
#BOOST_OBJECTS = /home/ostheer/rpi-chain/boost_1_67_0/lib
#Compiling with these finds boost, but disallows the linker to find pthread. Solved by copying shared objects to /lib/arm... and /usr/lib/arm...
BOOST_HEADERS = /home/ostheer/rpi-chain/rootfs/usr/include
BOOST_OBJECTS = /home/ostheer/rpi-chain/rootfs/usr/lib/arm-linux-gnueabihf
#Geral locations containing all(?) shared objects. Telling the linker about these doesnt work. They seem to be ignored entirely
#USR_LIB_DIR_OBJ = /home/ostheer/rpi-chain/rootfs/usr/lib/arm-linux-gnueabihf
#LIB_DIR_OBJ = /home/ostheer/rpi-chain/rootfs/lib/arm-linux-gnueabihf
#Local static libraries (locally compiled .a files)
STATIC_LOCAL_LIBS =/home/ostheer/Documents/museumverlichting/lib/

## Compiler settings
CXX = ~/rpi-chain/cross-pi-gcc-9.2.0-0/bin/arm-linux-gnueabihf-g++
CPPFLAGS = -std=c++14 -I./tgbot -I${BOOST_HEADERS}
LDFLAGS = 
#LDFLAGS = --verbose --sysroot=/home/ostheer/rpi-chain/rootfs
LDLIBS = -L${STATIC_LOCAL_LIBS} -L${BOOST_OBJECTS} -lrt -lboost_system -lssl -lcrypto -lTgBot -lpthread -ldl


## Targets
main: main.o ws2812-rpi.o tools.o callbacks.o
	${CXX} $(LDFLAGS) -o main main.o ws2812-rpi.o tools.o callbacks.o $(LDLIBS)

main.o: main.cpp main.h
	${CXX} ${CPPFLAGS} -o main.o -c main.cpp

callbacks.o: callbacks.cpp callbacks.h main.h
	${CXX} ${CPPFLAGS} -o callbacks.o -c callbacks.cpp

tools.o: tools.cpp tools.h
	${CXX} ${CPPFLAGS} -o tools.o -c tools.cpp

ws2812-rpi.o: ./ws2812/ws2812-rpi.cpp ./ws2812/ws2812-rpi.h ./ws2812/ws2812-rpi-defines.h
	${CXX} ${CPPFLAGS} -c -o ws2812-rpi.o ./ws2812/ws2812-rpi.cpp

clean:
	rm -f main main.o ws2812-rpi.o tools.o callbacks.o


## Comments
#'default' sysroot, dependent on compiler location (check with linker option --verbose)
#--sysroot=/home/ostheer/rpi-chain/cross-pi-gcc-9.2.0-0/bin/../arm-linux-gnueabihf/libc

#When using a custom boost (from a different/arbitrary directory):
#one crude solution is to copy "libboost_system.so.1.67.0" from "/home/ostheer/rpi/boost_1_67_0/lib" (compiled on PC) to "raspberry:/usr/lib/arm-linux-gnueabihf/libboost_system.so.1.67.0".

#Solution to some missing standard libraries:
#I have had to copy these files because I couldn't figure out how on earth to tell the linker to look in a different directory
#sudo cp	/home/ostheer/rpi-chain/rootfs/usr/lib/arm-linux-gnueabihf/libpthread_nonshared.a	/usr/lib/arm-linux-gnueabihf
#sudo cp	/home/ostheer/rpi-chain/rootfs/usr/lib/arm-linux-gnueabihf/libc_nonshared.a		/usr/lib/arm-linux-gnueabihf
#sudo cp	/home/ostheer/rpi-chain/rootfs/lib/arm-linux-gnueabihf/libpthread.so.0			/lib/arm-linux-gnueabihf
#sudo cp	/home/ostheer/rpi-chain/rootfs/lib/arm-linux-gnueabihf/libc.so.6			/lib/arm-linux-gnueabihf
#sudo cp	/home/ostheer/rpi-chain/rootfs/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3		/lib/arm-linux-gnueabihf
