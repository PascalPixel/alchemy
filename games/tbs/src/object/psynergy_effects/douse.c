/// @file
/// Douse.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// A FieldActor as Douse's orb and drops use it; the notes give the
/// FieldActor fields at the same offsets.
struct Effect05Object {
  u8 reserved00[8];
  s32 x, y, z;  ///< 16.16 fixed-point pixels
  s32 altitude; ///< terrainHeight: a drop's height to fall to
  s32 scaleX, scaleY;
  u8 reserved20[8];
  s32 velocityY; ///< 16.16 fixed-point pixels per frame, up
  u8 reserved2c[4];
  s32 speed; ///< speed limit, 16.16 fixed-point pixels per frame
  u8 reserved34[0x1c];
  struct WaveFxSprite *sprite;
  u8 reserved54;
  u8 flag; ///< motionFlags: 2 falls, 0 free flight
  u8 reserved56[8];
  /// wait: frames its script waits before it runs on; 2 or 6 here, before
  /// gBattleFx_CommonParticleScript frees it.
  s16 phase;
  u8 reserved60[0xc];
  void (*callback)(union FieldObject *object); ///< per-frame update
};

/// The effect scene (struct BattleFxScene, gEffectWork) as Douse reads it;
/// the notes give the BattleFxScene fields at the same offsets.
struct Effect05State {
  s32 angle;             ///< 1/65536 turns: the way the caster faces
  s32 x, y, z;           ///< the ground under the orb's destination
  struct Object *target; ///< mainObject: the caster, despite the name
  s32 initialized;       ///< child: the target's object, 0 for none
  u8 reserved18[8];
  /// enabled: set at the start, cleared when a scene event names the target
  /// (BattleFx_DispatchRequestKind). Set, 10 particles fall; cleared, 24
  /// drops, or 30 swaying particles in Mercury Lighthouse.
  u8 highArc;
  u8 reserved21[0x13];
  u8 variant; ///< childOption: set by a scene, the orb stops 80 pixels up
  u8 reserved35[0x10];
  /// lit: set in two Mercury Lighthouse maps (setup.c), where swaying
  /// particles (object 0x11c) fall rather than splashing drops.
  u8 alternate;
};

/// Per-frame update of a falling drop: on reaching altitude it is freed 2
/// frames later and scatters three half-size splashes, which hop toward
/// points 32 pixels away and are freed after 6 frames.
void FieldFx_SpawnRandomAngleTriplet(union FieldObject *fieldObject) {
  struct Effect05Object *particle = (struct Effect05Object *)fieldObject;
  s32 i;
  struct Effect05Object *p;
  s32 phase = 2;
  s32 phase2;
  s16 *pp;

  if (particle->y <= particle->altitude) {
    particle->phase = phase;
    ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                 gBattleFx_CommonParticleScript);
    p = NULL;
    particle->callback = NULL;

    // Half size, hopping up at a pixel a frame, toward a random point 32 pixels
    // away.
    for (i = 0; i <= 2; i++) {
      p = (struct Effect05Object *)Object_Spawn(0xf0, particle->x, particle->y,
                                                particle->z);
      if (p == NULL) {
        break;
      }
      p->scaleY = 0x8000;
      p->scaleX = 0x8000;
      p->flag = 2;
      p->velocityY = 1 << 16;
      p->speed = Random16() + 0x13333;
      Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)p,
                                                 32 << 16, Random16());
      pp = &p->phase;
      phase2 = 6;
      *pp = phase2;
      ObjectDispatch_InitializeFar((struct DispatchObject *)p,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
/// Douse's swaying particles in Mercury Lighthouse.
void BattleFx_UpdateDriftingFallObject(union FieldObject *fieldObject) {
  struct ObjectRuntime *particle = (struct ObjectRuntime *)fieldObject;
  s32 drift;

  particle->y -= 0x4ccc;
  drift = Random16();
  particle->x += drift - Random16();
  if (particle->y <= particle->terrainHeight) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                 gBattleFx_CommonParticleScript);
  }
}

/// Runs Douse cast on a target, through FieldFx_RunDouse.
///
/// Abilities: Douse (FIELD_KIND_DOUSE).
void FieldFx_RunDouseOnTarget(s32 unused) { FieldFx_RunDouse(); }

/// An orb rises from 16 pixels above the caster to 32 pixels above the
/// destination (80 with variant) over 10 frames, growing as it goes, rains
/// particles, and returns.
///
/// Abilities: Douse (FIELD_KIND_DOUSE).
///
/// The destination is the target's position (BattleFx_SetupObjectPair);
/// with no target it is 16 pixels in front of the caster, on the ground.
/// Called by BattleFx_Run, and through FieldFx_RunDouseOnTarget.
void FieldFx_RunDouse(void) {
  struct EffectVector pos;
  struct EffectVector start;
  struct EffectVector end;
  struct Effect05State *work = gEffectWork;
  struct Object *target = work->target;
  struct Effect05Object *lightOrb;
  struct EffectVector *from;
  struct EffectVector *to;
  s32 step;
  s32 steps = 11;
  s32 count;
  s32 y;
  struct Effect05Object *particle;

  lightOrb = (struct Effect05Object *)Object_Spawn(0xef, step = 0, 0, 0);
  if (lightOrb == 0)
    return;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(SE_ITEM_BREAK);
  // No target: 16 pixels in front of the caster, at the ground's height.
  if (work->initialized == 0) {
    work->x = target->pos.x;
    work->z = target->pos.z;
    Vector_AddPolarOffset(16 << 16, work->angle,
                          (struct EffectVector *)&work->x);
    work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
  }
  from = &start;
  SET_POSITION(from, target->pos.x, target->pos.y + (16 << 16), target->pos.z);
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
      struct EffectVector *a = &start;
      struct EffectVector *b = &end;
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
    scale = BattleFx_Interpolate(0x10000, 0x4000, step);
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
      particle =
          (struct Effect05Object *)Object_Spawn(0xf0, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->altitude = pos.y - (32 << 16);
        particle->callback = FieldFx_SpawnRandomAngleTriplet;
        particle->flag = 2;
      }
      AudioCommand_PlayFar(SE_132);
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
      particle =
          (struct Effect05Object *)Object_Spawn(0x11c, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->callback = BattleFx_UpdateDriftingFallObject;
        particle->flag = 0;
        particle->sprite->mode = 2;
        ObjectDispatch_SelectAnimationFar(particle, 8);
        Animation_ApplyChildValuesFar((struct DispatchObject *)particle, 7);
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
      struct EffectVector *a = &end;
      struct EffectVector *b = &start;
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
    scale = BattleFx_Interpolate(0x4000, 0x10000, step);
    lightOrb->scaleX = scale;
    lightOrb->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ObjectDispatch_ReleaseFar((struct DispatchObject *)lightOrb);
  BattleFx_PrepareBufferInterpolation();
}
