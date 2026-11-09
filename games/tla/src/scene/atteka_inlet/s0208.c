#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02001010(void);
void Func_02001018(void);

/// Once flag 0x9d0 is set and while flag 0x9b2 is clear, objects 9 and 10 are
/// placed, jump in and talk, four cells of the map change and flag 0x9b0 is set.
void Func_02000208(void) {
  if (GameFlag_Test(0x9d0) && !GameFlag_Test(0x9b2)) {
    Func_02001010();
    Event_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds(96 << 16, -1, 308 << 16, 1);
    {
      /* FAKEMATCH: the angle is shifted from a local so that it is built first,
       * as the native builds it; as a constant it is built last (differ 2). */
      s32 angle = 208;

      Actor_SetPositionAndFacing(9, 60 << 16, 362 << 16, angle << 8);
    }
    wait(1);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    Actor_Jump(9, 6, 0);
    Actor_MoveToAndWait(9, 64, 336);
    Actor_WalkToAndWait(9, 70, 308);
    Actor_ArmCallback(9, FACING_EAST, 0);
    {
      /* FAKEMATCH: as above (differ 2). */
      s32 angle = 160;

      Actor_SetPositionAndFacing(10, 94 << 16, 272 << 16, angle << 7);
    }
    wait(1);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_Jump(10, 6, 0);
    Actor_MoveToAndWait(10, 90, 288);
    Actor_SetPositionAndReset(10, 80, 316);
    Actor_ArmCallbackAndRefresh(10, FACING_EAST);
    Actor_GetById(9)->velocityY = 0x40000;
    Event_Wait(10);
    Event_SetMessageId((s32)MsgWhereDoYouThink);
    Event_ShowMessage(0x4009, 0);
    Func_080d47b4(10, 265, 0);
    Event_ShowMessageAndWait(10, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Event_ShowMessage(0x4009, 0);
    Actor_FaceDirectionAndWait(10, 0xb000);
    Actor_SetModeById(10, 3);
    Event_ShowMessageAndWait(10, 0);
    Actor_ArmCallback(10, FACING_EAST, 0);
    Func_080d47b4(9, 264, 0);
    Actor_ArmCallback(9, FACING_EAST, 20);
    Event_ShowMessageAndWait(0x4009, 0);
    Event_ShowMessageAndWait(10, 0);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    GameFlag_SetBit(0x9b0);
    Func_02001018();
  }
}
