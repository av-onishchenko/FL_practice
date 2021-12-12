cmake_minimum_required(VERSION 3.17)
project(03FL_task)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

INCLUDE(CodeCoverage)
SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

add_executable(executeTests tests.cpp)
target_link_libraries(executeTests ${GTEST_LIBRARIES} pthread)

SETUP_TARGET_FOR_COVERAGE(my_coverage_target executeTests coverage)