#include "types.h"

extern void Func_02004a84(s32, s32);
extern void Func_02004aa4(s32, s32);
extern void Func_02004a94(s32, s32);
extern u8 *Func_02004a62(s32);
extern void Func_02004a42(s32);

void Func_02001938(void)
{
    u8 *flags;

    Func_02004a84(21, 1);
    Func_02004aa4(21, 0);
    Func_02004a94(21, 2);
    flags = Func_02004a62(21) + 35;
    *flags &= 0xFD;
    Func_02004a42(0x201);
}
