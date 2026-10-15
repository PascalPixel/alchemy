#include "airs_rock_inside.h"

s32 Func_02000690(u32 layer, s32 x, s32 y)
{
    struct AirsRockInsideMapWork *map = (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
    u8                           *cell;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    return (cell[2] << 8) | cell[3];
}
