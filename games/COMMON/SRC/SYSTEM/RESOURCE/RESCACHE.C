#include "RESOURCE.H"
#include "VRAM_TAB.H"

#if !(defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT))
#include "IWRAM_CALL.H"
#include "DMA.H"

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

    blocks = size / VRAM_BLOCK_BYTES;
    if (id >= VRAM_CACHE_ENTRY_COUNT) {
        return -1;
    }
    pos = 0;
    for (;;) {
        result = -1;
        if (pos >= VRAM_BLOCK_COUNT) {
            goto done;
        }
        if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
            goto occupied;
        }
        result = pos;
        end = blocks + result;
        while (pos < end) {
            if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
                goto occupied;
            }
            pos++;
        }
        for (i = 0; i < blocks; i++) {
            ResourceBlockOwners[result + i] = id;
        }
        goto found;
occupied:
        pos += (u32)gVramBlockCache[ResourceBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
    }
found:
    result *= VRAM_BLOCK_BYTES;
done:
    return result;
}

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    s32 run = 0;
    u8 *marker = ResourceBlockOwners;
    s32 longest = 0;
    s32 remaining = VRAM_BLOCK_COUNT;

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

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    marker = ResourceBlockOwners;
    empty_marker = VRAM_BLOCK_OWNER_FREE;
    remaining = VRAM_BLOCK_COUNT;
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
    struct VramBlockCacheEntry *entry = &gVramBlockCache[resource_index];

    if (resource_index >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
        Resource_ClearSlotReferences(resource_index);
        entry->offset |= VRAM_CACHE_OFFSET_FREE;
        entry->size = 0;
    }
    return 0;
}

s32 Resource_ActivateEntry(u32 resource_index)
{
    struct VramBlockCacheEntry *entry = &gVramBlockCache[resource_index];

    if (resource_index >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    if (entry->size > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        entry->size = value;
    }
    return 0;
}

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source)
{
    struct VramBlockCacheEntry *entry;
    s32 offset;
    void *destination;

    entry = &gVramBlockCache[slot];
    if (slot >= VRAM_CACHE_ENTRY_COUNT)
        return 0;
    if (size > 0x2000)
        return 0;
    if (entry->size > 16) {
        if (entry->size != size) {
            Resource_ResetEntry(slot);
            offset = ResourceTable_AllocateBlocks(slot, size);
        } else {
            offset = entry->offset;
        }
    } else {
        offset = ResourceTable_AllocateBlocks(slot, size);
    }

    if (offset != -1) {
        destination = (void *)(0x06010000 + offset);
        entry->size = size;
        entry->offset = offset;
        if (source != 0) {
            if (source == (const void *)-1) {
                Iwram_ClearWords(destination, size);
            } else {
                Dma_Set(source, destination, (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
            }
        }
        return (u32)offset >> 5;
    }
    return 0;
}

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
        struct VramBlockCacheEntry *resource_entry = gVramBlockCache;

        count = 0;
        do {
            resource_entry->offset |= VRAM_CACHE_OFFSET_FREE;
            resource_entry->size = 0;
            resource_entry++;
            count++;
        } while (count < VRAM_CACHE_ENTRY_COUNT);
    }
}

/* An unused cache entry has no assigned VRAM byte offset. */
s32 Resource_FindFreeEntry(void)
{
    /* FAKEMATCH: the ordinary for scan reduces this complete
       native search from 52 to 36 bytes. Retain the existing first-entry
       test and subsequent scan over the actual cache records. */
    s32 free_slot;
    s32 slot;
    struct VramBlockCacheEntry *table;
    s32 first;
    struct VramBlockCacheEntry *entry;

    entry = gVramBlockCache;
    free_slot = VRAM_CACHE_ENTRY_COUNT;
    first = 0;
    slot = first;
    table = gVramBlockCache;
    if (table->offset == VRAM_CACHE_OFFSET_FREE)
        return first;
next_entry:
    slot++;
    entry++;
    if (slot < VRAM_CACHE_ENTRY_COUNT) {
        if (entry->offset == VRAM_CACHE_OFFSET_FREE)
            free_slot = slot;
        else
            goto next_entry;
    }
    return free_slot;
}

s32 Resource_LoadIntoFreeSlot(s32 size)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, size, 0);
    return slot;
}

#endif

s32 Resource_GetBuffer(s32 index, s32 source)
{
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    return VramBlock_LoadCached(index, ResourceTableEntries[index].size, (const void *)source);
#else
    return VramBlock_LoadCached(index, gVramBlockCache[index].size, (const void *)source);
#endif
}
