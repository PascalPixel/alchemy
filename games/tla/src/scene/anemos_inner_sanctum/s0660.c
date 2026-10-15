#include "types.h"
#include "object_runtime.h"

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);

s32 Func_02000660(struct ObjectRuntime *object)
{
    s32 radius;

    object->action += object->unk66;
    object->x = object->bounce;
    object->y = object->gravity;
    object->z = 0x980000;
    Vector_AddPolarOffsetFar(object->unk4c, object->action, &object->x);
    radius = object->unk4c - 0x10000;
    object->unk4c = radius;
    if (radius)
        return 1;
    return 0;
}
