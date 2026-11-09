#include "gabomba_statue_chamber.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.currentOwner));

  Func_02001da0();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(((struct GabombaStatueChamberObject *)Actor_GetById(
                                gMainRoutineWork.currentOwner)),
                            0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(object, x << 16, PIXELS(-12), z << 16);
  Script_WaitForEventTimeout(object);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}
