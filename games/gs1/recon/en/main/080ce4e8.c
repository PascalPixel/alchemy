#include "types.h"

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

void **Func_080b5098(s32 member_id);
typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef s32 (*MagnitudeFn)(s32 squared_distance);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);
extern u8 Value_000000a9;
extern u8 Value_000000bb;

s32 Func_080ce4e8(s32 actor) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    u32 *sp18;
    s32 sp1C;
    u32 sp20;
    u32 sp24;
    s32 sp28;
    u32 sp2C;
    s32 sp30[3];
    s32 sp3C[3];
    s32 *var_r6_142;
    s32 temp_r1_281;
    s32 temp_r2_277;
    s32 temp_r2_373;
    s32 temp_r3_271;
    s32 temp_r3_275;
    s32 temp_r3_279;
    s32 temp_r5_233;
    s32 temp_r5_298;
    s32 temp_r5_318;
    s32 temp_r5_324;
    s32 temp_r5_330;
    s32 temp_r7_288;
    s32 var_fp_192;
    s32 var_r4_146;
    s32 var_r4_170;
    s32 var_r4_260;
    s32 var_r4_87;
    s32 var_r5_147;
    s32 var_r5_171;
    u8 *var_r5_84;
    u8 *var_r6_262;
    void *temp_r0_29;
    void *temp_r5_216;
    void *temp_sl_23;

    sp2C = *(u32 *)0x03001EF0;
    temp_sl_23 = *(void **)0x03001EEC;
    M2C_FIELD(temp_sl_23, s32 *, 0x7828) = actor;
    Func_080cd594(0);
    temp_r0_29 = Func_08002f40((s32)&Value_000000a9);
    ((WordCopyFn)0x03001388)((void *)0x05000000, temp_r0_29, 0x80);
    Func_08005340(temp_r0_29 + 0x80, temp_sl_23);
    ((WordCopyFn)0x03001388)((void *)0x05000000,
        Func_08002f40((s32)&Value_000000bb), 0x80);
    Func_080ed408(0x2E, 7, 7, 3, 2);
    sp20 = *(u32 *)0x03001F08;
    Func_080ed408(0x2F, 7, 7, 3, 3);
    sp24 = *(u32 *)0x03001F0C;
    Func_080041d8(0x080DBB9D, 0x480);
    M2C_FIELD(temp_sl_23, s32 *, 0x7780) = 3;
    M2C_FIELD(temp_sl_23, s32 *, 0x7784) = 0x04040404;
    var_r5_84 = (u8 *)0x02010000;
    Func_080041d8(0x080CD261, 0x480);
    var_r4_87 = 0;
    do {
        sp8 = var_r4_87;
        M2C_FIELD(var_r5_84, s32 *, 0) = (s32) ((Func_08004458() - 0x7F) << 0xF);
        M2C_FIELD(var_r5_84, s32 *, 4) =
            (Func_08004458() - 0x7F) << 0xF;
        var_r4_87 += 1;
        M2C_FIELD(var_r5_84, s32 *, 8) =
            (Func_08004458() - 0x7F) << 0xF;
        var_r5_84 += 0x1C;
    } while (var_r4_87 != 0x200);
    Func_080f9010(0x8E);
    sp28 = 0;
    if ((M2C_FIELD(M2C_FIELD(temp_sl_23, void **, 0x7828), s32 *, 0x14) << 5) == -0x60) {

    } else {
loop_4:
        sp1C = *(s32 *)0x03001E80;
        if (sp28 == 0x60) {
            Func_080b50e8(0);
        }
        var_r6_142 = temp_sl_23 + 0x6980;
        if (M2C_FIELD(M2C_FIELD(temp_sl_23, void **, 0x7828), s32 *, 4) == 0) {
            var_r4_146 = 0;
            var_r5_147 = sp28 << 0xB;
            do {
                sp8 = var_r4_146;
                *var_r6_142 = (s32) (0x60000 - (Func_08002322(var_r5_147) * 6)) >> 0xA;
                var_r6_142 += 4;
                var_r4_146 += 1;
                var_r5_147 += 0x800;
            } while (var_r4_146 != 0xA0);
        } else {
            var_r4_170 = 0;
            var_r5_171 = sp28 << 0xB;
            do {
                sp8 = var_r4_170;
                var_r4_170 += 1;
                *var_r6_142 = (s32) (Func_08002322(var_r5_171) * 6) >> 0xA;
                var_r6_142 += 4;
                var_r5_171 += 0x800;
            } while (var_r4_170 != 0xA0);
        }
        var_fp_192 = 0;
        if (M2C_FIELD(M2C_FIELD(temp_sl_23, void **, 0x7828), s32 *, 0x14) == 0) {

        } else {
            sp18 = (u32 *)(sp1C + 12);
            sp10 = 0x24;
            spC = 0;
loop_15:
            temp_r5_216 = *Func_080b5098(M2C_FIELD(
                M2C_FIELD(temp_sl_23, void **, 0x7828), s16 *, sp10));
            Func_080049ac();
            Func_080051d8(sp1C, (s32)sp18);
            M2C_FIELD(&sp30, s32 *, 0) = (s32) M2C_FIELD(temp_r5_216, s32 *, 8);
            M2C_FIELD(&sp30, s32 *, 4) = 0x140000;
            M2C_FIELD(&sp30, s32 *, 8) = (s32) M2C_FIELD(temp_r5_216, s32 *, 0x10);
            Func_08004cb4(&sp30);
            temp_r5_233 = var_fp_192 << 5;
            if (sp28 > temp_r5_233) {
                Func_08004bd4(sp28 << 9);
                if (sp28 == (temp_r5_233 + 0x20)) {
                    Func_080d6888(M2C_FIELD(
                        M2C_FIELD(temp_sl_23, void **, 0x7828), s16 *, sp10),
                        7, 5, var_fp_192, 0x20);
                }
                sp14 = var_fp_192 * 8;
                var_r4_260 = 0;
                var_r6_262 = (u8 *)0x02010000 + spC;
                do {
                    if (sp28 > (s32) ((sp14 + var_r4_260) * 4)) {
                        temp_r3_271 = (s32) M2C_FIELD(var_r6_262, s32 *, 0) >> 8;
                        temp_r3_275 = (s32) M2C_FIELD(var_r6_262, s32 *, 4) >> 8;
                        temp_r2_277 = temp_r3_275 * temp_r3_275;
                        temp_r3_279 = (s32) M2C_FIELD(var_r6_262, s32 *, 8) >> 8;
                        temp_r1_281 = temp_r3_279 * temp_r3_279;
                        sp8 = var_r4_260;
                        temp_r7_288 = ((MagnitudeFn)0x030001D8)(
                            (temp_r3_271 * temp_r3_271)
                                + temp_r2_277 + temp_r1_281) >> 8;
                        if (temp_r7_288 != 0) {
                            Func_080e3944(var_r6_262, &sp3C);
                            temp_r5_298 = (s32) M2C_FIELD(&sp3C, s32 *, 0) >> 1;
                            M2C_FIELD(&sp3C, s32 *, 0) = temp_r5_298;
                            ((DrawRectangleFn)sp24)(
                                (void *)sp2C,
                                (u8 *)temp_sl_23
                                    + Func_080022fc(sp8, 3) * 0x240,
                                temp_r5_298 - 0xC, sp3C[1] - 0xC,
                                0x18, 0x18);
                            temp_r5_318 = M2C_FIELD(var_r6_262, s32 *, 0);
                            M2C_FIELD(var_r6_262, s32 *, 0) = (s32) (temp_r5_318 - Func_080022ec(temp_r5_318, temp_r7_288));
                            temp_r5_324 = M2C_FIELD(var_r6_262, s32 *, 4);
                            M2C_FIELD(var_r6_262, s32 *, 4) = (s32) (temp_r5_324 - Func_080022ec(temp_r5_324, temp_r7_288));
                            temp_r5_330 = M2C_FIELD(var_r6_262, s32 *, 8);
                            M2C_FIELD(var_r6_262, s32 *, 8) = (s32) (temp_r5_330 - Func_080022ec(temp_r5_330, temp_r7_288));
                            M2C_FIELD(var_r6_262, s32 *, 0x18) = (s32) (M2C_FIELD(var_r6_262, s32 *, 0x18) + 1);
                            var_r4_260 = sp8;
                        }
                    }
                    var_r4_260 += 1;
                    var_r6_262 += 0x1C;
                } while (var_r4_260 != 8);
            }
            sp10 += 2;
            spC += 0x700;
            var_fp_192 += 1;
            if (var_fp_192 != M2C_FIELD(M2C_FIELD(temp_sl_23, void **, 0x7828), s32 *, 0x14)) {
                goto loop_15;
            }
        }
        Func_080cd52c();
        M2C_FIELD(temp_sl_23, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        temp_r2_373 = sp28 + 1;
        sp28 = temp_r2_373;
        if (temp_r2_373 != ((M2C_FIELD(M2C_FIELD(temp_sl_23, void **, 0x7828), s32 *, 0x14) << 5) + 0x60)) {
            goto loop_4;
        }
    }
    Func_08004278(0x080CD261);
    Func_08004278(0x080DBB9D);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    return Func_080cdbc0();
}
