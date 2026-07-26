#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_08003fa4(s32, s32, s32);
s32 Func_08004080();

s32 Func_080209b0(void) {
    s32 temp_r0;

    temp_r0 = Func_08004080();
    Func_08003fa4(temp_r0, 0x80, 0x080310A4);
    return temp_r0;
}
