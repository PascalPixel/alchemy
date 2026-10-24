/// @file
/// Whirlwind.
// Part of field_services.c.

#include "../field_services_internal.h"

enum {
  WHIRLWIND_RADIAL_DIRECTIONS = 16,
  WHIRLWIND_RADIAL_ANGLE_SHIFT = 12,
  WHIRLWIND_ORBIT_PARTICLES = 16,
  WHIRLWIND_ORBIT_RADIUS = 8 << 16,
  WHIRLWIND_ORBIT_ANGLE_STEP = 0x800,
  WHIRLWIND_ORBIT_FRAMES = 121,
  WHIRLWIND_WANDER_FRAMES = 101,
  WHIRLWIND_TARGET_SCALE_STEP = 0x80,
  WHIRLWIND_TARGET_MIN_SCALE = 0x8000,
  WHIRLWIND_TRAVEL_FRAMES = 10
};

/// The shared effect-spawning source, at its place in the ROM.
#define EFFECT_SPAWN_RESIDENT
#include "../../map/common/effect_spawn.h"
#undef EFFECT_SPAWN_RESIDENT

/// Per-frame update of a particle: moves by its velocity, slowing its x and z
/// velocity a little, and changes its scale and spin. Assigned by
/// FieldEffect_SpawnRadialParticleRing for Carry's landing particles.
void FieldEffect_UpdateParticleMotionAndScale(
    union ObjectEventOrEffect *fieldObject) {
  struct FieldEffect *effect = &fieldObject->effect;

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
/// velocity 1.5 times their z velocity, so the ring is wider than deep as the
/// field is drawn. Carry's FieldEffect_SteerLiftedTarget spawns it where it
/// sets its load down.
void FieldEffect_SpawnRadialParticleRing(struct Object *origin) {
  struct EffectOptions opt;
  s32 vel[3];
  s32 vz;
  s32 angle;
  u32 i;

  opt.palette = 0;
  opt.update = FieldEffect_UpdateParticleMotionAndScale;
  opt.startScaleX = 0xcccc; // 0.8
  opt.startScaleY = 0xcccc;
  i = 0;
  do {
    angle = i << WHIRLWIND_RADIAL_ANGLE_SHIFT;
    vel[0] = Trig_Cos(angle) * 3 / 2;
    vel[1] = 0;
    vz = Trig_Sin(angle);
    vel[2] = vz;
    FieldEffect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1],
                              vz, 0x01090001, &opt);
    i++;
  } while (i <= WHIRLWIND_RADIAL_DIRECTIONS);
}

/// Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame,
/// now and then turning sharply; after 101 frames it hands over to the
/// common particle script. FieldEffect_CircleAnchor switches a particle to it.
/// While it wanders, the engine's own motion lifts the particle (the velocity
/// and gravity FieldEffect_CircleAnchor gave it); this only moves it across the
/// ground.
void FieldEffect_WanderAroundAnchor(union ObjectEventOrEffect *fieldObject) {
  struct Object *effect = (struct Object *)fieldObject;
  s32 len;
  s32 angle;
  s32 dx;
  s32 dz;

  // Random16 is 0 to 1 in 16.16, so 2 to 3 pixels.
  len = Random16() + 0x20000;
  angle = effect->facing;
  dx = Iwram_MulQ16(len, Trig_Cos(angle));
  dz = Iwram_MulQ16(len, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  // The u16 angle wraps: 0xfff0 gives a small backwards drift. A turn burst
  // adds 1/32 of a turn each frame without stopping the outward motion.
  effect->facing += 0xfff0;
  if (effect->turnFrames != 0) {
    effect->turnFrames--;
    effect->facing += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) // one chance in 32
  {
    effect->turnFrames = ((Random16() << 4) >> 16) + 8; // 8 to 23 frames
  }
  if (++effect->swirlFrames == WHIRLWIND_WANDER_FRAMES)
    Object_InitializeFar(effect, gFieldEffect_CommonParticleScript);
}

/// Per-frame update: circles the anchor at 8 pixels; after 121 frames it
/// starts to wander (FieldEffect_WanderAroundAnchor). FieldEffect_RunWhirlwind
/// gives it to each of the 16 particles it spawns round the orb.
void FieldEffect_CircleAnchor(union ObjectEventOrEffect *fieldObject) {
  struct Object *effect = (struct Object *)fieldObject;
  struct Object *anchor;
  s32 angle;
  s32 radius;
  s32 dx;
  s32 dz;

  anchor = effect->linkedObject;
  radius = WHIRLWIND_ORBIT_RADIUS;
  angle = effect->facing;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
  if (++effect->swirlFrames == WHIRLWIND_ORBIT_FRAMES) {
    // The engine integrates height after this callback. Supply upward speed
    // and gravity here; the wandering callback only changes x and z.
    effect->update = FieldEffect_WanderAroundAnchor;
    effect->swirlFrames = 0;
    effect->turnFrames = 0;
    effect->gravity = 0x1999;
    effect->velocityY = 0x30000;
    effect->facing = Random16();
  }
}

/// Shrinks an event-selected Whirlwind target each frame; below half size,
/// moves it to the origin, stops it and removes the update.
/// The scale falls by 1/512 each frame. From unit scale the strict lower bound
/// takes 257 updates.
void FieldEffect_ShrinkObjectScaleUntilHalf(
    union ObjectEventOrEffect *fieldObject) {
  struct Object *object = (struct Object *)fieldObject;
  s32 scale;

  scale = object->scaleX - WHIRLWIND_TARGET_SCALE_STEP;
  object->scaleY = scale;
  object->scaleX = scale;
  if (scale < WHIRLWIND_TARGET_MIN_SCALE) {
    Object_SetPositionAndResetMotionFar((struct Object *)object, 0, 0, 0);
    object->update = 0;
  }
}

/// Runs Whirlwind cast on a target, using the target already stored in the
/// effect scene.
///
/// Abilities: Whirlwind, Gale (FIELD_PSYNERGY_WHIRLWIND).
///
/// FieldEffect_DispatchRequestKind passes an argument this adapter ignores; the
/// shared runner reads scene->child instead.
void FieldEffect_RunWhirlwindOnTarget(s32 unused) {
  FieldEffect_RunWhirlwind();
}

/// An orb is carried from the caster to 4 pixels above the effect's position
/// over 10 frames, growing from 0.25.
///
/// Abilities: Whirlwind, Gale (FIELD_PSYNERGY_WHIRLWIND).
///
/// Without scene->enabled the target shrinks, and 16 particles appear round the
/// orb, one every 6 frames, and circle it.
///
/// Called by FieldEffect_Run and FieldEffect_RunWhirlwindOnTarget. Fades the
/// palettes to an effect colour and back, and gives the target its shrinking
/// update; the orb is released at the end, the particles finish on their own.
void FieldEffect_RunWhirlwind(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ObjectEvent *src = effectWork->mainObject;
  struct ObjectEvent *target = effectWork->child;
  struct Vector3 pos;
  struct Vector3 from;
  struct Vector3 to;
  struct ObjectEvent *ball;
  s32 step;
  struct ObjectEvent *particle;
  s32 steps = WHIRLWIND_TRAVEL_FRAMES + 1;

  step = 0;
  from.x = src->x.fixed;
  from.y = src->y.fixed;
  from.z = src->z.fixed;
  to.x = effectWork->x;
  to.y = effectWork->y - (4 << 16);
  to.z = effectWork->z;
  // Include both endpoints of the ten-frame interpolation.
  ball = (struct ObjectEvent *)Object_Spawn(0xda, 0, 0, 0);
  if (ball == 0) {
    return;
  }
  FieldEffect_FadeToEffectColor();
  Object_SelectAnimationFar(ball, 2);
  do {
    s32 scale;
    ball->x.fixed = from.x + step * (to.x - from.x) / WHIRLWIND_TRAVEL_FRAMES;
    ball->y.fixed = from.y + step * (to.y - from.y) / WHIRLWIND_TRAVEL_FRAMES;
    ball->z.fixed = from.z + step * (to.z - from.z) / WHIRLWIND_TRAVEL_FRAMES;
    scale =
        0x4000 + step * 0x10ccc / WHIRLWIND_TRAVEL_FRAMES; // from 0.25 to 1.3
    ball->scaleX = scale;
    ball->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ball->scaleX = 0x1b333; // 1.7 wide
  ball->scaleY = 0x14ccc; // 1.3 high
  Sound_PlaySongFar(SE_RUSH);
  WaitFrames(20);
  // Cleared by FieldEffect_DispatchRequestKind: cast on a target a map event
  // names, the target is blown away.
  if (effectWork->enabled == 0) {
    if (target != 0)
      target->update = FieldEffect_ShrinkObjectScaleUntilHalf;

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
      particle = (struct ObjectEvent *)Object_Spawn(0xf9, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = FieldEffect_CircleAnchor;
        particle->linkedObject = ball;
        particle->timer = 0;
        particle->work = 0;
        particle->facing = Random16();
      }
      WaitFrames(6);
      step++;
    } while (step <= WHIRLWIND_ORBIT_PARTICLES - 1);
    WaitFrames(20);
    WaitFrames(120);
  }
  Object_SelectAnimationFar(ball, 1);
  WaitFrames(30);
  Sound_PlaySongFar(SE_WHOOSH);
  WaitFrames(20);
  // Every particle has left its orbit by now. Its wandering callback no
  // longer reads the anchor, so the orb can be released independently.
  Object_ReleaseFar((struct Object *)ball);
  FieldEffect_FadeFromEffectColor();
}
