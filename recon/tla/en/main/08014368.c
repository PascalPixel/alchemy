#include "vram_tab.h"

void Resource_InitializeTable(void)
{
    u32 limit = VRAM_BLOCK_COUNT - 1;
    u8 *occupancy_markers = ResourceBlockOwners;
    u32 count = 0;
    u32 empty_marker = VRAM_BLOCK_OWNER_FREE;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct VramBlockCacheEntry *resource_entry = ResourceTableEntries;

        count = 0;
        do {
            resource_entry->offset |= VRAM_CACHE_OFFSET_FREE;
            resource_entry->size = 0;
            resource_entry++;
            count++;
        } while (count < VRAM_CACHE_ENTRY_COUNT);
    }
}
