/// @file
/// Vale's village: its doors, the Psynergy Stone, the stormy night when the
/// villagers send Isaac to the elders and he overhears the two surviving
/// travelers, the villagers' farewell when the party sets out (entrance 12),
/// and the village after the party's return from Kalay.
///
/// Group flag 0x302: the villager welcomed Isaac home. Group flag 0x306: the
/// villager searching for the Psynergy Stone told Isaac it is gone.

#include "game_state.h"
#include "flags.h"
#include "character_ids.h"
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
#include "songs.h"
#include "item_ids.h"

enum ValeHomeActor {
  /// One of the two travelers. "Only the two of us survived..."
  ACTOR_FIRST_TRAVELER = 23,
  ACTOR_SECOND_TRAVELER = 24
};

// Forward declarations.
static void Event_MeetSurvivingTravelers(void);
static void ValeHome_FightTravelers(void);
static void Event_TravelersLeaveAfterBattle(void);
static void Event_DepartForQuest(void);
static void Event_SayThenWait(s32 speaker, s32 frames);
static void ValeHome_FaceDirectionAndWait(s32 actorId, s32 facing, s32 frames);
static void ValeHome_FlashPsynergyPalette(struct FieldActor *actor);
static void ValeHome_SpawnEffectPair(struct FieldActor *parent);
static void ValeHome_UpdateActor15Psynergy(void);
static void ValeHome_UpdateActor17Psynergy(void);
static void ValeHome_UpdateActor20Psynergy(void);
static void ValeHome_SetProgressFlagsAndExit(void);

static inline void ValeHome_TurnAndWait(s32 actor, s32 facing, s32 frames) {
  /* FAKEMATCH: forwarding inline for Event_DepartForQuest's three calls;
   * called directly, the function grows by 4 bytes in tbs-en and builds its
   * stack arguments in other registers. */
  ValeHome_FaceDirectionAndWait(actor, facing, frames);
}

#ifdef UBFIX
#define VALE_HOME_TURN_AND_WAIT ValeHome_FaceDirectionAndWait
#else
static inline s32 ValeHome_TurnAndWaitValue(s32 actor, s32 facing, s32 frames) {
  /* FAKEMATCH: forwarding inline called as returning a value, its result
   * discarded; called directly, 30 instructions differ in tbs-en (r5 and r6
   * swap). */
  return ((s32 (*)())ValeHome_FaceDirectionAndWait)(actor, facing, frames);
}
#define VALE_HOME_TURN_AND_WAIT ValeHome_TurnAndWaitValue
#endif

#include "common/object_facing.h"

struct SceneEntrance *ValeHome_GetEntrances(void) {
  return gValeHome_Entrances;
}

struct SceneRegion *ValeHome_GetRegions(void) { return NULL; }

u32 *ValeHome_GetExits(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeHome_StormExits;
  }
  return gValeHome_Exits;
}

struct ScenePlacement *ValeHome_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeHome_StormPlacements;
  }
  if (gGameState.entrance == 12) {
    return gValeHome_DeparturePlacements;
  }
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    return gValeHome_PlacementsAfterKalay;
  }
  return gValeHome_Placements;
}

/// Searching the Psynergy Stone restores the party's PP.
static void Event_UsePsynergyStone(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeCheckedThePsynergyStone, 1);
  Engine_AudioPlayCue(SE_RECOVERY);
  BattleParty_ApplyDrain(999, 0);
  Engine_EventWait(10);
  Engine_MessageShowCentered(MsgValePartyPpRestored, 1);
  UiWork_FinalizePendingCore();
  Engine_GameFlagClear(FLAG_NOTHING_HAPPENS);
  Engine_EventEnd();
}

struct SceneEvent *ValeHome_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    return gValeHome_EventsAfterKalay;
  }
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    return gValeHome_EventsAfterLeaving;
  }
  if (gGameState.entrance == 12) {
    return gValeHome_DepartureEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeHome_StormEvents;
  }
  return gValeHome_Events;
}

/// Asks about meditating each day. The question is whether the party meditates
/// on Mt. Aleph each day.
static void Event_AskAboutMeditation(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeMeditateOnMtAlephDaily);
  Engine_ActorFaceEachOther(23, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(23, 0);
  Engine_EventEnd();
}

/// Discuss the difficult events of three years ago.
static void Event_RecallThreeYearsAgo(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeADifficultTimeThreeYears);
  Engine_ActorFaceEachOther(24, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(24, 0);
  Engine_EventEnd();
}

/// Asks whether the travelers went to meet the mayor.
static void Event_AskAboutTheTravelers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDidTheTravelersMeetThe);
  Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

/// Before the party sets out, a villager shows Isaac his Psynergy and shakes
/// the ground; later he asks whether they worry about crossing the land so few.
static void Event_DemonstratePsynergy(void) {
  u32 i;
  s32 message;
  s32 shakes;
  s32 *origin;

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    message = MsgValeArentWorriedCrossing;
    Engine_EventSetMessage(message);
    // "...with just the two of you?", three or four: the flag numbered for a
    // character is set once that character has joined.
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0) {
      gEventWork->message++;
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
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
    Scene_BeginCutscenePsynergy();
    shakes = 0;
    for (i = 0; i < 40; i++) {
      ValeHome_FlashPsynergyPalette(Object_GetById(17));
      Engine_TaskWait(1);
    }
    FieldScheduler_AddCallback(ValeHome_UpdateActor17Psynergy, 0xc80);
    Engine_AudioPlayCue(SE_107);
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
    Engine_AudioPlayCue(SE_289);
    Scheduler_RemoveCallback(ValeHome_UpdateActor17Psynergy);
    Engine_TaskWait(1);
    Scene_EndCutscenePsynergy();
    Engine_ActorSetChildValue(17, 0);
    Engine_EventWait(40);
    Engine_EventSetMessage(MsgValeBeholdPowerPsynergy);
    Engine_EventShowMessage(17, 0);
  }
  Engine_EventEnd();
}

/// Asks about the journey, the Wise One or learning Psynergy.
static void Event_AskAboutJourney(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
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

/// Turns toward the fallen stone while describing what happened.
static void Event_TalkAboutFallenStone(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(26, ANIM_STAND);
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

/// A villager with some Psynergy left lifts actor 20 into the air.
static void Event_DemonstrateLevitation(void) {
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
  Scene_BeginCutscenePsynergy();
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
  Event_PlayCue(SE_161);
  Actor_SetChildValue(15, 0);
  Actor_SetChildValue(20, 0);
  Engine_EventWait(40);
  Scene_EndCutscenePsynergy();
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
  Event_ShowMessage(15, 0);
  Engine_EventEnd();
}

/// Discuss whom the party should follow and help after the storm.
static void Event_TalkAboutHelping(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 10);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
    Engine_EventSetMessage(MsgValeSureHelp);
    Engine_EventShowMessage(16, 0);
  } else {
    Engine_EventSetMessage(MsgValeWentOffWay);
    Engine_EventShowMessage(16, 0);
  }
  Engine_EventEnd();
}

/// Welcomes the party home, then uses the repeat greeting on later visits.
static void Event_WelcomeIsaacHome(void) {
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

/// Pauses the villager's search to discuss the missing Psynergy Stone.
static void Event_PsynergyStoneIsGone(void) {
  struct FieldActor *actor = Object_GetById(21);
  Engine_EventBegin();
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  Engine_ActorSetAnimation(21, ANIM_STAND);
  Engine_ActorStop(21);
  Engine_ActorShowEmote(21, EMOTE_IN_FRONT, 40);
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

#include "common/vale.h"

/// Opens a door's map cells and sends the leader through its threshold.
static inline void Event_OpenDoorAndWalkThrough(s32 cellX, s32 cellZ, s32 x,
                                                s32 z) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeHome_DoorOpens, cellX, cellZ);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, z);
}

static void Event_OpenNorthwestDoor(void) {
  Event_OpenDoorAndWalkThrough(44, 7, 248, 279);
  Vale_RequestBackdropExit(1);
}

/// Opens the northeast doorway in two stages and walks inside.
static void Event_OpenNortheastDoor(void) {
  Event_PlayCue(SE_188);
  Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
  Engine_TaskWait(10);
  Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
  Engine_TaskWait(10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
  Vale_RequestBackdropExit(2);
}

/// Enters through the western door in the village's middle row.
static void Event_OpenCentralWestDoor(void) {
  Event_OpenDoorAndWalkThrough(43, 15, 230, 407);
  Vale_RequestBackdropExit(3);
}

/// Enters through the eastern door in the village's middle row.
static void Event_OpenCentralEastDoor(void) {
  Event_OpenDoorAndWalkThrough(52, 18, 374, 419);
  Vale_RequestBackdropExit(4);
}

static void Event_OpenSouthwestDoor(void) {
  Event_OpenDoorAndWalkThrough(41, 32, 200, 546);
  Vale_RequestBackdropExit(5);
}

static void Event_OpenWestEdgeDoor(void) {
  Event_OpenDoorAndWalkThrough(35, 36, 102, 611);
  Vale_RequestBackdropExit(6);
}

static void Event_OpenSoutheastDoor(void) {
  Event_OpenDoorAndWalkThrough(51, 39, 358, 670);
  Vale_RequestBackdropExit(7);
}

static void Event_LeaveThroughWestPassage(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(8);
}

/// Leaves by the south passage once the party has begun its journey.
static void Event_LeaveThroughSouthPassage(void) {
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Vale_RequestBackdropExit(10);
  }
}

/// Remembers that Whirlwind cleared actor 8.
static void Event_ClearFirstWhirlwindTarget(void) {
  Engine_GameFlagSet((FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED));
}

/// Remembers that Whirlwind cleared actor 9.
static void Event_ClearSecondWhirlwindTarget(void) {
  Engine_GameFlagSet((FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED));
}

/// Remembers that Whirlwind cleared actor 10.
static void Event_ClearThirdWhirlwindTarget(void) {
  Engine_GameFlagSet((FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED));
}

/// Arriving: hide the cleared Whirlwind targets, run entrances 97 to 99 (the
/// return from the travelers' battle and two debug presets), and set up the
/// storm, the farewell or the village after Kalay.
s32 ValeHome_Initialize(void) {
  struct FieldActor *actor;
  s32 flag;
  s32 verticalScale;

  if (Engine_GameFlagIsSet((FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED)) != 0) {
    Engine_ActorSetPosition(8, 0, 0);
  }
  if (Engine_GameFlagIsSet((FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED)) != 0) {
    Engine_ActorSetPosition(9, 0, 0);
  }
  if (Engine_GameFlagIsSet((FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED)) != 0) {
    Engine_ActorSetPosition(10, 0, 0);
  }
  switch (gGameState.entrance) {
  case 98:
    Engine_GameFlagSet(FLAG_CHILDHOOD);
    Engine_EventRequestExit(50);
    return 0;
  case 99:
    ValeHome_SetProgressFlagsAndExit();
    return 0;
  case 97:
    Event_TravelersLeaveAfterBattle();
    return 0;
  }
  verticalScale = 3 * Q16_ONE / 2;
  actor = Object_GetById(8);
  actor->scaleY = verticalScale;
  actor = Object_GetById(9);
  actor->scaleY = verticalScale;
  actor = Object_GetById(10);
  actor->scaleY = verticalScale;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    Map_CopyCellsTo(97, 2, 80, 5, 2, 2);
    Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
      FieldFx_StartGroundParticles();
      FieldFx_StartLightning();
      Map_CopyCellAttributes(21, 38, 1, 1, 18, 41);
      flag = Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
      if (flag != 0) {
        Engine_ActorSetPosition(17, 0, 0);
        Engine_ActorSetPosition(18, 0, 0);
        FieldObject_SetTargetAndCallback(
            19, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gValeHome_FaceTargetActions);
      }
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Actor_SetPosition(16, PIXELS(180), PIXELS(568));
        Map_CopyCellsTo(92, 2, 80, 5, 2, 2);
        Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
      }
    }
    if (gGameState.entrance == 12) {
      Event_DepartForQuest();
    } else {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
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
        if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
          Actor_SetPosition(21, PIXELS(331), PIXELS(249));
          actor = Object_GetById(21);
          Engine_ActorSetSpriteFlags(actor, 0);
        }
      }
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
        Engine_ActorSetPosition(26, 0, 0);
        Engine_ActorSetPosition(22, 0, 0);
      }
      if (gGameState.entrance == 19) {
        Event_TravelersLeaveAfterBattle();
      } else {
        if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0 &&
            (flag = Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_LEFT)) != 0) {
          Event_MeetSurvivingTravelers();
        } else {
          if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
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

/// Walks a separate scene actor back to the leader, then hides it.
static inline void ValeHome_RejoinParty(s32 actorId) {
  struct FieldActor *leader;

  Engine_ActorSetAnimation(actorId, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(actorId, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(actorId);
  Engine_ActorSetPosition(actorId, 0, 0);
}

/// On the stormy night, villagers send Isaac and his companion to aid the
/// elders holding back the Boulder. "Go aid the elders!"
static void Event_SentToAidElders(void) {
  struct FieldActor *leader;
  s32 skipReply = 0;
  s32 speechFlags;

  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
    // Gather the leader and companion while the elders organize the helpers.
    Engine_EventBegin();
    Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    Engine_CameraMoveTo(PIXELS(197), -1, PIXELS(768), 1);
    Camera_WaitForArrival();
    Engine_EventSetMessage(MsgValeGoAidElders);
    Engine_ActorRunRepeatedMotion(19, 2);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 19, 0, 10);
    Actor_SetSpeed(0, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(25, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(0, 179, 789);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Engine_ActorSetPosition(25, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkToAndWait(25, 179, 804);
    Engine_ActorFaceEachOther(0, 25, 40);
    Engine_ActorFaceDirection(0, FACING_EAST, 0);
    Engine_ActorFaceDirection(25, FACING_EAST, 0);
    Engine_ActorSetAnimation(17, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(18, ANIM_NOD);
    Engine_ActorFaceEachOther(17, 18, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(17, 1);
    Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 17, 0, 10);
    Engine_ActorSetAnimation(18, ANIM_NOD);
    Engine_EventShowMessageAndWait(18, 0, 10);
    Engine_ActorFaceDirection(17, FACING_EAST, 0);
    Actor_FaceDirection(18, FACING_NORTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(19, ANIM_NOD);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 19, 0, 10);
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
    Engine_ActorFaceDirection(0, FACING_EAST, 0);
    Engine_ActorFaceDirection(25, FACING_EAST, 10);
    Engine_EventShowMessage(25, 0);
    Actor_FaceDirection(19, FACING_WEST, 0);
    ValeHome_FaceDirectionAndWait(26, FACING_SOUTHWEST, 20);
    Engine_ActorRunRepeatedMotion(26, 2);
    Event_SayThenWait(26, 10);
    Engine_ActorSetAnimation(0, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(25, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | 19, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      skipReply = 1;
      Engine_ActorSetAnimation(19, ANIM_SHAKE_HEAD);
    } else {
      Engine_ActorSetAnimation(19, ANIM_NOD);
      gEventWork->message++;
    }
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 19, 0);
    if (skipReply != 0) {
      gEventWork->message++;
    }
    speechFlags = SPEAKER_WINDOW_ABOVE;
    ValeHome_FaceDirectionAndWait(22, FACING_SOUTH, 30);
    Engine_EventShowMessage(22, 0);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(26, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(0, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(25, EMOTE_IN_FRONT | 0, 40);
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
    Engine_ActorSetAnimationAndWait(22, ANIM_NOD);
    Event_SayThenWait(22, 20);
    Engine_ActorSetAnimationAndWait(19, ANIM_NOD);
    Engine_EventWait(10);
    ValeHome_FaceDirectionAndWait(19, FACING_SOUTH, 30);
    Event_SayThenWait(speechFlags + 19, 10);
    ValeHome_FaceDirectionAndWait(26, FACING_NORTHEAST, 30);
    Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
    ValeHome_FaceDirectionAndWait(19, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(19, 2);
    Event_SayThenWait(speechFlags + 19, 10);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 25, 40);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    ValeHome_FaceDirectionAndWait(25, FACING_EAST, 20);
    ValeHome_FaceDirectionAndWait(26, FACING_WEST, 30);
    Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
    Event_SayThenWait(26, 30);
    ValeHome_FaceDirectionAndWait(26, FACING_NORTH, 30);
    Engine_ActorSetAnimationAndWait(26, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(22, ANIM_NOD);
    // Return the separate companions to the leader and finish this story step.
    ValeHome_RejoinParty(25);
    ValeHome_RejoinParty(26);
    ValeHome_RejoinParty(22);
    FieldObject_SetTargetAndCallback(
        19, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
        gValeHome_FaceTargetActions);
    Engine_GameFlagSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
    Engine_EventEnd();
  }
}

/// On the stormy night, Isaac overhears the two travelers who survived Sol
/// Sanctum. "Only the two of us survived..."
static void Event_MeetSurvivingTravelers(void) {
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
  Engine_AudioPlayCue(MUS_017);
  FieldFx_ResumeLightning();
  Engine_ActorSetPosition(ACTOR_FIRST_TRAVELER, PIXELS(105), PIXELS(267));
  Engine_TaskWait(1);
  Actor_SetSpeed(0, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkToAndWait(0, 93, 343);
  Engine_EventSetMessage(MsgValeOnlyTwoSurvived);
  Engine_EventShowMessage(ACTOR_FIRST_TRAVELER, 0);
  Engine_AudioPlayCue(MUS_061);
  camera->motionFlags = 0;
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Engine_CameraMoveTo(PIXELS(109), PIXELS(176), PIXELS(281), 1);
  Camera_WaitForArrival();
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_SECOND_TRAVELER, PIXELS(135), PIXELS(177));
  Actor_SetSpeed(ACTOR_SECOND_TRAVELER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_SECOND_TRAVELER, 126, 258);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_NORTH + FACING_STEP,
                            0);
  Engine_ActorWaitForMove(ACTOR_SECOND_TRAVELER);
  Engine_ActorSetAnimation(ACTOR_SECOND_TRAVELER, ANIM_STAND);
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER,
                                FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
  Event_SayThenWait(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 30);
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER,
                                FACING_NORTHWEST + FACING_STEP, 20);
  Event_SayThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 10);
  ValeHome_FaceDirectionAndWait(ACTOR_FIRST_TRAVELER,
                                FACING_NORTHWEST + FACING_STEP, 40);
  Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
  ValeHome_FaceDirectionAndWait(ACTOR_FIRST_TRAVELER,
                                FACING_NORTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
  Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 0);
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
  Engine_EventWait(20);
  Event_SayThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 20);
  ValeHome_FightTravelers();
  Engine_EventEnd();
}

/// The travelers catch Isaac eavesdropping and fight; win or lose, the party
/// returns by entrance 19. "You must forget everything you heard."
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
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 25, 0);
  Actor_ShowEmote(ACTOR_FIRST_TRAVELER, EMOTE_IN_FRONT | 0, 0);
  Actor_ShowEmote(ACTOR_SECOND_TRAVELER, EMOTE_IN_FRONT | 0, 40);
  Engine_ActorSetPosition(25, 0, 0);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTH + FACING_STEP, 0);
  southSouthwestFacing = FACING_SOUTH + FACING_STEP;
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, southSouthwestFacing,
                                40);
  Engine_CameraSetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  Engine_CameraMoveTo(PIXELS(89), PIXELS(176), PIXELS(313), 1);
  Camera_WaitForArrival();
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_NORTHEAST, 0);
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER,
                                FACING_SOUTHWEST + FACING_STEP, 40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(100), PIXELS(144), PIXELS(333), 1);
  Actor_SetSpeed(ACTOR_FIRST_TRAVELER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_SECOND_TRAVELER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_FIRST_TRAVELER, 105, 329);
  Engine_EventWait(10);
  Engine_ActorWalkTo(ACTOR_SECOND_TRAVELER, 124, 329);
  Engine_ActorWaitForMove(ACTOR_FIRST_TRAVELER);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRAVELER, ANIM_STAND);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, southSouthwestFacing, 0);
  Engine_ActorWaitForMove(ACTOR_SECOND_TRAVELER);
  Engine_ActorSetAnimation(ACTOR_SECOND_TRAVELER, ANIM_STAND);
  Engine_ActorFaceDirection(ACTOR_SECOND_TRAVELER, southSouthwestFacing, 0);
  // Bring the companion into the conversation beside the party leader.
  Engine_ActorSetChildValue(25, 0);
  actor = Object_GetById(25);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetPosition(25, 0, PIXELS(331));
  Actor_SetSpeed(25, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkToAndWait(25, 37, 339);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_EventOpenMessage(ACTOR_FIRST_TRAVELER, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | 1, 0);
  northNortheastFacing = FACING_NORTH + FACING_STEP;
  ValeHome_FaceDirectionAndWait(0, northNortheastFacing, 10);
  Engine_EventChooseYesNo(0, 0);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_EAST, 0);
  westFacing = FACING_WEST;
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, westFacing, 20);
  Engine_ActorRunRepeatedMotion(25, 2);
  Event_SayThenWait(SPEAKER_PORTRAIT_LEFT | 25, 10);
  FieldObject_SetTargetAndCallback(0, ACTOR_LINK_KEEP_MOTION | 25,
                                   gValeHome_FaceTargetActions);
  Actor_WalkToAndWait(25, 93, 361);
  ValeHome_FaceDirectionAndWait(25, northNortheastFacing, 40);
  Event_SayThenWait(25, 20);
  Engine_ActorStop(0);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_EAST, 0);
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, westFacing, 15);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, southSouthwestFacing, 0);
  ValeHome_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, southSouthwestFacing,
                                30);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
  ValeHome_FaceDirectionAndWait(0, northNortheastFacing, 30);
  ValeHome_FaceDirectionAndWait(0, FACING_SOUTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Event_SayThenWait(ACTOR_FIRST_TRAVELER, 20);
  Actor_SetAttachedEffect(0, 0x102);
  Engine_ActorSetAttachedEffect(25, 0x102);
  Engine_EventWait(80);
  Actor_SetActionScript(ACTOR_SECOND_TRAVELER, gValeHome_Actor24Actions);
  Engine_EventWait(6);
  Actor_SetActionScript(ACTOR_FIRST_TRAVELER, gValeHome_Actor23Actions);
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
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// After the battle the travelers leave Isaac and his companion lying in the
/// storm, and the storm ends.
static void Event_TravelersLeaveAfterBattle(void) {
  struct FieldActor *actor;
  s32 gravityQ8;

  // Restore the defeated party and the travelers at their battle positions.
  Engine_AudioPlayCue(MUS_017);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_CameraMoveTo(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetPosition(0, PIXELS(48), PIXELS(346));
  Actor_SetPosition(25, PIXELS(78), PIXELS(358));
  Actor_SetPosition(ACTOR_FIRST_TRAVELER, PIXELS(103), PIXELS(342));
  Actor_SetPosition(ACTOR_SECOND_TRAVELER, PIXELS(112), PIXELS(360));
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_TRAVELER, FACING_WEST, 0);
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
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_TRAVELER, FACING_NORTH, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_WEST, 10);
  Actor_FaceDirection(ACTOR_SECOND_TRAVELER, FACING_WEST, 10);
  Engine_ActorSetSpritePriority(0, 3);
  Engine_ActorSetSpritePriority(25, 3);
  // Give each traveler its retreating bounce before the final lightning.
  Actor_SetSpeed(ACTOR_FIRST_TRAVELER, PIXELS(12) / 5, PIXELS(6) / 5);
  gravityQ8 = 128;
  actor = Object_GetById(ACTOR_FIRST_TRAVELER);
  actor->bounce = 0x28f;
  actor->gravity = gravityQ8 << 8;
  Actor_SetActionScript(ACTOR_FIRST_TRAVELER, gValeHome_Actor23BounceActions);
  Engine_EventWait(ACTOR_SECOND_TRAVELER);
  Engine_ActorSetSpeed(ACTOR_SECOND_TRAVELER, PIXELS(12) / 5, PIXELS(6) / 5);
  actor = Object_GetById(ACTOR_SECOND_TRAVELER);
  actor->bounce = 0x28f;
  actor->gravity = gravityQ8 << 8;
  Actor_RunActionScriptAndWait(ACTOR_SECOND_TRAVELER,
                               gValeHome_Actor24BounceActions);
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
  Engine_GameFlagClear(FLAG_VALE_STORM);
  Engine_EventRequestExit(9);
  Engine_EventEnd();
}

/// The villagers give their supplies and farewells before the party leaves
/// Vale.
static void Event_DepartForQuest(void) {
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
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 40);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 20);
  Event_SetMessage(MSGNO(MsgValeWhysEveryoneHanging));
  Event_SayThenWait(12, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 10);
  Event_SayThenWait(11, 10);
  Engine_ActorSetAnimationAndWait(12, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(11, 2);
  Event_SayThenWait(11, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 0, 40);
  Actor_SetSpeed(12, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(12, 184, 618);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 60);
  Event_SayThenWait(12, 20);
  Actor_SetSpeed(11, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(11, 168, 618);
  ValeHome_TurnAndWait(11, FACING_NORTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(11, ANIM_SHAKE_HEAD);
  Event_SayThenWait(11, 20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventShowMessage(12, 0);
  Engine_EventWait(10);
  // The two nearby actors run through while villagers turn to watch them.
  Actor_SetSpeed(30, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetPosition(30, PIXELS(110), PIXELS(744));
  Engine_TaskWait(2);
  Engine_ActorSetAnimation(30, ANIM_NOD);
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
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 5, 120);
  Engine_ActorFaceDirection(11, FACING_EAST + FACING_STEP, 0);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 80);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTH + FACING_STEP, 60);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTH + FACING_STEP, 60);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorFaceDirection(11, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(12, 184, 630);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTH + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 40);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
  Actor_WalkToAndWait(11, 168, 630);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 10);
  Event_SayThenWait(11, 20);
  Engine_ActorSetAnimationAndWait(12, ANIM_NOD);
  Event_SayThenWait(12, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 0, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTHEAST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(11, FACING_SOUTH + FACING_STEP, 60);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
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
  Engine_ActorSetAnimation(31, ANIM_WALK);
  actor31Actions = gValeHome_Actor31Actions;
  Actor_SetActionScript(31, actor31Actions);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(30, ANIM_NOD);
  Actor_SetSpeed(30, PIXELS(24) / 5, PIXELS(12) / 5);
  Actor_RunActionScriptAndWait(30, actor31Actions);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(12, 2);
  ValeHome_FaceDirectionAndWait(12, FACING_SOUTHWEST + FACING_STEP, 10);
  Event_SayThenWait(12, 10);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(11, FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
  Event_SayThenWait(11, 20);
  Engine_ActorSetAnimation(12, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
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
  Engine_ActorFaceDirection(0, FACING_EAST, 0);
  ValeHome_FaceDirectionAndWait(1, FACING_WEST + FACING_STEP, 20);
  ValeHome_FaceDirectionAndWait(0, FACING_NORTH, 10);
  ValeHome_FaceDirectionAndWait(1, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimation(0, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(1, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(16, 2);
  Engine_EventWait(20);
  Event_SayThenWait(16, 10);
  Actor_SetSpeed(16, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(16, 216, 800);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Engine_PartyGiveItem(ITEM_HERB, 0);
  Actor_WalkToAndWait(16, 264, 800);
  Actor_FaceDirection(16, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(1, EMOTE_IN_FRONT | 2, 40);
  ValeHome_TurnAndWait(1, FACING_NORTHEAST + FACING_STEP, 10);
  Event_SayThenWait(1, 10);
  Engine_ActorSetAnimationAndWait(17, ANIM_SHAKE_HEAD);
  Event_SayThenWait(17, 10);
  Engine_ActorFaceDirection(1, FACING_EAST + FACING_STEP, 0);
  Actor_ShowEmote(1, EMOTE_IN_FRONT | 3, 20);
  ObjectMotion_Launch(1, 4, 60);
  Engine_ActorRunRepeatedMotion(14, 2);
  Engine_EventWait(20);
  ValeHome_FaceDirectionAndWait(14, FACING_NORTH + FACING_STEP, 10);
  Event_SayThenWait(14, 60);
  Actor_ShowEmote(1, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(16, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(19, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | 0, 0);
  Event_SayThenWait(17, 60);
  // Have both party members follow the villager presenting the next gift.
  Object_LinkObjectAndSetCallback(0, 17);
  Object_LinkObjectAndSetCallback(1, 17);
  Actor_WalkToAndWait(17, 216, 800);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Event_SayThenWait(17, 60);
  Engine_PartyGiveItem(ITEM_CATCH_BEADS, 0);
  Engine_ActorStop(0);
  Engine_ActorStop(1);
  Actor_WalkToAndWait(17, 272, 816);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Engine_ActorRunRepeatedMotion(1, 2);
  ValeHome_FaceDirectionAndWait(1, FACING_WEST + FACING_STEP, 10);
  Event_SayThenWait(1, 10);
  Engine_ActorFaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  ValeHome_FaceDirectionAndWait(0, FACING_EAST, 10);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorRunRepeatedMotion(16, 1);
  Event_SayThenWait(16, 10);
  ValeHome_TurnAndWait(1, FACING_NORTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(1, EMOTE_IN_FRONT | 1, 20);
  Engine_ActorSetAnimationAndWait(16, ANIM_SHAKE_HEAD);
  Event_SayThenWait(16, 10);
  Engine_ActorSetAnimationAndWait(18, ANIM_NOD);
  Engine_EventShowMessage(18, 0);
  Engine_ActorFaceDirection(1, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorSetAnimation(18, ANIM_SHAKE_HEAD);
  Engine_EventShowMessage(18, 0);
  Engine_ActorStartRepeatedMotion(18, 3);
  Engine_EventShowMessage(18, 0);
  Engine_ActorSetAnimationAndWait(16, ANIM_NOD);
  Engine_ActorSetAnimation(19, ANIM_NOD);
  Engine_ActorSetAnimation(17, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(24, ANIM_NOD);
  Engine_ActorSetAnimation(18, ANIM_NOD);
  Engine_ActorSetAnimation(27, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(28, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(25, ANIM_NOD);
  Engine_ActorSetAnimation(20, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
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
  Engine_ActorSetAnimation(11, ANIM_NOD);
  Engine_ActorSetAnimation(14, ANIM_NOD);
  Engine_ActorSetAnimation(17, ANIM_NOD);
  Engine_ActorSetAnimation(20, ANIM_NOD);
  Engine_ActorSetAnimation(23, ANIM_NOD);
  Engine_ActorSetAnimation(26, ANIM_NOD);
  Engine_ActorSetAnimation(29, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(12, ANIM_NOD);
  Engine_ActorSetAnimation(15, ANIM_NOD);
  Engine_ActorSetAnimation(18, ANIM_NOD);
  Engine_ActorSetAnimation(21, ANIM_NOD);
  Engine_ActorSetAnimation(24, ANIM_NOD);
  Engine_ActorSetAnimation(27, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(13, ANIM_NOD);
  Engine_ActorSetAnimation(16, ANIM_NOD);
  Engine_ActorSetAnimation(19, ANIM_NOD);
  Engine_ActorSetAnimation(22, ANIM_NOD);
  Engine_ActorSetAnimation(25, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(28, ANIM_NOD);
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
  Actor_ShowEmote(0, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(1, EMOTE_IN_FRONT | 2, 80);
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

static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

static void ValeHome_FaceDirectionAndWait(s32 actorId, s32 facing, s32 frames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(frames);
}

/// Flashes the caster's palette and emits paired effects every sixteen frames.
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

/// Cycles six palette banks and emits the caster's paired effects.
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

/// Cycles the levitated actor through six palette banks.
static void ValeHome_CyclePsynergyPalette(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    Engine_ObjectSetPartPalettes(actor, palette);
  }
}

#include "common/effect_vertical.h"

/// Creates the two vertical Psynergy effects using the shared sprite tiles.
static void ValeHome_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct PairWork *work = gEffectWork;
  s32 i;

  Engine_AudioPlayCue(SE_131);
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

#include "common/effect_psynergy.h"

/// Updates the levitating villager's Psynergy display.
static void ValeHome_UpdateActor15Psynergy(void) {
  ValeHome_CyclePsynergyPaletteAndSpawnEffects(Object_GetById(15));
}

/// Updates the demonstrating villager's Psynergy display.
static void ValeHome_UpdateActor17Psynergy(void) {
  ValeHome_CyclePsynergyPaletteAndSpawnEffects(Object_GetById(17));
}

/// Updates the palette of the levitated actor.
static void ValeHome_UpdateActor20Psynergy(void) {
  ValeHome_CyclePsynergyPalette(Object_GetById(20));
}

/// Arriving by entrance 99, a debug entrance: set a preset of story flags,
/// which differs by edition, and leave by exit 40.
static void ValeHome_SetProgressFlagsAndExit(void) {
  Engine_GameFlagSet(352);
  Engine_GameFlagSet(0x16d);
#if TBS_PATCH_3
  Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_THIRD_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  Engine_GameFlagSet(FLAG_VALE_STORM_HEARD_GARET);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_JOINED);
  Engine_GameFlagSet(FLAG_VALE_BOULDER_FELL);
  Engine_GameFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
  Engine_GameFlagSet(FLAG_VALE_STORM_SAW_RIVER_RESCUE);
  Engine_GameFlagSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
  Engine_GameFlagSet(FLAG_VALE_STORM_RIVER_GATHERING_SEEN);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_LEFT);
  Engine_GameFlagSet(FLAG_VALE_ASKED_IF_GOING_TO_KRADEN);
  Engine_GameFlagSet(FLAG_VALE_ANGRY_VILLAGER_ARGUED);
  Engine_GameFlagSet(FLAG_MET_SATUROS_AND_MENARDI);
  Engine_GameFlagSet(FLAG_OVERHEARD_KRADEN);
  Engine_GameFlagSet(FLAG_SANCTUM_VISIT_PLANNED);
  Engine_GameFlagSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_ENTERED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED);
  Engine_GameFlagSet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_INNER_STATUES_SEEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FOUND);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_ERUPTED);
  Engine_GameFlagSet(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
  Engine_GameFlagSet(FLAG_PARTY_LEFT_VALE);
  Engine_GameFlagSet(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  Engine_GameFlagSet(FLAG_GARET_CAME_HOME);
  Engine_GameFlagSet(FLAG_REACHED_KALAY);
#else
  Engine_GameFlagSet(FLAG_TRET_DEFEATED);
  Engine_GameFlagSet(FLAG_KOLIMA_CURSE_LIFTED);
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
    SC_CALL(Scene_TurnTowardTarget),
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

struct SceneEntrance gValeHome_Entrances[] = {
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

u32 gValeHome_Exits[] = {
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

u32 gValeHome_StormExits[] = {
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

struct ScenePlacement gValeHome_Placements[] = {
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

struct ScenePlacement gValeHome_StormPlacements[] = {
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

struct ScenePlacement gValeHome_DeparturePlacements[] = {
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

struct ScenePlacement gValeHome_PlacementsAfterKalay[] = {
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

struct SceneEvent gValeHome_Events[] = {
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeQuietDay) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeKradenHadVisitors) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutMeditation) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RecallThreeYearsAgo) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeChosenPeople) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeProudOfAdeptGrandfather) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutTheTravelers) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValePracticePsynergy) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DemonstratePsynergy) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeWalkToRecoverPsynergy) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgValeFoodStoredForWinter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughSouthPassage) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, (FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearFirstWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, (FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearSecondWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, (FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearThirdWhirlwindTarget) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHome_StormEvents[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJourney) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeOldWomanCannotUsePsynergy) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeExhaustedFromFallingRocks) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeFelixFellInRiver) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DemonstrateLevitation) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutHelping) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeRaceAgainstTime) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughWestPassage) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SentToAidElders) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHome_DepartureEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHome_EventsAfterLeaving[] = {
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeTremorsSettled) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeRoadToVault) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgValeRoadToGomaRange) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgValePsynergyStonePowder) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJourney) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeLunpaFoundedNorthernTown) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeDoraHasBeenCrying) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeStonesScatteredByEruption) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DemonstratePsynergy) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeReplenishPsynergyBeforeDungeon) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutFallenStone) },
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughSouthPassage) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, (FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearFirstWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, (FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearSecondWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, (FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearThirdWhirlwindTarget) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHome_EventsAfterKalay[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WelcomeIsaacHome) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeCameBackSooner) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeMadeFriendsOnJourney) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeRiverHeatedAndCooled) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJourney) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeReturnHowHaveYouBeen) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHome) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeWantToMakeFriends) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeEruptionMadeMess) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgValeNorthwestCrack) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PsynergyStoneIsGone) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x415, 0x306, EVENT_SCRIPT(Event_PsynergyStoneIsGone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgValeEruptionTooMuchForPeople) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNortheastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughSouthPassage) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, (FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearFirstWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, (FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearSecondWhirlwindTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, (FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED), EVENT_SCRIPT(Event_ClearThirdWhirlwindTarget) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

u16 gValeHome_DoorOpens[] = {
    MAP_CELL_STEP(0, 57, 2, 2, 1), MAP_CELL_STEP(0, 59, 2, 2, 1), MAP_CELL_STEP(2, 59, 2, 2, 1), MAP_CELL_STEPS_END,
};
// clang-format on
