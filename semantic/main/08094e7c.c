#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08003dec(void *, s32);
u32 Func_08004458(void);
s32 Func_080091a8(s32, s32, s32);
s32 Func_080770c0(s32);

void Func_08094e7c(void) {
    s32 sp0;
    s32 sp4;
    u8 *sp8;
    u8 *spC;
    u32 sp10;
    u32 sp14;
    u8 *sp18;
    u8 *var_r7;
    s32 temp_r1;
    s32 temp_r3_2;
    s32 temp_r5_2;
    s32 temp_sl;
    s32 var_fp;
    s32 var_r3;
    s32 var_r8;
    s32 var_r9;
    u32 temp_r3_3;
    u32 temp_r5;
    u8 *temp_r3;
    u8 *temp_r0;
    u8 *temp_r6;
    u8 *temp_r6_2;
    s32 temp_r8;

    temp_r0 = *(u8 **)0x03001EC4;
    sp18 = temp_r0;
    sp14 = 0;
    temp_r3 = *(u8 **)0x03001E70;
    spC = temp_r3;
    sp8 = temp_r3 + 0xE4;
    sp4 = 0;
    sp0 = 0;
    sp10 = 0;
    var_r7 = temp_r0 + 8;
    var_r9 = 0;
    var_fp = 0;
    var_r8 = 0;
loop_1:
    if (M2C_FIELD(var_r7, u16 *, 0x1C) == 0) {

    } else {
        temp_r8 = M2C_FIELD(sp8, s32 *, 0);
        temp_sl = M2C_FIELD(sp8, s32 *, 4);
        if (Func_080770c0(0x166) != 0) {
            M2C_FIELD(var_r7, u16 *, 0x1C) = (u16) (M2C_FIELD(var_r7, u16 *, 0x1C) + 1);
        }
        temp_r6 = (u8 *)0x0809F024 +
            (((u16) M2C_FIELD(var_r7, u16 *, 0x1C) >> 1) * 4);
        temp_r5 = Func_08004458();
        temp_r1 = ((s32) (M2C_FIELD(var_r7, s32 *, 0xC) - temp_r8) >> 0x10) + ((u32) ((1 & temp_r5) + (Func_08004458() & 1)) >> 1);
        var_fp = temp_r1 - 1;
        var_r3 = (M2C_FIELD(var_r7, s32 *, 0x14) - M2C_FIELD(var_r7, s32 *, 0x10)) - temp_sl;
        if (var_r3 < 0) {
            var_r3 += 0xFFFF;
        }
        var_r8 = (var_r3 >> 0x10) + M2C_FIELD(temp_r6, s16 *, 0);
        if (((u32) (temp_r1 + 0xF) <= 0xFFU) && (var_r8 >= -0x20) && (var_r8 <= 0x9F)) {
            M2C_FIELD(var_r7, u16 *, 8) = (u16) ((M2C_FIELD(var_r7, u16 *, 8) & 0xFFFFFC00) | ((M2C_FIELD(sp18, s32 *, 4) + M2C_FIELD(temp_r6, u16 *, 2)) & 0x3FF));
            M2C_FIELD(var_r7, u16 *, 6) = (u16) ((M2C_FIELD(var_r7, u16 *, 6) & 0xFFFFFE00) | (var_fp & 0x1FF));
            M2C_FIELD(var_r7, u8 *, 5) = (u8) (0x3F & M2C_FIELD(var_r7, u8 *, 5));
            M2C_FIELD(var_r7, s8 *, 4) = (s8) var_r8;
            M2C_FIELD(var_r7, u8 *, 7) = (u8) ((((0x3F & M2C_FIELD(var_r7, u8 *, 7)) | 0x40) & ~0x3E) | ((((u32) *(u32 *)0x03001E40 >> 1) & 1) * 0x10));
            Func_08003dec(var_r7, 0xF0);
        }
        M2C_FIELD(var_r7, u16 *, 0x1C) = (u16) (M2C_FIELD(var_r7, u16 *, 0x1C) + 0xFFFF);
    }
    if ((sp14 <= 3U) && (M2C_FIELD(var_r7, u16 *, 0x1C) == 0)) {
        temp_r5_2 = M2C_FIELD(sp18, s32 *, 0x40C);
        if (temp_r5_2 == 0) {
            if (var_r9 != 0) {
                M2C_FIELD(var_r7, s32 *, 0xC) = sp4;
                M2C_FIELD(var_r7, s32 *, 0x14) = sp0;
                M2C_FIELD(var_r7, s32 *, 0x10) = (s32) (Func_080091a8(0, var_fp >> 0x10, var_r8 >> 0x10) << 0x10);
                M2C_FIELD(var_r7, u16 *, 0x1C) = (u16) (0x3E - var_r9);
                M2C_FIELD(var_r7, s32 *, 0x18) = temp_r5_2;
                sp14 += 1;
                var_r9 += 4;
            } else if ((u8) Func_08004458() == 0) {
                temp_r6_2 = M2C_FIELD(spC, u8 **, 0);
                sp4 = M2C_FIELD(temp_r6_2, s32 *, 0) + (Func_08004458() << 8) + 0xFF800000;
                temp_r3_2 = M2C_FIELD(temp_r6_2, s32 *, 8) + (Func_08004458() << 8) + 0xFF800000;
                sp0 = temp_r3_2;
                M2C_FIELD(var_r7, s32 *, 0x14) = temp_r3_2;
                M2C_FIELD(var_r7, s32 *, 0xC) = sp4;
                M2C_FIELD(var_r7, s32 *, 0x10) = (s32) (Func_080091a8(0, var_fp >> 0x10, var_r8 >> 0x10) << 0x10);
                M2C_FIELD(var_r7, u16 *, 0x1C) = 0x1EU;
                M2C_FIELD(var_r7, s32 *, 0x18) = var_r9;
                sp14 += 1;
                var_r9 = 4;
            }
        }
    }
    temp_r3_3 = sp10 + 1;
    sp10 = temp_r3_3;
    var_r7 += 0x20;
    if (temp_r3_3 <= 0x1FU) {
        goto loop_1;
    }
}
