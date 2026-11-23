/// @file
/// The crumbling and pounded pillars of the Izumo Ruins and Dehkan Plateau:
/// lists of (actor, flag) or (actor, kind, flag) entries ended by
/// ACTOR_LIST_END, placed from their flags and stepped on, struck or pounded
/// until they fall.
#include "pillar_place.h"

#include "pound_sink_target.h"

#include "actor_set_cell_open.h"

#include "pillar_stand_in.h"

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

/// Places each crumbling pillar of an (actor, flag) list: one that has given
/// way is moved out of sight; the others stand as their cell's ground, 32
/// pixels above the floor, which the leader jumps onto (MAP_CELL_JUMP), and a
/// cracked one shows its cracks. A leader below the ground where he stands is
/// raised onto it.
void CrumblingPillar_Place(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *leader;
  s32 height;

  while (list[0] != ACTOR_LIST_END) {
    u32 id = list[0];
    struct Actor *actor = Actor_Get(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[actor->layer].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(actor);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape =
          MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
      ground =
          Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);

      Actor_DisableById(id);
      cell->attributeB = MAP_CELL_JUMP;
      *slot = MapCollision_FindOrAddTag(actor->layer, shape,
                                        CELL_HEIGHT_STEPS(ground) + 4);
      if (Event_GetFlag(flag))
        Actor_SelectAnimation(actor, 0);
    }
    list += 2;
  }
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  height =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < height) {
    leader->groundHeight = height;
    leader->y.fixed = height;
  }
}

/// Stepping onto a crumbling pillar: the first time it cracks, the second time
/// it gives way with the leader.
void CrumblingPillar_Step(u16 *list) {
  struct Actor *leader;
  struct Actor *pillar;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  pillar = Actor_FindAtCell(position, 0);
  if (pillar == 0)
    return;
  while (list[0] != ACTOR_LIST_END) {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      continue;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Get(flag)) {
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(pillar, 0);
        Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
        Rubble_Spray(0);
        GameFlag_Set(flag);
      } else if (!GameFlag_Get(flag + 1)) {
        u8 *flags = &leader->motionFlags;

        *flags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        wait(8);
        Actor_SelectAnimation(pillar, 2);
        Leader_SetCellShapeAt(-2, pillar->layer);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        wait(5);
        Actor_SetPositionAndResetMotion(pillar, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        Rubble_Burst();
        wait(20);
        GameFlag_Set(flag + 1);
      }
      Event_End();
      return;
    }
  }
}

/// A crumbling pillar named in an (actor, flag) list gives way with a shake,
/// dust and a thump.
void CrumblingPillar_Collapse(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *actor = Actor_Get(id);
  s32 flag;

  while (entry->actor != ACTOR_LIST_END) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Event_Start();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        System_Wait(8);
        Actor_SelectAnimation(actor, 2);
        Actor_SetCellShape(-4, actor->layer, id);
        Rubble_Spray(1);
        System_Wait(16);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        System_Wait(8);
        FieldActor_SetPositionAndResetMotion(actor, 0, 0, 0);
        System_Wait(2);
        Event_PlaySound(SE_THUMP);
        Rubble_Burst();
        System_Wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Event_End();
      break;
    }
  }
}

/// Keeps an actor's sprite picture loaded even out of sight, and stops it
/// moving, colliding and showing (Actor_DisableById).
void CrumblingPillar_Prepare(s32 id) {
  Actor_Get(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}

void Actor_PlaceOnActor(s32 id, s32 target);

/// As CrumblingPillar_Place, in a room whose pillars share one sprite: first
/// loads the stand-in's and the two pictures' sprites beside the leader, the
/// cracked one showing its cracks, then puts them out of sight; a cracked
/// pillar takes the cracked picture.
void CrumblingPillar_PlaceWithStandIns(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u8 *grid;

  Actor_PlaceOnActor(ACTOR_PILLAR_STAND_IN, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_CRACKED_PILLAR_SPRITE,
                     gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_WHOLE_PILLAR_SPRITE, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  CrumblingPillar_Prepare(ACTOR_PILLAR_STAND_IN);
  CrumblingPillar_Prepare(ACTOR_CRACKED_PILLAR_SPRITE);
  CrumblingPillar_Prepare(ACTOR_WHOLE_PILLAR_SPRITE);
  Actor_SetAnimation(ACTOR_CRACKED_PILLAR_SPRITE, 0);
  System_Wait(1);
  Actor_SetPosition(ACTOR_PILLAR_STAND_IN, 0, 0);
  Actor_SetPosition(ACTOR_CRACKED_PILLAR_SPRITE, 0, 0);
  Actor_SetPosition(ACTOR_WHOLE_PILLAR_SPRITE, 0, 0);
  System_Wait(1);
  grid = gMapShapeGrid;
  while (list[0] != ACTOR_LIST_END) {
    u32 id = list[0];
    struct Actor *actor = Actor_Get(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[actor->layer].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->retainSpriteCache = 3;
    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(actor);
      struct MapCell *cell = cells + index;
      u8 *shape = shapes + index;
      s32 kind, ground;

      kind =
          MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
      ground =
          Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);

      Actor_DisableById(id);
      cell->attributeB = MAP_CELL_JUMP;
      *shape = MapCollision_FindOrAddTag(actor->layer, kind,
                                         CELL_HEIGHT_STEPS(ground) + 4);
      if (Event_GetFlag(flag))
        Actor_CopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
    }
    list += 2;
  }
  System_Wait(10);
}

#include "actor_take_place_of.h"

/// As CrumblingPillar_Step, in a room whose pillars share one sprite: the
/// stand-in takes the pillar's place to crack or give way, and a cracked pillar
/// takes the cracked picture.
void CrumblingPillar_StepWithStandIn(u16 *list) {
  struct Actor *leader;
  struct Actor *standIn;
  struct Actor *pillar;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  standIn = Actor_GetById(ACTOR_PILLAR_STAND_IN);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  pillar = Actor_FindAtCell(position, 0);
  if (pillar == 0)
    return;
  while (list[0] != ACTOR_LIST_END) {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      continue;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Get(flag)) {
        Actor_SelectAnimation(standIn, 1);
        Actor_TakePlaceOf(standIn, pillar);
        wait(1);
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_PILLAR_STAND_IN),
                             SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(standIn, 0);
        Actor_CopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_PILLAR_STAND_IN),
                             SPRITE_COLOR_NORMAL);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_PILLAR_STAND_IN),
                             SPRITE_COLOR_WHITE);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_PILLAR_STAND_IN),
                             SPRITE_COLOR_NORMAL);
        Rubble_Spray(0);
        Actor_TakePlaceOf(pillar, standIn);
        wait(1);
        GameFlag_Set(flag);
      } else if (!GameFlag_Get(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(standIn, 0);
        Actor_TakePlaceOf(standIn, pillar);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        wait(8);
        Actor_SelectAnimation(standIn, 2);
        Leader_SetCellShapeAt(-2, pillar->layer);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        wait(5);
        Actor_SetPositionAndResetMotion(standIn, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        Rubble_Burst();
        wait(20);
        GameFlag_Set(flag + 1);
      }
      Event_End();
      return;
    }
  }
}
