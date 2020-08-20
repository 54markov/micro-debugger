#pragma once

#include <string>
#include <linux/types.h>

class Breakpoint {
    public:
        Breakpoint(pid_t pid, std::intptr_t addr);
        void enable();
        void disable();
        bool enabled() const;
        std::intptr_t getAddress() const;

    private:
        pid_t pid_;
        std::intptr_t addr_;
        bool enabled_;
        uint8_t savedData_; //Data which used to be at the breakpoint address
};
