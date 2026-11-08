#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"

extern s16 gData_02003de8[];

struct Actor *Actor_GetById(s32 id);
struct Actor *Actor_FindInCell(s32 *position, struct Actor *except);
void GameFlag_SetBit(s32 flag);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001edc(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
  Call6((void (*)())Func_02003aa4, 94, 44, 5, 1, 99, 38);
  Func_020003b4();
  *terrainId = 0;
  Call6((void (*)())Func_02003aa4, 99, 42, 5, 1, 99, 38);
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de8);
  position[0] = object->x >> 20;
  position[2] = object->z >> 20;
  if (position[0] == 35 && position[2] == 38)
    GameFlag_SetBit(SPLIT_CONSTANT(0x800, 219));
}
