#include "types.h"

extern s32 *Func_020041f0(s32);
extern void Func_02004196(s32, s32, s32, s32, s32, s32);
extern void Func_020041aa(s32 *, s32);
extern void Func_020041ea(s32);

void Func_02000650(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_020041f0(12);
    s0 = 26;
    s1 = 15;
    Func_02004196(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Func_020041aa(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020041ea(0x204);
}
