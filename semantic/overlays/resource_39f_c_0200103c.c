typedef signed int s32;

/*
 * resource_39f owner at 0x0200103c, 60 bytes: code 0x0200103c-0x0200106f, then
 * the two pool words 0xfff00000 at 0x02001070 and 0xffe80000 at 0x02001074.
 *
 * Builds a 16.16 position triple on the stack from an object supplied by import
 * Func_0808a080, quantising X and Z to whole units (mask 0xfff00000 keeps the
 * sign and integer part and discards the low 20 fraction bits) and biasing them
 * by +0x80000 and -0x180000, then hands the triple to the overlay's own
 * Func_02000cd0.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `add sp, #12 / pop {r0} / bx r0`, so the owner is void.
 *
 * Uncertainty: the body writes r4 without saving it, which is not an ordinary
 * callee-saved discipline.  It is reproduced here only as the constant
 * 0x80000; nothing observable depends on the register choice.
 */

/* Returns the object whose words at +8, +12 and +16 are read below. */
s32 *Func_0808a080();
void Func_02000cd0();

void Func_0200103c(void)
{
    s32 place[3];
    s32 *source;

    /* r0 is zeroed immediately before the branch. */
    source = Func_0808a080(0);

    place[0] = (source[2] & (s32)0xfff00000) + 0x80000;
    place[1] = source[3];
    place[2] = (source[4] & (s32)0xfff00000) + (s32)0xffe80000;

    Func_02000cd0(place);
}
