#include "local.h"

s32 Func_020023a8(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    if (DIFF(a->x, b->x) < 0xc0000) {
        if ((u32)(a->y - b->y - 1) <= 0xffffe) {
            if (DIFF(a->z, b->z) < 0x80000)
                return 1;
        }
    }
    return 0;
}
