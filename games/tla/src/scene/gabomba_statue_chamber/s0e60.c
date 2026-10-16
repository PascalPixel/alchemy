#include "gabomba_statue_chamber.h"

void Func_02000e60(void) {
  Func_02001da0();
  Func_02001ee8(0);
  GabombaStatueChamber_CamMove(0x1880000, -1, 0x780000, 1);
  Func_02001ea8();
  FieldEvent_WaitFrames(20);
  Func_02001e48((s32)MsgIHaveFollowed);
  GabombaStatueChamber_ObjEmoteWait(10, 0x102);
  Func_02001e60(10, 0);
  GabombaStatueChamber_ObjTurn(10, 0x8000);
  Func_02001e60(10, 0);
  GabombaStatueChamber_ObjTurn(10, 0);
  Func_02001e60(10, 0);
  GabombaStatueChamber_ObjTurn(10, 0xd000);
  Motion_SetModeAndWaitAnimation(10, 4);
  Func_02001e60(10, 0);
  GameFlag_SetBit(0x8fe);
  Func_02001da8();
}
