STM32 CMake Projects
======

Personal test repository for STM32 projects using the `stm32-cmake` repository.

```sh
git submodule init
git submodule update --init --recursive
```

# Prerequisites for building

1. `CMake` installed
2. ARM cross-compiler installed. It is recommended to install the 
   [xPacks ARM cross compiler](https://xpack.github.io/arm-none-eabi-gcc/install/)
   because it offers good Eclipse integration.
3. Windows: It is strongly recommended to install and use a Unix environment
   like [MinGW64](https://www.msys2.org/). You can use `mingw32-make` or `ninja` to build
   the application
4. Debugging: [OpenOCD](https://xpack.github.io/openocd/) installed

## Setting up Windows MinGW64

Install MinGW64 as part of [MSYS2](https://www.msys2.org/) first.
Afer that, run the following command in the MinGW64 shell to install `make` and `CMake`

```sh
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-make
```

# Building the Minimal Project or the FreeRTOS project

The project expect the folder containing the toolchain binaries to be in the environment
path.

On Windows, when using MinGW64, add `-G "MinGW Makefiles"` to use MinGW makefiles or `-G "Ninja`
to use the Ninja build system.
You can flash the generated binary file to the connect board via drag-and-drop
or you can use Eclipse and OpenOCD to perform debugging with the `.elf` file.

## All Boards

Supply board: `-DBUILD_H743ZI=ON` and/or `-DBUILD_F303VC=ON`.

### Minimal Project

```sh
cd projects
mkdir build && cd build
cmake -D<BOARD_SELECT> ../minimal
cmake --build . -j
```

## H743ZI

## FreeRTOS Project

```sh
cd freertos
mkdir build && cd build
cmake ../freertos
cmake --build . -j
```

## LwIP UDP Echoserver Project

```sh
cd lwip-raw-udp-echoserver
mkdir build && cd build
cmake ../lwip-raw-udp-echoserver
cmake --build . -j
```

# Testing the LwIP UDP Echoserver

Connect the STM32H743ZIT to your router via Ethernet. The STM32 device should now get
an IP address via DHCP. Check that the IP address in your router configuration or retrieve
it from the serial console. The server is listening on requests on port 7. You can test it
with the following command, using netcat:

```sh
echo "Hello World" | nc -w1 -u <ip-address> 7
```

# Using VS Code

VS code files were supplied to perform debugging and building with VS code after the build system was
generated. The only prerequsite is to install the Cortex-Debug plugin. You can do this in the command
line with

```sh
code --install-extension marus25.cortex-debug
```

