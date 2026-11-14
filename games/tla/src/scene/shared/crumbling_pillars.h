/// @file
/// The crumbling and pounded pillars of the Izumo Ruins and Dehkan Plateau:
/// lists of (actor, flag) or (actor, kind, flag) entries ended by 0xffff,
/// placed from their flags and stepped on, struck or pounded until they fall.
/// Places each pillar of a list of three-halfword entries (actor, kind,
/// flag) ended by 0xffff, its collision bit cleared, raised or sunk by its
/// flag (Actor_SetCellOpen).
void Pillars_Place(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
  do {
    u32 id = entry[0];
    struct Actor *object = Actor_Get(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  } while (entry[0] != 0xffff);
}

// Finds the current target's table row and shrinks it.
void Pillars_Pound(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
  do {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = Q16_ONE;
      }
      return;
    }
    list += 3;
  } while (*list != 0xffff);
}

#include "actor_set_cell_open.h"

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void CrumblingPillars_Place(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *object = Actor_Get(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(object);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Actor_DisableById(id);
      cell->attributeB = 128;
      *slot = MapCollision_FindOrAddTag(object->terrainId, shape,
                                        (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Actor_SelectAnimation(object, 0);
    }
    list += 2;
  }
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}

void CrumblingPillars_Step(u16 *list) {
  struct Actor *leader;
  struct Actor *pillar;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
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
        Leader_SetCellShapeAt(-2, pillar->terrainId);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->terrainId);
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
  struct Actor *object = Actor_Get(id);
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
        Actor_SelectAnimation(object, 2);
        Actor_SetCellShape(-4, object->terrainId, id);
        Rubble_Spray(1);
        System_Wait(16);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        System_Wait(8);
        FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
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
  Actor_Get(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 target);

void CrumblingPillars_PlaceWithStandIns(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *grid;

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
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
    struct MapCell *cells = map->layers[actor->terrainId].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->unk5c[0] = 3;
    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(actor);
      struct MapCell *cell = cells + index;
      u8 *shape = shapes + index;
      s32 kind, ground;

      kind = MapGrid_GetTerrainKind(actor->terrainId, actor->x, actor->z);
      ground = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z);

      Actor_DisableById(id);
      cell->attributeB = 128;
      *shape =
          MapCollision_FindOrAddTag(actor->terrainId, kind, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Actor_CopyGraphics(id, 9);
    }
    list += 2;
  }
  System_Wait(10);
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void CrumblingPillars_StepWithStandIn(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *pillar;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
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
        Leader_SetCellShapeAt(-2, pillar->terrainId);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->terrainId);
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
