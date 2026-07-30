typedef signed int s32;

/*
 * resource_3c6 cutscene step at 0x020000c4, 72 bytes (0x020000c4-0x0200010b).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020000c4 through the single
 * interworking epilogue `pop {r5, r6} / pop {r0} / bx r0` at 0x02000100.
 * 0x02000106 is a two-byte alignment `nop` and 0x02000108-0x0200010b is this
 * owner's literal pool (0x000028be); neither is reached as an instruction.
 * The epilogue pops the return address into r0, so the owner is void.
 *
 * The owner takes one argument, the scene slot selector in r0, copied to r6.
 * r5 holds the base dialogue-line id 0x28be for the whole body; the two
 * branch arms use 0x28bf and 0x28c0, formed as r5 + 1 and r5 + 2.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * No pointer-shaped pool word is used here.
 *
 * Control flow: one two-armed conditional on Func_0808a070(0, 0).  The `bne`
 * at 0x020000e2 takes the nonzero arm at 0x020000f2; the zero arm at
 * 0x020000e4 ends with a `b.n` over it to the shared tail at 0x020000f8.
 *
 * Call accounting: 7 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3c6 00c4` - 5 distinct targets,
 * all import veneers, no prologue and no `call_via` site.  The multiset of
 * `Func_...(` calls below equals that multiset: Func_0808a010 x1,
 * Func_0808a070 x1, Func_0808a170 x3, Func_0808a178 x1, Func_0808a180 x1.
 * (The inventory's `calls=7` agrees here.)
 *
 * UNCERTAINTY: Func_0808a070's two zero arguments are a query selector whose
 * meaning is not established from this owner; only the zero/nonzero split of
 * its result is used, to pick between two dialogue lines.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay.
 * Func_0808a070 is used in a condition, so it needs a real return type. */
void Func_0808a010();           /* wait n frames */
s32 Func_0808a070();            /* scene-state query */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a178();           /* dialogue-line variant with a mode word */
void Func_0808a180();           /* wait for the slot's action to finish */

void Func_020000c4(s32 slot)
{
    Func_0808a170(0x28be);
    Func_0808a178(slot, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(0x28bf);
    } else {
        Func_0808a170(0x28c0);
    }

    Func_0808a180(slot, 0);
}
