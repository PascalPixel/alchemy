#include "DMA.H"



struct MapLayer {
    u32 *cells;
    u8 pad04[44];
};

struct MapState {
    u8 pad000[0x130];
    struct MapLayer layers[1];
};

/* Copy one map cell word of the given layer at (x, y) to dst with DMA 3 and
 * wait for the transfer to finish. */
/* NONMATCHING: 80 of 80 bytes, 38 halfword edits (2026-09-24). Remaining:
 * the map pointer should load into r2 with y moved to r4, and the wait loop
 * should reload the DMA base and read its control word at offset 8. */
void Func_02000fcc(s32 layer, s32 x, s32 y, u32 *dst)
{
    struct MapState *map;

    map = *(struct MapState **)0x03001e70;
    if (map != 0) {
        Dma_Set(map->layers[layer].cells + (x + (y << 7)), dst, 0x84000001, (volatile u32 *)0x040000d4);
        while (((volatile u32 *)0x040000d4)[2] & 0x80000000)
            ;
    }
}
