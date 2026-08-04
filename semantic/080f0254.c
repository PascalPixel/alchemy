#include "types.h"

void Func_080f0254(u32 page)
{
    u32 value;
    u32 *dma;
    u32 fill;
    u32 tiles;
    u32 palette;

    if (page == 0) {
        fill = 0x01010101;
        tiles = 0x06000000;
        palette = 0x05000000;
    } else {
        fill = 0x81818181;
        tiles = 0x06008000;
        palette = 0x05000100;
    }

    value = fill;
    dma = (u32 *)0x040000d4;
    dma[0] = (u32)&value;
    dma[1] = tiles;
    dma[2] = 0x85001e00;

    value = 0;
    dma = (u32 *)0x040000d4;
    dma[0] = (u32)&value;
    dma[1] = palette;
    dma[2] = 0x85000040;
}
