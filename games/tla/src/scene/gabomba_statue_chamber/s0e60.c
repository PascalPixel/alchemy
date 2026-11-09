#include "gabomba_statue_chamber.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_02000e60(void) {
  Func_02001da0();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(120), 1);
  Func_02001ea8();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgIHaveFollowed);
  Actor_SetAttachedEffect(10, 0x102);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_NORTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageAndWait(10, 0);
  GameFlag_SetBit(0x8fe);
  Func_02001da8();
}
