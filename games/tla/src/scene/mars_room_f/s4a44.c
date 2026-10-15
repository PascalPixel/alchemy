#include "room.h"

void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
    struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->map_work;
    u8 *cells = map->layers[layer].cells;
    u32 row;
    u32 column;

    cells += (x + (z << 7)) << 2;
    for (row = 0; row < height; row++) {
        u8 *cell = cells + (row << 9);

        for (column = 0; column < width; column++) {
            cell[2] = value;
            cell += 4;
        }
    }
}
