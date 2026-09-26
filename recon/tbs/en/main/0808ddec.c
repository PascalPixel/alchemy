/* DRAFT: complete [0808ddec,0808df1c), 304 bytes, including all five pools.
 * Used by trigger lookup 0808e14c and the interaction input family; returns
 * a nearby object slot, not a sprite or map-scroll owner. Baseline: 300/304
 * bytes, 99 differing halfwords, 51 aligned edits. Full listing/diff read.
 * H1: transfer exact SelectNearbyTargetObject's lookup, ordinary sqrt
 * callback, and absolute signed-angle test; share ObjectRuntime coordinates.
 * Own pool proves vertical limit 0x2fffff, not the old lift's 0x3fffff.
 * Transfer natural signed coordinate division from exact SetMoveTarget.
 * Complete model plus at most one evidence-led follow-up; stop by 01:05.
 * No adoption until complete bytes, compare/coverage/verify are exact.
 * H1 result: 300/304 bytes, 21 differing halfwords, 19 aligned edits.
 * Coordinate division, callback argument ownership, distance calculation,
 * main register roles, and vertical threshold are now exact. Remaining:
 * counter zero is hoisted across the null test; absolute-angle lowering
 * differs from the two signed bound tests and omits one pool word.
 */
#include "OBJECT_RUNTIME.H"
#include "FIXED_MATH.H"

u16 ArcTan2(s32 deltaZ, s32 deltaX);

s32 Func_0808ddec(s32 sourceId)
{
    struct ObjectRuntime *source;
    struct ObjectRuntime *candidate;
    s32 bestId;
    s32 bestDistance;
    s32 candidateId;
    s32 cellX;
    s32 cellY;
    s32 cellZ;
    s32 xSquared;
    s32 ySquared;
    s32 zSquared;
    s32 squaredDistance;
    s32 distance;
    s32 angle;

    bestId = -1;
    bestDistance = 32;
    source = ObjectTable_Get(sourceId);
    if (source == 0)
        return bestId;

    for (candidateId = 0; candidateId <= 66; candidateId++) {
        if (candidateId == sourceId)
            continue;

        candidate = ObjectTable_Get(candidateId);
        if (candidate == 0 || (candidate->unknown_56[3] & 8) != 0)
            continue;

        if (candidate->y - source->y >= 0) {
            if (candidate->y - source->y > 0x2fffff)
                continue;
        } else if (source->y - candidate->y > 0x2fffff) {
            continue;
        }

        cellX = (candidate->x - source->x) / 0x10000;
        cellY = (candidate->y - source->y) / 0x10000;
        cellZ = (candidate->z - source->z) / 0x10000;

        xSquared = cellX * cellX;
        ySquared = cellY * cellY;
        zSquared = cellZ * cellZ;
        squaredDistance = xSquared;
        squaredDistance += ySquared;
        squaredDistance += zSquared;
        distance = ((s32 (*)(s32))0x030001d8)(squaredDistance);
        if ((candidate->unknown_56[3] & 4) != 0)
            distance = Math_Div(distance * 10, 13);
        if (distance >= bestDistance)
            continue;

        angle = ArcTan2(candidate->z - source->z, candidate->x - source->x);
        if (distance > 11) {
            s32 angleDifference = (s16)(angle - source->angle);
            if (angleDifference < 0)
                angleDifference = -angleDifference;
            if (angleDifference >= 0x3000)
                continue;
        }

        bestId = candidateId;
        bestDistance = distance;
    }

    return bestId;
}
