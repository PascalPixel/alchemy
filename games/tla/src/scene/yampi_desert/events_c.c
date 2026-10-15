#include "scene.h"

void Func_02002ca0(void)
{
    Func_02004190(136, 1);
    Scene_TaskAdd(Func_02002c6c, 0x480);
    Func_02004198(21, -1);
    Func_020041b0();
    Field_DispatchTypeHandler(1);
    Func_020041a0();
    Scheduler_RemoveCallbackFar(Func_02002c6c);
    Func_020041a8();
}

void Func_02002ce0(s32 id)
{
    Func_020040f8(id);
    Object_SetPartAttribute(Object_GetById(id), 15);
    ObjectMotion_SetActionVariant(id, 1);
    Object_GetById(id)->priorityFlags |= 2;
}

void Func_02002d10(void)
{
    if (GameFlag_Test(265))
    {
        if (GameFlag_Test(0x20d) == 0)
            Func_0200163c();
    }
    else
    {
        GameFlag_SetBit(0x20d);
    }
}

void Func_02002d40(void)
{
    Func_02002ce0(11);
    Func_02002ce0(12);
    Func_02002d10();
}

void Func_02002d54(void)
{
    u32 i;

    for (i = 0; i <= 6; i++)
        Func_02002ce0(i + 9);
    Func_02002d10();
}
