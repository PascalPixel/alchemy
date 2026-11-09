#include "local.h"

void Func_02000de0(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e24(8, actor);
    return;
  }
  Func_02001cc4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgWowYouLook);
  Event_ShowMessageAndWait(actor, 0);
  Func_02001ccc();
}

void Func_02000e40(s32 actor) {
  s32 msg;

  Func_02001cc4();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgYouGuysAren;
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
  Func_02001ccc();
}

void Func_02000e98(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e1c(23, actor);
    return;
  }
  Func_02001cc4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgItDBe);
  Event_ShowMessageAndWait(actor, 0);
  Func_02001ccc();
}
