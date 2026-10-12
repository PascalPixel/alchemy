#include "ov656.h"

void Func_02001dc8(s32 id)
{
    struct Ov656Object *object = Object_GetById(id);

    Object_GetById(id)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(id, 1);
    object->motion_flags = 0;
    object->y = -0x200000;
    object->height = -0x200000;
}
