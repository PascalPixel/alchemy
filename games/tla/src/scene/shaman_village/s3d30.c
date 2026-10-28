#include "shaman_village.h"
#include "scene_tables.h"

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
  ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(182), 0);
  wait(1);
  Func_0200482c();
  wait(1);
  angle = 0x4000;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  ShamanVillage_ObjPut(16, PIXELS(184), PIXELS(128), angle);
  ShamanVillage_ObjPut(17, PIXELS(264), PIXELS(128), angle);
  ShamanVillage_ObjSpeed(16, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(17, 0x19999, 0xcccc);
  ShamanVillage_CamSpeed(0xcccc, 0x1999);
  ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(280), 1);
  ShamanVillage_ObjWalk2(16, 184, 256);
  ShamanVillage_ObjWalk(17, 264, 256);
  ShamanVillage_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  ShamanVillage_ObjWalk2(16, 208, 264);
  ShamanVillage_ObjWalk(17, 240, 264);
  Actor_SetModeById(16, 1);
  FieldEvent_Wait(10);
  ShamanVillage_ObjArm(16, angle, 0);
  ShamanVillage_ObjArm(17, angle, 0);
  ShamanVillage_ObjTurn(15, FACING_WEST);
  ShamanVillage_ObjArm(gMainRoutineWork.currentOwner, 0, 0);
  Func_020049f4();
  Func_0200498c((s32)MsgShamanMustFinishTrialRoad);
  ShamanVillage_MessagePut(0x200f, 0);
  Actor_SetModeById(15, 3);
  ShamanVillage_MessageOpen(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    ShamanVillage_MessageOpen(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
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
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
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
