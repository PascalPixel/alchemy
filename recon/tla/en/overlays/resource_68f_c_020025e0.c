// Draft, differ 8 of 150 (tla-en, 5 November 2026), under -Os: registers and frame are the native's. Left: the third Func_02003088's r2 ahead of its r0, the shape address's sum tied to the grid's register, and the first load's order. Sets the listed pillars as Func_0200222c does, after moving actors 8 to 10 to the leader.
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
  goto test;
  do {
    s32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    s32 nextFlag = flag + 1;
    struct MapCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(nextFlag)) {
      Func_02003088(id, 0, 0);
    } else {
      s32 index = Func_02003160(object);
      s32 shape;
      s32 ground;
      struct MapCell *cell = &cells[index];
      u8 *slot = shapes + index;
      shape = Func_02003010(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
      Func_020030c0(id);
      cell->attributeB = 128;
      *slot = Func_02003180(object->terrainId, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  test:;
  } while (list[0] != 0xffff);
  wait(10);
}
