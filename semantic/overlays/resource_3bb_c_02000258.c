typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3bb owner at 0x02000258, 52 bytes (0x02000258-0x0200028b):
 * 48 bytes of code plus the one-word literal pool at 0x02000288 (followed by
 * two alignment bytes) that the body loads its first argument from.
 *
 * Prologue `push {lr}` / `sub sp, #8` at 0x02000258, epilogue
 * `add sp, #8 / pop {r0} / bx r0`: r0 holds the popped return address, so the
 * owner is void.  The eight bytes of outgoing stack carry the fifth and sixth
 * arguments of the final six-argument call.
 *
 * This owner and 0x0200028c are byte-identical apart from three values (the
 * pool word 0x331 vs 0x332, the immediate 20 vs 21, and the fifth outgoing
 * argument 44 vs 50), and their `bl` halfwords are bit-identical while the
 * printed targets differ by exactly 0x34 - the spacing between the two owners.
 * That is the cheapest available proof that both call the same three imports.
 *
 * Targets come from tools/overlay_call_targets.ts (target offset = stored
 * displacement + 2), never from the disassembler's `bl` annotations, which are
 * wrong for every overlay branch.  All three sites reach the import veneer
 * table, in call order: 0x3ef0 -> Func_080770c8, 0x3f50 -> Func_0808a080, 0x3e88 ->
 * Func_080091c0.  That last one matches the byte-exact sibling
 * assets/code/resource_3bb_c_020002c0.c, which spells the same six-argument
 * call as Func_02004164(32, 37, 1, 4, 32, 77).
 *
 * Uncertainty: the record returned by Func_0808a080 is only touched at byte
 * offset 85, so no layout is asserted beyond that one field.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080770c8();
u8 *Func_0808a080();
void Func_080091c0();

void Func_02000258(void)
{
    u8 *record;

    Func_080770c8(0x331);

    record = Func_0808a080(20);
    record[85] = 0;

    Func_080091c0(46, 17, 1, 1, 44, 17);
}
