#include "resource.h"
#include "vram_tab.h"

s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;
    s32 pos;
    u32 end;
    u32 i;

    blocks = size / VRAM_BLOCK_BYTES;
    if (id >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    pos = 0;
    for (;;) {
        result = -1;
        if (pos >= VRAM_BLOCK_COUNT)
            goto done;
        if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
            goto occupied;
        end = blocks + pos;
        if (end > VRAM_BLOCK_COUNT)
            goto done;
        result = pos;
        while (pos < end) {
            if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
                goto occupied;
            pos++;
        }
        for (i = 0; i < blocks; i++)
            ResourceBlockOwners[result + i] = id;
        goto found;
occupied:
        pos += (u32)ResourceTableEntries[ResourceBlockOwners[pos]].size
            / VRAM_BLOCK_BYTES;
    }
found:
    result *= VRAM_BLOCK_BYTES;
done:
    return result;
}
