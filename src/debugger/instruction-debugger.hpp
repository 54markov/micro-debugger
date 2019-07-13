#pragma once

#include <debugger/i-debugger.hpp>

class InstructionDebugger : public IDebugger
{
    public:
        InstructionDebugger() = default;
        ~InstructionDebugger() = default;

        void run(const char* pathTo) override;
};
