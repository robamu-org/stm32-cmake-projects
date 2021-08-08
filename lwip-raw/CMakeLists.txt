cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-lwip)
set(REPO_ROOT "..")
set(CMAKE_SCRIPT_PATH ${REPO_ROOT}/cmake)
set(STM32_CMAKE_PATH  ${REPO_ROOT}/stm32-cmake)

# Please note: When using CMSIS, it is recommended to use the FreeRTOS version supplied in the 
# Cube repository because more recent kernels might be incompatible
option(USE_CMSIS_RTOS "Use CMSIS RTOS provided by Cube repository" ON)
option(USE_CUBE_RTOS "Use FreeRTOS provided by Cube repository" ON)

set(STM32_CUBE_H7_PATH ${REPO_ROOT}/STM32CubeH7)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
set(TARGET_TRIPLET "arm-none-eabi")


if(USE_CUBE_RTOS)
    set(FREERTOS_PATH ${STM32_CUBE_H7_PATH}/Middlewares/Third_Party/FreeRTOS)
else()
    set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel)
endif()

# This must come before the project call!
set(CMAKE_TOOLCHAIN_FILE ${STM32_CMAKE_PATH}/cmake/stm32_gcc.cmake)
project(${TARGET_NAME} CXX C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# include(${STM32_CMAKE_PATH}/cmake/stm32/devices.cmake)
# stm32_print_devices_by_family(FAMILY H7)

find_package(CMSIS COMPONENTS STM32H743ZI_M7 RTOS REQUIRED)
find_package(HAL COMPONENTS STM32H7M7 RCC GPIO CORTEX ETH PWR TIM UART REQUIRED)
find_package(FreeRTOS COMPONENTS ARM_CM7 REQUIRED)
find_package(LwIP REQUIRED)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.c
)

add_subdirectory(src)
add_subdirectory(inc)
add_subdirectory(lan8742)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME} PRIVATE
    FreeRTOS::Timers
    FreeRTOS::Heap::4
    FreeRTOS::ARM_CM7 
    HAL::STM32::H7::M7::RCC
    HAL::STM32::H7::M7::RCCEx
    HAL::STM32::H7::M7::GPIO
    HAL::STM32::H7::M7::CORTEX
    HAL::STM32::H7::M7::ETH
    HAL::STM32::H7::M7::TIM
    HAL::STM32::H7::M7::TIMEx
    HAL::STM32::H7::M7::UART
    HAL::STM32::H7::M7::PWREx
    CMSIS::STM32::H743xx::M7
    CMSIS::STM32::H7::M7::RTOS
    LwIP
    LwIP::IPv4
    LwIP::NETIF
    STM32::NoSys
    STM32::Nano
    STM32::Nano::FloatPrint
    STM32::Nano::FloatScan
)

stm32_print_size_of_target(${TARGET_NAME})
# LwIP requires adapted linker script for special sections
stm32_add_linker_script(${TARGET_NAME} PRIVATE STM32H743ZITx_FLASH.ld)

add_custom_command(
	TARGET ${TARGET_NAME}
	POST_BUILD
	COMMAND echo Generating binary file ${CMAKE_PROJECT_NAME}.bin..
	COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin
)

include(${CMAKE_SCRIPT_PATH}/BuildType.cmake)
set_build_type()

