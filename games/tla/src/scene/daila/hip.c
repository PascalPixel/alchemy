#include "types.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

void Daila_TalkHip(void) {
  s32 msg;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaHurtHip;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(12, 0);
  Import_EventEnd();
}

void Func_02000340(s32 keeper) {
  u16 facing = ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopOpen(3, keeper);
  } else {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiHome);
    Event_ShowMessageAndWait(keeper, 0);
    Import_EventEnd();
  }
}
