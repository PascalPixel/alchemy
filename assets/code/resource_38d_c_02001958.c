#include "types.h"

extern void Func_02003ec4(s32, s32, s32);
extern void Func_02003ed0(s32, s32, s32);
extern void Func_02003edc(s32, s32, s32);

void Func_02001958(void)
{
    Func_02003ec4(1, 0xE000, 0);
    Func_02003ed0(2, 0xA000, 0);
    Func_02003edc(3, 0x8000, 0);
}
