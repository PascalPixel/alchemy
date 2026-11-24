/// @file
/// The actor a field Psynergy or item aims at, and the helpers that follow it.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// How far, in pixels, a field Psynergy reaches when
/// gFieldEffect_TargetRangeByMode does not list its kind.
#define TARGET_RANGE_DEFAULT 16

/// Returns how far, in pixels, a field Psynergy of the given kind reaches:
/// gFieldEffect_TargetRangeByMode (field_sequence_tables.c) pairs kinds with
/// ranges, ended by -1.
s32 Field_GetPsynergyTargetRange(s32 kind) {
  const s16 *rangeEntry = gFieldEffect_TargetRangeByMode;
  s32 range = TARGET_RANGE_DEFAULT;
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
/// 48 pixels of height and 32 pixels of distance (an ACTOR_NEAR_TALK actor's
/// counted ten thirteenths) and, beyond 11 pixels, within 3/16 of a turn
/// (67.5 degrees) of the direction it faces.
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
      if (candidate == 0 ||
          (candidate->collisionFlags & ACTOR_NOT_TARGETED) != 0)
        continue;

      // y is height: an actor more than 48 pixels above or below is out
      // of reach.
      if (candidate->y.fixed - source->y.fixed >= 0) {
        if (candidate->y.fixed - source->y.fixed >= toFIXED(48))
          continue;
      } else if (source->y.fixed - candidate->y.fixed >= toFIXED(48)) {
        continue;
      }

      dx = (candidate->x.fixed - source->x.fixed) / FIXED_ONE;
      dy = (candidate->y.fixed - source->y.fixed) / FIXED_ONE;
      dz = (candidate->z.fixed - source->z.fixed) / FIXED_ONE;

      squaredX = dx * dx;
      squaredY = dy * dy;
      squaredZ = dz * dz;
      squaredDistance = squaredX + squaredY + squaredZ;
      distance = Iwram_Sqrt(squaredDistance);
      if ((candidate->collisionFlags & ACTOR_NEAR_TALK) != 0)
        distance = distance * 10 / 13;
      if (distance >= nearestDistance)
        continue;

      angle = (u16)Trig_Atan(candidate->z.fixed - source->z.fixed,
                             candidate->x.fixed - source->x.fixed);
      if (distance > 11) {
        s32 angleDifference = (s16)(angle - source->facing);

        if (angleDifference < -(TRIG_TURN * 3 / 16 - 1))
          continue;
        if (angleDifference > TRIG_TURN * 3 / 16 - 1)
          continue;
      }

      nearestActorId = actorIndex;
      nearestDistance = distance;
    }
  }
  return nearestActorId;
}

/// Returns the actor nearest in front of actor id that a field Psynergy of the
/// given kind can reach, or -1: within its range (an ACTOR_NEAR_PSYNERGY
/// actor's counted two thirds), within 8 pixels of height (more for Catch,
/// Douse and Mind Read) and, beyond 11 pixels, within a cone either side of
/// its facing's quarter turn, narrower beyond 19 pixels and wider for Mind
/// Read.
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind) {
  struct Actor *source;
  struct Actor *candidate;
  s32 nearestActorId;
  s32 nearestDistance;
  s32 facingQuadrant;
  s32 actorIndex;
  FIXED heightLimit;
  s32 dx;
  FIXED dz;
  s32 pixelZ;
  s32 distance;
  s32 angle;
  s32 coneHalfAngle;

  nearestActorId = -1;
  nearestDistance = Field_GetPsynergyTargetRange(kind);
  source = Actor_Lookup(id);
  if (source == 0)
    return nearestActorId;

  facingQuadrant = FACING_NEAREST_CARDINAL(source->facing);
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
    if (candidate == 0 || (candidate->collisionFlags & ACTOR_NOT_TARGETED) != 0)
      continue;

    heightLimit = toFIXED(8);
    if (kind == FIELD_PSYNERGY_CATCH)
      heightLimit = toFIXED(48);
    if (kind == FIELD_PSYNERGY_DOUSE)
      heightLimit = toFIXED(64);
    if (kind == FIELD_PSYNERGY_MIND_READ)
      heightLimit = toFIXED(16);

    {
      FIXED dy = candidate->y.fixed - source->y.fixed;
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
      dx += FIXED_ONE - 1;
    dx = FIXEDtoINT(dx);

    dz = candidate->z.fixed - source->z.fixed;
    if (dz < 0)
      dz += FIXED_ONE - 1;
    pixelZ = FIXEDtoINT(dz);

    distance = Iwram_Sqrt(dx * dx + pixelZ * pixelZ);
    if ((candidate->collisionFlags & ACTOR_NEAR_PSYNERGY) != 0)
      distance = distance * 2 / 3;
    if (distance >= nearestDistance)
      continue;

    angle = (u16)Trig_Atan(candidate->z.fixed - source->z.fixed,
                           candidate->x.fixed - source->x.fixed);
    coneHalfAngle = DEGtoANG(33.75);
    if (distance > 19)
      coneHalfAngle = DEGtoANG(22.5);
    if (kind == FIELD_PSYNERGY_MIND_READ)
      coneHalfAngle = DEGtoANG(45);

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

/// Opens the chest that is actor target (SE_CHEST_OPEN and the chest sheet's
/// animation 4) and, 12 frames later, shows itemId in front of actor actorId
/// with sparkles; returns the item's picture, or 0 when there is no chest.
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

/// Gives each running layer of an actor's one sprite the colour mode
/// palette - 1 (SPRITE_COLOR_NORMAL for 1), or for 0 the next of Mind Read's
/// caster cycle, a mode every 2 frames; a hidden layer stays hidden.
void Actor_SetSpritePalette(struct Actor *actor, s32 palette) {
  struct Sprite *sprite;
  struct SpriteAnimation **entrySlot;
  struct SpriteAnimation *entry;
  s32 remainingParts;
  s32 replacement;
  u8 partCount;

  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
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
  struct FieldState *state = gFieldState;

  state->busy = 0;
  if (state->revealActive != 0) {
    Field_RunPsynergyEndEvents(ABILITY_REVEAL | ACT_PACKED);
  }
}
