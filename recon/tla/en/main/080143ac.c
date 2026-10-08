#include "resource.h"
#include "vram_tab.h"

s32 Resource_FindFreeEntry(void)
{
    s32 slot;

    for (slot = 0; slot < VRAM_CACHE_ENTRY_COUNT; slot++) {
        if (ResourceTableEntries[slot].offset == VRAM_CACHE_OFFSET_FREE)
            return slot;
    }
    return VRAM_CACHE_ENTRY_COUNT;
}
