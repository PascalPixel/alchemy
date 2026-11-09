#include "islets.h"

void Func_0200274c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  Func_020034ec();
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}
