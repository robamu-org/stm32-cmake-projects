cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-l3gd20h)
set(REPO_ROOT "../../")
set(CMAKE_SCRIPT_PATH ${REPO_ROOT}/cmake)
set(CMAKE_TOOLCHAIN_FILE ${REPO_ROOT}/stm32-cmake/cmake/stm32_gcc.cmake)

set(STM32_CUBE_H7_PATH ${REPO_ROOT}/STM32CubeH7)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
set(TARGET_TRIPLET "arm-none-eabi")
set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel)

configure_file(Inc/conf.h.in conf.h)

project(stm32-l3gd20h C ASM)

find_package(CMSIS COMPONENTS STM32H743ZI STM32H7_M7 REQUIRED)
find_package(HAL COMPONENTS STM32H7M7 STM32H743ZI RCC GPIO CORTEX SPI DMA PWR EXTI REQUIRED)

add_executable(${TARGET_NAME})

add_subdirectory(Inc)

target_sources(${TARGET_NAME} PRIVATE
    main.c
    stm32h7xx_hal_msp.c
    stm32h7xx_it.c
    stm32h7xx_nucleo.c
)

target_link_libraries(${TARGET_NAME}
    CMSIS::STM32::H743xx::M7
    HAL::STM32::H7::M7::RCC
    HAL::STM32::H7::M7::GPIO
    HAL::STM32::H7::M7::SPI
    HAL::STM32::H7::M7::CORTEX
    HAL::STM32::H7::M7::DMA
    HAL::STM32::H7::M7::PWR
    HAL::STM32::H7::M7::PWREx
    HAL::STM32::H7::M7::EXTI
    STM32::NoSys
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

include (${CMAKE_SCRIPT_PATH}/BuildType.cmake)
set_build_type()
stm32_add_linker_script(${TARGET_NAME} "PRIVATE" STM32H743ZITx_FLASH.ld)
