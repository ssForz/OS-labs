cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(kpout
        ./src/main.cpp
        ./src/FBalloc.cpp
        ./src/FBalloc.h
        ./src/MKKalloc.cpp
        ./src/MKKalloc.h)
