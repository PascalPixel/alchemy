/*
 * resource_3a7 entity-group reset + task install at 0x02000c08, 72 bytes.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000c08 through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02000c40.  The return address is popped into r0, so
 * the owner is `void`.  Two literal-pool words sit inside the span past the
 * epilogue - 0x02000c48 (0x00001999) and 0x02000c4c (0x02008aa1); both are
 * data, not code.
 *
 * LINK BASE.  0x02008aa1 is passed to the task installer Func_080000d0.  Under
 * the overlay link base 0x02008000 that is file offset 0x02000aa1, i.e.
 * Func_02000aa0 + the Thumb bit - and 0x02000aa0 is an unconverted strict row of
 * this very overlay.  Odd word, in-image, installed as a callback: that is the
 * cheapest of the documented base witnesses, and it also NAMES 0x02000aa0 as
 * this scene's per-frame task.  Its twin at 0x02000c50 gives a second, matching
 * witness (0x020089c1 = Func_020009c0 + 1).
 *
 * Twin: 0x02000c50, 80 bytes, same six-instruction body with one extra call and
 * a different selector base.  Found by the sort-by-span scan; each checks the
 * other.
 *
 * Shape: for each of the six scene-entity records 10..15, clear the presentation
 * mode and rewrite three record fields, then install the per-frame task at
 * rank 0xc80.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0c08 -n 72 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 4 sites, matching the inventory's calls=4:
 *   Func_0808a080 x2 <- veneer 0x02001130
 *   Func_080091e0 x1 <- veneer 0x020010e0
 *   Func_080000d0 x1 <- veneer 0x02001088 (main-image task installer)
 *
 * The record is reloaded with the SAME selector between the two `Func_0808a080`
 * calls; that is the compiler not keeping the pointer across the intervening
 * call, not a second entity.
 *
 * Uncertainties: the three written fields (+68 = 0x1999, +72 = 0, +12 =
 * 0xff0000) are transcribed, not interpreted.  +12 is the same field the
 * predicate at 0x02000944 range-checks and then sets to 0xff0000, and +72 the
 * same one it clears, so the two owners agree about the layout.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay. */
typedef signed int s32;

s32 *Func_0808a080();   /* scene-entity record by selector */
void Func_080091e0();   /* set presentation mode (record, phase) */
void Func_080000d0();   /* install a per-frame task (callback, rank) */

void Func_02000c08(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Func_080091e0(Func_0808a080(selector), 0);

        record = Func_0808a080(selector);
        record[17] = 0x1999;        /* +68 */
        record[18] = 0;             /* +72 */
        remaining--;
        record[3] = 0x00ff0000;     /* +12 */
        selector++;
    } while (remaining >= 0);

    /* Func_02000aa0 + 1 under the proven 0x02008000 base: this scene's
     * per-frame task, Thumb-tagged. */
    Func_080000d0(0x02008aa1, 0xc80);   /* 200 << 4 */
}
