#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mercury.h"
#include "iwram_call.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_effect.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_runtime.h"
#include "heap_state.h"
#include "keystate.h"

struct FollowDetail {
  u8 unk00[5];
  u8 shade;
};

struct FollowSprite {
  u8 unk00[37];
  u8 visible;
  u8 unk26[2];
  struct FollowDetail *detail;
};

struct SpawnPoint {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct EventSpawns {
  u8 unk00[20];
  struct SpawnPoint *points[1];
};

struct Sprite39b {
  u8 pad[9];
  u8 lo : 2;
  u8 layer : 2;
  u8 hi : 4;
};

struct Work {
  u8 pad[20];
  u8 *actors[1];
};

struct Obj {
  u8 pad00[12];
  s32 y;
  u8 pad10[69];
  u8 f85;
  u8 pad56[18];
  u8 *owner;
};

struct PillarState {
  s32 step;
  u8 pad04[8];
  s32 x;
  s32 z;
  u8 *pillar;
};

struct EffectAnchor {
  u8 pad00[8];
  s32 f8;
  u8 pad0c[4];
  s32 f16;
};

struct AnchoredEffect {
  u8 pad00[8];
  s32 f8;
  s32 f12;
  s32 f16;
  u8 pad14[4];
  s32 f24;
  s32 f28;
  u8 pad20[68];
  s16 f100;
  u8 pad66[2];
  struct EffectAnchor *f104;
};

static s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work) {
  struct EffectAnchor *src = work->f104;
  s16 step = ++work->f100;
  s32 seed;

  if (step > 31) {
    return 0;
  }

  seed = Engine_MathSin(step << 10);
  work->f24 = seed;
  work->f28 = seed;
  work->f8 = src->f8;
  work->f12 += 0x10000;
  work->f16 = src->f16;
  return 1;
}

static s32 OverlayObject_RampWords24And28Over16Frames(struct FieldActor *o) {
  u16 *c = &o->timer;
  s16 n;

  *c = *c + 1;
  n = *c;
  if (n > 16) {
    return 0;
  }
  o->scaleX = (n * 3) << 10;
  o->scaleY = (n * 3) << 10;
  return 1;
}

static s32 OverlayObject_AdvanceScaleCounter(struct FieldActor *o) {
  u16 *c = &o->timer;
  s16 n;

  *c = *c + 1;
  n = *c;
  if (n > 16) {
    return 0;
  }
  o->scaleX = (n << 11) + 0x10000;
  o->scaleY = (n << 11) + 0x10000;
  return 1;
}

static s32 Mercury_FollowLeader(struct FieldActor *actor) {
  struct FieldActor *leader;
  struct FollowSprite *sprite;
  s32 tz;
  s32 tx;
  s32 dx;
  s32 dz;
  s32 dist;
  s32 step;

  actor->speed = 0x20000;
  leader = actor->linkedObject;
  actor->acceleration = 0x10000;
  tx = leader->x.fixed;
  tz = leader->z.fixed;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  dx = (tx - actor->x.fixed) / 0x10000;
  dz = (tz - actor->z.fixed) / 0x10000;
  dist = (u16)Iwram_Sqrt(dx * dx + dz * dz);
  dx = tx - actor->x.fixed;
  dz = tz - actor->z.fixed;
  dist <<= 16;
  if (dist < 0x400000)
    dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
  step = dist / 8;
  if (step > actor->speed)
    step = actor->speed;
  if (dist < 0x4000) {
    actor->x.fixed = tx;
    actor->z.fixed = tz;
  } else {
    if (dist > step) {
      dx = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dx), step);
      dz = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dz), step);
    }
    actor->x.fixed += dx;
    actor->z.fixed += dz;
  }
  sprite = (struct FollowSprite *)actor->sprite;
  sprite->detail->shade = ((gFrameCount >> 1) & 1) * 7;
  sprite->visible = 1;
  return 1;
}

static void MercuryLighthouse_SpawnSparkPair(void) {
  struct SpawnPoint *point;
  struct FieldActor *spark;
  struct FieldSprite *sprite;

  point =
      (*(struct EventSpawns **)&gEventWork)->points[gGameState.selectedActor];
  spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
  if (spark != NULL) {
    spark->terrainHeight = point->value;
    sprite = spark->sprite;
    Engine_ObjectSetScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->linkedObject = (struct FieldActor *)point;
    if (sprite != NULL) {
      AnimationObjects_SelectAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->priority = 1;
    }
  }
  spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
  if (spark != NULL) {
    spark->terrainHeight = point->value;
    sprite = spark->sprite;
    Engine_ObjectSetScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->linkedObject = (struct FieldActor *)point;
    spark->priorityFlags = 2;
    if (sprite != NULL) {
      AnimationObjects_SelectAnimation(sprite, 1);
      sprite->flags = 0;
    }
  }
  Engine_AudioPlayCue(130);
}

void MercuryEntrance_RaisePillar(void) {
  struct PillarState *state;
  u8 *leader;
  u8 *pillar;
  u8 *obj;
  struct Sprite39b *spr;
  s32 step;

  state = *(struct PillarState **)WORK_SLOT(35);
  leader = ((struct Work *)WORK_SLOT(HEAP_SLOT_EVENT))
               ->actors[gGameState.selectedActor];
  if ((u32)state->step > 2)
    return;
  Engine_EventBegin();
  pillar = state->pillar;
  if (pillar == 0) {
    obj = Engine_ObjectCreate(26, *(s32 *)(leader + 8),
                              *(s32 *)(leader + 12) + 0x180000,
                              *(s32 *)(leader + 16));
    if (obj != 0) {
      spr = *(struct Sprite39b **)(obj + 80);
      *(s32 *)(obj + 20) = *(s32 *)(leader + 20);
      Engine_ObjectSetScript(obj, gMercury_PillarScript);
      ((struct Obj *)obj)->owner = leader;
      ((struct Obj *)obj)->f85 = 4;
      ((struct Obj *)obj)->y += -0x8000;
      if (spr != 0) {
        ((u8 *)spr)[38] = 0;
        spr->layer = 1;
      }
      obj[84] = 0;
      state->pillar = obj;
      pillar = obj;
    } else {
      pillar = state->pillar;
    }
  }
  for (step = state->step; step <= 2; step++) {
    MercuryLighthouse_SpawnSparkPair();
    Engine_TaskWait(30);
    pillar[84] = 1;
    Object_SetMode(pillar, 5 - step);
  }
  state->step = 3;
  state->x = (*(s32 *)(pillar + 8) & -0x100000) + 0x80000;
  state->z = (*(s32 *)(pillar + 16) & -0x100000) + 0x80000;
  Engine_GameFlagSet(0x161);
  Engine_EventEnd();
}

struct PushState {
  s32 count;
  s32 level;
  s32 active;
  s32 targetX;
  s32 targetZ;
  struct FieldActor *source;
  struct FieldActor *effect;
};

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

void SceneState_ClearCurrentRecordAndReleaseTarget(void) {
  s32 *record = **(s32 ***)&((struct WorkPointers *)&gEventWork)->scene;
  s32 *target;

  if (record[0] == 0) {
    return;
  }

  record[0] = 0;
  Engine_GameFlagClear(0x161);

  target = (s32 *)record[5];
  if (target != 0) {
    *(short *)((u8 *)target + 0x64) = 0;
    Engine_ObjectSetScript(target, (s32)gMercury_ScaleCounterScript);
    Object_SetMode(target, 7);
    record[5] = 0;
  }
}

struct SceneTimer {
  u8 unk00[8];
  s32 count;
};

void Mercury_RunActorMove(void) {
  struct FieldStepWork *work;
  struct PushState *state;
  struct FieldActor *actor;
  struct FieldActor *source;
  struct FieldActor *obj;
  struct FieldSprite *sprite;
  struct MapCell *from;
  struct MapCell *to;
  u8 saved;
  u8 *flags;
  s32 angle;
  u32 keys;
  s32 n;
  s32 i;
  u8 zero;
  union FieldCoordinate pos[3];

  state = *(struct PushState **)WORK_SLOT(35);
  work = WORK_SLOT(HEAP_SLOT_EVENT);
  actor = (struct FieldActor *)work->actors[gGameState.selectedActor];
  flags = &actor->motionFlags;
  saved = *flags;
  keys = gKeysHeld;
  angle = gMercury_MoveAngles[(keys >> 4) & 15];
  if ((s16)gMercury_MoveAngles[(keys >> 4) & 15] == -1)
    return;
  pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
  pos[1].fixed = ((struct ObjectRuntime *)actor)->terrainHeight;
  pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
  from = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  Vector_AddPolarOffset(0x200000, angle, &pos[0].fixed);
  to = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  if (from->collisionCode != state->level &&
      to->collisionCode == state->level && state->count == 0)
    return;

  Engine_EventBegin();
  if (FieldObject_CheckMovementCollision(actor, (struct FieldPosition *)pos) !=
      0)
    return;
  obj = state->effect;
  if (obj != NULL) {
    ((union FieldObject *)obj)->effect.spin = 0;
    Engine_ObjectSetScript(obj, gMercury_ScaleCounterScript);
    Object_SetMode(obj, 7);
    state->effect = NULL;
  }
  if (to->collisionCode == state->level && state->count != 0) {
    source = state->source;
    obj = Engine_ObjectCreate(26, source->x.fixed, source->y.fixed,
                              source->z.fixed);
    if (obj != NULL) {
      sprite = obj->sprite;
      ((struct ObjectRuntime *)obj)->terrainHeight =
          ((struct ObjectRuntime *)source)->terrainHeight;
      Engine_ObjectSetScript(obj, gMercury_RampScript);
      obj->motionFlags = 0;
      ((union FieldObject *)obj)->effect.spin = 0;
      obj->priorityFlags = 2;
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      Engine_ObjectSetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
      if (sprite != NULL) {
        AnimationObjects_SelectAnimation(sprite, 6);
        zero = 0;
        sprite->flags = zero;
      }
      state->effect = obj;
    }
    n = state->count - 1;
    state->count = n;
    if (n == 0) {
      Engine_ObjectDispatchRelease(state->source);
      state->source = NULL;
      Engine_GameFlagClear(0x161);
    } else if (state->source != NULL) {
      Object_SetMode(state->source, 6 - n);
    }
  }
  Object_SetMode(actor, 6);
  Engine_TaskWait(3);
  Engine_AudioPlayCue(152);
  Object_SetMode(actor, 7);
  actor->speed = 0x30000;
  actor->acceleration = 0x20000;
  actor->velocityY = 0x40000;
  *flags &= 0x7e;
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorMoveToAndWait(0, pos[0].part.pixel, pos[2].part.pixel);
  Object_SetMode(actor, 6);
  Engine_TaskWait(2);
  if (to->collisionCode != state->level)
    Engine_ActorSetSpriteFlags(actor, 1);
  else
    Engine_AudioPlayCue(215);
  Engine_TaskWait(1);
  *flags = saved;
  if (to->collisionCode == state->level && state->effect == NULL) {
    Object_SetMode(actor, 18);
    Engine_AudioPlayCue(241);
    for (i = 0;; i++) {
      if ((i & 15) == 0)
        OverlayObject_SpawnKind24AtObject(actor);
      if (i > 31 && gKeyState != 0)
        break;
      Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(0x120);
    Engine_TaskWait(1);
    actor->x.fixed = state->targetX;
    actor->z.fixed = state->targetZ;
    Engine_ActorSetSpriteFlags(actor, 1);
  }
  state->active = 0;
  Engine_EventEnd();
  work->encounterSteps += Iwram_MulQ16(work->encounterRate, 0x200000);
}

void Mercury_TickSpawnTimer(void) {
  struct SceneTimer *timer;
  struct FieldActor *point;

  timer = *(struct SceneTimer **)WORK_SLOT(35);
  point =
      (struct FieldActor *)((struct FieldStepWork *)WORK_SLOT(HEAP_SLOT_EVENT))
          ->actors[gGameState.selectedActor];
  if (timer->count != 0) {
    timer->count--;
  } else {
    OverlayObject_SpawnKind24AtObject(point);
    timer->count = ((u32)(Engine_RandomNext() * 30) >> 16) + 10;
  }
}

static void OverlayObject_ReleasePublishedAttachment(void) {
  u8 **pub = Runtime_AllocateBlock(35, 4);
  u8 *state;
  u8 *obj;

  if (pub == 0)
    return;
  state = *pub;
  obj = *(u8 **)(state + 20);
  if (obj == 0)
    return;
  Engine_ObjectDispatchRelease(obj);
  *(u8 **)(state + 20) = 0;
}
