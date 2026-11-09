#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern s16 gData_02003de4[];

struct Actor *Actor_FindInCell(s32 *position, struct Actor *except);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001e08(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *cell = position;
  struct Actor *object;
  u8 *terrainId;

  cell[0] = leader->x + 0x100000;
  cell[1] = leader->y;
  cell[2] = leader->z;
  object = Actor_FindInCell(cell, leader);
  Func_0200074c(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals; as call arguments,
     * 2 of 92 instructions differ in tla-en. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    Func_020005cc(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
  Func_020003b4();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de4);
  cell[0] = object->x >> 20;
  cell[2] = object->z >> 20;
  if (cell[0] == 16 && cell[2] == 36)
    GameFlag_SetBit(0x8d9);
}
