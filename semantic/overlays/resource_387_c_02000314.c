typedef signed int s32;

/*
 * Resource 387 request-then-wait wrapper at 0x02000314 (22 bytes, 2 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000314 there is no branch other
 * than the two `bl`s until the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000324-0x02000328, so the executable
 * extent is exactly 0x02000314-0x02000329 (22 bytes).  The popped register is
 * r0, so by the epilogue rule the popped value is the return address and the
 * owner is `void`.
 *
 * Pool map: none.  This owner loads no literal.  The halfword at 0x0200032a is
 * `0x0000`, the two bytes of alignment before the next owner at 0x0200032c
 * (`ldr r0,[pc,#0] / bx lr`, an accessor returning the pool word 0x020092f8);
 * the walk never reaches it as an instruction.
 *
 * Nine overlays carry a prologue at 0x0314 and five of them are one
 * transposed 3-D distance helper (see resource_385_c_02000314.c for the
 * byte-diff).  This is NOT one of them: 387's prologue saves only r5, its body
 * is two imports rather than a call_via, and it shares no bytes with that
 * family beyond the `push {r5, lr}` halfword itself.
 *
 * Both call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_387 0314 032a`, assigning the
 * i-th occurrence to the i-th site in address order:
 *   0x0200031a -> veneer 0x020011cc -> Func_0808a180
 *   0x02000320 -> veneer 0x02001124 -> Func_0808a010
 * `overlay_show`'s printed targets (0x20014e8, 0x2001446) are the pc-relative
 * misreading and are not used.
 *
 * The argument shuffle is the whole point of the owner: r1 is saved into r5
 * *before* r1 is zeroed, so the second argument is carried across the first
 * call and delivered to the second.  All twelve call sites in this overlay
 * pass a small selector in r0 and a frame count in r1 — 0x0200099a passes
 * (1, 20) — which is the established `Func_0808a010` "wait this many frames"
 * shape, here bracketed by a `Func_0808a180(selector, 0)` request of the same
 * family the resource_371 sources call as `Func_0808a180(8, 0)`.
 */

void Func_0808a180();
void Func_0808a010();          /* wait this many frames */

void Func_02000314(s32 selector, s32 frames)
{
    Func_0808a180(selector, 0);
    Func_0808a010(frames);
}
