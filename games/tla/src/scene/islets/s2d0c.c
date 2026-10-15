#include "islets.h"

void Func_02002d0c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->unknown_5c[0] = 3;
    object->motion_flags = 0;
    object->priority_flags |= 0x80;
    object->acceleration = 0x2666;
    object->speed_limit = 0x4ccc;
}

void Func_02002d48(s32 id, s32 column, s32 row)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x = (column * 32 + object->action) << 16;
    s32 z = (row * 32 + (s16)object->unknown_66) << 16;

    Func_02003e0c(object, x, object->y, z);
}
