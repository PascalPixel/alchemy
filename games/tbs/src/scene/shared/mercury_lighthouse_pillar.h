#include "script_operands.h"
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
#include "event.h"
#include "system_call.h"
#include "field_effect.h"
#include "ram_buffer.h"
#include "map_grid.h"
#include "scene.h"
#include "heap.h"
#include "keystate.h"
#include "fixed_math.h"
#include "sounds.h"

// Mercury Lighthouse and its entrance, once the pool is woken: standing at the
// pool's heart (CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART) gives the leader
// three lights, shown as a column of light over them. Each hop into the pool
// (the floor's region, its cell behavior) spends one, and a light comes to hold
// the leader up where they land. A hop within the pool with no light left sinks
// the leader, who is put back at the heart.
// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_RIM takes the lights away, and Retreat
// removes the column.

/// Raises the spark while its scale follows a half-sine pulse.
static s32
MercuryLighthouse_AdvanceAnchoredRiseFrame(struct Actor *interpreter) {
  struct Actor *effect = interpreter;
  struct Actor *anchor = effect->watchedActor;
  s16 step = ++effect->timer;
  s32 scale;

  if (step > 31) {
    return 0;
  }

  scale = Trig_Sin(step << 10);
  effect->scaleX = scale;
  effect->scaleY = scale;
  effect->x.fixed = anchor->x.fixed;
  effect->y.fixed += PIXELS(1);
  effect->z.fixed = anchor->z.fixed;
  return 1;
}

/// Grows the moving light over the first sixteen script frames.
static s32 MercuryLighthouse_GrowLight(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
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
static s32 Actor_AdvanceScaleCounter(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s16 frame;

  frame = ++actor->timer;
  if (frame > 16) {
    return 0;
  }
  actor->scaleX = (frame << 11) + Q16_ONE;
  actor->scaleY = (frame << 11) + Q16_ONE;
  return 1;
}

/// Follows the watched actor and alternates the light's animation
/// parameter.
static s32 MercuryLighthouse_FollowLeader(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  struct Actor *leader;
  s32 targetZ;
  s32 targetX;
  s32 deltaX;
  s32 deltaZ;
  s32 distance;
  s32 moveDistance;

  actor->speedLimit = PIXELS(2);
  leader = actor->watchedActor;
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
  if (moveDistance > actor->speedLimit)
    moveDistance = actor->speedLimit;

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
  // The sprite slot is read by its offset, the plain-C form of
  // actor->sprite this site matches with (scene/README.md): through
  // Actor's single/group union the movs r0, #1 and the sprite load swap.
  {
    struct Sprite *sprite =
        *(struct Sprite **)((u8 *)actor + offsetof(struct Actor, sprite));
    struct SpriteAnimation *entry = sprite->entries[0];

    entry->colorMode = ((gFrameCount >> 1) & 1) * 7;
    sprite->dirty = 1;
  }
  return 1;
}

/// Places the two spark animations on the selected actor.
static void MercuryLighthouse_SpawnSparkPair(void) {
  struct Actor *point;
  struct Actor *spark;
  struct Sprite *sprite;

  point = gFieldWork->actors[gSequenceParentWork.leaderActorId];
  spark = FieldActor_Create(SPRITE_GOLDEN_RINGS, point->x.fixed, point->y.fixed,
                            point->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = point->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = point;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  spark = FieldActor_Create(SPRITE_GOLDEN_RINGS, point->x.fixed, point->y.fixed,
                            point->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = point->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = point;
    spark->priorityFlags = 2;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 1);
      sprite->flags = 0;
    }
  }
  Sound_Play(SE_PSYNERGY_START);
}

/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, the pool's heart: the column of
/// light appears over the leader if it is not there, and grows a stage with
/// each pair of sparks, half a second apart, up to three lights. The column's
/// cell is where a sinking leader comes back to; FLAG_NO_ENCOUNTERS_IN_AREA is
/// set while the leader has lights.
void MercuryLighthouse_ChargeLights(void) {
  struct MercuryLighthouseLights *state;
  struct Actor *leader;
  struct Actor *pillar;
  struct Actor *actor;
  s32 step;

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  leader = ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))
               ->actors[gSequenceParentWork.leaderActorId];
  if ((u32)state->levels > 2)
    return;
  Event_Start();
  pillar = state->pillar;
  if (pillar == 0) {
    actor = FieldActor_Create(SPRITE_GOLDEN_RINGS, leader->x.fixed,
                              leader->y.fixed + 0x180000, leader->z.fixed);
    if (actor != 0) {
      struct Sprite *sprite = actor->sprite;

      actor->groundHeight = leader->groundHeight;
      Actor_StartScript(actor, gMercuryLighthouse_PillarScript);
      actor->watchedActor = leader;
      actor->motionFlags = 4;
      // The height is written by its offset, the plain-C form of obj->y.fixed
      // this site matches with (scene/README.md): as the field, the sprite's
      // reload moves after the add and store.
      *(s32 *)((u8 *)actor + offsetof(struct Actor, y)) += -PIXELS(1) / 2;
      if (sprite != 0) {
        sprite->flags = 0;
        sprite->image.priority = 1;
      }
      actor->spriteStorageKind = 0;
      state->pillar = actor;
      pillar = actor;
    } else {
      pillar = state->pillar;
    }
  }
  for (step = state->levels; step <= 2; step++) {
    MercuryLighthouse_SpawnSparkPair();
    wait(30);
    pillar->spriteStorageKind = 1;
    Actor_SelectAnimation(pillar, 5 - step);
  }
  state->levels = 3;
  state->targetX = (pillar->x.fixed & -0x100000) + 0x80000;
  state->targetZ = (pillar->z.fixed & -0x100000) + 0x80000;
  GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_AREA);
  Event_End();
}

#define MAPGRID_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)

/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_RIM: the leader loses their lights,
/// and the column swells and fades away.
void MercuryLighthouse_LoseLights(void) {
  struct MercuryLighthouseLights *state =
      *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
          HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  struct Actor *target;

  if (state->levels == 0) {
    return;
  }

  state->levels = 0;
  GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_AREA);

  target = state->pillar;
  if (target != 0) {
    target->timer = 0;
    Actor_StartScript(target, gMercuryLighthouse_ScaleCounterScript);
    Actor_SelectAnimation(target, 7);
    state->pillar = NULL;
  }
}

/// Pushing against the pool's edge
/// (CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART,
/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER and
/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_RIM): the leader hops two cells the
/// way the pad points, unless the hop leads into the pool without a light or is
/// blocked. A hop into the pool spends a light, which flies ahead to the
/// landing cell; the column shrinks a stage, or goes with the last light.
/// Landing in the pool with no light under them, the leader sinks until a key
/// is pressed after half a second, and is put back at the heart.
void MercuryLighthouse_HopAcrossPool(void) {
  struct FieldWork *work;
  struct MercuryLighthouseLights *state;
  struct Actor *actor;
  struct Actor *source;
  struct Actor *obj;
  struct Sprite *sprite;
  struct MapGridCell *from;
  struct MapGridCell *to;
  u8 saved;
  u8 *flags;
  s32 angle;
  u32 keys;
  s32 n;
  s32 i;
  u8 zero;
  union FieldCoordinate pos[3];

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  actor = work->actors[gSequenceParentWork.leaderActorId];
  flags = &actor->motionFlags;
  saved = *flags;
  keys = gKeysHeld;
  angle = gMercuryLighthouse_MoveAngles[(keys >> 4) & 15];
  if ((s16)gMercuryLighthouse_MoveAngles[(keys >> 4) & 15] == -1)
    return;
  pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
  pos[1].fixed = actor->groundHeight;
  pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
  from =
      &MAPGRID_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  Vector_AddPolarOffset(0x200000, angle, &pos[0].fixed);
  to = &MAPGRID_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  if (from->behavior != state->poolBehavior &&
      to->behavior == state->poolBehavior && state->levels == 0)
    return;

  Event_Start();
  if (FieldActor_CheckMovementCollision(actor, (struct FieldPosition *)pos) !=
      0)
    return;
  obj = state->stepEffect;
  if (obj != NULL) {
    obj->timer = 0;
    Actor_StartScript(obj, gMercuryLighthouse_ScaleCounterScript);
    Actor_SelectAnimation(obj, 7);
    state->stepEffect = NULL;
  }
  if (to->behavior == state->poolBehavior && state->levels != 0) {
    source = state->pillar;
    obj = FieldActor_Create(SPRITE_GOLDEN_RINGS, source->x.fixed,
                            source->y.fixed, source->z.fixed);
    if (obj != NULL) {
      sprite = obj->sprite;
      obj->groundHeight = source->groundHeight;
      Actor_StartScript(obj, gMercuryLighthouse_RampScript);
      obj->motionFlags = 0;
      obj->timer = 0;
      obj->priorityFlags = 2;
      obj->speedLimit = 0x40000;
      obj->acceleration = 0x20000;
      FieldActor_SetMoveTarget(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 6);
        zero = 0;
        sprite->flags = zero;
      }
      state->stepEffect = obj;
    }
    n = state->levels - 1;
    state->levels = n;
    if (n == 0) {
      Actor_Release(state->pillar);
      state->pillar = NULL;
      GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_AREA);
    } else if (state->pillar != NULL) {
      Actor_SelectAnimation(state->pillar, 6 - n);
    }
  }
  Actor_SelectAnimation(actor, 6);
  wait(3);
  Sound_Play(SE_JUMP);
  Actor_SelectAnimation(actor, 7);
  actor->speedLimit = 0x30000;
  actor->acceleration = 0x20000;
  actor->velocityY = 0x40000;
  *flags &= 0x7e;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, pos[0].part.pixel,
                             pos[2].part.pixel);
  Actor_SelectAnimation(actor, 6);
  wait(2);
  if (to->behavior != state->poolBehavior)
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  else
    Sound_Play(SE_SPLASH_SMALL);
  wait(1);
  *flags = saved;
  if (to->behavior == state->poolBehavior && state->stepEffect == NULL) {
    Actor_SelectAnimation(actor, 18);
    Sound_Play(SE_WATER_MOVE);
    for (i = 0;; i++) {
      if ((i & 15) == 0)
        MercuryLighthouse_SpawnRipple(actor);
      if (i > 31 && gKeysPressed != 0)
        break;
      wait(1);
    }
    Sound_Play(SE_STOP_ACTION_SE);
    wait(1);
    actor->x.fixed = state->targetX;
    actor->z.fixed = state->targetZ;
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  }
  state->rippleTimer = 0;
  Event_End();
  work->walkingDistance += Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
}

/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER: while the leader stands there,
/// a blue ripple spreads from him every 10 to 39 frames.
void MercuryLighthouse_RippleAroundLeader(void) {
  struct MercuryLighthouseLights *state;
  struct Actor *point;

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  point = ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))
              ->actors[gSequenceParentWork.leaderActorId];
  if (state->rippleTimer != 0) {
    state->rippleTimer--;
  } else {
    MercuryLighthouse_SpawnRipple(point);
    state->rippleTimer = ((u32)(Import_RandomNext() * 30) >> 16) + 10;
  }
}

/// Retreat (PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST)):
/// remove the column of light.
static void MercuryLighthouse_RemoveColumnOnRetreat(void) {
  struct MercuryLighthouseLights **published =
      HeapSlot_AllocEwram(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, 4);
  struct MercuryLighthouseLights *state;
  struct Actor *pillar;

  if (published == NULL)
    return;
  state = *published;
  pillar = state->pillar;
  if (pillar == NULL)
    return;
  Actor_Release(pillar);
  state->pillar = NULL;
}
