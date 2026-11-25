/// @file
/// Frost: snowflakes spiral in on the target, and a puddle freezes into an
/// ice pillar.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "scene/frost_glint.h"

#define FROST_SNOWFLAKES 16    ///< spiralling in on the Psynergy's position
#define FROST_SPIRAL_FRAMES 60 ///< each snowflake's spiral
#define FROST_FLASHES 16       ///< the target's white flashes

/// Frost's glint on its target.
DEFINE_FROST_GLINT(, Actor_SetSpriteColorModeFar)

/// Per-frame update: shrinks by 1/16 a frame and is released at 1/16.
void FieldEffect_UpdateOrbitingParticleFade(struct Actor *particle) {
  FIXED scale;

  if (particle != NULL) {
    scale = particle->scaleX - FIXED_ONE / 16;
    particle->scaleY -= FIXED_ONE / 16;
    particle->scaleX = scale;
    if (scale <= FIXED_ONE / 16)
      Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
  }
}

/// Both winding directions share the countdown and position update. timer
/// starts at FROST_SPIRAL_FRAMES (FieldPsynergy_Frost).
static inline void FieldEffect_UpdateSpiralParticle(struct Actor *actor,
                                                    s32 clockwise) {
  struct Actor *particle = actor;
  struct Vector3 position;
  s16 remainingFrames;
  s32 decrementedTimer;

  if (particle != NULL) {
    // timer counts the spiral's frames down; the radius shrinks with it.
    decrementedTimer = (u16)particle->timer - 1;
    particle->timer = decrementedTimer;
    remainingFrames = decrementedTimer;
    if (remainingFrames != 0) {
      position.x = particle->targetX;
      position.y = particle->targetY;
      position.z = particle->targetZ;
      Vector_AddPolarOffset(
          remainingFrames * toFIXED(2),
          clockwise ? particle->userValue - remainingFrames * (TRIG_TURN / 32)
                    : particle->userValue + remainingFrames * (TRIG_TURN / 32),
          &position);
      particle->x.fixed = position.x;
      particle->y.fixed = position.y;
      particle->z.fixed = position.z;
    } else {
      particle->update = FieldEffect_UpdateOrbitingParticleFade;
    }
  }
}

/// Per-frame update of a snowflake: spirals anticlockwise in to the target
/// point (targetX to targetZ), at twice the frames left in pixels, a 32nd of a
/// turn further round for each, then fades.
void FieldEffect_UpdateOrbitingParticleLeft(struct Actor *actor) {
  FieldEffect_UpdateSpiralParticle(actor, 0);
}

/// Per-frame update: as FieldEffect_UpdateOrbitingParticleLeft, clockwise.
void FieldEffect_UpdateOrbitingParticleRight(struct Actor *actor) {
  FieldEffect_UpdateSpiralParticle(actor, 1);
}

/// Runs Frost cast on a target.
void FieldPsynergy_FrostOnTarget(s32 unused) {
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *target = effectState->target;

  if (target != 0) {
    // With leaveTarget the target neither flashes nor freezes.
    if (effectState->leaveTarget != 0) {
      effectState->aimAhead = 1;
    }
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    FieldPsynergy_Frost();
  }
}

/// FROST_SNOWFLAKES snowflakes spiral in on the Psynergy's position and make
/// the target flash FROST_FLASHES times; without sceneVariant the target
/// freezes.
void FieldPsynergy_Frost(void) {
  s32 eventTarget;
  struct Vector3 pos;
  struct FieldEffectState *effectState;
  struct Actor *target;
  struct Actor *particle;
  void *event;
  s32 i;

  effectState = gFieldEffectState;
  target = effectState->target;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ICE);

  // A particle every 3 frames, 0.5 to 1 size, turning either way at random,
  // centred 2 to 6 pixels from the Psynergy's position.
  for (i = 0; i < FROST_SNOWFLAKES; i++) {
    particle = Actor_Spawn(SPRITE_SNOWFLAKE, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      FIXED radius;

      scale = (Random16() >> 1) + FIXED_ONE / 2;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update = FieldEffect_UpdateOrbitingParticleLeft;
      else
        particle->update = FieldEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = FROST_SPIRAL_FRAMES;
      particle->userValue = Random16();
      Actor_SetSpriteColorModeFar(particle, SPRITE_COLOR_BLUE_TINT);

      SET_POSITION(&pos, effectState->x, effectState->y, effectState->z);
      radius = (Random16() << 2) + toFIXED(2);
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

  if (target != NULL && effectState->aimAhead == 0) {
    Sound_PlayFar(SE_POWER_UP);

    // Flash for 1 frame in every 5.
    for (i = 0; i < FROST_FLASHES; i++) {
      Actor_SetSpriteColorModeFar(target, SPRITE_COLOR_WHITE);
      wait(1);
      Actor_SetSpriteColorModeFar(target, SPRITE_COLOR_NORMAL);
      wait(4);
    }

    if (effectState->sceneVariant == 0) {
      Sound_PlayFar(SE_SHIMMER);
      Actor_SelectAnimationFar(target, PUDDLE_ANIMATION_FREEZE);
    }

    target->update = FieldEffect_UpdateFrostGlint;
    event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                      FIELD_PSYNERGY_FROST, &eventTarget);
    if (event != NULL) {
      Field_RunPsynergyEvent(event, gMainRoutineState.leaderActorId,
                             eventTarget);
    }
    wait(20);
  }

  FieldPsynergy_FadeFromCastColor();
}

#undef FROST_SNOWFLAKES
#undef FROST_SPIRAL_FRAMES
#undef FROST_FLASHES
