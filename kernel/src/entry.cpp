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

#include <stdint.h>
#include <stddef.h>

#include "kernel.hpp"
#include "Memory/Memory.hpp"
#include "limine.h"

extern "C" {

LIMINE_BASE_REVISION(1)

volatile struct limine_framebuffer_request framebuffer_request {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile struct limine_memmap_request memmap_request {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile struct limine_efi_system_table_request efi_system_table_request {
    .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile struct limine_rsdp_request rsdp_request {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile struct limine_kernel_address_request kernel_address_request {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile struct limine_kernel_file_request kernel_file_request {
    .id = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 0,
    .response = nullptr
};


volatile struct limine_hhdm_request hhdm_request {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile struct limine_module_request initramfs_request {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0,
    .response = nullptr,
    .internal_module_count = 0,
    .internal_modules = nullptr
};

}
 
static void done(void) {
    while (true) {
        __asm__("hlt");
    }
}

KernelParams kernelParams;

// called from asm
extern "C" void _start(void) {
    if (framebuffer_request.response == nullptr) {
        done();
    }

    limine_framebuffer_response* framebuffer_response = framebuffer_request.response;

    if (!(framebuffer_response->framebuffer_count > 0)) {
        done();
    }

    if (memmap_request.response == nullptr) {
        done();
    }

    if (efi_system_table_request.response == nullptr) {
        done();
    }

    if (rsdp_request.response == nullptr) {
        done();
    }

    if (kernel_address_request.response == nullptr) {
        done();
    }

    if (kernel_file_request.response == nullptr) {
        done();
    }

    if (hhdm_request.response == nullptr) {
        done();
    }

    if (initramfs_request.response == nullptr) {
        done();
    }

    limine_kernel_file_response* kernel_file_response = kernel_file_request.response;

    if (kernel_file_response->kernel_file == nullptr) {
        done();
    }

    limine_efi_system_table_response* efi_system_table_response = efi_system_table_request.response;

    limine_rsdp_response* rsdp_response = rsdp_request.response;

    limine_kernel_address_response* kernel_address_response = kernel_address_request.response;

    limine_memmap_response* memmap_response = memmap_request.response;
    
    limine_framebuffer* buffer = framebuffer_response->framebuffers[0];

    limine_file* kernel_file = kernel_file_response->kernel_file;

    limine_hhdm_response* hhdm_response = hhdm_request.response;

    limine_module_response* module_response = initramfs_request.response;

    if (module_response->module_count < 1) {
        done();
    }

    limine_file* initramfs_file = module_response->modules[0];

    if (kernel_file->size == 0) {
        done();
    }

    if ((hhdm_response->offset & UINT32_MAX) != 0 || hhdm_response->offset < 0xFFFF800000000000) {
        done();
    }

    

    MemoryMapEntry** memoryMap = (MemoryMapEntry**)memmap_response->entries;

    kernelParams = {
        .frameBuffer = {buffer->address, buffer->width, buffer->height, buffer->bpp, buffer->red_mask_size, buffer->red_mask_shift, buffer->green_mask_size, buffer->green_mask_shift, buffer->blue_mask_size, buffer->blue_mask_shift},
        .MemoryMap = memoryMap,
        .MemoryMapEntryCount = memmap_response->entry_count,
        .EFI_SYSTEM_TABLE_ADDR = efi_system_table_response->address,
        .kernel_physical_addr = kernel_address_response->physical_base,
        .kernel_virtual_addr = kernel_address_response->virtual_base,
        .kernel_size = kernel_file->size,
        .RSDP_table = rsdp_response->address,
        .hhdm_start_addr = hhdm_response->offset,
        .initramfs_addr = initramfs_file->address,
        .initramfs_size = initramfs_file->size
    };

    StartKernel(&kernelParams);

    // We're done, just hang...
    done();
}
