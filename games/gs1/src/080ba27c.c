#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);
s32 Func_080bb65c();
s32 Func_080bb938();
s32 Func_080bbb0c(void *, s32);
s32 Func_080c10e8(s32, s32);

s32 Func_080ba27c(void *arg0) {
    s32 var_r5;
    s8 temp_r3;

    Func_080c10e8(0, 0);
    temp_r3 = M2C_FIELD(arg0, s8 *, 1);
    if (temp_r3 == 0) {
        Func_080bb65c();
    } else {
        var_r5 = 0;
        if (var_r5 < (s32) temp_r3) {
            do {
                Func_080bbb0c(arg0, var_r5);
                Func_080bb938();
                var_r5 += 1;
            } while (var_r5 < (s32) M2C_FIELD(arg0, s8 *, 1));
        }
    }
    return Func_080030f8(1);
}
