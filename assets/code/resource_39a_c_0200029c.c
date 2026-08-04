#include "types.h"

extern void Func_020026b6(s32);
extern void Func_02000430(s32, s32, s32);
extern void Func_0200043a(s32, s32, s32);
extern void Func_020026d0(s32);
extern void Func_020025ee(s32);
extern void Func_0200252c(s32);
extern void Func_02001320(void);

void Func_0200029c(void)
{
    s32 offset = 112;

    offset = -offset;
    Func_020026b6(0xF1);
    Func_02000430(8, offset, 0);
    Func_0200043a(8, offset, 0);
    Func_020026d0(0x121);
    Func_020025ee(0x301);
    Func_0200252c(2);
    Func_02001320();
}
