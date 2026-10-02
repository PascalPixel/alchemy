#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "OBJECT_LOOKUP.H"
#include "SCENE.H"
#include "EFFECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "OBJECT_EFFECT.H"
#include "SYSTEM.H"

extern s16 BattleFx_TargetRangeByMode[];

u16 ArcTan2(s32 deltaZ, s32 deltaX);

struct BattleTargetObject {
    u8 reserved_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[69];
    u8 flags;
};

s32 BattleFx_MapKeyThroughTable(s32 battleMode);

s32 BattleFx_StartRandomParticleEmitter(s32, s32);

/* battle/effects/play_cue_and_start_emitter_on_target.c */
extern void Object_SetMode(struct ParticleEffectObject *, s32);
s32 Object_GetById(u32);
s32 Audio_PlayCue(s32);

s32 BattleFx_MapKeyThroughTable(s32 key)
{
    s16 *entry = BattleFx_TargetRangeByMode;
    s32 result = 16;
    s32 current = *entry;

    while (current != -1) {
        ++entry;
        if (key == current) {
            result = *entry;
            break;
        }
        ++entry;
        current = *entry;
    }
    return result;
}

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
        if (candidate == 0 || (candidate->unknown_59 & 8) != 0)
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
        if ((candidate->unknown_59 & 4) != 0)
            distance = distance * 10 / 13;
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
    s32 cellZ;
    s32 distance;
    s32 angle;
    s32 angleTolerance;

    bestId = -1;
    bestDistance = BattleFx_MapKeyThroughTable(battleMode);
    source = (struct BattleTargetObject *)ObjectTable_Get(sourceId);
    if (source == 0)
        return bestId;

    sourceFacing = (source->facing + 0x2000) & 0xc000;
    for (candidateId = 0; candidateId <= 66; candidateId++) {
        if (candidateId == sourceId)
            continue;

        candidate = (struct BattleTargetObject *)ObjectTable_Get(candidateId);
        if (candidate == 0 || (candidate->flags & 8) != 0)
            continue;

        verticalRange = 0x80000;
        if (battleMode == 13)
            verticalRange = 0x300000;
        if (battleMode == 5)
            verticalRange = 0x400000;
        if (battleMode == 2)
            verticalRange = 0x100000;

        {
            s32 deltaY = candidate->y - source->y;
            if (deltaY >= 0) {
                if (deltaY > verticalRange)
                    continue;
            } else if (source->y - candidate->y > verticalRange) {
                continue;
            }
        }

        deltaX = candidate->x - source->x;
        if (deltaX < 0)
            deltaX += 0xffff;
        deltaX >>= 16;

        deltaZ = candidate->z - source->z;
        if (deltaZ < 0)
            deltaZ += 0xffff;
        cellZ = deltaZ >> 16;

        distance = Iwram_Sqrt(deltaX * deltaX + cellZ * cellZ);
        if ((candidate->flags & 0x10) != 0)
            distance = distance * 2 / 3;
        if (distance >= bestDistance)
            continue;

        angle = (u16)ArcTan2(candidate->z - source->z,
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

s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
    s32 object;
    s32 result;

    object = Object_GetById(target);
    result = 0;
    if (object != 0) {
        Audio_PlayCue(0x7C);
        Object_SetMode(object, 4);
        WaitFrames(0xC);
        result = BattleFx_StartRandomParticleEmitter(effect, mode);
    }
    return result;
}
