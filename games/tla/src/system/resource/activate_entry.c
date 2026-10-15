#include "resource.h"
#include "types.h"

#include "vram_tab.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

// Frees a cache entry and the VRAM it holds; returns -1 for an index out of
// range.
s32 Resource_ResetEntry(u32 resource_index)
{
    struct VramBlockCacheEntry *entry = &ResourceTableEntries[resource_index];
    u32 offset;

    if (resource_index >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    offset = entry->offset;
    /* FAKEMATCH: ⚓️ reads the offset between the first two instructions of the split constant 0xffff; plainly the shift comes first (differ 2), as sched2 ranks the two equal and the shift has more dependents. An empty asm that reads the offset gives the load a second dependent, and it is taken first. */
    asm("" : "+l"(entry) : "l"(offset));
    if (offset != VRAM_CACHE_OFFSET_FREE) {
        Resource_ClearSlotReferences(resource_index);
        entry->offset |= VRAM_CACHE_OFFSET_FREE;
        entry->size = 0;
    }
    return 0;
}

s32 Resource_ActivateEntry(u32 resource_index)
{
    u16 *entry = (u16 *)&ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (*entry > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        *entry = value;
    }
    return 0;
}

// Loads size bytes from src into the object VRAM of cache entry no, reusing
// its blocks when the size is unchanged (src 0 loads nothing, -1 clears);
// returns the tile number, or 0 when it does not fit.
s32 VramBlock_LoadCached(u32 no, u32 size, const void *src)
{
    struct VramBlockCacheEntry *entry;
    s32 ofs;
    void *dst;

    {
        struct VramBlockCacheEntry *table = ResourceTableEntries;

        /* FAKEMATCH: ⚓️ loads the table's address before it copies no to r5; plainly the copy comes first (differ 2), as sched2 ranks the two equal in priority and in dependents and takes the earlier. Two empty asms that read the address give its load two more dependents, and it is taken first; one alone leaves the tie (differ 2). */
        asm("" : "+r"(src) : "l"(table));
        /* FAKEMATCH: the second of the pair above. */
        asm("" : "+l"(no) : "l"(table));
        entry = &table[no];
    }
    if (no >= VRAM_CACHE_ENTRY_COUNT)
        return 0;
    if (size > 8192)
        return 0;
    if (entry->size > 16) {
        if (entry->size != size) {
            Resource_ResetEntry(no);
            ofs = ResourceTable_AllocateBlocks(no, size);
        } else {
            ofs = entry->offset;
        }
    } else {
        ofs = ResourceTable_AllocateBlocks(no, size);
    }

    if (ofs != -1) {
        dst = (void *)(0x06010000 + ofs);
        entry->size = size;
        entry->offset = ofs;
        if (src != 0) {
            if (src == (const void *)-1)
                Iwram_ClearWords(dst, size);
            else
                Dma_Set(src, dst, (size >> 2) | (DMA_ENABLE32 | DMA_32BIT), REG_DMA3);
        }
        return (u32)ofs >> 5;
    }
    return 0;
}
