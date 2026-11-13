/// @file
/// Effect positions, projection and id dispatch helpers.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// Particle motion and projection into the battle view.
//
// Effect particles are Particles in 16.16 fixed point. The two
// integrators move a particle one frame: they add its velocity to its
// position, add gravity to its vertical speed and then damp every speed by a
// factor out of 64 (64 keeps the speed, 60 loses about 6% a frame). The 3D one
// works in field space, before projection; the 2D one works directly on the
// screen, where a positive gravity pulls down.
//
// The EffectPosition_Project* helpers turn a point or a unit into an
// ScreenPoint on the screen. Each returns a point 16 pixels higher than the
// projection, which is where effects anchor on a unit. They differ only
// in how the unit is projected: at its sprite's scaled height, scaled for
// the battle view, or plainly.

/// Moves a particle one frame in three dimensions: adds its velocity, then
/// gravity to its vertical speed, then damps every speed by damping / 64.
void Particle_AdvanceWithGravity3D(struct Particle *step, s32 damping,
                                   s32 gravity) {
  step->x += step->velocityX;
  step->y += step->velocityY;
  step->z += step->velocityZ;
  step->velocityY += gravity;
  SET_VELOCITY(step, step->velocityX * damping / 64,
               step->velocityY * damping / 64, step->velocityZ * damping / 64);
}

/// Moves a particle one frame on the screen: adds its velocity, then
/// gravity to its vertical speed, then damps both speeds by damping / 64.
void Particle_AdvanceWithGravity2D(struct Particle *step, s32 damping,
                                   s32 gravity) {
  step->x += step->velocityX;
  step->y += step->velocityY;
  step->velocityY += gravity;
  step->velocityX = step->velocityX * damping / 64;
  step->velocityY = step->velocityY * damping / 64;
}

/// Projects a point in the current transform onto the screen, 16 pixels
/// higher, and returns the projection's scale.
s32 Projection_Point(const void *point, struct ScreenPoint *position) {
  s32 result = Transform_ProjectPoint(point, ScreenPoint_Coordinates(position));
  position->y -= 16;
  return result;
}

/// The screen position of a unit's sprite, at its scaled height, 16
/// pixels higher.
void Projection_Sprite(s32 id, struct ScreenPoint *position) {
  BattleUnit_ProjectToScreen(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a unit, scaled for the battle view, 16 pixels
/// higher.
void Projection_ScaledUnit(s32 id, struct ScreenPoint *position) {
  BattleMotion_ProjectScaledPositionFar(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a unit, 16 pixels higher.
void Projection_Unit(s32 id, struct ScreenPoint *position) {
  BattleMotion_ProjectPositionFar(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// Projects a unit onto the screen, raised by half its table height (the
/// table holds the full height in 16.16) at the sprite's scale. The view is
/// reset to the battle camera first, so the result does not depend on the
/// transform an effect has left behind.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera = gBattleCamera;
  struct Actor *actor;
  struct Sprite *info;
  s32 scale;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: no instruction addresses any of the 12 bytes it reserves (sub sp,
   * #12); as without it, 6 of 52 instructions differ in tbs-en. */
  UNUSED s32 unused[3];

  actor = GetBattleObjectSlotFar(unit)->actor;
  info = GetMotionRecordFar(actor, 0);
  EFFECT_VIEW_FROM_CAMERA(camera);
  scale = Iwram_MulQ16(Transform_ProjectPoint(&actor->x.fixed, screen),
                       info->scale);
  // The table height is 16.16: >> 17 takes half of it in whole pixels.
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeightFar(unit) >> 17);
  return 0;
}

// The Lost Age keeps the same empty function, and neither game calls it or
// holds its address.
// XXX (no evidence): what it was for, perhaps keys that once chose an
// effect for enemy targets; only a debug build would show it.
/// Reads the held keys when the first target is an enemy (FIRST_ENEMY_UNIT or
/// more) and does nothing with them. Nothing calls it.
void BattleEffect_ProbeKeysForExtendedActor(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleEffectParams *args = work->args;

  if (args->targets[0] >= FIRST_ENEMY_UNIT)
    (void)gKeysHeld;
}

/// Runs a battle effect from the second range of kinds, split by enum
/// BattleEffectIdRange: the casting impacts (kind -
/// EFFECT_RANGE_CASTING_IMPACT), the beams and the ring and spark effects.
void BattleEffect_RunByIdRange(struct BattleEffectParams *args) {
  s32 kind;
  s32 value;

  // One local holds the sheet size, then the kind, then the impact.
  value = 1550;
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SHEET, value);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  value = args->kind;
  kind = value;
  value = kind - EFFECT_RANGE_CASTING_IMPACT;
  // Unsigned, so kinds below EFFECT_RANGE_CASTING_IMPACT fall through to the
  // beams too.
  if ((u32)value < EFFECT_RANGE_BEAM - EFFECT_RANGE_CASTING_IMPACT) {
    BattleEffect_RunCastingImpact(args, value);
  } else if (kind >= EFFECT_RANGE_RING_AND_SPARK) {
    BattleEffect_RunCriticalHit(args);
  } else {
    BattleEffect_RunAttackSlash(args);
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}
