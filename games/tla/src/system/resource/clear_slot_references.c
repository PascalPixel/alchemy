#include "resource.h"
#include "vram_tab.h"

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    u8 *marker = gResourceBlockOwners;
    s32 run;
    s32 longest;
    /* FAKEMATCH: the count takes r2 and the run r1; unpinned, the barrier below swaps them (differ 18), and without the barrier the pin alone schedules the count's first half ahead of the table load (differ 2) */
    register s32 remaining asm("r2");

    asm volatile(""); /* FAKEMATCH: ⚓️ loads the table address before it starts the count of 512; without the barrier the count's first half is scheduled ahead of the load (differ 2) */
    run = 0;
    longest = 0;
    remaining = VRAM_BLOCK_COUNT;
    do {
        if (*marker++ != VRAM_BLOCK_OWNER_FREE) {
            run = 0;
        } else {
            run++;
            if (longest < run)
                longest = run;
        }
        remaining--;
    } while (remaining != 0);
    return longest;
}

s32 Func_08014220(void)
{
    u8 *marker = gResourceBlockOwners;
    s32 freeCount;
    s32 remaining;

    asm volatile(""); /* FAKEMATCH: ⚓️ loads the table address before it starts the count of 512; without the barrier the count's first half is scheduled ahead of the load (differ 2) */
    freeCount = 0;
    remaining = 0x200;
    do {
        if (*marker++ == 0xff) {
            freeCount++;
        }
        remaining--;
    } while (remaining != 0);
    return freeCount;
}

s32 Resource_ClearSlotReferences(s32 resourceId)
{
    s32 clearedCount = 0;
    s32 remaining;
    u8 *marker;
    u8 emptyMarker;

    if ((u32)resourceId >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    marker = gResourceBlockOwners;
    /* FAKEMATCH: the plain order sets up the count before the marker. */
    __asm__("" : "+r"(marker));
    emptyMarker = VRAM_BLOCK_OWNER_FREE;
    /* FAKEMATCH: the plain order sets up the count before the marker. */
    __asm__("" : "+r"(marker));
    remaining = VRAM_BLOCK_COUNT;
    do {
        if (*marker == resourceId) {
            *marker = emptyMarker;
            clearedCount++;
        }
        marker++;
        remaining--;
    } while (remaining != 0);
    if (clearedCount != 0)
        return -1;
    return 0;
}
