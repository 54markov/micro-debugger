#include <iostream>

#include <sys/ptrace.h>

#include <breakpoint/breakpoint.hpp>
#include <exception/exception.hpp>


Breakpoint::Breakpoint(pid_t pid, std::intptr_t addr): pid_{pid}, addr_{addr}, enabled_{false}, savedData_{} {}

void Breakpoint::enable() {
    const auto data = ::ptrace(PTRACE_PEEKDATA, pid_, addr_, nullptr);
    // Saves bottom byte
    savedData_ = static_cast<uint8_t>(data & 0xff);
    const uint64_t int3 = 0xcc;
    // Sets bottom byte to 0xcc
    const uint64_t dataWithInt3 = ((data & ~0xff) | int3);
    if (::ptrace(PTRACE_POKEDATA, pid_, addr_, dataWithInt3) != 0) {
        throw Exception("ptrace write data", errno);
    }
    enabled_ = true;
}

void Breakpoint::disable() {
    const auto data = ::ptrace(PTRACE_PEEKDATA, pid_, addr_, nullptr);
    const auto restored = ((data & ~0xff) | savedData_);
    if (::ptrace(PTRACE_POKEDATA, pid_, addr_, restored) != 0) {
        throw Exception("ptrace write data", errno);
    }
    enabled_ = false;
}

bool Breakpoint::enabled() const {
    return enabled_;
}

std::intptr_t Breakpoint::getAddress() const {
    return addr_;
}
