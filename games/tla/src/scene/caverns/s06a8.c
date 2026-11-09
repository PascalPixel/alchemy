#include "../../../../games/tla/src/scene/caverns/caverns.h"
#include "actor.h"
#include "event_inline.h"

struct CurrentUnit {
  u8 unk00[0x14];
  struct Actor *object;
  u8 unk18[4];
  s16 unk1c;
};

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value);
void Func_0200053c(s32 offset);

void Func_020006a8(void) {
  s32 position[3];
  u8 *work = Ram_HeapSlots->eventWork;
  struct CurrentUnit *current = (struct CurrentUnit *)Ram_HeapSlots->effectWork;
  const u16 *ids = (const u16 *)gData_02001220;
  struct Actor *object = NULL;
  struct Actor *leader;
  s32 height;
  s32 angle;

  WORD_AT(work, 0xcbc) = 0;
  if (current != NULL && HALF_AT(work, 358) != 0) {
    object = (struct Actor *)(WORD_AT(work, 0xcbc) = (s32)current->object);
    if (current->unk1c != 147)
      Func_0200053c(1);
  } else {
    leader = (struct Actor *)((struct FieldActor *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    angle = (leader->angle + 0x2000) & 0xc000;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    if (*ids == 0xffff)
      goto search_done;
  search:
    object = (struct Actor *)((struct FieldActor *)Actor_GetById(*ids));
    if (position[0] >> 20 == object->x >> 20 &&
        position[1] / 0x10000 == object->y / 0x10000 &&
        position[2] >> 20 == object->z >> 20)
      goto search_done;
    ids++;
    object = NULL;
    if (*ids != 0xffff)
      goto search;
  search_done:
    if (object == NULL)
      return;
    WORD_AT(work, 0xcbc) = (s32)object;
    Func_0200053c(1);
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) - 2;
  Func_020010cc(object->x, object->z, 0, height);
  Func_020010cc(object->x, object->z, 2, height);
  Func_020003f0(0, object->x >> 20, object->z >> 20, object->unk4c);
}
