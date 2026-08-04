#include "types.h"

/*
 * Resource 385 3-D distance helper at 0x02000314 (60 bytes, 1 call).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000314 there is no branch of
 * any kind until the interworking return `pop {r5} / pop {r1} / bx r1` at
 * 0x02000346-0x0200034a.  Its ownership continues through the one-word
 * literal pool at 0x0200034c, making the complete span 60 bytes:
 *
 *   0x0200034c  0x030001d8   the ARM-mode square root relocated into IWRAM
 *
 * That word is even, so by the overlay parity rule it is a data address rather
 * than a Thumb entry, and it is well below the 0x02008000 link base, so it is
 * an absolute IWRAM address and not an in-image offset.  The next prologue
 * begins immediately at 0x02000350.  Keeping this referenced pool with its
 * owner avoids manufacturing a four-byte semantic gap.
 *
 * The epilogue pops into r1, not r0 (`pop {r1} / bx r1`), so by the epilogue
 * rule r0 survives as the result: the owner returns the helper's value.
 *
 * NINE OVERLAYS CARRY A PROLOGUE AT 0x0314 AND FIVE OF THEM ARE THIS ROUTINE.
 * resource_385, resource_39b, resource_3a6, resource_3b3 and resource_3be are
 * byte-identical over the whole 60 bytes 0x0314-0x034f — code and pool word
 * alike — with a single four-byte difference, the `bl` halfword pair at
 * 0x02000342:
 *
 *   resource_385  01f0 e1f8      resource_3b3  02f0 e3fd
 *   resource_39b  02f0 13fb      resource_3be  01f0 f3fa
 *   resource_3a6  01f0 b7fe
 *
 * Each of those resolves, under `target = stored_displacement + 2`, onto that
 * overlay's own `call_via` slot (0x20011c4 here), which is the only thing
 * that can differ between transposed copies: the slot's image offset moves
 * with the overlay's import band.  The remaining four 0x0314 prologues are
 * unrelated routines (382 saves r8-sl and loads 0x03001e8c; 387 is a
 * two-import wrapper; 3a0 and 3c0 are small constant-argument calls).
 *
 * The single call site was resolved directly against the raw region in
 * assets/code/resource_385_overlay.s (still unadopted there at the time of
 * writing): `bl sub_02001508` immediately after `ldr r3, [pc, #8]` loads
 * 0x030001d8.  sub_02001508 is this overlay's own `_call_via_r3` veneer --
 * NOT the 0x20011c4 address the transposed-family comment above would
 * suggest by `stored_displacement + 2` veneer math, which the raw source
 * contradicts (see LAWS.md's "Func_080072xx is usually not a function"
 * addendum and the src/0800dcdc.c veneer-audit precedent for this same
 * 0x030001d8 target).  The veneer is called directly with the target address
 * as the trailing argument that the ABI forces into r3, rather than through a
 * function-pointer spelling (which the Thumb `call_indirect` pattern cannot
 * reproduce byte-for-byte).
 *
 * The three coordinate differences are taken as 16.16 fixed point and reduced
 * to whole units by `asrs #16` before squaring, which is what keeps the sum
 * inside 32 bits; the pointers are walked with post-increment `*p++` (which
 * is what makes gcc emit `ldmia rN!, {rX}`) in the interleaved order
 * a[0], b[0], a[1], b[1], b[2], a[2] -- the same shape documented for the
 * sibling 0x0030 distance family in LAWS.md.
 */

s32 Func_02001508(s32 a, s32 b, s32 c, s32 target);   /* _call_via_r3 veneer */

s32 Func_02000314(s32 *arg0, s32 *arg1)
{
    s32 temp_r5 = (*arg0++ - *arg1++) >> 16;
    s32 temp_r4 = (*arg0++ - *arg1++) >> 16;
    s32 temp_r3 = (*arg0 - *arg1) >> 16;

    s32 temp_r0 = temp_r5 * temp_r5;
    s32 temp_r2 = temp_r4 * temp_r4;
    s32 temp_r1 = temp_r3 * temp_r3;

    return Func_02001508(temp_r0 + temp_r2 + temp_r1, temp_r1, temp_r2, 0x030001d8);
}
