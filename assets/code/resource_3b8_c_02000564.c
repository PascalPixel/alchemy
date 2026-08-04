#include "types.h"

extern u8 Value_000022ab;
extern void Func_02004a06(s32);
extern s32 Func_02004a16(s32, s32);
extern s32 Func_02004976(s32, s32);
extern void Func_02004a20(s32);
extern void Func_02004a28(s32);
extern s32 Func_02004a40(s32, s32);

void Func_02000564(s32 a)
{
    s32 k = (s32)&Value_000022ab;

    Func_02004a06(k);
    Func_02004a16(a, 0);
    if (Func_02004976(0, 0) == 0)
        Func_02004a20(k + 1);
    else
        Func_02004a28(k + 2);
    Func_02004a40(a, 0);
}
