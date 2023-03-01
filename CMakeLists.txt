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

# ffmpeg
set(FFMPEG_DEV_ROOT ${CMAKE_CURRENT_LIST_DIR}/thirdparty/ffmpeg)
include_directories(${FFMPEG_DEV_ROOT}/include)
link_directories(${FFMPEG_DEV_ROOT}/bin)
link_libraries(
    avcodec
    avformat
    avfilter
    swresample
    swscale
    avutil
)
file(GLOB_RECURSE FFMPEG_HEADS ${FFMPEG_DEV_ROOT}/include/*.h)
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/src/*.h ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
set(ProjectSrc ${SRC} ${srcAlut} ${FFMPEG_HEADS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${ProjectSrc})

add_executable(${APP} ${ProjectSrc})

# copy ffmpeg dlls
file(GLOB ffmpeg_shared_libries ${FFMPEG_DEV_ROOT}/bin/*dll)
add_custom_command(TARGET ${APP} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${ffmpeg_shared_libries} $(outdir)
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res $(outdir)
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/thirdparty/lib/alut.dll $(outdir)
)