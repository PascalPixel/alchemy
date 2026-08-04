#include "types.h"

extern s32 Value_00000f2e;

void Func_02006736(s32, s32, s32);
void Func_02006754(s32);
void Func_020067d0(s32, s32, s32);
void Func_020066f6(s32);
void Func_0200677e(s32, s32);
void Func_02006866(s32, s32);
void Func_0200678e(s32, s32);
void Func_0200672e(s32, s32);
void Func_0200670c(s32);
void Func_0200679e(s32, s32, s32);

void Func_020010a8(void)
{
    Func_02006736(0, 0x108, 0x318);
    Func_02006754(0);
    Func_020067d0(0, 0x4000, 0);
    Func_020066f6(10);
    Func_0200677e(0, 1);
    Func_02006866(0xea, 3);
    Func_0200678e(0, 1);
    Func_0200672e(0xea, 0);
    Func_0200670c((s32)&Value_00000f2e);
    Func_0200679e(8, 0, 0);
}
