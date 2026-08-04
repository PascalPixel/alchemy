#include "types.h"

extern void Func_0200899c(s32, s32);
extern void Func_020089a6(s32, s32);
extern s32 Func_0200891e(s32, s32);
extern void Func_02008710(s32);

void Func_020042bc(s32 o)
{
    s32 v;

    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Func_0200899c(o, 7);
    } else {
        Func_020089a6(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)0x03001e40;
        v = (*(s32 *)0x0200d7fc << 3) + 16;
        if (Func_0200891e(*q, v) == 0) {
            Func_02008710(o);
        }
    }

}
