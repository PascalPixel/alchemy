#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080aae14(u16 *arg0, u16 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 sp0;
    s32 *sp4;
    s32 sp8;
    s16 *var_r0_2;
    s32 var_fp;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r7;
    s32 var_r9;
    s32 var_sl;
    u16 *var_lr;
    u16 *var_r0;
    u16 *var_r1;
    u16 *var_r1_2;
    u16 *var_r5;

    sp8 = arg2;
    sp4 = arg3;
    sp0 = 0;
    var_sl = 0;
    var_fp = 0;
    if (*arg1 != 0) {
        var_r0 = arg1;
        var_r5 = sp8 - 2;
loop_2:
        M2C_FIELD(var_r5, s16 *, 2) = (s16) (0x3FFF & *var_r0);
        var_sl += 1;
        var_r1 = arg0;
        var_r5 += 2;
        var_r4 = 0;
        if ((*var_r0 ^ *var_r1) & 0x3FFF) {
loop_4:
            var_r4 += 1;
            if (var_r4 <= 0x1F) {
                var_r1 += 4;
                if ((*var_r0 ^ *var_r1) & 0x3FFF) {
                    goto loop_4;
                }
            }
        }
        if (var_r4 == 0x20) {
            var_fp += 1;
            *var_r5 |= 0x8000;
        }
        var_r0 += 4;
        if (((s32) var_r0 <= (s32) (arg1 + 0x7C)) && (*var_r0 != 0)) {
            goto loop_2;
        }
    }
    var_r9 = 0;
    if (*arg0 != 0) {
        var_lr = arg0;
        var_r0_2 = (var_sl * 2) + sp8;
        var_r7 = 0;
loop_12:
        var_r1_2 = arg1;
        var_r4_2 = 0;
        if ((*(var_r7 + arg0) ^ *var_r1_2) & 0x3FFF) {
loop_14:
            var_r4_2 += 1;
            if (var_r4_2 <= 0x1F) {
                var_r1_2 += 4;
                if ((*var_lr ^ *var_r1_2) & 0x3FFF) {
                    goto loop_14;
                }
            }
        }
        if (var_r4_2 == 0x20) {
            sp0 += 1;
            *var_r0_2 = (0x3FFF & *(var_r7 + arg0)) | 0x4000;
            var_r0_2 += 2;
            var_sl += 1;
        }
        var_r9 += 1;
        var_r7 += 4;
        var_lr += 4;
        if (var_r9 <= 0x1F) {
            if (*(var_r7 + arg0) == 0) {

            } else {
                goto loop_12;
            }
        }
    }
    *sp4 = var_fp;
    *arg4 = sp0;
    return var_sl;
}
