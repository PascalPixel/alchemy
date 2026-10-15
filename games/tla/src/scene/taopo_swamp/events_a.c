#include "local.h"

void Func_02000104(s32 a, s32 b)
{
    GameFlag_SetBit(b + 0x8d8);
}

void Func_02000118(void)
{
    Func_02002840_(50, 16, 1, 2, 47, 16);
    GameFlag_SetBit_(0x220);
}
