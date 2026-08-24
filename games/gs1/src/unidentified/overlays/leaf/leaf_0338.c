#include "types.h"

extern void Func_02001240(void);
extern s32 Func_0200122e(s32);
extern void Func_020012a8(s32);
extern void Func_020012b0(s32);
extern void Func_020012c8(s32, s32);
extern void Func_0200126c(void);

void Func_0200085c(void)
{
    Func_02001240();
    if (Func_0200122e(0x8BE) == 0) {
        Func_020012a8(0x23B3);
    } else {
        Func_020012b0(0x23B4);
    }
    Func_020012c8(25, 0);
    Func_0200126c();
}
