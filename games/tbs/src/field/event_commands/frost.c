/// @file
/// Frost.
// Part of event_commands.c.

#include "../event_commands_internal.h"

#include "scene/frost_glint.h"

/// Frost's glint on its target.
DEFINE_FROST_GLINT(, Actor_SetSpriteColorModeFar)

/// Per-frame update: shrinks by 1/16 a frame and is released at 1/16.
void FieldEffect_UpdateOrbitingParticleFade(struct Actor *particle) {
  s32 scale;

  if (particle != NULL) {
    scale = particle->scaleX - Q16_ONE / 16;
    particle->scaleY -= Q16_ONE / 16;
    particle->scaleX = scale;
    if (scale <= Q16_ONE / 16)
      Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
  }
}

/// Both winding directions share the countdown and position update. timer
/// starts at 60 (FieldPsynergy_Frost).
static inline void FieldEffect_UpdateSpiralParticle(struct Actor *actor,
                                                    s32 clockwise) {
  struct Actor *particle = (struct Actor *)actor;
  struct Vector3 position;
  s16 remainingFrames;
  s32 decrementedTimer;

  if (particle != NULL) {
    // Keep the countdown's halfword narrowing before it sets the radius.
    decrementedTimer = (u16)particle->timer - 1;
    particle->timer = decrementedTimer;
    remainingFrames = decrementedTimer;
    if (remainingFrames != 0) {
      position.x = particle->targetX;
      position.y = particle->targetY;
      position.z = particle->targetZ;
      Vector_AddPolarOffset(
          remainingFrames << 17,
          clockwise ? (s16)particle->userValue - (remainingFrames << 11)
                    : (s16)particle->userValue + (remainingFrames << 11),
          &position);
      particle->x.fixed = position.x;
      particle->y.fixed = position.y;
      particle->z.fixed = position.z;
    } else {
      particle->update = FieldEffect_UpdateOrbitingParticleFade;
    }
  }
}

/// Spirals anticlockwise at twice the frames left in pixels, then fades.
void FieldEffect_UpdateOrbitingParticleLeft(struct Actor *actor) {
  FieldEffect_UpdateSpiralParticle(actor, 0);
}

/// Per-frame update: as FieldEffect_UpdateOrbitingParticleLeft, clockwise.
void FieldEffect_UpdateOrbitingParticleRight(struct Actor *actor) {
  FieldEffect_UpdateSpiralParticle(actor, 1);
}

/// Runs Frost cast on a target.
void FieldPsynergy_FrostOnTarget(s32 unused) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *target = effectWork->child;

  if (target != 0) {
    // enabled set skips the target's part of FieldPsynergy_Frost.
    if (effectWork->childMode != 0) {
      effectWork->enabled = 1;
    }
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    FieldPsynergy_Frost();
  }
}

/// 16 particles spiral in on the Psynergy's position and make the target flash
/// 16 times; without childOption the target takes animation 2.
void FieldPsynergy_Frost(void) {
  s32 eventTarget;
  struct Vector3 pos;
  struct FieldEffectWork *effectWork;
  struct Actor *target;
  struct Actor *particle;
  void *event;
  s32 i;

  effectWork = gFieldEffectWork;
  target = effectWork->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ICE);

  // A particle every 3 frames, 0.5 to 1 size, turning either way at random,
  // centred 2 to 6 pixels from the Psynergy's position.
  for (i = 0; i < 16; i++) {
    particle = Actor_Spawn(SPRITE_SNOWFLAKE, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 radius;

      scale = (Random16() >> 1) + Q16_ONE / 2;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update = FieldEffect_UpdateOrbitingParticleLeft;
      else
        particle->update = FieldEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->userValue = Random16();
      Actor_SetSpriteColorModeFar(particle, SPRITE_COLOR_BLUE_TINT);

      SET_POSITION(&pos, effectWork->x, effectWork->y, effectWork->z);
      radius = (Random16() << 2) + px(2);
      Vector_AddPolarOffset(radius, Random16(), &pos);
      particle->targetX = pos.x;
      particle->targetY = pos.y;
      particle->targetZ = pos.z;
    }

    wait(3);
  }

  wait(10);
  Sound_PlayFar(SE_ICE);
  wait(50);

  if (target != NULL && effectWork->enabled == 0) {
    Sound_PlayFar(SE_POWER_UP);

    // Flash for 1 frame in every 5.
    for (i = 0; i < 16; i++) {
      Actor_SetSpriteColorModeFar(target, SPRITE_COLOR_WHITE);
      wait(1);
      Actor_SetSpriteColorModeFar(target, SPRITE_COLOR_NORMAL);
      wait(4);
    }

    if (effectWork->childOption == 0) {
      Sound_PlayFar(SE_SHIMMER);
      Actor_SelectAnimationFar(target, PUDDLE_ANIMATION_FREEZE);
    }

    target->update = FieldEffect_UpdateFrostGlint;
    event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                      FIELD_PSYNERGY_FROST, &eventTarget);
    if (event != NULL) {
      Field_RunPsynergyEvent(event, gMainRoutineWork.leaderActorId,
                             eventTarget);
    }
    wait(20);
  }

  FieldPsynergy_FadeFromCastColor();
}
