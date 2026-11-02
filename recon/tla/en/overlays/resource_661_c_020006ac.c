// Draft, differ 68 of 128, skeleton gap 22 (tla-en, 2026-11-01)
// Yampi Desert, for its rooms 3 to 5, raises each listed pillar whose flag is
// clear: marks its cell, clears its collision and raises the shape grid under
// it; then sets flag 0x20b.
// Left: one allocation swap, and the gap is its reloads. The native has list in
// r11, the flag result in r9 and shape spilled to sp+0; here list is spilled,
// the result is in r11 and shape in r9. The native follows if list is allocated
// before the result: the result then finds no register and takes r9 from shape
// (ratio 8/36 against 4/22). In the lreg dump list ranks below it: priority
// .49 against .67, because the NULL first set doubles list's live length to 164.
// Tried without gain: a literal 0 for the result (same pseudo; kept, it reads
// better), a chained zero store, list stepping in both arms (24 refs, .56, the
// tails do not merge while list is spilled), continue (bad rotation), list++
// twice (folded), a separate table pointer (loses the NULL in r11), and a goto
// loop with two back edges (30 refs, then caller-save takes r4, so the native
// has at most 24 refs). Rejected as a forced temporary: copying the result into
// a second local for the stores, which would shorten its life. The shape of
// the rest, `cells += index; shapes += index`, closed 16 of the difference.
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
void GameFlag_SetBit(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02004020(s32 layer, s32 x, s32 z);
s32 Func_020041e0(struct Actor *object);
s32 Func_020041e8(s32 layer, s32 shape, s32 height);

void Func_020006ac(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3)
    list = gData_0200478e;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room4)
    list = gData_020047d2;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room5)
    list = gData_020047ec;
  if (list == NULL)
    return;
  while (*list != 0xffff) {
    struct Actor *object = Actor_GetById(*list);

    list += 2;
    if (!GameFlag_Test(*list)) {
      s32 layer = object->terrainId;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes = gMapShapeGrid + (((s32)cells - 0x02010000) >> 2);
      s32 index = Func_020041e0(object);
      s32 shape = Func_02004020(layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(layer, object->x, object->z);

      cells += index;
      shapes += index;
      object->motionFlags = 0;
      cells->collisionCode = 0;
      cells->attributeB |= 128;
      *shapes = Func_020041e8(layer, shape, (height >> 19) + 4);
    }
    list++;
  }
  GameFlag_SetBit(0x20b);
}
