/// @file
/// The scene code of overlay 0x667, in ROM order.

#include "text/msg_ids.h"

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/alhafra_manor.h"
#include "module/system/system.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02001708[];
extern const u8 gData_02001840[];

const u8 *Scene_GetPlacements(void) {
  if (GameFlag_Test(0x8a7))
    return gData_02001840;
  return gData_02001708;
}

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "scene_ids.h"
#include "event_inline.h"

void Func_0200006c(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor02;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_020000b8(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor09;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(20);
    Actor_WatchAndSetScript(actor, gMainRoutineWork.currentOwner);
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 760, 320);
    Actor_FaceDirection(14, 0x5000, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_SetDestination((s32)&SceneId_WorldMap, 67);
  } else {
    Event_SetMessageId(msg + 2);
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

void Func_02000150(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor14;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_0200019c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_SetBit(0x8ba);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor18);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_ShowEmote(18, 256, 60);
  Actor_FaceActorById(18, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmote(18, 263, 20);
  Event_ShowMessageThenWait(18, 0, 10);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(18, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 4);
  Event_ShowMessageThenWait(18, 0, 10);
  Event_End();
}

void Func_02000238(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_SetBit(0x8bb);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor23);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_ShowEmote(8, 256, 30);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 304, 152);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Actor_ArmCallback(8, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(0x4008, 0, 10);
  Event_End();
}

void Func_02000370(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)((u8 *)work + 0x166) != 0) {
    Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
  }
  GameFlag_SetBit(0x8bc);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor24);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ArmCallback(9, 0xd000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(9, 258, 40);
  Event_ShowMessageThenWait(9, 0, 20);
  Actor_ShowEmote(9, 264, 40);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(9, 0, 10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Event_Wait(30);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessageThenWait(9, 0, 10);
  }
  Event_End();
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_FaceDirection(9, 0xd000, 0);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "input.h"

s32 Scene_Initialize(void) {
  s32 cameraY = -1;
  s32 cameraZ = 160 << 17;
  s16 *entrance;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Actor_GetById(15)->update = Func_02001488;
  if (gMainRoutineWork.entrance == 80) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 14;
    Actor_ArmCallback(14, 0x5000, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Event_SetMessageId((s32)&MsgAlhafraManor10);
    Event_ShowMessageAndWait(14, 0);
    GameFlag_ClearBit(0x12f);
  }
  entrance = &gMainRoutineWork.entrance;
  if (*entrance == 81) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 8;
    Func_02000658();
  }
  if (!GameFlag_Test(0x8a7) && GameFlag_Test(0x8ff)) {
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 99) {
    FieldCamera_MoveWithinMapBounds(152 << 17, cameraY, cameraZ, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Camera_SetSpeedParameters(0x20000, 0x4000);
    {
      /* FAKEMATCH(pin): the mode in r3 ahead of the other arguments; as a
       * literal, 2 of 128 instructions differ in tla-en. */
      register s32 mode asm("r3") = 1;

      FieldCamera_MoveWithinMapBounds(144 << 16, cameraY, cameraZ, mode);
    }
    goto check;
    do {
      wait(1);
    check:;
    } while (gInput.held == 0);
    Event_SetDestination((s32)&SceneId_Ov667, 80);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#define EVENT_WORD(offset) (*(s32 *)((u8 *)Ram_HeapSlots->eventWork + (offset)))

void Func_020015f0(void);

/// Walks actor `id` to the controlled character's cell, when it has one.
#define WALK_TO_LEADER(id)                                                     \
  {                                                                            \
    struct Actor *leader_;                                                     \
    Actor_SetSpeed((id), 0x13333, 0x9999);                                     \
    Actor_SetModeById((id), 2);                                                \
    leader_ = Actor_GetById(gMainRoutineWork.currentOwner);                    \
    if (leader_ != 0)                                                          \
      Actor_SetDestinationById((id), ((s16 *)&leader_->x)[1],                  \
                               ((s16 *)&leader_->z)[1]);                       \
    Actor_WaitForMoveById((id));                                               \
    Actor_SetHorizontalPositionWithTerrain((id), 0, 0);                        \
  }

/// A long scene in Alhafra's manor (message MsgAlhafraManor29 on): actors 4
/// to 13 are placed, turn, emote and speak in turn, the screen fades out and
/// back between two parts, and actor 4 asks a yes-or-no question (both
/// answers count up the event work's halfword 0x1c4); at the end actors 7, 5,
/// 6 and 13 walk to the controlled character, the camera follows it again
/// and flag 303 is cleared. Each call goes through the call layer
/// (call.h), which builds its constants at the call.
void Func_02000658(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAlhafraManor29);
  Actor_SetPosition(9, PIXELS(344), PIXELS(96));
  Actor_SetPosition(8, PIXELS(408), PIXELS(136));
  Actor_SetPosition(13, PIXELS(312), PIXELS(104));
  Actor_SetPosition(7, PIXELS(296), PIXELS(120));
  Actor_SetPosition(6, PIXELS(296), PIXELS(136));
  Actor_SetPosition(5, PIXELS(312), PIXELS(136));
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(30);
  FieldCamera_MoveTo(PIXELS(344), -1, PIXELS(120), 0);
  EVENT_WORD(0x1b4) = 60;
  EVENT_WORD(0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(13, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 4);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Actor_ShowEmote(9, 264, 30);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(10);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_ArmCallback(5, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(0x2005, 0, 10);
  Actor_ShowEmote(9, 263, 30);
  Actor_FaceDirection(9, 0x5000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Actor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 258);
  Actor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 258);
  Event_Wait(40);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetModeById(6, 4);
  Motion_SetModeAndWaitAnimation(5, 4);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Event_Wait(40);
  EVENT_WORD(0x1ac) = 0x100;
  PaletteFade_ApplyTarget(0x10002, 0);
  PaletteFade_StartInterpolation(40);
  Event_WaitTransitionFrames();
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Actor_ShowEmote(13, 257, 50);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_FaceDirection(13, 0x5000, 0);
  Event_Wait(50);
  Actor_ShowEmote(13, 261, 0);
  Actor_ShowEmote(4, 261, 0);
  Actor_ShowEmote(7, 261, 0);
  Actor_ShowEmote(5, 261, 0);
  Actor_ShowEmote(6, 261, 50);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 258);
  Event_Wait(40);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_ArmCallback(13, FACING_EAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(9, 0x5000, 0);
  Event_Wait(35);
  Actor_ShowEmote(9, 264, 50);
  Actor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 258);
  Actor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 258);
  Event_Wait(60);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(9, 260, 50);
  Motion_SetModeAndWaitAnimation(5, 4);
  Event_Wait(20);
  Actor_ShowEmote(6, 263, 60);
  Actor_Jump(4, 6, 15);
  Actor_Jump(4, 6, 23);
  Event_Wait(15);
  Actor_ShowEmoteById(9, 265, 70);
  Sound_Play(78);
  BattleEffect_ApplyColorToSourceBuffer(0x10002, 0);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  PaletteFade_StartInterpolation(60);
  Event_WaitTransitionFrames();
  Event_Wait(60);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Scene_PlayMusic();
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_SetSpeed(11, 0x19999, 0xcccc);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkByAndWaitById(9, 16, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(9, 259, 30);
  Event_ShowMessageThenWait(0x1009, 0, 10);
  Actor_ShowEmote(8, 258, 45);
  Actor_WalkByAndWait(8, -16, 0);
  Actor_FaceDirection(8, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 20);
  Sound_Play(78);
  Actor_SetPosition(11, PIXELS(344), PIXELS(176));
  Actor_WalkByAndWait(11, 0, -16);
  Event_ShowMessageThenWait(11, 0, 10);
  Actor_ShowEmote(9, 257, 30);
  Actor_FaceDirection(13, 0x3000, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(11, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(11, 16, -24);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Event_Wait(20);
  Actor_ShowEmote(11, 259, 30);
  Event_ShowMessageThenWait(11, 0, 10);
  Sound_Play(49);
  Actor_ShowEmoteById(13, 0x100, 0);
  Actor_ShowEmoteById(4, 0x100, 0);
  Actor_ShowEmoteById(7, 0x100, 0);
  Actor_ShowEmoteById(5, 0x100, 0);
  Actor_ShowEmoteById(6, 0x100, 0);
  Actor_ShowEmoteById(8, 0x100, 0);
  Actor_ShowEmoteById(9, 0x100, 60);
  Actor_ShowEmote(9, 258, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 4);
  Event_ShowMessageThenWait(11, 0, 10);
  Actor_ShowEmote(8, 263, 30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmoteById(11, 258, 50);
  Actor_ArmCallback(11, FACING_EAST, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 20);
  Actor_SetPosition(12, PIXELS(344), PIXELS(176));
  Actor_WalkByAndWait(12, 0, -16);
  Event_Wait(10);
  Actor_ShowEmoteById(12, 264, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(13, 261, 0);
  Actor_ShowEmote(4, 261, 0);
  Actor_ShowEmote(7, 261, 0);
  Actor_ShowEmote(5, 261, 0);
  Actor_ShowEmote(6, 261, 0);
  Actor_ShowEmote(8, 261, 0);
  Actor_ShowEmoteById(9, 261, 60);
  Actor_FaceDirection(12, 0xb000, 0);
  Event_Wait(30);
  Actor_FaceDirection(12, 0xd000, 0);
  Event_Wait(30);
  Actor_FaceDirection(12, 0xb000, 0);
  Event_Wait(30);
  Actor_FaceDirection(12, 0xd000, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(12, 257, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(11, 0x5000, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(12, 0x100, 50);
  Actor_WalkByAndWait(12, 0, -8);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(9, 259, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmoteById(12, 0x100, 40);
  Event_ShowMessageThenWait(12, 0, 10);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ArmCallback(11, FACING_EAST, 0);
  Actor_ArmCallback(12, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(11, 0, 10);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_FaceDirection(12, 0xd000, 0);
  Event_Wait(35);
  Motion_SetVarCbAndRefresh(12, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(8, 259, 30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_Jump(12, 6, 0);
  Actor_Jump(11, 6, 25);
  Actor_WalkByById(12, 0, 24);
  Actor_WalkByAndWait(11, -16, 16);
  Actor_WalkByById(11, 0, 24);
  Actor_WaitForMoveById(12);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_WaitForMoveById(11);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_Wait(10);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_ArmCallback(13, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WatchAndSetScript(4, 9);
  Actor_WatchAndSetScript(13, 9);
  Actor_WatchAndSetScript(7, 9);
  Actor_WatchAndSetScript(5, 9);
  Actor_WatchAndSetScript(6, 9);
  Actor_WalkByAndWaitById(9, 16, 0);
  Actor_WalkByAndWaitById(9, 0, 50);
  Actor_WalkByAndWait(9, -32, 4);
  Event_Wait(20);
  Actor_Stop(4);
  Actor_Stop(13);
  Actor_Stop(7);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_ShowEmoteById(9, 262, 30);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Actor_ArmCallback(9, 0xb000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_FaceDirection(13, 0x5000, 0);
  Event_Wait(50);
  Actor_ShowEmoteById(9, 263, 30);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 10);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(13, 0x3000, 0);
  Event_Wait(35);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x2009, 0, 10);
  Actor_WalkByAndWaitById(8, -32, 0);
  Event_Wait(10);
  Actor_Jump(8, 6, 15);
  Actor_Jump(8, 6, 23);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ArmCallback(9, 0xd000, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Sound_Play(78);
  Actor_ArmCallback(8, 0x5000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(20);
  Actor_WalkByById(9, 0, 24);
  Actor_WalkByAndWait(8, -16, 16);
  Actor_WalkByById(8, 0, 24);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_WaitForMoveById(8);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Event_Wait(30);
  Scene_PlayMusic();
  Actor_ShowEmoteById(7, 265, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmoteById(6, 263, 30);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_ArmCallback(5, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(13, 0x5000, 0);
  Event_Wait(20);
  FieldMessage_ShowAtActor(0x2005, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Func_020015f0();
    Event_Wait(30);
    Motion_SetModeAndWaitAnimation(6, 4);
    Event_ShowMessageThenWait(6, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Event_Wait(40);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_Wait(10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(6, 0, 10);
  }
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 10);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(13, 264, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200d, 0, 10);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(25);
  WALK_TO_LEADER(7);
  WALK_TO_LEADER(5);
  WALK_TO_LEADER(6);
  WALK_TO_LEADER(13);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Event_End();
  EVENT_WORD(0x1b4) = 16;
  EVENT_WORD(0x1ac) = 521;
  GameFlag_ClearBit(303);
}

void Func_020015e8(void);

void Func_02001488(void) { Func_020015e8(); }
