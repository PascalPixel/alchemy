#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Func_020005c4();
void Func_020005cc();
void Func_02000604();
void Func_02000614();
void Func_0200062c();
void Func_02000634();
void Func_0200063c();
void Func_02000644();
void Func_0200064c();
void Event_AskYesNoWithActorAnimation();
struct Actor *Actor_GetById();
void FieldMessage_ShowAtActor();

void Func_020000f4(void) {
  s32 result0;
  s32 result1;

  Func_020005c4();
  Func_0200062c(0);
  Func_02000604((s32)MsgMikasallaInteriors03);
  FieldMessage_ShowAtActor(10, 0);
  result0 = Value0((s32 (*)())Func_02000634);
  result1 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result0, 0);
  if (result1 != 0)
    goto L_0200812c;
  FieldEvent_Wait(10);
  Func_02000604(((s32)MsgMikasallaInteriors03 + 1));
  goto L_02008138;
L_0200812c:
  FieldEvent_Wait(20);
  Func_02000604(((s32)MsgMikasallaInteriors03 + 2));
L_02008138:
  Func_02000614(10, 0);
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
    Func_02000604((s32)MsgMikasallaInteriors11);
    Func_02000614(actor, 0);
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
    Func_0200062c(0);
    Func_02000604((s32)MsgMikasallaInteriors14);
    Func_02000614(actor, 0);
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
    Func_0200062c(0);
    Func_02000604((s32)MsgMikasallaInteriors17);
    FieldMessage_ShowAtActor(actor, 0);
    if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
               Value0((s32 (*)())Func_02000634), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02000604((s32)MsgMikasallaInteriors17 + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02000604((s32)MsgMikasallaInteriors17 + 2);
    }
    Func_02000614(actor, 0);
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
    Func_0200062c(0);
    Func_02000604((s32)MsgMikasallaInteriors29);
    Func_02000614(actor, 0);
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
    Func_0200062c(0);
    Func_02000604((s32)MsgMikasallaInteriors27);
    Func_02000614(actor, 0);
    Func_020005cc();
  }
}
