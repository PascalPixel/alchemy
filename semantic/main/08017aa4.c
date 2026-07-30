#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080173ac(void);
s32 Func_08018cac(s32, u32, s16, s32, s32);

void Func_08017aa4(const u16 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *var_r5;
    s16 temp_r9;
    s32 var_r1;
    s32 var_r7;
    s32 var_r8;
    u16 temp_r0;
    u16 var_r4;
    u32 temp_r1;
    u8 *temp_r6;

    var_r8 = arg3;
    var_r7 = arg2;
    temp_r6 = *(void **)0x03001E8C;
    var_r5 = (u8 *)arg0;
    temp_r9 = (s16) var_r7;
    if (var_r5 != NULL) {

    } else {
        M2C_FIELD(temp_r6, u16 *,
            (M2C_FIELD(temp_r6, u16 *, 0x12B2) * 2) + 0xEB0) = 0;
        var_r5 = temp_r6 + 0xEB0;
        M2C_FIELD(temp_r6, u16 *, 0x12B2) = (u16) ((M2C_FIELD(temp_r6, u16 *, 0x12B2) + 1) & 0x1FF);
    }
loop_20:
    var_r4 = M2C_FIELD(var_r5, u16 *, 0);
    var_r5 += 2;
    if ((u32) var_r4 > 0xFFU) {
        var_r4 = 0x40;
    }
    if (var_r4 != 0) {
        if ((u32) var_r4 <= 0x1EU) {
            temp_r1 = var_r4 - 3;
            switch (temp_r1) {
            case 5:
                var_r1 = 0xEAE;
block_9:
                M2C_FIELD(temp_r6, u16 *, var_r1) =
                    M2C_FIELD(var_r5, u16 *, 0);
            case 8:
            case 9:
            case 14:
            case 26:
block_10:
                var_r5 += 2;
                break;
            case 6:
                var_r1 = 0xEAC;
                goto block_9;
            case 7:
                var_r1 = 0xEA8;
                goto block_9;
            case 4:
                Func_080173ac();
                break;
            case 0:
                var_r7 = (s32) temp_r9;
                var_r8 += 0xF;
                break;
            case 11:
            case 12:
            case 25:
                var_r5 += 2;
                goto block_10;
            }
        } else {
            if (!(8 & M2C_FIELD(arg1, u16 *, 0x16))) {
                temp_r0 = M2C_FIELD(var_r5, u16 *, 0);
                if (((u32) var_r4 > 0x20U) && ((u32) temp_r0 > 0x20U) &&
                    ((u32) ((M2C_FIELD((u8 *)0x08032224, u16 *,
                        (var_r4 - 0x20) << 5) +
                        M2C_FIELD((u8 *)0x08032224, u16 *,
                        (temp_r0 - 0x20) << 5)) << 0x10) <= 0xF0000U)) {
                    var_r4 |= temp_r0 << 8;
                    var_r5 += 2;
                }
            }
            var_r7 += Func_08018cac(arg1, (u32) var_r4, (s16) var_r7, var_r8, 0);
        }
        goto loop_20;
    }
}
