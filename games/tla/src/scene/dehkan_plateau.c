/// @file
/// The scene code of overlay 0x655, in ROM order.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "field.h"
#include "map.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Func_02000038(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  id = spec[0];
  object = Actor_GetById(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  Actor_SetSingleChildFlags(object, 0);
  Func_02000128(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}

void Func_020000a4(const void *data, s32 mode, s32 timer) {
  const u16 *spec = data;
  struct EffectWork1a *work = Ram_HeapSlots->effectWork;
  while (*spec != 0xffff) {
    u32 value = *spec;
    s32 active = work->active;

    if (active == value) {
      struct Actor *object = Actor_GetById(active);
      s32 kind;
      s32 flag;

      spec++;
      kind = spec[0];
      flag = spec[1];
      if (timer <= 7)
        object->scaleY += -0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02000128(active, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
    spec += 3;
  }
}

void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer) {
  struct Actor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02003db0(object);
  if (GameFlag_Test(flag)) {
    /* FAKEMATCH(temporary): the flags read into a local before the cell store
     * and ORed into the register that holds 2, as the native does; as one |=,
     * which builds the 0 for the cell before the read, 8 of 75 instructions
     * differ in tla-en. */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell->collisionCode = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (mode) {
    case 0:
      Func_02003ba0(object, 2);
      break;
    case 1:
      Func_02003ba0(object, 4);
      break;
    case 2:
      Func_02003ba0(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (mode) {
    case 0:
      Func_02003ba0(object, 1);
      break;
    case 1:
      Func_02003ba0(object, 3);
      break;
    case 2:
      Func_02003ba0(object, 5);
      break;
    }
  }
}

#include "main_routine_work.h"
#include "field_sprite.h"
#include "iwram_call.h"

void Func_020001d0(const void *data) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  const u16 *list = data;
  struct Actor *leader;
  s32 height;

  if (list[0] != 0xffff) {
    do {
      s32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      s32 nextFlag = flag + 1;
      struct MapCell *cells = map->layers[object->terrainId].cells;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Test(nextFlag)) {
        Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
      } else {
        s32 index = Func_02003db0(object);
        struct MapCell *cell = &cells[index];
        u8 *shapeCell = &shapes[index];
        s32 shape = Func_02003c10(object->terrainId, object->x, object->z);
        s32 terrainHeight =
            Map_GetTerrainHeight(object->terrainId, object->x, object->z);

        Func_02003cc0(id);
        cell->attributeB = 128;
        *shapeCell =
            Func_02003dd0(object->terrainId, shape, (terrainHeight >> 19) + 4);
        if (GameFlag_Test(flag))
          Func_02003ba0(object, 0);
      }
      list += 2;
    } while (list[0] != 0xffff);
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}

void Func_020002f8(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003dd8(position, 0);
  if (target != NULL) {
  goto test;
next:
  {
    s32 id = list[0];
    s32 flag;

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    flag = list[1];
    Event_Start();
    if (GameFlag_Test(flag) == 0) {
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(2);
      Func_02003ba0(target, 0);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      Func_02000974(0);
      GameFlag_SetBit(flag);
    } else {
      s32 done = flag + 1;
      s32 result = GameFlag_Test(done);

      if (result == 0) {
        leader->motionFlags = result;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02003ba0(target, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(done);
      }
    }
    Event_End();
    return;
  }
test:
  if (*list != 0xffff)
    goto next;
  }
}

/// Runs the event of the listed object `id` once: the list holds (object id,
/// flag) pairs ended by 0xffff, and unless the flag after the entry's own is
/// set, cues 185 and 188 play around Func_02000bc0 and both flags are set. The
/// twin of overlay 67f's Func_0200048c.
void Func_0200048c(u16 *list, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  if (list[0] == 0xffff)
    return;
loop:
  if (list[0] != id) {
    list += 2;
    goto test;
  }
  flag = list[1];
  Event_Start();
  if (!GameFlag_Test(flag + 1)) {
    Sound_Play(185);
    MapGrid_SetShake(0x10000, 0x40000, 0x10000);
    Func_02000974(0);
    wait(8);
    Func_02003ba0(object, 2);
    Func_02000bc0(-4, object->terrainId, id);
    Func_02000974(1);
    wait(16);
    MapGrid_SetShake(-1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Sound_Play(188);
    Func_02000aa8();
    wait(20);
    GameFlag_SetBit(flag);
    GameFlag_SetBit(flag + 1);
  }
  Event_End();
  return;
test:
  if (list[0] != 0xffff)
    goto loop;
}

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_02002180(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];


void Func_0200056c(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Func_02003cc0(id);
}

void Actor_MoveToObject(s32 id, s32 target);

/// Places actors 8 to 10 at the leader, then walks a list of (actor, flag)
/// pairs ended by 0xffff: an actor whose flag + 1 is set is put back with
/// Actor_SetHorizontalPositionWithTerrain; any other has the cell under it
/// marked and its shape grid entry set from the terrain there, and copies
/// attributes 9 when its flag is set. The twin of overlay 67f's Func_02000584.
void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // Holds the grid's address for the whole function.
  u8 *grid = gMapShapeGrid;

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  wait(1);
  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *actor = Actor_GetById(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    u8 *terrain = &actor->terrainId;
    struct MapCell *cells = map->layers[*terrain].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->unk5c[0] = 3;
    if (GameFlag_Test(next)) {
      Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
    } else {
      s32 index = Func_02003db0(actor);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = Func_02003c10(*terrain, actor->x, actor->z);
      ground = Map_GetTerrainHeight(*terrain, actor->x, actor->z);

      Func_02003cc0(id);
      cell->attributeB = 128;
      *slot = Func_02003dd0(actor->terrainId, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
  wait(10);
}

void Func_020006ec(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void Actor_StartScript(struct Actor *object, const void *script);

void Func_02000708(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003dd8(position, 0);
  if (target != NULL) {
  goto test;
next:
  {
    s32 id = list[0];
    s32 flag;

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    flag = list[1];
    Event_Start();
    if (GameFlag_Test(flag) == 0) {
      Func_02003ba0(helper, 1);
      Func_020006ec(helper, target);
      wait(1);
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(2);
      Func_02003ba0(helper, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_02000974(0);
      Func_020006ec(target, helper);
      wait(1);
      GameFlag_SetBit(flag);
    } else {
      s32 done = flag + 1;
      s32 result = GameFlag_Test(done);

      if (result == 0) {
        Func_02003ba0(helper, 0);
        Func_020006ec(helper, target);
        wait(1);
        leader->motionFlags = result;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02003ba0(helper, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(done);
      }
    }
    Event_End();
    return;
  }
test:
  if (*list != 0xffff)
    goto next;
  }
}

#include "../field/field_effect/advance_motion_priority.c"
