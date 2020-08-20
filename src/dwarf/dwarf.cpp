#include <dwarf/dwarf.hpp>
#include <register/register.hpp>

#include <sys/ptrace.h>


ptrace_expr_context::ptrace_expr_context(pid_t pid) : pid_{pid} {}

dwarf::taddr ptrace_expr_context::reg(unsigned regnum) {
    return reg::getValueFromDwarf(pid_, regnum);
}

dwarf::taddr ptrace_expr_context::pc() {
    struct user_regs_struct regs;
    ::ptrace(PTRACE_GETREGS, pid_, nullptr, &regs);
    return regs.rip;
}

dwarf::taddr ptrace_expr_context::deref_size(dwarf::taddr address, unsigned size) {
    //TODO take into account size
    return ::ptrace(PTRACE_PEEKDATA, pid_, address, nullptr);
}
