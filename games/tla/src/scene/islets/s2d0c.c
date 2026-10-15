#include "islets.h"

void Func_02002d0c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->unk5c[0] = 3;
    object->motionFlags = 0;
    object->priorityFlags |= 0x80;
    object->acceleration = 0x2666;
    object->speedLimit = 0x4ccc;
}

void Func_02002d48(s32 id, s32 column, s32 row)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32                   x = (column * 32 + object->action) << 16;
    s32                   z = (row * 32 + (s16)object->unk66) << 16;

    Func_02003e0c(object, x, object->y, z);
}
