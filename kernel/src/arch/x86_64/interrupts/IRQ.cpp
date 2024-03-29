/*
Copyright (©) 2022-2023  Frosty515

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "IRQ.hpp"
#include "pic.hpp"
#include "../io.h"
#include <stdio.h>

#define PIC_REMAP_OFFSET 0x20

x86_64_IRQHandler_t g_IRQHandlers[16];

void x86_64_IRQ_Handler(x86_64_Interrupt_Registers* regs) {
    uint8_t irq = regs->interrupt - PIC_REMAP_OFFSET;

    if (g_IRQHandlers[irq] != nullptr) {
        g_IRQHandlers[irq](regs);
    }
    else {
        dbgprintf("Unhandled IRQ %d...\n", irq);
    }

    x86_64_PIC_sendEOI(irq);
}

void x86_64_IRQ_Initialize() {
    x86_64_PIC_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false /* temp, replace with correct autoEOI */);

    for (uint8_t i = 0; i < 16; i++)
        x86_64_ISR_RegisterHandler(PIC_REMAP_OFFSET + i, x86_64_IRQ_Handler);

    x86_64_EnableInterrupts();
}

void x86_64_IRQ_RegisterHandler(const uint8_t irq, x86_64_IRQHandler_t handler) {
    g_IRQHandlers[irq] = handler;
}
