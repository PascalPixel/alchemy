#include "ov6a9.h"

void Func_02000bf8(s32 unused, s32 id)
{
    struct FieldActor *object = Object_GetById(id);

    object->motion_flags = 0;
    Call6((void (*)())Func_020010ac, 13, 12, 2, 1, 13, 10);
    Call6((void (*)())Func_020010ac, 16, 10, 1, 1, object->x >> 20, 10);
}
