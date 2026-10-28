#include "kibombo.h"

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
  s16 scene = gAdventure.scene;

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
  s16 scene = gAdventure.scene;

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
  Func_02005c3c(0);
  Func_02005b4c((s32)MsgItLooksLikeThere);
  Func_02005b6c(15, 0);
  Func_02005aa4();
}

void Func_0200024c(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b4c((s32)MsgYouDidnT);
  Func_02005b6c(23, 0);
  Func_02005aa4();
}

void Func_02000270(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b4c((s32)MsgNowThatAkafubu);
  Func_02005b64(24, 0);
  ObjectMotion_SetAngleToward(24, gAdventure.currentOwner, 0);
  FieldEvent_WaitFrames(20);
  Func_02005b64(24, 0);
  Kibombo_ObjTurn(24, 0x3000);
  Func_02005aa4();
}

void Func_020002c4(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b4c((s32)MsgAreYouGoing);
  Func_02005b6c(12, 0);
  Func_02005aa4();
}

s32 Func_020002e8(void) {
  u16 turn;
  s32 facing;

  turn = Object_GetById(gAdventure.currentOwner)->facing + 0x5fff;
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
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgTheWeaponsI);
    Func_02005b64(20, 0);
    Func_02005aa4();
  }
}

void Func_02000350(void) {
  if (Func_020002e8()) {
    Func_02005c84(18, 21);
  } else {
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgTheOnlyDefenses);
    Func_02005b64(21, 0);
    Func_02005aa4();
  }
}

void Func_02000388(void) {
  if (Func_020002e8()) {
    Func_02005c84(19, 22);
  } else {
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgIMSorryBut);
    Func_02005b64(22, 0);
    Func_02005aa4();
  }
}

void Func_020003c0(void) {
  if (Func_020002e8()) {
    Func_02005c94(6, 23);
  } else {
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgDonTWorryAbout);
    Func_02005b64(23, 0);
    Func_02005aa4();
  }
}

void Func_020003f8(void) {
  if (Func_020002e8()) {
    Func_02005c8c(25);
  } else {
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgKibomboSPeace);
    Func_02005b64(25, 0);
    Func_02005aa4();
  }
}

void Func_0200042c(void) {
  if (Kibombo_FlagCheck(0x9df)) {
    Func_02005a9c();
    Func_02005c3c(0);
    ObjectMotion_SetAngleToward(36, gAdventure.currentOwner, 0);
    FieldEvent_WaitFrames(10);
    Func_02005c8c(36);
    Kibombo_ObjArm(36, 0xb000, 0);
    Func_02005aa4();
  } else {
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgOhOhMy);
    Func_02005b5c(36, 0, 40);
    Func_02005b64(36, 0);
    ObjectMotion_SetAngleToward(36, gAdventure.currentOwner, 0);
    FieldEvent_WaitFrames(10);
    Func_02005c8c(36);
    Kibombo_ObjArm(36, 0xb000, 0);
    Kibombo_FlagSet(0x9df);
    Func_02005aa4();
  }
}

void Func_020004dc(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b4c((s32)MsgOhMyBack);
  Func_02005b64(16, 0);
  Func_02005aa4();
}

void Func_02000500(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b4c((s32)MsgISwearThat);
  Func_02005b64(16, 0);
  Func_02005aa4();
}

void Func_02000524(s32 show) {
  s16 scene = gAdventure.scene;

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

  for (i = 0; i < count;) {
    u8 *flags = &Object_GetById(first + i)->priorityFlags;
    u32 next = i + 1;

    if (show)
      *flags &= ~0x10;
    else
      /* FAKEMATCH: the native ties the flag or to the constant's register. */
      *flags = *flags + 0x10 - (*flags & 0x10);
    i = next;
  }
}

void Func_020005d0(void) { Func_02005c6c(gData_02006324); }

void Func_020005e0(void) {
  Func_02005a64(34, 33, 23, 18, 1, 1);
  Kibombo_BlendStart(34, 33, 1, 1, 23, 18);
  GameFlag_SetBit(0x86a);
}

void Func_02000618(s32 mode) {
  if (mode == 1)
    Func_020005e0();
}

void Func_02000624(void) {
  Func_02005a64(36, 34, 16, 7, 1, 1);
  Kibombo_BlendStart(36, 34, 1, 1, 16, 7);
  GameFlag_SetBit(0x204);
}

void Func_0200065c(s32 mode) {
  if (mode == 1)
    Func_02000624();
}

void Func_02000668(void) {
  Func_02005a64(38, 34, 16, 11, 1, 1);
  Kibombo_BlendStart(38, 34, 1, 1, 16, 11);
  GameFlag_SetBit(0x205);
}

void Func_020006a0(s32 mode) {
  if (mode == 1)
    Func_02000668();
}

void Func_020006ac(void) {
  Func_02005a64(36, 36, 14, 9, 1, 1);
  Kibombo_BlendStart(36, 36, 1, 1, 14, 9);
  GameFlag_SetBit(0x206);
}

void Func_020006e4(s32 mode) {
  if (mode == 1)
    Func_020006ac();
}

void Func_020006f0(void) {
  Func_02005a64(38, 36, 18, 9, 1, 1);
  Kibombo_BlendStart(38, 36, 1, 1, 18, 9);
  GameFlag_SetBit(0x207);
}

void Func_02000728(s32 mode) {
  if (mode == 1)
    Func_020006f0();
}

void Func_02000734(void) {
  struct KibomboObject *object = Object_GetById(11);

  if (object->x >> 20 == 6 && object->z >> 20 == 18) {
    Func_02005a9c();
    Func_02005c3c(0);
    Object_GetById(11)->motionFlags = 3;
    object->motionFlags = 3;
    do
      WaitFrames(1);
    while (object->velocityY != 0);
    Func_02005c9c(188);
    WaitFrames(10);
    object->motionFlags = 0;
    Object_GetById(11)->motionFlags = 0;
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    GameFlag_SetBit(0x9ea);
    if (!GameFlag_Test(0x8f4))
      Func_02001318();
    Func_02005aa4();
  }
}

void Func_020007c8(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Func_02005ba4(0x9999, 0x1333);
  Kibombo_CamMove(0x580000, -1, 0x1880000, 1);
  Kibombo_ObjSpeed(gAdventure.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gAdventure.currentOwner, 72, 392);
  Kibombo_ObjWalk(gAdventure.currentOwner, 88, 392);
  Kibombo_ObjArm(7, 0x6000, 10);
  Func_02005b4c((s32)MsgWhWhoAre);
  Func_02005b64(7, 0);
  Func_02005b1c(26, gAdventure.currentOwner);
  Func_02005b1c(5, gAdventure.currentOwner);
  Func_02005b1c(6, gAdventure.currentOwner);
  WaitFrames(1);
  ObjectMotion_SetActionVariant(26, 2);
  ObjectMotion_SetActionVariant(5, 2);
  ObjectMotion_SetActionVariant(6, 2);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(5, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(6, 0xcccc, 0x6666);
  ObjectMotion_EnableActionAndSetCallback(26, gData_02005d18);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02005d5c);
  Object_SetActionCallbackAndRefreshById(6, gData_02005da0);
  Func_02005b64(7, 0);
  if (GameFlag_Test(0x911))
    Func_02000978();
  else
    Func_02000b2c();
  Func_02000d74();
  Kibombo_ObjSpeed(26, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(5, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(6, 0x19999, 0xcccc);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02005e54);
  ObjectMotion_EnableActionAndSetCallback(6, gData_02005e54);
  Object_SetActionCallbackAndRefreshById(26, gData_02005e54);
  GameFlag_SetBit(0x8f3);
  Func_02005aa4();
}

void Func_02000978(void) {
  Kibombo_ObjArm(gAdventure.currentOwner, 0x8000, 20);
  Func_02005b4c((s32)MsgItLooksLikeHas);
  UiText_OpenMessageAtObject(6, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Kibombo_ObjArm(5, 0x4000, 10);
    Object_SetModeById(5, 3);
    Func_02005b64(5, 0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    Kibombo_ObjArm(5, 0x4000, 10);
    Object_SetModeById(5, 4);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Func_02005b64(5, 0);
  }
  Kibombo_ObjEmote(7, 0x101, 20);
  Func_02005b64(7, 0);
  Kibombo_ObjArm(gAdventure.currentOwner, 0x2000, 0);
  Kibombo_ObjArm(26, 0xb000, 0);
  Kibombo_ObjArm(5, 0x6000, 0);
  Kibombo_ObjArm(6, 0xe000, 40);
  Kibombo_ObjArm(gAdventure.currentOwner, 0xe000, 0);
  Kibombo_ObjArm(26, 0xd000, 0);
  Kibombo_ObjArm(5, 0, 0);
  Kibombo_ObjArm(6, 0xe000, 20);
  Func_02005b64(7, 0);
  Kibombo_ObjEmote(26, 0x100, 0);
  Func_02005b64(26, 0);
  Func_02005b64(7, 0);
  Func_02005b64(6, 0);
  Kibombo_ObjArm(5, 0x4000, 40);
  Kibombo_ObjArm(5, 0, 0);
  Func_02005b64(5, 0);
  Func_02005b64(7, 0);
  Kibombo_ObjEmote(6, 0x103, 0);
  ObjectMotion_Launch(6, 2, 10);
  ObjectMotion_Launch(6, 4, 10);
  Func_02005b64(6, 0);
  Kibombo_ObjEmote(5, 0x103, 10);
  Func_02005b64(5, 0);
  Kibombo_ObjEmote(7, 0x103, 10);
  Func_02005b64(7, 0);
}

void Func_02000b2c(void) {
  Func_02005b4c((s32)MsgHeSeemsTo);
  Func_02005b64(6, 0);
  Kibombo_ObjArm(5, 0x4000, 10);
  Object_SetModeById(5, 4);
  Func_02005b64(5, 0);
  Kibombo_ObjEmote(7, 0x101, 20);
  Func_02005b64(7, 0);
  Kibombo_ObjArm(gAdventure.currentOwner, 0x2000, 0);
  Kibombo_ObjArm(26, 0xb000, 0);
  Kibombo_ObjArm(5, 0x6000, 0);
  Kibombo_ObjArm(6, 0xe000, 40);
  Kibombo_ObjArm(gAdventure.currentOwner, 0xe000, 0);
  Kibombo_ObjArm(26, 0xd000, 0);
  Kibombo_ObjArm(5, 0, 0);
  Kibombo_ObjArm(6, 0xe000, 0);
  Func_02005b9c(7, 0x102);
  FieldEvent_WaitFrames(20);
  Func_02005b64(7, 0);
  Func_02005b5c(26, 0, 20);
  Func_02005b64(7, 0);
  Kibombo_ObjEmote(6, 0x109, 0);
  Func_02005b64(6, 0);
  Kibombo_ObjArm(5, 0x6000, 40);
  Kibombo_ObjArm(5, 0, 0);
  Func_02005b64(5, 0);
  Func_02005b64(7, 0);
  Kibombo_ObjEmote(6, 0x103, 0);
  ObjectMotion_Launch(6, 2, 10);
  ObjectMotion_Launch(6, 4, 10);
  Func_02005b64(6, 0);
  Kibombo_ObjEmote(5, 0x103, 10);
  Func_02005b64(5, 0);
  Kibombo_ObjEmote(7, 0x103, 10);
  Func_02005b64(7, 0);
}
