cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(main LANGUAGES CXX)
add_executable(exec_child ./src/exec_child.cpp)
add_executable(main ./src/main.cpp)