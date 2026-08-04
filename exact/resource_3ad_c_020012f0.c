#include "types.h"

extern void Func_02002e9a(s32);
extern s32 Func_02002eaa(s32, s32);
extern s32 Func_02002e3a(s32, s32);

s32 Func_020012f0(void)
{
    Func_02002e9a(0x254D);
    Func_02002eaa(1, 0);
    return Func_02002e3a(0, 0) == 0;
}
