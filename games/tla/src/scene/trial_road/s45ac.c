#include "trial_road.h"
#include "event_inline.h"

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void Func_02005880(void);
void Func_02005888(void);
void Func_02004a94(s32 value);
void Func_02004bc8(void);
void Func_02004ce0(s32 a, s32 b, s32 c);

void Func_020045ac(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Func_02005880();
      if (!GameFlag_Test(flag + 1)) {
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        wait(8);
        Func_020057c8(object, 2);
        Func_02004ce0(-4, OBJECT_BYTE(object, 34), id);
        Func_02004a94(1);
        wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02004bc8();
        wait(20);
        GameFlag_SetBit(flag);
        GameFlag_SetBit(flag + 1);
      }
      Func_02005888();
      break;
    }
  }
}

void Func_02005958(s32 id);

void Func_0200468c(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Func_02005958(id);
}

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

void Func_020058f0(s32 id, s32 owner);
s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Func_02005a20(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
void Func_020046a4(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Func_020058f0(8, gMainRoutineWork.currentOwner);
  Func_020058f0(9, gMainRoutineWork.currentOwner);
  Func_020058f0(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_0200468c(8);
  Func_0200468c(9);
  Func_0200468c(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_GetById(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Test(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02005a18(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02005848(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02005958(id);
    cell->attributeB = 128;
    *slot = Func_02005a20(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}

void Func_0200480c(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}
