#include "local.h"

s32 Func_02002c1c(struct ObjectRuntime *object)
{
    u8 *steps;
    s32 v;
    u8  n;

    object->x += object->bounce;
    object->y += object->unk4c;
    object->z += object->gravity;
    steps = (u8 *)object + 98;
    v = *steps + 255;
    *steps = v;
    n = v;
    return n != 0;
}
