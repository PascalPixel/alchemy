typedef signed int s32;

/*
 * Resource 3b6 cutscene step at 0x0200085c.
 *
 * Complete owner: `push {lr}` at 0x0200085c and the matching
 * `pop {r0} / bx r0` interworking return at 0x02000886.  An alignment halfword
 * and a three-word literal pool follow at 0x0200088a-0x02000897.
 *
 * Signature.  The return address is popped into r0, so the owner is `void`.
 * It takes no argument: r0 is written before every call and never read on
 * entry.
 *
 * Imports are resolved with tools/overlay_call_targets.ts (an overlay `bl`
 * stores `target_offset - 2`, so the disassembler's labels are artefacts) and
 * named for the main-image address in the veneer entry's trailing word.  Six
 * sites, matching the row's advertised call count.  The begin/message/act/end
 * quartet is the same one that banked byte-exact source
 * assets/code/resource_3b6_c_0200073c.c uses under its printed names.
 *
 * The two `Func_0808a170` calls are deliberately kept as two distinct call
 * sites rather than folded into one call with a selected id: the per-target
 * accounting is over call sites, not over distinct behaviour.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_0808a018(); /* begin step */
s32 Func_080770c0();  /* query a numeric id; nonzero when selected */
void Func_0808a170(); /* present message by id */
void Func_0808a180(); /* act on the subject */
void Func_0808a020(); /* end step */

void Func_0200085c(void)
{
    Func_0808a018();

    /* 0x08be is a plain integer id, not a pointer: it is below this overlay's
     * 0x02008000 link base, so it is not an in-image address. */
    if (Func_080770c0(0x8be) != 0) {
        Func_0808a170(0x23b4);
    } else {
        Func_0808a170(0x23b3);
    }

    Func_0808a180(25, 0);
    Func_0808a020();
}
