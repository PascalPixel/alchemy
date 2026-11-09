#include "local.h"

void Func_02000de0(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e24(8, actor);
    return;
  }
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgWowYouLook);
  Event_ShowMessage(actor, 0);
  Func_02001ccc();
}

void Func_02000e40(s32 actor) {
  s32 msg;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)&MsgYouGuysAren;
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
  Func_02001ccc();
}

void Func_02000e98(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e1c(23, actor);
    return;
  }
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgItDBe);
  Event_ShowMessage(actor, 0);
  Func_02001ccc();
}
