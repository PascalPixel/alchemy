#include "../../../../../../games/tla/src/scene/shaman_village/shaman_village.h"
#include "scene_tables.h"

void Func_020010c8(void) {
  if (Func_02000f94()) {
    Func_02004a7c(8);
  } else {
    Func_020048c4();
    Func_02004a24(0);
    if (GameFlag_Test(SPLIT_CONSTANT(0x900, 254)))
      Func_0200498c((s32)MsgShamanMoapaDefeatIncredible);
    else
      Func_0200498c((s32)MsgShamanTribeForbidsTalking);
    Func_020049a4(8, 0);
    Func_020048cc();
  }
}

void Func_02001118(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanCheckOtherMountainSide);
  Func_020049ac(13, 0);
  Func_020048cc();
}

void Func_0200113c(void) {
  Func_020048c4();
  Func_02004a24(0);
  if (ShamanVillage_FlagCheck(0x9ed)) {
    Func_0200498c((s32)MsgShamanMoapaNeverForgetsVictor);
    Func_020049a4(11, 0);
  } else {
    Func_0200498c((s32)MsgShamanMoapaAsksVictorsNames);
    FieldMessage_ShowAtActor(11, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
      Func_020049a4(11, 0);
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Func_020049a4(11, 0);
      GameFlag_SetBit(SPLIT_CONSTANT(0x900, 237));
    }
  }
  Func_020048cc();
}

void Func_020011c0(void) {
  if (Func_02000f94()) {
    Func_02004a74(29, 12);
  } else {
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanWelcomeContigoHero);
    Func_020049a4(12, 0);
    Func_020048cc();
  }
}

void Func_020011f8(void) {
  Actor_Stop(17);
  wait(1);
  Actor_Jump(17, 4, 20);
  Import_SoundPlay(229);
  wait(2);
  Actor_SetMovementTypeAndWaitById(16, gData_020050f8);
}

void Func_0200122c(void) {
  Func_020048c4();
  Func_02004a24(0);
  if (GameFlag_Test(SPLIT_CONSTANT(0x300, 1))) {
    if (GameFlag_Test(SPLIT_CONSTANT(0xa00, 33)))
      Func_0200498c((s32)MsgShamanFortuneAvoidNortheast);
    else
      Func_0200498c((s32)MsgShamanFortuneAvoidSouth);
    Func_020049a4(17, 0);
  } else {
    Import_SoundPlay(0);
    if (GameFlag_Test(SPLIT_CONSTANT(0xa00, 33)))
      Func_0200498c((s32)MsgShamanFortuneOfferAfterTrial);
    else
      Func_0200498c((s32)MsgShamanFortuneOfferBeforeTrial);
    FieldMessage_ShowAtActor(17, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
      Func_020049a4(17, 0);
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Func_020049a4(17, 0);
      Import_SoundPlay(11);
      Actor_SetModeById(17, 5);
      Func_020049a4(17, 0);
      Func_020011f8();
      Actor_SetModeById(17, 6);
      Func_020049a4(17, 0);
      Func_020011f8();
      Actor_SetModeById(17, 5);
      Func_020049a4(17, 0);
      Func_020011f8();
      FieldEvent_Wait(20);
      Actor_SetModeById(17, 1);
      Actor_SetModeById(17, 3);
      Func_020049a4(17, 0);
      Func_020049d4(17, 0x102);
      FieldEvent_Wait(40);
      Func_020049a4(17, 0);
      Motion_SetVarCbAndRefresh(17, 1);
      Func_020049a4(17, 0);
      GameFlag_SetBit(SPLIT_CONSTANT(0x300, 1));
    }
    Func_02004a2c();
  }
  Func_020048cc();
}

void Func_02001380(void) {
  if (GameFlag_Test(0x200)) {
    Func_0200498c((s32)MsgShamanTrialRulesAreSimple);
  } else {
    Import_SoundPlay(78);
    Func_020048c4();
    Func_02004a24(0);
    Func_020049e4(0x19999, SPLIT_CONSTANT(0x3300, 51));
    ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 182, 280);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    Import_SoundPlay(76);
    ShamanVillage_ObjArm(15, FACING_WEST, 0);
    ShamanVillage_ObjArm(16, FACING_WEST, 0);
    ShamanVillage_ObjTurn(17, FACING_WEST);
    Motion_SetModeAndWaitAnimation(15, 3);
    Func_0200498c((s32)MsgShamanListenCarefully);
    ShamanVillage_MessagePut(0x200f, 0);
    ShamanVillage_ObjTurn(15, FACING_NORTH);
    Func_020049e4(0x33333, 0x6666);
    ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(182), 1);
    Func_020049f4();
    ShamanVillage_MessagePut(0x200f, 0);
    ShamanVillage_ObjEmoteWait(15, 0x102);
    ShamanVillage_MessagePut(0x200f, 0);
    ShamanVillage_CamMove(PIXELS(224), -1, PIXELS(280), 1);
    Func_020049f4();
    ShamanVillage_ObjTurn(15, FACING_WEST);
    ShamanVillage_MessagePut(SPLIT_CONSTANT(0x2000, 15), 0);
    ShamanVillage_CamMove(PIXELS(178), -1, PIXELS(220), 1);
    Func_020049f4();
    FieldEvent_Wait(10);
    ShamanVillage_CamMove(PIXELS(270), -1, PIXELS(220), 1);
    Func_020049f4();
    FieldEvent_Wait(20);
    Func_020049a4(15, 0);
    Camera_MoveWithinMapBounds(PIXELS(226), -1, PIXELS(280), 1);
    Func_020049f4();
    ShamanVillage_ObjTurn(15, FACING_WEST);
    Func_020049a4(0x200f, 0);
  }
  ShamanVillage_ObjTurn(15, FACING_NORTH);
  Func_020049a4(0x200f, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  ShamanVillage_ObjArm(16, FACING_SOUTHEAST, 0);
  ShamanVillage_ObjTurn(17, FACING_SOUTHWEST);
  Actor_SetModeById(16, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  ShamanVillage_ObjSpeed(16, 0x19999, 0xcccc);
  ShamanVillage_ObjSpeed(17, 0x19999, 0xcccc);
  ShamanVillage_ObjWalk2(16, 184, 256);
  ShamanVillage_ObjWalk(17, 264, 256);
  ShamanVillage_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  ShamanVillage_ObjArm(15, FACING_NORTH, 0);
  Func_020049e4(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(182), 1);
  Actor_ResetAndSetPositionInMode2(16, 184, 128);
  ShamanVillage_ObjWalk(17, 264, 128);
  Actor_SetModeById(16, 1);
  wait(1);
  Func_02004934(16, 0, 0);
  Func_02004934(17, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x9e0);
  GameFlag_SetBit(0x11b);
  Func_02004a04(10);
}

void Func_02001644(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_020049e4(0x33333, SPLIT_CONSTANT(0x6600, 102));
  ShamanVillage_CamMove(PIXELS(182), -1, PIXELS(304), 1);
  Func_020049f4();
  Func_0200498c((s32)MsgShamanGiveUpTrialPrompt);
  FieldMessage_ShowAtActor(15, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_020049a4(15, 0);
    Func_020049e4(0x4cccc, SPLIT_CONSTANT(0x9900, 153));
    Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(368), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 168, 368);
    GameFlag_ClearBit(SPLIT_CONSTANT(0x102, 255));
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Func_020049a4(15, 0);
    Func_020049e4(0x13333, 0x2666);
    Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(280), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 168, 280);
  }
  Func_020048cc();
}

void Func_02001764(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_020049e4(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x1cccc, 0xe666);
  ShamanVillage_ObjWalk(gMainRoutineWork.currentOwner, 182, 280);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  ShamanVillage_ObjArm(16, FACING_WEST, 0);
  ShamanVillage_ObjArm(17, FACING_WEST, 0);
  Func_020049bc(15, FACING_WEST);
  Func_0200498c((s32)MsgShamanChallengeTrialRoadPrompt);
  ShamanVillage_MessageOpen(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Func_020049a4(SPLIT_CONSTANT(0x2000, 15), 0);
    Func_0200289c();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    ShamanVillage_MessageOpen(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      Import_SoundPlay(76);
      Func_020049a4(SPLIT_CONSTANT(0x2000, 15), 0);
      GameFlag_SetBit(0x200);
      Func_02001380();
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Func_0200290c();
    }
  }
  Func_020048cc();
}
