typedef signed int s32;

/*
 * Resource 3b5, cutscene script step at 0x02000528 (64 bytes; 60 bytes of code
 * plus the 4-byte literal pool at 0x02000564).
 *
 * Complete owner: `push {lr}` at 0x02000528 and `pop {r0} / bx r0` at
 * 0x02000560.  r0 holds the popped return address: void.
 *
 * Role: this row's entry is named in advance by the cutscene script table in
 * `assets/code/resource_3b5_overlay.s`, which holds 0x02008529 in the handler
 * word of a 12-byte record.  Under this overlay's proven 0x02008000 link base
 * that is Func_02000528 + the Thumb bit, so the row is a script step handler
 * and takes no arguments, exactly like the byte-exact siblings
 * `assets/code/resource_3b5_c_02000954.c` and `..._02000964.c`.
 *
 * The two Func_0808a1b8/Func_0808a180 pairs are separate call sites reached in
 * sequence, not a loop: the fade values differ (0xc000 then 0x8000) and each
 * pair is spelled out in the assembly.  They are NOT merged.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all seven sites
 * are accounted for.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_0808a018();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a180();
void Func_0808a020();

void Func_02000528(void)
{
    Func_0808a018();

    Func_0808a170(0x1fa0);

    Func_0808a1b8(25, 0xc000, 0);
    Func_0808a180(25, 0);

    Func_0808a1b8(25, 0x8000, 0);
    Func_0808a180(25, 0);

    Func_0808a020();
}
