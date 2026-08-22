#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080a413c(s32 arg0, s32 arg1) {
    s32 var_r0;

    var_r0 = 0x1E;
    if (arg1 != 0) {
        var_r0 = 0x26;
    }
    return var_r0;
}
