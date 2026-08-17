#include "types.h"

struct DmaTransfer_080f0538 {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_080f0538(void)
{
    s32 phase;
    s32 remainder;
    s32 y;
    u32 tile;
    s32 column;
    u32 x;
    s32 row;
    u8 *entry;
    volatile struct DmaTransfer_080f0538 *dma;

    remainder = (*(volatile u16 *)0x02004c00) & 7;
    phase = (s16)(*(volatile u16 *)0x02004c00);
    if (phase < 0)
        phase += 7;
    tile = (phase >> 3) & 31;
    tile *= 24;
    entry = *(u8 **)0x02004c0c + 0xc0;
    y = -remainder + 16;

    row = 0;
    do {
        x = 24 << 16;
        column = 5;

        do {
            u32 *words = (u32 *)entry;

            *words++ = (u32)y | x | 0x40004000;
            *words = tile;
            tile += 4;
            entry += 8;
            if (tile == 0x300)
                tile = 0;
            column--;
            x += 32 << 16;
        } while (column >= 0);
        row++;
        y += 8;
    } while (row <= 15);

    dma = (volatile struct DmaTransfer_080f0538 *)0x040000d4;
    {
        u32 source;
        u32 destination;
        u32 control;

        source = *(u32 *)0x02004c0c;
        destination = 0x07000000;
        control = 0x84000100;
        *dma = (struct DmaTransfer_080f0538){
            source,
            destination,
            control,
        };
    }

    if (*(volatile s16 *)0x02004c04 == 0 &&
        (*(volatile u32 *)0x03001800 & 3) == 0)
        (*(volatile u16 *)0x02004c00)++;
}
