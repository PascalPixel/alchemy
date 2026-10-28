#include "shaman_village.h"

void Func_0200482c(void);

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

void Func_02003d30(void) {
  s32 angle;

  GameFlag_ClearBit(0x11b);
  Func_020048c4();
  Func_02004a24(0);
  Func_02004934(16, 0, 0);
  Func_02004934(17, 0, 0);
  Actor_GetById(15)->facing = 0xc000;
  ShamanVillage_CamMove(0xe00000, -1, 0xb60000, 0);
  WaitFrames(1);
  Func_0200482c();
  WaitFrames(1);
  angle = 0x4000;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  ShamanVillage_ObjPut(16, 0xb80000, 0x800000, angle);
  ShamanVillage_ObjPut(17, 0x1080000, 0x800000, angle);
  ShamanVillage_ObjSpeed(16, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(17, 0x19999, 0xcccc);
  ShamanVillage_CamSpeed(0xcccc, 0x1999);
  ShamanVillage_CamMove(0xe00000, -1, 0x1180000, 1);
  ShamanVillage_ObjWalk2(16, 184, 256);
  ShamanVillage_ObjWalk(17, 264, 256);
  ShamanVillage_ObjArm(gAdventure.currentOwner, 0xe000, 0);
  ShamanVillage_ObjWalk2(16, 208, 264);
  ShamanVillage_ObjWalk(17, 240, 264);
  Actor_SetModeById(16, 1);
  FieldEvent_WaitFrames(10);
  ShamanVillage_ObjArm(16, angle, 0);
  ShamanVillage_ObjArm(17, angle, 0);
  ShamanVillage_ObjTurn(15, 0x8000);
  ShamanVillage_ObjArm(gAdventure.currentOwner, 0, 0);
  Func_020049f4();
  Func_0200498c((s32)MsgShamanMustFinishTrialRoad);
  ShamanVillage_MessagePut(0x200f, 0);
  Actor_SetModeById(15, 3);
  ShamanVillage_MessageOpen(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 1) {
    ShamanVillage_MessageOpen(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
      Motion_SetModeAndWaitAnimation(15, 3);
      Import_SoundPlay(76);
      ShamanVillage_MessagePut(0x200f, 0);
      GameFlag_SetBit(0x200);
      Func_02001380();
      return;
    }
    EVENT_WORK->field1c4++;
  } else {
    EVENT_WORK->field1c4 += 2;
  }
  ShamanVillage_MessageOpen(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 1) {
    ShamanVillage_MessagePut(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    ShamanVillage_MessagePut(0x200f, 0);
    Func_0200290c();
  }
  Func_02004a2c();
  GameFlag_ClearBit(0x12f);
  Func_020048cc();
}
