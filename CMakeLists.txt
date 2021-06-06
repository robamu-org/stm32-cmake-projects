cmake_minimum_required(VERSION 3.13)
# set(STM32_CMAKE_DIR $ENV{STM32_CMAKE_DIR})

set(STM32_CUBE_H7_PATH STM32CubeH7)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
set(TARGET_TRIPLET "arm-none-eabi")
set(FREERTOS_PATH FreeRTOS-Kernel)

set(CMAKE_TOOLCHAIN_FILE stm32-cmake/cmake/stm32_gcc.cmake)

project(stm32-freertos CXX C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(stm32-cmake/cmake/stm32/devices.cmake)
stm32_print_devices_by_family(FAMILY H7)

find_package(CMSIS COMPONENTS STM32H743ZI REQUIRED)
find_package(HAL COMPONENTS STM32H743ZI REQUIRED)
find_package(FreeRTOS COMPONENTS ARM_CM7 REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    FreeRTOSConfig.h
)

add_executable(stm32-freertos ${PROJECT_SOURCES} stm32h7xx_hal_conf.h)
target_link_libraries(stm32-freertos PRIVATE
    FreeRTOS::Timers
    FreeRTOS::Heap::1
    FreeRTOS::ARM_CM7 
    HAL::STM32::H7::RCC
    HAL::STM32::H7::GPIO
    HAL::STM32::H7::CORTEX
    CMSIS::STM32::H743ZI
    STM32::NoSys
)
