/// @file
/// The object event a field action aims at, and the helpers that follow it.
// Part of object.c.

#include "../object_internal.h"

/// Returns how far, in pixels, a field Psynergy of the given kind reaches:
/// gFieldEffect_TargetRangeByMode (fxdata.c) pairs kinds with ranges, ended by
/// -1, and a kind not listed reaches 16 pixels. Called by
/// Field_FindNearbyPsynergyTarget.
s32 Field_GetPsynergyTargetRange(s32 kind) {
  s16 *rangeEntry = gFieldEffect_TargetRangeByMode;
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

/// Returns the object event nearest to object event id that it could talk to,
/// or -1: within 48 pixels of height and 32 pixels of distance and, beyond 11
/// pixels, within 0x2fff (about 67 degrees) of the direction it faces. Object
/// events with collision flag 8 are skipped, and those with flag 4 count as
/// 10/13 as far, so they can be talked to from up to 41 pixels. Called with the
/// leader by Map_Run for the A button, and by the item code (items.c) to
/// find whom an item is used on.
s32 Field_FindNearbyTalkTarget(s32 id) {
  struct Object *source;
  struct Object *candidate;
  s32 nearestLocalId;
  s32 nearestDistance;
  s32 objectEventIndex;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 squaredX;
  s32 squaredY;
  s32 squaredZ;
  s32 squaredDistance;
  s32 distance;
  s32 angle;

  nearestLocalId = -1;
  nearestDistance = 32;
  source = ObjectEvent_GetByLocalId(id);
  if (source != 0) {
    for (objectEventIndex = 0; objectEventIndex <= OBJECT_LAST;
         objectEventIndex++) {
      if (objectEventIndex == id)
        continue;

      candidate = ObjectEvent_GetByLocalId(objectEventIndex);
      if (candidate == 0 || (candidate->collisionFlags & 8) != 0)
        continue;

      // y is height: an object event more than 48 pixels above or below is out
      // of reach.
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
      if ((candidate->collisionFlags & 4) != 0)
        distance = distance * 10 / 13;
      if (distance >= nearestDistance)
        continue;

      // Close object events count from any side; further ones must be within
      // 0x2fff (about 67 degrees) of where the object event faces.
      angle = (u16)ArcTan2(candidate->z - source->z, candidate->x - source->x);
      if (distance > 11) {
        s32 angleDifference = (s16)(angle - source->facing);

        if (angleDifference < -0x2fff)
          continue;
        if (angleDifference > 0x2fff)
          continue;
      }

      nearestLocalId = objectEventIndex;
      nearestDistance = distance;
    }
  }
  return nearestLocalId;
}

/// Returns the object event nearest in front of object event id that a field
/// Psynergy of the given kind can reach, or -1. The direction is rounded to a
/// quarter turn and the distance measured on the ground; kind sets the range,
/// the height allowed and the width of the cone. Object events with collision
/// flag 0x10 count as 2/3 as far. Called with the leader by Field_UsePsynergy,
/// by the item code (items.c) and by the Psynergy dispatch
/// (psynergy_effects/dispatch.c).
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind) {
  struct Object *source;
  struct Object *candidate;
  s32 nearestLocalId;
  s32 nearestDistance;
  s32 facingQuadrant;
  s32 objectEventIndex;
  s32 heightLimit;
  s32 dx;
  s32 dz;
  s32 pixelZ;
  s32 distance;
  s32 angle;
  s32 coneHalfAngle;

  nearestLocalId = -1;
  nearestDistance = Field_GetPsynergyTargetRange(kind);
  source = (struct Object *)ObjectEvent_GetByLocalId(id);
  if (source == 0)
    return nearestLocalId;

  // The facing rounded to the nearest of the four quarter turns.
  facingQuadrant = (source->facing + 0x2000) & 0xc000;
  for (objectEventIndex = 0; objectEventIndex <= OBJECT_LAST;
       objectEventIndex++) {
    if (objectEventIndex == id)
      continue;

    candidate = (struct Object *)ObjectEvent_GetByLocalId(objectEventIndex);
    if (candidate == 0 || (candidate->collisionFlags & 8) != 0)
      continue;

    // The height allowed: 8 pixels, or 48 for Catch, 64 for Douse, 16 for
    // Mind Read.
    heightLimit = 8 << 16;
    if (kind == FIELD_PSYNERGY_CATCH)
      heightLimit = 48 << 16;
    if (kind == FIELD_PSYNERGY_DOUSE)
      heightLimit = 64 << 16;
    if (kind == FIELD_PSYNERGY_MIND_READ)
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
    if ((candidate->collisionFlags & 0x10) != 0)
      distance = distance * 2 / 3;
    if (distance >= nearestDistance)
      continue;

    // The cone: 0x1800 (about 34 degrees) either side up to 19 pixels,
    // 0x1000 (22.5 degrees) beyond; 0x2000 (45 degrees) for Mind Read.
    angle = (u16)ArcTan2(candidate->z - source->z, candidate->x - source->x);
    coneHalfAngle = 0x1800;
    if (distance > 19)
      coneHalfAngle = 0x1000;
    if (kind == FIELD_PSYNERGY_MIND_READ)
      coneHalfAngle = 0x2000;

    if (distance > 11) {
      s32 angleDifference = (s16)(angle - facingQuadrant);
      if (angleDifference < 0)
        angleDifference = -angleDifference;
      if (angleDifference >= coneHalfAngle)
        continue;
    }

    nearestLocalId = objectEventIndex;
    nearestDistance = distance;
  }

  return nearestLocalId;
}

/// If object event target exists, plays SE_124, gives it animation 4 and, 12
/// frames later, shows itemId in front of object event with sparkles. Returns
/// the picture's encoded object event pointer, or 0 when the target or picture
/// is absent. Called by two map overlays: McCoy's palace, when a reward chest
/// opens, and Vault's interiors, when Ivan finds the Shaman's Rod.
s32 FieldEffect_PlayCueAndStartEmitterOnTarget(s32 localId, s32 target,
                                               s32 itemId) {
  void *targetObjectEvent;
  s32 emitter;

  targetObjectEvent = ObjectEvent_GetById(target);
  emitter = 0;
  if (targetObjectEvent != 0) {
    Sound_PlaySongFar(SE_124);
    ObjectDispatch_SelectAnimationFar(targetObjectEvent, 4);
    WaitFrames(12);
    emitter = (s32)FieldEffect_StartRandomParticleEmitter(localId, itemId);
  }
  return emitter;
}

/// Gives every part of a motion object's sprite (but those at palette 15)
/// palette - 1, or for palette 0 the next palette of a cycle that changes every
/// 2 frames. Sets the sprite's dirty flag so it is drawn again. Called by
/// Mind Read's wave effect (psynergy_effects/mind_read.c) on the caster: each
/// frame with 0 to cycle its colours, and with 1 (palette 0) when it ends.
void Animation_ApplyChildPalette(struct MotionObject *object, s32 palette) {
  struct AnimationObject *sprite;
  struct AnimationEntry **entrySlot;
  struct AnimationEntry *entry;
  s32 remainingParts;
  s32 replacement;
  u8 partCount;

  if ((object->spritesKind & 15) == OBJECT_SPRITE_ONE) {
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
            entry->colorMode != SPRITE_COLOR_HIDDEN)
          entry->colorMode = replacement;
        remainingParts--;
      } while (remainingParts != 0);
    }
    sprite->dirty = 1;
  }
}

/// Clears the field's busy flag and, while Reveal is active, ends it: Reveal
/// used with ACT_PACKED runs its ending events. Called by FieldScript_Start
/// when a script starts while the field is busy, by Map_Run before the
/// pause menu, and by 30 map overlays.
void Field_EndBusyAndReveal(void) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;

  work->busy = 0;
  if (work->revealActive != 0) {
    Field_RunPsynergyEvents(ABILITY_REVEAL | ACT_PACKED);
  }
}
