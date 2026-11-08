#include "../../../../../../games/tla/src/scene/yampi_desert/scene.h"

extern const u8 MsgAsYouCan[];

void Func_02000550(void) {
  Func_02004040();
  Func_020041b8(0);
  if (!Scene_FlagTest(SPLIT_CONSTANT(0x88f)))
    Func_020004e0();
  Actor_SetAngleToward(12, gMainRoutineWork.currentOwner, 0);
  Func_020040c8((s32)MsgAsYouCan);
  Func_020040d0(12, 0);
  GameFlag_SetBit(SPLIT_CONSTANT(0x790, 255));
  Func_02004048();
}
