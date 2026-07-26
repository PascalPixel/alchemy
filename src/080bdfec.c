#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_080bdfec(void) {
    void *temp_r2;

    temp_r2 = *(void **)0x03001E74;
    M2C_FIELD(temp_r2, s32 *, 0x800) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x7FC) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x804) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x808) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x7F8) = 0;
    M2C_FIELD(temp_r2, s32 *, 0x820) = 0x86;
    M2C_FIELD(temp_r2, s32 *, 0x824) = 0;
}
