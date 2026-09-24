#include "DMA.H"

struct PaletteSnapshot {
    u16 *source;
    u16 unknown_04;
    u16 unknown_06;
    s16 value;
    u16 count;
    u16 colors[16];
};

struct PaletteSnapshotWork {
    struct PaletteSnapshot entries[4];
    u16 count;
};

extern struct PaletteSnapshotWork *Data_03001ec0;

/* Queues up to four palette snapshots (the queue Runtime_AllocateAndClearQueue
   clears): copies count colours of palette bank:index into the next slot. */
s32 PaletteQueue_Add(s16 bank, s16 index, s16 value, s16 count)
{
    struct PaletteSnapshotWork *work;
    struct PaletteSnapshot *entry;
    u16 *source;
    u32 slot;
    u16 size;

    work = Data_03001ec0;
    slot = work->count;
    if (slot > 3)
        return -1;
    entry = &work->entries[slot];
    source = (u16 *)0x05000000 + (((u16)bank << 4) + (u16)index);
    size = count;
    entry->unknown_04 = 0;
    entry->unknown_06 = 0;
    entry->count = size;
    entry->source = source;
    entry->value = value;
    Dma_Set(source, entry->colors, 0x80000000 | size, (volatile u32 *)0x040000d4);
    work->count++;
    return 0;
}
