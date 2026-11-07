// Matches (tla-en, 2026-11-07); all six editions agree. Sets the listed
// pillars as Func_0200222c does, after moving actors 8 to 10 to the leader;
// overlay 68f's copy of the function in resource_691_c_020030bc.c.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void Func_02003088(s32 id, s32 x, s32 z);
void Func_02003090(s32 id, s32 target);
void Func_020025c8(s32 id);
s32 Func_02003160(struct Actor *object);
s32 Func_02003010(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020030c0(s32 id);
s32 Func_02003180(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void Func_020025e0(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Func_02003090(8, gMainRoutineWork.currentOwner);
  Func_02003090(9, gMainRoutineWork.currentOwner);
  Func_02003090(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_020025c8(8);
  Func_020025c8(9);
  Func_020025c8(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02003088(8, 0, 0);
  Func_02003088(9, 0, 0);
  Func_02003088(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_GetById(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Test(nextFlag)) {
    Func_02003088(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02003160(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02003010(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_020030c0(id);
    cell->attributeB = 128;
    *slot = Func_02003180(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}
