/// @file
/// The actor a field Psynergy or item aims at, and the helpers that follow it.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// Returns how far, in pixels, a field Psynergy of the given kind reaches:
/// gFieldEffect_TargetRangeByMode (field_sequence_tables.c) pairs kinds with
/// ranges, ended by -1, and a kind not listed reaches 16 pixels.
s32 Field_GetPsynergyTargetRange(s32 kind) {
  const s16 *rangeEntry = gFieldEffect_TargetRangeByMode;
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
/// 0x2fff (about 67 degrees) of the direction it faces.
s32 Field_FindNearbyTalkTarget(s32 id) {
  struct Actor *source;
  struct Actor *candidate;
  s32 nearestActorId;
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

  nearestActorId = -1;
  nearestDistance = 32;
  source = Actor_Lookup(id);
  if (source != 0) {
    // BUG: one past the actor table: Actor_Lookup(ACTOR_ID_COUNT) is the
    // first treasure spot's actor, so the map's first chest or Psynergy Stone
    // is a candidate too.
#ifdef BUGFIX
    for (actorIndex = 0; actorIndex < ACTOR_ID_COUNT; actorIndex++) {
#else
    for (actorIndex = 0; actorIndex <= ACTOR_ID_COUNT; actorIndex++) {
#endif
      if (actorIndex == id)
        continue;

      candidate = Actor_Lookup(actorIndex);
      if (candidate == 0 || (candidate->collisionFlags & 8) != 0)
        continue;

      // y is height: an actor more than 48 pixels above or below is out
      // of reach.
      if (candidate->y.fixed - source->y.fixed >= 0) {
        if (candidate->y.fixed - source->y.fixed >= px(48))
          continue;
      } else if (source->y.fixed - candidate->y.fixed >= px(48)) {
        continue;
      }

      // The distance in pixels.
      dx = (candidate->x.fixed - source->x.fixed) / Q16_ONE;
      dy = (candidate->y.fixed - source->y.fixed) / Q16_ONE;
      dz = (candidate->z.fixed - source->z.fixed) / Q16_ONE;

      squaredX = dx * dx;
      squaredY = dy * dy;
      squaredZ = dz * dz;
      squaredDistance = squaredX + squaredY + squaredZ;
      distance = Iwram_Sqrt(squaredDistance);
      if ((candidate->collisionFlags & 4) != 0)
        distance = distance * 10 / 13;
      if (distance >= nearestDistance)
        continue;

      // Close actors count from any side; further ones must be within
      // 0x2fff (about 67 degrees) of where the actor faces.
      angle = (u16)ArcTan2(candidate->z.fixed - source->z.fixed,
                           candidate->x.fixed - source->x.fixed);
      if (distance > 11) {
        s32 angleDifference = (s16)(angle - source->facing);

        if (angleDifference < -0x2fff)
          continue;
        if (angleDifference > 0x2fff)
          continue;
      }

      nearestActorId = actorIndex;
      nearestDistance = distance;
    }
  }
  return nearestActorId;
}

/// Returns the actor nearest in front of actor id that a field Psynergy of the
/// given kind can reach, or -1.
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind) {
  struct Actor *source;
  struct Actor *candidate;
  s32 nearestActorId;
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

  nearestActorId = -1;
  nearestDistance = Field_GetPsynergyTargetRange(kind);
  source = Actor_Lookup(id);
  if (source == 0)
    return nearestActorId;

  // The facing rounded to the nearest of the four quarter turns.
  facingQuadrant = (source->facing + TRIG_TURN / 8) & 0xc000;
  // BUG: one past the actor table: Actor_Lookup(ACTOR_ID_COUNT) is the first
  // treasure spot's actor, so the map's first chest or Psynergy Stone is a
  // candidate too.
#ifdef BUGFIX
  for (actorIndex = 0; actorIndex < ACTOR_ID_COUNT; actorIndex++) {
#else
  for (actorIndex = 0; actorIndex <= ACTOR_ID_COUNT; actorIndex++) {
#endif
    if (actorIndex == id)
      continue;

    candidate = Actor_Lookup(actorIndex);
    if (candidate == 0 || (candidate->collisionFlags & 8) != 0)
      continue;

    // The height allowed: 8 pixels, or 48 for Catch, 64 for Douse, 16 for
    // Mind Read.
    heightLimit = px(8);
    if (kind == FIELD_PSYNERGY_CATCH)
      heightLimit = px(48);
    if (kind == FIELD_PSYNERGY_DOUSE)
      heightLimit = px(64);
    if (kind == FIELD_PSYNERGY_MIND_READ)
      heightLimit = px(16);

    {
      s32 dy = candidate->y.fixed - source->y.fixed;
      if (dy >= 0) {
        if (dy > heightLimit)
          continue;
      } else if (source->y.fixed - candidate->y.fixed > heightLimit) {
        continue;
      }
    }

    // The distance in whole pixels, rounded toward 0.
    dx = candidate->x.fixed - source->x.fixed;
    if (dx < 0)
      dx += 0xffff;
    dx >>= 16;

    dz = candidate->z.fixed - source->z.fixed;
    if (dz < 0)
      dz += 0xffff;
    pixelZ = dz >> 16;

    distance = Iwram_Sqrt(dx * dx + pixelZ * pixelZ);
    if ((candidate->collisionFlags & ACTOR_NEAR_PSYNERGY) != 0)
      distance = distance * 2 / 3;
    if (distance >= nearestDistance)
      continue;

    // The cone: TRIG_TURN * 3 / 32 (about 34 degrees) either side up to 19
    // pixels, TRIG_TURN / 16 (22.5 degrees) beyond; TRIG_TURN / 8 (45 degrees)
    // for Mind Read.
    angle = (u16)ArcTan2(candidate->z.fixed - source->z.fixed,
                         candidate->x.fixed - source->x.fixed);
    coneHalfAngle = TRIG_TURN * 3 / 32;
    if (distance > 19)
      coneHalfAngle = TRIG_TURN / 16;
    if (kind == FIELD_PSYNERGY_MIND_READ)
      coneHalfAngle = TRIG_TURN / 8;

    if (distance > 11) {
      s32 angleDifference = (s16)(angle - facingQuadrant);
      if (angleDifference < 0)
        angleDifference = -angleDifference;
      if (angleDifference >= coneHalfAngle)
        continue;
    }

    nearestActorId = actorIndex;
    nearestDistance = distance;
  }

  return nearestActorId;
}

/// If actor target exists, plays SE_CHEST_OPEN, gives it animation 4 and, 12
/// frames later, shows itemId in front of actor with sparkles.
s32 FieldEffect_PlayCueAndStartEmitterOnTarget(s32 actorId, s32 target,
                                               s32 itemId) {
  void *targetActor;
  s32 emitter;

  targetActor = Actor_GetById(target);
  emitter = 0;
  if (targetActor != 0) {
    Sound_PlayFar(SE_CHEST_OPEN);
    Actor_SelectAnimationFar(targetActor, 4);
    wait(12);
    emitter = (s32)ItemDisplay_ShowItemInFront(actorId, itemId);
  }
  return emitter;
}

/// Gives every part of an actor's sprite (but those at palette 15) palette - 1,
/// or for palette 0 the next palette of a cycle that changes every 2 frames.
void Actor_SetSpritePalette(struct Actor *actor, s32 palette) {
  struct Sprite *sprite;
  struct SpriteAnimation **entrySlot;
  struct SpriteAnimation *entry;
  s32 remainingParts;
  s32 replacement;
  u8 partCount;

  if ((actor->spriteStorageKind & 15) == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    replacement = palette - 1;
    if (palette == 0)
      replacement = gMindRead_CasterPaletteCycle[(gFrameCount >> 1) & 7];

    partCount = sprite->count;
    if (partCount != 0) {
      entrySlot = sprite->entries;
      remainingParts = partCount;
      do {
        entry = *entrySlot++;
        if (entry != 0 && entry->script != 0 &&
            entry->colorMode != SPRITE_COLOR_HIDDEN)
          entry->colorMode = replacement;
        remainingParts--;
      } while (remainingParts != 0);
    }
    sprite->dirty = 1;
  }
}

/// Clears the field's busy flag and, while Reveal is active, ends it: Reveal
/// used with ACT_PACKED runs its ending events.
void Field_EndBusyAndReveal(void) {
  struct FieldWork *work = gFieldWork;

  work->busy = 0;
  if (work->revealActive != 0) {
    Field_RunPsynergyEndEvents(ABILITY_REVEAL | ACT_PACKED);
  }
}
