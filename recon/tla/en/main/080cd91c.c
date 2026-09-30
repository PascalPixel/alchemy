#include "OBJECT_RUNTIME.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"

u16 ArcTan2(s32 deltaZ, s32 deltaX);

/* The nearest of the 67 objects to SOURCEID's object, other than itself, by
   whole-unit distance under 32: objects flagged 8 are skipped, objects
   flagged 4 count as ten thirteenths as far, the vertical gap may not exceed
   0x2fffff, and beyond 11 units the object must lie within 0x2fff of the
   source's facing. Returns its id, or -1. */

s32 BattleEffect_SelectNearbyObject(s32 sourceId)
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
        goto done;

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
        squaredDistance = xSquared + ySquared + zSquared;
        distance = Iwram_Sqrt(squaredDistance);
        if ((candidate->unknown_56[3] & 4) != 0)
            distance = Math_Div(distance * 10, 13);
        if (distance >= bestDistance)
            continue;

        angle = ArcTan2(candidate->z - source->z, candidate->x - source->x);
        if (distance > 11) {
            s32 angleDifference = (s16)(angle - source->angle);

            if (angleDifference < -0x2fff)
                continue;
            if (angleDifference > 0x2fff)
                continue;
        }

        bestId = candidateId;
        bestDistance = distance;
    }

done:
    return bestId;
}
