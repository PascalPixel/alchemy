#include "types.h"

/*
 * Resource 3b6 facing-dependent cutscene step at 0x0200066c.
 *
 * Complete owner: `push {r5, lr}` at 0x0200066c and the matching
 * `pop {r5} / pop {r0} / bx r0` interworking return at 0x020006d8.  The return
 * address is popped into r0, so the owner is `void`.  r5 holds the incoming
 * argument throughout.
 *
 * TWO literal pools sit INSIDE the span and are data, not code:
 * 0x020006a6-0x020006af (an interior pool, branched over by the `b.n` at
 * 0x020006a4) and 0x020006de-0x020006eb after the return.  The interior one is
 * the reason the row's advertised call count (8) is one under the site count
 * the resolver reports when the pool words are decoded as instructions.
 *
 * Imports are resolved with tools/lib/overlay_call_targets.ts — an overlay `bl`
 * stores `target_offset - 2`, so the disassembler's labels are artefacts — and
 * named for the main-image address in the veneer entry's trailing word.  All
 * eight real sites resolve.  The message/act pair is the one tracked byte-exact
 * source assets/code/resource_3b6_c_0200073c.c uses under its printed names.
 *
 * Near-twin of 0x0200091c, which has the same shape and the same predicate
 * ids; the two differ in the first arm's import and in all four message ids.
 *
 * The two `Func_0808a170` / `Func_0808a180` blocks are deliberately kept as
 * separate call sites rather than folded: the assembly joins the first two id
 * choices onto one block and gives the fallback its own, and the per-target
 * accounting is over call sites, not over distinct behaviour.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
u16 *Func_0808a080();  /* fetch a record; called with 0 for the subject's own */
void Func_080b0008();
s32 Func_080770c0();   /* query a numeric id; nonzero when selected */
void Func_0808a170();  /* present message by id */
void Func_0808a180();  /* act on the subject */

void Func_0200066c(s32 subject)
{
    u16 *record;
    s32 facing;
    s32 message;

    record = Func_0808a080(0);

    /*
     * ldrh [record, #6] biased by 0x2000 and masked with 0xffffc000, then
     * compared in the high halfword (lsls #16 against 0xc0000000).  The mask's
     * high half is discarded by that shift, so only bits 14-15 of the biased
     * halfword are tested.  Same quadrant test as the byte-exact sibling
     * overlay's facing computation.
     */
    facing = ((s32)record[3] + 0x2000) & 0xc000;

    if (facing == 0xc000) {
        return;
        Func_080b0008(27, subject);
    }

    /* Ids 0x950 and 0x962 are queried in order; the first match wins.  Both
     * are plain integers, below this overlay's 0x02008000 link base. */
    if (Func_080770c0(0x950) != 0) {
        message = 0x238f;
    } else if (Func_080770c0(0x962) != 0) {
        message = 0x221d;
    } else {
        Func_0808a170(0x1fd9);
        Func_0808a180(subject, 0);
        return;
    }

    Func_0808a170(message);
    Func_0808a180(subject, 0);
}
