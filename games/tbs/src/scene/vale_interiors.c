/// @file
/// The houses and shops of Vale. Garet's family lives in the house at entrance
/// 6: his grandfather, his grandmother, and his parents ("Mom, Dad... I'm
/// home...").

#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/vale_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"
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
#include "flags.h"
#include "songs.h"

enum ValeInteriorsActor {
  /// Garet's grandfather. "Isaac, please take care of my grandson, Garet."
  ACTOR_GARETS_GRANDFATHER = 8,
  /// Garet's grandmother. "Grandma! What's the matter? Are you choking?"
  ACTOR_GARETS_GRANDMOTHER = 9,
  /// Garet's father.
  /// "I'm proud of you, Garet. My own son...on such an important quest!"
  ACTOR_GARETS_FATHER = 10,
  /// Garet's mother. "As mothers, Dora and I will always worry about you."
  ACTOR_GARETS_MOTHER = 11,
  ACTOR_ITEM_SHOPKEEPER = 20,
  ACTOR_WEAPON_SHOPKEEPER = 21,
  ACTOR_ARMOR_SHOPKEEPER = 22,
  /// Three sprites drifting up from Garet's sleeping grandfather.
  ACTOR_FIRST_DRIFTING_SPRITE = 23
};

// Forward declarations.
static void Event_GaretComesHome(void);
static void ValeInteriors_SpawnDriftingSprite(void);

#include "common/object_facing.h"

/// Moves and grows the sprite, using timer/work as signed 8.8 pixel steps.
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

struct SceneEntrance *ValeInteriors_GetEntrances(void) {
  return gValeInteriors_Entrances;
}

struct SceneRegion *ValeInteriors_GetRegions(void) { return NULL; }

/// On the stormy night, the doors lead out into the storm.
u32 *ValeInteriors_GetExits(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0)
    return gValeInteriors_StormExits;
  return gValeInteriors_Exits;
}

/// Prepares placements for the first departure or the later return to Vale.
struct ScenePlacement *ValeInteriors_GetPlacements(void) {
  struct ScenePlacement *table;

  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
    table = gValeInteriors_PlacementsAfterKalay;
  } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE)) {
    table = gValeInteriors_PlacementsAfterLeaving;
  } else {
    table = gValeInteriors_Placements;
  }
  SceneEvents_UpdateInView(table);
  return table;
}

/// Selects dialogue and triggers for the current visit to Vale.
struct SceneEvent *ValeInteriors_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0)
    return gValeInteriors_StormEvents;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    return gValeInteriors_EventsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gValeInteriors_EventsAfterLeaving;
  return gValeInteriors_Events;
}

/// A villager asks whether Isaac knew why Kraden came to Vale. He came to
/// study Mt. Aleph.
static void Event_AskAboutKradensStudies(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeKradenCameToStudyMt);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

/// A villager asks Isaac to help scare Kraden's visitors.
static void Event_AskToScareVisitors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeLetsScareKradensVisitors);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
  Engine_EventAskYesNo(19, 0);
  Engine_EventEnd();
}

/// After the party has left Vale, the item shopkeeper gives Isaac a Herb as a
/// farewell gift, and afterwards urges him to save Jenna.
static void ValeInteriors_GiveFarewellHerb(void) {
  s32 gift;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_VALE_FAREWELL_HERB_RECEIVED) != 0) {
    Engine_EventSetMessage(MsgValeYouMustSaveJenna);
    Event_ShowMessage(ACTOR_ITEM_SHOPKEEPER, 0);
    FieldObject_SetTargetAndCallback(
        ACTOR_ITEM_SHOPKEEPER, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
        gValeInteriors_FaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    Engine_EventSetMessage(gift);
    Event_ShowMessageAndWait(ACTOR_ITEM_SHOPKEEPER, 0, 20);
    Engine_MessageShowCentered((gift + 1), 1);
    Engine_PartyGiveItem(ITEM_HERB, 0);
    GameFlag_Set(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  }
  Engine_EventEnd();
}

/// A villager tells of the culprits' strange powers and asks about them.
static void Event_AskAboutCulpritsPowers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeTheCulpritsHadStrangePowers);
  Engine_EventAskYesNo(16, 0);
  Engine_EventEnd();
}

/// Back from Kalay, Garet's father asks whether Isaac heard them talking about
/// Dora.
static void Event_AskAboutDora(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDidYouHearAboutDora);
  Engine_EventAskYesNo(ACTOR_GARETS_FATHER, 0);
  Engine_EventEnd();
}

/// Garet's mother hopes the travelers did not get sick on the way.
static void Event_WorryAboutTravelersHealth(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeHopeDidntGet);
  Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARETS_MOTHER, 0);
  Engine_EventEnd();
}

/// Stops any lightning effect, closes the window transition, and requests the
/// exit.
static void ValeInteriors_RequestWindowExit(s32 exitId) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0)
    FieldFx_StopLightning();
  Engine_AudioPlayCue(SE_MAP_EXIT);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(exitId);
}

static void Event_LeaveByDoor1(void) { ValeInteriors_RequestWindowExit(1); }

static void Event_LeaveByDoor2(void) { ValeInteriors_RequestWindowExit(2); }

static void Event_LeaveByDoor3(void) { ValeInteriors_RequestWindowExit(3); }

static void Event_LeaveByDoor4(void) { ValeInteriors_RequestWindowExit(4); }

static void Event_LeaveByDoor5(void) { ValeInteriors_RequestWindowExit(5); }

static void Event_LeaveByDoor6(void) { ValeInteriors_RequestWindowExit(6); }

static void Event_LeaveByDoor7(void) { ValeInteriors_RequestWindowExit(7); }

static void Event_LeaveByDoor8(void) { ValeInteriors_RequestWindowExit(8); }

/// Arriving in a house: on the stormy night the residents are gone and
/// lightning flashes outside; back from Kalay, entering Garet's house the first
/// time plays his homecoming.
s32 ValeInteriors_Initialize(void) {
  struct FieldActor *actor;
  struct EventWork **eventSlot = &gEventWork;

  (*eventSlot)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  // The late visit hides residents while lightning plays outside the house.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
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
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    if (gGameState.entrance == 6) {
      if (Engine_GameFlagIsSet(FLAG_GARET_CAME_HOME) == 0) {
        Event_GaretComesHome();
      }
    }
    Object_GetById(ACTOR_GARETS_FATHER)->collisionFlags |= 0x80;
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

/// Opens the weapon shop from the counter or speaks to its owner.
static void Event_TalkToWeaponShopkeeper(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (FACING_IS_NORTH(leader->facing)) {
    Engine_ShopOpen(1, ACTOR_WEAPON_SHOPKEEPER);
    return;
  }

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    Engine_EventSetMessage(MsgValeYouCameBack);
    Engine_EventAskYesNo(ACTOR_WEAPON_SHOPKEEPER, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
      Engine_EventSetMessage(MsgValeValeFeelsEmpty);
    } else {
      Engine_EventSetMessage(MsgValeGoodWeaponsDrawOutStrength);
    }
    Engine_EventShowMessage(ACTOR_WEAPON_SHOPKEEPER, 0);
  }
  Engine_EventEnd();
}

/// Opens the armor shop from the counter or speaks to its owner.
static void Event_TalkToArmorShopkeeper(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Engine_ShopOpen(2, ACTOR_ARMOR_SHOPKEEPER);
  } else {
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
      Engine_EventSetMessage(MsgValeTheRumorWasTrue);
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage(MsgValeImNotSadJustGo);
      } else {
        Engine_EventSetMessage(MsgValeGoodArmorDrawsOutStrength);
      }
    }
    Engine_EventShowMessage(ACTOR_ARMOR_SHOPKEEPER, 0);
    Engine_EventEnd();
  }
}

/// Opens the item shop or speaks to its owner about the party's departure.
static void Event_TalkToItemShopkeeper(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Engine_ShopOpen(3, ACTOR_ITEM_SHOPKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeWhenDidYouComeBack);
      Engine_EventShowMessage(ACTOR_ITEM_SHOPKEEPER, 0);
      Engine_EventEnd();
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        ValeInteriors_GiveFarewellHerb();
      } else {
        Engine_EventBegin();
        Engine_EventSetMessage(MsgValeAWiseManFleesWhen);
        Engine_EventShowMessage(ACTOR_ITEM_SHOPKEEPER, 0);
        Engine_EventEnd();
      }
    }
  }
}

/// Garet's grandparents and father nod.
static inline void ValeInteriors_FamilyNods(void) {
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
}

/// Starts the repeated motion before waiting for the requested animation.
static inline void ValeInteriors_PlayRepeatedAnimationAndWait(s32 actorId,
                                                              s32 animation) {
  Engine_ActorStartRepeatedMotion(actorId, 1);
  Engine_ActorSetAnimationAndWait(actorId, animation);
}

/// Back from Kalay, Garet comes home: his grandfather dozes, his parents argue
/// over whether to look for him, and his grandmother spots him first. Garet
/// introduces Ivan and Mia, the family sees them off, and his father paces.
static void Event_GaretComesHome(void) {
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
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 508);
  driftingSprite = Actor_Get(ACTOR_FIRST_DRIFTING_SPRITE);
  Engine_ActorSetSpriteFlags(driftingSprite, 0);
  driftingSprite = Actor_Get(ACTOR_FIRST_DRIFTING_SPRITE + 1);
  Engine_ActorSetSpriteFlags(driftingSprite, 0);
  driftingSprite = Actor_Get(ACTOR_FIRST_DRIFTING_SPRITE + 2);
  Engine_ActorSetSpriteFlags(driftingSprite, 0);
  motionFlags = 0;
  Object_GetById(ACTOR_FIRST_DRIFTING_SPRITE)->motionFlags = motionFlags;
  Object_GetById(ACTOR_FIRST_DRIFTING_SPRITE + 1)->motionFlags = motionFlags;
  Object_GetById(ACTOR_FIRST_DRIFTING_SPRITE + 2)->motionFlags = motionFlags;
  timedUpdate = ValeInteriors_SpawnDriftingSprite;
  FieldScheduler_AddCallback(timedUpdate, 0xc80);
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
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
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_EventWait(10);
  // Turn the party toward actor 10 while the household exchange begins.
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriors_FaceTargetScript);
  FieldObject_SetTargetAndCallback(GARET,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriors_FaceTargetScript);
  FieldObject_SetTargetAndCallback(IVAN,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriors_FaceTargetScript);
  FieldObject_SetTargetAndCallback(MIA,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriors_FaceTargetScript);
  Engine_EventWait(300);
  Camera_GetEventObject()->motionFlags = motionFlags;
  // Pan slowly into the household conversation and follow its reactions.
  Camera_SetSpeed(PIXELS(1) / 10, PIXELS(1) / 80);
  Camera_MoveTo(PIXELS(786), 0, PIXELS(430), 1);
  Engine_EventWait(240);
  Engine_ActorStop(10);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | 2, 80);
  Actor_WalkToAndWait(ACTOR_GARETS_FATHER, 819, 405);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH + FACING_STEP, 20);
  Engine_EventSetMessage(MsgValeHey2);
  Event_ShowMessageAndWait(0x9000 | ACTOR_GARETS_FATHER, 0, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | 0, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(0x9000 | ACTOR_GARETS_FATHER, 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x9000 | ACTOR_GARETS_FATHER, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | 6, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | 2, 20);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(0x9000 | ACTOR_GARETS_FATHER, 0, 10);
  ValeInteriors_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
  Engine_EventWait(20);
  ValeInteriors_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                             ANIM_SHAKE_HEAD);
  ValeInteriors_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
  ValeInteriors_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                             ANIM_SHAKE_HEAD);
  Actor_ShowEmote(ACTOR_GARETS_GRANDMOTHER, EMOTE_IN_FRONT | 5, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 3);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0,
                           40);
  Engine_ActorSetAnimation(ACTOR_GARETS_MOTHER, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0,
                           10);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | 0, 20);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_FATHER, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorJump(ACTOR_GARETS_GRANDMOTHER, 2, 0);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0,
                           10);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  Event_ShowTwoMessagesAndWait(ACTOR_GARETS_FATHER, 11, 6, 6, 6,
                               ACTOR_GARETS_MOTHER, 12, 1, 7, 1, 0);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  // Stop the drifting sprites before the camera returns to the party.
  Engine_TaskRemoveCallback(timedUpdate);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 6);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0,
                           20);
  Camera_MoveTo(PIXELS(750), 0, PIXELS(451), 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 40);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | 0, 40);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 6);
  Engine_EventWait(60);
  Engine_ActorJump(ACTOR_GARETS_GRANDFATHER, 6, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0,
                           20);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(GARET, 789, 473);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST + FACING_STEP, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Camera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_ShowMessage(ACTOR_GARETS_FATHER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  // Ask about leaving again and give the companions their individual replies.
  Engine_EventSetMessage(MsgValeYoureLeavingAgainSoon);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH + FACING_STEP, 10);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_GARETS_MOTHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Bring Ivan and Mia forward for their goodbyes.
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(IVAN, 819, 489);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  ValeInteriors_FamilyNods();
  Object_GetById(MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(MIA, 794, 520);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(MIA, 784, 496);
  Actor_FaceDirection(MIA, FACING_WEST + FACING_STEP, 10);
  Object_GetById(MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageAndWait(MIA, 0, 20);
  ValeInteriors_FamilyNods();
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_GARETS_MOTHER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 835, 388);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 20);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_EventWait(40);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(IVAN, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_GARETS_FATHER, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  // Send the companions out with the shared departure script.
  partyScript = gValeInteriors_PartyLeavesScript;
  Actor_SetActionScript(GARET, partyScript);
  Actor_SetActionScript(IVAN, partyScript);
  Actor_RunActionScriptAndWait(MIA, partyScript);
  Actor_SetActionScript(ACTOR_GARETS_FATHER, gValeInteriors_FatherPacingScript);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 837, 376);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_NORTH + FACING_STEP, 20);
  GameFlag_Set(FLAG_GARET_CAME_HOME);
  Engine_EventEnd();
}

/// Restarts one drifting sprite at frames 10, 20, and 30 of each 180-frame
/// cycle.
static void ValeInteriors_SpawnDriftingSprite(void) {
  s32 actorId;
  u32 cycleFrame;
  struct FieldActor *actor;
  struct FieldActor *origin;

  cycleFrame = gFrameCount % 180;
  actorId = ACTOR_FIRST_DRIFTING_SPRITE;
  switch (cycleFrame) {
  case 10:
    break;
  case 20:
    actorId = ACTOR_FIRST_DRIFTING_SPRITE + 1;
    break;
  case 30:
    actorId = ACTOR_FIRST_DRIFTING_SPRITE + 2;
    break;
  default:
    return;
  }
  actor = Object_GetById(actorId);
  if (actor == NULL) {
    return;
  }
  origin = Object_GetById(ACTOR_GARETS_GRANDFATHER);
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
  Actor_SetActionScript(actorId, gValeInteriors_DriftingSpriteScript);
}

s32 gValeInteriors_FaceTargetScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeInteriors_FatherPacingScript[] = {
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

s32 gValeInteriors_PartyLeavesScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(808), 0, PIXELS(508)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

s32 gValeInteriors_DriftingSpriteScript[] = {
    SC_CALL(ValeInteriors_AdvanceDriftingSprite),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gValeInteriors_Entrances[] = {
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

u32 gValeInteriors_Exits[] = {
    SCENE_EXITS(7),      SCENE_EXIT(1, 4, 10), SCENE_EXIT(2, 4, 9),
    SCENE_EXIT(3, 5, 3), SCENE_EXIT(4, 5, 5),  SCENE_EXIT(5, 4, 7),
    SCENE_EXIT(6, 4, 6), SCENE_EXIT(7, 5, 4),  SCENE_EXIT(8, 5, 1),
    SCENE_EXITS_END,
};

u32 gValeInteriors_StormExits[] = {
    SCENE_EXITS(7),      SCENE_EXIT(1, 3, 10), SCENE_EXIT(2, 3, 9),
    SCENE_EXIT(3, 5, 3), SCENE_EXIT(4, 5, 5),  SCENE_EXIT(5, 3, 7),
    SCENE_EXIT(6, 3, 6), SCENE_EXIT(7, 5, 4),  SCENE_EXIT(8, 5, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeInteriors_Placements[] = {
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

struct ScenePlacement gValeInteriors_PlacementsAfterLeaving[] = {
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

struct ScenePlacement gValeInteriors_PlacementsAfterKalay[] = {
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
    { 0x36, 0x0, (s32)gValeInteriors_FatherPacingScript,
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

struct SceneEvent gValeInteriors_StormEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor8) },
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

struct SceneEvent gValeInteriors_Events[] = {
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
    { EVENT_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
    { EVENT_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
    { EVENT_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutKradensStudies) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskToScareVisitors) },
    { EVENT_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponShopkeeper) },
    { EVENT_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorShopkeeper) },
    { EVENT_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemShopkeeper) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor8) },
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

struct SceneEvent gValeInteriors_EventsAfterLeaving[] = {
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
    { EVENT_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
    { EVENT_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
    { EVENT_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutCulpritsPowers) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
    { EVENT_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponShopkeeper) },
    { EVENT_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorShopkeeper) },
    { EVENT_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacYoureGoingOnADifficult) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeIfYouFaceAnyHardshipIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeSoIfThatWasFelixWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItsHardLettingMyPreciousGaret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeTheWiseOneChoseGaretHimself) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeOnceGaretLeavesIWontBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIAmConcernedForGaretBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeFelixInheritedHisFathersSkillWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeJennaAnytimeYouFaceDangerJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeWhyWouldFelixTakeHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeWeAlmostLostTheTownOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeIdLikeToBelieveThatOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeIfOnlyIdGoneToSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeValesLawsStateThatYouCannot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeIWishIWereTheOne) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor8) },
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

struct SceneEvent gValeInteriors_EventsAfterKalay[] = {
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
    { EVENT_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
    { EVENT_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutDora) },
    { EVENT_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WorryAboutTravelersHealth) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
    { EVENT_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponShopkeeper) },
    { EVENT_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorShopkeeper) },
    { EVENT_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItMustBeNiceToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeHisNewFriendsAreAllSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeItWasntTooSmartToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretSeemsToHaveGrownUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeWeveSeenManyMonstersLatelyAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeIWishICouldLearnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER, FLAG_VALE_FAREWELL_HERB_RECEIVED, MSGNO(MsgValeWhatHappenedToJennaHaventYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeIGaveHimAFarewellGift) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeWillIEverSeeJennaAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeDoraSaysItsJustACold) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIKnowThatThereAreAlways) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeEvenIfTheEruptionHadNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeEveryTimeTheGroundShakesI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeIfThePsynergyStonesInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheLavaLookedLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeTheHealersAreSupposedToGuard) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor1) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor2) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor5) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor6) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor7) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByDoor8) },
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
