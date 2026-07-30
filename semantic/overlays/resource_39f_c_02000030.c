typedef signed int s32;

/*
 * resource_39f owner at 0x02000030 (60 bytes of code, 0x02000030-0x02000067,
 * followed by the single pool word 0x030001d8 at 0x02000068).
 *
 * Squared 3D distance between two 16.16 position triples, reduced to whole
 * units by an arithmetic shift, then passed through the relocated IWRAM square
 * root.  This file is the anchor for the overlay: the two conventions below
 * are stated once here and referred to from the other resource_39f sources.
 *
 * ---------------------------------------------------------------------------
 * 1. Link base 0x02008000, proven six times over.
 *
 * The six-entry veneer table at file offset 0 is `ldr r4,[pc,#0] / bx r4 /
 * .word <target|1>` and its words are 0x0200a501, 0x02008ee1, 0x02008f39,
 * 0x02008f41, 0x0200a4ad and 0x02008f35.  Subtracting 0x8000 and the Thumb bit
 * gives file offsets 0x2500, 0x0ee0, 0x0f38, 0x0f40, 0x24ac and 0x0f34; four of
 * those already carry byte-exact C under assets/code, and the other two are
 * inventoried function entries.  Data pool words behave the same way: the owner
 * at 0x020002a8 loads 0x0200aef8 and 0x0200af38, which are the in-image tables
 * at offsets 0x2ef8 and 0x2f38.  So `Data_0200bxxx`-style constants in this
 * overlay are in-image data, not RAM globals.
 *
 * ---------------------------------------------------------------------------
 * 2. An overlay `bl` stores its target as an image offset biased by -2, NOT as
 *    a pc-relative displacement.  Measured, and it resolves the overlay
 *    completely.
 *
 * Decoding the 723 `bl` sites in this overlay the ordinary way (target =
 * site + 4 + displacement) produces 689 distinct addresses, 453 of which lie
 * beyond the end of the image - the decoded stream is exactly 0x3e2c bytes,
 * per assets/code/resource_39f_stream.lz.json - while others land inside
 * literal tables.  Decoding the *stored displacement alone* produces only 74
 * distinct values, i.e. the same callee always carries the same two halfwords
 * no matter where it is called from, which a pc-relative branch cannot do.
 *
 * Adding 2 to the stored displacement resolves all 74:
 *
 *   - 59 land exactly on the 8-byte grid of the import veneer table at
 *     0x02002ce4-0x02002ebb (`ldr r4,[pc,#0] / bx r4 / .word <main-image
 *     address|1>`).  Their words are real main-image manifest row starts
 *     (0x0808a010, 0x08009080, 0x080770c0, 0x08015040, 0x080f9010, ...).
 *   - 1 lands on 0x02002ec8, the `bx r3` slot of this overlay's own
 *     `call_via rN` bank at 0x02002ebc-0x02002ef7 - and its single caller is
 *     this function, which loads 0x030001d8 into r3 first.  That is the
 *     HANDOVER call-via-r3 signature, independently confirming the mapping.
 *   - 14 land on in-image addresses, and every one of the 14 is a genuine
 *     `push {..., lr}` prologue.  There is therefore no hidden-context caller
 *     anywhere in resource_39f.
 *
 * A random mapping would not hit 59 exact veneer slots, the one thunk slot the
 * caller's own pool word predicts, and 14 prologues out of 14.  The same test
 * run over resource_371/372/373/38f/3b8/3bf/3c4/3c8 gives the same shape, so
 * this is an overlay-wide encoding, not a resource_39f quirk.  Practically it
 * means the loader fixes up each `bl` when the module is placed; the stored
 * form is the unrelocated addend.
 *
 * Consequence for these sources: callees are named by what they really are -
 * `Func_08xxxxxx` for an import (the main-image address its veneer jumps to,
 * Thumb bit stripped) and `Func_0200xxxx` for an intra-overlay call.  Imports
 * keep old-style declarations because one import is reached with different
 * argument counts from different owners.
 * ---------------------------------------------------------------------------
 *
 * The epilogue here is `pop {r5} / pop {r1} / bx r1`: r1, not r0, carries the
 * popped return address, so r0 survives and is the result.
 */

/*
 * The relocated ARM-mode IWRAM square root, reached through this overlay's
 * call-via-r3 veneer at 0x02002ec8.  r3 is loaded from the pool word at
 * 0x02000068 with 0x030001d8 immediately before the branch, so r3 is the
 * callee and never scratch.  r1 and r2 still hold the third and second squared
 * differences at the call; the helper takes one argument and ignores them.
 */
typedef s32 (*IwramHelper_02000030)(s32 value);
s32 Func_02002ec8(s32 value, s32 spare1, s32 spare2, IwramHelper_02000030 viaR3);

s32 Func_02000030(const s32 *here, const s32 *there)
{
    s32 dx;
    s32 dy;
    s32 dz;

    /* The two operands are walked with post-incrementing loads, so the
     * differences are formed in x, y, z order from three consecutive words of
     * each triple. */
    dx = (here[0] - there[0]) >> 16;
    dy = (here[1] - there[1]) >> 16;
    dz = (here[2] - there[2]) >> 16;

    /* `asrs` on each difference: the subtraction happens at 16.16 precision and
     * only then is reduced, so the squares below are in whole units.  Signed
     * shift, signed multiply, no saturation. */
    return Func_02002ec8(dx * dx + dy * dy + dz * dz, dz * dz, dy * dy,
                         (IwramHelper_02000030)0x030001d8);
}
