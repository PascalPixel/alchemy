#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080091a8(s32, s32, s32);

void Func_0808e9a8(void *arg0) {
    s32 temp_r0;

    temp_r0 = Func_080091a8(0, M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0x10));
    M2C_FIELD(arg0, s32 *, 0xC) = temp_r0;
    M2C_FIELD(arg0, s32 *, 0x14) = temp_r0;
}
