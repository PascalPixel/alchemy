/// @file
/// The scene code of overlay 0x64f, in ROM order.

#include "suhalla_gate/local.h"
#include "types.h"
#include "main_routine_work.h"

void Func_02000038(void) { Func_080d2b4c(13, 26); }

extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;

extern u8 gData_02000cd8[];
extern u8 gData_02000d98[];
extern u8 gData_02000e40[];

u8 *Func_02000044(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gData_02000d98;
  if (scene == (s32)&SceneId_SuhallaGate3)
    return gData_02000e40;
  return gData_02000cd8;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern char SceneId_SuhallaGate1;

extern u8 gData_02000f10[];
extern u8 gData_02000f58[];
extern u8 gData_02001048[];
extern u8 gData_02001078[];
extern u8 gData_020010e4[];
extern u8 gData_02001180[];

u8 *Func_02000090(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gData_02000f58;
  if (scene == (s32)&SceneId_SuhallaGate1)
    return gData_02001048;
  return gData_02000f10;
}

u8 *Func_020000d0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gData_020010e4;
  if (scene == (s32)&SceneId_SuhallaGate3)
    return gData_02001180;
  return gData_02001078;
}

#include "event_inline.h"
#include "ram_buffer.h"
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

#include "actor.h"
extern u8 MsgHaThatWas[];

void Func_02000494(void) {
  struct MainRoutineWork *work;
  s32 *owner;

  Func_02000988();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgHaThatWas);
  work = &gMainRoutineWork;
  owner = &work->currentOwner;
  Actor_WalkByAndWait(*owner, 0, -4);
  FieldEvent_Wait(30);
  Actor_GetById(*owner)->actionFlags &= 254;
  Actor_WalkByAndWait(*owner, 0, 12);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(*owner, 0, 16);
  FieldEvent_Wait(30);
  Actor_SetPosition(13, 0x2680000, 0x3900000);
  Actor_WalkToAndWait(13, 0x258, 0x398);
  Actor_SetPosition(15, 0x2680000, 0x3900000);
  Actor_FaceDirection(13, 0x4000, 0);
  Actor_WalkToAndWait(15, 0x278, 0x398);
  Actor_FaceDirection(15, 0x4000, 0);
  Actor_SetPosition(14, 0x2680000, 0x3900000);
  Actor_WalkToAndWait(14, 0x268, 0x398);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(13, 3);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimationAndWait(15, 3);
  Event_SetAfterBattleDestination((s32)&SceneId_SuhallaGate2, 99);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseEntrance, 98);
  *((u8 *)work + 0x24b) = 2;
  Func_02000a40(9, 2);
  Func_02000990();
}

void Func_020005e8(void) {
  struct Actor *owner;

  Actor_Jump(8, 4, 0);
  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  if (owner != NULL)
    Actor_ResetAndSetPosition(8, *(s16 *)((u8 *)owner + 10),
                                     *(s16 *)((u8 *)owner + 18));
  Event_Wait(20);
  Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate3, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gMainRoutineWork.battleRule24b = 2;
  Func_02000a40(9, 3);
  Func_02000990();
}

void Func_02000650(void) {
  Func_02000988();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(8, PIXELS(472), PIXELS(480));
  Event_Wait(16);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 8, 0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x100, 0);
  Camera_FollowActor(8, 1);
  Func_02000a30();
  Actor_Jump(8, 4, 0);
  Event_Wait(16);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit(8, 480, 472);
  Event_Wait(30);
  Actor_Jump(8, 2, 0);
  Actor_MoveToAndWait(8, 496, 464);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 488, 456);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 496, 448);
  Func_020005e8();
}

void Func_0200071c(void) {
  s32 x, z;

  Func_02000988();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(8, PIXELS(472), PIXELS(480));
  Event_Wait(16);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 8, 0);
  x = 0x100;
  z = 0;
  Func_080d47b4(gMainRoutineWork.currentOwner, x, z);
  Event_Wait(16);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 488, 440);
  Event_Wait(16);
  Camera_FollowActor(8, 1);
  Func_02000a30();
  Actor_Jump(8, 4, 0);
  Event_Wait(16);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit(8, 488, 480);
  Event_Wait(30);
  Actor_Jump(8, 2, 0);
  Actor_MoveToAndWait(8, 504, 472);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 520, 464);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 528, 456);
  Func_020005e8();
}

void Func_02000800(void) {
  struct Actor *leader;

  Func_02000988();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgMyThatWas);
  Actor_SetPosition(9, PIXELS(512), PIXELS(432));
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, PIXELS(528), PIXELS(432));
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_Wait(30);
  GameFlag_SetBit(0x896);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader)
    Actor_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Event_Wait(10);
  Func_02000990();
}

void Func_020008cc(void) {
  struct Actor *leader;

  Func_02000988();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNotThatWay);
  Func_080d3070(16, -16, 0, 0);
  Actor_WaitForMoveById(16);
  Event_ShowMessageThenWait(0x1010, 0, 10);
  Actor_SetModeById(16, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader)
    Actor_ResetAndSetPosition(16, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Event_Wait(10);
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, 16, 0);
  Func_02000990();
}
