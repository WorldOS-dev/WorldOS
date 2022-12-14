#include "PageFrameAllocator.hpp"

#include <util.h>
#include <stdio.hpp>

#include <HAL/hal.hpp>

#include "PageMapIndexer.hpp"

x86_64_WorldOS::PageFrameAllocator* g_PFA = nullptr;

namespace x86_64_WorldOS {

    /* PageFrameAllocator class */

    /* Public Methods */

    PageFrameAllocator::PageFrameAllocator() {
        m_FreeMem = 0;
        m_ReservedMem = 0;
        m_UsedMem = 0;
        m_MemSize = 0;
    }

    PageFrameAllocator::~PageFrameAllocator() {
        
    }

    void PageFrameAllocator::SetMemoryMap(const WorldOS::MemoryMapEntry* FirstMemoryMapEntry, const size_t MemoryMapEntryCount) {
        using namespace WorldOS;
        // Setup
        m_MemSize = GetMemorySize(&FirstMemoryMapEntry, MemoryMapEntryCount);
        m_FreeMem = 0;
        m_ReservedMem = 0;
        m_UsedMem = 0;

        size_t BitmapSize = m_MemSize / 4096 / 8;
        if ((m_MemSize / 4096) % 8 != 0) {
            BitmapSize++;
        }

        m_Bitmap.SetSize(BitmapSize);
        // Get Location for Bitmap
        bool BitmapAddressFound = false;
        for (uint64_t i = 0; i < MemoryMapEntryCount; i++) {
            MemoryMapEntry* entry = (MemoryMapEntry*)((uint64_t)FirstMemoryMapEntry + (i * MEMORY_MAP_ENTRY_SIZE));
            if ((entry->Address + entry->length) > GiB(1)) break; // bitmap must be under 1 GiB
            if ((entry->type == WORLDOS_MEMORY_FREE) && (entry->length >= BitmapSize)) {
                m_ReservedMem += BitmapSize;
                m_Bitmap.SetBuffer((uint8_t*)entry->Address);
                x86_64_map_page(m_Bitmap.GetBuffer(), m_Bitmap.GetBuffer(), 3);
                fast_memset(m_Bitmap.GetBuffer(), 0, BitmapSize/8);
                for (uint64_t j = 0; j < (BitmapSize / 4096); j++) {
                    m_Bitmap.Set(((entry->Address / 4096) + j), true);
                }
                BitmapAddressFound = true;
            }
        }
        if (!BitmapAddressFound) {
            Panic("Failed to find address for page bitmap", nullptr, false);
            return;
        }
        // Fill bitmap
        for (uint64_t i = 0; i < MemoryMapEntryCount; i++) {
            MemoryMapEntry* entry = (MemoryMapEntry*)((uint64_t)FirstMemoryMapEntry + (i * MEMORY_MAP_ENTRY_SIZE));
            if (entry->type == WORLDOS_MEMORY_FREE) {
                m_FreeMem += entry->length;
                if (entry->length > 4096) {
                    for (uint64_t j = 0; j < (entry->length / 4096); j += 4096) {
                        m_Bitmap.Set(j + entry->Address, false);
                    }
                } else {
                    m_Bitmap.Set(entry->Address, false);
                }
            } else {
                m_ReservedMem += entry->length;
                if (entry->length > 4096) {
                    for (uint64_t j = 0; j < (entry->length / 4096); j += 4096) {
                        m_Bitmap.Set(j + entry->Address, true);
                    }
                } else {
                    m_Bitmap.Set(entry->Address, true);
                }
            }
        }

        if (!m_Bitmap[0]) {
            m_ReservedMem += 0x1000;
            m_Bitmap.Set(0, true);
        }
    }

    void* PageFrameAllocator::AllocatePage() {
        uint64_t index = FindFreePage();
        LockPage((void*)(index * 4096));
        return (void*)(index * 4096);
    }

    void PageFrameAllocator::ReservePage(void* page) {
        if (m_Bitmap[((uint64_t)page / 4096)]) return;
        m_Bitmap.Set(((uint64_t)page / 4096), true);
        m_FreeMem -= 4096;
        m_ReservedMem += 4096;
    }

    void PageFrameAllocator::FreePage(void* page) {
        if (!m_Bitmap[((uint64_t)page / 4096)]) return;
        m_Bitmap.Set(((uint64_t)page / 4096), false);
        m_FreeMem += 4096;
        m_ReservedMem -= 4096;
    }

    /* Private Methods */

    void PageFrameAllocator::LockPage(void* page) {
        if (m_Bitmap[((uint64_t)page / 4096)]) return;
        m_Bitmap.Set(((uint64_t)page / 4096), true);
        m_FreeMem -= 4096;
        m_UsedMem += 4096;
    }

    void PageFrameAllocator::UnlockPage(void* page) {
        if (!m_Bitmap[((uint64_t)page / 4096)]) return;
        m_Bitmap.Set(((uint64_t)page / 4096), false);
        m_FreeMem += 4096;
        m_UsedMem -= 4096;
    }


    uint64_t PageFrameAllocator::FindFreePage() {
        for (uint64_t i = 0; i < m_Bitmap.GetSize(); i++) {
            if (m_Bitmap[i] == false) {
                return i * 8;
            }
        }
        return 0;
    }

}