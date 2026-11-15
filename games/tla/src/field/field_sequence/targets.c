/// @file
/// The actor a field Psynergy or item aims at, and the helpers that follow it.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/map.h"
#include "module/system/system.h"

/// Picks the object nearest object id, under 32 pixels away and 48 pixels in
/// height. Distances to ids above 63 count double and to objects with
/// collision flag 4 ten thirteenths; beyond 11 pixels the object must also lie
/// within 0x2fff of id's facing. Returns -1 when none qualifies.
s32 Field_FindNearbyTalkTarget(s32 id) {
  struct Actor *src;
  struct Actor *obj;
  s32 best;
  s32 bestDist;
  s32 i;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 xx;
  s32 yy;
  s32 zz;
  s32 sq;
  s32 dist;
  s32 angle;

  best = -1;
  bestDist = 32;
  src = Actor_Lookup(id);
  if (src != 0) {
    for (i = 0; i <= 80; i++) {
      if (i == id)
        continue;

      obj = Actor_Lookup(i);
      if (obj == 0 || (obj->collisionFlags & 8) != 0)
        continue;

      if (obj->y.fixed - src->y.fixed >= 0) {
        if (obj->y.fixed - src->y.fixed >= px(48))
          continue;
      } else if (src->y.fixed - obj->y.fixed >= px(48)) {
        continue;
      }

      dx = (obj->x.fixed - src->x.fixed) / Q16_ONE;
      dy = (obj->y.fixed - src->y.fixed) / Q16_ONE;
      dz = (obj->z.fixed - src->z.fixed) / Q16_ONE;

      xx = dx * dx;
      yy = dy * dy;
      zz = dz * dz;
      sq = xx + yy + zz;
      dist = Iwram_Sqrt(sq);
      if (i > 63)
        dist <<= 1;
      if ((obj->collisionFlags & 4) != 0)
        dist = dist * 10 / 13;
      if (dist >= bestDist)
        continue;

      angle = (u16)ArcTan2(obj->z.fixed - src->z.fixed,
                           obj->x.fixed - src->x.fixed);
      if (dist > 11) {
        s32 diff = (s16)(angle - src->facing);

        if (diff < -0x2fff)
          continue;
        if (diff > 0x2fff)
          continue;
      }

      best = i;
      bestDist = dist;
    }
  }
  return best;
}

/// Returns the row of gFieldPsynergy_Reach for a field Psynergy kind: its
/// range, height and facing width. A kind not found returns the -1 terminator.
const struct FieldPsynergyReach *Field_GetPsynergyReach(s32 id) {
  const struct FieldPsynergyReach *entry = gFieldPsynergy_Reach;

  while (entry->fieldPsynergy != -1 && id != entry->fieldPsynergy)
    entry++;
  return entry;
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Returns how far the ground at position lies above or below the actor's,
/// made positive, when that is within the kind's reach in height, else -1.
s32 Field_MeasurePsynergyHeight(s32 id, struct Actor *actor, s32 *position) {
  const struct FieldPsynergyReach *entry = Field_GetPsynergyReach(id);
  s32 height = MapGrid_GetTerrainHeightAboveFar(
      position[0], position[2], actor->layer, actor->groundHeight);

  if (ABS(height) <= entry->height << 16)
    return ABS(height);
  return -1;
}

// The Lift-style animation measures height by the object's footing, not y.
#define ANIMATION_MEASURES_FOOTING 18

/// How far object otherId stands from object selfId, in pixels over the
/// ground, when it lies within the animation's reach in front of selfId;
/// -1 when it does not.
s32 Field_MeasurePsynergyReach(s32 animation, s32 selfId, s32 otherId) {
  struct Actor *self;
  struct Actor *other;
  const struct FieldPsynergyReach *rule;
  s32 height;
  s32 range;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 width;
  s32 facing;
  s32 diff;
  s32 angle;

  self = Actor_Lookup(selfId);
  other = Actor_Lookup(otherId);
  if (self == 0 || other == 0 || (other->collisionFlags & 8) != 0)
    return -1;

  // Height: within the rule's pixels above or below.
  rule = Field_GetPsynergyReach(animation);
  height = rule->height << 16;
  if (animation == ANIMATION_MEASURES_FOOTING) {
    if (ABS(other->groundHeight - self->groundHeight) > height)
      return -1;
  } else {
    if (ABS(other->y.fixed - self->y.fixed) > height)
      return -1;
  }

  // Distance over the ground; small objects (flag 0x10) count from half as
  // far again.
  range = rule->range + 1;
  if ((other->collisionFlags & 0x10) != 0)
    range = range * 3 / 2;
  dx = (other->x.fixed - self->x.fixed) / Q16_ONE;
  dz = (other->z.fixed - self->z.fixed) / Q16_ONE;
  distance = Iwram_Sqrt(dx * dx + dz * dz);
  if (distance > range)
    return -1;

  // Beyond 11 pixels it must also lie in front: within the rule's width of
  // the caster's facing, rounded to a quarter turn (two thirds as wide
  // beyond 19 pixels).
  if (distance > 11) {
    width = rule->width;
    if (distance > 19)
      width = width * 2 / 3;
    facing = (self->facing + TRIG_TURN / 8) & TRIG_TURN * 3 / 4;
    angle = (u16)ArcTan2(other->z.fixed - self->z.fixed,
                         other->x.fixed - self->x.fixed);
    diff = (s16)(angle - facing);
    if (diff < 0)
      diff = -diff;
    if (diff >= width)
      return -1;
  }
  return distance;
}

/// Returns the actor nearest in front of actor id that a field Psynergy of the
/// given kind can reach, or -1.
s32 Field_FindNearbyPsynergyTarget(s32 self, s32 effect) {
  s32 nearest = 9999;
  s32 found = -1;
  s32 i;

  for (i = 0; i <= 80; i++) {
    if (i != self) {
      s32 distance = Field_MeasurePsynergyReach(effect, self, i);

      if (distance != -1 && nearest > distance) {
        found = i;
        nearest = distance;
      }
    }
  }
  return found;
}

/// If actor target exists, plays SE_CHEST_OPEN, gives it animation 4 and, 12
/// frames later, shows itemId in front of actor with sparkles.
s32 FieldEffect_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target,
                                               s32 mode) {
  s32 object;
  s32 result;

  object = (s32)Actor_GetById(target);
  result = 0;
  if (object != 0) {
    Sound_PlayFar(SE_CHEST_OPEN);
    Actor_SelectAnimationFar(object, 4);
    wait(0xC);
    result = (s32)BattleEffect_StartRandomParticleEmitter(effect, mode);
  }
  return result;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Whether the object 16 pixels in front of the current owner, along its
/// facing rounded to a quarter turn, can be pushed on by 16 pixels: 1 when
/// nothing solid stands on the cell beyond it or above it and its terrain
/// test passes there, else 0. Leader_PushObjectAhead, after it, makes the push.
// The test against the paused object comes from patch 1; the Japanese release
// has none.
s32 Leader_CanPushObjectAhead(void) {
  s32 position[3];
  s32 above[3];
  struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *target;
  struct Actor *hit;
  s32 id;
  s32 terrain;
  s32 result;
  s32 half;
  s32 angle = owner->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = owner->x.fixed;
  half = 128;
  position[1] = (owner->y.fixed + (half << 11)) & 0xfff80000;
  position[2] = owner->z.fixed;
  Vector_AddPolarOffset(px(16), angle, position);
  target = Actor_FindAtCell(position, 1);
  if (target == 0)
    return 0;
  if (BYTE_AT(target, 98) != 0)
    return 0;
#if TLA_PATCH_1
  id = gMainRoutineWork.haltedActor;
  if (id != 1 && target == Actor_GetById(id))
    return 0;
#endif
  position[0] = target->x.fixed;
  position[1] = target->y.fixed;
  position[2] = target->z.fixed;
  Vector_AddPolarOffset(px(16), angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != 0 && (hit->collisionFlags & 1))
    return 0;
  above[0] = target->x.fixed;
  above[1] = target->y.fixed + 0x100000;
  above[2] = target->z.fixed;
  hit = Actor_FindAtCell(above, 1);
  if (hit != 0 && (hit->collisionFlags & 1))
    return 0;
  terrain = target->layer;
  target->layer = 2;
  result = MapGrid_CheckCollisionFar(target, position);
  target->layer = terrain;
  if (result > 0)
    return 0;
  return 1;
}

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

/// Pushes the object ahead of the current owner on by 16 pixels, as
/// Leader_CanPushObjectAhead allowed.
void Leader_PushObjectAhead(void) {
  s32 position[3];
  struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *target;
  s32 terrain;
  s32 result;
  s32 speed;
  s32 acceleration;
  s32 angle = owner->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = owner->x.fixed;
  position[1] = (owner->y.fixed + 0x40000) & 0xfff80000;
  position[2] = owner->z.fixed;
  Vector_AddPolarOffset(px(16), angle, position);
  target = Actor_FindAtCell(position, 1);
  position[0] = target->x.fixed;
  position[1] = target->y.fixed;
  position[2] = target->z.fixed;
  Vector_AddPolarOffset(px(16), angle, position);
  terrain = target->layer;
  target->layer = 2;
  result = MapGrid_CheckCollisionFar(target, position);
  target->layer = terrain;
  if (result <= 0) {
    Actor_SelectAnimationFar(owner, 8);
    wait(15);
    Sound_PlayFar(SE_PUSH);
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    Actor_SetMoveTargetFar(target, position[0], position[1], position[2]);
    wait(2);
    acceleration = owner->acceleration;
    owner->acceleration = 0x3333;
    speed = owner->speedLimit;
    owner->speedLimit = 0x3333;
    Actor_SetMoveTargetFar(owner, position[0], position[1], position[2]);
    Actor_WaitForMoveFar(target);
    wait(1);
    Event_NoOp();
    Actor_SetPositionAndResetMotionFar(target, position[0], position[1],
                                       position[2]);
    Actor_SetPositionAndResetMotionFar(owner, S16_AT(owner, 10) << 16,
                                       owner->y.fixed, S16_AT(owner, 18) << 16);
    Actor_SelectAnimationFar(owner, 1);
    owner->speedLimit = speed;
    owner->acceleration = acceleration;
  }
}
