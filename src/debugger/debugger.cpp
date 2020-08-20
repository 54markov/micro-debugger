#include <iostream>

#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include <debugger/debugger.hpp>
#include <register/register.hpp>
#include <exception/exception.hpp>
#include <logger/logger.hpp>
#include <utils/utils.hpp>
#include <dwarf/dwarf.hpp>


Debugger::Debugger(std::string prog, pid_t pid): prog_{std::move(prog)}, pid_{pid} {
    auto fd = ::open(prog_.c_str(), O_RDONLY);
    elf_ = elf::elf{elf::create_mmap_loader(fd)};
    dwarf_ = dwarf::dwarf{dwarf::elf::create_loader(elf_)};
}

void Debugger::attach() {
    logger::printf("attached to pid %d with status %d", pid_, waitSignal_());
}

void Debugger::terminate() {
    logger::printf("killed pid %d with status %d", pid_, ::kill(pid_, SIGKILL));
}

bool Debugger::continueExecution() {
    logger::print("continue execution ...");
    if (::ptrace(PTRACE_CONT, pid_, nullptr, nullptr) != 0) {
        throw Exception("ptrace continue error", errno);
    }
    auto status = waitSignal_();
    if (WIFSTOPPED(status)) {
        logger::printf("stoped %d with status %d", pid_, status);
        stepOverBp_();
        return true;
    } else if (WIFEXITED(status)) {
        logger::printf("exited %d with status %d", pid_, status);
        return false;
    }
    return false;
}

void Debugger::printRegisters() {
    reg::print(pid_);
}

void Debugger::printBacktrace() {
    auto outputFrame = [frame_number = 0] (auto&& func) mutable {
        logger::printf("frame #%d\t0x%x\t%s", frame_number++, dwarf::at_low_pc(func), (dwarf::at_name(func)).data());
    };

    auto func = getFunctionFromPc_(getPc_());
    outputFrame(func);

    auto framePointer = reg::getValue(pid_, reg::Reg86_64::rbp);
    auto address = readMemory(framePointer + 8);

    while (dwarf::at_name(func) != "main") {
        func = getFunctionFromPc_(address);
        outputFrame(func);
        framePointer = readMemory(framePointer);
        address = readMemory(framePointer + 8);
    }
}

void Debugger::printVariables() {
    using namespace dwarf;

    auto func = getFunctionFromPc_(getPc_());
    for (const auto& die : func) {
        if (die.tag == DW_TAG::variable) {
            auto loc_val = die[DW_AT::location];
            //only supports exprlocs for now
            if (loc_val.get_type() == value::type::exprloc) {
                ptrace_expr_context context {pid_};
                auto result = loc_val.as_exprloc().evaluate(&context);
                switch (result.location_type) {
                    case expr_result::type::address:
                    {
                        auto value = readMemory(result.value);
                        logger::printf("%s\t0x%x\t%x", dwarf::at_name(die).data(), result.value, value);
                        break;
                    }
                    case expr_result::type::reg:
                    {
                        auto value = reg::getValueFromDwarf(pid_, result.value);
                        logger::printf("%s\treg %x\t%x", dwarf::at_name(die).data(), result.value, value);
                        break;
                    }
                    default:
                        throw Exception{"Unhandled variable location"};
                }
            } else {
                throw Exception{"Unhandled variable location"};
            }
        }
    }
}

void Debugger::setBreakpoint(const std::intptr_t addr, const std::string &source, const uint32_t line) {
    if (addr != 0) {
        setBreakpoint(addr);
    } else {
        setBreakpoint(source, line);
    }
}

void Debugger::setBreakpoint(const std::string &source, const uint32_t line) {
    logger::printf("set breakpoint in %s %d", source.data(), line);
    for (const auto& cu : dwarf_.compilation_units()) {
        if (utils::isSuffix(source, at_name(cu.root()))) {
            const auto& lt = cu.get_line_table();
            for (const auto& entry : lt) {
                if (entry.is_stmt && entry.line == line) {
                    setBreakpoint(entry.address);
                    return;
                }
            }
        }
    }
}

void Debugger::setBreakpoint(const std::intptr_t addr) {
    logger::printf("set breakpoint at address 0x%x for pid %d", addr, pid_);
    Breakpoint bp{pid_, addr};
    bp.enable();
    breakpoints_.insert({addr, bp});
}


uint64_t Debugger::readMemory(const uint64_t address) {
    return ::ptrace(PTRACE_PEEKDATA, pid_, address, nullptr);
}

void Debugger::writeMemory(const uint64_t address, const uint64_t value) {
    ::ptrace(PTRACE_POKEDATA, pid_, address, value);
}

void Debugger::stepOverBp_() {
    // '-1' Because of execution will go past the breakpoint
    auto possible_breakpoint_location = getPc_() - 1;
    if (breakpoints_.count(possible_breakpoint_location)) {
        auto got = breakpoints_.find (possible_breakpoint_location);
        if (got == breakpoints_.end()) {
            logger::error("breakpoint not found");
            return;
        }
        auto& bp = got->second;
        if (bp.enabled()) {
            auto previous_instruction_address = possible_breakpoint_location;
            setPc_(previous_instruction_address);
            bp.disable();
            if (::ptrace(PTRACE_SINGLESTEP, pid_, nullptr, nullptr) != 0) {
                throw Exception("ptrace single step error", errno);
            }
            waitSignal_();
            bp.enable();
        }
    }
}

int Debugger::waitSignal_() {
    auto status{0};
    auto options{0};
    if (::waitpid(pid_, &status, options) == -1) {
        throw Exception("wait pid error", errno);
    }
    return status;
}

uint64_t Debugger::getPc_() {
    return reg::getValue(pid_, reg::Reg86_64::rip);
}

void Debugger::setPc_(uint64_t pc) {
    reg::setValue(pid_, reg::Reg86_64::rip, pc);
}

dwarf::die Debugger::getFunctionFromPc_(const uint64_t pc) {
    for (auto &cu : dwarf_.compilation_units()) {
        if (die_pc_range(cu.root()).contains(pc)) {
            for (const auto& die : cu.root()) {
                if (die.tag == dwarf::DW_TAG::subprogram) {
                    if (die_pc_range(die).contains(pc)) {
                        return die;
                    }
                }
            }
        }
    }
    throw Exception("cannot find function");
}