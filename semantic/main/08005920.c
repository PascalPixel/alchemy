#include "types.h"

struct RecordStore_08005920 {
    u8 occupied[16];
    u8 record_ids[16];
    u16 generations[16];
    u8 image[4096];
};

struct RecordHeader_08005920 {
    u8 template_data[7];
    u8 record_id;
    u16 checksum;
    u16 generation;
    u8 unknown_0C[4];
};

struct Dma3_08005920 {
    const void *source;
    void *destination;
    u32 control;
};

u32 Func_08005810(s32);
u32 Func_08005868(u16);
s32 Func_08005ae0(void);
u32 Func_08005b24(s32);
u32 Func_08005b64(s32);
u32 Func_08005c2c(s32);

/*
 * Build a record image in the shared work area, validate it, and attach it
 * to a free metadata slot. A generation-counter wrap replaces the old slot.
 */
s32 Func_08005920(s32 record_id, const void *source)
{
    struct RecordStore_08005920 *store =
        *(struct RecordStore_08005920 **)0x03001F1C;
    volatile struct Dma3_08005920 *dma =
        (volatile struct Dma3_08005920 *)0x040000D4;
    struct RecordHeader_08005920 header;
    u32 previous_slot;
    u32 slot;
    u32 zero;

    zero = 0;
    dma->source = &zero;
    dma->destination = store->image;
    dma->control = 0x85000400;
    while ((dma->control & 0x80000000) != 0) {
    }

    previous_slot = Func_08005b24(record_id);
    slot = Func_08005810(record_id);
    if (slot > 15) {
        return 1;
    }

    dma->source = source;
    dma->destination = store->image + 16;
    dma->control = 0x840003FC;
    while ((dma->control & 0x80000000) != 0) {
    }

    dma->source = (const void *)0x080079B8;
    dma->destination = &header;
    dma->control = 0x84000002;
    while ((dma->control & 0x80000000) != 0) {
    }

    header.record_id = (u8)record_id;
    header.checksum = (u16)Func_08005ae0();
    header.generation = (u16)(Func_08005c2c(record_id) + 1);

    dma->source = &header;
    dma->destination = store->image;
    dma->control = 0x84000004;
    while ((dma->control & 0x80000000) != 0) {
    }

    if (Func_08005868((u16)slot) != 0) {
        return 1;
    }
    if (previous_slot <= 15 && Func_08005b64((s32)previous_slot) != 0) {
        return 1;
    }

    if (header.generation > 0xFDE8) {
        header.generation = 1;
        dma->source = &header;
        dma->destination = store->image;
        dma->control = 0x84000004;
        while ((dma->control & 0x80000000) != 0) {
        }

        if (Func_08005868((u16)previous_slot) != 0) {
            return 1;
        }
        if (Func_08005b64((s32)slot) != 0) {
            return 1;
        }
        slot = previous_slot;
    }

    store->occupied[slot] = 1;
    store->record_ids[slot] = (u8)record_id;
    store->generations[slot] = header.generation;
    return 0;
}
