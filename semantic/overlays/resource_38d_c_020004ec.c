typedef signed int s32;

/*
 * resource_38d owner at 0x020004ec, 124 bytes: code 0x020004ec-0x0200055b and
 * the three-word literal pool at 0x0200055c-0x02000567.  A control-flow walk
 * from the prologue reaches the pool only through `ldr rN, [pc, #imm]`; both
 * forward branches land at or before 0x02000548 and the last instruction
 * before the pool is `bx r0`, so no pool word is decoded as an instruction.
 *
 * The shared "award" step of this overlay's item scenes.  Three byte-exact
 * siblings call it and settle its interface exactly:
 *   assets/code/resource_38d_c_0200043c.c -> (21, 182, 0x210)
 *   assets/code/resource_38d_c_02000468.c -> (22, 183, 0x211)
 *   assets/code/resource_38d_c_02000494.c -> (23, 186, 0x212)
 *   assets/code/resource_38d_c_020004c0.c -> (24, 189, 0x213)
 * (each of those files names the callee 0x020009c8 / 0x02000970 / ... under
 * the old printed-`bl` convention; `overlay_call_targets.ts` resolves all four
 * sites to this prologue, and their `bl` halfword pairs are bit-identical,
 * which is the standard proof that they are one callee under four names.)
 *
 * Behaviour: it opens the scripted bracket, creates a presentation for the
 * (slot, subject) pair, then tries to acquire the subject.  On success it
 * requests scene mode 2 for the slot, sets story flags 0x84e and the caller's
 * id, and clears 0x322 and 0x202.  On failure (-1) it plays sound cue 125 and
 * requests scene mode 5 instead.  Either way the created record is torn down
 * and the bracket closed.
 *
 * Call accounting: 12 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 04ec --json`: Func_0808a100
 * x2, Func_080770c8 x2, Func_080770d0 x2, and one each of Func_0808a018,
 * Func_0808a388, Func_0808a060, Func_080f9010, Func_080090d0, Func_0808a020 -
 * nine distinct targets.  Every site appears below exactly once and the
 * per-target multisets match.  The inventory row says calls=11, the documented
 * lower bound.  No intra-overlay call and no `call_via` slot: r3 is written
 * only to build the -1 comparison constant.
 *
 * The epilogue is `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} /
 * bx r0`, so r0 holds the popped return address and the owner is void.  The
 * prologue saves r0, r1 and r2 into r6, r5 and r7 before the first branch,
 * which is what fixes the three parameters.
 *
 * Link base 0x02008000 is witnessed elsewhere in this overlay (the installed
 * handler words 0x020085f5 = Func_020005f4 + 1 and 0x02008569 =
 * Func_02000568 + 1); nothing in this owner depends on it, since all three
 * pool words are small ids.
 *
 * UNCERTAINTIES:
 *  - Func_0808a388's first argument is the literal 0 at this, its only site in
 *    the overlay; its interface is not established.
 *  - Func_0808a060's result is compared against -1 only, so "unavailable" is
 *    read from the branch, not from the import.
 *  - 2 and 5 are the two Func_0808a100 modes; nothing here names them.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
s32 Func_0808a388();            /* create the presentation; returns a record */
s32 Func_0808a060();            /* acquire the subject; -1 when unavailable */
void Func_0808a100();           /* per-record scene request */
void Func_080090d0();           /* tear down the record */
void Func_080770c8();           /* set a story flag */
void Func_080770d0();           /* clear a story flag */
void Func_080f9010();           /* play a sound cue */

void Func_020004ec(s32 slot, s32 subject, s32 flag)
{
    s32 record;

    Func_0808a018();

    record = Func_0808a388(0, slot, subject);

    if (Func_0808a060(subject, 0) != -1) {
        Func_0808a100(slot, 2);
        Func_080770c8(0x84e);
        Func_080770c8(flag);
        Func_080770d0(0x322);
        Func_080770d0(0x202);
    } else {
        Func_080f9010(125);
        Func_0808a100(slot, 5);
    }

    Func_080090d0(record);
    Func_0808a020();
}
