#ifndef _KERNEL_PANIC_HPP
#define _KERNEL_PANIC_HPP

#include "interrupts/isr.hpp"

struct x86_64_PanicArgs {
    const char* reason;
    x86_64_Registers* regs;
    bool type = false;
};

// reason = message to display, regs = registers at the time of error, type = the type of error (true for interrupt and false for other)
void x86_64_Panic(const char* reason, x86_64_Registers* regs, const bool type = false);

#endif /* _KERNEL_PANIC_HPP */