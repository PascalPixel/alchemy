#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08003d28();
void Func_08003dec(void *, s32);
s32 Func_0800aa0c(u8 *, u16);

void Func_0800b168(u8 *arg0, u8 *arg1, u8 *arg2, u16 arg3) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    u8 *sp14;
    u32 sp18;
    u32 sp1C;
    s32 sp20;
    s32 sp24;
    u8 *temp_r0_2;
    s32 temp_fp;
    s32 temp_r0;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r4;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r8;
    s32 temp_r9;
    s32 temp_sl;
    s32 var_r4;
    s32 var_r5;
    u8 *temp_r1;

    sp1C = (u8) M2C_FIELD(arg0, u8 *, 0x20) >> 1;
    sp14 = arg0 + 0x21;
    sp18 = (u8) M2C_FIELD(arg0, u8 *, 0x21) >> 1;
    sp10 = 8;
    spC = 4;
    temp_r1 = arg1 + 4;
    temp_r9 = M2C_FIELD(arg2, s32 *, 4);
    sp4 = M2C_FIELD(arg1, s32 *, 4);
    temp_r8 = M2C_FIELD(arg2, s32 *, 0);
    temp_r6 = M2C_FIELD((temp_r1 + 4), s32 *, 4);
    temp_sl = M2C_FIELD(arg1, s32 *, 0);
    temp_fp = M2C_FIELD(temp_r1, s32 *, 4);
    temp_r0 = Func_0800aa0c(arg0, arg3);
    if ((temp_r0 == 0) && (temp_r8 == 0x10000) && (temp_r9 == temp_r8)) {
        if (M2C_FIELD(arg0, u16 *, 0x1E) == 0) {
            sp8 = 0;
            sp20 = 0;
        } else {
            goto block_6;
        }
    } else {
block_6:
        sp8 = 1;
        temp_r1_2 = (u16) (((u32) temp_r8 << 8) >> 0x10) |
            ((((u32) temp_r9 << 8) >> 0x10) << 0x10);
        sp24 = temp_r1_2;
        if (temp_r0 != 0) {
            sp24 = (0xFFFF0000 & temp_r1_2) | (u16) (0 - M2C_FIELD(&sp24, u16 *, 0));
        }
        sp20 = Func_08003d28();
    }
    if ((temp_r8 > 0x10000) || (temp_r9 > 0x10000)) {
        sp8 = 3;
        sp1C *= 2;
        sp18 *= 2;
        sp10 = 0x10;
        spC = 8;
    }
    if (sp4 <= 0xFF9C0000) {
        sp0 = 1;
        var_r4 = 0;
    } else {
        sp0 = (temp_fp >> 0x11) + 0xA;
        var_r4 = 2;
    }
    temp_r6_2 = ((s32) (temp_fp - temp_r6) >> 0x10) - spC;
    var_r5 = temp_sl >> 0x10;
    if (1 & M2C_FIELD(arg0, u8 *, 0x26)) {
        if (temp_r6_2 <= 0x9F) {
            temp_r0_2 = arg0 + 0xC;
            M2C_FIELD(temp_r0_2, u8 *, 5) = (u8) ((-4 & M2C_FIELD(temp_r0_2, u8 *, 5)) | sp8);
            M2C_FIELD(temp_r0_2, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(temp_r0_2, u8 *, 7)) | ((sp20 & 0x1F) * 2));
            M2C_FIELD(temp_r0_2, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r0_2, u16 *, 6)) | ((var_r5 - sp10) & 0x1FF));
            M2C_FIELD(temp_r0_2, s8 *, 4) = (s8) temp_r6_2;
            Func_08003dec(temp_r0_2, var_r4);
        } else {
            var_r5 = temp_sl >> 0x10;
        }
    }
    temp_r4 = (var_r5 - sp1C) + ((s32) ((temp_r8 * (s8) M2C_FIELD(arg0, u8 *, 0x22)) + 0xFFFF) >> 0x10);
    temp_r6_3 = (((s32) (temp_fp - sp4) >> 0x10) - sp18) - ((s32) ((temp_r9 * (((u8) *sp14 >> 1) - (s8) M2C_FIELD(arg0, u8 *, 0x23))) + 0xFFFF) >> 0x10);
    if ((temp_r4 <= 0xEF) && (temp_r6_3 <= 0x9F)) {
        M2C_FIELD(arg0, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(arg0, u16 *, 6)) | (temp_r4 & 0x1FF));
        M2C_FIELD(arg0, s8 *, 4) = (s8) temp_r6_3;
        M2C_FIELD(arg0, u8 *, 5) = (u8) ((-4 & M2C_FIELD(arg0, u8 *, 5)) | sp8);
        temp_r1_3 = sp20 & 0x1F;
        sp20 = temp_r1_3;
        M2C_FIELD(arg0, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(arg0, u8 *, 7)) | (temp_r1_3 * 2));
        Func_08003dec(arg0, sp0);
    }
}
