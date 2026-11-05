// Draft, differ 2 of 92 (tla-en, 2026-11-05). Pushes the object in the cell east of
// the leader one cell further, copying the cells it leaves and enters, and sets flag
// 0x8d9 when it stops at (16, 36). Actor_FindInCell takes the leader as its second
// argument, which settles the first block. Left: the first Func_020005cc's ldrb r0
// comes after the shifts in the native, before them here.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"

extern s16 gData_02003de4[];

struct Actor *Actor_GetById(s32 id);
struct Actor *Actor_FindInCell(s32 *position, struct Actor *except);
void GameFlag_SetBit(s32 flag);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
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
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                object->terrainHeight >> 19);
  Call6((void (*)())Func_02003aa4, 79, 46, 2, 1, 79, 36);
  Func_020003b4();
  *terrainId = 0;
  Call6((void (*)())Func_02003aa4, 79, 37, 2, 1, 79, 36);
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de4);
  cell[0] = object->x >> 20;
  cell[2] = object->z >> 20;
  if (cell[0] == 16 && cell[2] == 36)
    GameFlag_SetBit(0x8d9);
}
