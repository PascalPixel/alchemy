#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_WaitFrames();
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

  ((void (*)())Func_020005c4)();
  Call1((void (*)())Func_0200062c, 0);
  Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors03);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 10, 0);
  result0 = Value0((s32 (*)())Func_02000634);
  result1 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, result0, 0);
  if (result1 != 0)
    goto L_0200812c;
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call1((void (*)())Func_02000604, ((s32)MsgMikasallaInteriors03 + 1));
  goto L_02008138;
L_0200812c:
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())Func_02000604, ((s32)MsgMikasallaInteriors03 + 2));
L_02008138:
  Call2((void (*)())Func_02000614, 10, 0);
  ((void (*)())Func_020005cc)();
  return;
}

void Func_0200014c(s32 actor) {
  struct Actor *leader;
  u16 facing;

  leader = Actor_GetById(gAdventure.currentOwner);
  /* FAKEMATCH: ⚓️ loads the facing before it builds the quarter turn. */
  __asm__ volatile("" : "+l"(leader));
  facing = (leader->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Call2((void (*)())Func_0200063c, 13, actor);
  } else {
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors11);
    Call2((void (*)())Func_02000614, actor, 0);
  }
}

void Func_0200019c(s32 actor) {
  struct Actor *leader;
  u16 facing;

  leader = Actor_GetById(gAdventure.currentOwner);
  /* FAKEMATCH: ⚓️ loads the facing before it builds the quarter turn. */
  __asm__ volatile("" : "+l"(leader));
  facing = (leader->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Call2((void (*)())Func_0200064c, 4, actor);
  } else {
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors14);
    Call2((void (*)())Func_02000614, actor, 0);
    ((void (*)())Func_020005cc)();
  }
}

void Func_020001fc(s32 actor) {
  struct Actor *leader;
  u16 facing;

  leader = Actor_GetById(gAdventure.currentOwner);
  /* FAKEMATCH: ⚓️ loads the facing before it builds the quarter turn. */
  __asm__ volatile("" : "+l"(leader));
  facing = (leader->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Call1((void (*)())Func_02000644, actor);
  } else {
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors17);
    Value2((s32 (*)())FieldMessage_ShowAtActor, actor, 0);
    if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
               Value0((s32 (*)())Func_02000634), 0) == 0) {
      Call1((void (*)())FieldEvent_WaitFrames, 10);
      Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors17 + 1);
    } else {
      Call1((void (*)())FieldEvent_WaitFrames, 20);
      Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors17 + 2);
    }
    Call2((void (*)())Func_02000614, actor, 0);
    ((void (*)())Func_020005cc)();
  }
}

void Func_02000288(s32 actor) {
  struct Actor *leader;
  u16 facing;

  leader = Actor_GetById(gAdventure.currentOwner);
  /* FAKEMATCH: ⚓️ loads the facing before it builds the quarter turn. */
  __asm__ volatile("" : "+l"(leader));
  facing = (leader->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Call1((void (*)())Func_02000644, actor);
  } else {
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors29);
    Call2((void (*)())Func_02000614, actor, 0);
    ((void (*)())Func_020005cc)();
  }
}

void Func_020002e4(s32 actor) {
  struct Actor *leader;
  u16 facing;

  leader = Actor_GetById(gAdventure.currentOwner);
  /* FAKEMATCH: ⚓️ loads the facing before it builds the quarter turn. */
  __asm__ volatile("" : "+l"(leader));
  facing = (leader->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Call1((void (*)())Func_02000644, actor);
  } else {
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors27);
    Call2((void (*)())Func_02000614, actor, 0);
    ((void (*)())Func_020005cc)();
  }
}
