#include "script.h"
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
#include "fixed_math.h"

// Raise the spark while its scale follows a half-sine pulse.
static s32
SceneEffect_AdvanceAnchoredRiseFrame(struct ScriptInterpreter *interpreter) {
  struct FieldActor *effect = (struct FieldActor *)interpreter;
  struct FieldActor *anchor = effect->linkedObject;
  s16 step = ++effect->timer;
  s32 scale;

  if (step > 31) {
    return 0;
  }

  scale = Engine_MathSin(step << 10);
  effect->scaleX = scale;
  effect->scaleY = scale;
  effect->x.fixed = anchor->x.fixed;
  effect->y.fixed += PIXELS(1);
  effect->z.fixed = anchor->z.fixed;
  return 1;
}

// Grow the moving light over the first sixteen script frames.
static s32 OverlayObject_RampWords24And28Over16Frames(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 frame;

  frame = ++actor->timer;
  if (frame > 16) {
    return 0;
  }
  actor->scaleX = (frame * 3) << 10;
  actor->scaleY = (frame * 3) << 10;
  return 1;
}

// Expand the released light before its script removes it.
static s32
OverlayObject_AdvanceScaleCounter(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 frame;

  frame = ++actor->timer;
  if (frame > 16) {
    return 0;
  }
  actor->scaleX = (frame << 11) + Q16_ONE;
  actor->scaleY = (frame << 11) + Q16_ONE;
  return 1;
}

// Follow the linked actor and alternate the light's animation parameter.
static s32 MercuryLighthouse_FollowLeader(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct FieldActor *leader;
  s32 targetZ;
  s32 targetX;
  s32 deltaX;
  s32 deltaZ;
  s32 distance;
  s32 moveDistance;

  actor->speed = PIXELS(2);
  leader = actor->linkedObject;
  actor->acceleration = PIXELS(1);
  targetX = leader->x.fixed;
  targetZ = leader->z.fixed;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  // Measure in whole pixels first; use fixed-point precision near the leader.
  deltaX = (targetX - actor->x.fixed) / PIXELS(1);
  deltaZ = (targetZ - actor->z.fixed) / PIXELS(1);
  distance = (u16)Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
  deltaX = targetX - actor->x.fixed;
  deltaZ = targetZ - actor->z.fixed;
  distance <<= 16;
  if (distance < PIXELS(64))
    distance =
        FixedSqrt(Iwram_MulQ16(deltaX, deltaX) + Iwram_MulQ16(deltaZ, deltaZ));
  moveDistance = distance / 8;
  if (moveDistance > actor->speed)
    moveDistance = actor->speed;

  // Snap within a quarter pixel; otherwise ease toward the leader.
  if (distance < PIXELS(1) / 4) {
    actor->x.fixed = targetX;
    actor->z.fixed = targetZ;
  } else {
    if (distance > moveDistance) {
      deltaX = Iwram_MulQ16(Iwram_RatioMulQ14(distance, deltaX), moveDistance);
      deltaZ = Iwram_MulQ16(Iwram_RatioMulQ14(distance, deltaZ), moveDistance);
    }
    actor->x.fixed += deltaX;
    actor->z.fixed += deltaZ;
  }

  // Alternate the light's draw parameter every two frames and redraw it.
  /* FAKEMATCH: retain the scalar sprite-slot access while FieldActor exposes
   * the single/group union. Plain actor->sprite swaps the movs r0, #1 and
   * sprite load at tbs-en 0x531e; the typed scalar slot keeps their order. */
  {
    struct AnimationObject *sprite =
        *(struct AnimationObject **)((u8 *)actor +
                                     offsetof(struct FieldActor, sprite));
    struct AnimationEntry *entry = sprite->entries[0];

    entry->param = ((gFrameCount >> 1) & 1) * 7;
    sprite->dirty = 1;
  }
  return 1;
}

// Place the two spark animations on the selected actor.
static void MercuryLighthouse_SpawnSparkPair(void) {
  struct FieldActor *point;
  struct FieldActor *spark;
  struct AnimationObject *sprite;

  point = gEventWork->actors[gGameState.selectedActor];
  spark =
      Engine_ObjectCreate(26, point->x.fixed, point->y.fixed, point->z.fixed);
  if (spark != NULL) {
    spark->terrainHeight = point->terrainHeight;
    sprite = spark->sprite;
    Engine_ObjectSetScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->linkedObject = point;
    if (sprite != NULL) {
      AnimationObjects_SelectAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  spark =
      Engine_ObjectCreate(26, point->x.fixed, point->y.fixed, point->z.fixed);
  if (spark != NULL) {
    spark->terrainHeight = point->terrainHeight;
    sprite = spark->sprite;
    Engine_ObjectSetScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->linkedObject = point;
    spark->priorityFlags = 2;
    if (sprite != NULL) {
      AnimationObjects_SelectAnimation(sprite, 1);
      sprite->flags = 0;
    }
  }
  Engine_AudioPlayCue(130);
}

// Raise the pillar one stage for each spark pair, then mark it complete.
void MercuryLighthouseEntrance_RaisePillar(void) {
  struct MercuryLighthouseLights *state;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  struct FieldActor *obj;
  s32 step;

  state = *(struct MercuryLighthouseLights **)WORK_SLOT(35);
  leader = ((struct FieldWork *)WORK_SLOT(HEAP_SLOT_EVENT))
               ->actors[gGameState.selectedActor];
  if ((u32)state->levels > 2)
    return;
  Engine_EventBegin();
  pillar = state->pillar;
  if (pillar == 0) {
    obj = Engine_ObjectCreate(26, leader->x.fixed, leader->y.fixed + 0x180000,
                              leader->z.fixed);
    if (obj != 0) {
      struct AnimationObject *sprite = obj->sprite;

      obj->terrainHeight = leader->terrainHeight;
      Engine_ObjectSetScript(obj, gMercuryLighthouse_PillarScript);
      obj->linkedObject = leader;
      obj->motionFlags = 4;
      /* FAKEMATCH: retain the scalar height access. Plain obj->y.fixed
       * subtraction moves the saved sprite reload after the height add/store
       * at tbs-en 0x5498; the scalar coordinate keeps its original order. */
      *(s32 *)((u8 *)obj + offsetof(struct FieldActor, y)) += -PIXELS(1) / 2;
      if (sprite != 0) {
        sprite->flags = 0;
        sprite->image.priority = 1;
      }
      obj->spriteStorageKind = 0;
      state->pillar = obj;
      pillar = obj;
    } else {
      pillar = state->pillar;
    }
  }
  for (step = state->levels; step <= 2; step++) {
    MercuryLighthouse_SpawnSparkPair();
    Engine_TaskWait(30);
    pillar->spriteStorageKind = 1;
    Object_SetMode(pillar, 5 - step);
  }
  state->levels = 3;
  state->targetX = (pillar->x.fixed & -0x100000) + 0x80000;
  state->targetZ = (pillar->z.fixed & -0x100000) + 0x80000;
  Engine_GameFlagSet(0x161);
  Engine_EventEnd();
}

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

// Reset the published light count and start the attached actor's release
// script.
void SceneState_ClearCurrentRecordAndReleaseTarget(void) {
  struct MercuryLighthouseLights *state = *(struct MercuryLighthouseLights **)WORK_SLOT(35);
  struct FieldActor *target;

  if (state->levels == 0) {
    return;
  }

  state->levels = 0;
  Engine_GameFlagClear(0x161);

  target = state->pillar;
  if (target != 0) {
    target->timer = 0;
    Engine_ObjectSetScript(target, gMercuryLighthouse_ScaleCounterScript);
    Object_SetMode(target, 7);
    state->pillar = NULL;
  }
}

// Move between adjacent cells, spending a light when crossing into its region.
void MercuryLighthouse_RunActorMove(void) {
  struct FieldWork *work;
  struct MercuryLighthouseLights *state;
  struct FieldActor *actor;
  struct FieldActor *source;
  struct FieldActor *obj;
  struct AnimationObject *sprite;
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

  state = *(struct MercuryLighthouseLights **)WORK_SLOT(35);
  work = WORK_SLOT(HEAP_SLOT_EVENT);
  actor = work->actors[gGameState.selectedActor];
  flags = &actor->motionFlags;
  saved = *flags;
  keys = gKeysHeld;
  angle = gMercuryLighthouse_MoveAngles[(keys >> 4) & 15];
  if ((s16)gMercuryLighthouse_MoveAngles[(keys >> 4) & 15] == -1)
    return;
  pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
  pos[1].fixed = actor->terrainHeight;
  pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
  from = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  Vector_AddPolarOffset(0x200000, angle, &pos[0].fixed);
  to = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  if (from->collisionCode != state->terrainCode &&
      to->collisionCode == state->terrainCode && state->levels == 0)
    return;

  Engine_EventBegin();
  if (FieldObject_CheckMovementCollision(actor, (struct FieldPosition *)pos) !=
      0)
    return;
  obj = state->stepEffect;
  if (obj != NULL) {
    obj->timer = 0;
    Engine_ObjectSetScript(obj, gMercuryLighthouse_ScaleCounterScript);
    Object_SetMode(obj, 7);
    state->stepEffect = NULL;
  }
  if (to->collisionCode == state->terrainCode && state->levels != 0) {
    source = state->pillar;
    obj = Engine_ObjectCreate(26, source->x.fixed, source->y.fixed,
                              source->z.fixed);
    if (obj != NULL) {
      sprite = obj->sprite;
      obj->terrainHeight = source->terrainHeight;
      Engine_ObjectSetScript(obj, gMercuryLighthouse_RampScript);
      obj->motionFlags = 0;
      obj->timer = 0;
      obj->priorityFlags = 2;
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      Engine_ObjectSetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
      if (sprite != NULL) {
        AnimationObjects_SelectAnimation(sprite, 6);
        zero = 0;
        sprite->flags = zero;
      }
      state->stepEffect = obj;
    }
    n = state->levels - 1;
    state->levels = n;
    if (n == 0) {
      Engine_ObjectDispatchRelease(state->pillar);
      state->pillar = NULL;
      Engine_GameFlagClear(0x161);
    } else if (state->pillar != NULL) {
      Object_SetMode(state->pillar, 6 - n);
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
  if (to->collisionCode != state->terrainCode)
    Engine_ActorSetSpriteFlags(actor, 1);
  else
    Engine_AudioPlayCue(215);
  Engine_TaskWait(1);
  *flags = saved;
  if (to->collisionCode == state->terrainCode && state->stepEffect == NULL) {
    Object_SetMode(actor, 18);
    Engine_AudioPlayCue(241);
    for (i = 0;; i++) {
      if ((i & 15) == 0)
        OverlayObject_SpawnKind24AtObject(actor);
      if (i > 31 && gKeysPressed != 0)
        break;
      Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(0x120);
    Engine_TaskWait(1);
    actor->x.fixed = state->targetX;
    actor->z.fixed = state->targetZ;
    Engine_ActorSetSpriteFlags(actor, 1);
  }
  state->sparkTimer = 0;
  Engine_EventEnd();
  work->walkingDistance += Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
}

// Emit an effect at the selected actor after a randomized delay.
void MercuryLighthouse_TickSpawnTimer(void) {
  struct MercuryLighthouseLights *state;
  struct FieldActor *point;

  state = *(struct MercuryLighthouseLights **)WORK_SLOT(35);
  point = ((struct FieldWork *)WORK_SLOT(HEAP_SLOT_EVENT))
              ->actors[gGameState.selectedActor];
  if (state->sparkTimer != 0) {
    state->sparkTimer--;
  } else {
    OverlayObject_SpawnKind24AtObject(point);
    state->sparkTimer = ((u32)(Engine_RandomNext() * 30) >> 16) + 10;
  }
}

// Remove the actor stored in the scene's published attachment slot.
static void OverlayObject_ReleasePublishedAttachment(void) {
  struct MercuryLighthouseLights **published = Runtime_AllocateBlock(35, 4);
  struct MercuryLighthouseLights *state;
  struct FieldActor *pillar;

  if (published == NULL)
    return;
  state = *published;
  pillar = state->pillar;
  if (pillar == NULL)
    return;
  Engine_ObjectDispatchRelease(pillar);
  state->pillar = NULL;
}
