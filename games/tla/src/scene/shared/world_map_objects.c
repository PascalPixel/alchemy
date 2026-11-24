/// @file
/// The World Map's placed actors, which both the World Map's and the staff
/// credits' overlays keep a copy of, included as text: the exit chosen by the
/// side the leader approaches a place from, the task that shows the places'
/// actors coming into view and releases those leaving it, and the sparks and
/// particles about the ship.

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
#include "system/main_routine_state.h"
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/flag_call.h"
#include "field/field_sequence.h"

extern struct Actor *gWorldMap_PlacedActors[];

/// Unused: chooses a passage exit from the leader's east-west approach:
/// exitBefore while the leader is west of the place's actor, else exitAfter.
UNUSED void WorldMap_SelectExitByX(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct FieldState *state;
  struct Actor *area;
  struct Actor *leader;

  area = gWorldMap_PlacedActors[trigger - STEP_TRIGGER_AREA_BASE];
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  state = Ram_HeapSlots->fieldState;
  if (leader->x.fixed < area->x.fixed) {
    state->exit = exitBefore;
  } else {
    state->exit = exitAfter;
  }
  Event_PlaySound(SE_MAP_EXIT);
}

/// Chooses a passage exit from the leader's north-south approach.
void WorldMap_SelectExitByZ(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct FieldState *state;
  struct Actor *area;
  struct Actor *leader;

  area = gWorldMap_PlacedActors[trigger - STEP_TRIGGER_AREA_BASE];
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  state = Ram_HeapSlots->fieldState;
  if (leader->z.fixed < area->z.fixed) {
    state->exit = exitBefore;
  } else {
    state->exit = exitAfter;
  }
  Event_PlaySound(SE_MAP_EXIT);
}

extern s32 gWorldMap_PlacedActorScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);
void Actor_Release(struct Actor *actor);

/// Each frame: creates the actor of each place (FlagEntries_GetTable) whose
/// flag shows it as it comes within reach of the camera, releases those that
/// leave it, and raises a place's step trigger when the leader touches its
/// actor; in Sand's form on the World Map it asks for Sand again, at no PP,
/// instead.
void WorldMap_UpdatePlacedActors(void) {
  const struct FlagEntry *placement = FlagEntries_GetTable();
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  s32 leaderReach = ((struct Sprite *)leader->sprite)->scale * leader->radius;
  u32 i;
  s32 next;
  FIXED left, right, top, bottom;
  struct Actor **slot;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = state->camera;
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
  slot = objects;
  /* FAKEMATCH(barrier): an empty asm on leader and state that reads next. */
  asm("" : "+g"(leader), "+r"(state) : "g"(next));
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
      else if (flag & CONDITION_FLAG_SET)
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
        if (gMainRoutineState.ownerOverride == 0) {
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
              if (gMainRoutineState.movementMode == LEADER_MOVEMENT_WORLD_SAND)
                state->psynergy = ACT_PACKED | ABILITY_SAND;
              else
                state->stepTrigger = i + STEP_TRIGGER_AREA_BASE;
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
    if (i >= MAP_SCREEN_PLACE_COUNT)
      break;
    next = placement->flag;
  }
}

/// Unused: unless a menu has suspended the actors, queues the World Map's
/// backgrounds lightened toward white.
UNUSED void WorldMap_QueueBlend(void) {
  if (Ram_HeapSlots->actorSystem->suspended == 0) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                            BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 12, IO_WRITE_HALFWORD);
  }
}

/// Clears the places' actors and starts WorldMap_UpdatePlacedActors.
void WorldMap_StartPlacedActors(void) {
  Iwram_ClearWords(gWorldMap_PlacedActors,
                   MAP_SCREEN_PLACE_COUNT * sizeof(struct Actor *));
  Task_AddFrame(WorldMap_UpdatePlacedActors, TASK_PASS_FRAME);
}

/// Unused: the cell index of a point in 16.16 pixels.
UNUSED s32 WorldMap_GetCellIndex(FIXED x, FIXED z) {
  return x / toFIXED(MAPGRID_CELL_PIXELS) +
         (z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE;
}

/// The frames a ship's spark rises for.
#define SPARK_RISE_FRAMES 32

/// A spark's rise (gWorldMap_SparkScript): rises a pixel a frame over the actor
/// it watches, growing and shrinking again over SPARK_RISE_FRAMES; returns 0
/// when done.
s32 WorldMap_RiseFromAnchor(struct Actor *actor) {
  struct Actor *parent = actor->watchedActor;
  s32 step = ++actor->timer;
  FIXED scale;

  if (step >= SPARK_RISE_FRAMES)
    return 0;
  scale = Iwram_MulFX(Math_Sin(step * (DEGtoANG(180) / SPARK_RISE_FRAMES)),
                      FIXED_ONE * 5 / 4);
  actor->scaleX = scale;
  actor->scaleY = scale;
  actor->x.fixed = parent->x.fixed;
  actor->y.fixed += toFIXED(1);
  actor->z.fixed = parent->z.fixed;
  return 1;
}

extern s32 gWorldMap_SparkScript[];
extern u32 gWorldMap_ParticleScript[];

/// Spawns a pair of rising sparks at the actor, as The Broken Seal's
/// MercuryLighthouse_SpawnSparkPair does at a spawn point.
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

/// Spawns a particle at a random spot near the actor.
void WorldMap_SpawnParticle(struct Actor *actor) {
  s32 position[3];
  FIXED spread;
  struct Actor *particle;
  struct FieldSprite *sprite;

  // DEAD: a scale multiplied and dropped; the ROM keeps the call.
  Iwram_MulFX(actor->scaleX, FIXED_ONE * 3 / 2);
  SET_VECTOR(position, actor->x.fixed, actor->y.fixed + toFIXED(2),
             actor->z.fixed);
  spread = Random16();
  FieldVector_AddPolarOffsetFX(spread * 20, Random16(), position);
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

#undef SPARK_RISE_FRAMES
