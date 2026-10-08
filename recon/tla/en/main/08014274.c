#include "resource.h"
#include "vram_tab.h"

s32 Resource_ResetEntry(u32 resource_index)
{
    struct VramBlockCacheEntry *entry = &ResourceTableEntries[resource_index];

    if (resource_index >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
        Resource_ClearSlotReferences(resource_index);
        entry->offset |= VRAM_CACHE_OFFSET_FREE;
        entry->size = 0;
    }
    return 0;
}
