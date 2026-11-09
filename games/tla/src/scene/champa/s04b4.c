#include "types.h"
#include "event_inline.h"

extern u8 MsgChampa30[];
extern u8 MsgChampa05[];

void Func_020004b4(s32 actor) {
  if (Event_GetFlag(0x97f)) {
    FieldEvent_SetMessageId((s32)MsgChampa30);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampa30 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampa30 + 2);
    }
    Event_ShowMessage(actor, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampa05);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampa05 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampa05 + 2);
    }
    Event_ShowMessage(actor, 0);
  }
}
