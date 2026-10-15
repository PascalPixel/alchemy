#include "local.h"

void Func_02000694(s32 a, s32 b, s32 c, s32 d);

void Func_020034bc(void)
{
    s32 i;

    GameFlag_SetBit(0x300);
    Func_02000694(0, 15, 13, 0);
    for (i = 0; i != 6; i++)
        Object_SetModeById(i + 9, 2);
    Object_SetModeById(15, 4);
}
