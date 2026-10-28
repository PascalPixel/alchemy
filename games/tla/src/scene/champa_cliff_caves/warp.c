#include "scene.h"

void Func_0200016c(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200345c();
  Func_020035b4(0);
  Func_0200362c(158);
  Func_02003414(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_Wait(10);
  Func_02003574(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003464();
}

void Func_020001f4(s32 door) { Func_0200016c(&gData_02003d10, door, 0); }
