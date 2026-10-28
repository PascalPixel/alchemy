// Draft, differ 34 of 130 (was 81): the reverse of Func_020006ac, lowering
// each listed pillar whose flag is clear and clearing flag 0x20b. The walk is
// a goto loop entered by a first test of the end mark, as the native has the
// test at the top and at the bottom. Left: registers inside the lowering (the
// native keeps the cells in r6, the shape grid's pointer in r5, the cell
// index in r10 and the shape in r9; here the pairs are swapped) and the final
// store, which the native makes through the shape grid's pointer advanced by
// the index. Measured: the declarations in other orders (no change), the
// pointer advanced in the source (43), the cell indexed in place (84). The
// same entry test gives Func_020006ac 107 (its list goes to the stack).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
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

  if (gSequenceParentWork.scene == (s32)&SceneId_Ov661Room3)
    list = gData_0200478e;
  else if (gSequenceParentWork.scene == (s32)&SceneId_Ov661Room4)
    list = gData_020047d2;
  else if (gSequenceParentWork.scene == (s32)&SceneId_Ov661Room5)
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
      u8 *shapes = gMapShapeGrid + (((s32)cells - 0x02010000) >> 2);
      s32 index = Func_020041e0(object);
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
