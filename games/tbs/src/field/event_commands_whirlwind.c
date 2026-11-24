/// @file
/// Whirlwind: an orb flies from the caster and spins up a ring of leaves that
/// blows its target away.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Whirlwind's particles: the ring that spreads from the caster, the leaves
/// that circle the orb 8 pixels out, a 32nd of a turn a frame, for 121 frames
/// before they wander off for 101, the target shrinking to half its size, and
/// the orb's 10 frames from caster to target.
enum {
  WHIRLWIND_RADIAL_DIRECTIONS = 16,
  WHIRLWIND_ORBIT_PARTICLES = 16,
  WHIRLWIND_ORBIT_RADIUS = toFIXED(8),
  WHIRLWIND_ORBIT_ANGLE_STEP = TRIG_TURN / 32,
  WHIRLWIND_ORBIT_FRAMES = 121,
  WHIRLWIND_WANDER_FRAMES = 101,
  WHIRLWIND_TARGET_SCALE_STEP = FIXED_ONE / 512,
  WHIRLWIND_TARGET_MIN_SCALE = FIXED_ONE / 2,
  WHIRLWIND_TRAVEL_FRAMES = 10
};

/// The shared effect-spawning source, at its place in the ROM.
#define EFFECT_SPAWN_RESIDENT
#include "../scene/shared/field_effect_spawn.c"
#include "../scene/shared/field_effect_advance.c"
#undef EFFECT_SPAWN_RESIDENT

/// Spawns 17 particles that spread from origin in a flat ring, their x velocity
/// 1.5 times their z velocity, so the ring is wider than deep as the field is
/// drawn.
void FieldEffect_SpawnRadialParticleRing(struct Actor *origin) {
  struct EffectOptions opt;
  FIXED vel[3];
  FIXED vz;
  s32 angle;
  u32 i;

  opt.palette = 0;
  opt.update = FieldEffect_AdvanceMotion;
  opt.startScaleX = FIXED_ONE * 4 / 5;
  opt.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i <= WHIRLWIND_RADIAL_DIRECTIONS; i++) {
    angle = i * (TRIG_TURN / WHIRLWIND_RADIAL_DIRECTIONS);
    vel[0] = Trig_Cos(angle) * 3 / 2;
    vel[1] = 0;
    vz = Trig_Sin(angle);
    vel[2] = vz;
    FieldEffect_SpawnResident(origin->x.fixed, origin->y.fixed, origin->z.fixed,
                              vel[0], vel[1], vz,
                              EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                                  EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                              &opt);
  }
}

/// Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame, now
/// and then turning sharply; after 101 frames it hands over to the shared
/// particle script.
void FieldEffect_WanderAroundAnchor(struct Actor *effect) {
  FIXED len;
  s32 angle;
  FIXED dx;
  FIXED dz;

  // Random16 is 0 to 1 in 16.16, so 2 to 3 pixels.
  len = Random16() + toFIXED(2);
  angle = effect->facing;
  dx = Iwram_MulFX(len, Trig_Cos(angle));
  dz = Iwram_MulFX(len, Trig_Sin(angle));
  effect->x.fixed += dx;
  effect->z.fixed += dz;
  // A small backwards drift, a 4096th of a turn a frame.
  effect->facing += -(TRIG_TURN / 4096);
  if (effect->turnFrames != 0) {
    effect->turnFrames--;
    effect->facing += TRIG_TURN / 32;
  } else if (Random16_Below(32) == 0) {
    effect->turnFrames = Random16_Below(16) + 8; // 8 to 23 frames
  }
  if (++effect->swirlFrames == WHIRLWIND_WANDER_FRAMES)
    Actor_StartScriptFar(effect, gFieldEffect_SharedParticleScript);
}

/// Per-frame update: circles the anchor at 8 pixels; after 121 frames it starts
/// to wander (FieldEffect_WanderAroundAnchor).
void FieldEffect_CircleAnchor(struct Actor *effect) {
  struct Actor *anchor;
  s32 angle;
  FIXED dx;
  FIXED dz;

  anchor = effect->watchedActor;
  {
    FIXED radius = WHIRLWIND_ORBIT_RADIUS;

    angle = effect->facing;
    dx = Iwram_MulFX(radius, Trig_Cos(angle));
    dz = Iwram_MulFX(radius, Trig_Sin(angle));
    effect->x.fixed = anchor->x.fixed + dx;
    effect->z.fixed = anchor->z.fixed + dz;
    effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
    if (++effect->swirlFrames == WHIRLWIND_ORBIT_FRAMES) {
      // The field integrates height after this update routine.
      effect->update = FieldEffect_WanderAroundAnchor;
      effect->swirlFrames = 0;
      effect->turnFrames = 0;
      effect->gravity = toFIXED(1) / 10;
      effect->velocityY = toFIXED(3);
      effect->facing = Random16();
    }
  }
}

/// Shrinks an event-selected Whirlwind target each frame; below half size,
/// moves it to the origin, stops it and removes the update.
void FieldEffect_ShrinkActorScaleUntilHalf(struct Actor *actor) {
  s32 scale;

  scale = actor->scaleX - WHIRLWIND_TARGET_SCALE_STEP;
  actor->scaleY = scale;
  actor->scaleX = scale;
  if (scale < WHIRLWIND_TARGET_MIN_SCALE) {
    Actor_SetPositionAndResetMotionFar(actor, 0, 0, 0);
    actor->update = 0;
  }
}

/// Runs Whirlwind cast on a target, using the target already stored in the
/// field effect.
void FieldPsynergy_WhirlwindOnTarget(s32 unused) { FieldPsynergy_Whirlwind(); }

/// Whirlwind: an orb grows as it flies from the caster to 4 pixels below the
/// Psynergy's position in 10 frames, swells and gusts; cast at a target a
/// trigger names, it shrinks the target away and sends 16 leaves circling, then
/// it whooshes off.
void FieldPsynergy_Whirlwind(void) {
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *caster = effectState->mainActor;
  struct Actor *target = effectState->child;
  struct Vector3 pos;
  struct Vector3 from;
  struct Vector3 to;
  struct Actor *orb;
  s32 step;
  struct Actor *leaf;
  s32 steps = WHIRLWIND_TRAVEL_FRAMES + 1;

  step = 0;
  from.x = caster->x.fixed;
  from.y = caster->y.fixed;
  from.z = caster->z.fixed;
  to.x = effectState->x;
  to.y = effectState->y - toFIXED(4);
  to.z = effectState->z;
  orb = Actor_Spawn(SPRITE_WHIRLWIND, 0, 0, 0);
  if (orb == 0) {
    return;
  }
  FieldPsynergy_FadeToCastColor();
  Actor_SelectAnimationFar(orb, 2);
  // Both ends of the ten-frame flight.
  for (; step < steps; step++) {
    FIXED scale;
    orb->x.fixed = from.x + step * (to.x - from.x) / WHIRLWIND_TRAVEL_FRAMES;
    orb->y.fixed = from.y + step * (to.y - from.y) / WHIRLWIND_TRAVEL_FRAMES;
    orb->z.fixed = from.z + step * (to.z - from.z) / WHIRLWIND_TRAVEL_FRAMES;
    scale = FIXED_ONE / 4 + step * (FIXED_ONE * 21 / 20) /
                                WHIRLWIND_TRAVEL_FRAMES; // from 0.25 to 1.3
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
  }
  orb->scaleX = FIXED_ONE * 17 / 10; // wide
  orb->scaleY = FIXED_ONE * 13 / 10; // high
  Sound_PlayFar(SE_RUSH);
  wait(20);
  // Cleared by FieldPsynergy_Dispatch: cast on a target a trigger
  // names, the target is blown away.
  if (effectState->enabled == 0) {
    if (target != 0)
      target->update = FieldEffect_ShrinkActorScaleUntilHalf;

    // Leaves from 4 pixels above the orb, each 0.8 pixel higher, 3 to 8
    // pixels away.
    for (step = 0; step < WHIRLWIND_ORBIT_PARTICLES; step++) {
      FIXED dist;

      pos.x = orb->x.fixed;
      pos.y = orb->y.fixed + step * (toFIXED(4) / 5) + toFIXED(4);
      pos.z = orb->z.fixed;
      dist = Random16() * 5 + toFIXED(3);
      Vector_AddPolarOffset(dist, Random16(), &pos);
      leaf = Actor_Spawn(SPRITE_LEAF, pos.x, pos.y, pos.z);
      if (leaf != 0) {
        leaf->update = FieldEffect_CircleAnchor;
        leaf->watchedActor = orb;
        leaf->timer = 0;
        leaf->userValue = 0;
        leaf->facing = Random16();
      }
      wait(6);
    }
    wait(20);
    wait(120);
  }
  Actor_SelectAnimationFar(orb, 1);
  wait(30);
  Sound_PlayFar(SE_WHOOSH);
  wait(20);
  // Every leaf has left its orbit by now.
  Actor_ReleaseFar(orb);
  FieldPsynergy_FadeFromCastColor();
}
