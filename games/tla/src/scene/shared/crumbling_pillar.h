/// @file
/// The crumbling and pounded pillars of the Izumo Ruins and Dehkan Plateau:
/// lists of (actor, flag) or (actor, kind, flag) entries ended by 0xffff,
/// placed from their flags and stepped on, struck or pounded until they fall.
#include "pillar_place.h"

#include "pound_sink_target.h"

#include "actor_set_cell_open.h"

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void CrumblingPillar_Place(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *leader;
  s32 height;

  while (list[0] != 0xffff) {
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
      cell->attributeB = 128;
      *slot =
          MapCollision_FindOrAddTag(actor->layer, shape, (ground >> 19) + 4);
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
  while (list[0] != 0xffff) {
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
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Actor_SelectAnimation(pillar, 0);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
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
        *flags = 3;
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

void CrumblingPillar_Collapse(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *actor = Actor_Get(id);
  s32 flag;

  while (entry->actor != 0xffff) {
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

void CrumblingPillar_Prepare(s32 id) {
  Actor_Get(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 target);

void CrumblingPillar_PlaceWithStandIns(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u8 *grid;

  Actor_MoveToObject(8, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(9, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(10, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  CrumblingPillar_Prepare(8);
  CrumblingPillar_Prepare(9);
  CrumblingPillar_Prepare(10);
  Actor_SetAnimation(9, 0);
  System_Wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  System_Wait(1);
  grid = gMapShapeGrid;
  while (list[0] != 0xffff) {
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
      cell->attributeB = 128;
      *shape =
          MapCollision_FindOrAddTag(actor->layer, kind, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Actor_CopyGraphics(id, 9);
    }
    list += 2;
  }
  System_Wait(10);
}

void Actor_TakePlaceOf(struct Actor *actor, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(actor, target->x.fixed, target->y.fixed,
                                       target->z.fixed);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void CrumblingPillar_StepWithStandIn(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *pillar;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  helper = Actor_GetById(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  pillar = Actor_FindAtCell(position, 0);
  if (pillar == 0)
    return;
  while (list[0] != 0xffff) {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      continue;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Get(flag)) {
        Actor_SelectAnimation(helper, 1);
        Actor_TakePlaceOf(helper, pillar);
        wait(1);
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Actor_SelectAnimation(helper, 0);
        Actor_CopyGraphics(id, 9);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        Rubble_Spray(0);
        Actor_TakePlaceOf(pillar, helper);
        wait(1);
        GameFlag_Set(flag);
      } else if (!GameFlag_Get(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(helper, 0);
        Actor_TakePlaceOf(helper, pillar);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        wait(8);
        Actor_SelectAnimation(helper, 2);
        Leader_SetCellShapeAt(-2, pillar->layer);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
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
