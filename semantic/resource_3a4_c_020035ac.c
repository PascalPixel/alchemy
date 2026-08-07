#include "types.h"

/*
 * resource_3a4 owner at 0x020035ac, 188 bytes: swing a scene record
 * through a quarter turn around the grid point one probe-length ahead
 * of it, one frame per step, with a sting at each end.
 *
 * The record layout is this package's actor record (x/y/z 16.16 words
 * at +8/+12/+16, heading halfword at +6 -- the same +8/+12/+16 triple
 * resource_3c8_c_02001f60.c types as Actor_02001f60).  The routine:
 *
 * 1. Snap the heading to its quadrant midpoint: (heading + 0x4000) &
 *    0xc000.
 * 2. Copy the record's x/y/z into a 3-word stack probe and displace it
 *    with Func_08000128(0x180000, snapped, probe) -- the established
 *    "(scale, heading, position)" displacement helper (192 << 13 = 3.0
 *    in 16.16).
 * 3. Round the displaced x and z to the nearest 16-unit grid point:
 *    (word + 0x80000) & 0xfff00000.  That pair is the pivot, held in
 *    r9/sl across the loop.
 * 4. Add 0x8000 to the snapped heading (now pointing at the pivot),
 *    mark the record with Func_08009080(record, 5), fire
 *    Func_080f9010(184).
 * 5. Sixteen steps: heading += 0x400 (16 * 0x400 = 0x4000, a quarter
 *    turn); re-seed the probe's x/z with the pivot, displace with the
 *    same Func_08000128(0x180000, heading, probe) call, write the
 *    probe's x/z back to the record's +8/+16 and `heading + 0x4000` to
 *    its +6, then Func_080000c0(1) -- one frame per step.  The probe's
 *    y word keeps whatever step 2 left there; only x and z are re-fed.
 * 6. Func_080f9010(233) and return.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves
 * at 0x020035ac, `sub sp, #12`, through the matching unwind
 * `add sp, #12 / pop {r3, r5, r6} / mov r8-sl / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x2003652-0x2003660, trailing pool word
 * 0xfff00000 at 0x2003664.  One argument (the record pointer), void.
 *
 * The park note's Func_02007xxx callee names were pc-relative
 * misdecodes; all six bl sites resolve through the veneer table with
 * the `+2` stored-displacement rule:
 *   0x20035de, 0x2003626 -> Func_08000128   displace (scale, heading, pos)
 *   0x2003602 -> Func_08009080              mark record (record, n)
 *   0x2003608, 0x200364e -> Func_080f9010   sound/sting (id)
 *   0x200363c -> Func_080000c0              wait n frames, established
 */

void Func_08000128();          /* displace (scale, heading, position) */
void Func_08009080();          /* mark record (record, n), established */
void Func_080f9010();          /* sound/sting, established */
void Func_080000c0();          /* wait n frames, established */

void Func_020035ac(u8 *record)
{
    s32 probe[3];
    s32 pivot_x;
    s32 pivot_z;
    s32 heading;
    s32 i;

    heading = (*(u16 *)(record + 6) + 0x4000) & 0xc000;

    probe[0] = *(s32 *)(record + 8);
    probe[1] = *(s32 *)(record + 12);
    probe[2] = *(s32 *)(record + 16);
    Func_08000128(0x180000, heading, probe);    /* 192 << 13 */

    pivot_x = (probe[0] + 0x80000) & 0xfff00000;
    pivot_z = (probe[2] + 0x80000) & 0xfff00000;

    heading += 0x8000;                          /* 128 << 8 */
    Func_08009080(record, 5);
    Func_080f9010(184);

    for (i = 15; i >= 0; i--) {
        heading += 0x400;                       /* 128 << 3 */
        probe[0] = pivot_x;
        probe[2] = pivot_z;
        Func_08000128(0x180000, heading, probe);
        *(s32 *)(record + 8) = probe[0];
        *(s32 *)(record + 16) = probe[2];
        *(u16 *)(record + 6) = heading + 0x4000;    /* 128 << 7 */
        Func_080000c0(1);
    }

    Func_080f9010(233);
}
