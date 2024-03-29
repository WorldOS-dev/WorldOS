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

#ifndef _KERNEL_X86_64_IRQ_HPP
#define _KERNEL_X86_64_IRQ_HPP

#include "isr.hpp"

typedef void (*x86_64_IRQHandler_t)(x86_64_Interrupt_Registers* regs);

void x86_64_IRQ_Initialize();
void x86_64_IRQ_RegisterHandler(const uint8_t irq, x86_64_IRQHandler_t handler);

#endif /* _KERNEL_X86_64_IRQ_HPP */