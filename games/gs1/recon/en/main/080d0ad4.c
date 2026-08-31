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

s32 Func_080d0ad4(s32 actor) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    void *sp20;
    s32 *sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    u32 sp34;
    s32 sp38;
    s32 sp3C;
    u32 sp40;
    void *sp44;
    s32 sp48[3];
    s32 sp54[3];
    s32 sp60[3];
    s32 temp_r0_37;
    s32 temp_r1_252;
    s32 temp_r3_103;
    s32 temp_r3_257;
    s32 temp_r3_376;
    s32 temp_r5_357;
    s32 temp_r5_429;
    s32 temp_r6_346;
    s32 temp_r6_359;
    s32 temp_r7_314;
    s32 temp_r7_342;
    s32 var_fp_191;
    s32 var_r2_331;
    s32 var_r4_339;
    s32 var_r7_258;
    s32 var_r7_304;
    s32 var_r8_341;
    u32 temp_r6_25;
    void **temp_r5_28;
    void *temp_r2_18;
    void *temp_r3_135;
    void *temp_r6_203;
    void *temp_r9_332;
    void *var_r5_253;

    temp_r2_18 = *(void **)0x03001EEC;
    sp44 = temp_r2_18;
    sp40 = *(u32 *)0x03001EF0;
    temp_r6_25 = *(u32 *)0x03001EF4;
    temp_r5_28 = temp_r2_18 + 0x7828;
    sp2C = *(s32 *)0x03001E80;
    M2C_FIELD(temp_r2_18, s32 *, 0x7828) = actor;
    Func_080cd594(0);
    *(u16 *)0x04000020 = 0x100;
    temp_r0_37 = Func_08002f40(0x79);
    Func_080072f0(0x05000000, temp_r0_37, 0x80, 0x03001388);
    Func_08005340(temp_r0_37 + 0x80, sp44);
    Func_08005340(Func_08002f40(0x73), (void *) temp_r6_25);
    Func_08005340(Func_08002f40(0x76), temp_r2_18 + 0x1000);
    M2C_FIELD(sp44, s32 *, 0x7780) = 3;
    M2C_FIELD(sp44, s32 *, 0x7784) = 0x04040404;
    Func_080041d8(0x080CD261, 0x480);
    sp24 = (s32 *)((u8 *)&sp0 + 0x6C);
    Func_080e396c(M2C_FIELD(*temp_r5_28, s16 *, 0x24));
    temp_r3_103 = 0x40 - *sp24;
    sp28 = temp_r3_103;
    *(s32 *)0x04000028 = temp_r3_103 << 8;
    Func_080f9010(0x8E);
    sp3C = 0;
    if ((M2C_FIELD(*temp_r5_28, s32 *, 0x14) * 0x14) == -0x48) {

    } else {
loop_3:
        if (sp3C == 0x40) {
            Func_080b50e8(0);
        }
        if (sp3C == 0x2E) {
            temp_r3_135 = M2C_FIELD(sp44, void **, 0x7828);
            Func_080b5078(M2C_FIELD(temp_r3_135, s32 *, 8), M2C_FIELD(temp_r3_135, s16 *, 0x24), 0x10, 0);
        }
        Func_080d40ec(sp3C, 0xAAAB, 0x5555, 0);
        sp0 = 2;
        Func_080ed408(0x2E, 7, 7, 3);
        sp34 = *(u32 *)0x03001F08;
        sp0 = 2;
        Func_080ed408(0x2F, 7, 7, 7);
        sp38 = *(s32 *)0x03001F0C;
        if ((sp3C > 0x10) && !(0xF & sp3C)) {
            M2C_FIELD(sp44, s32 *, 0x7784) = (s32) (M2C_FIELD(sp44, s32 *, 0x7784) + 0x01010101);
        }
        sp30 = 0;
        sp10 = 0;
        spC = sp3C * 0x600;
        var_fp_191 = sp3C;
loop_11:
        temp_r6_203 = *Func_080b5098(M2C_FIELD(
            M2C_FIELD(sp44, void **, 0x7828), s16 *,
            (sp30 * 2) + 0x24));
        if ((u32) var_fp_191 > 0x5FU) {

        } else {
            Func_080049ac();
            Func_080051d8(sp2C, sp2C + 0xC);
            M2C_FIELD(&sp60, s32 *, 0) = M2C_FIELD(temp_r6_203, s32 *, 8);
            M2C_FIELD(&sp60, s32 *, 4) = (s32) M2C_FIELD(temp_r6_203, s32 *, 0xC);
            M2C_FIELD(&sp60, s32 *, 8) = (s32) M2C_FIELD(temp_r6_203, s32 *, 0x10);
            Func_080e3944(&sp60, &sp54);
            M2C_FIELD(&sp54, s32 *, 0) = *sp24 + sp28;
            M2C_FIELD(&sp54, s32 *, 4) = (s32) (M2C_FIELD(&sp54, s32 *, 4) - 0x18);
            if (var_fp_191 > 0x43) {

            } else {
                sp14 = 0;
                temp_r1_252 = 0x2A000 - spC;
                var_r5_253 = (sp10 * 0x1C) + sp44 + 0x7080;
                temp_r3_257 = (0x40 - var_fp_191) << 9;
                var_r7_258 = 0;
                do {
                    Func_080049ac();
                    if (var_fp_191 <= 0x3F) {
                        M2C_FIELD(&sp48, s32 *, 0) = temp_r1_252;
                        M2C_FIELD(&sp48, s32 *, 4) = temp_r1_252;
                        M2C_FIELD(&sp48, s32 *, 8) = temp_r1_252;
                        Func_08004cf0(&sp48);
                        Func_08004c6c(temp_r3_257);
                        Func_08004c1c(temp_r3_257);
                    }
                    Func_08004c6c(sp14);
                    Func_080e3944((s32 *)0x080EE134, &sp60);
                    M2C_FIELD(var_r5_253, s32 *, 0xC) =
                        sp60[0] + sp54[0];
                    M2C_FIELD(var_r5_253, s32 *, 0x10) =
                        sp60[1] + sp54[1] + 0x10;
                    var_r7_258 += 1;
                    sp14 += 0x5555;
                    var_r5_253 += 0x1C;
                } while (var_r7_258 != 3);
                sp1C = sp10;
                var_r7_304 = 0;
                do {
                    temp_r7_314 = var_r7_304 + 1;
                    sp20 = sp44 + ((var_r7_304 + sp1C) * 0x1C) + 0x7080;
                    sp18 = temp_r7_314;
                    var_r2_331 = var_fp_191;
                    temp_r9_332 = sp44 + ((Func_080022fc(temp_r7_314, 3) + sp1C) * 0x1C) + 0x7080;
                    if (var_r2_331 < 0) {
                        var_r2_331 += 0xF;
                    }
                    var_r4_339 = 5 - (var_r2_331 >> 4);
                    var_r8_341 = 0;
                    temp_r7_342 = var_r4_339 * 2;
loop_23:
                    temp_r6_346 = M2C_FIELD(sp20, s32 *, 0xC);
                    sp8 = var_r4_339;
                    temp_r5_357 = M2C_FIELD(sp20, s32 *, 0x10);
                    temp_r6_359 = temp_r6_346 + Func_080022ec(var_r8_341 * (M2C_FIELD(temp_r9_332, s32 *, 0xC) - temp_r6_346), 0x18);
                    temp_r3_376 = (temp_r5_357 + Func_080022ec(var_r8_341 * (M2C_FIELD(temp_r9_332, s32 *, 0x10) - temp_r5_357), 0x18)) - var_r4_339;
                    sp0 = temp_r7_342;
                    sp4 = temp_r7_342;
                    Func_080072f8(sp40,
                        (u8 *)sp44
                            + *(u16 *)(0x080EDE5C + temp_r7_342 - 2)
                            + 0x1000,
                        temp_r6_359 - var_r4_339, temp_r3_376);
                    var_r8_341 += 1;
                    if (var_r8_341 != 0x18) {
                        goto loop_23;
                    }
                    var_r7_304 = sp18;
                } while (var_r7_304 != 3);
            }
            if (var_fp_191 > 0x3F) {
                sp0 = 0x18;
                sp4 = 0x30;
                Func_080072f4(sp40, sp44, M2C_FIELD(&sp54, s32 *, 0) - 0x18, M2C_FIELD(&sp54, s32 *, 4) - 0x18);
                sp4 = 0x30;
                sp0 = 0x18;
                Func_080072f8(sp40, sp44, M2C_FIELD(&sp54, s32 *, 0), M2C_FIELD(&sp54, s32 *, 4) - 0x18);
            }
        }
        temp_r5_429 = sp30 + 1;
        sp10 += 0x20;
        spC += 0xFFFFD000;
        var_fp_191 -= 8;
        sp30 = temp_r5_429;
        if (temp_r5_429 != 1) {
            goto loop_11;
        }
        Func_08002dd8(0x2F);
        Func_08002dd8(0x2E);
        M2C_FIELD(sp44, s32 *, 0x7824) = temp_r5_429;
        Func_080030f8(1);
        sp3C += 1;
        if (sp3C != ((M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14) * 0x14) + 0x48)) {
            goto loop_3;
        }
    }
    Func_08004278(0x080CD261);
    return Func_080cdbc0();
}
