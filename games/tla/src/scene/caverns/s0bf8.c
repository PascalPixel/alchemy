#include "caverns.h"

void Func_02000ba8(void)
{
    struct FieldActor *object;
    s32                arg;

    Object_GetById(4)->priorityFlags |= 0x20;
    Func_0200114c();
    Func_02001154(0, 0x200, 8, 9);
    object = Object_GetById(10);
    object->motionFlags = 0;
    /* FAKEMATCH: the empty one-pass block ends CSE's block after the store, so the last argument's 10 is built before object->x is loaded (differ 4 without it, 8 without the shared arg). */
    do
    {
    } while (0);
    arg = 10;
    Func_020010ac(16, arg, 1, 1, object->x >> 20, arg);
}

void Func_02000bf8(s32 unused, s32 id)
{
    struct FieldActor *object = Object_GetById(id);

    object->motionFlags = 0;
    Call6((void (*)())Func_020010ac, 13, 12, 2, 1, 13, 10);
    Call6((void (*)())Func_020010ac, 16, 10, 1, 1, object->x >> 20, 10);
}
