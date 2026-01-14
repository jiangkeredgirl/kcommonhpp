cmake_minimum_required(VERSION 3.29)

# # 查找所有的 .h 和 .cpp 文件
# file(GLOB_RECURSE KCOMMON_HPP_SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
# )
# add_executable(kcommonhpp ${KCOMMON_HPP_SOURCES})
file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.cpp *.h *.hpp)
target_sources(${PROJECT_NAME} PUBLIC ${sources})

# 添加包含目录
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
