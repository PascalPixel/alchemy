#include "kibombo.h"
#include "scene_tables.h"

void Func_02003a08(s32 x, s32 z) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader = Actor_GetById(party->currentOwner);

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(leader, x << 16, PIXELS(-12), z << 16);
  Script_WaitForEventTimeout(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}
