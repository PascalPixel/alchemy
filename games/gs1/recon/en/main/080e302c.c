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

void Func_080e302c(s32 arg0) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    u32 sp1C;
    u32 sp20;
    u32 sp24;
    u32 sp28;
    void *sp2C;
    s32 sp30;
    s32 sp34;
    M2C_UNK sp38;
    s32 *var_r2_424;
    s32 *var_r5_1095;
    s32 temp_r0_1097;
    s32 temp_r0_399;
    s32 temp_r0_402;
    s32 temp_r0_897;
    s32 temp_r1_534;
    s32 temp_r2_725;
    s32 temp_r2_859;
    s32 temp_r2_920;
    s32 temp_r3_260;
    s32 temp_r3_30;
    s32 temp_r3_413;
    s32 temp_r3_62;
    s32 temp_r3_840;
    s32 temp_r3_949;
    s32 temp_r4_777;
    s32 var_fp_477;
    s32 var_r0_127;
    s32 var_r0_955;
    s32 var_r1_128;
    s32 var_r1_303;
    s32 var_r3_520;
    s32 var_r3_865;
    s32 var_r3_975;
    s32 var_r4_129;
    s32 var_r4_173;
    s32 var_r4_599;
    s32 var_r4_668;
    s32 var_r4_899;
    s32 var_r5_288;
    s32 var_r5_579;
    s32 var_r5_648;
    s32 var_r6_1026;
    s32 var_r6_174;
    s32 var_r6_214;
    s32 var_r7_1019;
    s32 var_r7_1094;
    s32 var_r7_117;
    s32 var_r7_179;
    s32 var_r7_217;
    s32 var_r7_240;
    s32 var_r7_289;
    s32 var_r7_364;
    s32 var_r7_423;
    s32 var_r7_434;
    s32 var_r7_525;
    s32 var_r7_595;
    s32 var_r7_664;
    s32 var_r7_768;
    s32 var_r8_184;
    s32 var_r8_257;
    s32 var_r8_831;
    s32 var_sl_823;
    struct M2cAggregate_absolute_02010000 *var_r2_190;
    struct M2cAggregate_absolute_02010000 *var_r2_222;
    struct M2cAggregate_absolute_02010000 *var_r5_171;
    struct M2cAggregate_absolute_02010000 *var_r5_433;
    struct M2cAggregate_absolute_02010000 *var_r5_767;
    u16 *var_r5_115;
    u16 temp_r2_119;
    u16 var_r3_34;
    u32 temp_r0_642;
    u32 temp_r0_776;
    u32 temp_r2_797;
    u32 temp_r3_120;
    u32 temp_r3_771;
    u8 *var_lr_172;
    u8 *var_r0_253;
    u8 *var_r1_189;
    u8 *var_r1_221;
    u8 *var_r1_252;
    u8 *var_r1_422;
    u8 *var_r2_242;
    u8 *var_r2_265;
    u8 temp_r0_544;
    u8 temp_r2_558;
    u8 temp_r3_224;
    u8 temp_r3_605;
    u8 temp_r3_674;
    u8 temp_r4_535;
    void **temp_r5_25;
    void *temp_r0_294;
    void *temp_r1_311;
    void *temp_r1_91;
    void *temp_r2_56;
    void *temp_r6_719;
    void *temp_r6_744;
    void *var_r5_366;
    void *var_r5_838;

    sp2C = M2C_FIELD(&absolute_03001eec, void **, 0);
    sp28 = absolute_03001eec.field_0004;
    sp1C = absolute_03001eec.field_0008;
    temp_r5_25 = (void **)((u8 *)sp2C + 0x7828);
    M2C_FIELD(M2C_FIELD(&absolute_03001eec, void **, 0), s32 *, 0x7828) = arg0;
    Func_080cd594(0);
    temp_r3_30 = M2C_FIELD(*temp_r5_25, s32 *, 0x18);
    if (temp_r3_30 == 0) {
        var_r3_34 = 0xCC;
        goto block_5;
    }
    if (temp_r3_30 == 1) {
        var_r3_34 = 0xAA;
block_5:
        absolute_04000020.field_0000 = var_r3_34;
    }
    temp_r2_56 = M2C_FIELD(sp2C, void **, 0x7828);
    if (M2C_FIELD(temp_r2_56, s32 *, 4) == 1) {
        sp18 = 8;
        temp_r3_62 = M2C_FIELD(temp_r2_56, s32 *, 0x18);
        sp14 = 0x28;
        if (temp_r3_62 != 0) {
            sp14 = 0x24;
            if (temp_r3_62 != 1) {
                sp14 = 0x28;
            }
        }
    } else {
        sp18 = -0x10;
        sp14 = -0xC;
    }
    temp_r1_91 = M2C_FIELD(sp2C, void **, 0x7828);
    absolute_04000028.field_0000 = *(const u8 *)(0x080EEDA6 + M2C_FIELD(temp_r1_91, s32 *, 0x18) + (M2C_FIELD(temp_r1_91, s32 *, 4) * 3)) << 8;
    Func_080e0524(0x7B, &absolute_02010000, 1, 0);
    Func_080072f0(0x05000000, Func_08002f40(0x7C), 0x80, 0x03001388);
    var_r5_115 = (u16 *)0x05000002;
    var_r7_117 = 0;
    do {
        temp_r2_119 = *var_r5_115;
        temp_r3_120 = temp_r2_119 << 0x10;
        var_r0_127 = ((temp_r3_120 >> 0x1A) & 0x1F) - 8;
        var_r1_128 = ((temp_r3_120 >> 0x15) & 0x1F) - 8;
        var_r4_129 = (0x1F & temp_r2_119) - 8;
        if (var_r0_127 < 0) {
            var_r0_127 = 0;
        }
        if (var_r1_128 < 0) {
            var_r1_128 = 0;
        }
        if (var_r4_129 < 0) {
            var_r4_129 = 0;
        }
        var_r7_117 += 1;
        *var_r5_115 = (var_r0_127 << 0xA) | (var_r1_128 << 5) | var_r4_129;
        var_r5_115 += 2;
    } while (var_r7_117 != 0x3F);
    var_r5_171 = &absolute_02010000;
    var_lr_172 = (u8 *)0x080EED90;
    var_r4_173 = 0;
    var_r6_174 = 0;
    do {
        var_r7_179 = 0;
loop_21:
        var_r8_184 = 0;
        if (*var_lr_172 != 0) {
            var_r1_189 = var_r4_173 + sp2C;
            var_r2_190 = var_r5_171;
            do {
                *var_r1_189 = var_r2_190->unknown_0000[0];
                var_r8_184 += 1;
                var_r2_190 += 1;
                var_r1_189 += 1;
                var_r4_173 += 1;
            } while (var_r8_184 != *((u8 *)0x080EED90 + var_r6_174));
        }
        var_r7_179 += 1;
        if (var_r7_179 != 0x20) {
            goto loop_21;
        }
        var_r6_174 += 1;
        var_r5_171 = (struct M2cAggregate_absolute_02010000 *) &var_r5_171->unknown_0000[*var_lr_172];
        var_lr_172 += 1;
    } while (var_r6_174 != 9);
    var_r6_214 = 0;
    do {
        var_r7_217 = 0;
loop_29:
        var_r1_221 = var_r4_173 + sp2C;
        var_r2_222 = var_r5_171;
loop_30:
        temp_r3_224 = var_r2_222->unknown_0000[0];
        var_r2_222 += 1;
        *var_r1_221 = temp_r3_224;
        var_r4_173 += 1;
        var_r1_221 += 1;
        if (var_r2_222 != &var_r5_171->unknown_0024[0xC]) {
            goto loop_30;
        }
        var_r7_217 += 1;
        if (var_r7_217 != 3) {
            goto loop_29;
        }
        var_r6_214 += 1;
        var_r5_171 += 0x30;
    } while (var_r6_214 != 0x20);
    var_r7_240 = 0;
    var_r2_242 = var_r4_173 + sp2C;
    do {
        var_r7_240 += 1;
        *var_r2_242 = var_r5_171->unknown_0000[0];
        var_r5_171 += 1;
        var_r2_242 += 1;
        var_r4_173 += 1;
    } while (var_r7_240 != 0x3F0);
    var_r1_252 = (u8 *)0x080EEDA0;
    var_r0_253 = (u8 *)0x080EEDA3;
    do {
        var_r8_257 = 0;
        temp_r3_260 = *var_r0_253 * *var_r1_252;
        if (temp_r3_260 != 0) {
            var_r2_265 = var_r4_173 + sp2C;
            do {
                *var_r2_265 = var_r5_171->unknown_0000[0];
                var_r8_257 += 1;
                var_r2_265 += 1;
                var_r5_171 += 1;
                var_r4_173 += 1;
            } while (var_r8_257 != temp_r3_260);
        }
        var_r1_252 += 1;
        var_r0_253 += 1;
    } while (var_r1_252 != (u8 *)0x080EEDA3);
    Func_080e0524(0x73, (struct M2cAggregate_absolute_02010000 *) sp1C, 0, 0);
    var_r5_288 = 0x77D8;
    var_r7_289 = 0;
    do {
        temp_r0_294 = Func_08009030(0x186);
        M2C_FIELD(sp2C, void **, var_r5_288) = temp_r0_294;
        if (temp_r0_294 != NULL) {
            M2C_FIELD(temp_r0_294, s8 *, 0x26) = 0;
            var_r1_303 = var_r7_289;
            if (var_r7_289 < 0) {
                var_r1_303 = var_r7_289 + 3;
            }
            Func_08009020(temp_r0_294, var_r1_303 >> 2);
            temp_r1_311 = M2C_FIELD(sp2C, void **, var_r5_288);
            M2C_FIELD(temp_r1_311, u8 *, 9) = (u8) ((M2C_FIELD(temp_r1_311, u8 *, 9) & ~0xC) | 4);
        }
        var_r7_289 += 1;
        var_r5_288 += 4;
    } while (var_r7_289 != 0xB);
    Func_080ed408(0x2E, 7, 7, 3, 2);
    sp20 = absolute_03001e50.field_00b8;
    Func_080ed408(0x2F, 7, 7, 7, 2);
    absolute_04000050.field_0000 = 0x3F46;
    absolute_04000050.field_0002 = 0x1010;
    M2C_FIELD(sp2C, s32 *, 0x7780) = 2;
    M2C_FIELD(sp2C, s32 *, 0x7784) = 0x4B;
    sp24 = absolute_03001e50.field_00bc;
    Func_080041d8(0x080CD261, 0x480);
    var_r7_364 = 0;
    var_r5_366 = sp2C + 0x7198;
    do {
        temp_r0_399 = (Func_08004458() & 0xF) + 0x58;
        M2C_FIELD(var_r5_366, s32 *, 4) = 0x80;
        M2C_FIELD(var_r5_366, s32 *, 0) = temp_r0_399;
        temp_r0_402 = Func_08004458();
        M2C_FIELD(var_r5_366, s32 *, 0x10) = 1;
        M2C_FIELD(var_r5_366, s32 *, 0x14) = 0x8000;
        M2C_FIELD(var_r5_366, s32 *, 0xC) = var_r7_364;
        temp_r3_413 = 0x2C - (var_r7_364 * 4);
        var_r7_364 += 1;
        M2C_FIELD(var_r5_366, s32 *, 8) = (s32) ((temp_r0_402 & 0xF) + 2);
        M2C_FIELD(var_r5_366, s32 *, 0x18) = temp_r3_413;
        var_r5_366 += 0x1C;
    } while (var_r7_364 != 0xB);
    var_r1_422 = (u8 *)0x080EEDAC;
    var_r7_423 = 0;
    var_r2_424 = sp2C + 0x7240;
    do {
        var_r7_423 += 1;
        *var_r2_424 = (s32) *var_r1_422;
        var_r1_422 += 1;
        var_r2_424 += 0x1C;
    } while (var_r7_423 != 6);
    var_r5_433 = &absolute_02010000;
    var_r7_434 = 0;
    do {
        M2C_FIELD(var_r5_433, s32 *, 0) = (s32) (((Func_08004458() & 0x3F) + sp18 + 0x20) << 0x10);
        var_r5_433->field_0004 = ((7 & Func_08004458()) + 0x60) << 0x10;
        var_r5_433->field_0010 = ((Func_08004458() & 0x3F) + 0x20) << 0xD;
        var_r7_434 += 1;
        var_r5_433->field_0018 = 0x1F & Func_08004458();
        var_r5_433 += 0x1C;
    } while (var_r7_434 != 0x100);
    absolute_0400000c.field_0000 = 0x785;
    M2C_FIELD(sp2C, s32 *, 0x77A8) = 0xFA;
    sp10 = sp18 + 0x40;
    var_fp_477 = 0;
loop_54:
    if (var_fp_477 == 0) {
        Func_080f9010(0xD4);
    }
    if (var_fp_477 == 0x28) {
        Func_080f9010(0x8D);
    }
    if (var_fp_477 == 0x60) {
        Func_080f9010(0x91);
    }
    if (var_fp_477 == 0x78) {
        Func_080b50e8(0x86);
    }
    if (var_fp_477 <= 0x51) {
        var_r3_520 = var_fp_477;
        if (var_fp_477 < 0) {
            var_r3_520 += 3;
        }
        var_r7_525 = var_r3_520 >> 2;
        if (var_r7_525 > 2) {
            var_r7_525 = (1 & var_r7_525) + 1;
        }
        temp_r1_534 = var_r7_525 * 2;
        temp_r4_535 = *(const u8 *)(0x080EEDA0 + var_r7_525);
        temp_r0_544 = *(const u8 *)(0x080EEDA3 + var_r7_525);
        Func_080072f4(sp28, sp2C + *(const u16 *)(0x080EED9A + temp_r1_534), (sp18 - temp_r4_535) + 0x40, 0x74 - temp_r0_544, (u32) temp_r4_535, (s32) temp_r0_544);
        temp_r2_558 = *(const u8 *)(0x080EEDA3 + var_r7_525);
        Func_080072fc(sp28, sp2C + *(const u16 *)(0x080EED9A + temp_r1_534), sp10, 0x74 - temp_r2_558, (s32) *(const u8 *)(0x080EEDA0 + var_r7_525), (s32) temp_r2_558);
    }
    if ((u32) (var_fp_477 - 0xC) <= 0x4BU) {
        var_r5_579 = Func_080022ec(var_fp_477 - 0x40, 3);
        if (var_r5_579 < 0) {
            var_r5_579 = 0;
        }
        if (var_r5_579 > 7) {
            var_r5_579 = 7;
        }
        sp8 = var_r5_579 * 2;
        var_r7_595 = 0;
        var_r4_599 = -0xC;
        do {
            temp_r3_605 = *(const u8 *)(0x080EED90 + var_r5_579);
            spC = var_r4_599;
            Func_080072fc(sp28, sp2C + *(const u16 *)(0x080EED7E + sp8), (sp18 - temp_r3_605) + 0x40, var_r4_599, (s32) temp_r3_605, 0x20);
            Func_080072fc(sp28, sp2C + *(const u16 *)(0x080EED7E + sp8), sp18 + 0x40, var_r4_599, (s32) *(const u8 *)(0x080EED90 + var_r5_579), 0x20);
            var_r7_595 += 1;
            var_r4_599 += 0x20;
        } while (var_r7_595 != 4);
    }
    temp_r0_642 = var_fp_477 - 0xA0;
    if (temp_r0_642 <= 0x17U) {
        var_r5_648 = 7 - Func_080022ec(temp_r0_642, 3);
        if (var_r5_648 < 0) {
            var_r5_648 = 0;
        }
        if (var_r5_648 > 7) {
            var_r5_648 = 7;
        }
        sp8 = var_r5_648 * 2;
        var_r7_664 = 0;
        var_r4_668 = -0xC;
        do {
            temp_r3_674 = *(const u8 *)(0x080EED90 + var_r5_648);
            spC = var_r4_668;
            Func_080072fc(sp28, sp2C + *(const u16 *)(0x080EED7E + sp8), (sp18 - temp_r3_674) + 0x40, var_r4_668, (s32) temp_r3_674, 0x20);
            Func_080072fc(sp28, sp2C + *(const u16 *)(0x080EED7E + sp8), sp18 + 0x40, var_r4_668, (s32) *(const u8 *)(0x080EED90 + var_r5_648), 0x20);
            var_r7_664 += 1;
            var_r4_668 += 0x20;
        } while (var_r7_664 != 4);
    }
    if ((u32) (var_fp_477 - 0x58) <= 0x47U) {
        temp_r6_719 = sp2C + 0x13C0;
        temp_r2_725 = sp18 + 0x10;
        Func_0800730c(sp28, temp_r6_719, temp_r2_725, 0, 0x30, 0x60);
        Func_08007304(sp28, temp_r6_719, sp10, 0, 0x30, 0x60);
        temp_r6_744 = sp2C + 0x25C0;
        Func_0800730c(sp28, temp_r6_744, temp_r2_725, 0x60, 0x30, 0x15);
        Func_08007304(sp28, temp_r6_744, sp10, 0x60, 0x30, 0x15);
    }
    if (var_fp_477 > 0x57) {
        var_r5_767 = &absolute_02010000;
        var_r7_768 = 0;
        do {
            temp_r3_771 = var_r5_767->field_0018;
            if (temp_r3_771 == 0) {
                temp_r0_776 = (3 & var_r7_768) + 5;
                temp_r4_777 = temp_r0_776 * 2;
                Func_080072f4(sp28, sp1C + *(const u16 *)(0x080EDE48 + temp_r4_777 - 2), M2C_FIELD(var_r5_767, s16 *, 2) - (temp_r0_776 >> 1), M2C_FIELD(var_r5_767, s16 *, 6) - temp_r0_776, temp_r0_776, temp_r4_777);
                temp_r2_797 = var_r5_767->field_0004 - var_r5_767->field_0010;
                var_r5_767->field_0004 = temp_r2_797;
                if (((s32) temp_r2_797 < 0) && (var_fp_477 <= 0x9F)) {
                    var_r5_767->field_0004 = 0x600000;
                }
            } else {
                var_r5_767->field_0018 = temp_r3_771 - 1;
            }
            var_r7_768 += 1;
            var_r5_767 += 0x1C;
        } while (var_r7_768 != 0x40);
    }
    if (var_fp_477 <= 4) {

    } else {
        var_sl_823 = 6;
        if (var_fp_477 > 0x47) {
            var_sl_823 = 0xB;
        }
        var_r8_831 = 0;
        if (var_sl_823 == 0) {

        } else {
            var_r5_838 = sp2C + 0x7198;
loop_100:
            temp_r3_840 = M2C_FIELD(var_r5_838, s32 *, 0x18);
            if (temp_r3_840 != 0) {
                var_r3_975 = temp_r3_840 - 1;
                goto block_121;
            }
            sp30 = M2C_FIELD((void *)0x080EDAB0, s32 *, 0);
            sp34 = M2C_FIELD((void *)0x080EDAB0, s32 *, 4);
            if (var_fp_477 > 0x47) {
                temp_r2_859 = (var_r8_831 << 0xC) + 0x8000;
                sp30 = temp_r2_859;
                var_r3_865 = (M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x18) << 0xE) + temp_r2_859;
            } else {
                var_r3_865 = 0x8000;
            }
            sp30 = var_r3_865;
            M2C_FIELD(&sp30, s32 *, 4) = var_r3_865;
            M2C_FIELD(&sp38, s32 *, 0xC) = 0;
            M2C_FIELD(&sp38, s32 *, 0) = (s32) ((M2C_FIELD(var_r5_838, s32 *, 0) + (sp14 * 2)) << 0x10);
            M2C_FIELD(&sp38, s32 *, 8) = 0x02000000;
            M2C_FIELD(&sp38, s32 *, 4) = (s32) (0x02000000 - (M2C_FIELD(var_r5_838, s32 *, 4) << 0x10));
            temp_r0_897 = Func_080022fc(((s32) (((u32) var_fp_477 >> 0x1F) + var_fp_477) >> 1) + var_r8_831, 0xB);
            var_r4_899 = -1;
            if (temp_r0_897 != -1) {
                spC = -1;
                Func_08009008(M2C_FIELD(sp2C, void **, (temp_r0_897 * 4) + 0x77D8), &sp38, &sp30, 0);
                var_r4_899 = spC;
            }
            M2C_FIELD(var_r5_838, s32 *, 4) = (s32) (M2C_FIELD(var_r5_838, s32 *, 4) - M2C_FIELD(var_r5_838, s32 *, 8));
            temp_r2_920 = M2C_FIELD(var_r5_838, s32 *, 0xC) + M2C_FIELD(var_r5_838, s32 *, 0x10);
            M2C_FIELD(var_r5_838, s32 *, 0xC) = temp_r2_920;
            if (temp_r2_920 > 0xC) {
                M2C_FIELD(var_r5_838, s32 *, 0xC) = (s32) (temp_r2_920 - 0xC);
            }
            if ((s32) M2C_FIELD(var_r5_838, s32 *, 4) < 0) {
                if (var_fp_477 > 0x9F) {
                    M2C_FIELD(var_r5_838, s32 *, 0x18) = var_r4_899;
                } else {
                    if (var_fp_477 > 0x57) {
                        M2C_FIELD(var_r5_838, s32 *, 8) = (s32) ((7 & Func_08004458()) + 8);
                        temp_r3_949 = M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x18);
                        if (temp_r3_949 == 0) {
                            var_r0_955 = Func_08002304(Func_08004458(), 0x60) + 0x2A;
                        } else if (temp_r3_949 == 1) {
                            var_r0_955 = Func_08002304(Func_08004458(), 0x70) + 0x22;
                        } else {
                            var_r0_955 = Func_08002304(Func_08004458(), 0xA0) + 0xA;
                        }
                        M2C_FIELD(var_r5_838, s32 *, 0) = var_r0_955;
                    }
                    M2C_FIELD(var_r5_838, s32 *, 4) = 0x80;
                    var_r3_975 = 8;
block_121:
                    M2C_FIELD(var_r5_838, s32 *, 0x18) = var_r3_975;
                }
            }
            var_r8_831 += 1;
            var_r5_838 += 0x1C;
            if (var_r8_831 != var_sl_823) {
                goto loop_100;
            }
        }
    }
    if (var_fp_477 <= 0x9E) {
        var_r7_1019 = 0;
        if (M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r6_1026 = 0x24;
            do {
                if (var_fp_477 > 0x55) {
                    if (Func_080022fc(var_fp_477, 0xC) == 0) {
                        Func_080d6888(M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s16 *, var_r6_1026), 7, 5, var_r7_1019, 6);
                    }
                    if (!(var_fp_477 & 3)) {
                        Func_080b5088(M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s16 *, var_r6_1026), 5);
                    }
                }
                var_r7_1019 += 1;
                var_r6_1026 += 2;
            } while (var_r7_1019 != M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x14));
        }
    }
    if ((u32) (var_fp_477 - 0x5A) > 0x46U) {
        Func_080e155c(2, 2);
    } else {
        Func_080e155c(8, 8);
    }
    Func_080cd52c();
    M2C_FIELD(sp2C, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    var_fp_477 += 1;
    if (var_fp_477 != 0xC0) {
        goto loop_54;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    var_r7_1094 = 0;
    var_r5_1095 = sp2C + 0x77D8;
    do {
        temp_r0_1097 = *var_r5_1095;
        var_r5_1095 += 4;
        var_r7_1094 += 1;
        Func_08009038(temp_r0_1097);
    } while (var_r7_1094 != 0xB);
    Func_080cdbc0();
}
