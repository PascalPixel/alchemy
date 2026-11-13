/// @file
/// The actor a field Psynergy or item aims at, and the helpers that follow it.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
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
  src = ObjectTable_Get(id);
  if (src != 0) {
    for (i = 0; i <= 80; i++) {
      if (i == id)
        continue;

      obj = ObjectTable_Get(i);
      if (obj == 0 || (obj->collisionFlags & 8) != 0)
        continue;

      if (obj->y - src->y >= 0) {
        if (obj->y - src->y >= 48 << 16)
          continue;
      } else if (src->y - obj->y >= 48 << 16) {
        continue;
      }

      dx = (obj->x - src->x) / Q16_ONE;
      dy = (obj->y - src->y) / Q16_ONE;
      dz = (obj->z - src->z) / Q16_ONE;

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

      angle = (u16)ArcTan2(obj->z - src->z, obj->x - src->x);
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

/// Returns the row of gData_080eff28 for an animation: its range, height
/// and facing width. A row not found returns the -1 terminator.
struct DescriptorEntry *Field_GetPsynergyTargetRule(s32 id) {
  struct DescriptorEntry *entry = gData_080eff28;

  while (entry->id != -1 && id != entry->id)
    entry++;
  return entry;
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Returns how far the ground at position lies above or below the actor's,
/// made positive, when that is within the id's table range (pixels), else -1.
s32 Func_080cda84(s32 id, struct Actor *actor, s32 *position) {
  struct DescriptorEntry *entry = Field_GetPsynergyTargetRule(id);
  s32 height = MapGrid_GetTerrainHeightAboveFar__field_sequence(
      position[0], position[2], actor->terrainId, actor->terrainHeight);

  if (ABS(height) <= entry->range << 16)
    return ABS(height);
  return -1;
}

// The Lift-style animation measures height by the object's footing, not y.
#define ANIMATION_MEASURES_FOOTING 18

// How far object otherId stands from object selfId, in pixels over the
// ground, when it lies within the animation's reach in front of selfId;
// -1 when it does not.
s32 Func_080cdac0(s32 animation, s32 selfId, s32 otherId) {
  struct Actor *self;
  struct Actor *other;
  struct NearbyRule *rule;
  s32 height;
  s32 range;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 width;
  s32 facing;
  s32 diff;
  s32 angle;

  self = ObjectTable_Get(selfId);
  other = ObjectTable_Get(otherId);
  if (self == 0 || other == 0 || (other->collisionFlags & 8) != 0)
    return -1;

  // Height: within the rule's pixels above or below.
  rule = Field_GetPsynergyTargetRule(animation);
  height = rule->height << 16;
  if (animation == ANIMATION_MEASURES_FOOTING) {
    if (ABS(other->terrainHeight - self->terrainHeight) > height)
      return -1;
  } else {
    if (ABS(other->y - self->y) > height)
      return -1;
  }

  // Distance over the ground; small objects (flag 0x10) count from half as
  // far again.
  range = rule->range + 1;
  if ((other->collisionFlags & 0x10) != 0)
    range = range * 3 / 2;
  dx = (other->x - self->x) / 0x10000;
  dz = (other->z - self->z) / 0x10000;
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
    facing = (self->facing + 0x2000) & 0xc000;
    angle = (u16)ArcTan2(other->z - self->z, other->x - self->x);
    diff = (s16)(angle - facing);
    if (diff < 0)
      diff = -diff;
    if (diff >= width)
      return -1;
  }
  return distance;
}

s32 BattleEffect_FindNearestObject(s32 self, s32 effect) {
  s32 nearest = 9999;
  s32 found = -1;
  s32 i;

  for (i = 0; i <= 80; i++) {
    if (i != self) {
      s32 distance = Func_080cdac0(effect, self, i);

      if (distance != -1 && nearest > distance) {
        found = i;
        nearest = distance;
      }
    }
  }
  return found;
}

s32 BattleEffect_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target,
                                                s32 mode) {
  s32 object;
  s32 result;

  object = (s32)Actor_GetById(target);
  result = 0;
  if (object != 0) {
    Sound_PlayFar(0x7C);
    Actor_SetMode(object, 4);
    wait(0xC);
    result = (s32)BattleEffect_StartRandomParticleEmitter(effect, mode);
  }
  return result;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Whether the object 16 pixels in front of the current owner, along its
/// facing rounded to a quarter turn, can be pushed on by 16 pixels: 1 when
/// nothing solid stands on the cell beyond it or above it and its terrain
/// test passes there, else 0. Func_080cdd80, after it, makes the push.
// The test against the paused object comes from patch 1. The Japanese release
// has none and stays on its scaffold: with the branch below it differs by one
// reordered row (the angle's argument copy issued after the next position
// load, differ 4).
s32 Func_080cdc74(void) {
  s32 position[3];
  s32 above[3];
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *target;
  struct Actor *hit;
  s32 id;
  s32 terrain;
  s32 result;
  s32 half;
  s32 angle = owner->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = owner->x;
  half = 128;
  position[1] = (owner->y + (half << 11)) & 0xfff80000;
  position[2] = owner->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  target = Func_080eaf28(position, 1);
  if (target == 0)
    return 0;
  if (BYTE_AT(target, 98) != 0)
    return 0;
#if TLA_PATCH_1
  id = gMainRoutineWork.pausedObject;
  if (id != 1 && target == Actor_GetById(id))
    return 0;
#endif
  position[0] = target->x;
  position[1] = target->y;
  position[2] = target->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  hit = Func_080eaf28(position, 1);
  if (hit != 0 && (hit->collisionFlags & 1))
    return 0;
  above[0] = target->x;
  above[1] = target->y + 0x100000;
  above[2] = target->z;
  hit = Func_080eaf28(above, 1);
  if (hit != 0 && (hit->collisionFlags & 1))
    return 0;
  terrain = target->terrainId;
  target->terrainId = 2;
  result = Func_08020210(target, position);
  target->terrainId = terrain;
  if (result > 0)
    return 0;
  return 1;
}

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

void Func_080cdd80(void) {
  s32 position[3];
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *target;
  s32 terrain;
  s32 result;
  s32 speed;
  s32 acceleration;
  s32 angle = owner->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = owner->x;
  position[1] = (owner->y + 0x40000) & 0xfff80000;
  position[2] = owner->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  target = Func_080eaf28(position, 1);
  position[0] = target->x;
  position[1] = target->y;
  position[2] = target->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  terrain = target->terrainId;
  target->terrainId = 2;
  result = Func_08020210(target, position);
  target->terrainId = terrain;
  if (result <= 0) {
    Actor_SetMode(owner, 8);
    wait(15);
    Sound_PlayFar(185);
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    Actor_SetTargetPosition(target, position[0], position[1], position[2]);
    wait(2);
    acceleration = owner->acceleration;
    owner->acceleration = 0x3333;
    speed = owner->speedLimit;
    owner->speedLimit = 0x3333;
    Actor_SetTargetPosition(owner, position[0], position[1], position[2]);
    Actor_CommitPosition(target);
    wait(1);
    Event_NoOp();
    Actor_SetPositionAndResetMotionFar(target, position[0], position[1],
                                       position[2]);
    Actor_SetPositionAndResetMotionFar(owner, S16_AT(owner, 10) << 16, owner->y,
                                       S16_AT(owner, 18) << 16);
    Actor_SetMode(owner, 1);
    owner->speedLimit = speed;
    owner->acceleration = acceleration;
  }
}
