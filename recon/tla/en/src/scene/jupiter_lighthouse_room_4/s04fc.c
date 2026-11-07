#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 GameFlag_Test(s32 flag);
void Scene_SetArrivalFlags(s32 value);
void Func_020006a0(s32 value);
void Func_02000724(s32 id);
void Func_02001e70(s32 id, s32 x, s32 z);

s32 Func_020004fc(void) {
  Scene_SetArrivalFlags(0);
  Func_020006a0(0);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (GameFlag_Test(0xa00)) {
    Func_02000724(8);
    Func_02000724(9);
    Func_02000724(10);
    Func_02000724(11);
    Func_02000724(12);
    Func_02000724(13);
    Func_02000724(16);
    Func_02000724(17);
    Func_02000724(18);
    Func_02000724(19);
    Actor_GetById(19)->gravity = 0x30000;
  }
  Actor_SetSingleChildFlags(Actor_GetById(19), 0);
  if (GameFlag_Test(0xa06)) {
    Func_02001e70(15, PIXELS(168), PIXELS(520));
    Func_02000724(15);
  }
  return 0;
}
