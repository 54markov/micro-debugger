#include <target/target.hpp>
#include <exception/exception.hpp>
#include <debugger/breakpoints/debugger-breakpoints.hpp>

#include <iostream>

#include <sys/ptrace.h>

void DebuggerBreakpoints::run(const char* pathTo)
{
    if (!pathTo)
        throw Exception("Not valid pointer to pathTo: ", -1);

    // TODO
}
