/// @file
/// Part of object.c, the field object, compiled only through it.
/// The actor a field action aims at, and the helpers that follow it.

#include "../object_internal.h"

/// Returns how far, in pixels, a field Psynergy of the given kind reaches:
/// gBattleFx_TargetRangeByMode pairs kinds with ranges, ended by -1, and a kind
/// not listed reaches 16 pixels.
s32 Field_GetPsynergyTargetRange(s32 kind) {
  s16 *rangeEntry = gBattleFx_TargetRangeByMode;
  s32 range = 16;
  s32 listedKind = *rangeEntry;

  while (listedKind != -1) {
    ++rangeEntry;
    if (kind == listedKind) {
      range = *rangeEntry;
      break;
    }
    ++rangeEntry;
    listedKind = *rangeEntry;
  }
  return range;
}

/// Returns the actor nearest to actor id that it could talk to, or -1: within
/// 48 pixels of height and 32 pixels of distance and, beyond 11 pixels, within
/// 0x2fff of the direction it faces. Actors with flag 8 are skipped, and those
/// with flag 4 count as 10/13 as far.
s32 Field_FindNearbyTalkTarget(s32 id) {
  struct ObjectRuntime *source;
  struct ObjectRuntime *candidate;
  s32 nearestActor;
  s32 nearestDistance;
  s32 actorIndex;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 squaredX;
  s32 squaredY;
  s32 squaredZ;
  s32 squaredDistance;
  s32 distance;
  s32 angle;

  nearestActor = -1;
  nearestDistance = 32;
  source = ObjectTable_Get(id);
  if (source != 0) {
    for (actorIndex = 0; actorIndex <= OBJ_LAST; actorIndex++) {
      if (actorIndex == id)
        continue;

      candidate = ObjectTable_Get(actorIndex);
      if (candidate == 0 || (candidate->unk59 & 8) != 0)
        continue;

      if (candidate->y - source->y >= 0) {
        if (candidate->y - source->y >= 48 << 16)
          continue;
      } else if (source->y - candidate->y >= 48 << 16) {
        continue;
      }

      // The distance in pixels.
      dx = (candidate->x - source->x) / 0x10000;
      dy = (candidate->y - source->y) / 0x10000;
      dz = (candidate->z - source->z) / 0x10000;

      squaredX = dx * dx;
      squaredY = dy * dy;
      squaredZ = dz * dz;
      squaredDistance = squaredX + squaredY + squaredZ;
      distance = Iwram_Sqrt(squaredDistance);
      if ((candidate->unk59 & 4) != 0)
        distance = distance * 10 / 13;
      if (distance >= nearestDistance)
        continue;

      angle = (u16)ArcTan2(candidate->z - source->z, candidate->x - source->x);
      if (distance > 11) {
        s32 angleDifference = (s16)(angle - source->angle);

        if (angleDifference < -0x2fff)
          continue;
        if (angleDifference > 0x2fff)
          continue;
      }

      nearestActor = actorIndex;
      nearestDistance = distance;
    }
  }
  return nearestActor;
}

/// Returns the actor nearest in front of actor id that a field Psynergy of the
/// given kind can reach, or -1. The direction is rounded to a quarter turn and
/// the distance measured on the ground; kind sets the range, the height
/// allowed and the width of the cone.
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind) {
  struct ObjectRuntime *source;
  struct ObjectRuntime *candidate;
  s32 nearestActor;
  s32 nearestDistance;
  s32 facingQuadrant;
  s32 actorIndex;
  s32 heightLimit;
  s32 dx;
  s32 dz;
  s32 pixelZ;
  s32 distance;
  s32 angle;
  s32 coneHalfAngle;

  nearestActor = -1;
  nearestDistance = Field_GetPsynergyTargetRange(kind);
  source = (struct ObjectRuntime *)ObjectTable_Get(id);
  if (source == 0)
    return nearestActor;

  facingQuadrant = (source->angle + 0x2000) & 0xc000;
  for (actorIndex = 0; actorIndex <= OBJ_LAST; actorIndex++) {
    if (actorIndex == id)
      continue;

    candidate = (struct ObjectRuntime *)ObjectTable_Get(actorIndex);
    if (candidate == 0 || (candidate->unk59 & 8) != 0)
      continue;

    // The height allowed: 8 pixels, or 48 for Catch, 64 for Douse, 16 for
    // Mind Read.
    heightLimit = 8 << 16;
    if (kind == FIELD_KIND_CATCH)
      heightLimit = 48 << 16;
    if (kind == FIELD_KIND_DOUSE)
      heightLimit = 64 << 16;
    if (kind == FIELD_KIND_MIND_READ)
      heightLimit = 16 << 16;

    {
      s32 dy = candidate->y - source->y;
      if (dy >= 0) {
        if (dy > heightLimit)
          continue;
      } else if (source->y - candidate->y > heightLimit) {
        continue;
      }
    }

    // The distance in whole pixels, rounded toward 0.
    dx = candidate->x - source->x;
    if (dx < 0)
      dx += 0xffff;
    dx >>= 16;

    dz = candidate->z - source->z;
    if (dz < 0)
      dz += 0xffff;
    pixelZ = dz >> 16;

    distance = Iwram_Sqrt(dx * dx + pixelZ * pixelZ);
    if ((candidate->unk59 & 0x10) != 0)
      distance = distance * 2 / 3;
    if (distance >= nearestDistance)
      continue;

    // The cone: 0x1800 either side up to 19 pixels, 0x1000 beyond; 0x2000
    // for Mind Read.
    angle = (u16)ArcTan2(candidate->z - source->z, candidate->x - source->x);
    coneHalfAngle = 0x1800;
    if (distance > 19)
      coneHalfAngle = 0x1000;
    if (kind == FIELD_KIND_MIND_READ)
      coneHalfAngle = 0x2000;

    if (distance > 11) {
      s32 angleDifference = (s16)(angle - facingQuadrant);
      if (angleDifference < 0)
        angleDifference = -angleDifference;
      if (angleDifference >= coneHalfAngle)
        continue;
    }

    nearestActor = actorIndex;
    nearestDistance = distance;
  }

  return nearestActor;
}

/// If actor target exists, plays sound 124, gives it animation 4 and, 12 frames
/// later, shows itemId in front of actor with sparkles. Returns the picture's
/// encoded actor pointer, or 0 when the target or picture is absent.
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 actor, s32 target, s32 itemId) {
  void *targetActor;
  s32 emitter;

  targetActor = Object_GetById(target);
  emitter = 0;
  if (targetActor != 0) {
    AudioCommand_PlayFar(SE_124);
    ObjectDispatch_SelectAnimationFar(targetActor, 4);
    WaitFrames(12);
    emitter = (s32)BattleFx_StartRandomParticleEmitter(actor, itemId);
  }
  return emitter;
}

/// Gives every part of a motion object's sprite (but those at palette 15)
/// palette - 1, or for palette 0 the next palette of a cycle that changes every
/// 2 frames.
void Animation_ApplyChildPalette(struct MotionObject *object, s32 palette) {
  struct AnimationObject *sprite;
  struct AnimationEntry **entrySlot;
  struct AnimationEntry *entry;
  s32 remainingParts;
  s32 replacement;
  u8 partCount;

  if ((object->recordStorageKind & 15) == 1) {
    sprite = object->records;
    replacement = palette - 1;
    if (palette == 0)
      replacement = gAnimation_ChildPaletteCycle[(gFrameCount >> 1) & 7];

    partCount = sprite->count;
    if (partCount != 0) {
      entrySlot = sprite->entries;
      remainingParts = partCount;
      do {
        entry = *entrySlot++;
        if (entry != 0 && entry->script != 0 &&
            entry->param != SPRITE_COLOR_HIDDEN)
          entry->param = replacement;
        remainingParts--;
      } while (remainingParts != 0);
    }
    sprite->dirty = 1;
  }
}

/// Clears the field's busy flag and, while Reveal is active, ends it: Reveal
/// used with ACT_PACKED runs its ending events.
void Battle_ResetEffectCounter(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;

  work->busy = 0;
  if (work->revealActive != 0) {
    Field_RunPsynergyEvents(ABILITY_REVEAL | ACT_PACKED);
  }
}
