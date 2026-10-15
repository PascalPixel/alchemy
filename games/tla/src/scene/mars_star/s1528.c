#include "mars_star.h"

s32 Func_02001528(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    if (DIFF(a->x, b->x) < 0x80000)
    {
        if (DIFF(a->y, b->y) < 0x80000)
        {
            if (DIFF(a->z, b->z) < 0x80000)
                return 1;
        }
    }
    return 0;
}

s32 Func_0200158c(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    if (DIFF(a->x, b->x) < 0xc0000)
    {
        if ((u32)(a->y - b->y + 0x7ffff) <= 0x1ffffe)
        {
            if (DIFF(a->z, b->z) < 0xc0000)
                return 1;
        }
    }
    return 0;
}
