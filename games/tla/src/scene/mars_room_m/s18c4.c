#include "mars_room_m.h"

s32 Func_020018c4(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0x80000
        && (a->y - b->y >= 0 ? a->y - b->y : b->y - a->y) < 0x80000
        && (a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
        return 1;
    return 0;
}

s32 Func_02001928(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    s32 height;

    if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000)
    {
        height = a->y - b->y;
        if (height > -0x80000 && height < 0x180000)
        {
            if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0xc0000)
                return 1;
        }
    }
    return 0;
}
