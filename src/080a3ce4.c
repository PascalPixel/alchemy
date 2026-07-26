#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080a3ce4(s32 arg0) {
    s32 ret;
    s32 lo;

    if (arg0 > 0xC4) {
        goto L0;
    }
    lo = 0xC1;
    if (arg0 < lo) {
        goto L0;
    }
    ret = 1;
    return ret;
L0:
    ret = 0;
    return ret;
}
