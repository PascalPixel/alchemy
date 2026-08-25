#include "types.h"

struct BattleTargetObject {
    u8 reserved_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[69];
    u8 flags;
};

s32 Func_0808ddb8(s32 battleMode);
struct BattleTargetObject *Func_0808ba1c(s32 objectId);
s32 Func_080072f0(s32 value, s32 unused1, s32 unused2, s32 iwramRoutine);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080044d0(s32 deltaZ, s32 deltaX);

#define BattleEffect_SelectNearbyTargetObject Func_0808df1c
s32 BattleEffect_SelectNearbyTargetObject(s32 sourceId, s32 battleMode)
{
    struct BattleTargetObject *source;
    struct BattleTargetObject *candidate;
    s32 bestId;
    s32 bestDistance;
    s32 sourceFacing;
    s32 candidateId;
    s32 verticalRange;
    s32 deltaX;
    s32 deltaZ;
    s32 cellX;
    s32 cellZ;
    s32 zSquared;
    s32 xSquared;
    s32 squaredDistance;
    s32 distance;
    s32 angle;
    s32 angleTolerance;
    s32 squareRoot = 0x030001d8;

    bestId = -1;
    bestDistance = Func_0808ddb8(battleMode);
    source = Func_0808ba1c(sourceId);
    if (source == 0)
        return bestId;

    sourceFacing = (source->facing + 0x2000) & 0xc000;
    for (candidateId = 0; candidateId <= 66; candidateId++) {
        if (candidateId == sourceId)
            continue;

        candidate = Func_0808ba1c(candidateId);
        if (candidate == 0 || (candidate->flags & 8) != 0)
            continue;

        verticalRange = 0x80000;
        if (battleMode == 13)
            verticalRange = 0x3000000;
        if (battleMode == 5)
            verticalRange = 0x4000000;
        if (battleMode == 2)
            verticalRange = 0x100000;

        {
            s32 deltaY = candidate->y - source->y;
            if (deltaY >= 0) {
                if (deltaY > verticalRange)
                    continue;
            } else if (-deltaY > verticalRange) {
                continue;
            }
        }

        deltaX = candidate->x - source->x;
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;

        deltaZ = candidate->z - source->z;
        if (deltaZ < 0)
            deltaZ += 0xffff;
        cellZ = deltaZ >> 16;

        zSquared = cellZ * cellZ;
        xSquared = cellX * cellX;
        squaredDistance = xSquared;
        squaredDistance += zSquared;
        distance = Func_080072f0(squaredDistance, zSquared, xSquared,
                                 squareRoot);
        if ((candidate->flags & 0x10) != 0)
            distance = Func_080022ec(distance * 2, 3);
        if (distance >= bestDistance)
            continue;

        angle = (u16)Func_080044d0(candidate->z - source->z,
                                   candidate->x - source->x);
        angleTolerance = 0x1800;
        if (distance > 19)
            angleTolerance = 0x1000;
        if (battleMode == 2)
            angleTolerance = 0x2000;

        if (distance > 11) {
            s32 angleDifference = (s16)(angle - sourceFacing);
            if (angleDifference < 0)
                angleDifference = -angleDifference;
            if (angleDifference >= angleTolerance)
                continue;
        }

        bestId = candidateId;
        bestDistance = distance;
    }

    return bestId;
}
