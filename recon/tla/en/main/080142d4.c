/* Draft, differ 2 of 65: The Broken Seal's VramBlock_LoadCached with this
 * game's ResourceTableEntries. The native loads the table address first,
 * before it copies no into r5 (ldr r3, =ResourceTableEntries; adds r5, r0,
 * #0); this copies the argument first. The same one-row load order as the
 * other near misses of this lane (Resource_InitializeTable, GameFlag_SetNibble,
 * UiText_ShowCenteredMessage): in each, the native schedules a load one slot
 * earlier than sched2 ranks it here. */
#include "resource.h"
#include "vram_tab.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

s32 VramBlock_LoadCached(u32 no, u32 size, const void *src)
{
    struct VramBlockCacheEntry *entry;
    s32 ofs;
    void *dst;

    entry = &ResourceTableEntries[no];
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
