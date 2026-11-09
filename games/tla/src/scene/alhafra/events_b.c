#include "types.h"
#include "event_inline.h"

extern u8 MsgAlhafra03[];
extern u8 MsgAlhafra04[];
extern u8 MsgAlhafra16[];
extern u8 MsgAlhafra33[];

void Func_0200013c(s32 actor) {
  s32 msg;

  if (Event_GetFlag(0x8b0) == 0) {
    msg = (s32)&MsgAlhafra03;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      GameFlag_SetBit(0x8b0);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else {
    Event_SetMessageId((s32)&MsgAlhafra04);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_020001a4(s32 actor) {
  s32 msg = (s32)&MsgAlhafra16;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_020001f0(s32 actor) {
  s32 msg = (s32)&MsgAlhafra33;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}
