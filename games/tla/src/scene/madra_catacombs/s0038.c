#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "iwram_call.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_020010ec(s32 layer, s32 x, s32 z);
s32 Func_020011d4(struct Actor *object);
s32 Func_020011f4(s32 layer, s32 shape, s32 height);

// One-field views: the terminator's read and the shape store are memory of their
// own to sched2, which sinks them past the cell stores; plain reads and stores
// keep their order. A struct is padded to 4 bytes here, so the pointers stay plain.
struct ListedActor {
  u16 id;
};

struct ShapeCell {
  u8 shape;
};

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
void Func_02000038(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (((struct ListedActor *)list)->id != 0xffff) {
    struct Actor *object = Actor_GetById(*list);
    u8 *layer = &object->terrainId;
    s32 shape = Func_020010ec(*layer, object->x, object->z);
    s32 level = Map_GetTerrainHeight(*layer, object->x, object->z);
    s32 index;
    u8 *cells;
    u8 *shapes;
    u8 newShape;
    u8 *grid;

    level >>= 19;
    index = Func_020011d4(object);
    cells = (u8 *)map->layers[*layer].cells;
    grid = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid);
    shapes =
        grid + (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    level += 4;
    newShape = Func_020011f4(*layer, shape, level);
    shapes += index;
    index <<= 2;
    ((struct ShapeCell *)shapes)->shape = newShape;
    cells += index;
    ((struct MapCell *)cells)->attributeB |= 192;
    cells += 0x200;
    ((struct MapCell *)cells)->attributeB |= 64;
    list++;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
