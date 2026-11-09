#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

s32 Func_02001508(struct Actor *object);
s32 Func_02001478(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020014c0(s32 id);
s32 Func_02001528(s32 layer, s32 shape, s32 height);
void Func_02001428(struct Actor *object, s32 mode);

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
      shape = Func_02001478(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      *slot = Func_02001528(object->terrainId, shape, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Func_02001428(object, 0);
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
