#include "ELFKernel.hpp"

#include "Memory/PageMapIndexer.hpp"

namespace x86_64_WorldOS {
    bool MapKernel(void* kernel_phys, void* kernel_virt, size_t kernel_size) {
        // page aligned start addresses
        uint64_t text_start_addr = (uint64_t)_text_start_addr;
        text_start_addr -= text_start_addr % 4096;
        uint64_t rodata_start_addr = (uint64_t)_rodata_start_addr;
        rodata_start_addr -= rodata_start_addr % 4096;
        uint64_t data_start_addr = (uint64_t)_data_start_addr;
        data_start_addr -= data_start_addr % 4096;
        uint64_t bss_start_addr = (uint64_t)_bss_start_addr;
        bss_start_addr -= bss_start_addr % 4096;

        uint64_t text_size = (uint64_t)_text_end_addr - text_start_addr;
        uint64_t rodata_size = (uint64_t)_rodata_end_addr - rodata_start_addr;
        uint64_t data_size = (uint64_t)_data_end_addr - data_start_addr;
        uint64_t bss_size = (uint64_t)_bss_end_addr - bss_start_addr;

        // this just makes sure nothing is cut off
        text_size   += 4095;
        rodata_size += 4095;
        data_size   += 4095;
        bss_size    += 4095;

        // convert to pages
        text_size   /= 4096;
        rodata_size /= 4096;
        data_size   /= 4096;
        bss_size    /= 4096;

        // pre-map kernel as Read-only and No execute in case there are more unused sections
        kernel_size += 4095;
        kernel_size /= 4096;

        for (uint64_t i = 0; i < kernel_size; i++) {
            x86_64_map_page((void*)((uint64_t)kernel_phys + (i * 4096)), (void*)((uint64_t)kernel_virt + (i * 4096)), 0x8000001);
        }

        // actually do the mapping
        for (uint64_t i = 0; i < text_size; i++) {
            x86_64_map_page((void*)(text_start_addr + (i * 4096) - (uint64_t)kernel_virt + (uint64_t)kernel_phys), (void*)(text_start_addr + (i * 4096)), 0x1); // Present, Read-only, Execute
        }

        for (uint64_t i = 0; i < rodata_size; i++) {
            x86_64_map_page((void*)(rodata_start_addr + (i * 4096) - (uint64_t)kernel_virt + (uint64_t)kernel_phys), (void*)(rodata_start_addr + (i * 4096)), 0x8000001); // Present, Read-only, Execute Disable
        }

        for (uint64_t i = 0; i < data_size; i++) {
            x86_64_map_page((void*)(data_start_addr + (i * 4096) - (uint64_t)kernel_virt + (uint64_t)kernel_phys), (void*)(data_start_addr + (i * 4096)), 0x8000003); // Present, Read-Write, Execute Disable
        }

        for (uint64_t i = 0; i < bss_size; i++) {
            x86_64_map_page((void*)(bss_start_addr + (i * 4096) - (uint64_t)kernel_virt + (uint64_t)kernel_phys), (void*)(bss_start_addr + (i * 4096)), 0x8000003); // Present, Read-Write, Execute Disable
        }

        return true;
    }
}