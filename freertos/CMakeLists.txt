cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-freertos)
set(REPO_ROOT "..")
set(STM32_CMAKE_PATH  ${REPO_ROOT}/stm32-cmake)

# Please note: When using CMSIS, it is recommended to use the FreeRTOS version supplied in the 
# Cube repository because more recent kernels might be incompatible
option(USE_CMSIS_RTOS "Use CMSIS RTOS provided by Cube repository" ON)
option(USE_CUBE_RTOS "Use FreeRTOS provided by Cube repository" ON)
option(USE_CMSIS_RTOS_V2 "Use CMSIS RTOSV2 provided by Cube repository" OFF)
if(USE_CMSIS_RTOS AND NOT USE_CUBE_RTOS)
    message(WARNING "Using CMSIS RTOS but not using Cube RTOS. Build might fail")
endif()
if(USE_CUBE_RTOS AND USE_CMSIS_RTOS_V2)
    message(FATAL_ERROR "Can't use USE_CUBE_RTOS together with USE_CMSIS_RTOS_V2")
endif()

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

set(CMSIS_COMP_LIST STM32H743ZI STM32H7_M7)
set(FREERTOS_COMP_LIST ARM_CM7)
set(FREERTOS_NAMESPACE FreeRTOS)

if(USE_CMSIS_RTOS)
    list(APPEND CMSIS_COMP_LIST RTOS)
    list(APPEND FREERTOS_COMP_LIST STM32H7)
    set(FREERTOS_NAMESPACE ${FREERTOS_NAMESPACE}::STM32::H7::M7)
endif()

find_package(CMSIS COMPONENTS ${CMSIS_COMP_LIST} REQUIRED)
find_package(HAL COMPONENTS STM32H7M7 STM32H743ZI RCC GPIO CORTEX REQUIRED)
find_package(FreeRTOS COMPONENTS ${FREERTOS_COMP_LIST} REQUIRED)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
    FreeRTOSConfig.h
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(stm32-freertos PRIVATE
    ${FREERTOS_NAMESPACE}::Timers
    ${FREERTOS_NAMESPACE}::Heap::4
    ${FREERTOS_NAMESPACE}::ARM_CM7
    HAL::STM32::H7::M7::RCC
    HAL::STM32::H7::M7::GPIO
    HAL::STM32::H7::M7::CORTEX
    CMSIS::STM32::H743ZI::M7
    STM32::NoSys
    STM32::Nano
    STM32::Nano::FloatPrint
    STM32::Nano::FloatScan
)

if(USE_CMSIS_RTOS)
    target_link_libraries(stm32-freertos PRIVATE
        CMSIS::STM32::H7::M7::RTOS
    )
endif()

if(USE_CMSIS_RTOS_V2)
    target_link_libraries(${TARGET_NAME} PRIVATE
        CMSIS::STM32::H7::M7::RTOS_V2
    )
    target_compile_definitions(${TARGET_NAME} PRIVATE
        USE_CMSIS_RTOS_V2
        CMSIS_RTOS_V2_DEVICE_HEADER="stm32h7xx_hal.h"
    )
endif()

stm32_print_size_of_target(${TARGET_NAME})
stm32_generate_binary_file(${TARGET_NAME})
