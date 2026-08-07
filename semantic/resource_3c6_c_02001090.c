/*
 * resource_3c6 cutscene script at 0x02001090, 300 bytes
 * (0x02001090-0x020011bb).
 *
 * Complete owner: `push {lr}` at 0x02001090 through the single interworking
 * epilogue `pop {r0} / bx r0` at 0x020011b4.  0x020011b8-0x020011bb is this
 * owner's literal pool (0x000028b0), past the `bx` and never reached as an
 * instruction; the next inventory row begins at 0x020011bc.  The epilogue
 * pops the return address into r0, so the owner is void.  Nothing reads
 * r0-r3 before they are first written, so the owner takes no arguments.
 *
 * Entirely straight-line: no branch of any kind between the prologue and the
 * epilogue, so the body is a literal transcription of the script.  Slots 1, 2,
 * 3, 22 and the group selectors 0/1/2/3 are the only entities touched.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * The single pool word here is a small integer, not an address.
 *
 * Call accounting: 37 `bl` sites, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3c6 1090` - 7 distinct targets,
 * all import veneers, no prologue and no `call_via` site.  The multiset of
 * `Func_...(` calls below equals that multiset exactly: Func_0808a010 x16,
 * Func_0808a180 x7, Func_0808a1e8 x5, Func_0808a110 x4, Func_0808a138 x2,
 * Func_0808a1b8 x2, Func_0808a170 x1.  (The inventory's `calls=37` agrees.)
 *
 * UNCERTAINTY: Func_0808a1e8's second argument is 256 at four sites and 258
 * at one, each built as `movs r1,#128` / `movs r1,#129` followed by
 * `lsls r1, r1, #1`.  Two neighbouring animation ids rather than a flag pair
 * is the reading that fits the surrounding waits, but it is not proven here.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay. */
void Func_0808a010();           /* wait n frames */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face the slot along a heading */
void Func_0808a1e8();           /* play animation id for n frames */

void Func_02001090(void)
{
    Func_0808a170(0x28b0);
    Func_0808a010(20);

    Func_0808a138(22, 2);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);

    Func_0808a110(22, 4);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);

    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(1, 256, 0);
    Func_0808a1e8(3, 256, 0);
    Func_0808a1e8(2, 256, 55);
    Func_0808a010(10);

    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);

    Func_0808a110(22, 4);
    Func_0808a010(20);
    Func_0808a180(22, 0);
    Func_0808a010(10);

    Func_0808a1e8(3, 258, 40);
    Func_0808a180(3, 0);
    Func_0808a010(10);

    Func_0808a110(1, 3);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(10);

    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a010(65);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a010(40);

    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a180(2, 0);
}
