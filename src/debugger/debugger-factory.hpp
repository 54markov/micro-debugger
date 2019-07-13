#pragma once

#include <debugger/i-debugger.hpp>

#include <memory>

enum class DebuggerType { instructions, breakpoints, stack, registers };

class DebuggerFactory
{
    public:
        static auto createDebugger(
            const DebuggerType& debuggerType) -> std::unique_ptr<IDebugger>;
};
