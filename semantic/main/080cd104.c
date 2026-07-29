#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(u32);
u32 Func_08004458(void);

void Func_080cd104(s32 arg0, s32 arg1) {
    u8 *sp0;
    s32 sp4;
    s32 temp_r1;
    s32 temp_r1_2;
    s8 *temp_r9;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r8;
    s32 var_r8_2;
    s32 var_sl;
    u32 var_sl_2;
    u8 *var_r0;
    u8 *var_r0_2;
    u8 *var_r5;
    u8 offsets[128];

    sp4 = arg1;
    sp0 = *(u8 **)0x03001EEC;
    temp_r9 = *(s8 **)0x03001EF0;
    var_r5 = offsets;
    do {
        *var_r5 = Func_08004458() & 0x3F;
        var_r5 += 1;
    } while (var_r5 != offsets + 128);
    if (arg0 == 1) {
        var_r8 = 0;
        var_sl = 1;
        var_r5_2 = 0;
        do {
            var_r8 += var_sl;
            var_sl += 1;
            if (var_r5_2 != var_r8) {
                do {
                    var_r6 = 0;
                    var_r0 = offsets;
loop_7:
                    temp_r1 = var_r5_2 - *var_r0;
                    var_r0 += 1;
                    if ((temp_r1 >= 0) && (temp_r1 <= 0x7F)) {
                        var_r2 = var_r6;
                        if (var_r6 < 0) {
                            var_r2 = var_r6 + 7;
                        }
                        var_r3 = temp_r1;
                        if (temp_r1 < 0) {
                            var_r3 = temp_r1 + 7;
                        }
                        *(temp_r9 + (((((((var_r2 >> 3) * 0x10) + (var_r3 >> 3)) * 8) + (var_r6 & 7)) * 8) + (temp_r1 & 7))) = 1 - sp4;
                    }
                    var_r6 += 1;
                    if (var_r6 != 0x80) {
                        goto loop_7;
                    }
                    var_r5_2 += 1;
                } while (var_r5_2 != var_r8);
            }
            M2C_FIELD(sp0, s32 *, 0x7824) = 1;
            Func_080030f8(1U);
        } while (var_r8 <= 0x100);
        return;
    }
    var_r8_2 = 0;
    var_sl_2 = 1;
    var_r6_2 = 0;
    do {
        var_r8_2 += (s32) ((var_sl_2 >> 0x1F) + var_sl_2) >> 1;
        var_sl_2 += 4;
        if (var_r6_2 != var_r8_2) {
            do {
                var_r5_3 = 0;
                var_r0_2 = offsets;
loop_22:
                temp_r1_2 = var_r6_2 - *var_r0_2;
                var_r0_2 += 1;
                if ((temp_r1_2 >= 0) && (temp_r1_2 <= 0x7F)) {
                    var_r2_2 = temp_r1_2;
                    if (temp_r1_2 < 0) {
                        var_r2_2 = temp_r1_2 + 7;
                    }
                    var_r3_2 = var_r5_3;
                    if (var_r5_3 < 0) {
                        var_r3_2 = var_r5_3 + 7;
                    }
                    *(temp_r9 + (((((((var_r2_2 >> 3) * 0x10) + (var_r3_2 >> 3)) * 8) + (temp_r1_2 & 7)) * 8) + (var_r5_3 & 7))) = 1 - sp4;
                }
                var_r5_3 += 1;
                if (var_r5_3 != 0x80) {
                    goto loop_22;
                }
                var_r6_2 += 1;
            } while (var_r6_2 != var_r8_2);
        }
        M2C_FIELD(sp0, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
    } while (var_r8_2 <= 0xBF);
}
