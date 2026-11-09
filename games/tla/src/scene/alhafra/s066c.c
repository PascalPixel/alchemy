#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u8 MsgAlhafra92[];

/// Talking to the actor: once flag 0x8b9 is set it says its last line;
/// otherwise it asks a yes-or-no question. A nonzero answer ends with another
/// line; the other sets the flag, and the actor and the leader step aside.
void Func_0200066c(s32 actor) {
  s32 message = (s32)MsgAlhafra92;

  if (Event_GetFlag(0x8b9) != 0) {
    Event_SetMessageId(message + 4);
    FieldMessage_ShowAtActor(actor, 0);
  } else {
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_Wait(20);
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      GameFlag_SetBit(0x8b9);
      Event_Wait(20);
      Event_SetMessageId(message + 2);
      Event_ShowMessage(actor, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 344);
      Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
      Event_Wait(10);
      Actor_SetSpeed(actor, 0x10000, 0x8000);
      Actor_WalkToAndWait(actor, 56, 328);
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
      Actor_ArmCallback(actor, FACING_SOUTHEAST, 0);
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(actor, 3);
      Event_Wait(10);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}
