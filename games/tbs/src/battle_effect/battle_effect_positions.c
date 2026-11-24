/// @file
/// Effect positions, projection and id dispatch helpers.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// Particle motion and projection. A particle moves a frame by its velocity,
// gravity on its vertical speed, and a damping out of DAMPING_SCALE; the
// projection helpers return a point 16 pixels above where a unit stands, where
// effects anchor on it.

/// Moves a particle one frame in three dimensions.
void Particle_AdvanceWithGravity3D(struct Particle *particle, s32 damping,
                                   s32 gravity) {
  particle->x += particle->velocityX;
  particle->y += particle->velocityY;
  particle->z += particle->velocityZ;
  particle->velocityY += gravity;
  SET_VELOCITY(particle, particle->velocityX * damping / DAMPING_SCALE,
               particle->velocityY * damping / DAMPING_SCALE,
               particle->velocityZ * damping / DAMPING_SCALE);
}

/// Moves a particle one frame on the screen.
void Particle_AdvanceWithGravity2D(struct Particle *particle, s32 damping,
                                   s32 gravity) {
  particle->x += particle->velocityX;
  particle->y += particle->velocityY;
  particle->velocityY += gravity;
  particle->velocityX = particle->velocityX * damping / DAMPING_SCALE;
  particle->velocityY = particle->velocityY * damping / DAMPING_SCALE;
}

/// Projects a point in the current transform onto the screen, 16 pixels
/// higher, and returns the projection's scale.
s32 Projection_Point(const void *point, struct ScreenPoint *position) {
  s32 result = Matrix_ProjectPoint(point, ScreenPoint_Coordinates(position));
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

/// Projects a unit onto the screen, raised by half its height, from the battle
/// camera.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera = gBattleCamera;
  struct Actor *actor;
  struct Sprite *sprite;
  FIXED scale;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 unused[3];

  actor = GetBattleObjectSlotFar(unit)->actor;
  sprite = GetMotionRecordFar(actor, 0);
  EFFECT_VIEW_FROM_CAMERA(camera);
  scale =
      Iwram_MulFX(Matrix_ProjectPoint(&actor->x.fixed, screen), sprite->scale);
  screen[1] -=
      Iwram_MulFX(scale, FIXEDtoINT(BattleUnit_GetEffectHeightFar(unit)) >> 1);
  return 0;
}

/// Unused: reads the held keys when the first target is an enemy and does
/// nothing with them.
UNUSED void BattleEffect_ProbeKeysForExtendedActor(void) {
  // XXX (no evidence): what the keys were for, perhaps choosing an effect for
  // enemy targets; nothing calls this in either game.
  struct BattleEffectState *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleEffectParams *args = state->args;

  if (args->targets[0] >= FIRST_ENEMY_UNIT)
    (void)gKeysHeld;
}

/// Runs a battle effect from the second range of kinds: the casting impacts,
/// the beams and the ring and spark effects.
void BattleEffect_RunByIdRange(struct BattleEffectParams *args) {
  s32 kind;

  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SHEET, 1550);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectState));
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  kind = args->kind;
  if (kind >= EFFECT_RANGE_CASTING_IMPACT && kind < EFFECT_RANGE_BEAM) {
    BattleEffect_RunCastingImpact(args, kind - EFFECT_RANGE_CASTING_IMPACT);
  } else if (kind >= EFFECT_RANGE_RING_AND_SPARK) {
    BattleEffect_RunCriticalHit(args);
  } else {
    BattleEffect_RunAttackSlash(args);
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}
