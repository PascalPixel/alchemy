#include "islets.h"

void Func_02001278(struct ObjectRuntime *object)
{
    struct ObjectRuntime *source = Object_GetById(9);

    Object_SetPositionAndResetMotion(object, source->x, source->y + 0x100000, source->z);
    object->angle = source->angle;
}
