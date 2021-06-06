# STM32H743ZI-Nucleo Minimal CMake project

This repository provides most components to cross-compile with `CMake` for the STM32 Nucleo-H743ZI
board.

## Prerequisites for building

1. `CMake` installed
2. ARM cross-compiler installed. It is recommended to install the 
   [xPacks ARM cross compiler](https://xpack.github.io/arm-none-eabi-gcc/install/)
   because it offers good Eclipse integration.
3. Windows: It is strongly recommended to install and use a Unix environment
   like [MinGW64](https://www.msys2.org/)

## Building the Minimal Project or the FreeRTOS project

The project expect the cross-compiler path to be set in the `STM32_TOOLCHAIN_PATH` environmental
variable. Assuming you use a Unix environment, you can set up the environmental path like this

```sh
export STM32_TOOLCHAIN_PATH=<pathToToolchainBinaries>
```

On Windows, when using MinGW64, add `-G "MinGW Makefiles"` to use MinGW makefiles.

### Minimal Project

```sh
cd minimal
mkdir build && cd build
cmake ..
```

### FreeRTOS Project

```sh
cd freertos
mkdir build && cd build
cmake ..
```
