#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "objdisp.h"
#include "scene.h"
#include "object_effect_scripts.h"
#include "fixed_math.h"
#include "system.h"
#include "frame.h"
#include "actor_view.h"
#include "actor.h"
#include "fx_scene.h"
#include "ram_buffer.h"
#include "macros.h"

struct OrbitingParticle;

s32 BattleEffect_UpdateOrbitingParticleMain(struct OrbitingParticle *particle) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    Animation_ApplyChildValuesFar((struct ActorView *)particle, 2);
  } else if (phase == 2) {
    Animation_ApplyChildValuesFar((struct ActorView *)particle, 0);
  }
  return 0;
}

void BattleEffect_UpdateOrbitingParticleFade(struct Actor *object) {
  s32 scale;

  if (object != NULL) {
    scale = object->scaleX - 0x1000;
    object->scaleY -= 0x1000;
    object->scaleX = scale;
    if (scale <= 0x1000)
      Actor_StartScriptFar((struct ActorView *)object,
                                   (u32)gBattleEffect_SharedParticleScript);
  }
}

struct OrbitingParticleVector {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct OrbitingParticleVector *pos);

void BattleEffect_UpdateOrbitingParticleLeft(struct Actor *particle) {
  struct OrbitingParticleVector pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->action - 1;
    particle->action = count;
    life = count;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset(life << 17, (s16)particle->unk66 + (life << 11),
                            &pos);
      SET_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (void (*)(void))BattleEffect_UpdateOrbitingParticleFade;
    }
  }
}

void BattleEffect_UpdateOrbitingParticleRight(struct Actor *particle) {
  struct OrbitingParticleVector pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->action - 1;
    particle->action = count;
    life = count;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset(life << 17, (s16)particle->unk66 - (life << 11),
                            &pos);
      SET_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (void (*)(void))BattleEffect_UpdateOrbitingParticleFade;
    }
  }
}

struct OrbitingParticleChild {
  u8 reserved00[35];
  u8 flags;
};

void FieldEffect_RunFrostOnTarget(void) {
  struct BattleFxScene *state = Ram_HeapSlots->effectWork;
  struct OrbitingParticleChild *child = state->child;

  if (child != 0) {
    if (state->childMode != 0) {
      state->enabled = 1;
    }
    child->flags |= 2;
    FieldEffect_RunFrost();
  }
}

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

/// Sixteen particles, one every three frames, orbit points scattered 2 to 6
/// pixels around the effect; the target then flashes eight times, takes the
/// orbiting particles' main update and runs its pending event.
void FieldEffect_RunFrost(void) {
  struct OrbitingParticleVector pos;
  struct OrbitingParticleVector *p;
  struct BattleFxScene *scene;
  struct Actor *target;
  struct Actor *particle;
  struct PendingEvent *ev;
  s32 cnt;

  scene = Ram_HeapSlots->effectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(115);

  p = &pos;
  cnt = 15;
  do {
    particle = Actor_Spawn(681, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 dist;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update = (void (*)(void))BattleEffect_UpdateOrbitingParticleLeft;
      else
        particle->update = (void (*)(void))BattleEffect_UpdateOrbitingParticleRight;

      particle->angle = Random16();
      particle->action = 60;
      particle->unk66 = Random16();
      Animation_ApplyChildValuesFar((struct ActorView *)particle, 9);

      SET_POSITION(p, scene->x, scene->y, scene->z);
      dist = (Random16() << 2) + (2 << 16);
      Vector_AddPolarOffset(dist, Random16(), p);
      particle->targetX = p->x;
      particle->targetY = p->y;
      particle->targetZ = p->z;
    }

    wait(3);
    cnt--;
  } while (cnt >= 0);

  wait(10);
  Sound_PlayFar(115);
  wait(50);

  if (target != NULL && scene->enabled == 0) {
    Sound_PlayFar(212);

    cnt = 7;
    do {
      Animation_ApplyChildValuesFar((struct ActorView *)target, 7);
      wait(1);
      Animation_ApplyChildValuesFar((struct ActorView *)target, 0);
      wait(4);
      cnt--;
    } while (cnt >= 0);

    if (scene->childOption == 0) {
      Sound_PlayFar(220);
      Actor_SetMode(target, 2);
    }

    target->update = (void (*)(void))BattleEffect_UpdateOrbitingParticleMain;
    ev = Func_080ce458(0x50000005, 6, Func_080ce31c(6));
    if (ev != NULL)
      Field_RunPsynergyEvent(ev, scene->firstObjectId,
                                   scene->secondObjectId);
    wait(20);
  }

  BattleEffect_PrepareBufferInterpolation();
}
