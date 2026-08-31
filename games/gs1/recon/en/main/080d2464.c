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

typedef void (*BattleEffectCopyFn)(void *, const void *, u32);
typedef void (*BattleEffectDrawFn)(s32, void *, s32, s32, s32, s32);

void Func_080d2464(void *arg0, s32 arg1) {
    BattleEffectCopyFn copy = (BattleEffectCopyFn)0x03001388;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    u32 sp20;
    s32 sp24;
    u32 sp28;
    u32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    M2C_UNK sp3C;
    s32 *var_r3_174;
    s32 *var_r3_215;
    s32 *var_r5_1153;
    s32 temp_r0_1132;
    s32 temp_r0_1155;
    s32 temp_r1_675;
    s32 temp_r2_746;
    s32 temp_r3_1079;
    s32 temp_r3_706;
    s32 temp_r3_753;
    s32 temp_r4_780;
    s32 temp_r4_992;
    s32 temp_r5_417;
    s32 temp_r6_482;
    s32 temp_r6_624;
    s32 temp_r6_984;
    s32 temp_r8_476;
    s32 temp_r8_618;
    s32 var_fp_457;
    s32 var_fp_605;
    s32 var_r0_191;
    s32 var_r3_491;
    s32 var_r3_602;
    s32 var_r3_631;
    s32 var_r6_288;
    s32 var_r6_947;
    s32 var_r7_1152;
    s32 var_r7_172;
    s32 var_r7_182;
    s32 var_r7_216;
    s32 var_r7_284;
    s32 var_r7_395;
    s32 var_r7_463;
    s32 var_r7_606;
    s32 var_r7_672;
    s32 var_r7_720;
    s32 var_r7_770;
    s32 var_r7_943;
    s32 var_r7_978;
    s32 var_r8_385;
    struct M2cAggregate_absolute_02010000 *var_r5_769;
    struct M2cAggregate_absolute_02010000 *var_r6_394;
    u32 temp_r0_431;
    u32 temp_r1_797;
    u32 temp_r2_804;
    u32 temp_r3_1017;
    u32 temp_r3_1070;
    u32 temp_r3_147;
    u32 temp_r3_831;
    u32 temp_r3_987;
    u32 var_r3_158;
    u8 temp_r0_1001;
    u8 temp_r0_778;
    void *temp_r0_59;
    void *temp_r3_25;
    void *var_r5_184;
    void *var_r5_465;
    void *var_r5_608;
    void *var_r5_673;
    void *var_r5_722;
    void *var_r5_982;

    sp38 = arg1;
    sp34 = M2C_FIELD(&absolute_03001ef0, s32 *, 0);
    sp30 = *(s32 *)0x03001E80;
    temp_r3_25 = *(void **)0x03001EEC;
    sp20 = absolute_03001ef0.field_0004;
    M2C_FIELD(temp_r3_25, void **, 0x7828) = arg0;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;
    *(s16 *)0x0400000C = 0x784;
    Func_080ed408(0x2E, 7, 7, 3, 2);
    sp28 = absolute_03001ef0.field_0018;
    Func_080ed408(0x2F, 7, 7, 3);
    sp2C = absolute_03001ef0.field_001c;
    temp_r0_59 = Func_08002f40(0x7D);
    copy((void *)0x05000000, temp_r0_59, 0x80);
    Func_08005340(temp_r0_59 + 0x80, temp_r3_25);
    Func_08005340(Func_08002f40(0xB4) + 0x80, temp_r3_25 + 0x3000);
    Func_08005340(Func_08002f40(0x73), (void *) sp20);
    if (sp38 == 1) {
        copy((void *)0x05000000, Func_08002f40(0xC4), 0x80);
    }
    M2C_FIELD(temp_r3_25, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r3_25, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    sp14 = 1;
    if (sp38 != 0) {
        sp14 = -1;
        if (M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 4) != 1) {
            sp14 = 1;
        }
    }
    if (sp38 == 1) {
        Func_080e396c(M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 8), &sp3C);
        temp_r3_147 = M2C_FIELD(&sp3C, u32 *, 0);
        M2C_FIELD(&sp3C, u32 *, 0) = (u32) ((s32) (temp_r3_147 + (temp_r3_147 >> 0x1F)) >> 1);
        M2C_FIELD(&sp3C, s32 *, 4) = 0x42;
        if (M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 4) == 1) {
            var_r3_158 = 0x4C;
        } else {
            var_r3_158 = 0x2C;
        }
        M2C_FIELD(&sp3C, u32 *, 0) = var_r3_158;
    }
    sp1C = 0xFFC40000;
    sp18 = 0xD40000;
    var_r7_172 = 0;
    var_r3_174 = temp_r3_25 + 0x7098;
    do {
        var_r7_172 += 1;
        *var_r3_174 = -1;
        var_r3_174 += 0x1C;
    } while (var_r7_172 != 0x40);
    var_r7_182 = 0;
    var_r5_184 = temp_r3_25 + 0x7320;
    do {
        if (sp14 == 1) {
            var_r0_191 = (Func_08004458() & 0x7F) + 0x80;
        } else {
            var_r0_191 = (Func_08004458() & 0x7F) - 0x80;
        }
        M2C_FIELD(var_r5_184, s32 *, 0) = var_r0_191;
        M2C_FIELD(var_r5_184, s32 *, 4) = (s32) ((7 & Func_08004458()) - 0x48);
        var_r7_182 += 1;
        M2C_FIELD(var_r5_184, s32 *, 0x18) = (s32) (0 - (0x1F & Func_08004458()));
        var_r5_184 += 0x1C;
    } while (var_r7_182 != 0x10);
    var_r3_215 = (s32 *)0x02010018;
    var_r7_216 = 0;
    do {
        var_r7_216 += 1;
        *var_r3_215 = -1;
        var_r3_215 += 0x1C;
    } while (var_r7_216 != 0x200);
    if (sp38 == 0) {
        Func_080d6750(M2C_FIELD(temp_r3_25, void **, 0x7828));
        Func_080030f8(1);
        Func_080dbb24(8, 0x179, 2);
    }
    sp24 = 0;
    spC = sp30 + 0xC;
loop_23:
    if ((*(s32 *)0x03001B04 & 3) && (sp24 > 0x30) && (sp24 <= 0x9F)) {
        if (sp38 == 0) {
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77D8), 8);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77DC), 9);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77E4), 0xA);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77E8), 0xB);
        }
        var_r7_284 = 0;
        if (M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r6_288 = 0x24;
            do {
                Func_080d6888(M2C_FIELD(
                    M2C_FIELD(temp_r3_25, void **, 0x7828), s16 *,
                    var_r6_288), 0xA, 5, -1, 0);
                Func_080b5088(M2C_FIELD(
                    M2C_FIELD(temp_r3_25, void **, 0x7828), s16 *,
                    var_r6_288), 4);
                var_r7_284 += 1;
                var_r6_288 += 2;
            } while (var_r7_284 != M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 0x14));
        }
        sp24 = 0xA0;
    }
    Func_080049ac();
    Func_080051d8(sp30, spC);
    if (sp24 == 0xB2) {
        Func_080b50e8(0x86);
    }
    if (sp24 == 0x80) {
        M2C_FIELD(temp_r3_25, s32 *, 0x7784) = 0x32;
    }
    if (sp24 == 0xB0) {
        M2C_FIELD(temp_r3_25, s32 *, 0x7780) = 3;
        M2C_FIELD(temp_r3_25, s32 *, 0x7784) = 0x02020202;
        Func_080072f0(0x05000000, Func_08002f40(0xB4), 0x80, 0x03001388);
    } else if ((u32) (sp24 - 0xA0) <= 0xFU) {
        M2C_FIELD(temp_r3_25, s32 *, 0x7780) = 1;
        M2C_FIELD(temp_r3_25, s32 *, 0x7784) = 0x10101010;
        if (sp24 > 0xAD) {
            M2C_FIELD(temp_r3_25, s32 *, 0x7784) = 0x3F3F3F3F;
        }
        Func_080e727c(2, 2, 2);
    }
    if ((u32) (sp24 - 0x21) <= 0x8EU) {
        var_r8_385 = 0;
        sp10 = 1;
        if (sp24 > 0x67) {
            sp10 = 8;
        }
        var_r6_394 = &absolute_02010000;
        var_r7_395 = 0;
loop_46:
        if ((var_r6_394->field_0018 != -1U) || (M2C_FIELD(var_r6_394, s32 *, 0) = (s32) (((0xFF & Func_08004458()) - 0x20) << 0x10), var_r6_394->field_0004 = 0x700000, temp_r5_417 = 3 & var_r7_395, var_r6_394->field_000c = ((Func_08004458() & 0x7F) + *(u8 *)(0x080EE184 + temp_r5_417)) << 9, temp_r0_431 = (0 - ((Func_08004458() & 0x7F) + *(u8 *)(0x080EE184 + temp_r5_417))) << 0xB, var_r6_394->field_0018 = 0, var_r6_394->field_0010 = temp_r0_431, var_r8_385 += 1, (var_r8_385 != sp10))) {
            var_r7_395 += 1;
            var_r6_394 += 0x1C;
            if (var_r7_395 != 0x200) {
                goto loop_46;
            }
        }
    }
    if ((u32) (sp24 - 0x29) > 0x56U) {

    } else {
        var_fp_457 = 0;
        if (1 & sp24) {
            var_r7_463 = 0;
            var_r5_465 = temp_r3_25 + 0x74E0;
loop_53:
            if (M2C_FIELD(var_r5_465, s32 *, 0x18) == -1) {
                temp_r8_476 = (0xFF & Func_08004458()) + 0x80;
                temp_r6_482 = (0x1FFF & Func_08004458()) + 0x4E20;
                if (sp38 == 0) {
                    var_r3_491 = 0x460000;
                    M2C_FIELD(var_r5_465, s32 *, 0) = (s32) (((Func_08004458() & 7) + 0x4E) << 0x10);
                } else {
                    M2C_FIELD(var_r5_465, s32 *, 0) = (s32) ((((Func_08004458() & 7) + M2C_FIELD(&sp3C, u32 *, 0)) - 8) << 0x10);
                    var_r3_491 = M2C_FIELD(&sp3C, s32 *, 4) << 0x10;
                }
                M2C_FIELD(var_r5_465, s32 *, 4) = var_r3_491;
                M2C_FIELD(var_r5_465, s32 *, 0xC) = (s32) ((s32) (temp_r8_476 * Func_08002322(temp_r6_482)) >> 9);
                var_fp_457 += 1;
                M2C_FIELD(var_r5_465, s32 *, 0x10) = (s32) ((s32) (temp_r8_476 * Func_0800231c(temp_r6_482)) >> 9);
                M2C_FIELD(var_r5_465, s32 *, 0x18) = 0;
                if (var_fp_457 != 1) {
                    goto block_59;
                }
            } else {
block_59:
                var_r7_463 += 1;
                var_r5_465 += 0x1C;
                if (var_r7_463 != 0x18) {
                    goto loop_53;
                }
            }
        }
    }
    if (sp24 == 0x30) {
        Func_080f9010(0x8D);
    }
    if (sp24 == 0x80) {
        Func_080f9010(0x91);
    }
    if ((u32) (sp24 - 0x81) <= 0x2EU) {
        var_r3_602 = 0;
        var_fp_605 = 0;
        var_r7_606 = 0;
        var_r5_608 = temp_r3_25 + 0x7080;
loop_66:
        if (var_r3_602 == 1) {
            temp_r8_618 = (0xFF & Func_08004458()) + 0x80;
            temp_r6_624 = (0x1FFF & Func_08004458()) + 0xFFFFB1E0;
            if (sp38 == 0) {
                M2C_FIELD(var_r5_608, s32 *, 0) = 0x440000;
                var_r3_631 = 0x400000;
            } else {
                M2C_FIELD(var_r5_608, s32 *, 0) = (s32) (M2C_FIELD(&sp3C, u32 *, 0) << 0x10);
                var_r3_631 = M2C_FIELD(&sp3C, s32 *, 4) << 0x10;
            }
            M2C_FIELD(var_r5_608, s32 *, 4) = var_r3_631;
            M2C_FIELD(var_r5_608, s32 *, 0xC) = (s32) ((s32) (temp_r8_618 * Func_08002322(temp_r6_624)) >> 6);
            M2C_FIELD(var_r5_608, s32 *, 0x10) = (s32) ((s32) (temp_r8_618 * Func_0800231c(temp_r6_624)) >> 6);
            M2C_FIELD(var_r5_608, s32 *, 0x18) = 0;
            var_r3_602 = 1;
            var_fp_605 += 1;
            if (var_fp_605 != 1) {
                goto block_71;
            }
        } else {
block_71:
            var_r7_606 += 1;
            var_r5_608 += 0x1C;
            if (var_r7_606 != 0x18) {
                goto loop_66;
            }
        }
    }
    if (sp24 <= 0xAF) {
        var_r7_672 = 0;
        var_r5_673 = temp_r3_25 + 0x7080;
        do {
            temp_r1_675 = M2C_FIELD(var_r5_673, s32 *, 0x18);
            if (temp_r1_675 >= 0) {
                ((BattleEffectDrawFn)sp28)(sp34, ((temp_r1_675 >> 2) << 0xB) + temp_r3_25, M2C_FIELD(var_r5_673, s16 *, 2) - 0x10, M2C_FIELD(var_r5_673, s16 *, 6) - 0x20, 0x20, 0x40);
                M2C_FIELD(var_r5_673, s32 *, 0) = (s32) (M2C_FIELD(var_r5_673, s32 *, 0) + (sp14 * M2C_FIELD(var_r5_673, s32 *, 0xC)));
                M2C_FIELD(var_r5_673, s32 *, 4) = (s32) (M2C_FIELD(var_r5_673, s32 *, 4) + M2C_FIELD(var_r5_673, s32 *, 0x10));
                temp_r3_706 = M2C_FIELD(var_r5_673, s32 *, 0x18) + 1;
                M2C_FIELD(var_r5_673, s32 *, 0x18) = temp_r3_706;
                if (temp_r3_706 == 0x18) {
                    M2C_FIELD(var_r5_673, s32 *, 0x18) = -1;
                }
            }
            var_r7_672 += 1;
            var_r5_673 += 0x1C;
        } while (var_r7_672 != 0x18);
    }
    var_r7_720 = 0;
    var_r5_722 = temp_r3_25 + 0x74E0;
    do {
        if ((s32) M2C_FIELD(var_r5_722, s32 *, 0x18) >= 0) {
            ((BattleEffectDrawFn)sp2C)(sp34, (void *)0x080EE188, M2C_FIELD(var_r5_722, s16 *, 2), M2C_FIELD(var_r5_722, s16 *, 6) - 1, 1, 2);
            M2C_FIELD(var_r5_722, s32 *, 0) = (s32) (M2C_FIELD(var_r5_722, s32 *, 0) + (sp14 * M2C_FIELD(var_r5_722, s32 *, 0xC)));
            temp_r2_746 = M2C_FIELD(var_r5_722, s32 *, 0x10);
            M2C_FIELD(var_r5_722, s32 *, 4) = (s32) (M2C_FIELD(var_r5_722, s32 *, 4) + temp_r2_746);
            temp_r3_753 = M2C_FIELD(var_r5_722, s32 *, 0x18) + 1;
            M2C_FIELD(var_r5_722, s32 *, 0x10) = (s32) (temp_r2_746 + 0xFFFFFC00);
            M2C_FIELD(var_r5_722, s32 *, 0x18) = temp_r3_753;
            if (temp_r3_753 == 0x30) {
                M2C_FIELD(var_r5_722, s32 *, 0x18) = -1;
            }
        }
        var_r7_720 += 1;
        var_r5_722 += 0x1C;
    } while (var_r7_720 != 0x18);
    if (sp24 <= 0xAF) {
        var_r5_769 = &absolute_02010000;
        var_r7_770 = 0;
        do {
            if ((s32) var_r5_769->field_0018 >= 0) {
                temp_r0_778 = *(u8 *)(0x080EE18A + (3 & var_r7_770));
                temp_r4_780 = temp_r0_778 * 2;
                ((BattleEffectDrawFn)sp28)(sp34, (void *)(sp20 + *(u16 *)(0x080EDE48 + (s32) (temp_r4_780 - 2))), M2C_FIELD(var_r5_769, s16 *, 2) - (temp_r0_778 >> 1), M2C_FIELD(var_r5_769, s16 *, 6) - temp_r0_778, (s32) temp_r0_778, temp_r4_780);
                temp_r1_797 = var_r5_769->field_000c;
                M2C_FIELD(var_r5_769, s32 *, 0) = (s32) (M2C_FIELD(var_r5_769, s32 *, 0) + (sp14 * temp_r1_797));
                temp_r2_804 = var_r5_769->field_0010;
                var_r5_769->field_0004 += temp_r2_804;
                if (sp24 > 0x80) {
                    if (1 & var_r7_770) {
                        var_r5_769->field_000c = temp_r1_797 + 0xFFFF8000;
                    } else {
                        var_r5_769->field_000c = temp_r1_797 + 0xFFFFE000;
                    }
                } else {
                    var_r5_769->field_0010 = temp_r2_804 + 0xFFFFFC00;
                }
                temp_r3_831 = var_r5_769->field_0018 + 1;
                var_r5_769->field_0018 = temp_r3_831;
                if (temp_r3_831 == 0x100) {
                    var_r5_769->field_0018 = -1U;
                }
            }
            var_r7_770 += 1;
            var_r5_769 += 0x1C;
        } while (var_r7_770 != 0x200);
    }
    if (sp24 == 0x80) {
        M2C_FIELD(temp_r3_25, s32 *, 0x77A8) = 0x30;
    }
    if ((sp38 == 0) && (sp24 == 0x30)) {
        M2C_FIELD(temp_r3_25, s32 *, 0x77A8) = 8;
    }
    if ((u32) (sp24 - 0x28) <= 7U) {
        sp18 += 0xFFF80000;
        sp1C += 0x100000;
    }
    if (sp38 == 0) {
        if (sp24 == 0x80) {
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77D8), 8);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77DC), 9);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77E4), 0xA);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77E8), 0xB);
        }
        if (sp24 == 0xB0) {
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77D8), 0);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77DC), 1);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77E4), 3);
            Func_08009020(M2C_FIELD(temp_r3_25, s32 *, 0x77E8), 4);
        }
        Func_080e6d3c(3, sp18, sp1C);
    }
    if (sp24 == 0x8A) {
        var_r7_943 = 0;
        if (M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r6_947 = 0x24;
            do {
                Func_080d6888(M2C_FIELD(
                    M2C_FIELD(temp_r3_25, void **, 0x7828), s16 *,
                    var_r6_947), 0xA, 5, -1, 0);
                Func_080b5088(M2C_FIELD(
                    M2C_FIELD(temp_r3_25, void **, 0x7828), s16 *,
                    var_r6_947), 4);
                var_r7_943 += 1;
                var_r6_947 += 2;
            } while (var_r7_943 != M2C_FIELD(M2C_FIELD(temp_r3_25, void **, 0x7828), s32 *, 0x14));
        }
    }
    if (sp24 > 0xAF) {

    } else {
        var_r7_978 = 0;
        var_r5_982 = temp_r3_25 + 0x7320;
loop_114:
        temp_r6_984 = M2C_FIELD(var_r5_982, s32 *, 4);
        if (temp_r6_984 > 0x37) {
            temp_r3_987 = M2C_FIELD(var_r5_982, u32 *, 0x18);
            if (temp_r3_987 <= 0xBU) {
                temp_r4_992 = (s32) (temp_r3_987 + (temp_r3_987 >> 0x1F)) >> 1;
                temp_r0_1001 = *(u8 *)(0x080EE18E + temp_r4_992);
                ((BattleEffectDrawFn)sp28)(sp34,
                    *(u16 *)(0x080EE1A0 + (temp_r4_992 * 2)) +
                        temp_r3_25 + 0x3000,
                    M2C_FIELD(var_r5_982, s32 *, 0) -
                        (temp_r0_1001 >> 1),
                    temp_r6_984 + *(u8 *)(0x080EE19A + temp_r4_992),
                    (s32)temp_r0_1001,
                    *(u8 *)(0x080EE194 + temp_r4_992));
            }
            temp_r3_1017 = M2C_FIELD(var_r5_982, u32 *, 0x18) + 1;
            M2C_FIELD(var_r5_982, u32 *, 0x18) = temp_r3_1017;
            if (temp_r3_1017 == 0xC) {
                M2C_FIELD(var_r5_982, u32 *, 0x18) = 0U;
            }
        } else {
            temp_r3_1070 = M2C_FIELD(var_r5_982, u32 *, 0x18);
            if (temp_r3_1070 == 0) {
                temp_r3_1079 = temp_r6_984 + 6;
                M2C_FIELD(var_r5_982, s32 *, 0) = (s32) (M2C_FIELD(var_r5_982, s32 *, 0) - (sp14 * 6));
                M2C_FIELD(var_r5_982, s32 *, 4) = temp_r3_1079;
                if ((sp24 <= 0x2F) && (temp_r3_1079 > 0x37)) {
                    sp8 = 0xA;
                    Func_080f9010(0x88);
                }
                    ((BattleEffectDrawFn)sp28)(sp34, (void *)(sp20 + *(u16 *)0x080EDE5A), M2C_FIELD(var_r5_982, s32 *, 0) - 5, M2C_FIELD(var_r5_982, s32 *, 4) + 0x1E, 0xA, 0x14);
            } else {
                M2C_FIELD(var_r5_982, u32 *, 0x18) = (u32) (temp_r3_1070 + 1);
            }
        }
        var_r7_978 += 1;
        var_r5_982 += 0x1C;
        if (var_r7_978 != 0x10) {
            goto loop_114;
        }
    }
    Func_080e155c(8, 8);
    Func_080cd52c();
    M2C_FIELD(temp_r3_25, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    temp_r0_1132 = sp24 + 1;
    sp24 = temp_r0_1132;
    if (temp_r0_1132 != 0xD0) {
        goto loop_23;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    if (sp38 == 0) {
        Func_080e6eac(3, sp18, sp1C);
        var_r7_1152 = 0;
        var_r5_1153 = temp_r3_25 + 0x77D8;
        do {
            temp_r0_1155 = *var_r5_1153;
            var_r5_1153 += 4;
            var_r7_1152 += 1;
            Func_08009038(temp_r0_1155);
        } while (var_r7_1152 != 8);
    }
    Func_080cdbc0();
}
