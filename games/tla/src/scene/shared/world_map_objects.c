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
#include "field/world_map.h"
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

extern struct Actor *gWorldMap_PlacedActors[];
extern s32 gWorldMap_PlacedActorScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);
void Actor_Release(struct Actor *actor);

void WorldMap_UpdatePlacedActors(void) {
  const struct FlagEntry *placement = FlagEntries_GetTable();
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  s32 leaderReach = ((struct Sprite *)leader->sprite)->scale * leader->radius;
  u32 i;
  s32 next;
  FIXED left, right, top, bottom;
  struct Actor **slot;
  /* FAKEMATCH(pin): the form kept. */
  register struct FieldWork *work asm("r9") = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = work->camera;
  s32 zoom = map->zoom;
  FIXED x = actor->x.fixed;
  u8 kind = placement->flag;
  FIXED z = actor->z.fixed;
  struct Actor **objects = gWorldMap_PlacedActors;

  if (zoom <= FIXED_ONE) {
    left = x - toFIXED(160);
    right = x + toFIXED(160);
    top = z - toFIXED(300);
    bottom = z + toFIXED(200);
  } else {
    left = x - toFIXED(240);
    right = x + toFIXED(240);
    top = z - toFIXED(450);
    bottom = z + toFIXED(300);
  }
  i = 0;
  next = kind;
  /* FAKEMATCH(barrier): the form kept. */
  asm("" : "+g"(next));
  slot = objects;
  /* FAKEMATCH(barrier): the form kept. */
  asm("" : "+g"(leader));
  while (next != 0) {
    FIXED px, pz;

    if (placement->value == WORLD_MAP_MARKER_SHIP)
      goto next;
    actor = *slot;
    px = toFIXED(placement->x);
    pz = toFIXED(placement->z);
    if (px > left && px < right && pz > top && pz < bottom) {
      s32 flag = placement->shownFlag;
      s32 shown;

      if (flag == -1)
        shown = 1;
      else if (flag & 0x1000)
        shown = GameFlag_Get(flag);
      else
        shown = Flag_IsClear(flag);
      if (shown) {
        if (actor == NULL) {
          struct Sprite *sprite;

          actor = ((struct Actor *)Import_ActorCreate(placement->sprite, px, 0,
                                                      pz));
          if (actor == NULL)
            goto next;
          Actor_StartScript(actor, gWorldMap_PlacedActorScript);
          actor->motionFlags = 0;
          actor->collisionFlags = ACTOR_SOLID;
          actor->facing = placement->facing;
          Actor_SelectAnimation(actor, 1);
          sprite = actor->sprite;
          sprite->scale = Iwram_MulFX(sprite->scale, FIXED_ONE * 3 / 4);
          *slot = actor;
        }
        if (gMainRoutineWork.ownerOverride == 0) {
          s32 scale = ((struct Sprite *)actor->sprite)->scale;
          FIXED dx;
          s32 reach;
          FIXED dz;

          dx = actor->x.fixed - leader->x.fixed;
          if (dx < 0)
            dx = leader->x.fixed - actor->x.fixed;
          reach = leaderReach + actor->radius * scale;
          dz = actor->z.fixed - leader->z.fixed;
          if (dz >= 0 ? dx + dz < reach
                      : dx + (leader->z.fixed - actor->z.fixed) < reach) {
            if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
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
    if (actor != NULL) {
      Actor_Release(actor);
      *slot = NULL;
    }
  next:
    i++;
    slot++;
    placement++;
    if (i > 127)
      break;
    next = placement->flag;
  }
}

/// While the object system runs, the world map's blend is queued for the
/// next frames.
void WorldMap_QueueBlend(void) {
  if ((u16)Ram_HeapSlots->actorSystem->suspended == 0) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                            BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 12, IO_WRITE_HALFWORD);
  }
}

void WorldMap_StartPlacedActors(void) {
  Iwram_ClearWords(gWorldMap_PlacedActors, 128 * sizeof(struct Actor *));
  Task_AddFrame(WorldMap_UpdatePlacedActors, TASK_PASS_FRAME);
}

/// The cell index of a point in 16.16 pixels.
s32 WorldMap_GetCellIndex(FIXED x, FIXED z) {
  return x / toFIXED(16) + (z / toFIXED(16)) * 128;
}

s32 WorldMap_RiseFromAnchor(struct Actor *actor) {
  struct Actor *parent = actor->watchedActor;
  s32 step = ++actor->timer;
  FIXED scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulFX(Math_Sin(step << 10), FIXED_ONE * 5 / 4);
  actor->scaleX = scale;
  actor->scaleY = scale;
  actor->x.fixed = parent->x.fixed;
  actor->y.fixed += toFIXED(1);
  actor->z.fixed = parent->z.fixed;
  return 1;
}

extern s32 gWorldMap_SparkScript[];
extern u32 gWorldMap_ParticleScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

/// Spawns a pair of sparks at the actor.
// As ☀️'s MercuryLighthouse_SpawnSparkPair does at a spawn point.
void WorldMap_SpawnSparkPair(struct Actor *actor) {
  struct Actor *spark;
  struct FieldSprite *sprite;

  // DEAD: a scale multiplied and dropped; the ROM keeps the call.
  Math_MulFX(actor->scaleX, FIXED_ONE * 3 / 2);
  spark = FieldActor_Create(SPRITE_GOLDEN_RINGS, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = actor->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gWorldMap_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = actor;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->priority = 1;
    }
  }
  spark = FieldActor_Create(SPRITE_GOLDEN_RINGS, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
  if (spark != NULL) {
    spark->groundHeight = actor->groundHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gWorldMap_SparkScript);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedActor = actor;
    spark->layer = 1;
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 1);
      sprite->flags = 0;
    }
  }
}

/// Spawns a particle at a random spot around the object.
void WorldMap_SpawnParticle(struct Actor *actor) {
  s32 position[3];
  s32 angle;
  struct Actor *particle;
  struct FieldSprite *sprite;

  Iwram_MulFX(actor->scaleX, FIXED_ONE * 3 / 2);
  SET_VECTOR(position, actor->x.fixed, actor->y.fixed + toFIXED(2),
             actor->z.fixed);
  angle = Random16();
  FieldVector_AddPolarOffsetFX(angle * 20, Random16(), position);
  particle =
      (FieldActor_Create(SPRITE_GLINT, position[0], position[1], position[2]));
  if (particle != NULL) {
    particle->groundHeight = actor->groundHeight;
    sprite = particle->sprite;
    Actor_StartScript(particle, gWorldMap_ParticleScript);
    Sprite_StartAnimation(sprite, 2);
    particle->gravity = toFIXED(1) / 128;
    particle->scaleX = FIXED_ONE * 4 / 5;
    particle->scaleY = FIXED_ONE * 4 / 5;
    if (sprite != NULL)
      sprite->flags = 0;
  }
}
