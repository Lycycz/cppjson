cmake_minimum_required (VERSION 3.0)
project (JSON_test)

include_directories(include)
#set(SOURCES src/JSON.cpp src/test.cpp)
file(GLOB SOURCES "src/*.cpp")
add_executable(Cpp_Json ${SOURCES})
