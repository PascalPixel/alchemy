/// @file
/// Douse.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// A Actor as Douse's orb and drops use it; the notes give the
/// Actor fields at the same offsets.
struct ActorDouseView {
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
  /// gFieldEffect_SharedParticleScript releases it.
  s16 wait;
  u8 reserved60[0xc];
  void (*callback)(struct Actor *actor); ///< per-frame update
};

/// The field effect (struct FieldEffectWork, gFieldEffectWork) as Douse reads
/// it; the notes give the FieldEffectWork fields at the same offsets.
struct FieldEffectDouseView {
  s32 angle;            ///< 1/65536 turns: the way the caster faces
  s32 x, y, z;          ///< the ground under the orb's destination
  struct Actor *caster; ///< mainActor
  s32 targetActor;      ///< child: the target's actor, 0 for none
  u8 reserved18[8];
  /// enabled: set at the start, cleared when a trigger names the target
  /// (FieldPsynergy_Dispatch). Set, 10 particles fall; cleared, 24
  /// drops, or 30 swaying particles in Mercury Lighthouse.
  u8 highArc;
  u8 reserved21[0x13];
  u8 variant; ///< childOption: set by an event, the orb stops 80 pixels up
  u8 reserved35[0x10];
  /// lit: set in two Mercury Lighthouse scenes (setup.c), where swaying
  /// particles (SPRITE_PARTICLE) fall rather than splashing drops.
  u8 alternate;
};

/// Per-frame update of a falling drop: on reaching altitude it is released 2
/// frames later and scatters three half-size splashes, which hop toward
/// points 32 pixels away and are released after 6 frames.
void FieldEffect_SpawnRandomAngleTriplet(struct Actor *actor) {
  struct ActorDouseView *particle = (struct ActorDouseView *)actor;
  s32 i;
  struct ActorDouseView *p;
  s32 dropWait = 2;
  s32 splashWait;
  s16 *pp;

  if (particle->y <= particle->altitude) {
    particle->wait = dropWait;
    Actor_StartScriptFar((struct Actor *)particle,
                         gFieldEffect_SharedParticleScript);
    p = NULL;
    particle->callback = NULL;

    // Half size, hopping up at a pixel a frame, toward a random point 32 pixels
    // away.
    for (i = 0; i <= 2; i++) {
      p = (struct ActorDouseView *)Actor_Spawn(240, particle->x, particle->y,
                                               particle->z);
      if (p == NULL) {
        break;
      }
      p->scaleY = 0x8000;
      p->scaleX = 0x8000;
      p->flag = 2;
      p->velocityY = 1 << 16;
      p->speed = Random16() + 0x13333;
      Actor_SetTargetPositionFromMagnitudeAngle((struct Actor *)p, 32 << 16,
                                                Random16());
      pp = &p->wait;
      splashWait = 6;
      *pp = splashWait;
      Actor_StartScriptFar((struct Actor *)p,
                           gFieldEffect_SharedParticleScript);
    }
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
/// Douse's swaying particles in Mercury Lighthouse.
void FieldEffect_UpdateDriftingFallActor(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  s32 drift;

  particle->y.fixed -= 0x4ccc;
  drift = Random16();
  particle->x.fixed += drift - Random16();
  if (particle->y.fixed <= particle->groundHeight) {
    Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
  }
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
///
/// Abilities: Douse (FIELD_PSYNERGY_DOUSE).
void FieldPsynergy_DouseOnTarget(s32 unused) { FieldPsynergy_Douse(); }

/// An orb rises from 16 pixels above the caster to 32 pixels above the
/// destination (80 with variant) over 10 frames, growing as it goes, rains
/// particles, and returns.
///
/// Abilities: Douse (FIELD_PSYNERGY_DOUSE).
///
/// The destination is the target's position (FieldPsynergy_SetupActorPair);
/// with no target it is 16 pixels in front of the caster, on the ground.
/// Called by FieldPsynergy_Run, and through FieldPsynergy_DouseOnTarget.
void FieldPsynergy_Douse(void) {
  struct Vector3 pos;
  struct Vector3 start;
  struct Vector3 end;
  struct FieldEffectDouseView *work = gFieldEffectWork;
  struct Actor *caster = work->caster;
  struct ActorDouseView *lightOrb;
  struct Vector3 *from;
  struct Vector3 *to;
  s32 step;
  s32 steps = 11;
  s32 count;
  s32 y;
  struct ActorDouseView *particle;

  lightOrb = (struct ActorDouseView *)Actor_Spawn(239, step = 0, 0, 0);
  if (lightOrb == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  // No target: 16 pixels in front of the caster, at the ground's height.
  if (work->targetActor == 0) {
    work->x = caster->x.fixed;
    work->z = caster->z.fixed;
    Vector_AddPolarOffset(16 << 16, work->angle, (struct Vector3 *)&work->x);
    work->y = MapGrid_GetTerrainHeightFar(0, work->x, work->z);
  }
  from = &start;
  SET_POSITION(from, caster->x.fixed, caster->y.fixed + (16 << 16),
               caster->z.fixed);
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
    wait(1);
  }
  wait(10);

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
        wait(25);
        pos.x = lightOrb->x;
        pos.y = lightOrb->y;
        pos.z = lightOrb->z;
      }
      particle = (struct ActorDouseView *)Actor_Spawn(240, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->altitude = pos.y - (32 << 16);
        particle->callback = FieldEffect_SpawnRandomAngleTriplet;
        particle->flag = 2;
      }
      Sound_PlayFar(SE_WATER);
      wait(6);
    }
    wait(10);
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
      particle = (struct ActorDouseView *)Actor_Spawn(SPRITE_PARTICLE, pos.x,
                                                      pos.y, pos.z);
      if (particle != 0) {
        particle->callback = FieldEffect_UpdateDriftingFallActor;
        particle->flag = 0;
        particle->sprite->priority = 2;
        Actor_SelectAnimationFar(particle, 8);
        Actor_SetSpriteColorModeFar((struct Actor *)particle,
                                    SPRITE_COLOR_WHITE);
      }
      wait(6);
    }
    wait(70);
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
    wait(1);
    step++;
  } while (step < steps);
  Actor_ReleaseFar((struct Actor *)lightOrb);
  FieldPsynergy_FadeFromCastColor();
}
