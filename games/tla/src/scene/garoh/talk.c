#include "garoh.h"
#include "event_inline.h"

extern u8 MsgGaroh02[];
extern u8 MsgGaroh04[];
extern u8 MsgGaroh06[];
extern u8 MsgGaroh17[];
extern u8 MsgGaroh18[];

void Func_02000580(s32 actor) {
  s32 msg = (s32)MsgGaroh02;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_020005cc(s32 actor) {
  s32 msg = (s32)MsgGaroh06;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000618(s32 actor) {
  s32 msg = (s32)MsgGaroh18;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000664(s32 actor) {
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaroh17);
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Func_020051b4();
}

void Func_02000698(s32 actor) {
  s32 flag;
  s32 msg;

  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  flag = Event_GetFlag(0x300);
  msg = (s32)MsgGaroh04;
  if (flag) {
    FieldEvent_SetMessageId(msg);
  } else {
    FieldEvent_SetMessageId(msg);
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
  }
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Func_020051b4();
}
