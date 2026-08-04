#include "types.h"

void Func_0800ff54(u32 block, s32 x, s32 y)
{
    u16 *destination;
    u32 row;
    u32 column;
    u32 source_column;
    u32 source_row;
    u32 parity;
    u32 row_mask;
    u32 i;
    s32 adjusted;

    destination = (u16 *)(0x06002800 + (block << 11));
    adjusted = y + ((u32)y >> 31);
    source_row = ((adjusted >> 1) & 127) << 7;
    source_column = (y & 30) << 5;
    adjusted = x + ((u32)x >> 31);
    row = (adjusted >> 1) & 127;
    column = x & 30;
    parity = x & 1;
    row_mask = 0x3f80;
    i = 0;

    do {
        u32 descriptor;
        u32 offset;
        u16 *target;

        descriptor = ((u32 *)0x02010000)[source_row + row];
        offset = (descriptor << 20) >> 18;
        target = (u16 *)((u8 *)destination
            + ((source_column + column + parity) << 1));
        target[0] = *(u16 *)(0x02020000 + ((offset + parity) << 1));
        target[32] = *(u16 *)(0x02020004 + ((offset + parity) << 1));
        source_row += 128;
        source_column += 64;
        i++;
        source_row &= row_mask;
        source_column &= 960;
    } while (i <= 10);
}
