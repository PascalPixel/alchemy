#include "local.h"

void Func_02001ca0(s32 a, s32 b)
{
    struct ObjectRuntime **work = Ram_HeapSlots->effect_work;

    work[5] = Object_GetById(9);
    Func_02001190(a, b);
    GameFlag_SetBit(0x350);
}

void Func_02001cd0(s32 a, s32 b)
{
    struct ObjectRuntime **work = Ram_HeapSlots->effect_work;

    work[5] = Object_GetById(11);
    Func_02001190(a, b);
    GameFlag_SetBit(0x351);
}
