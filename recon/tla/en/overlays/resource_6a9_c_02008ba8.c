/*
 * Draft of Func_02000ba8 (overlay 6A9, 02008ba8). differ 4: the object's x
 * is loaded after the second argument, where ⚓️ loads it before. With
 * a direct call instead of call.h's Call6 the two tens swap r2 and r3
 * (differ 4 as well).
 */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

void Func_02000ba8(void)
{
    struct FieldActor *object;
    s32 x;

    Object_GetById(4)->priority_flags |= 0x20;
    Func_0200114c();
    Func_02001154(0, 512, 8, 9);
    object = Object_GetById(10);
    object->motion_flags = 0;
    x = object->x >> 20;
    Call6((void (*)())Func_020010ac, 16, 10, 1, 1, x, 10);
}
