cmake_minimum_required(VERSION 3.13)

set(REPO_ROOT "../..")
set(CMAKE_SCRIPTS_PATH ${REPO_ROOT}/cmake)

include(${CMAKE_SCRIPTS_PATH}/CommonSTM.cmake)
common_config()

project(stm32-minimal C CXX ASM)
set(TARGET_NAME ${CMAKE_PROJECT_NAME})

set(CMSIS_COMP_LIST "")

if(BUILD_H743ZI)
    list(APPEND CMSIS_COMP_LIST STM32H743ZI STM32H7_M7)
endif()

if(BUILD_F303VC)
    list(APPEND CMSIS_COMP_LIST STM32F3)
endif()


find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)

set(FMT_TEST OFF)
set(FMT_OS OFF)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 8.1.1
)
FetchContent_MakeAvailable(fmt)

# Necessary to add important architecture flags to fmt build process
if(BUILD_H743ZI)
    target_link_libraries(fmt PRIVATE
        CMSIS::STM32::H743ZI::M7
    )
endif()

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${TARGET_NAME} PRIVATE
    STM32::NoSys
    fmt::fmt
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

generate_generic_executable_file()

stm32_print_size_of_target(${TARGET_NAME})

include(${CMAKE_SCRIPTS_PATH}/BuildType.cmake)
set_build_type()
