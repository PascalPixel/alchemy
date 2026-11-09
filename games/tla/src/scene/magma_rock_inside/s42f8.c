/// @file
/// Places the listed actors on their cells.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

void Func_02004da8(s32 id, s32 owner);
void Func_020042e0(s32 id);
s32 Func_02004ed0(struct Actor *object);
s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004df8(s32 id);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
void Func_020042f8(u16 *list) {
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

  Func_02004da8(8, gMainRoutineWork.currentOwner);
  Func_02004da8(9, gMainRoutineWork.currentOwner);
  Func_02004da8(10, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Func_020042e0(8);
  Func_020042e0(9);
  Func_020042e0(10);
  Actor_SetAnimation(9, 0);
  System_Wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  System_Wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_Get(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (Event_GetFlag(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02004ed0(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02004d20(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02004df8(id);
    cell->attributeB = 128;
    *slot = Func_02004ef0(object->terrainId, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  System_Wait(10);
}
