#include "trial_road.h"

void Func_02000cf8(s32 id, s32 side);

void Func_02000d70(void)
{
    Map_CopyCells(11, 64, 11, 8, 11, 22);
    Map_CopyCells(79, 64, 11, 8, 79, 22);
    Func_02000cf8(16, 0);
    Func_02000cf8(17, 0);
    Func_02000cf8(18, 1);
    if (GameFlag_Test(0x253))
        Map_CopyCells(0, 48, 1, 3, 19, 23);
    if (GameFlag_Test(0x254))
        Map_CopyCells(0, 48, 1, 3, 19, 26);
}

s32 Func_02000df8(s32 x, s32 z)
{
    struct ObjectRuntime    *object = Object_GetById(15);
    struct TrialRoadMapWork *map = (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
    u8                      *cells;
    u8                      *cell;
    s32                      value;

    if (object->x >> 20 == x && object->z >> 20 == z)
        return 1;
    cells = map->layers[0].cells;
    cell = cells + ((x + (z << 7)) << 2);
    value = cell[2];
    return IsZero(value ^ 0xff);
}
