#include "types.h"

void Func_080030f8();
void Func_08003f3c();
void Func_0800447c();
void Func_08009080();
void Func_08009098();
void Func_08009150();
s32 Func_080091d8();
void Func_08009240();
void Func_08009248();
s32 Func_08009250();
s32 Func_0808e4b4();
void Func_08096b28();
void *Func_08096c48();
void *Func_08096c80();
void Func_08097384();
void Func_0809748c();
void Func_080974d8();
void Func_0809ba70();
void Func_0809ba7c();
void Func_0809ba90();
void Func_080f9010();

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

#endif

void Func_08098cd8(void) {
    M2C_UNK *sp0;
    M2C_UNK *sp4;
    M2C_UNK *sp8;
    s32 spC;
    M2C_UNK sp10;
    M2C_UNK *sp24;
    M2C_UNK **var_r7_161;
    M2C_UNK *temp_r0_167;
    M2C_UNK *temp_r0_95;
    M2C_UNK *temp_r5_263;
    s32 temp_r0_363;
    s32 temp_r3_142;
    s32 var_r8_160;
    s32 var_r8_260;
    s32 var_r8_287;
    s32 var_r8_30;
    u8 temp_fp_212;
    void *temp_r2_216;
    void *temp_r2_33;
    void *temp_r2_73;
    void *temp_r3_16;
    void *var_fp_159;
    void *var_r5_28;

    temp_r3_16 = *(void **)0x03001F30;
    sp8 = M2C_FIELD(temp_r3_16, M2C_UNK **, 0x14);
    Func_08097384();
    Func_080f9010(0x82);
    var_r5_28 = temp_r3_16 + 0x58;
    var_r8_30 = 0xB;
    do {
        temp_r2_33 = M2C_FIELD(temp_r3_16, void **, 0x10);
        M2C_FIELD(&sp10, s32 *, 0) = (s32) M2C_FIELD(temp_r2_33, s32 *, 8);
        M2C_FIELD(&sp10, s32 *, 4) = (s32) (M2C_FIELD(temp_r2_33, s32 *, 0xC) + 0x100000);
        M2C_FIELD(&sp10, s32 *, 8) = (s32) M2C_FIELD(temp_r2_33, s32 *, 0x10);
        Func_080974d8(&sp10);
        Func_0809ba90(var_r5_28, 0x11C, M2C_FIELD(&sp10, s32 *, 0), M2C_FIELD(&sp10, s32 *, 8));
        Func_0809ba7c(var_r5_28, 0x08098B11);
        Func_0809ba70(var_r5_28, 7);
        Func_08009248(M2C_FIELD(var_r5_28, s32 *, 0), 9);
        M2C_FIELD(var_r5_28, s32 *, 0x2C) = 0xB333;
        M2C_FIELD(var_r5_28, s32 *, 0x28) = 0xB333;
        Func_080030f8(2);
        var_r8_30 -= 1;
        var_r5_28 += 0x48;
    } while (var_r8_30 >= 0);
    temp_r2_73 = M2C_FIELD(temp_r3_16, void **, 0x10);
    M2C_FIELD(&sp10, s32 *, 0) = (s32) M2C_FIELD(temp_r2_73, s32 *, 8);
    M2C_FIELD(&sp10, s32 *, 4) = (s32) (M2C_FIELD(temp_r2_73, s32 *, 0xC) + 0x100000);
    M2C_FIELD(&sp10, s32 *, 8) = (s32) M2C_FIELD(temp_r2_73, s32 *, 0x10);
    Func_0800447c(0x80000, M2C_FIELD(temp_r3_16, s32 *, 0), &sp10);
    temp_r0_95 = Func_08096c80(0xD7, M2C_FIELD(&sp10, s32 *, 0), M2C_FIELD(&sp10, s32 *, 4), M2C_FIELD(&sp10, s32 *, 8));
    if (temp_r0_95 == NULL) {
        Func_0809748c();
        return;
    }
    M2C_FIELD(temp_r0_95, s32 *, 0x1C) = 0x4000;
    M2C_FIELD(temp_r0_95, s32 *, 0x18) = 0x4000;
    M2C_FIELD(temp_r0_95, s16 *, 6) = (s16) M2C_FIELD(temp_r3_16, s32 *, 0);
    M2C_FIELD(temp_r0_95, s32 *, 0x30) = 0x40000;
    M2C_FIELD(temp_r0_95, s32 *, 0x34) = 0x40000;
    M2C_FIELD(temp_r0_95, s8 *, 0x55) = 0;
    Func_08009080(temp_r0_95, 5);
    Func_08009240(temp_r0_95, 3);
    if ((s32) M2C_FIELD(temp_r0_95, s32 *, 0x18) < 0x10000) {
        do {
            temp_r3_142 = M2C_FIELD(temp_r0_95, s32 *, 0x18) + 0x500;
            M2C_FIELD(temp_r0_95, s32 *, 0x1C) = temp_r3_142;
            M2C_FIELD(temp_r0_95, s32 *, 0x18) = temp_r3_142;
            Func_080030f8(1);
        } while ((s32) M2C_FIELD(temp_r0_95, s32 *, 0x18) <= 0xFFFF);
    }
    Func_080030f8(3);
    sp4 = &*(&sp0 + 0x1C);
    var_fp_159 = NULL;
    var_r8_160 = 2;
    var_r7_161 = &sp24;
    do {
        temp_r0_167 = Func_08096c80(0xD7, M2C_FIELD(temp_r0_95, s32 *, 8), M2C_FIELD(temp_r0_95, s32 *, 0xC), M2C_FIELD(temp_r0_95, s32 *, 0x10));
        *var_r7_161 = temp_r0_167;
        var_r7_161 -= 4;
        if (temp_r0_167 != NULL) {
            M2C_FIELD(temp_r0_167, s32 *, 0x1C) = 0xF000;
            M2C_FIELD(temp_r0_167, s32 *, 0x18) = 0xF000;
            M2C_FIELD(temp_r0_167, s16 *, 6) = (s16) M2C_FIELD(temp_r3_16, s32 *, 0);
            M2C_FIELD(temp_r0_167, s32 *, 0x30) = 0x40000;
            M2C_FIELD(temp_r0_167, s32 *, 0x34) = 0x40000;
            M2C_FIELD(temp_r0_167, s8 *, 0x55) = 0;
            Func_08009080(temp_r0_167, 5);
            Func_08009240(temp_r0_167, 2);
            var_fp_159 = Func_08096c48(M2C_FIELD(temp_r0_167, s32 *, 0x50), var_fp_159);
        }
        var_r8_160 -= 1;
    } while (var_r8_160 >= 0);
    temp_fp_212 = M2C_FIELD(var_fp_159, u8 *, 0x1C);
    if ((s8) M2C_FIELD(temp_r3_16, u8 *, 0x20) != 0) {
        temp_r2_216 = M2C_FIELD(temp_r3_16, void **, 0x10);
        M2C_FIELD(&sp10, s32 *, 0) = (s32) M2C_FIELD(temp_r2_216, s32 *, 8);
        M2C_FIELD(&sp10, s32 *, 4) = (s32) (M2C_FIELD(temp_r2_216, s32 *, 0xC) + 0x100000);
        M2C_FIELD(&sp10, s32 *, 8) = (s32) M2C_FIELD(temp_r2_216, s32 *, 0x10);
        Func_0800447c(0x380000, M2C_FIELD(temp_r3_16, s32 *, 0), &sp10);
    } else {
        M2C_FIELD(&sp10, s32 *, 0) = (s32) M2C_FIELD(temp_r3_16, s32 *, 4);
        M2C_FIELD(&sp10, s32 *, 4) = (s32) (M2C_FIELD(temp_r3_16, s32 *, 8) + 0x100000);
        M2C_FIELD(&sp10, s32 *, 8) = (s32) M2C_FIELD(temp_r3_16, s32 *, 0xC);
    }
    Func_08009150(temp_r0_95, M2C_FIELD(&sp10, s32 *, 0), M2C_FIELD(&sp10, s32 *, 4), M2C_FIELD(&sp10, s32 *, 8));
    Func_08009098(temp_r0_95, 0x0809F12C);
    sp0 = sp4;
    var_r8_260 = 2;
    do {
        temp_r5_263 = *sp0;
        sp0 += 4;
        if (temp_r5_263 != NULL) {
            Func_080030f8(3);
            Func_08009150(temp_r5_263, M2C_FIELD(&sp10, s32 *, 0), M2C_FIELD(&sp10, s32 *, 4), M2C_FIELD(&sp10, s32 *, 8));
            Func_08009098(temp_r5_263, 0x0809F0B4);
        }
        var_r8_260 -= 1;
    } while (var_r8_260 >= 0);
    var_r8_287 = 0;
    if (M2C_FIELD(temp_r0_95, M2C_UNK **, 0) != NULL) {
loop_21:
        Func_080030f8(1);
        var_r8_287 += 1;
        if (var_r8_287 <= 0x3B) {
            if (M2C_FIELD(temp_r0_95, M2C_UNK **, 0) != NULL) {
                goto loop_21;
            }
        }
    }
    if ((sp8 != NULL) && ((s8) M2C_FIELD(temp_r3_16, u8 *, 0x35) == 0)) {
        if ((s8) M2C_FIELD(temp_r3_16, u8 *, 0x34) != 0) {
            M2C_FIELD(sp8, s32 *, 0x28) = 0x80000;
        }
        M2C_FIELD(&sp10, s32 *, 0) = (s32) M2C_FIELD(sp8, s32 *, 8);
        M2C_FIELD(&sp10, s32 *, 4) = (s32) M2C_FIELD(sp8, s32 *, 0xC);
        M2C_FIELD(&sp10, s32 *, 8) = (s32) M2C_FIELD(sp8, s32 *, 0x10);
        Func_0800447c(0x100000, M2C_FIELD(temp_r3_16, s32 *, 0), &sp10);
        if ((Func_080091d8(sp8, &sp10) == 0) && (Func_08009250(sp8, &sp10) == 0)) {
            M2C_FIELD(sp8, s32 *, 0x34) = 0x10000;
            M2C_FIELD(sp8, s32 *, 0x30) = 0x10000;
            Func_08009150(sp8, M2C_FIELD(&sp10, s32 *, 0), M2C_FIELD(&sp10, s32 *, 4), M2C_FIELD(&sp10, s32 *, 8));
        }
    }
    temp_r0_363 = Func_0808e4b4(0x50000005, 4, &spC);
    if (temp_r0_363 != 0) {
        Func_08096b28(temp_r0_363, *(s32 *)0x02000434, spC);
    }
    Func_080030f8(0xA);
    Func_0809748c();
    Func_080030f8(0x14);
    if (temp_fp_212 != 0x60) {
        Func_08003f3c(temp_fp_212);
    }
}
