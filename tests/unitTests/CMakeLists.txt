include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests basicTests.cpp advancedTests.cpp)

target_link_libraries(runUnitTests gtest gtest_main)