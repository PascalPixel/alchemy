/*
 * Resource 3a0 constant-argument request at 0x02000314 (16 bytes, 1 call).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000314, the only branch is the
 * `bl` at 0x0200031c and the walk stops at the interworking return
 * `pop {r0} / bx r0` at 0x02000320-0x02000322, so the executable extent is
 * exactly 0x02000314-0x02000323 (16 bytes).  The popped register is r0, so the
 * popped value is the return address and the owner is `void`.  The next
 * prologue begins immediately at 0x02000324 with no alignment halfword
 * between, which is why the span cannot be guessed from an interval boundary.
 *
 * Pool map: none — no literal is loaded.
 *
 * Nine overlays carry a prologue at 0x0314; five are one transposed 3-D
 * distance helper (see resource_385_c_02000314.c).  This is not one of them,
 * and it is not resource_3c0's 0x0314 either: same `push {lr}` shape, but
 * three constants to a different import instead of two.
 *
 * The one call site was resolved with
 * `bun tools/overlay_call_targets.ts resource_3a0 0314 0324`:
 * 0x0200031c -> veneer 0x020014a0 -> Func_0808a4f8.  `overlay_show`'s printed
 * 0x20017be is the pc-relative misreading and is not used.  All three
 * arguments are immediates, so this is a named shorthand for one fixed
 * request; the same import is called with a different constant triple
 * (8, 0, 0) elsewhere in the semantic tree.
 */

void Func_0808a4f8();

void Func_02000314(void)
{
    Func_0808a4f8(22, 1, 2);
}
