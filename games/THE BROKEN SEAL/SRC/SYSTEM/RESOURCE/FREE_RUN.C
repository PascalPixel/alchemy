#include "TYPES.H"
#include "VRAM_BLOCK.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

extern u8 ResourceBlockOwners[512];

/* Finds the first run of SIZE / 64 free VRAM blocks, marks them as owned by
   resource ID and returns the run's byte offset, or -1 when ID is out of range
   or no run is free. Occupied blocks are skipped a whole cached entry at a
   time. */
s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;
    s32 pos;
    u32 end;
    u32 i;

    blocks = size >> 6;
    if (id > 95) {
        return -1;
    }
    pos = 0;
    for (;;) {
        result = -1;
        if (pos >= 512) {
            goto done;
        }
        if (ResourceBlockOwners[pos] != 0xff) {
            goto occupied;
        }
        result = pos;
        end = blocks + result;
        while (pos < end) {
            if (ResourceBlockOwners[pos] != 0xff) {
                goto occupied;
            }
            pos++;
        }
        for (i = 0; i < blocks; i++) {
            ResourceBlockOwners[result + i] = id;
        }
        goto found;
occupied:
        pos += gVramBlockCache[ResourceBlockOwners[pos]].size >> 6;
    }
found:
    result <<= 6;
done:
    return result;
}
#endif

extern u8 ResourceBlockOwners[];

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    s32 run = 0;
    u8 *marker = ResourceBlockOwners;
    s32 longest = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ != 0xff) {
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

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern u8 ResourceBlockOwners[];
extern struct ResourceTableEntry ResourceTableEntries[];

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id > 0x5f)
        return -1;
    marker = ResourceBlockOwners;
    empty_marker = 0xff;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0)
        return -1;
    return 0;
}

s32 Resource_ResetEntry(u32 resource_index)
{
    struct ResourceTableEntry *entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (entry->flags != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        entry->flags |= 0xffff;
        entry->value = 0;
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
