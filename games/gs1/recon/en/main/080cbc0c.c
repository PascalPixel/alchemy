#include "shared-aggregates.h"

#define BattleEffect_RunScreenShatter Func_080cbc0c

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

void BattleEffect_RunScreenShatter(void *arg0) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    struct M2cAggregate_deref_absolute_03001e74_0 *sp14;
    u32 sp18;
    u32 sp1C;
    s32 sp20;
    struct M2cAggregate_absolute_02010000 *sp24;
    struct M2cAggregate_absolute_02010000 *sp28;
    s16 *var_r0_1012;
    s32 temp_lr_618;
    s32 temp_r1_518;
    s32 temp_r1_585;
    s32 temp_r1_688;
    s32 temp_r1_742;
    s32 temp_r1_835;
    s32 temp_r1_872;
    s32 temp_r1_895;
    s32 temp_r2_1182;
    s32 temp_r2_299;
    s32 temp_r2_800;
    s32 temp_r3_529;
    s32 temp_r3_652;
    s32 temp_r3_721;
    s32 temp_r3_797;
    s32 temp_r3_817;
    s32 temp_r3_912;
    s32 temp_r4_526;
    s32 temp_r4_586;
    s32 temp_r4_656;
    s32 temp_r5_671;
    s32 temp_r5_801;
    s32 temp_r5_934;
    s32 temp_r6_672;
    s32 temp_r7_655;
    s32 temp_r7_818;
    s32 temp_sl_762;
    s32 var_fp_603;
    s32 var_ip_132;
    s32 var_r0_153;
    s32 var_r0_516;
    s32 var_r0_613;
    s32 var_r0_701;
    s32 var_r0_757;
    s32 var_r0_847;
    s32 var_r1_1017;
    s32 var_r1_1139;
    s32 var_r1_155;
    s32 var_r1_681;
    s32 var_r1_735;
    s32 var_r1_828;
    s32 var_r2_515;
    s32 var_r2_617;
    s32 var_r2_703;
    s32 var_r2_761;
    s32 var_r2_851;
    s32 var_r3_1121;
    s32 var_r3_638;
    s32 var_r3_646;
    s32 var_r3_665;
    s32 var_r3_714;
    s32 var_r3_783;
    s32 var_r3_791;
    s32 var_r3_811;
    s32 var_r3_865;
    s32 var_r3_888;
    s32 var_r4_154;
    s32 var_r4_619;
    s32 var_r4_763;
    s32 var_r5_509;
    s32 var_r5_63;
    s32 var_r6_1041;
    s32 var_r6_1118;
    s32 var_r6_295;
    s32 var_r6_454;
    s32 var_r6_616;
    s32 var_r6_764;
    s32 var_r6_953;
    s32 var_r7_136;
    s32 var_r9_602;
    struct M2cAggregate_absolute_02010000 *var_r1_291;
    struct M2cAggregate_absolute_02010000 *var_r5_1072;
    struct M2cAggregate_absolute_02010000 *var_r5_1119;
    struct M2cAggregate_absolute_02010000 *var_r5_954;
    struct M2cAggregate_deref_absolute_03001e74_8c *temp_r9_36;
    u16 *var_lr_453;
    u16 *var_r9_457;
    u16 temp_r3_506;
    u16 temp_r3_514;
    u16 temp_r4_1228;
    u16 temp_r4_193;
    u16 temp_r4_345;
    u32 temp_r1_989;
    u32 temp_r3_1131;
    u32 temp_r3_300;
    u32 temp_r3_510;
    u32 temp_r3_519;
    u32 temp_r4_522;
    u32 temp_sl_521;
    u32 var_r0_992;
    u32 var_r1_1134;
    u32 var_r6_1013;
    u8 *var_r0_292;
    u8 *var_r4_290;
    void *temp_r3_116;
    void *temp_r3_1237;
    void *temp_r3_1240;
    void *temp_r3_202;
    void *temp_r3_205;
    void *temp_r3_354;
    void *temp_r3_357;
    void *var_r6_129;

    sp28 = Func_080048b0(0x27, 0x782C);
    sp24 = Func_080048b0(0x28, 0x4000);
    Func_080048b0(0x29, 0x302);
    sp14 = absolute_03001e74.field_0000;
    temp_r9_36 = absolute_03001e74.field_008c;
    M2C_FIELD(sp28, void **, 0x7828) = arg0;
    Func_080cd508();
    temp_r9_36->field_000c = 1;
    absolute_03001ad0.field_0006 = 0x20;
    Func_080b5038(1, M2C_FIELD(sp14, u16 *, 0x648), 0);
    absolute_0400000c.field_0000 = 0x784;
    Func_080b5028(0, 0, 0, 0x64);
    var_r5_63 = 0;
    temp_r9_36->field_000c = 0;
    absolute_04000028.field_0000 = 0;
    *(s32 *)0x0400002C = 0xFFFFF000;
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x80;
    absolute_04000022.field_0000 = 0;
    absolute_04000022.field_0002 = 0;
    M2C_FIELD((void *)0x04000020, s16 *, 6) = 0x100;
    absolute_04000040.field_0000 = 0xF0;
    absolute_04000040.field_0004 = 0x1088;
    temp_r3_116 = &absolute_04000040.field_0004 - 2;
    M2C_FIELD(temp_r3_116, s16 *, 0) = 0xF0;
    M2C_FIELD(temp_r3_116, s16 *, 4) = 0x1088;
    absolute_04000048.field_0000 = 0x3537;
    absolute_04000048.field_0002 = 0x3F21;
    var_r6_129 = NULL;
    var_ip_132 = 0;
    var_r7_136 = 0;
    do {
        var_r0_153 = var_r7_136 + 0x100;
        var_r4_154 = 0;
        var_r1_155 = var_r5_63 * 2;
loop_4:
        var_r4_154 += 1;
        *(s16 *)(0x06003800 + (s32) var_r6_129) = (s16) (var_r0_153 | var_r1_155);
        var_r0_153 += 0x200;
        var_r1_155 += 2;
        var_r6_129 += 2;
        if (var_r4_154 != 8) {
            goto loop_4;
        }
        var_ip_132 += 1;
        var_r7_136 += 0x1000;
        var_r5_63 += 8;
    } while (var_ip_132 != 0x10);
    Func_080072f8(sp24, 0x4000);
    Func_080072f8((struct M2cAggregate_absolute_02010000 *)0x06004000, 0x4000);
    temp_r4_193 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x04000208;
    if ((s32) absolute_02002090.field_0000 <= 0x1F) {
        temp_r3_202 = (absolute_02002090.field_0000 * 0xC) + &absolute_02002090;
        absolute_02002090.field_0000 += 1;
        temp_r3_205 = temp_r3_202 + 4;
        M2C_FIELD(temp_r3_202, s32 *, 4) = 0x7741;
        M2C_FIELD(temp_r3_205, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_205 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r4_193;
    *(s16 *)0x04000052 = 0x1010;
    *(s16 *)0x04000050 = 0;
    Func_080e0524(0x44, sp28, 1, 1);
    M2C_FIELD(sp28, s32 *, 0x7780) = 1;
    M2C_FIELD(sp28, s32 *, 0x7784) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080ed408(0x2E, 7, 7, 3, 1);
    sp18 = absolute_03001e50.field_00b8;
    Func_080ed408(0x2F, 7, 7, 3, 2);
    sp1C = absolute_03001e50.field_00bc;
    spC = 0;
    var_r4_290 = (u8 *)0x080EE037;
    var_r1_291 = sp28 + 0x7080;
    var_r0_292 = (u8 *)0x080EE016;
    var_r6_295 = 0;
    do {
        temp_r2_299 = *var_r0_292 << 0x10;
        temp_r3_300 = *var_r4_290 << 0x10;
        M2C_FIELD(var_r1_291, s32 *, 0) = temp_r2_299;
        var_r1_291->field_0004 = temp_r3_300;
        var_r6_295 += 1;
        var_r1_291->field_000c = (u32) ((s32) (temp_r2_299 + 0xFFE00000) >> 2);
        var_r1_291->field_0010 = (u32) ((s32) (temp_r3_300 + 0xFFC40000) >> 2);
        var_r0_292 += 1;
        var_r4_290 += 1;
        var_r1_291 += 0x1C;
    } while (var_r6_295 != 0x21);
    Func_080072f0(&absolute_02010000, (struct M2cAggregate_absolute_02010000 *)0x06008000, 0x7800, 0x03001388);
    Func_080072f8(&absolute_02010000, 0x7800, 0x01010101);
    temp_r9_36->field_0010 = 1;
    M2C_FIELD(sp28, s32 *, 0x77A0) = (s32) absolute_03001ad0.field_0004;
    M2C_FIELD(sp28, s32 *, 0x77A4) = (s32) absolute_03001ad0.field_0006;
    absolute_03001ad0.field_0004 = 0;
    temp_r4_345 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0;
    if ((s32) absolute_02002090.field_0000 <= 0x1F) {
        temp_r3_354 = (absolute_02002090.field_0000 * 0xC) + &absolute_02002090;
        absolute_02002090.field_0000 += 1;
        temp_r3_357 = temp_r3_354 + 4;
        M2C_FIELD(temp_r3_354, s32 *, 4) = 0x1F81;
        M2C_FIELD(temp_r3_357, struct M2cAggregate_absolute_0400000a **, 4) = &absolute_0400000a;
        M2C_FIELD((temp_r3_357 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r4_345;
    Func_080072f8((struct M2cAggregate_absolute_02010000 *)0x050000C0, 0x100, 0x7FFF7FFF);
    Func_080f9010(0xD4);
    Func_080d6888(M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s16 *, 0x24), 7, 3, 0, 0x1E);
    sp20 = 0;
loop_14:
    if (sp20 == 2) {
        Func_080f9010(0xD4);
    }
    if (sp20 == 3) {
        Func_080f9010(0xD4);
    }
    if (sp20 == 0x1C) {
        Func_080d6888(M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s16 *, 0x24), -1, 3, 0);
    }
    if (sp20 == 0x20) {
        Func_080f9010(0x95);
    }
    if (sp20 == 5) {
        Func_080f9010(0x91);
        absolute_03001ad0.field_0004 = (u16) M2C_FIELD(sp28, s32 *, 0x77A0);
        Func_080b5040(1, M2C_FIELD(sp14, u16 *, 0x648), -1);
    }
    if (sp20 > 7) {
        var_lr_453 = (u16 *)0x050000C0;
        var_r6_454 = 0;
        var_r9_457 = (u16 *)((u8 *)sp14 + 0x544);
        do {
            temp_r3_506 = *var_lr_453;
            var_r5_509 = 0x1F & temp_r3_506;
            temp_r3_510 = temp_r3_506 << 0x10;
            temp_r3_514 = *var_r9_457;
            var_r2_515 = (temp_r3_510 >> 0x15) & 0x1F;
            var_r0_516 = (temp_r3_510 >> 0x1A) & 0x1F;
            temp_r1_518 = 0x1F & temp_r3_514;
            temp_r3_519 = temp_r3_514 << 0x10;
            temp_sl_521 = temp_r3_519;
            temp_r4_522 = temp_r3_519 >> 0x15;
            var_r9_457 += 2;
            temp_r4_526 = temp_r4_522 & 0x1F;
            temp_r3_529 = (temp_sl_521 >> 0x1A) & 0x1F;
            if (var_r5_509 < temp_r1_518) {
                var_r5_509 += 1;
            } else if (var_r5_509 > temp_r1_518) {
                var_r5_509 -= 1;
            }
            if (var_r2_515 < temp_r4_526) {
                var_r2_515 += 1;
            } else if (var_r2_515 > temp_r4_526) {
                var_r2_515 -= 1;
            }
            if (var_r0_516 < temp_r3_529) {
                var_r0_516 += 1;
            } else if (var_r0_516 > temp_r3_529) {
                var_r0_516 -= 1;
            }
            var_r6_454 += 1;
            *var_lr_453 = (var_r0_516 << 0xA) | (var_r2_515 << 5) | var_r5_509;
            var_lr_453 += 2;
        } while (var_r6_454 != 0x80);
    }
    if (sp20 == 4) {
        Func_080072f0((struct M2cAggregate_absolute_02010000 *)0x06008000, (struct M2cAggregate_absolute_02010000 *)0x7800, 0x02020202, 0x03000168);
    }
    if (sp20 > 3) {

    } else {
        temp_r1_585 = (sp20 * 4) + 8;
        temp_r4_586 = sp20 << 5;
        sp10 = temp_r4_586;
        *(s16 *)0x05000004 = (temp_r1_585 << 0xA) | (temp_r1_585 << 5) | temp_r1_585;
        if (spC == temp_r4_586) {

        } else {
loop_46:
            var_r9_602 = spC;
            var_fp_603 = 0;
            sp8 = spC;
            if (spC < 0) {

            } else {
loop_48:
                var_r0_613 = 0x60 - var_r9_602;
                var_r6_616 = 0x3C - var_fp_603;
                var_r2_617 = var_r9_602 + 0x60;
                temp_lr_618 = var_r0_613;
                var_r4_619 = var_fp_603 + 0x3C;
                if (var_r6_616 < 0) {
                    var_r6_616 = 0;
                }
                if (var_r4_619 > 0x77) {
                    var_r4_619 = 0x77;
                }
                if (var_r0_613 < 0) {
                    var_r0_613 = 0;
                }
                if (var_r2_617 > 0xFF) {
                    var_r2_617 = 0xFF;
                }
                var_r3_638 = var_r2_617;
                if (var_r2_617 < 0) {
                    var_r3_638 = var_r2_617 + 7;
                }
                var_r3_646 = var_r4_619;
                if (var_r4_619 < 0) {
                    var_r3_646 = var_r4_619 + 7;
                }
                temp_r3_652 = (var_r3_646 >> 3) << 0xB;
                temp_r7_655 = (7 & var_r4_619) * 8;
                temp_r4_656 = ((var_r3_638 >> 3) << 6) + (7 & var_r2_617);
                absolute_02010000.unknown_0000[temp_r7_655 + temp_r4_656 + temp_r3_652] = 2;
                var_r3_665 = var_r6_616;
                if (var_r6_616 < 0) {
                    var_r3_665 = var_r6_616 + 7;
                }
                temp_r5_671 = (7 & var_r6_616) * 8;
                temp_r6_672 = (var_r3_665 >> 3) << 0xB;
                absolute_02010000.unknown_0000[temp_r6_672 + (temp_r5_671 + temp_r4_656)] = 2;
                var_r1_681 = var_r0_613;
                if (var_r0_613 < 0) {
                    var_r1_681 = var_r0_613 + 7;
                }
                temp_r1_688 = ((var_r1_681 >> 3) << 6) + (7 & var_r0_613);
                absolute_02010000.unknown_0000[temp_r7_655 + temp_r1_688 + temp_r3_652] = 2;
                var_r0_701 = temp_lr_618 + 1;
                absolute_02010000.unknown_0000[temp_r6_672 + (temp_r5_671 + temp_r1_688)] = 2;
                var_r2_703 = var_r9_602 + 0x61;
                if (var_r0_701 < 0) {
                    var_r0_701 = 0;
                }
                if (var_r2_703 > 0xFF) {
                    var_r2_703 = 0xFF;
                }
                var_r3_714 = var_r2_703;
                if (var_r2_703 < 0) {
                    var_r3_714 = var_r2_703 + 7;
                }
                temp_r3_721 = ((var_r3_714 >> 3) << 6) + (7 & var_r2_703);
                absolute_02010000.unknown_0000[temp_r7_655 + temp_r3_721 + temp_r3_652] = 2;
                absolute_02010000.unknown_0000[temp_r6_672 + (temp_r5_671 + temp_r3_721)] = 2;
                var_r1_735 = var_r0_701;
                if (var_r0_701 < 0) {
                    var_r1_735 = var_r0_701 + 7;
                }
                temp_r1_742 = ((var_r1_735 >> 3) << 6) + (7 & var_r0_701);
                absolute_02010000.unknown_0000[temp_r7_655 + temp_r1_742 + temp_r3_652] = 2;
                absolute_02010000.unknown_0000[temp_r6_672 + (temp_r5_671 + temp_r1_742)] = 2;
                var_r0_757 = 0x60 - var_fp_603;
                var_r2_761 = var_fp_603 + 0x60;
                temp_sl_762 = var_r0_757;
                var_r4_763 = var_r9_602 + 0x3C;
                var_r6_764 = 0x3C - var_r9_602;
                if (var_r0_757 < 0) {
                    var_r0_757 = 0;
                }
                if (var_r2_761 > 0xFF) {
                    var_r2_761 = 0xFF;
                }
                if (var_r6_764 < 0) {
                    var_r6_764 = 0;
                }
                if (var_r4_763 > 0x77) {
                    var_r4_763 = 0x77;
                }
                var_r3_783 = var_r2_761;
                if (var_r2_761 < 0) {
                    var_r3_783 = var_r2_761 + 7;
                }
                var_r3_791 = var_r4_763;
                if (var_r4_763 < 0) {
                    var_r3_791 = var_r4_763 + 7;
                }
                temp_r3_797 = (var_r3_791 >> 3) << 0xB;
                temp_r2_800 = (7 & var_r4_763) * 8;
                temp_r5_801 = ((var_r3_783 >> 3) << 6) + (7 & var_r2_761);
                absolute_02010000.unknown_0000[temp_r2_800 + temp_r5_801 + temp_r3_797] = 2;
                var_r3_811 = var_r6_764;
                if (var_r6_764 < 0) {
                    var_r3_811 = var_r6_764 + 7;
                }
                temp_r3_817 = (var_r3_811 >> 3) << 0xB;
                temp_r7_818 = (7 & var_r6_764) * 8;
                absolute_02010000.unknown_0000[temp_r7_818 + temp_r5_801 + temp_r3_817] = 2;
                var_r1_828 = var_r0_757;
                if (var_r0_757 < 0) {
                    var_r1_828 = var_r0_757 + 7;
                }
                temp_r1_835 = ((var_r1_828 >> 3) << 6) + (7 & var_r0_757);
                absolute_02010000.unknown_0000[temp_r2_800 + temp_r1_835 + temp_r3_797] = 2;
                var_r0_847 = temp_sl_762 + 1;
                absolute_02010000.unknown_0000[temp_r7_818 + temp_r1_835 + temp_r3_817] = 2;
                var_r2_851 = var_fp_603 + 0x61;
                if (var_r0_847 < 0) {
                    var_r0_847 = 0;
                }
                if (var_r2_851 > 0xFF) {
                    var_r2_851 = 0xFF;
                }
                var_r3_865 = var_r2_851;
                if (var_r2_851 < 0) {
                    var_r3_865 = var_r2_851 + 7;
                }
                temp_r1_872 = ((var_r3_865 >> 3) << 6) + (var_r2_851 & 7);
                absolute_02010000.unknown_0000[temp_r2_800 + temp_r1_872 + temp_r3_797] = 2;
                absolute_02010000.unknown_0000[temp_r7_818 + temp_r1_872 + temp_r3_817] = 2;
                var_r3_888 = var_r0_847;
                if (var_r0_847 < 0) {
                    var_r3_888 = var_r0_847 + 7;
                }
                temp_r1_895 = ((var_r3_888 >> 3) << 6) + (var_r0_847 & 7);
                absolute_02010000.unknown_0000[temp_r2_800 + temp_r1_895 + temp_r3_797] = 2;
                absolute_02010000.unknown_0000[temp_r7_818 + temp_r1_895 + temp_r3_817] = 2;
                temp_r3_912 = (sp8 - (var_fp_603 * 2)) - 1;
                sp8 = temp_r3_912;
                if (temp_r3_912 < 0) {
                    sp8 = (sp8 + (var_r9_602 * 2)) - 2;
                    var_r9_602 -= 1;
                }
                var_fp_603 += 1;
                if (var_r9_602 >= var_fp_603) {
                    goto loop_48;
                }
            }
            temp_r5_934 = spC + 1;
            spC = temp_r5_934;
            if (temp_r5_934 != sp10) {
                goto loop_46;
            }
        }
        Func_080072f0((struct M2cAggregate_absolute_02010000 *)0x06008000, &absolute_02010000, 0x7800, 0x03001388);
    }
    if (sp20 <= 0x32) {
        var_r6_953 = 0;
        var_r5_954 = sp28 + 0x7080;
        do {
            Func_080072f4(sp24, &sp28->unknown_0000[*(u16 *)(0x080EDFD2 + (var_r6_953 * 2))], M2C_FIELD(var_r5_954, s16 *, 2), M2C_FIELD(var_r5_954, s16 *, 6), (s32) *(u8 *)(0x080EDF90 + var_r6_953), (s32) *(u8 *)(0x080EDFB1 + var_r6_953));
            if (sp20 > 3) {
                Func_080e3908(var_r5_954, 0x40, 0x4000);
            }
            var_r6_953 += 1;
            var_r5_954 += 0x1C;
        } while (var_r6_953 != 0x21);
    }
    temp_r1_989 = sp20 - 8;
    if (temp_r1_989 <= 0x2AU) {
        var_r0_992 = temp_r1_989;
        if ((s32) var_r0_992 > 0x1F) {
            var_r0_992 = 0x1F;
        }
        *(s16 *)0x05000002 = (var_r0_992 << 0xA) | (var_r0_992 << 5) | var_r0_992;
    }
    if (sp20 == 0x33) {
        Func_080e0524(0x7D, sp28, 1, 0);
        var_r0_1012 = (s16 *)0x05000002;
        var_r6_1013 = 1;
        do {
            var_r1_1017 = (s32) (var_r6_1013 + (var_r6_1013 >> 0x1F)) >> 1;
            if (var_r1_1017 < 0) {
                var_r1_1017 = 0;
            }
            var_r6_1013 += 1;
            *var_r0_1012 = (var_r1_1017 << 0xA) | (((s32) (var_r1_1017 + ((u32) var_r1_1017 >> 0x1F)) >> 1) << 5) | var_r1_1017;
            var_r0_1012 += 2;
        } while (var_r6_1013 != 0x40);
        absolute_04000050.field_0000 = 0x3F44;
        var_r6_1041 = 0;
        var_r5_1072 = sp28 + 0x7080;
        do {
            M2C_FIELD(var_r5_1072, s32 *, 0) = (s32) (((Func_08004458() & 0x1F) + 0x20) << 0x10);
            var_r5_1072->field_0004 = ((Func_08004458() & 0x1F) + 0x50) << 0x10;
            var_r6_1041 += 1;
            var_r5_1072->field_000c = ((0x1FF & Func_08004458()) + 0xFFFFFF00) << 0xC;
            var_r5_1072->field_0010 = 0;
            var_r5_1072->field_0018 = 0;
            var_r5_1072 += 0x1C;
        } while (var_r6_1041 != 0x20);
        M2C_FIELD(sp28, s32 *, 0x7780) = 2;
        M2C_FIELD(sp28, s32 *, 0x7784) = 0x32;
    }
    if (sp20 > 0x34) {
        var_r6_1118 = 0;
        var_r5_1119 = sp28 + 0x7080;
        do {
            var_r3_1121 = var_r6_1118;
            if (var_r6_1118 < 0) {
                var_r3_1121 = var_r6_1118 + 3;
            }
            if (sp20 >= (s32) ((var_r3_1121 >> 2) + 0x34)) {
                temp_r3_1131 = var_r5_1119->field_0018;
                if ((s32) temp_r3_1131 <= 0x27) {
                    var_r1_1134 = temp_r3_1131;
                    if ((s32) var_r1_1134 < 0) {
                        var_r1_1134 += 3;
                    }
                    var_r1_1139 = (s32) var_r1_1134 >> 2;
                    if (var_r1_1139 > 5) {
                        var_r1_1139 = 5;
                    }
                    Func_08007300(sp24, sp28 + (var_r1_1139 << 0xB), M2C_FIELD(var_r5_1119, s16 *, 2) - 0x10, M2C_FIELD(var_r5_1119, s16 *, 6) - 0x20, 0x20, 0x40);
                    Func_080e3908(var_r5_1119, 0x3C, 0xFFFFF000);
                    var_r5_1119->field_0018 += 1;
                }
            }
            var_r6_1118 += 1;
            var_r5_1119 += 0x1C;
        } while (var_r6_1118 != 0x20);
    }
    Func_080cd52c();
    M2C_FIELD(sp28, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    temp_r2_1182 = sp20 + 1;
    sp20 = temp_r2_1182;
    if (temp_r2_1182 != 0x80) {
        goto loop_14;
    }
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_08004278(0x080CD261);
    Func_080d6888(M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s16 *, 0x24), -1, 1, 0);
    absolute_03001ad0.field_0004 = (u16) M2C_FIELD(sp28, s32 *, 0x77A0);
    absolute_03001ad0.field_0006 = 0x20;
    Func_080b5038(2, M2C_FIELD(sp14, u16 *, 0x648), 0);
    Func_080030f8(1);
    temp_r4_1228 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0;
    if ((s32) absolute_02002090.field_0000 <= 0x1F) {
        temp_r3_1237 = (absolute_02002090.field_0000 * 0xC) + &absolute_02002090;
        absolute_02002090.field_0000 += 1;
        temp_r3_1240 = temp_r3_1237 + 4;
        M2C_FIELD(temp_r3_1237, s32 *, 4) = 0x7541;
        M2C_FIELD(temp_r3_1240, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_1240 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r4_1228;
    Func_08002dd8(0x29);
    Func_08002dd8(0x28);
    Func_08002dd8(0x27);
}
