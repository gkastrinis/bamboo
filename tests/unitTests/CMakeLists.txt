include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests dummy.cpp)

target_link_libraries(runUnitTests gtest gtest_main)