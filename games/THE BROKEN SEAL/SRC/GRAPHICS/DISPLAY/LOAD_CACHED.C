#include "VRAM_BLOCK.H"
#include "DMA.H"



s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source)
{
    struct VramBlockCacheEntry *entry;
    s32 offset;
    void *destination;

    entry = &gVramBlockCache[slot];
    if (slot > 95)
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
                ((void (*)(void *, u32))0x03000164)(destination, size);
            } else {
                Dma_Set(source, destination, (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
            }
        }
        return (u32)offset >> 5;
    }
    return 0;
}
