/// @file
/// The overlay code of scene/izumo_ruins between two shared objects, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"
#include "izumo_ruins.h"
#include "map.h"
#include "main_routine_work.h"
#include "../trial_road/trial_road.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer);

// Finds the current target's table row and shrinks it.
void Func_020000a4(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
  // A goto loop: as a do-while loop, 42 of 61 instructions differ in tla-en.
top:
  {
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
        Func_02000128(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02001508(object);
  if (Event_GetFlag(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02001428(object, 2);
      break;
    case 1:
      Func_02001428(object, 4);
      break;
    case 2:
      Func_02001428(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02001428(object, 1);
      break;
    case 1:
      Func_02001428(object, 3);
      break;
    case 2:
      Func_02001428(object, 5);
      break;
    }
  }
}

extern u8 gMapShapeGrid[];

s32 Func_02001478(s32 layer, s32 x, s32 z);
s32 Func_02001528(s32 layer, s32 shape, s32 height);

void Func_020001d0(u16 *list) {
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
      s32 index = Func_02001508(object);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = Func_02001478(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      *slot = Func_02001528(object->terrainId, shape, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Func_02001428(object, 0);
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

s32 Func_020014d8(void);
struct Actor *Func_02001530(s32 *position, s32 flags);
void Func_02000c4c(s32 level, s32 layer);

void Func_020002f8(u16 *list) {
  struct Actor *leader;
  struct Actor *pillar;
  s32 position[3];

  if (Func_020014d8() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  pillar = Func_02001530(position, 0);
  if (pillar != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02001488();
      if (!GameFlag_Test(flag)) {
        Sound_Play(125);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        wait(2);
        Func_02001428(pillar, 0);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        Func_02000974(0);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags = &leader->motionFlags;

        *flags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02001428(pillar, 2);
        Func_02000c4c(-2, pillar->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, pillar->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(pillar, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02001490();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void Func_0200048c(const void *data, s32 id) {
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
      Func_02001488();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        System_Wait(8);
        Func_02001428(object, 2);
        Func_02000bc0(-4, OBJECT_BYTE(object, 34), id);
        Func_02000974(1);
        System_Wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        System_Wait(8);
        FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
        System_Wait(2);
        Event_PlaySound(188);
        Func_02000aa8();
        System_Wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Func_02001490();
      break;
    }
  }
}

void Func_0200056c(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Func_020014c0(id);
}

void Func_020014a8(s32 id, s32 target);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *grid;

  Func_020014a8(8, gMainRoutineWork.currentOwner);
  Func_020014a8(9, gMainRoutineWork.currentOwner);
  Func_020014a8(10, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
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
      s32 index = Func_02001508(actor);
      struct MapCell *cell = cells + index;
      u8 *shape = shapes + index;
      s32 kind, ground;

      kind = Func_02001478(actor->terrainId, actor->x, actor->z);
      ground = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      // Read through the actor, not actor->terrainId: the native reloads it
      // after the cell store.
      *shape = Func_02001528(actor->terrainId, kind, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
  System_Wait(10);
}

void Func_020006ec(struct Actor *object, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void Func_02000708(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *pillar;
  s32 position[3];

  if (Func_020014d8() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  pillar = Func_02001530(position, 0);
  if (pillar != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02001488();
      if (!GameFlag_Test(flag)) {
        Func_02001428(helper, 1);
        Func_020006ec(helper, pillar);
        wait(1);
        Sound_Play(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(2);
        Func_02001428(helper, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_02000974(0);
        Func_020006ec(pillar, helper);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02001428(helper, 0);
        Func_020006ec(helper, pillar);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02001428(helper, 2);
        Func_02000c4c(-2, pillar->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, pillar->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02001490();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
