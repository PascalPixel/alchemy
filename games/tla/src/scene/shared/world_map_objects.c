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
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "actor_view.h"
#include "system/main_routine_work.h"
#include "event_work.h"
#include "map.h"
#include "flag_call.h"

extern struct Actor *gWorldMap_PlacedActors[];

/// The event countdown takes before when the party leader's x is less than
/// that of the object in slot - 100 of the overlay's table, else after.
void WorldMap_SetCountdownByX(s32 slot, s32 before, s32 after) {
  /* FAKEMATCH(pin): after copied to r6 on entry. */
  register s32 elseValue asm("r6") = after;
  struct Actor *target = gWorldMap_PlacedActors[slot - 100];
  struct Actor *owner;
  struct EventWork *work;

  /* FAKEMATCH(barrier): ⚓️ loads gMainRoutineWork's address before it builds
   * the currentOwner offset. */
  asm volatile("");
  {
    /* FAKEMATCH(pin): before copied to r7 after the barrier, beside the call.
     */
    register s32 thenValue asm("r7") = before;

    owner = Actor_Get(gMainRoutineWork.currentOwner);
    work = Ram_HeapSlots->eventWork;
    if (owner->x < target->x)
      work->countdown = thenValue;
    else
      work->countdown = elseValue;
  }
  Event_PlaySound(SE_MAP_EXIT);
}

/// The same on z.
void WorldMap_SetCountdownByZ(s32 slot, s32 before, s32 after) {
  /* FAKEMATCH(pin): after copied to r6 on entry. */
  register s32 elseValue asm("r6") = after;
  struct Actor *target = gWorldMap_PlacedActors[slot - 100];
  struct Actor *owner;
  struct EventWork *work;

  /* FAKEMATCH(barrier): ⚓️ loads gMainRoutineWork's address before it builds
   * the currentOwner offset. */
  asm volatile("");
  {
    /* FAKEMATCH(pin): before copied to r7 after the barrier, beside the call.
     */
    register s32 thenValue asm("r7") = before;

    owner = Actor_Get(gMainRoutineWork.currentOwner);
    work = Ram_HeapSlots->eventWork;
    if (owner->z < target->z)
      work->countdown = thenValue;
    else
      work->countdown = elseValue;
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
  register struct EventWork *work asm("r9") = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = work->viewCenter;
  s32 zoom = map->zoom;
  s32 x = object->x;
  u8 kind = placement->kind;
  s32 z = object->z;
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
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
          *slot = object;
        }
        if (gMainRoutineWork.ownerOverride == 0) {
          s32 scale = ((struct WorldMapSprite *)object->sprite)->scale;
          s32 dx;
          s32 reach, dz;

          dx = object->x - leader->x;
          if (dx < 0)
            dx = leader->x - object->x;
          reach = leaderReach + object->radius * scale;
          dz = object->z - leader->z;
          if (dz >= 0 ? dx + dz < reach
                      : dx + (leader->z - object->z) < reach) {
            if (GameFlag_Get(0x104) == 0) {
              if (gMainRoutineWork.renderMode == 9)
                work->unk166 = 0x208b;
              else
                work->exit = i + 100;
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
  if ((u16)Ram_HeapSlots->objectSystem->suspended == 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, 12, 0x20000);
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
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(step << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = parent->x;
  object->y += 0x10000;
  object->z = parent->z;
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
  s32 (*mulQ16)(s32 a, s32 b) = Iwram_MulQ16;

  /* FAKEMATCH(temporary): the resident multiply called through a local pointer.
   */
  mulQ16(object->scaleX, 0x18000);
  spark = FieldActor_Create(271, object->x, object->y, object->z);
  if (spark != NULL) {
    spark->terrainHeight = object->terrainHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gWorldMap_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedObject = object;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 2);
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  spark = FieldActor_Create(271, object->x, object->y, object->z);
  if (spark != NULL) {
    spark->terrainHeight = object->terrainHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gWorldMap_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedObject = object;
    spark->terrainId = 1;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 1);
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

  Iwram_MulQ16(object->scaleX, 0x18000);
  SET_VECTOR(position, object->x, object->y + 0x20000, object->z);
  angle = Random16();
  FieldVector_AddPolarOffset(angle * 20, Random16(), position);
  particle = (FieldActor_Create(674, position[0], position[1], position[2]));
  if (particle != NULL) {
    particle->terrainHeight = object->terrainHeight;
    sprite = particle->sprite;
    Actor_StartScript(particle, gWorldMap_ParticleScript);
    Animation_ApplyChildArgument(sprite, 2);
    particle->gravity = 0x200;
    particle->scaleX = 0xcccc;
    particle->scaleY = 0xcccc;
    if (sprite != NULL)
      sprite->nextFrame = 0;
  }
}
