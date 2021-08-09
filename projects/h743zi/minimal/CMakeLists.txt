cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-minimal)
set(REPO_ROOT "../../..")
set(CMAKE_TOOLCHAIN_FILE ${REPO_ROOT}/stm32-cmake/cmake/stm32_gcc.cmake)

set(STM32_CUBE_H7_PATH ${REPO_ROOT}/STM32CubeH7)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
set(TARGET_TRIPLET "arm-none-eabi")
set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel)

project(stm32-minimal C ASM)

find_package(CMSIS COMPONENTS STM32H743ZI STM32H7_M7 REQUIRED)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.c
)

target_link_libraries(${TARGET_NAME}
    CMSIS::STM32::H743ZI::M7
    STM32::NoSys
)
