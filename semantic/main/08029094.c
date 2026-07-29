#include "types.h"

s16 Func_08029094(void *arg0, s16 arg1, s16 *arg2, s16 *arg3) {
    s16 temp_r3_3;
    s16 temp_r3_5;
    s16 var_r3;
    s16 var_r5;
    u16 temp_r1;
    u16 temp_r2;
    u16 temp_r2_2;
    u16 temp_r3;
    u16 temp_r3_2;
    u16 temp_r3_4;
    u16 temp_r3_6;

    var_r5 = arg1;
    if (*(s32 *)0x03001B04 & 1) {
        return -1;
    }
    if (*(s32 *)0x03001B04 & 2) {
        return -2;
    }
    if ((*(s32 *)0x03001B04 & 0x80) || (temp_r1 = *(s32 *)0x03001B04 & 0x40, (temp_r1 != 0))) {
        *arg3 = (u16) *arg3 ^ 1;
    } else {
        if (*(s32 *)0x03001B04 & 0x10) {
            if (*arg3 == 0) {
                var_r3 = var_r5 + 1;
                goto block_22;
            }
            temp_r3 = *arg2 + 1;
            *arg2 = temp_r3;
            if ((s32) (temp_r3 << 0x10) > 0x630000) {
                *arg2 = temp_r1;
            }
            goto block_25;
        }
        if (*(s32 *)0x03001B04 & 0x20) {
            if (*arg3 == 0) {
                var_r5 -= 1;
            } else {
                temp_r3_2 = *arg2 - 1;
                *arg2 = temp_r3_2;
                if ((s32) (temp_r3_2 << 0x10) < 0) {
                    *arg2 = 0x63;
                }
            }
            if ((s32) var_r5 < 0) {
                var_r5 = 0xC8;
            }
            goto block_27;
        }
        if (*(s32 *)0x03001B04 & 0x100) {
            temp_r3_3 = *arg3;
            if (temp_r3_3 == 0) {
                *arg2 = (u16) temp_r3_3;
                var_r3 = var_r5 + 0xA;
block_22:
                var_r5 = var_r3;
            } else {
                temp_r2 = *arg2;
                temp_r3_4 = temp_r2 + 0xA;
                *arg2 = temp_r3_4;
                if ((s32) (temp_r3_4 << 0x10) > 0x630000) {
                    *arg2 = temp_r2 - 0x59;
                }
            }
block_25:
            if ((s32) var_r5 > 0xC8) {
                var_r5 = 0;
            }
block_27:
            Func_08028ef0(arg0, var_r5, arg2);
        } else if (*(s32 *)0x03001B04 & 0x200) {
            temp_r3_5 = *arg3;
            if (temp_r3_5 == 0) {
                *arg2 = (u16) temp_r3_5;
                var_r5 -= 0xA;
            } else {
                temp_r2_2 = *arg2;
                temp_r3_6 = temp_r2_2 - 0xA;
                *arg2 = temp_r3_6;
                if ((s32) (temp_r3_6 << 0x10) < 0) {
                    *arg2 = temp_r2_2 + 0x59;
                }
            }
            if ((s32) var_r5 < 0) {
                var_r5 = 0xC8;
            }
            Func_08028ef0(arg0, var_r5, arg2);
        }
    }
    return var_r5;
}
