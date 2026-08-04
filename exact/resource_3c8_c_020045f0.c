/*
 * Resource 3c8 fixed-argument shorthand at 0x020045f0 (16 bytes, 1 call).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020045f0 the only branch is the
 * `bl` at 0x020045f8, and the walk stops at the interworking return
 * `pop {r0} / bx r0` at 0x020045fc-0x020045fe.  So the executable extent is
 * exactly 0x020045f0-0x020045ff (16 bytes).  The popped register is r0, so the
 * popped value is the return address and the owner is `void`.
 *
 * Pool map: none.  Both arguments are built with `movs` + `lsls`, and the next
 * prologue follows immediately at 0x02004600 with no alignment halfword — so
 * the boundary cannot be taken from an interval start here.
 *
 * The one call site was resolved with
 * `bun tools/overlay_call_targets.ts resource_3c8 45f0 4600`: 0x020045f8
 * classifies as a prologue at image offset 0x4520, i.e. the in-overlay
 * Func_02004520.  `overlay_show`'s printed 0x2008b1a is the pc-relative
 * misreading and is not used.
 *
 * `movs r0,#216 / lsls r0,#1` builds 432 as a VALUE, not a displacement — it
 * is passed straight to the callee.  The immediately following owner at
 * 0x02004600 is the same three instructions with 560 and 44, so this is one of
 * a run of named constant pairs rather than a computation.
 */

void Func_02008b1a();

void Func_020045f0(void)
{
    Func_02008b1a(432, 32);
}
