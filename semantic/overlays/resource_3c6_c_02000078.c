typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3c6 cutscene step at 0x02000078, 76 bytes (0x02000078-0x020000c3).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000078 through the single
 * interworking epilogue `pop {r5, r6} / pop {r0} / bx r0` at 0x020000b6.
 * The final eight bytes of the row, 0x020000bc-0x020000c3, are this owner's
 * literal pool (0x000026af and 0x02009638); they are past the `bx` and are
 * never reached as instructions.  Because the epilogue pops the return
 * address into r0, nothing is returned: the owner is void.
 *
 * The owner takes one argument, the scene slot selector in r0, which is
 * copied to r6 in the prologue and reused by every later call.
 *
 * LINK BASE 0x02008000.  Two witnesses in this image: the handler words at
 * file offsets 0x1ee4+8 and 0x1ee4+24 read 0x020087c5 and 0x020091bd, which
 * are Func_020007c4 + 1 and Func_020011bc + 1 (Thumb bit set).  So the even
 * pool word 0x02009638 is in-image data at file offset 0x1638, not a RAM
 * global, and is declared as such below.
 *
 * Call accounting: 7 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3c6 0078` - 6 distinct targets,
 * all import veneers, no prologue and no `call_via` site.  The multiset of
 * `Func_...(` calls below equals that multiset: Func_0808a010 x1,
 * Func_0808a080 x2, Func_0808a098 x1, Func_0808a170 x1, Func_0808a180 x1,
 * Func_0808a1b8 x1.  (The inventory's `calls=7` agrees here.)
 *
 * UNCERTAINTY: the two stores through Func_0808a080's result write the same
 * 16.16 constant 1.0 (0x00010000, built as 128 << 9) to record fields +0x18
 * and +0x1c.  Two separate lookups of the same record are used rather than
 * one, which is what the assembly does; the field pair is most likely an
 * X/Z motion rate, but that is not established from this owner alone.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay. */
void Func_0808a010();           /* wait n frames */
s32 *Func_0808a080();           /* scene record by slot selector */
void Func_0808a098();           /* start a scripted motion from a script */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face the slot along a heading */

extern u8 Data_02009638[];      /* in-image motion script, file offset 0x1638 */

void Func_02000078(s32 slot)
{
    Func_0808a080(slot)[6] = 0x00010000;    /* record + 0x18 */
    Func_0808a080(slot)[7] = 0x00010000;    /* record + 0x1c */

    Func_0808a170(0x26af);
    Func_0808a180(slot, 0);
    Func_0808a1b8(slot, 0xc000, 0);
    Func_0808a010(20);
    Func_0808a098(slot, Data_02009638);
}
