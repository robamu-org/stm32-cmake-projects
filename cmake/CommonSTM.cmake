function(common_config)

option(BUILD_H743ZI "Build for H743ZI board" OFF)
option(BUILD_F303VC "Build for F303VC board" OFF)

if(NOT BUILD_H743ZI AND NOT BUILD_F303VC)
    message(
        FATAL_ERROR "Please pick at least one board and supply it with -DBUILD_<BOARD>=ON"
    )
endif()

set(STM32_CUBE_H7_PATH ${REPO_ROOT}/STM32CubeH7 PARENT_SCOPE)
set(STM32_CUBE_F3_PATH ${REPO_ROOT}/STM32CubeF3 PARENT_SCOPE)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH} PARENT_SCOPE)
set(TARGET_TRIPLET "arm-none-eabi" PARENT_SCOPE)
set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel PARENT_SCOPE)
set(STM32_CMAKE_PATH ${REPO_ROOT}/stm32-cmake)
set(STM32_CMAKE_PATH ${STM32_CMAKE_PATH} PARENT_SCOPE)

# This must come before the project call!
set(CMAKE_TOOLCHAIN_FILE ${STM32_CMAKE_PATH}/cmake/stm32_gcc.cmake PARENT_SCOPE)

endfunction()
