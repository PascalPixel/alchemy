#include "types.h"

#define ApplyProjectedRenderPlacement Func_0800b168

/* Converts fixed-point position and scale inputs into the primary and optional
 * secondary render records, applies viewport offsets and size-dependent
 * bounds, then submits each visible record to the render queue. */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

u32 Func_0800aa0c(void *object, u16 mode);
s32 Func_08003d28(void);
void Func_08003dec(void *object, s32 priority);

void ApplyProjectedRenderPlacement(
    void *object, s32 *position, s32 *scale, u16 mode) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    u8 *sp14;
    u32 sp18;
    u32 sp1C;
    s32 sp20;
    s32 packed[2];
    s32 temp_r0_49;
    s32 temp_r10_45;
    s32 temp_r11_48;
    s32 temp_r1_261;
    s32 temp_r1_90;
    s32 temp_r4_217;
    s32 temp_r6_145;
    s32 temp_r6_238;
    s32 temp_r6_44;
    s32 temp_r8_40;
    s32 temp_r9_37;
    s32 var_r4_132;
    s32 var_r5_152;
    void *temp_r0_158;
    s32 *position_cursor;
    s32 *scale_cursor;

    void *window = object;

    sp1C = (u8) M2C_FIELD(window, u8 *, 0x20) >> 1;
    sp14 = (u8 *)window + 0x21;
    sp18 = (u8)M2C_FIELD(window, u8 *, 0x21) >> 1;
    sp10 = 8;
    spC = 4;
    scale_cursor = scale;
    position_cursor = position;
    temp_r8_40 = *scale_cursor++;
    temp_r10_45 = *position_cursor++;
    temp_r9_37 = *scale_cursor;
    sp4 = *position_cursor++;
    temp_r11_48 = *position_cursor++;
    temp_r6_44 = *position_cursor;
    temp_r0_49 = Func_0800aa0c(window, mode);
    if ((temp_r0_49 == 0) && (temp_r8_40 == 0x10000) && (temp_r9_37 == temp_r8_40)) {
        if (M2C_FIELD(window, u16 *, 0x1E) == 0) {
            sp8 = 0;
            sp20 = 0;
        } else {
            goto block_6;
        }
    } else {
block_6:
        sp8 = 1;
        M2C_FIELD(packed, s32 *, 4) = (s32) ((M2C_FIELD(packed, s32 *, 4) & 0xFFFF0000) | M2C_FIELD(window, u16 *, 0x1E));
        temp_r1_90 = (u16) ((packed[0] & 0xFFFF0000) | ((u32) (temp_r8_40 << 8) >> 0x10)) | (((u32) (temp_r9_37 << 8) >> 0x10) << 0x10);
        packed[0] = temp_r1_90;
        if (temp_r0_49 != 0) {
            packed[0] = (0xFFFF0000 & temp_r1_90) | (u16) (0 - M2C_FIELD(packed, u16 *, 0));
        }
        sp20 = Func_08003d28();
    }
    if ((temp_r8_40 > 0x10000) || (temp_r9_37 > 0x10000)) {
        sp8 = 3;
        sp1C *= 2;
        sp18 *= 2;
        sp10 = 0x10;
        spC = 8;
    }
    if (sp4 <= 0xFF9C0000) {
        sp0 = 1;
        var_r4_132 = 0;
    } else {
        sp0 = (temp_r11_48 >> 0x11) + 0xA;
        var_r4_132 = 2;
    }
    temp_r6_145 = ((s32) (temp_r11_48 - temp_r6_44) >> 0x10) - spC;
    var_r5_152 = temp_r10_45 >> 0x10;
    if (1 & M2C_FIELD(window, u8 *, 0x26)) {
        if (temp_r6_145 <= 0x9F) {
            temp_r0_158 = window + 0xC;
            M2C_FIELD(temp_r0_158, u8 *, 5) = (u8) ((-4 & M2C_FIELD(temp_r0_158, u8 *, 5)) | sp8);
            M2C_FIELD(temp_r0_158, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(temp_r0_158, u8 *, 7)) | ((sp20 & 0x1F) * 2));
            M2C_FIELD(temp_r0_158, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r0_158, u16 *, 6)) | ((var_r5_152 - sp10) & 0x1FF));
            M2C_FIELD(temp_r0_158, s8 *, 4) = (s8) temp_r6_145;
            Func_08003dec(temp_r0_158, var_r4_132);
        } else {
            var_r5_152 = temp_r10_45 >> 0x10;
        }
    }
    temp_r4_217 = (var_r5_152 - sp1C) + ((s32) ((temp_r8_40 * (s8) M2C_FIELD(window, u8 *, 0x22)) + 0xFFFF) >> 0x10);
    temp_r6_238 = (((s32) (temp_r11_48 - sp4) >> 0x10) - sp18) - ((s32) ((temp_r9_37 * (((u8) *sp14 >> 1) - (s8) M2C_FIELD(window, u8 *, 0x23))) + 0xFFFF) >> 0x10);
    if ((temp_r4_217 <= 0xEF) && (temp_r6_238 <= 0x9F)) {
        M2C_FIELD(window, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(window, u16 *, 6)) | (temp_r4_217 & 0x1FF));
        M2C_FIELD(window, s8 *, 4) = (s8) temp_r6_238;
        M2C_FIELD(window, u8 *, 5) = (u8) ((-4 & M2C_FIELD(window, u8 *, 5)) | sp8);
        temp_r1_261 = sp20 & 0x1F;
        sp20 = temp_r1_261;
        M2C_FIELD(window, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(window, u8 *, 7)) | (temp_r1_261 * 2));
        Func_08003dec(window, sp0);
    }
}
