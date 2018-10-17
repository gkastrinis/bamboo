# cmake_minimum_required(VERSION <specify CMake version here>)
project(bamboo)

set(CMAKE_CXX_STANDARD 17)

add_executable(bamboo main1.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h)