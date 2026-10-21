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
#include "songs.h"

// Mercury Lighthouse and its entrance, once the pool is woken: standing at the
// pool's heart (touch trigger 11) gives the leader three lights, shown as a
// column of light over them. Each hop into the pool (the floor's region, its
// collision code) spends one, and a light comes to hold the leader up where
// they land. A hop within the pool with no light left sinks the leader, who is
// put back at the heart. Touch trigger 31 takes the lights away, and Retreat
// removes the column.

/// Raises the spark while its scale follows a half-sine pulse.
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

/// Grows the moving light over the first sixteen script frames.
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

/// Expands the released light before its script removes it.
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

/// Follows the linked actor and alternates the light's animation parameter.
static s32
MercuryLighthouse_FollowLeader(struct ScriptInterpreter *interpreter) {
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
  // The sprite slot is read by its offset, the plain-C form of actor->sprite
  // this site matches with (scene/README.md): through FieldActor's
  // single/group union the movs r0, #1 and the sprite load swap.
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

/// Places the two spark animations on the selected actor.
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
  Engine_AudioPlayCue(SE_130);
}

/// Touch trigger 11, the pool's heart: the column of light appears over the
/// leader if it is not there, and grows a stage with each pair of sparks, half
/// a second apart, up to three lights. The column's cell is where a sinking
/// leader comes back to; flag 0x161 is set while the leader has lights.
void MercuryLighthouse_ChargeLights(void) {
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
      // The height is written by its offset, the plain-C form of obj->y.fixed
      // this site matches with (scene/README.md): as the field, the sprite's
      // reload moves after the add and store.
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
  Engine_GameFlagSet(FLAG_NO_ENCOUNTERS_IN_PLACE);
  Engine_EventEnd();
}

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

/// Touch trigger 31: the leader loses their lights, and the column swells and
/// fades away.
void MercuryLighthouse_LoseLights(void) {
  struct MercuryLighthouseLights *state =
      *(struct MercuryLighthouseLights **)WORK_SLOT(35);
  struct FieldActor *target;

  if (state->levels == 0) {
    return;
  }

  state->levels = 0;
  Engine_GameFlagClear(FLAG_NO_ENCOUNTERS_IN_PLACE);

  target = state->pillar;
  if (target != 0) {
    target->timer = 0;
    Engine_ObjectSetScript(target, gMercuryLighthouse_ScaleCounterScript);
    Object_SetMode(target, 7);
    state->pillar = NULL;
  }
}

/// Pushing against the pool's edge (triggers 11, 21 and 31): the leader hops
/// two cells the way the pad points, unless the hop leads into the pool without
/// a light or is blocked. A hop into the pool spends a light, which flies ahead
/// to the landing cell; the column shrinks a stage, or goes with the last
/// light. Landing in the pool with no light under them, the leader sinks until
/// a key is pressed after half a second, and is put back at the heart.
void MercuryLighthouse_HopAcrossPool(void) {
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
      Engine_GameFlagClear(FLAG_NO_ENCOUNTERS_IN_PLACE);
    } else if (state->pillar != NULL) {
      Object_SetMode(state->pillar, 6 - n);
    }
  }
  Object_SetMode(actor, 6);
  Engine_TaskWait(3);
  Engine_AudioPlayCue(SE_JUMP);
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
    Engine_AudioPlayCue(SE_215);
  Engine_TaskWait(1);
  *flags = saved;
  if (to->collisionCode == state->terrainCode && state->stepEffect == NULL) {
    Object_SetMode(actor, 18);
    Engine_AudioPlayCue(SE_241);
    for (i = 0;; i++) {
      if ((i & 15) == 0)
        OverlayObject_SpawnKind24AtObject(actor);
      if (i > 31 && gKeysPressed != 0)
        break;
      Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(SE_288);
    Engine_TaskWait(1);
    actor->x.fixed = state->targetX;
    actor->z.fixed = state->targetZ;
    Engine_ActorSetSpriteFlags(actor, 1);
  }
  state->sparkTimer = 0;
  Engine_EventEnd();
  work->walkingDistance += Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
}

/// Touch trigger 21: while the leader stands there, a sparkle rises from them
/// every 10 to 39 frames.
void MercuryLighthouse_SparkleAtLeader(void) {
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

/// Retreat (PSYNERGY_IN_AREA(ABILITY_RETREAT, 1)): remove the column of light.
static void MercuryLighthouse_RemoveColumnOnRetreat(void) {
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
