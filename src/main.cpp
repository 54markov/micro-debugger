#include <iostream>
#include <string>

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

#include <debugger/debugger.hpp>
#include <cli/cli.hpp>
#include <exception/exception.hpp>
#include <logger/logger.hpp>


/*
 * runTarget - sets 'PTRACE_TRACEME' allows parent process trace it.
 * Executes the 'target' program.
 */
void runTarget(const char *target) {
    if (::ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        throw Exception("ptrace trace me error", errno);
        return;
    }
    ::execl(target, target, nullptr);
}

/*
 * runDebugger - executes debugger routine
 */
void runDebugger(const char *target, pid_t pid) {
    auto isExit = false;

    auto dbg = Debugger{target, pid};
    auto cli = cli::Reader();

    while(!isExit) {
        auto [cmd, addr, src, line] = cli.read();
        switch (cmd) {
            case cli::Commands::run:
                dbg.attach();
                break;
            case cli::Commands::quit:
                dbg.terminate();
                isExit = true;
                break;
            case cli::Commands::cont:
                if (!dbg.continueExecution()) {
                    isExit = true;
                }
                break;
            case cli::Commands::dump:
                dbg.printRegisters();
                break;
            case cli::Commands::bp:
                dbg.setBreakpoint(addr, src, line); // 0x401145
                break;
            case cli::Commands::backtrace:
                dbg.printBacktrace();
                break;
            case cli::Commands::variables:
                dbg.printVariables();
                break;
            case cli::Commands::help:
                break;
            default:
                logger::error("unknown command");
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        logger::error("program name not specified");
        return -1;
    }

    try {
        auto pid = ::fork();
        if (pid == 0) {
            runTarget(argv[1]);
        }
        else if (pid >= 1)  {
            runDebugger(argv[1], pid);
        }
    } catch (const Exception& e) {
        logger::errorf("%s, %d", e.what(), e.code());
        return -2;
    } catch (...) {
        logger::error("unhandled error");
        return -3;
    }

    return 0;
}
