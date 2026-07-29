#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08003dec(void *, s32);
s32 Func_080091a8(s32, s32, s32);

void Func_080912b8(void) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    u8 *temp_r1;
    u8 *temp_r5;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r2;
    s32 temp_r7;
    s32 temp_r7_2;
    s32 var_r6;
    s32 var_r6_2;
    u32 temp_r9;
    u8 temp_r3_3;
    u8 *temp_r3;
    u8 *temp_r3_2;

    temp_r1 = *(u8 **)0x03001EE0;
    temp_r3 = *(u8 **)0x03001E70 + 0xE4;
    sp8 = (s32) M2C_FIELD(temp_r3, s16 *, 2);
    sp4 = (s32) M2C_FIELD(temp_r3, s16 *, 6);
    temp_r3_2 = M2C_FIELD(temp_r1, void **, 0x18);
    if (temp_r3_2 == NULL) {
        return;
    }
    temp_r2 = M2C_FIELD(temp_r3_2, s32 *, 0x10);
    sp0 = (s32) M2C_FIELD(temp_r3_2, s16 *, 0x16);
    temp_r3_3 = M2C_FIELD(temp_r3_2, u8 *, 0x22);
    temp_r7 = M2C_FIELD(temp_r3_2, s32 *, 8) + 0xFFF80000;
    temp_r9 = (u16) *(u16 *)0x03001C8A >> 5;
    var_r6 = Func_080091a8((s32) temp_r3_3, temp_r7, temp_r2 + 0x100000) >> 0x10;
    temp_r0 = (Func_080091a8((s32) temp_r3_3, temp_r7, temp_r2 + 0x200000) >> 0x10) - 0x10;
    if (temp_r0 > var_r6) {
        var_r6 = temp_r0;
    }
    if ((var_r6 > 0) && (var_r6 > sp0)) {
        M2C_FIELD(temp_r1, s32 *, 4) = 0x40000800;
        M2C_FIELD(temp_r1, s32 *, 8) = 0x400;
        M2C_FIELD(temp_r1, u8 *, 9) = (u8) (-0xD & M2C_FIELD(temp_r1, u8 *, 9));
        M2C_FIELD(temp_r1, u16 *, 8) =
            (u16) ((0xFC00 & M2C_FIELD(temp_r1, u16 *, 8)) | (temp_r9 & 0x3FF));
        M2C_FIELD(temp_r1, u8 *, 5) = (u8) ((-0xD & M2C_FIELD(temp_r1, u8 *, 5)) | 4);
        M2C_FIELD(temp_r1, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1, u16 *, 6)) | ((((temp_r7 >> 0x10) & 0xFFF0) - sp8) & 0x1FF));
        M2C_FIELD(temp_r1, s8 *, 4) =
            (s8) ((((temp_r2 >> 0x10) & 0xF0) - sp4 - var_r6) + 0x10);
        Func_08003dec(temp_r1, 0);
    }
    temp_r7_2 = temp_r7 + 0x100000;
    var_r6_2 = Func_080091a8((s32) temp_r3_3, temp_r7_2, temp_r2 + 0x100000) >> 0x10;
    temp_r0_2 = (Func_080091a8((s32) temp_r3_3, temp_r7_2, temp_r2 + 0x200000) >> 0x10) - 0x10;
    temp_r5 = temp_r1 + 0xC;
    if (temp_r0_2 > var_r6_2) {
        var_r6_2 = temp_r0_2;
    }
    if ((var_r6_2 > 0) && (var_r6_2 > sp0)) {
        M2C_FIELD(temp_r5, s32 *, 4) = 0x40000800;
        M2C_FIELD(temp_r5, s32 *, 8) = 0;
        M2C_FIELD(temp_r5, u8 *, 9) = (u8) (-0xD & M2C_FIELD(temp_r5, u8 *, 9));
        M2C_FIELD(temp_r5, u16 *, 8) =
            (u16) ((0xFC00 & M2C_FIELD(temp_r5, u16 *, 8)) | (temp_r9 & 0x3FF));
        M2C_FIELD(temp_r5, u8 *, 5) = (u8) ((-0xD & M2C_FIELD(temp_r5, u8 *, 5)) | 4);
        M2C_FIELD(temp_r5, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r5, u16 *, 6)) | ((((temp_r7_2 >> 0x10) & 0xFFF0) - sp8) & 0x1FF));
        M2C_FIELD(temp_r5, s8 *, 4) =
            (s8) ((((temp_r2 >> 0x10) & 0xF0) - sp4 - var_r6_2) + 0x10);
        Func_08003dec(temp_r5, 0);
    }
}
