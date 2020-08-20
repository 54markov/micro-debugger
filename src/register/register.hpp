#pragma once

#include <algorithm>

#include <sys/user.h>


namespace reg {

enum class Reg86_64 {
    rax, rbx, rcx, rdx,
    rdi, rsi, rbp, rsp,
    r8,  r9,  r10, r11,
    r12, r13, r14, r15,
    rip, rflags, cs,
    orig_rax, fs_base,
    gs_base,
    fs, gs, ss, ds, es
};

static constexpr std::size_t size = 27;

struct descriptor {
    Reg86_64 r;
    int dwarf;
    std::string name;
};

// Have a look in /usr/include/sys/user.h for how to lay this out
static const std::array<descriptor, size> descriptors
{
    {
        { Reg86_64::r15, 15, "r15" },
        { Reg86_64::r14, 14, "r14" },
        { Reg86_64::r13, 13, "r13" },
        { Reg86_64::r12, 12, "r12" },
        { Reg86_64::rbp, 6, "rbp" },
        { Reg86_64::rbx, 3, "rbx" },
        { Reg86_64::r11, 11, "r11" },
        { Reg86_64::r10, 10, "r10" },
        { Reg86_64::r9, 9, "r9" },
        { Reg86_64::r8, 8, "r8" },
        { Reg86_64::rax, 0, "rax" },
        { Reg86_64::rcx, 2, "rcx" },
        { Reg86_64::rdx, 1, "rdx" },
        { Reg86_64::rsi, 4, "rsi" },
        { Reg86_64::rdi, 5, "rdi" },
        { Reg86_64::orig_rax, -1, "orig_rax" },
        { Reg86_64::rip, -1, "rip" },
        { Reg86_64::cs, 51, "cs" },
        { Reg86_64::rflags, 49, "eflags" },
        { Reg86_64::rsp, 7, "rsp" },
        { Reg86_64::ss, 52, "ss" },
        { Reg86_64::fs_base, 58, "fs_base" },
        { Reg86_64::gs_base, 59, "gs_base" },
        { Reg86_64::ds, 53, "ds" },
        { Reg86_64::es, 50, "es" },
        { Reg86_64::fs, 54, "fs" },
        { Reg86_64::gs, 55, "gs" },
    }
};

uint64_t getValue(const pid_t pid, const Reg86_64 r);
uint64_t getValueFromDwarf(const pid_t pid, const unsigned regnum);
void setValue(const pid_t pid, const Reg86_64 r, const uint64_t value);
std::string getName(const Reg86_64 r);
Reg86_64 getName(const std::string &name);
void print(const pid_t pid);

} // namespace reg
