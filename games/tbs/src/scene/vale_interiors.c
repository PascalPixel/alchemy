#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/vale_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "vale_interiors.h"
#include "call.h"
#include "fxblend.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "ram_buffer.h"

// Forward declarations.
static void ValeInteriors_ReuniteWithFamily(void);
static void ValeInteriors_SpawnDriftingSprite(void);

// Shared source is included here in ROM order.
#include "common/object_facing.h"

// Move and grow the sprite, using timer/work as signed 8.8 pixel steps.
static s32
ValeInteriors_AdvanceDriftingSprite(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;

  // This script uses timer and work as signed 8.8 pixel steps.
  actor->x.fixed += (s16)actor->timer * 256;
  actor->y.fixed += (s16)actor->work * 256;
  actor->scaleX += Q16_ONE / 40;
  actor->scaleY += Q16_ONE / 40;
  actor->timer += 5;
  actor->work = (s16)actor->work - 1;
  return 0;
}

// Return the entrance positions and view bounds for the Vale interiors.
struct SceneEntrance *ValeInteriors_GetEntrances(void) {
  return gValeHouseEntrances;
}

// This overlay has no separate region table.
struct SceneRegion *ValeInteriors_GetRegions(void) { return NULL; }

// Choose the normal exits or the exits used during the lightning sequence.
u32 *ValeInteriors_SelectExits(void) {
  if (Engine_GameFlagIsSet(0x834) != 0)
    return gValeHouseLateExits;
  return gValeHouseExits;
}

// Prepare placements for the first departure or the later return to Vale.
struct ScenePlacement *ValeInteriors_SelectPlacements(void) {
  struct ScenePlacement *table;

  if (Engine_GameFlagIsSet(0x87a)) {
    table = gValeHouseReturnPlacements;
  } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE)) {
    table = gValeHousePlacementsAfterLeaving;
  } else {
    table = gValeHousePlacements;
  }
  SceneEvents_UpdateInView(table);
  return table;
}

// Select dialogue and triggers for the current visit to Vale.
struct SceneEvent *ValeInteriors_SelectEvents(void) {
  if (Engine_GameFlagIsSet(0x834) != 0)
    return gValeHouseLateEvents;
  if (Engine_GameFlagIsSet(0x87a) != 0)
    return gValeHouseReturnEvents;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gValeHouseEventsAfterLeaving;
  return gValeHouseEvents;
}

// Ask about Kraden coming to Vale to study Mount Aleph.
static void ValeInteriors_AskAboutKradensStudies(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeKradenCameToStudyMt);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

// Ask the leader about frightening Kraden's visitors.
static void ValeInteriors_AskToScareVisitors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeLetsScareKradensVisitors);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
  Engine_EventAskYesNo(19, 0);
  Engine_EventEnd();
}

// Give the one-time farewell herb or remind the leader to rescue Jenna.
static void ValeInteriors_GiveFarewellHerb(void) {
  s32 gift;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_GOT_FAREWELL_HERB) != 0) {
    Engine_EventSetMessage(MsgValeYouMustSaveJenna);
    Event_ShowMessage(20, 0);
    FieldObject_SetTargetAndCallback(
        20, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER, gValeFaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    Engine_EventSetMessage(gift);
    Event_ShowMessageAndWait(20, 0, 20);
    Engine_MessageShowCentered((gift + 1), 1);
    Engine_PartyGiveItem(ITEM_HERB, 0);
    GameFlag_Set(FLAG_GOT_FAREWELL_HERB);
  }
  Engine_EventEnd();
}

// Ask about the culprits' unusual powers.
static void ValeInteriors_AskAboutCulpritsPowers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeTheCulpritsHadStrangePowers);
  Engine_EventAskYesNo(16, 0);
  Engine_EventEnd();
}

// Ask whether the leader overheard the conversation about Dora.
static void ValeInteriors_AskAboutDora(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDidYouHearAboutDora);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

// Show concern about the travelers' health and Garet's appetite.
static void ValeInteriors_DiscussTravelersHealth(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeHopeDidntGet);
  Engine_EventShowMessage(0x800b, 0);
  Engine_EventEnd();
}

// Stop any lightning effect, close the window transition, and request the exit.
static void ValeInteriors_RequestWindowExit(s32 exitId) {
  if (Engine_GameFlagIsSet(0x834) != 0)
    FieldFx_StopLightning();
  Engine_AudioPlayCue(123);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(exitId);
}

// Leave through the doorway connected to exit 1.
static void ValeInteriors_LeaveThroughExit1(void) {
  ValeInteriors_RequestWindowExit(1);
}

// Leave through the doorway connected to exit 2.
static void ValeInteriors_LeaveThroughExit2(void) {
  ValeInteriors_RequestWindowExit(2);
}

// Leave through the doorway connected to exit 3.
static void ValeInteriors_LeaveThroughExit3(void) {
  ValeInteriors_RequestWindowExit(3);
}

// Leave through the doorway connected to exit 4.
static void ValeInteriors_LeaveThroughExit4(void) {
  ValeInteriors_RequestWindowExit(4);
}

// Leave through the doorway connected to exit 5.
static void ValeInteriors_LeaveThroughExit5(void) {
  ValeInteriors_RequestWindowExit(5);
}

// Leave through the doorway connected to exit 6.
static void ValeInteriors_LeaveThroughExit6(void) {
  ValeInteriors_RequestWindowExit(6);
}

// Leave through the doorway connected to exit 7.
static void ValeInteriors_LeaveThroughExit7(void) {
  ValeInteriors_RequestWindowExit(7);
}

// Leave through the doorway connected to exit 8.
static void ValeInteriors_LeaveThroughExit8(void) {
  ValeInteriors_RequestWindowExit(8);
}

// Restore the room state and play the returning party's reunion once.
s32 ValeInteriors_Initialize(void) {
  struct FieldActor *actor;
  struct EventWork **eventSlot = &gEventWork;

  (*eventSlot)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  // The late visit hides residents while lightning plays outside the house.
  if (Engine_GameFlagIsSet(0x834) != 0) {
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_ActorSetPosition(16, 0, 0);
    Engine_ActorSetPosition(17, 0, 0);
    Engine_ActorSetPosition(18, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_ActorSetPosition(21, 0, 0);
    Engine_ActorSetPosition(22, 0, 0);
    FieldFx_StartLightning();
    (*(struct FieldBlendWork **)((u8 *)eventSlot + Ram_SceneMapStateOffset))
        ->loud = 1;
    FieldFx_LastLightning();
    Engine_TaskWait(30);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldFx_ResumeLightning();
  }
  // The returning party gets the household reunion only on its first visit.
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    if (gGameState.entrance == 6) {
      if (Engine_GameFlagIsSet(0x81d) == 0) {
        ValeInteriors_ReuniteWithFamily();
      }
    }
    Object_GetById(10)->collisionFlags |= 0x80;
  }
  // Restore actor 13's pose and map layer after the party's first departure.
  if (gGameState.entrance == 2) {
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
      Engine_ActorSetPosition(13, PIXELS(454), PIXELS(150));
      actor = Object_GetById(13);
      Engine_ActorSetSpriteFlags(actor, 0);
      Engine_ActorSetAnimation(13, 5);
      Map_ClearLayerEntryFlag(4);
    }
  }
  return 0;
}

// Open the weapon shop from the counter or speak to its owner.
static void ValeInteriors_TalkToWeaponShopkeeper(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = leader->facing;
  relativeFacing += 0xffff5fff;

  if (relativeFacing <= 0x3ffe) {
    Engine_ShopOpen(1, 21);
    return;
  }

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    Engine_EventSetMessage(MsgValeYouCameBack);
    Engine_EventAskYesNo(21, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
      Engine_EventSetMessage(MsgValeValeFeelsEmpty);
    } else {
      Engine_EventSetMessage(MsgValeGoodWeaponsDrawOutStrength);
    }
    Engine_EventShowMessage(21, 0);
  }
  Engine_EventEnd();
}

// Open the armor shop from the counter or speak to its owner.
static void ValeInteriors_TalkToArmorShopkeeper(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
    Engine_ShopOpen(2, 22);
  } else {
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x87a) != 0) {
      Engine_EventSetMessage(MsgValeTheRumorWasTrue);
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage(MsgValeImNotSadJustGo);
      } else {
        Engine_EventSetMessage(MsgValeGoodArmorDrawsOutStrength);
      }
    }
    Engine_EventShowMessage(22, 0);
    Engine_EventEnd();
  }
}

// Open the item shop or speak to its owner about the party's departure.
static void ValeInteriors_TalkToItemShopkeeper(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
    Engine_ShopOpen(3, 20);
  } else {
    if (Engine_GameFlagIsSet(0x87a) != 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeWhenDidYouComeBack);
      Engine_EventShowMessage(20, 0);
      Engine_EventEnd();
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        ValeInteriors_GiveFarewellHerb();
      } else {
        Engine_EventBegin();
        Engine_EventSetMessage(MsgValeAWiseManFleesWhen);
        Engine_EventShowMessage(20, 0);
        Engine_EventEnd();
      }
    }
  }
}

// Reset the household poses, waiting for actor 9's animation.
static inline void ValeInteriors_ResetHouseholdPoses(void) {
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
}

// Start the repeated motion before waiting for the requested animation.
static inline void ValeInteriors_PlayRepeatedAnimationAndWait(s32 actorId,
                                                              s32 animation) {
  Engine_ActorStartRepeatedMotion(actorId, 1);
  Engine_ActorSetAnimationAndWait(actorId, animation);
}

// Reunite the returning party with the household, then stage another farewell.
static void ValeInteriors_ReuniteWithFamily(void) {
  struct FieldActor *driftingSprite;
  struct FieldActor *actor;
  s32 motionFlags;
  SchedulerCallback timedUpdate;
  const s32 *partyScript;

  // Stage the returning party and start the three drifting sprites.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(IVAN, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(MIA, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_ActorSetAnimation(8, 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 508);
  driftingSprite = Actor_Get(23);
  Engine_ActorSetSpriteFlags(driftingSprite, 0);
  driftingSprite = Actor_Get(24);
  Engine_ActorSetSpriteFlags(driftingSprite, 0);
  driftingSprite = Actor_Get(25);
  Engine_ActorSetSpriteFlags(driftingSprite, 0);
  motionFlags = 0;
  Object_GetById(23)->motionFlags = motionFlags;
  Object_GetById(24)->motionFlags = motionFlags;
  Object_GetById(25)->motionFlags = motionFlags;
  timedUpdate = ValeInteriors_SpawnDriftingSprite;
  FieldScheduler_AddCallback(timedUpdate, 0xc80);
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkTo(GARET, 792, 512);
  Actor_WalkTo(IVAN, 824, 504);
  Actor_WalkToAndWait(MIA, 818, 524);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_EventWait(10);
  // Turn the party toward actor 10 while the household exchange begins.
  FieldObject_SetTargetAndCallback(
      ACTOR_PARTY_LEADER, ACTOR_LINK_KEEP_MOTION | 10, gValeFaceTargetScript);
  FieldObject_SetTargetAndCallback(GARET, ACTOR_LINK_KEEP_MOTION | 10,
                                   gValeFaceTargetScript);
  FieldObject_SetTargetAndCallback(IVAN, ACTOR_LINK_KEEP_MOTION | 10,
                                   gValeFaceTargetScript);
  FieldObject_SetTargetAndCallback(MIA, ACTOR_LINK_KEEP_MOTION | 10,
                                   gValeFaceTargetScript);
  Engine_EventWait(300);
  Camera_GetEventObject()->motionFlags = motionFlags;
  // Pan slowly into the household conversation and follow its reactions.
  Camera_SetSpeed(PIXELS(1) / 10, PIXELS(1) / 80);
  Camera_MoveTo(PIXELS(786), 0, PIXELS(430), 1);
  Engine_EventWait(240);
  Engine_ActorStop(10);
  Actor_ShowEmote(10, 0x102, 80);
  Actor_WalkToAndWait(10, 819, 405);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(10, 4);
  Engine_EventWait(40);
  Actor_FaceDirection(10, 0xd000, 20);
  Engine_EventSetMessage(MsgValeHey2);
  Event_ShowMessageAndWait(0x900a, 0, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Actor_ShowEmote(11, 0x100, 40);
  Event_ShowMessageAndWait(0x200b, 0, 20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(0x900a, 0, 10);
  Actor_FaceDirection(11, 0x5000, 10);
  Event_ShowMessageAndWait(0x200b, 0, 40);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x900a, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
  Actor_ShowEmote(11, 0x106, 40);
  Event_ShowMessageAndWait(0x200b, 0, 40);
  Engine_ActorStartRepeatedMotion(10, 2);
  Actor_ShowEmote(10, 0x102, 20);
  Engine_ActorSetAnimation(10, 4);
  Event_ShowMessageAndWait(0x900a, 0, 10);
  ValeInteriors_PlayRepeatedAnimationAndWait(11, 3);
  Engine_EventWait(20);
  ValeInteriors_PlayRepeatedAnimationAndWait(10, 4);
  ValeInteriors_PlayRepeatedAnimationAndWait(11, 3);
  ValeInteriors_PlayRepeatedAnimationAndWait(10, 4);
  Actor_ShowEmote(9, 0x105, 0);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(9, 0x1000, 40);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(9, 3);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 9, 0, 40);
  Engine_ActorSetAnimation(11, 0);
  Engine_ActorRunRepeatedMotion(11, 2);
  Event_ShowMessageAndWait(0x200b, 0, 10);
  Engine_ActorSetAnimationAndWait(9, 4);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 9, 0, 10);
  Actor_ShowEmote(10, 0x100, 20);
  Actor_FaceDirection(10, 0x5000, 40);
  Engine_ActorSetAnimationAndWait(10, 3);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Engine_ActorSetAnimationAndWait(9, 4);
  Actor_FaceDirection(9, 0xd000, 10);
  Engine_ActorJump(9, 2, 0);
  Engine_ActorSetAnimation(9, 4);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 9, 0, 10);
  Actor_ShowEmote(11, 0x101, 0);
  Actor_ShowEmote(10, 0x101, 40);
  Actor_FaceDirection(10, 0xd000, 80);
  Actor_FaceDirection(10, 0x5000, 60);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(11, 2);
  Event_ShowTwoMessagesAndWait(10, 11, 6, 6, 6, 11, 12, 1, 7, 1, 0);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Event_ShowMessageAndWait(0x1001, 0, 20);
  Engine_ActorRunRepeatedMotion(8, 2);
  // Stop the drifting sprites before the camera returns to the party.
  Engine_TaskRemoveCallback(timedUpdate);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(8, 6);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
  Camera_MoveTo(PIXELS(750), 0, PIXELS(451), 1);
  Engine_EventWait(20);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_FaceDirection(10, 0x5000, 10);
  Actor_FaceDirection(8, 0x1000, 40);
  Actor_ShowEmote(8, 0x100, 40);
  Actor_FaceDirection(8, 0x3000, 20);
  Actor_FaceDirection(8, 0x1000, 20);
  Actor_FaceDirection(8, 0x3000, 40);
  Engine_ActorSetAnimationAndWait(8, 6);
  Engine_EventWait(60);
  Engine_ActorJump(8, 6, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(GARET, 789, 473);
  Actor_FaceDirection(GARET, 0x7000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 1, 0, 10);
  Actor_FaceDirection(8, 0x1000, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(9, 0x1000, 0);
  Actor_FaceDirection(GARET, 0x3000, 0);
  Actor_FaceDirection(IVAN, 0x7000, 0);
  Actor_FaceDirection(MIA, 0xb000, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Camera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Event_ShowMessage(10, 0);
  Engine_ActorSetAnimationAndWait(11, 4);
  Engine_EventWait(20);
  // Ask about leaving again and give the companions their individual replies.
  Engine_EventSetMessage(MsgValeYoureLeavingAgainSoon);
  Event_ShowMessage(0x200b, 0);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0xd000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(9, 4);
  Actor_FaceDirection(9, 0xd000, 10);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 9, 0);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
  Actor_FaceDirection(GARET, 0x7000, 10);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Actor_FaceDirection(9, 0x1000, 10);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0x3000, 20);
  Actor_ShowEmote(GARET, 0x102, 80);
  Actor_FaceDirection(GARET, 0x7000, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 1, 0, 20);
  Actor_FaceDirection(GARET, 0x3000, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0x1000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
  // Bring Ivan and Mia forward for their goodbyes.
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(IVAN, 819, 489);
  Actor_FaceDirection(IVAN, 0xb000, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  ValeInteriors_ResetHouseholdPoses();
  Object_GetById(3)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(MIA, 794, 520);
  Actor_FaceDirection(GARET, 0x5000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
  Actor_WalkToAndWait(MIA, 784, 496);
  Actor_FaceDirection(MIA, 0x9000, 10);
  Object_GetById(3)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageAndWait(MIA, 0, 20);
  ValeInteriors_ResetHouseholdPoses();
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Engine_EventWait(20);
  Actor_SetSpeed(11, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(11, 835, 388);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_ShowEmote(11, 0x108, 40);
  Event_ShowMessageAndWait(0x200b, 0, 20);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_EventWait(40);
  Actor_FaceDirection(IVAN, 0x7000, 0);
  Actor_FaceDirection(MIA, 0xf000, 40);
  Actor_FaceDirection(IVAN, 0x9000, 0);
  Actor_FaceDirection(MIA, 0xd000, 20);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(10, 3);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xd000, 0);
  Actor_FaceDirection(IVAN, 0xb000, 0);
  Actor_FaceDirection(MIA, 0xd000, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  // Send the companions out with the shared departure script.
  partyScript = gValePartyDepartureScript;
  Actor_SetActionScript(GARET, partyScript);
  Actor_SetActionScript(2, partyScript);
  Actor_RunActionScriptAndWait(3, partyScript);
  Actor_SetActionScript(10, gValeHouseholdPacingScript);
  Actor_WalkToAndWait(11, 837, 376);
  Actor_FaceDirection(11, 0xd000, 20);
  GameFlag_Set(0x81d);
  Engine_EventEnd();
}

// Restart one drifting sprite at frames 10, 20, and 30 of each 180-frame cycle.
static void ValeInteriors_SpawnDriftingSprite(void) {
  s32 actorId;
  u32 cycleFrame;
  struct FieldActor *actor;
  struct FieldActor *origin;

  cycleFrame = gFrameCount % 180;
  actorId = 23;
  switch (cycleFrame) {
  case 10:
    break;
  case 20:
    actorId = 24;
    break;
  case 30:
    actorId = 25;
    break;
  default:
    return;
  }
  actor = Object_GetById(actorId);
  if (actor == NULL) {
    return;
  }
  origin = Object_GetById(8);
  if (origin != NULL) {
    Engine_ActorSetPosition(actorId, origin->x.fixed, origin->z.fixed);
  }
  // Restart this sprite above the origin with a small scale and 8.8 motion.
  actor->scaleX = 2 * Q16_ONE / 5;
  actor->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = actor->y.fixed + PIXELS(24);

    actor->y.fixed = height;
    actor->targetY = height;
    // The drifting script consumes these two work fields as signed 8.8 steps.
    actor->timer = 25; // 25/256 pixel per frame.
    actor->work = 128; // 1/2 pixel per frame.
  }
  Actor_SetActionScript(actorId, gValeDriftingSpriteScript);
}

s32 gValeFaceTargetScript[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeHouseholdPacingScript[] = {
    SC_SET(SW_SPEED, PIXELS(3) / 5),
    SC_SET(SW_ACCEL, PIXELS(3) / 10),
    SC_ARG(2),
    SC_MOVE(PIXELS(784), 0, PIXELS(405)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(40),
    SC_LABEL(1),
    SC_ADD(SW_FACING, -1024),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_WAIT(40),
    SC_LABEL(3),
    SC_ADD(SW_FACING, -1024),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(3)),
    SC_ARG(2),
    SC_MOVE(PIXELS(819), 0, PIXELS(405)),
    SC_SYNC,
    SC_ARG(1),
    SC_LABEL(2),
    SC_ADD(SW_FACING, -2048),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(2)),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValePartyDepartureScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(808), 0, PIXELS(508)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

s32 gValeDriftingSpriteScript[] = {
    SC_CALL(ValeInteriors_AdvanceDriftingSprite),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gValeHouseEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 96, 0, 222, FACING_NORTH, 0, 4, 4, 244, 292, 0},
    {2, CONDITION_ALWAYS, 337, 0, 199, FACING_NORTH, 0, 252, 4, 548, 236, 0},
    {3, CONDITION_ALWAYS, 656, 0, 244, FACING_NORTH, 0, 556, 4, 900, 276, 0},
    {4, CONDITION_ALWAYS, 159, 0, 525, FACING_NORTH, 0, 4, 324, 332, 564, 0},
    {5, CONDITION_ALWAYS, 528, 0, 486, FACING_NORTH, 0, 348, 284, 612, 516, 0},
    {6, CONDITION_ALWAYS, 816, 0, 522, FACING_NORTH, 0, 628, 284, 884, 548, 0},
    {7, CONDITION_ALWAYS, 463, 0, 851, FACING_NORTH, 0, 324, 604, 588, 884, 0},
    {8, CONDITION_ALWAYS, 769, 0, 857, FACING_NORTH, 0, 580, 628, 852, 884, 0},
    {SCENE_TABLE_END},
};

u32 gValeHouseExits[] = {
    SCENE_EXITS(7),      SCENE_EXIT(1, 4, 10), SCENE_EXIT(2, 4, 9),
    SCENE_EXIT(3, 5, 3), SCENE_EXIT(4, 5, 5),  SCENE_EXIT(5, 4, 7),
    SCENE_EXIT(6, 4, 6), SCENE_EXIT(7, 5, 4),  SCENE_EXIT(8, 5, 1),
    SCENE_EXITS_END,
};

u32 gValeHouseLateExits[] = {
    SCENE_EXITS(7),      SCENE_EXIT(1, 3, 10), SCENE_EXIT(2, 3, 9),
    SCENE_EXIT(3, 5, 3), SCENE_EXIT(4, 5, 5),  SCENE_EXIT(5, 3, 7),
    SCENE_EXIT(6, 3, 6), SCENE_EXIT(7, 5, 4),  SCENE_EXIT(8, 5, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeHousePlacements[] = {
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x36, 0x0, ACTOR_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x26, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(331), 0, PIXELS(144), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(486), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHousePlacementsAfterLeaving[] = {
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x36, 0x0, ACTOR_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x26, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(486), 0, PIXELS(118), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 3, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHouseReturnPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x36, 0x0, (s32)gValeHouseholdPacingScript,
        PIXELS(819), 0, PIXELS(405), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x26, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(370), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(494), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeHouseLateEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit8) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { SEARCH(SEARCH_BARREL), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { SEARCH(SEARCH_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 103, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 104, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseEvents[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_AskAboutKradensStudies) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_AskToScareVisitors) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToItemShopkeeper) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit8) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseEventsAfterLeaving[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_AskAboutCulpritsPowers) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToItemShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgValeIsaacYoureGoingOnADifficult) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgValeIfYouFaceAnyHardshipIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgValeSoIfThatWasFelixWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeItsHardLettingMyPreciousGaret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgValeTheWiseOneChoseGaretHimself) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgValeOnceGaretLeavesIWontBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeIAmConcernedForGaretBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeFelixInheritedHisFathersSkillWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeJennaAnytimeYouFaceDangerJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeWhyWouldFelixTakeHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeWeAlmostLostTheTownOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeIdLikeToBelieveThatOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeIfOnlyIdGoneToSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeValesLawsStateThatYouCannot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeIWishIWereTheOne) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit8) },
    { SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseReturnEvents[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_AskAboutDora) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_DiscussTravelersHealth) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_TalkToItemShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeItMustBeNiceToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgValeHisNewFriendsAreAllSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgValeItWasntTooSmartToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeGaretSeemsToHaveGrownUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgValeWeveSeenManyMonstersLatelyAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgValeIWishICouldLearnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x81b, MSGNO(MsgValeWhatHappenedToJennaHaventYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgValeIGaveHimAFarewellGift) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeWillIEverSeeJennaAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeDoraSaysItsJustACold) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIKnowThatThereAreAlways) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeEvenIfTheEruptionHadNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeEveryTimeTheGroundShakesI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeIfThePsynergyStonesInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheLavaLookedLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeTheHealersAreSupposedToGuard) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeInteriors_LeaveThroughExit8) },
    { SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SCENE_EVENTS_END },
};
// clang-format on
