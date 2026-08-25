#include "types.h"

#define VramBlock_LoadCached Func_08003fa4

struct VramBlockCacheEntry {
    u16 size;
    u16 offset;
};

struct DmaChannelVram {
    const void *source;
    void *destination;
    u32 control;
};

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source)
{
    struct VramBlockCacheEntry *entries =
        (struct VramBlockCacheEntry *)0x03001b10;
    volatile struct DmaChannelVram *dma =
        (volatile struct DmaChannelVram *)0x040000d4;
    struct VramBlockCacheEntry *entry;
    s32 offset;

    if (slot > 95 || size > 0x2000) {
        return 0;
    }

    entry = &entries[slot];
    if (entry->size > 16) {
        if (entry->size == size) {
            offset = entry->offset;
        } else {
            Func_08003f3c(slot);
            offset = Func_08003e58(slot, size);
        }
    } else {
        offset = Func_08003e58(slot, size);
    }

    if (offset == -1) {
        return 0;
    }

    entry->size = size;
    entry->offset = offset;
    if (source != 0) {
        void *destination = (void *)(0x06010000 + offset);

        if (source == (const void *)-1) {
            Func_080072f0(destination, size, source, 0x03000164);
        } else {
            dma->source = source;
            dma->destination = destination;
            dma->control = (size >> 2) | 0x84000000;
        }
    }

    return (u32)offset >> 5;
}
