// ---- games/tla/src/scene/izumo_ruins/s0038.c ----
#include "../shared/map_cells.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "event_inline.h"
#include "field.h"
#include "module/field/object.h"
#include "module/scene/izumo_ruins.h"
#include "module/scene/scene.h"
#include "module/tla.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000038(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r0, r3, r7, r8, v0, r5, r6, r4, v1;
  s32 stack[1];

  /* sub (s32)stack, #4 */
  r0 = a0;
  r3 = FIELD_AT(a0, u16, 0);
  if ((FIELD_AT(a0, u16, 0)) == 0xffff)
    goto L_0200809c;
  r7 = r0;
L_0200804e:
  r8 = FIELD_AT(r7, u16, 0);
  v0 = ((s32)Actor_Get(r8));
  r5 = v0;
  r6 = *(s16 *)(r7 + 2);
  r4 = *(s16 *)(r7 + 4);
  FIELD_AT(r5, u8, 89) = 254 & (FIELD_AT(r5, u8, 89));
  stack[0] = r4;
  Actor_SetSpriteFlags(v0, 0);
  r4 = stack[0];
  r6 = r6 << 16;
  r4 = r4 << 16;
  r4 = (u32)r4 >> 16;
  r6 = (u32)r6 >> 16;
  r5 = r5 + 34;
  v1 = Func_02000128(r8, r6, r4, FIELD_AT(r5, u8, 0));
  r7 = r7 + 6;
  r0 = v1;
  r3 = FIELD_AT(r7, u16, 0);
  if ((FIELD_AT(r7, u16, 0)) != 0xffff)
    goto L_0200804e;
L_0200809c:
  /* add (s32)stack, #4 */
  return r0;
}
#undef FIELD_AT

// ---- games/tla/src/scene/izumo_ruins/s00a4.c ----
// The overlay code of scene/izumo_ruins between two shared objects, in ROM
// order.

#include "izumo_ruins.h"
#include "map.h"
#include "../shared/trial_road.h"

/* VIEW(s00a4): routine Func_02000128 as void Func_02000128(s32 id, u32 kind,
 * s32 flag, s32 layer); s0038 has s32 Func_02000128(); tried as the
 * definition's void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer), which
 * the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/izumo_ruins/s0038.c:102: conflicting types for
 * `Func_02000128'. */
extern void Actor_SetCellOpen(s32 id, u32 kind, s32 flag,
                              s32 layer) __asm__("Func_02000128");

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
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Func_02001508,
                           Actor_SelectAnimation)

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
      shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      *slot = Func_02001528(object->terrainId, shape, (ground >> 19) + 4);
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

void Func_020002f8(u16 *list) {
  struct Actor *leader;
  struct Actor *pillar;
  s32 position[3];

  if (Import_FieldTryJumpForward() != 0)
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

      Event_Start();
      if (!GameFlag_Test(flag)) {
        Sound_Play(125);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Actor_SelectAnimation(pillar, 0);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        Func_02000974(0);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags = &leader->motionFlags;

        *flags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Actor_SelectAnimation(pillar, 2);
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
      Event_End();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

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
      Event_Start();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        System_Wait(8);
        Actor_SelectAnimation(object, 2);
        Actor_SetCellShapeAt(-4, OBJECT_BYTE(object, 34), id);
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
      Event_End();
      break;
    }
  }
}

void Func_0200056c(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Func_020014c0(id);
}

void Actor_MoveToObject(s32 id, s32 target);

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *grid;

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
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

      kind = MapGrid_GetTerrainKind(actor->terrainId, actor->x, actor->z);
      ground = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      // Read through the actor, not actor->terrainId: the native reloads it
      // after the cell store.
      *shape = Func_02001528(actor->terrainId, kind, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Actor_CopyGraphics(id, 9);
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

  if (Import_FieldTryJumpForward() != 0)
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

      Event_Start();
      if (!GameFlag_Test(flag)) {
        Actor_SelectAnimation(helper, 1);
        Func_020006ec(helper, pillar);
        wait(1);
        Sound_Play(125);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Actor_SelectAnimation(helper, 0);
        Actor_CopyGraphics(id, 9);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        Func_02000974(0);
        Func_020006ec(pillar, helper);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(helper, 0);
        Func_020006ec(helper, pillar);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Actor_SelectAnimation(helper, 2);
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
      Event_End();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
