#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern char SceneId_Ov661Room3;
extern char SceneId_Ov661Room4;
extern char SceneId_Ov661Room5;
extern u16 gData_0200478e[];
extern u16 gData_020047d2[];
extern u16 gData_020047ec[];
extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02004020(s32 layer, s32 x, s32 z);
s32 Func_020041e0(struct Actor *object);
s32 Func_020041e8(s32 layer, s32 shape, s32 height);

void Func_020007e0(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3)
    list = gData_0200478e;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room4)
    list = gData_020047d2;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room5)
    list = gData_020047ec;
  if (list == NULL)
    return;
  if (*list == 0xffff)
    goto done;
next:
  {
    struct Actor *object = Actor_GetById(*list);
    s32 set;

    list += 2;
    set = GameFlag_Test(*list);
    if (!set) {
      s32 layer = object->terrainId;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      /* FAKEMATCH(pin): the index in r10; as a plain local, 32 of 130 instructions
       * differ in tla-en. */
      register s32 index asm("r10") = Func_020041e0(object);
      s32 shape = Func_02004020(layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(layer, object->x, object->z);
      struct MapCell *cell = cells + index;

      object->motionFlags = set;
      cell->collisionCode = 255;
      cell->attributeB &= 127;
      shapes[index] = Func_020041e8(layer, shape, (height >> 19) - 4);
    }
  }
  list++;
  if (*list != 0xffff)
    goto next;
done:
  GameFlag_ClearBit(0x20b);
}
