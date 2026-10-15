/* Draft, differ 2 of 26: the native loads the table address between the
 * first two instructions of the split constant 511 (movs r0, #128; ldr r3;
 * lsls r0, r0, #1), this schedules the shift first. sched2 ranks the load and
 * the shift equal (priority 2) and takes the shift. Measured: the twin's
 * do-while with a limit variable is differ 4; a for loop over the index or the
 * pointer, any declaration order, a barrier or a one-pass block before the
 * loop are differ 2 or worse; three minutes of permuter found nothing. */
#include "vram_tab.h"

void Resource_InitializeTable(void)
{
    u32 count;
    u8 *marker = ResourceBlockOwners;

    for (count = 0; count < VRAM_BLOCK_COUNT; count++)
        *marker++ = VRAM_BLOCK_OWNER_FREE;
    {
        struct VramBlockCacheEntry *entry = ResourceTableEntries;

        count = 0;
        do {
            entry->offset |= VRAM_CACHE_OFFSET_FREE;
            entry->size = 0;
            entry++;
            count++;
        } while (count < VRAM_CACHE_ENTRY_COUNT);
    }
}
