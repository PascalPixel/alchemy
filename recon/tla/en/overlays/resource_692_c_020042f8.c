// Draft, differ 98 of 150: the trap table's setup for maps with objects 8 to 10
// (shared with overlay 697 and the other trap maps): those three are placed and
// put away first; then each trap whose second flag is set is put away, and any
// other marks its cell, writes a shape for its height into the shape grid and,
// when its first flag is set, copies object 9's look. Rough draft, the loop as
// in Func_02003f44.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "sequence_parent_work.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02004da8(s32 id, s32 owner);
void Func_020042e0(s32 id);
void wait(s32 frames);
void Func_02004da0(s32 id, s32 a, s32 b);
s32 Func_02004ed0(struct Actor *object);
s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004df8(s32 id);
u8 Func_02004ef0(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(s32 id, s32 from);

void Func_020042f8(const u16 *table) {
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

  Func_02004da8(8, gSequenceParentWork.currentOwner);
  Func_02004da8(9, gSequenceParentWork.currentOwner);
  Func_02004da8(10, gSequenceParentWork.currentOwner);
  wait(1);
  Func_020042e0(8);
  Func_020042e0(9);
  Func_020042e0(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02004da0(8, 0, 0);
  Func_02004da0(9, 0, 0);
  Func_02004da0(10, 0, 0);
  wait(1);
  goto test;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->movementState = 3;
  if (GameFlag_Test(flag + 1)) {
    Func_02004da0(id, 0, 0);
  } else {
    index = Func_02004ed0(object);
    cell = (u8 *)&cells[index];
    shape = Func_02004d20(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02004df8(id);
    cell[3] = 128;
    shapes[index] = Func_02004ef0(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  table += 2;
test:
  if (*table != 0xffff)
    goto next;
  wait(10);
}
