#include "resource.h"
#include "vram_tab.h"

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    marker = ResourceBlockOwners;
    /* FAKEMATCH: the plain order sets up the count before the marker. */
    __asm__("" : "+r"(marker));
    empty_marker = VRAM_BLOCK_OWNER_FREE;
    /* FAKEMATCH: the plain order sets up the count before the marker. */
    __asm__("" : "+r"(marker));
    remaining = VRAM_BLOCK_COUNT;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        marker++;
        remaining--;
    } while (remaining != 0);
    if (cleared_count != 0)
        return -1;
    return 0;
}
