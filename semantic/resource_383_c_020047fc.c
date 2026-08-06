#include "types.h"

/*
 * resource_383 owner at 0x020047fc, 68 bytes (0x020047fc-0x0200483f), all
 * code; no literal pool.  (The row is 68 bytes because the next owner starts
 * at 0x02004840; the last two bytes, 0x0200483e-0x0200483f, are the tail of
 * the `bx r1`.)
 *
 * Identical to 0x020047bc except for the proximity threshold, which is built
 * as 128 << 1 = 256 instead of the immediate 64: "is entity `who` within 16
 * units of (x, z)?".  The entity record comes from the import at veneer
 * 0x02004ec4 (main image Func_0808a400); its destination words at +56/+64
 * carry the sentinel 0x80000000 when unset, in which case the current
 * coordinates at +8/+16 are used.  Coordinates are 16.16, reduced to whole
 * units by `asrs #16` before squaring.
 *
 * `bl` target resolved with tools/overlay_call_targets.ts (target offset =
 * stored displacement + 2).  Note that this site's `bl` halfwords are
 * bit-identical to 0x020047bc's and resolve to the same veneer, which is the
 * cheapest available confirmation of the encoding rule in this overlay.
 *
 * Epilogue is `pop {r5, r6} / pop {r1} / bx r1`, so r0 is the result.
 */

/* Import veneer at 0x02004ec4 -> main image Func_0808a400. */


extern s32 * Func_020096ca();
s32 Func_020047fc(s32 x, s32 z, s32 who)
{
    s32 *entity;
    s32 ex;
    s32 ez;
    s32 dx;
    s32 dz;

    entity = Func_020096ca(who);

    ex = entity[14];                    /* +56 */
    if (ex == (s32)0x80000000) {
        ex = entity[2];                 /* +8 */
    }
    ez = entity[16];                    /* +64 */
    if (ez == (s32)0x80000000) {
        ez = entity[4];                 /* +16 */
    }

    dx = (ex - x) >> 16;
    dz = (ez - z) >> 16;

    if ((dx * dx + dz * dz) > 256) {
        return 0;
    }
    return 1;
}
