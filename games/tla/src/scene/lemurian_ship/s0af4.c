#include "lemurian_ship.h"
#include "event_inline.h"

void Func_02000af4(void) {
  struct LemurianShipObject *leader;

  if (!GameFlag_Test(0x983)) {
    leader = ((struct LemurianShipObject *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    Func_02002c68();
    Event_ResolvePendingActions(0);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 812, 72);
    Sound_Play(243);
    Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
    Map_SetScreenShake(0, 0x20000, 0x10000);
    Event_Wait(20);
    Map_SetScreenShake(0, 0, 0);
    Map_CopyCellPictureIndicesRect(68, 68, 50, 64, 3, 3);
    leader->field22 = 2;
    Event_Wait(20);
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x33333, 0x19999);
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 812, 88);
    Event_Wait(20);
    Sound_Play(232);
    Map_CopyCellPictureIndicesRect(72, 68, 50, 64, 3, 3);
    Event_Wait(20);
    leader->field22 = 1;
  }
}

void Func_02000bf0(s32 id) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(id, gMainRoutineWork.currentOwner, 0);
  Actor_StartRepeatedMotion(id, 2);
  Actor_SetAttachedEffect(id, 0x102);
  Event_Wait(20);
  gMainRoutineWork.battleRule24b = 2;
  Func_02002d78(11, 0);
  GameFlag_SetBit(id + 0x2f8);
}

void Func_02000c50(s32 id) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(id, gMainRoutineWork.currentOwner, 0);
  Actor_StartRepeatedMotion(id, 2);
  Actor_SetAttachedEffect(id, 0x102);
  Event_Wait(20);
  gMainRoutineWork.battleRule24b = 2;
  Func_02002d78(11, 0);
  GameFlag_SetBit(id + 0x2f9);
}

void Func_02000cb0(s32 id) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(id, gMainRoutineWork.currentOwner, 0);
  Actor_StartRepeatedMotion(id, 2);
  Actor_SetAttachedEffect(id, 0x102);
  Event_Wait(20);
  gMainRoutineWork.battleRule24b = 2;
  Func_02002d78(11, 0);
  GameFlag_SetBit(id + 0x2ff);
}

void Func_02000d10(s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  Actor_SetSpeedParameters(id, 0x33333, 0x19999);
  object->field48 = 0x8000;
  object->velocityY = 0x80000;
  Actor_ResetAndSetPosition(id, 120, 120);
}

void Func_02000d44(void) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Sound_Play(0);
  Actor_StartRepeatedMotion(9, 2);
  Actor_StartRepeatedMotion(10, 2);
  Actor_StartRepeatedMotion(11, 2);
  Actor_StartRepeatedMotion(12, 2);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(20);
  Sound_Play(147);
  Func_02000d10(9);
  Func_02000d10(10);
  Func_02000d10(11);
  Func_02000d10(12);
  Func_02000d10(13);
  Actor_WaitForMoveById(13);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Event_Wait(20);
  Sound_Play(52);
  Actor_SetMovementTypeAndWaitById(8, gData_02002e9c);

  gMainRoutineWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov656Room2, 15);
  Func_02002d78(11, 1);
  GameFlag_SetBit(0x30f);
}
