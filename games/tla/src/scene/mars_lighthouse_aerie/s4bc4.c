#include "mars_lighthouse_aerie.h"

extern const u8 MsgWillYouCast[];

void Func_02003658(void);
void Func_0200567c(s32 id, s32 a1, s32 a2);
void GameFlag_SetBit(s32 flag);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
void WaitFrames(s32 frames);

void Func_02004bc4(void) {
  Func_0200560c();
  Func_02005784(0);
  Func_02005704(19, 0x5000);
  Func_020056dc(MsgDoNotLet);
  Func_020056f4(0x8013, 0);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gAdventure.currentOwner, 480, 348);
  MarsLighthouseAerie_ObjTurn(19, 0xb000);
  Func_02005614();
}

void Func_02004c2c(void) {
  Func_0200560c();
  Func_02005784(0);
  Func_02005704(19, 0x3000);
  Func_020056dc(MsgDoNotLet);
  Func_020056f4(0x8013, 0);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gAdventure.currentOwner, 544, 348);
  MarsLighthouseAerie_ObjTurn(19, 0xb000);
  Func_02005614();
}

void Func_02004c94(void) {
  Func_0200560c();
  Func_02005784(0);
  Func_0200567c(18, 0x2000000, 0xe80000);
  WaitFrames(1);
  Func_020056dc(MsgWillYouCast);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Func_0200567c(18, 0, 0);
    WaitFrames(1);
    Func_02003658();
  }
  Func_0200567c(18, 0, 0);
  WaitFrames(1);
  GameFlag_SetBit(0x202);
  Func_02005614();
}
