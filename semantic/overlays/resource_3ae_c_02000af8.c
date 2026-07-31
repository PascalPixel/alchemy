typedef signed int s32;

/*
 * Resource 3ae, owner at 0x02000af8 (112 bytes, no literal pool of its own —
 * every constant is an immediate).
 *
 * Role known in advance from the call graph: the in-image handler table at
 * file offset ~0x1268 holds the odd word 0x02008af9 == Func_02000af8 + 1 at
 * four separate entries (keys 0xffff000e / 0xffff000f under condition word
 * 0x00008c15), and `Func_02000958` — the room-0x70 setup hook — calls it
 * directly with no arguments.  So it takes none.
 *
 * Prologue `push {r5,r6,lr}` + `mov r6,r8 / push {r6}` + `sub sp,#8`;
 * epilogue `add sp,#8 / pop {r3} / mov r8,r3 / pop {r5,r6} / pop {r0} / bx r0`.
 * The return address is popped into r0, so r0 does not survive: the owner is
 * void.  The 8-byte frame carries the fifth and sixth arguments of the
 * six-argument renderer import Func_080091c0.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x02000b02, 0x02000b12 -> Func_0808a080  (object record for a slot)
 *   0x02000b28, 0x02000b3a, 0x02000b4c -> Func_080091c0 (6-arg renderer ABI)
 *   0x02000b52, 0x02000b58 -> Func_02000b68 (prologue; byte-exact source)
 *
 * sites=7 equals the inventory's calls=7, so the call set is complete.
 * Func_02000b68's byte-exact source takes one s32 actor id, which is what the
 * two immediates 14 and 15 are here — the same two slot ids passed to
 * Func_0808a080 at the top.
 *
 * Both slots' field at offset 8 is arithmetic-shifted right by 20
 * (`asrs r3,r3,#20`), i.e. a signed 16.16 (or 12.20) fixed-point coordinate
 * reduced to a whole-unit count; that value is passed as the renderer's fifth
 * argument.  r8 caches slot 14's value across the intervening call because
 * Func_0808a080 clobbers r0-r3.
 *
 * Uncertainties: the meaning of the renderer's six arguments beyond their
 * established ABI position is not derivable here; the constants are preserved
 * exactly.  Field +8 of the Func_0808a080 record is read as a signed word from
 * the `ldr` / `asrs` pair alone.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 *Func_0808a080();
void Func_080091c0();

void Func_02000b68(s32);

void Func_02000af8(void)
{
    s32 slot14_pos;
    s32 slot15_pos;

    slot14_pos = Func_0808a080(14)[2] >> 20;   /* [r0,#8], asrs #20 */
    slot15_pos = Func_0808a080(15)[2] >> 20;

    Func_080091c0(5, 12, 5, 1, 5, 11);
    Func_080091c0(1, 0, 1, 1, slot15_pos, 11);
    Func_080091c0(1, 0, 1, 1, slot14_pos, 11);

    Func_02000b68(14);
    Func_02000b68(15);
}
