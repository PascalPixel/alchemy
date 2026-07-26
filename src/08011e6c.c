#include "types.h"

s32 Func_08011e6c(u8 *arg0, u32 arg1, u32 arg2) {
    s32 value;
    if (arg2 <= 7)
        value = (s8)arg0[0];
    else
        value = (s8)arg0[1];
    return value << 19;
}
