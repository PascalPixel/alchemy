#include "ride.h"

struct MapLayer
{
    u8  unk00[0x30];
    u8 *cells;
    u8  unk34[4];
};

struct MapWork
{
    u8              unk000[0x108];
    struct MapLayer layers[3];
};

extern u8 gMapCellBuffer[];

s32 Func_0200012c(u32 layer, s32 x, s32 y)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    u8             *cell;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    return (cell[2] << 8) | cell[3];
}

s32 Func_02000168(u32 layer, s32 x, s32 y, s32 value)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    u8             *cell;
    s32             old;

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
