/// @file
/// Places actors 8 to 10 at the leader, then walks a list of (actor, flag)
/// pairs ended by 0xffff: an actor whose flag + 1 is set is put back with
/// Func_02003c68; any other has the cell under it marked and its shape grid
/// entry set from the terrain there, and copies attributes 9 when its flag is
/// set. The twin of overlay 67f's Func_02000584.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003db0(struct Actor *actor);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // Holds the grid's address for the whole function.
  u8 *grid = gMapShapeGrid;

  Func_02003c78(8, gMainRoutineWork.currentOwner);
  Func_02003c78(9, gMainRoutineWork.currentOwner);
  Func_02003c78(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02003c68(8, 0, 0);
  Func_02003c68(9, 0, 0);
  Func_02003c68(10, 0, 0);
  wait(1);
  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *actor = Actor_GetById(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    u8 *terrain = &actor->terrainId;
    struct MapCell *cells = map->layers[*terrain].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->unk5c[0] = 3;
    if (GameFlag_Test(next)) {
      Func_02003c68(id, 0, 0);
    } else {
      s32 index = Func_02003db0(actor);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = Func_02003c10(*terrain, actor->x, actor->z);
      ground = Map_GetTerrainHeight(*terrain, actor->x, actor->z);

      Func_02003cc0(id);
      cell->attributeB = 128;
      *slot = Func_02003dd0(actor->terrainId, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
  wait(10);
}
