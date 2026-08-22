/*
 * resource_3a7 "claim the entity occupying the actor's tile" predicate at 0x02000944,
 * 124 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000944 through `pop {r5, r6,
 * r7} / pop {r1} / bx r1` at 0x020009b0.  The popped register is r1, NOT r0, so
 * r0 survives the return and IS the result: the owner returns s32 (1 when a
 * candidate was claimed, 0 otherwise).  The words at 0x020009b8 (0x000ffffe)
 * and 0x020009bc (0x000fffff) are the literal pool, past the epilogue and
 * inside the span; they are data, not code.
 *
 * r0 is read before it is written, so the owner takes one argument: the
 * selector of the subject entity.
 *
 * Call targets resolved with
 *   bun tools/overlay-show resource_3a7 0944 -n 124 |
 *   cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a7 --annotate
 * Per-target multiset over the 2 sites, matching the inventory's calls=2:
 *   Func_0808a080 x2 <- veneer 0x02001130  (once for the subject, once per
 *                       candidate inside the loop)
 *
 * `>> 20` on the 16.16 coordinates at +8 and +16 is the tile-grid idiom, and
 * the `if (v < 0) v += 0xfffff` before each shift is the round-toward-zero bias,
 * exactly as in this overlay's 0x02000458 predicate.  Written here as one
 * truncating division by 0x100000.
 *
 * The `(u32)(record[3] - 1) > 0x000ffffe` test is an unsigned range check: it
 * accepts +12 values in 1..0x000fffff and rejects 0 and anything at or above
 * 0x00100000.  Since the sibling owners at 0x02000c08/0x02000c50 SET +12 to
 * 0x00ff0000, the check is "this candidate has not been claimed yet", and the
 * store below marks it claimed.
 *
 * Uncertainty, recorded rather than "fixed": r4 holds the candidate's Z cell
 * across three instructions but is never saved by the prologue.  This build
 * compiles with `-fcall-used-r4`, and the documented overlay idiom is that r4 is
 * used as call-clobbered scratch without being saved; nothing observable depends
 * on it here because no call intervenes between its write and its last read.
 *
 * Uncertainty: which of +8/+16 is X and which is Z follows the overlay's
 * convention rather than proof; the predicate is symmetric in the two tests, so
 * nothing depends on the naming.
 */

/* Old-style declaration: overlay imports vary their argument count between
 * call sites in this overlay. */
#include "types.h"

/* The overlay import table has two physical veneer slots for this logical
 * accessor.  Naming the slots separately lets the normal linker encode the
 * overlay's target-offset BL words without changing the call ABI. */
s32 *Func_02001a78();   /* subject selector veneer */
s32 *Func_02001a84();   /* candidate selector veneer */

/* Truncating >> 20: whole pixels, then the 16-pixel tile grid. */
static s32 TileCell(s32 fixed)
{
    if (fixed < 0) {
        fixed += 0x000fffff;
    }
    return fixed >> 20;
}

s32 Func_02000944(s32 subjectSelector)
{
    s32 *subject = Func_02001a78(subjectSelector);
    s32 index = 0;

    do {
        s32 *candidate = Func_02001a84(index + 11);

        /* Unclaimed: +12 in 1..0x000fffff. */
        if ((u32)(candidate[3] - 1) <= 0x000ffffe) {
            s32 candidateZ = TileCell(candidate[4]);   /* +16 */
            s32 candidateX = TileCell(candidate[2]);   /* +8  */
            s32 subjectZ = TileCell(subject[4]);
            s32 subjectX = TileCell(subject[2]);

            s32 zDelta = subjectZ - candidateZ;
            s32 xDelta = subjectX - candidateX;
            if (xDelta != 0 || zDelta != 0) {
                goto next_candidate;
            }
            candidate[3] = 0x00ff0000;   /* +12: mark claimed */
            candidate[18] = 0;           /* +72 */
            candidate[10] = 0;           /* +40 */
            return 1;
        }

    next_candidate:
        index++;
    } while (index <= 3);

    return 0;
}
