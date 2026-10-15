#include "resource.h"
#include "types.h"

#include "vram_tab.h"

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
