cmake_minimum_required(VERSION 3.12)

project(bamboo)

set(CMAKE_CXX_STANDARD 17)

add_executable(bamboo1 main1.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h relation/Column.h)
add_executable(bamboo2 main2.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h relation/Column.h relation/Relation.h)
add_executable(bamboo4 main4.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h relation/Column.h)
add_executable(bambooVPT main3.cpp index/Index.h index/ArrayIndex.h index/HashIndex.h relation/Column.h relation/Relation.h)

add_subdirectory(tests)