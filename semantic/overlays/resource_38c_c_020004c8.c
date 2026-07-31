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
 * Resource 38c actor-27 state reset at 0x020004c8.
 *
 * Complete owner: `push {lr}` at 0x020004c8, interworking return
 * `pop {r1} / bx r1` at 0x020004f6-0x020004f8.  The control-flow walk reaches
 * every halfword to 0x020004f8; 0x020004fa-0x020004ff is a six-byte pool (one
 * alignment halfword plus 0x03001ebc).  0x02000500 is the first entry of the
 * import veneer band.  Handler pool word: 0x020084c9 = `Func_020004c8 + 1`.
 *
 * Signature.  The return address is popped into r1, NOT r0, so r0 survives
 * and IS the result; r0 is set to 0 immediately before the epilogue, so this
 * owner returns a constant 0 status.
 *
 * The displacement/value trap, additive form.  r2 is built as 224 << 1 = 448
 * and used as the workspace displacement; `adds r2, #73` turns the SAME
 * register into the stored value 521.  Reading it as a single running offset
 * is the natural mistake and nothing else in the owner would catch it.  The
 * sibling initialiser resource_386_c_020004e4.c carries the identical pair.
 *
 * The mask is written by `movs r3,#0 / subs r3,#13`, so it is -13 = 0xf3: it
 * clears the two bits 0x0c and leaves everything else, before 0x08 is set.
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


#define WORKSPACE (*(u8 **)0x03001ebc)

s32 Func_020004c8(void)
{
    u8 *actor;
    u8 *record;

    *(s32 *)(WORKSPACE + 448) = 521;

    actor = Func_0808a080(27);
    actor[0x23] = 0;

    record = *(u8 **)(actor + 0x50);
    record[9] = (u8) ((record[9] & (u8) -13) | 8);

    return 0;
}
