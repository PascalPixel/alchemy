#include "types.h"




static s32 Abs_02000758(s32 value)
{
    return value < 0 ? -value : value;
}

/* Exact reviewed skeleton homolog of resource_39c:0x02000758, with the local
 * delta tables mapped to 0x0200acf8/0x0200aca0.  Complete 404-byte owner
 * through alignment and its three-word pool at 0x020008e0-0x020008eb. */
extern u8 * Func_02000da6(s32 *referenceBucketOut, s32 *slotIndexOut,
                         s32 *keyIndexOut);
extern s32 Func_0200330c(u8 *slot, s32 *position);
s32 Func_02000758(s32 *result)
{
    s32 permuted_22;
    s32 permuted_29;
    const s32 *deltas = (const s32 *)0x0200acf8;
    const s32 *referenceDeltas = (const s32 *)0x0200aca0;
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
    slot = Func_02000da6(&referenceBucket, &result[1], &result[0]);
    if (slot == 0)
        return 0;

    delta = &deltas[result[0] * 4];
    depth = (Abs_02000758(delta[1]) + Abs_02000758(delta[3])) >> 4;
    width = (Abs_02000758(delta[0]) + Abs_02000758(delta[2])) >> 4;
    slot[0x22] = 2;
    permuted_22 = referenceDeltas[referenceBucket];
    referenceStepX = referenceDelta & (s32)0xffff0000;
    referenceDelta  = permuted_22;
    referenceStepZ = (s32)((u32)referenceDelta << 16);
    basePosition[0] = *(s32 *)(slot + 8) + referenceStepX;
    basePosition[1] = *(s32 *)(slot + 12);
    permuted_29 = *(s32 *)(slot + 16) + referenceStepZ;
    result[3] = basePosition[1];
    basePosition[2]  = permuted_29;

    for (;;) {
        s32 z;
        result[4] = basePosition[2] + delta[1] * 0x10000;
        for (z = 0; z < depth; z++) {
            s32 x;
            result[2] = basePosition[0] + delta[0] * 0x10000;
            for (x = 0; x < width; x++) {
                if (Func_0200330c(slot, &result[2]) == 2)
                    goto found;
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
    if (wholeGridSteps == 0)
        return 0;
    result[2] = *(s32 *)(slot + 8) + referenceStepX * wholeGridSteps;
    result[3] = *(s32 *)(slot + 12);
    result[4] = *(s32 *)(slot + 16) + referenceStepZ * wholeGridSteps;
    return 1;
}
