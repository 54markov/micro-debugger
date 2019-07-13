#pragma once

#include <debugger/i-debugger.hpp>

class DebuggerRegisters : public IDebugger
{
    public:
        DebuggerRegisters() = default;
        ~DebuggerRegisters() = default;

        void run(const char* pathTo) override;
};
