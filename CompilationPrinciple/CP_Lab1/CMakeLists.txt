cmake_minimum_required(VERSION 3.29)
project(CP_Lab1)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(CP_Lab1
        main.cpp
        Scan_Analyzer.cpp
        Scan_function.h
        Token.h
)
