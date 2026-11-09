#include "garoh.h"

extern u8 MsgGaroh02[];
extern u8 MsgGaroh04[];
extern u8 MsgGaroh06[];
extern u8 MsgGaroh17[];
extern u8 MsgGaroh18[];

void Func_02000580(s32 actor) {
  s32 msg = (s32)MsgGaroh02;

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

void Func_020005cc(s32 actor) {
  s32 msg = (s32)MsgGaroh06;

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

void Func_02000618(s32 actor) {
  s32 msg = (s32)MsgGaroh18;

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

void Func_02000664(s32 actor) {
  Func_020051ac();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGaroh17);
  Event_ShowMessageThenWait(actor, 0, 10);
  Event_ShowMessageThenWait(actor, 0, 10);
  Func_020051b4();
}

void Func_02000698(s32 actor) {
  s32 flag;
  s32 msg;

  Func_020051ac();
  Event_ResolvePendingActions(0);
  flag = GameFlag_Test(0x300);
  msg = (s32)MsgGaroh04;
  if (flag) {
    Event_SetMessageId(msg);
  } else {
    Event_SetMessageId(msg);
    Event_ShowMessageThenWait(actor, 0, 10);
    Event_ShowMessageThenWait(actor, 0, 10);
  }
  Event_ShowMessageThenWait(actor, 0, 10);
  Func_020051b4();
}
