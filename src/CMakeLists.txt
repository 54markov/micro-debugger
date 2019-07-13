cmake_minimum_required(VERSION 3.0)

project(micro-debbuger VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(micro-debuger main.cpp)

add_subdirectory(debugger)
add_subdirectory(exception)
add_subdirectory(target)

target_link_libraries(micro-debuger
                      debugger-factory-lib
                      debugger-instructions-lib
                      debugger-breakpoints-lib
                      debugger-registers-lib
                      debugger-stack-lib
                      target-lib
                      exception-lib)
