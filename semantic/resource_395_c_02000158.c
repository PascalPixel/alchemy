#include "types.h"

/*
 * Resource 395 scripted scene at 0x02000158 (240 bytes,
 * 0x02000158-0x02000247).
 *
 * Complete owner: `push {lr}` prologue at 0x02000158 and the matching
 * `pop {r0} / bx r0` interworking return at 0x02000224.  The popped return
 * address lands in r0, so nothing is returned and the owner is `void`.
 * 0x02000228-0x02000247 is this owner's eight-word literal pool, after the
 * epilogue and never reached as code.  Under the proven 0x02008000 overlay
 * link base none of those eight words is an in-image address: six are small
 * ids (flags 0x845/0x844, lines 0x151c/0x14eb/0x14c9, amount 0x00406218),
 * 0x0000200e is an action id, and 0x03001ebc is RAM below the image — the same
 * workspace pointer the byte-exact assets/code/resource_395_c_02000050.c
 * dereferences.
 *
 * All 24 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), never from overlay_show.ts's
 * `bl` annotations, which are off by one veneer entry on this overlay.  Placed
 * multiset, matching the 24 sites exactly: Func_020012f4 x4, Func_0808a170 x3,
 * Func_0808a180 x4, Func_080770c0 x2, Func_0808a330 x2, Func_0808a348 x2,
 * Func_080000c0 x2, and one each of Func_0808a018, Func_08077040,
 * Func_0808a188, Func_0808a138, Func_0808a020.  The inventory reports
 * calls=24, which agrees here.
 *
 * The Func_0808a020 at 0x02000220 is a SHARED tail: four paths reach it, three
 * by `b.n` and one by falling through.  It is spelled once through the `close`
 * label so the multiset stays exact.
 *
 * Behaviour: three variants selected by two progress flags.  0x845 gives the
 * short acknowledgement; 0x844 gives a second line and then, if the subject at
 * id 184 is available, writes 1 into the workspace halfword at +370; with
 * neither flag set the full sequence runs.
 *
 * Uncertainties:
 *  - the first argument of Func_0808a180/Func_0808a188 is 8 on the flagged
 *    arms but the pooled 0x200e on the long arm.  Both spellings are preserved
 *    literally; that the argument is a small slot index is therefore NOT
 *    asserted here.
 *  - `movs r1,#185 / lsls r1,#1` builds the byte offset 370 into the workspace
 *    record.  Its field is written as a u16, matching the `strh`.
 *  - Func_0808a330's first argument is a 16.16 amount (0x00406218 then
 *    0x00010000); what it scales is not established.
 */

void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_080000c0();           /* wait n frames */
s32 Func_080770c0();            /* story-flag test; used in a condition */
s32 Func_08077040();            /* acquire the subject; -1 when unavailable */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the action to finish */
void Func_0808a188();           /* as Func_0808a180, with a duration */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a330();           /* scene amount (16.16) with a mode */
void Func_0808a348();           /* scene-effect request */
void Func_020012f4();           /* overlay-local channel state (channel, state) */

void Func_02000158(void)
{
    Func_0808a018();

    if (Func_080770c0(0x845) != 0) {
        Func_020012f4(10, 1);
        Func_0808a170(0x151c);
        Func_0808a180(8, 0);
        Func_020012f4(10, 0);
        goto close;
    }

    if (Func_080770c0(0x844) != 0) {
        Func_020012f4(10, 1);
        Func_0808a170(0x14eb);
        Func_0808a180(8, 0);
        Func_020012f4(10, 0);

        if (Func_08077040(184) == -1) goto close;

        *(u16 *)(*(u8 **)0x03001ebc + 370) = 1;
        goto close;
    }

    Func_0808a170(0x14c9);
    Func_0808a180(8, 0);

    Func_0808a330(0x00406218, 1);
    Func_0808a348(20);
    Func_080000c0(40);

    Func_0808a188(0x200e, 0, 10);
    Func_0808a138(0, 2);
    Func_0808a180(0x200e, 0);

    Func_0808a330(0x00010000, 1);       /* 128 << 9 */
    Func_0808a348(20);
    Func_080000c0(40);

close:
    Func_0808a020();
}
