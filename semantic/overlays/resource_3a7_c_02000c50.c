/*
 * resource_3a7 entity-group reset + task install at 0x02000c50, 80 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3a7_c_02000c08.c, its twin eight
 * bytes longer in the same overlay.  The two share the same prologue, the same
 * reset body (clear the presentation mode, then rewrite +68 / +72 / +12 on a
 * reloaded record) and the same trailing task install; they differ in the
 * selector base (11 rather than 10), the iteration count (4 rather than 6), the
 * installed callback, and one extra `Func_0808a1e0` call per iteration.  The
 * pool word 0x00001999 is byte-identical in both.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000c50 through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02000c92.  The return address is popped into r0, so
 * the owner is `void`.  The words at 0x02000c98 (0x00001999) and 0x02000c9c
 * (0x020089c1) are this owner's literal pool - past the epilogue, inside the
 * 80-byte span, and data.
 *
 * LINK BASE, second independent witness in this overlay: 0x020089c1 is handed
 * to the task installer Func_080000d0, and under the 0x02008000 base that is
 * Func_020009c0 + the Thumb bit - an unconverted strict row of this overlay.
 * (The twin's 0x02008aa1 = Func_02000aa0 + 1 is the first.)  Two odd, in-image,
 * installed pool words agreeing on the same base settle it.
 *
 * The loop counter and the selector run together: `selector` covers 11..14 and
 * the second argument block passes 11..14 as well, computed independently as
 * `index + 11`.  Both are transcribed as found rather than merged, because the
 * assembly keeps them in two registers (r6 and r5).
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0c50 -n 80 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 5 sites, matching the inventory's calls=5:
 *   Func_0808a080 x2 <- veneer 0x02001130
 *   Func_080091e0 x1 <- veneer 0x020010e0
 *   Func_0808a1e0 x1 <- veneer 0x02001148
 *   Func_080000d0 x1 <- veneer 0x02001088
 *
 * Uncertainties: the three written fields are transcribed, not interpreted (see
 * the twin's header), and Func_0808a1e0's second argument is the constant 1 at
 * every site in this overlay, so its meaning is not established.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay. */
typedef signed int s32;

s32 *Func_0808a080();   /* scene-entity record by selector */
void Func_080091e0();   /* set presentation mode (record, phase) */
void Func_0808a1e0();   /* per-entity enable, by selector */
void Func_080000d0();   /* install a per-frame task (callback, rank) */

void Func_02000c50(void)
{
    s32 selector = 11;
    s32 index = 0;

    do {
        s32 *record;

        Func_080091e0(Func_0808a080(selector), 0);

        record = Func_0808a080(selector);
        record[17] = 0x1999;        /* +68 */
        record[18] = 0;             /* +72 */
        record[3] = 0x00ff0000;     /* +12 */

        Func_0808a1e0(index + 11, 1);
        index++;
        selector++;
    } while (index <= 3);

    /* Func_020009c0 + 1 under the proven 0x02008000 base: this scene's
     * per-frame task, Thumb-tagged. */
    Func_080000d0(0x020089c1, 0xc80);   /* 200 << 4 */
}
