#include <target/target.hpp>
#include <exception/exception.hpp>
#include <debugger/registers/debugger-registers.hpp>

#include <iostream>

#include <sys/ptrace.h>

void DebuggerRegisters::run(const char* pathTo)
{
    if (!pathTo)
        throw Exception("Not valid pointer to pathTo: ", -1);

    // TODO
}
