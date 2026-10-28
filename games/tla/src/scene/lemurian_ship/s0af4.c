#include "lemurian_ship.h"

void Func_02000af4(void) {
  struct LemurianShipObject *leader;

  if (!GameFlag_Test(0x983)) {
    leader = Actor_GetById(gSequenceParentWork.currentOwner);
    Func_02002c68();
    Func_02002da8(0);
    LemurianShip_ObjSpeed(gSequenceParentWork.currentOwner, 0xcccc, 0x6666);
    LemurianShip_ObjWalk(gSequenceParentWork.currentOwner, 812, 72);
    Func_02002de0(243);
    LemurianShip_ObjEmoteWait(gSequenceParentWork.currentOwner, 0x102);
    Func_02002c48(0, 0x20000, 0x10000);
    FieldEvent_Wait(20);
    Func_02002c48(0, 0, 0);
    LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
    leader->field22 = 2;
    FieldEvent_Wait(20);
    Actor_Jump(gSequenceParentWork.currentOwner, 4, 0);
    LemurianShip_ObjSpeed(gSequenceParentWork.currentOwner, 0x33333, 0x19999);
    LemurianShip_ObjCommit(gSequenceParentWork.currentOwner, 812, 88);
    FieldEvent_Wait(20);
    Func_02002de0(232);
    LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
    FieldEvent_Wait(20);
    leader->field22 = 1;
  }
}

void Func_02000bf0(s32 id) {
  Func_02002c68();
  Func_02002da8(0);
  Actor_SetAngleToward(id, gSequenceParentWork.currentOwner, 0);
  Actor_StartRepeatedMotion(id, 2);
  LemurianShip_ObjEmoteWait(id, 0x102);
  FieldEvent_Wait(20);
  gSequenceParentWork.battleRule24b = 2;
  Func_02002d78(11, 0);
  GameFlag_SetBit(id + 0x2f8);
}

void Func_02000c50(s32 id) {
  Func_02002c68();
  Func_02002da8(0);
  Actor_SetAngleToward(id, gSequenceParentWork.currentOwner, 0);
  Actor_StartRepeatedMotion(id, 2);
  LemurianShip_ObjEmoteWait(id, 0x102);
  FieldEvent_Wait(20);
  gSequenceParentWork.battleRule24b = 2;
  Func_02002d78(11, 0);
  GameFlag_SetBit(id + 0x2f9);
}

void Func_02000cb0(s32 id) {
  Func_02002c68();
  Func_02002da8(0);
  Actor_SetAngleToward(id, gSequenceParentWork.currentOwner, 0);
  Actor_StartRepeatedMotion(id, 2);
  LemurianShip_ObjEmoteWait(id, 0x102);
  FieldEvent_Wait(20);
  gSequenceParentWork.battleRule24b = 2;
  Func_02002d78(11, 0);
  GameFlag_SetBit(id + 0x2ff);
}

void Func_02000d10(s32 id) {
  struct LemurianShipObject *object = Actor_GetById(id);

  LemurianShip_ObjSpeed(id, 0x33333, 0x19999);
  object->field48 = 0x8000;
  object->velocityY = 0x80000;
  Actor_ResetAndSetPosition(id, 120, 120);
}

void Func_02000d44(void) {
  Func_02002c68();
  Func_02002da8(0);
  Func_02002de0(0);
  Actor_StartRepeatedMotion(9, 2);
  Actor_StartRepeatedMotion(10, 2);
  Actor_StartRepeatedMotion(11, 2);
  Actor_StartRepeatedMotion(12, 2);
  Motion_SetVarCbAndRefresh(13, 2);
  FieldEvent_Wait(20);
  Func_02002de0(147);
  Func_02000d10(9);
  Func_02000d10(10);
  Func_02000d10(11);
  Func_02000d10(12);
  Func_02000d10(13);
  Actor_WaitForMoveById(13);
  Func_02002cd0(9, 0, 0);
  Func_02002cd0(10, 0, 0);
  Func_02002cd0(11, 0, 0);
  Func_02002cd0(12, 0, 0);
  Func_02002cd0(13, 0, 0);
  FieldEvent_Wait(20);
  Func_02002de0(52);
  Actor_SetMovementTypeAndWaitById(8, gData_02002e9c);
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party
   * state address before it builds the field offset (2 differ without it). */
  do {
  } while (0);
  gSequenceParentWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov656Room2, 15);
  Func_02002d78(11, 1);
  GameFlag_SetBit(0x30f);
}
