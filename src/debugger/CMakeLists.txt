cmake_minimum_required(VERSION 3.0)

add_subdirectory(instructions)
add_subdirectory(breakpoints)
add_subdirectory(registers)
add_subdirectory(stack)

add_library(debugger-factory-lib SHARED debugger-factory.cpp)
