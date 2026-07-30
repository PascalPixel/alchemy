typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);

/*
 * Measures an encoded text stream, returning its widest row, total width, and
 * optional per-row spacing values.
 */
void Func_08018a50(s32 arg0, u32 *arg1, u32 *arg2, s16 *arg3) {
    u32 sp4;
    u32 *sp8;
    s16 line_widths[4];
    s16 glyph_counts[4];
    u32 row_widths[16];
    s16 *var_r8;
    s16 var_r4;
    s16 var_r6;
    s32 var_ip;
    s32 var_r0;
    s32 var_r0_3;
    s32 var_r5;
    s32 var_r7;
    u16 temp_r2;
    u16 temp_r3;
    u16 var_r2;
    u32 temp_r2_2;
    u32 temp_r3_2;
    u32 temp_r7;
    u32 var_r4_2;
    u32 var_r6_2;
    u32 var_sl;
    u8 *temp_r5;

    var_r0 = arg0;
    sp8 = arg1;
    var_r8 = arg3;
    temp_r5 = *(void **)0x03001E8C;
    for (var_r5 = 0; var_r5 < 16; var_r5++) {
        row_widths[var_r5] = 0xF;
    }
    sp4 = 0;
    var_sl = 0;
    var_r4 = 0;
    var_r6 = 0;
    var_r7 = 0;
    var_ip = 0;
loop_1:
    temp_r2 = M2C_FIELD(temp_r5, u16 *, (var_r0 * 2) + 0xEB0);
    var_r0 = (var_r0 + 1) & 0x1FF;
    if ((u32) temp_r2 > 0x1FU) {
        if (temp_r2 == 0x20) {
            var_r4 += 5;
            var_r6 += 1;
        } else {
            var_r2 = M2C_FIELD(((temp_r2 - 0x20) << 5), u16 *, 0x08032224);
            temp_r3 = M2C_FIELD(temp_r5, u16 *, 0xEAC);
            if ((temp_r3 == 1) || (temp_r3 == 5)) {
                var_r2 += 1;
            }
            var_r4 += var_r2;
        }
        goto loop_1;
    }
    switch (temp_r2) {
    case 3:
        glyph_counts[sp4] = var_r6 + 1;
        line_widths[sp4] = var_r4;
        if ((var_r7 == 0) && (var_sl < (u32) var_r4)) {
            var_sl = (u32) var_r4;
        }
        if (sp4 <= 2U) {
            temp_r3_2 = sp4 + 1;
            sp4 = temp_r3_2;
            var_ip = temp_r3_2 * 2;
        }
        var_r6 = 0;
        var_r4 = 0;
        row_widths[var_r7] += 0xF;
        goto loop_1;
    case 1:
        var_r6 += 1;
        glyph_counts[sp4] = var_r6;
        line_widths[sp4] = var_r4;
        if ((var_r7 == 0) && (var_sl < (u32) var_r4)) {
            var_sl = (u32) var_r4;
        }
        var_r7 += 1;
        goto loop_1;
    case 14:
    case 28:
        var_r0 = (var_r0 + 1) & 0x1FF;
    case 8:
    case 10:
    case 15:
    case 17:
block_26:
        var_r0 = (var_r0 + 1) & 0x1FF;
        goto loop_1;
    case 9:
        M2C_FIELD(temp_r5, u16 *, 0xEAC) =
            M2C_FIELD(temp_r5, u16 *, (var_r0 * 2) + 0xEB0);
        goto block_26;
    }
    glyph_counts[sp4] = var_r6 + 1;
    line_widths[sp4] = var_r4;
    if ((var_r7 == 0) && (var_sl < (u32) var_r4)) {
        var_sl = (u32) var_r4;
    }
    temp_r7 = var_r7 + 1;
    if (M2C_FIELD(temp_r5, u8 *, 0xEA4) != 0) {
        var_sl += 2;
    }
    var_r4_2 = 0;
    if (temp_r7 > 0U) {
        do {
            if (var_r4_2 == 0) {
                *arg2 = row_widths[0];
            } else {
                temp_r2_2 = row_widths[var_r4_2];
                if ((u32) *arg2 < temp_r2_2) {
                    *arg2 = temp_r2_2;
                }
            }
            var_r4_2 += 1;
        } while (var_r4_2 < temp_r7);
    }
    *sp8 = var_sl;
    if (var_r8 != 0) {
        var_r6_2 = 0;
        var_r5 = 0;
        do {
            if ((u32)glyph_counts[var_r6_2] <= 1U) {
                *var_r8 = 0;
            } else {
                var_r0_3 = (((((u32) (var_sl + 0x13) >> 3) * 8) - 0x10) -
                    line_widths[var_r6_2]) - 4;
                if (var_r0_3 < 0) {
                    var_r0_3 = 0;
                }
                *var_r8 = (s16)Func_080022ec(
                    var_r0_3 << 8, glyph_counts[var_r6_2] - 1);
            }
            var_r8 += 2;
            var_r6_2 += 1;
            var_r5 += 2;
        } while (var_r6_2 <= sp4);
    }
}
