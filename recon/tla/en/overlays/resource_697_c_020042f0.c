// Draft, differ 116 of 130: overlay 697's copy of the function drafted in
// resource_692_c_02003f44.c (overlay 692's Func_02003f44, differ 116 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"
#include "party_state.h"

extern u8 gMapShapeGrid[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_020058e0(s32 id, s32 a, s32 b);
s32 Func_02005a18(struct ObjectRuntime *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
u8 Func_02005a20(s32 layer, s32 shape, s32 height);
void Func_020057c8(struct ObjectRuntime *object, s32 mode);

void Func_020042f0(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object;
  struct MapCell *cells;
  u8 *shapes;
  u8 *cell;
  s32 id;
  s32 flag;
  s32 index;
  s32 shape;
  s32 height;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Object_GetById(id);
  flag = table[1];
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
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
      Func_020057c8(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  object = Object_GetById(gPartyState.currentOwner);
  height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
  if (object->y < height) {
    object->terrainHeight = height;
    object->y = height;
  }
}
