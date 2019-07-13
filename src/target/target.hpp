#pragma once

#include <string>

#include <sys/types.h>

class Target
{
    public:
        explicit Target(std::string&& name);
        ~Target() = default;

        void run(void);
        int wait(void);
        void terminate(void);

        pid_t getPid(void) noexcept;

    private:
        pid_t pid_;
        std::string name_;
};
