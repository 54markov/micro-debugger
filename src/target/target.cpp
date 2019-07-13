#include <target/target.hpp>
#include <exception/exception.hpp>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>

#include <unistd.h>

Target::Target(std::string&& name) : name_(std::move(name))
{
    ;
}

void Target::run(void)
{
    this->pid_ = ::fork();
    if (this->pid_ == 0)
    {
        // Allows parent to trace child process
        if (::ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0)
            throw Exception("Can't ::ptrace(): " +
                            std::string(::strerror(errno)), errno);

        // Replaces child process's image with the given program
        if (::execl(this->name_.c_str(), this->name_.c_str(), nullptr) == -1)
            throw Exception("Can't ::execl(): " +
                            std::string(::strerror(errno)), errno);
    }
    else if (this->pid_ == -1)
    {
        throw Exception("Can't ::fork(): " +
                        std::string(::strerror(errno)), errno);
    }
}

int Target::wait(void)
{
    auto wait_status = 0;
    auto options = 0;
    
    if (::waitpid(this->pid_, &wait_status, options) == -1)
        throw Exception("Can't ::waitpid(): " +
                        std::string(::strerror(errno)), errno);

    return wait_status;
}

void Target::terminate(void)
{
    return;
}

pid_t Target::getPid(void) noexcept
{
    return this->pid_;
}
