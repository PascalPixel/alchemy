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
 * Link base: resource_395 is linked at 0x02008000, proven here by eleven pool
 * words that are this overlay's own prologues plus the Thumb bit (0x02008089 =
 * Func_02000088 + 1, 0x02008159, 0x02008249, 0x02008285, 0x020082c1,
 * 0x02009071, 0x020091e9, 0x02009219, 0x020092b5 = this owner + 1, 0x0200952d,
 * 0x020095a1) — all installed per-frame callbacks.  So this owner is itself
 * installed as a task.
 *
 * Under that base 0x02009dd4 is offset 0x1dd4, which is 0x18 bytes PAST the
 * end of the overlay image (0x1dbc).  It is therefore not image data: it is
 * working RAM the loader leaves immediately after the loaded overlay.  Five
 * such words are used across the overlay (0x02009dc0, 0x02009dcc, 0x02009dd0,
 * 0x02009dd4, 0x02009de0, plus 0x0200a4e0 further out); they are uninitialised
 * scratch, so nothing here may assume a starting value.
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
