#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
u8 *Func_08077008(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);

void Func_0808a6e4(s16 arg0) {
    s16 *var_r3_4;
    s16 temp_r1_2;
    s16 var_r0;
    s16 var_r2;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r5;
    s32 var_r5_2;
    u16 temp_r1;
    u8 *temp_r0;
    u8 *temp_r0_4;

    *(s16 *)0x02000476 = arg0;
    if (arg0 != -1) {
        var_r2 = *(s16 *)0x0200040E;
        var_r0 = *(s16 *)0x02000410;
        if (var_r2 == -1) {
            if (var_r0 != var_r2) {
                goto block_40;
            }
            *(u16 *)0x02000400 = *(u16 *)0x02000408;
            *(u16 *)0x02000402 = *(u16 *)0x0200040A;
            Func_080770c8(0x109);
            return;
        }
        goto block_41;
    }
    temp_r0 = Func_08077008(*(s32 *)0x02000434);
    if (M2C_FIELD(temp_r0, s16 *, 0x38) == 0) {
        M2C_FIELD(temp_r0, s16 *, 0x38) = 1;
        var_r5 = 0x4000;
        temp_r0_2 = Func_080022ec(0x4000, (s32) M2C_FIELD(temp_r0, s16 *, 0x34));
        if (temp_r0_2 <= 0x4000) {
            var_r5 = 0;
            if (temp_r0_2 >= 0) {
                var_r5 = temp_r0_2;
            }
        }
        M2C_FIELD(temp_r0, s16 *, 0x14) = (s16) var_r5;
        if ((var_r5 == 0) && (M2C_FIELD(temp_r0, s16 *, 0x38) != 0)) {
            M2C_FIELD(temp_r0, s16 *, 0x14) = 1;
        }
        temp_r0_3 = Func_080022ec(
            M2C_FIELD(temp_r0, s16 *, 0x3A) * 0x4000,
            (s32) M2C_FIELD(temp_r0, s16 *, 0x36));
        var_r3 = 0x4000;
        if (temp_r0_3 <= 0x4000) {
            var_r3 = 0;
            if (temp_r0_3 >= 0) {
                var_r3 = temp_r0_3;
            }
        }
        M2C_FIELD(temp_r0, s16 *, 0x16) = (s16) var_r3;
        if ((var_r3 == 0) && (M2C_FIELD(temp_r0, s16 *, 0x3A) != 0)) {
            M2C_FIELD(temp_r0, s16 *, 0x16) = 1;
        }
    }
    if (Func_080770c0(0x20) != 0) {
        var_r5_2 = 0;
        do {
            temp_r0_4 = Func_08077008(var_r5_2);
            temp_r1 = M2C_FIELD(temp_r0_4, u16 *, 0x34);
            M2C_FIELD(temp_r0_4, u16 *, 0x38) = temp_r1;
            M2C_FIELD(temp_r0_4, u16 *, 0x3A) = (u16) M2C_FIELD(temp_r0_4, u16 *, 0x36);
            temp_r1_2 = (s16) temp_r1;
            temp_r0_5 = Func_080022ec(temp_r1_2 * 0x4000, (s32) temp_r1_2);
            var_r3_2 = 0x4000;
            if (temp_r0_5 <= 0x4000) {
                var_r3_2 = 0;
                if (temp_r0_5 >= 0) {
                    var_r3_2 = temp_r0_5;
                }
            }
            M2C_FIELD(temp_r0_4, s16 *, 0x14) = (s16) var_r3_2;
            if ((var_r3_2 == 0) && ((s16) M2C_FIELD(temp_r0_4, u16 *, 0x38) != 0)) {
                M2C_FIELD(temp_r0_4, s16 *, 0x14) = 1;
            }
            temp_r0_6 = Func_080022ec(
                (s16) M2C_FIELD(temp_r0_4, u16 *, 0x3A) * 0x4000,
                (s32) (s16) M2C_FIELD(temp_r0_4, u16 *, 0x36));
            var_r3_3 = 0x4000;
            if (temp_r0_6 <= 0x4000) {
                var_r3_3 = 0;
                if (temp_r0_6 >= 0) {
                    var_r3_3 = temp_r0_6;
                }
            }
            M2C_FIELD(temp_r0_4, s16 *, 0x16) = (s16) var_r3_3;
            if ((var_r3_3 == 0) && ((s16) M2C_FIELD(temp_r0_4, u16 *, 0x3A) != 0)) {
                M2C_FIELD(temp_r0_4, s16 *, 0x16) = 1;
            }
            var_r5_2 += 1;
        } while (var_r5_2 <= 1);
    }
    var_r2 = *(s16 *)0x02000412;
    var_r0 = *(s16 *)0x02000414;
    if (var_r2 == -1) {
        if (var_r0 == var_r2) {
            *(u16 *)0x02000400 = *(u16 *)0x02000404;
            *(u16 *)0x02000402 = *(u16 *)0x02000406;
            return;
        }
block_40:
        var_r2 = *(s16 *)0x02000408;
block_41:
        var_r3_4 = (s16 *)0x02000400;
        goto block_42;
    }
    var_r3_4 = (s16 *)0x02000400;
block_42:
    *var_r3_4 = var_r2;
    if (var_r0 != -1) {
        *(u16 *)0x02000402 = (u16) var_r0;
        return;
    }
    *(u16 *)0x02000402 = *(u16 *)0x0200040A;
}
