typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a9 owner at 0x0200018c, 112 bytes: the interaction handler for one
 * approach direction — if the player is facing into the accepted arc, open the
 * interaction; otherwise play the refusal line.
 *
 * Role known in advance from the in-image scene-script table, no disassembly
 * needed: the three-word record `(0xffff0008, 0x0200818d, 0x00000000)` names
 * this function (0x0200818d - 0x8000 - 1 = 0x018c) as the selector-0x08 entry
 * with parameter 0.  Its near-sibling `(0xffff0008, 0x020081fd, 0x00008d15)`
 * names Func_020001fc, converted alongside it under a different condition word
 * — the two are the two halves of the same approach guard and were read
 * together.  Link-base evidence is in the header of
 * `semantic/overlays/resource_3a9_c_0200007c.c`.
 *
 * Complete owner.  Prologue `push {lr}` at 0x0200018c; single epilogue
 * `pop {r0} / bx r0` at 0x020001e4, so the popped word is the return address
 * and the owner is **void**.  Code runs 0x0200018c..0x020001e7; the literal
 * pool is 0x020001e8..0x020001fb and 0x020001fc is the next prologue.  112
 * bytes, matching the inventory row.
 *
 * All 10 call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3a9 018c` (an overlay `bl` stores target offset - 2).  Per-target:
 *   Func_0808a080 1   Func_080b0018 1   Func_0808a018 1   Func_080770c0 1
 *   Func_0808a170 2   Func_0808a180 1   Func_0808a190 1   Func_080770c8 1
 *   Func_0808a020 1
 * The two Func_0808a170 sites are on opposite arms with different line ids and
 * are NOT merged.
 *
 * THE BAND GUARD is the documented family idiom, and it is the whole point of
 * the row: `ldrh r3,[player,#6] / adds r3, 0xffff9fff / lsls #16 / cmp
 * 0x3ffe0000 / bhi` is an unsigned half-open range test on the wrapped 16-bit
 * facing word — `(u16)(facing - 0x6001) <= 0x3ffe`, i.e. facing in
 * 0x6001..0x9fff.  It is not a mask, not a sign test, and not a signed
 * comparison; the `lsls #16` is what makes the 16-bit truncation happen.  The
 * sibling at 0x020001fc uses `+ 0x5fff` against the same 0x3ffe bound, which is
 * the adjacent arc 0xa001..0xdfff — the two constants differing by 0x4000 is a
 * free check that both were read correctly.
 *
 * The one-shot pairing is explicit: the refusal arm reads flag 0x911 and the
 * first-time branch SETS flag 0x910 after its line, so the two lines
 * (0x1ad7 first, 0x1afb thereafter) alternate on a flag this owner writes
 * itself.  Note the flag read and the flag written are DIFFERENT ids — 0x911 is
 * set elsewhere in the overlay (Func_0200033c reads it too).
 *
 * Uncertainties: 0x1ad7 and 0x1afb are read as dialogue line ids from their
 * argument position on Func_0808a170, and 0x0910 / 0x0911 as event-flag ids.
 * Func_080b0018's `(7, 8)` is the documented `(id, parameter)` interaction
 * entry.  The player's facing lives at +6 of the record Func_0808a080 returns,
 * which is the same field the resource_3ae owners quantise.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();            /* scene entity by selector */
void Func_080b0018();           /* enter the interaction (id, parameter) */
void Func_0808a018();           /* open a scene bracket */
void Func_0808a020();           /* close a scene bracket */
s32 Func_080770c0();            /* read an event flag */
void Func_080770c8();           /* set an event flag */
void Func_0808a170();           /* show a dialogue line */
void Func_0808a180();
void Func_0808a190();

void Func_0200018c(void)
{
    u8 *player = Func_0808a080(0);

    /* Band guard: facing in 0x6001..0x9fff. */
    if ((u16)(*(u16 *)(player + 6) - 0x6001) > 0x3ffe) {
        Func_0808a018();

        if (Func_080770c0(0x911) != 0) {
            Func_0808a170(0x1afb);
            Func_0808a180(8, 0);
        } else {
            Func_0808a170(0x1ad7);
            Func_0808a190(8, 0);
            Func_080770c8(0x910);           /* 145 << 4 */
        }

        Func_0808a020();
    } else {
        Func_080b0018(7, 8);
    }
}
