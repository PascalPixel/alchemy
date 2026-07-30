/*
 * resource_3b7 owner at 0x02000178, 36 bytes (0x02000178-0x0200019b).
 *
 * A minimal cutscene beat: open the scene, run one scripted text/animation
 * entry, wait, and close.  The body is 32 bytes of code (0x02000178-0x02000195)
 * followed by a two-byte alignment `nop` at 0x02000196 and one literal-pool
 * word at 0x02000198 (0x00000e35), which is the entry id loaded into r0.  The
 * pool word is data: it is past the `bx r0` return and is only ever reached by
 * the `ldr r0, [pc, #24]` at 0x0200017e.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`, so the disassembler's printed targets are
 * wrong).  All four sites are import veneers:
 *   0x0200017a -> veneer 0x0200191c -> Func_0808a018
 *   0x02000180 -> veneer 0x02001964 -> Func_0808a170
 *   0x0200018a -> veneer 0x0200196c -> Func_0808a178
 *   0x0200018e -> veneer 0x02001924 -> Func_0808a020
 * That is 4 sites / 4 distinct targets, matching the inventory's calls=4.
 *
 * The Func_0808a018 ... Func_0808a020 bracket and the Func_0808a170(id) entry
 * call are the established cutscene-script family (see
 * semantic/overlays/resource_373_c_02002f14.c and
 * semantic/overlays/resource_39f_c_02001150.c).
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner returns nothing: void.
 *
 * `Func_02000154` is this owner byte-for-byte except for the pool word
 * (0x00000e34 instead of 0x00000e35); the two are proven siblings.
 *
 * Uncertainty: no argument register is written before Func_0808a018 or
 * Func_0808a020, so both are spelled with no arguments; the imports' real
 * interfaces are not known, hence the old-style declarations.
 */

void Func_0808a018();
void Func_0808a170();
void Func_0808a178();
void Func_0808a020();

void Func_02000178(void)
{
    Func_0808a018();

    Func_0808a170(0xe35);

    /* `movs r0, #1 / movs r1, #0 / negs r0, r0` - r0 is -1, r1 is 0. */
    Func_0808a178(-1, 0);

    Func_0808a020();
}
