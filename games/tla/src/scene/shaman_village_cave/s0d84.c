#include "local.h"

void Func_02000d84(void) {
  struct ShamanVillageCaveEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  u32 id;

  Func_02001080();
  FieldEvent_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_Get(id);
  if (object)
    object->motionFlags = 0;
  id++;
  if (id < 64)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(5);
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(work->value154);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001088();
}
