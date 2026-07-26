#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080045a4(s32 arg0) {
    s32 temp_r2;
    s32 var_r0;
    s32 var_r4;
    s32 var_r5;

    var_r0 = arg0;
    var_r5 = 0;
    var_r4 = 0xF;
    do {
        temp_r2 = (var_r5 << (var_r4 + 1)) + (1 << (var_r4 * 2));
        if (temp_r2 <= var_r0) {
            var_r5 |= 1 << var_r4;
            var_r0 -= temp_r2;
        }
        var_r4 -= 1;
    } while (var_r4 >= 0);
    return var_r5;
}
