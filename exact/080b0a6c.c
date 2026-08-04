#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080b09fc(s32, s32, s32, s8);

void Func_080b0a6c(void *arg0, s32 arg1, s32 arg2) {
    s32 var_r1;
    s32 var_r2;
    void *temp_r3;

    var_r1 = arg1;
    var_r2 = arg2;
    temp_r3 = *(void **)0x03001F2C;
    if (arg0 != NULL) {
        var_r1 = var_r1 + (M2C_FIELD(arg0, u16 *, 0xC) * 8) + 8;
        var_r2 = var_r2 + (M2C_FIELD(arg0, u16 *, 0xE) * 8) + 8;
    }
    Func_080b09fc(temp_r3 + 0x380, var_r1, var_r2, (s8) M2C_FIELD(temp_r3, u8 *, 0x3A8));
}
