cmake_minimum_required(VERSION 3.20)
project(Container)

set(CMAKE_CXX_STANDARD 17)

add_executable(Container main.cpp List.h Tests.h Tests.cpp)
