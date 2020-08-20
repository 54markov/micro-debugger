cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(micro-debugger main.cpp)

add_subdirectory(debugger)
add_subdirectory(cli)
add_subdirectory(breakpoint)
add_subdirectory(register)
add_subdirectory(utils)
add_subdirectory(exception)
add_subdirectory(logger)
add_subdirectory(dwarf)

add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/ext/libelfin
)

target_link_libraries(micro-debugger
                      cli-lib
                      debugger-lib
                      breakpoint-lib
                      register-lib
                      utils-lib
                      exception-lib
                      logger-lib
                      dwarf-lib
                      ${PROJECT_SOURCE_DIR}/src/ext/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/src/ext/libelfin/elf/libelf++.so)

add_dependencies(micro-debugger libelfin)
