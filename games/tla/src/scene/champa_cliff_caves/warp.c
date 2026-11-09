#include "scene.h"

void Func_0200016c(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200345c();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003464();
}

void Func_020001f4(s32 door) { Func_0200016c(&gData_02003d10, door, 0); }
