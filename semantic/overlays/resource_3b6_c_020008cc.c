typedef signed int s32;

/*
 * Resource 3b6 cutscene step at 0x020008cc.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020008cc and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return 0x44 bytes later.  An
 * alignment halfword and a one-word literal pool close the 80-byte row.
 *
 * Signature.  The return address is popped into r0, so the owner is `void`.
 * r6 holds the incoming argument across the calls; r5 holds the message-id
 * base loaded from the pool.
 *
 * PROVEN TRIPLET.  0x020006ec, 0x02000760 and 0x020008cc are 80-byte rows that
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
 * call count.  The begin/message/act/end quartet is the same one that banked
 * byte-exact source assets/code/resource_3b6_c_0200073c.c uses under its
 * printed names, which also fixes the void return and the s32 argument.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_0808a018(); /* begin step */
void Func_0808a170(); /* present message by id */
void Func_0808a178(); /* act on the subject, variant used before the branch */
s32 Func_0808a070();  /* branch predicate; nonzero selects the second arm */
void Func_0808a010(); /* wait / delay by count */
void Func_0808a180(); /* act on the subject */
void Func_0808a020(); /* end step */

void Func_020008cc(s32 subject)
{
    s32 message = 0x23ac;

    Func_0808a018();
    Func_0808a170(message);
    Func_0808a178(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(message + 1);
    } else {
        Func_0808a170(message + 2);
    }

    Func_0808a180(subject, 0);
    Func_0808a020();
}
