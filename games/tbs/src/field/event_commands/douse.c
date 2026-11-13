/// @file
/// Douse.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// The field effect (struct FieldEffectWork, gFieldEffectWork) as Douse reads
/// it; the notes give the FieldEffectWork fields at the same offsets.
struct FieldEffectDouseView {
  s32 angle;            ///< 1/65536 turns: the way the caster faces
  s32 x, y, z;          ///< the ground under the cloud's destination
  struct Actor *caster; ///< mainActor
  s32 targetActor;      ///< child: the target's actor, 0 for none
  u8 reserved18[8];
  /// enabled: set at the start, cleared when a trigger names the target
  /// (FieldPsynergy_Dispatch). Set, 10 particles fall; cleared, 24
  /// drops, or 30 swaying particles in Mercury Lighthouse.
  u8 highArc;
  u8 reserved21[19];
  u8 variant; ///< childOption: set by an event, the cloud stops 80 pixels up
  u8 reserved35[16];
  /// lit: set in two Mercury Lighthouse scenes (setup.c), where swaying
  /// particles (SPRITE_PARTICLE) fall rather than splashing drops.
  u8 alternate;
};

/// Per-frame update of a falling drop: on reaching altitude it is released 2
/// frames later and scatters three half-size splashes, which hop toward
/// points 32 pixels away and are released after 6 frames.
void FieldEffect_UpdateDouseDrop(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  s32 i;
  struct Actor *p;
  s32 dropWait = 2;
  s16 *pp;

  if (particle->y.fixed <= particle->groundHeight) {
    particle->wait = dropWait;
    Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
    p = NULL;
    particle->update = NULL;

    // Half size, hopping up at a pixel a frame, toward a random point 32 pixels
    // away.
    for (i = 0; i <= 2; i++) {
      p = Actor_Spawn(SPRITE_DROPLET, particle->x.fixed, particle->y.fixed,
                      particle->z.fixed);
      if (p == NULL) {
        break;
      }
      p->scaleY = Q16_ONE / 2;
      p->scaleX = Q16_ONE / 2;
      p->motionFlags = 2;
      p->velocityY = px(1);
      p->speedLimit = Random16() + px(6) / 5;
      Actor_SetTargetPositionFromMagnitudeAngle(p, px(32), Random16());
      pp = &p->wait;
      {
        s32 splashWait = 6;

        *pp = splashWait;
        Actor_StartScriptFar(p, gFieldEffect_SharedParticleScript);
      }
    }
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
/// Douse's swaying particles in Mercury Lighthouse.
void FieldEffect_UpdateDriftingFallActor(struct Actor *particle) {
  s32 drift;

  particle->y.fixed -= px(3) / 10;
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

/// A rain cloud rises from 16 pixels above the caster to 32 pixels above the
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
  struct Actor *cloud;
  s32 step;
  s32 steps = 11;
  s32 count;
  s32 y;
  struct Actor *particle;

  cloud = Actor_Spawn(SPRITE_RAIN_CLOUD, step = 0, 0, 0);
  if (cloud == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  // No target: 16 pixels in front of the caster, at the ground's height.
  if (work->targetActor == 0) {
    work->x = caster->x.fixed;
    work->z = caster->z.fixed;
    Vector_AddPolarOffset(px(16), work->angle, (struct Vector3 *)&work->x);
    work->y = MapGrid_GetTerrainHeightFar(0, work->x, work->z);
  }
  SET_POSITION(&start, caster->x.fixed, caster->y.fixed + px(16),
               caster->z.fixed);
  end.x = work->x;
  y = work->y;
  end.y = y + px(32);
  end.z = work->z;
  if ((s8)work->variant != 0)
    end.y = y + px(80);

  // Grow from a quarter to full size on the way.
  for (; step < steps; step++) {
    s32 scale;

    SET_ACTOR_POSITION(cloud, start.x + (end.x - start.x) * step / 10,
                 start.y + (end.y - start.y) * step / 10,
                 start.z + (end.z - start.z) * step / 10);
    scale = FieldEffect_Interpolate(Q16_ONE, Q16_ONE / 4, step);
    cloud->scaleX = scale;
    cloud->scaleY = scale;
    wait(1);
  }
  wait(10);

  if ((s8)work->alternate == 0) {
    // Drop a particle every 6 frames from 3 to 8 pixels round the cloud (the
    // last right under it).
    count = 10;
    if ((s8)work->highArc == 0)
      count = 24;
    for (step = 0; step < count; step++) {
      pos.x = cloud->x.fixed;
      pos.y = cloud->y.fixed;
      pos.z = cloud->z.fixed;
      Vector_AddPolarOffset(Random16() * 5 + px(3), Random16(), &pos);
      if (step == count - 1) {
        wait(25);
        pos.x = cloud->x.fixed;
        pos.y = cloud->y.fixed;
        pos.z = cloud->z.fixed;
      }
      particle = Actor_Spawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->groundHeight = pos.y - px(32);
        particle->update = FieldEffect_UpdateDouseDrop;
        particle->motionFlags = 2;
      }
      Sound_PlayFar(SE_WATER);
      wait(6);
    }
    wait(10);
  } else {
    // Every 6 frames, a swaying particle falls from 3 to 8 pixels round the
    // cloud.
    if ((s8)work->highArc != 0)
      count = 10;
    else
      count = 30;
    for (step = 0; step < count; step++) {
      pos.x = cloud->x.fixed;
      pos.y = cloud->y.fixed;
      pos.z = cloud->z.fixed;
      Vector_AddPolarOffset(Random16() * 5 + px(3), Random16(), &pos);
      particle = Actor_Spawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = FieldEffect_UpdateDriftingFallActor;
        particle->motionFlags = 0;
        particle->sprite->image.priority = 2;
        Actor_SelectAnimationFar(particle, 8);
        Actor_SetSpriteColorModeFar(particle, SPRITE_COLOR_WHITE);
      }
      wait(6);
    }
    wait(70);
  }

  // Shrink back to a quarter on the way back to the caster.
  step = 0;
  do {
    s32 scale;

    SET_ACTOR_POSITION(cloud, end.x + (start.x - end.x) * step / 10,
                 end.y + (start.y - end.y) * step / 10,
                 end.z + (start.z - end.z) * step / 10);
    scale = FieldEffect_Interpolate(Q16_ONE / 4, Q16_ONE, step);
    cloud->scaleX = scale;
    cloud->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  Actor_ReleaseFar(cloud);
  FieldPsynergy_FadeFromCastColor();
}
