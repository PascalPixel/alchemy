/// @file
/// Frost.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Per-frame update Frost leaves on its target: sprite param 2 for 2 frames in
/// every 8, otherwise 0, so the target glints.
void FieldEffect_UpdateOrbitingParticleMain(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  s32 frame = gFrameCount & 7;
  if (frame == 0) {
    Anim_ApplyChildValuesFar(particle, SPRITE_COLOR_RBG);
  } else if (frame == 2) {
    Anim_ApplyChildValuesFar(particle, SPRITE_COLOR_NORMAL);
  }
}

/// Per-frame update: shrinks by 1/16 a frame and is released at 1/16.
void FieldEffect_UpdateOrbitingParticleFade(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  s32 scale;

  if (particle != NULL) {
    scale = particle->scaleX - 0x1000;
    particle->scaleY -= 0x1000;
    particle->scaleX = scale;
    if (scale <= 0x1000)
      Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript);
  }
}

/// Both winding directions share the countdown and position update. timer
/// starts at 60 (FieldPsynergy_Frost). The particle circles its target point
/// (targetX to targetZ) at a radius of 2 pixels and an angle of 1/32 turn for
/// each frame left, so it closes in from 118 pixels.
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
///
/// Abilities: Frost (FIELD_PSYNERGY_FROST).
///
/// The target is the actor in effectWork->child. With childMode the
/// target is left as it is and only the particles appear.
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
///
/// Abilities: Frost (FIELD_PSYNERGY_FROST).
///
/// The target's part runs only when a trigger named the target
/// (FieldPsynergy_Dispatch clears enabled) and no event set
/// childMode: it flashes, takes animation 2 with SE_SHIMMER unless childOption
/// is set, keeps glinting (FieldEffect_UpdateOrbitingParticleMain) and runs
/// the scene's Frost event. Called by FieldPsynergy_Run and
/// FieldPsynergy_FrostOnTarget.
void FieldPsynergy_Frost(void) {
  s32 eventTarget;
  struct Vector3 pos;
  struct Vector3 *p;
  struct FieldEffectWork *effectWork;
  struct Actor *target;
  struct Actor *particle;
  void *event;
  s32 remainingCycles;

  effectWork = gFieldEffectWork;
  target = effectWork->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlaySongFar(SE_ICE);

  // A particle every 3 frames, 0.5 to 1 size, turning either way at random,
  // centred 2 to 6 pixels from the Psynergy's position.
  p = &pos;
  remainingCycles = 15;
  do {
    particle = Actor_Spawn(0xe8, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 radius;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update = FieldEffect_UpdateOrbitingParticleLeft;
      else
        particle->update = FieldEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->userValue = Random16();
      Anim_ApplyChildValuesFar(particle, SPRITE_COLOR_BLUE_TINT);

      SET_POSITION(p, effectWork->x, effectWork->y, effectWork->z);
      radius = (Random16() << 2) + (2 << 16);
      Vector_AddPolarOffset(radius, Random16(), p);
      particle->targetX = p->x;
      particle->targetY = p->y;
      particle->targetZ = p->z;
    }

    WaitFrames(3);
    remainingCycles--;
  } while (remainingCycles >= 0);

  WaitFrames(10);
  Sound_PlaySongFar(SE_ICE);
  WaitFrames(50);

  if (target != NULL && effectWork->enabled == 0) {
    Sound_PlaySongFar(SE_POWER_UP);

    // Flash for 1 frame in every 5.
    remainingCycles = 15;
    do {
      Anim_ApplyChildValuesFar(target, SPRITE_COLOR_WHITE);
      WaitFrames(1);
      Anim_ApplyChildValuesFar(target, SPRITE_COLOR_NORMAL);
      WaitFrames(4);
      remainingCycles--;
    } while (remainingCycles >= 0);

    if (effectWork->childOption == 0) {
      Sound_PlaySongFar(SE_SHIMMER);
      Actor_SelectAnimationFar(target, 2);
    }

    target->update = FieldEffect_UpdateOrbitingParticleMain;
    event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                      FIELD_PSYNERGY_FROST, &eventTarget);
    if (event != NULL) {
      Field_RunPsynergyEvent(event, gAdventure.leaderActorId, eventTarget);
    }
    WaitFrames(20);
  }

  FieldPsynergy_FadeFromCastColor();
}
