set(SIMPLEGL_EXEC_NAME simplegl)
set(SIMPLEGL_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(SIMPLEGL_INCLUDE_PATH "${SIMPLEGL_PATH}/inc")

list(APPEND SIMPLEGL_SOURCE
    ${SIMPLEGL_PATH}/main.c
)

add_executable(${SIMPLEGL_EXEC_NAME} ${SIMPLEGL_SOURCE})

target_include_directories(${SIMPLEGL_EXEC_NAME} PRIVATE ${SIMPLEGL_INCLUDE_PATH})

target_include_directories(${SIMPLEGL_EXEC_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../core/inc)
target_include_directories(${SIMPLEGL_EXEC_NAME} PUBLIC ${SIMPLEGL_SDK_INCLUDE_PATH})

target_link_libraries(${SIMPLEGL_EXEC_NAME} simplegl_core)
