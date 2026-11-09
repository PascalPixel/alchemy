#include "types.h"
#include "event_inline.h"

extern u8 MsgChampa30[];
extern u8 MsgChampa05[];

void Func_020004b4(s32 actor) {
  if (GameFlag_Test(0x97f)) {
    Event_SetMessageId((s32)MsgChampa30);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId((s32)MsgChampa30 + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId((s32)MsgChampa30 + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else {
    Event_SetMessageId((s32)MsgChampa05);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId((s32)MsgChampa05 + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId((s32)MsgChampa05 + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
}
