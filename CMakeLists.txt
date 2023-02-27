cmake_minimum_required(VERSION 3.0.0)
set(APP "pck-player")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/dist)
project(${APP} VERSION 0.1.0)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/AL)

link_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/lib)
link_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty)
link_libraries(OpenAL32)

# alut



# add_definitions(-DHAVE_CONFIG_H=1)
add_definitions(-DHAVE___INT8=1)
if(HAVE_GLIBC)
	add_definitions(-D__NO_CTYPE=1)
endif()


if(HAVE_GCC_VISIBILITY)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
	add_definitions(-DHAVE_GCC_VISIBILITY)
endif()  
include_directories(${CMAKE_CURRENT_LIST_DIR}/openalut)
file(GLOB_RECURSE srcAlut
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/AL/*.h
    # ${CMAKE_CURRENT_LIST_DIR}/thirdparty/alut/*.h
    # ${CMAKE_CURRENT_LIST_DIR}/thirdparty/alut/*.c
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${srcAlut})

add_executable(${APP} ./main.cpp ${srcAlut})
add_custom_command(TARGET ${APP} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res $(outdir)
)