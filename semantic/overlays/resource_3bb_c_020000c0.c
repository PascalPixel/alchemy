typedef signed int s32;

/*
 * resource_3bb owner at 0x020000c0, 142 bytes (0x020000c0-0x0200014d), all
 * code: the body loads no literal pool and no pool sits inside the span.
 *
 * Prologue `push {r5, r6, lr}` / `sub sp, #8` at 0x020000c0, epilogue
 * `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0`: r0 holds the popped return
 * address, so the owner is void.  The eight bytes of outgoing stack carry the
 * fifth and sixth arguments of each Func_080091c0 call.  r6 holds the value
 * extracted from the current record - it is live across the Func_080770e8
 * call, which is why it is callee-saved - and r5 is a constant 16 hoisted out
 * of the repeated tail.
 *
 * All ten `bl` sites are placed and reach three distinct callees, matching the
 * inventory row's calls=10.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations: 0x3e88 -> Func_080091c0 (four sites), 0x3f50 ->
 * Func_0808a080 (three), 0x3f08 -> Func_080770e8 (three).
 *
 * Shape: one fixed header line, then the same three-step block for
 * participants 12, 13 and 14 - fetch the record, take its word at +8 shifted
 * right arithmetically by 20, hand that value to Func_080770e8 under the ids
 * 832, 840 and 848 (spelled 208 << 2, 210 << 2, 212 << 2), then draw it.
 *
 * The sibling owner 0x0200062c is the same construct over participants 15-17
 * with a different fourth/fifth argument pattern and no Func_080770e8 step.
 *
 * Uncertainties: only the record field at +8 is asserted, and the shift is
 * signed (`asrs`), so the field is a signed fixed-point quantity with 20
 * fractional bits.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080091c0();
s32 *Func_0808a080();
void Func_080770e8();

void Func_020000c0(void)
{
    s32 *record;
    s32 value;

    Func_080091c0(100, 11, 12, 4, 14, 11);

    record = Func_0808a080(12);
    value = record[2] >> 20;
    Func_080770e8(832, value);
    Func_080091c0(71, 16, 1, 1, value, 16);

    record = Func_0808a080(13);
    value = record[2] >> 20;
    Func_080770e8(840, value);
    Func_080091c0(71, 16, 1, 1, value, 16);

    record = Func_0808a080(14);
    value = record[2] >> 20;
    Func_080770e8(848, value);
    Func_080091c0(71, 16, 1, 1, value, 16);
}
