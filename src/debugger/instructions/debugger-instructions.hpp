#pragma once

#include <debugger/i-debugger.hpp>

class DebuggerInstructions : public IDebugger
{
    public:
        DebuggerInstructions() = default;
        ~DebuggerInstructions() = default;

        void run(const char* pathTo) override;
};
