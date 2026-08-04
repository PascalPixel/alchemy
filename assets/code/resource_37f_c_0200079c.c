#include "types.h"

extern u8 *Data_03001ebc;

s32 *Func_0200079c(s32 arg0, s32 arg1) {
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (arg0 == (p[2] >> 20) && arg1 == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}
