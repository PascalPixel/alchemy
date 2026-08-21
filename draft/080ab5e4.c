#include "types.h"

/*
 * Colosso tournament selection and progression handler.
 *
 * This draft was reconstructed mechanically from the owner's complete
 * reference assembly, including its jump table and literal pools.  Most
 * locals are still m2c's raw sp/temp/reg names; renaming the rest needs the
 * same per-site tracing this file has already had for a few spots:
 *
 *   - ctx: the save/session pointer loaded from 0x03001F2C once at entry
 *     and reused for every M2C_FIELD access below.
 *   - statusPtr: a write cursor over the local status[8] array (one byte
 *     per tournament slot), cleared to 0 in the entry loop.
 *   - mode: arg0, compared against 0 and 1 to select which per-slot status
 *     pass and layout call run; a third mode is handled by the fallback
 *     path further down.
 *
 * differing_halfwords=2401 at 4728/4888 bytes; still a genuine mismatch in
 * structure, not just naming, so this pass is cosmetic only. Names and
 * aggregate types elsewhere remain provisional; keep every edit tied to
 * local evidence and score it through gs1cc.
 */
typedef s32 M2C_UNK;
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((s8 *)(expr) + (offset)))

s32 Func_080ab5e4(s32 arg0) {
    s8 status[8];
    s32 *win10C;
    s32 sp0;
    s32 sp4;
    u32 sp8;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s8 *statusPtr;
    s32 sp20;
    s32 sp24;
    u32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    void *ctx;
    s32 mode;
    u8 buf54[8];
    s32 *var_r2_1122;
    s32 temp_r0_119;
    s32 temp_r0_2083;
    s32 temp_r0_2233;
    s32 temp_r0_2301;
    s32 temp_r0_2363;
    s32 temp_r1_126;
    s32 temp_r1_1953;
    s32 temp_r1_1966;
    s32 temp_r1_2240;
    s32 temp_r1_2308;
    s32 temp_r1_910;
    s32 temp_r2_25;
    s32 temp_r3_1572;
    s32 temp_r3_1597;
    s32 temp_r3_1908;
    s32 temp_r3_1936;
    s32 temp_r3_2211;
    s32 temp_r3_2279;
    s32 temp_r3_88;
    s32 temp_r5_1001;
    s32 temp_r5_1029;
    s32 temp_r5_1083;
    s32 temp_r5_1163;
    s32 temp_r5_1278;
    s32 temp_r5_1327;
    s32 temp_r5_1374;
    s32 temp_r5_1702;
    s32 temp_r5_1817;
    s32 temp_r5_1872;
    s32 temp_r5_95;
    s32 temp_r6_1509;
    s32 temp_r6_216;
    s32 temp_r6_339;
    s32 temp_r6_601;
    s32 temp_r7_312;
    s32 temp_r7_514;
    s32 temp_r7_723;
    s32 var_fp_905;
    s32 var_r0_255;
    s32 var_r3_1124;
    s32 var_r5_1634;
    s32 var_r5_1785;
    s32 var_r5_237;
    s32 var_r6_240;
    s32 var_r8_1869;
    s8 *var_r0_148;
    s8 *var_r1_151;
    s8 *var_r1_82;
    s8 *var_r2_152;
    s8 *var_r3_61;
    s8 var_r1_2077;
    s8 var_r1_2127;
    s8 var_r1_2319;
    u16 *var_sl_1853;
    u16 temp_r0_47;
    u16 temp_r2_218;
    u16 temp_r2_2357;
    u16 temp_r2_424;
    u16 temp_r5_204;
    u16 temp_r5_34;
    u16 temp_r6_199;
    u16 temp_r7_1856;
    u32 temp_r3_921;
    u32 temp_r4_1863;
    u32 temp_r5_1508;
    u32 temp_r5_1573;
    u32 temp_r5_1598;
    u32 temp_r5_1909;
    u32 temp_r5_1937;
    u32 temp_r5_515;
    u32 temp_r5_596;
    u32 temp_r5_724;
    u32 temp_r6_1574;
    u32 temp_r6_1599;
    u32 temp_r6_1701;
    u32 temp_r6_1871;
    u32 temp_r6_1910;
    u32 temp_r6_1938;
    u32 temp_r6_509;
    u32 temp_r6_719;
    u32 temp_r7_1504;
    u32 temp_r7_1696;
    u32 temp_r7_600;
    u32 var_r3_581;
    u32 var_r4_1290;
    u32 var_r4_1337;
    u32 var_r4_1384;
    u32 var_r4_1515;
    u32 var_r4_606;
    u32 var_r4_907;
    u8 *var_r2_85;
    u8 temp_r2_2035;
    u8 temp_r2_2137;
    u8 temp_r2_2165;
    void *temp_r3_18;
    void *temp_r5_343;
    void *temp_r9_32;

    mode = arg0;
    temp_r3_18 = *(void **)0x03001F2C;
    ctx = temp_r3_18;
    temp_r9_32 = M2C_FIELD(temp_r3_18, void **, 0x184);
    temp_r2_25 = mode * 2;
    sp48 = 1;
    sp34 = temp_r2_25;
    temp_r5_34 = M2C_FIELD(ctx, u16 *, temp_r2_25 + 0x174);
    sp38 = (u16) Func_08002304(temp_r5_34, 0xA);
    temp_r0_47 = Func_080022f4(temp_r5_34, 0xA);
    statusPtr = status;
    sp30 = (s32) temp_r0_47;
    sp2C = 0;
    sp28 = 0;
    sp24 = 0;
    sp20 = 0;
    sp3C = -1;
    var_r3_61 = &status[7];
    do {
        *var_r3_61 = 0;
        var_r3_61 -= 1;
    } while ((s32) var_r3_61 >= (s32) statusPtr);
    if (mode == 0) {
        Func_080aafb8(temp_r9_32);
        sp44 = 0;
        if (mode < (s32) M2C_FIELD(ctx, u8 *, 0x219)) {
            var_r1_82 = statusPtr;
            var_r2_85 = temp_r9_32 + 0xA0;
            do {
                temp_r3_88 = *var_r2_85 << 0x18;
                var_r2_85 += 1;
                if (temp_r3_88 == 0) {
                    *var_r1_82 = 4;
                }
                temp_r5_95 = sp44 + 1;
                sp44 = temp_r5_95;
                var_r1_82 += 1;
            } while (temp_r5_95 < (s32) M2C_FIELD(ctx, u8 *, 0x219));
        }
        sp2C = sp30;
        sp44 = 0;
        if ((s32) M2C_FIELD(ctx, u8 *, 0x219) > 0) {
            do {
                if (statusPtr[sp38] == 4) {
                    temp_r0_119 = sp38 + 1;
                    sp38 = temp_r0_119;
                    sp38 = Func_080aa538(temp_r0_119, M2C_FIELD(ctx, u8 *, 0x219));
                }
                temp_r1_126 = sp44 + 1;
                sp44 = temp_r1_126;
            } while (temp_r1_126 < (s32) M2C_FIELD(ctx, u8 *, 0x219));
        }
    } else {
        Func_080ae714(buf54, M2C_FIELD(ctx, s8 *, 0x1C));
        sp44 = 0;
        if ((s32) M2C_FIELD(ctx, u8 *, 0x219) > 0) {
            var_r0_148 = statusPtr;
            var_r1_151 = temp_r9_32 + 0xA0;
            var_r2_152 = var_r0_148;
            do {
                if (sp44 == M2C_FIELD(ctx, s8 *, 0x1C)) {
                    *var_r2_152 = 7;
                } else if (buf54[sp44] != 0) {
                    *var_r2_152 = 0;
                } else {
                    *var_r2_152 = 3;
                    if (*var_r1_151 == 0) {
                        *var_r0_148 = 7;
                    }
                }
                sp44 += 1;
                var_r0_148 += 1;
                var_r1_151 += 1;
                var_r2_152 += 1;
            } while (sp44 < (s32) M2C_FIELD(ctx, u8 *, 0x219));
        }
    }
    if (mode == 1) {
        temp_r6_199 = M2C_FIELD(ctx, u16 *, 0x174);
        temp_r5_204 = Func_08002304(temp_r6_199, 0xA);
        temp_r6_216 = (temp_r5_204 * 7) + 1;
        temp_r2_218 = Func_080022f4(temp_r6_199, 0xA);
        sp4 = 0xE;
        sp0 = mode;
        Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), temp_r6_216, temp_r2_218 + 2, 6);
        sp0 = 7;
        sp4 = 6;
        Func_080ab2ec(M2C_FIELD(ctx, s32 *, 0x30), temp_r6_216, 2);
        var_r5_237 = 0;
        if ((s32) M2C_FIELD(ctx, u8 *, 0x219) > 0) {
            var_r6_240 = 8;
            do {
                if (var_r5_237 == M2C_FIELD(ctx, s8 *, 0x1C)) {
                    if (M2C_FIELD(ctx, u16 *, 0x178) & 0x8000) {
                        var_r0_255 = 0xBB0;
                    } else {
                        var_r0_255 = 0xBAF;
                    }
                } else if (2 & (u8) statusPtr[var_r5_237]) {
                    var_r0_255 = 0xBAE;
                } else {
                    var_r0_255 = 0xBB1;
                }
                Func_08015080(var_r0_255, M2C_FIELD(ctx, s32 *, 0x30), var_r6_240, 8);
                var_r5_237 += 1;
                var_r6_240 += 0x38;
            } while (var_r5_237 < (s32) M2C_FIELD(ctx, u8 *, 0x219));
        }
    }
    win10C = (s32 *) ((s8 *) temp_r3_18 + 0x10C);
    Func_08015270((*win10C));
    M2C_FIELD(M2C_FIELD(ctx, void **, 0x14), s8 *, 5) = 1;
    sp18 = sp38 * 8;
loop_36:
    temp_r7_312 = sp48;
    if (temp_r7_312 == 0) {

    } else {
        sp48 = 0;
        sp3C = -1;
        if (!(1 & (u8) statusPtr[sp38])) {
            sp3C = sp30;
        }
        temp_r6_339 = M2C_FIELD(ctx, s32 *, 0x10);
        temp_r5_343 = Func_08077008(M2C_FIELD(ctx, u16 *, (sp38 * 2) + 0x208));
        Func_08015270(temp_r6_339);
        Func_08015090(temp_r5_343, temp_r6_339, 0, 0);
        Func_08015080(M2C_FIELD(temp_r5_343, u8 *, 0x129) + 0x741, temp_r6_339, 0, 8);
        Func_08015090((void *)0x080AF28C, temp_r6_339, 0x30, 0);
        sp0 = sp48;
        Func_080150b0(M2C_FIELD(temp_r5_343, u8 *, 0xF), 2, temp_r6_339, 0x48);
        if (mode == 0) {
            Func_08015080(0xBA9, temp_r6_339, 0, 0x10);
        }
        if (sp3C != -1) {
            sp28 = (u32) M2C_FIELD(temp_r9_32, u16 *, ((sp38 * 0xA) + sp3C) * 2);
        }
        Func_08015270((*win10C));
        if (mode == 1) {
            Func_08015120(M2C_FIELD(ctx, u8 *, 0x21A), 1);
            sp8 = 0xBB2;
            Func_08015080(0xBB2, (*win10C), 0, 0);
            temp_r2_424 = M2C_FIELD(ctx, u16 *, 0x178);
            Func_08015120((((u32) (0xE0 & temp_r2_424) >> 5) * 0x14) + (0x1F & temp_r2_424) + 0x12C, 4);
            sp0 = sp48;
            Func_08015280((*win10C), ((u32) (0xE0 & M2C_FIELD(ctx, u16 *, 0x178)) >> 5) + 0x5001, 6, 0);
            Func_08015080(0xBB3, (*win10C), 0x38, 0);
            Func_08015080(0xBB4, (*win10C), 0, 8);
        }
        if (sp3C == -1) {
            Func_080ad5b4(mode, 0, 0xC8, 0);
        } else if (mode == 0) {
            if (sp24 != 0) {
                if (sp20 == 0) {
                    Func_08015080(0xB98, (*win10C), 0, 0);
                } else {
                    Func_08015080(0xB99, (*win10C), 0, 0);
                }
                temp_r6_509 = (u32) (0xF00 & sp28) >> 8;
                temp_r7_514 = 0x1F & sp28;
                temp_r5_515 = (u32) (0xE0 & sp28) >> 5;
                if ((Func_08077210(temp_r6_509, temp_r5_515, temp_r7_514) != 0) || (Func_08077208(temp_r6_509, temp_r5_515, temp_r7_514) != 0)) {
                    if (Func_08077210(temp_r6_509, temp_r5_515, temp_r7_514) != 0) {
                        Func_080ad608(mode, temp_r5_515, 1);
                    } else {
                        Func_080ad608(mode, temp_r5_515, 2);
                    }
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                } else {
                    Func_08015080(0xB9E, (*win10C), 0, 0x10);
                    Func_080ad608(mode, temp_r5_515, 1);
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x3E, 1);
                }
                var_r3_581 = (u32) temp_r7_312 >> 1;
            } else {
                sp8 = 0xB9A;
                Func_08015080(0xB9A, (*win10C), 0, 0);
                temp_r5_596 = (u32) (0xF00 & sp28) >> 8;
                temp_r7_600 = (u32) (0xE0 & sp28) >> 5;
                temp_r6_601 = 0x1F & sp28;
                var_r4_606 = 0xB9A;
                if ((Func_08077210(temp_r5_596, temp_r7_600, temp_r6_601) != 0) || (var_r4_606 = sp8, (Func_08077208(temp_r5_596, temp_r7_600, temp_r6_601) != 0))) {
                    sp8 = var_r4_606;
                    if (Func_08077210(temp_r5_596, temp_r7_600, temp_r6_601) != 0) {
                        Func_08015080(var_r4_606 + 3, (*win10C), 0, 0x10);
                        Func_080ad608(0, temp_r7_600, 1);
                    } else {
                        Func_08015080(var_r4_606 + 2, (*win10C), 0, 0x10);
                        Func_080ad608(0, temp_r7_600, 2);
                    }
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                } else {
                    Func_08015080(var_r4_606 + 4, (*win10C), 0, 0x10);
                    Func_080ad608(0, temp_r7_600, 1);
                    Func_080ad5b4(0, ((sp18 - sp38) * 8) + 0x30, 0x3E, 1);
                }
                var_r3_581 = (u32) temp_r7_312 >> 1;
            }
            if (var_r3_581 != 0) {
                Func_080ad5f4(mode, 0);
            }
        } else {
            temp_r6_719 = (u32) (0xF00 & sp28) >> 8;
            temp_r7_723 = 0x1F & sp28;
            temp_r5_724 = (u32) (0xE0 & sp28) >> 5;
            if ((Func_08077210(temp_r6_719, temp_r5_724, temp_r7_723) != 0) || (Func_08077208(temp_r6_719, temp_r5_724, temp_r7_723) != 0)) {
                if (Func_08077210(temp_r6_719, temp_r5_724, temp_r7_723) != 0) {
                    Func_080ad608(mode, temp_r5_724, 1);
                } else {
                    Func_080ad608(mode, temp_r5_724, 2);
                }
                Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x36, 0);
            } else {
                Func_080ad608(mode, temp_r5_724, 1);
                Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x36, 1);
            }
            if (((u32) temp_r7_312 >> 1) != 0) {
                Func_080ad5f4(mode, 0);
            }
        }
        Func_08015278(M2C_FIELD(ctx, s32 *, 0x30));
        if (sp3C != -1) {
            Func_08015080(0xBAD, M2C_FIELD(ctx, s32 *, 0x30), 0, 0x50);
            sp0 = 0x68;
            Func_08015068(M2C_FIELD(ctx, s32 *, 0x30), 0, 0x60, 0xE0);
            Func_08015080((((u32) (0xE0 & sp28) >> 5) * 0x14) + (0x1F & sp28) + 0x666, M2C_FIELD(ctx, s32 *, 0x30), 0, 0x60);
        }
        if (!(1 & (u8) statusPtr[sp38])) {
            sp4 = 0xE;
            sp0 = 1;
            Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6);
        }
        M2C_FIELD(*(void **)0x03001E8C, s8 *, 0xEA3) = 1;
    }
    if (1 & (u8) statusPtr[sp38]) {
        Func_080a1a40(((sp18 - sp38) * 8) - 8, 0x34);
    } else {
        Func_080a1a40(((sp18 - sp38) * 8) - 8, (sp30 * 8) + 0x3C);
    }
    Func_080030f8(1);
    if (!(*(s32 *)0x03001AE8 & 0x100) || (*(s32 *)0x03001AF8 & 0x100)) {
        if (sp24 != 0) {
            sp48 = 1;
        }
        sp24 = 0;
        sp20 = 0;
    }
    var_fp_905 = *(s32 *)0x03001B04;
    var_r4_907 = *(u32 *)0x03001C94;
    temp_r1_910 = M2C_FIELD(temp_r9_32, s32 *, 0x212C);
    if (temp_r1_910 == 0) {

    } else {
        var_r4_907 = 0;
        M2C_FIELD(temp_r9_32, s32 *, 0x2128) = (s32) (M2C_FIELD(temp_r9_32, s32 *, 0x2128) + 1);
        temp_r3_921 = temp_r1_910 - 1;
        var_fp_905 = 0;
        switch (temp_r3_921) {                      /* irregular */
        case 27:
            if (!(*(u32 *)0x03001C94 & 1)) {
                do {
                    Func_080a1a40(0x96, 0x1A);
                    Func_080030f8(1);
                } while (!(*(u32 *)0x03001C94 & 1));
            }
            var_r4_907 = 2;
            var_fp_905 = 2;
            break;
        case 26:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x3C) {

            } else {
                temp_r5_1001 = Func_08015038(0xC4C, 9, 9, 1);
                *(s8 *)0x0200044C = 1;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                Func_08015018(temp_r5_1001, 1);
                Func_080aafb8(temp_r9_32);
                Func_080030f8(1);
                temp_r5_1029 = Func_08015038(0xC4D, 9, 9, 1);
                *(u8 *)0x0200044C = 1;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                Func_08015018(temp_r5_1029, 1);
                Func_080aafb8(temp_r9_32);
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                Func_080b50f8();
                Func_080b50f8();
                Func_080b50f8();
                Func_080771b8(0U, 0U, 0);
                Func_080771c8(0U, 0U, 0);
                Func_08077010(0U);
                var_fp_905 = 2;
block_167:
                var_r4_907 = 2;
            }
            break;
        case 0:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x3C) {

            } else {
                sp8 = 0;
                temp_r5_1083 = Func_08015038(0xC40, 9, 9, 1);
                *(u8 *)0x0200044C = 1;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                Func_08015018(temp_r5_1083, 1);
                Func_080aafb8(temp_r9_32);
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = 2;
                var_r4_907 = sp8;
            }
            break;
        case 1:
        case 3:
        case 7:
        case 8:
        case 21:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x5A) {

            } else {
                var_fp_905 = 1;
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                var_r2_1122 = temp_r9_32 + 0x212C;
                var_r3_1124 = M2C_FIELD(temp_r9_32, s32 *, 0x212C) + 1;
block_162:
                var_r4_907 = 1;
                *var_r2_1122 = var_r3_1124;
            }
            break;
        case 2:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x5A) {

            } else {
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                var_fp_905 = 0x10;
                var_r4_907 = 0x10;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = 4;
            }
            break;
        case 5:
        case 6:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x3C) {

            } else {
                sp8 = 0;
                temp_r5_1163 = Func_08015038(0xC41, 9, 9, 1);
                *(u8 *)0x0200044C = 1;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                Func_08015018(temp_r5_1163, 1);
                Func_080aafb8(temp_r9_32);
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = 8;
                var_r4_907 = sp8;
            }
            break;
        case 12:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x28) {

            } else {
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                var_fp_905 = 2;
                var_r4_907 = 2;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = (s32) (M2C_FIELD(temp_r9_32, s32 *, 0x212C) + 1);
            }
            break;
        case 13:
        case 15:
        case 16:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x28) {

            } else {
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = (s32) (M2C_FIELD(temp_r9_32, s32 *, 0x212C) + 1);
            }
            break;
        case 14:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) != 0x3C) {

            } else {
                *(u8 *)0x0200044C = 1;
                sp8 = 0;
                temp_r5_1278 = Func_08015038(0xC44, 9, 9, 1);
                Func_080a1ac0(2, 0x92);
                var_r4_1290 = sp8;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                if (!(*(u32 *)0x03001C94 & 1)) {
                    do {
                        sp8 = var_r4_1290;
                        Func_080a1a40(2, 0x92);
                        Func_080030f8(1);
                    } while (!(*(u32 *)0x03001C94 & 1));
                }
                sp8 = var_r4_1290;
                Func_08015018(temp_r5_1278, 1);
                Func_080aafb8(temp_r9_32);
                Func_080030f8(1);
                temp_r5_1327 = Func_08015038(0xC45, 9, 9, 1);
                var_r4_1337 = sp8;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                if (!(*(u32 *)0x03001C94 & 1)) {
                    do {
                        sp8 = var_r4_1337;
                        Func_080a1a40(2, 0x92);
                        Func_080030f8(1);
                    } while (!(*(u32 *)0x03001C94 & 1));
                }
                sp8 = var_r4_1337;
                Func_08015018(temp_r5_1327, 1);
                Func_080aafb8(temp_r9_32);
                Func_080030f8(1);
                temp_r5_1374 = Func_08015038(0xC46, 9, 9, 1);
                var_r4_1384 = sp8;
                while (Func_08015048() == 0) {
                    Func_080030f8(1);
                }
                if (!(*(u32 *)0x03001C94 & 1)) {
                    do {
                        sp8 = var_r4_1384;
                        Func_080a1a40(2, 0x92);
                        Func_080030f8(1);
                    } while (!(*(u32 *)0x03001C94 & 1));
                }
                sp8 = var_r4_1384;
                Func_08015018(temp_r5_1374, 1);
                Func_080aafb8(temp_r9_32);
                Func_080030f8(1);
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = 0x10;
                var_r4_907 = sp8;
            }
            break;
        case 17:
        case 19:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) == 0x5A) {
                var_fp_905 = 1;
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                var_r3_1124 = 0x15;
                var_r2_1122 = temp_r9_32 + 0x212C;
                goto block_162;
            }
            break;
        case 20:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) == 0x5A) {
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                var_fp_905 = 0x20;
                var_r4_907 = 0x20;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = 0x16;
            }
            break;
        case 23:
            if (M2C_FIELD(temp_r9_32, s32 *, 0x2128) == 0x3C) {
                M2C_FIELD(temp_r9_32, s32 *, 0x2128) = 0;
                M2C_FIELD(temp_r9_32, s32 *, 0x212C) = 0x19;
                var_fp_905 = 2;
                goto block_167;
            }
            break;
        }
    }
    if (mode != 0) {
        goto block_186;
    }
    if (!(0x100 & var_r4_907)) {
        goto block_183;
    }
    if (sp3C == -1) {
        goto block_193;
    }
    sp44 = 0;
    temp_r7_1504 = (u32) (0xF00 & sp28) >> 8;
    temp_r5_1508 = (u32) (0xE0 & sp28) >> 5;
    temp_r6_1509 = 0x1F & sp28;
    sp8 = var_r4_907;
    var_r4_1515 = var_r4_907;
    if ((Func_08077210(temp_r7_1504, temp_r5_1508, temp_r6_1509) != 0) || (var_r4_1515 = sp8, (Func_08077208(temp_r7_1504, temp_r5_1508, temp_r6_1509) != 0))) {
        sp44 = 1;
    }
    sp24 = 1;
    *(u32 *)0x03001AF8 = 0;
    if (sp44 == 0) {
        Func_080f9010(0x72);
        Func_08015278(M2C_FIELD(ctx, s32 *, 0x30));
        sp0 = 0x68;
        Func_08015068(M2C_FIELD(ctx, s32 *, 0x30), 0, 0x50, 0xD8);
        Func_08015078(0xBBE, M2C_FIELD(ctx, s32 *, 0x30), 0, 0x60);
        sp48 = 1;
        goto loop_36;
    }
    if ((sp28 >> 0xF) != 0) {
        sp8 = var_r4_1515;
        Func_080f9010(0xAF);
        temp_r3_1572 = 0x1F & sp28;
        temp_r5_1573 = (u32) (0xF00 & sp28) >> 8;
        temp_r6_1574 = (u32) (0xE0 & sp28) >> 5;
        Func_080771b8(temp_r5_1573, temp_r6_1574, temp_r3_1572);
        Func_080771c8(temp_r5_1573, temp_r6_1574, temp_r3_1572);
    } else {
        sp8 = var_r4_1515;
        Func_080f9010(0x8B);
        temp_r3_1597 = 0x1F & sp28;
        temp_r5_1598 = (u32) (0xF00 & sp28) >> 8;
        temp_r6_1599 = (u32) (0xE0 & sp28) >> 5;
        Func_080771b0(temp_r5_1598, temp_r6_1599, temp_r3_1597);
        Func_080771c0(temp_r5_1598, temp_r6_1599, temp_r3_1597);
    }
    Func_08077010((u32) (0xF00 & sp28) >> 8);
    Func_080aafb8(temp_r9_32);
    sp48 = 1;
    var_r4_907 = sp8;
block_183:
    if ((mode == 0) && (0x200 & var_r4_907)) {
        var_r5_1634 = 7;
        Func_080f9010(0x70);
    } else {
block_186:
        if (!(1 & var_r4_907)) {
            if (mode != 1) {
                goto block_208;
            }
            if (!(0x100 & var_r4_907)) {
block_208:
                if (8 & var_r4_907) {
                    var_r5_1785 = 2;
                    goto block_212;
                }
                if (2 & var_r4_907) {
                    var_r5_1785 = 1;
block_212:
                    Func_080f9010(0x71);
                    var_r5_1634 = 0 - var_r5_1785;
                } else {
                    if (mode != 0) {
                        goto block_241;
                    }
                    if (!(4 & var_r4_907)) {
                        goto block_241;
                    }
                    if (sp24 == 0) {
                        sp0 = 1;
                        sp4 = 0xF;
                        Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6);
                        var_r5_1634 = 0xA;
                        Func_080f9010(0x70);
                    } else {
                        temp_r5_1817 = sp20 ^ 1;
                        sp20 = temp_r5_1817;
                        if (temp_r5_1817 != 0) {
                            Func_080f9010(0x8B);
                        } else {
                            Func_080f9010(0xAF);
                        }
                        sp44 = 0;
                        if ((s32) M2C_FIELD(ctx, u8 *, 0x219) <= 0) {

                        } else {
                            sp14 = 0xA0;
                            sp10 = 0;
loop_225:
                            sp40 = 0;
                            if ((s32) M2C_FIELD(temp_r9_32, u8 *, sp14) > 0) {
                                var_sl_1853 = (sp10 * 2) + temp_r9_32;
                                do {
                                    temp_r7_1856 = *var_sl_1853;
                                    var_sl_1853 += 1;
                                    temp_r4_1863 = (u32) (temp_r7_1856 & 0xF00) >> 8;
                                    var_r8_1869 = 0;
                                    temp_r6_1871 = (u32) (temp_r7_1856 & 0xE0) >> 5;
                                    temp_r5_1872 = temp_r7_1856 & 0x1F;
                                    if ((Func_08077210(temp_r4_1863, temp_r6_1871, temp_r5_1872) != 0) || (Func_08077208(temp_r4_1863, temp_r6_1871, temp_r5_1872) != 0)) {
                                        var_r8_1869 = 1;
                                    }
                                    if (var_r8_1869 != 0) {
                                        if (sp20 != 0) {
                                            if ((temp_r7_1856 >> 0xF) == 0) {
                                                temp_r3_1908 = 0x1F & temp_r7_1856;
                                                temp_r5_1909 = (u32) (temp_r7_1856 & 0xF00) >> 8;
                                                temp_r6_1910 = (u32) (temp_r7_1856 & 0xE0) >> 5;
                                                Func_080771b0(temp_r5_1909, temp_r6_1910, temp_r3_1908);
                                                Func_080771c0(temp_r5_1909, temp_r6_1910, temp_r3_1908);
                                                Func_08077010(temp_r5_1909);
                                            }
                                        } else if ((temp_r7_1856 >> 0xF) != 0) {
                                            temp_r3_1936 = 0x1F & temp_r7_1856;
                                            temp_r5_1937 = (u32) (temp_r7_1856 & 0xF00) >> 8;
                                            temp_r6_1938 = (u32) (temp_r7_1856 & 0xE0) >> 5;
                                            Func_080771b8(temp_r5_1937, temp_r6_1938, temp_r3_1936);
                                            Func_080771c8(temp_r5_1937, temp_r6_1938, temp_r3_1936);
                                            Func_08077010(temp_r5_1937);
                                        }
                                    }
                                    temp_r1_1953 = sp40 + 1;
                                    sp40 = temp_r1_1953;
                                } while (temp_r1_1953 < (s32) M2C_FIELD(temp_r9_32, u8 *, sp14));
                            }
                            temp_r1_1966 = sp44 + 1;
                            sp14 += 1;
                            sp10 += 0xA;
                            sp44 = temp_r1_1966;
                            if (temp_r1_1966 < (s32) M2C_FIELD(ctx, u8 *, 0x219)) {
                                goto loop_225;
                            }
                        }
                        Func_080aafb8(temp_r9_32);
                        sp48 = 1;
block_241:
                        if (0x40 & var_fp_905) {
                            Func_080f9010(0x6F);
                            if (!(4 & (u8) statusPtr[sp38])) {
                                sp4 = 0xF;
                                sp0 = 1;
                                Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6);
                                temp_r2_2035 = (u8) statusPtr[sp38];
                                if (!(4 & temp_r2_2035)) {
                                    if (1 & temp_r2_2035) {
                                        statusPtr[sp38] = -2 & temp_r2_2035;
                                        sp30 = 0;
                                        goto block_249;
                                    }
                                    if ((sp30 == 0) && (2 & temp_r2_2035)) {
                                        statusPtr[sp38] = temp_r2_2035 | 1;
                                        sp48 = 2;
                                    } else {
block_249:
                                        sp30 -= 1;
                                        var_r1_2077 = M2C_FIELD(temp_r9_32, s8 *, sp38 + 0xA0);
                                        if (var_r1_2077 == 0) {
                                            var_r1_2077 = 1;
                                        }
                                        temp_r0_2083 = Func_080aa538(sp30, (u8) var_r1_2077);
                                        sp30 = temp_r0_2083;
                                        sp2C = temp_r0_2083;
                                        sp48 = 2;
                                    }
                                }
                            }
                        } else if (0x80 & var_fp_905) {
                            Func_080f9010(0x6F);
                            if (!(4 & (u8) statusPtr[sp38])) {
                                sp4 = 0xF;
                                sp0 = 1;
                                Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6);
                                sp30 += 1;
                                var_r1_2127 = M2C_FIELD(temp_r9_32, s8 *, sp38 + 0xA0);
                                if (var_r1_2127 == 0) {
                                    var_r1_2127 = 1;
                                }
                                sp30 = Func_080aa538(sp30, (u8) var_r1_2127);
                                temp_r2_2137 = (u8) statusPtr[sp38];
                                if ((1 & temp_r2_2137) && !(4 & temp_r2_2137)) {
                                    statusPtr[sp38] = -2 & temp_r2_2137;
                                    sp30 = 0;
                                } else if (sp30 == 0) {
                                    temp_r2_2165 = (u8) statusPtr[sp38];
                                    if (2 & temp_r2_2165) {
                                        statusPtr[sp38] = 1 | temp_r2_2165;
                                    }
                                }
                                sp2C = sp30;
                                sp48 = 2;
                            }
                        } else {
                            if (0x20 & var_fp_905) {
                                Func_080f9010(0x6F);
                                if (!(4 & (u8) statusPtr[sp38])) {
                                    sp0 = 1;
                                    sp4 = 0xF;
                                    Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6);
                                }
                                temp_r3_2211 = sp38 - 1;
                                sp38 = temp_r3_2211;
                                sp38 = Func_080aa538(temp_r3_2211, M2C_FIELD(ctx, u8 *, 0x219));
                                if (mode == 0) {
                                    sp44 = 0;
                                    if (mode < (s32) M2C_FIELD(ctx, u8 *, 0x219)) {
                                        do {
                                            if (statusPtr[sp38] == 4) {
                                                temp_r0_2233 = sp38 - 1;
                                                sp38 = temp_r0_2233;
                                                sp38 = Func_080aa538(temp_r0_2233, M2C_FIELD(ctx, u8 *, 0x219));
                                            }
                                            temp_r1_2240 = sp44 + 1;
                                            sp44 = temp_r1_2240;
                                        } while (temp_r1_2240 < (s32) M2C_FIELD(ctx, u8 *, 0x219));
                                    }
                                }
                                goto block_281;
                            }
                            if (0x10 & var_fp_905) {
                                Func_080f9010(0x6F);
                                if (!(4 & (u8) statusPtr[sp38])) {
                                    sp0 = 1;
                                    sp4 = 0xF;
                                    Func_080ab1f4(M2C_FIELD(ctx, s32 *, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6);
                                }
                                temp_r3_2279 = sp38 + 1;
                                sp38 = temp_r3_2279;
                                sp38 = Func_080aa538(temp_r3_2279, M2C_FIELD(ctx, u8 *, 0x219));
                                if (mode == 0) {
                                    sp44 = 0;
                                    if (mode < (s32) M2C_FIELD(ctx, u8 *, 0x219)) {
                                        do {
                                            if (statusPtr[sp38] == 4) {
                                                temp_r0_2301 = sp38 + 1;
                                                sp38 = temp_r0_2301;
                                                sp38 = Func_080aa538(temp_r0_2301, M2C_FIELD(ctx, u8 *, 0x219));
                                            }
                                            temp_r1_2308 = sp44 + 1;
                                            sp44 = temp_r1_2308;
                                        } while (temp_r1_2308 < (s32) M2C_FIELD(ctx, u8 *, 0x219));
                                    }
                                }
block_281:
                                sp30 = sp2C;
                                var_r1_2319 = M2C_FIELD(temp_r9_32, s8 *, sp38 + 0xA0);
                                if (var_r1_2319 == 0) {
                                    var_r1_2319 = 1;
                                }
                                sp30 = Func_080aa538(sp30, (u8) var_r1_2319);
                                sp48 = 2;
                                sp18 = sp38 * 8;
                            }
                        }
                        goto loop_36;
                    }
                }
            } else {
                goto block_191;
            }
        } else {
block_191:
            sp44 = 1;
            if (!(1 & (u8) statusPtr[sp38])) {
                if (sp3C == -1) {
block_193:
                    Func_080f9010(0x72);
                    goto loop_36;
                }
                temp_r7_1696 = (u32) (0xF00 & sp28) >> 8;
                temp_r6_1701 = (u32) (0xE0 & sp28) >> 5;
                temp_r5_1702 = 0x1F & sp28;
                sp44 = 0;
                if ((Func_08077210(temp_r7_1696, temp_r6_1701, temp_r5_1702) != 0) || (Func_08077208(temp_r7_1696, temp_r6_1701, temp_r5_1702) != 0)) {
                    sp44 = 1;
                }
            }
            if (sp44 == 0) {
                Func_080f9010(0x72);
                Func_08015278(M2C_FIELD(ctx, s32 *, 0x30));
                sp0 = 0x68;
                Func_08015068(M2C_FIELD(ctx, s32 *, 0x30), 0, 0x50, 0xD8);
                Func_08015078(0xBBE, M2C_FIELD(ctx, s32 *, 0x30), 0, 0x60);
                goto loop_36;
            }
            if (mode == 1) {
                var_r5_1634 = 4;
                if (mode & (u8) statusPtr[sp38]) {
                    if (sp38 == M2C_FIELD(ctx, s8 *, 0x1C)) {
                        var_r5_1634 = 2;
                        if (!(0x8000 & M2C_FIELD(ctx, u16 *, 0x178))) {
                            goto block_206;
                        }
                    } else {
                        var_r5_1634 = 3;
                    }
                }
            } else {
block_206:
                var_r5_1634 = 1;
            }
            Func_080f9010(0x70);
        }
    }
    M2C_FIELD(ctx, s8 *, mode + 0x1C) = sp38;
    if (sp3C != -1) {
        temp_r2_2357 = M2C_FIELD(temp_r9_32, u16 *, ((sp38 * 0xA) + sp3C) * 2);
        M2C_FIELD(ctx, u16 *, sp34 + 0x178) = temp_r2_2357;
        temp_r0_2363 = mode + 0x254;
        M2C_FIELD(ctx, s8 *, temp_r0_2363) = 0x1F & temp_r2_2357;
        M2C_FIELD(ctx, s8 *, temp_r0_2363 + 2) = (s8) ((u32) (0xE0 & temp_r2_2357) >> 5);
        M2C_FIELD(ctx, s8 *, mode + 0x258) = (s8) ((u32) (0xF00 & temp_r2_2357) >> 8);
    }
    M2C_FIELD(ctx, u16 *, sp34 + 0x174) = sp38 + (sp30 * 0xA);
    return var_r5_1634;
}
