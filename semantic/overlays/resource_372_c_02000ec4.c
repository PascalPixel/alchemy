typedef signed int s32;

/*
 * Resource 372 dialogue/portrait script step at 0x02000ec4 (114 bytes,
 * 5 calls).
 *
 * Complete owner: `push {r5, r6, lr} / mov r6, r8 / push {r6} / sub sp, #8`
 * at 0x02000ec4, matching `add sp, #8 / pop {r3} / mov r8, r3 / pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02000f2a.  `pop {r0} ; bx r0` — r0 is the popped
 * return address, so the owner returns nothing.  0x02000f36 is a 2-byte
 * alignment halfword, not code.
 *
 * All five calls use the established six-argument ABI of this overlay:
 * r0-r3 plus two stack words at [sp,#0] and [sp,#4].  The byte-exact siblings
 * `assets/code/resource_372_c_020010a4.c` and `..._02001498.c` prove that
 * spelling, so it is reused verbatim here.
 *
 * The 8 bytes of stack are the outgoing argument block only; r5, r6 and r8
 * hold the repeated fifth/sixth arguments across calls (r8 = 57, r5 = 58,
 * r6 = 21 then 20).
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes.  As documented for this overlay, that address is a stable identity
 * for an import, not a place to disassemble; consecutive script steps reach the
 * same helper under consecutively increasing names.
 */

/* Imports, old-style: the six-argument renderer/script ABI is established by
 * the byte-exact siblings, but the individual interfaces are not named. */
void Func_080091c0(s32, s32, s32, s32, s32, s32);

void Func_02000ec4(void)
{
    s32 a = 57;   /* r8 */
    s32 b = 58;   /* r5 */
    s32 c = 21;   /* r6, then 22 inline, then 20 */
    s32 d = 20;   /* r6 reloaded */

    Func_080091c0(29, 64, 1, 1, c, a);
    Func_080091c0(29, 64, 1, 1, c, b);
    Func_080091c0(29, 64, 1, 1, 22, b);
    Func_080091c0(29, 64, 1, 1, d, b);
    Func_080091c0(28, 20, 1, 1, d, a);
}
