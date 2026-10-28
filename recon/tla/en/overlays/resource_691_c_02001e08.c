// Draft, differ 52 of 92: pushes the object in the cell east of the leader one
// cell further, copying the cells it leaves and enters, and sets flag 0x8d9
// when it stops at (16, 36). Left: the native keeps the position's address in
// r10 from its first store (8 added to sp) and 79 in r6 and 36 in r8, where
// this form builds the address with add sp and swaps the two constants.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

extern s16 gData_02003de4[];

struct Actor *Actor_GetById(s32 id);
struct Actor *Actor_FindInCell(s32 *position);
void GameFlag_SetBit(s32 flag);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001e08(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct { s32 x, y, z; } position;
  struct { s32 x, y, z; } *cell = (void *)&position;
  struct Actor *object;

  cell->x = leader->x + 0x100000;
  cell->y = leader->y;
  cell->z = leader->z;
  object = Actor_FindInCell((s32 *)cell);
  Func_0200074c(object);
  Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                object->terrainHeight >> 19);
  Func_02003aa4(79, 46, 2, 1, 79, 36);
  Func_020003b4();
  object->terrainId = 0;
  Func_02003aa4(79, 37, 2, 1, 79, 36);
  Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de4);
  cell->x = object->x >> 20;
  cell->z = object->z >> 20;
  if (cell->x == 16 && cell->z == 36)
    GameFlag_SetBit(0x8d9);
}
