#include "types.h"

extern u8 *Func_02007ebe(s32);
extern void Func_02007e72(u8 *, s32);
extern void Func_02007e0a(u8 *, s32);
extern void Func_02007fa2(s32, s32);

void Func_0200300c(s32 arg0)
{
    u8 *p;
    s32 m;

    p = Func_02007ebe(arg0);
    p[0x55] = 0;
    m = 252;
    m &= p[0x59];
    p[0x59] = m;
    Func_02007e72(p, 0);
    Func_02007e0a(p, 5);
    Func_02007fa2(arg0, 3);
    {
        s32 v = 2;
        v |= p[0x23];
        p[0x23] = v;
    }
}
