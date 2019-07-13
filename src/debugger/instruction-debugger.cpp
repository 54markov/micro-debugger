#include <debugger/instruction-debugger.hpp>
#include <exception/exception.hpp>
#include <target/target.hpp>

#include <iostream>

#include <sys/ptrace.h>

void InstructionDebugger::run(const char* pathTo)
{
    if (!pathTo)
        throw Exception("Not valid pointer to pathTo: " + std::string(), -1);

    auto iCounter = 0;

    auto target = Target(std::string(pathTo));
    target.run();

    // Waits for target to stop on its first instruction
    auto waitStatus = target.wait();
    auto targetPid = target.getPid();

    while (WIFSTOPPED(waitStatus))
    {
        iCounter++;
        
        // Makes the target execute another instruction
        if (::ptrace(PTRACE_SINGLESTEP, targetPid, nullptr, nullptr) < 0)
            throw Exception("Can't ::ptrace(): " +
                            std::string(::strerror(errno)), errno);

        // Waits for target to stop on its next instruction
        waitStatus = target.wait();
    }

    std::cout << "The child executed " << iCounter << " instructions" << std::endl;
}
