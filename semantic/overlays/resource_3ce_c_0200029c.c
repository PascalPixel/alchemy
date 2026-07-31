typedef signed int s32;

/*
 * Resource 3ce script-command handler at 0x0200029c (1574 bytes,
 * 0x0200029c-0x020008c1).
 *
 * Role, settled BEFORE any disassembly (call-graph-first, HANDOVER section 0):
 * `assets/code/resource_3ce_overlay.s` carries the pool word 0x0200829d, which
 * under the proven 0x02008000 link base is 0x0200029c + the Thumb bit -- this
 * owner's own entry -- and it appears in the overlay's script-command table as
 * the triple `0xffff0002 / 0x0200829d / 0x00000000`, the same shape that names
 * nine already-banked byte-exact siblings.  So this is a no-argument command
 * handler, command id 0x02.
 *
 * Link base proof for this overlay: see the header of
 * semantic/overlays/resource_3ce_c_02000b10.c -- fifteen odd
 * `0x0200[89ab]xxx` words in `assets/code/resource_3ce_overlay.s` resolve at
 * `word - 0x8000 - 1` onto already-banked byte-exact sibling entries.
 *
 * Complete owner: `push {lr}` prologue at 0x0200029c and the matching
 * interworking return `pop {r0} / bx r0` at 0x020008be.  r0 holds the popped
 * return address, so nothing is returned: the owner is `void`.  No stack frame
 * and no saved registers: r0 and r1 are both reloaded with `movs` immediately
 * before every single call, so there is no long-lived register state anywhere
 * in the body.
 *
 * Pool map, derived by walking the owner's control flow from the prologue.
 * The body is entirely straight-line -- the ONLY branch instruction in 1,574
 * bytes is the bare unconditional `b.n 0x020006a0` at 0x02000698 -- and that
 * branch exists solely to hop this owner's single literal pool:
 *   0x0200069a   2-byte zero alignment word
 *   0x0200069c   0x00000c1e   (the only pool word in the owner)
 * The hop is MID-COMPUTATION: r1 is loaded with 236 at 0x02000696, before the
 * branch, and is consumed by the call at 0x020006a2 after it.  A pool walk
 * that stopped at the branch, or one that did not carry r1 across it, would
 * lose that argument.  Code resumes at 0x020006a0 and runs to the epilogue, so
 * there is no trailing pool; the 2 bytes at 0x020008c2 are the alignment
 * halfword before the next owner at 0x020008c4.
 * 0x00000c1e does not decode as a BL prefix, so no phantom call site.
 *
 * All 196 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are NOT the callees.  This
 * overlay resolves to veneers only (sites=243, prologue=0 across the whole
 * image), so every call here is a main-image import.
 * sites=196, distinct_targets=3, matching the inventory's calls=196.
 *
 * Per-target multiset, tool count == C call count, checked PER TARGET:
 *   Func_08015040   1     Func_08077010   4     Func_08077028 191
 *
 * The 191 Func_08077028 sites are NOT a loop and are NOT folded.  In the
 * assembly they are 191 separate `movs r1,#id / movs r0,#slot / bl` triples in
 * sixteen consecutive runs of one (slot, id) pair; there is no counter, no
 * back edge and no comparison anywhere in the owner.  Writing them as sixteen
 * loops would deflate the multiset by 175, which is exactly the error class
 * HANDOVER warns about.  The banked byte-exact sibling
 * `assets/code/resource_3ce_c_020008c4.c` spells its own long call run out the
 * same way, so this is the established shape in this overlay rather than a
 * transcription choice.
 *
 * Run structure (slot, id, repeat count):
 *   (0,187,17) (0,180,14) (0,181,11) (0,182,13) (0,183,9)
 *   (1,186,13) (1,187,12)
 *   (2,188,14) (2,189,11) (2,236,16)
 *   (3,191,11) (3,192,11) (3,193,11) (3,194,7) (3,195,13) (3,196,8)
 * The four slots are the same four the sibling handler at 0x02000b10 drives,
 * and the owner closes with the same Func_08077010 refresh of all four in the
 * same non-sequential 0, 1, 3, 2 order -- an independent cross-check of both
 * files.
 *
 * Behaviour: the handler queues a fixed script of 191 entries against the four
 * battle slots and then refreshes them.
 *
 * Uncertainties: 0x00000c1e is an id passed to Func_08015040 with a second
 * argument of 1, not an address under the link base (it is far below the link
 * band); the meaning of the Func_08077028 ids is not established beyond their
 * grouping by slot.  Arities are taken from the registers written before each
 * site.
 */

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_08015040();
void Func_08077010();
void Func_08077028();

void Func_0200029c(void)
{
    Func_08015040(0xc1e, 1);

    /* slot 0, id 187 -- 17 consecutive sites */
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);
    Func_08077028(0, 187);

    /* slot 0, id 180 -- 14 consecutive sites */
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);
    Func_08077028(0, 180);

    /* slot 0, id 181 -- 11 consecutive sites */
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);
    Func_08077028(0, 181);

    /* slot 0, id 182 -- 13 consecutive sites */
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);
    Func_08077028(0, 182);

    /* slot 0, id 183 -- 9 consecutive sites */
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);
    Func_08077028(0, 183);

    /* slot 1, id 186 -- 13 consecutive sites */
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);
    Func_08077028(1, 186);

    /* slot 1, id 187 -- 12 consecutive sites */
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);
    Func_08077028(1, 187);

    /* slot 2, id 188 -- 14 consecutive sites */
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);
    Func_08077028(2, 188);

    /* slot 2, id 189 -- 11 consecutive sites */
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);
    Func_08077028(2, 189);

    /* slot 2, id 236 -- 16 consecutive sites */
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);
    Func_08077028(2, 236);

    /* slot 3, id 191 -- 11 consecutive sites */
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);
    Func_08077028(3, 191);

    /* slot 3, id 192 -- 11 consecutive sites */
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);
    Func_08077028(3, 192);

    /* slot 3, id 193 -- 11 consecutive sites */
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);
    Func_08077028(3, 193);

    /* slot 3, id 194 -- 7 consecutive sites */
    Func_08077028(3, 194);
    Func_08077028(3, 194);
    Func_08077028(3, 194);
    Func_08077028(3, 194);
    Func_08077028(3, 194);
    Func_08077028(3, 194);
    Func_08077028(3, 194);

    /* slot 3, id 195 -- 13 consecutive sites */
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);
    Func_08077028(3, 195);

    /* slot 3, id 196 -- 8 consecutive sites */
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077028(3, 196);
    Func_08077010(0);
    Func_08077010(1);
    Func_08077010(3);
    Func_08077010(2);
}
