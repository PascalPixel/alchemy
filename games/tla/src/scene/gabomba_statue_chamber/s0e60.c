#include "gabomba_statue_chamber.h"
#include "scene_tables.h"

void Func_02000e60(void) {
  Func_02001da0();
  Func_02001ee8(0);
  GabombaStatueChamber_CamMove(PIXELS(392), -1, PIXELS(120), 1);
  Func_02001ea8();
  FieldEvent_Wait(20);
  Func_02001e48((s32)MsgIHaveFollowed);
  GabombaStatueChamber_ObjEmoteWait(10, 0x102);
  Func_02001e60(10, 0);
  GabombaStatueChamber_ObjTurn(10, FACING_WEST);
  Func_02001e60(10, 0);
  GabombaStatueChamber_ObjTurn(10, FACING_EAST);
  Func_02001e60(10, 0);
  GabombaStatueChamber_ObjTurn(10, FACING_NORTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(10, 4);
  Func_02001e60(10, 0);
  GameFlag_SetBit(0x8fe);
  Func_02001da8();
}
