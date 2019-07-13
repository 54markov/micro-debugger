#include <debugger/i-debugger.hpp>
#include <debugger/debugger-factory.hpp>

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
        auto debugger = DebuggerFactory::createDebugger(DebuggerType::instructions);
        debugger->run(argv[1]);
    }
    catch (...)
    {
        std::cerr << "Unhandled exception!" << std::endl;
        return -1;
    }

    return 0;
}
