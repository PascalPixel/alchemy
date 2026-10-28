// Draft, differ 116 of 130: the trap table's setup (shared with overlay 697 and
// the other trap maps): a trap whose second flag is set is put away; any other
// marks its cell (attribute byte 128), writes a shape for its height into the
// shape grid, and takes mode 0 when its first flag is set. Then the leader is
// lifted onto the ground if it stands below it. A goto loop because the
// native rebuilds the end marker for the test at the bottom. Rough: the native
// keeps the id and flag on the stack and the cells in r5.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "adventure.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02004da0(s32 id, s32 a, s32 b);
s32 Func_02004ed0(struct Actor *object);
s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004df8(s32 id);
u8 Func_02004ef0(s32 layer, s32 shape, s32 height);
void Func_02004c98(struct Actor *object, s32 mode);

void Func_02003f44(const u16 *table) {
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

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
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
      Func_02004c98(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  object = Actor_GetById(gAdventure.currentOwner);
  height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
  if (object->y < height) {
    object->terrainHeight = height;
    object->y = height;
  }
}
