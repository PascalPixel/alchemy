#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern s16 gData_02003de8[];

struct Actor *Actor_FindInCell(s32 *position, struct Actor *except);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001edc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  struct Actor *object;
  u8 *terrainId;

  position[0] = leader->x - 0x100000;
  position[1] = leader->y;
  position[2] = leader->z;
  object = Actor_FindInCell(position, leader);
  Func_0200074c(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals; as call arguments,
     * 2 of 90 instructions differ in tla-en. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    Func_020005cc(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
  Func_020003b4();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de8);
  position[0] = object->x >> 20;
  position[2] = object->z >> 20;
  if (position[0] == 35 && position[2] == 38)
    Event_SetFlag(0x8db);
}
