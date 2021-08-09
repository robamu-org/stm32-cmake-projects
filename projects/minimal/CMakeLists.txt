cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-minimal)
set(REPO_ROOT "../..")
set(CMAKE_TOOLCHAIN_FILE ${REPO_ROOT}/stm32-cmake/cmake/stm32_gcc.cmake)

option(BUILD_H743ZI "Build for H743ZI board" OFF)
option(BUILD_F303VC "Build for F303VC board" OFF)

if(NOT BUILD_H743ZI AND NOT BUILD_F303VC)
    message(
        FATAL_ERROR "Please pick at least once board and supply it with -DBUILD_<BOARD>"
    )
endif()

set(STM32_CUBE_H7_PATH ${REPO_ROOT}/STM32CubeH7)
set(STM32_CUBE_F3_PATH ${REPO_ROOT}/STM32CubeF3)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
set(TARGET_TRIPLET "arm-none-eabi")
set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel)

project(stm32-minimal C ASM)

set(CMSIS_COMP_LIST "")

if(BUILD_H743ZI)
    list(APPEND CMSIS_COMP_LIST STM32H743ZI STM32H7_M7)
endif()

if(BUILD_F303VC)
    list(APPEND CMSIS_COMP_LIST STM32F3)
endif()

find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.c
)

target_link_libraries(${TARGET_NAME} PRIVATE
    STM32::NoSys
)
if(BUILD_H743ZI)
    target_link_libraries(${TARGET_NAME} PRIVATE
        CMSIS::STM32::H743ZI::M7
    )
endif()

if(BUILD_F303VC)
    target_link_libraries(${TARGET_NAME} PRIVATE
        CMSIS::STM32::F303VC
    )
endif()
