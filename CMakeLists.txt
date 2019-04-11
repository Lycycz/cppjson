cmake_minimum_required(VERSION 3.0.0)
project(CppJson VERSION 0.1.0)

set(SRC_LIST JSON.cpp test.cpp)
add_executable(CppJson ${SRC_LIST})
