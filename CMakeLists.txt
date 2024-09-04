cmake_minimum_required(VERSION 3.5)

# 添加包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 查找所有的 .h 和 .cpp 文件
file(GLOB_RECURSE KCOMMON_HPP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(kcommonhpp ${KCOMMON_HPP_SOURCES})
