// Part of psynergy_effects.c, compiled only through it.
// Frost.

#include "../psynergy_effects_internal.h"

// Per-frame update: flashes (animation 2) for 2 frames in every 8.
void BattleFx_UpdateOrbitingParticleMain(union FieldObject *fieldObject) {
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  s32 frame = gFrameCount & 7;
  if (frame == 0) {
    Animation_ApplyChildValuesFar(particle, 2);
  } else if (frame == 2) {
    Animation_ApplyChildValuesFar(particle, 0);
  }
}

// Per-frame update: shrinks by 1/16 a frame and is released at 1/16.
void BattleFx_UpdateOrbitingParticleFade(union FieldObject *fieldObject) {
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  s32 scale;

  if (particle != NULL) {
    scale = particle->scaleX - 0x1000;
    particle->scaleY -= 0x1000;
    particle->scaleX = scale;
    if (scale <= 0x1000)
      ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                   gBattleFx_CommonParticleScript);
  }
}

// Both winding directions share the countdown and position update. timer is
// initialized to 60; preserve its halfword narrowing before computing radius.
static inline void BattleFx_UpdateSpiralParticle(union FieldObject *fieldObject,
                                                 s32 clockwise) {
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  struct EffectVector position;
  s16 remainingFrames;
  s32 decrementedTimer;

  if (particle != NULL) {
    decrementedTimer = (u16)particle->timer - 1;
    particle->timer = decrementedTimer;
    remainingFrames = decrementedTimer;
    if (remainingFrames != 0) {
      position.x = particle->targetX;
      position.y = particle->targetY;
      position.z = particle->targetZ;
      Vector_AddPolarOffset(remainingFrames << 17,
                            clockwise
                                ? (s16)particle->work - (remainingFrames << 11)
                                : (s16)particle->work + (remainingFrames << 11),
                            &position);
      particle->x.fixed = position.x;
      particle->y.fixed = position.y;
      particle->z.fixed = position.z;
    } else {
      particle->update = BattleFx_UpdateOrbitingParticleFade;
    }
  }
}

// Spirals anticlockwise at twice the frames left in pixels, then fades.
void BattleFx_UpdateOrbitingParticleLeft(union FieldObject *fieldObject) {
  BattleFx_UpdateSpiralParticle(fieldObject, 0);
}

// Per-frame update: as BattleFx_UpdateOrbitingParticleLeft, clockwise.
void BattleFx_UpdateOrbitingParticleRight(union FieldObject *fieldObject) {
  BattleFx_UpdateSpiralParticle(fieldObject, 1);
}

// Effect 6 cast on a target (scene->child); with childMode the target is
// left as it is and only the particles appear.
void BattleFx_StartOrbitingParticles(s32 unused) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *target = scene->child;

  if (target != 0) {
    if (scene->childMode != 0) {
      scene->enabled = 1;
    }
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    BattleFx_RunOrbitingParticles();
  }
}

// Psynergy effect 6: 16 particles spiral in on the effect's position and
// make the target flash 16 times; without childOption the target takes
// mode 2.
void BattleFx_RunOrbitingParticles(void) {
  s32 eventTarget;
  struct EffectVector pos;
  struct EffectVector *p;
  struct BattleFxScene *scene;
  struct FieldActor *target;
  struct FieldActor *particle;
  void *event;
  s32 remainingCycles;

  scene = gEffectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(SE_115);

  // A particle every 3 frames, 0.5 to 1 size, turning either way at random,
  // centred 2 to 6 pixels from the effect's position.
  p = &pos;
  remainingCycles = 15;
  do {
    particle = Object_Spawn(0xe8, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 radius;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update = BattleFx_UpdateOrbitingParticleLeft;
      else
        particle->update = BattleFx_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->work = Random16();
      Animation_ApplyChildValuesFar(particle, 9);

      SET_POSITION(p, scene->x, scene->y, scene->z);
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
  AudioCommand_PlayFar(SE_115);
  WaitFrames(50);

  if (target != NULL && scene->enabled == 0) {
    AudioCommand_PlayFar(SE_212);

    // Flash for 1 frame in every 5.
    remainingCycles = 15;
    do {
      Animation_ApplyChildValuesFar(target, 7);
      WaitFrames(1);
      Animation_ApplyChildValuesFar(target, 0);
      WaitFrames(4);
      remainingCycles--;
    } while (remainingCycles >= 0);

    if (scene->childOption == 0) {
      AudioCommand_PlayFar(SE_220);
      ObjectDispatch_ApplyArgumentToChildrenFar(target, 2);
    }

    target->update = BattleFx_UpdateOrbitingParticleMain;
    event = BattleFx_FindMatchingEvent(0x50000005, 6, &eventTarget);
    if (event != NULL) {
      BattleFx_RunEventAction(event, gGameState.selectedActor, eventTarget);
    }
    WaitFrames(20);
  }

  BattleFx_PrepareBufferInterpolation();
}
