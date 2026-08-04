#include "types.h"

s32 Func_080022ec(s32, s32);

void Func_080f62b8(u32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4) {
    s8 *temp_fp;
    s32 temp_ip_5;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 var_ip;
    s32 var_r0;
    s32 var_r0_3;
    s32 var_r1_2;
    s32 var_r2;
    s32 var_r3;
    s32 var_r5;
    s32 var_r6;
    s32 var_r9;
    u32 temp_ip;
    u32 temp_ip_2;
    u32 temp_ip_3;
    u32 temp_ip_4;
    u32 var_r0_2;
    u32 var_r0_4;
    u32 var_r1;
    u32 var_r1_3;
    u32 var_r4;
    u32 var_r7;
    u32 var_r8;
    u32 var_sl;

    var_r4 = arg3;
    var_r7 = arg1;
    var_r6 = arg2 - arg0;
    var_sl = arg2;
    var_r8 = arg0;
    var_r5 = var_r4 - var_r7;
    var_r9 = 0x80;
    temp_fp = *(s8 **)0x03001EF0;
    var_r2 = var_r6;
    if (var_r6 < 0) {
        var_r2 = 0 - var_r6;
    }
    var_r3 = var_r5;
    if (var_r5 < 0) {
        var_r3 = 0 - var_r5;
    }
    if (var_r2 < var_r3) {
        if (var_r5 < 0) {
            temp_ip = var_r8;
            var_r8 = var_sl;
            var_sl = temp_ip;
            temp_ip_2 = var_r7;
            var_r7 = var_r4;
            var_r4 = temp_ip_2;
            var_r6 = var_sl - var_r8;
            var_r5 = var_r4 - var_r7;
        }
        var_r0 = (var_r6 < 0 ? var_r8 - var_sl : (u32)var_r6) * 0x100;
        if (var_r5 >= 0) {
            var_ip = Func_080022ec(var_r0, var_r5);
        } else {
            var_ip = Func_080022ec(var_r0, var_r7 - var_r4);
        }
        var_r0_2 = var_r7;
        var_r1 = var_r8;
        if (var_r0_2 != var_r4) {
            do {
                temp_r2 = ((((((var_r0_2 >> 3) << 5) + (var_r1 >> 3)) * 8) + (var_r0_2 & 7)) * 8) + (var_r1 & 7);
                if ((s32) *(temp_fp + temp_r2) < arg4) {
                    *(temp_fp + temp_r2) = (s8) arg4;
                }
                var_r9 += var_ip;
                if (var_r9 & 0x100) {
                    if (var_r6 > 0) {
                        var_r1 += 1;
                    } else {
                        var_r1 -= 1;
                    }
                    var_r9 &= 0xFFFFFEFF;
                }
                var_r0_2 += 1;
            } while (var_r0_2 != var_r4);
        }
    } else {
        if (var_r6 < 0) {
            temp_ip_3 = var_r8;
            var_r8 = var_sl;
            var_sl = temp_ip_3;
            temp_ip_4 = var_r7;
            var_r7 = var_r4;
            var_r4 = temp_ip_4;
            var_r6 = var_sl - var_r8;
            var_r5 = var_r4 - var_r7;
        }
        var_r0_3 = (var_r5 < 0 ? var_r7 - var_r4 : (u32)var_r5) * 0x100;
        if (var_r6 >= 0) {
            var_r1_2 = var_r6;
        } else {
            var_r1_2 = var_r8 - var_sl;
        }
        temp_ip_5 = Func_080022ec(var_r0_3, var_r1_2);
        var_r0_4 = var_r8;
        var_r1_3 = var_r7;
        if (var_r0_4 != var_sl) {
            do {
                temp_r2_2 = ((((((var_r1_3 >> 3) << 5) + (var_r0_4 >> 3)) * 8) + (var_r1_3 & 7)) * 8) + (var_r0_4 & 7);
                if ((s32) *(temp_fp + temp_r2_2) < arg4) {
                    *(temp_fp + temp_r2_2) = (s8) arg4;
                }
                var_r9 += temp_ip_5;
                if (var_r9 & 0x100) {
                    if (var_r5 > 0) {
                        var_r1_3 += 1;
                    } else {
                        var_r1_3 -= 1;
                    }
                    var_r9 &= 0xFFFFFEFF;
                }
                var_r0_4 += 1;
            } while (var_r0_4 != var_sl);
        }
    }
}
