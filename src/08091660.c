#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0808ba1c(s32);
s32 Func_08009080(s32, s32);

void Func_08091660(void) {
    u8 *base;
    void *temp_r0;

    base = (u8 *) 0x02000240;
    temp_r0 = (void *) Func_0808ba1c(*(s32 *)(base + 0x1F4));
    M2C_FIELD(temp_r0, s32 *, 0x30) = 0x10000;
    M2C_FIELD(temp_r0, s32 *, 0x34) = 0x8000;
    M2C_FIELD(temp_r0, s32 *, 0x38) = 0x80000000;
    M2C_FIELD(temp_r0, s32 *, 0x40) = 0x80000000;
    M2C_FIELD(temp_r0, s32 *, 0x24) = 0;
    M2C_FIELD(temp_r0, s32 *, 0x2C) = 0;
    if (*(u8 *)(base + 0x1F2) == 1) {
        Func_08009080((s32) temp_r0, 0xC);
        return;
    }
    Func_08009080((s32) temp_r0, 1);
}
