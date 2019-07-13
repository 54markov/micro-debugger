#pragma once

// This is an debugger interface class
class IDebugger
{
    public:
        virtual void run(const char* pathTo) = 0;

    protected:
        virtual ~IDebugger() = default;
        IDebugger& operator=(const IDebugger&) = delete;
};
