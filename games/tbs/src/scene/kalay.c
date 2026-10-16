#include "game_state.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/kalay.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_scene.h"
#include "resource_3a8_effect.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "script.h"

#include "field_obj.h"
#include "object_effect.h"
#include "orbit.h"
#include "fixed_math.h"
#include "field_runtime.h"

enum { KALAY_CALLBACK_PRIORITY = 3200 };

// Forward declarations.
static void Kalay_SetupEntryActors(void);
static void Kalay_RestorePalaceEntry(void);
static void Kalay_RestoreCaveEntry(void);
static void Kalay_StartWindowTransition(void);
static void Kalay_RestoreTunnelPuzzles(void);
static void Kalay_StartTouristIdleActions(void);
static void Kalay_StartDancerIdleAction(s32 actorId);
static void Kalay_SpeedUpActors(void);
static void Kalay_AlternateDance(void);
static void Kalay_IvanMeetsPalaceGuards(void);
static void Kalay_HammetReturnsToPalace(void);
static void Kalay_UpdateTunnelPassages(void);
static void Kalay_UpdateTunnelEmitters(void);
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z);
static void Kalay_UpdateNutOrbit(void);

// Clear the dance script completion timer.
static __inline__ void Kalay_ClearDanceCompletion(struct FieldActor *actor) {
  s32 completed = 0;

  actor->timer = completed;
}

// Add the Djinni encountered in the Kalay tunnel to the party.
static void Kalay_RecruitDjinn(void) { Djinn_JoinParty(0xC, 2, 3); }

// Choose a random idle action or heading when the dancer timer expires.
static s32 Kalay_UpdateDancerIdleAction(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  u32 action;
  u32 nextTimer;

  if ((s16)actor->work == 0) {
    action = (u32)(Engine_RandomNext() * 8) >> 0x10;
    switch (action) {
    case 0:
      Object_SetMode(actor, 3);
      break;
    case 1:
      Object_SetMode(actor, 4);
      break;
    case 3:
    case 4:
      actor->facing += (u32)(Engine_RandomNext() << 0xF) >> 0x10;
      break;
    }
    nextTimer = (u32)(Engine_RandomNext() * 0x50) >> 0x10;
    actor->work = nextTimer;
  }
  if ((s16)actor->work != 0) {
    actor->work = (s16)actor->work - 1;
  }
  return 1;
}

// Shared source, included in link order.
#include "common/object_heading.h"

// Select the entrance table for the current Kalay scene.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kalay1) {
    return gKalayEntrances1;
  }
  if (scene == (s32)&SceneId_Kalay2) {
    return gKalayEntrances2;
  }
  if (scene == (s32)&SceneId_Kalay3) {
    return gKalayEntrances3;
  }
  if (scene == (s32)&SceneId_Kalay4) {
    return gKalayEntrances4;
  }
  if (scene == (s32)&SceneId_Kalay5) {
    return gKalayEntrances5;
  }
  if (scene == (s32)&SceneId_Kalay6) {
    return gKalayEntrances6;
  }
  return gKalayEntrancesOther;
}

// Return the scene hook result without additional work.
s32 SceneData_ReturnZero(void) { return 0; }

// Return the Kalay exit table.
u8 *Kalay_GetExits(void) { return (u8 *)gKalay_SceneTable01; }

// Select the actor placements for the current Kalay scene.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kalay1) {
    return gKalayPlacements1;
  }
  if (scene == (s32)&SceneId_Kalay2) {
    return gKalayPlacements2;
  }
  if (scene == (s32)&SceneId_Kalay3) {
    return gKalayPlacements3;
  }
  if (scene == (s32)&SceneId_Kalay6) {
    return gKalayPlacements6;
  }
  return gKalayPlacementsOther;
}

// Show the well description and the note about its cleanliness.
static void Kalay_SearchWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgKalayVeryCleanMaintained, 1);
  Engine_EventEnd();
}

// Award the Nut caught in the cave and hide its actor.
static void Kalay_CatchCaveNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(12, 0, 0);
  Engine_GameFlagSet(0xfd6);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

// Select the event bindings for the current Kalay scene.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kalay1) {
    return gKalayEvents1;
  }
  if (scene == (s32)&SceneId_Kalay2) {
    return gKalayEvents2;
  }
  if (scene == (s32)&SceneId_Kalay3) {
    return gKalayEvents3;
  }
  if (scene == (s32)&SceneId_Kalay4) {
    return gKalayEvents4;
  }
  if (scene == (s32)&SceneId_Kalay5) {
    return gKalayEvents5;
  }
  if (scene == (s32)&SceneId_Kalay6) {
    return gKalayEvents6;
  }
  return gKalayEventsOther;
}

// Ask the visitor why they came to Kalay.
static void Kalay_TalkAboutLeavingHome(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayCameKalayBecauseDidntLike);
  Engine_EventAskYesNo(19, 0);
  Engine_EventEnd();
}

// Discuss cave access when the leader approaches from the allowed side.
static void Kalay_AskAboutCaveAccess(void) {
  struct FieldActor *leader;
  u32 relativeFacing;
  u16 *message;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing + 0xfffff000) << 16;
  if (relativeFacing > 0x60000000) {
    Engine_EventBegin();
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKalayDoWantGoCaveUp);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_ActorSetAnimationAndWait(8, 4);
      Engine_EventShowMessage(8, 0);
    } else {
      message = &gEventWork->message;
      *message = *message + 1;
      Engine_ActorSetAnimationAndWait(8, 3);
      Engine_EventShowMessage(8, 0);
    }
    Engine_EventEnd();
  }
}

// Ask about how Layana treated the speaker.
static void Kalay_TalkAboutLayana(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayLayanaWasVeryHardOn);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

// Open shop 22 from the counter side or show the merchant dialogue.
static void Kalay_TalkToMerchant22(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0x80000000) {
    Engine_ShopOpen(22, 22);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayLordHammetSellsHisBest);
    Engine_EventShowMessage(22, 0);
    Engine_EventEnd();
  }
}

// Open the weapon shop from the counter side or discuss purchases.
static void Kalay_TalkToWeaponMerchant(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x6001) << 16;
  if (relativeFacing <= 0x7ffe0000) {
    Engine_ShopOpen(23, 23);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayDidFindNeededInWeapon);
    Engine_EventAskYesNo(23, 0);
    Engine_EventEnd();
  }
}

// Open shop 24 from the counter side or show the merchant dialogue.
static void Kalay_TalkToMerchant24(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0xC0000000) {
    Engine_ShopOpen(24, 24);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayLadyLayanaSharedInLord);
    Engine_EventShowMessage(24, 0);
    Engine_EventEnd();
  }
}

// Stop actor motion, open the touched doorway, and take its exit.
static void Kalay_RunExitDoor(void) {
  struct EventWork *event;
  struct FieldActor *actor;
  s16 *trigger;
  u32 i;
  s32 door;

  event = gEventWork;
  Engine_EventBegin();
  for (i = 8; i <= 65; i++) {
    actor = Object_GetById(i);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  trigger = &event->touchedTrigger;
  door = *trigger - 1;
  Engine_AudioPlayCue(158);
  {
    s32 x = gKalay_DoorCells[door].x;
    s32 y = gKalay_DoorCells[door].y;

    Engine_MapAnimateCells(gKalay_DoorCells[door].steps, x, y);
  }
  Engine_ActorSetSpeed(0, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(0)->motionFlags = 0;
  Engine_ActorSetAnimation(0, 2);
  Actor_CenterAndWalk(0, 2, -8);
  Engine_EventWait(10);
  Engine_EventRequestExit(*trigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// Play the door cue and draw the two opening frames.
static __inline__ void Kalay_AnimatePalaceDoor(void) {
  Event_PlayCue(188);
  Map_CopyCellsTo(36, 23, 43, 12, 2, 2);
  Engine_TaskWait(5);
  Map_CopyCellsTo(39, 23, 43, 12, 2, 2);
  Engine_TaskWait(5);
}

// Animate the palace door and move the leader through exit 2.
static void Kalay_ExitPalaceDoor(void) {
  Engine_EventBegin();
  Kalay_AnimatePalaceDoor();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Actor_SetDestinationOffset(0, 0, -8);
  Engine_EventWait(10);
  Engine_EventRequestExit(2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// Show the tour group stopping, waiting for its missing member, and leaving.
static void Kalay_WatchTourGroupDeparture(void) {
  s32 message;
  struct FieldActor *headWait;
  struct FieldActor *lastWait;
  struct FieldActor *closing;
  struct FieldActor *late;
  struct FieldActor *headEnd;
  struct FieldActor *lastEnd;
  struct FieldActor *leader;
  struct FieldActor *last;
  struct FieldActor *head;
  struct FieldActor *headBack;
  struct FieldActor *headAgain;
  struct FieldActor *headHome;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  if (GameFlag_IsSet(0x910) != 0 && GameFlag_IsSet(0x911) == 0) {
    Event_CallWithLastActiveObjectId(gKalay_StagedGroupScript);
    Actor_SetPosition(20, PIXELS(252), PIXELS(272));
    Actor_SetPosition(27, PIXELS(284), PIXELS(264));
    Actor_SetPosition(28, PIXELS(284), PIXELS(280));
    Actor_SetPosition(29, PIXELS(300), PIXELS(264));
    Actor_SetPosition(30, PIXELS(300), PIXELS(280));
    Actor_SetPosition(32, PIXELS(316), PIXELS(264));
    Actor_SetPosition(31, PIXELS(316), PIXELS(280));
    Actor_SetPosition(33, PIXELS(332), PIXELS(264));
    Actor_SetPosition(34, PIXELS(332), PIXELS(280));
    Actor_SetPosition(21, PIXELS(364), PIXELS(272));
    Event_PlayCue(17);
    Ui_SetMessageVoiceFromActor(20);
    UiText_ShowCenteredMessage(MsgKalayOkListeningLets, 1, 0);
    Event_PlayCue(9);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    if (work->touchedTrigger == 9) {
      Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
    } else {
      Camera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
    }
    // Stagger the group speeds so the guides can gather everyone together.
    Actor_SetSpeed(20, PIXELS(11) / 10, PIXELS(11) / 20);
    Actor_SetSpeed(27, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(28, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(29, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(30, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(32, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(31, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(33, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_SetSpeed(34, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_SetSpeed(21, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_SetActionScript(20, gSceneAction_GroupMotion);
    Actor_SetActionScript(27, gSceneAction_GroupMotion);
    Actor_SetActionScript(28, gSceneAction_GroupMotion);
    Actor_SetActionScript(29, gSceneAction_GroupMotion);
    Actor_SetActionScript(30, gSceneAction_GroupMotion);
    Actor_SetActionScript(32, gSceneAction_GroupMotion);
    Actor_SetActionScript(31, gSceneAction_GroupMotion);
    Actor_SetActionScript(33, gSceneAction_GroupMotion);
    Actor_SetActionScript(34, gSceneAction_GroupMotion);
    last = Actor_Get(21);
    last->timer = 0;
    Actor_SetActionScript(21, gSceneAction_GroupMotion);
    Camera_MoveTo(PIXELS(186), -1, PIXELS(272), 1);
    Object_RefreshSelectorById(20);
    Actor_FaceDirection(20, FACING_EAST, 0);
    do {
      Engine_TaskWait(1);
    } while (last->timer == 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(27, 2);
    Actor_FaceDirection(27, FACING_SOUTH + FACING_STEP, 20);
    Engine_EventSetMessage(MsgKalayWhyWeStoppingAtPlace);
    Event_ShowMessageAndWait(27, 0, 10);
    Engine_ActorRunRepeatedMotion(28, 2);
    Actor_FaceDirection(28, FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimation(28, ANIM_NOD);
    Event_ShowMessageAndWait(28, 0, 10);
    Actor_SetAttachedEffect(32, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(32, 0, 10);
    Actor_ShowEmote(31, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 10);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, FACING_WEST, 10);
    Engine_ActorRunRepeatedMotion(31, 2);
    Engine_ActorSetAnimation(31, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 20);
    Engine_ActorSetAnimation(31, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(32, ANIM_NOD);
    Engine_ActorRunRepeatedMotion(20, 2);
    Actor_SetAttachedEffect(20, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(20, 0, 10);
    Engine_ActorRunRepeatedMotion(20, 2);
    head = Actor_Get(20);
    head->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 172, 264);
    Engine_EventWait(1);
    headBack = Actor_Get(20);
    headBack->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(27, FACING_WEST, 0);
    Actor_FaceDirection(28, FACING_WEST, 0);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Actor_FaceDirection(31, FACING_WEST, 20);
    Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
    Engine_EventWait(20);
    headAgain = Actor_Get(20);
    headAgain->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 172, 272);
    Engine_EventWait(1);
    headHome = Actor_Get(20);
    headHome->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 180, 272);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_ShowEmote(34, EMOTE_IN_FRONT | 5, 0);
    Engine_ActorRunRepeatedMotion(34, 1);
    Engine_ActorSetAnimationAndWait(34, ANIM_NOD);
    Event_ShowMessageAndWait(34, 0, 10);
    Engine_ActorRunRepeatedMotion(33, 1);
    Event_ShowMessageAndWait(33, 0, 10);
    Engine_ActorSetAnimation(33, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(33, 0, 10);
    Engine_ActorRunRepeatedMotion(21, 2);
    Actor_ShowEmote(21, EMOTE_IN_FRONT | 2, 0);
    Event_ShowMessageAndWait(21, 0, 10);
    Engine_ActorJump(20, 2, 20);
    Engine_ActorJump(20, 4, 40);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_SetSpeed(21, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(21, 265, 282);
    Actor_WalkToAndWait(21, 251, 284);
    Actor_WalkToAndWait(21, 246, 296);
    Actor_FaceDirection(21, FACING_NORTH, 0);
    // Start the tourists idle actions while the guides count the group.
    Kalay_StartTouristIdleActions();
    Engine_EventWait(40);
    Actor_SetSpeed(21, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(21, 228, 296);
    Actor_FaceDirection(21, FACING_NORTH, 40);
    Actor_WalkToAndWait(21, 212, 296);
    Actor_FaceDirection(21, FACING_NORTH, 40);
    Actor_WalkToAndWait(21, 192, 296);
    Actor_FaceDirection(21, FACING_NORTH, 40);
    Engine_ActorStartRepeatedMotion(21, 2);
    Actor_ShowEmote(21, EMOTE_IN_FRONT, 60);
    Actor_FaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_WalkToAndWait(21, 184, 286);
    Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_SetAttachedEffect(21, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(21, ANIM_SHAKE_HEAD);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | 1, 40);
    Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
    Engine_ActorRunRepeatedMotion(20, 2);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Actor_FaceDirection(21, FACING_EAST, 60);
    Actor_FaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
    Actor_FaceDirection(21, FACING_EAST, 0);
    Actor_SetSpeed(20, PIXELS(8) / 5, PIXELS(4) / 5);
    Object_SetActionCallbackAndRefreshById(20, (s32)gKalay_DanceScriptB);
    Actor_WalkToAndWait(20, 228, 296);
    Actor_FaceDirection(20, FACING_NORTH, 40);
    Actor_WalkToAndWait(20, 212, 296);
    Actor_FaceDirection(20, FACING_NORTH, 40);
    Actor_WalkToAndWait(20, 192, 296);
    Actor_FaceDirection(20, FACING_NORTH, 40);
    Actor_FaceDirection(20, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_SetAttachedEffect(20, EMOTE_IN_FRONT | 2);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(20, ANIM_SHAKE_HEAD);
    message = MsgKalayShortOnePerson;
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(20, 0, 40);
    // Alternate the guides dance while individual tourists interrupt.
    Kalay_SpeedUpActors();
    Actor_WalkToAndWait(20, 178, 272);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Engine_EventWait(240);
    Engine_ActorStop(27);
    Engine_TaskWait(1);
    Actor_FaceDirection(27, FACING_WEST, 10);
    Actor_ShowEmote(27, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(27, 0, 10);
    Kalay_StartDancerIdleAction(27);
    Engine_EventWait(80);
    Engine_ActorStop(28);
    Engine_TaskWait(1);
    Actor_FaceDirection(28, FACING_NORTH + FACING_STEP, 20);
    Engine_ActorRunRepeatedMotion(28, 2);
    Event_ShowMessageAndWait(28, 0, 10);
    Kalay_StartDancerIdleAction(28);
    Engine_EventWait(160);
    Engine_ActorStop(32);
    Engine_TaskWait(1);
    Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 10);
    Actor_ShowEmote(32, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(32, 0, 10);
    Kalay_StartDancerIdleAction(32);
    Engine_EventWait(80);
    Engine_ActorStop(30);
    Engine_TaskWait(1);
    Actor_FaceDirection(30, FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ActorRunRepeatedMotion(30, 1);
    Engine_EventSetMessage(message + 6);
    Event_ShowMessageAndWait(30, 0, 10);
    Scheduler_RemoveCallback(Kalay_AlternateDance);
    Engine_ActorStop(20);
    Engine_ActorStop(21);
    Engine_TaskWait(1);
    headWait = Actor_Get(20);
    headWait->timer = 0;
    lastWait = Actor_Get(21);
    lastWait->timer = 0;
    Actor_SetSpeed(20, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(21, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetActionScript(20, gKalay_DanceScriptC);
    Actor_SetActionScript(21, gKalay_DanceScriptD);
    Engine_ActorStop(29);
    Engine_TaskWait(1);
    Actor_FaceDirection(29, FACING_SOUTH + FACING_STEP, 10);
    message += 5;
    Engine_ActorRunRepeatedMotion(29, 2);
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(29, 0, 20);
    Kalay_StartDancerIdleAction(29);
    Kalay_StartDancerIdleAction(30);
    do {
      Engine_TaskWait(1);
    } while (Actor_Get(20)->timer == 0 || Actor_Get(21)->timer != 1);
    Actor_SetActionScript(20, gKalay_DanceScriptE);
    Actor_SetActionScript(21, gKalay_DanceScriptF);
    Engine_ActorStop(31);
    Engine_TaskWait(1);
    Actor_FaceDirection(31, FACING_SOUTH + FACING_STEP, 10);
    Engine_ActorRunRepeatedMotion(31, 1);
    Engine_ActorSetAnimationAndWait(31, ANIM_SHAKE_HEAD);
    message = MsgKalayDecideEnteringColosso;
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(31, 0, 10);
    Kalay_StartDancerIdleAction(31);
    Engine_ActorStop(34);
    Engine_ActorStop(33);
    Engine_TaskWait(1);
    Actor_ShowEmote(34, EMOTE_IN_FRONT | 5, 40);
    Actor_ShowEmote(33, EMOTE_IN_FRONT | 5, 60);
    Actor_FaceDirection(34, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_FaceDirection(33, FACING_SOUTH + FACING_STEP, 10);
    message += 3;
    Engine_ActorSetAnimationAndWait(34, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(34, 0, 10);
    Engine_ActorRunRepeatedMotion(33, 1);
    Engine_ActorSetAnimation(33, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_ShowEmote(34, EMOTE_IN_FRONT | 2, 60);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | 3, 0);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventSetMessage(MsgKalayThatsWeCantWaitAny);
    Event_ShowMessageAndWait(20, 0, 10);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_ActorStop(30);
    Engine_ActorStop(32);
    Engine_ActorStop(31);
    Engine_ActorStop(33);
    Engine_ActorStop(34);
    Engine_ActorStop(20);
    Engine_ActorStop(21);
    Engine_TaskWait(1);
    Engine_ActorJump(27, 2, 0);
    Engine_ActorJump(28, 2, 0);
    Engine_ActorJump(29, 2, 0);
    Engine_ActorJump(30, 2, 0);
    Engine_ActorJump(32, 2, 0);
    Engine_ActorJump(31, 2, 0);
    Engine_ActorJump(33, 2, 0);
    Engine_ActorJump(34, 2, 0);
    Engine_ActorJump(21, 2, 40);
    Actor_FaceDirection(27, FACING_WEST, 0);
    Actor_FaceDirection(28, FACING_WEST, 0);
    Actor_FaceDirection(29, FACING_WEST, 0);
    Actor_FaceDirection(30, FACING_WEST, 0);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Actor_FaceDirection(31, FACING_WEST, 0);
    Actor_FaceDirection(33, FACING_WEST, 0);
    Actor_FaceDirection(34, FACING_WEST, 40);
    Engine_ActorJump(21, 4, 40);
    Event_ShowMessageAndWait(21, 0, 10);
    Engine_ActorRunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(20, 0, 10);
    Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
    Event_ShowMessageAndWait(21, 0, 10);
    Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_ShowEmote(27, EMOTE_IN_FRONT | 2, 40);
    Engine_ActorStartRepeatedMotion(27, 1);
    Event_ShowMessageAndWait(27, 0, 10);
    Actor_ShowEmote(28, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessageAndWait(28, 0, 10);
    Engine_ActorSetAnimationAndWait(21, ANIM_SHAKE_HEAD);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
    Event_ShowMessageAndWait(21, 0, 20);
    Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_FaceDirection(27, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(28, FACING_NORTHWEST + FACING_STEP, 4);
    Actor_FaceDirection(29, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(30, FACING_NORTHWEST + FACING_STEP, 4);
    Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 4);
    Actor_FaceDirection(33, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(34, FACING_NORTHWEST + FACING_STEP, 4);
    Engine_ActorSetAnimation(27, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(28, ANIM_NOD);
    Engine_ActorSetAnimation(29, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(30, ANIM_NOD);
    Engine_ActorSetAnimation(32, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(31, ANIM_NOD);
    Engine_ActorSetAnimation(33, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(34, ANIM_NOD);
    Engine_ActorJump(20, 2, 40);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_FaceDirection(27, FACING_WEST, 0);
    Actor_FaceDirection(28, FACING_WEST, 4);
    Actor_FaceDirection(29, FACING_WEST, 0);
    Actor_FaceDirection(30, FACING_WEST, 4);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Actor_FaceDirection(31, FACING_WEST, 4);
    Actor_FaceDirection(33, FACING_WEST, 0);
    Actor_FaceDirection(34, FACING_WEST, 4);
    Actor_SetSpeed(20, PIXELS(11) / 10, PIXELS(11) / 20);
    Actor_SetSpeed(27, PIXELS(21) / 20, PIXELS(21) / 40);
    Actor_SetSpeed(28, PIXELS(21) / 20, PIXELS(21) / 40);
    Actor_SetSpeed(29, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(30, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(32, PIXELS(19) / 20, PIXELS(19) / 40);
    Actor_SetSpeed(31, PIXELS(19) / 20, PIXELS(19) / 40);
    Actor_SetSpeed(33, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(34, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(21, PIXELS(17) / 20, PIXELS(17) / 40);
    Engine_ActorSetSpritePriority(27, 1);
    Engine_ActorSetSpritePriority(28, 1);
    Engine_ActorSetSpritePriority(29, 1);
    Engine_ActorSetSpritePriority(30, 1);
    Engine_ActorSetSpritePriority(32, 1);
    Engine_ActorSetSpritePriority(31, 1);
    Engine_ActorSetSpritePriority(33, 1);
    Engine_ActorSetSpritePriority(34, 1);
    Engine_ActorSetSpritePriority(20, 1);
    Engine_ActorSetSpritePriority(21, 1);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_ActorStop(30);
    Engine_ActorStop(32);
    Engine_ActorStop(31);
    Engine_ActorStop(33);
    Engine_ActorStop(34);
    Engine_ActorStop(20);
    Engine_ActorStop(21);
    Engine_TaskWait(1);
    // Send the group onward, then bring in the missing tourist.
    Actor_SetActionScript(20, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(27, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(28, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(29, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(30, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(32, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(31, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(33, gSceneAction_GroupOffsetMotion);
    Actor_SetActionScript(34, gSceneAction_GroupOffsetMotion);
    closing = Actor_Get(21);
    closing->timer = 0;
    Actor_SetActionScript(21, gSceneAction_GroupOffsetMotion);
    do {
      Engine_TaskWait(1);
    } while (Actor_Get(21)->timer != 1);
    Engine_EventWait(80);
    // The late tourist finds that the rest of the group has already left.
    Actor_SetPosition(14, PIXELS(340), PIXELS(274));
    Engine_TaskWait(1);
    Actor_SetSpeed(14, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(14, 224, 274);
    Actor_FaceDirection(14, FACING_EAST, 40);
    Actor_FaceDirection(14, FACING_WEST, 40);
    Actor_FaceDirection(14, FACING_NORTH, 40);
    Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 40);
    Actor_ShowEmote(14, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, FACING_EAST, 40);
    Actor_FaceDirection(14, FACING_NORTH, 40);
    Actor_FaceDirection(14, FACING_WEST, 40);
    Actor_SetAttachedEffect(14, EMOTE_IN_FRONT | 2);
    Engine_ActorJump(14, 4, 40);
    Event_ShowMessageAndWait(14, 0, 20);
    Engine_ActorRunRepeatedMotion(14, 2);
    Event_ShowMessageAndWait(14, 0, 10);
    Engine_ActorJump(14, 4, 40);
    Actor_SetSpeed(14, PIXELS(6) / 5, PIXELS(3) / 5);
    closing = Actor_Get(14);
    closing->timer = 0;
    Actor_SetActionScript(14, gSceneAction_GroupOffsetMotion);
    do {
      Engine_TaskWait(1);
    } while (Actor_Get(14)->timer != 1);
    Actor_SetPosition(14, PIXELS(359), PIXELS(314));
    late = Actor_Get(14);
    late->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(20, PIXELS(455), PIXELS(434));
    headEnd = Actor_Get(20);
    headEnd->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(21, PIXELS(464), PIXELS(416));
    lastEnd = Actor_Get(21);
    lastEnd->facing = FACING_SOUTH + FACING_STEP;
    Engine_ActorDestroy(27);
    Engine_ActorDestroy(28);
    Engine_ActorDestroy(29);
    Engine_ActorDestroy(30);
    Engine_ActorDestroy(31);
    Engine_ActorDestroy(32);
    Engine_ActorDestroy(33);
    Engine_ActorDestroy(34);
    Event_PlayCue(17);
    if (work->touchedTrigger == 9) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 224, 458);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      leader->facing = FACING_NORTH;
    } else {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 248);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      leader->facing = FACING_SOUTH;
    }
    Audio_PlayCueFromEventWork();
    GameFlag_Set(0x911);
  } else {
    Event_PlayCue(123);
    Engine_EventRequestExit(work->touchedTrigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  }
  Engine_EventEnd();
}

// Stop the fade overlay from tracking the leader and clear its flag.
static void Kalay_StopTrackingLeader(void) {
  if (GameFlag_IsSet(0x200) != 0) {
    gActorEffectWork->actor = NULL;
    GameFlag_Clear(0x200);
  }
}

// Track the leader with the fade overlay and save its active flag.
static void Kalay_StartTrackingLeader(void) {
  struct FadeOverlayWork *work;

  if (GameFlag_IsSet(0x200) == 0) {
    work = gActorEffectWork;
    work->actor = Object_GetById(ACTOR_PARTY_LEADER);
    GameFlag_Set(0x200);
  }
}

// Enable the leader automatic depth priority.
static void Kalay_EnableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags;
  flags = ACTOR_PRIORITY_AUTOMATIC;
  flags |= *priorityFlags;
  *priorityFlags = flags;
}

// Disable the leader automatic depth priority.
static void Kalay_DisableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags;
  flags = (u8)~ACTOR_PRIORITY_AUTOMATIC;
  flags &= *priorityFlags;
  *priorityFlags = flags;
}

// Stop the orbit callback and award the Nut caught in town.
static void Kalay_CatchTownNut(void) {
  Scheduler_RemoveCallback(Kalay_UpdateNutOrbit);
  Engine_TaskWait(1);
  Engine_ActorSetPosition(26, 0, 0);
  Engine_GameFlagSet(0x916);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
}

// Initialize the actors and puzzle state for the current Kalay scene.
s32 Scene_Initialize(void) {
  s32 scene;

  Engine_GameFlagSet(0x87a);
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_Kalay1) {
    Kalay_SetupEntryActors();
  } else if (scene == (s32)&SceneId_Kalay2) {
    Kalay_RestorePalaceEntry();
  } else if (scene == (s32)&SceneId_Kalay3) {
    Kalay_RestoreCaveEntry();
  } else if (scene == (s32)&SceneId_Kalay5) {
    Kalay_StartWindowTransition();
  } else if (scene == (s32)&SceneId_Kalay6) {
    Kalay_RestoreTunnelPuzzles();
  }
  return 0;
}

// Restore town flags and initialize the orbiting Nut when it remains unfound.
static void Kalay_SetupEntryActors(void) {
  OrbitingSceneObject *nut;
  s32 found;
  u8 *heap;
  OrbitingSceneObjectSprite *sprite;

  if (Engine_GameFlagIsSet(0x941) != 0) {
    Engine_GameFlagSet(0x321);
    Engine_GameFlagSet(0x913);
    Engine_GameFlagSet(0x912);
    Engine_GameFlagSet(0x915);
  }
  if (Engine_GameFlagIsSet(0x940) != 0) {
    Engine_GameFlagSet(0x321);
  }
  if (gGameState.entrance == 14) {
    Actor_SetPosition(25, PIXELS(424), PIXELS(88));
  }
  Engine_ActorSetChildValue(21, 2);
  found = Engine_GameFlagIsSet(0x916);
  if (found != 0) {
    Engine_ActorSetPosition(26, 0, 0);
  } else {
    nut = (OrbitingSceneObject *)Object_GetById(26);
    sprite = nut->sprite;
    sprite->flags09Mode = 1;
    sprite->flags05Bit5 = 0;
    sprite->flags09High = 0;
    sprite->state = found;
    nut->active = 1;
    nut->mode = found;
    nut->y = PIXELS(10);
    nut->visible = 1;
    heap = Engine_HeapAllocate(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    Engine_VramLoad(sprite->palette, 128, heap + 0x400);
    Engine_HeapRelease(17);
    nut->orbitAngle = found;
    nut->orbitCenterX = nut->x;
    nut->orbitCenterY = nut->y;
    nut->orbitCenterZ = nut->z;
    FieldScheduler_AddCallback(Kalay_UpdateNutOrbit, KALAY_CALLBACK_PRIORITY);
  }
}

// Restore palace flags and introduce Ivan to the guards on the first visit.
static void Kalay_RestorePalaceEntry(void) {
  if (Engine_GameFlagIsSet(0x941) != 0) {
    Engine_GameFlagSet(0x321);
    Engine_GameFlagSet(0x913);
    Engine_GameFlagSet(0x912);
    Engine_GameFlagSet(0x915);
  }
  if (Engine_GameFlagIsSet(0x940) != 0) {
    Engine_GameFlagSet(0x321);
  }
  if (gGameState.entrance != 0) {
    if (Engine_GameFlagIsSet(0x912) == 0) {
      Kalay_IvanMeetsPalaceGuards();
    }
  }
}

// Restore the cave Nut and trigger the returning Hammet scene when needed.
static void Kalay_RestoreCaveEntry(void) {
  struct FieldActor *guard;

  if (Engine_GameFlagIsSet(0xfd6) == 0) {
    InitializeStagedActorSceneOrbitingEffect(12);
  }
  if (Engine_GameFlagIsSet(0x915) != 0) {
    guard = Object_GetById(8);
    guard->facing = 0;
  }
  if (gGameState.entrance == 10) {
    Kalay_HammetReturnsToPalace();
  }
}

// Restore the opened passage cells and prepare the entrance transition.
static void Kalay_StartWindowTransition(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(0x915) != 0) {
    Map_CopyCellsTo(58, 5, 58, 8, 2, 3);
    {
      Map_CopyCellAttributes(8, 11, 2, 1, 8, 10);
    }
    Map_CopyCellsTo(8, 12, 8, 11, 2, 1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
  if (gGameState.entrance <= 3) {
    BattleFx_SetQueuedSoundAndPlay(170);
  }
}

// Restore tracking, movable blocks, and opened tunnel passages from saved
// flags.
static void Kalay_RestoreTunnelPuzzles(void) {
  s32 resumedScene;
  struct FieldActor *actor;
  s32 trackingValue;
  s32 column;
  struct FadeOverlayWork *effectWork;
  // The resident engine stores fade-overlay work in pointer slot nine.
  u8 **workSlots = (u8 **)&gEventWork;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  BattleFx_StartFadeOverlay(0);
#if TBS_PATCH_1
  resumedScene = GameFlag_IsSet(0x109);
  if (resumedScene != 0) {
    effectWork = (struct FadeOverlayWork *)workSlots[9];
    trackingValue = GameFlag_IsSet(0x200);
    if (trackingValue != 0) {
      trackingValue = Object_GetById(ACTOR_PARTY_LEADER);
    }
    effectWork->actor = (struct FieldActor *)trackingValue;
  } else {
    GameFlag_Set(0x200);
    if (gGameState.entrance == 4) {
      ((struct FadeOverlayWork *)workSlots[9])->actor = NULL;
      GameFlag_Clear(0x200);
    }
  }
#else
  GameFlag_Set(0x200);
  if (GameFlag_IsSet(0x109) != 0) {
    effectWork = (struct FadeOverlayWork *)workSlots[9];
    trackingValue = GameFlag_IsSet(0x200);
    if (trackingValue != 0) {
      trackingValue = Object_GetById(ACTOR_PARTY_LEADER);
    }
    effectWork->actor = (struct FieldActor *)trackingValue;
  } else if (gGameState.entrance == 4) {
    ((struct FadeOverlayWork *)workSlots[9])->actor = NULL;
    GameFlag_Clear(0x200);
  }
#endif
  if (GameFlag_IsSet(0x302) != 0) {
    Actor_SetPosition(11, PIXELS(150), PIXELS(728));
    if (GameFlag_IsSet(0x201) != 0) {
      Actor_Get(11);
      column = 9;
      Engine_ActorSetAnimation(11, 5);
      Map_CopyCellAttributes(0, 0, 1, 1, column, 14);
      Map_CopyCellAttributes(0, 0, 1, 1, column, 45);
      {
        u8 *p = &Object_GetById(11)->priorityFlags;
        s32 v = ACTOR_PRIORITY_UNDERFOOT;

        v |= *p;
        *p = v;
      }
    }
  }
  actor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(9);
  Engine_ActorSetSpriteFlags(actor, 0);
  FieldScheduler_AddCallback(Kalay_UpdateTunnelEmitters,
                             KALAY_CALLBACK_PRIORITY);
  if (GameFlag_IsSet(0x915) != 0) {
    Actor_SetPosition(10, PIXELS(426), PIXELS(730));
    actor = Actor_Get(10);
    {
      u16 shown = 0x5000;

      actor->facing = shown;
    }
    Map_CopyCellsTo(88, 48, 88, 45, 2, 3);
    Map_CopyCellsTo(24, 49, 24, 48, 2, 1);
    Map_CopyCellsTo(25, 42, 25, 47, 1, 1);
    Map_CopyCellAttributes(22, 50, 2, 1, 24, 49);
  }
  if (GameFlag_IsSet(0x302) != 0) {
    Actor_SetPosition(8, PIXELS(232), PIXELS(732));
    Map_CopyCellAttributes(7, 44, 1, 1, 0, 1);
    Map_CopyCellsTo(74, 58, 78, 41, 1, 5);
    Map_CopyCellsTo(16, 109, 13, 109, 3, 2);
    Map_CopyCellsTo(67, 64, 71, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 72, 44, 1, 2);
    Map_CopyCellsTo(67, 68, 73, 43, 1, 2);
    Map_CopyCellsTo(67, 68, 74, 43, 1, 2);
    Map_CopyCellsTo(67, 64, 75, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 76, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 77, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 78, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 79, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 80, 44, 1, 2);
    Map_CopyCellsTo(2, 0, 9, 42, 2, 2);
    Map_CopyCellsTo(68, 64, 71, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 72, 44, 1, 2);
    Map_CopyCellsTo(68, 68, 73, 43, 1, 2);
    Map_CopyCellsTo(68, 68, 74, 43, 1, 2);
    Map_CopyCellsTo(68, 64, 75, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 76, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 77, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 78, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 79, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 80, 44, 1, 2);
    Map_CopyCellsTo(4, 0, 9, 42, 2, 2);
    Map_CopyCellsTo(7, 11, 7, 42, 10, 8);
    Map_CopyCellsTo(71, 12, 71, 43, 10, 13);
    Map_CopyCellAttributes(6, 13, 12, 12, 6, 44);
    Map_CopyCellAttributes(0, 1, 1, 1, 7, 44);
  } else {
    switch (gGameState.entrance) {
    case 1:
    case 2:
      BattleFx_SetQueuedSoundAndPlay(170);
      break;
    }
  }
  if (GameFlag_IsSet(0x303) != 0) {
    Actor_SetPosition(9, PIXELS(696), PIXELS(732));
    Map_CopyCellsTo(74, 58, 107, 41, 1, 5);
    Map_CopyCellsTo(45, 109, 42, 109, 3, 2);
    Map_CopyCellsTo(67, 64, 102, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 103, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 104, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 105, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 106, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 107, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 108, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 109, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 102, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 103, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 104, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 105, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 106, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 107, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 108, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 109, 44, 1, 2);
    Map_CopyCellsTo(38, 14, 38, 44, 8, 4);
    Map_CopyCellsTo(102, 14, 102, 44, 8, 12);
    Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
  } else {
    switch (gGameState.entrance) {
    case 3:
    case 4:
      BattleFx_SetQueuedSoundAndPlay(170);
      break;
    }
  }
}

// Start the idle action script for all eight tourists.
static void Kalay_StartTouristIdleActions(void) {
  Kalay_StartDancerIdleAction(27);
  Kalay_StartDancerIdleAction(28);
  Kalay_StartDancerIdleAction(29);
  Kalay_StartDancerIdleAction(30);
  Kalay_StartDancerIdleAction(32);
  Kalay_StartDancerIdleAction(31);
  Kalay_StartDancerIdleAction(33);
  Kalay_StartDancerIdleAction(34);
}

// Seed an actor idle phase and start its random action script.
static void Kalay_StartDancerIdleAction(s32 actorId) {
  struct FieldActor *actor;
  u32 random;

  actor = Object_GetById(actorId);
  actor->timer = actorId;
  random = Engine_RandomNext();
  actor->work = (random * 5) >> 12;
  Engine_ObjectSetScript(actor, (s32)gKalay_Script01);
}

// Speed up the two guides and start their alternating dance callback.
static void Kalay_SpeedUpActors(void) {
  gKalay_DanceStep = 0;
  Actor_SetSpeed(20, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(21, PIXELS(8) / 5, PIXELS(4) / 5);
  FieldScheduler_AddCallback(Kalay_AlternateDance, KALAY_CALLBACK_PRIORITY);
}

// Alternate the two guide scripts as each completion timer is set.
static void Kalay_AlternateDance(void) {
  switch (gKalay_DanceStep) {
  case 0:
    Kalay_ClearDanceCompletion(Object_GetById(21));
    Actor_SetActionScript(21, gKalay_DanceScriptA);
    gKalay_DanceStep++;
    break;
  case 1:
    if (Object_GetById(21)->timer != 0) {
      Kalay_ClearDanceCompletion(Object_GetById(20));
      Actor_SetActionScript(20, gKalay_DanceScriptB);
      gKalay_DanceStep++;
    }
    break;
  case 2:
    if (Object_GetById(20)->timer != 0) {
      Kalay_ClearDanceCompletion(Object_GetById(20));
      Actor_SetActionScript(20, gKalay_DanceScriptA);
      gKalay_DanceStep++;
    }
    break;
  case 3:
    if (Object_GetById(20)->timer != 0) {
      Kalay_ClearDanceCompletion(Object_GetById(21));
      Actor_SetActionScript(21, gKalay_DanceScriptB);
      gKalay_DanceStep++;
    }
    break;
  case 4:
    if (Object_GetById(21)->timer != 0) {
      gKalay_DanceStep = 0;
    }
    break;
  }
}

// Introduce Ivan to the palace guards and lead the party through the door.
static void Kalay_IvanMeetsPalaceGuards(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(192), PIXELS(342));
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(1) / 5, PIXELS(1) / 40);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(252), 1);
  gEventWork->transitionFrames = 40;
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 278);
  Actor_SetSpeed(8, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(9, PIXELS(1), PIXELS(1) / 2);
  actor = Actor_Get(8);
  actor->facing = FACING_SOUTHEAST + FACING_STEP;
  actor = Actor_Get(9);
  actor->facing = FACING_SOUTH + FACING_STEP;
  Engine_TaskWait(1);
  Actor_Get(8)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(9)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(8, 184, 232);
  Actor_WalkToAndWait(9, 198, 232);
  Engine_ActorSetAnimation(8, ANIM_STAND);
  Engine_EventWait(20);
  Actor_Get(8)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(9)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(MsgKalayLordHammetsPalaceLordAway);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Event_ShowMessageAndWait(9, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(IVAN, 212, 268);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WalkToAndWait(IVAN, 202, 254);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimationAndWait(8, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_ShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_OpenMessage(8, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
    gEventWork->message++;
  } else {
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  }
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(9, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Actor_FaceDirection(9, FACING_WEST, 10);
  Event_ShowMessageAndWait(9, 0, 10);
  Actor_FaceDirection(8, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_SetSpeed(8, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorSetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
  actor = Actor_Get(8);
  actor->facing = 0;
  actor = Actor_Get(9);
  actor->facing = FACING_WEST;
  Actor_Get(8)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(9)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(8, 168, 232);
  Actor_WalkToAndWait(9, 212, 232);
  Engine_ActorSetAnimation(8, ANIM_STAND);
  Engine_EventWait(20);
  Actor_Get(8)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(9)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(IVAN, 192, 232);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kalay_AnimatePalaceDoor();
  Actor_WalkToAndWait(IVAN, 192, 222);
  // Lead the party and guards through the opened palace door.
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 222);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_ActorSetAnimation(8, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Actor_WalkTo(8, 184, 232);
  Actor_WalkToAndWait(9, 198, 232);
  Actor_WalkTo(8, 188, 212);
  Actor_WalkToAndWait(9, 194, 212);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  gEventWork->transitionFrames = 24;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(5);
  Engine_EventEnd();
}

// Bring Hammet back to the palace and play the party conversation.
static void Kalay_HammetReturnsToPalace(void) {
  u8 *leaderPhase;
  u32 random;
  u32 motionPhase;
  struct FieldActor *object;
  struct EventWork *work;
  const s32 *motionActions;
  s32 southFacing;
  const s32 *exitActions;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(9, PIXELS(440), PIXELS(522));
  object = Actor_Get(9);
  Engine_ActorSetSpriteFlags(object, 0);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(522), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(400), 1);
  Event_PlayCue(141);
  Actor_SetSpeed(9, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(9, 440, 400);
  Camera_SetSpeed(PIXELS(3) / 4, PIXELS(3) / 32);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(9, 440, 300);
  Engine_ActorSetAnimation(9, 0);
  Event_PlayCue(0x121);
  Engine_EventWait(40);
  Actor_SetSpeed(11, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(11, PIXELS(439), PIXELS(306));
  Engine_ActorJump(11, 4, 0);
  Actor_WalkToAndWait(11, 439, 312);
  Actor_WalkToAndWait(11, 416, 312);
  Actor_WalkToAndWait(11, 400, 256);
  Actor_FaceDirection(11, FACING_SOUTHEAST + FACING_STEP, 40);
  Camera_MoveTo(PIXELS(410), -1, PIXELS(280), 1);
  Actor_SetSpeed(10, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetPosition(10, PIXELS(439), PIXELS(306));
  Engine_ActorJump(10, 4, 0);
  Actor_WalkToAndWait(10, 439, 312);
  Actor_WalkToAndWait(10, 416, 312);
  Actor_WalkToAndWait(10, 388, 270);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 10);
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(439), PIXELS(306));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 388, 300);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  // Seed the party idle phases for the arrival conversation.
  leaderPhase = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
  random = Random_Next();
  motionPhase = random * 5;
  motionActions = gKalay_Data02;
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = motionPhase >> 12;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, motionActions);
  Engine_ActorJump(11, 2, 20);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventSetMessage(MsgKalayWeveArrivedHammet);
  Event_ShowMessageAndWait(11, 0, 10);
  Engine_ActorSetAnimationAndWait(10, 3);
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(GARET, object->x.fixed, object->z.fixed);
  }
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(IVAN, object->x.fixed, object->z.fixed);
  }
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(MIA, object->x.fixed, object->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(GARET, 378, 310);
  Actor_WalkTo(IVAN, 400, 288);
  Actor_WalkToAndWait(MIA, 410, 308);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  leaderPhase = (u8 *)Actor_Get(GARET);
  random = Random_Next();
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)Actor_Get(IVAN);
  random = Random_Next();
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)Actor_Get(MIA);
  random = Random_Next();
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  Actor_SetActionScript(GARET, motionActions);
  Actor_SetActionScript(IVAN, motionActions);
  Actor_SetActionScript(MIA, motionActions);
  Actor_ShowEmote(IVAN, 0x101, 60);
  Event_ShowMessageAndWait(0x2002, 0, 10);
  Actor_ShowEmote(10, 0x100, 40);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(10, 0, 10);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Engine_TaskWait(1);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(10, 3);
  Event_ShowMessageAndWait(10, 0, 10);
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessageAndWait(0x2003, 0, 10);
  Engine_ActorSetAnimationAndWait(10, 3);
  Event_ShowMessageAndWait(10, 0, 10);
  Actor_ShowEmote(GARET, 0x101, 60);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorRunRepeatedMotion(11, 2);
  Event_ShowMessageAndWait(11, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 10);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(10, 3);
  Event_ShowMessageAndWait(10, 0, 10);
  Engine_ActorSetAnimationAndWait(11, 3);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(0x2002, 0, 10);
  Actor_FaceDirection(11, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(11, 4);
  Event_ShowMessageAndWait(11, 0, 10);
  Actor_ShowEmote(MIA, 0x101, 40);
  Event_ShowMessageAndWait(0x2003, 0, 10);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(10, 0x108, 20);
  Event_ShowMessageAndWait(10, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimation(11, 4);
  Event_ShowMessageAndWait(11, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(11, 4);
  Event_ShowMessageAndWait(11, 0, 10);
  Engine_ActorSetAnimationAndWait(10, 4);
  Event_ShowMessageAndWait(10, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
  Actor_ShowEmote(GARET, 0x105, 0);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Actor_ShowEmote(MIA, 0x105, 60);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(10, 3);
  Event_OpenMessage(10, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    gEventWork->message += 3;
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 40);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_ShowEmote(MIA, 0x106, 40);
    Actor_FaceDirection(MIA, FACING_NORTH, 20);
    Event_ShowMessageAndWait(0x2003, 0, 10);
  }
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 10);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimationAndWait(10, 3);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(280), -1, PIXELS(200), 1);
  Actor_WalkToAndWait(10, 333, 222);
  Actor_WalkToAndWait(10, 284, 198);
  Actor_FaceDirection(10, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(10, 3);
  Actor_FaceDirection(8, 0, 10);
  Engine_ActorSetAnimationAndWait(8, 3);
  Actor_SetSpeed(8, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(8, 268, 198);
  Actor_FaceDirection(8, FACING_NORTH, 10);
  Engine_ActorRunRepeatedMotion(8, 2);
  // Open the passage and let the returning visitors enter the palace.
  Event_PlayCue(125);
  Map_CopyCellsTo(71, 60, 76, 11, 2, 1);
  Map_CopyCellAttributes(71, 60, 2, 1, 16, 11);
  Engine_EventWait(20);
  Actor_WalkToAndWait(8, 246, 198);
  Actor_FaceDirection(8, 0, 20);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(170), 1);
  Actor_WalkToAndWait(10, 270, 198);
  Actor_WalkToAndWait(10, 270, 174);
  Actor_WalkToAndWait(10, 224, 170);
  Actor_WalkToAndWait(10, 210, 158);
  Actor_WalkToAndWait(10, 246, 148);
  Actor_WalkToAndWait(10, 246, 142);
  Actor_SetPosition(10, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Actor_SetPosition(9, PIXELS(440), PIXELS(340));
  object = Actor_Get(9);
  southFacing = FACING_SOUTH;
  object->facing = southFacing;
  Camera_MoveTo(PIXELS(380), -1, PIXELS(280), 0);
  Engine_MapRedraw();
  Engine_TaskWait(10);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(3, 3);
  Actor_WalkToAndWait(11, 420, 282);
  Actor_WalkToAndWait(11, 420, 312);
  Actor_WalkToAndWait(11, 439, 312);
  Actor_WalkToAndWait(11, 439, 306);
  Actor_SetPosition(11, 0, 0);
  // Return the party to the entrance before the final departure.
  exitActions = gKalay_Data03;
  Actor_SetActionScript(GARET, exitActions);
  Actor_SetActionScript(IVAN, exitActions);
  Object_SetActionCallbackAndRefreshById(3, (s32)exitActions);
  Camera_MoveTo(PIXELS(410), -1, PIXELS(300), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 306);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Event_PlayCue(141);
  Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(420), 1);
  Actor_WalkToAndWait(9, 440, 420);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(600), 1);
  Actor_SetSpeed(9, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(9, 440, 500);
  Actor_WalkTo(9, 440, 600);
  Event_PlayCue(0x121);
  work = gEventWork;
  work->transitionFrames = 24;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(10);
  Engine_EventEnd();
}

// Shared source, included in link order.
#include "common/object_findcell.h"

// Push the adjacent block one tile when its destination is clear.
static void Kalay_PushTunnelBlock(void) {
  struct FieldPosition destination;
  struct FieldActor *leader;
  struct FieldActor *block;
  s32 direction;
  s32 probeStep;
  s32 moveStep;
  s32 leaderX;
  struct FieldPosition *target;
  s32 stoppedVelocity;
  s32 pushSpeed;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  direction = (leader->facing >> 12);
  leaderX = leader->x.part.pixel;
  probeStep = gKalay_Data01[direction];
  block =
      SceneActor_FindActorAtCell((leaderX + (probeStep >> 16)) >> 4,
                                 (leader->z.part.pixel + (s16)probeStep) >> 4);
  if (block != NULL) {
    stoppedVelocity = 0;
    block->terrainId = 2;
    target = &destination;
    moveStep = gKalay_Data01[direction];
    target->x = block->x.fixed + (moveStep & -0x10000);
    target->y = block->y.fixed;
    target->z = block->z.fixed + (moveStep << 16);
    if (FieldObject_CheckMovementCollision(block, target) <= 0) {
      Object_SetMode(leader, 8);
      pushSpeed = PIXELS(1) / 5;
      Engine_TaskWait(15);
      Engine_AudioPlayCue(185);
      block->speed = pushSpeed;
      block->acceleration = pushSpeed;
      Engine_ObjectSetPosition(block, target->x, target->y, target->z);
      leader->speed = pushSpeed;
      leader->acceleration = pushSpeed;
      Engine_ObjectSetPosition(leader, target->x, target->y, target->z);
      Object_CommitPosition(block);
      BattleFx_PlayQueuedSound();
      block->x.fixed = target->x;
      block->z.fixed = target->z;
      block->velocityX = stoppedVelocity;
      block->velocityZ = stoppedVelocity;
      Object_SetMode(leader, 1);
      Kalay_UpdateTunnelPassages();
    }
  }
}

// Animate each tunnel passage when its movable block reaches the opening
// position.
static void Kalay_UpdateTunnelPassages(void) {
  struct FieldActor *firstBlock;
  struct FieldActor *secondBlock;
  s32 firstPassageOpen;
  struct FieldActor *frostEffect;
  u32 i;
  s32 span;
  s32 height;
  s32 width;

  // Open each passage only after its corresponding block reaches the trigger.
  firstBlock = Object_GetById(8);
  secondBlock = Object_GetById(9);
  firstPassageOpen = GameFlag_IsSet(0x302);
  if (firstPassageOpen != 0) {
  } else if ((firstBlock->x.fixed >> 19) > 29) {
  } else {
    frostEffect = Object_GetById(11);
    Engine_EventBegin();
    Map_CopyCellAttributes(7, 44, 1, 1, firstPassageOpen, 1);
    i = 67;
    width = 1;
    height = 5;
    do {
      Map_CopyCellsTo(i, 58, 78, 41, width, height);
      Engine_EventWait(4);
      if (i == 70) {
        GameFlag_Set(0x302);
      }
      i++;
    } while (i <= 74);
    span = 2;
    Map_CopyCellsTo(16, 109, 13, 109, 3, span);
    Engine_EventWait(40);
    frostEffect->scaleX = Q16_ONE / 10;
    frostEffect->scaleY = Q16_ONE / 10;
    Actor_SetPosition(11, PIXELS(150), PIXELS(728));
    Actor_SetActionScript(11, gKalay_ActionScript01);
    height = 1;
    Map_CopyCellsTo(67, 64, 71, 44, height, span);
    Map_CopyCellsTo(67, 64, 72, 44, height, span);
    Map_CopyCellsTo(67, 68, 73, 43, height, span);
    Map_CopyCellsTo(67, 68, 74, 43, height, span);
    Map_CopyCellsTo(67, 64, 75, 44, height, span);
    Map_CopyCellsTo(67, 66, 76, 44, height, span);
    Map_CopyCellsTo(67, 64, 77, 44, height, span);
    Map_CopyCellsTo(67, 64, 78, 44, height, span);
    Map_CopyCellsTo(67, 64, 79, 44, height, span);
    Map_CopyCellsTo(67, 66, 80, 44, height, span);
    Map_CopyCellsTo(2, 0, 9, 42, span, span);
    Engine_EventWait(40);
    Map_CopyCellsTo(68, 64, 71, 44, height, span);
    Map_CopyCellsTo(68, 64, 72, 44, height, span);
    Map_CopyCellsTo(68, 68, 73, 43, height, span);
    Map_CopyCellsTo(68, 68, 74, 43, height, span);
    Map_CopyCellsTo(68, 64, 75, 44, height, span);
    Map_CopyCellsTo(68, 66, 76, 44, height, span);
    Map_CopyCellsTo(68, 64, 77, 44, height, span);
    Map_CopyCellsTo(68, 64, 78, 44, height, span);
    Map_CopyCellsTo(68, 64, 79, 44, height, span);
    Map_CopyCellsTo(68, 66, 80, 44, height, span);
    Map_CopyCellsTo(4, 0, 9, 42, span, span);
    Engine_EventWait(40);
    span = 10;
    Map_CopyCellsTo(7, 11, 7, 42, span, 8);
    Map_CopyCellsTo(71, 12, 71, 43, span, 13);
    span = 44;
    Map_CopyCellAttributes(6, 13, 12, 12, 6, span);
    Engine_EventWait(40);
    Kalay_StartTrackingLeader();
    Map_CopyCellAttributes(0, 1, 1, 1, 7, span);
    Engine_EventEnd();
  }
  if (GameFlag_IsSet(0x303) != 0) {
  } else if ((secondBlock->x.fixed >> 19) > 87) {
  } else {
    Engine_EventBegin();
    i = 67;
    width = 1;
    height = 5;
    do {
      Map_CopyCellsTo(i, 58, 107, 41, width, height);
      Engine_EventWait(4);
      if (i == 70) {
        GameFlag_Set(0x303);
      }
      i++;
    } while (i <= 74);
    height = 2;
    Map_CopyCellsTo(45, 109, 42, 109, 3, height);
    Engine_EventWait(40);
    span = 1;
    Map_CopyCellsTo(67, 64, 102, 44, span, height);
    Map_CopyCellsTo(67, 64, 103, 44, span, height);
    Map_CopyCellsTo(67, 64, 104, 44, span, height);
    Map_CopyCellsTo(67, 66, 105, 44, span, height);
    Map_CopyCellsTo(67, 64, 106, 44, span, height);
    Map_CopyCellsTo(67, 64, 107, 44, span, height);
    Map_CopyCellsTo(67, 64, 108, 44, span, height);
    Map_CopyCellsTo(67, 66, 109, 44, span, height);
    Engine_EventWait(40);
    Map_CopyCellsTo(68, 64, 102, 44, span, height);
    Map_CopyCellsTo(68, 64, 103, 44, span, height);
    Map_CopyCellsTo(68, 64, 104, 44, span, height);
    Map_CopyCellsTo(68, 66, 105, 44, span, height);
    Map_CopyCellsTo(68, 64, 106, 44, span, height);
    Map_CopyCellsTo(68, 64, 107, 44, span, height);
    Map_CopyCellsTo(68, 64, 108, 44, span, height);
    Map_CopyCellsTo(68, 66, 109, 44, span, height);
    Engine_EventWait(40);
    span = 8;
    Map_CopyCellsTo(38, 14, 38, 44, span, 4);
    Map_CopyCellsTo(102, 14, 102, 44, span, 12);
    Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
    Engine_EventWait(40);
    Kalay_StartTrackingLeader();
    Engine_EventEnd();
  }
}

// UB: the native void epilogue returns through r0, leaving the nonzero return
// address as SC_CALL's status. Repeat once per frame until the object is
// destroyed, with an explicit status in the modern build.
// Advance a particle, damp its velocity, and expire its lifetime.
#ifdef UBFIX
static s32 Kalay_UpdateTunnelParticle(struct ScriptInterpreter *interpreter) {
#else
static void Kalay_UpdateTunnelParticle(struct ScriptInterpreter *interpreter) {
#endif
  struct FieldActor *particle = (struct FieldActor *)interpreter;
  s16 *lifetime;
  s32 remainingFrames;
  s32 velocityX;
  s32 velocityY;

  lifetime = &particle->timer;
  remainingFrames = *lifetime;
  if (remainingFrames == 0) {
    Object_Destroy((struct DispatchObject *)particle);
  } else if (remainingFrames == 1) {
    particle->velocityX = 0;
    particle->velocityY = 0;
    particle->x.fixed = 0;
    particle->y.fixed = 0;
  } else {
    particle->scaleX += Q16_ONE / 32;
    particle->scaleY += Q16_ONE / 32;
  }
  particle->x.fixed += particle->velocityX;
  particle->y.fixed += particle->velocityY;
  velocityX = particle->velocityX;
  velocityY = particle->velocityY;
  particle->velocityX = velocityX - velocityX / 256;
  particle->velocityY = velocityY - velocityY / 16;
  {
    s32 nextLifetime = *(u16 *)lifetime;
    nextLifetime -= 1;
    *(u16 *)lifetime = nextLifetime;
  }
#ifdef UBFIX
  return 1;
#endif
}

// Spawn effects at the two blocks while their passages remain closed.
static void Kalay_UpdateTunnelEmitters(void) {
  struct FieldActor *firstEmitter;
  struct FieldActor *secondEmitter;

  firstEmitter = Actor_Get(8);
  secondEmitter = Actor_Get(9);
  if ((u32)(firstEmitter->x.part.pixel - 381) <= 12 &&
      firstEmitter->z.part.pixel > 777) {
    firstEmitter->sprite->image.priority =
        Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
  } else if (!GameFlag_IsSet(0x302) && firstEmitter->x.part.pixel <= 245 &&
             (gFrameCount & 1) == 0) {
    if (!GameFlag_IsSet(0x202)) {
      BattleFx_SetQueuedSoundAndPlay(-1);
      Event_PlayCue(230);
      GameFlag_Set(0x202);
    }
    Kalay_SpawnTunnelParticle(firstEmitter->x.fixed, firstEmitter->y.fixed,
                              firstEmitter->z.fixed);
  }
  if (!GameFlag_IsSet(0x303) && secondEmitter->x.part.pixel <= 709 &&
      (gFrameCount & 1) == 0) {
    if (!GameFlag_IsSet(0x203)) {
      BattleFx_SetQueuedSoundAndPlay(-1);
      Event_PlayCue(230);
      GameFlag_Set(0x203);
    }
    Kalay_SpawnTunnelParticle(secondEmitter->x.fixed, secondEmitter->y.fixed,
                              secondEmitter->z.fixed);
  }
}

// Create a short-lived particle with random position and velocity offsets.
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z) {
  s32 depth;
  struct FieldActor *particle;
  s32 random;
  s32 attributeMask;
  u8 *spriteBytes;

  depth = z;
  random = Engine_RandomNext();
  particle = Engine_ObjectCreate(
      222, (x - PIXELS(8)),
      (((((u32)(random << 3) >> 16) << 16) + height) + PIXELS(16)), depth);
  if (particle != 0) {
    ((u8 *)particle)[offsetof(struct FieldActor, motionFlags)] =
        (attributeMask = 0);
    spriteBytes = (u8 *)particle->sprite;
    attributeMask -= 13;
    spriteBytes[9] = ((spriteBytes[9] & attributeMask) | 8);
    ObjectGroup_SetChildValue(particle, 9);
    Engine_ActorSetSpriteFlags(particle, 0);
    random = Random_Next();
    particle->velocityX = ((((u32)(random << 1) >> 16) - 1) << 16);
    random = Random_Next();
    particle->velocityY =
        ((((u32)(((random << 1) + random) << 1) >> 16) - 3) << 16);
    {
      u16 *target = (u16 *)&particle->timer;
      u16 shown = 20;

      *target = shown;
      particle->unk61 = 1;
    }
    Object_SetMode(particle, 1);
    Engine_ObjectSetScript(particle, (s32)gKalay_Script02);
  }
}

// Move the town Nut around its center and update the sprite rotation.
static void Kalay_UpdateNutOrbit(void) {
  OrbitingSceneObject *effect = (OrbitingSceneObject *)Object_GetById(26);
  struct AnimationObject *sprite = (struct AnimationObject *)effect->sprite;
  s32 offset = Engine_MathSin(effect->orbitAngle) * 2;
  s32 first;

  if (offset > 0) {
    offset = -offset;
  }
  effect->x = effect->orbitCenterX + Engine_MathCos(effect->orbitAngle) * 2;
  effect->y = effect->orbitCenterY + offset;
  sprite->rotation = Engine_MathCos(effect->orbitAngle + 0x8000) >> 3;
  first = Engine_RandomNext();
  effect->orbitAngle += ((u32)(first << 9) >> 16) +
                        ((u32)(Engine_RandomNext() << 9) >> 16) + 0x400;
}

// Freeze tunnel actor 11, lower it, and restore its collision cells.
static void Kalay_FreezeTunnelActor11(void) {
  struct FieldActor *effect = Object_GetById(11);

  Map_CopyCellAttributes(0, 0, 1, 1, 9, 14);
  Map_CopyCellAttributes(0, 0, 1, 1, 9, 45);
  if (effect != 0) {
    Engine_ActorSetSpriteFlags(effect, 0);
    effect->y.fixed -= PIXELS(32);
    effect->priorityFlags = 2;
  }
  Engine_GameFlagSet(0x201);
}

// Shared source, included in link order.
#include "common/object_orbit.h"

// One tile in each facing direction: x in the high 16 bits, z in the low 16.
// UB: negative x coordinates cannot be left-shifted as signed integers.
#ifdef UBFIX
#define FACING_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))
#else
#define FACING_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

s32 gKalay_Data01[] = {
    FACING_STEP(16, 0),  FACING_STEP(16, 0),  FACING_STEP(16, 0),
    FACING_STEP(0, 16),  FACING_STEP(0, 16),  FACING_STEP(0, 16),
    FACING_STEP(0, 16),  FACING_STEP(-16, 0), FACING_STEP(-16, 0),
    FACING_STEP(-16, 0), FACING_STEP(-16, 0), FACING_STEP(0, -16),
    FACING_STEP(0, -16), FACING_STEP(0, -16), FACING_STEP(0, -16),
    FACING_STEP(16, 0),
};

s32 gSceneAction_GroupMotion[] = {
    SC_ARG(2),       SC_MOVE_BY(-PIXELS(80), 0, 0),
    SC_SYNC,         SC_ARG(1),
    SC_SET(0x23, 1), SC_WAIT(1),
    SC_END,
};

s32 gSceneAction_GroupOffsetMotion[] = {
    SC_ARG(2),       SC_MOVE_BY(-PIXELS(72), 0, 0),
    SC_SYNC,         SC_MOVE_BY(-PIXELS(100), 0, 0),
    SC_SYNC,         SC_MOVE_BY(-PIXELS(100), 0, 0),
    SC_SYNC,         SC_ARG(1),
    SC_SET(0x23, 1), SC_POS(0, 0, 0),
    SC_WAIT(1),      SC_END,
    SC_ARG(2),       SC_MOVE(PIXELS(192), 0, PIXELS(222)),
    SC_SYNC,         SC_POS(0, 0, 0),
    SC_WAIT(1),      SC_END,
};

s32 gKalay_Script01[] = {
    SC_CALL(Kalay_UpdateDancerIdleAction),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKalay_DanceScriptA[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(228), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_SET(0x23, 1),
    SC_ARG(2),
    SC_MOVE(PIXELS(178), 0, PIXELS(272)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_DanceScriptB[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(164), 0, PIXELS(278)),
    SC_SYNC,
    SC_MOVE(PIXELS(182), 0, PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(238), 0, PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(246), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(40),
    SC_SET(0x23, 1),
    SC_END,
};

s32 gKalay_DanceScriptC[] = {
    SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
    SC_SET(SW_VELOCITY_X, 0),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_SET(SW_VELOCITY_Z, 0),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(184), 0, PIXELS(294)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_DanceScriptD[] = {
    SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
    SC_SET(SW_VELOCITY_X, 0),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_SET(SW_VELOCITY_Z, 0),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(198), 0, PIXELS(300)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_DanceScriptE[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(178), 0, PIXELS(272)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_DanceScriptF[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(255), 0, PIXELS(289)),
    SC_SYNC,
    SC_MOVE(PIXELS(265), 0, PIXELS(280)),
    SC_SYNC,
    SC_MOVE(PIXELS(284), 0, PIXELS(272)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_SET(0x23, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_Data02[] = {
    SC_CALL(OverlayObject_UpdateHeadingTimer),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKalay_Data03[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(388), 0, PIXELS(300)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gKalay_ActionScript01[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

struct SceneEntrance gKalayEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayEntrances1[] = {
    {0, CONDITION_ALWAYS, 224, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 72, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 408, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 408, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 408, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 328, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 120, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 312, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 152, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 224, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 56, 0, 272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 224, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 296, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 152, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 440, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayEntrances2[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 192, 0, 280, FACING_NORTH, 0, 0, 0, 400, 352, 0},
    {2, CONDITION_ALWAYS, 192, 0, 232, FACING_SOUTH, 0, 0, 0, 400, 352, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayEntrances3[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 248, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 424, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 440, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayEntrances4[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 296, 0, 104, FACING_SOUTH, 0, 100, 50, 440, 445, 0},
    {2, CONDITION_ALWAYS, 280, 0, 392, FACING_NORTH, 0, 100, 50, 440, 445, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayEntrances5[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 296, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 632, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 152, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 568, 0, 152, FACING_SOUTH, 0, 390, 0, 630, 220, 0},
    {5, CONDITION_ALWAYS, 472, 0, 72, FACING_SOUTH, 0, 390, 0, 630, 220, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayEntrances6[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 408, 0, 632, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {2, CONDITION_ALWAYS, 296, 0, 488, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {3, CONDITION_ALWAYS, 744, 0, 488, FACING_SOUTH, 0, 595, 400, 930, 880, 0},
    {4, CONDITION_ALWAYS, 856, 0, 856, FACING_NORTH, 0, 595, 400, 930, 880, 0},
    {SCENE_TABLE_END},
};

u32 gKalay_SceneTable01[] = {
    SCENE_EXITS(99),        SCENE_EXIT(1, 100, 1),  SCENE_EXIT(2, 100, 2),
    SCENE_EXIT(3, 100, 3),  SCENE_EXIT(4, 100, 4),  SCENE_EXIT(5, 100, 6),
    SCENE_EXIT(6, 100, 7),  SCENE_EXIT(7, 100, 10), SCENE_EXIT(8, 101, 1),
    SCENE_EXIT(9, 2, 20),   SCENE_EXIT(10, 2, 19),  SCENE_EXIT(11, 102, 1),
    SCENE_EXIT(12, 100, 5), SCENE_EXIT(13, 100, 8), SCENE_EXIT(14, 156, 4),
    SCENE_EXIT(0, 102, 0),  SCENE_EXIT(1, 99, 11),  SCENE_EXIT(2, 103, 1),
    SCENE_EXIT(5, 103, 20), SCENE_EXIT(0, 153, 0),  SCENE_EXIT(1, 154, 2),
    SCENE_EXIT(2, 2, 60),   SCENE_EXIT(10, 2, 71),  SCENE_EXIT(0, 154, 0),
    SCENE_EXIT(1, 155, 5),  SCENE_EXIT(2, 153, 1),  SCENE_EXIT(0, 155, 0),
    SCENE_EXIT(1, 156, 2),  SCENE_EXIT(2, 156, 3),  SCENE_EXIT(3, 155, 4),
    SCENE_EXIT(4, 155, 3),  SCENE_EXIT(5, 154, 1),  SCENE_EXIT(0, 156, 0),
    SCENE_EXIT(1, 103, 11), SCENE_EXIT(2, 155, 1),  SCENE_EXIT(3, 155, 2),
    SCENE_EXIT(4, 99, 14),  SCENE_EXITS_END,
};

struct ScenePlacement gKalayPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalayPlacements1[] = {
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(188), 0, PIXELS(476), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(260), 0, PIXELS(476), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(222), 0, PIXELS(382), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(134), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(115), 0, PIXELS(453), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(484), 0, PIXELS(258), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(359), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(404), 0, PIXELS(268), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(154), 0, PIXELS(119), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(198), 0, PIXELS(84), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(199), 0, PIXELS(233), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(46), 0, PIXELS(217), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(468), 0, PIXELS(434), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(464), 0, PIXELS(416), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(408), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(318), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(56), 0, PIXELS(332), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalay_StagedGroupScript[] = {
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayPlacements2[] = {
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayPlacements3[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x111, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x31, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x44, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0xfd6, 7,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayPlacements6[] = {
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(826), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(732), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x44, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5c, 0x5b, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(712), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKalayEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayEvents1[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RunExitDoor) },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_WatchTourGroupDeparture) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_WatchTourGroupDeparture) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayWelcomeInPeace) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersProtectPeace) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayHammetGenerousMerchant) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayHammetOnJourney) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayNorthernBridgeRebuilt) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayLamakanTrouble) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKalayElderTownNews) },
    { EVENT_TALK, 15, 0x911, MSGNO(MsgKalayInnFestivalTourists) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKalayFormerInnFestivalTourists) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgKalayChildWantsFestival) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayTraderTownNews) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKalayHammetBusinessSkill) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkAboutLeavingHome) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCapturedRumor) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgKalayHammetWentNorth) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToMerchant22) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToWeaponMerchant) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToMerchant24) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorHonestEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayGuardsJudgeVisitors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayHammetPeaceAndLiberty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCanBeRash) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersWentToVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayStrangeEvents) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKalayElderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKalayWishSavedForFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayChildFestivalDream) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayTraderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKalayHammetValuesTrust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKalayTrustAgainstForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKalayLunpaNorthOfVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKalayRumorsHideTruth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKalayWishBetterWeapons) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKalayWeaponsHaveSoul) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgKalayPalaceWaterSupply) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 25, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, 0x916, EVENT_SCRIPT(Kalay_CatchTownNut) },
    { SEARCH(SEARCH_JAR), 100, 0xf81, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_JAR), 101, 0xf82, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_SearchWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_ExitPalaceDoor) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersHandleAffairs) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersRescueHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayPrayForHammetSafety) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorsWillSaveHammet) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_AskAboutCaveAccess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayPassageClosedByOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, 0xfd6, EVENT_SCRIPT(Kalay_CatchCaveNut) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayEvents4[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { SEARCH(SEARCH_CHEST), 100, 0xf22, FIND_ITEM(0xc1) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayEvents5[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { 0x31, 4, CONDITION_ALWAYS, 4 },
    { 0x21, 5, CONDITION_ALWAYS, 5 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayEvents6[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x21, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkAboutLayana) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTreasureBeyondDoors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x302, EVENT_SCRIPT(Kalay_UpdateTunnelPassages) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x303, EVENT_SCRIPT(Kalay_UpdateTunnelPassages) },
    { 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x201, EVENT_SCRIPT(Kalay_FreezeTunnelActor11) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_StopTrackingLeader) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_StartTrackingLeader) },
    { EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_DisableLeaderAutomaticPriority) },
    { EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_EnableLeaderAutomaticPriority) },
    { SEARCH(SEARCH_CHEST), 100, 0xf1c, FIND_ITEM(0x84) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RecruitDjinn) },
    { SEARCH(SEARCH_CHEST), 102, 0xf1e, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_CHEST), 103, 0xf1f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 104, 0xf20, FIND_COINS(200) },
    { SEARCH(SEARCH_CHEST), 105, 0xf21, FIND_ITEM(0xb7) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gKalay_DoorCellData[] = {
    MAP_CELL_STEP(35, 33, 1, 2, 5),
    MAP_CELL_STEP(37, 33, 1, 2, 5),
    MAP_CELL_STEPS_END,
};

struct MapCellAnim gKalay_DoorCells[] = {
    {gKalay_DoorCellData, 36, 10}, {gKalay_DoorCellData, 57, 7},
    {gKalay_DoorCellData, 57, 11}, {gKalay_DoorCellData, 57, 23},
    {gKalay_DoorCellData, 52, 25}, {gKalay_DoorCellData, 39, 14},
    {gKalay_DoorCellData, 51, 14}, {gKalay_DoorCellData, 41, 25},
};

s32 gKalay_Script02[] = {
    SC_CALL(Kalay_UpdateTunnelParticle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
