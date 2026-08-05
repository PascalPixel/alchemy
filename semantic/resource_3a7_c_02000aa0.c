/*
 * resource_3a7 per-frame scene task at 0x02000aa0, 180 bytes.
 *
 * This is the task the owner at 0x02000c08 installs: its pool word 0x02008aa1
 * is Func_02000aa0 + the Thumb bit under the proven 0x02008000 link base.  The
 * install therefore names this row, and this row's own even in-image pool words
 * (0x0200a22c, 0x0200a230) confirm the same base from the other direction -
 * they resolve to file offsets 0x222c and 0x2230, inside the image.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000aa0 through `pop {r5, r6,
 * r7} / pop {r0} / bx r0` at 0x02000b3a.  The return address is popped into r0,
 * so the owner is `void` - which is what a per-frame task must be.  Five
 * literal-pool words sit inside the span past the epilogue (0x02000b40 ..
 * 0x02000b50); the listing decodes several of them as plausible instructions,
 * but the control-flow walk reaches none of them: 0x02000b3a is the only exit
 * and the loop's back edge at 0x02000b38 targets 0x02000ae4.
 *
 * Shape, per frame:
 *   - bail out entirely while byte +91 of record 10 is non-zero (a suspend
 *     flag);
 *   - bump the in-image frame counter at 0x0200a22c, and every 64th frame pick
 *     a fresh random member of the group (`helper(random, 6)` -> 0..5, stored at
 *     0x0200a230, selector = that + 10) and set its +72 to 0x0a3d;
 *   - then sweep the six records 10..15.  For each, test scene flag
 *     (0x200 + index) - the same 0x200..0x205 flags the presentation family at
 *     0x0200050c..0x02000690 raises - and, when the record's +40 is positive or
 *     its +12 has passed a threshold, reset +12 to 0xff0000 with +40/+72 and
 *     wait 106 frames.
 *
 * The two arms of that test are BEHAVIOURALLY IDENTICAL apart from the +12
 * threshold (0x0020ffff when the flag is set, 0x0000ffff when it is clear) and
 * the value written to +40/+72 (0 either way - in the clear arm it is written
 * from the flag test's own zero result).  They are nevertheless two distinct
 * call sites of Func_080f9010, and are written out twice on purpose: merging
 * behaviourally identical arms deflates the per-target multiset.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0aa0 -n 180 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 8 sites, matching the inventory's calls=8:
 *   Func_0808a080 x3 <- veneer 0x02001130
 *   Func_080000f8 x1 <- veneer 0x02001090  (random source, no arguments)
 *   Func_030003e0 x1 <- veneer 0x02001080  (relocated IWRAM arithmetic helper)
 *   Func_080770c0 x1 <- veneer 0x02001100  (test a scene flag)
 *   Func_080f9010 x2 <- veneer 0x02001150  (scripted delay, 106 frames)
 *
 * Uncertainties:
 *  - Func_030003e0 is a relocated IWRAM helper reached through an ordinary
 *    veneer, in the same family as the documented 0x03000380 (divide) and
 *    0x030003ac (modulo).  Its result is used as an index 0..5 with a divisor of
 *    6, so it behaves as a remainder here; that is inference from the use, not
 *    from the helper's own code, so it is left named by address.
 *  - the record fields (+12, +40, +72, +91) are transcribed, not interpreted.
 *    +12 and +72 are the same fields 0x02000944 and 0x02000c08 write, and the
 *    0xff0000 value is shared with both, so the layout is cross-checked.
 *  - `Func_080f9010` inside a per-frame task reads as a blocking wait; how the
 *    task yields is a property of the scheduler, not of this row.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay.  The flag test needs a return type. */
#include "types.h"

u8 *Func_0808a080();    /* scene-entity record by selector */
s32 Func_080000f8();    /* random source, no arguments */
s32 Func_030003e0();    /* relocated IWRAM arithmetic helper (see above) */
s32 Func_080770c0();    /* test a scene completion flag */
void Func_080f9010();   /* scripted delay, in frames */

/* In-image data at file offsets 0x222c and 0x2230 (pool words 0x0200a22c and
 * 0x0200a230 under the proven 0x02008000 link base).  The overlay image is
 * writable EWRAM, so these are ordinary mutable cells. */
extern s32 Data_0200a22c;   /* frame counter */
extern s32 Data_0200a230;   /* last randomly chosen group member, 0..5 */

void Func_02000aa0(void)
{
    u8 *record = Func_0808a080(10);
    s32 counter;
    s32 index;

    if (record[91] != 0) {
        return;
    }

    counter = Data_0200a22c + 1;
    Data_0200a22c = counter;

    if ((counter & 63) == 0) {
        s32 pick = Func_030003e0(Func_080000f8(), 6);

        Data_0200a230 = pick;
        *(s32 *)(Func_0808a080(pick + 10) + 72) = 0x0a3d;
    }

    index = 0;
    do {
        u8 *entry = Func_0808a080(index + 10);

        if (Func_080770c0(index + 0x200) != 0) {
            if (*(s32 *)(entry + 40) > 0
                || *(s32 *)(entry + 12) <= 0x0020ffff) {
                *(s32 *)(entry + 72) = 0;
                *(s32 *)(entry + 12) = 0x00ff0000;
                *(s32 *)(entry + 40) = 0;
                Func_080f9010(106);
            }
        } else {
            /* Same effect as the arm above with a lower threshold; the zero
             * stored here is the flag test's own result register.  Kept as a
             * separate call site rather than merged. */
            if (*(s32 *)(entry + 40) > 0
                || *(s32 *)(entry + 12) <= 0x0000ffff) {
                *(s32 *)(entry + 72) = 0;
                *(s32 *)(entry + 40) = 0;
                *(s32 *)(entry + 12) = 0x00ff0000;
                Func_080f9010(106);
            }
        }

        index++;
    } while (index <= 5);
}
