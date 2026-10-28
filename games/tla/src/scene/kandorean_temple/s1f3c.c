#include "local.h"

void Func_02001f3c(void) {
  Func_02005170(39, 39, 24, 38, 2, 2);
  WaitFrames(10);
  Func_02005170(39, 41, 24, 38, 2, 2);
  WaitFrames(20);
  GameFlag_SetBit(0x212);
}

void Func_02001f7c(void) {
  Func_020051c0();
  Func_02005370(0);
  ObjectMotion_SetSpeedParameters_(gAdventure.currentOwner, 0x10000, 0x8000);
  ObjectMotion_SetPositionAndReset_(gAdventure.currentOwner, 400, 648);
  Func_020052b0(gAdventure.currentOwner, 0xc000);
  if (GameFlag_Test(0x212) == 0) {
    Func_020053d8(188);
    Func_02001f3c();
  }
  Object_SetModeById(gAdventure.currentOwner, 2);
  ObjectMotion_SetSpeedParameters_(gAdventure.currentOwner, 0x8000, 0x4000);
  ObjectMotion_SetActionVariant(gAdventure.currentOwner, 3);
  ObjectMotion_ResetAndSetPositionInMode2_(gAdventure.currentOwner, 400, 640);
  Func_020053d8(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(9);
}

void Func_02002014(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  u32 i;

  Func_020051c0();
  Func_02005370(0);
  ObjectMotion_SetPositionAndReset(gAdventure.currentOwner, 416, 288);
  ObjectMotion_ArmCallback(gAdventure.currentOwner, 0x4000, 10);
  ObjectMotion_ArmCallback(gAdventure.currentOwner, 0xc000, 0);
  leader->motionFlags = 0;
  ObjectMotion_SetActionVariantValue(gAdventure.currentOwner, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     0);
  for (i = 0; i < 8; i++) {
    leader->z += 0x10000;
    WaitFrames(1);
  }
  Object_SetModeById(gAdventure.currentOwner, 13);
  for (i = 0; i < 8; i++) {
    leader->y -= 0x10000;
    WaitFrames(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(4);
}

void Func_020020c8(void) { Func_020053c0(gData_020054dc); }

void Func_020020d8(void) {
  s32 x = Object_GetById(12)->x >> 20;

  Func_020053c8();
  if (x == 43)
    GameFlag_SetBit(0x211);
  else
    GameFlag_ClearBit(0x211);
  if (x != 39)
    Func_02005178_(39, 0, 1, 1, 39, 10);
}

void Func_02002120(void) {
  if ((u32)((Object_GetById(8)->x >> 20) - 15) > 6)
    GameFlag_SetBit(0x20a);
  GameFlag_SetBit(0x20b);
  Func_020053c0(gData_020054e0);
}

void Func_02002154(void) {
  struct ObjectRuntime *object = Object_GetById(8);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  Func_020053c8();
  if (x == 13 && z == 13)
    GameFlag_SetBit(0x300);
  else
    GameFlag_ClearBit(0x300);
  if (x == 23 && z == 15)
    GameFlag_SetBit(0x301);
  else
    GameFlag_ClearBit(0x301);
  GameFlag_ClearBit(0x20a);
  GameFlag_ClearBit(0x20b);
}

void Func_020021b8(void) {
  GameFlag_ClearBit(0x302);
  GameFlag_ClearBit(0x303);
  Func_020053c0(gData_020054e0);
}

void Func_020021dc(void) {
  s32 x = Object_GetById(8)->x >> 20;

  Func_020053c8();
  if (x == 19) {
    GameFlag_SetBit(0x302);
    GameFlag_SetBit(0x304);
    Func_02005178(0, 0, 3, 3, x, 28);
  } else if (x == 21) {
    GameFlag_SetBit(0x303);
    GameFlag_SetBit(0x304);
    Func_02005178_(0, 4, 3, 3, 19, 28);
  }
}

void Func_02002248(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);

  Func_020051c0();
  Func_02005370(0);
  Func_020052c8(gAdventure.currentOwner, 0x100, 20);
  Func_020053d8(132);
  Object_SetPartAttribute(Object_GetById(gAdventure.currentOwner), 10);
  leader->velocityY = 0x60000;
  WaitFrames(20);
  while (leader->velocityY != 0)
    WaitFrames(1);
  Object_SetPartAttribute(Object_GetById(gAdventure.currentOwner), 0);
  Func_020051c8();
}

void Func_020022c0(void) {
  Func_020051c0();
  Func_02005370(0);
  Func_020053d8(188);
  Func_02005170(77, 33, 77, 24, 2, 2);
  Func_02005178_(32, 30, 2, 1, 17, 25);
  WaitFrames(10);
  GameFlag_SetBit(0x212);
  Func_020051c8();
}

void Func_02002314(void) {
  struct ObjectRuntime *object;

  Func_02005178_(0, 0, 1, 1, 5, 13);
  GameFlag_SetBit(0x842);
  Func_020051c0();
  Func_02005370(0);
  ObjectMotion_SetPositionAndReset(4, 89, 237);
  ObjectMotion_ArmCallback_(4, 0x2000, 0);
  Func_02005378(14, 16, 16, 0xa000);
  ObjectMotion_CommitCurrentPositionAndActivate(14);
  FieldEvent_WaitFrames(20);
  Func_02005280((s32)&MsgKandoreanTemple12);
  Func_02005298(14, 0);
  FieldEvent_WaitFrames(30);
  Object_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_WaitFrames(20);
  ObjectMotion_SetSpeedParameters_(14, 0x13333, 0x9999);
  Object_SetModeById(14, 2);
  object = Object_GetById(4);
  if (object)
    ObjectMotion_ResetAndSetPosition(14, *(s16 *)((u8 *)object + 10),
                                     *(s16 *)((u8 *)object + 18));
  ObjectMotion_CommitCurrentPositionAndActivate(14);
  Func_02005230(14, 0, 0);
  Func_020051c8();
}
