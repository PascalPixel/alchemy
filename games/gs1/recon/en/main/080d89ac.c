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

void **Func_080b5098(s32 actor_id);

#define BattleEffectA Func_080d89ac

void BattleEffectA(s32 arg0, u32 arg1) {
    u32 *sp0;                                       /* compiler-managed */
    s32 *sp4;                                       /* compiler-managed */
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    u32 sp18;
    u32 **sp1C;
    u8 *sp20;
    struct M2cAggregate_deref_absolute_03001e80_0 *sp24;
    u32 **sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    u32 sp38;
    s32 sp3C;
    s32 sp40;
    u32 sp44;
    void *sp48;
    s32 sp54;
    u32 sp58;
    M2C_UNK sp68;
    s32 sp74;
    s32 temp_r0_887;
    s32 temp_r1_44;
    s32 temp_r1_796;
    s32 temp_r1_951;
    s32 temp_r2_792;
    s32 temp_r3_39;
    s32 temp_r3_421;
    s32 temp_r3_656;
    s32 temp_r3_786;
    s32 temp_r3_790;
    s32 temp_r3_794;
    s32 temp_r3_90;
    s32 temp_r5_301;
    s32 temp_r5_304;
    s32 temp_r5_708;
    s32 temp_r5_913;
    s32 temp_r5_919;
    s32 temp_r5_925;
    s32 temp_r7_802;
    s32 temp_r8_489;
    s32 temp_r9_580;
    s32 var_fp_449;
    s32 var_r0_141;
    s32 var_r0_169;
    s32 var_r2_764;
    s32 var_r2_839;
    s32 var_r3_244;
    s32 var_r3_814;
    s32 var_r3_825;
    s32 var_r5_497;
    s32 var_r5_588;
    s32 var_r8_210;
    s32 var_r8_252;
    s32 var_r8_293;
    s32 var_r8_344;
    s32 var_r8_378;
    struct M2cAggregate_absolute_02010000 *var_r5_209;
    struct M2cAggregate_absolute_02010000 *var_r5_251;
    struct M2cAggregate_absolute_02010000 *var_r5_343;
    struct M2cAggregate_absolute_02010000 *var_r5_377;
    struct M2cAggregate_absolute_02010000 *var_r7_292;
    u16 temp_r6_298;
    u32 **temp_r3_198;
    u32 temp_r4_875;
    u32 var_r3_770;
    u32 var_r4_848;
    u32 var_r8_743;
    u8 *var_r6_753;
    u8 temp_r0_226;
    u8 temp_r0_873;
    void **temp_r5_27;
    void **var_r3_373;
    void *temp_r5_33;
    void *temp_r5_657;
    void *temp_r6_599;
    void *temp_r7_508;

    sp48 = M2C_FIELD(&absolute_03001eec, void **, 0);
    sp44 = absolute_03001eec.field_0004;
    sp38 = absolute_03001eec.field_0008;
    temp_r5_27 = M2C_FIELD(&absolute_03001eec, void **, 0) + 0x7828;
    sp30 = 0;
    M2C_FIELD(M2C_FIELD(&absolute_03001eec, void **, 0), s32 *, 0x7828) = arg0;
    Func_080cd594(0);
    temp_r5_33 = *temp_r5_27;
    if (M2C_FIELD(temp_r5_33, s32 *, 0x1C) == 1) {
        temp_r3_39 = 6 ^ arg1;
        temp_r1_44 = 2 - ((u32) ((0 - temp_r3_39) | temp_r3_39) >> 0x1F);
        if ((arg1 == 6) || (arg1 == 0)) {
            sp0 = &sp58;
            sp4 = &sp54;
            Func_080de2f8(arg0, temp_r1_44, M2C_FIELD(temp_r5_33, s32 *, 4), 0);
        } else {
            sp0 = &sp58;
            sp4 = &sp54;
            Func_080de2f8(arg0, temp_r1_44, M2C_FIELD(temp_r5_33, s32 *, 4), 1);
        }
        M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x18) = 0;
    }
    if (arg1 == 0) {
        Func_080e396c(M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s16 *, 0x24), &sp74);
        temp_r3_90 = 0x40 - sp74;
        sp30 = temp_r3_90;
        absolute_04000028.field_0000 = temp_r3_90 << 8;
        *(u16 *)((u8 *)&absolute_04000028 - 8) = 0x100;
        sp2C = 0;
    } else {
        sp2C = 1;
    }
    Func_080e0524(0x73, sp38, 0, 0);
    Func_080e0524(0xBA, (u32) sp48, 0, 0);
    if ((arg1 <= 1U) || (arg1 == 3) || (arg1 == 4) || (arg1 == 5)) {
        if (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x18) == 0) {
            var_r0_141 = 0xB3;
        } else {
            var_r0_141 = 0xB9;
        }
    } else if (arg1 == 6) {
        var_r0_141 = 0x8D;
    } else {
        var_r0_141 = 0xC0;
    }
    Func_080072f0(0x05000000, Func_08002f40(var_r0_141), 0x80, 0x03001388);
    if (sp2C == 0) {
        if (arg1 == 6) {
            var_r0_169 = 0x8D;
        } else {
            var_r0_169 = 0x91;
        }
    } else if (arg1 == 6) {
        var_r0_169 = 0x8E;
    } else {
        var_r0_169 = 0x92;
    }
    Func_08005340(Func_08002f40(var_r0_169) + 0x80, sp48 + 0x1000);
    temp_r3_198 = &sp0 + 0x4C;
    sp28 = temp_r3_198;
    Func_080cef64(M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 4), temp_r3_198);
    switch (arg1) {                                 /* switch 1; irregular */
    case 0:                                         /* switch 1 */
    case 6:                                         /* switch 1 */
        var_r5_209 = &absolute_02010000;
        var_r8_210 = 0;
        do {
            M2C_FIELD(var_r5_209, s32 *, 0) = (s32) ((Func_08004458() - 0x7F) << 0xF);
            var_r5_209->field_0004 = ((0x7F & Func_08004458()) + 0x40) << 0xF;
            temp_r0_226 = Func_08004458();
            var_r5_209->field_0018 = 0;
            var_r8_210 += 1;
            var_r5_209->field_0008 = (temp_r0_226 - 0x7F) << 0xF;
            var_r5_209 += 0x1C;
        } while (var_r8_210 != 0x200);
        var_r3_244 = (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) * 8) + 0x58;
        break;
    case 1:                                         /* switch 1 */
        var_r5_251 = &absolute_02010000;
        var_r8_252 = 0;
        do {
            M2C_FIELD(var_r5_251, s32 *, 0) = (s32) ((Func_08004458() - 0x7F) << 0xF);
            var_r5_251->field_0004 = (Func_08004458() - 0x7F) << 0xF;
            var_r5_251->field_0008 = (Func_08004458() - 0x7F) << 0xF;
            var_r8_252 += 1;
            var_r5_251->field_0018 = 0;
            var_r5_251 += 0x1C;
        } while (var_r8_252 != 0x200);
        var_r3_244 = (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) * 8) + 0x58;
        break;
    case 2:                                         /* switch 1 */
        var_r7_292 = &absolute_02010000;
        var_r8_293 = 0;
        do {
            temp_r6_298 = (u16) Func_08004458();
            temp_r5_301 = 0x3F & Func_08004458();
            temp_r5_304 = temp_r5_301 + 0x20;
            M2C_FIELD(var_r7_292, s32 *, 0) = (s32) (temp_r5_304 * Func_08002322(temp_r6_298));
            var_r7_292->field_0004 = 0xFFCE0000;
            var_r7_292->field_0008 = temp_r5_304 * Func_0800231c(temp_r6_298);
            var_r7_292->field_0010 = ((0x1F & Func_08004458()) + 0x20) << 0xD;
            var_r7_292->field_0018 = 0;
            var_r8_293 += 1;
            var_r7_292 += 0x1C;
        } while (var_r8_293 != 0x200);
        var_r3_244 = (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) * 8) + 0x58;
        break;
    case 3:                                         /* switch 1 */
        var_r5_343 = &absolute_02010000;
        var_r8_344 = 0;
        do {
            M2C_FIELD(var_r5_343, s32 *, 0) = (s32) ((Func_08004458() - 0x7F) << 0xF);
            var_r5_343->field_0004 = (Func_08004458() - 0x7F) << 0xE;
            var_r8_344 += 1;
            var_r5_343->field_0008 = (Func_08004458() - 0x7F) << 0xF;
            var_r5_343->field_0018 = 0;
            var_r5_343 += 0x1C;
        } while (var_r8_344 != 0x200);
        var_r3_373 = sp48 + 0x7828;
block_47:
        var_r3_244 = (M2C_FIELD(*var_r3_373, s32 *, 0x14) * 8) + 0x48;
        break;
    default:                                        /* switch 1 */
        var_r5_377 = &absolute_02010000;
        var_r8_378 = 0;
        do {
            M2C_FIELD(var_r5_377, s32 *, 0) = (s32) ((Func_08004458() - 0x7F) << 0xF);
            var_r5_377->field_0004 = (Func_08004458() - 0x7F) << 0xF;
            var_r5_377->field_0008 = (Func_08004458() - 0x7F) << 0xF;
            var_r5_377->field_0018 = 0;
            var_r8_378 += 1;
            var_r5_377 += 0x1C;
        } while (var_r8_378 != 0x200);
        var_r3_373 = sp48 + 0x7828;
        goto block_47;
    }
    sp40 = var_r3_244;
    sp34 = 0x40;
    temp_r3_421 = M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x18);
    if (temp_r3_421 == 0) {
        sp34 = 0x20;
    } else if (temp_r3_421 == 2) {
        sp34 = 0x80;
    }
    M2C_FIELD(sp48, s32 *, 0x7780) = 2;
    M2C_FIELD(sp48, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    var_fp_449 = 0;
    if (sp40 == 0) {

    } else {
        spC = 0;
loop_55:
        sp24 = absolute_03001e80.field_0000;
        if (var_fp_449 == 0x28) {
            Func_080b50e8(0);
        }
        if (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x1C) != 1) {

        } else if (sp2C == 0) {
            temp_r8_489 = (((s32) (Func_08002322((u16) spC) * 0x14) >> 0x10) + sp58 + sp30) - 0x14;
            var_r5_497 = (((s32) (Func_0800231c((u16) spC) * 4) >> 0x10) + sp54) - 0x18;
            if (var_fp_449 > 0x20) {
                var_r5_497 = (var_r5_497 - (var_fp_449 * 2)) + 0x40;
            }
            temp_r7_508 = sp48 + 0x1000;
            sp0 = (u32 *)0x28;
            sp4 = (s32 *)0x28;
            Func_080072f4(sp44, temp_r7_508, temp_r8_489, var_r5_497);
            if (var_fp_449 <= 3) {
                sp0 = (u32 *)0x28;
                sp4 = (s32 *)0x28;
                Func_080072f4(sp44, temp_r7_508, temp_r8_489, var_r5_497);
            }
        } else {
            temp_r9_580 = (((s32) (Func_08002322((u16) spC) * 0xA) >> 0x10) + ((s32) (sp58 + (sp58 >> 0x1F)) >> 1)) - 0xA;
            var_r5_588 = (((s32) (Func_0800231c((u16) spC) * 4) >> 0x10) + sp54) - 0x18;
            if (var_fp_449 > 0x20) {
                var_r5_588 = (var_r5_588 - (var_fp_449 * 2)) + 0x40;
            }
            temp_r6_599 = sp48 + 0x1000;
            sp0 = (u32 *)0x14;
            sp4 = (s32 *)0x28;
            Func_080072f4(sp44, temp_r6_599, temp_r9_580, var_r5_588);
            if (var_fp_449 <= 3) {
                sp0 = (u32 *)0x14;
                sp4 = (s32 *)0x28;
                Func_080072f4(sp44, temp_r6_599, temp_r9_580, var_r5_588);
            }
        }
        sp3C = 0;
        if (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) == 0) {

        } else {
            sp20 = &sp24->unknown_0002[0xA];
            sp1C = &sp0 + 0x5C;
            sp14 = var_fp_449 << 9;
            sp10 = 0x24;
            sp8 = 0;
loop_72:
            temp_r3_656 = sp3C * 8;
            temp_r5_657 = *Func_080b5098(*(s16 *)((u8 *)M2C_FIELD(sp48, void **, 0x7828) + sp10));
            Func_080049ac();
            Func_080051d8(sp24, sp20);
            M2C_FIELD(sp1C, u32 **, 0) = M2C_FIELD(temp_r5_657, u32 **, 8);
            M2C_FIELD(sp1C, s32 *, 4) = 0x280000;
            M2C_FIELD(sp1C, s32 *, 8) = (s32) M2C_FIELD(temp_r5_657, s32 *, 0x10);
            Func_08004cb4(sp1C);
            if (var_fp_449 == (temp_r3_656 + 0x14)) {
                Func_080f9010(0x7E);
            }
            if (var_fp_449 == (temp_r3_656 + 0x24)) {
                sp0 = (u32 *)0x1C;
                Func_080d6888(*(s16 *)((u8 *)M2C_FIELD(sp48, void **, 0x7828) + sp10), 7, -1, sp3C);
            }
            if (var_fp_449 <= temp_r3_656) {

            } else {
                switch (arg1) {                     /* switch 2; irregular */
                case 1:                             /* switch 2 */
                    temp_r5_708 = var_fp_449 << 9;
                    Func_08004bd4(temp_r5_708);
                    Func_08004c6c(temp_r5_708);
                    break;
                case 2:                             /* switch 2 */
                    Func_08004c1c((var_fp_449 - (sp3C * 0x28)) << 9);
                    break;
                case 0:                             /* switch 2 */
                case 6:                             /* switch 2 */
                case 3:                             /* switch 2 */
                    Func_08004c1c(sp14);
                    break;
                default:                            /* switch 2 */
                    Func_08004c1c(sp14);
                    Func_08004bd4(sp14);
                    break;
                }
                var_r8_743 = 0;
                if (sp34 == 0) {

                } else {
                    sp18 = arg1 - 3;
                    var_r6_753 = &absolute_02010000.unknown_0000[sp8];
loop_90:
                    if (sp18 <= 2U) {
                        var_r2_764 = ((s32) ((var_r8_743 >> 0x1F) + var_r8_743) >> 1) + temp_r3_656 + 0x20;
                    } else {
                        var_r2_764 = 0x10000;
                    }
                    var_r3_770 = var_r8_743;
                    if ((s32) var_r3_770 < 0) {
                        var_r3_770 += 3;
                    }
                    if (var_fp_449 <= (s32) (((s32) var_r3_770 >> 2) + temp_r3_656)) {

                    } else if (var_fp_449 >= var_r2_764) {

                    } else {
                        temp_r3_786 = (s32) M2C_FIELD(var_r6_753, s32 *, 0) >> 8;
                        temp_r3_790 = (s32) M2C_FIELD(var_r6_753, s32 *, 4) >> 8;
                        temp_r2_792 = temp_r3_790 * temp_r3_790;
                        temp_r3_794 = (s32) M2C_FIELD(var_r6_753, s32 *, 8) >> 8;
                        temp_r1_796 = temp_r3_794 * temp_r3_794;
                        temp_r7_802 = Func_080072f0((temp_r3_786 * temp_r3_786) + temp_r2_792 + temp_r1_796, temp_r1_796, temp_r2_792, 0x030001D8) >> 9;
                        if (temp_r7_802 != 0) {
                            Func_080e3944(var_r6_753, &sp68);
                            if (arg1 == 0) {
                                var_r3_814 = M2C_FIELD(&sp68, s32 *, 0) + sp30;
                            } else {
                                var_r3_814 = (s32) M2C_FIELD(&sp68, s32 *, 0) >> 1;
                            }
                            M2C_FIELD(&sp68, s32 *, 0) = var_r3_814;
                            M2C_FIELD(&sp68, s32 *, 4) = (s32) (M2C_FIELD(&sp68, s32 *, 4) + 0x10);
                            var_r3_825 = M2C_FIELD(&sp68, s32 *, 8);
                            if (var_r3_825 <= 0x139) {
                                var_r3_825 = 0x13A;
                                M2C_FIELD(&sp68, s32 *, 8) = 0x13A;
                            }
                            if (var_r3_825 > 0x27A) {
                                M2C_FIELD(&sp68, s32 *, 8) = 0x27A;
                                var_r3_825 = 0x27A;
                            }
                            var_r2_839 = var_r3_825 + 0xFFFFFEC6;
                            if (var_r2_839 < 0) {
                                var_r2_839 = var_r3_825 - 0xBB;
                            }
                            var_r4_848 = 3 - (var_r2_839 >> 7);
                            switch (arg1) {         /* switch 3; irregular */
                            case 0:                 /* switch 3 */
                                var_r4_848 = Func_080022fc((var_r8_743 * 4) + var_fp_449, 9);
                                /* fallthrough */
                            case 3:                 /* switch 3 */
                            case 4:                 /* switch 3 */
                            case 5:                 /* switch 3 */
                                temp_r0_873 = *(u8 *)(0x080EDE96 + var_r4_848);
                                temp_r4_875 = temp_r0_873 >> 1;
                                sp0 = (u32 *) temp_r0_873;
                                sp4 = (s32 *) temp_r0_873;
                                Func_080072f4(sp44, sp48 + *(u16 *)(0x080EDE84 + (var_r4_848 * 2)), M2C_FIELD(&sp68, s32 *, 0) - temp_r4_875, M2C_FIELD(&sp68, s32 *, 4) - temp_r4_875);
                                break;
                            default:                /* switch 3 */
                                temp_r0_887 = var_r4_848 * 2;
                                sp4 = (s32 *)temp_r0_887;
                                sp0 = (u32 *)var_r4_848;
                                Func_080072f4(sp44, sp38 + *(u16 *)(0x080EDE48 + (s32) (temp_r0_887 - 2)), M2C_FIELD(&sp68, s32 *, 0) - ((s32) (var_r4_848 + (var_r4_848 >> 0x1F)) >> 1), M2C_FIELD(&sp68, s32 *, 4) - var_r4_848);
                                break;
                            }
                            if ((arg1 <= 2U) || (arg1 == 6)) {
                                temp_r5_913 = M2C_FIELD(var_r6_753, s32 *, 0);
                                M2C_FIELD(var_r6_753, s32 *, 0) = (s32) (temp_r5_913 - Func_080022ec(temp_r5_913, temp_r7_802));
                                temp_r5_919 = M2C_FIELD(var_r6_753, s32 *, 4);
                                M2C_FIELD(var_r6_753, s32 *, 4) = (s32) (temp_r5_919 - Func_080022ec(temp_r5_919, temp_r7_802));
                                temp_r5_925 = M2C_FIELD(var_r6_753, s32 *, 8);
                                M2C_FIELD(var_r6_753, s32 *, 8) = (s32) (temp_r5_925 - Func_080022ec(temp_r5_925, temp_r7_802));
                            }
                        }
                    }
                    var_r8_743 += 1;
                    var_r6_753 += 0x1C;
                    if (var_r8_743 != sp34) {
                        goto loop_90;
                    }
                }
            }
            sp14 += 0xFFFFF000;
            temp_r1_951 = sp3C + 1;
            sp10 += 2;
            sp8 += 0x700;
            sp3C = temp_r1_951;
            if (temp_r1_951 != M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14)) {
                goto loop_72;
            }
        }
        Func_080cd52c();
        M2C_FIELD(sp48, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        var_fp_449 += 1;
        spC += 0x800;
        if (var_fp_449 != sp40) {
            goto loop_55;
        }
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
