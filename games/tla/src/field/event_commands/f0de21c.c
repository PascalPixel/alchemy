#include "types.h"
#include "task.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_particle.h"

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
void Camera_WorldToScreen(struct Effect04Vector *position);
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
#include "actor_view.h"
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct ResourceEntry *Sprite_ShareVramCacheEntry(void *sprite,
                                                 struct ResourceEntry *last);
#include "objdisp.h"
s32 Func_08020210(struct Actor *object, struct Effect04Vector *position);
s32 ScriptObject_CheckOverlapFar(struct Actor *object,
                                 struct Effect04Vector *position);
s32 Func_080ce31c(s32 kind);
#include "battle.h"
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target);
s32 Resource_ResetEntry(u32 slot);
void Func_080de060(struct FieldParticle *slot);

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
    Camera_WorldToScreen(&pos);
    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080de060);
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
  Vector_AddPolarOffset(0x80000, scene->angle, &pos);
  object = Actor_Spawn(278, pos.x, pos.y, pos.z);
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
    copy = copies[i] = Actor_Spawn(278, object->x, object->y, object->z);
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
    Vector_AddPolarOffset(0x380000, scene->angle, &pos);
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
    Vector_AddPolarOffset(0x100000, scene->angle, &pos);
    if (Func_08020210(target, &pos) == 0 &&
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

#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "scene.h"
#include "object_effect_scripts.h"
#include "fixed_math.h"
#include "frame.h"
#include "field_psynergy_work.h"
#include "macros.h"

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
      Vector_AddPolarOffset(life << 17, (s16)particle->unk66 + (life << 11),
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
      Vector_AddPolarOffset(life << 17, (s16)particle->unk66 - (life << 11),
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
    particle = Actor_Spawn(681, 0, 0, 0);
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

#include "iwram_call.h"

struct SwayObject {
  u8 unk00[8];
  s32 y;
  u8 unk0c[44];
  s32 baseY;
  u8 unk3c[40];
  s16 phase;
};

struct OrbitVector {
  s32 x;
  s32 y;
  s32 z;
};

/// Per-frame update: sways 4 pixels either side of the anchor, once every
/// 128 frames.
void BattleEffect_SwayParticle(struct SwayObject *object) {
  s16 *phase = &object->phase;
  s32 amplitude = 0x40000;

  object->y = object->baseY + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

/// The particle circles 10 pixels above the scene's position, its radius in
/// pixels the frames it has left; at 0 it runs the shared particle script.
void BattleEffect_UpdateShrinkingOrbitObject(struct Actor *object) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct OrbitVector pos;
  s16 life;
  s32 count;
  s32 radius;
  s32 lift;

  if (object != NULL) {
    count = (u16)object->timer - 1;
    object->timer = count;
    life = count;
    if (life != 0) {
      pos.x = scene->x;
      lift = 160;
      pos.y = scene->y + (lift << 12);
      radius = life << 16;

      pos.z = scene->z;
      Vector_AddPolarOffset(radius, (s16)object->unk66 + (life << 11), &pos);
      SET_POSITION(object, pos.x, pos.y, pos.z);
    } else {
      Actor_StartScriptFar((struct Actor *)object,
                           gBattleEffect_SharedParticleScript);
    }
  }
}

struct SparkObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX;
  u8 unk3c[12];
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
  u8 unk60[4];
  s16 phase;
  u8 unk66[6];
  void (*update)(struct SparkObject *);
};

void Func_080de8d0(struct SparkObject *obj) {
  struct BattleFxScene *scene;
  s16 *phase;
  struct SparkObject *spark;
  s32 pos[3];

  scene = Ram_HeapSlots->effectWork;
  phase = &obj->phase;
  if (*phase != -1) {
    s32 amplitude = 6 << 16;

    obj->x = scene->x + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 10));
    obj->y = scene->y + (16 << 16);
    obj->z = scene->z;
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    pos[0] = obj->x;
    pos[1] = obj->y + (2 << 16);
    pos[2] = obj->z;
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark = Actor_Spawn(673, pos[0], pos[1], pos[2]);
    if (spark != 0) {
      spark->update = BattleEffect_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999;
      spark->motionFlags = 2;
      spark->gravity = 458;
      spark->phase = Random16() >> 9;
      spark->anchorX = spark->x;
      Animation_ApplyChildValuesFar(spark, 9);
      spark->unk5e = 72;
      Actor_StartScriptFar((struct Actor *)spark,
                           gBattleEffect_SharedParticleScript);
    }
  }
}

void FieldEffect_RunPly(void);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);

/// Effect 3 cast on a target: runs effect 3, then stops the caster's hook.
void FieldEffect_RunPlyOnTarget(void) {
  FieldEffect_RunPly();
  BattleEffect_StopMainActor();
}

/// Eight particles spiral in on the main object, one a frame; then a centre
/// object flashes for 60 frames, emits sparks and flies off.
void FieldEffect_RunPly(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->mainObject;
  struct Actor *object;
  struct ResourceEntry *last;
  u8 marker;
  s32 i;

  BattleEffect_InitializeSharedScene();

  last = 0;
  for (i = 0; i < 8; i++) {
    object = Actor_Spawn(296, target->x, target->y + 0x200000, target->z);
    if (object != 0) {
      object->scaleY = 0xb333;
      object->scaleX = 0xb333;
      object->update = (void (*)(void))BattleEffect_UpdateShrinkingOrbitObject;
      object->timer = 120;
      object->unk66 = i << 13;
      object->motionFlags = 4;
      last = Sprite_ShareVramCacheEntry(object->sprite, last);
    }
    wait(1);
  }

  marker = last->vramBlock;
  Sound_PlayFar(130);
  wait(110);

  target = Actor_Spawn(296, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    SET_POSITION(target, scene->x, scene->y + 0x100000, scene->z);
    target->motionFlags = 4;
    Animation_ApplyChildValuesFar(target, 7);
  }

  Sound_PlayFar(131);
  wait(12);
  if (target != 0) {
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH(temporary): a redundant copy keeps object as a second
       * register copy of the centre object (r5 beside r6); without it, 123 of
       * 188 instructions differ in tla-en. */
      target = object;
      if (i & 3)
        Animation_ApplyChildValuesFar(object, 9);
      else
        Animation_ApplyChildValuesFar(object, 10);
      wait(2);
    }
  }

  Animation_ApplyChildValuesFar(object, 0);
  Sound_PlayFar(84);
  if (object != 0) {
    target->update = (void (*)(void))Func_080de8d0;
    target->timer = 0;
    if (scene->enabled != 0)
      wait(128);
    else
      wait(192);
  }

  if (target != 0) {
    target->timer = -1;
    target->speedLimit = 0x50000;
    target->acceleration = 0x6666;
    target->actionFlags = 0;
    Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
    Actor_CommitPosition(target);
    Actor_Destroy(target);
  }
  if (marker != 0x60)
    Resource_ResetEntry(marker);
  if (scene->finishCallback != 0)
    scene->finishCallback();
  BattleEffect_PrepareBufferInterpolation();
}

struct OwnerObject {
  u8 unk00[8];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[14];
  u8 layer;
};

struct EventWork {
  u8 unk000[0x166];
  u16 state;
};

s32 Party_GetLeaderActor(void);
s32 Func_080dbca8(s32 x, s32 z, s32 layer);

/// A task of Cloak: when the controlled character does not
/// stand on a tile Cloak can hide on, requests CLOAK_END (0x2092).
void FieldEffect_CheckCloakTile(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct OwnerObject *owner = ObjectTable_Get(Party_GetLeaderActor());

  if (Func_080dbca8(owner->x, owner->z, owner->layer) == 0)
    event->state = 0x2092;
}

#include "main_routine_work.h"
#include "sprite.h"

// ⚓️'s composite sprite (as worka0/f0daecc.c's CompositeAnimation).
struct CloakSprite {
  u8 unk00[0x19];
  u8 dirty; // redraw the parts
  u8 blend; // 2 draws the caster half transparent, 1 shows it plainly
  u8 unk1b[0x0d];
  struct AnimationEntry *entries[4];
};

struct CloakCaster {
  u8 unk00[0x50];
  struct CloakSprite *sprite;
  u8 unk54[0x18];
  void (*hook)(void);
};

#define SE_CLOAK_END 154

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

/// Show the Cloaked caster again: stop checking the ground under it, then
/// flicker it back into view over twenty frames and clear the cloak (The
/// Broken Seal's FieldPsynergy_EndCloak).
void Func_080debd8(void) {
  struct CloakCaster *caster =
      ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->mainObject;
  struct CloakSprite *sprite = caster->sprite;
  struct AnimationEntry *entry = sprite->entries[0];
  u32 i;

  Sound_PlayFar(SE_CLOAK_END);
  Task_Remove(FieldEffect_CheckCloakTile);
  Actor_SetMode((struct Actor *)caster, 0);
  caster->hook = 0;

  // Blink between the flicker palette half transparent and solid...
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 2;
    sprite->dirty = 1;
    wait(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    wait(2);
  }

  // ...then between the flicker palette and the caster's own.
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 0;
    sprite->dirty = 1;
    wait(2);
    entry->param = 0;
    sprite->dirty = 1;
    wait(2);
  }

  sprite->blend = 1;
  gMainRoutineWork.cloaked = 0;
}

void FieldEffect_ResumeCloakCheck(void) {
  if (gMainRoutineWork.cloaked != 0)
    Task_Add(FieldEffect_CheckCloakTile, 0x480);
}
