cmake_minimum_required(VERSION 3.0)

project(micro-debbuger VERSION 1.0.0 LANGUAGES C CXX)

enable_testing()

include_directories(src)

add_subdirectory(src src)
add_subdirectory(tests tests)
add_subdirectory(examples examples)
