// Draft, differ 94 of 130: on entering, sets each listed pillar from its two
// flags (pushed: moved to its place; else its cell raised and its shape
// redrawn), then lifts the leader onto the ground under it. The same function
// is overlay 68F's Func_0200222c and 691's Func_02002d08.
// Measured against its copies: overlay 68F's Func_0200222c (differ 94 of 130),
// overlay 691's Func_02002d08 (differ 94 of 130).
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_020014a0(s32 id, s32 x, s32 z);
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

  if (list[0] != 0xffff) {
    do {
      u32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      struct MapCell *cells = map->layers[object->terrainId].cells;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Test(flag + 1)) {
        Func_020014a0(id, 0, 0);
      } else {
        s32 index = Func_02001508(object);
        s32 shape;
        s32 ground;

        cells += index;
        shape = Func_02001478(object->terrainId, object->x, object->z);
        ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
        Func_020014c0(id);
        cells->attributeB = 128;
        *(shapes + index) =
            Func_02001528(object->terrainId, shape, (ground >> 19) + 4);
        if (GameFlag_Test(flag))
          Func_02001428(object, 0);
      }
      list += 2;
    } while (list[0] != 0xffff);
  }
  leader = Actor_GetById(gSequenceParentWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
