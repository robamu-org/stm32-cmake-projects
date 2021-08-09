cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-freertos)

set(REPO_ROOT "../..")
set(CMAKE_SCRIPTS_PATH ${REPO_ROOT}/cmake)

include(${CMAKE_SCRIPTS_PATH}/CommonSTM.cmake)
common_config()

# Please note: When using CMSIS, it is recommended to use the FreeRTOS version supplied in the 
# Cube repository because more recent kernels might be incompatible
option(USE_CMSIS_RTOS "Use CMSIS RTOS provided by Cube repository" ON)
option(USE_CUBE_RTOS "Use FreeRTOS provided by Cube repository" ON)
option(USE_CMSIS_RTOS_V2 "Use CMSIS RTOSV2 provided by Cube repository" OFF)
if(USE_CMSIS_RTOS_V2)
    set(USE_CMSIS_RTOS OFF CACHE BOOL "CMSIS RTOS disabled because V2 is used" FORCE)
endif()
if(USE_CMSIS_RTOS AND NOT USE_CUBE_RTOS)
    message(WARNING "Using CMSIS RTOS but not using Cube RTOS. Build might fail")
endif()

if(NOT USE_CUBE_RTOS)
    set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel)
endif()

project(${TARGET_NAME} CXX C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# stm32_print_devices_by_family(FAMILY H7)

set(CMSIS_COMP_LIST "")
set(HAL_COMP_LIST RCC GPIO CORTEX)
set(CMSIS_COMP_LIST )
set(FREERTOS_COMP_LIST "")
set(FREERTOS_NAMESPACE FreeRTOS)

if(USE_CMSIS_RTOS)
    list(APPEND CMSIS_COMP_LIST RTOS)
endif()

if(BUILD_H743ZI)
    list(APPEND FREERTOS_COMP_LIST STM32H7 ARM_CM7)
    list(APPEND CMSIS_COMP_LIST STM32H743ZI_M7)
    list(APPEND HAL_COMP_LIST STM32H7M7)
    if(USE_CMSIS_RTOS)
        set(FREERTOS_NAMESPACE ${FREERTOS_NAMESPACE}::STM32::H7::M7)
    endif()
endif()

if(BUILD_F303VC)
    list(APPEND FREERTOS_COMP_LIST STM32F3 ARM_CM3)
    list(APPEND CMSIS_COMP_LIST STM32F303VC)
    list(APPEND HAL_COMP_LIST STM32F3)
    if(USE_CMSIS_RTOS)
        set(FREERTOS_NAMESPACE ${FREERTOS_NAMESPACE}::STM32::F3)
    endif()
endif()

find_package(CMSIS COMPONENTS ${CMSIS_COMP_LIST} REQUIRED)
find_package(HAL COMPONENTS ${HAL_COMP_LIST} REQUIRED)
find_package(FreeRTOS COMPONENTS ${FREERTOS_COMP_LIST} REQUIRED)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
    FreeRTOSConfig.h
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME} PRIVATE
    STM32::NoSys
    STM32::Nano
    STM32::Nano::FloatPrint
    STM32::Nano::FloatScan
)

if(USE_CMSIS_RTOS_V2)
    target_compile_definitions(${TARGET_NAME} PRIVATE
        USE_CMSIS_RTOS_V2
        CMSIS_RTOS_V2_DEVICE_HEADER="stm32h7xx_hal.h"
    )
endif()

if(BUILD_H743ZI)
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${FREERTOS_NAMESPACE}::Timers
        ${FREERTOS_NAMESPACE}::Heap::4
        ${FREERTOS_NAMESPACE}::ARM_CM7
        HAL::STM32::H7::M7::RCC
        HAL::STM32::H7::M7::GPIO
        HAL::STM32::H7::M7::CORTEX
        CMSIS::STM32::H743ZI::M7
    )
    if(USE_CMSIS_RTOS)
        target_link_libraries(${TARGET_NAME} PRIVATE
            CMSIS::STM32::H7::M7::RTOS
        )
    endif()
    if(USE_CMSIS_RTOS_V2)
        target_link_libraries(${TARGET_NAME} PRIVATE
            CMSIS::STM32::H7::M7::RTOS_V2
        )
    endif()
endif()

if(BUILD_F303VC)
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${FREERTOS_NAMESPACE}::Timers
        ${FREERTOS_NAMESPACE}::Heap::4
        ${FREERTOS_NAMESPACE}::ARM_CM3
        HAL::STM32::F3::RCC
        HAL::STM32::F3::GPIO
        HAL::STM32::F3::CORTEX
        CMSIS::STM32::F303VC
    )
    if(USE_CMSIS_RTOS)
        target_link_libraries(${TARGET_NAME} PRIVATE
            CMSIS::STM32::F3::RTOS
        )
    endif()
    if(USE_CMSIS_RTOS_V2)
        target_link_libraries(${TARGET_NAME} PRIVATE
            CMSIS::STM32::F3::RTOS_V2
        )
    endif()
endif()

stm32_print_size_of_target(${TARGET_NAME})
stm32_generate_binary_file(${TARGET_NAME})

include(${CMAKE_SCRIPTS_PATH}/BuildType.cmake)
set_build_type()
