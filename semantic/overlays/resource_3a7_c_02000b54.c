/*
 * resource_3a7 step-and-place helper at 0x02000b54, 54 bytes.
 *
 * Complete owner: `push {r5, r6, lr} / sub sp, #12` at 0x02000b54 through
 * `add sp,#12 / pop {r5, r6} / pop {r0} / bx r0` at 0x02000b82.  The return
 * address is popped into r0, so the owner is `void`.  No literal pool inside
 * the span; the halfword at 0x02000b8a is alignment before the next prologue.
 *
 * Three arguments arrive in r0-r2 and are used as: r0 is the record (null
 * checked, so it is a pointer), r1 and r2 are passed through unchanged as the
 * first two arguments of Func_08000128.  The 12-byte stack block is a scratch
 * (x, y, z) triple: it is filled from the record's +8/+12/+16 words, handed to
 * Func_08000128 as its third argument, and read back afterwards - so
 * Func_08000128 UPDATES the block in place.  The updated triple is then handed
 * to Func_08009150 together with the record.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0b54 -n 54 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 2 sites, matching the inventory's calls=2:
 *   Func_08000128 x1 <- veneer 0x02001098   (advance a position block)
 *   Func_08009150 x1 <- veneer 0x020010b8   (place a record at (x, y, z))
 *
 * The record layout - 16.16 X at +8, Y at +12, Z at +16 - is the same one the
 * byte-exact sibling assets/code/resource_3a7_c_020006e4.c reads (+12 there) and
 * that this overlay's 0x02000458 tile predicate uses (+8 and +16).
 *
 * Its caller 0x02000b8c invokes it as Func_02000b54(record, 0x200000, distance),
 * which is consistent with r1 being a fixed-point magnitude and r2 a heading.
 * That reading is not proven here, so the parameters are left neutral.
 *
 * Uncertainty: whether Func_08000128 reads all three words of the block or only
 * two is not established - all three are written before the call and all three
 * read after it, so all three are preserved.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay. */
typedef signed int s32;

void Func_08000128();   /* advance a position block one step */
void Func_08009150();   /* place a record at (x, y, z) */

void Func_02000b54(s32 *record, s32 arg1, s32 arg2)
{
    s32 position[3];

    if (record == 0) {
        return;
    }

    position[0] = record[2];    /* +8  */
    position[1] = record[3];    /* +12 */
    position[2] = record[4];    /* +16 */

    Func_08000128(arg1, arg2, position);

    Func_08009150(record, position[0], position[1], position[2]);
}
