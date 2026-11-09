/// @file
/// Places the listed actors on their cells.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

void Func_020058f0(s32 id, s32 owner);
void Func_0200468c(s32 id);
s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
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
