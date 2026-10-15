#include "types.h"

void GameFlag_ClearBit(s32 flag);
void Func_020000bc(s32 id, s32 slot, s32 mode);

void Func_02000054(void)
{
    Func_020000bc(8, 0, 0);
    GameFlag_ClearBit(48);
}

void Func_02000068(void)
{
    Func_020000bc(9, 1, 0);
    GameFlag_ClearBit(68);
}

void Func_0200007c(void)
{
    Func_020000bc(10, 2, 0);
    GameFlag_ClearBit(88);
}

void Func_02000090(void)
{
    Func_020000bc(11, 3, 0);
    GameFlag_ClearBit(108);
}
