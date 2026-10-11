#include "ov6ae.h"

void Func_0200298c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->collision_flags = 0;
    object->motion_flags = 0;
    ObjectMotion_SetActionVariant(id, 1);
}
