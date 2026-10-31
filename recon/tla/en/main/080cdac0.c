// Matches: differ 0 of 148 (tla-en, 2026-10-30).
// The test inside The Broken Seal's Field_FindNearbyPsynergyTarget
// (field_sequence/targets.c), taken out into a function of its own: ⚓️'s
// BattleEffect_FindNearestObject (play_cue_and_start_emitter_on_target.c)
// calls it for each object, and the per-animation limits come from a table
// (Field_GetPsynergyTargetRule) instead of ☀️'s literals. Belongs after
// descriptor_range.c's Func_080cda84.
// The height test is ABS(difference) > height, which GCC splits into the two
// arms the native shows; written out as nested ifs (☀️'s form) it differs in 23
// of 148, and needed a one-pass block to reach 4.
#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "macros.h"

// How far, how high and how wide of the caster's facing an animation reaches
// (gData_080eff28, one row per animation, ended by -1).
struct NearbyRule {
  s16 animation;
  s16 range;  // pixels, inclusive
  s16 height; // pixels above or below
  s16 width;  // of the facing, in 1/65536 turns
};

struct NearbyRule *Field_GetPsynergyTargetRule(s32 animation);
s32 ArcTan2(s32 y, s32 x);

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
    facing = (self->angle + 0x2000) & 0xc000;
    angle = (u16)ArcTan2(other->z - self->z, other->x - self->x);
    diff = (s16)(angle - facing);
    if (diff < 0)
      diff = -diff;
    if (diff >= width)
      return -1;
  }
  return distance;
}
