#include "shaman_village.h"

void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02003ce4(void) {
  if (GameFlag_Test(0x202)) {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Sound_Play(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(13);
  } else {
    Func_02003b6c(43, 40, 7, 6, 1, 2);
  }
}
