cmake_minimum_required(VERSION 3.0.0)
set(APP "pck-player")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/dist)

project(${APP} VERSION 0.1.0)
add_executable(${APP} ./main.cpp)

add_custom_command(TARGET ${APP} PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res $(outdir)
)