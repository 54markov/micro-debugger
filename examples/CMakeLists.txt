cmake_minimum_required(VERSION 3.0)

project(micro-debbuger VERSION 1.0.0 LANGUAGES C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -gdwarf-2 -g -O0")

add_executable(hello-world hello-world.c)
add_executable(stack_unwinding stack_unwinding.c)
add_executable(variables variables.c)
