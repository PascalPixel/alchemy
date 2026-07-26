#include "types.h"

s32 Func_080773d8(s32 arg0) {
    u32 var_r0;

    var_r0 = arg0 - 8;
    if (var_r0 > 0xF9U) {
        var_r0 = 0;
    }
    return (var_r0 * 0x54) + 0x08080EC8;
}
