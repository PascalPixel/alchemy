#include "types.h"
#include "field_event.h"

struct TimedObject {
  u8 unk00[0x64];
  s16 age;
  u8 unk66[6];
  void *update;
};

void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object) {
  if (object->age == 80) {
    object->update = 0;
    Sound_PlayFar(136);
  }
  object->age++;
}

#include "system.h"
#include "field_particle.h"
#include "field_psynergy_work.h"
#include "ram_buffer.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void Camera_WorldToScreen(struct EffectVector *pos);
void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot);

void Func_080e0020(struct FieldParticle *effect) {
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = Ram_HeapSlots->effectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->limitTurnStep = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot(effect);
}

void Func_080e0134(void);

/// Pauses the scene's child (byte 91) and runs the fallback transition.
void BattleEffect_MarkChildAndRunFallbackTransition(void) {
  (*(u8 **)((u8 *)Ram_HeapSlots->effectWork + 20))[91] = 1;
  Func_080e0134();
}

#include "actor.h"
#include "fixed_math.h"

// ⚓️'s effect work holds its slots 8 bytes earlier than ☀️'s.
struct FallbackScene {
  u8 unk00[0x50];
  struct FieldParticle slots[16];
};

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z);
#include "actor_view.h"

void Func_080e0134(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *user = scene->mainObject;
  struct Actor *obj;
  struct FieldParticle *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y;
  obj = Actor_Spawn(309, 0, 0, 0);
  step = 0;
  Actor_SetMode(obj, 0);
  if (obj == 0)
    return;

  BattleEffect_InitializeSharedScene();
  from.x = user->x;
  from.y = user->y + (16 << 16);
  from.z = user->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  for (;;) {
    s32 steps = 11;
    s32 scale;

    obj->x = from.x + (to.x - from.x) * step / 10;
    obj->y = from.y + (to.y - from.y) * step / 10;
    obj->z = from.z + (to.z - from.z) * step / 10;
    scale = BattleEffect_Interpolate(Q16_ONE, 0x4000, step);
    obj->scaleX = scale;
    obj->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  wait(5);
  Actor_SetMode(obj, 1);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(109);

  slot = &((struct FallbackScene *)scene)->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = obj->x;
    pos.y = obj->y + (8 << 16);
    pos.z = obj->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    FieldParticle_Initialize(slot, 673, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080e0020);
    Animation_ApplyChildValuesToRecordFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  wait(8);
  Actor_Destroy(obj);
  wait(4);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "object_lookup.h"
#include "main_routine_work.h"
#include "sprite.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(void *, s32))Actor_SetAnimationSpeedFar)

void BattleEffect_SetRandomTableValueOnObject(struct ActorView *object) {
  s8 *table = gBattleEffect_RandomChildValues;
  s32 index = Random16();
  Animation_ApplyChildValuesFar(object, table[(u32)(index * 8) >> 16]);
}

void BattleEffect_PauseObject(s32 id) {
  struct ActorView *object;
  struct AnimationEntry *entry;

  object = ObjectTable_Get(id);
  if (object != NULL) {
    gMainRoutineWork.savedCallback = (u32)object->update;
    gMainRoutineWork.savedByte = 0;
    if (object->kind == 1) {
      entry = object->target.animation->entries[0];
      if (entry != NULL) {
        gMainRoutineWork.savedByte = entry->param;
      }
    }
    object->update = BattleEffect_SetRandomTableValueOnObject;
    object->value5b = 1;
    Actor_SetAnimationSpeedFar(object, 0);
  }
}

void BattleEffect_ResumeObject(s32 id) {
  struct ActorView *object = ObjectTable_Get(id);

  if (object != NULL) {
    if (object->update == BattleEffect_SetRandomTableValueOnObject) {
      object->update = (void *)gMainRoutineWork.savedCallback;
      gMainRoutineWork.savedCallback = 0;
      Animation_ApplyChildValuesFar(object, (s8)gMainRoutineWork.savedByte);
    }
    object->value5b = 0;
    Actor_SetAnimationSpeedFar(object, 16);
  }
}

// Battle effects: filter the effect's target object.

s32 BattleEffect_GetFlags();

/// Returns no target (-1) when the current effect definition's flags are
/// 0xff, else id.
/// id: target object ID.
s32 BattleEffect_FilterObjectIdByFlags(s32 id) {
  if (BattleEffect_GetFlags() == 0xff)
    return -1;
  return id;
}

void Func_080e03cc(void);

/// Runs field effect 13 (Func_080e03cc) for the request dispatcher.
void BattleEffect_CallEffect13(void) { Func_080e03cc(); }

struct FxScene13 {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  u8 unk18[8];
  s8 enabled;
};

struct FxPosition13 {
  s32 x;
  s32 y;
  s32 z;
};

static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
  return from + (to - from) * step / 10;
}

void Func_080e03cc(void) {
  struct FxScene13 *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject = scene->mainObject;
  struct Actor *secondary = scene->child;
  struct Actor *object;
  struct FxPosition13 spawn;
  struct FxPosition13 origin;
  struct FxPosition13 target;
  s32 step;
  s32 index;
  s32 steps = 11;

  origin.x = mainObject->x;
  origin.y = mainObject->y + 0x100000;
  origin.z = mainObject->z;
  if (scene->enabled) {
    target.x = mainObject->x;
    target.y = mainObject->y + 0x200000;
    target.z = mainObject->z;
    Vector_AddPolarOffset(0x200000, scene->angle, &target.x);
  } else {
    target.x = scene->x;
    target.y = scene->y + 0x200000;
    target.z = scene->z;
  }
  spawn.x = scene->x;
  spawn.y = scene->y + 0x200000;
  spawn.z = scene->z;
  object = Actor_Spawn(278, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(138);
  object->facing = mainObject->facing;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  Actor_SetMode(object, 5);
  Animation_ApplyChildValuesFar(object, 1);
  step = 0;
  do {
    s32 scale;

    object->x = origin.x + (target.x - origin.x) * step / 10;
    object->y = origin.y + (target.y - origin.y) * step / 10;
    object->z = origin.z + (target.z - origin.z) * step / 10;
    scale = Interpolate(0x4000, Q16_ONE, step);
    object->scaleX = scale;
    object->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  wait(10);
  Actor_SetMode(object, 6);
  wait(15);
  for (index = 0; index < 10; index++) {
    object->y -= 0x20000;
    wait(1);
  }
  Actor_SetMode(object, 5);
  Sound_PlayFar(132);
  if (secondary != 0)
    Actor_SetPositionAndResetMotionFar(secondary, -0x90000, secondary->y,
                                       -0x90000);
  wait(20);
  for (index = 0; index < 13; index++) {
    object->y += 0x18000;
    wait(1);
  }
  wait(10);
  Sound_PlayFar(114);
  step = 0;
  do {
    s32 scale;

    object->x = target.x + (origin.x - target.x) * step / 10;
    object->y = target.y + (origin.y - target.y) * step / 10;
    object->z = target.z + (origin.z - target.z) * step / 10;
    scale = Interpolate(Q16_ONE, 0x4000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  Actor_Destroy(object);
  BattleEffect_PrepareBufferInterpolation();
}

#include "effect_sprite.h"
#include "vector3.h"

struct ScatterParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct ScatterWork {
  u8 tiles[0x1000];
  struct EffectSprite sprites[64];
  struct ScatterParticle particles[64];
  struct Actor *object;
  s16 phase;
  s16 timer;
  s16 spawnCount;
  s16 next;
  u16 tileA;
  u16 tileB;
  s16 flash;
  s16 hit;
};

struct ScatterEvent {
  u8 unk000[0xd31];
  s8 target;
  s8 targetY;
  s8 targetZ;
};

void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct ScatterParticle *p);
void Func_080d92a4(s32 target, struct Vector3 *position);

void Func_080e0618(void) {
  struct ScatterWork *work =
      (struct ScatterWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScatterEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *object = work->object;
  struct Vector3 position;
  struct EffectSprite *sprite;
  struct ScatterParticle *particle;
  s32 i;

  work->flash = 0;
  for (i = 0; i < work->spawnCount; i++) {
    s32 distance;
    s32 speed;

    particle = &work->particles[work->next];
    particle->x = object->x;
    particle->y = object->y;
    particle->z = object->z;
    distance = Random16() * 12 + 0x40000;
    Vector_AddPolarOffset(distance, Random16(), particle);
    particle->velocityX = 0;
    particle->velocityY = 0x20000 - Random16() * 2;
    particle->velocityZ = 0;
    speed = Random16() + 0x18000;
    Vector_AddPolarOffset(speed, Random16(), &particle->velocityX);
    particle->timer = 0;
    work->next++;
    work->next &= 63;
  }

  for (i = 0, particle = work->particles; i <= 63; i++, particle++) {
    s32 timer = particle->timer;

    if (timer >= 0) {
      sprite = &work->sprites[i];
      if (i & 1)
        sprite->oam.index = work->tileA + (timer & 3) * 2;
      else
        sprite->oam.index = work->tileB + (timer & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_IntegrateVector3(particle, 63, -0x1400);
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  switch (work->phase) {
  case 0: {
    s32 a = Random16();
    s32 b = Random16();
    s32 c, d;

    position.x = scene->x + a * 3 - b * 3;
    c = Random16();
    d = Random16();
    position.y = scene->y + c * 3 - d * 3;
    position.z = scene->z;
    Actor_SetPositionAndResetMotionFar(object, position.x, position.y,
                                       position.z);
    if (work->timer == 20) {
      work->phase++;
      work->timer = -1;
    }
    break;
  }
  case 1:
    Actor_SetPositionAndResetMotionFar(object, scene->x, scene->y, scene->z);
    if (work->timer == 6) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 0)
      work->flash = 1;
    Func_080d92a4(event->target, &position);
    object->x = position.x;
    object->y = position.y - 0x40000;
    object->z = position.z;
    if (work->hit != 0) {
      work->phase++;
      work->timer = -1;
    }
    if (work->timer == 90) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 3:
    Actor_SetMode(object, 3);
    work->spawnCount = 3;
    object->y += work->timer << 17;
    if (work->timer == 10) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 4:
    object->scaleX = 0;
    work->spawnCount = 0;
    if (work->timer == 30)
      work->phase = 999;
    break;
  }
  work->timer++;
}

struct EffectSceneState {
  u8 unk000[0xd31];
  u8 valueD31;
  u8 valueD32;
  u8 valueD33;
};

struct EffectSceneWork {
  u8 unk00[0x20];
  u8 failed;
};

s32 Func_080d9a74(s32 value, s32 *first, s32 *second);
void Func_080e09c0(void);

/// Effect 17 on a target: asks Func_080d9a74 for two results; on failure
/// marks the effect failed, else stores the target and both in the event work.
/// Then runs Func_080e09c0.
void Func_080e0978(s32 value) {
  struct EffectSceneState *state = Ram_HeapSlots->eventWork;
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;
  s32 second;
  s32 first;

  if (Func_080d9a74(value, &first, &second)) {
    work->failed = 1;
  } else {
    state->valueD32 = value;
    // An empty once-only loop keeps the first result's load after the value's
    // store; without it, 4 of 34 instructions differ in tla-en.
    do {
    } while (0);
    state->valueD33 = first;
    state->valueD31 = second;
  }
  Func_080e09c0();
}

#include "graphics/resource.h"
#include "system/task.h"
#include "system/heap_release.h"

void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
void Func_080d92d4(s32 target, s32 y, s32 z);

extern char ResourceId_SparkleFade;
extern char ResourceId_StarSparkleFade;

void Func_080e09c0(void) {
  struct ScatterWork *work = HeapSlot_Alloc(92, 0x2114);
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScatterEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *object;
  struct EffectSprite *sprite;
  struct ScatterParticle *particle;
  s32 slotA;
  s32 slotB;
  s32 tileA;
  s32 i;

  if (scene->child != 0) {
    scene->y += 0x40000;
    scene->z += 0x40000;
  }
  BattleEffect_InitializeSharedScene();
  object = Actor_Spawn(363, 0, 0, 0);
  if (object != 0) {
    object->facing = caster->facing;
    object->motionFlags = 0;
    if (caster->priorityFlags & 0x20)
      object->priorityFlags |= 0x20;
    Actor_SetMode(object, 0);
    Sound_PlayFar(138);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                          work);
    slotA = Resource_FindFreeEntry();
    tileA = VramCache_Load(slotA, 0x100, work);
    work->tileA = tileA;
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    slotB = Resource_FindFreeEntry();
    work->tileB = VramCache_Load(slotB, 0x100, work);

    particle = work->particles;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--) {
      Func_080eaf98(sprite, 4, 4, 0, tileA);
      sprite->depth = 250;
      sprite->oam.priority = 0;
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      particle->x = object->x;
      particle->y = object->y;
      particle->z = object->z;
      Vector_AddPolarOffset(0xc0000, Random16(), particle);
      particle->timer = -1;
      sprite++;
      particle++;
    }

    Func_080dbed0(object, scene->x, scene->y + 0x200000, scene->z);
    Actor_SetMode(object, 1);
    wait(10);
    for (i = 15; i >= 0; i--) {
      object->y += -0x20000;
      wait(1);
    }
    Sound_PlayFar(132);
    if (scene->enabled != 0) {
      for (i = 15; i >= 0; i--)
        BattleEffect_SpawnScatterParticle(object->x, object->y, object->z);
      wait(1);
    } else {
      wait(6);
      work->object = object;
      work->phase = 0;
      work->timer = 0;
      work->next = 0;
      work->spawnCount = 1;
      work->hit = 0;
      Task_Add(Func_080e0618, 0x480);
      while (work->phase != 999) {
        if (work->flash == 1) {
          Sound_PlayFar(0x200);
          Func_080d92d4(event->target, event->targetY, event->targetZ);
          work->hit = 1;
          Sound_PlayFar(395);
        }
        wait(1);
      }
      Task_Remove(Func_080e0618);
    }
    Resource_ResetEntry(slotA);
    Resource_ResetEntry(slotB);
    Actor_Destroy(object);
  }
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}

#include "scene.h"
#include "motobj.h"

void BattleEffect_AdvanceAngleWithRamp(struct Actor *object) {
  u32 step;

  step = object->timer * 0x50;
  object->facing += step + 0x1000;
  if (step < 0x1000)
    object->timer++;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
// As ☀️'s FieldEffect_UpdateDescendingObject.
void BattleEffect_UpdateDescendingObject(struct Actor *obj) {
  u32 scale;
  u32 scaleX;
  s32 step = -0x280;

  obj->scaleY += step;
  scaleX = obj->scaleX;
  scale = scaleX + step;
  obj->facing += 0x2000;
  obj->y += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000) {
    obj->animationKind = 0;
  }
}

#include "frame.h"

struct LaunchVector {
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchScene {
  u8 unk00[0x10];
  struct LaunchObject *mainObject;
};

void Func_080e0cec(struct FieldParticle *slot) {
  struct LaunchVector pos;
  struct LaunchObject *src;
  s32 state;
  u32 r;
  struct LaunchVector *arg;
  s32 lift;

  src = ((struct LaunchScene *)Ram_HeapSlots->effectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x;
    arg = &pos;
    pos.y = src->y;
    pos.z = src->z;
    Camera_WorldToScreen(arg);

    slot->x = slot->originX = pos.x;
    slot->z = slot->originZ = pos.z + (8 << 16);

    r = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->limitTurnStep = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlayFar(246);
  } else if (state == 1) {
    if (FieldParticle_HasPendingTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(slot) == 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
}
