cmake_minimum_required(VERSION 3.6)
project(CMSC441)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES proj2_cpp.cpp)
add_executable(CMSC441 ${SOURCE_FILES})