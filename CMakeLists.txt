# cmake_minimum_required(VERSION <specify CMake version here>)
project(bamboo)

set(CMAKE_CXX_STANDARD 17)

add_executable(bamboo1 main1.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h Column.h)
add_executable(bamboo2 main2.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h)