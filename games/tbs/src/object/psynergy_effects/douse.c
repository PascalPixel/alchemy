/// @file
/// Douse.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// A FieldActor seen with the halfword at 0x5e (here its phase), which
/// FieldActor does not name.
struct Effect05Object {
  u8 reserved00[8];
  s32 x, y, z;
  s32 altitude; ///< the height it falls to
  s32 scaleX, scaleY;
  u8 reserved20[8];
  s32 velocityY;
  u8 reserved2c[4];
  s32 speed;
  u8 reserved34[0x1c];
  struct WaveFxSprite *sprite;
  u8 reserved54;
  u8 flag;
  u8 reserved56[8];
  s16 phase;
  u8 reserved60[0xc];
  void (*callback)(union FieldObject *object); ///< per-frame update
};

struct Effect05State {
  s32 angle;
  s32 x, y, z; ///< the ground under the orb's destination
  struct Object *target;
  s32 initialized;
  u8 reserved18[8];
  u8 highArc;
  u8 reserved21[0x13];
  u8 variant;
  u8 reserved35[0x10];
  u8 alternate;
};

/// Per-frame update of a falling particle: on reaching altitude, it scatters
/// three small particles.
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
      p = Object_Spawn(0xf0, particle->x, particle->y, particle->z);
      if (p == NULL) {
        break;
      }
      p->scaleY = 0x8000;
      p->scaleX = 0x8000;
      p->flag = 2;
      p->velocityY = 1 << 16;
      p->speed = Random16() + 0x13333;
      Motion_SetTargetPositionFromMagnitudeAngle(p, 32 << 16, Random16());
      pp = &p->phase;
      phase2 = 6;
      *pp = phase2;
      ObjectDispatch_InitializeFar((struct DispatchObject *)p,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

/// Per-frame update: drifts from side to side as it slowly falls, and is
/// released on reaching the ground.
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
/// Abilities: Douse (field kind 5), cast on a target.
void FieldFx_RunDouseOnTarget(s32 unused) { FieldFx_RunDouse(); }

/// An orb of light rises from 16 pixels above the target to 32 pixels above the
/// destination (80 with variant) over 10 frames, growing as it goes, rains
/// particles, and returns.
///
/// Abilities: Douse (field kind 5).
///
/// With no destination yet, the destination is 16 pixels from the target toward
/// angle.
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

  lightOrb = Object_Spawn(0xef, step = 0, 0, 0);
  if (lightOrb == 0)
    return;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(SE_ITEM_BREAK);
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
      particle = Object_Spawn(0xf0, pos.x, pos.y, pos.z);
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
      particle = Object_Spawn(0x11c, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->callback = BattleFx_UpdateDriftingFallObject;
        particle->flag = 0;
        particle->sprite->mode = 2;
        ObjectDispatch_SelectAnimationFar(particle, 8);
        Animation_ApplyChildValuesFar(particle, 7);
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
  ObjectDispatch_ReleaseFar(lightOrb);
  BattleFx_PrepareBufferInterpolation();
}
