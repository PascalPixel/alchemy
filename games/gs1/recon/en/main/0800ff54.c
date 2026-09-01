#include "types.h"

static __inline__ void CopyBlock(u32 *map, u8 *destination, u32 parity)
{
    volatile u16 *colors = (volatile u16 *)0x02020000;
    u32 index = (*map & 0xFFF) * 4 + parity;

    colors += index;
    *(u16 *)destination = *colors;
    colors += 2;
    *(u16 *)(destination + 64) = *colors;
}

void Func_0800ff54(u32 a0, s32 a1, s32 a2)
{
    u8 *destination = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 parity = a1 & 1;
    u32 counter;

    for (counter = 0; counter <= 10; counter++) {
        u32 *map = (u32 *)(0x02010000 + (row + col) * 4);
        u32 offset = (rowmod + colmod + parity) * 2;

        CopyBlock(map, destination + offset, parity);

        row = (row + 128) & 0x3F80;
        rowmod = (rowmod + 64) & 0x3C0;
    }
}
