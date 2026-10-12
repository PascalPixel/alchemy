#include "ov67f.h"

s32 Func_02000eac(u32 layer, s32 x, s32 y)
{
    struct Ov67fMapWork *map = (struct Ov67fMapWork *)Ram_HeapSlots->map_work;
    u8 *cell;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    return (cell[2] << 8) | cell[3];
}

s32 Func_02000ee8(u32 layer, s32 x, s32 y, s32 value)
{
    struct Ov67fMapWork *map = (struct Ov67fMapWork *)Ram_HeapSlots->map_work;
    u8 *cell;
    s32 old;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    old = (cell[2] << 8) | cell[3];
    cell[2] = value >> 8;
    cell[3] = value;
    return old;
}
