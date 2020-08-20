#include <iomanip>
#include <iostream>

#include <sys/ptrace.h>

#include <register/register.hpp>
#include <exception/exception.hpp>
#include <logger/logger.hpp>


uint64_t reg::getValue(const pid_t pid, const reg::Reg86_64 r) {
    user_regs_struct regs;
    if (::ptrace(PTRACE_GETREGS, pid, nullptr, &regs) != 0) {
        throw Exception("ptrace get register value error", errno);
    }

    auto it = std::find_if(begin(reg::descriptors), end(reg::descriptors), [r](auto&& rd) {
        return rd.r == r;
    });

    return *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(reg::descriptors)));
}

uint64_t reg::getValueFromDwarf(const pid_t pid, const unsigned regnum) {
    auto it = std::find_if(begin(reg::descriptors), end(reg::descriptors), [regnum](auto&& rd) {
        return rd.dwarf == (int)regnum;
    });

    if (it == end(reg::descriptors)) {
        throw Exception("unknown dwarf register");
    }

    return reg::getValue(pid, it->r);
}

void reg::setValue(const pid_t pid, const reg::Reg86_64 r, const uint64_t value) {
    user_regs_struct regs;
    if (::ptrace(PTRACE_GETREGS, pid, nullptr, &regs) != 0) {
        throw Exception("ptrace get register value error", errno);
    }

    auto it = std::find_if(begin(reg::descriptors), end(reg::descriptors), [r](auto&& rd) {
        return rd.r == r;
    });

    *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(reg::descriptors))) = value;
    if (::ptrace(PTRACE_SETREGS, pid, nullptr, &regs) != 0) {
        throw Exception("ptrace set register value error", errno);
    }
}

std::string reg::getName(const reg::Reg86_64 r) {
    auto it = std::find_if(begin(reg::descriptors), end(reg::descriptors), [r](auto&& rd) {
        return rd.r == r;
    });

    return it->name;
}

reg::Reg86_64 reg::getName(const std::string &name) {
    auto it = std::find_if(begin(reg::descriptors), end(reg::descriptors), [name](auto&& rd) {
        return rd.name == name;
    });

    return it->r;
}

void reg::print(pid_t pid) {
    for (const auto& rd : reg::descriptors) {
        logger::printf("%s\t0x%x", rd.name.data(), reg::getValue(pid, rd.r));
    }
}
