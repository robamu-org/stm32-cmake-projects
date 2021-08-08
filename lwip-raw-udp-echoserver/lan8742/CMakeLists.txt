target_sources(${TARGET_NAME} PRIVATE
    lan8742.c
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
