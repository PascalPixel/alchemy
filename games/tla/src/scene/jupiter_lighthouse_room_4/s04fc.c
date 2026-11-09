#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Scene_SetArrivalFlags(s32 value);
void Func_020006a0(s32 value);
void Func_02000724(s32 id);

s32 Func_020004fc(void) {
  Scene_SetArrivalFlags(0);
  Func_020006a0(0);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Event_GetFlag(0xa00)) {
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
    Actor_Get(19)->gravity = 0x30000;
  }
  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  if (Event_GetFlag(0xa06)) {
    Actor_SetPosition(15, PIXELS(168), PIXELS(520));
    Func_02000724(15);
  }
  return 0;
}
