#include "types.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

void Daila_TalkBeenBefore(void) {
  s32 msg;

  if (GameFlag_Test(0x843) == 0) {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    msg = (s32)MsgDailaBeenBefore;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
    GameFlag_SetBit(0x843);
    Import_EventEnd();
  } else {
    Event_SetMessageId((s32)MsgDailaWaveNoInjuries);
    Event_ShowMessageAndWait(8, 0);
  }
}

void Daila_TalkIndraMoved(void) {
  s32 msg;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaIndraMoved;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Import_EventEnd();
}

void Daila_TalkBoat(void) {
  s32 msg;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaWantBoat;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Import_EventEnd();
}

void Func_02000288(s32 keeper) {
  u16 facing = ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopOpen(3, keeper);
  } else {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiWorry);
    Event_ShowMessageAndWait(keeper, 0);
    Import_EventEnd();
  }
}
