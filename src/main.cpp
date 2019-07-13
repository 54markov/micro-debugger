#include <debugger/i-debugger.hpp>
#include <debugger/instruction-debugger.hpp>

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Program name not specified" << std::endl;
        return -1;
    }

    try
    {
        std::shared_ptr<IDebugger> debugger = std::make_shared<InstructionDebugger>();
        debugger->run(argv[1]);
    }
    catch (...)
    {
        std::cerr << "Unhandled exception!" << std::endl;
        return -1;
    }

    return 0;
}
