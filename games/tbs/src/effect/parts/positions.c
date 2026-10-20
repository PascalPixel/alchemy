/// @file
/// Effect positions, projection and id dispatch helpers.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// Particle motion and projection into the battle view.
//
// Effect particles are EffectStep records in 16.16 fixed point. The two
// integrators move a particle one frame: they add its velocity to its
// position, add gravity to its vertical speed and then damp every speed by a
// factor out of 64 (64 keeps the speed, 60 loses about 6% a frame). The 3D one
// works in field space, before projection; the 2D one works directly on the
// screen, where a positive gravity pulls down.
//
// The EffectPosition_Project* helpers turn a point or a battler into an
// EffectPosition on the screen. Each returns a point 16 pixels higher than the
// projection, which is where effects anchor on a battler. They differ only in
// how the battler is projected: at its sprite's scaled height, scaled for the
// battle view, or plainly.

/// Moves a particle one frame in three dimensions: adds its velocity, then
/// gravity to its vertical speed, then damps every speed by damping / 64.
///
/// Called each frame for the 3D sparks, motes, shards and shells of 18 parts,
/// from the casting impacts, the beam sequences and the burst scene to the
/// lobbed shells and the stat reductions; each then projects the particle
/// with EffectPosition_ProjectPoint. Changes only the step.
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
///
/// Called each frame for the screen-space sparks, motes, chips and puffs of
/// 20 parts, among them Thor, Spire, Ramses, Ragnarok, the Titan Blade, the
/// Helm Splitter, the Djinn attacks and the projectile volleys. Changes only
/// the step; z and velocityZ are left alone.
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
///
/// The common way an effect places a 3D particle: 35 parts call it each
/// frame once it has set the view, the casting impacts, beams, Judgment,
/// Procne and the stat changes most often. Only Astral Blast (stars.c) reads
/// the scale, to skip a ring whose nearest point comes too close to the
/// camera.
///
/// Returns the scale in 16.16, with position's x and y in screen pixels and
/// its depth in whole units. A point outside gProjection's depth range
/// returns 0 and leaves the transformed point in position, still in 16.16
/// (Render_ProjectPoint); the 16 is taken off its y all the same.
///
/// point: three s32s in 16.16, x, y and z (an EffectStep's position).
s32 EffectPosition_ProjectPoint(const void *point,
                                struct EffectPosition *position) {
  s32 result = Render_ProjectPoint(point, EffectPosition_Coordinates(position));
  position->y -= 16;
  return result;
}

/// The screen position of a battler's sprite, at its scaled height, 16
/// pixels higher.
///
/// Called by BattleFx_RunBeamSequence (beams.c), for the first target, and
/// BattleFx_RunBurstShower (burst_shower.c), for the user. Leaves the view set
/// to the battle camera (BattleUnit_ProjectToScreen).
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position) {
  BattleUnit_ProjectToScreen(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a battler, scaled for the battle view, 16 pixels
/// higher.
///
/// The usual anchor for an effect on its user or first target: called during
/// the setup of 21 parts, from the casting impacts, beams, the burst
/// scene and Spire to the healing motes and the stat boosts.
void EffectPosition_ProjectScaledBattler(s32 id,
                                         struct EffectPosition *position) {
  BattleMotion_ProjectScaledPositionFar(id,
                                        EffectPosition_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a battler, 16 pixels higher.
///
/// Called during the setup of 11 parts, among them the target strikes, Bolt,
/// the rushing strike, the Titan Blade, the Helm Splitter and the element
/// pillar.
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position) {
  BattleMotion_ProjectPositionFar(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

/// Projects a battler onto the screen, raised by half its table height (the
/// table holds the full height in 16.16) at the sprite's scale. The view is
/// reset to the battle camera first, so the result does not depend on the
/// transform an effect has left behind.
///
/// Called only by EffectPosition_ProjectSprite. Leaves the render transform
/// looking from gCameraWork's eye to its position, and always returns 0.
///
/// screen: receives x and y.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera;
  struct MotionObject *object;
  struct AnimationObject *info;
  s32 scale;
  /* FAKEMATCH(dead): an unused vector for the frame's 12 bytes; without it, 1
   * of 52 instructions differ in tbs-en. */
  UNUSED s32 unused[3];

  camera = gCameraWork;
  object = GetBattleObjectSlotFar(unit)->object;
  info = GetMotionRecordFar(object, 0);
  EFFECT_VIEW_FROM_CAMERA(camera);
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), info->scale);
  // The table height is 16.16: >> 17 takes half of it in whole pixels.
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeightFar(unit) >> 17);
  return 0;
}

/// Reads the held keys when the first target is an enemy (id 128 or more)
/// and does nothing with them. Nothing calls it.
///
/// XXX: what it was for; a debug build or the Lost Age's twin of this part
/// would show whether the keys once chose an effect for enemy targets.
void BattleFx_ProbeKeysForExtendedActor(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleEffectArgument *effect = work->effect;

  if (effect->actors[0] > 0x7f)
    (void)gKeysHeld;
}

/// Runs a battle effect from the second range of kinds: 100 to 135 are the
/// casting impacts (kind - 100), 136 to 199 the beam sequences, 200 and up
/// the ring and spark scenes.
///
/// Reached from the battle's presentation through its far-call stub: from
/// the attack, item, Psynergy cast and unit transition runners
/// (battle/staging.c) when the plan has STAGING_BY_ID_RANGE (bit 14 of the
/// effect word), and always from the plain attack runner and the approach
/// action (battle/auto_battle.c). Takes heap slots
/// HEAP_SLOT_BATTLE_SHEET (from EWRAM first), HEAP_SLOT_BATTLE_EFFECT and
/// HEAP_SLOT_BATTLE_CANVAS for the effect, and releases them in the reverse
/// order once it returns.
void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect) {
  s32 kind;
  s32 value;

  // One local holds the sheet size, then the kind, then the impact.
  // XXX: why 0x60e bytes, a little over two mote sets (2 * MOTE_SET_BYTES is
  // 0x604); the largest picture these runners decode into the sheet slot
  // would settle it.
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
