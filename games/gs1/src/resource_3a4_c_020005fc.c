#include "types.h"

extern s32 *Func_0200419c(s32);
extern void Func_02004142(s32, s32, s32, s32, s32, s32);
extern void Func_02004156(s32 *, s32);
extern void Func_02004194(s32);

void Func_020005fc(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200419c(11);
    s0 = 17;
    s1 = 10;
    Func_02004142(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Func_02004156(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004194(0x201);
}
