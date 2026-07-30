typedef signed int s32;

/*
 * Resource 395 per-frame step driver at 0x020012b4 (64 bytes,
 * 0x020012b4-0x020012f3).
 *
 * Complete owner: `push {r5, lr}` prologue at 0x020012b4 and the matching
 * `pop {r5} / pop {r0} / bx r0` interworking return at 0x020012ea.  The popped
 * return address lands in r0, so nothing is returned and the owner is `void`.
 * The single pool word (0x02009dd4) sits at 0x020012f0, after the epilogue.
 *
 * Link base: resource_395's pool words follow the proven 0x02008000 overlay
 * link base, so 0x02009dd4 is IN-IMAGE data at file offset 0x1dd4 — past the
 * import band's veneer table (which runs 0x1900-0x1ab7) and inside the
 * overlay's own data.  The overlay image is writable EWRAM, so this is an
 * ordinary mutable counter, not ROM.  It is not modelled `const`.
 *
 * Behaviour: a frame counter that cycles 0..29 and fires two cues on the way.
 * The counter is re-read at 0x020012dc after the callees run, so a callee that
 * changed it would be respected; that reload is preserved below.
 *
 * All four call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), never from overlay_show.ts's
 * `bl` annotations.  Placed multiset, matching the four sites exactly:
 * Func_020018b8 x2, Func_0808a348 x2.  The inventory reports calls=4.
 * Func_020018b8 is an ordinary prologue in this overlay, called ten times in
 * all; here it takes a single 0/1 selector.
 *
 * Uncertainty: what Func_0808a348's argument means is not established.  Its
 * banked uses elsewhere are small counts (1, 40, 60, 120) in the same position,
 * so the 20/8 here are treated as plain arguments and nothing is inferred.
 */

void Func_020018b8();           /* overlay-local; takes a 0/1 selector here */
void Func_0808a348();           /* scene-effect request; interface not established */

void Func_020012b4(void)
{
    s32 *step = (s32 *)0x02009dd4;
    s32 value;

    value = *step;
    if (value == 0) {
        Func_020018b8(0);
        Func_0808a348(20);
    } else if (value == 20) {
        Func_020018b8(1);
        Func_0808a348(8);
    }

    value = *step + 1;
    *step = value;
    if (value == 30) {
        *step = 0;
    }
}
