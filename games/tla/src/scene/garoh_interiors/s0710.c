#include "garoh_interiors.h"

void Func_02000710(void) {
  GameFlag_SetBit(0x91b);
  Func_020007f4();
  Event_ResolvePendingActions(0);
  Event_Wait(30);
  Func_020007cc();
  Sound_Play(86);
  Func_0803ce64((s32)MsgGarohInteriors33, 0, 0);
  Event_Wait(20);
  Func_020007d4();
  Func_020007bc();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId((s32)MsgGarohInteriors34);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(15, 0, 10);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(15, 0, 10);
  Func_020007fc();
}
