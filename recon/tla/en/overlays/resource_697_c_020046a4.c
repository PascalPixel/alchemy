// Draft, differ 98 of 150: overlay 697's copy of the function drafted in
// resource_692_c_020042f8.c (overlay 692's Func_020042f8, differ 98 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "sequence_parent_work.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_020058f0(s32 id, s32 owner);
void Func_0200468c(s32 id);
void Task_Wait(s32 frames);
void Func_020058e0(s32 id, s32 a, s32 b);
s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
u8 Func_02005a20(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(s32 id, s32 from);

void Func_020046a4(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  struct MapCell *cells;
  u8 *shapes;
  u8 *cell;
  s32 id;
  s32 flag;
  s32 index;
  s32 shape;
  s32 height;

  Func_020058f0(8, gSequenceParentWork.currentOwner);
  Func_020058f0(9, gSequenceParentWork.currentOwner);
  Func_020058f0(10, gSequenceParentWork.currentOwner);
  Task_Wait(1);
  Func_0200468c(8);
  Func_0200468c(9);
  Func_0200468c(10);
  Actor_SetModeById(9, 0);
  Task_Wait(1);
  Func_020058e0(8, 0, 0);
  Func_020058e0(9, 0, 0);
  Func_020058e0(10, 0, 0);
  Task_Wait(1);
  goto test;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->movementState = 3;
  if (GameFlag_Test(flag + 1)) {
    Func_020058e0(id, 0, 0);
  } else {
    index = Func_02005a18(object);
    cell = (u8 *)&cells[index];
    shape = Func_02005848(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02005958(id);
    cell[3] = 128;
    shapes[index] = Func_02005a20(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  table += 2;
test:
  if (*table != 0xffff)
    goto next;
  Task_Wait(10);
}
