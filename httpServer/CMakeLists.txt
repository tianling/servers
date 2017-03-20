cmake_minimum_required(VERSION 3.6)
project(httpServer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp server.cpp)
add_executable(httpServer ${SOURCE_FILES})