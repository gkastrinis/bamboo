cmake_minimum_required(VERSION 3.16)

project(bamboo
		VERSION 0.1
		DESCRIPTION "paNda Engine"
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(VPT apps/vpt.cpp)

target_include_directories(VPT PUBLIC include)
#		"include/index"
#		"include/relation"
#		"include/string"
#		)

option(PACKAGE_TESTS "Build the tests" ON)

if (PACKAGE_TESTS)
	enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif ()