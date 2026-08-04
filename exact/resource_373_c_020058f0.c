#include "types.h"

extern void Func_0200b7d4(s32, s32, s32 *);
extern void Func_0200b838(s32, s32, s32, s32);

void Func_020058f0(s32 *p, s32 a, s32 b)
{
    s32 buf[3];
    if (p != 0) {
        buf[0] = p[2];
        buf[1] = p[3];
        buf[2] = p[4];
        Func_0200b7d4(a, b, buf);
        Func_0200b838((s32)p, buf[0], buf[1], buf[2]);
    }
}
