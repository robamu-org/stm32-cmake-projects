echo "Entering stm32-cmake environment"
export CONSOLE_PREFIX="[stm32-cmake]"
cd STM32CubeH7
export STM32_CUBE_H7_PATH=$(pwd)
echo "Setting STM32_CUBE_H7_PATH to ${STM32_CUBE_H7_PATH}"
cd ..
cd STM32CubeF4
export STM32_CUBE_F4_PATH=$(pwd)
echo "Setting STM32_CUBE_F4_PATH to ${STM32_CUBE_F4_PATH}"
cd ..
cd STM32CubeF3
export STM32_CUBE_F3_PATH=$(pwd)
echo "Setting STM32_CUBE_F3_PATH to ${STM32_CUBE_F3_PATH}"
cd ..

export PATH="$(pwd)/xpacks/.bin":$PATH
export STM32_TOOLCHAIN_PATH="$(pwd)/xpacks/xpack-dev-tools-arm-none-eabi-gcc/.content/arm-none-eabi
"
export REPO_ROOT="$(pwd)"

exec /bin/bash
