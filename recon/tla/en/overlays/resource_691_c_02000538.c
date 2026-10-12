/* Draft, differ 22 of 43: the native keeps the width in r5, the value in r6 and the map in r4 and reads x and z from their argument registers; this form copies x to r6 and z to r1 first. */
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapCellBuffer[];

struct MapLayer {
    u8 *cells;
    u8 unknown_04[52];
};

struct MapLayers {
    u8 unknown_000[0x138];
    struct MapLayer layers[3];
};

s32 Func_02000538(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
    struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->map_work;
    u8 *cells;
    u8 *cell;
    u32 row;
    u32 column;

    if (map != 0) {
        if (layer <= 2)
            cells = map->layers[layer].cells;
        else
            cells = gMapCellBuffer;
        cells += (x + (z << 7)) << 2;
        for (row = 0; row < height; row++) {
            cell = cells + (row << 9);
            for (column = 0; column < width; column++) {
                cell[2] = value;
                cell += 4;
            }
        }
    }
    return 0;
}
