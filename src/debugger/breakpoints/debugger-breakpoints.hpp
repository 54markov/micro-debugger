#pragma once

#include <debugger/i-debugger.hpp>

class DebuggerBreakpoints : public IDebugger
{
    public:
        DebuggerBreakpoints() = default;
        ~DebuggerBreakpoints() = default;

        void run(const char* pathTo) override;
};
