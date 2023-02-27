cmake_minimum_required(VERSION 3.0.0)
set(APP "pck-player")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/dist)
project(${APP} VERSION 0.1.0)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/AL)

link_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/lib)
link_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty)
link_libraries(OpenAL32 alut)

include_directories(${CMAKE_CURRENT_LIST_DIR}/openalut)
file(GLOB_RECURSE srcAlut
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/AL/*.h
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${srcAlut})

add_executable(${APP} ./main.cpp ${srcAlut})
add_custom_command(TARGET ${APP} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res $(outdir)
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/thirdparty/lib/alut.dll $(outdir)
)