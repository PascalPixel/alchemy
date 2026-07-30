typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3bb owner at 0x0200028c, 52 bytes (0x0200028c-0x020002bf):
 * 48 bytes of code plus the one-word literal pool at 0x020002bc (followed by
 * two alignment bytes) that the body loads its first argument from.
 *
 * Prologue `push {lr}` / `sub sp, #8` at 0x0200028c, epilogue
 * `add sp, #8 / pop {r0} / bx r0`: r0 holds the popped return address, so the
 * owner is void.  The eight bytes of outgoing stack carry the fifth and sixth
 * arguments of the final six-argument call.
 *
 * Byte-identical to the neighbouring owner 0x02000258 apart from three values
 * (pool word 0x332 vs 0x331, immediate 21 vs 20, fifth outgoing argument 50
 * vs 44); the `bl` halfwords are bit-identical there, so the two owners
 * provably share all three callees even though the disassembler prints
 * targets 0x34 apart.
 *
 * Targets come from tools/overlay_call_targets.ts (target offset = stored
 * displacement + 2), in call order: 0x3ef0 -> Func_080770c8, 0x3f50 ->
 * Func_0808a080, 0x3e88 -> Func_080091c0.
 *
 * Uncertainty: the record returned by Func_0808a080 is only touched at byte
 * offset 85, so no layout is asserted beyond that one field.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080770c8();
u8 *Func_0808a080();
void Func_080091c0();

void Func_0200028c(void)
{
    u8 *record;

    Func_080770c8(0x332);

    record = Func_0808a080(21);
    record[85] = 0;

    Func_080091c0(46, 17, 1, 1, 50, 17);
}
