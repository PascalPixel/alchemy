#include "game_state.h"
#include "version.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/scene/vale_home.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
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
#include "resource.h"
#include "global_cells.h"
#include "vale_home.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_service.h"
#include "types.h"
#include "animspr.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "facing_object.h"
#include "field_obj.h"
#include "field_effect.h"
#include "fixed_math.h"

// Forward declarations.
static void ValeHome_MeetSurvivingTravelers(void);
static void ValeHome_FightTravelers(void);
static void ValeHome_TravelersLeaveAfterBattle(void);
static void ValeHome_DepartForQuest(void);
static void Event_SayThenWait(s32 speaker, s32 frames);
static void ValeHome_FaceDirectionAndWait(s32 actorId, s32 facing, s32 frames);
static void ValeHome_FlashPsynergyPalette(struct FieldActor *actor);
static void ValeHome_SpawnEffectPair(struct FieldActor *parent);
static void ValeHome_UpdateActor15Psynergy(void);
static void ValeHome_UpdateActor17Psynergy(void);
static void ValeHome_UpdateActor20Psynergy(void);
static void ValeHome_SetProgressFlagsAndExit(void);

// Turn and wait by calling ValeHome_FaceDirectionAndWait through this inline.
static inline void ValeHome_TurnAndWait(s32 actor, s32 facing, s32 frames) {
  /* FAKEMATCH: forwarding inline for ValeHome_DepartForQuest's three
   * calls (call.h's Call3 did the same); called directly the function grows by
   * 4 bytes and builds its stack arguments in other registers (2026-10-13). */
  ValeHome_FaceDirectionAndWait(actor, facing, frames);
}

#ifdef UBFIX
#define VALE_HOME_TURN_AND_WAIT ValeHome_FaceDirectionAndWait
#else
static inline s32 ValeHome_TurnAndWaitValue(s32 actor, s32 facing, s32 frames) {
  /* FAKEMATCH: forwarding inline called as returning a value, its result
   * discarded (call.h's Value3 did the same); through ValeHome_TurnAndWait the
   * argument allocation changes, called directly differ 30 (r5 and r6 swap),
   * and through a plain s32 cast ValeHome_SendHelpToElders differs 15 halfwords
   * (2026-10-13). Forced tbs-en on 2026-10-16: a direct void call changes 15
   * Thumb halfwords across ValeHome_SendHelpToElders (15 text bytes), with r5
   * and r6 swapped.
   */
  return ((s32 (*)())ValeHome_FaceDirectionAndWait)(actor, facing, frames);
}
#define VALE_HOME_TURN_AND_WAIT ValeHome_TurnAndWaitValue
#endif

// Shared source is included here in ROM order.
#include "common/object_facing.h"

// Return the village's entrance positions.
struct SceneEntrance *ValeHome_GetEntrances(void) { return gValeHomeEntrances; }

// This overlay callback returns zero.
s32 ValeHome_GetDefaultResult(void) { return 0; }

// Select the storm or ordinary village exits.
u32 *ValeHome_SelectExits(void) {
  if (Engine_GameFlagIsSet(0x834) != 0) {
    return gValeHomeExitsFlag834;
  }
  return gValeHomeExits;
}

// Select the villagers for the storm, farewell or return visit.
struct ScenePlacement *ValeHome_SelectPlacements(void) {
  if (Engine_GameFlagIsSet(0x834) != 0) {
    return gValeHomePlacementsFlag834;
  }
  if (gGameState.entrance == 12) {
    return gValeHomePlacementsEntrance12;
  }
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    return gValeHomePlacementsFlag87a;
  }
  return gValeHomePlacements;
}

// Restore the party's PP at the Psynergy Stone.
static void ValeHome_UsePsynergyStone(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeCheckedThePsynergyStone, 1);
  Engine_AudioPlayCue(126);
  BattleParty_ApplyDrain(999, 0);
  Engine_EventWait(10);
  Engine_MessageShowCentered(MsgValePartyPpRestored, 1);
  UiWork_FinalizePendingCore();
  Engine_GameFlagClear(322);
  Engine_EventEnd();
}

// Select the village's interactions for the current story state.
struct SceneEvent *ValeHome_SelectEvents(void) {
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    return gValeHomeEventsFlag87a;
  }
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    return gValeHomeEventsAfterLeaving;
  }
  if (gGameState.entrance == 12) {
    return gValeHomeEventsEntrance12;
  }
  if (Engine_GameFlagIsSet(0x834) != 0) {
    return gValeHomeEventsFlag834;
  }
  return gValeHomeEvents;
}

// Ask whether the party meditates on Mt. Aleph each day.
static void ValeHome_DiscussDailyMeditation(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeMeditateOnMtAlephDaily);
  Engine_ActorFaceEachOther(23, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(23, 0);
  Engine_EventEnd();
}

// Discuss the difficult events of three years ago.
static void ValeHome_DiscussThreeYearsAgo(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeADifficultTimeThreeYears);
  Engine_ActorFaceEachOther(24, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(24, 0);
  Engine_EventEnd();
}

// Ask whether the travelers went to meet the mayor.
static void Villager_AskAboutTheTravelers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDidTheTravelersMeetThe);
  Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

// Demonstrate Psynergy before departure, or discuss the party's journey later.
static void ValeHome_DemonstratePsynergy(void) {
  u32 i;
  s32 message;
  s32 shakes;
  s32 *origin;

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    message = MsgValeArentWorriedCrossing;
    Engine_EventSetMessage(message);
    if (Engine_GameFlagIsSet(2) != 0) {
      gEventWork->message++;
    }
    if (Engine_GameFlagIsSet(3) != 0) {
      gEventWork->message++;
    }
    Engine_EventOpenMessage(17, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(message + 3);
    } else {
      Engine_EventSetMessage(message + 4);
    }
    Engine_EventShowMessage(17, 0);
  } else {
    origin = ((struct MapScrollWork *)gMapWork[0])->origin;
    Engine_EventSetMessage(MsgValeShownNewAbility);
    Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 0);
    Engine_EventAskYesNo(17, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(17, 2);
    Engine_EventWait(15);
    // Flash the caster before starting the palette cycle and camera shake.
    SceneState_SetValue140Mode0();
    shakes = 0;
    for (i = 0; i < 40; i++) {
      ValeHome_FlashPsynergyPalette(Object_GetById(17));
      Engine_TaskWait(1);
    }
    FieldScheduler_AddCallback(ValeHome_UpdateActor17Psynergy, 0xc80);
    Engine_AudioPlayCue(107);
    for (i = 0; i != 180; i++) {
      if (Math_RemainderUnsigned(i, 10) == 0) {
        if ((1 & shakes) != 0) {
          *origin -= PIXELS(1);
        } else {
          *origin += PIXELS(1);
        }
        shakes++;
      }
      Engine_EventWait(1);
    }
    // End the display and restore the villager's ordinary appearance.
    Engine_AudioPlayCue(0x121);
    Scheduler_RemoveCallback(ValeHome_UpdateActor17Psynergy);
    Engine_TaskWait(1);
    SceneEffect_CleanupSceneObjects();
    Engine_ActorSetChildValue(17, 0);
    Engine_EventWait(40);
    Engine_EventSetMessage(MsgValeBeholdPowerPsynergy);
    Engine_EventShowMessage(17, 0);
  }
  Engine_EventEnd();
}

// Ask about the journey, the Wise One or learning Psynergy.
static void ValeHome_AskAboutJourney(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    Engine_EventSetMessage(MsgValeAnythingInterestingOnYourTrip);
    Engine_EventOpenMessage(15, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Engine_EventShowMessage(15, 0);
    } else {
      struct EventWork *work = gEventWork;

      work->message++;
      Engine_EventAskYesNo(15, 0);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_EventSetMessage(MsgValeYouSawTheWiseOne);
    Engine_EventAskYesNo(11, 0);
  } else {
    Engine_EventSetMessage(MsgValeCanIUsePsynergy);
    Engine_EventAskYesNo(11, 0);
  }
  Engine_EventEnd();
}

// Turn toward the fallen stone while describing what happened.
static void ValeHome_TalkAboutFallenStone(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(26, 1);
  Engine_ActorFaceActor(26, ACTOR_PARTY_LEADER, 20);
  Engine_ActorFaceActor(26, 21, 40);
  Engine_EventSetMessage(MsgValeTheStoneFellOnThe);
  Event_SayThenWait(26, 20);
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Engine_CameraMoveTo(PIXELS(337), -1, PIXELS(272), 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(26, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(26, ACTOR_PARTY_LEADER, 10);
  Event_SayThenWait(26, 40);
  Actor_SetBuiltinAction(26, 2);
  Engine_EventEnd();
}

// Show a villager using Psynergy to lift actor 20.
static void ValeHome_DemonstrateLevitation(void) {
  u32 i;
  s32 savedMotionFlags;
  struct FieldActor *levitatingActor;

  Engine_EventBegin();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 760);
  Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 30);
  Engine_EventSetMessage(MsgValeIHaveSomePsynergyLeft);
  Event_SayThenWait(15, 20);
  ValeHome_FaceDirectionAndWait(15, FACING_NORTHWEST, 20);
  Actor_SetAttachedEffect(15, 0x102);
  Engine_EventWait(20);
  // Charge the caster's display, then start both palette updates.
  SceneState_SetValue140Mode0();
  for (i = 0; i < 40; i++) {
    ValeHome_FlashPsynergyPalette(Object_GetById(15));
    Engine_TaskWait(1);
  }
  FieldScheduler_AddOrUpdateCallback(ValeHome_UpdateActor15Psynergy, 0xc80);
  FieldScheduler_AddCallback(ValeHome_UpdateActor20Psynergy, 0xc80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  // Suspend ordinary movement while lifting the target a little each frame.
  levitatingActor = Object_GetById(20);
  savedMotionFlags = levitatingActor->motionFlags;
  levitatingActor->motionFlags = 0;
  for (i = 0; i < 40; i++) {
    levitatingActor->y.fixed += PIXELS(3) / 32;
    Engine_TaskWait(1);
  }
  levitatingActor->motionFlags = savedMotionFlags;
  Scheduler_RemoveCallback(ValeHome_UpdateActor15Psynergy);
  Scheduler_RemoveCallback(ValeHome_UpdateActor20Psynergy);
  Engine_TaskWait(1);
  Event_PlayCue(161);
  Actor_SetChildValue(15, 0);
  Actor_SetChildValue(20, 0);
  Engine_EventWait(40);
  SceneEffect_CleanupSceneObjects();
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
  Event_ShowMessage(15, 0);
  Engine_EventEnd();
}

// Discuss whom the party should follow and help after the storm.
static void ValeHome_TalkAboutHelping(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 10);
  if (Engine_GameFlagIsSet(0x840) != 0) {
    Engine_EventSetMessage(MsgValeSureHelp);
    Engine_EventShowMessage(16, 0);
  } else {
    Engine_EventSetMessage(MsgValeWentOffWay);
    Engine_EventShowMessage(16, 0);
  }
  Engine_EventEnd();
}

// Welcome the party home, then use the repeat greeting on later visits.
static void ValeHome_WelcomePartyHome(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x302) != 0) {
    Engine_EventSetMessage(MsgValeThisIsVale);
  } else {
    Engine_EventSetMessage(MsgValeYouCameBackHome);
    GameFlag_Set(0x302);
  }
  Event_ShowMessage(11, 0);
  Engine_EventEnd();
}

// Pause the villager's search to discuss the missing Psynergy Stone.
static void Scene_PsynergyStoneIsGone(void) {
  struct FieldActor *actor = Object_GetById(21);
  Engine_EventBegin();
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  Engine_ActorSetAnimation(21, 1);
  Engine_ActorStop(21);
  Engine_ActorShowEmote(21, 256, 40);
  actor->facing = FACING_NORTHWEST + FACING_STEP;
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(21, 2);
  Engine_EventSetMessage(MsgValeThePsynergyStoneIsGone);
  Engine_EventShowMessageAndWait(21, 0, 40);
  Engine_ActorFaceActor(21, ACTOR_PARTY_LEADER, 20);
  Engine_ActorStartRepeatedMotion(21, 2);
  Engine_EventShowMessage(21, 0);
  Engine_GameFlagSet(0x306);
  Engine_ActorStop(21);
  Engine_TaskWait(1);
  Actor_SetActionScript(21, gValeHome_Actor21Actions);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vale.h"

// Open a door's map cells and send the leader through its threshold.
static inline void ValeHome_OpenDoorAndWalkThrough(s32 cellX, s32 cellZ, s32 x,
                                                   s32 z) {
  Event_PlayCue(158);
  Map_AnimateCells(gValeHome_CellSteps, cellX, cellZ);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, z);
}

// Enter through the village's northwest door.
static void ValeHome_OpenNorthwestDoor(void) {
  ValeHome_OpenDoorAndWalkThrough(44, 7, 248, 279);
  Vale_RequestBackdropExit(1);
}

// Open the northeast doorway in two stages and walk inside.
static void ValeHome_OpenNortheastDoor(void) {
  Event_PlayCue(188);
  Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
  Engine_TaskWait(10);
  Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
  Engine_TaskWait(10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
  Vale_RequestBackdropExit(2);
}

// Enter through the western door in the village's middle row.
static void ValeHome_OpenCentralWestDoor(void) {
  ValeHome_OpenDoorAndWalkThrough(43, 15, 230, 407);
  Vale_RequestBackdropExit(3);
}

// Enter through the eastern door in the village's middle row.
static void ValeHome_OpenCentralEastDoor(void) {
  ValeHome_OpenDoorAndWalkThrough(52, 18, 374, 419);
  Vale_RequestBackdropExit(4);
}

// Enter through the southwestern door.
static void ValeHome_OpenSouthwestDoor(void) {
  ValeHome_OpenDoorAndWalkThrough(41, 32, 200, 546);
  Vale_RequestBackdropExit(5);
}

// Enter through the door at the village's western edge.
static void ValeHome_OpenWestEdgeDoor(void) {
  ValeHome_OpenDoorAndWalkThrough(35, 36, 102, 611);
  Vale_RequestBackdropExit(6);
}

// Enter through the southeastern door.
static void ValeHome_OpenSoutheastDoor(void) {
  ValeHome_OpenDoorAndWalkThrough(51, 39, 358, 670);
  Vale_RequestBackdropExit(7);
}

// Leave through the village's western passage.
static void ValeHome_LeaveThroughWestPassage(void) {
  Engine_AudioPlayCue(123);
  Vale_RequestBackdropExit(8);
}

// Leave by the south passage once the party has begun its journey.
static void ValeHome_LeaveThroughSouthPassage(void) {
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_AudioPlayCue(123);
    Vale_RequestBackdropExit(10);
  }
}

// Remember that Whirlwind cleared actor 8.
static void ValeHome_ClearFirstWhirlwindTarget(void) {
  Engine_GameFlagSet(0x90b);
}

// Remember that Whirlwind cleared actor 9.
static void ValeHome_ClearSecondWhirlwindTarget(void) {
  Engine_GameFlagSet(0x90c);
}

// Remember that Whirlwind cleared actor 10.
static void ValeHome_ClearThirdWhirlwindTarget(void) {
  Engine_GameFlagSet(0x90d);
}

// Restore cleared targets and stage the event selected by this entrance.
s32 ValeHome_RestoreEntryState(void) {
  struct FieldActor *actor;
  s32 flag;
  s32 verticalScale;

  if (Engine_GameFlagIsSet(0x90b) != 0) {
    Engine_ActorSetPosition(8, 0, 0);
  }
  if (Engine_GameFlagIsSet(0x90c) != 0) {
    Engine_ActorSetPosition(9, 0, 0);
  }
  if (Engine_GameFlagIsSet(0x90d) != 0) {
    Engine_ActorSetPosition(10, 0, 0);
  }
  switch (gGameState.entrance) {
  case 98:
    Engine_GameFlagSet(32);
    Engine_EventRequestExit(50);
    return 0;
  case 99:
    ValeHome_SetProgressFlagsAndExit();
    return 0;
  case 97:
    ValeHome_TravelersLeaveAfterBattle();
    return 0;
  }
  verticalScale = 3 * Q16_ONE / 2;
  actor = Object_GetById(8);
  actor->scaleY = verticalScale;
  actor = Object_GetById(9);
  actor->scaleY = verticalScale;
  actor = Object_GetById(10);
  actor->scaleY = verticalScale;
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    Map_CopyCellsTo(97, 2, 80, 5, 2, 2);
    Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  } else {
    if (Engine_GameFlagIsSet(0x834) != 0) {
      FieldFx_StartGroundParticles();
      FieldFx_StartLightning();
      Map_CopyCellAttributes(21, 38, 1, 1, 18, 41);
      flag = Engine_GameFlagIsSet(0x840);
      if (flag != 0) {
        Engine_ActorSetPosition(17, 0, 0);
        Engine_ActorSetPosition(18, 0, 0);
        FieldObject_SetTargetAndCallback(
            19, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gValeHome_FaceTargetActions);
      }
    } else {
      if (Engine_GameFlagIsSet(0x815) != 0) {
        Actor_SetPosition(16, PIXELS(180), PIXELS(568));
        Map_CopyCellsTo(92, 2, 80, 5, 2, 2);
        Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
      }
    }
    if (gGameState.entrance == 12) {
      ValeHome_DepartForQuest();
    } else {
      if (Engine_GameFlagIsSet(0x834) != 0) {
        actor = Object_GetById(20);
        actor->scaleX = 3 * Q16_ONE / 10;
        actor->scaleY = 3 * Q16_ONE / 10;
        actor = Object_GetById(20);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor = Object_GetById(21);
        actor->scaleX = 3 * Q16_ONE / 5;
        actor->scaleY = 3 * Q16_ONE / 5;
        Engine_ActorSetAnimation(13, 5);
      } else {
        if (Engine_GameFlagIsSet(0x815) != 0) {
          Actor_SetPosition(21, PIXELS(331), PIXELS(249));
          actor = Object_GetById(21);
          Engine_ActorSetSpriteFlags(actor, 0);
        }
      }
      if (Engine_GameFlagIsSet(0x840) != 0) {
        Engine_ActorSetPosition(26, 0, 0);
        Engine_ActorSetPosition(22, 0, 0);
      }
      if (gGameState.entrance == 19) {
        ValeHome_TravelersLeaveAfterBattle();
      } else {
        if (Engine_GameFlagIsSet(0x834) != 0 &&
            (flag = Engine_GameFlagIsSet(0x842)) != 0) {
          ValeHome_MeetSurvivingTravelers();
        } else {
          if (Engine_GameFlagIsSet(0x834) != 0) {
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            FieldFx_ResumeLightning();
          }
        }
      }
    }
  }
  return 0;
}

// Walk a separate scene actor back to the leader, then hide it.
static inline void ValeHome_RejoinParty(s32 actorId) {
  struct FieldActor *leader;

  Engine_ActorSetAnimation(actorId, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(actorId, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(actorId);
  Engine_ActorSetPosition(actorId, 0, 0);
}

// Send the villagers to aid the elders and gather Isaac's companions again.
static void ValeHome_SendHelpToElders(void) {
  struct FieldActor *leader;
  s32 skipReply = 0;
  s32 speechFlags;

  if (Engine_GameFlagIsSet(0x834) != 0 && Engine_GameFlagIsSet(0x840) == 0) {
    // Gather the leader and companion while the elders organize the helpers.
    Engine_EventBegin();
    Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    Engine_CameraMoveTo(PIXELS(197), -1, PIXELS(768), 1);
    Camera_WaitForArrival();
    Engine_EventSetMessage(MsgValeGoAidElders);
    Engine_ActorRunRepeatedMotion(19, 2);
    Event_ShowMessageAndWait(0x4013, 0, 10);
    Actor_SetSpeed(0, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(25, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(0, 179, 789);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Engine_ActorSetPosition(25, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkToAndWait(25, 179, 804);
    Engine_ActorFaceEachOther(0, 25, 40);
    Engine_ActorFaceDirection(0, 0, 0);
    Engine_ActorFaceDirection(25, 0, 0);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_ActorFaceEachOther(17, 18, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(17, 1);
    Engine_EventShowMessageAndWait(0x4011, 0, 10);
    Engine_ActorSetAnimation(18, 3);
    Engine_EventShowMessageAndWait(18, 0, 10);
    Engine_ActorFaceDirection(17, 0, 0);
    Actor_FaceDirection(18, FACING_NORTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(0x4013, 0, 10);
#if TBS_PATCH_4
    Engine_ActorSetSpritePriority(18, 1);
#endif
    Actor_SetSpeed(17, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetSpeed(18, PIXELS(8) / 5, PIXELS(4) / 5);
    // Send the helpers away before continuing the conversation.
    Actor_SetActionScript(17, gValeHome_Actor17Actions);
    Engine_EventWait(20);
#if TBS_PATCH_4
    Object_GetById(18)->priorityFlags |= 1;
#endif
    Actor_SetActionScript(18, gValeHome_Actor17Actions);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(25, FACING_NORTH, 60);
    Actor_SetActionScript(0, gValeHome_LeaderStepActions);
    Actor_RunActionScriptAndWait(25, gValeHome_Actor25StepActions);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(0, 0, 0);
    Engine_ActorFaceDirection(25, 0, 10);
    Engine_EventShowMessage(25, 0);
    Actor_FaceDirection(19, FACING_WEST, 0);
    ValeHome_FaceDirectionAndWait(26, FACING_SOUTHWEST, 20);
    Engine_ActorRunRepeatedMotion(26, 2);
    Event_SayThenWait(26, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(25, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventOpenMessage(0x4013, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      skipReply = 1;
      Engine_ActorSetAnimation(19, 4);
    } else {
      Engine_ActorSetAnimation(19, 3);
      gEventWork->message++;
    }
    Event_ShowMessage(0x4013, 0);
    if (skipReply != 0) {
      gEventWork->message++;
    }
    speechFlags = 0x4000;
    ValeHome_FaceDirectionAndWait(22, FACING_SOUTH, 30);
    Engine_EventShowMessage(22, 0);
    Actor_ShowEmote(19, 0x100, 0);
    Actor_ShowEmote(26, 0x100, 0);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(25, 0x100, 40);
    Actor_FaceDirection(19, FACING_NORTHWEST, 0);
    Actor_FaceDirection(26, FACING_NORTHWEST, 0);
    Actor_FaceDirection(0, FACING_NORTHEAST, 0);
    ValeHome_FaceDirectionAndWait(25, FACING_NORTHEAST, 10);
    Engine_CameraSetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
    Camera_MoveTo(PIXELS(215), -1, PIXELS(758), 1);
    Camera_WaitForArrival();
    Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    Camera_MoveTo(PIXELS(205), -1, PIXELS(778), 1);
    Actor_SetActionScript(22, gValeHome_Actor22Actions);
    Object_RefreshSelectorById(22);
    VALE_HOME_TURN_AND_WAIT(22, FACING_SOUTHEAST, 60);
    Engine_ActorRunRepeatedMotion(19, 2);
    Event_SayThenWait(19, 10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Event_SayThenWait(22, 20);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(10);
    ValeHome_FaceDirectionAndWait(19, FACING_SOUTH, 30);
    Event_SayThenWait(speechFlags + 19, 10);
    ValeHome_FaceDirectionAndWait(26, FACING_NORTHEAST, 30);
    Engine_ActorSetAnimationAndWait(26, 3);
    ValeHome_FaceDirectionAndWait(19, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(19, 2);
    Event_SayThenWait(speechFlags + 19, 10);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 25, 40);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    ValeHome_FaceDirectionAndWait(25, 0, 20);
    ValeHome_FaceDirectionAndWait(26, FACING_WEST, 30);
    Engine_ActorSetAnimationAndWait(26, 3);
    Event_SayThenWait(26, 30);
    ValeHome_FaceDirectionAndWait(26, FACING_NORTH, 30);
    Engine_ActorSetAnimationAndWait(26, 3);
    Engine_ActorSetAnimationAndWait(22, 3);
    // Return the separate companions to the leader and finish this story step.
    ValeHome_RejoinParty(25);
    ValeHome_RejoinParty(26);
    ValeHome_RejoinParty(22);
    FieldObject_SetTargetAndCallback(
        19, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
        gValeHome_FaceTargetActions);
    Engine_GameFlagSet(0x840);
    Engine_EventEnd();
  }
}

// Meet the two surviving travelers during the storm.
static void ValeHome_MeetSurvivingTravelers(void) {
  struct FieldActor *camera;

  camera = Camera_GetEventObject();
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  // Establish the storm scene before the travelers notice the party.
  Engine_CameraMoveTo(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Event_PrepareObjectAndApplyValue(1, 0);
  Engine_EventOpenScreen();
  Engine_AudioPlayCue(17);
  FieldFx_ResumeLightning();
  Engine_ActorSetPosition(23, PIXELS(105), PIXELS(267));
  Engine_TaskWait(1);
  Actor_SetSpeed(0, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkToAndWait(0, 93, 343);
  Engine_EventSetMessage(MsgValeOnlyTwoSurvived);
  Engine_EventShowMessage(23, 0);
  Engine_AudioPlayCue(61);
  camera->motionFlags = 0;
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Engine_CameraMoveTo(PIXELS(109), PIXELS(176), PIXELS(281), 1);
  Camera_WaitForArrival();
  Engine_EventWait(40);
  Actor_SetPosition(24, PIXELS(135), PIXELS(177));
  Actor_SetSpeed(24, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(24, 126, 258);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(23, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(24);
  Engine_ActorSetAnimation(24, 1);
  ValeHome_FaceDirectionAndWait(24, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(23, 3);
  Engine_ActorSetAnimationAndWait(24, 4);
  Event_ShowMessage(0x2018, 0);
  Engine_ActorRunRepeatedMotion(23, 2);
  Event_SayThenWait(0x8017, 30);
  ValeHome_FaceDirectionAndWait(24, FACING_NORTHWEST + FACING_STEP, 20);
  Event_SayThenWait(0x2018, 10);
  ValeHome_FaceDirectionAndWait(23, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_EventShowMessage(0x8017, 0);
  Engine_ActorSetAnimationAndWait(24, 4);
  Engine_EventShowMessage(0x2018, 0);
  ValeHome_FaceDirectionAndWait(23, FACING_NORTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(23, 2);
  Engine_EventShowMessage(0x8017, 0);
  ValeHome_FaceDirectionAndWait(24, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimationAndWait(24, 3);
  Engine_EventWait(20);
  Event_SayThenWait(0x2018, 20);
  ValeHome_FightTravelers();
  Engine_EventEnd();
}

// Bring the companion into the confrontation and start the travelers' battle.
static void ValeHome_FightTravelers(void) {
  struct FieldActor *actor;
  s32 southSouthwestFacing;
  s32 westFacing;
  s32 northNortheastFacing;

  // Keep the companion offscreen until the travelers have approached Isaac.
  Engine_ActorSetChildValue(25, 15);
  actor = Object_GetById(25);
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_SetPosition(25, 0, PIXELS(331));
  Engine_TaskWait(1);
  Event_ShowMessage(0x1019, 0);
  Actor_ShowEmote(23, 0x100, 0);
  Actor_ShowEmote(24, 0x100, 40);
  Engine_ActorSetPosition(25, 0, 0);
  Actor_FaceDirection(23, FACING_SOUTH + FACING_STEP, 0);
  southSouthwestFacing = FACING_SOUTH + FACING_STEP;
  ValeHome_FaceDirectionAndWait(24, southSouthwestFacing, 40);
  Engine_CameraSetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  Engine_CameraMoveTo(PIXELS(89), PIXELS(176), PIXELS(313), 1);
  Camera_WaitForArrival();
  Engine_EventWait(40);
  Actor_FaceDirection(23, FACING_NORTHEAST, 0);
  ValeHome_FaceDirectionAndWait(24, FACING_SOUTHWEST + FACING_STEP, 40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(100), PIXELS(144), PIXELS(333), 1);
  Actor_SetSpeed(23, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(24, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(23, 105, 329);
  Engine_EventWait(10);
  Engine_ActorWalkTo(24, 124, 329);
  Engine_ActorWaitForMove(23);
  Engine_ActorSetAnimation(23, 1);
  Engine_ActorFaceDirection(23, southSouthwestFacing, 0);
  Engine_ActorWaitForMove(24);
  Engine_ActorSetAnimation(24, 1);
  Engine_ActorFaceDirection(24, southSouthwestFacing, 0);
  // Bring the companion into the conversation beside the party leader.
  Engine_ActorSetChildValue(25, 0);
  actor = Object_GetById(25);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetPosition(25, 0, PIXELS(331));
  Actor_SetSpeed(25, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkToAndWait(25, 37, 339);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(23, 3);
  Engine_EventOpenMessage(23, 0);
  Actor_ShowEmote(25, 0x101, 0);
  northNortheastFacing = FACING_NORTH + FACING_STEP;
  ValeHome_FaceDirectionAndWait(0, northNortheastFacing, 10);
  Engine_EventChooseYesNo(0, 0);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(23, 0, 0);
  westFacing = FACING_WEST;
  ValeHome_FaceDirectionAndWait(24, westFacing, 20);
  Engine_ActorRunRepeatedMotion(25, 2);
  Event_SayThenWait(0x1019, 10);
  FieldObject_SetTargetAndCallback(0, ACTOR_LINK_KEEP_MOTION | 25,
                                   gValeHome_FaceTargetActions);
  Actor_WalkToAndWait(25, 93, 361);
  ValeHome_FaceDirectionAndWait(25, northNortheastFacing, 40);
  Event_SayThenWait(25, 20);
  Engine_ActorStop(0);
  Engine_ActorFaceDirection(23, 0, 0);
  ValeHome_FaceDirectionAndWait(24, westFacing, 15);
  Engine_ActorSetAnimation(23, 3);
  Engine_ActorSetAnimationAndWait(24, 3);
  Engine_ActorFaceDirection(23, southSouthwestFacing, 0);
  ValeHome_FaceDirectionAndWait(24, southSouthwestFacing, 30);
  Engine_ActorSetAnimationAndWait(24, 4);
  Engine_EventShowMessage(0x2018, 0);
  ValeHome_FaceDirectionAndWait(0, northNortheastFacing, 30);
  ValeHome_FaceDirectionAndWait(0, FACING_SOUTH, 40);
  Engine_ActorRunRepeatedMotion(23, 2);
  Engine_ActorSetAnimationAndWait(23, 3);
  Event_SayThenWait(23, 20);
  Actor_SetAttachedEffect(0, 0x102);
  Engine_ActorSetAttachedEffect(25, 0x102);
  Engine_EventWait(80);
  Actor_SetActionScript(24, gValeHome_Actor24Actions);
  Engine_EventWait(6);
  Actor_SetActionScript(23, gValeHome_Actor23Actions);
  Engine_EventWait(20);
  Actor_SetActionScript(0, gValeHome_LeaderActions);
  Engine_EventWait(6);
  Actor_RunActionScriptAndWait(25, gValeHome_Actor25Actions);
  // The confrontation continues in battle; both outcomes return here.
  Battle_SetStart(2);
  {
    const void *message = &SceneId_ValeHome;

    Event_SetNextDestination((s32)message, 19);
    Event_SetDefeatDestination((s32)message, 19);
  }
  Field_RequestBattleFromEncounterTable(12, 4);
  Engine_GameFlagSet(0x11a);
}

// Leave Isaac and his companion behind as the travelers retreat in the storm.
static void ValeHome_TravelersLeaveAfterBattle(void) {
  struct FieldActor *actor;
  s32 gravityQ8;

  // Restore the defeated party and the travelers at their battle positions.
  Engine_AudioPlayCue(17);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_CameraMoveTo(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetPosition(0, PIXELS(48), PIXELS(346));
  Actor_SetPosition(25, PIXELS(78), PIXELS(358));
  Actor_SetPosition(23, PIXELS(103), PIXELS(342));
  Actor_SetPosition(24, PIXELS(112), PIXELS(360));
  Actor_FaceDirection(23, FACING_WEST, 0);
  Actor_FaceDirection(24, FACING_WEST, 0);
  Engine_ActorSetAnimation(0, 16);
  actor = Object_GetById(0);
  actor->scaleX = -Q16_ONE;
  actor = Object_GetById(0);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(25, 7);
  actor = Object_GetById(25);
  actor->sprite->rotation = 0x1555;
  actor = Object_GetById(25);
  Engine_ActorSetSpriteFlags(actor, 0);
  {
    struct EventWork *work = gEventWork;

    work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  }
  Engine_EventOpenScreen();
  FieldFx_ResumeLightning();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  Actor_FaceDirection(23, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_NORTH, 40);
  Engine_ActorSetAnimationAndWait(23, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(24, 3);
  Actor_FaceDirection(23, FACING_WEST, 10);
  Actor_FaceDirection(24, FACING_WEST, 10);
  Engine_ActorSetSpritePriority(0, 3);
  Engine_ActorSetSpritePriority(25, 3);
  // Give each traveler its retreating bounce before the final lightning.
  Actor_SetSpeed(23, PIXELS(12) / 5, PIXELS(6) / 5);
  gravityQ8 = 128;
  actor = Object_GetById(23);
  actor->bounce = 0x28f;
  actor->gravity = gravityQ8 << 8;
  Actor_SetActionScript(23, gValeHome_Actor23BounceActions);
  Engine_EventWait(24);
  Engine_ActorSetSpeed(24, PIXELS(12) / 5, PIXELS(6) / 5);
  actor = Object_GetById(24);
  actor->bounce = 0x28f;
  actor->gravity = gravityQ8 << 8;
  Actor_RunActionScriptAndWait(24, gValeHome_Actor24BounceActions);
  Engine_EventWait(40);
  FieldFx_StopLightning();
  FieldFx_LastLightning();
  Engine_TaskWait(20);
  FieldFx_LastLightning();
  Engine_TaskWait(60);
  FieldFx_LastLightning();
  Engine_TaskWait(20);
  FieldFx_StopLightning();
  Engine_EventWait(40);
  gEventWork->transitionFrames = 120;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_GameFlagClear(0x834);
  Engine_EventRequestExit(9);
  Engine_EventEnd();
}

// The villagers give their supplies and farewells before the party leaves Vale.
static void ValeHome_DepartForQuest(void) {
  struct FieldActor *actor;
  struct EventWork *work;
  s32 priorityFlags;
  const s32 *faceActions;
  const s32 *actor31Actions;
  const s32 *departureActions;

  // Gather the villagers and hide actors reserved for the later exchanges.
  Party_RestoreHpPp(1);
  Engine_EventBegin();
  Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
  Camera_MoveTo(PIXELS(180), PIXELS(16), PIXELS(618), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  actor = Object_GetById(22);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(23);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(24);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(25);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(26);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(29);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetSpritePriority(0, 1);
  Engine_ActorSetSpritePriority(1, 1);
  Engine_ActorSetSpritePriority(17, 1);
  Engine_ActorSetSpritePriority(16, 1);
  Engine_ActorSetSpritePriority(15, 1);
  Actor_SetPosition(0, PIXELS(208), PIXELS(814));
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  Actor_ShowEmote(12, 0x101, 40);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 20);
  Event_SetMessage(MSGNO(MsgValeWhysEveryoneHanging));
  Event_SayThenWait(12, 10);
  Actor_ShowEmote(11, 0x102, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 10);
  Event_SayThenWait(11, 10);
  Engine_ActorSetAnimationAndWait(12, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(11, 2);
  Event_SayThenWait(11, 10);
  Actor_ShowEmote(12, 0x100, 40);
  Actor_SetSpeed(12, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(12, 184, 618);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 60);
  Event_SayThenWait(12, 20);
  Actor_SetSpeed(11, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(11, 168, 618);
  ValeHome_TurnAndWait(11, FACING_NORTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(11, 4);
  Event_SayThenWait(11, 20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventShowMessage(12, 0);
  Engine_EventWait(10);
  // The two nearby actors run through while villagers turn to watch them.
  Actor_SetSpeed(30, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetPosition(30, PIXELS(110), PIXELS(744));
  Engine_TaskWait(2);
  Engine_ActorSetAnimation(30, 3);
  Actor_SetActionScript(30, gValeHome_Actor30Actions);
  Engine_EventWait(40);
  faceActions = gValeHome_FaceTargetActions;
  FieldObject_SetTargetAndCallback(11, ACTOR_LINK_KEEP_MOTION | 30,
                                   faceActions);
  FieldObject_SetTargetAndCallback(12, ACTOR_LINK_KEEP_MOTION | 30,
                                   faceActions);
  Object_RefreshSelectorById(30);
  Engine_ActorStop(11);
  Engine_ActorStop(12);
  Engine_EventWait(60);
  Actor_ShowEmote(11, 0x105, 0);
  Actor_ShowEmote(12, 0x105, 120);
  Engine_ActorFaceDirection(11, FACING_EAST + FACING_STEP, 0);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 80);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTH + FACING_STEP, 60);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTH + FACING_STEP, 60);
  Actor_ShowEmote(12, 0x101, 80);
  Engine_ActorFaceDirection(11, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(12, 184, 630);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTH + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(12, 0x101, 40);
  Actor_ShowEmote(11, 0x101, 40);
  Actor_WalkToAndWait(11, 168, 630);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ShowEmote(11, 0x101, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 10);
  Event_SayThenWait(11, 20);
  Engine_ActorSetAnimationAndWait(12, 3);
  Event_SayThenWait(12, 10);
  Actor_ShowEmote(11, 0x100, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 60);
  Engine_ActorSetAnimationAndWait(11, 3);
  Event_SayThenWait(11, 10);
  {
    struct FieldActor *actor = Object_GetById(30);

    if (actor != NULL) {
      Engine_ActorSetPosition(31, actor->x.fixed, actor->z.fixed);
    }
  }
  priorityFlags = 0xfe;
  Engine_TaskWait(2);
  Object_GetById(30)->priorityFlags &= priorityFlags;
  Object_GetById(31)->priorityFlags &= priorityFlags;
  Engine_ActorSetSpritePriority(30, 2);
  Engine_ActorSetSpritePriority(31, 2);
  Actor_SetSpeed(31, PIXELS(18) / 5, PIXELS(9) / 5);
  Engine_ActorSetAnimation(31, 2);
  actor31Actions = gValeHome_Actor31Actions;
  Actor_SetActionScript(31, actor31Actions);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(30, 3);
  Actor_SetSpeed(30, PIXELS(24) / 5, PIXELS(12) / 5);
  Actor_RunActionScriptAndWait(30, actor31Actions);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(12, 2);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 10);
  Event_SayThenWait(12, 10);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(11, 3);
  Event_SayThenWait(11, 20);
  Engine_ActorSetAnimation(12, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(11, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetSpeed(12, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetActionScript(11, gValeHome_Actor11Actions);
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_MoveTo(PIXELS(215), PIXELS(16), PIXELS(801), 1);
  Engine_EventWait(10);
  Actor_SetActionScript(12, gValeHome_Actor12Actions);
  Object_RefreshSelectorById(12);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 120);
  // Return attention to the departing party and the villagers' gifts.
  Engine_ActorRunRepeatedMotion(13, 2);
  Engine_EventWait(20);
  Event_SayThenWait(13, 20);
  Engine_ActorFaceDirection(0, 0, 0);
  ValeHome_FaceDirectionAndWait(1, FACING_WEST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(0, FACING_NORTH, 10);
  ValeHome_FaceDirectionAndWait(1, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimation(0, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(1, 3);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(16, 2);
  Engine_EventWait(20);
  Event_SayThenWait(16, 10);
  Actor_SetSpeed(16, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(16, 216, 800);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Engine_PartyGiveItem(180, 0);
  Actor_WalkToAndWait(16, 264, 800);
  Actor_FaceDirection(16, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(1, 0x102, 40);
  ValeHome_TurnAndWait(1, FACING_NORTHEAST + FACING_STEP, 10);
  Event_SayThenWait(1, 10);
  Engine_ActorSetAnimationAndWait(17, 4);
  Event_SayThenWait(17, 10);
  Engine_ActorFaceDirection(1, FACING_EAST + FACING_STEP, 0);
  Actor_ShowEmote(1, 0x103, 20);
  ObjectMotion_Launch(1, 4, 60);
  Engine_ActorRunRepeatedMotion(14, 2);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(14, FACING_NORTH + FACING_STEP, 10);
  Event_SayThenWait(14, 60);
  Actor_ShowEmote(1, 0x102, 0);
  Actor_ShowEmote(16, 0x102, 0);
  Actor_ShowEmote(17, 0x102, 0);
  Actor_ShowEmote(18, 0x102, 0);
  Actor_ShowEmote(19, 0x102, 80);
  Actor_ShowEmote(17, 0x100, 0);
  Event_SayThenWait(17, 60);
  // Have both party members follow the villager presenting the next gift.
  Object_LinkObjectAndSetCallback(0, 17);
  Object_LinkObjectAndSetCallback(1, 17);
  Actor_WalkToAndWait(17, 216, 800);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Event_SayThenWait(17, 60);
  Engine_PartyGiveItem(207, 0);
  Engine_ActorStop(0);
  Engine_ActorStop(1);
  Actor_WalkToAndWait(17, 272, 816);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Engine_ActorRunRepeatedMotion(1, 2);
  ValeHome_FaceDirectionAndWait(1, FACING_WEST + FACING_STEP, 10);
  Event_SayThenWait(1, 10);
  Engine_ActorFaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  ValeHome_FaceDirectionAndWait(0, 0, 10);
  Actor_ShowEmote(0, 0x101, 60);
  Engine_ActorRunRepeatedMotion(16, 1);
  Event_SayThenWait(16, 10);
  ValeHome_TurnAndWait(1, FACING_NORTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(1, 0x101, 20);
  Engine_ActorSetAnimationAndWait(16, 4);
  Event_SayThenWait(16, 10);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventShowMessage(18, 0);
  Engine_ActorFaceDirection(1, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorSetAnimation(18, 4);
  Engine_EventShowMessage(18, 0);
  Engine_ActorStartRepeatedMotion(18, 3);
  Engine_EventShowMessage(18, 0);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_ActorSetAnimation(19, 3);
  Engine_ActorSetAnimation(17, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(24, 3);
  Engine_ActorSetAnimation(18, 3);
  Engine_ActorSetAnimation(27, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(28, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(25, 3);
  Engine_ActorSetAnimation(20, 3);
  Engine_ActorSetAnimationAndWait(21, 3);
  ObjectMotion_Launch(15, 2, 10);
  ObjectMotion_Launch(15, 4, 40);
  Event_SayThenWait(15, 10);
  Engine_ActorFaceDirection(1, FACING_NORTHWEST + FACING_STEP, 0);
  ValeHome_FaceDirectionAndWait(0, FACING_NORTH, 20);
  ValeHome_FaceDirectionAndWait(15, FACING_NORTH + FACING_STEP, 10);
  Event_SayThenWait(15, 10);
  ValeHome_FaceDirectionAndWait(15, FACING_WEST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP, 10);
  // Stagger the villagers' farewell gestures across the assembled groups.
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(14, 3);
  Engine_ActorSetAnimation(17, 3);
  Engine_ActorSetAnimation(20, 3);
  Engine_ActorSetAnimation(23, 3);
  Engine_ActorSetAnimation(26, 3);
  Engine_ActorSetAnimation(29, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(15, 3);
  Engine_ActorSetAnimation(18, 3);
  Engine_ActorSetAnimation(21, 3);
  Engine_ActorSetAnimation(24, 3);
  Engine_ActorSetAnimation(27, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(13, 3);
  Engine_ActorSetAnimation(16, 3);
  Engine_ActorSetAnimation(19, 3);
  Engine_ActorSetAnimation(22, 3);
  Engine_ActorSetAnimation(25, 3);
  Engine_ActorSetAnimationAndWait(28, 3);
  Engine_EventWait(80);
  ObjectMotion_Launch(11, 4, 0);
  ObjectMotion_Launch(14, 4, 0);
  ObjectMotion_Launch(17, 4, 0);
  ObjectMotion_Launch(20, 4, 0);
  ObjectMotion_Launch(23, 4, 0);
  ObjectMotion_Launch(26, 4, 0);
  ObjectMotion_Launch(29, 4, 0);
  ObjectMotion_Launch(12, 4, 0);
  ObjectMotion_Launch(15, 4, 0);
  ObjectMotion_Launch(18, 4, 0);
  ObjectMotion_Launch(21, 4, 0);
  ObjectMotion_Launch(24, 4, 0);
  ObjectMotion_Launch(27, 4, 0);
  ObjectMotion_Launch(13, 4, 0);
  ObjectMotion_Launch(16, 4, 0);
  ObjectMotion_Launch(19, 4, 0);
  ObjectMotion_Launch(22, 4, 0);
  ObjectMotion_Launch(25, 4, 0);
  ObjectMotion_Launch(28, 4, 0);
  Engine_MessageShowCentered(MSGNO(MsgValeFarewell), 1);
  priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  Engine_EventWait(80);
  Object_GetById(0)->priorityFlags |= priorityFlags;
  {
    struct FieldActor *actor = Object_GetById(1);
    u8 value = (priorityFlags | actor->priorityFlags);

    actor->priorityFlags = value;
  }
  Actor_ShowEmote(0, 0x102, 0);
  Actor_ShowEmote(1, 0x102, 80);
  ValeHome_FaceDirectionAndWait(0, FACING_SOUTH, 10);
  ValeHome_FaceDirectionAndWait(1, FACING_SOUTH + FACING_STEP, 20);
  // Let the party walk out while the camera follows and the village fades.
  Actor_SetSpeed(0, PIXELS(4) / 5, PIXELS(2) / 5);
  departureActions = gValeHome_Actor0And1Actions;
  Actor_SetActionScript(0, departureActions);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Engine_CameraMoveTo(PIXELS(216), PIXELS(16), PIXELS(905), 1);
  Engine_EventWait(20);
  Actor_SetSpeed(1, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetActionScript(1, departureActions);
  Engine_EventWait(60);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_ActorStop(0);
  Engine_ActorStop(1);
  Engine_EventRequestExit(10);
  Engine_EventEnd();
}

// Show the current message, then leave a pause before the next scene action.
static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

// Turn an actor immediately, then pause the scene.
static void ValeHome_FaceDirectionAndWait(s32 actorId, s32 facing, s32 frames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(frames);
}

// Flash the caster's palette and emit paired effects every sixteen frames.
static void ValeHome_FlashPsynergyPalette(struct FieldActor *actor) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    ValeHome_SpawnEffectPair(actor);
  }
}

// Cycle six palette banks and emit the caster's paired effects.
static void
ValeHome_CyclePsynergyPaletteAndSpawnEffects(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    Engine_ObjectSetPartPalettes(actor, palette);
  }
  if ((gFrameCount & 15) == 0) {
    ValeHome_SpawnEffectPair(actor);
  }
}

// Cycle the levitated actor through six palette banks.
static void ValeHome_CyclePsynergyPalette(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    Engine_ObjectSetPartPalettes(actor, palette);
  }
}

// Shared source is included here in ROM order.
#include "common/effect_vertical.h"

// Create the two vertical Psynergy effects using the shared sprite tiles.
static void ValeHome_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct PairWork *work = gEffectWork;
  s32 i;

  Engine_AudioPlayCue(131);
  for (i = 0; i < 2; ++i) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        AnimationObjects_SelectAnimation(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->vramBlock;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct FieldActor *effect = pair[0];
    struct AnimationObject *effectSprite = effect->sprite;

    effect->update = Effect_AnimateVerticalNegative;
    effectSprite->image.priority = 2;
  }
  {
    struct FieldActor *effect = pair[1];
    struct AnimationObject *effectSprite = effect->sprite;

    effectSprite->image.priority = 2;
    effect->update = Effect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

// Shared source is included here in ROM order.
#include "common/effect_psynergy.h"

// Update the levitating villager's Psynergy display.
static void ValeHome_UpdateActor15Psynergy(void) {
  ValeHome_CyclePsynergyPaletteAndSpawnEffects(Object_GetById(15));
}

// Update the demonstrating villager's Psynergy display.
static void ValeHome_UpdateActor17Psynergy(void) {
  ValeHome_CyclePsynergyPaletteAndSpawnEffects(Object_GetById(17));
}

// Update the palette of the levitated actor.
static void ValeHome_UpdateActor20Psynergy(void) {
  ValeHome_CyclePsynergyPalette(Object_GetById(20));
}

// Set this edition's progress preset and leave through exit 40.
static void ValeHome_SetProgressFlagsAndExit(void) {
  Engine_GameFlagSet(352);
  Engine_GameFlagSet(0x16d);
#if TBS_PATCH_3
  Engine_GameFlagSet(0x830);
  Engine_GameFlagSet(0x831);
  Engine_GameFlagSet(0x832);
  Engine_GameFlagSet(0x833);
  Engine_GameFlagSet(0x835);
  Engine_GameFlagSet(0x836);
  Engine_GameFlagSet(0x837);
  Engine_GameFlagSet(0x838);
  Engine_GameFlagSet(0x839);
  Engine_GameFlagSet(0x83a);
  Engine_GameFlagSet(0x840);
  Engine_GameFlagSet(0x841);
  Engine_GameFlagSet(0x842);
  Engine_GameFlagSet(0x806);
  Engine_GameFlagSet(0x807);
  Engine_GameFlagSet(0x808);
  Engine_GameFlagSet(0x800);
  Engine_GameFlagSet(0x801);
  Engine_GameFlagSet(0x823);
  Engine_GameFlagSet(0x802);
  Engine_GameFlagSet(0xf01);
  Engine_GameFlagSet(0x81a);
  Engine_GameFlagSet(0x804);
  Engine_GameFlagSet(0xf02);
  Engine_GameFlagSet(0x821);
  Engine_GameFlagSet(0x825);
  Engine_GameFlagSet(0x809);
  Engine_GameFlagSet(0x80a);
  Engine_GameFlagSet(0x818);
  Engine_GameFlagSet(0x80b);
  Engine_GameFlagSet(0x80c);
  Engine_GameFlagSet(0x80d);
  Engine_GameFlagSet(0x80e);
  Engine_GameFlagSet(0x80f);
  Engine_GameFlagSet(0x813);
  Engine_GameFlagSet(0x810);
  Engine_GameFlagSet(0x811);
  Engine_GameFlagSet(0x819);
  Engine_GameFlagSet(0x83b);
  Engine_GameFlagSet(0x83c);
  Engine_GameFlagSet(0x83d);
  Engine_GameFlagSet(0x83e);
  Engine_GameFlagSet(0x83f);
  Engine_GameFlagSet(0x814);
  Engine_GameFlagSet(0x879);
  Engine_GameFlagSet(0x815);
  Engine_GameFlagSet(0x81b);
  Engine_GameFlagSet(0x81d);
  Engine_GameFlagSet(0x87a);
#else
  Engine_GameFlagSet(0x844);
  Engine_GameFlagSet(0x845);
#endif
  Engine_EventRequestExit(40);
}

// clang-format off
u32 gValeHome_Actor22ActionsHead[] = {
    0x1000000, 0x2020101, 0x3030302, 0x5040404, 0x6060505, 0x1000006, 0x3020201, 0x5040403,
};

s32 gValeHome_Actor22Actions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(214), 0, PIXELS(721)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(40),
    SC_SE(153),
    SC_SET(SW_MOTION, 3),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_ARG(2),
    SC_MOVE(PIXELS(214), 0, PIXELS(763)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeHome_Actor24Actions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(12)),
    SC_MOVE(PIXELS(114), PIXELS(144), PIXELS(360)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeHome_Actor23Actions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(12)),
    SC_MOVE(PIXELS(114), PIXELS(144), PIXELS(343)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeHome_LeaderActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(82), PIXELS(144), PIXELS(343)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeHome_Actor25Actions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(82), PIXELS(144), PIXELS(360)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeHome_Actor23BounceActions[] = {
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_MOVE(PIXELS(65), 0, PIXELS(328)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_MOVE(PIXELS(31), 0, PIXELS(338)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(0, 0, PIXELS(338)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeHome_Actor24BounceActions[] = {
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_MOVE(PIXELS(63), 0, PIXELS(374)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SET(SW_VELOCITY_Y, PIXELS(7)),
    SC_MOVE(PIXELS(17), 0, PIXELS(338)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(0, 0, PIXELS(338)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeHome_FaceTargetActions[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeHome_Actor30Actions[] = {
    SC_MOVE(PIXELS(132), 0, PIXELS(636)),
    SC_SYNC,
    SC_MOVE(PIXELS(171), 0, PIXELS(629)),
    SC_SYNC,
    SC_MOVE(PIXELS(207), 0, PIXELS(627)),
    SC_SYNC,
    SC_MOVE(PIXELS(235), 0, PIXELS(625)),
    SC_SYNC,
    SC_MOVE(PIXELS(261), 0, PIXELS(574)),
    SC_SYNC,
    SC_MOVE(PIXELS(327), 0, PIXELS(547)),
    SC_SYNC,
    SC_END,
};

s32 gValeHome_Actor31Actions[] = {
    SC_MOVE(PIXELS(271), 0, PIXELS(571)),
    SC_SYNC,
    SC_MOVE(PIXELS(187), 0, PIXELS(577)),
    SC_SYNC,
    SC_MOVE(PIXELS(154), 0, PIXELS(580)),
    SC_SYNC,
    SC_MOVE(PIXELS(127), 0, PIXELS(543)),
    SC_SYNC,
    SC_MOVE(PIXELS(139), 0, PIXELS(498)),
    SC_SYNC,
    SC_END,
};

s32 gValeHome_Actor11Actions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(137), 0, PIXELS(639)),
    SC_SYNC,
    SC_MOVE(PIXELS(122), 0, PIXELS(692)),
    SC_SYNC,
    SC_MOVE(PIXELS(141), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(6) / 5),
    SC_SET(SW_ACCEL, PIXELS(3) / 5),
    SC_MOVE(PIXELS(148), 0, PIXELS(770)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeHome_Actor12Actions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(137), 0, PIXELS(639)),
    SC_SYNC,
    SC_MOVE(PIXELS(122), 0, PIXELS(692)),
    SC_SYNC,
    SC_MOVE(PIXELS(141), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(6) / 5),
    SC_SET(SW_ACCEL, PIXELS(3) / 5),
    SC_MOVE(PIXELS(164), 0, PIXELS(758)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeHome_Actor0And1Actions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(854)),
    SC_SYNC,
    SC_MOVE(PIXELS(215), 0, PIXELS(921)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeHome_Actor21Actions[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 20),
    SC_SET(SW_COLLISION, 3),
    SC_LABEL(1),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gValeHome_Actor17Actions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(148), 0, PIXELS(758)),
    SC_SYNC,
    SC_MOVE(PIXELS(128), 0, PIXELS(709)),
    SC_SYNC,
    SC_MOVE(PIXELS(129), 0, PIXELS(647)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeHome_LeaderStepActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(774)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeHome_Actor25StepActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(790)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

struct SceneEntrance gValeHomeEntrances[] = {
    { 0, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 352, 0, 308, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 3, CONDITION_ALWAYS, 230, 0, 423, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 4, CONDITION_ALWAYS, 374, 0, 435, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 200, 0, 562, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 6, CONDITION_ALWAYS, 102, 0, 627, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 7, CONDITION_ALWAYS, 358, 0, 678, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 8, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 217, 0, 843, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 11, CONDITION_ALWAYS, 186, 0, 474, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 12, CONDITION_ALWAYS, 180, 0, 618, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 15, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 19, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gValeHomeExits[] = {
    SCENE_EXITS(5),
    SCENE_EXIT(1, 7, 8),
    SCENE_EXIT(2, 8, 3),
    SCENE_EXIT(3, 7, 3),
    SCENE_EXIT(4, 7, 7),
    SCENE_EXIT(5, 7, 4),
    SCENE_EXIT(6, 8, 1),
    SCENE_EXIT(7, 8, 4),
    SCENE_EXIT(8, 4, 5),
    SCENE_EXIT(9, 0, 1),
    SCENE_EXIT(10, 2, 1),
    SCENE_EXIT(11, 28, 5),
    SCENE_EXIT(50, 8, 20),
#if !TBS_PATCH_3
    SCENE_EXIT(40, 30, 1),
#else
    SCENE_EXIT(40, 12, 0),
#endif
    SCENE_EXITS_END,
};

u32 gValeHomeExitsFlag834[] = {
    SCENE_EXITS(5),
    SCENE_EXIT(1, 7, 8),
    SCENE_EXIT(2, 8, 3),
    SCENE_EXIT(3, 7, 3),
    SCENE_EXIT(4, 7, 7),
    SCENE_EXIT(5, 7, 4),
    SCENE_EXIT(6, 8, 1),
    SCENE_EXIT(7, 8, 4),
    SCENE_EXIT(8, 3, 5),
    SCENE_EXIT(9, 0, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gValeHomePlacements[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x67, CONDITION_ALWAYS, 3,
        PIXELS(392), 0, PIXELS(712), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(616), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(760), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(600), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(824), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(568), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x75, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHWEST, 3, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(760), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(680), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(370), 0, PIXELS(319), FACING_WEST, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x30, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHomePlacementsFlag834[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x67, CONDITION_ALWAYS, 3,
        PIXELS(249), 0, PIXELS(628), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(214), 0, PIXELS(635), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(294), 0, PIXELS(670), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, 3,
        PIXELS(136), 0, PIXELS(678), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(87), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(173), 0, PIXELS(568), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(213), 0, PIXELS(771), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
#if TBS_PATCH_4
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(198), 0, PIXELS(779), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
#else
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(204), 0, PIXELS(783), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
#endif
    { 0x75, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(250), 0, PIXELS(773), FACING_SOUTHWEST, 3, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(729), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(729), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(215), 0, PIXELS(709), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x20, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x21, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x13, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x30, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(786), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHomePlacementsEntrance12[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(225), 0, PIXELS(814), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(607), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(607), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(199), 0, PIXELS(781), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(180), 0, PIXELS(790), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x75, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(778), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(799), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x25, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(270), 0, PIXELS(814), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x26, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(787), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x36, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(781), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(742), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(286), 0, PIXELS(761), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(271), 0, PIXELS(778), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(762), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(227), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(209), 0, PIXELS(760), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(288), 0, PIXELS(795), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(161), 0, PIXELS(790), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(171), 0, PIXELS(815), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(191), 0, PIXELS(758), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x88, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHomePlacementsFlag87a[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(217), 0, PIXELS(804), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(107), 0, PIXELS(765), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(267), 0, PIXELS(744), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(681), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, 3,
        PIXELS(391), 0, PIXELS(716), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(617), FACING_NORTH + FACING_STEP, 3, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(166), 0, PIXELS(603), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(568), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(315), 0, PIXELS(823), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(287), 0, PIXELS(571), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(370), 0, PIXELS(319), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeHomeEvents[] = {
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeQuietDay) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeKradenHadVisitors) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_DiscussDailyMeditation) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_DiscussThreeYearsAgo) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeChosenPeople) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeProudOfAdeptGrandfather) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Villager_AskAboutTheTravelers) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValePracticePsynergy) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_DemonstratePsynergy) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeWalkToRecoverPsynergy) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgValeFoodStoredForWinter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_LeaveThroughSouthPassage) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { PSYNERGY_ON_ACTOR(0x4e), 8, 0x90b, EVENT_SCRIPT(ValeHome_ClearFirstWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(0x4e), 9, 0x90c, EVENT_SCRIPT(ValeHome_ClearSecondWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(0x4e), 10, 0x90d, EVENT_SCRIPT(ValeHome_ClearThirdWhirlwindTarget) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHomeEventsFlag834[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_AskAboutJourney) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeOldWomanCannotUsePsynergy) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeExhaustedFromFallingRocks) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeFelixFellInRiver) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_DemonstrateLevitation) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_TalkAboutHelping) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeRaceAgainstTime) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSoutheastDoor) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_LeaveThroughWestPassage) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_SendHelpToElders) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHomeEventsEntrance12[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHomeEventsAfterLeaving[] = {
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeTremorsSettled) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeRoadToVault) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgValeRoadToGomaRange) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgValePsynergyStonePowder) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_AskAboutJourney) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeLunpaFoundedNorthernTown) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeDoraHasBeenCrying) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeStonesScatteredByEruption) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_DemonstratePsynergy) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeReplenishPsynergyBeforeDungeon) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_TalkAboutFallenStone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeWorriedAboutAnotherEruption) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeVaultUndergroundPath) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgValeGomaWinterCold) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgValeStonesPowerAleph) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeProofOfWiseOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeLunpaThievesChanged) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeDoraMustBeLonely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeLuckyToFindPsynergyStones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAlive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeRecoverPsynergyByWalking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgValeStonesReplenishPsynergy) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_LeaveThroughSouthPassage) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { PSYNERGY_ON_ACTOR(0x4e), 8, 0x90b, EVENT_SCRIPT(ValeHome_ClearFirstWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(0x4e), 9, 0x90c, EVENT_SCRIPT(ValeHome_ClearSecondWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(0x4e), 10, 0x90d, EVENT_SCRIPT(ValeHome_ClearThirdWhirlwindTarget) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHomeEventsFlag87a[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_WelcomePartyHome) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeCameBackSooner) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeMadeFriendsOnJourney) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeRiverHeatedAndCooled) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_AskAboutJourney) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeReturnHowHaveYouBeen) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHome) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeWantToMakeFriends) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeEruptionMadeMess) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgValeNorthwestCrack) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_PsynergyStoneIsGone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeSurprisedByEarlyReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeIsaacHasGrown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeNewFriendsAreAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeEruptionHeatsRiver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeStrangeEventsElsewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeWhenDidYouReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValePlayTogetherAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeWantToJourney) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeLavaBurntOtherSide) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgValeCrackWidening) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x415, 0x306, EVENT_SCRIPT(Scene_PsynergyStoneIsGone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgValeEruptionTooMuchForPeople) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenNortheastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_LeaveThroughSouthPassage) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { PSYNERGY_ON_ACTOR(0x4e), 8, 0x90b, EVENT_SCRIPT(ValeHome_ClearFirstWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(0x4e), 9, 0x90c, EVENT_SCRIPT(ValeHome_ClearSecondWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(0x4e), 10, 0x90d, EVENT_SCRIPT(ValeHome_ClearThirdWhirlwindTarget) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(ValeHome_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

u16 gValeHome_CellSteps[] = {
    MAP_CELL_STEP(0, 57, 2, 2, 1), MAP_CELL_STEP(0, 59, 2, 2, 1), MAP_CELL_STEP(2, 59, 2, 2, 1), MAP_CELL_STEPS_END,
};
// clang-format on
