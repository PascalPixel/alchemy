/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * This draft had modelled the callee as a fourth ARGUMENT (and as an
 * `extern` data symbol at 0x03001388).  It was never data and never an
 * argument: it is the call target.
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002df0(void *);
void Func_080030f8(u32);
u8 *Func_08004970(s32);
s32 Func_080063bc(void *, s32);
void Func_08006458(void);
s32 Func_08077000(s32);
s32 Func_08077008(s32);
s32 Func_080b6a60(u16 *);

void Func_080b5f0c(void) {
    u8 *temp_r6;
    u8 *temp_r6_2;
    u8 *var_r2;
    s32 temp_r0;
    u8 *temp_r3;
    s32 var_r1;
    s32 var_r5;
    s32 var_r5_2;
    s8 *var_r3;
    u16 ids[8];

    temp_r3 = *(u8 **)0x03001E74;
    temp_r6 = Func_08004970(0x154);
    var_r5 = 7;
    var_r3 = temp_r3 + 0x4F;
    do {
        var_r5 -= 1;
        *var_r3 = 0xFF;
        var_r3 -= 1;
    } while (var_r5 >= 0);
    temp_r0 = Func_080b6a60(ids);
    var_r5_2 = 0;
    if (temp_r0 > 0) {
loop_4:
        ((WordCopy)0x03001388)((void *)temp_r6,
            (const void *)Func_08077008(ids[var_r5_2]), 0x154);
        M2C_FIELD(temp_r6, s8 *, 0x12A) = 2;
        M2C_FIELD(temp_r3, s8 *, ids[var_r5_2] + 0x48) =
            (s8)(var_r5_2 - 0x80);
        if (Func_080063bc(temp_r6, 0x154) != -1) {
            Func_08006458();
            var_r5_2 += 1;
            Func_080030f8(2U);
            if (var_r5_2 < temp_r0) {
                goto loop_4;
            }
        }
    }
loop_8:
    if (var_r5_2 <= 2) {
        M2C_FIELD(temp_r6, s8 *, 0x12A) = 0;
        if (Func_080063bc(temp_r6, 0x154) != -1) {
            Func_08006458();
            Func_080030f8(2U);
            var_r5_2 += 1;
            goto loop_8;
        }
    }
    Func_08002df0(temp_r6);
    temp_r6_2 = Func_08004970(0x140);
    ((WordCopy)0x03001388)((void *)temp_r6_2,
        (const void *)Func_08077000(0), 0x140);
    var_r1 = 0;
    if ((s32) M2C_FIELD(temp_r6_2, s32 *, 0x108) > 0) {
        var_r2 = temp_r6_2 + 8;
        do {
            M2C_FIELD(var_r2, u8 *, 2) = (u8) *(temp_r3 + (M2C_FIELD(var_r2, u8 *, 2) + 0x48));
            var_r1 += 1;
            var_r2 += 4;
        } while (var_r1 < (s32) M2C_FIELD(temp_r6_2, s32 *, 0x108));
    }
    if (Func_080063bc(temp_r6_2, 0x140) != -1) {
        Func_08006458();
        Func_080030f8(1U);
        Func_080030f8(2U);
    }
    Func_08002df0(temp_r6_2);
}
