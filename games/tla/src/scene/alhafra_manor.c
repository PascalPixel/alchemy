/// @file
/// The mayor of Alhafra's manor, where the party stays as his guests once
/// Briggs is caught and hears in the night that Briggs has escaped
/// (AlhafraManor_BriggsEscapes), with other people about once the boat's mast
/// is cleared (FLAG_ALHAFRA_MAST_CLEARED).

#include "text/msg_ids.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/input.h"

void AlhafraManor_BriggsEscapes(void);
void AlhafraManor_KeepToTheLeadersLayer(void);

void AlhafraManor_GrumbleAboutTheMayor(void);

void AlhafraManor_DefendTheMayor(void);
void AlhafraManor_TellOfTheEasternSea(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gActorPlacements[];
extern s16 gAlhafraManor_ActorPlacementsOnceTheMastIsCleared[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED))
    return gAlhafraManor_ActorPlacementsOnceTheMastIsCleared;
  return gActorPlacements;
}

void AlhafraManor_AskAboutAlhafra(s32 actor) {
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

void AlhafraManor_PointOutAirsRock(s32 actor) {
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

void AlhafraManor_AskIfTheHeroes(s32 actor) {
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

void AlhafraManor_GrumbleAboutTheMayor(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_Set(0x8ba);
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

void AlhafraManor_DefendTheMayor(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_Set(0x8bb);
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

void AlhafraManor_TellOfTheEasternSea(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)((u8 *)work + 0x166) != 0) {
    Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
  }
  GameFlag_Set(0x8bc);
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

s32 Scene_Initialize(void) {
  s32 cameraY = -1;
  s32 cameraZ = 160 << 17;
  s16 *entrance;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Actor_GetById(15)->update = AlhafraManor_KeepToTheLeadersLayer;
  if (gMainRoutineWork.entrance == 80) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 14;
    Actor_ArmCallback(14, 0x5000, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Event_SetMessageId((s32)&MsgAlhafraManor10);
    Event_ShowMessageAndWait(14, 0);
    GameFlag_Clear(0x12f);
  }
  entrance = &gMainRoutineWork.entrance;
  if (*entrance == 81) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 8;
    AlhafraManor_BriggsEscapes();
  }
  if (!GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED) && GameFlag_Get(0x8ff)) {
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
void AlhafraManor_BriggsEscapes(void) {
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
  Sound_Play(SOUND_FADE_OUT_BGM);
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
  Sound_Play(SOUND_FADE_OUT_BGM);
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
  Sound_Play(MUS_049);
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
  Sound_Play(SOUND_FADE_OUT_BGM);
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
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
  } else {
    Event_Wait(40);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_Wait(10);
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
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
  GameFlag_Clear(303);
}

void Actor_TakeLeaderLayer(void);

void AlhafraManor_KeepToTheLeadersLayer(void) { Actor_TakeLeaderLayer(); }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 760, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 312, 0, 120, FACING_NORTHEAST, 0, 224, 16, 464, 192,
     0},
    {99, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(97),
    SCENE_EXIT_IF(1, 95, 14, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 97, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 97, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 97, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 97, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 97, 15, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 97, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 97, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 97, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 97, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 97, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 97, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(13, 97, 14, CONDITION_ALWAYS),
    SCENE_EXIT_IF(14, 97, 13, CONDITION_ALWAYS),
    SCENE_EXIT_IF(15, 97, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(16, 97, 17, CONDITION_ALWAYS),
    SCENE_EXIT_IF(17, 97, 16, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {112, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(152), FACING_EAST, 1, 0},
    {24, CONDITION_ALWAYS, 1, PIXELS(372), 0, PIXELS(92), FACING_NORTH, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(344), FACING_NORTHEAST, 0,
     0},
    {116, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(328), FACING_SOUTHEAST, 0,
     0},
    {128, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(104), FACING_NORTH, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(120), FACING_EAST, 0, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(296), FACING_NORTHWEST, 0,
     0},
    {112, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(248), FACING_NORTHWEST, 1,
     0},
    {209, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(680), FACING_EAST, 0, 0},
    {186, CONDITION_ALWAYS, 2, PIXELS(104), 0, PIXELS(504), FACING_SOUTH, 0, 0},
    {20, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(120), FACING_WEST, 0, 0},
    {19, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(112), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

s16 gAlhafraManor_ActorPlacementsOnceTheMastIsCleared[] = {
    112,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    1,
    24,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    1,
    115,
    -1,
    1,
    0,
    0,
    344,
    0,
    0,
    0,
    320,
    0,
    1,
    231,
    -1,
    2,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTH,
    1,
    195,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTH,
    1,
    56,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    0,
    2,
    5,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTHEAST,
    2,
    6,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTHEAST,
    2,
    7,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTHEAST,
    2,
    -1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x8a7), MSGNO(MsgAlhafraManor30)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x8a7),
     MSGNO(MsgAlhafraManor31)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor01)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(AlhafraManor_AskAboutAlhafra)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor03)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor08)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(AlhafraManor_PointOutAirsRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor11)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor13)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(AlhafraManor_AskIfTheHeroes)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor17)},
    {TRIGGER_TALK, 18, 0x8ba, EVENT(AlhafraManor_GrumbleAboutTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1042, 0x8ba,
     EVENT(AlhafraManor_GrumbleAboutTheMayor)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor19)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor22)},
    {TRIGGER_TALK, 8, 0x8bb, EVENT(AlhafraManor_DefendTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1032, 0x8bb,
     EVENT(AlhafraManor_DefendTheMayor)},
    {TRIGGER_TALK, 9, 0x8bc, EVENT(AlhafraManor_TellOfTheEasternSea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1033, 0x8bc,
     EVENT(AlhafraManor_TellOfTheEasternSea)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor25)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor28)},
    {371, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraInteriors125))},
    {TRIGGER_TABLE_END},
};
