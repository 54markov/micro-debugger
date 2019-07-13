#include <target/target.hpp>
#include <exception/exception.hpp>
#include <debugger/instructions/debugger-instructions.hpp>

#include <iostream>

#include <sys/ptrace.h>
#include <sys/user.h>

void DebuggerInstructions::run(const char* pathTo)
{
    if (!pathTo)
        throw Exception("Not valid pointer to pathTo: ", -1);

    auto iCounter = 0;

    auto target = Target(std::string(pathTo));
    target.run();

    // Waits for target to stop on its first instruction
    auto waitStatus = target.wait();
    auto targetPid = target.getPid();

    while (WIFSTOPPED(waitStatus))
    {
        iCounter++;

        struct user_regs_struct regs;
        if (::ptrace(PTRACE_GETREGS, targetPid, nullptr, &regs) < 0)
            throw Exception("Can't ::ptrace(): " +
                            std::string(::strerror(errno)), errno);

        unsigned instr = ::ptrace(PTRACE_PEEKTEXT, targetPid, regs.rip, nullptr);
        std::cout << "Instruction = " << iCounter << " EIP = 0x" << regs.rip
                  << " ADDR = 0x" << instr << std::endl;

        // Makes the target execute another instruction
        if (::ptrace(PTRACE_SINGLESTEP, targetPid, nullptr, nullptr) < 0)
            throw Exception("Can't ::ptrace(): " +
                            std::string(::strerror(errno)), errno);

        // Waits for target to stop on its next instruction
        waitStatus = target.wait();
    }

    std::cout << "The target (" << pathTo << ") executed "
              << iCounter << " instructions" << std::endl;
}
