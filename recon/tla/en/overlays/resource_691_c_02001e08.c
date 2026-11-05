// // Draft, differ 46 of 92 (tla-en, 2026-11-05). Pushes the object in the cell east
// // of the leader one cell further, copying the cells it leaves and enters, and sets
// // flag 0x8d9 when it stops at (16, 36). The Call6 forwarding, as the rest of this
// // overlay calls Func_02003aa4, puts 79 in r6 and 36 in r8 as in the native.
// // Left: the native holds the leader in r1 after the first call, not r0, though r0
// // stays free until the argument; here it stays in r0, which moves the 0x100000 and
// // the position's address onto other scratch registers.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"

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
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *cell = position;
  struct Actor *object;

  cell[0] = leader->x + 0x100000;
  cell[1] = leader->y;
  cell[2] = leader->z;
  object = Actor_FindInCell(cell);
  Func_0200074c(object);
  Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                object->terrainHeight >> 19);
  Call6((void (*)())Func_02003aa4, 79, 46, 2, 1, 79, 36);
  Func_020003b4();
  object->terrainId = 0;
  Call6((void (*)())Func_02003aa4, 79, 37, 2, 1, 79, 36);
  Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de4);
  cell[0] = object->x >> 20;
  cell[2] = object->z >> 20;
  if (cell[0] == 16 && cell[2] == 36)
    GameFlag_SetBit(0x8d9);
}
