#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_0808b3d0(s32 arg0, s32 arg1) {
    s32 var_r0;

    var_r0 = arg0;
    if ((var_r0 <= 8) && (arg1 != 0)) {
        if (var_r0 == 0) {
            var_r0 = 0x12;
        }
        if (var_r0 == 1) {
            var_r0 = 0x13;
        }
    }
    return var_r0;
}
