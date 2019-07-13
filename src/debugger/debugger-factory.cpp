#include <exception/exception.hpp>
#include <debugger/debugger-factory.hpp>
#include <debugger/instructions/debugger-instructions.hpp>
#include <debugger/breakpoints/debugger-breakpoints.hpp>
#include <debugger/registers/debugger-registers.hpp>
#include <debugger/stack/debugger-stack.hpp>

auto DebuggerFactory::createDebugger(
    const DebuggerType& debuggerType) -> std::unique_ptr<IDebugger>
{
    switch (debuggerType)
    {
        case DebuggerType::instructions:
            return std::make_unique<DebuggerInstructions>();

        case DebuggerType::breakpoints:
            return std::make_unique<DebuggerBreakpoints>();

        case DebuggerType::stack:
            return std::make_unique<DebuggerStack>();

        case DebuggerType::registers:
            return std::make_unique<DebuggerRegisters>();
    }

    throw Exception("Can't to create debugger!", -1);
}
