typedef signed int s32;

/*
 * Resource 386 cutscene beat at 0x020004a0 (one of two lines, chosen by an
 * event flag).
 *
 * Complete owner: `push {lr}` at 0x020004a0, interworking return
 * `pop {r0} / bx r0` at 0x020004ca.  The control-flow walk reaches every
 * halfword to 0x020004cc; 0x020004ce-0x020004db is the literal pool (an
 * alignment halfword plus 0x0000085b, 0x00001382, 0x00001cf4).  The next
 * owner is the constant loader at 0x020004dc.
 *
 * Signature: `pop {r0} / bx r0` — `void`.
 *
 * Link base 0x02008000; 0x020084a1 in the handler pool is `Func_020004a0 + 1`.
 *
 * Flag 0x85b is the same flag the sibling at 0x02000204 tests and sets, so
 * the two rows are the "first time" and "afterwards" halves of one scene.
 *
 * Both arms reach the SAME two trailing calls, which the assembly does with a
 * `b.n` into the shared tail.  They are written once here rather than copied
 * into each arm: duplicating them would inflate the per-target multiset by
 * two.  Func_0808a170 genuinely appears twice, once per arm.
 */

void Func_0808a018();  /* veneer 0x63c — scene begin */
void Func_0808a020();  /* veneer 0x644 — scene end */
void Func_0808a170();  /* veneer 0x694 — message */
void Func_0808a180();  /* veneer 0x6a4 — act */
s32  Func_080770c0();  /* veneer 0x614 — event-flag test */

void Func_020004a0(void)
{
    Func_0808a018();

    if (Func_080770c0(0x85b) == 0) {
        Func_0808a170(0x1382);
    } else {
        Func_0808a170(0x1cf4);
    }

    Func_0808a180(18, 0);
    Func_0808a020();
}
