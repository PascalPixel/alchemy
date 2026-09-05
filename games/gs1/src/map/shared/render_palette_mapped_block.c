#include "types.h"

#define Map_RenderPaletteMappedBlock Func_0800ff54

static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod, u32 parity)
{
    volatile u16 *colors;
    u8 *destination;
    u32 index = ((*map << 20) >> 18) + parity;

    colors = (volatile u16 *)0x02020000;
    colors += index;
    destination = base + (rowmod + colmod + parity) * 2;
    *(u16 *)destination = *colors;
    colors = (volatile u16 *)0x02020004;
    colors += index;
    *(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedBlock(u32 a0, s32 a1, s32 a2)
{
    u8 *destination = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 parity = a1 & 1;
    u32 counter;

    for (counter = 0; counter <= 10; counter++) {
        u32 *map = (u32 *)0x02010000;

        map += row + col;
        CopyBlock(map, destination, rowmod, colmod, parity);

        row = (row + 128) & 0x3F80;
        rowmod = (rowmod + 64) & 0x3C0;
    }
}
