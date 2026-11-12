/// @file
/// Scene code of overlay 0x64f, in ROM order, from events_b's offset up to the next object the overlay links.

#include "local.h"
#include "event_inline.h"
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"

void Func_02000110(void) {
  if (gMainRoutineWork.entrance == 99)
    GameFlag_SetBit(0x894);
  if (GameFlag_Test(0x894)) {
    Actor_SetPosition(8, PIXELS(456), PIXELS(312));
    Func_02000a98(8, 0x4000, 0);
    Actor_SetActionCallback(Actor_GetById(8), 8);
  }
}

void Func_02000168(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  if (gMainRoutineWork.entrance == 99)
    GameFlag_SetBit(0x895);
  if (GameFlag_Test(0x895)) {
    Actor_SetPosition(13, PIXELS(590), PIXELS(920));
    Actor_SetPosition(14, PIXELS(624), PIXELS(939));
    Actor_SetHorizontalPositionWithTerrain(15, PIXELS(632), PIXELS(920));
    Actor_GetById(13)->angle = 0x4000;
    Func_02000a90(13, 0x6000);
    Actor_GetById(14)->angle = 0;
    Func_02000a90(14, 0);
    Actor_GetById(15)->angle = 0xc000;
    Func_02000a90(15, 0);
    Actor_GetById(13)->collisionFlags = 1;
    Actor_GetById(14)->collisionFlags = 1;
    Actor_GetById(15)->collisionFlags = 1;
  }
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 4);
  Actor_SetModeById(10, 3);
  Actor_SetModeById(11, 4);
  Actor_SetModeById(12, 3);
}

void Func_02000264(void) {
  if (GameFlag_Test(0x109) == 0 && gMainRoutineWork.entrance == 99)
    Func_02000800();
}

extern char SceneId_SuhallaGate1;
extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;

void Func_02000110(void);
void Func_02000168(void);
void Func_02000264(void);

s32 Func_0200028c(void) {
  struct EventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 256;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_SuhallaGate1)
    Func_02000110();
  else if (scene == (s32)&SceneId_SuhallaGate2)
    Func_02000168();
  else if (scene == (s32)&SceneId_SuhallaGate3)
    Func_02000264();
  return 0;
}

void Func_020002e0(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  GameFlag_SetBit(0x8de);
}

void Func_02000308(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Actor_GetById(0)->motionFlags = 0;
  Sound_Play(158);
  Map_CopyCellPictureIndicesRect(66, 36, 71, 8, 2, 2);
  wait(4);
  Map_CopyCellPictureIndicesRect(68, 36, 71, 8, 2, 2);
  wait(4);
  Actor_CenterAndWalk(0, 3, -16);
  Sound_Play(123);
  Import_EventSetExitRequest(value);
}

void Func_02000378(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  gMainRoutineWork.currentOwner = 4;
  Sound_Play(123);
  Import_EventSetExitRequest(value);
}

s32 Scene_PrepareMap(void) { return 0; }

extern char SceneId_SuhallaGate1;
extern char SceneId_VenusLighthouseEntrance;
extern u8 MsgWaahhhItS[];

void Func_02000a40(s32 a, s32 b);

/// Places the leader and the camera after a message, runs actor 8's motions
/// and sends the party to Suhalla Gate 1, entrance 99. The message is
/// "Waahhh".
void Func_020003ac(void) {
  Func_02000988();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWaahhhItS);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 472, 320);
  Camera_MoveWithinMapBounds(PIXELS(456), -1, PIXELS(280), 1);
  Func_02000a30();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Actor_Jump(8, 6, 20);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  {
    s32 x = 256;
    s32 z = 50;

    Func_080d47b4(8, x, z);
  }
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_WalkToAndWait(8, 456, 312);
  Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate1, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gMainRoutineWork.battleRule24b = 2;
  Func_02000a40(9, 1);
  Func_02000990();
}
