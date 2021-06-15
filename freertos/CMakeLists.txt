cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME stm32-freertos)
set(REPO_ROOT "..")
set(STM32_CMAKE_PATH  ${REPO_ROOT}/stm32-cmake)

# Please note: When using CMSIS, it is recommended to use the FreeRTOS version supplied in the 
# Cube repository because more recent kernels might be incompatible
option(USE_CMSIS_RTOS ON "Use CMSIS RTOS provided by Cube repository")
option(USE_CUBE_RTOS ON "Use FreeRTOS provided by Cube repository")

set(STM32_CUBE_H7_PATH ${REPO_ROOT}/STM32CubeH7)
set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
set(TARGET_TRIPLET "arm-none-eabi")


if(USE_CUBE_RTOS)
    set(FREERTOS_PATH ${REPO_ROOT}/FreeRTOS-Kernel)
else()
    set(FREERTOS_PATH ${STM32_CUBE_H7_PATH}/Middlewares/Third_Party/FreeRTOS)
endif()

# This must come before the project call!
set(CMAKE_TOOLCHAIN_FILE ${STM32_CMAKE_PATH}/cmake/stm32_gcc.cmake)
project(${TARGET_NAME} CXX C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# include(${STM32_CMAKE_PATH}/cmake/stm32/devices.cmake)
# stm32_print_devices_by_family(FAMILY H7)

find_package(CMSIS COMPONENTS STM32H743ZI STM32H7_M7 RTOS REQUIRED)
find_package(HAL COMPONENTS STM32H7M7 STM32H743ZI RCC GPIO CORTEX REQUIRED)
find_package(FreeRTOS COMPONENTS ARM_CM7 REQUIRED)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
    FreeRTOSConfig.h
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(stm32-freertos PRIVATE
    FreeRTOS::Timers
    FreeRTOS::Heap::4
    FreeRTOS::ARM_CM7 
    HAL::STM32::H7::M7::RCC
    HAL::STM32::H7::M7::GPIO
    HAL::STM32::H7::M7::CORTEX
    CMSIS::STM32::H743ZI::M7
    STM32::NoSys
)

if(USE_CMSIS_RTOS)
    target_link_libraries(stm32-freertos PRIVATE
        CMSIS::STM32::H7::M7::RTOS
    )
endif()

stm32_print_size_of_target(${TARGET_NAME})

add_custom_command(
	TARGET ${TARGET_NAME}
	POST_BUILD
	COMMAND echo Generating binary file ${CMAKE_PROJECT_NAME}.bin..
	COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin
)
