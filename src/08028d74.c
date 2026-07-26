#include "types.h"

void *Func_080284dc(void);
void Func_0802851c(void);
s32 Func_08028574(s32);
void Func_080287a8(s32 arg0);
s32 Func_08028808(s32, s32, s32);

s32 Func_08028d74(s32 arg0) {
    s32 temp_r5;

    Func_080284dc();
    Func_080287a8(0x11);
    Func_080287a8(0x12);
    Func_080287a8(0x13);
    Func_080287a8(0x14);
    Func_08028808(0x11, 7, 0);
    temp_r5 = Func_08028574(arg0);
    Func_0802851c();
    return temp_r5;
}
