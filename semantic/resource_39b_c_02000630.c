#include "types.h"

extern u8 *Data_03001ebc;
extern u8 *Func_0808a080(s32 id);

/*
 * Find the first scene object whose authored footprint straddles the tile in
 * front of actor 0.  The three outputs identify the facing direction, actor
 * slot, and one of the six footprint classes.  Odd classes are X-facing and
 * even classes Z-facing, so a candidate already in the same coarse lane is
 * ignored.  Complete owner and four-word pool: 0x02000630-0x02000757.
 */
u8 *Func_02000630(s32 *directionOut, s32 *slotOut, s32 *classOut)
{
    const s32 *classCursor = (const s32 *)0x0200a740;
    const s32 *boundsCursor = (const s32 *)0x0200a758;
    const s32 *steps = (const s32 *)0x0200a700;
    u8 *leader = Func_0808a080(0);
    s32 direction = *(u16 *)(leader + 6) >> 12;
    s32 probeX = ((*(s32 *)(leader + 8) >> 16) + (steps[direction] >> 16)) >> 4;
    s32 probeZ = ((*(s32 *)(leader + 16) >> 16) + (s16)steps[direction]) >> 4;
    s32 slot;
    s32 kind;

    *directionOut = direction;
    for (slot = 8; slot <= 65; slot++) {
        u8 *object = ((u8 **)(Data_03001ebc + 0x14))[slot];
        u8 *record = *(u8 **)(object + 80);
        s16 type = *(s16 *)(record + 40);

        for (kind = 0; kind < 6; kind++) {
            const s32 *bounds = boundsCursor;

            if (*classCursor == type) {
                s32 left = (*(s16 *)(record + 10) + bounds[0]) >> 4;
                s32 top = (*(s16 *)(record + 18) + bounds[1]) >> 4;
                s32 right = (*(s16 *)(record + 10) + bounds[2]) >> 4;
                s32 bottom = (*(s16 *)(record + 18) + bounds[3]) >> 4;

                *classOut = kind;
                if (left <= probeX && probeX < right && top <= probeZ && probeZ < bottom) {
                    s32 edge = (kind & 1) != 0
                        ? (*(s32 *)(leader + 8) >> 20)
                        : (*(s32 *)(leader + 16) >> 20);
                    s32 minimum = (kind & 1) != 0 ? left : top;
                    if (minimum != edge) {
                        *slotOut = slot;
                        return object;
                    }
                }
            }

            classCursor++;
            boundsCursor += 4;
        }
    }
    return 0;
}
