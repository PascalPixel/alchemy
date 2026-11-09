#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_020010c8(void) {
  if (Func_02000f94()) {
    Func_02004a7c(8);
  } else {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9fe))
      Event_SetMessageId((s32)MsgShamanMoapaDefeatIncredible);
    else
      Event_SetMessageId((s32)MsgShamanTribeForbidsTalking);
    Event_ShowMessageAndWait(8, 0);
    Func_020048cc();
  }
}

void Func_02001118(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCheckOtherMountainSide);
  Event_AskYesNoAndShowResponse(13, 0);
  Func_020048cc();
}

void Func_0200113c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(0x9ed)) {
    Event_SetMessageId((s32)MsgShamanMoapaNeverForgetsVictor);
    Event_ShowMessageAndWait(11, 0);
  } else {
    Event_SetMessageId((s32)MsgShamanMoapaAsksVictorsNames);
    FieldMessage_ShowAtActor(11, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
      Event_ShowMessageAndWait(11, 0);
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Event_ShowMessageAndWait(11, 0);
      GameFlag_SetBit(0x9ed);
    }
  }
  Func_020048cc();
}

void Func_020011c0(void) {
  if (Func_02000f94()) {
    Func_02004a74(29, 12);
  } else {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanWelcomeContigoHero);
    Event_ShowMessageAndWait(12, 0);
    Func_020048cc();
  }
}

void Func_020011f8(void) {
  Actor_Stop(17);
  wait(1);
  Actor_Jump(17, 4, 20);
  Sound_Play(229);
  wait(2);
  Actor_SetMovementTypeAndWaitById(16, gData_020050f8);
}

void Func_0200122c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x301)) {
    if (GameFlag_Test(0xa21))
      Event_SetMessageId((s32)MsgShamanFortuneAvoidNortheast);
    else
      Event_SetMessageId((s32)MsgShamanFortuneAvoidSouth);
    Event_ShowMessageAndWait(17, 0);
  } else {
    Sound_Play(0);
    if (GameFlag_Test(0xa21))
      Event_SetMessageId((s32)MsgShamanFortuneOfferAfterTrial);
    else
      Event_SetMessageId((s32)MsgShamanFortuneOfferBeforeTrial);
    FieldMessage_ShowAtActor(17, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
      Event_ShowMessageAndWait(17, 0);
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Event_ShowMessageAndWait(17, 0);
      Sound_Play(11);
      Actor_SetModeById(17, 5);
      Event_ShowMessageAndWait(17, 0);
      Func_020011f8();
      Actor_SetModeById(17, 6);
      Event_ShowMessageAndWait(17, 0);
      Func_020011f8();
      Actor_SetModeById(17, 5);
      Event_ShowMessageAndWait(17, 0);
      Func_020011f8();
      Event_Wait(20);
      Actor_SetModeById(17, 1);
      Actor_SetModeById(17, 3);
      Event_ShowMessageAndWait(17, 0);
      Func_080d489c(17, 0x102);
      Event_Wait(40);
      Event_ShowMessageAndWait(17, 0);
      Motion_SetVarCbAndRefresh(17, 1);
      Event_ShowMessageAndWait(17, 0);
      GameFlag_SetBit(0x301);
    }
    Func_02004a2c();
  }
  Func_020048cc();
}

void Func_02001380(void) {
  if (GameFlag_Test(0x200)) {
    Event_SetMessageId((s32)MsgShamanTrialRulesAreSimple);
  } else {
    Sound_Play(78);
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Camera_SetSpeedParameters(0x19999, 0x3333);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 182, 280);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    Sound_Play(76);
    Actor_FaceDirection(15, FACING_WEST, 0);
    Actor_FaceDirection(16, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(17, FACING_WEST);
    Motion_SetModeAndWaitAnimation(15, 3);
    Event_SetMessageId((s32)MsgShamanListenCarefully);
    Event_ShowMessage(0x200f, 0);
    Actor_FaceDirectionAndWait(15, FACING_NORTH);
    Camera_SetSpeedParameters(0x33333, 0x6666);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 1);
    Func_020049f4();
    Event_ShowMessage(0x200f, 0);
    Actor_SetAttachedEffect(15, 0x102);
    Event_ShowMessage(0x200f, 0);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    Func_020049f4();
    Actor_FaceDirectionAndWait(15, FACING_WEST);
    Event_ShowMessage(0x200f, 0);
    FieldCamera_MoveTo(PIXELS(178), -1, PIXELS(220), 1);
    Func_020049f4();
    Event_Wait(10);
    FieldCamera_MoveTo(PIXELS(270), -1, PIXELS(220), 1);
    Func_020049f4();
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0);
    Camera_MoveWithinMapBounds(PIXELS(226), -1, PIXELS(280), 1);
    Func_020049f4();
    Actor_FaceDirectionAndWait(15, FACING_WEST);
    Event_ShowMessageAndWait(0x200f, 0);
  }
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessageAndWait(0x200f, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_SetModeById(16, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_WalkTo(16, 184, 256);
  Actor_WalkToAndWait(17, 264, 256);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(15, FACING_NORTH, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(182), 1);
  Actor_ResetAndSetPositionInMode2(16, 184, 128);
  Actor_WalkToAndWait(17, 264, 128);
  Actor_SetModeById(16, 1);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x9e0);
  GameFlag_SetBit(0x11b);
  Import_EventSetExitRequest(10);
}

void Func_02001644(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Camera_SetSpeedParameters(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(182), -1, PIXELS(304), 1);
  Func_020049f4();
  Event_SetMessageId((s32)MsgShamanGiveUpTrialPrompt);
  FieldMessage_ShowAtActor(15, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_ShowMessageAndWait(15, 0);
    Camera_SetSpeedParameters(0x4cccc, 0x9999);
    Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(368), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 368);
    GameFlag_ClearBit(0x201);
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(15, 0);
    Camera_SetSpeedParameters(0x13333, 0x2666);
    Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(280), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 280);
  }
  Func_020048cc();
}

void Func_02001764(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x1cccc, 0xe666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 182, 280);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(15, FACING_WEST);
  Event_SetMessageId((s32)MsgShamanChallengeTrialRoadPrompt);
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200289c();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      Sound_Play(76);
      Event_ShowMessageAndWait(0x200f, 0);
      GameFlag_SetBit(0x200);
      Func_02001380();
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Func_0200290c();
    }
  }
  Func_020048cc();
}
