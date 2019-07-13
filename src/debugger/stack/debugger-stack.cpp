#include <target/target.hpp>
#include <exception/exception.hpp>
#include <debugger/stack/debugger-stack.hpp>

#include <iostream>

#include <sys/ptrace.h>

void DebuggerStack::run(const char* pathTo)
{
    if (!pathTo)
        throw Exception("Not valid pointer to pathTo: ", -1);

    // TODO
}
