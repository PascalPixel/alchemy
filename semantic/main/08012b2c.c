#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_08012b2c(s32 arg0, s32 arg1, s32 *arg2) {
    s32 sp0;
    s16 var_r5;
    s32 *var_r6;
    s32 *var_r7;
    s32 temp_r0;
    s32 temp_r1;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_4;
    s32 var_r4_5;
    s32 var_r4_6;
    s32 var_r4_7;
    s32 var_r4_8;
    s32 var_r5_2;
    s32 var_r5_3;
    u16 temp_r5;
    u16 temp_r5_2;
    u16 temp_r5_3;
    u16 temp_r5_4;
    u16 temp_r5_5;
    u8 temp_r4;

    temp_r4 = M2C_FIELD(M2C_FIELD(*(void **)0x03001E60, void **, 0x28), u8 *, 4);
    var_r7 = arg2;
    var_r5 = 0;
    if (*(s32 *)0x03001AE8 & 2) {
        var_r5 = *(s32 *)0x03001800 << 8;
    }
    switch (temp_r4) {
    case 0x3:
        var_r4 = 5;
        do {
            temp_r5 = (u16) var_r5;
            M2C_FIELD(var_r7, s32 *, 0) = arg0 << 0x10;
            M2C_FIELD(var_r7, s32 *, 4) = 0;
            M2C_FIELD(var_r7, s32 *, 8) = (s32) (arg1 << 0x10);
            sp0 = var_r4;
            Func_0800447c(0x380000, (s32) temp_r5, var_r7);
            var_r4 -= 1;
            var_r7 += 0x10;
            var_r5 = temp_r5 + 0x2AAA;
        } while (var_r4 >= 0);
        var_r4_2 = 6;
        do {
            var_r4_2 += 1;
        } while (var_r4_2 <= 9);
        return;
    case 0x2C:
    case 0x58:
    case 0x8:
    case 0x5:
        var_r4_3 = 7;
        do {
            temp_r5_2 = (u16) var_r5;
            M2C_FIELD(var_r7, s32 *, 0) = arg0 << 0x10;
            M2C_FIELD(var_r7, s32 *, 4) = 0;
            M2C_FIELD(var_r7, s32 *, 8) = (s32) (arg1 << 0x10);
            sp0 = var_r4_3;
            Func_0800447c(0x380000, (s32) temp_r5_2, var_r7);
            var_r4_3 -= 1;
            var_r7 += 0x10;
            var_r5 = temp_r5_2 + 0x2000;
        } while (var_r4_3 >= 0);
        var_r4_4 = 8;
        do {
            var_r4_4 += 1;
        } while (var_r4_4 <= 9);
        return;
    case 0x6:
    case 0x4:
        var_r4_5 = 0;
        do {
            temp_r5_3 = (u16) var_r5;
            M2C_FIELD(var_r7, s32 *, 0) = arg0 << 0x10;
            M2C_FIELD(var_r7, s32 *, 4) = 0;
            M2C_FIELD(var_r7, s32 *, 8) = (s32) (arg1 << 0x10);
            sp0 = var_r4_5;
            Func_0800447c(0x380000, (s32) temp_r5_3, var_r7);
            var_r4_5 += 1;
            var_r7 += 0x10;
            var_r5 = temp_r5_3 + 0x1999;
        } while (var_r4_5 <= 9);
        return;
    case 0x14:
        var_r5_2 = (s32) ((var_r5 << 0x10) + 0x40000000) >> 0x10;
        var_r4_6 = 0;
        temp_r0 = arg0 << 0x10;
        temp_r1 = arg1 << 0x10;
        var_r6 = var_r7;
        do {
            M2C_FIELD(var_r6, s32 *, 0) = temp_r0;
            temp_r5_4 = (u16) var_r5_2;
            M2C_FIELD(var_r6, s32 *, 4) = 0;
            M2C_FIELD(var_r6, s32 *, 8) = temp_r1;
            sp0 = var_r4_6;
            Func_0800447c(0x280000, (s32) temp_r5_4, var_r7);
            M2C_FIELD(var_r6, s32 *, 0x10) = temp_r0;
            M2C_FIELD(var_r6, s32 *, 0x14) = 0;
            M2C_FIELD(var_r6, s32 *, 0x18) = temp_r1;
            Func_0800447c(0x280000, (s32) temp_r5_4, var_r7 + 0x10);
            var_r4_6 += 1;
            var_r6 += 0x20;
            var_r7 += 0x20;
            var_r5_2 = (s32) (s16) (temp_r5_4 + 0x8000);
        } while (var_r4_6 <= 1);
        return;
    default:
        var_r5_3 = (s32) ((var_r5 << 0x10) + 0x20000000) >> 0x10;
        var_r4_7 = 3;
        do {
            M2C_FIELD(var_r7, s32 *, 0) = arg0 << 0x10;
            temp_r5_5 = (u16) var_r5_3;
            M2C_FIELD(var_r7, s32 *, 4) = 0;
            M2C_FIELD(var_r7, s32 *, 8) = (s32) (arg1 << 0x10);
            sp0 = var_r4_7;
            Func_0800447c(0x380000, (s32) temp_r5_5, var_r7);
            var_r4_7 -= 1;
            var_r7 += 0x10;
            var_r5_3 = (s32) (s16) (temp_r5_5 + 0x4000);
        } while (var_r4_7 >= 0);
        var_r4_8 = 5;
        do {
            var_r4_8 -= 1;
        } while (var_r4_8 >= 0);
        return;
    }
}
