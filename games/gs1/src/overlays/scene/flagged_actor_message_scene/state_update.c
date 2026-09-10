#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_ApplyValues14And0And5 Func_02000030
#define SceneState_SetRuntimeWord448To521AndSend303 Func_02000ad4

extern u8 *Data_03001ebc;

void Func_020016ca(s32, s32, s32);
void Func_02002048(s32);

void SceneState_ApplyValues14And0And5(void)
{
    Func_020016ca(0xE, 0, 5);
}

void SceneState_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = Data_03001ebc;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    Func_02002048(0x12F);
}
