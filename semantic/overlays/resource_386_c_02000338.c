typedef signed int s32;

/*
 * Resource 386 cutscene beat at 0x02000338 (guarded line, with a "cannot do
 * that now" refusal arm).
 *
 * Complete owner: `push {lr}` at 0x02000338, interworking return
 * `pop {r0} / bx r0` at 0x02000378.  The control-flow walk reaches every
 * halfword to 0x0200037a; 0x0200037c-0x0200037f is the owner's single
 * literal pool word (0x00001384), reached only by the `ldr r0,[pc,#36]` at
 * 0x02000354.  Next owner at 0x02000380.
 *
 * Signature: `pop {r0} / bx r0` — `void`.
 *
 * Link base 0x02008000; 0x02008339 in the handler pool is `Func_02000338 + 1`.
 *
 * The two arms are NOT merged.  They share only the trailing scene-end call,
 * which the assembly reaches by a `b.n` from the first arm — spelled here as
 * fall-out of the `if`/`else`, so Func_0808a020 appears exactly once, as it
 * does in the assembly.
 */

void Func_0808a010();  /* veneer 0x634 */
void Func_0808a018();  /* veneer 0x63c — scene begin */
void Func_0808a020();  /* veneer 0x644 — scene end */
void Func_0808a060();  /* veneer 0x654 */
void Func_0808a110();  /* veneer 0x67c */
void Func_0808a170();  /* veneer 0x694 — message */
void Func_0808a180();  /* veneer 0x6a4 — act */
void Func_0808a398();  /* veneer 0x6c4 */
s32  Func_08077250();  /* veneer 0x62c — party/state query */

void Func_02000338(void)
{
    Func_0808a018();

    if (Func_08077250() == 0) {
        Func_0808a110(18, 4);
        Func_0808a010(20);
        Func_0808a170(0x1384);
        Func_0808a180(18, 0);
    } else {
        Func_0808a398(0xe7, 3);
        Func_0808a060(0xe7, 0);
    }

    Func_0808a020();
}
