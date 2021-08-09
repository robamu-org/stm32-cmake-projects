cmake_minimum_required(VERSION 3.13)

set(REPO_ROOT "../..")
set(CMAKE_SCRIPTS_PATH ${REPO_ROOT}/cmake)

include(${CMAKE_SCRIPTS_PATH}/CommonSTM.cmake)
common_config()

project(stm32-minimal C ASM)
set(TARGET_NAME ${CMAKE_PROJECT_NAME})

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

include(${CMAKE_SCRIPTS_PATH}/BuildType.cmake)
set_build_type()
