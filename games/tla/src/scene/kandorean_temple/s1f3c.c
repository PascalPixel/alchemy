#include "local.h"
#include "event_inline.h"

void Func_02001f3c(void) {
  Map_CopyCellPictureIndicesRect(39, 39, 24, 38, 2, 2);
  wait(10);
  Map_CopyCellPictureIndicesRect(39, 41, 24, 38, 2, 2);
  wait(20);
  GameFlag_SetBit(0x212);
}

void Func_02001f7c(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 400, 648);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  if (GameFlag_Test(0x212) == 0) {
    Sound_Play(188);
    Func_02001f3c();
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 400, 640);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(9);
}

void Func_02002014(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;

  Func_020051c0();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 416, 288);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  for (i = 0; i < 8; i++) {
    leader->z += PIXELS(1);
    wait(1);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  for (i = 0; i < 8; i++) {
    leader->y -= PIXELS(1);
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(4);
}

void Func_020020c8(void) { Func_020053c0(gData_020054dc); }

void Func_020020d8(void) {
  s32 x = Actor_GetById(12)->x >> 20;

  Func_020053c8();
  if (x == 43)
    GameFlag_SetBit(0x211);
  else
    GameFlag_ClearBit(0x211);
  if (x != 39)
    MapGrid_CopyCellAttributes(39, 0, 1, 1, 39, 10);
}

void Func_02002120(void) {
  if ((u32)((Actor_GetById(8)->x >> 20) - 15) > 6)
    GameFlag_SetBit(0x20a);
  GameFlag_SetBit(0x20b);
  Func_020053c0(gData_020054e0);
}

void Func_02002154(void) {
  struct Actor *object = Actor_GetById(8);
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
  s32 x = Actor_GetById(8)->x >> 20;

  Func_020053c8();
  if (x == 19) {
    GameFlag_SetBit(0x302);
    GameFlag_SetBit(0x304);
    Map_CopyCellAttributeRect(0, 0, 3, 3, x, 28);
  } else if (x == 21) {
    GameFlag_SetBit(0x303);
    GameFlag_SetBit(0x304);
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
}

void Func_02002248(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_020051c0();
  Event_ResolvePendingActions(0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x100, 20);
  Sound_Play(132);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 10);
  leader->velocityY = PIXELS(6);
  wait(20);
  while (leader->velocityY != 0)
    wait(1);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_020051c8();
}

void Func_020022c0(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Sound_Play(188);
  Map_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
  MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  wait(10);
  GameFlag_SetBit(0x212);
  Func_020051c8();
}

void Func_02002314(void) {
  struct Actor *object;

  MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  GameFlag_SetBit(0x842);
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(4, 89, 237);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Func_080d3070(14, 16, 16, FACING_NORTHWEST);
  Actor_WaitForMoveById(14);
  Event_Wait(20);
  Event_SetMessageId((s32)&MsgKandoreanTemple12);
  Event_ShowMessageAndWait(14, 0);
  Event_Wait(30);
  Actor_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(14, 0x13333, 0x9999);
  Actor_SetModeById(14, 2);
  object = Actor_GetById(4);
  if (object)
    Actor_ResetAndSetPosition(14, *(s16 *)((u8 *)object + 10),
                              *(s16 *)((u8 *)object + 18));
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Func_020051c8();
}
