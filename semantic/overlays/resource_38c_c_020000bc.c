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
 * Resource 38c approach guard at 0x020000bc.
 *
 * Complete owner: `push {r5, lr}` at 0x020000bc, interworking return
 * `pop {r5} / pop {r0} / bx r0`.  The control-flow walk reaches every
 * halfword of the body; the tail of the span is the literal pool
 * (0xffff5fff, 0x00003ffe, 0x00000845 and the two message ids), reached only
 * by `ldr rN,[pc,#imm]`.  Handler pool word: 0x020080bd = `Func_020000bc + 1`.
 *
 * Signature: the return address is popped into r0, so the owner is `void`.
 *
 * The guard.  The u16 at +6 of actor record 0 is added to 0xffff5fff (that
 * is, 0xa001 is subtracted) and compared UNSIGNED against 0x3ffe, so it is a
 * half-open band test on the wrapped value.  Inside the band the scene runs
 * its transition and says nothing; outside it, one of two lines is spoken,
 * chosen by event flag 0x845.
 *
 * The two message arms are not merged, and the trailing act/end pair is
 * written once rather than copied into each arm — the assembly reaches it by
 * a `b.n` from the first arm, so copying it would inflate the per-target
 * multiset by two.
 *
 * TWIN PROOF (byte diff, not inference).  Over all 96 bytes this row and its
 * sibling at 0x0200005c differ in exactly five halfwords: the two immediates
 * 7/16 against 9/18, the actor id 16 against 18, and the two message-id pool
 * words.  Every `bl` halfword is bit-identical, so the two files are
 * transpositions of one another with the diff as their correctness proof.
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

void Func_020000bc(void)
{
    u32 place;

    place = *(u16 *)(Func_0808a080(0) + 6);
    Func_0808a018();

    if ((u32) (place + 0xffff5fff) <= 0x3ffe) {
        Func_080b0008(9, 18);
    } else {
        if (Func_080770c0(0x845) == 0) {
            Func_0808a170(0x13e9);
        } else {
            Func_0808a170(0x16f9);
        }
        Func_0808a180(18, 0);
    }

    Func_0808a020();
}
