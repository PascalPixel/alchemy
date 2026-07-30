/*
 * resource_3c6 cutscene step at 0x0200010c, 76 bytes (0x0200010c-0x02000157).
 *
 * Complete owner: `push {lr}` at 0x0200010c through the single interworking
 * epilogue `pop {r0} / bx r0` at 0x0200014c.  0x02000150-0x02000157 is this
 * owner's literal pool (0x000009bb and 0x000028b8), past the `bx` and never
 * reached as an instruction.  The epilogue pops the return address into r0,
 * so the owner is void.  Nothing reads r0-r3 before they are first written,
 * so the owner takes no arguments.
 *
 * Straight-line: no branches at all.  Slot 18 is the only entity touched.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * Both pool words here are small integers, not addresses.
 *
 * Call accounting: 7 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3c6 010c` - 7 distinct targets,
 * all import veneers, no prologue and no `call_via` site.  The multiset of
 * `Func_...(` calls below equals that multiset, one call each of
 * Func_080770c8, Func_0808a010, Func_0808a090, Func_0808a170, Func_0808a180,
 * Func_0808a1b8 and Func_0808a580.  (The inventory's `calls=7` agrees here.)
 *
 * UNCERTAINTY: Func_0808a580's second argument is the negated immediate 16,
 * i.e. -16, and its third is 0.  Whether the value is a signed displacement
 * or a signed rate is not established from this owner; it is written as the
 * signed constant the assembly builds.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay. */
void Func_080770c8();           /* audio/sequence cue by id */
void Func_0808a010();           /* wait n frames */
void Func_0808a090();           /* per-slot reorientation request */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face the slot along a heading */
void Func_0808a580();           /* signed per-slot adjustment */

void Func_0200010c(void)
{
    Func_080770c8(0x9bb);
    Func_0808a170(0x28b8);
    Func_0808a180(18, 0);
    Func_0808a090(18, 0x00010000, 0x00008000);
    Func_0808a580(18, -16, 0);
    Func_0808a1b8(18, 0, 0);
    Func_0808a010(10);
}
