cmake_minimum_required(VERSION 3.12)

project(bamboo)

set(CMAKE_CXX_STANDARD 17)

add_executable(VPT vpt.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h relation/Column.h relation/Relation.h)

add_subdirectory(tests)