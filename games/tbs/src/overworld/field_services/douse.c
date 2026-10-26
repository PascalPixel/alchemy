/// @file
/// Douse.
// Part of field_services.c.

#include "../field_services_internal.h"

/// A ObjectEvent as Douse's orb and drops use it; the notes give the
/// ObjectEvent fields at the same offsets.
struct ObjectEventDouseView {
  u8 reserved00[8];
  s32 x, y, z;  ///< 16.16 fixed-point pixels
  s32 altitude; ///< groundHeight: a drop's height to fall to
  s32 scaleX, scaleY;
  u8 reserved20[8];
  s32 velocityY; ///< 16.16 fixed-point pixels per frame, up
  u8 reserved2c[4];
  s32 speed; ///< speed limit, 16.16 fixed-point pixels per frame
  u8 reserved34[0x1c];
  struct WaveEffectSprite *sprite;
  u8 reserved54;
  u8 flag; ///< motionFlags: 2 falls, 0 free flight
  u8 reserved56[8];
  /// Frames its script waits before it runs on; 2 or 6 here, before
  /// gFieldEffect_CommonParticleScript frees it.
  s16 wait;
  u8 reserved60[0xc];
  void (*callback)(
      union ObjectEventOrEffect *fieldObject); ///< per-frame update
};

/// The field effect (struct FieldEffectWork, gFieldEffectWork) as Douse reads
/// it; the notes give the FieldEffectWork fields at the same offsets.
struct FieldEffectDouseView {
  s32 angle;             ///< 1/65536 turns: the way the caster faces
  s32 x, y, z;           ///< the ground under the orb's destination
  struct Object *caster; ///< mainObject
  s32 targetObject;      ///< child: the target's object, 0 for none
  u8 reserved18[8];
  /// enabled: set at the start, cleared when a map event names the target
  /// (FieldEffect_DispatchRequestKind). Set, 10 particles fall; cleared, 24
  /// drops, or 30 swaying particles in Mercury Lighthouse.
  u8 highArc;
  u8 reserved21[0x13];
  u8 variant; ///< childOption: set by a map script, the orb stops 80 pixels up
  u8 reserved35[0x10];
  /// lit: set in two Mercury Lighthouse maps (setup.c), where swaying
  /// particles (SPRITE_PARTICLE) fall rather than splashing drops.
  u8 alternate;
};

/// Per-frame update of a falling drop: on reaching altitude it is freed 2
/// frames later and scatters three half-size splashes, which hop toward
/// points 32 pixels away and are freed after 6 frames.
void FieldEffect_SpawnRandomAngleTriplet(
    union ObjectEventOrEffect *fieldObject) {
  struct ObjectEventDouseView *particle =
      (struct ObjectEventDouseView *)fieldObject;
  s32 i;
  struct ObjectEventDouseView *p;
  s32 dropWait = 2;
  s32 splashWait;
  s16 *pp;

  if (particle->y <= particle->altitude) {
    particle->wait = dropWait;
    Object_InitializeFar((struct Object *)particle,
                         gFieldEffect_CommonParticleScript);
    p = NULL;
    particle->callback = NULL;

    // Half size, hopping up at a pixel a frame, toward a random point 32 pixels
    // away.
    for (i = 0; i <= 2; i++) {
      p = (struct ObjectEventDouseView *)Object_Spawn(0xf0, particle->x,
                                                      particle->y, particle->z);
      if (p == NULL) {
        break;
      }
      p->scaleY = 0x8000;
      p->scaleX = 0x8000;
      p->flag = 2;
      p->velocityY = 1 << 16;
      p->speed = Random16() + 0x13333;
      ObjectMotion_SetTargetPositionFromMagnitudeAngle((struct Object *)p,
                                                       32 << 16, Random16());
      pp = &p->wait;
      splashWait = 6;
      *pp = splashWait;
      Object_InitializeFar((struct Object *)p,
                           gFieldEffect_CommonParticleScript);
    }
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
/// Douse's swaying particles in Mercury Lighthouse.
void FieldEffect_UpdateDriftingFallObject(
    union ObjectEventOrEffect *fieldObject) {
  struct Object *particle = (struct Object *)fieldObject;
  s32 drift;

  particle->y -= 0x4ccc;
  drift = Random16();
  particle->x += drift - Random16();
  if (particle->y <= particle->groundHeight) {
    Object_InitializeFar((struct Object *)particle,
                         gFieldEffect_CommonParticleScript);
  }
}

/// Runs Douse cast on a target, through FieldEffect_RunDouse.
///
/// Abilities: Douse (FIELD_PSYNERGY_DOUSE).
void FieldEffect_RunDouseOnTarget(s32 unused) { FieldEffect_RunDouse(); }

/// An orb rises from 16 pixels above the caster to 32 pixels above the
/// destination (80 with variant) over 10 frames, growing as it goes, rains
/// particles, and returns.
///
/// Abilities: Douse (FIELD_PSYNERGY_DOUSE).
///
/// The destination is the target's position (FieldEffect_SetupObjectPair);
/// with no target it is 16 pixels in front of the caster, on the ground.
/// Called by FieldEffect_Run, and through FieldEffect_RunDouseOnTarget.
void FieldEffect_RunDouse(void) {
  struct Vector3 pos;
  struct Vector3 start;
  struct Vector3 end;
  struct FieldEffectDouseView *work = gFieldEffectWork;
  struct Object *caster = work->caster;
  struct ObjectEventDouseView *lightOrb;
  struct Vector3 *from;
  struct Vector3 *to;
  s32 step;
  s32 steps = 11;
  s32 count;
  s32 y;
  struct ObjectEventDouseView *particle;

  lightOrb = (struct ObjectEventDouseView *)Object_Spawn(0xef, step = 0, 0, 0);
  if (lightOrb == 0)
    return;
  FieldEffect_FadeToEffectColor();
  Sound_PlaySongFar(SE_ITEM_BREAK);
  // No target: 16 pixels in front of the caster, at the ground's height.
  if (work->targetObject == 0) {
    work->x = caster->x;
    work->z = caster->z;
    Vector_AddPolarOffset(16 << 16, work->angle, (struct Vector3 *)&work->x);
    work->y = MapGrid_GetTerrainHeightFar(0, work->x, work->z);
  }
  from = &start;
  SET_POSITION(from, caster->x, caster->y + (16 << 16), caster->z);
  to = &end;
  to->x = work->x;
  y = work->y;
  to->y = y + (32 << 16);
  to->z = work->z;
  if ((s8)work->variant != 0)
    to->y = y + (80 << 16);

  // Grow from a quarter to full size on the way.
  for (; step < steps; step++) {
    s32 scale;

    {
      struct Vector3 *a = &start;
      struct Vector3 *b = &end;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += step * (e - t) / 10;
      lightOrb->x = t;
      e = b->y;
      t = a->y;
      t += step * (e - t) / 10;
      lightOrb->y = t;
      e = b->z;
      t = a->z;
      t += step * (e - t) / 10;
      lightOrb->z = t;
    }
    scale = FieldEffect_Interpolate(Q16_ONE, 0x4000, step);
    lightOrb->scaleX = scale;
    lightOrb->scaleY = scale;
    WaitFrames(1);
  }
  WaitFrames(10);

  if ((s8)work->alternate == 0) {
    // Drop a particle every 6 frames from 3 to 8 pixels round the orb (the
    // last right under it).
    count = 10;
    if ((s8)work->highArc == 0)
      count = 24;
    for (step = 0; step < count; step++) {
      pos.x = lightOrb->x;
      pos.y = lightOrb->y;
      pos.z = lightOrb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      if (step == count - 1) {
        WaitFrames(25);
        pos.x = lightOrb->x;
        pos.y = lightOrb->y;
        pos.z = lightOrb->z;
      }
      particle = (struct ObjectEventDouseView *)Object_Spawn(0xf0, pos.x, pos.y,
                                                             pos.z);
      if (particle != 0) {
        particle->altitude = pos.y - (32 << 16);
        particle->callback = FieldEffect_SpawnRandomAngleTriplet;
        particle->flag = 2;
      }
      Sound_PlaySongFar(SE_WATER);
      WaitFrames(6);
    }
    WaitFrames(10);
  } else {
    // Every 6 frames, a swaying particle falls from 3 to 8 pixels round the
    // orb.
    if ((s8)work->highArc != 0)
      count = 10;
    else
      count = 30;
    for (step = 0; step < count; step++) {
      pos.x = lightOrb->x;
      pos.y = lightOrb->y;
      pos.z = lightOrb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      particle = (struct ObjectEventDouseView *)Object_Spawn(
          SPRITE_PARTICLE, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->callback = FieldEffect_UpdateDriftingFallObject;
        particle->flag = 0;
        particle->sprite->priority = 2;
        Object_SelectAnimationFar(particle, 8);
        Anim_ApplyChildValuesFar((struct Object *)particle, SPRITE_COLOR_WHITE);
      }
      WaitFrames(6);
    }
    WaitFrames(70);
  }

  // Shrink back to a quarter on the way back over the target.
  step = 0;
  do {
    s32 scale;

    {
      struct Vector3 *a = &end;
      struct Vector3 *b = &start;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += step * (e - t) / 10;
      lightOrb->x = t;
      e = b->y;
      t = a->y;
      t += step * (e - t) / 10;
      lightOrb->y = t;
      e = b->z;
      t = a->z;
      t += step * (e - t) / 10;
      lightOrb->z = t;
    }
    scale = FieldEffect_Interpolate(0x4000, Q16_ONE, step);
    lightOrb->scaleX = scale;
    lightOrb->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  Object_ReleaseFar((struct Object *)lightOrb);
  FieldEffect_FadeFromEffectColor();
}
