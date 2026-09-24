/* Draft, not exact (2026-09-24): 95 differing halfwords, 308 of 308 bytes.
   Residual: register allocation of the three nibble loops. The reference
   keeps the buffer base in lr and the table pointer (later the 15 mask) in
   sl; this C swaps them, which moves every loop register. The slot search
   still differs: the reference truncates slot with lsls/lsrs after the
   increment (slot = (u8)value gives that but grows the owner by 4 bytes). */
#include "DMA.H"

struct PaletteSlotWork {
    u8 unknown_000[0xda0];
    u8 used[0x100];
    u16 next;
};

extern struct PaletteSlotWork *Data_03001e8c;

u8 *Resource_GetTableEntry(s32 index);
extern u8 Value_00000013;

void Func_0801dd28(u16 *entry, u16 *mirror, s32 index, u8 *remap)
{
    struct PaletteSlotWork *work = Data_03001e8c;
    u8 *table = Resource_GetTableEntry((s32)&Value_00000013);
    u32 slot = *(u8 *)entry;
    u8 buf[128];
    u8 *src;
    u8 *dst;
    u32 i;
    u32 value;
    u32 color;

    src = (u8 *)(0x06000000 + slot * 32);
    dst = buf;
    for (i = 0; i < 32; i++) {
        value = *src++;
        dst[0] = value & 15;
        dst[1] = value >> 4;
        dst += 2;
    }

    src = table + index * 32;
    dst = buf;
    for (i = 0; i < 32; i++) {
        value = *src++;
        color = remap[value & 15];
        if (color != 0)
            *dst = color;
        dst++;
        color = remap[value >> 4];
        if (color != 0)
            *dst = color;
        dst++;
    }

    src = buf;
    dst = buf;
    for (i = 0; i < 32; i++) {
        *dst++ = src[0] | (src[1] << 4);
        src += 2;
    }

    if ((s8)slot >= 0) {
        for (i = 0; i < 128; i++) {
            value = work->next;
            work->next = (u8)(value + 1) & 127;
            slot = value;
            if (work->used[slot] == 0)
                break;
        }
        work->used[slot] = 1;
        slot |= 0x80;
        *entry = slot | 0xf000;
        *mirror = slot | 0xf000;
    }
    Dma_Set(buf, (void *)(0x06000000 + slot * 32), 0x84000008, (volatile u32 *)0x040000d4);
}
