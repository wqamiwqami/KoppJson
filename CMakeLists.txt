# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
#设置编译器
set (CMAKE_C_COMPILER "C:/sofeware/mingw/mingw64/bin/gcc.exe")
set (CMAKE_CXX_COMPILER "C:/sofeware/mingw/mingw64/bin/g++.exe")

# 之前编译报错需要这个,不知道为什么突然就不用了
# set (CMAKE_C_COMPILER_FORCED 1)
# set (CMAKE_CXX_COMPILER_FORCED 1)
# set (CMAKE_C_COMPILER_WORK 1)
# set (CMAKE_CXX_COMPILER_WORK 1)
# 项目信息

project(KoppJson)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}/include")


add_executable(Kopptest)
aux_source_directory(. sources)
aux_source_directory(src sources)
target_sources(Kopptest PUBLIC ${sources})
