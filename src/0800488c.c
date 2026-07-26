#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_0800488c(void) {
    s32 _c0 = 0x03001E50;
    return 0x03007800 - M2C_FIELD((void *)_c0, s32 *, 4);
}
