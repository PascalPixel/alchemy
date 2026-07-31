typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 398 one-shot scene at 0x02000214.
 *
 * Complete owner: `push {lr}` / `sub sp,#8` at 0x02000214 and the matching
 * `add sp,#8 / pop {r0} / bx r0` at 0x02000276.  108-byte row: 104 bytes of
 * code plus the single pool word 0x00000883 at 0x0200027c.  The pool map is
 * from a control-flow walk — the body is straight line and nothing is reached
 * at or past 0x0200027c.  The return address is popped into r0, so the owner
 * is `void`.
 *
 * ONE-SHOT SCENE, proven in one line.  The interaction record that names this
 * address (pool word 0x02008215 = 0x0200_0214 + the Thumb bit under the proven
 * 0x02008000 link base) is keyed 0x08830008, and the body sets event flag
 * 0x883 with argument 8 — the key names the flag the handler sets.
 *
 * Imports resolved with tools/overlay_call_targets.ts (an overlay `bl` stores
 * `target_offset - 2`).  The begin/end bracket Func_0808a018/Func_0808a020 and
 * the Func_080770c8 flag setter are the same quartet the banked byte-exact
 * source assets/code/resource_398_c_020001f0.c uses under its printed names
 * Func_02000b5c / Func_02000b78 / Func_02000b54.
 *
 * Ten call sites, matching the row's advertised count.  Note the two separate
 * Func_0808a080(15) lookups: the record is fetched twice rather than kept in a
 * register, and both are real sites.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_080091c0();  /* place a fixture: (x, z, w, h, sx, sz) */
void Func_080770c8();  /* set event flag */
void Func_0808a018();  /* begin scene */
void Func_0808a020();  /* end scene */
void Func_0808a010();  /* wait / delay by count */
u8 *Func_0808a080();   /* actor record by slot id */
void Func_0808a0f0();  /* run script (id, a, b) */
void Func_0808a110();  /* set actor pose (slot, pose) */
void Func_0808a1e0();  /* set actor facing (slot, facing) */

void Func_02000214(void)
{
    u8 *actor;

    Func_0808a018();
    Func_0808a0f0(8, 0, 0);
    Func_080770c8(0x883);
    Func_0808a010(0x28);
    Func_0808a110(15, 2);

    actor = Func_0808a080(15);
    actor[0x55] = 0;

    actor = Func_0808a080(15);
    actor[0x23] |= 2;

    Func_0808a1e0(15, 2);
    Func_080091c0(0, 0, 1, 1, 18, 14);
    Func_0808a020();
}
