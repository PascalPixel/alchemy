#include "types.h"

s32 Func_020064e6(u8 *o);

s32 Func_020000b0(u8 *o) {
    u32 v = (u32)(Func_020064e6(o) << 6) >> 16;

    if (v == 6) {
        s32 k = 0x3000;
        *(u16 *)(o + 6) = k;
    } else if (v == 9) {
        s32 k = 0x5000;
        *(u16 *)(o + 6) = k;
    }
    return 1;
}
