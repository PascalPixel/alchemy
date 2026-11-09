#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_0200282c();
void Func_02002834();

void Func_02000154(s32 arg0) {
  s32 result0;
  s32 result1;

  Event_SetMessageId((s32)MsgLemuriaInteriors13);
  FieldMessage_ShowAtActor(arg0, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_02008184;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgLemuriaInteriors13 + 1));
  goto L_02008190;
L_02008184:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgLemuriaInteriors13 + 2));
L_02008190:
  Event_ShowMessageAndWait(arg0, 0);
  return;
}

void Func_020001a0(s32 arg0) {
  s32 result0;
  s32 result1;

  Event_SetMessageId((s32)MsgLemuriaInteriors18);
  FieldMessage_ShowAtActor(arg0, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_020081d0;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgLemuriaInteriors18 + 1));
  goto L_020081dc;
L_020081d0:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgLemuriaInteriors18 + 2));
L_020081dc:
  Event_ShowMessageAndWait(arg0, 0);
  return;
}

void Func_020001ec(s32 arg0, s32 arg1) {
  struct Actor *object0;
  s32 value1;
  s32 result2;
  s32 result3;

  object0 = Actor_GetById(arg1);
  value1 = object0->angle;
  GameFlag_SetBit(0x967);
  Func_0200282c();
  Event_ResolvePendingActions(0);
  Actor_ShowEmote(arg1, 0x101, 30);
  Actor_SetAngleToward(arg1, 4, 20);
  Event_SetMessageId((s32)MsgLemuriaInteriors19);
  FieldMessage_ShowAtActor(arg1, 0);
  result2 = FieldParty_GetLeaderActor();
  result3 = FieldEvent_AskYesNoWithActorAnimation(result2, 0);
  if (result3 != 0)
    goto L_0200824e;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgLemuriaInteriors19 + 1));
  goto L_0200825a;
L_0200824e:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgLemuriaInteriors19 + 2));
L_0200825a:
  Event_ShowMessageAndWait(arg1, 0);
  Actor_ArmCallback(arg1, value1, 0);
  Event_Wait(10);
  Func_02002834();
  return;
}
