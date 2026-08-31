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

#define BattleEffectB Func_080d91dc

void Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
typedef void (*BattleEffectDrawFn)(u32, void *, s32, s32, s32, s32);

void BattleEffectB(s32 arg0, s32 arg1) {
    void **heap_cursor;
    s32 *sp0;
    s32 *sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 **sp14;
    s32 **sp18;
    s32 **sp1C;
    u32 *sp20;
    struct M2cAggregate_absolute_03001e80 *sp24;
    struct M2cAggregate_deref_absolute_03001e80_0 *sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C;
    u32 sp40;
    void *sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp60;
    s32 sp64;
    s32 sp78;
    s32 **temp_r8_625;
    s32 **var_r1_620;
    s32 **var_r6_626;
    s32 *temp_r5_705;
    s32 temp_r0_390;
    s32 temp_r0_648;
    s32 temp_r0_688;
    s32 temp_r0_881;
    s32 temp_r0_932;
    s32 temp_r1_1069;
    s32 temp_r2_639;
    s32 temp_r3_29;
    s32 temp_r3_633;
    s32 temp_r3_637;
    s32 temp_r3_641;
    s32 temp_r4_412;
    s32 temp_r4_689;
    s32 temp_r5_1031;
    s32 temp_r5_369;
    s32 temp_r5_389;
    s32 temp_r5_610;
    s32 temp_r5_711;
    s32 temp_r5_717;
    s32 temp_r5_769;
    s32 temp_r5_826;
    s32 temp_r7_496;
    s32 temp_r7_649;
    s32 temp_ret_648;
    s32 temp_ret_720;
    s32 temp_sl_382;
    s32 var_fp_344;
    s32 var_r0_133;
    s32 var_r0_182;
    s32 var_r0_215;
    s32 var_r1_42;
    s32 var_r2_36;
    s32 var_r2_661;
    s32 var_r3_1022;
    s32 var_r3_680;
    s32 var_r3_760;
    s32 var_r3_817;
    s32 var_r3_874;
    s32 var_r7_414;
    s32 var_r9_478;
    s32 var_sl_245;
    s32 var_sl_624;
    struct M2cAggregate_absolute_02010000 *var_r5_244;
    u32 temp_r0_927;
    void **temp_r5_23;
    void *temp_r2_277;
    void *temp_r5_28;
    void *temp_r5_488;
    void *temp_r5_961;
    void *temp_r6_426;
    void *var_r1_216;

    sp48 = arg1;
    heap_cursor = (void **)0x03001EEC;
    sp44 = *heap_cursor++;
    temp_r5_23 = (void **)((u8 *)sp44 + 0x7828);
    sp40 = *heap_cursor;
    *temp_r5_23 = (void *)arg0;
    Func_080cd594(0);
    temp_r5_28 = *temp_r5_23;
    temp_r3_29 = M2C_FIELD(temp_r5_28, s32 *, 0x1C);
    if (temp_r3_29 == 1) {
        if (sp48 == 3) {
            var_r2_36 = M2C_FIELD(temp_r5_28, s32 *, 4) ^ temp_r3_29;
            var_r1_42 = 0;
            Func_080de2f8(arg0, var_r1_42, var_r2_36, 1, &sp50, &sp4C);
        } else if (sp48 == 2 || sp48 == 4) {
            var_r2_36 = M2C_FIELD(temp_r5_28, s32 *, 4) ^ temp_r3_29;
            var_r1_42 = 3;
            Func_080de2f8(arg0, var_r1_42, var_r2_36, 1, &sp50, &sp4C);
        } else {
            Func_080de2f8(arg0, 2,
                M2C_FIELD(temp_r5_28, s32 *, 4) ^ temp_r3_29,
                1, &sp50, &sp4C);
        }
        sp50 = Func_080022ec(sp50 * 4, 5);
    }
    absolute_04000020.field_0000 = 0xCC;
    Func_080e0524(0x76, sp44, 0, 0);
    Func_080e0524(0xB7, sp44 + 0x60E, 1, 1);
    switch (sp48) {                                 /* switch 2; irregular */
    case 3:                                         /* switch 2 */
    case 5:                                         /* switch 2 */
        Func_080e0524(0xB0, sp44 + 0x2B8E, 1, 1);
        if (sp48 == 3) {
            var_r0_133 = 0x93;
        } else {
            var_r0_133 = 0x8D;
        }
        Func_080072f0(0x05000000, Func_08002f40(var_r0_133), 0x80, 0x03001388);
        break;
    case 4:                                         /* switch 2 */
        Func_080e0524(0xA5, sp44 + 0x2B8E, 1, 1);
        break;
    case 0:                                         /* switch 2 */
        Func_080e0524(0x9C, sp44 + 0x2B8E, 1, 0);
block_22:
        switch (sp48) {                             /* switch 3; irregular */
        case 0:                                     /* switch 3 */
            var_r0_182 = 0x8D;
            break;
        case 2:                                     /* switch 3 */
        case 4:                                     /* switch 3 */
            var_r0_182 = 0x8F;
            break;
        case 1:                                     /* switch 3 */
            var_r0_182 = 0x8D;
            break;
        default:                                    /* switch 3 */
            var_r0_182 = 0xBB;
            break;
        }
        Func_080072f0(0x05000000, Func_08002f40(var_r0_182), 0x80, 0x03001388);
        break;
    default:                                        /* switch 2 */
        Func_080e0524(0x9B, sp44 + 0x2B8E, 1, 0);
        goto block_22;
    }
    switch (sp48) {                                 /* switch 4; irregular */
    case 3:                                         /* switch 4 */
        var_r0_215 = 0x93;
        var_r1_216 = sp44 + 0x65C0;
block_36:
        Func_080e0524(var_r0_215, var_r1_216, 1, 0);
        break;
    case 2:                                         /* switch 4 */
    case 4:                                         /* switch 4 */
        var_r0_215 = 0x8F;
        var_r1_216 = sp44 + 0x65C0;
        goto block_36;
    default:                                        /* switch 4 */
        Func_080e0524(0x8D, sp44 + 0x65C0, 1, 0);
        break;
    }
    var_r5_244 = &absolute_02010000;
    var_sl_245 = 0;
    do {
        M2C_FIELD(var_r5_244, s32 *, 0) = (s32) ((Func_08002304(Func_08004458(), 0xC8) - 0x64) << 0xE);
        var_r5_244->field_0004 = (Func_08002304(Func_08004458(), 0xC8) - 0x64) << 0xF;
        var_sl_245 += 1;
        var_r5_244->field_0008 = (Func_08002304(Func_08004458(), 0xC8) - 0x64) << 0xE;
        var_r5_244->field_0018 = 0;
        var_r5_244 += 0x1C;
    } while (var_sl_245 != 0x200);
    temp_r2_277 = M2C_FIELD(sp44, void **, 0x7828);
    if (M2C_FIELD(temp_r2_277, s32 *, 0x14) == 1) {
        Func_080e396c(M2C_FIELD(temp_r2_277, s16 *, 0x24), &sp78);
        sp30 = Func_080022ec((0 - sp78) * 4, 5) + 0x40;
    } else {
        sp30 = -0x40;
        if (M2C_FIELD(temp_r2_277, s32 *, 4) != 1) {
            sp30 = 0;
        }
    }
    absolute_04000028.field_0000 = sp30 << 8;
    M2C_FIELD(sp44, s32 *, 0x7780) = 2;
    M2C_FIELD(sp44, s32 *, 0x7784) = 0x32;
    Func_080041d8(0x080CD261, 0x480);
    sp2C = 7;
    if (M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 4) != 1) {
        sp2C = 3;
    }
    Func_080f9010(0x8E);
    var_fp_344 = 0;
    if ((M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14) * 8) == -0x6C) {

    } else {
        sp24 = &absolute_03001e80;
loop_49:
        sp28 = sp24->field_0000;
        if (var_fp_344 == 0x50) {
            Func_080b50e8(0);
        }
        if (M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x1C) == 1) {
            temp_r5_369 = var_fp_344 << 0xB;
            temp_sl_382 = (((s32) (Func_08002322(temp_r5_369) * 0x14) >> 0x10) + sp50 + sp30) - 0x14;
            temp_r5_389 = 4 ^ sp2C;
            temp_r0_390 = ((s32) (Func_0800231c(temp_r5_369) * 4) >> 0x10) + sp4C;
            sp0 = (s32 *)2;
            Func_080ed408(0x2E, 7, 7, temp_r5_389, 2);
            sp38 = M2C_FIELD(sp24, s32 *, 0x88);
            sp0 = (s32 *)3;
            Func_080ed408(0x2F, 7, 7, temp_r5_389, 3);
            temp_r4_412 = *(s32 *)0x03001F0C;
            var_r7_414 = temp_r0_390 - 0x18;
            sp3C = temp_r4_412;
            if (var_fp_344 > 0x20) {
                var_r7_414 = (var_r7_414 - (var_fp_344 * 2)) + 0x40;
            }
            temp_r6_426 = sp44 + 0x65C0;
            sp8 = temp_r4_412;
            sp0 = (s32 *)0x28;
            sp4 = (s32 *)0x28;
            ((BattleEffectDrawFn)sp38)(sp40, temp_r6_426, temp_sl_382, var_r7_414,
                0x28, 0x28);
            if (var_fp_344 <= 3) {
                sp0 = (s32 *)0x28;
                sp4 = (s32 *)0x28;
                ((BattleEffectDrawFn)sp3C)(sp40, temp_r6_426, temp_sl_382, var_r7_414,
                    0x28, 0x28);
            }
            Func_08002dd8(0x2F);
            Func_08002dd8(0x2E);
        }
        sp34 = 0;
        if (M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14) == 0) {

        } else {
            sp20 = &sp28->field_000c;
            sp1C = &sp0 + 0x54;
            sp18 = &sp0 + 0x60;
            sp14 = &sp0 + 0x6C;
            sp10 = 0;
            spC = 0x24;
            var_r9_478 = var_fp_344;
loop_60:
            temp_r5_488 = Func_080b5098(M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s16 *, spC));
            temp_r7_496 = Func_080022ec(Func_080b5070(M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s16 *, spC)) * 2, 3);
            if (var_fp_344 == (sp10 + 0x50)) {
                Func_080f9010(0xD4);
            }
            Func_080049ac();
            Func_080051d8(sp28, sp20);
            M2C_FIELD(sp1C, s32 **, 0) = M2C_FIELD(temp_r5_488, s32 **, 8);
            M2C_FIELD(sp1C, s32 *, 4) = temp_r7_496;
            M2C_FIELD(sp1C, s32 *, 8) = (s32) M2C_FIELD(temp_r5_488, s32 *, 0x10);
            Func_08004cb4(sp1C);
            if (var_fp_344 == (sp10 + 0x30)) {
                sp0 = (s32 *)0x10;
                Func_080d6888(M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s16 *, spC), 7, -1, sp34, 0x10);
            }
            if (var_fp_344 <= sp10) {

            } else {
                sp0 = (s32 *)2;
                Func_080ed408(0x2E, 7, 7, sp2C, 2);
                sp38 = *(s32 *)0x03001F08;
                sp0 = (s32 *)3;
                Func_080ed408(0x2F, 7, 7, sp2C, 3);
                sp3C = *(s32 *)0x03001F0C;
                switch (sp48) {                     /* switch 5; irregular */
                case 1:                             /* switch 5 */
                    break;
                case 0:                             /* switch 5 */
                    Func_08004bd4((0 - var_fp_344) << 0xA);
                    break;
                case 2:                             /* switch 5 */
                    Func_08004c1c(var_fp_344 << 0xA);
                    break;
                default:                            /* switch 5 */
                    temp_r5_610 = var_fp_344 << 0xA;
                    Func_08004c1c(temp_r5_610);
                    Func_08004c6c(temp_r5_610);
                    break;
                }
                var_r1_620 = sp18;
                var_sl_624 = 0;
                temp_r8_625 = var_r1_620;
                var_r6_626 = (sp34 * 0x1C0 * 4) + &absolute_02010000;
                do {
                    if (var_fp_344 > (s32) (sp10 + var_sl_624)) {
                        temp_r3_633 = (s32) M2C_FIELD(var_r6_626, s32 *, 0) >> 8;
                        temp_r3_637 = (s32) M2C_FIELD(var_r6_626, s32 *, 4) >> 8;
                        temp_r2_639 = temp_r3_637 * temp_r3_637;
                        temp_r3_641 = (s32) M2C_FIELD(var_r6_626, s32 *, 8) >> 8;
                        temp_ret_648 = Func_080072f0((temp_r3_633 * temp_r3_633) + temp_r2_639 + (temp_r3_641 * temp_r3_641), var_r1_620, temp_r2_639, 0x030001D8);
                        temp_r0_648 = temp_ret_648;
                        var_r1_620 = temp_r8_625;
                        temp_r7_649 = temp_r0_648 >> 9;
                        if (temp_r7_649 != 0) {
                            Func_080e3944(var_r6_626, temp_r8_625);
                            var_r2_661 = M2C_FIELD(temp_r8_625, s32 *, 8);
                            M2C_FIELD(temp_r8_625, s32 **, 0) = Func_080022ec((s32) M2C_FIELD(temp_r8_625, s32 **, 0) * 4, 5) + sp30;
                            if (var_r2_661 <= 0x139) {
                                M2C_FIELD(sp18, s32 *, 8) = 0x13A;
                                var_r2_661 = 0x13A;
                            }
                            if (var_r2_661 > 0x27A) {
                                M2C_FIELD(sp18, s32 *, 8) = 0x27A;
                                var_r2_661 = 0x27A;
                            }
                            var_r3_680 = var_r2_661 + 0xFFFFFEC6;
                            if (var_r3_680 < 0) {
                                var_r3_680 = var_r2_661 - 0xFB;
                            }
                            temp_r0_688 = 6 - (var_r3_680 >> 6);
                            temp_r4_689 = temp_r0_688 * 2;
                            sp0 = temp_r4_689;
                            sp4 = temp_r4_689;
                            ((BattleEffectDrawFn)sp3C)(sp40, sp44 + *(const u16 *)(0x080EDE5C + temp_r4_689 - 2), M2C_FIELD(sp18, s32 **, 0) - temp_r0_688, M2C_FIELD(sp18, s32 *, 4) - temp_r0_688, temp_r4_689, temp_r4_689);
                            temp_r5_705 = M2C_FIELD(var_r6_626, s32 **, 0);
                            M2C_FIELD(var_r6_626, s32 **, 0) = temp_r5_705 - Func_080022ec((s32) temp_r5_705, temp_r7_649);
                            temp_r5_711 = M2C_FIELD(var_r6_626, s32 *, 4);
                            M2C_FIELD(var_r6_626, s32 *, 4) = (s32) (temp_r5_711 - Func_080022ec(temp_r5_711, temp_r7_649));
                            temp_r5_717 = M2C_FIELD(var_r6_626, s32 *, 8);
                            temp_ret_720 = Func_080022ec(temp_r5_717, temp_r7_649);
                            var_r1_620 = temp_r8_625;
                            M2C_FIELD(var_r6_626, s32 *, 8) = (s32) (temp_r5_717 - temp_ret_720);
                        }
                    }
                    var_sl_624 += 1;
                    var_r6_626 += 0x1C;
                } while (var_sl_624 != 0x20);
                Func_08002dd8(0x2F);
                Func_08002dd8(0x2E);
            }
            M2C_FIELD(sp14, s32 **, 0) = NULL;
            M2C_FIELD(sp14, s32 *, 4) = 0;
            M2C_FIELD(sp14, s32 *, 8) = 0;
            Func_080e3944(sp14, &sp60);
            sp60 = Func_080022ec((s32) sp60 * 4, 5) + sp30;
            if ((var_fp_344 >= (s32) (sp10 + 0x34)) && (var_fp_344 < (s32) (sp10 + 0x4C))) {
                var_r3_760 = var_r9_478 - 0x34;
                if (var_r3_760 < 0) {
                    var_r3_760 += 3;
                }
                temp_r5_769 = Func_080022fc(var_r3_760 >> 2, 6);
                sp0 = (s32 *)2;
                Func_080ed408(0x2E, 7, 7, sp2C, 2);
                sp0 = (s32 *)0x28;
                sp4 = (s32 *)0x28;
                sp38 = *(s32 *)0x03001F08;
                ((BattleEffectDrawFn)sp38)(sp40, sp44 + (temp_r5_769 * 0x640) + 0x60E, M2C_FIELD(sp18, s32 **, 0) - 0x14, M2C_FIELD(sp18, s32 *, 4) - 0x14, 0x28, 0x28);
                Func_08002dd8(0x2E);
            }
            switch (sp48) {                         /* switch 6; irregular */
            case 0:                                 /* switch 6 */
                if (var_fp_344 < (s32) (sp10 + 0x50)) {

                } else if (var_fp_344 >= (s32) (sp10 + 0x6C)) {

                } else {
                    var_r3_817 = var_r9_478 - 0x50;
                    if (var_r3_817 < 0) {
                        var_r3_817 += 3;
                    }
                    temp_r5_826 = Func_080022fc(var_r3_817 >> 2, 7);
                    sp0 = (s32 *)2;
                    Func_080ed408(0x2E, 7, 7, sp2C, 2);
                    sp0 = (s32 *)0x18;
                    sp4 = (s32 *)0x28;
                    sp38 = *(s32 *)0x03001F08;
                    ((BattleEffectDrawFn)sp38)(sp40, sp44 + (temp_r5_826 * 0x3C0) + 0x2B8E, sp60 - 0xC, sp64 - 0x14, 0x18, 0x28);
block_112:
                    Func_08002dd8(0x2E);
                }
                break;
            case 3:                                 /* switch 6 */
            case 5:                                 /* switch 6 */
                if (var_fp_344 < (s32) (sp10 + 0x50)) {

                } else if (var_fp_344 >= (s32) (sp10 + 0x68)) {

                } else {
                    var_r3_874 = var_r9_478 - 0x50;
                    if (var_r3_874 < 0) {
                        var_r3_874 += 3;
                    }
                    temp_r0_881 = Func_080022fc(var_r3_874 >> 2, 6);
                    sp0 = (s32 *)2;
                    Func_080ed408(0x2E, 7, 7, sp2C, 2);
                    sp0 = (s32 *)0x20;
                    sp4 = (s32 *)0x40;
                    sp38 = *(s32 *)0x03001F08;
                    ((BattleEffectDrawFn)sp38)(sp40, sp44 + (temp_r0_881 << 0xB) + 0x2B8E, sp60 - 0x10, sp64 - 0x20, 0x20, 0x40);
                    goto block_112;
                }
                break;
            case 4:                                 /* switch 6 */
                if (var_fp_344 < (s32) (sp10 + 0x50)) {

                } else if (var_fp_344 >= (s32) (sp10 + 0x68)) {

                } else {
                    temp_r0_927 = var_r9_478 - 0x50;
                    temp_r0_932 = Func_080022fc((s32) (temp_r0_927 + (temp_r0_927 >> 0x1F)) >> 1, 6);
                    sp0 = (s32 *)2;
                    Func_080ed408(0x2E, 7, 7, sp2C, 2);
                    sp0 = (s32 *)2;
                    sp38 = *(s32 *)0x03001F08;
                    Func_080ed408(0x2F, 7, 7, 8 | sp2C, 2);
                    temp_r5_961 = sp44 + (temp_r0_932 << 0xB) + 0x2B8E;
                    sp3C = *(s32 *)0x03001F0C;
                    sp0 = (s32 *)0x40;
                    sp8 = 0x40;
                    sp4 = (s32 *)0x20;
                    ((BattleEffectDrawFn)sp38)(sp40, temp_r5_961, sp60 - 0x20, sp64 - 0x18,
                        0x40, 0x20);
                    sp0 = (s32 *)0x40;
                    sp4 = (s32 *)0x20;
                    ((BattleEffectDrawFn)sp3C)(sp40, temp_r5_961, sp60 - 0x20, sp64 + 8,
                        0x40, 0x20);
                    Func_08002dd8(0x2F);
                    goto block_112;
                }
                break;
            default:                                /* switch 6 */
                if ((var_fp_344 >= (s32) (sp10 + 0x50)) && (var_fp_344 < (s32) (sp10 + 0x68))) {
                    var_r3_1022 = var_r9_478 - 0x50;
                    if (var_r3_1022 < 0) {
                        var_r3_1022 += 3;
                    }
                    temp_r5_1031 = Func_080022fc(var_r3_1022 >> 2, 6);
                    sp0 = (s32 *)3;
                    Func_080ed408(0x2E, 7, 7, sp2C, 3);
                    sp0 = (s32 *)0x28;
                    sp4 = (s32 *)0x28;
                    sp38 = *(s32 *)0x03001F08;
                    ((BattleEffectDrawFn)sp38)(sp40, sp44 + (temp_r5_1031 * 0x640) + 0x2B8E, sp60 - 0x14, sp64 - 0x14, 0x28, 0x28);
                    Func_08002dd8(0x2E);
                }
                break;
            }
            temp_r1_1069 = sp34 + 1;
            sp10 += 8;
            spC += 2;
            sp34 = temp_r1_1069;
            var_r9_478 -= 8;
            if (temp_r1_1069 != M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14)) {
                goto loop_60;
            }
        }
        Func_080cd52c();
        M2C_FIELD(sp44, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        var_fp_344 += 1;
        if (var_fp_344 != ((M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14) * 8) + 0x6C)) {
            goto loop_49;
        }
    }
    Func_08004278(0x080CD261);
    Func_080cdbc0();
}
