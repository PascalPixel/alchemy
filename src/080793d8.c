#include "types.h"

u8 Func_080793d8(s32 arg0) {
    u8 *base;

    base = (u8 *) 0x02000040;
    arg0 = (u32) (arg0 << 0x14) >> 0x17;
    if (base[arg0] <= 0xFE) {
        base[arg0] = base[arg0] + 1;
    }
    return base[arg0];
}
