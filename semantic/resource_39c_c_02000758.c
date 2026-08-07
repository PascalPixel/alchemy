#include "types.h"

/*
 * resource_39c owner at 0x02000758, 406 bytes through its pool and
 * boundary material: obtain a keyed slot and grid dimensions, scan a
 * rectangular set of candidate positions until the placement probe
 * reports 2, then report whether reaching that position required at
 * least one whole-grid step.
 *
 * m2c_guard independently bounds 390 code bytes from the r8-r11-saving
 * prologue at 0x02000758 through the real-value return at 0x020008dc.
 * Alignment at 0x020008de is followed by the three referenced pool words
 * at 0x020008e0-0x020008eb (0x0200dda8, 0x0200dd50, 0xffff0000) and one
 * The next owner begins at 0x020008ec by reserving stack space before its push.
 * The two bounded calls resolve to Func_02000630 and, through a veneer,
 * Func_080091d8; there is no indirect-call hazard.
 *
 * The caller's six-word result record remains positional: words 0 and 1
 * receive the keyed indices, words 2 through 4 hold the candidate
 * position, and word 5 is cleared.  The fixed tables and probe result 2
 * are recorded as observed rather than assigned broader game-level names.
 */

extern u8 *Func_02000630(s32 *referenceBucketOut, s32 *slotIndexOut,
                         s32 *keyIndexOut);
extern s32 Func_080091d8(u8 *slot, s32 *position);

static s32 Abs_02000758(s32 value)
{
    return value < 0 ? -value : value;
}

s32 Func_02000758(s32 *result)
{
    s32 permuted_27;
    s32 permuted_23;
    const s32 *const deltas = (const s32 *)0x0200dda8;
    const s32 *const referenceDeltas = (const s32 *)0x0200dd50;
    s32 referenceBucket;
    u8 *slot;
    const s32 *delta;
    s32 width;
    s32 depth;
    s32 referenceDelta;
    s32 referenceStepX;
    s32 referenceStepZ;
    s32 basePosition[3];
    s32 wholeGridSteps = 0;

    result[5] = 0;
    slot = Func_02000630(&referenceBucket, &result[1], &result[0]);
    if (slot == 0) {
        return 0;
    }

    delta = &deltas[result[0] * 4];
    depth = (Abs_02000758(delta[1]) + Abs_02000758(delta[3])) >> 4;
    width = (Abs_02000758(delta[0]) + Abs_02000758(delta[2])) >> 4;
    slot[0x22] = 2;

    permuted_23 = referenceDeltas[referenceBucket];
    referenceStepX = referenceDelta & (s32)0xffff0000;
    referenceDelta  = permuted_23;
    referenceStepZ = (s32)((u32)referenceDelta << 16);
    basePosition[0] = *(s32 *)(slot + 8) + referenceStepX;
    basePosition[1] = *(s32 *)(slot + 12);
    permuted_27 = *(s32 *)(slot + 16) + referenceStepZ;
    result[3] = basePosition[1];
    basePosition[2]  = permuted_27;

    for (;;) {
        s32 z;

        result[4] = basePosition[2] + delta[1] * 0x10000;
        for (z = 0; z < depth; z++) {
            s32 x;

            result[2] = basePosition[0] + delta[0] * 0x10000;
            for (x = 0; x < width; x++) {
                if (Func_080091d8(slot, &result[2]) == 2) {
                    goto found;
                }
                result[2] += 0x100000;
            }
            result[4] += 0x100000;
        }

        wholeGridSteps++;
        basePosition[0] += referenceStepX;
        basePosition[2] += referenceStepZ;
    }

found:
    slot[0x22] = 0;
    if (wholeGridSteps == 0) {
        return 0;
    }

    result[2] = *(s32 *)(slot + 8) + referenceStepX * wholeGridSteps;
    result[3] = *(s32 *)(slot + 12);
    result[4] = *(s32 *)(slot + 16) + referenceStepZ * wholeGridSteps;
    return 1;
}
