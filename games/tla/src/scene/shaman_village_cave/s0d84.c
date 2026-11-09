#include "local.h"

void Func_02000d84(void) {
  struct ShamanVillageCaveEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  u32 id;

  Func_02001080();
  Event_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_GetById(id);
  if (object)
    object->motionFlags = 0;
  id++;
  if (id < 64)
    goto next;
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterOnColumn(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(5);
  Sound_Play(123);
  Import_EventSetExitRequest(work->value154);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001088();
}
