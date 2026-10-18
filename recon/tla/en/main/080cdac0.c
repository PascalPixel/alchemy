// Near miss, differ 4 of 148 (2026-10-15). The test inside The Broken
// Seal's Field_FindNearbyPsynergyTarget (object.c), taken out into a
// function of its own: ⚓️'s BattleFx_FindNearestObject
// (play_cue_and_start_emitter_on_target.c) calls it for each object, and
// the per-animation limits come from a table (Field_GetPsynergyTargetRule) instead of ☀️'s
// literals. Belongs after descriptor_range.c's Func_080cda84.
// Left: one branch. In the footing test the native branches to the shared
// -1 on "above" (bgt; b to the distance) where this branches to the
// distance on "not above" (ble; b to the -1).
// Tried: plain early returns, differ 23 (the one-pass block below found by
// alchemy permute brings it to 4); a goto to one -1, differ 58; the two
// height bases in locals, 38; the tests inverted, 21 and 47; ☀️'s angle
// as one expression, 30 (the native keeps (u16) on its own statement). A
// second 6-minute permute found nothing below 4.
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"

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
  struct ObjectRuntime *self;
  struct ObjectRuntime *other;
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
  /* FAKEMATCH: a one-pass block around the tests up to the distance lays
   * out their early returns as the native does (differ 23 without it). */
  do {
    other = ObjectTable_Get(otherId);
    if (self == 0 || other == 0 || (other->collisionFlags & 8) != 0)
      return -1;

    // Height: within the rule's pixels above or below.
    rule = Field_GetPsynergyTargetRule(animation);
    height = rule->height << 16;
    if (animation == ANIMATION_MEASURES_FOOTING) {
      if (other->terrainHeight - self->terrainHeight >= 0) {
        if (other->terrainHeight - self->terrainHeight > height)
          return -1;
      } else if (self->terrainHeight - other->terrainHeight > height) {
        return -1;
      }
    } else {
      if (other->y - self->y >= 0) {
        if (other->y - self->y > height)
          return -1;
      } else if (self->y - other->y > height) {
        return -1;
      }
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
  } while (0);

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
