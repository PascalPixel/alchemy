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
///
/// Called each frame for the 3D sparks, motes, shards and shells of 18 parts,
/// from the casting impacts, the beams and the flurry to the
/// lobbed shells and the stat reductions; each then projects the particle
/// with Projection_Point. Changes only the step.
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
///
/// Called each frame for the screen-space sparks, motes, chips and puffs of
/// 20 parts, among them Thor, Spire, Ramses, Ragnarok, the Titan Blade, the
/// Helm Splitter, the Djinn attacks and the projectile volleys. Changes only
/// the step; z and velocityZ are left alone.
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
/// (Transform_ProjectPoint); the 16 is taken off its y all the same.
///
/// point: three s32s in 16.16, x, y and z (an Particle's position).
s32 Projection_Point(const void *point, struct ScreenPoint *position) {
  s32 result = Transform_ProjectPoint(point, ScreenPoint_Coordinates(position));
  position->y -= 16;
  return result;
}

/// The screen position of a unit's sprite, at its scaled height, 16
/// pixels higher.
///
/// Called by BattleEffect_RunBeam (beams.c), for the first target, and
/// BattleEffect_RunBurstShower (burst_shower.c), for the user. Leaves the view
/// set to the battle camera (BattleUnit_ProjectToScreen).
void Projection_Sprite(s32 id, struct ScreenPoint *position) {
  BattleUnit_ProjectToScreen(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a unit, scaled for the battle view, 16 pixels
/// higher.
///
/// The usual anchor for an effect on its user or first target: called during
/// the setup of 21 parts, from the casting impacts, beams, the burst
/// effect and Spire to the healing motes and the stat boosts.
void Projection_ScaledUnit(s32 id, struct ScreenPoint *position) {
  BattleMotion_ProjectScaledPositionFar(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// The screen position of a unit, 16 pixels higher.
///
/// Called during the setup of 11 parts, among them the target strikes, Bolt,
/// the rushing strike, the Titan Blade, the Helm Splitter and the element
/// pillar.
void Projection_Unit(s32 id, struct ScreenPoint *position) {
  BattleMotion_ProjectPositionFar(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// Projects a unit onto the screen, raised by half its table height (the
/// table holds the full height in 16.16) at the sprite's scale. The view is
/// reset to the battle camera first, so the result does not depend on the
/// transform an effect has left behind.
///
/// Called only by Projection_Sprite. Leaves the render transform
/// looking from gBattleCamera's eye to its position, and always returns 0.
///
/// screen: receives x and y.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera;
  struct Actor *actor;
  struct Sprite *info;
  s32 scale;
  /* FAKEMATCH(dead): an unused vector for the stack frame's 12 bytes; without
   * it, 1 of 52 instructions differ in tbs-en. */
  UNUSED s32 unused[3];

  camera = gBattleCamera;
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
///
/// Reached from the battle's presentation through its far-call stub: from
/// the attack, item, Psynergy cast and unit transition runners
/// (battle/staging.c) when the turn has EFFECT_BY_ID_RANGE (bit 14 of the
/// effect word), and always from the plain attack runner and the approach
/// action (battle/auto_battle.c). Takes heap slots
/// HEAP_SLOT_BATTLE_SHEET (from EWRAM first), HEAP_SLOT_BATTLE_EFFECT and
/// HEAP_SLOT_BATTLE_CANVAS for the effect, and releases them in the reverse
/// order once it returns.
void BattleEffect_RunByIdRange(struct BattleEffectParams *args) {
  s32 kind;
  s32 value;

  // One local holds the sheet size, then the kind, then the impact. The
  // largest picture the three runners decode into the sheet slot is
  // ParticleSpritesD's ten flare cells, 2x2 to 20x20 pixels, 1,540 bytes
  // (2 * MOTE_SET_BYTES); the slot's last 10 bytes are never written.
  value = 0x60e;
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
    BattleEffect_RunRingAndSpark(args);
  } else {
    BattleEffect_RunBeam(args);
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}
