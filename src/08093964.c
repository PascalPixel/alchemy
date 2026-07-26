#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080091e0(void *, s32);

s32 Func_08093964(void *arg0) {
    Func_080091e0(arg0, 0);
    M2C_FIELD(arg0, s8 *, 0x59) = 0;
    return 0;
}
