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

#endif

void Func_08099da4(void) {
    void *sp0;
    void **sp4;
    void *sp8;
    s8 *spC;
    s8 *sp10;
    void *sp14;
    void *sp18;
    void *sp1C;
    M2C_UNK sp20;
    s32 temp_r0_288;
    s32 temp_r0_33;
    s32 temp_r0_457;
    s32 temp_r1_211;
    s32 temp_r1_40;
    s32 temp_r1_77;
    s32 temp_r2_44;
    s32 temp_r5_317;
    s32 temp_r5_472;
    s32 temp_r6_318;
    s32 var_r8_440;
    s32 var_r8_528;
    s32 var_r8_547;
    s8 temp_r0_277;
    u16 temp_r6_207;
    void *temp_r0_447;
    void *temp_r0_52;
    void *temp_r0_63;
    void *temp_r3_16;
    void *temp_r7_20;
    void *var_r1_149;

    temp_r3_16 = *(void **)0x03001F30;
    sp1C = M2C_FIELD(temp_r3_16, void **, 0x10);
    temp_r7_20 = M2C_FIELD(temp_r3_16, void **, 0x14);
    if (temp_r7_20 == NULL) {
        return;
    }
    Func_08097384();
    M2C_FIELD(sp1C, void **, 0x68) = temp_r7_20;
    Func_08009098(sp1C, 0x0809F0BC);
    temp_r0_33 = M2C_FIELD(temp_r3_16, s32 *, 4);
    M2C_FIELD(&sp20, s32 *, 0) = temp_r0_33;
    temp_r1_40 = M2C_FIELD(temp_r3_16, s32 *, 8) + 0x100000;
    M2C_FIELD(&sp20, s32 *, 4) = temp_r1_40;
    temp_r2_44 = M2C_FIELD(temp_r3_16, s32 *, 0xC);
    M2C_FIELD(&sp20, s32 *, 8) = temp_r2_44;
    temp_r0_52 = Func_0809a3c4(temp_r0_33 + 0x200000, temp_r1_40, temp_r2_44, 0x8000);
    sp14 = temp_r0_52;
    temp_r0_63 = Func_0809a3c4(M2C_FIELD(&sp20, s32 *, 0) + 0xFFE00000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8), 0);
    sp18 = temp_r0_63;
    if ((sp14 == NULL) || (temp_r0_63 == NULL)) {
        Func_0809748c();
        return;
    }
    Func_080030f8(0xF);
    temp_r1_77 = M2C_FIELD(temp_r7_20, s32 *, 8);
    M2C_FIELD(&sp20, s32 *, 0) = temp_r1_77;
    M2C_FIELD(&sp20, s32 *, 4) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) + 0x100000);
    M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r7_20, s32 *, 0x10);
    Func_08009150(temp_r0_52, temp_r1_77 + 0x100000);
    Func_08009150(temp_r0_63, M2C_FIELD(&sp20, s32 *, 0) + 0xFFF00000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
    Func_08009158(temp_r0_52);
    Func_08009158(temp_r0_63);
    M2C_FIELD(temp_r0_52, s32 *, 0x24) = 0;
    M2C_FIELD(temp_r0_52, s32 *, 8) = (s32) (M2C_FIELD(&sp20, s32 *, 0) + 0x100000);
    M2C_FIELD(temp_r0_63, s32 *, 8) = (s32) (M2C_FIELD(&sp20, s32 *, 0) + 0xFFF00000);
    M2C_FIELD(temp_r0_63, s32 *, 0x24) = 0;
    M2C_FIELD(temp_r7_20, s32 *, 0x6C) = 0x08096B89;
    Func_080041d8(0x08099D19, 0xC80);
    Func_080f9010(0x82);
    sp10 = temp_r7_20 + 0x55;
    M2C_FIELD(temp_r7_20, s8 *, 0x55) = 4;
    Func_080091e0(temp_r7_20, 0);
    if ((sp14 != NULL) && (sp18 != NULL) && ((s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) - M2C_FIELD(temp_r7_20, s32 *, 0x14)) <= 0x180000)) {
        var_r1_149 = (void *)0x6000;
        do {
            M2C_FIELD(temp_r0_52, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r0_52, s32 *, 0xC) + var_r1_149);
            M2C_FIELD(temp_r0_63, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r0_63, s32 *, 0xC) + var_r1_149);
            M2C_FIELD(temp_r7_20, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) + var_r1_149);
            sp0 = var_r1_149;
            Func_080030f8(1);
        } while ((s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) - M2C_FIELD(temp_r7_20, s32 *, 0x14)) <= 0x180000);
    }
    M2C_FIELD(sp14, s32 *, 0x30) = 0x40000;
    M2C_FIELD(sp14, s32 *, 0x34) = 0x8000;
    M2C_FIELD(sp18, s32 *, 0x30) = 0x40000;
    M2C_FIELD(sp18, s32 *, 0x34) = 0x8000;
    M2C_FIELD(temp_r7_20, s32 *, 0x30) = 0x6666;
    M2C_FIELD(temp_r7_20, s32 *, 0x34) = 0x3333;
    M2C_FIELD(temp_r7_20, s8 *, 0x5A) = 0;
    spC = temp_r7_20 + 0x22;
    *spC = 2;
    sp8 = sp18;
loop_18:
    Func_080030f8(1);
    if (!(*(s32 *)0x03001C94 & 0x303)) {
        temp_r6_207 = Func_08097b54(*(s32 *)0x03001AE8);
        if (temp_r6_207 == 0xFFFF) {
            temp_r1_211 = M2C_FIELD(temp_r7_20, s32 *, 8);
            M2C_FIELD(&sp20, s32 *, 0) = temp_r1_211;
            M2C_FIELD(&sp20, s32 *, 4) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) + 0x100000);
            M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r7_20, s32 *, 0x10);
            Func_08009150(sp14, temp_r1_211 + 0x100000);
            Func_08009150(sp18, M2C_FIELD(&sp20, s32 *, 0) + 0xFFF00000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
            Func_08009080(sp14, 1);
            Func_08009080(sp18, 1);
            goto loop_18;
        }
        M2C_FIELD(&sp20, s32 *, 0) = (s32) M2C_FIELD(temp_r7_20, s32 *, 8);
        M2C_FIELD(&sp20, s32 *, 4) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) + 0x100000);
        M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r7_20, s32 *, 0x10);
        Func_0800447c(0x20000, temp_r6_207, &sp20);
        Func_08009150(sp14, M2C_FIELD(&sp20, s32 *, 0) + 0x100000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
        Func_08009150(sp18, M2C_FIELD(&sp20, s32 *, 0) + 0xFFF00000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
        Func_08009158(sp14);
        Func_08009158(sp18);
        M2C_FIELD(&sp20, s32 *, 0) = (s32) M2C_FIELD(temp_r7_20, s32 *, 8);
        M2C_FIELD(&sp20, s32 *, 4) = (s32) M2C_FIELD(temp_r7_20, s32 *, 0x14);
        M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r7_20, s32 *, 0x10);
        Func_0800447c(0x100000, temp_r6_207, &sp20);
        temp_r0_277 = Func_08009250(temp_r7_20, &sp20);
        if ((temp_r0_277 != 0) || (M2C_FIELD(temp_r7_20, s32 *, 0x14) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0x14) + 0x100000), temp_r0_288 = Func_080091d8(temp_r7_20, &sp20), M2C_FIELD(temp_r7_20, s32 *, 0x14) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0x14) + 0xFFF00000), (temp_r0_288 > 0))) {
            Func_08009080(sp14, 4);
            Func_08009080(sp18, 4);
            if (!(*(s32 *)0x03001E40 & 0xF)) {
                Func_080f9010(0x72);
            }
            goto loop_18;
        }
        Func_080f9010(0xAF);
        temp_r5_317 = M2C_FIELD(&sp20, s32 *, 0);
        temp_r6_318 = M2C_FIELD(&sp20, s32 *, 8);
        Func_08009080(sp14, 4);
        Func_08009080(sp18, 4);
        Func_080030f8(0xF);
        M2C_FIELD(temp_r7_20, s8 *, 0x5B) = temp_r0_277;
        M2C_FIELD(temp_r7_20, s32 *, 0x30) = 0x3333;
        M2C_FIELD(temp_r7_20, s32 *, 0x34) = 0x3333;
        Func_08009150(temp_r7_20, M2C_FIELD(&sp20, s32 *, 0), M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
        M2C_FIELD(sp14, s32 *, 0x30) = 0x3333;
        M2C_FIELD(sp14, s32 *, 0x34) = 0x3333;
        M2C_FIELD(sp8, s32 *, 0x30) = 0x3333;
        M2C_FIELD(sp8, s32 *, 0x34) = 0x3333;
        Func_08009150(sp14, M2C_FIELD(&sp20, s32 *, 0) + 0x100000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
        Func_08009150(sp8, M2C_FIELD(&sp20, s32 *, 0) + 0xFFF00000, M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
        Func_08009158(temp_r7_20);
        M2C_FIELD(temp_r7_20, s32 *, 8) = temp_r5_317;
        M2C_FIELD(temp_r7_20, s32 *, 0x10) = temp_r6_318;
        M2C_FIELD(temp_r7_20, s32 *, 0x24) = (s32) temp_r0_277;
        M2C_FIELD(temp_r7_20, s32 *, 0x2C) = (s32) temp_r0_277;
        Func_080030f8(0xA);
    }
    Func_08009080(sp14, 4);
    Func_08009080(sp18, 4);
    Func_08004278(0x08099D19);
    Func_080f9010(0x87);
    Func_080030f8(0xF);
    Func_080f9010(0x87);
    Func_080030f8(0xF);
    M2C_FIELD(&sp20, s32 *, 0) = (s32) M2C_FIELD(temp_r7_20, s32 *, 8);
    M2C_FIELD(&sp20, s32 *, 4) = (s32) (M2C_FIELD(temp_r7_20, s32 *, 0xC) + 0x100000);
    M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r7_20, s32 *, 0x10);
    sp4 = &sp0 + 0x2C;
    var_r8_440 = 0x13;
    do {
        temp_r0_447 = Func_08096c80(0x11D, M2C_FIELD(&sp20, s32 *, 0), M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
        *sp4 = temp_r0_447;
        sp4 += 4;
        if (temp_r0_447 != NULL) {
            Func_08009098(temp_r0_447, 0x0809F0D4);
            temp_r0_457 = Func_08004458();
            M2C_FIELD(temp_r0_447, s32 *, 0x34) = 0x20000;
            M2C_FIELD(temp_r0_447, s32 *, 0x30) = (s32) (temp_r0_457 + 0x20000);
            M2C_FIELD(temp_r0_447, s8 *, 0x55) = 0;
            temp_r5_472 = (Func_08004458() * 0x18) + 0x80000;
            Func_08096bec(temp_r0_447, temp_r5_472, Func_08004458());
        }
        var_r8_440 -= 1;
    } while (var_r8_440 >= 0);
    Func_080f9010(0x83);
    Func_080090d0(sp14);
    Func_080090d0(sp18);
    Func_08009240(temp_r7_20, M2C_FIELD(temp_r3_16, u8 *, 0x44));
    Func_08009098(temp_r7_20, M2C_FIELD(temp_r3_16, s32 *, 0x3C));
    M2C_FIELD(temp_r7_20, s32 *, 0x6C) = (s32) M2C_FIELD(temp_r3_16, s32 *, 0x38);
    *sp10 = 3;
    M2C_FIELD(temp_r7_20, s32 *, 0x28) = 0xA0000;
    M2C_FIELD(temp_r7_20, s32 *, 0x44) = 0x3333;
    *spC = 0;
    M2C_FIELD(sp1C, s32 *, 0x6C) = 0;
    Func_08009240(sp1C, 0U);
    if ((s8) M2C_FIELD(temp_r3_16, u8 *, 0x34) != 0) {
        var_r8_528 = 0;
        if ((s32) M2C_FIELD(temp_r7_20, s32 *, 0x28) >= 0) {
loop_26:
            Func_080030f8(1);
            var_r8_528 += 1;
            if (var_r8_528 <= 0x59) {
                if ((s32) M2C_FIELD(temp_r7_20, s32 *, 0x28) >= 0) {
                    goto loop_26;
                }
            }
        }
        Func_080030f8(1);
        var_r8_547 = 0;
        if ((s32) M2C_FIELD(temp_r7_20, s32 *, 0x28) < 0) {
loop_29:
            Func_080030f8(1);
            var_r8_547 += 1;
            if (var_r8_547 <= 0x59) {
                if ((s32) M2C_FIELD(temp_r7_20, s32 *, 0x28) < 0) {
                    goto loop_29;
                }
            }
        }
        Func_0809a6b8(temp_r7_20);
        Func_0809748c();
        Func_080030f8(0x1E);
        return;
    }
    Func_0809748c();
}
