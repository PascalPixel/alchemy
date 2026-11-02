#include "lemurian_ship.h"
#include "scene_tables.h"

/// A switch that toggles flag 0x207 and its map edits once flag 0x983 is set;
/// before that, it does nothing. "There's no reaction... It doesn't seem to be
/// running."
void Func_0200065c(void) {
  Func_02002c68();
  Func_02002da8(0);
  if (GameFlag_Test(0x983)) {
    if (LemurianShip_FlagCheck(0x207)) {
      Func_02002de0(232);
      LemurianShip_ObjSetPos(Actor_GetById(15), PIXELS(824), PIXELS(-32),
                             PIXELS(72));
      Actor_GetById(15)->height = 0xffe00000;
      Func_02002c30(72, 68, 50, 64, 3, 3);
      Func_02002c38(106, 16, 3, 2, 110, 4);
      Func_02002c38(50, 1, 3, 1, 50, 4);
      GameFlag_ClearBit(0x207);
    } else if (GameFlag_Test(0x212)) {
      Func_02002de0(117);
    } else {
      Func_02002de0(232);
      Func_02002cd0(15, 0, 0);
      Func_02002c30(68, 68, 50, 64, 3, 3);
      Func_02002c38(102, 16, 3, 2, 110, 4);
      Func_02002c38(50, 0, 3, 1, 50, 4);
      GameFlag_SetBit(0x207);
    }
  } else {
    FieldMessage_Show((s32)MsgLemurianShip02, 1);
  }
  Func_02002c70();
}
