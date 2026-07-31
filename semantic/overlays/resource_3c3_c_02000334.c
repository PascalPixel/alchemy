typedef unsigned char u8;

/*
 * Resource 3c3, scene bracket at 0x02000334 (142 bytes, 11 call sites).
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x02000334 and
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x020003ba.  The popped branch
 * register IS r0, so it holds the return address and the owner is `void`.
 * The span ends with the two-byte zero alignment word at 0x020003c2; there is
 * NO literal pool — every constant in this body is built with
 * `movs` + `lsls`, which is why the row runs straight into the next owner's
 * prologue at 0x020003c4.
 *
 * Shift-built constants, spelled as their values below: 0x80 << 9 = 0x10000,
 * 0x80 << 8 = 0x8000, 0xc0 << 1 = 384, 0x80 << 7 = 0x4000.
 *
 * The body is the standard begin/…/end bracket: Func_0808a018 opens it,
 * Func_0808a020 closes it.  Between them slots 8 and 9 are scaled to
 * 1.0/0.5 in 16.16 (0x10000 / 0x8000 against 0x10000 as one), placed at
 * columns 136 and 152 of row 384, faced along heading 0x4000, and slot 8 is
 * put in presentation mode 1.  The two Func_080091c0 requests share r5 = 7 as
 * their first stack argument; the second stack argument differs (27 then 26)
 * and is spelled per call.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 11 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080091c0 x2, Func_0808a090 x2, Func_0808a1b8 x2, Func_0808a018 x1,
 *   Func_0808a020 x1, Func_0808a0c8 x1, Func_0808a0d0 x1, Func_0808a100 x1.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080091c0();   /* six-argument presentation request, last two on the stack */
void Func_0808a018();   /* open a scripted scene */
void Func_0808a020();   /* close a scripted scene */
void Func_0808a090();   /* set a slot's x/y scale in 16.16 */
void Func_0808a0c8();   /* place an entity */
void Func_0808a0d0();   /* place an entity */
void Func_0808a100();   /* set a slot's presentation mode */
void Func_0808a1b8();   /* face the slot along a heading */

void Func_02000334(void)
{
    int mode = 7;

    Func_0808a018();

    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a090(9, 0x10000, 0x8000);

    Func_0808a0c8(8, 136, 384);
    Func_0808a0d0(9, 152, 384);

    Func_0808a1b8(8, 0x4000, 0);
    Func_0808a1b8(9, 0x4000, 0);

    Func_0808a100(8, 1);

    Func_080091c0(6, 27, 1, 1, mode, 27);
    Func_080091c0(9, 26, 2, 1, mode, 26);

    Func_0808a020();
}
