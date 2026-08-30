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

void Func_080eb754(s32 arg0) {
    u32 sp0;                                        /* compiler-managed */
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    u32 *sp14;
    u32 *sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    void *sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    u32 sp44;
    u32 sp48;
    void *sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
    s32 sp60;
    M2C_UNK sp64;
    M2C_UNK sp94;
    M2C_UNK spA0;
    s32 *var_r3_770;
    s32 *var_r5_324;
    s32 *var_r5_875;
    s32 *var_r6_1193;
    s32 temp_r0_1195;
    s32 temp_r0_349;
    s32 temp_r0_391;
    s32 temp_r0_894;
    s32 temp_r0_982;
    s32 temp_r1_596;
    s32 temp_r2_523;
    s32 temp_r3_421;
    s32 temp_r3_468;
    s32 temp_r3_619;
    s32 temp_r3_833;
    s32 temp_r3_849;
    s32 temp_r4_475;
    s32 temp_r5_406;
    s32 var_fp_208;
    s32 var_fp_800;
    s32 var_r0_836;
    s32 var_r0_844;
    s32 var_r0_852;
    s32 var_r3_530;
    s32 var_r4_862;
    s32 var_r5_1095;
    s32 var_r5_278;
    s32 var_r5_582;
    s32 var_r6_1147;
    s32 var_r6_589;
    s32 var_r6_963;
    s32 var_r7_363;
    s32 var_r8_1032;
    s32 var_r8_1094;
    s32 var_r8_1143;
    s32 var_r8_1192;
    s32 var_r8_166;
    s32 var_r8_377;
    s32 var_r8_464;
    s32 var_r8_588;
    s32 var_r8_714;
    s32 var_r8_771;
    s32 var_r8_930;
    s32 var_r9_938;
    s32 var_sl_257;
    s32 var_sl_364;
    struct M2cAggregate_absolute_02010000 *var_r5_1031;
    u16 temp_r6_396;
    u16 var_r0_289;
    u32 *temp_r4_728;
    u32 *var_r3_734;
    u32 *var_r5_758;
    u32 temp_r0_474;
    u32 temp_r3_579;
    u8 *var_r5_965;
    void **var_r1_716;
    void *temp_r1_1106;
    void *temp_r2_124;
    void *temp_r2_718;
    void *temp_r3_20;
    void *var_r5_168;
    void *var_r5_466;
    void *var_r8_322;
    void *var_r8_872;

    sp50 = M2C_FIELD(&absolute_03001ef0, s32 *, 0);
    temp_r3_20 = *(void **)0x03001EEC;
    sp4C = temp_r3_20;
    sp44 = absolute_03001ef0.field_0004;
    M2C_FIELD(temp_r3_20, s32 *, 0x7828) = arg0;
    Func_080cd594(0);
    Func_080c9048();
    absolute_0400000c.field_0000 = 0x784;
    M2C_FIELD((void *)0x05000000, s16 *, 0) = 0;
    M2C_FIELD((void *)0x05000000, s16 *, 2) = 0;
    M2C_FIELD(sp4C, s32 *, 0x7780) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080cd104(1, 0);
    Func_080dbb24(9, 0x175, 1);
    absolute_03001ce0.field_0010 = 0xF0;
    Func_080d6750(M2C_FIELD(temp_r3_20, s32 *, 0x7828));
    absolute_04000048.field_0000 = 0x2737;
    *(u16 *)0x04000038 = 0xCA;
    Func_080030f8(1);
    Func_080b5040(1, 0x3A, 0);
    Func_080cd104(1, 1);
    Func_080e0524(0x73, sp44, 0, 0);
    Func_080e0524(0x95, (u32) sp4C, 1, 1);
    M2C_FIELD((void *)0x04000000, s16 *, 0) = 0x7741;
    temp_r2_124 = (void *)0x04000000 + 0x20;
    M2C_FIELD((void *)0x04000000, s16 *, 0x20) = 0x80;
    M2C_FIELD(temp_r2_124, s16 *, 0x32) = 0x100E;
    M2C_FIELD(temp_r2_124, s16 *, 0x30) = 0x3F44;
    sp40 = 0;
    sp3C = 0;
    sp38 = (s32) absolute_03001ad0.field_0004;
    sp34 = M2C_FIELD(&absolute_03001ef0, void **, 0x10);
    sp30 = 0;
    M2C_FIELD(sp4C, s32 *, 0x7780) = 1;
    M2C_FIELD(sp4C, s32 *, 0x7784) = sp40;
    M2C_FIELD(sp34, s32 *, 0x10) = 1;
    var_r8_166 = 0;
    var_r5_168 = sp4C + 0x7080;
    do {
        M2C_FIELD(var_r5_168, s32 *, 0) = (s32) ((Func_08004458() & 0x1F) + 0x10);
        M2C_FIELD(var_r5_168, s32 *, 4) = (s32) (((Func_08004458() & 0x1F) + 0x30) << 0x10);
        M2C_FIELD(var_r5_168, s32 *, 0x10) = (s32) (((Func_08004458() & 0x1F) - 0x10) << 0x10);
        var_r8_166 += 1;
        M2C_FIELD(var_r5_168, s32 *, 0x18) = (s32) (Func_08002304(Func_08004458(), 0x30) + 2);
        var_r5_168 += 0x1C;
    } while (var_r8_166 != 0x40);
    sp0 = 3;
    Func_080ed408(0x2E, 7, 7);
    sp48 = absolute_03001e50.field_00b8;
    var_fp_208 = 0;
    absolute_0400000c.field_0000 = 0x786;
loop_63:
    if (var_fp_208 != 0x78) {
        sp2C = 0;
        if (var_fp_208 == 0) {
            Func_080f9010(0x88);
        }
        if (var_fp_208 == 0x1A) {
            Func_080f9010(0x8D);
        }
        if (var_fp_208 == 0x28) {
            Func_080f9010(0x9A);
        }
        if (var_fp_208 == 0x48) {
            Func_080f9010(0x9A);
        }
        if (var_fp_208 == 0x68) {
            Func_080f9010(0x9A);
        }
        if (!(*(s32 *)0x03001B04 & 3)) {
            goto block_8;
        }
        if (var_fp_208 <= 0x10) {
block_8:
            if ((u32) (var_fp_208 - 0x18) <= 0x1FU) {
                sp30 += 1;
            }
            if (sp30 > 0x18) {
                sp30 = 0x18;
            }
            if (var_fp_208 <= 0x87) {
                absolute_03001ad0.field_0004 -= sp30;
                sp3C += sp30;
            }
            if (var_fp_208 <= 0x95) {
                sp5C = M2C_FIELD((void *)0x080EDAD8, s32 *, 0);
                sp60 = M2C_FIELD((void *)0x080EDAD8, s32 *, 4);
                var_sl_257 = 0;
                if (var_fp_208 > 0x67) {
                    var_sl_257 = (var_fp_208 * 0x10) + 0xFFFFF980;
                }
                if ((u32) (var_fp_208 - 8) <= 0x17U) {
                    sp40 = (sp40 + sp30) - 8;
                }
                if (var_fp_208 > 7) {
                    var_r5_278 = 0x60;
                    if (var_fp_208 <= 0x68) {
                        var_r5_278 = 0x20;
                    }
                    var_r0_289 = (var_fp_208 << 0xA) + 0xFFFFE000;
                    if ((s32) var_r0_289 > 0x8000) {
                        var_r0_289 += 0xFFFF8000;
                    }
                    sp2C = (s32) (var_r5_278 * Func_08002322(var_r0_289)) >> 0x10;
                    if ((0x1F & var_fp_208) == 8) {
                        M2C_FIELD(sp4C, s32 *, 0x77A8) = 4;
                    }
                }
                M2C_FIELD(&spA0, s32 *, 0xC) = 0;
                M2C_FIELD(&spA0, s32 *, 4) = 0xFF0000;
                var_r8_322 = NULL;
                var_r5_324 = sp4C + 0x77D8;
                do {
                    M2C_FIELD(&spA0, s32 *, 0) = (s32) ((((sp40 + *(u8 *)(0x080EEF56 + (s32) var_r8_322)) - var_sl_257) << 0x10) + 0xE00000);
                    M2C_FIELD(&spA0, s32 *, 8) = (s32) (((*(u8 *)(0x080EEF5F + (s32) var_r8_322) - sp2C) << 0x10) + 0x480000);
                    temp_r0_349 = *var_r5_324;
                    var_r5_324 += 4;
                    Func_08009008(temp_r0_349, &spA0, &sp5C, 0);
                    var_r8_322 += 1;
                } while (var_r8_322 != (void *)9);
            }
            if (var_fp_208 <= 0x1A) {
                var_r7_363 = sp3C + 4;
                var_sl_364 = var_fp_208 * 8;
                if (var_r7_363 > 0xA) {
                    var_r7_363 = 0xA;
                }
                if (var_sl_364 > 0x40) {
                    var_sl_364 = 0x40;
                }
                var_r8_377 = 0;
                if (var_sl_364 != 0) {
                    sp28 = sp3C * 2;
                    sp24 = (sp3C * 0xC) + 0x30;
                    temp_r0_391 = var_r7_363 * 2;
                    sp20 = (s32) (var_r7_363 + ((u32) var_r7_363 >> 0x1F)) >> 1;
                    do {
                        temp_r6_396 = var_r8_377 << 0xA;
                        temp_r5_406 = ((s32) ((sp28 + 8) * Func_08002322(temp_r6_396)) >> 0x10) + sp3C;
                        temp_r3_421 = ((s32) (sp24 * Func_0800231c(temp_r6_396)) >> 0x10) + 0x40;
                        sp4 = temp_r0_391;
                        sp0 = var_r7_363;
                        Func_080072f4(sp50, sp44 + *(u16 *)(0x080EDE48 + (s32) (temp_r0_391 - 2)), (temp_r5_406 + 0x60) - sp20, temp_r3_421 - var_r7_363);
                        var_r8_377 += 1;
                    } while (var_r8_377 != var_sl_364);
                }
            }
            if (var_fp_208 == 0x18) {
                M2C_FIELD(sp4C, s32 *, 0x7780) = 2;
                M2C_FIELD(sp4C, s32 *, 0x7784) = 0x32;
            }
            if (var_fp_208 == 0x1C) {
                absolute_0400000c.field_0000 = 0x784;
            }
            if (var_fp_208 > 0x11) {
                var_r8_464 = 0;
                var_r5_466 = sp4C + 0x7080;
                do {
                    temp_r3_468 = M2C_FIELD(var_r5_466, s32 *, 0x18);
                    if (temp_r3_468 == 0) {
                        temp_r0_474 = Func_080022fc(var_r8_464, 3) + 1;
                        temp_r4_475 = temp_r0_474 * 2;
                        sp0 = temp_r0_474;
                        sp4 = temp_r4_475;
                        Func_080072f4(sp50, sp44 + *(u16 *)(0x080EDE48 + (s32) (temp_r4_475 - 2)), M2C_FIELD(var_r5_466, s32 *, 0), M2C_FIELD(var_r5_466, s16 *, 6) - temp_r0_474);
                        temp_r2_523 = M2C_FIELD(var_r5_466, s32 *, 0x10);
                        M2C_FIELD(var_r5_466, s32 *, 0) = (s32) (M2C_FIELD(var_r5_466, s32 *, 0) + 2);
                        M2C_FIELD(var_r5_466, s32 *, 4) = (s32) (M2C_FIELD(var_r5_466, s32 *, 4) + temp_r2_523);
                        var_r3_530 = temp_r2_523 * 0x30;
                        if (var_r3_530 < 0) {
                            var_r3_530 += 0x3F;
                        }
                        M2C_FIELD(var_r5_466, s32 *, 0x10) = (s32) (var_r3_530 >> 6);
                    } else {
                        M2C_FIELD(var_r5_466, s32 *, 0x18) = (s32) (temp_r3_468 - 1);
                    }
                    if (((s32) M2C_FIELD(var_r5_466, s32 *, 0) > 0x80) || (M2C_FIELD(var_r5_466, s32 *, 0x18) == 1)) {
                        M2C_FIELD(var_r5_466, s32 *, 0) = (s32) ((Func_08004458() & 0x1F) + sp40 + 0xAC);
                        M2C_FIELD(var_r5_466, s32 *, 4) = (s32) ((((Func_08004458() & 0x1F) - sp2C) + 0x38) << 0x10);
                        M2C_FIELD(var_r5_466, s32 *, 0x10) = (s32) (((Func_08004458() & 0x1F) - 0x10) << 0xF);
                    }
                    var_r8_464 += 1;
                    var_r5_466 += 0x1C;
                } while (var_r8_464 != 0x30);
            }
            if (var_fp_208 > 0x1F) {
                temp_r3_579 = var_fp_208 - 0x20;
                var_r5_582 = (s32) (temp_r3_579 + (temp_r3_579 >> 0x1F)) >> 1;
                if (var_r5_582 > 0x28) {
                    var_r5_582 = 0x28;
                }
                var_r8_588 = 0;
                var_r6_589 = 0;
                do {
                    temp_r1_596 = (Func_08004458() & 3) * 3;
                    sp0 = 0x30;
                    sp4 = 0x20;
                    Func_080072f4(sp50, sp4C + (temp_r1_596 << 9), 0x78 - var_r5_582, var_r6_589);
                    var_r8_588 += 1;
                    var_r6_589 += 0x12;
                } while (var_r8_588 != 6);
            }
            temp_r3_619 = M2C_FIELD(sp4C, s32 *, 0x77A8);
            if (temp_r3_619 > 0) {
                M2C_FIELD(sp4C, s32 *, 0x77A8) = (s32) (temp_r3_619 - 1);
                absolute_03001ad0.field_0006 = (Func_08004458() & 7) + 0x1C;
            } else {
                absolute_03001ad0.field_0006 = 0x20;
            }
            M2C_FIELD(sp4C, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            var_fp_208 += 1;
            goto loop_63;
        }
    }
    absolute_03001ad0.field_0004 = sp38;
    M2C_FIELD(sp34, s32 *, 0x10) = 0;
    Func_080d67dc();
    absolute_04000040.field_0000 = 0xF0;
    var_r8_714 = 0;
    var_r1_716 = sp4C + 0x77D8;
    do {
        temp_r2_718 = *var_r1_716;
        var_r1_716 += 4;
        var_r8_714 += 1;
        M2C_FIELD(temp_r2_718, u8 *, 9) = (u8) (M2C_FIELD(temp_r2_718, u8 *, 9) | 0xC);
    } while (var_r8_714 != 9);
    temp_r4_728 = &sp0 + 0x74;
    sp18 = temp_r4_728;
    sp1C = 0xE0;
    var_r3_734 = temp_r4_728;
    do {
        *var_r3_734 = 0;
        var_r3_734 += 1;
    } while (var_r3_734 != (&sp0 + 0x82));
    sp14 = &sp0 + 0x84;
    var_r5_758 = sp14;
    do {
        *var_r5_758 = (s8) (Func_08004458() & 0x1F);
        var_r5_758 += 1;
    } while (var_r5_758 != &sp94);
    var_r3_770 = (s32 *)0x02010018;
    var_r8_771 = 0;
    do {
        var_r8_771 += 1;
        *var_r3_770 = 0;
        var_r3_770 += 0x1C;
    } while (var_r8_771 != 0x140);
    M2C_FIELD(sp4C, s32 *, 0x7780) = 2;
    M2C_FIELD(sp4C, s32 *, 0x7784) = 0x4B;
    absolute_0400000c.field_0000 = 0x784;
    absolute_0400000c.field_0046 = 0x1010;
    sp10 = 0xFFFFFE20;
    var_fp_800 = 0;
loop_88:
    if (var_fp_800 <= 0x17) {
        sp54 = M2C_FIELD((void *)0x080EDAE0, s32 *, 0);
        sp58 = M2C_FIELD((void *)0x080EDAE0, s32 *, 4);
        sp1C -= 0x10;
        if (var_fp_800 <= 8) {
            temp_r3_833 = var_fp_800 << 0xB;
            var_r0_836 = temp_r3_833 + 0x4000;
            if (var_r0_836 > 0x8000) {
                var_r0_836 = temp_r3_833 + 0xFFFFC000;
            }
            var_r0_844 = Func_08002322((u16) var_r0_836) << 6;
        } else {
            temp_r3_849 = var_fp_800 << 0xB;
            var_r0_852 = temp_r3_849 + 0x4000;
            if (var_r0_852 > 0x8000) {
                var_r0_852 = temp_r3_849 + 0xFFFFC000;
            }
            var_r0_844 = Func_08002322((u16) var_r0_852) << 5;
        }
        var_r4_862 = var_r0_844 >> 0x10;
        M2C_FIELD(&sp64, s32 *, 0xC) = 0;
        M2C_FIELD(&sp64, s32 *, 4) = 0xFF0000;
        var_r8_872 = NULL;
        var_r5_875 = sp4C + 0x77D8;
        do {
            M2C_FIELD(&sp64, s32 *, 0) = (s32) ((sp1C + *(u8 *)(0x080EEF56 + (s32) var_r8_872)) << 0x10);
            M2C_FIELD(&sp64, s32 *, 8) = (s32) (((*(u8 *)(0x080EEF5F + (s32) var_r8_872) - var_r4_862) << 0x10) + 0x480000);
            temp_r0_894 = *var_r5_875;
            var_r5_875 += 4;
            sp8 = var_r4_862;
            Func_08009008(temp_r0_894, &sp64, &sp54, 0);
            var_r8_872 += 1;
            var_r4_862 = sp8;
        } while (var_r8_872 != (void *)9);
    }
    if (var_fp_800 == 8) {
        M2C_FIELD(sp4C, s32 *, 0x77A8) = var_fp_800;
        Func_080f9010(0x91);
    }
    if (var_fp_800 == 0xB) {
        Func_080f9010(0x91);
    }
    if (var_fp_800 == 0x2E) {
        Func_080f9010(0x89);
    }
    var_r8_930 = 0;
    if (M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s32 *, 0x14) != 0) {
        spC = 0;
        var_r9_938 = 0x24;
        do {
            if (*(sp18 + var_r8_930) == 0) {
                Func_080e3980(M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s16 *, var_r9_938), &sp94);
                if ((s32) M2C_FIELD(&sp94, s32 *, 0) > sp1C) {
                    *(sp18 + var_r8_930) = 1;
                    var_r6_963 = 0;
                    var_r5_965 = &absolute_02010000.unknown_0000[spC];
                    do {
                        M2C_FIELD(var_r5_965, s32 *, 0) = (s32) (M2C_FIELD(&sp94, s32 *, 0) << 0xF);
                        M2C_FIELD(var_r5_965, s32 *, 4) = (s32) ((M2C_FIELD(&sp94, s32 *, 4) - 0x10) << 0x10);
                        M2C_FIELD(var_r5_965, s32 *, 0xC) = (s32) ((Func_08004458() - 0x80) << 0xA);
                        temp_r0_982 = Func_08004458() - 0xC0;
                        M2C_FIELD(var_r5_965, s32 *, 0x10) = (s32) (temp_r0_982 << 0xB);
                        M2C_FIELD(var_r5_965, s32 *, 0) = (s32) (M2C_FIELD(var_r5_965, s32 *, 0) + (M2C_FIELD(var_r5_965, s32 *, 0xC) * 4));
                        M2C_FIELD(var_r5_965, s32 *, 4) = (s32) (M2C_FIELD(var_r5_965, s32 *, 4) + (temp_r0_982 << 0xD));
                        var_r6_963 += 1;
                        M2C_FIELD(var_r5_965, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 8);
                        var_r5_965 += 0x1C;
                    } while (var_r6_963 != 0x20);
                    Func_080b5088(M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s16 *, var_r9_938), 1);
                    Func_080f9010(0x86);
                }
            }
            spC += 0x380;
            var_r9_938 += 2;
            var_r8_930 += 1;
        } while (var_r8_930 != M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s32 *, 0x14));
    }
    var_r5_1031 = &absolute_02010000;
    var_r8_1032 = 0;
    do {
        if ((s32) var_r5_1031->field_0018 > 0) {
            sp0 = 3;
            sp4 = 6;
            Func_080072f4(sp50, sp44 + M2C_FIELD((void *)0x080EDE48, u16 *, 4), M2C_FIELD(var_r5_1031, s16 *, 2) - 1, M2C_FIELD(var_r5_1031, s16 *, 6) - 3);
            M2C_FIELD(var_r5_1031, s32 *, 0) = (s32) (M2C_FIELD(var_r5_1031, s32 *, 0) + var_r5_1031->field_000c);
            var_r5_1031->field_0004 += var_r5_1031->field_0010;
            var_r5_1031->field_0018 -= 1;
        }
        var_r8_1032 += 1;
        var_r5_1031 += 0x1C;
    } while (var_r8_1032 != 0xC0);
    if (var_fp_800 == 0x30) {
        Func_080f9010(0x88);
    }
    if (var_fp_800 > 0x28) {
        M2C_FIELD(sp4C, s32 *, 0x7780) = 0;
        M2C_FIELD(sp4C, s32 *, 0x7784) = 0x4B;
        var_r8_1094 = 0;
        var_r5_1095 = -8;
        do {
            temp_r1_1106 = sp4C + ((Func_08004458() & 3) * 0x600);
            sp0 = 0x30;
            sp4 = 0x20;
            Func_08007300(sp50, temp_r1_1106, (*(sp14 + var_r8_1094) - sp10) + 0x78, var_r5_1095);
            var_r8_1094 += 1;
            var_r5_1095 += 8;
        } while (var_r8_1094 != 0x10);
    }
    if (var_fp_800 > 0x40) {
        M2C_FIELD(sp4C, s32 *, 0x7780) = 2;
    }
    if (var_fp_800 == 0x3A) {
        var_r8_1143 = 0;
        if (M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r6_1147 = 0x24;
            do {
                sp0 = 0;
                Func_080d6888(M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s16 *, var_r6_1147), 0xE, 5, -1);
                var_r8_1143 += 1;
                var_r6_1147 += 2;
            } while (var_r8_1143 != M2C_FIELD(M2C_FIELD(sp4C, void **, 0x7828), s32 *, 0x14));
        }
    }
    Func_080e155c(8, 8);
    Func_080cd52c();
    M2C_FIELD(sp4C, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    var_fp_800 += 1;
    sp10 += 0xC;
    if (var_fp_800 != 0x60) {
        goto loop_88;
    }
    Func_080b50e8(0x86);
    var_r8_1192 = 0;
    var_r6_1193 = sp4C + 0x77D8;
    do {
        temp_r0_1195 = *var_r6_1193;
        var_r6_1193 += 4;
        Func_08009038(temp_r0_1195);
        var_r8_1192 += 1;
    } while (var_r8_1192 != 9);
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
