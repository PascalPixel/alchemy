#include "shaman_village.h"
#include "scene_tables.h"

void Func_02000d08(void) {
  Func_020048c4();
  Func_02004a24(0);
  FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);
  Func_020048cc();
}

void Func_02000d28(void) {
  Func_020048c4();
  Func_02004a24(0);
  FieldMessage_Show((s32)MsgShamanEntranceClosed, 1);
  Func_020048cc();
}

void Func_02000d48(void) {
  Func_020048c4();
  Func_02004a24(0);
  FieldMessage_Show((s32)MsgShamanTrialRoadPermissionSign, 1);
  Func_020048cc();
}

void Func_02000d68(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanGuardRepeatRulesBeforeTrial);
  FieldMessage_ShowAtActor(17, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(15, 3);
    ShamanVillage_CamSpeed(0x19999, 0x3333);
    ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(280), 1);
    ShamanVillage_ObjSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 246, 292);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 204, 292);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 182, 280);
    ShamanVillage_ObjTurn(gMainRoutineWork.currentOwner, 0);
    ShamanVillage_MessagePut(0x200f, 0);
    GameFlag_SetBit(0x200);
    Func_02001380();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Func_020049a4(17, 0);
  }
  Func_020048cc();
}

void Func_02000e3c(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanOtherGuardRepeatRulesBeforeTrial);
  FieldMessage_ShowAtActor(16, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(15, 3);
    ShamanVillage_CamSpeed(0x19999, 0x3333);
    ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(280), 1);
    ShamanVillage_ObjSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 182, 280);
    ShamanVillage_ObjTurn(gMainRoutineWork.currentOwner, 0);
    ShamanVillage_MessagePut(0x200f, 0);
    GameFlag_SetBit(0x200);
    Func_02001380();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Func_020049a4(16, 0);
  }
  Func_020048cc();
}

void Func_02000ef8(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanOutsidersLeaveTown);
  Func_020049a4(15, 0);
  work->field15a = 1;
  Func_020048cc();
}

void Func_02000f2c(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanFearMoapaPower);
  Func_020049a4(17, 0);
  work->field15a = 1;
  Func_020048cc();
}

void Func_02000f60(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanSpeakToMoapa);
  Func_020049a4(16, 0);
  work->field15a = 1;
  Func_020048cc();
}

s32 Func_02000f94(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}
