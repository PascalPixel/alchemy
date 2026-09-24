#include "TYPES.H"
#include "DMA.H"

struct MapLayer {
    u32 *cells;
    u8 pad04[44];
};

struct MapState {
    u8 pad000[0x130];
    struct MapLayer layers[1];
};

/* Copy the map cell at x, y of a layer to dst with DMA 3 and wait for it. */
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst)
{
    struct MapState *map = *(struct MapState **)0x03001e70;

    if (map != 0) {
        u32 *cell = map->layers[layer].cells;

        cell += x + (y << 7);
        Dma_Set(cell, dst, 0x84000001, (volatile u32 *)0x040000d4);
        {
            volatile u32 *dma = (volatile u32 *)0x040000d4;

            while (dma[2] & 0x80000000)
                ;
        }
    }
}
