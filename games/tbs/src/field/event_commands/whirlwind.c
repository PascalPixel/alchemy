/// @file
/// Whirlwind.
// Part of event_commands.c.

#include "../event_commands_internal.h"

enum {
  WHIRLWIND_RADIAL_DIRECTIONS = 16,
  WHIRLWIND_RADIAL_ANGLE_SHIFT = 12,
  WHIRLWIND_ORBIT_PARTICLES = 16,
  WHIRLWIND_ORBIT_RADIUS = px(8),
  WHIRLWIND_ORBIT_ANGLE_STEP = TRIG_TURN / 32,
  WHIRLWIND_ORBIT_FRAMES = 121,
  WHIRLWIND_WANDER_FRAMES = 101,
  WHIRLWIND_TARGET_SCALE_STEP = Q16_ONE / 512,
  WHIRLWIND_TARGET_MIN_SCALE = Q16_ONE / 2,
  WHIRLWIND_TRAVEL_FRAMES = 10
};

/// The shared effect-spawning source, at its place in the ROM.
#define EFFECT_SPAWN_RESIDENT
#include "../../scene/shared/field_effect_spawn.c"
#include "../../scene/shared/field_effect_advance.c"
#undef EFFECT_SPAWN_RESIDENT

/// Spawns 17 particles that spread from origin in a flat ring, their x velocity
/// 1.5 times their z velocity, so the ring is wider than deep as the field is
/// drawn.
void FieldEffect_SpawnRadialParticleRing(struct Actor *origin) {
  struct EffectOptions opt;
  s32 vel[3];
  s32 vz;
  s32 angle;
  u32 i;

  opt.palette = 0;
  opt.update = FieldEffect_AdvanceMotion;
  opt.startScaleX = Q16_ONE * 4 / 5;
  opt.startScaleY = Q16_ONE * 4 / 5;
  i = 0;
  do {
    angle = i << WHIRLWIND_RADIAL_ANGLE_SHIFT;
    vel[0] = Trig_Cos(angle) * 3 / 2;
    vel[1] = 0;
    vz = Trig_Sin(angle);
    vel[2] = vz;
    FieldEffect_SpawnResident(origin->x.fixed, origin->y.fixed, origin->z.fixed,
                              vel[0], vel[1], vz, 0x01090001, &opt);
    i++;
  } while (i <= WHIRLWIND_RADIAL_DIRECTIONS);
}

/// Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame, now
/// and then turning sharply; after 101 frames it hands over to the shared
/// particle script.
void FieldEffect_WanderAroundAnchor(struct Actor *effect) {
  s32 len;
  s32 angle;
  s32 dx;
  s32 dz;

  // Random16 is 0 to 1 in 16.16, so 2 to 3 pixels.
  len = Random16() + px(2);
  angle = effect->facing;
  dx = Iwram_MulQ16(len, Trig_Cos(angle));
  dz = Iwram_MulQ16(len, Trig_Sin(angle));
  effect->x.fixed += dx;
  effect->z.fixed += dz;
  // The u16 angle wraps: 0xfff0 gives a small backwards drift.
  effect->facing += 0xfff0;
  if (effect->turnFrames != 0) {
    effect->turnFrames--;
    effect->facing += TRIG_TURN / 32;
  } else if ((Random16() << 5) >> 16 == 0) // one chance in 32
  {
    effect->turnFrames = ((Random16() << 4) >> 16) + 8; // 8 to 23 frames
  }
  if (++effect->swirlFrames == WHIRLWIND_WANDER_FRAMES)
    Actor_StartScriptFar(effect, gFieldEffect_SharedParticleScript);
}

/// Per-frame update: circles the anchor at 8 pixels; after 121 frames it starts
/// to wander (FieldEffect_WanderAroundAnchor).
void FieldEffect_CircleAnchor(struct Actor *effect) {
  struct Actor *anchor;
  s32 angle;
  s32 dx;
  s32 dz;

  anchor = effect->watchedActor;
  {
    s32 radius = WHIRLWIND_ORBIT_RADIUS;

    angle = effect->facing;
    dx = Iwram_MulQ16(radius, Trig_Cos(angle));
    dz = Iwram_MulQ16(radius, Trig_Sin(angle));
    effect->x.fixed = anchor->x.fixed + dx;
    effect->z.fixed = anchor->z.fixed + dz;
    effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
    if (++effect->swirlFrames == WHIRLWIND_ORBIT_FRAMES) {
      // The field integrates height after this update routine.
      effect->update = FieldEffect_WanderAroundAnchor;
      effect->swirlFrames = 0;
      effect->turnFrames = 0;
      effect->gravity = px(1) / 10;
      effect->velocityY = px(3);
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

/// An orb is carried from the caster to 4 pixels above the Psynergy's position
/// over 10 frames, growing from 0.25.
void FieldPsynergy_Whirlwind(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *src = effectWork->mainActor;
  struct Actor *target = effectWork->child;
  struct Vector3 pos;
  struct Vector3 from;
  struct Vector3 to;
  struct Actor *ball;
  s32 step;
  struct Actor *particle;
  s32 steps = WHIRLWIND_TRAVEL_FRAMES + 1;

  step = 0;
  from.x = src->x.fixed;
  from.y = src->y.fixed;
  from.z = src->z.fixed;
  to.x = effectWork->x;
  to.y = effectWork->y - px(4);
  to.z = effectWork->z;
  // Include both endpoints of the ten-frame interpolation.
  ball = Actor_Spawn(SPRITE_WHIRLWIND, 0, 0, 0);
  if (ball == 0) {
    return;
  }
  FieldPsynergy_FadeToCastColor();
  Actor_SelectAnimationFar(ball, 2);
  do {
    s32 scale;
    ball->x.fixed = from.x + step * (to.x - from.x) / WHIRLWIND_TRAVEL_FRAMES;
    ball->y.fixed = from.y + step * (to.y - from.y) / WHIRLWIND_TRAVEL_FRAMES;
    ball->z.fixed = from.z + step * (to.z - from.z) / WHIRLWIND_TRAVEL_FRAMES;
    scale = Q16_ONE / 4 + step * (Q16_ONE * 21 / 20) /
                              WHIRLWIND_TRAVEL_FRAMES; // from 0.25 to 1.3
    ball->scaleX = scale;
    ball->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  ball->scaleX = Q16_ONE * 17 / 10; // wide
  ball->scaleY = Q16_ONE * 13 / 10; // high
  Sound_PlayFar(SE_RUSH);
  wait(20);
  // Cleared by FieldPsynergy_Dispatch: cast on a target a trigger
  // names, the target is blown away.
  if (effectWork->enabled == 0) {
    if (target != 0)
      target->update = FieldEffect_ShrinkActorScaleUntilHalf;

    // Particles from 4 pixels above the orb, each 0.8 pixel higher, 3 to 8
    // pixels away.
    step = 0;
    do {
      s32 dist;

      pos.x = ball->x.fixed;
      pos.y = ball->y.fixed + step * (px(4) / 5) + px(4);
      pos.z = ball->z.fixed;
      dist = Random16() * 5 + px(3);
      Vector_AddPolarOffset(dist, Random16(), &pos);
      particle = Actor_Spawn(SPRITE_LEAF, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = FieldEffect_CircleAnchor;
        particle->watchedActor = ball;
        particle->timer = 0;
        particle->userValue = 0;
        particle->facing = Random16();
      }
      wait(6);
      step++;
    } while (step <= WHIRLWIND_ORBIT_PARTICLES - 1);
    wait(20);
    wait(120);
  }
  Actor_SelectAnimationFar(ball, 1);
  wait(30);
  Sound_PlayFar(SE_WHOOSH);
  wait(20);
  // Every particle has left its orbit by now.
  Actor_ReleaseFar(ball);
  FieldPsynergy_FadeFromCastColor();
}
