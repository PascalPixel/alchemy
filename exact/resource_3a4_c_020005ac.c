#include "types.h"

extern s32 *Func_0200414c(s32);
extern void Func_020040f2(s32, s32, s32, s32, s32, s32);
extern void Func_02004106(s32 *, s32);
extern void Func_02004146(s32);

void Func_020005ac(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200414c(9);
    s0 = 43;
    s1 = 41;
    Func_020040f2(45, 41, 1, 1, s0, s1);
    if (p != 0) {
        Func_02004106(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004146(0x200);
}
