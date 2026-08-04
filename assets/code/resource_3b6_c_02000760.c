#include "types.h"

/* Pool-forced message base: a SYMBOL_REF gcc cannot fold, so the two
 * sibling ids stay `adds r0, r5, #1` / `#2` off one pooled word. */
extern u8 Value_00001fbb;

/*
 * Resource 3b6 cutscene step at 0x02000760.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000760 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return 0x44 bytes later.  An
 * alignment halfword and a one-word literal pool close the 80-byte row.
 *
 * Signature.  The return address is popped into r0, so the owner is `void`.
 * r6 holds the incoming argument across the calls; r5 holds the message-id
 * base loaded from the pool.
 *
 * PROVEN TRIPLET.  0x02000760, 0x02000760 and 0x020008cc are 80-byte rows that
 * are byte-identical except for their single pool word — 0x239e, 0x1fbb and
 * 0x23ac respectively.  Every `bl` halfword is bit-identical across the three,
 * and tools/overlay_call_targets.ts reports the same seven callees over nine
 * sites for each.  That diff is the correctness proof for this file: the three
 * differ only in which message-id triple they present.  (It is also the
 * cheapest available witness that an overlay `bl`'s printed label is a
 * per-call-site artefact rather than a distinct callee.)
 *
 * Imports are resolved with tools/overlay_call_targets.ts — an overlay `bl`
 * stores `target_offset - 2` — and named for the main-image address in the
 * veneer entry's trailing word.  Nine sites, matching the row's advertised
 * call count.  The begin/message/act/end quartet is the same one that tracked
 * byte-exact source assets/code/resource_3b6_c_0200073c.c uses under its
 * printed names, which also fixes the void return and the s32 argument.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_02001146(); /* begin step */
void Func_020011a6(); /* present message by id */
s32 Func_020011b6();  /* act on the subject, variant used before the branch */
s32 Func_0200116e();  /* branch predicate; nonzero selects the second arm */
void Func_02001160(); /* wait / delay by count */
void Func_020011c6(); /* present message, first arm */
void Func_020011ce(); /* present message, second arm */
void Func_020011e6(); /* act on the subject */
void Func_0200118a(); /* end step */

void Func_02000760(s32 subject)
{
    s32 message;

    Func_02001146();

    message = (s32)&Value_00001fbb;
    Func_020011a6(message);
    Func_020011b6(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Func_0200116e(0, 0) == 0) {
        Func_02001160(10);
        Func_020011c6(message + 1);
    } else {
        Func_020011ce(message + 2);
    }

    Func_020011e6(subject, 0);
    Func_0200118a();
}
