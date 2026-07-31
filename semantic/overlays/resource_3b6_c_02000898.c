typedef signed int s32;

/*
 * Resource 3b6 cutscene step at 0x02000898.
 *
 * Complete owner: `push {r5, lr}` at 0x02000898 and the matching
 * `pop {r5} / pop {r0} / bx r0` interworking return at 0x020008bc.  An
 * alignment halfword and a two-word literal pool follow at
 * 0x020008c2-0x020008cb; both are data.
 *
 * Signature.  The epilogue pops the saved return address into r0, so nothing
 * is returned: the owner is `void` (HANDOVER section 0).  r5 holds the single
 * incoming argument across the calls.
 *
 * Import naming.  An overlay `bl` stores `target_offset - 2`, so the labels
 * printed by the disassembler are artefacts.  Each site here is resolved with
 * tools/overlay_call_targets.ts to an entry of this overlay's eight-byte
 * veneer table, and the import is named for the main-image address in that
 * entry's trailing word.  All five sites resolve; the row's advertised call
 * count is five.
 *
 * The naming is checked against banked byte-exact material rather than
 * inferred: assets/code/resource_3b6_c_0200073c.c is the same construct with
 * one fewer call, and its `Func_02001122 / Func_02001180 / Func_02001198 /
 * Func_0200113c` resolve to veneer offsets 0x9e0 / 0xa38 / 0xa48 / 0x9e8 —
 * exactly the imports below.  That source's `void Func_0200073c(s32 arg0)`
 * also confirms the void return and the s32 argument type used here.
 */

/* Old-style declarations are mandatory in overlay sources: one import name can
 * be reached with different argument counts from different call sites. */
void Func_0808a018(); /* begin step */
void Func_0808a170(); /* present message by id */
void Func_0808a1e8();
void Func_0808a180(); /* act on the subject */
void Func_0808a020(); /* end step */

void Func_02000898(s32 subject)
{
    Func_0808a018();
    Func_0808a170(0x23a8);

    /* Argument registers are set r2, r0, r1 in that order, so the call takes
     * (31, 0x103, 40).  The meaning of the triple is not established. */
    Func_0808a1e8(31, 0x103, 40);

    Func_0808a180(subject, 0);
    Func_0808a020();
}
