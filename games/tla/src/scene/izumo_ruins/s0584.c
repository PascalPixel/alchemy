#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

void Func_020014a8(s32 id, s32 target);
void Func_0200056c(s32 id);
s32 Func_02001508(struct Actor *actor);
s32 Func_02001478(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020014c0(s32 id);
s32 Func_02001528(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *grid;

  Func_020014a8(8, gMainRoutineWork.currentOwner);
  Func_020014a8(9, gMainRoutineWork.currentOwner);
  Func_020014a8(10, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
  Actor_SetAnimation(9, 0);
  System_Wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  System_Wait(1);
  grid = gMapShapeGrid;
  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *actor = Actor_Get(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[actor->terrainId].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->unk5c[0] = 3;
    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Func_02001508(actor);
      struct MapCell *cell = cells + index;
      u8 *shape = shapes + index;
      s32 kind, ground;

      kind = Func_02001478(actor->terrainId, actor->x, actor->z);
      ground = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      // Read through the actor, not actor->terrainId: the native reloads it
      // after the cell store.
      *shape = Func_02001528(actor->terrainId, kind, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
  System_Wait(10);
}
