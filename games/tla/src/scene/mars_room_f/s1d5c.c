#include "room.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

s32 Func_02001d5c(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 && ABS(a->z - b->z) < 0x80000)
        return 1;
    return 0;
}

s32 Func_02001dc0(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    s32 dy;

    if (ABS(a->x - b->x) < 0xc0000)
    {
        dy = a->y - b->y;
        if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)
            return 1;
    }
    return 0;
}
