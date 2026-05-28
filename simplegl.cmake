set(SIMPLEGL_SDK_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/sdk/include")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/core)
if(NOT DISABLE_SIMPLEGL_MAIN_PROGRAM)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/program)
endif()
