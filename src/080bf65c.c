#include "types.h"

s32 Func_080bf5a8();

s32 Func_080bf65c(void) {
    s32 var_r5;

    var_r5 = 0x13;
    do {
        var_r5 -= 1;
        Func_080bf5a8();
    } while (var_r5 >= 0);
    return 0;
}
