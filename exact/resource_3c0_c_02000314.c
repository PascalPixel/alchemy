/*
 * Resource 3c0 constant-argument request at 0x02000314 (14 bytes, 1 call).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000314, the only branch is the
 * `bl` at 0x0200031a and the walk stops at the interworking return
 * `pop {r0} / bx r0` at 0x0200031e-0x02000320, so the executable extent is
 * exactly 0x02000314-0x02000321 (14 bytes).  The popped register is r0, so the
 * popped value is the return address and the owner is `void`.
 *
 * Pool map: none — no literal is loaded.  The halfword at 0x02000322 is
 * `0x0000` alignment before the next prologue at 0x02000324
 * (`push {r5, lr} / movs r1,#0 / adds r5,r0,#0`), and the walk never reaches
 * it as an instruction.
 *
 * Nine overlays carry a prologue at 0x0314; five are one transposed 3-D
 * distance helper (see resource_385_c_02000314.c).  This is not one of them —
 * it saves no register but lr, loads two small constants and tail-calls a
 * single import — and it is not the same routine as resource_3a0's 0x0314
 * either, which passes three constants to a different import.
 *
 * The one call site was resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c0 0314 0322`:
 * 0x0200031a -> veneer 0x02001344 -> Func_02001660.  `overlay_show`'s printed
 * 0x2001660 is the pc-relative misreading and is not used.  Both arguments are
 * immediates, so there is no dataflow to recover: this is a named shorthand
 * for one fixed request, the same two-argument import the resource_371 owners
 * at 0x0200008c and 0x020000d4 call as `Func_02001660(0x809, 42)` and
 * `Func_02001660(0x80a, 24)`.
 */

void Func_02001660();

void Func_02000314(void)
{
    Func_02001660(15, 45);
}
