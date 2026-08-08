#include "types.h"

/*
 * Resource 3a0 3-D distance helper at 0x020003cc (56 bytes, 1 call).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020003cc there is no branch of
 * any kind until the interworking return `pop {r5} / pop {r1} / bx r1` at
 * 0x020003fe-0x0200034a, so the executable extent is exactly
 * 0x020003cc-0x02000403 (56 bytes).  The walk never reaches 0x02000404, which
 * is this owner's one-word literal pool:
 *
 *   0x02000404  0x030001d8   the ARM-mode square root relocated into IWRAM
 *
 * That word is even, so by the overlay parity rule it is a data address rather
 * than a Thumb entry, and it is well below the 0x02008000 link base, so it is
 * an absolute IWRAM address and not an in-image offset.  The next prologue
 * begins at 0x02000408, and nothing between 0x02000404 and it belongs here.
 *
 * The epilogue pops into r1, not r0 (`pop {r1} / bx r1`), so by the epilogue
 * rule r0 survives as the result: the owner returns the helper's value.
 *
 * THIS ROUTINE IS TRANSPOSED INTO SIX OVERLAYS.  Five of them carry it at
 * offset 0x0314 (resource_385, resource_39b, resource_3a6, resource_3b3,
 * resource_3be); this copy sits at 0x03cc instead, which is why the offset
 * alone does not find the family.  All six are byte-identical over the whole
 * 60 bytes, code and pool word alike, with a single four-byte difference — the
 * `bl` halfword pair, here at 0x020003fa:
 *
 *   resource_385:0314  01f0 e1f8      resource_3b3:0314  02f0 e3fd
 *   resource_39b:0314  02f0 13fb      resource_3be:0314  01f0 f3fa
 *   resource_3a6:0314  01f0 b7fe      resource_3a0:03cc  01f0 65fa
 *
 * Each of those resolves, under `target = stored_displacement + 2`, onto that
 * overlay's own `call_via` slot (0x020014cc here), which is the only thing
 * that can differ between transposed copies: the slot's image offset moves
 * with the overlay's import band.  resource_3a0's own 0x0314 is a different,
 * unrelated routine (a three-constant call to Func_0808a4f8), so the two
 * cannot be told apart by address family alone.
 *
 * The single call site was resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a0 03cc 0404`: one site,
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

s32 Func_020003cc(s32 *a, s32 *b)
{
    s32 dz = (a[2] - b[2]) >> 16;
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;

    return Func_030001d8(dx * dx + dy * dy + dz * dz);
}
