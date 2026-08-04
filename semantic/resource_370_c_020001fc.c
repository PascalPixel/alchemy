#include "types.h"

/*
 * Resource 370 cheat-code watcher at 0x020001fc.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020001fc through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020002ee.  The return address is
 * popped into r0, so the owner is `void` (HANDOVER section 0, epilogue rule).
 *
 * Two literal pools sit inside the span and are skipped by the control-flow
 * walk from the prologue: 0x0200025a-0x02000273 and 0x020002d2-0x020002eb.
 * The first instruction after each pool (0x02000274 / 0x020002ec) is a real
 * branch target and is code.  `overlay_show.ts`'s "pool words referenced"
 * listing agrees here, but the walk is what settles it.
 *
 * Both call sites -- 0x02000254 and 0x020002cc -- resolve through the SAME
 * veneer at file offset 0x14cc to Func_080f9010
 * (`bun tools/overlay_call_targets.ts resource_370 --json`), even though the
 * two `bl` encodings differ.  That is the overlay `bl` rule in action: the
 * printed targets 0x02001722 and 0x0200179a are meaningless.  Two sites, two C
 * call expressions, matching the inventory's calls=2.
 *
 * The body is one routine written twice, once per channel, with a different
 * set of in-image state words.  Under this overlay's confirmed 0x02008000 link
 * base (see semantic/overlays/resource_370_c_02000154.c, where the pool word
 * 0x02008155 is that owner's own entry plus the Thumb bit) every one of those
 * words is in-image data:
 *
 *   channel    gate        held        position    sequence
 *   A          0x020096b2  0x020096ba  0x020096b6  0x020094d4
 *   B          0x020096b4  0x020096bc  0x020096b8  0x020094dc
 *
 * 0x03001ae8 is the current button mask in IWRAM, read as a full word.  The
 * sequences confirm the reading directly: channel A's table at file offset
 * 0x14d4 is {0x0004, 0x0004, 0x0004, 0x0000} -- SELECT three times -- and
 * channel B's at 0x14dc begins {0x0040, 0x0080, 0x0040, 0x0080, 0x0020,
 * 0x0010, 0x0020, 0x0010, ...}, i.e. up, down, up, down, left, right, left,
 * right.  Each table is terminated by a zero entry.
 *
 * Per channel and per frame: do nothing once the channel has fired; while a
 * previously matched press is still held, wait for the release; otherwise, on
 * a fresh non-empty button mask, either advance one step when the mask equals
 * the expected entry exactly or reset to the start.  Reaching the table's zero
 * terminator latches the gate and notifies Func_080f9010 with code 110.
 *
 * Uncertainties: the comparison is against the whole button word, so the
 * expected entry must match exactly and any extra button held breaks the
 * sequence; and Func_080f9010's interface is unknown beyond the single
 * argument 110 that both sites pass.
 */

/* Overlay import (via the veneer table at file offset 0x14cc).  Old-style
 * declaration: overlay imports vary their argument count between sites. */
void Func_080f9010();

/*
 * One channel's frame step.  Returns nonzero exactly when the channel's
 * sequence has just been completed; the caller then latches the gate and
 * notifies.  The gate write and the notification are kept in the caller so
 * that this file has the same two Func_080f9010 call expressions the assembly
 * has call sites.
 */
static s32 Step_020001fc(const s16 *gate, s16 *held, s16 *position,
                         const u16 *sequence)
{
    u32 buttons;

    if (*gate != 0) {
        return 0;
    }

    if (*held != 0) {
        /* Wait for every button to be released before accepting a new step. */
        if (*(u32 *)0x03001ae8 != 0) {
            return 0;
        }
        *held = 0;
        return 0;
    }

    buttons = *(u32 *)0x03001ae8;
    if (buttons == 0) {
        return 0;
    }

    /* The mask is re-read here, exactly as the assembly does. */
    if (*(u32 *)0x03001ae8 != (u32)sequence[*position]) {
        *position = 0;
        return 0;
    }

    *position = (s16)(*position + 1);
    *held = 1;

    /* A zero entry terminates the sequence. */
    return sequence[*position] == 0;
}

void Func_020001fc(void)
{
    if (Step_020001fc((const s16 *)0x020096b2, (s16 *)0x020096ba,
                      (s16 *)0x020096b6, (const u16 *)0x020094d4) != 0) {
        *(s16 *)0x020096b2 = 1;
        Func_080f9010(110);
    }

    if (Step_020001fc((const s16 *)0x020096b4, (s16 *)0x020096bc,
                      (s16 *)0x020096b8, (const u16 *)0x020094dc) != 0) {
        *(s16 *)0x020096b4 = 1;
        Func_080f9010(110);
    }
}
