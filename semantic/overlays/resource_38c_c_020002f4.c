typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Link base 0x02008000, proven against banked byte-exact material: the
 * overlay pools the odd words 0x02008045, 0x020082b5, 0x020082d5,
 * 0x020083f5 and 0x02008471, which under `offset = value - 0x8000` are
 * Func_02000044/02b4/02d4/03f4/0470 plus the Thumb bit — and all five of
 * those addresses have byte-exact sources in assets/code.  Every strict row
 * of this overlay is named the same way; this one appears as the pool word
 * noted below.
 *
 * Import naming.  An overlay `bl` stores `target_offset - 2`, so each site is
 * resolved as `stored_displacement + 2` into the eight-byte veneer band at
 * 0x02000500-0x02000598 and named for the main-image address in the veneer's
 * trailing word.  The disassembler's own `bl` annotations are wrong for every
 * site in this overlay, and the banked byte-exact siblings above were written
 * with those wrong names — they invert consistently through the +2 rule.
 */

/*
 * Resource 38c approach guard at 0x020002f4 (actor 19).
 *
 * Complete owner: `push {r5, lr}` at 0x020002f4, interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000342-0x02000346.  The control-flow
 * walk reaches every halfword to 0x02000346; 0x02000348-0x0200035b is the
 * literal pool (0xffff5fff, 0x00003ffe, 0x00000845, 0x000016fb, 0x000013eb).
 * Handler pool word: 0x020082f5 = `Func_020002f4 + 1`.
 *
 * Signature: `pop {r0} / bx r0` — `void`.
 *
 * Same band guard as 0x0200005c, but note the POLARITY: here the `beq` sends
 * the flag-clear case to the second arm, so the 0x845 test reads `!= 0` for
 * the first line and `== 0` for the second — the reverse of 0x0200005c.  The
 * guard appears with both polarities in this overlay; read the comparison at
 * each site rather than assuming one.  The arms also end differently
 * (Func_0808a190 against Func_0808a180), which is why they are not merged.
 */

/* Old-style declarations: several of these imports are reached with
 * different argument counts at different sites in this overlay. */
void Func_0808a010();  /* veneer 0x510 */
void Func_0808a018();  /* veneer 0x518 — scene begin */
void Func_0808a020();  /* veneer 0x520 — scene end */
s32  Func_0808a070();  /* veneer 0x530 */
u8  *Func_0808a080();  /* veneer 0x538 — actor record accessor */
void Func_0808a130();  /* veneer 0x540 */
void Func_0808a148();  /* veneer 0x548 */
void Func_0808a170();  /* veneer 0x550 — message */
void Func_0808a178();  /* veneer 0x558 */
void Func_0808a180();  /* veneer 0x560 — act */
void Func_0808a188();  /* veneer 0x568 */
void Func_0808a190();  /* veneer 0x570 */
void Func_0808a1b8();  /* veneer 0x578 */
void Func_0808a1f0();  /* veneer 0x580 */
s32  Func_080770c0();  /* veneer 0x500 — event-flag test */
void Func_080770c8();  /* veneer 0x508 — event-flag set */
void Func_080b0008();  /* veneer 0x588 */
void Func_080b0018();  /* veneer 0x590 */


void Func_020002f4(void)
{
    u32 place;

    place = *(u16 *)(Func_0808a080(0) + 6);
    Func_0808a018();

    if ((u32) (place + 0xffff5fff) <= 0x3ffe) {
        Func_080b0018(2, 19);
    } else if (Func_080770c0(0x845) != 0) {
        Func_0808a170(0x16fb);
        Func_0808a190(19, 0);
    } else {
        Func_0808a170(0x13eb);
        Func_0808a180(19, 0);
    }

    Func_0808a020();
}
