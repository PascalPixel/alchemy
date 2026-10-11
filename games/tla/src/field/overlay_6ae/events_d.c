#include "ov6ae.h"

void Func_020035ec(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectMotion_SetActionVariant(id, 1);
    Object_SetPositionAndResetMotion(object, object->x - 0x100000, object->y, object->z + 0x200000);
}
