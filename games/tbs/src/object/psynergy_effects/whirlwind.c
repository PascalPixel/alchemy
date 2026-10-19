/// @file
/// Part of psynergy_effects.c, compiled only through it.
/// Whirlwind.

#include "../psynergy_effects_internal.h"

/// A FieldActor seen with the halfword at 0x64 unsigned and the one at 0x66
/// signed, as the bytes read them (FieldActor's own types change the code).
struct OrbitEffect {
  u8 unk00[6];
  u16 angle; ///< the way it heads, or its angle round the anchor
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;                  ///< frames so far
  s16 pause;                  ///< frames left of a sharp turn
  struct OrbitEffect *anchor; ///< what it circles
  void (*update)(union FieldObject *object);
};

/// The shared effect-spawning source, at its place in the ROM.
#define EFFECT_SPAWN_RESIDENT
#include "../../scene/common/effect_spawn.h"
#undef EFFECT_SPAWN_RESIDENT

/// Per-frame update of a particle: moves by its velocity, slowing its x and z
/// velocity a little, and changes its scale and spin.
void FieldFx_UpdateParticleMotionAndScale(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;

  effect->x += effect->velocity.x;
  effect->y += effect->velocity.y;
  effect->z += effect->velocity.z;
  effect->velocity.x -= effect->velocity.x / 18;
  effect->velocity.z -= effect->velocity.z / 16;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

/// Spawns 17 particles that spread from origin in a flat ring, their x
/// velocity 1.5 times their z velocity.
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions opt;
  s32 vel[3];
  s32 vz;
  s32 angle;
  u32 i;

  opt.palette = 0;
  opt.update = FieldFx_UpdateParticleMotionAndScale;
  opt.startScaleX = 0xcccc; // 0.8
  opt.startScaleY = 0xcccc;
  i = 0;
  do {
    angle = i << 12; // 1/16 turn apart
    vel[0] = Trig_Cos(angle) * 3 / 2;
    vel[1] = 0;
    vz = Trig_Sin(angle);
    vel[2] = vz;
    Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz,
                         0x01090001, &opt);
    i++;
  } while (i <= 16);
}

/// Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame,
/// now and then turning sharply; after 101 frames it hands over to the
/// common particle script.
void BattleFx_WanderAroundAnchor(union FieldObject *fieldObject) {
  struct OrbitEffect *effect = (struct OrbitEffect *)fieldObject;
  s32 len;
  s32 angle;
  s32 dx;
  s32 dz;

  len = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(len, Trig_Cos(angle));
  dz = Iwram_MulQ16(len, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) // one chance in 32
  {
    effect->pause = ((Random16() << 4) >> 16) + 8; // 8 to 23 frames
  }
  if (++effect->timer == 101)
    ObjectDispatch_InitializeFar((struct DispatchObject *)effect,
                                 gBattleFx_CommonParticleScript);
}

/// Per-frame update: circles the anchor at 8 pixels; after 121 frames it
/// starts to wander (BattleFx_WanderAroundAnchor).
void BattleFx_CircleAnchor(union FieldObject *fieldObject) {
  struct OrbitEffect *effect = (struct OrbitEffect *)fieldObject;
  struct OrbitEffect *anchor;
  s32 angle;
  s32 radius;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  radius = 0x80000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += 0x800;
  if (++effect->timer == 121) {
    effect->update = BattleFx_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}

/// Per-frame update: shrinks the object; below half size, puts its position
/// back and removes the update.
void BattleFx_ShrinkObjectScaleUntilHalf(union FieldObject *fieldObject) {
  struct MotionObject *obj = (struct MotionObject *)fieldObject;
  s32 scale;

  scale = obj->scaleX - 0x80;
  obj->scaleY = scale;
  obj->scaleX = scale;
  if (scale < 0x8000) {
    Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
    ((struct ScriptMotionObject *)obj)->hook = 0;
  }
}

/// Abilities: Whirlwind, Gale (field kind 14), cast on a target.
void FieldFx_RunWhirlwindOnTarget(s32 unused) { FieldFx_RunWhirlwind(); }

/// Abilities: Whirlwind, Gale (field kind 14).
///
/// An orb is carried from the caster to 4 pixels above the effect's position
/// over 10 frames, growing from 0.25. Without scene->enabled the target
/// shrinks, and 16 particles appear round the orb, one every 6 frames, and
/// circle it.
void FieldFx_RunWhirlwind(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *src = scene->mainObject;
  struct FieldActor *target = scene->child;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  struct FieldActor *ball;
  s32 step;
  struct FieldActor *particle;
  s32 steps = 11;

  step = 0;
  from.x = src->x.fixed;
  from.y = src->y.fixed;
  from.z = src->z.fixed;
  to.x = scene->x;
  to.y = scene->y - (4 << 16);
  to.z = scene->z;
  ball = Object_Spawn(0xda, 0, 0, 0);
  if (ball == 0) {
    return;
  }
  BattleEffect_InitializeSharedScene();
  ObjectDispatch_SelectAnimationFar(ball, 2);
  do {
    s32 scale;
    ball->x.fixed = from.x + step * (to.x - from.x) / 10;
    ball->y.fixed = from.y + step * (to.y - from.y) / 10;
    ball->z.fixed = from.z + step * (to.z - from.z) / 10;
    scale = 0x4000 + step * 0x10ccc / 10; // from 0.25 to 1.3
    ball->scaleX = scale;
    ball->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ball->scaleX = 0x1b333; // 1.7 wide
  ball->scaleY = 0x14ccc; // 1.3 high
  AudioCommand_PlayFar(SE_163);
  WaitFrames(20);
  if (scene->enabled == 0) {
    if (target != 0)
      target->update = BattleFx_ShrinkObjectScaleUntilHalf;

    // Particles from 4 pixels above the orb, each 0.8 pixel higher, 3 to 8
    // pixels away.
    step = 0;
    do {
      s32 dist;

      pos.x = ball->x.fixed;
      pos.y = ball->y.fixed + step * 0xcccc + (4 << 16);
      pos.z = ball->z.fixed;
      dist = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(dist, Random16(), &pos);
      particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = BattleFx_CircleAnchor;
        particle->linkedObject = ball;
        particle->timer = 0;
        particle->work = 0;
        particle->facing = Random16();
      }
      WaitFrames(6);
      step++;
    } while (step <= 15);
    WaitFrames(20);
    WaitFrames(120);
  }
  ObjectDispatch_SelectAnimationFar(ball, 1);
  WaitFrames(30);
  AudioCommand_PlayFar(SE_136);
  WaitFrames(20);
  ObjectDispatch_ReleaseFar(ball);
  BattleFx_PrepareBufferInterpolation();
}
