/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags.  Two things were
 * needed: the callee had been declared at its ROM-space veneer address, so the
 * `bl' went through a veneer instead of straight to 0x02009ca8; and the
 * reference hoists THREE loop-invariant constants (28, 8, 0) into r5/r6/r7,
 * the zero being an OR identity that folds away unless it is written as its
 * own local.  Initialising the three in reference order -- index, bit, zero --
 * is what fixes the `movs' sequence; a for-init of index emits it last.
 *
 * Span is 34 bytes of code; the halfword at 0x020037d6 is alignment.
 */
#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x020037b4 (36 bytes, 5 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` at 0x020037b4, epilogue `pop {r5,r6,r7} / pop {r0} / bx
 * r0` at 0x020037d0-0x020037d4; no pool needed, the halfword at 0x20037d6
 * is the alignment pad, immediately followed by the next owner's push at
 * 0x020037d8 (already this overlay's row `0x020037d8 | 4 calls`), so the
 * span is 0x020037b4-0x020037d8, 36 bytes.
 *
 * A flat sweep, index 28..35 inclusive (`cmp r5,#35; bls`), setting bit 3
 * of each object's `+0x59` flags byte.
 */

u8 *Func_02009ca8();

void Func_020037b4(void)
{
    u32 index;
    u32 bit;
    u32 zero;

    index = 28;
    bit = 8;
    zero = 0;
    for (; index <= 35; index++) {
        u8 *obj = Func_02009ca8(index);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}
