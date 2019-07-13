#pragma once

#include <debugger/i-debugger.hpp>

class DebuggerStack : public IDebugger
{
    public:
        DebuggerStack() = default;
        ~DebuggerStack() = default;

        void run(const char* pathTo) override;
};
