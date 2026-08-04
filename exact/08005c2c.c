#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Func_08005c2c(s32 record_id)
{
    u16 *var_r1;
    u16 temp_r3_2;
    u32 var_r0;
    u32 var_r4;
    void *temp_r3;
    void *var_r2;

    temp_r3 = *(void **)0x03001F1C;
    var_r4 = 0;
    var_r0 = 0;
    var_r1 = temp_r3 + 0x20;
    var_r2 = temp_r3;
    do {
        if ((M2C_FIELD(var_r2, u8 *, 0) != 0) && (record_id == M2C_FIELD(var_r2, u8 *, 0x10))) {
            temp_r3_2 = *var_r1;
            if (var_r0 < (u32) temp_r3_2) {
                var_r0 = (u32) temp_r3_2;
            }
        }
        var_r4 += 1;
        var_r1 += 1;
        var_r2 += 1;
    } while (var_r4 <= 0xFU);
    return var_r0;
}
