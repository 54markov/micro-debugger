#pragma once

// This is an debugger interface class
class IDebugger
{
    public:
        virtual void run(const char* pathTo) = 0;

    
        virtual ~IDebugger() = default; // TODO
    protected:
        IDebugger& operator=(const IDebugger&) = delete;
};
