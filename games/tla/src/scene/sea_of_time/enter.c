#include "sea_of_time.h"
#include "event_inline.h"

void Func_02001068(void) {
  Func_02001fac();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x8dd)) {
    Import_EventSetExitRequest(4);
    GameFlag_SetBit(0x8dd);
  } else {
    Import_EventSetExitRequest(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}
