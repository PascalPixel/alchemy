#include "shaman_village.h"

extern u32 gData_02005aa0[];
extern u32 gData_02005aac[];
extern u32 gData_02005bc0[];
extern u32 gData_02005ca4[];
extern u32 gData_02005f98[];
extern u32 gData_020061f0[];
extern u32 gData_020062d4[];

void *Func_02000c80(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gData_02005aac;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gData_02005bc0;
  if (scene == (s32)&SceneId_ShamanVillage2)
    return gData_02005ca4;
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gData_02005f98;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_020061f0;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_020062d4;
  return gData_02005aa0;
}

#include "event_inline.h"
#include "scene_tables.h"

void Func_02000d08(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);
  Func_020048cc();
}

void Func_02000d28(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceClosed, 1);
  Func_020048cc();
}

void Func_02000d48(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanTrialRoadPermissionSign, 1);
  Func_020048cc();
}

void Func_02000d68(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanGuardRepeatRulesBeforeTrial);
  FieldMessage_ShowAtActor(17, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(15, 3);
    FieldCamera_SetSpeed(0x19999, 0x3333);
    Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 246, 292);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 204, 292);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 182, 280);
    Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
    Event_ShowMessageAndWait(0x200f, 0);
    GameFlag_SetBit(0x200);
    Func_02001380();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(17, 0);
  }
  Func_020048cc();
}

void Func_02000e3c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOtherGuardRepeatRulesBeforeTrial);
  FieldMessage_ShowAtActor(16, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(15, 3);
    FieldCamera_SetSpeed(0x19999, 0x3333);
    Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 182, 280);
    Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
    Event_ShowMessageAndWait(0x200f, 0);
    GameFlag_SetBit(0x200);
    Func_02001380();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(16, 0);
  }
  Func_020048cc();
}

void Func_02000ef8(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOutsidersLeaveTown);
  Event_ShowMessageAndWait(15, 0);
  work->field15a = 1;
  Func_020048cc();
}

void Func_02000f2c(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanFearMoapaPower);
  Event_ShowMessageAndWait(17, 0);
  work->field15a = 1;
  Func_020048cc();
}

void Func_02000f60(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanSpeakToMoapa);
  Event_ShowMessageAndWait(16, 0);
  work->field15a = 1;
  Func_020048cc();
}

s32 Func_02000f94(void) {
  u16 turn;
  s32 facing;

  turn = ((struct ShamanVillageObject *)Actor_GetById(
              gMainRoutineWork.currentOwner))
             ->facing +
         0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

u32 Func_02004a8c(s32 shop);
void Func_02004a94(u32 price);
void Func_0200487c(u32 value, s32 digits);

void Func_02000fc4(void) {
  if (GameFlag_Test(0x9fe)) {
    if (Func_02000f94()) {
      Func_02004a84(11, 13);
    } else {
      Func_020048c4();
      Event_ResolvePendingActions(0);
      Event_SetMessageId((s32)MsgShamanVisitorsHaveTraveledFar);
      Event_AskYesNoAndShowResponse(13, 0);
      Func_020048cc();
    }
  } else {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    if (Func_02000f94()) {
      u32 price = Func_02004a8c(11);

      Actor_SetModeById(13, 5);
      Event_Wait(30);
      Event_SetMessageId((s32)MsgShamanInnPaymentPrompt);
      Func_0200487c(price, 5);
      Func_0200487c(price, 5);
      FieldMessage_ShowAtActor(12, 0);
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        if ((u32)gMainRoutineWork.coins < price) {
          Actor_SetModeById(13, 6);
          Event_Wait(20);
          Actor_SetModeById(13, 5);
          Event_Wait(30);
          Event_SetMessageId((s32)MsgShamanInnPaymentPrompt + 3);
          Event_ShowMessageAndWait(12, 0);
        } else {
          Func_02004a94(price);
        }
      }
      Actor_SetModeById(13, 6);
    } else {
      Event_SetMessageId((s32)MsgShamanSilentVillager);
      Event_ShowMessageAndWait(13, 0);
    }
    Func_020048cc();
  }
}

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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        1) {
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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        1) {
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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
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

extern const u8 gData_02004c5c[];
extern const u8 gData_02004cac[];

void Func_02001880(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = ((struct ShamanVillageObject *)Actor_GetById(
      gMainRoutineWork.currentOwner));
  shaman = ((struct ShamanVillageObject *)Actor_GetById(15));
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetModeById(16, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(16, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetModeById(15, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 15, 0);
  Actor_FaceDirection(17, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Motion_SetModeAndWaitAnimation(17, 3);
  Actor_SetModeById(17, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(17, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessageAndWait(15, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(200), 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02004c5c);
  Actor_SetMovementTypeById(15, gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  Func_02003040();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(168), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 184, 128);
  Actor_WalkToAndWait(15, 264, 128);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020048b4();
  Import_EventSetExitRequest(11);
}

void Func_02001aa4(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = ((struct ShamanVillageObject *)Actor_GetById(
      gMainRoutineWork.currentOwner));
  shaman = ((struct ShamanVillageObject *)Actor_GetById(15));
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(17, 3);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_SetModeById(17, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(17, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetModeById(15, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 15, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetModeById(16, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(16, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Event_ShowMessageAndWait(15, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(200), 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetMovementTypeById(15, gData_02004c5c);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  Func_02003040();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(168), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_ResetAndSetPositionInMode2(15, 184, 128);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 128);
  Actor_SetModeById(15, 1);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020048b4();
  Import_EventSetExitRequest(12);
}

void Func_02001cc8(void) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Actor_GetById(14));

  if (object->x >> 20 == 32 && object->z >> 20 == 11) {
    Func_020048c4();
    Event_ResolvePendingActions(0);
    object->motionFlags = 3;
    do {
      wait(1);
    } while (object->velocityY != 0);
    Sound_Play(188);
    wait(10);
    object->motionFlags = 0;
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(1);
    GameFlag_SetBit(0x94e);
    Func_020048cc();
  }
}

void Func_02001d3c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  Event_Wait(20);
  Actor_SetPosition(15, PIXELS(120), PIXELS(194));
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x26666, 0x13333);
  Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags &= ~1;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 120, 220);
  Event_Wait(1);
  {
    u8 *flags = &((struct ShamanVillageObject *)Actor_GetById(
                      gMainRoutineWork.currentOwner))
                     ->actionFlags;

    *flags |= 1;
  }
  Actor_SetSpeed(15, 0x6666, 0x3333);
  Actor_SetPositionAndReset(15, 120, 208);
  Func_02004944(16, 15);
  Func_02004944(17, 15);
  wait(1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetSpeed(17, 0xcccc, 0x6666);
  Actor_ResetAndSetPositionInMode2(16, 104, 212);
  Actor_SetPositionAndReset(17, 136, 212);
  Actor_SetModeById(16, 1);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(15, 1);
  Event_SetMessageId((s32)MsgShamanMoapaGiveMeRoom);
  Event_ShowMessageAndWait(15, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)Func_080d4568())->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(120), 0x320000, PIXELS(224), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 120, 256);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags &= ~1;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Event_Wait(1);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags |= 1;
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 13 of 246 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  Event_ShowMessage(0x800f, 0);
  Actor_StartRepeatedMotion(17, 1);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 246 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 259, 0)
  Event_ShowMessageAndWait(0x8011, 0);
  Actor_StartRepeatedMotion(16, 1);
  Func_080d47b4(16, 259, 0);
  Event_ShowMessageAndWait(0x8010, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_ShowMessageAndWait(0x800f, 0);
  GameFlag_SetBit(0x94f);
  Func_020048cc();
}
