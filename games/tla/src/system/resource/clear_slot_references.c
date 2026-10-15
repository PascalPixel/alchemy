#include "resource.h"
#include "vram_tab.h"

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    u8 *marker = ResourceBlockOwners;
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
    u8 *marker = ResourceBlockOwners;
    s32 free_count;
    s32 remaining;

    asm volatile(""); /* FAKEMATCH: ⚓️ loads the table address before it starts the count of 512; without the barrier the count's first half is scheduled ahead of the load (differ 2) */
    free_count = 0;
    remaining = 0x200;
    do {
        if (*marker++ == 0xff) {
            free_count++;
        }
        remaining--;
    } while (remaining != 0);
    return free_count;
}

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
