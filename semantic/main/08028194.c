#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08003d28();
void Func_08003dec(void *, s32);

void Func_08028194(void) {
    s32 sp4;
    s16 temp_r0;
    s16 temp_r0_2;
    s16 temp_r1_2;
    s16 temp_r2;
    s16 var_r0;
    s16 var_r0_2;
    u8 *temp_r3;
    u8 *var_r6;
    s32 temp_r1_10;
    s32 temp_r1_11;
    s32 temp_r1_5;
    s32 temp_r1_6;
    s32 temp_r1_9;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r2_4;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r9;
    s32 var_r1_2;
    s32 var_r2;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    u8 *var_r4;
    u8 *var_r1_3;
    u8 *var_r1_4;
    u8 *var_r1_5;
    u8 *var_r1_6;
    u16 temp_r1;
    u16 temp_r7;
    u16 temp_r7_2;
    u32 temp_r2_5;
    u32 temp_r4;
    u32 var_r5;
    u32 var_r5_2;
    u32 var_r5_3;
    u32 var_r5_4;
    u32 var_r5_5;
    u8 var_r1;
    void *temp_r1_3;
    void *temp_r1_4;
    void *temp_r1_7;
    void *temp_r1_8;

    temp_r3 = *(u8 **)0x03001F38;
    temp_r7 = M2C_FIELD((((*(s32 *)0x03001E40 * 2) & 0x1F) * 2), u16 *, 0x080366F8);
    var_r3 = temp_r7 + 0xFFFFFF00;
    var_r6 = temp_r3;
    if (var_r3 < 0) {
        var_r3 = temp_r7 - 0xFD;
    }
    temp_r7_2 = (var_r3 >> 2) + 0x130;
    temp_r1 = temp_r7_2;
    sp4 = (u16) temp_r1 | (temp_r1 << 0x10);
    var_r5 = 0;
    temp_r9 = Func_08003d28();
    if ((u32) M2C_FIELD(temp_r3, s16 *, 0x8E) > 0U) {
        var_r4 = (u8 *)0x03001B10;
        do {
            temp_r2 = M2C_FIELD(var_r6, s16 *, 0xC);
            if (temp_r2 != 0) {
                if (var_r5 == M2C_FIELD(temp_r3, s16 *, 0x8C)) {
                    var_r3_2 = temp_r7_2 * 7;
                    if (var_r3_2 < 0) {
                        var_r3_2 += 0x1FF;
                    }
                    temp_r1_2 = M2C_FIELD(var_r6, s16 *, 0xE);
                    if (temp_r1_2 != 0) {
                        var_r3_3 = temp_r7_2 * 3;
                        if (var_r3_3 < 0) {
                            var_r3_3 += 0xFF;
                        }
                        var_r1 = (temp_r1_2 + (var_r3_3 >> 8)) - 0x14;
                    } else {
                        var_r3_4 = temp_r7_2 * 0xF;
                        if (var_r3_4 < 0) {
                            var_r3_4 += 0xFF;
                        }
                        var_r1 = (var_r3_4 >> 8) - 0x1E;
                    }
                    M2C_FIELD(var_r6, s32 *, 0) = 0;
                    M2C_FIELD(var_r6, s32 *, 4) = (s32) ((temp_r9 << 0x19) | var_r1 | (((temp_r2 + (var_r3_2 >> 9)) - 0x14) << 0x10) | 0x80002300);
                    M2C_FIELD(var_r6, u32 *, 8) = (u32) ((u16) M2C_FIELD(((M2C_FIELD(var_r6, u16 *, 0x12) * 4) + var_r4), u16 *, 2) >> 5);
                    var_r1_2 = 0xF6;
                } else {
                    M2C_FIELD(var_r6, s32 *, 0) = 0;
                    M2C_FIELD(var_r6, s32 *, 4) = (s32) (M2C_FIELD(var_r6, s16 *, 0xE) | (temp_r2 << 0x10) | 0x80002000);
                    M2C_FIELD(var_r6, u32 *, 8) = (u32) ((u16) M2C_FIELD(((M2C_FIELD(var_r6, u16 *, 0x12) * 4) + var_r4), u16 *, 2) >> 5);
                    var_r1_2 = 0xF5;
                }
                Func_08003dec(var_r6, var_r1_2);
            }
            var_r5 += 1;
            var_r6 += 0x14;
        } while (var_r5 < (u32) M2C_FIELD(temp_r3, s16 *, 0x8E));
    }
    if (M2C_FIELD(temp_r3, s16 *, 0x94) == 0) {
        temp_r1_3 = *(void **)0x03001ECC;
        if (temp_r1_3 == NULL) {
            return;
        }
        if (M2C_FIELD(temp_r3, s16 *, 0x8E) == 0) {
            return;
        }
        temp_r1_4 = temp_r1_3 + (M2C_FIELD(temp_r1_3, u8 *, 0x539) * 0x284);
        temp_r1_5 = (M2C_FIELD(temp_r3, s16 *, 0x8C) * 0x14) + 0xC;
        temp_r0 = M2C_FIELD(temp_r3, s16 *, temp_r1_5);
        temp_r3_2 = temp_r7_2 * 0xC;
        var_r2 = temp_r3_2 + 0xFFFFF4FF;
        if (var_r2 < 0) {
            var_r2 = temp_r3_2 + 0xFFFFF5FE;
        }
        temp_r2_2 = var_r2 >> 8;
        temp_r2_3 = temp_r7_2 << 5;
        var_r3_5 = temp_r2_3 + 0xFFFFE0FF;
        if (var_r3_5 < 0) {
            var_r3_5 = temp_r2_3 + 0xFFFFE2FE;
        }
        temp_r4 = M2C_FIELD((temp_r3 + temp_r1_5), s16 *, 2) + 0x18 + (var_r3_5 >> 9) + 1;
        var_r5_2 = 0x18;
        var_r1_3 = temp_r1_4 + 0x66;
        if (temp_r4 > 0x18U) {
            do {
                var_r5_2 += 1;
                M2C_FIELD(var_r1_3, u16 *, 0) =
                    (0xFF & M2C_FIELD(var_r1_3, u16 *, 0)) |
                    (((temp_r0 - temp_r2_2) << 8) + (temp_r0 + temp_r2_2) + 0x17);
                var_r1_3 += 4;
            } while (var_r5_2 < temp_r4);
        }
        var_r0 = M2C_FIELD(temp_r3, s16 *, 0xC);
        if (M2C_FIELD(temp_r3, s16 *, 0x8C) == 0) {
            temp_r1_6 = temp_r7_2 * 0xC;
            var_r3_6 = temp_r1_6 + 0xFFFFF4FF;
            if (var_r3_6 < 0) {
                var_r3_6 = temp_r1_6 + 0xFFFFF5FE;
            }
            var_r0 -= var_r3_6 >> 8;
        }
        var_r1_4 = temp_r1_4 + 6;
        var_r5_3 = 0;
        do {
            var_r5_3 += 1;
            M2C_FIELD(var_r1_4, u16 *, 0) =
                (0xFF & M2C_FIELD(var_r1_4, u16 *, 0)) | (var_r0 << 8);
            var_r1_4 += 4;
        } while (var_r5_3 <= 0x17U);
        return;
    }
    temp_r1_7 = *(void **)0x03001ECC;
    if ((temp_r1_7 != NULL) && (M2C_FIELD(temp_r3, s16 *, 0x8E) != 0)) {
        temp_r1_8 = temp_r1_7 + (M2C_FIELD(temp_r1_7, u8 *, 0x539) * 0x284);
        temp_r1_9 = (M2C_FIELD(temp_r3, s16 *, 0x8C) * 0x14) + 0xC;
        temp_r0_2 = M2C_FIELD(temp_r3, s16 *, temp_r1_9);
        temp_r2_4 = temp_r7_2 * 0xC;
        var_r3_7 = temp_r2_4 + 0xFFFFF4FF;
        if (var_r3_7 < 0) {
            var_r3_7 = temp_r2_4 + 0xFFFFF5FE;
        }
        temp_r3_3 = var_r3_7 >> 8;
        temp_r1_10 = temp_r7_2 << 5;
        var_r3_8 = temp_r1_10 + 0xFFFFE0FF;
        if (var_r3_8 < 0) {
            var_r3_8 = temp_r1_10 + 0xFFFFE2FE;
        }
        temp_r2_5 = (M2C_FIELD((temp_r3 + temp_r1_9), s16 *, 2) - (var_r3_8 >> 9)) - 1;
        var_r5_4 = temp_r2_5;
        var_r1_5 = (temp_r2_5 * 4) + temp_r1_8 + 6;
        if (var_r5_4 <= 0x87U) {
            do {
                var_r5_4 += 1;
                M2C_FIELD(var_r1_5, u16 *, 0) =
                    (0xFF & M2C_FIELD(var_r1_5, u16 *, 0)) |
                    (((temp_r0_2 - temp_r3_3) << 8) + (temp_r0_2 + temp_r3_3) + 0x17);
                var_r1_5 += 4;
            } while (var_r5_4 <= 0x87U);
        }
        var_r0_2 = M2C_FIELD(temp_r3, s16 *, 0xC);
        if (M2C_FIELD(temp_r3, s16 *, 0x8C) == 0) {
            temp_r1_11 = temp_r7_2 * 0xC;
            var_r3_9 = temp_r1_11 + 0xFFFFF4FF;
            if (var_r3_9 < 0) {
                var_r3_9 = temp_r1_11 + 0xFFFFF5FE;
            }
            var_r0_2 -= var_r3_9 >> 8;
        }
        var_r1_6 = temp_r1_8 + 0x226;
        var_r5_5 = 0x88;
        do {
            var_r5_5 += 1;
            M2C_FIELD(var_r1_6, u16 *, 0) =
                (0xFF & M2C_FIELD(var_r1_6, u16 *, 0)) | (var_r0_2 << 8);
            var_r1_6 += 4;
        } while (var_r5_5 <= 0x9FU);
    }
}
