#include "types.h"

#define RenderScriptGlyphTiles Func_0801de5c

/*
 * Parses the control-word stream, expands packed glyph rows from resource 19
 * into a temporary 4bpp tile buffer, allocates available render tile slots,
 * and copies the resulting eight tile rows to VRAM.  The unrelated battle
 * template selected by coarse aggregate similarity was not used.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*ClearFn)(void *, s32);
typedef void (*FillFn)(void *, s32, u32);

void *Func_08004938(s32 size);
void *Func_08002f40(s32 resource_id);
extern u8 Value_00000013;

s32 RenderScriptGlyphTiles(
    u16 *script, u16 *tile_out, u16 *mirror_out, s32 start_column, s32 unused) {
    s32 sp0;
    u8 *sp4;
    u8 *sp8;
    u16 *spC;
    u16 *sp10;
    u8 palette_map[16];
    s32 *var_r0_252;
    s32 *var_r0_312;
    s32 *var_r5_246;
    s32 temp_r1_265;
    s32 temp_r1_272;
    s32 temp_r2_431;
    s32 temp_r3_281;
    s32 var_r4_173;
    u16 *var_r6_23;
    s32 var_r8_18;
    s32 var_r9_169;
    s32 var_r9_256;
    void *temp_fp_25;
    u16 temp_r1_130;
    u16 temp_r1_236;
    u16 temp_r3_425;
    u32 *var_r5_167;
    u32 temp_r2_184;
    u32 temp_r2_266;
    u32 temp_r2_273;
    u32 temp_r3_90;
    u32 temp_r6_245;
    u32 var_r1_329;
    u32 var_r1_347;
    u32 var_r2_172;
    u32 var_r4_263;
    u32 var_r4_351;
    u32 var_r5_321;
    u32 var_r9_317;
    u8 temp_r2_320;
    u8 temp_r3_179;
    u8 temp_r3_189;
    u8 var_r3_153;
    void *var_r0_168;

    sp10 = tile_out;
    spC = mirror_out;
    var_r8_18 = start_column;
    var_r6_23 = script;
    temp_fp_25 = *(void **)0x03001e8c;
    sp8 = Func_08004938(0x800);
    sp4 = Func_08002f40((s32)&Value_00000013);
    sp0 = M2C_FIELD(temp_fp_25, u8 *, 0xEA7) << 0xC;
    ((ClearFn)0x03000164)(palette_map, 0x10);
    if (sp0 == 0xF000) {
        M2C_FIELD(palette_map, u8 *, 1) = (u8) *((u8 *)0x080371B4 + (0xF & M2C_FIELD(temp_fp_25, u16 *, 0xEAE)));
        M2C_FIELD(palette_map, s8 *, 3) = 3;
        ((FillFn)0x03000168)(sp8, 0x800, 0x04040404);
    } else {
        M2C_FIELD(palette_map, u8 *, 1) = (u8) (0xF & (u8) M2C_FIELD(temp_fp_25, u16 *, 0xEAE));
        M2C_FIELD(palette_map, s8 *, 3) = 1;
        ((FillFn)0x03000168)(sp8, 0x800, 0x0E0E0E0E);
    }
    if (var_r6_23 == 0) {

    } else {
loop_33:
        temp_r1_236 = *var_r6_23;
        var_r6_23 += 2;
        if (temp_r1_236 != 0) {
            if ((u32) temp_r1_236 <= 0x1EU) {
                temp_r3_90 = temp_r1_236 - 3;
                switch (temp_r3_90) {               /* irregular */
                case 5:
                    temp_r1_130 = *var_r6_23;
                    M2C_FIELD(temp_fp_25, u16 *, 0xEAE) = temp_r1_130;
                    var_r6_23 += 2;
                    M2C_FIELD(palette_map, u8 *, 1) = (u8) *((u8 *)0x080371B4 + (0xF & temp_r1_130));
                    break;
                case 4:
                case 6:
                case 7:
                    M2C_FIELD(temp_fp_25, u16 *, 0xEAE) = 0xFU;
                    M2C_FIELD(palette_map, u8 *, 1) = (u8) *(u8 *)0x080371C3;
                    break;
                case 0:
                    var_r3_153 = *(u8 *)0x080370D4;
                    goto block_32;
                case 11:
                case 12:
                case 25:
                    var_r6_23 += 2;
                    /* fallthrough */
                case 8:
                case 9:
                case 14:
                case 26:
                    var_r6_23 += 2;
                    break;
                }
            } else {
                var_r5_167 = sp4 + ((u8) temp_r1_236 << 5);
                var_r0_168 = sp8 + var_r8_18;
                var_r9_169 = 0;
                do {
                    var_r2_172 = *var_r5_167;
                    var_r5_167 += 4;
                    var_r4_173 = 3;
loop_18:
                    temp_r3_179 = palette_map[var_r2_172 & 0xF];
                    if (temp_r3_179 != 0) {
                        M2C_FIELD(var_r0_168, u8 *, 0) = temp_r3_179;
                    }
                    temp_r2_184 = var_r2_172 >> 4;
                    temp_r3_189 = palette_map[temp_r2_184 & 0xF];
                    if (temp_r3_189 != 0) {
                        M2C_FIELD(var_r0_168, u8 *, 1) = temp_r3_189;
                    }
                    var_r4_173 -= 1;
                    var_r0_168 = var_r0_168 + 1 + 1;
                    var_r2_172 = temp_r2_184 >> 4;
                    if (var_r4_173 >= 0) {
                        goto loop_18;
                    }
                    var_r9_169 += 1;
                    var_r0_168 += 0xF8;
                } while (var_r9_169 <= 7);
                if ((temp_r1_236 == 0xF01D) || (temp_r1_236 == 0xF01F)) {
                    var_r8_18 += 8;
                } else {
                    if (temp_r1_236 == 0xF01E) {
                        var_r3_153 = 3;
                    } else if ((u32) (u8) temp_r1_236 > 0x1FU) {
                        var_r3_153 = *((u8 *)0x080370D4 + (s32) ((u8) temp_r1_236 - 0x20));
                    } else {
                        var_r3_153 = 1;
                    }
block_32:
                    var_r8_18 += var_r3_153;
                }
            }
            goto loop_33;
        }
    }
    temp_r6_245 = (u32) (var_r8_18 + 7) >> 3;
    var_r5_246 = sp8;
    var_r0_252 = var_r5_246;
    var_r9_256 = 7;
    do {
        if (temp_r6_245 != 0) {
            var_r4_263 = temp_r6_245;
            do {
                temp_r1_265 = M2C_FIELD(var_r5_246, s32 *, 0);
                temp_r2_266 = M2C_FIELD(var_r5_246, u32 *, 4);
                temp_r1_272 = (temp_r1_265 | (temp_r1_265 * 0x10)) & 0x0FF00FF0;
                temp_r2_273 = (temp_r2_266 | (temp_r2_266 >> 4)) & 0xFF00FF;
                temp_r3_281 = ((u32) ((temp_r1_272 | (temp_r1_272 << 8)) * 0x10) >> 0x10) | ((temp_r2_273 | (temp_r2_273 >> 8)) << 0x10);
                var_r4_263 -= 1;
                var_r5_246 += 8;
                *var_r0_252 = temp_r3_281;
                var_r0_252 += 4;
            } while (var_r4_263 != 0);
        }
        var_r0_252 = (var_r0_252 - (temp_r6_245 * 4)) + 0x100;
        var_r9_256 -= 1;
        var_r5_246 = (var_r5_246 - (temp_r6_245 * 8)) + 0x100;
    } while (var_r9_256 >= 0);
    if (temp_r6_245 == 0) {

    } else {
        var_r0_312 = sp8;
        var_r9_317 = temp_r6_245;
loop_43:
        temp_r2_320 = M2C_FIELD(temp_fp_25, u8 *, 0xEA2);
        var_r5_321 = 0x7F;
        if (temp_r2_320 != 0) {
            var_r5_321 = 0xFF;
        }
        var_r1_329 = 0x3FF & *sp10;
        if (((u32) (var_r1_329 - 0x80) > 0x7FU) && ((temp_r2_320 == 0) || (var_r1_329 < 0x200U) || (var_r1_329 >= 0x280U))) {
            var_r1_347 = M2C_FIELD(temp_fp_25, u16 *, 0xEA0) & var_r5_321;
            var_r4_351 = 0;
            if (*((u8 *)temp_fp_25 + (var_r1_347 + 0xDA0)) != 0) {
loop_50:
                var_r4_351 += 1;
                var_r1_347 = (var_r1_347 + 1) & var_r5_321;
                if (var_r4_351 <= var_r5_321) {
                    if (*((u8 *)temp_fp_25 + (var_r1_347 + 0xDA0)) != 0) {
                        goto loop_50;
                    }
                }
            }
            M2C_FIELD(temp_fp_25, u16 *, 0xEA0) = (u16) ((var_r1_347 + 1) & var_r5_321);
            *((u8 *)temp_fp_25 + (var_r1_347 + 0xDA0)) = 1;
            if (var_r1_347 > 0x7FU) {
                var_r1_329 = var_r1_347 + 0x180;
            } else {
                var_r1_329 = var_r1_347 | 0x80;
            }
            temp_r3_425 = sp0 | var_r1_329;
            *sp10 = temp_r3_425;
            *spC = temp_r3_425;
        }
        temp_r2_431 = var_r1_329 << 5;
        *(s32 *)((u8 *)0x06000000 + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0);
        *(s32 *)((u8 *)0x06000004 + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x100);
        *(s32 *)((u8 *)0x06000008 + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x200);
        *(s32 *)((u8 *)0x0600000C + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x300);
        *(s32 *)((u8 *)0x06000010 + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x400);
        *(s32 *)((u8 *)0x06000014 + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x500);
        *(s32 *)((u8 *)0x06000018 + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x600);
        *(s32 *)((u8 *)0x0600001C + temp_r2_431) = (s32) M2C_FIELD(var_r0_312, s32 *, 0x700);
        var_r9_317 -= 1;
        sp10 += 2;
        spC += 2;
        var_r0_312 += 4;
        if (var_r9_317 != 0) {
            goto loop_43;
        }
    }
    Func_08002df0(sp8);
    return (s32) temp_r6_245;
}
