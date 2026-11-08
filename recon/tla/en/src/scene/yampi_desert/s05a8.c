#include "../../../../../../games/tla/src/scene/yampi_desert/scene.h"

extern const u8 MsgThisIsWhere[];

void Func_020005a8(void) {
  Func_02004040();
  Func_020041b8(0);
  if (!Scene_FlagTest(SPLIT_CONSTANT(0x790, 255)))
    Func_020004e0();
  Actor_SetAngleToward(13, gMainRoutineWork.currentOwner, 0);
  Func_020040c8((s32)MsgThisIsWhere);
  Func_020040d0(13, 0);
  GameFlag_SetBit(0x88f);
  Func_02004048();
}
