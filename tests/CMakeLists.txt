cmake_minimum_required(VERSION 3.0)

project(micro-debbuger-tests VERSION 1.0.0 LANGUAGES C CXX)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

set(tests-debugger-instructions tests-debugger-instructions.cpp)

add_executable(DebuggerInstructionTests ${tests-debugger-instructions})

target_link_libraries(DebuggerInstructionTests 
                      ${Boost_LIBRARIES}
                      debugger-factory-lib
                      debugger-instructions-lib
                      debugger-breakpoints-lib
                      debugger-registers-lib
                      debugger-stack-lib
                      target-lib
                      exception-lib)

add_test(NAME DebuggerInstructionTests COMMAND DebuggerInstructionTests)
