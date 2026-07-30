#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08003dec(void *, s32);
u32 Func_08004458(void);
s32 Func_080091a8(s32, s32, s32);
s32 Func_080770c0(s32);

void Func_08094820(void) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    u8 *var_r7;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r3_3;
    s32 var_r2;
    s32 var_r3;
    u16 temp_r3_2;
    u32 var_r9;
    u8 *temp_r0;
    u8 *temp_r3;
    u8 *temp_r4;
    u8 *temp_r4_2;
    u8 *temp_r4_3;
    u8 *temp_r6;

    temp_r0 = M2C_FIELD((void *)0x03001E70, void **, 0);
    temp_r3 = M2C_FIELD((void *)0x03001E70, void **, 0x54);
    sp8 = M2C_FIELD(temp_r0, s32 *, 0xE4);
    sp4 = M2C_FIELD((temp_r0 + 0xE4), s32 *, 4);
    var_r7 = temp_r3 + 8;
    var_r9 = 0;
loop_1:
    temp_r3_2 = M2C_FIELD(var_r7, u16 *, 0x1C) + 0xFFFF;
    M2C_FIELD(var_r7, u16 *, 0x1C) = temp_r3_2;
    if ((0xFFFF & temp_r3_2) == 0xFFFF) {

    } else {
        if (Func_080770c0(0x166) != 0) {
            M2C_FIELD(var_r7, u16 *, 0x1C) = (u16) (M2C_FIELD(var_r7, u16 *, 0x1C) + 1);
        }
        temp_r3_3 = M2C_FIELD(var_r7, u16 *, 0x1C) * 0xA;
        temp_r4 = (u8 *)0x0809EF84 + temp_r3_3;
        var_r2 = M2C_FIELD(var_r7, s32 *, 0xC) - sp8;
        if (var_r2 < 0) {
            var_r2 += 0xFFFF;
        }
        temp_r1 = (var_r2 >> 0x10) + M2C_FIELD(temp_r4, s16 *, 0);
        var_r3 = (M2C_FIELD(var_r7, s32 *, 0x14) - M2C_FIELD(var_r7, s32 *, 0x10)) - sp4;
        temp_r4_2 = temp_r4 + 2;
        if (var_r3 < 0) {
            var_r3 += 0xFFFF;
        }
        temp_r0_2 = (var_r3 >> 0x10) + M2C_FIELD(temp_r4, s16 *, 2);
        temp_r4_3 = temp_r4_2 + 2;
        if (((u32) (temp_r1 + 0x10) <= 0xFFU) && (temp_r0_2 >= -0x20) && (temp_r0_2 <= 0x9F)) {
            M2C_FIELD(var_r7, u8 *, 9) = (u8) ((M2C_FIELD(var_r7, u8 *, 9) & ~0xC) | 4);
            M2C_FIELD(var_r7, u16 *, 6) = (u16) ((M2C_FIELD(var_r7, u16 *, 6) & 0xFFFFFE00) | (temp_r1 & 0x1FF));
            M2C_FIELD(var_r7, s8 *, 4) = (s8) temp_r0_2;
            M2C_FIELD(var_r7, u16 *, 8) = (u16) ((M2C_FIELD(var_r7, u16 *, 8) & 0xFFFFFC00) | ((M2C_FIELD(temp_r3, s32 *, 4) + M2C_FIELD(temp_r4_2, u16 *, 2)) & 0x3FF));
            M2C_FIELD(var_r7, u8 *, 5) = (u8) ((0x3F & M2C_FIELD(var_r7, u8 *, 5)) | (M2C_FIELD(temp_r4_3, u8 *, 2) << 6));
            M2C_FIELD(var_r7, u8 *, 7) = (u8) ((0x3F & M2C_FIELD(var_r7, u8 *, 7)) | (M2C_FIELD((temp_r4_3 + 2), u8 *, 2) << 6));
            Func_08003dec(var_r7, 0xF0);
        }
        if (M2C_FIELD(var_r7, u16 *, 0x1C) == 0) {
            temp_r6 = M2C_FIELD(temp_r0, void **, 0);
            temp_r1_2 = M2C_FIELD(temp_r6, s32 *, 0) + (Func_08004458() << 8) + 0xFF800000;
            sp0 = temp_r1_2;
            temp_r0_3 = M2C_FIELD(temp_r6, s32 *, 8) + (Func_08004458() << 8) + 0xFF800000;
            M2C_FIELD(var_r7, s32 *, 0xC) = temp_r1_2;
            M2C_FIELD(var_r7, s32 *, 0x14) = temp_r0_3;
            M2C_FIELD(var_r7, s32 *, 0x10) = (s32) (Func_080091a8(0, temp_r1_2 >> 0x10, temp_r0_3 >> 0x10) << 0x10);
            M2C_FIELD(var_r7, u16 *, 0x1C) = 0x10U;
        }
    }
    var_r9 += 1;
    var_r7 += 0x20;
    if (var_r9 <= 0x1FU) {
        goto loop_1;
    }
}
