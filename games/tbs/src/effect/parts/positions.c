/// @file
/// Effect positions, projection and id dispatch helpers.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// Particle motion and projection into the battle view.
//
// Effect particles are EffectStep records in 16.16 fixed point. The two
// integrators move a particle one frame: they add its velocity to its
// position, add gravity to its vertical speed and then damp every speed by a
// factor out of 64. The 3D one works in field space, before projection; the
// 2D one works directly on the screen.
//
// The EffectPosition_Apply*AndYOffset helpers turn a point or a battler into
// an EffectPosition on the screen through the current transform. Each returns
// a point 16 pixels higher than the projection, which is where effects anchor
// on a battler. They differ only in how the battler is projected: at its
// sprite's scaled height, scaled for the battle view, or plainly.

/// Moves a particle one frame in three dimensions: adds its velocity, then
/// gravity to its vertical speed, then damps every speed by damping / 64.
void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
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
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                     s32 gravity) {
  step->x += step->velocityX;
  step->y += step->velocityY;
  step->velocityY += gravity;
  step->velocityX = step->velocityX * damping / 64;
  step->velocityY = step->velocityY * damping / 64;
}

/// Projects a point in the current transform onto the screen, 16 pixels
/// higher, and returns the projection's scale.
s32 EffectPosition_ProjectPoint(const void *point,
                                struct EffectPosition *position) {
  s32 result = Render_ProjectPoint(point, EffectPosition_Coordinates(position));
  position->y -= 16;
  return result;
}

/// The screen position of a battler's sprite, at its scaled height, 16
/// pixels higher.
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position) {
  BattleUnit_ProjectToScreen(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a battler, scaled for the battle view, 16 pixels
/// higher.
void EffectPosition_ProjectScaledBattler(s32 id,
                                         struct EffectPosition *position) {
  BattleMotion_ProjectScaledPositionFar(id,
                                        EffectPosition_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a battler, 16 pixels higher.
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position) {
  BattleMotion_ProjectPositionFar(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

/// Projects a battler onto the screen, raised by half its table height (the
/// table holds the full height in 16.16) at the sprite's scale. The view is
/// reset to the battle camera first, so the result does not depend on the
/// transform an effect has left behind.
///
/// unit: the battler.
/// screen: receives x and y.
/// Returns 0.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera;
  struct MotionObject *object;
  struct AnimationObject *info;
  s32 scale;
  /* FAKEMATCH: the unused 12-byte vector preserves both stack
   * adjustments and camera-load order. */
  s32 unused[3];

  camera = gCameraWork;
  object = GetBattleObjectSlotFar(unit)->object;
  info = GetMotionRecordFar(object, 0);
  EFFECT_VIEW_FROM_CAMERA(camera);
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), info->scale);
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeightFar(unit) >> 17);
  return 0;
}

/// Reads the held keys when the first target is an enemy (id 128 or more)
/// and does nothing with them. No effect calls it.
void BattleFx_ProbeKeysForExtendedActor(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleEffectArgument *effect = work->effect;

  if (effect->actors[0] > 0x7f)
    (void)gKeysHeld;
}

/// Runs a battle effect from the second range of kinds: 100 to 135 are the
/// casting impacts (kind - 100), 136 to 199 the beam sequences, 200 and up
/// the ring and spark scenes. Allocates the sheet, the work and the canvas
/// for the effect and frees them after.
///
/// effect: the effect argument.
void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect) {
  s32 kind;
  s32 value;

  // One local holds the sheet size, then the kind, then the impact.
  value = 0x60e;
  Runtime_AllocateBlock(HEAP_SLOT_BATTLE_SHEET, value);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT,
                            sizeof(struct BattleEffectWork));
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  value = effect->kind;
  kind = value;
  value = kind - 100;
  // Unsigned, so kinds below 100 fall through to the beam sequences too.
  if ((u32)value <= 35) {
    BattleFx_RunCastingImpact(effect, value);
  } else if (kind > 199) {
    BattleFx_RunRingAndSparkScene(effect);
  } else {
    BattleFx_RunBeamSequence(effect);
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}
