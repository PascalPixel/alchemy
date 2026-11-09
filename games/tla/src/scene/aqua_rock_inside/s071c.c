#include "scene_tables.h"
#include "local.h"

void Func_0200071c(void) {
  s32 priority;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Event_PlaySound(0x22d);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeed(0x28000, 0x4000);
  FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(648), 1);
  Func_02003e54();
  FieldEvent_Wait(30);
  Func_02003d84();
  Event_SetFlag(0x240);
  Func_02003d1c_(59, 16, 5, 32, 46, 16);
}
