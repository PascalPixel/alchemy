#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800d14c(void *, s32, s32, s32);

s32 Func_0800daa0(void *arg0) {
    void *temp_r3;

    temp_r3 = M2C_FIELD(arg0, void **, 0x68);
    Func_0800d14c(arg0, M2C_FIELD(temp_r3, s32 *, 8), M2C_FIELD(temp_r3, s32 *, 0xC), M2C_FIELD(temp_r3, s32 *, 0x10));
    M2C_FIELD(arg0, u16 *, 4) = (u16) (M2C_FIELD(arg0, u16 *, 4) + 1);
    return 1;
}
