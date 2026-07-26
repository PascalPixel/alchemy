#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_08009080(s32, s32);
s32 Func_08009158(s32);
s32 Func_0808ba1c();
s32 Func_080922c4(s32, s32, s32);

void Func_08092304(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r7;

    temp_r7 = Func_0808ba1c();
    Func_080922c4(arg0, arg1, arg2);
    if (temp_r7 != 0) {
        Func_08009158(temp_r7);
        Func_08009080(temp_r7, 1);
    }
}
