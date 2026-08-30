#include "shared-aggregates.h"

/*
 * This header contains macros emitted by m2c in "valid syntax" mode,
 * which can be enabled by passing `--valid-syntax` on the command line.
 *
 * In this mode, unhandled types and expressions are emitted as macros so
 * that the output is compilable without human intervention.
 */

#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/* Unhandled instructions */
#define M2C_ERROR(desc) (0)
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)
#define REVERSE_BITS(x) (0)
#define ROTATE_RIGHT(x, shift) (0)
#define ARM_RRX(x, carry) (0)
#define BSWAP32(x) (0)
#define BSWAP16(x) (0)
#define BSWAP16X2(x) (0)

/* Carry/overflow bits from partially-implemented instructions */
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_STRUCT_COPY memcpy

/* Sh2 control register loads/stores */
#define M2C_LOAD_SR() (0)
#define M2C_LOAD_GBR() (0)
#define M2C_LOAD_VBR() (0)
#define M2C_STORE_SR(a)
#define M2C_STORE_GBR(a)
#define M2C_STORE_VBR(a)

#define M2C_CMP_STR(a, b) (0)

#endif

u8 Func_08024934(u8 *arg2) {
    s32 sp0;
    s32 sp4;
    s32 *sp8;
    u32 spC;
    s32 sp10;
    s32 sp14;
    M2C_UNK *sp18;
    M2C_UNK *sp1C;
    s32 *sp20;
    u16 *sp24;
    s32 sp28;
    s32 sp2C;
    u32 sp30;
    s32 sp34;
    void *sp38;
    void *sp3C;
    s32 sp40;
    u32 sp44;
    struct M2cAggregate_deref_absolute_03001e8c_0 *sp48;
    void *sp4C;
    s32 sp50;
    u8 *sp54;
    u32 sp58;
    u8 sp5C;
    u32 sp60[4];
    M2C_UNK sp70;
    u8 sp114;
    M2C_UNK sp138;
    M2C_UNK sp168;
    M2C_UNK *var_r5_70;
    M2C_UNK *var_r6_578;
    s32 temp_r0_142;
    s32 temp_r0_550;
    s32 temp_r0_704;
    s32 temp_r1_315;
    s32 temp_r1_387;
    s32 temp_r2_314;
    s32 temp_r2_766;
    s32 temp_r3_146;
    s32 temp_r3_361;
    s32 temp_r3_599;
    s32 temp_r3_821;
    s32 temp_r3_935;
    s32 temp_r5_509;
    s32 temp_r5_717;
    s32 temp_r6_681;
    s32 temp_r8_383;
    s32 var_r0_814;
    s32 var_r1_518;
    s32 var_r1_722;
    s32 var_r1_812;
    s32 var_r2_199;
    s32 var_r2_279;
    s32 var_r2_689;
    s32 var_r2_740;
    s32 var_r3_544;
    s32 var_r3_760;
    s32 var_r3_884;
    s32 var_r3_900;
    s32 var_r3_952;
    s32 var_r3_994;
    s32 var_r4_103;
    s32 var_r4_1044;
    s32 var_r4_163;
    s32 var_r4_327;
    s32 var_r4_379;
    s32 var_r4_435;
    s32 var_r4_494;
    s32 var_r4_507;
    s32 var_r4_579;
    s32 var_r4_68;
    s32 var_r4_715;
    s32 var_r5_100;
    s32 var_r5_301;
    s32 var_r5_439;
    s32 var_r5_682;
    s32 var_r6_303;
    s32 var_r7_144;
    s32 var_r7_345;
    s32 var_r7_436;
    s8 *var_r2_491;
    s8 temp_r0_92;
    u16 *temp_r0_158;
    u16 *temp_r0_260;
    u16 *temp_r0_338;
    u16 *var_r1_342;
    u16 *var_r6_280;
    u16 *var_r6_438;
    u32 *var_r5_1045;
    u32 *var_r6_101;
    u32 temp_r0_1047;
    u32 temp_r0_116;
    u32 var_fp_52;
    u32 var_r3_517;
    u32 var_r3_728;
    u32 var_r9_24;
    u32 var_sl_53;
    u8 *var_r0_160;
    u8 *var_r0_200;
    u8 *var_r0_341;
    u8 *var_r1_159;
    u8 *var_r1_198;
    u8 *var_r5_154;
    u8 *var_r5_580;
    u8 temp_r3_202;
    u8 temp_r3_282;
    u8 temp_r3_582;
    u8 temp_r6_156;
    u8 var_r6_326;
    u8 var_r6_843;
    void *temp_r2_804;

    sp54 = arg2;
    sp48 = absolute_03001e8c.field_0000;
    sp44 = -1U;
    var_r9_24 = -1U;
    sp40 = (s32) Func_080040b4(0x80);
    sp0 = 0x2A;
    sp3C = Func_080162d4(0, 4, 0x1E, 4);
    sp0 = 6;
    sp38 = Func_080162d4(0x14, 8, 0xA, 3);
    sp34 = 0;
    var_fp_52 = absolute_03001e8c.field_00a8->field_0034;
    var_sl_53 = absolute_03001e8c.field_00a8->field_0030;
    sp30 = absolute_03001e8c.field_00a8->field_0038;
    sp0 = 6;
    sp4C = Func_080162d4(0xD, 0xB, 0x11, 9);
    sp1C = &sp138;
    var_r4_68 = 0;
    var_r5_70 = &sp138;
    do {
        M2C_FIELD(var_r5_70, s32 *, 4) = 0x40000000;
        M2C_FIELD(var_r5_70, s32 *, 8) = 0;
        M2C_FIELD(var_r5_70, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(var_r5_70, u16 *, 6)) | (((M2C_FIELD(sp4C, u16 *, 0xC) * 8) + 8) & 0x1FF));
        temp_r0_92 = (((var_r4_68 * 2) + M2C_FIELD(sp4C, u16 *, 0xE)) * 8) + 4;
        var_r4_68 += 1;
        M2C_FIELD(var_r5_70, s32 *, 4) = temp_r0_92;
        var_r5_70 += 0xC;
    } while (var_r4_68 <= 3);
    var_r5_100 = 8;
    var_r6_101 = &sp60[0];
    var_r4_103 = 3;
    do {
        sp4 = var_r4_103;
        temp_r0_116 = (u32) Func_080040b4(0x80);
        *var_r6_101 = temp_r0_116;
        var_r6_101 += 4;
        var_r4_103 -= 1;
        *(var_r5_100 + sp1C) = (*(var_r5_100 + sp1C) & 0xFFFFFC00) | (Func_080040d0(temp_r0_116, -1) & 0x3FF);
        var_r5_100 += 0xC;
    } while (var_r4_103 >= 0);
    temp_r0_142 = Func_080771d8(&sp114);
    sp50 = temp_r0_142;
    var_r7_144 = 0;
    temp_r3_146 = temp_r0_142 - 1;
    sp14 = temp_r3_146;
    if (temp_r3_146 >= 0) {
        sp20 = &sp0 + 0xF0;
        var_r5_154 = &(&sp114)[temp_r3_146];
        do {
            temp_r6_156 = *var_r5_154;
            temp_r0_158 = Func_080771e0(temp_r6_156);
            var_r1_159 = sp54;
            var_r0_160 = temp_r0_158 + 4;
            var_r4_163 = 0;
            if ((u32) M2C_FIELD(temp_r0_158, u8 *, 4) <= (u32) *var_r1_159) {
loop_8:
                var_r4_163 += 1;
                if (var_r4_163 <= 3) {
                    var_r0_160 += 1;
                    var_r1_159 += 1;
                    if ((u32) *var_r0_160 <= (u32) *var_r1_159) {
                        goto loop_8;
                    }
                }
            }
            if (var_r4_163 == 4) {
                *(sp20 + var_r7_144) = temp_r6_156;
                *var_r5_154 = 0x20;
                var_r7_144 += 1;
            }
            var_r5_154 -= 1;
        } while ((s32) var_r5_154 >= (s32) &sp114);
    } else {
        sp20 = &sp0 + 0xF0;
    }
    if (sp50 > 0) {
        var_r1_198 = var_r7_144 + sp20;
        var_r2_199 = sp50;
        var_r0_200 = &sp114;
        do {
            temp_r3_202 = *var_r0_200;
            var_r0_200 += 1;
            if (temp_r3_202 != 0x20) {
                *var_r1_198 = temp_r3_202;
                var_r7_144 += 1;
                var_r1_198 += 1;
            }
            var_r2_199 -= 1;
        } while (var_r2_199 != 0);
    }
    *(sp20 + var_r7_144) = 0x20;
    sp18 = &sp168;
    sp10 = var_sl_53 * 2;
    spC = sp40 << 0x10;
loop_21:
    if ((var_fp_52 == var_r9_24) && (var_sl_53 == sp44)) {

    } else {
        M2C_FIELD(sp48, s8 *, 0xEA6) = 1;
        sp0 = 0xF;
        Func_08022768(M2C_FIELD(sp4C, u16 *, 0xC) + 1, M2C_FIELD(sp4C, u16 *, 0xE) + (sp44 * 2) + 1, M2C_FIELD(sp4C, u16 *, 8) - 2, 1);
        Func_08016738();
        temp_r0_260 = Func_080771e0(*(sp20 + (var_fp_52 + var_sl_53)));
        Func_0801965c(*temp_r0_260 + 0x53A, &sp70, 0x34);
        Func_08017aa4(&sp70, sp3C, 0, 4);
        sp34 = 0;
        sp44 = var_sl_53;
        var_r2_279 = 0;
        var_r6_280 = temp_r0_260 + 4;
        do {
            temp_r3_282 = *var_r6_280;
            var_r6_280 += 1;
            if (temp_r3_282 != 0) {
                sp34 |= 1 << var_r2_279;
            }
            var_r2_279 += 1;
        } while (var_r2_279 <= 3);
        if (var_fp_52 == var_r9_24) {

        } else {
            Func_08016498(sp4C);
            var_r5_301 = 0;
            var_r6_303 = 1;
            do {
                sp0 = 0;
                Func_08019000(sp38, var_r5_301 + 0x5001, var_r5_301 * 2, 0);
                temp_r2_314 = var_r6_303;
                temp_r1_315 = sp54[var_r5_301] + 0x30;
                var_r5_301 += 1;
                sp0 = 0;
                var_r6_303 += 2;
                Func_08018efc(sp38, temp_r1_315, temp_r2_314, 0);
            } while (var_r5_301 <= 3);
            var_r6_326 = *(sp20 + var_fp_52);
            var_r4_327 = 0;
            if (var_r6_326 == 0x20) {
                goto block_50;
            }
            sp8 = &sp0 + 0x58;
loop_35:
            sp4 = var_r4_327;
            temp_r0_338 = Func_080771e0(var_r6_326);
            sp24 = temp_r0_338;
            var_r0_341 = sp54;
            var_r1_342 = temp_r0_338 + 4;
            var_r7_345 = 0;
            if ((u32) M2C_FIELD(temp_r0_338, u8 *, 4) <= (u32) *var_r0_341) {
loop_36:
                var_r7_345 += 1;
                if (var_r7_345 <= 3) {
                    var_r1_342 += 1;
                    var_r0_341 += 1;
                    if ((u32) *var_r1_342 <= (u32) *var_r0_341) {
                        goto loop_36;
                    }
                }
            }
            temp_r3_361 = 4 ^ var_r7_345;
            sp0 = 1;
            sp4 = var_r4_327;
            Func_0801a3d0(0x3FFF & *sp24, 0, &(&sp60[0])[var_r4_327], sp8);
            var_r4_379 = var_r4_327;
            temp_r8_383 = var_r4_379 * 2;
            temp_r1_387 = (var_r4_379 * 0xC) + 8;
            *(sp1C + temp_r1_387) = (*(sp1C + temp_r1_387) & 0xFFFFFC00) | (sp58 & 0x3FF);
            if ((1 - ((u32) ((0 - temp_r3_361) | temp_r3_361) >> 0x1F)) == 0) {
                Func_0801e71c(2);
                var_r4_379 = sp4;
            }
            sp4 = var_r4_379;
            Func_0801e7c0(*(u16 *)Func_080771e0(var_r6_326) + 0x333, sp4C, 0x10, var_r4_379 * 0x10);
            var_r4_435 = sp4;
            var_r7_436 = 0;
            var_r6_438 = sp24 + 4;
            var_r5_439 = 0xD;
            do {
                if (*var_r6_438 != 0) {
                    sp0 = 0;
                    sp4 = var_r4_435;
                    Func_08019000(sp4C, var_r7_436 + 0x5001, var_r5_439, temp_r8_383);
                    sp0 = 0;
                    Func_08018efc(sp4C, *var_r6_438 + 0x30, var_r5_439 + 1, temp_r8_383);
                    var_r5_439 += 2;
                }
                var_r7_436 += 1;
                var_r6_438 += 1;
            } while (var_r7_436 <= 3);
            sp4 = var_r4_435;
            Func_0801e71c(0xF);
            (&sp5C)[var_r4_435] = 1;
            var_r4_327 = var_r4_435 + 1;
            if (var_r4_327 <= 3) {
                var_r6_326 = *(sp20 + (var_fp_52 + var_r4_327));
                if (var_r6_326 != 0x20) {
                    goto loop_35;
                }
block_50:
                if (var_r4_327 <= 3) {
                    var_r2_491 = (s8 *)&sp5C + var_r4_327;
                    var_r4_494 = 4 - var_r4_327;
                    do {
                        var_r4_494 -= 1;
                        *var_r2_491 = 0;
                        var_r2_491 += 1;
                    } while (var_r4_494 != 0);
                }
            }
            var_r9_24 = var_fp_52;
        }
        if (sp50 > 4) {
            var_r4_507 = 0;
            temp_r5_509 = sp50 + 3;
loop_61:
            var_r3_544 = temp_r5_509;
            if (temp_r5_509 < 0) {
                var_r3_544 = sp50 + 6;
            }
            temp_r0_550 = var_r3_544 >> 2;
            if (var_r4_507 < temp_r0_550) {
                var_r3_517 = var_fp_52;
                var_r1_518 = var_r4_507 + 0xF301;
                if ((s32) var_r3_517 < 0) {
                    var_r3_517 += 3;
                }
                if (var_r4_507 == ((s32) var_r3_517 >> 2)) {
                    var_r1_518 = var_r4_507 + 0xF30B;
                }
                sp0 = 0;
                sp4 = var_r4_507;
                Func_08019000(sp4C, var_r1_518, ((M2C_FIELD(sp4C, u16 *, 8) - temp_r0_550) + var_r4_507) - 2, -1);
                var_r4_507 += 1;
                goto loop_61;
            }
        }
        sp0 = 0xE;
        Func_08022768(M2C_FIELD(sp4C, u16 *, 0xC) + 1, M2C_FIELD(sp4C, u16 *, 0xE) + sp10 + 1, M2C_FIELD(sp4C, u16 *, 8) - 2, 1);
        M2C_FIELD(sp48, u8 *, 0xEA3) = 1U;
        M2C_FIELD(sp48, s8 *, 0xEA6) = 0;
    }
    var_r6_578 = sp1C;
    var_r4_579 = 0;
    var_r5_580 = &sp5C;
    do {
        temp_r3_582 = *var_r5_580;
        var_r5_580 += 1;
        if (temp_r3_582 != 0) {
            sp4 = var_r4_579;
            Func_08003dec(var_r6_578, 0xF0);
        }
        var_r4_579 += 1;
        var_r6_578 += 0xC;
    } while (var_r4_579 <= 3);
    temp_r3_599 = (M2C_FIELD(sp4C, u16 *, 0xC) * 8) - 2;
    sp28 = temp_r3_599;
    sp2C = ((sp10 + M2C_FIELD(sp4C, u16 *, 0xE)) * 8) + 0x14;
    M2C_FIELD(sp18, s32 *, 4) = 0x40000000;
    M2C_FIELD(sp18, s32 *, 8) = 0;
    M2C_FIELD(sp18, s32 *, 8) = (s16) (((u16) M2C_FIELD(sp18, s32 *, 8) & 0xFFFFFC00) | (Func_080040d0(spC >> 0x10, 0x080310A4) & 0x3FF));
    M2C_FIELD(sp18, u16 *, 6) = (u16) ((M2C_FIELD(sp18, u16 *, 6) & 0xFFFFFE00) | ((temp_r3_599 + ((u32) (*(s32 *)0x03001E40 & 4) >> 1) + 0xFFFC) & 0x1FF));
    M2C_FIELD(sp18, s32 *, 4) = (s8) ((sp2C - ((u32) (*(u32 *)0x03001E40 & 4) >> 2)) + 0xF8);
    Func_08003dec(sp18, 0xF2);
    temp_r6_681 = *(u32 *)0x03001E40 & 8;
    var_r5_682 = 0;
    do {
        var_r2_689 = 0xF - ((u32) ((0 - temp_r6_681) | temp_r6_681) >> 0x1F);
        if (!((1 << var_r5_682) & sp34)) {
            var_r2_689 = 0xF;
        }
        sp0 = var_r2_689;
        temp_r0_704 = M2C_FIELD(sp38, u16 *, 0xC) + (var_r5_682 * 2) + 1;
        var_r5_682 += 1;
        Func_08022768(temp_r0_704, M2C_FIELD(sp38, u16 *, 0xE) + 1, 2, 1);
    } while (var_r5_682 <= 3);
    if (sp50 > 4) {
        var_r4_715 = 0;
        temp_r5_717 = sp50 + 3;
loop_84:
        var_r3_760 = temp_r5_717;
        if (temp_r5_717 < 0) {
            var_r3_760 = sp50 + 6;
        }
        temp_r2_766 = var_r3_760 >> 2;
        if (var_r4_715 < temp_r2_766) {
            var_r1_722 = var_r4_715 + 0xF301;
            if ((u32) (*(u32 *)0x03001E40 & 0xF) <= 0xBU) {
                var_r3_728 = var_fp_52;
                if ((s32) var_r3_728 < 0) {
                    var_r3_728 += 3;
                }
                if (var_r4_715 == ((s32) var_r3_728 >> 2)) {
                    var_r1_722 = var_r4_715 + 0xF30B;
                }
            }
            var_r2_740 = temp_r5_717;
            if (temp_r5_717 < 0) {
                var_r2_740 = sp50 + 6;
            }
            sp0 = 0;
            sp4 = var_r4_715;
            Func_08019000(sp4C, var_r1_722, ((M2C_FIELD(sp4C, u16 *, 8) - (var_r2_740 >> 2)) + var_r4_715) - 2, -1);
            var_r4_715 += 1;
            goto loop_84;
        }
        sp0 = 0;
        Func_08019000(sp4C, 0xF334, (M2C_FIELD(sp4C, u16 *, 8) - temp_r2_766) - 3, -1);
        sp0 = 0;
        Func_08019000(sp4C, 0xF335, M2C_FIELD(sp4C, u16 *, 8) - 2, -1);
        M2C_FIELD(sp48, u8 *, 0xEA3) = (u8) ((2 << ((u32) (M2C_FIELD(sp4C, u16 *, 0xE) - 1) >> 2)) | M2C_FIELD(sp48, u8 *, 0xEA3));
    }
    temp_r2_804 = *(void **)0x03001F34;
    M2C_FIELD(temp_r2_804, u32 *, 0x34) = var_fp_52;
    M2C_FIELD(temp_r2_804, u32 *, 0x30) = var_sl_53;
    M2C_FIELD(temp_r2_804, u32 *, 0x38) = sp30;
    var_r1_812 = *(s32 *)0x03001C94;
    var_r0_814 = *(s32 *)0x03001B04;
    if (M2C_FIELD(temp_r2_804, s32 *, 0xD8) != 0) {
        temp_r3_821 = M2C_FIELD(temp_r2_804, s32 *, 0xDC);
        var_r0_814 = 0;
        var_r1_812 = 0;
        if (temp_r3_821 == 0) {
            M2C_FIELD(temp_r2_804, s32 *, 0xDC) = 0x78;
            var_r0_814 = 1;
            var_r1_812 = 1;
        } else {
            M2C_FIELD(temp_r2_804, s32 *, 0xDC) = (s32) (temp_r3_821 - 1);
        }
    }
    if (var_r1_812 & 1) {
        var_r6_843 = *(sp20 + (var_fp_52 + var_sl_53));
    } else if ((M2C_FIELD(*(void **)0x03001F34, s32 *, 0x4C) == 0) || (2 & var_r1_812)) {
        Func_080f9010(0x71);
        var_r6_843 = -1U;
    } else {
        if (0x80 & var_r0_814) {
            Func_080f9010(0x6F);
            var_sl_53 += 1;
            if ((var_sl_53 == 4) || ((var_fp_52 + var_sl_53) == sp50)) {
                var_sl_53 = 0;
            }
            var_r3_884 = var_sl_53 * 2;
            sp30 = var_sl_53;
            goto block_131;
        }
        if (0x40 & var_r0_814) {
            Func_080f9010(0x6F);
            var_sl_53 -= 1;
            if ((s32) var_sl_53 < 0) {
                var_r3_900 = sp14;
                if (var_r3_900 < 0) {
                    var_r3_900 = sp50 + 2;
                }
                if (var_fp_52 == ((var_r3_900 >> 2) * 4)) {
                    var_sl_53 = (sp50 - var_fp_52) - 1;
                } else {
                    var_sl_53 = 3;
                }
            }
            var_r3_884 = var_sl_53 * 2;
            sp30 = var_sl_53;
            goto block_131;
        }
        if (0x10 & var_r0_814) {
            Func_080f9010(0x6F);
            Func_0800352c();
            temp_r3_935 = var_fp_52 + 4;
            if (temp_r3_935 >= sp50) {
                if (var_fp_52 != 0) {
                    var_sl_53 = sp30;
                    var_fp_52 = 0;
                    sp10 = var_sl_53 * 2;
                }
            } else {
                var_fp_52 = (u32) temp_r3_935;
                var_r3_952 = sp14;
                var_sl_53 = sp30;
                if (var_r3_952 < 0) {
                    var_r3_952 = sp50 + 2;
                }
                if (var_fp_52 == ((var_r3_952 >> 2) * 4)) {
                    var_sl_53 = (sp50 - var_fp_52) - 1;
                    if ((s32) var_sl_53 > (s32) sp30) {
                        var_sl_53 = sp30;
                        goto block_129;
                    }
                    sp10 = var_sl_53 * 2;
                } else {
                    sp10 = var_sl_53 * 2;
                }
            }
        } else if (0x20 & var_r0_814) {
            Func_080f9010(0x6F);
            Func_0800352c();
            if (var_fp_52 != 0) {
                var_sl_53 = sp30;
                var_fp_52 -= 4;
                sp10 = var_sl_53 * 2;
            } else {
                var_r3_994 = sp14;
                if (var_r3_994 < 0) {
                    var_r3_994 = sp50 + 2;
                }
                var_fp_52 = (var_r3_994 >> 2) * 4;
                var_sl_53 = sp30;
                if (var_fp_52 != 0) {
                    var_sl_53 = (sp50 - var_fp_52) - 1;
                    if ((s32) var_sl_53 > (s32) sp30) {
                        var_sl_53 = sp30;
                    }
                    var_r3_884 = var_sl_53 * 2;
block_131:
                    sp10 = var_r3_884;
                } else {
block_129:
                    sp10 = var_sl_53 * 2;
                }
            }
        }
        Func_080030f8(1);
        goto loop_21;
    }
    Func_080030f8(1);
    var_r4_1044 = 3;
    var_r5_1045 = &sp60[0];
    do {
        temp_r0_1047 = *var_r5_1045;
        var_r5_1045 += 4;
        sp4 = var_r4_1044;
        Func_08003f3c(temp_r0_1047);
        var_r4_1044 -= 1;
    } while (var_r4_1044 >= 0);
    Func_08003f3c(spC >> 0x10);
    Func_08016418(sp38, 1);
    Func_08016418(sp3C, 1);
    Func_08016418(sp4C, 1);
    Func_080030f8(1);
    return var_r6_843;
}
