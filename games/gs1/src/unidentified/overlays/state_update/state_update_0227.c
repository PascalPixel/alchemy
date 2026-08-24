#include "types.h"

s32 Func_02007d28(u32, s32);
void Func_02007e10(s32, s32);
s32 Func_02007d38(u32, s32);
void Func_0200434e(s32);

void Func_0200211c(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Func_02007e10(a, Func_02007d28(*p >> 1, 6));
    }
    if (Func_02007d38(*p, 15) == 0) {
        Func_0200434e(a);
    }
}
