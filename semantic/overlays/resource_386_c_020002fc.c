typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 386 cutscene beat at 0x020002fc.
 *
 * Complete owner: `push {r5, lr}` at 0x020002fc, interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000332-0x02000336.  The 60-byte span
 * is all code — the control-flow walk reaches every halfword and there is no
 * literal pool; the next owner starts at 0x02000338.
 *
 * Signature: the return address is popped into r0, so the owner is `void`.
 *
 * Link base 0x02008000; 0x020082fd in the overlay's handler pool is
 * `Func_020002fc + 1`.
 *
 * Import naming uses the overlay `bl` rule (stored displacement + 2) and the
 * veneer band at 0x020005dc-0x020006dc.  One site is NOT an import: the `bl`
 * at 0x0200031a resolves to offset 0x11c, an ordinary prologue in this same
 * overlay, so it is a genuine intra-overlay call to Func_0200011c.
 *
 * Actor 16's byte at +0x5b is raised across that nested beat and lowered
 * afterwards — a re-entrancy guard, not a state change the scene observes.
 */

void Func_0808a018();  /* veneer 0x63c — scene begin */
void Func_0808a020();  /* veneer 0x644 — scene end */
void Func_0808a098();  /* veneer 0x66c */
u8  *Func_0808a080();  /* veneer 0x664 — actor record accessor */
void Func_0808a138();  /* veneer 0x684 */

void Func_0200011c(void);

void Func_020002fc(void)
{
    Func_0808a018();
    Func_0808a138(16, 1);
    Func_0808a020();

    Func_0808a080(16)[0x5b] = 1;
    Func_0200011c();
    Func_0808a080(16)[0x5b] = 0;

    Func_0808a098(16, 2);
}
