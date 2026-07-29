#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080151c0(s32, s16 *, s32);

s32 Func_080b920c(s32 arg0) {
    s32 *sp0;
    s16 *sp4;
    s16 *sp8;
    s32 spC;
    s16 *var_r1;
    s16 *var_r6;
    s32 temp_r0;
    s32 temp_r0_3;
    s32 temp_r2;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r8;
    s32 var_r9;
    s32 var_sl;
    void *temp_r0_2;
    void *var_r2;

    spC = arg0;
    sp8 = Func_08004970(0x11);
    sp4 = Func_08004970(9);
    temp_r0 = Func_080b6b40(1, (u16 *) sp4);
    var_r8 = 0;
    var_r9 = 0;
    if (temp_r0 > 0) {
        var_r6 = sp4;
        var_sl = temp_r0;
        do {
            temp_r0_2 = (void *) Func_08077008((s32) (u16) *var_r6);
            var_r5_2 = 0;
            if ((s32) M2C_FIELD(temp_r0_2, u8 *, 0x43) > 0) {
                sp0 = temp_r0_2 + 0x138;
                var_r1 = &sp8[var_r9];
                var_r2 = spC + (var_r8 * 0x10);
                do {
                    if ((M2C_FIELD(temp_r0_2, u8 *, 0x13C) != 0) || (*sp0 & 0xFFFFFF00)) {
                        M2C_FIELD(var_r2, u16 *, 0) = (u16) *var_r6;
                        M2C_FIELD(var_r2, u16 *, 4) = (u16) M2C_FIELD(temp_r0_2, u16 *, 0x40);
                        M2C_FIELD(var_r2, s16 *, 6) = 8;
                        M2C_FIELD(var_r2, s16 *, 8) = 0;
                        M2C_FIELD(var_r2, s16 *, 0xA) = 0x180;
                        var_r8 += 1;
                        var_r2 += 0x10;
                    } else {
                        *var_r1 = (s16) (u16) *var_r6;
                        var_r1 += 2;
                        var_r9 += 1;
                    }
                    var_r5_2 += 1;
                } while (var_r5_2 < (s32) M2C_FIELD(temp_r0_2, u8 *, 0x43));
            }
            var_sl -= 1;
            var_r6 += 2;
        } while (var_sl != 0);
    }
    temp_r2 = spC + (var_r8 * 0x10);
    spC = temp_r2;
    temp_r0_3 = Func_080151c0(temp_r2, sp8, var_r9);
    var_r5 = -1;
    if (temp_r0_3 >= 0) {
        var_r5 = var_r8 + temp_r0_3;
    }
    Func_08002df0(sp4);
    Func_08002df0(sp8);
    return var_r5;
}
