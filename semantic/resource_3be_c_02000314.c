#include "types.h"

/*
 * Resource 3be 3-D distance helper at 0x02000314 (56 bytes, 1 call).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000314 there is no branch of
 * any kind until the interworking return `pop {r5} / pop {r1} / bx r1` at
 * 0x02000346-0x0200034a, so the executable extent is exactly
 * 0x02000314-0x0200034b (56 bytes).  The walk never reaches 0x0200034c, which
 * is this owner's one-word literal pool:
 *
 *   0x0200034c  0x030001d8   the ARM-mode square root relocated into IWRAM
 *
 * That word is even, so by the overlay parity rule it is a data address rather
 * than a Thumb entry, and it is well below the 0x02008000 link base, so it is
 * an absolute IWRAM address and not an in-image offset.  The next prologue
 * begins at 0x02000350, and nothing between 0x0200034c and it belongs here.
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
 * overlay's own `call_via` slot (0x20015e8 here), which is the only thing
 * that can differ between transposed copies: the slot's image offset moves
 * with the overlay's import band.  The remaining four 0x0314 prologues are
 * unrelated routines (382 saves r8-sl and loads 0x03001e8c; 387 is a
 * two-import wrapper; 3a0 and 3c0 are small constant-argument calls).
 *
 * The single call site was resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3be 0314 034c`: one site,
 * classified `call_via`, and r3 is loaded from the pool with 0x030001d8
 * immediately before it.  So the callee is the IWRAM square root, the same
 * helper resource_3b7_c_02000e5c.c already names.
 *
 * The three coordinate differences are taken as 16.16 fixed point and reduced
 * to whole units by `asrs #16` before squaring, which is what keeps the sum
 * inside 32 bits; the pointers are walked with `ldmia rN!` in the interleaved
 * order a[0], b[0], a[1], b[1], b[2], a[2].
 */

s32 Func_030001d8();   /* IWRAM square root, reached through the call_via slot */

s32 Func_02000314(s32 *a, s32 *b)
{
    s32 dz = (a[2] - b[2]) >> 16;
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;

    return Func_030001d8(dx * dx + dy * dy + dz * dz);
}
