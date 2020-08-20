#pragma once

#include <ext/libelfin/dwarf/dwarf++.hh>

class ptrace_expr_context : public dwarf::expr_context {
    public:
        ptrace_expr_context(pid_t pid);

        dwarf::taddr reg(unsigned regnum) override;
        dwarf::taddr pc() override;
        dwarf::taddr deref_size(dwarf::taddr address, unsigned size) override;

    private:
        pid_t pid_;
};