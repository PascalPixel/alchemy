#include "room.h"

void Func_020009ec(void)
{
    s32 value = 65;

    /* FAKEMATCH: ⚓️ builds the value before the index */
    asm volatile("" : "+l"(value));
    Func_020027ac_(2, value);
    value = 65;
    /* FAKEMATCH: ⚓️ builds the value before the index */
    asm volatile("" : "+l"(value));
    Func_020027ac_(1, value);
    Func_020027ac_(0, 65);
    Flag_Clear(0x350);
    if (Object_GetById(10)->z >> 20 != 15 && Object_GetById(9)->z >> 20 != 15) {
        Func_020027ac_(1, 33);
        return;
    }
    if (Object_GetById(9)->z >> 20 != 13) {
        Func_020027ac_(0, 17);
        return;
    }
    if (Object_GetById(8)->x >> 20 != 37) {
        Func_020027ac_(2, 49);
        return;
    }
    Flag_Set(0x350);
    if (!Flag_IsSet(0x351)) {
        Func_020055d0(0x20000, 0x4000);
        Camera_SetBounds(0x2680000, -1, 0x180000, 1);
        Battle_WaitMode0(60);
        Func_02005660();
        Func_020055f0(9);
    }
}

void Func_02000aac(void)
{
    s32 value = 3;

    /* FAKEMATCH: ⚓️ builds the value before the index */
    asm volatile("" : "+l"(value));
    Func_020027ac_(3, value);
    Func_020055d0(0x10000, 0x2000);
    Camera_SetBounds(0xb80000, -1, 0x2300000, 1);
}

void Func_02000ad8(void)
{
    struct SceneWork *work = Ram_HeapSlots->event_work;

    Flag_Set(0x106);
    Func_02005560();
    Func_02005648(0);
    work->unknown_166 = 0;
    Flag_Clear(0x106);
    Flag_Clear(0x104);
    Battle_WaitMode0(60);
    Func_020055d0(0x30000, 0x6000);
    Camera_SetBounds(0xb80000, -1, 0x2e80000, 1);
    Func_020055e0();
    Battle_WaitMode0(60);
    Func_02005568();
}
