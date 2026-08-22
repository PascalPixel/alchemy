#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080044d0(s32, s32);

s32 Func_08093a14(void *arg0) {
    s16 var_r0;
    u16 temp_r3;
    s32 temp;
    void *temp_r1;

    temp_r1 = M2C_FIELD(arg0, void *, 0x68);
    if (temp_r1 != NULL) {
        M2C_FIELD(arg0, u8, 0x5A) = (u8) (0xFE & M2C_FIELD(arg0, u8, 0x5A));
        temp = (u16) Func_080044d0(M2C_FIELD(temp_r1, s32, 0x10) - M2C_FIELD(arg0, s32, 0x10), M2C_FIELD(temp_r1, s32, 8) - M2C_FIELD(arg0, s32, 8));
        temp_r3 = M2C_FIELD(arg0, u16, 6);
        var_r0 = temp - temp_r3;
        if (var_r0 != 0) {
            if (var_r0 > 0x1000) {
                var_r0 = 0x1000;
            }
            if (var_r0 < -0x1000) {
                var_r0 = -0x1000;
            }
            M2C_FIELD(arg0, u16, 6) = (u16) (temp_r3 + var_r0);
        }
    }
    return 1;
}
