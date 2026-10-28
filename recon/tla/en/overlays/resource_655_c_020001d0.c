// Draft, differ 94 of 130: the Dehkan Plateau prepare step for a list of
// (actor, flag) pairs: an actor whose second flag is set is put away,
// otherwise its cell is marked and the shape grid raised under it. Left:
// register choice and the frame (the native keeps the list in r11).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003ba0(struct Actor *object, s32 mode);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003cc0(s32 id);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);

void Func_020001d0(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (*list != 0xffff) {
    s32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    struct MapCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    if (GameFlag_Test(flag + 1)) {
      Func_02003c68(id, 0, 0);
    } else {
      s32 index = Func_02003db0(object);
      u8 *cell = (u8 *)(cells + index);
      s32 shape = Func_02003c10(object->terrainId, object->x, object->z);
      s32 h = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Func_02003cc0(id);
      cell[3] = 128;
      shapes[index] = Func_02003dd0(object->terrainId, shape, (h >> 19) + 4);
      if (GameFlag_Test(flag))
        Func_02003ba0(object, 0);
    }
    list += 2;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
