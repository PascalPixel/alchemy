#include "islets.h"
#include "event_inline.h"

void Func_0200274c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Func_020034ec();
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}
