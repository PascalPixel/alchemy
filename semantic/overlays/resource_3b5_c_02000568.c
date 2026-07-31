typedef signed int s32;

/*
 * Resource 3b5, cutscene script step at 0x02000568 (52 bytes; 48 bytes of code
 * plus the 4-byte literal pool at 0x02000598).
 *
 * Complete owner: `push {lr}` at 0x02000568 and `pop {r0} / bx r0` at
 * 0x02000594.  r0 holds the popped return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds
 * 0x02008569 in a record's handler word — Func_02000568 + the Thumb bit under
 * this overlay's proven 0x02008000 link base.  A no-argument script step.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; six sites,
 * matching the inventory's `calls=6`.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_0808a018();
void Func_0808a1b8();
void Func_0808a130();
void Func_0808a170();
void Func_0808a180();
void Func_0808a020();

void Func_02000568(void)
{
    Func_0808a018();

    Func_0808a1b8(26, 0x4000, 0);
    Func_0808a130(26, 2);

    Func_0808a170(0x1fa2);

    Func_0808a180(26, 0);

    Func_0808a020();
}
