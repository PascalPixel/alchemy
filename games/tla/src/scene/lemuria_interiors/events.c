#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Func_0200282c();
void Func_02002834();
void Func_020028ac();
void Func_020028c4();
void Func_020028dc();
void Func_020028fc();
void Func_0200291c();
void GameFlag_SetBit();
void Event_AskYesNoWithActorAnimation();
void Actor_ArmCallback();
void Actor_SetAngleToward();
struct Actor *Actor_GetById();
void FieldMessage_ShowAtActor();

void FieldEvent_Wait();
void Func_020028ac();
void Func_020028c4();
void Func_0200291c();
void Event_AskYesNoWithActorAnimation();
void FieldMessage_ShowAtActor();

void Func_02000154(s32 arg0) {
  s32 result0;
  s32 result1;

  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors13);
  Value2((s32 (*)())FieldMessage_ShowAtActor, arg0, 0);
  result0 = Value0((s32 (*)())Func_0200291c);
  result1 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result0, 0);
  if (result1 != 0)
    goto L_02008184;
  Value1((s32 (*)())FieldEvent_Wait, 10);
  Call1((void (*)())Func_020028ac, ((s32)MsgLemuriaInteriors13 + 1));
  goto L_02008190;
L_02008184:
  Value1((s32 (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_020028ac, ((s32)MsgLemuriaInteriors13 + 2));
L_02008190:
  Call2((void (*)())Func_020028c4, arg0, 0);
  return;
}

void Func_020001a0(s32 arg0) {
  s32 result0;
  s32 result1;

  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors18);
  Value2((s32 (*)())FieldMessage_ShowAtActor, arg0, 0);
  result0 = Value0((s32 (*)())Func_0200291c);
  result1 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result0, 0);
  if (result1 != 0)
    goto L_020081d0;
  Value1((s32 (*)())FieldEvent_Wait, 10);
  Call1((void (*)())Func_020028ac, ((s32)MsgLemuriaInteriors18 + 1));
  goto L_020081dc;
L_020081d0:
  Value1((s32 (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_020028ac, ((s32)MsgLemuriaInteriors18 + 2));
L_020081dc:
  Call2((void (*)())Func_020028c4, arg0, 0);
  return;
}

void Func_020001ec(s32 arg0, s32 arg1) {
  struct Actor *object0;
  s32 value1;
  s32 result2;
  s32 result3;

  object0 = Actor_GetById(arg1);
  value1 = object0->angle;
  Call1((void (*)())GameFlag_SetBit, 0x967);
  ((void (*)())Func_0200282c)();
  Call1((void (*)())Func_020028fc, 0);
  Value3((s32 (*)())Func_020028dc, arg1, 0x101, 30);
  Call3((void (*)())Actor_SetAngleToward, arg1, 4, 20);
  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors19);
  Value2((s32 (*)())FieldMessage_ShowAtActor, arg1, 0);
  result2 = Value0((s32 (*)())Func_0200291c);
  result3 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result2, 0);
  if (result3 != 0)
    goto L_0200824e;
  Value1((s32 (*)())FieldEvent_Wait, 10);
  Call1((void (*)())Func_020028ac, ((s32)MsgLemuriaInteriors19 + 1));
  goto L_0200825a;
L_0200824e:
  Value1((s32 (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_020028ac, ((s32)MsgLemuriaInteriors19 + 2));
L_0200825a:
  Call2((void (*)())Func_020028c4, arg1, 0);
  Call3((void (*)())Actor_ArmCallback, arg1, value1, 0);
  Value1((s32 (*)())FieldEvent_Wait, 10);
  ((void (*)())Func_02002834)();
  return;
}
