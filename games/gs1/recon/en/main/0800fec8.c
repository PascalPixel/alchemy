#include "types.h"

void Func_0800fec8(u32 a0, s32 a1, s32 a2)
{
    s32 col = (a1 / 2) & 0x7F;
    s32 colmod = a1 & 30;
    u32 counter = 0;

    do {
        u32 cell = *(u32 *)(0x02010000 + ((((a2 / 2) & 0x7F) << 7) + col) * 4);
        u8 *dest = (u8 *)((0x06002800 + (a0 << 11)) + (((a2 & 30) << 5) + colmod) * 2);
        u32 offset = (u32)(cell << 20) >> 17;

        *(u32 *)dest = *(u32 *)(0x02020000 + offset);
        *(u32 *)(dest + 64) = *(u32 *)(0x02020004 + offset);

        col = (col + 1) & 0x7F;
        colmod = (colmod + 2) & 30;
        counter++;
    } while (counter <= 15);
}
