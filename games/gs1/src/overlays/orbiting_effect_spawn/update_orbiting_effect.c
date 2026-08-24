#include "types.h"

extern s32 Func_02003122(s32);
extern s32 Func_02003138(s32);
extern s32 Func_02003152(s32);
extern s32 Func_02003150(void);
extern s32 Func_02003156(void);

s32 Func_020017d8(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_02003122(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_02003138(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_02003152(p[12] + 0x8000) / 8;
    a = Func_02003150();
    b = Func_02003156();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}
