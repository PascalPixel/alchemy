#include "types.h"

/*
 * resource_3b0 owner at 0x0200007c, 38 bytes: a scripted per-object step that
 * turns the object to face one of two fixed angles when a sampled value picks
 * them out, and reports itself finished.
 *
 * Role known in advance, with no disassembly: the in-image script stream near
 * file offset 0x1900 holds the pair `(0x00000022, 0x0200807d)`, and under this
 * overlay's proven 0x02008000 link base 0x0200807d is file offset 0x007c plus
 * the Thumb bit — this function.  Its neighbour in the same stream is
 * `(0x00000022, 0x020080c1)`, i.e. Func_020000c0, converted alongside it, so
 * command 0x22 takes an in-overlay callback and both callbacks take the object
 * as their single argument and return a "done" flag.  Link-base evidence is in
 * the header of `semantic/overlays/resource_3b0_c_02000240.c`.
 *
 * Complete owner.  Prologue `push {r5, lr}` at 0x0200007c; single epilogue
 * `movs r0,#1 / pop {r5} / pop {r1} / bx r1` at 0x0200009a.  The popped
 * register is r1, not r0, so **r0 survives and IS the result** — the owner
 * returns 1.  Code runs 0x0200007c..0x020000a1; there is no literal pool, and
 * the 2-byte zero at 0x020000a2 is the alignment word in front of the next
 * prologue and belongs to nobody.  38 bytes, matching the inventory row.
 *
 * One call site, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3b0
 * 007c`: 0x02000080 -> veneer 0x11c4 -> Func_080000f8.  The same import the
 * root at 0x02000240 samples twice and Func_020000c0 samples four times.
 *
 * `lsls #6 / lsrs #16` is a WINDOWED extraction, not a shift: it keeps bits
 * 10..15 of the sample, i.e. a value in 0..63.  Reading it as `sample >> 10`
 * without the truncation would let values above 63 reach the comparisons.
 *
 * `movs r3,#0xd0 / lsls r3,r3,#8` is the additive/shifted-constant habit: the
 * stored value is 0xd000, not 0xd0.  The halfword at object + 6 is the facing
 * angle used across this family (the tracked `assets/code` sources and the
 * resource_3ae owners read the same field), so 0xd000 and 0xb000 are two fixed
 * headings.
 *
 * Uncertainty: Func_080000f8's result is treated as a sampled value here
 * because both consumers window it and compare it against small constants;
 * nothing in this overlay establishes what it samples.  When it is neither 6
 * nor 9 the facing is left alone — the owner still reports 1.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_02001246();            /* sampled value (veneer to Func_080000f8) */

s32 Func_0200007c(u8 *object)
{
    u32 sample = (u32)(Func_02001246() << 6) >> 16;   /* bits 10..15 */

    if (sample == 6) {
        s32 value = 0xd000;

        *(u16 *)(object + 6) = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        *(u16 *)(object + 6) = value;
    }

    return 1;
}
