#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020005c4();
void Func_020005cc();
void Func_0200063c();
void Func_02000644();
void Func_0200064c();

void Func_020000f4(void) {
  s32 result0;
  s32 result1;

  Func_020005c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMikasallaInteriors03);
  FieldMessage_ShowAtActor(10, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_0200812c;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgMikasallaInteriors03 + 1));
  goto L_02008138;
L_0200812c:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgMikasallaInteriors03 + 2));
L_02008138:
  Event_ShowMessageAndWait(10, 0);
  Func_020005cc();
  return;
}

void Func_0200014c(s32 actor) {
  u16 facing;

  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_0200063c(13, actor);
  } else {
    Event_SetMessageId((s32)MsgMikasallaInteriors11);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_0200019c(s32 actor) {
  u16 facing;

  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_0200064c(4, actor);
  } else {
    Func_020005c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors14);
    Event_ShowMessageAndWait(actor, 0);
    Func_020005cc();
  }
}

void Func_020001fc(s32 actor) {
  u16 facing;

  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Func_020005c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors17);
    FieldMessage_ShowAtActor(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(
            Value0((s32 (*)())Party_GetLeaderActor), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId((s32)MsgMikasallaInteriors17 + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId((s32)MsgMikasallaInteriors17 + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
    Func_020005cc();
  }
}

void Func_02000288(s32 actor) {
  u16 facing;

  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Func_020005c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors29);
    Event_ShowMessageAndWait(actor, 0);
    Func_020005cc();
  }
}

void Func_020002e4(s32 actor) {
  u16 facing;

  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Func_020005c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessageAndWait(actor, 0);
    Func_020005cc();
  }
}
