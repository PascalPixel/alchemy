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

void Func_080e823c(s32 arg0) {
    u32 sp0;
    s32 sp4;
    u32 *sp8;
    u32 *spC;
    u32 *sp10;
    u32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    u32 sp24;
    s32 sp28;
    void *sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    u32 sp3C;
    u32 sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 *var_r2_339;
    s32 *var_r3_254;
    s32 *var_r3_267;
    s32 *var_r5_930;
    s32 *var_r6_362;
    s32 temp_r0_381;
    s32 temp_r0_710;
    s32 temp_r0_719;
    s32 temp_r0_934;
    s32 temp_r2_578;
    s32 temp_r3_468;
    s32 temp_r3_596;
    s32 temp_r5_806;
    s32 temp_r5_819;
    s32 temp_r5_856;
    s32 temp_r7_910;
    s32 var_r6_414;
    s32 var_r6_71;
    s32 var_r6_767;
    s32 var_r7_222;
    s32 var_r7_435;
    s32 var_r7_608;
    s32 var_r8_228;
    s32 var_r8_252;
    s32 var_r8_264;
    s32 var_r8_413;
    s32 var_r8_550;
    s32 var_r8_707;
    s32 var_r8_72;
    s32 var_r8_758;
    s32 var_r8_802;
    s32 var_r8_848;
    s32 var_r8_929;
    struct M2cAggregate_absolute_02010000 *var_r5_708;
    struct M2cAggregate_absolute_02010000 *var_r6_847;
    struct M2cAggregate_absolute_02010000 *var_r7_801;
    u16 temp_r4_723;
    u16 temp_r5_392;
    u16 temp_r6_810;
    u32 *temp_r2_128;
    u32 temp_r0_729;
    u32 temp_r0_851;
    u32 temp_r0_855;
    u32 temp_r3_601;
    u32 temp_r3_612;
    void *temp_r0_77;
    void *temp_r1_94;
    void *temp_r2_135;
    void *temp_r2_185;
    void *temp_r2_20;
    void *var_r5_230;
    void *var_r5_437;
    void *var_r5_552;
    void *var_r6_610;
    void *var_r8_361;
    void *var_r9_553;
    void *var_sl_415;

    sp30 = M2C_FIELD(&absolute_03001ef0, s32 *, 0);
    temp_r2_20 = *(void **)((u8 *)&absolute_03001ef0 - 4);
    sp2C = temp_r2_20;
    sp24 = absolute_03001ef0.field_0004;
    M2C_FIELD(temp_r2_20, s32 *, 0x7828) = arg0;
    Func_080cd594(0);
    Func_080c9048();
    M2C_FIELD((void *)0x05000000, s16 *, 0) = 0;
    M2C_FIELD((void *)0x05000000, s16 *, 2) = 0;
    M2C_FIELD(sp2C, s32 *, 0x7780) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080cd104(1, 0);
    Func_080d6750(M2C_FIELD(temp_r2_20, s32 *, 0x7828));
    Func_080dbb24(9, 0x17B, 2);
    var_r6_71 = 0x77FC;
    var_r8_72 = 0;
    do {
        temp_r0_77 = Func_08009030(0x186);
        *(void **)((u8 *)sp2C + var_r6_71) = temp_r0_77;
        if (temp_r0_77 != NULL) {
            M2C_FIELD(temp_r0_77, s8 *, 0x26) = 0;
            Func_08009020(temp_r0_77, Func_080022fc(var_r8_72, 3));
            temp_r1_94 = *(void **)((u8 *)sp2C + var_r6_71);
            M2C_FIELD(temp_r1_94, u8 *, 9) = (u8) ((M2C_FIELD(temp_r1_94, u8 *, 9) & ~0xC) | 4);
        }
        var_r8_72 += 1;
        var_r6_71 += 4;
    } while (var_r8_72 != 6);
    sp0 = 2;
    Func_080ed408(0x2E, 7, 7, 3);
    sp3C = absolute_03001e50.field_00b8;
    sp0 = 3;
    Func_080ed408(0x2F, 7, 7);
    temp_r2_128 = &sp0 + 0x3C;
    sp10 = temp_r2_128;
    M2C_FIELD(temp_r2_128, u32 *, 4) = (u32) absolute_03001e50.field_00bc;
    absolute_04000048.field_0000 = 0x2737;
    temp_r2_135 = &absolute_04000048 - 8;
    M2C_FIELD(temp_r2_135, s16 *, 0) = 0xF0;
    M2C_FIELD(temp_r2_135, s16 *, 6) = 0x1088;
    Func_080030f8(1);
    Func_080b5040(1, 0x3C, 0);
    Func_080cd104(1, 1);
    Func_080e0524(0x73, sp24, 0, 0);
    Func_080e0524(0xC0, (u32) sp2C, 1, 1);
    M2C_FIELD((void *)0x04000000, s16 *, 0) = 0x7741;
    temp_r2_185 = (void *)0x04000000 + 0x20;
    M2C_FIELD((void *)0x04000000, s16 *, 0x20) = 0x80;
    M2C_FIELD(temp_r2_185, s16 *, 0x32) = 0x1010;
    *(u16 *)((u8 *)temp_r2_185 + 0x30) = 0x3F44;
    M2C_FIELD(sp2C, s32 *, 0x7780) = 2;
    M2C_FIELD(sp2C, s32 *, 0x7784) = 0x32;
    var_r7_222 = 0;
    sp1C = 0xBC0000;
    sp20 = 0x5C0000;
    sp18 = 0x5C0000;
    sp14 = 0xA00000;
    var_r8_228 = 0;
    var_r5_230 = sp2C + 0x7080;
    do {
        M2C_FIELD(var_r5_230, s32 *, 0) = (s32) ((0x7F & Func_08004458()) << 0x10);
        var_r8_228 += 1;
        M2C_FIELD(var_r5_230, s32 *, 4) = var_r7_222;
        M2C_FIELD(var_r5_230, s32 *, 0xC) = 0;
        M2C_FIELD(var_r5_230, s32 *, 0x10) = 0;
        M2C_FIELD(var_r5_230, s32 *, 0x18) = 0;
        var_r7_222 += 0xFFF00000;
        var_r5_230 += 0x1C;
    } while (var_r8_228 != 6);
    var_r8_252 = 0;
    var_r3_254 = sp2C + 0x7140;
    do {
        var_r8_252 += 1;
        *var_r3_254 = 0x18;
        var_r3_254 += 0x1C;
    } while (var_r8_252 != 0x3A);
    var_r8_264 = 0;
    var_r3_267 = (s32 *)0x02010018;
    do {
        var_r8_264 += 1;
        *var_r3_267 = -1;
        var_r3_267 += 0x1C;
    } while (var_r8_264 != 0x400);
    M2C_FIELD(sp2C, s32 *, 0x77B4) = 0x18;
    M2C_FIELD(sp2C, s32 *, 0x77B8) = 0;
    sp28 = 0;
    if (*(s32 *)0x03001B04 & 3) {

    } else {
        sp8 = &sp0 + 0x44;
        spC = &sp0 + 0x34;
loop_18:
        if (sp28 == 0x5E) {
            Func_080f9010(0x9C);
        }
        if (sp28 == 0x88) {
            Func_080f9010(0x9C);
        }
        if (sp28 == 0xB2) {
            Func_080f9010(0x9C);
        }
        if (sp28 == 0x104) {
            Func_080f9010(0x91);
        }
        sp34 = M2C_FIELD((void *)0x080EDAC8, s32 *, 0);
        sp38 = M2C_FIELD((void *)0x080EDAC8, s32 *, 4);
        if ((u32) (sp28 - 0x60) <= 0x9BU) {
            var_r2_339 = sp2C + 0x77A8;
            goto block_30;
        }
        if ((u32) (sp28 + 0xFFFFFEFC) <= 3U) {
            var_r2_339 = sp2C + 0x77A8;
block_30:
            *var_r2_339 = 1;
        }
        sp50 = 0;
        sp48 = 0;
        var_r8_361 = NULL;
        var_r6_362 = sp2C + 0x77D8;
        do {
            M2C_FIELD(sp8, u32 *, 0) = (*(u8 *)(0x080EEED8 + (s32) var_r8_361) << 0x10) + sp1C + 0xFFE00000;
            M2C_FIELD(sp8, s32 *, 8) = (s32) ((*(u8 *)(0x080EEEE1 + (s32) var_r8_361) << 0x10) + sp20 + 0xFFE00000);
            temp_r0_381 = *var_r6_362;
            var_r6_362 += 4;
            Func_08009008(temp_r0_381, sp8, spC, 0);
            var_r8_361 += 1;
        } while (var_r8_361 != (void *)7);
        if (sp28 <= 0x5A) {
            temp_r5_392 = sp28 << 9;
            sp14 = (Func_08002322(temp_r5_392) * 0x10) + 0x9C0000;
            sp18 = (Func_0800231c(temp_r5_392) * 0x10) + 0x5C0000;
        }
        if (sp28 <= 0xC4) {
            var_r8_413 = 0;
            var_r6_414 = 0x5B;
            var_sl_415 = sp2C;
            do {
                if ((sp28 >= var_r6_414) && (sp28 < (s32) (var_r6_414 + 4))) {
                    sp18 += 0x80000;
                }
                if (sp28 == (var_r6_414 + 3)) {
                    var_r7_435 = 0;
                    var_r5_437 = var_sl_415 + 0x7128;
                    do {
                        M2C_FIELD(var_r5_437, s32 *, 0) = 0x400000;
                        M2C_FIELD(var_r5_437, s32 *, 4) = 0x600000;
                        M2C_FIELD(var_r5_437, s32 *, 0xC) = (s32) ((Func_08004458() - 0x7F) << 0xA);
                        M2C_FIELD(var_r5_437, s32 *, 0x10) = (s32) ((Func_08004458() - 0x7F) << 0xA);
                        var_r7_435 += 1;
                        M2C_FIELD(var_r5_437, s32 *, 0x18) = (s32) (0xF & Func_08004458());
                        var_r5_437 += 0x1C;
                    } while (var_r7_435 != 4);
                }
                temp_r3_468 = var_r6_414 + 0x14;
                if ((sp28 >= temp_r3_468) && (sp28 < (s32) (temp_r3_468 + 0x10))) {
                    sp18 += 0xFFFE0000;
                }
                var_r8_413 += 1;
                var_r6_414 += 0x28;
                var_sl_415 += 0xE0;
            } while (var_r8_413 != 3);
        }
        if ((u32) (sp28 - 0xF4) <= 7U) {
            sp14 += 0xFFFF0000;
        }
        if ((u32) (sp28 - 0xFC) <= 0x17U) {
            sp14 -= (sp28 - 0xFA) << 0x10;
        }
        if (sp28 <= 0x103) {
            sp48 = 0xFF000000;
            sp4C = sp18 + 0xFF000000;
            sp44 = sp14;
            Func_08009008(M2C_FIELD(sp2C, s32 *, 0x77F4), &sp44, spC, 0);
            sp44 = sp14 + 0x200000;
            Func_08009008(M2C_FIELD(sp2C, s32 *, 0x77F8), &sp44, spC, 0);
        }
        M2C_FIELD(sp8, s32 *, 4) = 0;
        var_r8_550 = 0;
        var_r5_552 = sp2C + 0x7080;
        var_r9_553 = sp2C;
loop_54:
        if (M2C_FIELD(var_r5_552, s32 *, 0x18) == 2) {

        } else {
            M2C_FIELD(sp8, u32 *, 0) = M2C_FIELD(var_r5_552, u32 *, 0);
            M2C_FIELD(sp8, s32 *, 8) = (s32) M2C_FIELD(var_r5_552, s32 *, 4);
            Func_08009008(*(s32 *)((u8 *)sp2C + (var_r8_550 * 4) + 0x77FC), sp8, spC, 0);
            M2C_FIELD(var_r5_552, u32 *, 0) = (u32) (M2C_FIELD(var_r5_552, u32 *, 0) + M2C_FIELD(var_r5_552, s32 *, 0xC));
            temp_r2_578 = M2C_FIELD(var_r5_552, s32 *, 0x10);
            M2C_FIELD(var_r5_552, s32 *, 4) = (s32) (M2C_FIELD(var_r5_552, s32 *, 4) + temp_r2_578);
            if (sp28 > 0x60) {
                M2C_FIELD(var_r5_552, s32 *, 0x10) = (s32) (temp_r2_578 + 0x4000);
            }
            if ((s32) M2C_FIELD(var_r5_552, s32 *, 4) > 0x780000) {
                temp_r3_596 = M2C_FIELD(var_r5_552, s32 *, 0x18) + 1;
                M2C_FIELD(var_r5_552, s32 *, 0x18) = temp_r3_596;
                if (temp_r3_596 == 1) {
                    temp_r3_601 = 0 - M2C_FIELD(var_r5_552, s32 *, 0x10);
                    M2C_FIELD(var_r5_552, s32 *, 0x10) = (s32) ((s32) (temp_r3_601 + (temp_r3_601 >> 0x1F)) >> 1);
                    var_r7_608 = 0;
                    var_r6_610 = var_r9_553 + 0x73C8;
                    do {
                        temp_r3_612 = M2C_FIELD(var_r5_552, u32 *, 0);
                        M2C_FIELD(var_r6_610, s32 *, 0) = (s32) ((s32) (temp_r3_612 + (temp_r3_612 >> 0x1F)) >> 1);
                        M2C_FIELD(var_r6_610, s32 *, 4) = (s32) (M2C_FIELD(var_r5_552, s32 *, 4) + 0xFFE00000);
                        M2C_FIELD(var_r6_610, s32 *, 0xC) = (s32) ((Func_08004458() - 0x7F) << 0xA);
                        M2C_FIELD(var_r6_610, s32 *, 0x10) = (s32) ((Func_08004458() - 0x7F) << 0xA);
                        var_r7_608 += 1;
                        M2C_FIELD(var_r6_610, s32 *, 0x18) = (s32) (0xF & Func_08004458());
                        var_r6_610 += 0x1C;
                    } while (var_r7_608 != 2);
                } else if (sp28 <= 0xC7) {
                    M2C_FIELD(var_r5_552, s32 *, 4) = 0;
                    M2C_FIELD(var_r5_552, s32 *, 0x10) = 0;
                    M2C_FIELD(var_r5_552, s32 *, 0x18) = 0;
                }
            }
        }
        var_r8_550 += 1;
        var_r5_552 += 0x1C;
        var_r9_553 += 0x38;
        if (var_r8_550 != 6) {
            goto loop_54;
        }
        var_r8_707 = 0;
        var_r5_708 = sp2C + 0x7128;
        do {
            temp_r0_710 = var_r5_708->field_0018;
            if (temp_r0_710 >= 0) {
                if ((u32) temp_r0_710 <= 0x17U) {
                    temp_r0_719 = (Func_080022ec(temp_r0_710, 6) + 3) * 2;
                    temp_r4_723 = *(u16 *)(0x080EEEF8 + temp_r0_719);
                    temp_r0_729 = temp_r4_723 >> 1;
                    sp0 = (u32) temp_r4_723;
                    sp4 = (s32) temp_r4_723;
                    Func_080072f4(sp30, (u8 *)sp2C + *(u16 *)(0x080EEEEA + temp_r0_719), M2C_FIELD(var_r5_708, s16 *, 2) - temp_r0_729, M2C_FIELD(var_r5_708, s16 *, 6) - temp_r0_729);
                }
                Func_080e3908(var_r5_708, 0x3C, 0xFFFFC000);
                var_r5_708->field_0018 += 1;
            }
            var_r8_707 += 1;
            var_r5_708 += 0x1C;
        } while (var_r8_707 != 0x38);
        if (sp28 == 0x104) {
            var_r8_758 = 0;
            if (M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x14) != 0) {
                var_r6_767 = 0x24;
                do {
                    Func_080b5088(*(s16 *)((u8 *)M2C_FIELD(sp2C, void **, 0x7828) + var_r6_767), 4);
                    sp0 = 8;
                    Func_080d6888(*(s16 *)((u8 *)M2C_FIELD(sp2C, void **, 0x7828) + var_r6_767), 7, -1, var_r8_758);
                    var_r8_758 += 1;
                    var_r6_767 += 2;
                } while (var_r8_758 != M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x14));
            }
            M2C_FIELD(sp2C, s32 *, 0x77A8) = 8;
            if (sp28 == 0x104) {
                var_r7_801 = &absolute_02010000;
                var_r8_802 = 0;
                do {
                    temp_r5_806 = 0x3FF & Func_08004458();
                    temp_r6_810 = (u16) Func_08004458();
                    M2C_FIELD(var_r7_801, s32 *, 0) = 0x200000;
                    var_r7_801->field_0004 = 0x5C0000;
                    temp_r5_819 = temp_r5_806 + 0x20;
                    var_r7_801->field_000c = (u32) ((s32) (temp_r5_819 * Func_08002322(temp_r6_810)) >> 7);
                    var_r7_801->field_0010 = (u32) ((s32) (0 - (temp_r5_819 * Func_0800231c(temp_r6_810) * 2)) >> 7);
                    var_r7_801->field_0018 = (0xF & Func_08004458()) + 0x20;
                    var_r8_802 += 1;
                    var_r7_801 += 0x1C;
                } while (var_r8_802 != 0x200);
            }
        }
        var_r6_847 = &absolute_02010000;
        var_r8_848 = 0;
        do {
            temp_r0_851 = var_r6_847->field_0018;
            if ((s32) temp_r0_851 >= 0) {
                temp_r0_855 = ((s32) temp_r0_851 >> 3) + 1;
                temp_r5_856 = temp_r0_855 * 2;
                sp0 = temp_r0_855;
                sp4 = temp_r5_856;
                Func_080072f4(sp30, sp24 + *(u16 *)(0x080EDE48 + (s32) (temp_r5_856 - 2)), M2C_FIELD(var_r6_847, s16 *, 2) - ((s32) (temp_r0_855 + (temp_r0_855 >> 0x1F)) >> 1), M2C_FIELD(var_r6_847, s16 *, 6) - temp_r0_855);
                Func_080e3908(var_r6_847, 0x3E, 0x1000);
                var_r6_847->field_0018 -= 1;
            }
            var_r8_848 += 1;
            var_r6_847 += 0x1C;
        } while (var_r8_848 != 0x200);
        Func_080e155c(8, 8);
        Func_080cd52c();
        M2C_FIELD(sp2C, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        temp_r7_910 = sp28 + 1;
        sp28 = temp_r7_910;
        if ((temp_r7_910 != 0x140) && !(*(u32 *)0x03001B04 & 3)) {
            goto loop_18;
        }
    }
    Func_080b50e8(0x86);
    Func_080d67dc();
    var_r8_929 = 0;
    var_r5_930 = sp2C + 0x77D8;
    do {
        var_r8_929 += 1;
        temp_r0_934 = *var_r5_930;
        var_r5_930 += 4;
        Func_08009038(temp_r0_934);
    } while (var_r8_929 != 0xF);
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
