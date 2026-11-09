#include "kibombo.h"
#include "scene_tables.h"

extern u8 gData_02006510[];
extern u8 gData_020066d8[];
extern u8 gData_020069a8[];
extern u8 gData_02006a98[];
extern u8 gData_02006cd8[];
extern u8 gData_020064f8[];
extern u8 gData_02006ef4[];
extern u8 gData_020070d4[];
extern u8 gData_02007434[];
extern u8 gData_020074dc[];
extern u8 gData_02007698[];
extern u8 gData_02006ee8[];
extern const u8 gData_02006324[];

u8 *Func_0200012c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    return gData_02006510;
  if (scene == (s32)&SceneId_Kibombo4)
    return gData_020066d8;
  if (scene == (s32)&SceneId_Kibombo2)
    return gData_020069a8;
  if (scene == (s32)&SceneId_Kibombo5)
    return gData_02006a98;
  if (scene == (s32)&SceneId_Kibombo3) {
    if (GameFlag_Test(0x8ee))
      gData_02006cd8[22] = 0;
    return gData_02006cd8;
  }
  return gData_020064f8;
}

u8 *Func_020001b4(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    return gData_02006ef4;
  if (scene == (s32)&SceneId_Kibombo4)
    return gData_020070d4;
  if (scene == (s32)&SceneId_Kibombo2)
    return gData_02007434;
  if (scene == (s32)&SceneId_Kibombo5)
    return gData_020074dc;
  if (scene == (s32)&SceneId_Kibombo3)
    return gData_02007698;
  return gData_02006ee8;
}

void Func_02000228(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgItLooksLikeThere);
  Event_AskYesNoAndShowResponse(15, 0);
  Func_02005aa4();
}

void Func_0200024c(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYouDidnT);
  Event_AskYesNoAndShowResponse(23, 0);
  Func_02005aa4();
}

void Func_02000270(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNowThatAkafubu);
  Event_ShowMessageAndWait(24, 0);
  Actor_SetAngleToward(24, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(24, 0);
  Kibombo_ObjTurn(24, FACING_SOUTHEAST + FACING_STEP);
  Func_02005aa4();
}

void Func_020002c4(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAreYouGoing);
  Event_AskYesNoAndShowResponse(12, 0);
  Func_02005aa4();
}

s32 Func_020002e8(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

void Func_02000318(void) {
  if (Func_020002e8()) {
    Func_02005c84(17, 20);
  } else {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheWeaponsI);
    Event_ShowMessageAndWait(20, 0);
    Func_02005aa4();
  }
}

void Func_02000350(void) {
  if (Func_020002e8()) {
    Func_02005c84(18, 21);
  } else {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheOnlyDefenses);
    Event_ShowMessageAndWait(21, 0);
    Func_02005aa4();
  }
}

void Func_02000388(void) {
  if (Func_020002e8()) {
    Func_02005c84(19, 22);
  } else {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgIMSorryBut);
    Event_ShowMessageAndWait(22, 0);
    Func_02005aa4();
  }
}

void Func_020003c0(void) {
  if (Func_020002e8()) {
    Func_02005c94(6, 23);
  } else {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDonTWorryAbout);
    Event_ShowMessageAndWait(23, 0);
    Func_02005aa4();
  }
}

void Func_020003f8(void) {
  if (Func_020002e8()) {
    Func_02005c8c(25);
  } else {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboSPeace);
    Event_ShowMessageAndWait(25, 0);
    Func_02005aa4();
  }
}

void Func_0200042c(void) {
  if (Kibombo_FlagCheck(0x9df)) {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Actor_SetAngleToward(36, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Func_02005c8c(36);
    Kibombo_ObjArm(36, FACING_NORTHWEST + FACING_STEP, 0);
    Func_02005aa4();
  } else {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgOhOhMy);
    Event_ShowMessageThenWait(36, 0, 40);
    Event_ShowMessageAndWait(36, 0);
    Actor_SetAngleToward(36, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Func_02005c8c(36);
    Kibombo_ObjArm(36, FACING_NORTHWEST + FACING_STEP, 0);
    GameFlag_SetBit(0x9df);
    Func_02005aa4();
  }
}

void Func_020004dc(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgOhMyBack);
  Event_ShowMessageAndWait(16, 0);
  Func_02005aa4();
}

void Func_02000500(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgISwearThat);
  Event_ShowMessageAndWait(16, 0);
  Func_02005aa4();
}

void Func_02000524(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo2 || scene == (s32)&SceneId_Kibombo5)
    Func_02000590(show, 15, 2);
  else if (scene == (s32)&SceneId_Kibombo3)
    Func_02000590(show, 26, 2);
}

void Func_02000568(void) {
  Func_02000524(1);
  GameFlag_SetBit(0x200);
}

void Func_0200057c(void) {
  Func_02000524(0);
  GameFlag_ClearBit(0x200);
}

void Func_02000590(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct KibomboObject *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_020005d0(void) { Func_02005c6c(gData_02006324); }

void Func_020005e0(void) {
  Map_CopyCellPictureIndicesRect(34, 33, 23, 18, 1, 1);
  Kibombo_BlendStart(34, 33, 1, 1, 23, 18);
  GameFlag_SetBit(0x86a);
}

void Func_02000618(s32 mode) {
  if (mode == 1)
    Func_020005e0();
}

void Func_02000624(void) {
  Map_CopyCellPictureIndicesRect(36, 34, 16, 7, 1, 1);
  Kibombo_BlendStart(36, 34, 1, 1, 16, 7);
  GameFlag_SetBit(0x204);
}

void Func_0200065c(s32 mode) {
  if (mode == 1)
    Func_02000624();
}

void Func_02000668(void) {
  Map_CopyCellPictureIndicesRect(38, 34, 16, 11, 1, 1);
  Kibombo_BlendStart(38, 34, 1, 1, 16, 11);
  GameFlag_SetBit(0x205);
}

void Func_020006a0(s32 mode) {
  if (mode == 1)
    Func_02000668();
}

void Func_020006ac(void) {
  Map_CopyCellPictureIndicesRect(36, 36, 14, 9, 1, 1);
  Kibombo_BlendStart(36, 36, 1, 1, 14, 9);
  GameFlag_SetBit(0x206);
}

void Func_020006e4(s32 mode) {
  if (mode == 1)
    Func_020006ac();
}

void Func_020006f0(void) {
  Map_CopyCellPictureIndicesRect(38, 36, 18, 9, 1, 1);
  Kibombo_BlendStart(38, 36, 1, 1, 18, 9);
  GameFlag_SetBit(0x207);
}

void Func_02000728(s32 mode) {
  if (mode == 1)
    Func_020006f0();
}

void Func_02000734(void) {
  struct KibomboObject *object = Actor_GetById(11);

  if (object->x >> 20 == 6 && object->z >> 20 == 18) {
    Func_02005a9c();
    Event_ResolvePendingActions(0);
    Actor_GetById(11)->motionFlags = 3;
    object->motionFlags = 3;
    do
      wait(1);
    while (object->velocityY != 0);
    Sound_Play(188);
    wait(10);
    object->motionFlags = 0;
    Actor_GetById(11)->motionFlags = 0;
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    GameFlag_SetBit(0x9ea);
    if (!GameFlag_Test(0x8f4))
      Func_02001318();
    Func_02005aa4();
  }
}

void Func_020007c8(void) {
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Camera_SetSpeedParameters(0x9999, 0x1333);
  Camera_MoveWithinMapBounds(PIXELS(88), -1, PIXELS(392), 1);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 72, 392);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 88, 392);
  Actor_ArmCallback(7, FACING_SOUTHWEST, 10);
  Event_SetMessageId((s32)MsgWhWhoAre);
  Event_ShowMessageAndWait(7, 0);
  Func_02005b1c(26, gMainRoutineWork.currentOwner);
  Func_02005b1c(5, gMainRoutineWork.currentOwner);
  Func_02005b1c(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(5, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(6, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(26, gData_02005d18);
  Actor_SetMovementTypeById(5, gData_02005d5c);
  Actor_SetMovementTypeAndWaitById(6, gData_02005da0);
  Event_ShowMessageAndWait(7, 0);
  if (GameFlag_Test(0x911))
    Func_02000978();
  else
    Func_02000b2c();
  Func_02000d74();
  Kibombo_ObjSpeed(26, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(5, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(6, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(5, gData_02005e54);
  Actor_SetMovementTypeById(6, gData_02005e54);
  Actor_SetMovementTypeAndWaitById(26, gData_02005e54);
  GameFlag_SetBit(0x8f3);
  Func_02005aa4();
}

void Func_02000978(void) {
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Event_SetMessageId((s32)MsgItLooksLikeHas);
  FieldMessage_ShowAtActor(6, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Kibombo_ObjArm(5, FACING_SOUTH, 10);
    Actor_SetModeById(5, 3);
    Event_ShowMessageAndWait(5, 0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    Kibombo_ObjArm(5, FACING_SOUTH, 10);
    Actor_SetModeById(5, 4);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(5, 0);
  }
  Func_080d47b4(7, 0x101, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(26, 0x100, 0);
  Event_ShowMessageAndWait(26, 0);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTH, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmote(6, 0x103, 0);
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjEmote(5, 0x103, 10);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(7, 0x103, 10);
  Event_ShowMessageAndWait(7, 0);
}

void Func_02000b2c(void) {
  Event_SetMessageId((s32)MsgHeSeemsTo);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTH, 10);
  Actor_SetModeById(5, 4);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(7, 0x101, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 0);
  Func_080d489c(7, 0x102);
  Event_Wait(20);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageThenWait(26, 0, 20);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(6, 0x109, 0);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmote(6, 0x103, 0);
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjEmote(5, 0x103, 10);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(7, 0x103, 10);
  Event_ShowMessageAndWait(7, 0);
}
