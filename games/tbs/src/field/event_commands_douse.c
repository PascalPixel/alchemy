/// @file
/// Douse: a rain cloud floats out over the target and rains on it, then
/// shrinks back into the caster.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The drops Douse rains: cast at nothing, cast on a target, and cast on a
/// target where they sway down (FieldEffectState.lit).
#define DOUSE_DROPS 10
#define DOUSE_DROPS_ON_TARGET 24
#define DOUSE_SWAYING_DROPS_ON_TARGET 30

/// Per-frame update of a falling drop: on reaching its ground height it is
/// released 2 frames later and scatters three half-size splashes, which hop
/// toward points 32 pixels away and are released after 6 frames.
void FieldEffect_UpdateDouseDrop(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  s32 i;
  struct Actor *splash;
  s32 dropWait = 2;
  s16 *splashWaitField;

  if (particle->y.fixed <= particle->groundHeight) {
    particle->wait = dropWait;
    Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
    splash = NULL;
    particle->update = NULL;

    // Half size, hopping up at a pixel a frame, toward a random point 32 pixels
    // away.
    for (i = 0; i <= 2; i++) {
      splash = Actor_Spawn(SPRITE_DROPLET, particle->x.fixed, particle->y.fixed,
                           particle->z.fixed);
      if (splash == NULL) {
        break;
      }
      splash->scaleY = FIXED_ONE / 2;
      splash->scaleX = FIXED_ONE / 2;
      splash->motionFlags = ACTOR_FALLS;
      splash->velocityY = toFIXED(1);
      splash->speedLimit = Random16() + toFIXED(6) / 5;
      Actor_SetTargetPositionFromMagnitudeAngle(splash, toFIXED(32),
                                                Random16());
      splashWaitField = &splash->wait;
      {
        s32 splashWait = 6;

        *splashWaitField = splashWait;
        Actor_StartScriptFar(splash, gFieldEffect_SharedParticleScript);
      }
    }
  }
}

/// Per-frame update: drifts from side to side, up to a pixel a frame, as it
/// falls 0.3 pixels a frame, and is released on reaching its ground height.
void FieldEffect_UpdateDriftingFallActor(struct Actor *particle) {
  s32 drift;

  particle->y.fixed -= toFIXED(3) / 10;
  drift = Random16();
  particle->x.fixed += drift - Random16();
  if (particle->y.fixed <= particle->groundHeight) {
    Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
  }
}

/// Runs Douse cast on a target, through FieldPsynergy_Douse.
void FieldPsynergy_DouseOnTarget(s32 unused) { FieldPsynergy_Douse(); }

/// A rain cloud rises from 16 pixels above the caster to 32 pixels above the
/// destination (80 with sceneVariant) over GROW_STEPS frames, growing as it
/// goes, rains drops, and returns.
void FieldPsynergy_Douse(void) {
  struct Vector3 pos;
  struct Vector3 start;
  struct Vector3 end;
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *caster = effectState->caster;
  struct Actor *cloud;
  s32 step;
  s32 steps = GROW_STEPS + 1;
  s32 count;
  s32 y;
  struct Actor *particle;

  cloud = Actor_Spawn(SPRITE_RAIN_CLOUD, step = 0, 0, 0);
  if (cloud == 0)
    return;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  // No target: 16 pixels in front of the caster, at the ground's height.
  if (effectState->target == 0) {
    effectState->x = caster->x.fixed;
    effectState->z = caster->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), effectState->angle,
                          (struct Vector3 *)&effectState->x);
    effectState->y = MapGrid_GetTerrainHeightFar(
        MAPGRID_LAYER_GROUND, effectState->x, effectState->z);
  }
  SET_POSITION(&start, caster->x.fixed, caster->y.fixed + toFIXED(16),
               caster->z.fixed);
  end.x = effectState->x;
  y = effectState->y;
  end.y = y + toFIXED(32);
  end.z = effectState->z;
  if (effectState->sceneVariant != 0)
    end.y = y + toFIXED(80);

  // Grow from a quarter to full size on the way.
  for (; step < steps; step++) {
    s32 scale;

    SET_ACTOR_POSITION(cloud, start.x + (end.x - start.x) * step / GROW_STEPS,
                       start.y + (end.y - start.y) * step / GROW_STEPS,
                       start.z + (end.z - start.z) * step / GROW_STEPS);
    scale = FieldEffect_Interpolate(FIXED_ONE, FIXED_ONE / 4, step);
    cloud->scaleX = scale;
    cloud->scaleY = scale;
    wait(1);
  }
  wait(10);

  if ((s8)effectState->lit == 0) {
    // Drop a particle every 6 frames from 3 to 8 pixels round the cloud (the
    // last right under it); enabled is cleared once a target is dispatched.
    count = DOUSE_DROPS;
    if (effectState->aimAhead == 0)
      count = DOUSE_DROPS_ON_TARGET;
    for (step = 0; step < count; step++) {
      pos.x = cloud->x.fixed;
      pos.y = cloud->y.fixed;
      pos.z = cloud->z.fixed;
      Vector_AddPolarOffset(Random16() * 5 + toFIXED(3), Random16(), &pos);
      if (step == count - 1) {
        wait(25);
        pos.x = cloud->x.fixed;
        pos.y = cloud->y.fixed;
        pos.z = cloud->z.fixed;
      }
      particle = Actor_Spawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->groundHeight = pos.y - toFIXED(32);
        particle->update = FieldEffect_UpdateDouseDrop;
        particle->motionFlags = ACTOR_FALLS;
      }
      Sound_PlayFar(SE_WATER);
      wait(6);
    }
    wait(10);
  } else {
    // Every 6 frames, a swaying particle falls from 3 to 8 pixels round the
    // cloud.
    if (effectState->aimAhead != 0)
      count = DOUSE_DROPS;
    else
      count = DOUSE_SWAYING_DROPS_ON_TARGET;
    for (step = 0; step < count; step++) {
      pos.x = cloud->x.fixed;
      pos.y = cloud->y.fixed;
      pos.z = cloud->z.fixed;
      Vector_AddPolarOffset(Random16() * 5 + toFIXED(3), Random16(), &pos);
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

    SET_ACTOR_POSITION(cloud, end.x + (start.x - end.x) * step / GROW_STEPS,
                       end.y + (start.y - end.y) * step / GROW_STEPS,
                       end.z + (start.z - end.z) * step / GROW_STEPS);
    scale = FieldEffect_Interpolate(FIXED_ONE / 4, FIXED_ONE, step);
    cloud->scaleX = scale;
    cloud->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  Actor_ReleaseFar(cloud);
  FieldPsynergy_FadeFromCastColor();
}

#undef DOUSE_DROPS
#undef DOUSE_DROPS_ON_TARGET
#undef DOUSE_SWAYING_DROPS_ON_TARGET
