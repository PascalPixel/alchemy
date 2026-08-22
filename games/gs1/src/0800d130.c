#include "object_runtime.h"

void Func_0800d130(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    object->z = z;
    object->target_x = 0x80000000;
    object->target_y = 0x80000000;
    object->target_z = 0x80000000;
    object->x = x;
    object->y = y;
    object->velocity_x = 0;
    object->velocity_y = 0;
    object->velocity_z = 0;
}
