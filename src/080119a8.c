#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0800439c(u32 value);

void Func_080119a8(void) {
    if ((s8) M2C_FIELD(*(void **)0x03001E70, u8 *, 0xFC) == 0) {
        Func_0800439c(0x0801179D);
    }
}
