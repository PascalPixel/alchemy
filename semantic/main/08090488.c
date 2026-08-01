#include "types.h"
/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and the
 * `bl 0x80072f0` at 0x080904e6 calls whatever r3 holds.
 *
 * r3 is loaded one instruction earlier, at 0x080904e4, from the pool word at
 * 0x0809055c: the constant 0x03000380.  That is the relocated ARM-mode IWRAM
 * signed divide, already read that way independently in four overlays
 * (resource_393, 39a, 3b2, 3b3) and named in main-regions.json's evidence for
 * 0x080935d4.  So the draft's fourth argument was the call target.
 *
 * ARITY IS TWO, NOT THREE, and that is the part worth checking rather than
 * assuming.  At the branch three argument registers happen to hold values:
 *
 *   r0  0x080904de  muls r0, r2   -- step_index * (field_0x53B - field_0x53A)
 *   r1  0x080904e2  ldrsb r1, [r4, #0]  -- field 0x53C, the step count
 *   r2  0x080904d2  subs r2, r2, r3     -- the range, set for the multiply
 *
 * r0 and r1 are written immediately before the call.  r2 is not: it was built
 * at 0x080904d2 to be the multiplier at 0x080904de and merely survives, and
 * every other caller of 0x03000380 in this tree passes exactly two.  Reading
 * r2's survival as a third argument is the welded-argument error, so the
 * range is dropped here.
 *
 * The reading it produces makes sense of the whole block: the result is added
 * to field 0x53A at 0x080904f0 and stored to field 0x52A, i.e.
 * base + step_index * range / steps -- a linear interpolation across the
 * animation, which is why the guard above it compares the step index against
 * the step count.
 */
typedef s32 (*IwramDivide)(s32 numerator, s32 denominator);
#define IWRAM_DIVIDE_08090488 ((IwramDivide)0x03000380)

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_08090488(void) {
    s16 *var_r2;
    s16 var_r3;
    s32 temp_r2_2;
    s8 temp_r2;
    s8 temp_r3;
    u16 temp_r5;
    void *temp_r6;
    void *temp_r7;

    temp_r6 = *(void **)0x03001ECC;
    temp_r2 = M2C_FIELD(temp_r6, s8 *, 0x53C);
    temp_r7 = *(void **)0x03001E70;
    if (temp_r2 != 0) {
        if ((s32) M2C_FIELD(temp_r6, s8 *, 0x53D) >= (s32) temp_r2) {
            M2C_FIELD(temp_r6, s8 *, 0x53C) = 0;
            Func_08004278((void *)0x08090489);
            Func_0800307c(1, 0, 0);
            return;
        }
        temp_r2_2 = M2C_FIELD(temp_r6, s8 *, 0x53B) - M2C_FIELD(temp_r6, s8 *, 0x53A);
        temp_r3 = (u8) M2C_FIELD(temp_r6, s8 *, 0x53D) + 1;
        M2C_FIELD(temp_r6, s8 *, 0x53D) = temp_r3;
        M2C_FIELD(temp_r6, u16 *, 0x52A) = (u16) (M2C_FIELD(temp_r6, s8 *, 0x53A) + IWRAM_DIVIDE_08090488(temp_r3 * temp_r2_2, (s32) M2C_FIELD(temp_r6, s8 *, 0x53C)));
        goto block_4;
    }
block_4:
    temp_r5 = M2C_FIELD(temp_r6, u16 *, 0x52A);
    if ((u32) temp_r5 > 0x4FU) {
        M2C_FIELD(temp_r7, s16 *, 0x100) = 0xC8;
        var_r2 = temp_r7 + 0x102;
        var_r3 = 0xFA;
        goto block_10;
    }
    if ((temp_r5 != 0) && (*(s32 *)0x03001E40 & 1)) {
        M2C_FIELD(temp_r7, s16 *, 0x100) = (s16) (temp_r5 + 0x50);
        M2C_FIELD(temp_r7, s16 *, 0x102) = (s16) (0x50 - temp_r5);
        return;
    }
    M2C_FIELD(temp_r7, s16 *, 0x100) = 0;
    var_r2 = temp_r7 + 0x102;
    var_r3 = 0x9F;
block_10:
    *var_r2 = var_r3;
}
