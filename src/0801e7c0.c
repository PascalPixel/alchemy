#include "types.h"

struct TextPosition_0801e7c0 {
    u8 padding[12];
    u16 x;
    u16 y;
};

extern u8 *Data_03001e8c;

s32 Func_08018038(s32 character, s32 count);
s32 Func_0801de5c(u16 *text, s32 map_address, s32 vram_address, s32 phase);

void Func_0801e7c0(
    s32 character,
    struct TextPosition_0801e7c0 *position,
    u32 offset_x,
    u32 offset_y)
{
    s32 byte_offset;
    s32 vram_address;
    u16 *text;
    s32 zero;
    u32 cell;
    u8 *base;
    u16 *counter;

    base = Data_03001e8c;
    counter = (u16 *)(base + 0x12B2);
    zero = 0;
    *counter = zero;
    Func_08018038(character, 1);

    byte_offset = *counter * 2 + 0xEB0;
    *(u16 *)(base + byte_offset) = zero;
    *counter = (u16)((*counter + 1) & 0x1FF);

    cell = ((position->y + (offset_y >> 3) + 1) << 5)
        + (position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = (u16 *)(base + 0xEB0);
        Func_0801de5c(
            text,
            (s32)(base + byte_offset),
            vram_address,
            7 & offset_x);
    }
}
