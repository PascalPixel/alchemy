typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 395 per-frame effect step at 0x0200152c (116 bytes,
 * 0x0200152c-0x0200159f).
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp, #12` prologue (the `sub sp`
 * is delayed to 0x02001538, after r0 has been copied to r6) and the matching
 * `add sp, #12 / pop {r5, r6, r7} / pop {r0} / bx r0` interworking return at
 * 0x02001594.  The popped return address lands in r0, so nothing is returned
 * and the owner is `void` — note that this is a per-frame step whose sibling
 * 0x020011e8 (byte-exact, in assets/code) does return 1; this one does not.
 * The single pool word (0x00000147) sits at 0x0200159c, after the epilogue.
 *
 * All three call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), not from overlay_show.ts's
 * `bl` annotations, which are off by one veneer entry on this overlay.  Placed
 * multiset, matching the three sites exactly: Func_08000128, Func_080001b8,
 * Func_080090d0, one each.  The inventory reports calls=3.
 *
 * Frame layout: the same record the byte-exact assets/code/
 * resource_395_c_020011e8.c models — s32 words at +0x18 and +0x38/+0x3c/+0x40.
 * This owner adds +0x08/+0x0c/+0x10 (the live position written back), +0x1c
 * (the second angle), +0x50 (a pointer whose byte at +0x1c is released) and the
 * halfword pair at +0x64/+0x66.
 *
 * Behaviour: for the first 120 steps, take the anchor position (+0x38/+0x3c/
 * +0x40), advance it `step << 16` along heading `step * 768 + base_heading`
 * with Func_08000128, and store the result as the live position; then bump both
 * angle words by 0x147 and increment the step counter.  On step 120 and after,
 * release the handle byte at *(+0x50) + 0x1c and tear the record down.
 *
 * Uncertainties, both noted rather than smoothed over:
 *  - the counter at +0x64 is compared with `ldrsh` (signed) but incremented
 *    through `ldrh`/`strh` (unsigned).  It is modelled as s16 with the
 *    increment written through a u16 view, which reproduces both.
 *  - Func_08000128's argument block is the three-word stack copy; whether the
 *    callee reads all three words or only writes them is not established, so
 *    the copy-in and copy-out are both preserved.
 */

struct Step_0200152c {
    s32 pad00[2];
    s32 x;              /* +0x08 */
    s32 y;              /* +0x0c */
    s32 z;              /* +0x10 */
    s32 pad14;
    s32 angle0;         /* +0x18 */
    s32 angle1;         /* +0x1c */
    s32 pad20[6];
    s32 anchorX;        /* +0x38 */
    s32 anchorY;        /* +0x3c */
    s32 anchorZ;        /* +0x40 */
    s32 pad44[3];
    u8 *owner;          /* +0x50 */
    s32 pad54[4];
    s16 step;           /* +0x64 */
    s16 heading;        /* +0x66 */
};

void Func_08000128();           /* advance the position block one step along a heading */
void Func_080001b8();           /* release the handle identified by a byte id */
void Func_080090d0();           /* tear down the record */

void Func_0200152c(struct Step_0200152c *frame)
{
    s32 position[3];
    s32 step;

    step = frame->step;
    if (step > 119) {
        Func_080001b8(frame->owner[0x1c]);
        Func_080090d0(frame);
        return;
    }

    position[0] = frame->anchorX;
    position[1] = frame->anchorY;
    position[2] = frame->anchorZ;

    /* r1 = (step * 2 + step) << 8, i.e. step * 768, plus the base heading. */
    Func_08000128(step << 16, step * 768 + frame->heading, position);

    frame->x = position[0];
    frame->y = position[1];
    frame->z = position[2];

    frame->angle0 += 0x147;
    frame->angle1 += 0x147;

    *(u16 *)&frame->step = (u16)(*(u16 *)&frame->step + 1);
}
