#include "types.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);

void Func_02000058(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 24 && z == 40)
    GameFlag_SetBit(0x900);
  if (x == 23 && z == 40)
    GameFlag_ClearBit(0x900);
}
