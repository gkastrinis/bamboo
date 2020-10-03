cmake_minimum_required(VERSION 3.16)

project(bamboo)

set(CMAKE_CXX_STANDARD 17)

add_executable(VPT vpt.cpp)

add_subdirectory(tests)

target_include_directories(VPT PUBLIC
		"${PROJECT_SOURCE_DIR}/index"
		"${PROJECT_SOURCE_DIR}/relation"
		"${PROJECT_SOURCE_DIR}/string"
		)
