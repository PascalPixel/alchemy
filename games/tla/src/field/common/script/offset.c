#include "script.h"
#include "object_runtime.h"

void Object_SetMoveTarget(void *, s32, s32, s32);

s32 Script_ApplyLocalOffsetPosition(struct ObjectRuntime *object)
{
    s32 offset[3];

    Object_SetMoveTarget(
        object, object->x + offset[0], object->y + offset[1], object->z + offset[2]);
    object->step += 3;
    return 1;
}
