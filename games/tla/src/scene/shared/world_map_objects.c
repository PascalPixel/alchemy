/// @file
/// The world map's placed objects, which both the world map and the staff
/// credits' overlays keep a copy of, included as text: the countdowns an
/// object's side of the leader chooses, the task that shows the placed objects
/// coming into view and hides those leaving it, the cell an object stands on,
/// and the sparks and particles about them.

#include "types.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "field/sprite.h"
#include "system/io_write_queue.h"
#include "system/task.h"
#include "field/event_inline.h"
#include "system/system.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "field/actor_view.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/flag_call.h"

extern struct Actor *gWorldMap_PlacedActors[];

/// Chooses a passage exit from the leader's east-west approach: exitBefore
/// while the leader is west of the placed object in slot - 100, else exitAfter.
void WorldMap_SelectExitByX(s32 slot, s32 exitBefore, s32 exitAfter) {
  struct FieldWork *work;
  struct Actor *area;
  struct Actor *leader;

  area = gWorldMap_PlacedActors[slot - 100];
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  work = Ram_HeapSlots->fieldWork;
  if (leader->x.fixed < area->x.fixed) {
    work->exit = exitBefore;
  } else {
    work->exit = exitAfter;
  }
  Event_PlaySound(SE_MAP_EXIT);
}

/// Chooses a passage exit from the leader's north-south approach.
void WorldMap_SelectExitByZ(s32 slot, s32 exitBefore, s32 exitAfter) {
  struct FieldWork *work;
  struct Actor *area;
  struct Actor *leader;

  area = gWorldMap_PlacedActors[slot - 100];
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  work = Ram_HeapSlots->fieldWork;
  if (leader->z.fixed < area->z.fixed) {
    work->exit = exitBefore;
  } else {
    work->exit = exitAfter;
  }
  Event_PlaySound(SE_MAP_EXIT);
}

/// One of the scene's placed objects, 20 bytes; a zero kind ends the list.
struct WorldMapPlacement {
  u8 kind;
  u8 unk01; ///< 15 skips it
  s16 sprite;
  s16 x; ///< pixels
  s16 z; ///< pixels
  u16 angle;
  s16 flag; ///< -1 always shown; bit 12 shown while set, else while clear
  u8 unk0c[8];
};

struct WorldMapSprite {
  u8 unk00[0x0c];
  s32 scale;
};

extern struct Actor *gWorldMap_PlacedActors[];
extern s32 gWorldMap_PlacedActorScript[];

struct WorldMapPlacement *FlagEntries_GetTable(void);
void Actor_StartScript(struct Actor *object, const s32 *script);
void Import_ActorSelectAnimation(struct Actor *object, s32 mode);
void Actor_Release(struct Actor *object);

void WorldMap_UpdatePlacedActors(void) {
  struct WorldMapPlacement *placement = FlagEntries_GetTable();
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  s32 leaderReach =
      ((struct WorldMapSprite *)leader->sprite)->scale * leader->radius;
  u32 i;
  s32 next;
  s32 left, right, top, bottom;
  struct Actor **slot;
  /* FAKEMATCH(pin): the form kept. */
  register struct FieldWork *work asm("r9") = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = work->camera;
  s32 zoom = map->zoom;
  s32 x = object->x.fixed;
  u8 kind = placement->kind;
  s32 z = object->z.fixed;
  struct Actor **objects = gWorldMap_PlacedActors;

  if (zoom <= 0x10000) {
    left = x - 0xa00000;
    right = x + 0xa00000;
    top = z - 0x12c0000;
    bottom = z + 0xc80000;
  } else {
    left = x - 0xf00000;
    right = x + 0xf00000;
    top = z - 0x1c20000;
    bottom = z + 0x12c0000;
  }
  i = 0;
  next = kind;
  /* FAKEMATCH(barrier): the form kept. */
  asm("" : "+g"(next));
  slot = objects;
  /* FAKEMATCH(barrier): the form kept. */
  asm("" : "+g"(leader));
  while (next != 0) {
    s32 px, pz;

    if (placement->unk01 == 15)
      goto next;
    object = *slot;
    px = placement->x << 16;
    pz = placement->z << 16;
    if (px > left && px < right && pz > top && pz < bottom) {
      s32 flag = placement->flag;
      s32 shown;

      if (flag == -1)
        shown = 1;
      else if (flag & 0x1000)
        shown = GameFlag_Get(flag);
      else
        shown = Flag_IsClear(flag);
      if (shown) {
        if (object == NULL) {
          struct WorldMapSprite *sprite;

          object = ((struct Actor *)Import_ActorCreate(placement->sprite, px, 0,
                                                       pz));
          if (object == NULL)
            goto next;
          Actor_StartScript(object, gWorldMap_PlacedActorScript);
          object->motionFlags = 0;
          object->collisionFlags = 1;
          object->facing = placement->angle;
          Import_ActorSelectAnimation(object, 1);
          sprite = object->sprite;
          sprite->scale = Iwram_MulQ16(sprite->scale, Q16_ONE * 3 / 4);
          *slot = object;
        }
        if (gMainRoutineWork.ownerOverride == 0) {
          s32 scale = ((struct WorldMapSprite *)object->sprite)->scale;
          s32 dx;
          s32 reach, dz;

          dx = object->x.fixed - leader->x.fixed;
          if (dx < 0)
            dx = leader->x.fixed - object->x.fixed;
          reach = leaderReach + object->radius * scale;
          dz = object->z.fixed - leader->z.fixed;
          if (dz >= 0 ? dx + dz < reach
                      : dx + (leader->z.fixed - object->z.fixed) < reach) {
            if (GameFlag_Get(0x104) == 0) {
              if (gMainRoutineWork.movementMode == 9)
                work->psynergy = 0x208b;
              else
                work->stepTrigger = i + 100;
            }
          }
        }
        goto next;
      }
    }
    if (object != NULL) {
      Actor_Release(object);
      *slot = NULL;
    }
  next:
    i++;
    slot++;
    placement++;
    if (i > 127)
      break;
    next = placement->kind;
  }
}

/// While the object system runs, the world map's blend is queued for the
/// next frames.
void WorldMap_QueueBlend(void) {
  if ((u16)Ram_HeapSlots->actorSystem->suspended == 0) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f8e, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 12, 0x20000);
  }
}

void WorldMap_StartPlacedActors(void) {
  Iwram_ClearWords(gWorldMap_PlacedActors, 0x200);
  Task_AddFrame(WorldMap_UpdatePlacedActors, TASK_PASS_VBLANK);
}

/// The cell index of a point in 16.16 pixels.
s32 WorldMap_GetCellIndex(s32 x, s32 z) {
  return x / 0x100000 + (z / 0x100000) * 128;
}

s32 WorldMap_RiseFromAnchor(struct Actor *object) {
  struct Actor *parent = object->watchedActor;
  s32 step = ++object->timer;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(step << 10), Q16_ONE * 5 / 4);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x.fixed = parent->x.fixed;
  object->y.fixed += px(1);
  object->z.fixed = parent->z.fixed;
  return 1;
}

extern s32 gWorldMap_SparkScript[];
extern u32 gWorldMap_ParticleScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

/// Spawns a pair of sparks at the object.
// As ☀️'s MercuryLighthouse_SpawnSparkPair does at a spawn point.
void WorldMap_SpawnSparkPair(struct Actor *object) {
  struct Actor *spark;
  struct FieldSprite *sprite;

  // DEAD: a scale multiplied and dropped; the ROM keeps the call.
  Math_MulQ16(object->scaleX, Q16_ONE * 3 / 2);
  spark =
      FieldActor_Create(271, object->x.fixed, object->y.fixed, object->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = object->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gWorldMap_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = object;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 2);
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  spark =
      FieldActor_Create(271, object->x.fixed, object->y.fixed, object->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = object->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gWorldMap_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = object;
    spark->layer = 1;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 1);
      sprite->nextFrame = 0;
    }
  }
}

/// Spawns a particle at a random spot around the object.
void WorldMap_SpawnParticle(struct Actor *object) {
  s32 position[3];
  s32 angle;
  struct Actor *particle;
  struct FieldSprite *sprite;

  Iwram_MulQ16(object->scaleX, Q16_ONE * 3 / 2);
  SET_VECTOR(position, object->x.fixed, object->y.fixed + px(2),
             object->z.fixed);
  angle = Random16();
  FieldVector_AddPolarOffsetFixed(angle * 20, Random16(), position);
  particle = (FieldActor_Create(674, position[0], position[1], position[2]));
  if (particle != NULL) {
    particle->groundHeight = object->groundHeight;
    sprite = particle->sprite;
    Actor_StartScript(particle, gWorldMap_ParticleScript);
    Sprite_StartAnimation(sprite, 2);
    particle->gravity = px(1) / 128;
    particle->scaleX = Q16_ONE * 4 / 5;
    particle->scaleY = Q16_ONE * 4 / 5;
    if (sprite != NULL)
      sprite->nextFrame = 0;
  }
}
