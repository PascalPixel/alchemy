#include "action_script.h"
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
#include "script_layer.h"
#include "engine_call.h"
#include "field_effect.h"
#include "ram_buffer.h"
#include "map_grid.h"
#include "map_header.h"
#include "heap.h"
#include "keystate.h"
#include "fixed_math.h"
#include "songs.h"

// Mercury Lighthouse and its entrance, once the pool is woken: standing at the
// pool's heart (coord event 11) gives the leader three lights, shown as a
// column of light over them. Each hop into the pool (the floor's region, its
// metatile behavior) spends one, and a light comes to hold the leader up where
// they land. A hop within the pool with no light left sinks the leader, who is
// put back at the heart. Coord event 31 takes the lights away, and Retreat
// removes the column.

/// Raises the spark while its scale follows a half-sine pulse.
static s32
MercuryLighthouse_AdvanceAnchoredRiseFrame(struct Object *interpreter) {
  struct ObjectEvent *effect = (struct ObjectEvent *)interpreter;
  struct ObjectEvent *anchor = effect->linkedObject;
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
static s32 MercuryLighthouse_GrowLight(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 frame;

  frame = ++objectEvent->timer;
  if (frame > 16) {
    return 0;
  }
  objectEvent->scaleX = (frame * 3) << 10;
  objectEvent->scaleY = (frame * 3) << 10;
  return 1;
}

/// Expands the released light before its script removes it.
static s32 ObjectEvent_AdvanceScaleCounter(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 frame;

  frame = ++objectEvent->timer;
  if (frame > 16) {
    return 0;
  }
  objectEvent->scaleX = (frame << 11) + Q16_ONE;
  objectEvent->scaleY = (frame << 11) + Q16_ONE;
  return 1;
}

/// Follows the linked object event and alternates the light's animation
/// parameter.
static s32 MercuryLighthouse_FollowLeader(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  struct ObjectEvent *leader;
  s32 targetZ;
  s32 targetX;
  s32 deltaX;
  s32 deltaZ;
  s32 distance;
  s32 moveDistance;

  objectEvent->speed = PIXELS(2);
  leader = objectEvent->linkedObject;
  objectEvent->acceleration = PIXELS(1);
  targetX = leader->x.fixed;
  targetZ = leader->z.fixed;
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;

  // Measure in whole pixels first; use fixed-point precision near the leader.
  deltaX = (targetX - objectEvent->x.fixed) / PIXELS(1);
  deltaZ = (targetZ - objectEvent->z.fixed) / PIXELS(1);
  distance = (u16)Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
  deltaX = targetX - objectEvent->x.fixed;
  deltaZ = targetZ - objectEvent->z.fixed;
  distance <<= 16;
  if (distance < PIXELS(64))
    distance =
        FixedSqrt(Iwram_MulQ16(deltaX, deltaX) + Iwram_MulQ16(deltaZ, deltaZ));
  moveDistance = distance / 8;
  if (moveDistance > objectEvent->speed)
    moveDistance = objectEvent->speed;

  // Snap within a quarter pixel; otherwise ease toward the leader.
  if (distance < PIXELS(1) / 4) {
    objectEvent->x.fixed = targetX;
    objectEvent->z.fixed = targetZ;
  } else {
    if (distance > moveDistance) {
      deltaX = Iwram_MulQ16(Iwram_RatioMulQ14(distance, deltaX), moveDistance);
      deltaZ = Iwram_MulQ16(Iwram_RatioMulQ14(distance, deltaZ), moveDistance);
    }
    objectEvent->x.fixed += deltaX;
    objectEvent->z.fixed += deltaZ;
  }

  // Alternate the light's draw parameter every two frames and redraw it.
  // The sprite slot is read by its offset, the plain-C form of
  // objectEvent->sprite this site matches with (map/README.md): through
  // ObjectEvent's single/group union the movs r0, #1 and the sprite load swap.
  {
    struct Sprite *sprite =
        *(struct Sprite **)((u8 *)objectEvent +
                            offsetof(struct ObjectEvent, sprite));
    struct SpriteAnim *entry = sprite->entries[0];

    entry->colorMode = ((gFrameCount >> 1) & 1) * 7;
    sprite->dirty = 1;
  }
  return 1;
}

/// Places the two spark animations on the selected object event.
static void MercuryLighthouse_SpawnSparkPair(void) {
  struct ObjectEvent *point;
  struct ObjectEvent *spark;
  struct Sprite *sprite;

  point = gFieldWork->objectEvents[gSaveBlock.leaderLocalId];
  spark =
      ObjectEvent_Create(26, point->x.fixed, point->y.fixed, point->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = point->groundHeight;
    sprite = spark->sprite;
    ObjectEvent_SetScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->linkedObject = point;
    if (sprite != NULL) {
      Sprite_StartAnim(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 1;
    }
  }
  spark =
      ObjectEvent_Create(26, point->x.fixed, point->y.fixed, point->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = point->groundHeight;
    sprite = spark->sprite;
    ObjectEvent_SetScript(spark, gMercuryLighthouse_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->linkedObject = point;
    spark->priorityFlags = 2;
    if (sprite != NULL) {
      Sprite_StartAnim(sprite, 1);
      sprite->flags = 0;
    }
  }
  Sound_PlaySong(SE_130);
}

/// Coord event 11, the pool's heart: the column of light appears over the
/// leader if it is not there, and grows a stage with each pair of sparks, half
/// a second apart, up to three lights. The column's cell is where a sinking
/// leader comes back to; FLAG_NO_ENCOUNTERS_IN_MAPSEC is set while the leader
/// has lights.
void MercuryLighthouse_ChargeLights(void) {
  struct MercuryLighthouseLights *state;
  struct ObjectEvent *leader;
  struct ObjectEvent *pillar;
  struct ObjectEvent *objectEvent;
  s32 step;

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  leader = ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))
               ->objectEvents[gSaveBlock.leaderLocalId];
  if ((u32)state->levels > 2)
    return;
  FieldScript_Start();
  pillar = state->pillar;
  if (pillar == 0) {
    objectEvent = ObjectEvent_Create(
        26, leader->x.fixed, leader->y.fixed + 0x180000, leader->z.fixed);
    if (objectEvent != 0) {
      struct Sprite *sprite = objectEvent->sprite;

      objectEvent->groundHeight = leader->groundHeight;
      ObjectEvent_SetScript(objectEvent, gMercuryLighthouse_PillarScript);
      objectEvent->linkedObject = leader;
      objectEvent->motionFlags = 4;
      // The height is written by its offset, the plain-C form of obj->y.fixed
      // this site matches with (map/README.md): as the field, the sprite's
      // reload moves after the add and store.
      *(s32 *)((u8 *)objectEvent + offsetof(struct ObjectEvent, y)) +=
          -PIXELS(1) / 2;
      if (sprite != 0) {
        sprite->flags = 0;
        sprite->image.priority = 1;
      }
      objectEvent->spriteStorageKind = 0;
      state->pillar = objectEvent;
      pillar = objectEvent;
    } else {
      pillar = state->pillar;
    }
  }
  for (step = state->levels; step <= 2; step++) {
    MercuryLighthouse_SpawnSparkPair();
    WaitFrames(30);
    pillar->spriteStorageKind = 1;
    Object_SelectAnimation(pillar, 5 - step);
  }
  state->levels = 3;
  state->targetX = (pillar->x.fixed & -0x100000) + 0x80000;
  state->targetZ = (pillar->z.fixed & -0x100000) + 0x80000;
  GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_MAPSEC);
  FieldScript_End();
}

#define MAPGRID_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)

/// Coord event 31: the leader loses their lights, and the column swells and
/// fades away.
void MercuryLighthouse_LoseLights(void) {
  struct MercuryLighthouseLights *state =
      *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
          HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  struct ObjectEvent *target;

  if (state->levels == 0) {
    return;
  }

  state->levels = 0;
  GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_MAPSEC);

  target = state->pillar;
  if (target != 0) {
    target->timer = 0;
    ObjectEvent_SetScript(target, gMercuryLighthouse_ScaleCounterScript);
    Object_SelectAnimation(target, 7);
    state->pillar = NULL;
  }
}

/// Pushing against the pool's edge (coord events 11, 21 and 31): the leader
/// hops two cells the way the pad points, unless the hop leads into the pool
/// without a light or is blocked. A hop into the pool spends a light, which
/// flies ahead to the landing cell; the column shrinks a stage, or goes with
/// the last light. Landing in the pool with no light under them, the leader
/// sinks until a key is pressed after half a second, and is put back at the
/// heart.
void MercuryLighthouse_HopAcrossPool(void) {
  struct FieldWork *work;
  struct MercuryLighthouseLights *state;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *source;
  struct ObjectEvent *obj;
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
  objectEvent = work->objectEvents[gSaveBlock.leaderLocalId];
  flags = &objectEvent->motionFlags;
  saved = *flags;
  keys = gKeysHeld;
  angle = gMercuryLighthouse_MoveAngles[(keys >> 4) & 15];
  if ((s16)gMercuryLighthouse_MoveAngles[(keys >> 4) & 15] == -1)
    return;
  pos[0].fixed = (objectEvent->x.fixed & -0x100000) + 0x80000;
  pos[1].fixed = objectEvent->groundHeight;
  pos[2].fixed = (objectEvent->z.fixed & -0x100000) + 0x80000;
  from =
      &MAPGRID_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  Vector_AddPolarOffset(0x200000, angle, &pos[0].fixed);
  to = &MAPGRID_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
  if (from->behavior != state->poolBehavior &&
      to->behavior == state->poolBehavior && state->levels == 0)
    return;

  FieldScript_Start();
  if (FieldObject_CheckMovementCollision(objectEvent,
                                         (struct FieldPosition *)pos) != 0)
    return;
  obj = state->stepEffect;
  if (obj != NULL) {
    obj->timer = 0;
    ObjectEvent_SetScript(obj, gMercuryLighthouse_ScaleCounterScript);
    Object_SelectAnimation(obj, 7);
    state->stepEffect = NULL;
  }
  if (to->behavior == state->poolBehavior && state->levels != 0) {
    source = state->pillar;
    obj = ObjectEvent_Create(26, source->x.fixed, source->y.fixed,
                             source->z.fixed);
    if (obj != NULL) {
      sprite = obj->sprite;
      obj->groundHeight = source->groundHeight;
      ObjectEvent_SetScript(obj, gMercuryLighthouse_RampScript);
      obj->motionFlags = 0;
      obj->timer = 0;
      obj->priorityFlags = 2;
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      Object_SetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
      if (sprite != NULL) {
        Sprite_StartAnim(sprite, 6);
        zero = 0;
        sprite->flags = zero;
      }
      state->stepEffect = obj;
    }
    n = state->levels - 1;
    state->levels = n;
    if (n == 0) {
      ObjectEvent_Release(state->pillar);
      state->pillar = NULL;
      GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_MAPSEC);
    } else if (state->pillar != NULL) {
      Object_SelectAnimation(state->pillar, 6 - n);
    }
  }
  Object_SelectAnimation(objectEvent, 6);
  WaitFrames(3);
  Sound_PlaySong(SE_JUMP);
  Object_SelectAnimation(objectEvent, 7);
  objectEvent->speed = 0x30000;
  objectEvent->acceleration = 0x20000;
  objectEvent->velocityY = 0x40000;
  *flags &= 0x7e;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectMotion_SetPositionAndCommit(0, pos[0].part.pixel, pos[2].part.pixel);
  Object_SelectAnimation(objectEvent, 6);
  WaitFrames(2);
  if (to->behavior != state->poolBehavior)
    ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  else
    Sound_PlaySong(SE_215);
  WaitFrames(1);
  *flags = saved;
  if (to->behavior == state->poolBehavior && state->stepEffect == NULL) {
    Object_SelectAnimation(objectEvent, 18);
    Sound_PlaySong(SE_WATER_MOVE);
    for (i = 0;; i++) {
      if ((i & 15) == 0)
        MercuryLighthouse_SpawnRipple(objectEvent);
      if (i > 31 && gKeysPressed != 0)
        break;
      WaitFrames(1);
    }
    Sound_PlaySong(SE_STOP_SLOT3);
    WaitFrames(1);
    objectEvent->x.fixed = state->targetX;
    objectEvent->z.fixed = state->targetZ;
    ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  }
  state->rippleTimer = 0;
  FieldScript_End();
  work->walkingDistance += Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
}

/// Coord event 21: while the leader stands there, a blue ripple spreads from
/// him every 10 to 39 frames.
void MercuryLighthouse_RippleAroundLeader(void) {
  struct MercuryLighthouseLights *state;
  struct ObjectEvent *point;

  state = *(struct MercuryLighthouseLights **)HEAP_SLOT_BLOCK(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
  point = ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))
              ->objectEvents[gSaveBlock.leaderLocalId];
  if (state->rippleTimer != 0) {
    state->rippleTimer--;
  } else {
    MercuryLighthouse_SpawnRipple(point);
    state->rippleTimer = ((u32)(Import_RandomNext() * 30) >> 16) + 10;
  }
}

/// Retreat (PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST)):
/// remove the column of light.
static void MercuryLighthouse_RemoveColumnOnRetreat(void) {
  struct MercuryLighthouseLights **published =
      AllocHeapSlotEwram(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, 4);
  struct MercuryLighthouseLights *state;
  struct ObjectEvent *pillar;

  if (published == NULL)
    return;
  state = *published;
  pillar = state->pillar;
  if (pillar == NULL)
    return;
  ObjectEvent_Release(pillar);
  state->pillar = NULL;
}
