echo "Entering stm32-cmake environment"
cross_bin_path="~/.local/xPacks/@xpack-dev-tools/arm-none-eabi-gcc/10.2.1-1.1.2/.content/bin"
export PATH=$PATH:${cross_bin_path}
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

exec /bin/bash
