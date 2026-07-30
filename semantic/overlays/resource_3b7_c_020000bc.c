/*
 * resource_3b7 owner at 0x020000bc, 72 bytes (0x020000bc-0x02000103).
 *
 * A branching dialogue beat.  Code runs 0x020000bc-0x020000fd; 0x020000fe is a
 * two-byte alignment `nop` and 0x02000100 is a single literal-pool word
 * (0x00000e19), the base script-entry id loaded into r5 at 0x020000be.  The
 * pool word sits past the `bx r0` return and is reached only by that `ldr`, so
 * it is data, not an instruction.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts`.  An overlay
 * `bl` stores `target_offset - 2`, so the disassembler's printed targets are
 * wrong; this owner is a good witness for that, because it and its sibling
 * `Func_02000074` print entirely different callee addresses while resolving to
 * exactly the same five veneers.  In program order:
 *   0x020000c4 -> veneer 0x02001964 -> Func_0808a170
 *   0x020000cc -> veneer 0x0200196c -> Func_0808a178
 *   0x020000d4 -> veneer 0x02001934 -> Func_0808a070
 *   0x020000de -> veneer 0x02001914 -> Func_0808a010
 *   0x020000e4 -> veneer 0x02001964 -> Func_0808a170
 *   0x020000ec -> veneer 0x02001964 -> Func_0808a170
 *   0x020000f4 -> veneer 0x02001974 -> Func_0808a180
 * Completeness: that multiset is 7 sites over 5 distinct targets, which is
 * exactly what the tool reports (sites=7 distinct_targets=5), and the inventory
 * row's calls=7 agrees.
 *
 * Func_0808a170(id) / Func_0808a178 / Func_0808a180 / Func_0808a070 /
 * Func_0808a010 are the established cutscene-script imports; see
 * semantic/overlays/resource_373_c_02002f14.c, where Func_0808a070's result is
 * likewise tested for zero and Func_0808a010 is likewise called with 20 as a
 * wait length.
 *
 * The epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: void.
 *
 * `Func_02000074` is this owner byte-for-byte except for the pool word
 * (0x00000e39 instead of 0x00000e19); the two are proven siblings.
 *
 * Uncertainties: the single argument arrives in r0 and is saved to r6, where it
 * is used only as the first argument of Func_0808a178 and Func_0808a180; on the
 * evidence of the sibling overlays that is a scene slot id, but nothing here
 * proves it.  The imports' real interfaces are unknown, hence the old-style
 * declarations, and Func_0808a070 must be declared with a return type because
 * it is used in a condition.
 */

typedef signed int s32;

void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a010();
void Func_0808a180();

void Func_020000bc(s32 slot)
{
    s32 entry = 0xe19;

    Func_0808a170(entry);
    Func_0808a178(slot, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(entry + 1);
    } else {
        Func_0808a170(entry + 2);
    }

    Func_0808a180(slot, 0);
}
