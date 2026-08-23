#include "types.h"

extern s32 Func_020058aa(s32);
extern s32 Func_020058c0(s32);
extern s32 Func_020058da(s32);
extern s32 Func_020058d8(void);
extern s32 Func_020058de(void);

s32 Func_02002ba4(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_020058aa(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_020058c0(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_020058da(p[12] + 0x8000) / 8;
    a = Func_020058d8();
    b = Func_020058de();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}
