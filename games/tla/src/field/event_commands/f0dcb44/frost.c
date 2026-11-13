/// @file
/// Frost, and the ball that pushes a target away.
// Part of f0dcb44.c.

#include "../f0dcb44_internal.h"

struct PendingEvent;

struct ResourceEntry {
  u8 unk00[0x10];
  u8 vramBlock;
};

struct Effect04Scene {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[4];
  s8 enabled;
  u8 unk21[0x13];
  s8 childOption;
  s8 childMode;
  u8 unk36[0x1a];
  struct FieldParticle slots[24];
};

struct Effect04Vector {
  s32 x;
  s32 y;
  s32 z;
};

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
/* VIEW(f0de21c): routine Camera_WorldToScreen as void Camera_WorldToScreen(struct Effect04Vector *position); f0dd820 has void Camera_WorldToScreen(struct EffectPosition *value). */
extern void Camera_WorldToScreen__f0de21c(struct Effect04Vector *position) __asm__("Camera_WorldToScreen");
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
/* VIEW(f0de21c): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); hue has extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *). */
extern void Vector_AddPolarOffset__f0de21c(s32 radius, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");
/* VIEW(f0de21c): routine Actor_Spawn as struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z); hue has extern struct ItemBreakFragmentObject *Actor_Spawn(s32, s32, s32, s32). */
extern struct Actor *Actor_Spawn__f0de21c(s32 kind, s32 x, s32 y, s32 z) __asm__("Actor_Spawn");
struct ResourceEntry *Sprite_ShareVramCacheEntry(void *sprite,
                                                 struct ResourceEntry *last);
/* VIEW(f0de21c): routine Func_08020210 as s32 Func_08020210(struct Actor *object, struct Effect04Vector *position); f0dd054 has s32 Func_08020210(struct Actor *object, struct Vec3 *position). */
extern s32 Func_08020210__f0de21c(struct Actor *object, struct Effect04Vector *position) __asm__("Func_08020210");
s32 ScriptObject_CheckOverlapFar(struct Actor *object,
                                 struct Effect04Vector *position);
s32 Func_080ce31c(s32 kind);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target);
s32 Resource_ResetEntry(u32 slot);
/* VIEW(f0de21c): routine Func_080de060 as void Func_080de060(struct FieldParticle *slot); the definition has void Func_080de060(struct RadialBurstEffect *effect). */
extern void Func_080de060__f0de21c(struct FieldParticle *slot) __asm__("Func_080de060");

extern const u8 gData_080f0ef8[];
extern const u8 gData_080f0e58[];

// Twelve sparks burst over the user, two frames apart; a ball grows in
// front of it and flies off with three trailing copies, and the target is
// pushed 16 pixels away unless something blocks it.
void Func_080de21c(void) {
  struct Actor *copies[4];
  struct Effect04Vector pos;
  struct Effect04Scene *scene;
  struct Actor *target;
  struct Actor *source;
  struct Actor *object;
  struct Actor *copy;
  struct FieldParticle *slot;
  struct ResourceEntry *resource;
  struct PendingEvent *event;
  s32 scale;
  s32 i;
  u8 marker;

  scene = Ram_HeapSlots->effectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(130);
  slot = scene->slots;

  for (i = 0; i <= 11; i++) {
    source = scene->mainObject;
    pos.x = source->x;
    pos.y = source->y + 0x100000;
    pos.z = source->z;
    Camera_WorldToScreen__f0de21c(&pos);
    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080de060__f0de21c);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, 9);
    slot->scaleY = 0xb333;
    slot->scaleX = 0xb333;
    wait(2);
    slot++;
  }

  source = scene->mainObject;
  pos.x = source->x;
  pos.y = source->y + 0x100000;
  pos.z = source->z;
  Vector_AddPolarOffset__f0de21c(0x80000, scene->angle, &pos);
  object = Actor_Spawn__f0de21c(278, pos.x, pos.y, pos.z);
  if (object == NULL) {
    BattleEffect_PrepareBufferInterpolation();
    return;
  }
  object->scaleY = 0x4000;
  object->scaleX = 0x4000;
  object->facing = scene->angle;
  object->speedLimit = 0x40000;
  object->acceleration = 0x40000;
  object->motionFlags = 0;
  Actor_SetMode(object, 5);
  Animation_ApplyChildValuesFar(object, 3);
  scale = object->scaleX;
  if (scale < 0x10000) {
    do {
      scale += 0x500;
      object->scaleY = scale;
      object->scaleX = scale;
      wait(1);
      scale = object->scaleX;
    } while (scale <= 0xffff);
  }
  wait(3);

  resource = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] = Actor_Spawn__f0de21c(278, object->x, object->y, object->z);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = scene->angle;
      copy->speedLimit = 0x40000;
      copy->acceleration = 0x40000;
      copy->motionFlags = 0;
      Actor_SetMode(copy, 5);
      Animation_ApplyChildValuesFar(copy, 2);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
    }
  }
  marker = resource->vramBlock;

  if (scene->enabled != 0) {
    source = scene->mainObject;
    pos.x = source->x;
    pos.y = source->y + 0x100000;
    pos.z = source->z;
    Vector_AddPolarOffset__f0de21c(0x380000, scene->angle, &pos);
  } else {
    pos.x = scene->x;
    pos.y = scene->y + 0x100000;
    pos.z = scene->z;
  }
  Actor_SetTargetPosition(object, pos.x, pos.y, pos.z);
  Actor_StartScriptFar(object, gData_080f0ef8);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      wait(3);
      Actor_SetTargetPosition(copy, pos.x, pos.y, pos.z);
      Actor_StartScriptFar(copy, gData_080f0e58);
    }
  }

  for (i = 0; i <= 59 && object->script != NULL; i++)
    wait(1);

  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = 0x80000;
    pos.x = target->x;
    pos.y = target->y;
    pos.z = target->z;
    Vector_AddPolarOffset__f0de21c(0x100000, scene->angle, &pos);
    if (Func_08020210__f0de21c(target, &pos) == 0 &&
        ScriptObject_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = 0x10000;
      target->speedLimit = 0x10000;
      Actor_SetTargetPosition(target, pos.x, pos.y, pos.z);
    }
  }

  event = Func_080ce458(0x50000005, 4, Func_080ce31c(4));
  if (event != NULL)
    Field_RunPsynergyEvent(event, scene->firstObjectId, scene->secondObjectId);
  wait(10);
  BattleEffect_PrepareBufferInterpolation();
  wait(20);
  if (marker != 96)
    Resource_ResetEntry(marker);
}


struct OrbitingParticle;

s32 BattleEffect_UpdateOrbitingParticleMain(struct OrbitingParticle *particle) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    Animation_ApplyChildValuesFar((struct Actor *)particle, 2);
  } else if (phase == 2) {
    Animation_ApplyChildValuesFar((struct Actor *)particle, 0);
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
      Actor_StartScriptFar((struct Actor *)object,
                           (u32)gBattleEffect_SharedParticleScript);
  }
}

struct OrbitingParticleVector {
  s32 x;
  s32 y;
  s32 z;
};

void BattleEffect_UpdateOrbitingParticleLeft(struct Actor *particle) {
  struct OrbitingParticleVector pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->timer - 1;
    particle->timer = count;
    life = count;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset__f0de21c(life << 17, (s16)particle->unk66 + (life << 11),
                            &pos);
      SET_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update =
          (void (*)(void))BattleEffect_UpdateOrbitingParticleFade;
    }
  }
}

void BattleEffect_UpdateOrbitingParticleRight(struct Actor *particle) {
  struct OrbitingParticleVector pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->timer - 1;
    particle->timer = count;
    life = count;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset__f0de21c(life << 17, (s16)particle->unk66 - (life << 11),
                            &pos);
      SET_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update =
          (void (*)(void))BattleEffect_UpdateOrbitingParticleFade;
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
    particle = Actor_Spawn__f0de21c(681, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 dist;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update =
            (void (*)(void))BattleEffect_UpdateOrbitingParticleLeft;
      else
        particle->update =
            (void (*)(void))BattleEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->unk66 = Random16();
      Animation_ApplyChildValuesFar((struct Actor *)particle, 9);

      SET_POSITION(p, scene->x, scene->y, scene->z);
      dist = (Random16() << 2) + (2 << 16);
      Vector_AddPolarOffset__f0de21c(dist, Random16(), p);
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
      Animation_ApplyChildValuesFar((struct Actor *)target, 7);
      wait(1);
      Animation_ApplyChildValuesFar((struct Actor *)target, 0);
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
      Field_RunPsynergyEvent(ev, scene->firstObjectId, scene->secondObjectId);
    wait(20);
  }

  BattleEffect_PrepareBufferInterpolation();
}
