#include "types.h"

extern u8 *Data_03001e8c;

u8 *Func_08002f40(s32);

void Func_0801dd28(u16 *palette_id, u16 *palette_id_copy,
                   s32 source_index, const u8 *translation)
{
    u8 unpacked[128];
    u8 *runtime = Data_03001e8c;
    u8 *source = Func_08002f40(0xf000);
    s32 palette = *(u8 *)palette_id;
    s32 index;
    u8 *cursor;
    s32 vram_offset;

    cursor = unpacked;
    index = 0;
    do {
        u8 packed = *(u8 *)(0x06000000 + palette * 32 + index);
        *cursor++ = packed & 0x0f;
        *cursor++ = packed >> 4;
        index++;
    } while ((u32)index <= 31);

    source += source_index * 32;
    cursor = unpacked;
    index = 0;
    do {
        u8 packed = *source++;
        u8 value = translation[packed & 0x0f];
        if (value != 0)
            cursor[0] = value;
        cursor++;
        value = translation[packed >> 4];
        if (value != 0)
            cursor[0] = value;
        cursor++;
        index++;
    } while ((u32)index <= 31);

    cursor = unpacked;
    index = 0;
    do {
        cursor[index] = unpacked[index * 2] |
                        (unpacked[index * 2 + 1] << 4);
        index++;
    } while ((u32)index <= 31);

    vram_offset = palette << 5;
    if ((s8)palette >= 0) {
        u16 *next = (u16 *)(runtime + 0xea0);
        s32 attempts = 0;
        s32 slot;

        do {
            s32 previous = *next;
            slot = (u8)previous;
            *next = (previous + 1) & 0x7f;
            if (runtime[0xda0 + slot] == 0)
                break;
            attempts++;
        } while ((u32)attempts <= 127);

        runtime[0xda0 + slot] = 1;
        palette = slot | 0x80;
        *palette_id = palette | 0xf000;
        *palette_id_copy = palette | 0xf000;
        vram_offset = palette << 5;
    }

    {
        u32 *dma = (u32 *)0x040000d4;
        u32 dma_source = (u32)unpacked;
        u32 destination = 0x06000000 + vram_offset;
        u32 control = 0x84000008;

        dma[0] = dma_source;
        dma[1] = destination;
        dma[2] = control;
    }
}
