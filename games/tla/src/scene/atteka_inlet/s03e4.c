#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02001010(void);
void Func_02001018(void);
void Func_020010e0(void);

/// Once flag 0x9d0 is set, the camera pans over object 23, who greets the
/// party, turns, talks and walks to its place; flag 0x9b1 is set.
void Func_020003e4(void) {
  if (GameFlag_Test(0x9d0)) {
    Func_02001010();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Camera_SetSpeedParameters(0x26666, 0x4ccc);
    FieldCamera_MoveTo(PIXELS(520), -1, PIXELS(372), 1);
    Func_020010e0();
    Actor_ShowEmote(23, 264, 0);
    Actor_FaceDirection(23, 0x3000, 20);
    Event_SetMessageId((s32)MsgIVeBeenWaiting);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, 0xb000);
    Event_ShowMessage(0x2017, 0);
    Func_080d4568()->motionFlags = 0;
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(PIXELS(440), PIXELS(32), PIXELS(368), 1);
    Func_020010e0();
    Event_Wait(40);
    FieldCamera_MoveTo(PIXELS(432), PIXELS(32), PIXELS(232), 1);
    Func_020010e0();
    Event_Wait(40);
    Camera_SetSpeedParameters(0x4cccc, 0x9999);
    Camera_MoveWithinMapBounds(PIXELS(520), PIXELS(32), PIXELS(372), 1);
    Func_020010e0();
    Event_Wait(20);
    Actor_FaceDirectionAndWait(23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmote(23, 265, 20);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetModeById(23, 4);
    Event_ShowMessageAndWait(23, 0);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, 0xb000);
    Event_ShowMessageAndWait(0x2017, 0);
    Actor_FaceDirectionAndWait(23, 0x3000);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetModeById(23, 4);
    Event_ShowMessageAndWait(23, 0);
    Func_080d47b4(gMainRoutineWork.currentOwner, 257, 40);
    Func_080d47b4(23, 265, 20);
    Event_ShowMessageAndWait(23, 0);
    Func_080d47b4(23, 264, 20);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetSpeedParameters(23, 0xcccc, 0x6666);
    Actor_GetById(23)->actionFlags &= ~1;
    Actor_SetPositionAndReset(23, 504, 360);
    Event_Wait(1);
    Actor_GetById(23)->actionFlags |= 1;
    Actor_ArmCallbackAndRefresh(23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    GameFlag_SetBit(0x9b1);
    Func_02001018();
  }
}
