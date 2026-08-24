#include "types.h"

extern s32 Func_02008998(s32, s32);
extern void Func_02008a38(s32, s32);

void Func_0200434c(s32 o)
{
    volatile s32 *p = (s32 *)0x03001e40;
    s32 t;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Func_02008a38(o, Func_02008998(t, 6));
    }
}
