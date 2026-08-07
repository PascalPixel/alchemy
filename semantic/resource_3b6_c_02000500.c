#include "types.h"

/*
 * Resource 3b6 facing-dependent cutscene step at 0x02000500.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000500 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000596.  The
 * return address is popped into r0, so the owner is `void`.  r6 holds the
 * incoming argument throughout; r5 holds a message-id base in the last arm.
 *
 * Two literal pools lie in the row: an interior one at 0x0200053a-0x02000543
 * (alignment halfword plus the 0xffffc000 mask, branched over by the `b.n` at
 * 0x02000538) and the trailing pool at 0x0200059c-0x020005a7.  Both are data.
 *
 * All thirteen `bl` sites resolve with tools/overlay_call_targets.ts — an
 * overlay `bl` stores `target_offset - 2`, so the disassembler's labels are
 * artefacts — and are named for the main-image address in each veneer entry's
 * trailing word.  Thirteen sites, matching the row's advertised call count
 * exactly.
 *
 * Family.  The head is the facing quadrant test shared with 0x0200066c,
 * 0x0200091c and 0x020005a8; this one tests for 0x8000 where those two test
 * for 0xc000.  The final arm is the body of the 0x020006ec / 0x02000760 /
 * 0x020008cc triplet verbatim, which independently corroborates the reading of
 * that construct.
 *
 * The 0x0808a170 / 0x0808a180 pair reached by the first two id matches and the
 * pair inside the last arm are distinct call sites and are kept distinct.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
u16 *Func_0808a080();  /* fetch a record; called with 0 for the subject's own */
void Func_080b0008();
s32 Func_080770c0();   /* query a numeric id; nonzero when selected */
void Func_0808a170();  /* present message by id */
void Func_0808a178();  /* act on the subject, variant used before a branch */
s32 Func_0808a070();   /* branch predicate; nonzero selects the second arm */
void Func_0808a010();  /* wait / delay by count */
void Func_0808a180();  /* act on the subject */

void Func_02000500(s32 subject)
{
    u16 *record;
    s32 facing;
    s32 message;

    record = Func_0808a080(0);

    /* Bits 14-15 of the 0x2000-biased halfword at record + 6, tested in the
     * high halfword after `lsls #16`; here against 0x80000000. */
    facing = ((s32)record[3] + 0x2000) & 0xc000;

    if (facing == 0x8000) {
        Func_080b0008(28, subject);
        return;
    }

    /* Ids 0x950 and 0x962 are queried in order; the first match wins. */
    if (Func_080770c0(0x950) != 0) {
        message = 0x238d;
        goto matched_message;
    }
    if (Func_080770c0(0x962) != 0) {
        message = 0x221b;
        goto matched_message;
    }
    goto fallback_message;

matched_message:
    Func_0808a170(message);
    Func_0808a180(subject, 0);
    return;

fallback_message:
    /* Fallback arm: the full three-id sequence of the 0x020006ec family. */
    Func_0808a170(message);
    message = 0x1fd5;
    Func_0808a178(subject, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(message + 1);
    } else {
        Func_0808a170(message + 2);
    }
    Func_0808a180(subject, 0);
}
