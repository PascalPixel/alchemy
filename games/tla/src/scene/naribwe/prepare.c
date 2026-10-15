#include "local.h"

void Func_02000400(void)
{
    Func_0200054c();
    Func_020005c4(0);
    GameFlag_SetBit(0x201);
    Func_02000554();
}

void Func_0200041c(void)
{
    u8 *work = Ram_HeapSlots->eventWork;

    work[0xd31] = 0;
    work[0xd32] = 24;
    work[0xd33] = 25;
}

void Func_02000448(void)
{
    Func_02000574_(27, 0, 0);
    Func_020005bc_(65, 0x680000, 0x680000);
}

void Func_02000464(void)
{
    Func_020005bc_(65, 0, 0);
    Func_02000574_(27, 0x680000, 0x680000);
}

s32 Func_02000480(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(23), 0);
    Object_GetById(23)->scaleY = 0x19999;
    Object_GetById(27)->scaleX = 0x8000;
    Object_GetById(27)->scaleY = 0x8000;
    Func_02000464();
    if (GameFlag_Test(0x201))
        Func_02000574(23, 0, 0);
    Func_020005cc();
    Func_020005d4(0, 0x202, 24, 25);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(24), 0);
    Func_0200059c(25);
    return 0;
}

s32 Scene_PrepareMap(void)
{
    return 0;
}
