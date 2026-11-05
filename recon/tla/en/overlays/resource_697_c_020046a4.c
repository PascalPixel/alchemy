// Draft, differ 8: overlay 697's copy of the function drafted in
// resource_691_c_020030bc.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_020058f0(s32 id, s32 owner);
void Func_0200468c(s32 id);
void Func_020058e0(s32 id, s32 x, s32 z);
s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
s32 Func_02005a20(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void Task_Wait(s32 frames);

void Func_020046a4(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Func_020058f0(8, gMainRoutineWork.currentOwner);
  Func_020058f0(9, gMainRoutineWork.currentOwner);
  Func_020058f0(10, gMainRoutineWork.currentOwner);
  Task_Wait(1);
  Func_0200468c(8);
  Func_0200468c(9);
  Func_0200468c(10);
  Actor_SetModeById(9, 0);
  Task_Wait(1);
  Func_020058e0(8, 0, 0);
  Func_020058e0(9, 0, 0);
  Func_020058e0(10, 0, 0);
  Task_Wait(1);
  if (*list == 0xffff)
    goto done;
next:
  {
    s32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    s32 nextFlag = flag + 1;
    u8 *layer = &object->terrainId;
    struct MapCell *cells = map->layers[*layer].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(nextFlag)) {
      Func_020058e0(id, 0, 0);
    } else {
      s32 index = Func_02005a18(object);
      u8 *cell = (u8 *)&cells[index];
      u8 *slot = shapes + index;
      s32 shape = Func_02005848(*layer, object->x, object->z);
      s32 ground = Map_GetTerrainHeight(*layer, object->x, object->z);

      Func_02005958(id);
      cell[3] = 128;
      *slot = Func_02005a20(*layer, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  Task_Wait(10);
}
