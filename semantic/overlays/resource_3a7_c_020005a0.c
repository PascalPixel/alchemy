/*
 * resource_3a7 scene-entity presentation step at 0x020005a0, 76 bytes.
 *
 * Complete owner: `push {r5, lr}` / `sub sp, #8` at 0x020005a0 through the
 * interworking return `add sp,#8 / pop {r5} / pop {r0} / bx r0`.  The return
 * address is popped into r0, so nothing is returned and the owner is `void`.
 * A single literal-pool word sits at 0x020005e8, inside the span and after
 * the epilogue; it is the completion flag id and is data, not code.
 *
 * One of a family of six near-identical owners in this overlay - 0x0200050c,
 * 0x02000554, 0x020005a0, 0x020005ec, 0x02000640 and 0x02000690 - found by
 * sorting the strict rows by span and eyeballing the equal sizes (72/72,
 * 76/76, 84/84 with matching `calls`).  They differ only in the scene-entity
 * selector, the two stack words of the presentation request, the completion
 * flag id, and whether the `Func_080091e0` reset is present.  Each member is
 * therefore a correctness check on the other five.
 *
 * Shape:
 *   record = Func_0808a080(18);          // scene-entity record by selector
 *   Func_080091c0(26, 30, 1, 1, 24, 34);  // six-argument request
 *   if (record != 0) { 
 *       Func_0808a080(18)[85] = 0;      // reloaded, same selector
 *       record[35] = 1;
 *   }
 *   Func_080770c8(0x202);            // raise the completion flag
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 05a0 -n 76 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 4 sites, matching the inventory's calls=4:
 *   Func_0808a080 x2 <- veneer 0x02001130
 *   Func_080091c0 x1 <- veneer 0x020010d0
 *   Func_080770c8 x1 <- veneer 0x02001108
 *
 * `Func_080770c8` is the flag SETTER: the byte-exact sibling
 * assets/code/resource_3a7_c_020003e0.c spells the same import pair as
 * `Func_020014e6` (test) / `Func_02001500` (set) around a 0x9A9 flag, and the
 * printed overlay names resolve to Func_080770c0 / Func_080770c8.  The exact
 * sibling therefore fixes the direction; the printed names in it are the
 * pre-correction spelling of the same two veneers.
 *
 * Uncertainties: the record is treated as raw bytes because only offsets 35 and
 * 85 are touched here, and the two selectors' meanings are transcribed, not
 * interpreted.  The reload of the record before the [85] store is the
 * compiler's, not a second entity - the selector immediate is identical.
 */

/* Old-style declarations: overlay imports are reached with different argument
 * counts elsewhere in this overlay, so arities are left open. */
unsigned char *Func_0808a080();  /* scene-entity record by selector */
void Func_080091c0();            /* six-argument presentation request */
void Func_080091e0();            /* per-record presentation reset (record, phase) */
void Func_080770c8();            /* raise a scene completion flag */

void Func_020005a0(void)
{
    unsigned char *record = Func_0808a080(18);

    Func_080091c0(26, 30, 1, 1, 24, 34);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_0808a080(18)[85] = 0;
        record[35] = 1;
    }

    Func_080770c8(0x202);
}
