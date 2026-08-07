#include "types.h"

/*
 * resource_3b0 owner at 0x020000c0, 192 bytes: a scripted per-object drift
 * step — advance two independent axes by a sampled amount, bounce each one at
 * its limits, and move the object along X only while it is inside the allowed
 * band.
 *
 * Role known in advance, with no disassembly: the in-image script stream near
 * file offset 0x1900 holds the pair `(0x00000022, 0x020080c1)`, and under this
 * overlay's proven 0x02008000 link base 0x020080c1 is file offset 0x00c0 plus
 * the Thumb bit — this function.  Its neighbour `(0x00000022, 0x0200807d)` is
 * Func_0200007c, converted alongside it.  Link-base evidence is in the header
 * of `semantic/overlays/resource_3b0_c_02000240.c`.
 *
 * Object layout is taken from the tracked byte-exact sibling
 * `assets/code/resource_3b0_c_02000030.c`, which types the same record: +8 is
 * the s32 X coordinate and +12 the s32 Z coordinate.  That source reproduces
 * the ROM, so the two field offsets are evidence rather than inference.  The
 * three further fields this owner touches — the s32 at +76 and the two s16
 * direction states at +100 and +102 — are named from their use here.
 *
 * Complete owner.  Prologue `push {r5, r6, lr}` at 0x020000c0; single epilogue
 * `movs r0,#1 / pop {r5, r6} / pop {r1} / bx r1` at 0x0200016c.  The popped
 * register is r1, not r0, so **r0 survives and IS the result** — the owner
 * returns 1.  Code runs 0x020000c0..0x02000173; the literal pool is
 * 0x02000174..0x0200017f (three words: 0xffff4000, 0xffd7ffff, 0x0117fffe) and
 * 0x02000180 is the next prologue.  192 bytes, matching the inventory row.
 *
 * Four call sites, resolved with `bun tools/lib/overlay_call_targets.ts
 * resource_3b0 00c0`: 0x020000da, 0x020000f2, 0x02000134 and 0x0200014e, all
 * to veneer 0x11c4 -> Func_080000f8.  They are four DISTINCT sites on four
 * distinct arms and are not merged; the two arms of each axis are near-twins
 * differing only in the sign of the step and in the limit tested, which is
 * exactly the shape HANDOVER warns against folding.
 *
 * THE BAND GUARD is the documented family idiom: `ldr r2,[r5,#8] /
 * adds r3, r2, 0xffd7ffff / cmp r3, 0x0117fffe / bhi` is an unsigned half-open
 * range test on a wrapped position word, not a mask and not a sign trick.  X
 * advances only while it passes; the Z axis has no such guard.
 *
 * The sampled steps are windowed, not shifted: `lsls #11 / lsrs #16` keeps bits
 * 5..15 of the sample for the X velocity, and `sample * 3` then `lsls #14 /
 * lsrs #16` keeps bits 2..15 of three times the sample for the Z step.
 * Dropping the truncation lets values far outside the intended range through.
 *
 * Direction state, per axis: 9 means "stopped" and zeroes the axis outright; 0
 * means "advancing" and any other value means "retreating".  Each arm writes
 * the state back as 0 or 1 when its limit is crossed, so the object bounces
 * between the two.  The X limits are -0x0000c000 (as the signed pool word
 * 0xffff4000) and +0x0000c000; the Z limits are 0 and 0x00100000 (128 << 13).
 * The asymmetry between the two axes' lower bounds is what the reference does.
 *
 * Uncertainty: Func_080000f8's result is treated as a sampled value, as in
 * Func_0200007c; nothing in this overlay establishes what it samples.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_080000f8();            /* sampled value */

s32 Func_020000c0(u8 *object)
{
    s16 *xState = (s16 *)(object + 100);
    s16 *zState = (s16 *)(object + 102);
    s32 x;
    s32 z;

    if (*xState == 9) {
    } else if (*xState != 0) {
        x = *(s32 *)(object + 76) - (s32)((u32)(Func_080000f8() << 11) >> 16);
        *(s32 *)(object + 76) = x;
        *(s32 *)(object + 76) = 0;
        if (x < (s32)0xffff4000) {
            *xState = 0;
        }
    } else {
        x = *(s32 *)(object + 76) + (s32)((u32)(Func_080000f8() << 11) >> 16);
        *(s32 *)(object + 76) = x;
        if (x > 0xc000) {                       /* 192 << 8 */
            *xState = 1;
        }
    }

    /* Band guard: only move while the wrapped X word is inside the window. */
    if ((u32)(*(s32 *)(object + 8) + 0xffd7ffff) <= 0x0117fffe) {
        *(s32 *)(object + 8) += *(s32 *)(object + 76);
    }

    if (*zState == 9) {
        *(s32 *)(object + 12) = 0;
    } else if (*zState != 0) {
        z = *(s32 *)(object + 12) -
            (s32)((u32)((Func_080000f8() * 3) << 14) >> 16);
        *(s32 *)(object + 12) = z;
        if (z < 0) {
            *zState = 0;
        }
    } else {
        z = *(s32 *)(object + 12) +
            (s32)((u32)((Func_080000f8() * 3) << 14) >> 16);
        *(s32 *)(object + 12) = z;
        if (z > 0x00100000) {                   /* 128 << 13 */
            *zState = 1;
        }
    }

    return 1;
}
