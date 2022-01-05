cmake_minimum_required(VERSION 3.16)

project(bamboo
		VERSION 0.1
		DESCRIPTION "paNda Engine"
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# The compiled library code is here
#add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

option(PACKAGE_TESTS "Build the tests" ON)

if (PACKAGE_TESTS)
	enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif ()