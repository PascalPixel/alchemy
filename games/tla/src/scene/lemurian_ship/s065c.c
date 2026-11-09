#include "lemurian_ship.h"
#include "event_inline.h"
#include "scene_tables.h"

/// A switch that toggles flag 0x207 and its map edits once flag 0x983 is set;
/// before that, it does nothing. "There's no reaction... It doesn't seem to be
/// running."
void Func_0200065c(void) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x983)) {
    if (Event_GetFlag(0x207)) {
      Sound_Play(232);
      FieldActor_SetPositionAndResetMotion(
          ((struct LemurianShipObject *)Actor_GetById(15)), PIXELS(824),
          PIXELS(-32), PIXELS(72));
      ((struct LemurianShipObject *)Actor_GetById(15))->height = 0xffe00000;
      Map_CopyCellPictureIndicesRect(72, 68, 50, 64, 3, 3);
      Map_CopyCellAttributeRect(106, 16, 3, 2, 110, 4);
      Map_CopyCellAttributeRect(50, 1, 3, 1, 50, 4);
      GameFlag_ClearBit(0x207);
    } else if (GameFlag_Test(0x212)) {
      Sound_Play(117);
    } else {
      Sound_Play(232);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
      Map_CopyCellPictureIndicesRect(68, 68, 50, 64, 3, 3);
      Map_CopyCellAttributeRect(102, 16, 3, 2, 110, 4);
      Map_CopyCellAttributeRect(50, 0, 3, 1, 50, 4);
      GameFlag_SetBit(0x207);
    }
  } else {
    FieldMessage_Show((s32)MsgLemurianShip02, 1);
  }
  Func_02002c70();
}
