#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02001010(void);
void Func_02001018(void);

/// Object 24 calls out to the party, jumps down to it and talks with object
/// 23; flag 0x9ad is set.
void Func_0200097c(void) {
  s32 angle;

  Func_02001010();
  Event_ResolvePendingActions(0);
  angle = 0xd000;
  Actor_SetHorizontalPositionAndAngleWithTerrain(24, PIXELS(504), PIXELS(460),
                                                 angle);
  wait(1);
  Event_SetMessageId((s32)MsgHeyWait2);
  Event_ShowMessageAndWait(24, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(23, 0x3000, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  Func_080d4568()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(380), 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_WalkToAndWait(24, 520, 432);
  Actor_WalkToAndWait(24, 520, 388);
  Actor_FaceDirection(23, 0x3000, 0);
  Event_ShowMessageAndWait(24, 0);
  Func_080d47b4(23, 256, 10);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(24, 265, 0);
  Event_ShowMessageAndWait(24, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Actor_ArmCallbackAndRefresh(24, angle);
  Actor_GetById(24)->actionFlags &= ~1;
  Actor_WalkToAndWait(24, 520, 384);
  Event_Wait(1);
  Actor_GetById(24)->actionFlags |= 1;
  Motion_SetVarCbAndRefresh(24, 2);
  Actor_SetHorizontalPositionWithTerrain(65, PIXELS(520), PIXELS(380));
  wait(1);
  Actor_GetById(24)->actionFlags &= ~1;
  Actor_WalkToAndWait(24, 520, 388);
  Event_Wait(1);
  Actor_GetById(24)->actionFlags |= 1;
  Event_Wait(10);
  Actor_FaceDirection(24, 0xb000, 10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Actor_WalkToAndWait(24, 520, 434);
  Camera_MoveWithinMapBounds(PIXELS(512), -1, PIXELS(348), 1);
  Actor_WalkToAndWait(24, 504, 460);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Actor_ArmCallbackAndRefresh(23, angle);
  Event_ShowMessageAndWait(23, 0);
  Func_080d47b4(23, 265, 0);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_ShowMessageAndWait(23, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Func_080d47b4(23, 264, 0);
  Event_ShowMessageAndWait(23, 0);
  GameFlag_SetBit(0x9ad);
  Func_02001018();
}
