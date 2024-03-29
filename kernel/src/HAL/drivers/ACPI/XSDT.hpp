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

#ifndef _HAL_ACPI_XSDT_HPP
#define _HAL_ACPI_XSDT_HPP

#include <stdint.h>

#include "SDT.hpp"

bool InitAndValidateXSDT(void* XSDT);

ACPISDTHeader* getOtherSDT(uint64_t index);

uint64_t getSDTCount();

#endif /* _HAL_ACPI_XSDT_HPP */