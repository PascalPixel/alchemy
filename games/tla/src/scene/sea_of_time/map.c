#include "sea_of_time.h"

s32 Func_0200033c(struct SeaOfTimeObject *object)
{
    object->sprite->visible = 0;
    object->layer = 1;
    return 1;
}

s32 Func_0200034c(u32 layer, s32 x, s32 y)
{
    struct SeaOfTimeMapWork *map = (struct SeaOfTimeMapWork *)Ram_HeapSlots->mapWork;
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

s32 Func_02000388(u32 layer, s32 x, s32 y, s32 value)
{
    struct SeaOfTimeMapWork *map = (struct SeaOfTimeMapWork *)Ram_HeapSlots->mapWork;
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

s32 Func_020003d0(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Iwram_Sqrt(dx * dx + dz * dz);
}
