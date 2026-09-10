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

struct BattleTargetObject *Func_0808ba1c(s32 objectId);
s32 Func_080072f0(s32 value, s32 unused1, s32 unused2, s32 iwramRoutine);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080044d0(s32 deltaZ, s32 deltaX);

s32 Func_0808ddec(s32 sourceId)
{
    struct BattleTargetObject *source;
    struct BattleTargetObject *candidate;
    s32 bestId;
    s32 bestDistance;
    s32 candidateId;
    s32 deltaX;
    s32 deltaY;
    s32 deltaZ;
    s32 cellX;
    s32 cellY;
    s32 cellZ;
    s32 xSquared;
    s32 ySquared;
    s32 zSquared;
    s32 squaredDistance;
    s32 distance;
    s32 angle;
    s32 squareRoot = 0x030001d8;

    bestId = -1;
    bestDistance = 32;
    source = Func_0808ba1c(sourceId);
    if (source == 0)
        return bestId;

    for (candidateId = 0; candidateId <= 66; candidateId++) {
        if (candidateId == sourceId)
            continue;

        candidate = Func_0808ba1c(candidateId);
        if (candidate == 0 || (candidate->flags & 8) != 0)
            continue;

        if (candidate->y - source->y >= 0) {
            if (candidate->y - source->y > 0x3fffff)
                continue;
        } else if (source->y - candidate->y > 0x3fffff) {
            continue;
        }

        deltaX = candidate->x - source->x;
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;

        deltaY = candidate->y - source->y;
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;

        deltaZ = candidate->z - source->z;
        if (deltaZ < 0)
            deltaZ += 0xffff;
        cellZ = deltaZ >> 16;

        xSquared = cellX * cellX;
        ySquared = cellY * cellY;
        zSquared = cellZ * cellZ;
        squaredDistance = xSquared;
        squaredDistance += ySquared;
        squaredDistance += zSquared;
        distance = Func_080072f0(squaredDistance, zSquared, ySquared, squareRoot);
        if ((candidate->flags & 4) != 0)
            distance = Func_080022ec(distance * 10, 13);
        if (distance >= bestDistance)
            continue;

        angle = (u16)Func_080044d0(candidate->z - source->z, candidate->x - source->x);
        if (distance > 11) {
            s32 angleDifference = (s16)(angle - source->facing);
            if (angleDifference < -12287 || angleDifference > 12287)
                continue;
        }

        bestId = candidateId;
        bestDistance = distance;
    }

    return bestId;
}
