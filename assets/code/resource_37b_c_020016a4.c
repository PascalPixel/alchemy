#include "types.h"
#define NULL ((void *)0)

s32 *Func_02003b16(s32);
s32 *Func_02003b20(s32);
void Func_02003af8(s32, s32, s32, s32);

void Func_020016a4(void) {
    s32 v = Func_02003b16(0)[2] >> 20;
    if ((Func_02003b20(0)[4] >> 20) == 7 && (u32)(v - 21) <= 1) {
        Func_02003af8(2, 0x1600000, 0x700000, 255);
    }
}
