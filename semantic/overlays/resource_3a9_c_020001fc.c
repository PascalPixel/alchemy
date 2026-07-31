typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a9 owner at 0x020001fc, 68 bytes: the interaction handler for the
 * other approach direction — the near-twin of Func_0200018c.
 *
 * Role known in advance from the in-image scene-script table: the record
 * `(0xffff0008, 0x020081fd, 0x00008d15)` names this function
 * (0x020081fd - 0x8000 - 1 = 0x01fc) as a selector-0x08 entry under condition
 * word 0x8d15, where Func_0200018c is the same selector under 0x00000000.  So
 * the two are two conditioned variants of one interaction and they were read
 * together.  Link-base evidence is in the header of
 * `semantic/overlays/resource_3a9_c_0200007c.c`.
 *
 * DIFFED AGAINST ITS TWIN BEFORE BEING WRITTEN, which is what makes the arc
 * constants trustworthy: both owners load the facing at +6 of Func_0808a080(0),
 * bias it, shift left 16 and compare unsigned against the SAME 0x3ffe0000
 * bound.  Func_0200018c biases by 0xffff9fff (arc 0x6001..0x9fff); this one
 * biases by +0x5fff (arc 0xa001..0xdfff).  The two arcs are adjacent and the
 * biases differ by exactly 0x4000, so each is a check on the other.  It is a
 * 16-bit windowed range test, not a mask and not a signed comparison.
 *
 * What is DIFFERENT from the twin and is preserved: this copy has no flag test
 * and no flag set — one line, unconditionally — and it calls Func_080b0010(8)
 * where the twin calls Func_080b0018(7, 8).  Six call sites against the twin's
 * ten.
 *
 * Complete owner.  Prologue `push {lr}` at 0x020001fc; single epilogue
 * `pop {r0} / bx r0` at 0x02000230, so the popped word is the return address
 * and the owner is **void**.  Code runs 0x020001fc..0x02000233; the literal
 * pool is 0x02000234..0x0200023f and 0x02000240 is the next prologue.  68
 * bytes, matching the inventory row.
 *
 * All 6 call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3a9 01fc`: Func_0808a080 1, Func_080b0010 1, Func_0808a018 1,
 * Func_0808a170 1, Func_0808a180 1, Func_0808a020 1.
 *
 * Uncertainty: 0x1a8f is read as a dialogue line id from its argument position
 * on Func_0808a170, exactly as 0x1ad7 and 0x1afb are in the twin.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();            /* scene entity by selector */
void Func_080b0010();           /* enter the interaction */
void Func_0808a018();           /* open a scene bracket */
void Func_0808a020();           /* close a scene bracket */
void Func_0808a170();           /* show a dialogue line */
void Func_0808a180();

void Func_020001fc(void)
{
    u8 *player = Func_0808a080(0);

    /* Band guard: facing in 0xa001..0xdfff. */
    if ((u16)(*(u16 *)(player + 6) + 0x5fff) > 0x3ffe) {
        Func_0808a018();
        Func_0808a170(0x1a8f);
        Func_0808a180(8, 0);
        Func_0808a020();
    } else {
        Func_080b0010(8);
    }
}
