#include "garoh_interiors.h"

void Func_02000710(void) {
  GameFlag_SetBit(0x91b);
  Func_020007f4();
  Func_02000884(0);
  FieldEvent_WaitTicks(30);
  Func_020007cc();
  Func_020008b4(86);
  Func_020007e4((s32)MsgGarohInteriors33, 0, 0);
  FieldEvent_WaitTicks(20);
  Func_020007d4();
  Func_020007bc();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000844((s32)MsgGarohInteriors34);
  Func_02000854(16, 0, 10);
  Motion_SetVarCbAndRefresh(15, 2);
  FieldEvent_WaitTicks(20);
  Func_02000854(15, 0, 10);
  Motion_SetModeAndWaitAnimation(15, 3);
  FieldEvent_WaitTicks(30);
  Func_02000854(15, 0, 10);
  Func_020007fc();
}
