#include "local.h"

void Func_020010f4(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->priorityFlags = 0;
    object->motionFlags = 0;
    Func_02002960_(42, 32, 1, 1, 44, 32);
    GameFlag_SetBit(0x221);
}

void Func_0200112c(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->priorityFlags = 0;
    object->motionFlags = 0;
    Func_02002960_(48, 32, 1, 1, 46, 32);
    GameFlag_SetBit(0x222);
}

void Func_02001164(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->priorityFlags = 0;
    object->motionFlags = 0;
    Func_02002960_(44, 44, 1, 1, 46, 44);
    GameFlag_SetBit(0x223);
}

void Func_0200119c(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32                   column = object->x >> 20;

    if (column == 53)
    {
        WaitFrames(30);
        object->z += 0x10000;
        GameFlag_SetBit(0x951);
        Func_02002960(54, 58, 1, 1, column, 52);
    }
}

void Func_020011e0(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32                   column = object->x >> 20;

    if (column == 53)
    {
        WaitFrames(30);
        object->z += 0x10000;
        GameFlag_SetBit(0x952);
        Func_02002960(54, 58, 1, 1, column, 58);
    }
}
