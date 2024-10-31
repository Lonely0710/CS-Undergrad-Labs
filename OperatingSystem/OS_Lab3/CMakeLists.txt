cmake_minimum_required(VERSION 3.29)
project(OS_Lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(OS_Lab3 main.cpp
        CPUscheduling.cpp
        CPUscheduling.h)
