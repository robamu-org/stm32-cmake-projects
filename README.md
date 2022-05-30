STM32 CMake Projects
======

Personal test repository for STM32 projects using the `stm32-cmake` repository.

```sh
git submodule init
git submodule update --init --recursive
```

# Prerequisites

If you have not set up the prerequisites yet, go to the [prerequisites chapter](#prereq) for
detailed instructions on how to install these.

Here is a brief overview of the required tools to develop software for the STM32H7 microcontroller:

1. CMake build system generator installed
2. Build system like [Ninja Build](https://ninja-build.org/) or [Make](https://www.msys2.org/)
   installed.
3. Bare-Metal ARM toolchain installed
4. Recommended for application code development: 
   [Eclipse for C/C++](https://www.eclipse.org/downloads/packages/) installed with the Eclipse MCU 
   plugin
5. [OpenOCD](https://xpack.github.io/openocd/) installed for Eclipse debugging
6. STM32 USB drivers installed, separate steps for 
   [Windows](https://www.st.com/en/development-tools/stsw-link009.html) or 
   [Linux](https://fishpepper.de/2016/09/16/installing-using-st-link-v2-to-flash-stm32-on-linux/)

# Building the Projects

The project expect the folder containing the toolchain binaries to be in the environment
path.

On Windows, when using MinGW64, add `-G "MinGW Makefiles"` to use MinGW makefiles or `-G "Ninja`
to use the Ninja build system.
You can flash the generated binary file to the connect board via drag-and-drop
or you can use Eclipse and OpenOCD to perform debugging with the `.elf` file.

## Multi-Board Projects

Supply board: `-DBUILD_H743ZI=ON` and/or `-DBUILD_F303VC=ON`.

### Minimal Project

```sh
cd projects
mkdir build && cd build
cmake -D<BOARD_SELECT> ../minimal
cmake --build . -j
```

### FreeRTOS Project

```sh
cd projects
mkdir build && cd build
cmake -D<BOARD_SELECT> ../freertos
cmake --build . -j
```

## H743ZI

## LwIP UDP Echoserver Project

```sh
cd projects
mkdir build && cd build
cmake ../h743zi/lwip-raw-udp-echoserver
cmake --build . -j
```

# <a id="prereq"></a> Setting up Prerequisites

## CMake

**Linux**

```sh
sudo apt-get install cmake
```

**Windows**

On Windows, you can use `pacman -S mingw-w64-x86_64-cmake`, but you can also install the Windows
CMake via the [installer](https://cmake.org/download/). It is recommended to pick the install
option `Add CMake to system PATH for all users` to CMake can be used from the command line.
Please note that you need to add the Windows CMake path to the MinGW64 path manually
if you want to use it in CMake.

## Cross-Compiler

The instuctions here specify how to install and use a specific version of the
[xPacks cross-compiler](https://xpack.github.io/arm-none-eabi-gcc/) but you can use any other
ARM cross-compiler which can generate bare-metal code, usually denoted by the `arm-none-eabi`
cross-compiler triplet.

If you don't want to install nodejs you may go with the 
[four-command manual installation](https://xpack.github.io/arm-none-eabi-gcc/install/#manual-install). 

**Windows**

On Windows, it is recommended to perform the `xpm` and toolchain installation from the Windows
command line.The simple way required npm, which can be installed by installing
[NodeJS](https://nodejs.org/en/). Make sure `npm` can be run from the command line by adding
the folder containing `npm.exe` to the system path and running the following command 

```sh
npm install --global xpm@latest
xpm install --global @xpack-dev-tools/arm-none-eabi-gcc@11.2.1-1.1.1 --verbose
```

On Windows, the toolchain binaries will be located in a folder like this

```sh
C:\Users\<User>\AppData\Roaming\xPacks\@xpack-dev-tools\arm-none-eabi-gcc\<version>\.content\bin
```

You can now run the following commands in the repository root:

```sh
xpm init
xpm install @xpack-dev-tools/arm-none-eabi-gcc@11.2.1-1.1.1
```

to create symlinks to the toolchain in `./xpacks/.bin`.
You can now set up the environment by using `. load_path.sh` or the following command

```sh
export PATH="$(pwd)/xpacks/.bin":$PATH
```

On Windows, you can use git bash to perform the step above.

Alternatively, you can use the graphical system environmental variables editor to add the
`.bin` path to the system variables permanently or use the appriate command for `CMD` or
PowerShell to update the `PATH` variable.

**Linux**

Install `npm` and `nodejs` first. Example for Ubuntu according to
[this guide](https://linuxize.com/post/how-to-install-node-js-on-ubuntu-20-04/).

```sh
curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash -
sudo apt-get update && sudo apt-get install nodejs
```

Check that `npm` is installed with `npm --version`.
Then `xpm` and the cross-compiler are installed.

```sh
sudo npm install --global xpm@latest
xpm install --global @xpack-dev-tools/arm-none-eabi-gcc@11.2.1-1.1.1 --verbose
```

You can now run the following commands in the repository root:

```sh
xpm init
xpm install @xpack-dev-tools/arm-none-eabi-gcc@11.2.1-1.1.1
```

to create symlinks to the toolchain in `./xpacks/.bin`.
You can now set up the environment by using `. load_path.sh` or the following command

```sh
export PATH="$(pwd)/xpacks/.bin":$PATH
```

## Build System

It is recommended to use `ninja` or `make` as the software build system.

**Windows**

It is recommended to use the [Ninja build system](https://ninja-build.org/). Download the ninja
executable and place it somewhere.
You need to add the folder containing the ninja executable to the system environmental variables
so it can be used in the build process. You can test whether `ninja` works by running
`ninja --version` in the command line.

Alternatively or additionally, you can also install `mingw32-make`
which comes bundled with [MinGW64](https://www.msys2.org/). Make sure to add the binaries path to
the Windows path during installation. Otherwise, you need to add `msys64/mingw64/bin` to the
Windows path so you can run installed binaries from the command line.
Open the `MinGW64` shell and run the following commands.

```sh
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-make mingw-w64-x86_64-cmake
```

You can test successfull installation with `mingw32-make -v` from the Windows Command Line.

**Linux**

On Linux, `make` is pre-installed and it is recommended to use it directly.
You can install `ninja` with

```sh
sudo apt-get install ninja-build
```

## USB Drivers

**Windows**

Install the [STM32 USB drivers](https://www.st.com/en/development-tools/stsw-link009.html).

**Linux**

Install the [USB drivers](https://github.com/stlink-org/stlink) on Linux.
On Ubuntu, you can run the following command to install it:

```sh
sudo apt-get install stlink-tools
```

# Debugging the application

Some basic OpenOCD launch configurations were provided. It is expected that the user starts
the OpenOCD application.

For example, to open an OpenOCD session specifically for the STM32H743ZI-Nucleo, perform
the following steps

```sh
cd projects/h743zi
openocd
```

Now you can flash the application with a GDB application like `arm-none-eabi-gdb` or `gdb-multiarch`.
You can try this in the command line as well (here with `gdb-multiarch`):

```sh
cd projects
gdb-multiarch -q -x h743zi/openocd.gdb build/stm32.elf
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
