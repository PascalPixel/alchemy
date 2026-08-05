#include "types.h"

/*
 * Resource 3b6 facing-dependent cutscene step at 0x0200091c.
 *
 * Complete owner: `push {r5, lr}` at 0x0200091c and the matching
 * `pop {r5} / pop {r0} / bx r0` interworking return at 0x02000984.  The return
 * address is popped into r0, so the owner is `void`.  r5 holds the incoming
 * argument throughout.
 *
 * TWO literal pools sit INSIDE the span and are data, not code:
 * 0x02000954-0x0200095b (an interior pool, branched over by the `b.n` at
 * 0x02000952) and 0x0200098a-0x02000997 after the return.  The interior one is
 * why the resolver reports one more site than the row's advertised call count
 * of 8 — the pool words decode as a plausible BL pair.
 *
 * This row ends at 0x02000998, which is exactly where this overlay's
 * eight-byte import veneer table begins; there is no further overlay code.
 *
 * Imports are resolved with tools/overlay_call_targets.ts — an overlay `bl`
 * stores `target_offset - 2` — and named for the main-image address in the
 * veneer entry's trailing word.  The message/act pair is the one tracked
 * byte-exact source assets/code/resource_3b6_c_0200073c.c uses under its
 * printed names.
 *
 * Near-twin of 0x0200066c: same predicate ids (0x950, 0x962) and same control
 * flow.  The differences are the first arm's import — 0x080b0010 taking only
 * the subject, against 0x0200066c's 0x080b0008 taking (27, subject) — and all
 * four message ids.  The dropped second argument is real and is preserved.
 *
 * The two `Func_0808a170` / `Func_0808a180` blocks are kept as separate call
 * sites rather than folded, matching the assembly's join structure.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
u16 *Func_0808a080();  /* fetch a record; called with 0 for the subject's own */
void Func_080b0010();
s32 Func_080770c0();   /* query a numeric id; nonzero when selected */
void Func_0808a170();  /* present message by id */
void Func_0808a180();  /* act on the subject */

void Func_0200091c(s32 subject)
{
    u16 *record;
    s32 facing;
    s32 message;

    record = Func_0808a080(0);

    /* Bits 14-15 of the 0x2000-biased halfword at record + 6; see the note in
     * the 0x0200066c sibling. */
    facing = ((s32)record[3] + 0x2000) & 0xc000;

    if (facing == 0xc000) {
        /* Only r0 is set here.  The sibling at 0x0200066c calls a different
         * import with two arguments at the same position; this site genuinely
         * passes one. */
        Func_080b0010(subject);
        return;
    }

    if (Func_080770c0(0x950) != 0) {
    } else if (Func_080770c0(0x962) != 0) {
        message = 0x23bf;
        message = 0x2231;
    } else {
        Func_0808a170(0x1feb);
        Func_0808a180(subject, 0);
        return;
    }

    Func_0808a170(message);
    Func_0808a180(subject, 0);
}
