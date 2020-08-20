#pragma once

#include <string>
#include <utility>
#include <unordered_map>

#include <linux/types.h>

#include <cli/cli.hpp>
#include <breakpoint/breakpoint.hpp>
#include <ext/libelfin/dwarf/dwarf++.hh>
#include <ext/libelfin/elf/elf++.hh>


class Debugger {
    public:
        Debugger(std::string prog, pid_t pid);
        ~Debugger() = default;

        void attach();
        void terminate();
        bool continueExecution();
        void printRegisters();
        void printBacktrace();
        void printVariables();
        void setBreakpoint(const std::intptr_t addr, const std::string &source, const uint32_t line);
        void setBreakpoint(const std::string &source, const uint32_t line);
        void setBreakpoint(const std::intptr_t addr);
        uint64_t readMemory(const uint64_t address);
        void writeMemory(const uint64_t address, const uint64_t value);

    private:
        std::string prog_;
        pid_t pid_;
        dwarf::dwarf dwarf_;
        elf::elf elf_;
        std::unordered_map<std::intptr_t, Breakpoint> breakpoints_;

        void stepOverBp_();
        uint64_t getPc_();
        void setPc_(uint64_t pc);
        int waitSignal_();
        dwarf::die getFunctionFromPc_(const uint64_t pc);
};
