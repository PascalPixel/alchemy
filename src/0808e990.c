#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_0808e14c(u16);

u32 Func_0808e990(u16 arg0) {
    s32 temp_r0;

    temp_r0 = Func_0808e14c(arg0);
    return (u32) ((0 - temp_r0) | temp_r0) >> 0x1F;
}
