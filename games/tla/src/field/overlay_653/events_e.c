#include "local.h"

s32 Func_02000824(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    s32 left = b->x - 0xc0000;
    s32 right = b->x + 0xc0000;

    if (a->x > left && a->x < right && a->z > b->z - 0x80000 && a->z < b->z + 0x80000)
        return 1;
    return 0;
}
