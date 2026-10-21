/// @file
/// The buildings of Tolbi (TolbiInteriors, scene 136): its houses, inns and
/// shops, which the area names call Tolbi, and Tolbi Sanctum, as the same
/// scene is named at TOLBI_INTERIORS_SANCTUM and TOLBI_INTERIORS_SANCTUM_NORTH.
///
/// - Tolbi, the rooms behind the town's doors, which Tolbi's exits
///   TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_HOUSE_SOUTHWEST lead into; this scene's
///   exits TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI to
///   TOLBI_INTERIORS_TO_TOLBI_WEST lead back out. During the festival one
///   household lines up beds and becomes a makeshift inn, visitors have
///   rented another house, and the weapon, armor and item shops
///   (TOLBI_INTERIORS_WEAPON_SHOP, TOLBI_INTERIORS_ARMOR_SHOP and
///   TOLBI_INTERIORS_ITEM_SHOP) open across their counters. The inn behind
///   TOLBI_TO_INN is full, `Inn Full / No Vacancies`; its exits 10 and
///   11 join it to the part of the inn whose doors and exits (12 to 19) lead
///   into four guest rooms.
/// - Tolbi Sanctum, behind TOLBI_TO_SANCTUM (TOLBI_INTERIORS_SANCTUM), whose
///   priest opens the sanctum across the counter. TOLBI_INTERIORS_SANCTUM_NORTH
///   is where a defeat returns the party while Iodem waits at Gondowan Passage.
///
/// The rooms' talk follows Colosso in the same three stages as the town, each
/// with its own event table (TolbiInteriors_SelectEvents) and, after
/// Colosso, a placement table of its own. During the festival Babi is
/// missing and the town is full of visitors, among them a tour group from
/// Kalay that reserved only two rooms. On finals day (FLAG_BABI_SAVED) Babi
/// has shown himself at the finals; the Spanish and Italian editions add a
/// question about it (TBS_PATCH_4). After Colosso (FLAG_COLOSSO_OVER) no ship
/// sails the Karagol, and the visitors are stranded; one of them says they
/// made it to Tolbi with the party's help.
///
/// After Colosso two short scripted scenes play, each once, on entering their
/// rooms (TolbiInteriors_ApplyEntryState). In one guest room (entrance 19) a
/// woman finds something on the floor, starts at the sight of the party and
/// hides it behind the trash pile (TolbiInteriors_WatchWomanHideMedal,
/// FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN): a Lucky Medal, which the party can then
/// take from that barrel (FLAG_TOLBI_INTERIORS_MEDAL_TAKEN), after which she
/// greets them, startled, and Mind Read hears her hope its owner will not ask
/// for it back. In another (entrance 16) stranded passengers argue that the
/// captain will not sail although they agreed to do whatever it takes
/// (TolbiInteriors_WatchPassengersArgue, a place flag). A man who rode the ship
/// from Kalay with the party greets it as `the would-be warrior`, and he and
/// TOLBI_INTERIORS_COLOSSO_FAN leave once the party has watched the passengers
/// confront the tour guide on the Tolbi docks
/// (FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN). Barrels hold a Vial and a Lucky
/// Medal, and searching the ovens shows what is cooking.

#include "game_state.h"
#include "unit/scene/tolbi_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "tolbi_interiors.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
/// The stranded passengers' argument has played.
#define FLAG_TOLBI_INTERIORS_PASSENGERS_SEEN 0x300

#if !TBS_PATCH_1
#define TOLBI_INTERIORS_INN_CAMERA_MAX_Z 736
#define TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X 944
#define TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z 928
#else
#define TOLBI_INTERIORS_INN_CAMERA_MAX_Z 744
#define TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X 952
#define TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z 936
#endif

/// The shop rows and door triggers used by the room event tables below.
enum {
  TOLBI_INTERIORS_WEAPON_SHOP = 26,
  TOLBI_INTERIORS_ARMOR_SHOP = 27,
  TOLBI_INTERIORS_ITEM_SHOP = 28,
  TOLBI_INTERIORS_FIRST_DOOR_TRIGGER = 14,
  TOLBI_INTERIORS_ACTOR_SLOT_END = 66
};

/// Asks a yes/no question, then answers with the message after the question
/// for yes or the one after that for no.
static inline void TolbiInteriors_AskYesNo(s32 question, s32 speaker) {
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(speaker, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(speaker, 0);
}

const struct SceneEntrance *TolbiInteriors_GetEntrances(void) {
  return gTolbiInteriorsEntrances;
}

const struct SceneRegion *TolbiInteriors_GetRegions(void) { return 0; }

const u32 *TolbiInteriors_GetExits(void) { return gTolbiInteriorsExits; }

/// After Colosso the rooms hold a different crowd.
const struct ScenePlacement *TolbiInteriors_SelectPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gTolbiInteriorsPlacementsAfterColosso;
  }
  return gTolbiInteriorsPlacements;
}

#if TBS_PATCH_4
/// In Tolbi on finals day, patch 4 (the Spanish and Italian editions) adds a
/// question: was Babi missing at all, since he showed himself at the finals?
/// Its lines are in every edition's text, Japanese included, so it is a
/// later change to the script, not a translation's.
static void TolbiInteriors_AskWhetherBabiWasMissing(s32 speaker) {
  TolbiInteriors_AskYesNo(MsgTolbiInteriorsBabiShowedHimselfAtFinals, speaker);
}
#endif

/// Picks the event table for the current point in the Colosso story.
const struct SceneEvent *TolbiInteriors_SelectEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gTolbiInteriorsEventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    return gTolbiInteriorsEventsFinalsDay;
  }
  return gTolbiInteriorsEventsFestival;
}

/// Unused: leaves through TRANSITION_WINDOW, a screen transition of 24
/// frames; no event row names it.
UNUSED static void TolbiInteriors_ExitThroughWindow(void) {
  struct EventWork *work = gEventWork;

  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  work->transitionFrames = 24;
  Engine_EventRequestExit();
}

/// In Tolbi, walking into one of the four doors at triggers 14 to 17: every
/// placed actor stops, the door opens and the leader walks through it.
static void TolbiInteriors_EnterDoor(void) {
  u32 actorId;
  struct FieldActor *actor;
  struct EventWork *work;
  s16 *trigger;
  s32 doorIndex;

  work = gEventWork;
  // Stop the room's actors before animating the selected doorway.
  for (actorId = ACTOR_FIRST_PLACED; actorId < TOLBI_INTERIORS_ACTOR_SLOT_END;
       actorId++) {
    actor = Object_GetById(actorId);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }
  trigger = &work->touchedTrigger;
  doorIndex = *trigger - TOLBI_INTERIORS_FIRST_DOOR_TRIGGER;
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gTolbiInteriorsDoors[doorIndex].steps,
                   gTolbiInteriorsDoors[doorIndex].x,
                   gTolbiInteriorsDoors[doorIndex].y);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_EventRequestExit(*trigger);
}

/// In Tolbi after Colosso, entering the guest room of entrance 19: a woman
/// finds something on the floor, is startled to see the party and hides it in
/// the barrel behind the trash pile. "Hey, what's this?" The thing is a Lucky
/// Medal, found later in that barrel.
static void TolbiInteriors_WatchWomanHideMedal(void) {
  struct FieldActor *sparkle;

  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiHeyWhatsThis);

  // A sparkle on the floor where she finds it.
  Engine_EventWait(40);
  sparkle = Engine_ObjectCreate(0x11c, PIXELS(600), 0, PIXELS(824));
  Engine_ActorSetSpriteFlags(sparkle, 0);
  Object_SetMode(sparkle, 6);
  Engine_EventWait(10);
  Object_SetMode(sparkle, 1);
  Engine_EventWait(40);
  Engine_ObjectDispatchRelease(sparkle);
  Engine_EventWait(2);

  // She runs to it and picks it up.
  Actor_ShowEmote(TOLBI_INTERIORS_MEDAL_WOMAN, EMOTE_IN_FRONT, 50);
  Actor_SetSpeed(TOLBI_INTERIORS_MEDAL_WOMAN, 0x10000, 0x8000);
  Actor_WalkToAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 600, 848);
  Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, FACING_NORTH, 0);
  Engine_EventWait(40);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_ActorRunRepeatedMotion(TOLBI_INTERIORS_MEDAL_WOMAN, 2);
  Engine_EventWait(30);
  Actor_WalkToAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 568, 848);
  Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, FACING_NORTH, 0);
  Engine_EventWait(30);

  // She notices the party.
  Actor_ShowEmote(TOLBI_INTERIORS_MEDAL_WOMAN, EMOTE_IN_FRONT | 8, 50);
  Engine_EventWait(20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_EventWait(20);
  Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, 0x3000, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(TOLBI_INTERIORS_MEDAL_WOMAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 50);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(TOLBI_INTERIORS_MEDAL_WOMAN, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);

  // She hides it and comes back, then leaves.
  Actor_SetSpeed(TOLBI_INTERIORS_MEDAL_WOMAN, 0x16666, 0xb333);
  Actor_WalkByAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 16, 0);
  Actor_WalkByAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 0, 32);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(TOLBI_INTERIORS_MEDAL_WOMAN, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 0, 48);
  Actor_SetPosition(TOLBI_INTERIORS_MEDAL_WOMAN, 0, 0);
  Engine_EventEnd();
}

/// In Tolbi after Colosso, entering the guest room of entrance 16: three
/// stranded passengers argue about the captain, who won't sail the ship. "He
/// won't sail the ship, even though we agreed to do whatever it takes?"
static void TolbiInteriors_WatchPassengersArgue(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiHeWontSailShipEven);
  Engine_EventWait(30);
  Engine_ActorJump(TOLBI_INTERIORS_ANGRY_PASSENGER, 4, 13);
  Engine_ActorJump(TOLBI_INTERIORS_ANGRY_PASSENGER, 4, 30);
  Engine_EventShowMessage(TOLBI_INTERIORS_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);

  Engine_ActorShowEmote(TOLBI_INTERIORS_PASSENGER_WIFE, EMOTE_IN_FRONT | 2, 50);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_PASSENGER_WIFE, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(TOLBI_INTERIORS_PASSENGER_WIFE, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_THIRD_PASSENGER,
                                  ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(TOLBI_INTERIORS_THIRD_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(TOLBI_INTERIORS_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(TOLBI_INTERIORS_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_PASSENGER_WIFE, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventEnd();
}

/// Sets up the rooms on entry. After Colosso: a row of three map cells takes
/// new attributes, two of the passengers are drawn at a fixed priority, the
/// medal woman stands where she hid the medal, the two short scenes play once
/// each on their entrances, and the passenger who rode the ship with the party
/// and TOLBI_INTERIORS_COLOSSO_FAN have left once the party has seen the tour
/// guide on the Tolbi docks tell a passenger his ticket was refunded.
s32 TolbiInteriors_ApplyEntryState(void) {
  s32 clearedPriorityFlags;
  struct FieldActor *actor;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    // Restore the changed room tiles and the passengers' fixed draw priority.
    Map_CopyCellAttributes(51, 47, 3, 1, 51, 45);
    actor = Object_GetById(TOLBI_INTERIORS_ANGRY_PASSENGER);
    // Stored as a plain byte: the field store allocates the zero differently.
    *(u8 *)&actor->priorityFlags = clearedPriorityFlags = 0;
    actor->sprite->image.priority = 2;
    actor = Object_GetById(TOLBI_INTERIORS_PASSENGER_WIFE);
    actor->priorityFlags = clearedPriorityFlags;
    actor->sprite->image.priority = 2;
    if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN) != 0) {
      Actor_SetPosition(TOLBI_INTERIORS_MEDAL_WOMAN, PIXELS(560), PIXELS(680));
      Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, FACING_WEST, 0);
    }
    // Each short scene plays once, only when entering its room.
    if (gGameState.entrance == 19) {
      if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN) == 0) {
        Engine_GameFlagSet(FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN);
        Engine_EventOpenScreen();
        TolbiInteriors_WatchWomanHideMedal();
      }
    }
    if (gGameState.entrance == 16) {
      if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_PASSENGERS_SEEN) == 0) {
        Engine_GameFlagSet(FLAG_TOLBI_INTERIORS_PASSENGERS_SEEN);
        Engine_EventOpenScreen();
        TolbiInteriors_WatchPassengersArgue();
      }
    }
    // Once the docks' confrontation (the refund) is seen, these two have left.
    if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN) != 0) {
      Engine_ActorSetPosition(TOLBI_INTERIORS_FELLOW_PASSENGER, 0, 0);
      Engine_ActorSetPosition(TOLBI_INTERIORS_COLOSSO_FAN, 0, 0);
    }
  }
  return 0;
}

/// In Tolbi, the item shopkeeper: from across the counter (the leader facing
/// west) opens TOLBI_INTERIORS_ITEM_SHOP; from elsewhere talks about the
/// finals.
static void TolbiInteriors_TalkToItemShopkeeper(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_WEST) {
    Engine_ShopOpen(TOLBI_INTERIORS_ITEM_SHOP, speaker);
  } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    Engine_EventSetMessage(MsgTolbiRightOneWell);
    Engine_EventShowMessage(speaker, 0);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_EventSetMessage(MsgTolbiRequireLotHealing);
    Engine_EventShowMessage(speaker, 0);
  } else {
    TolbiInteriors_AskYesNo(MsgTolbiFirstTimeTolbi, speaker);
  }
}

/// In Tolbi, the weapon shopkeeper: across the counter (the leader facing
/// north) opens the shop; otherwise talks about Colosso.
static void TolbiInteriors_TalkToWeaponShopkeeper(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Engine_ShopOpen(TOLBI_INTERIORS_WEAPON_SHOP, speaker);
  } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    TolbiInteriors_AskYesNo(MsgTolbiComeWayKalay, speaker);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_EventSetMessage(MsgTolbiColossoFinalsFinally);
    Engine_EventShowMessage(speaker, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiLookStrongGo);
    Engine_EventShowMessage(speaker, 0);
    Engine_ActorShowEmote(speaker, EMOTE_IN_FRONT | 6, 0);
    Engine_EventWait(40);
    Engine_EventShowMessage(speaker, 0);
  }
}

/// In Tolbi, the armor shopkeeper: across the counter (the leader facing north)
/// opens the shop; otherwise talks about the finals.
static void TolbiInteriors_TalkToArmorShopkeeper(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Engine_ShopOpen(TOLBI_INTERIORS_ARMOR_SHOP, speaker);
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      Engine_EventSetMessage(MsgTolbiWasntAbleWatch);
      Engine_EventShowMessage(speaker, 0);
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      Engine_EventSetMessage(MsgTolbiMaybeCloseShop);
      Engine_EventShowMessage(speaker, 0);
    } else {
      Engine_EventSetMessage(MsgTolbiIfCanMakeNameFor);
      Engine_EventShowMessage(speaker, 0);
    }
  }
}

/// In Tolbi, the chef asks whether the party watched Colosso, which he missed.
static void TolbiInteriors_TalkToChef(s32 speaker) {
  Engine_EventBegin();
  TolbiInteriors_AskYesNo(MsgTolbiLookLikeWarrior, speaker);
  Engine_EventEnd();
}

/// In Tolbi after Colosso, a waiter grumbles about the chef's bad mood.
static void TolbiInteriors_TalkToWaiter(s32 speaker) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiGrrrChefInBadMood);
  Engine_EventShowMessage(speaker, 0);
  Engine_EventEnd();
}

/// In Tolbi during the festival, a resident offers the party a bed.
static void TolbiInteriors_TalkToHost(s32 speaker) {
  Engine_EventBegin();
  TolbiInteriors_AskYesNo(MsgTolbiWantStay, speaker);
  Engine_EventEnd();
}

/// In Tolbi, the woman who hid the Lucky Medal. Until the party takes it from
/// the barrel she teases; once it is gone she greets them, startled, the first
/// time, and then claims she is fine.
static void TolbiInteriors_TalkToMedalWoman(s32 speaker) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_TAKEN) == 0) {
    TolbiInteriors_AskYesNo(MsgTolbiHeeHeeLook, speaker);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_MISSED) == 0) {
      GameFlag_Set(FLAG_TOLBI_INTERIORS_MEDAL_MISSED);
      Engine_EventSetMessage(MsgTolbiHello);
      Event_ShowMessage(speaker, 0);
      Engine_EventWait(10);
      Engine_ActorRunRepeatedMotion(speaker, 2);
      Engine_EventWait(20);
    }
    Engine_EventSetMessage(MsgTolbiHoHumFine);
    Event_ShowMessage(speaker, 0);
  }
  Engine_EventEnd();
}

/// In Tolbi, Mind Read on the medal woman gives the hiding place away.
static void TolbiInteriors_ReadMedalWomanMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_INTERIORS_MEDAL_MISSED) == 0) {
    Engine_EventSetMessage(MsgTolbiHehHehSheJustHid);
  } else {
    Engine_EventSetMessage(MsgTolbiThingFoundDefinitelySameAs);
  }
  Engine_EventShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_EventEnd();
}

/// In Tolbi after Colosso, the angry passenger complains the ships won't sail.
/// "Grrr! What a scam!"
static void TolbiInteriors_TalkToAngryPassenger(s32 speaker) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiGrrrScamWhyWontThey);
  Actor_ShowEmote(TOLBI_INTERIORS_ANGRY_PASSENGER, EMOTE_IN_FRONT | 3, 40);
  Event_ShowMessage(speaker, 0);
  Engine_EventEnd();
}

/// In Tolbi after Colosso, the man who rode the ship from Kalay with the party
/// asks whether it remembers him. "Hey! You're the would-be warrior!"
static void TolbiInteriors_TalkToFellowPassenger(s32 speaker) {
  Engine_EventBegin();
  TolbiInteriors_AskYesNo(MsgTolbiWarriorRemember, speaker);
  Engine_EventEnd();
}

/// In Tolbi Sanctum, the priest: across the counter (the leader facing north)
/// opens the sanctum; otherwise talks about the festival.
static void TolbiInteriors_TalkToPriest(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Engine_SanctumOpen(speaker);
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      Engine_EventSetMessage(MsgTolbiShipsArentGoing);
      Engine_EventShowMessage(speaker, 0);
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      Engine_EventSetMessage(MsgTolbiMissFinalsTolbis);
      Engine_EventShowMessage(speaker, 0);
    } else {
      Engine_EventSetMessage(MsgTolbiWasteStuckHereWhenSuch);
      Engine_EventShowMessage(speaker, 0);
    }
  }
}

/// An actor that squashes and stretches twice, then rests 40 frames.
s32 gTolbiInteriorsSquashStretch[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gTolbiInteriorsEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_INTERIORS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 160, 0, 216,
     FACING_NORTH, 0, 0, 0, 256, 256, 0},
    {TOLBI_INTERIORS_HOUSE_NORTHWEST, CONDITION_ALWAYS, 336, 0, 200,
     FACING_NORTH, 0, 248, 64, 488, 216, 0},
    {TOLBI_INTERIORS_HOUSE_EAST, CONDITION_ALWAYS, 544, 0, 200, FACING_NORTH, 0,
     520, 48, 696, 216, 0},
    {TOLBI_INTERIORS_HOUSE_SOUTH, CONDITION_ALWAYS, 792, 0, 216, FACING_NORTH,
     0, 688, 0, 960, 240, 0},
    {TOLBI_INTERIORS_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 544, 0, 424,
     FACING_NORTH, 0, 488, 264, 696, 448, 0},
    {TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 312, 0, 472,
     FACING_NORTH, 0, 208, 272, 456, 488, 0},
    {TOLBI_INTERIORS_SANCTUM, CONDITION_ALWAYS, 824, 0, 528, FACING_NORTH, 0,
     688, 248, 944, 568, 0},
    {TOLBI_INTERIORS_INN, CONDITION_ALWAYS, 128, 0, 768, FACING_NORTH, 0, 0,
     536, 304, 800, 0},
    {TOLBI_INTERIORS_INN_FROM_INN_CORRIDOR, CONDITION_ALWAYS, 56, 0, 608,
     FACING_NORTH, 0, 0, 536, 304, 800, 0},
    {11, CONDITION_ALWAYS, 464, 0, 624, FACING_SOUTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {12, CONDITION_ALWAYS, 392, 0, 728, FACING_NORTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {13, CONDITION_ALWAYS, 536, 0, 728, FACING_NORTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {14, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {15, CONDITION_ALWAYS, 536, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {16, CONDITION_ALWAYS, 808, 0, 664, FACING_SOUTH, 0, 704, 608,
     TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X, 768, 0},
    {17, CONDITION_ALWAYS, 840, 0, 824, FACING_SOUTH, 0, 704, 768,
     TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X, 928, 0},
    {18, CONDITION_ALWAYS, 440, 0, 920, FACING_NORTH, 0, 288, 768, 528,
     TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z, 0},
    {19, CONDITION_ALWAYS, 584, 0, 920, FACING_NORTH, 0, 496, 768, 736,
     TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z, 0},

    {TOLBI_INTERIORS_SANCTUM_NORTH, CONDITION_ALWAYS, 824, 0, 352, FACING_NORTH,
     0, 688, 248, 944, 568, 0},
    {SCENE_TABLE_END},
};

u32 gTolbiInteriorsExits[] = {
    SCENE_EXITS(TolbiInteriors),
    SCENE_EXIT(TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_NORTHWEST),
    SCENE_EXIT(TOLBI_INTERIORS_INN_TO_TOLBI, Tolbi, TOLBI_FROM_INN),
    SCENE_EXIT(TOLBI_INTERIORS_TO_TOLBI_EAST, Tolbi,
               TOLBI_FROM_TOLBI_INTERIORS_EAST),
    SCENE_EXIT(TOLBI_INTERIORS_HOUSE_NORTHEAST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(TOLBI_INTERIORS_HOUSE_EAST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_EAST),
    SCENE_EXIT(TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, Tolbi,
               TOLBI_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(TOLBI_INTERIORS_HOUSE_SOUTHWEST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_SOUTHWEST),
    SCENE_EXIT(TOLBI_INTERIORS_SANCTUM_TO_TOLBI, Tolbi, TOLBI_FROM_SANCTUM),
    SCENE_EXIT(TOLBI_INTERIORS_TO_TOLBI_WEST, Tolbi,
               TOLBI_FROM_TOLBI_INTERIORS_WEST),
    SCENE_EXIT(TOLBI_INTERIORS_INN_TO_INN_CORRIDOR, TolbiInteriors,
               TOLBI_INTERIORS_INN_CORRIDOR_FROM_INN),
    SCENE_EXIT(TOLBI_INTERIORS_INN_CORRIDOR_TO_INN, TolbiInteriors,
               TOLBI_INTERIORS_INN_FROM_INN_CORRIDOR),
    SCENE_EXIT(TOLBI_INTERIORS_INN_CORRIDOR_TO_GUEST_ROOM_SOUTHWEST,
               TolbiInteriors,
               TOLBI_INTERIORS_GUEST_ROOM_SOUTHWEST_FROM_INN_CORRIDOR),
    SCENE_EXIT(TOLBI_INTERIORS_INN_CORRIDOR_TO_GUEST_ROOM_SOUTHEAST,
               TolbiInteriors,
               TOLBI_INTERIORS_GUEST_ROOM_SOUTHEAST_FROM_INN_CORRIDOR),
    SCENE_EXIT(TOLBI_INTERIORS_INN_CORRIDOR_TO_GUEST_ROOM_NORTHWEST,
               TolbiInteriors,
               TOLBI_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_CORRIDOR),
    SCENE_EXIT(TOLBI_INTERIORS_INN_CORRIDOR_TO_GUEST_ROOM_NORTHEAST,
               TolbiInteriors,
               TOLBI_INTERIORS_GUEST_ROOM_NORTHEAST_FROM_INN_CORRIDOR),
    SCENE_EXIT(TOLBI_INTERIORS_GUEST_ROOM_SOUTHEAST_TO_INN_CORRIDOR,
               TolbiInteriors,
               TOLBI_INTERIORS_INN_CORRIDOR_FROM_GUEST_ROOM_SOUTHEAST),
    SCENE_EXIT(TOLBI_INTERIORS_GUEST_ROOM_SOUTHWEST_TO_INN_CORRIDOR,
               TolbiInteriors,
               TOLBI_INTERIORS_INN_CORRIDOR_FROM_GUEST_ROOM_SOUTHWEST),
    SCENE_EXIT(TOLBI_INTERIORS_GUEST_ROOM_NORTHWEST_TO_INN_CORRIDOR,
               TolbiInteriors,
               TOLBI_INTERIORS_INN_CORRIDOR_FROM_GUEST_ROOM_NORTHWEST),
    SCENE_EXIT(TOLBI_INTERIORS_GUEST_ROOM_NORTHEAST_TO_INN_CORRIDOR,
               TolbiInteriors,
               TOLBI_INTERIORS_INN_CORRIDOR_FROM_GUEST_ROOM_NORTHEAST),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gTolbiInteriorsPlacements[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(104), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(144), 0, PIXELS(104), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(352), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(832), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(584), 0, PIXELS(848), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(230), 0, PIXELS(648), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiInteriorsPlacementsAfterColosso[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(520), 0, PIXELS(520), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(600), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(848), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x94, CONDITION_ALWAYS, (s32)gTolbiInteriorsSquashStretch,
        PIXELS(230), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(712), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(816), 0, PIXELS(712), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(688), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(672), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(864), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gTolbiInteriorsEventsFestival[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, TOLBI_INTERIORS_INN_TO_TOLBI },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, TOLBI_INTERIORS_TO_TOLBI_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_NORTHEAST_TO_TOLBI },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_EAST_TO_TOLBI },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_SOUTHWEST_TO_TOLBI },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, TOLBI_INTERIORS_SANCTUM_TO_TOLBI },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, TOLBI_INTERIORS_TO_TOLBI_WEST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, TOLBI_INTERIORS_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring) },
    { EVENT_TALK, TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToHost) },
    { EVENT_TALK, TOLBI_INTERIORS_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICouldntSleepAtAllLast) },
    { EVENT_TALK, TOLBI_INTERIORS_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrowdsAtTheColosseumMade) },
    { EVENT_TALK, TOLBI_INTERIORS_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWePaidGoodMoneyToRent) },
    { EVENT_TALK, TOLBI_INTERIORS_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIReallyWantToSeeColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWahhhIWantToSeeColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfTheyReallyCant) },
    { EVENT_TALK, TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiWasntAtColossoAccordingTo) },
    { EVENT_TALK, TOLBI_INTERIORS_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereGoingToMakeSomeMoney) },
    { EVENT_TALK, TOLBI_INTERIORS_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsUsingThe) },
    { EVENT_TALK, TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereFullyBookedWithReservationsDuring) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsTheBusiestTimeOf) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereSoBusyRunningTheInn) },
    { EVENT_TALK, TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsATourGroupCameFromKalay) },
    { EVENT_TALK, TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeArentGoingToGetTo) },
    { EVENT_TALK, TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImNotVeryInterestedInColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOhNoWeBarelyMadeIt) },
    { EVENT_TALK, TOLBI_INTERIORS_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWedGottenHereJustA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceColossoStartsWereGoingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsTooLateToStartLooking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfICouldHaveSleptLast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhUhCrowdsAreADefinite) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeGetToStayHereUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasStubbornISaidIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIKeepOnCryingButDaddy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeBabiLovesColossoBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeJustTakesACutOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNamingTheLighthouseHeBuiltAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGuessINeverWouldHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheyHaveSomeNiceThingsFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiChoosesColossosVictorsToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTourGroupIsFeelingCramped) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeCantHandleTooManyMore) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToGoToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentEnoughBedsForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheInnkeeperToldMeToCook) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHaveAHeadacheJustThinking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenThoughICantEnterColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWeCouldHaveEnteredColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderHowTheTryoutsAre) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_VIAL_BARREL, 0xf9a, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_MEDAL_BARREL, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_INTERIORS_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiInteriorsEventsFinalsDay[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, TOLBI_INTERIORS_INN_TO_TOLBI },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, TOLBI_INTERIORS_TO_TOLBI_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_NORTHEAST_TO_TOLBI },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_EAST_TO_TOLBI },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_SOUTHWEST_TO_TOLBI },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, TOLBI_INTERIORS_SANCTUM_TO_TOLBI },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, TOLBI_INTERIORS_TO_TOLBI_WEST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, TOLBI_INTERIORS_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoHasAlreadyReachedThe) },
    { EVENT_TALK, TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheFestivalScheduleWasDelayedFor) },
    { EVENT_TALK, TOLBI_INTERIORS_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatsStrangeIWonderWhatHappened) },
    { EVENT_TALK, TOLBI_INTERIORS_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIJustSawBabiTheTowns) },
    { EVENT_TALK, TOLBI_INTERIORS_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHereIAmInTolbiFor) },
    { EVENT_TALK, TOLBI_INTERIORS_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToWatchTheFinals) },
    { EVENT_TALK, TOLBI_INTERIORS_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHoorayWereGoingToColossoNow) },
#if TBS_PATCH_4
    { EVENT_TALK, TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_AskWhetherBabiWasMissing) },
    { EVENT_TALK, TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiDecidedShowUpForFinals) },
#else
    { EVENT_TALK, TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWellIGuessSoSinceHes) },
    { EVENT_TALK, TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhThenTheyFoundHimIn) },
#endif
    { EVENT_TALK, TOLBI_INTERIORS_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiApparentlyHasSomeReasonFor) },
    { EVENT_TALK, TOLBI_INTERIORS_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsSearchingFor) },
    { EVENT_TALK, TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheFinalsBeginEveryoneTries) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThisYearImNotLettingAnyone) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWifeHasACloseEye) },
    { EVENT_TALK, TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHmmIThoughtAllMenLoved) },
    { EVENT_TALK, TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheMomentIHearTheFinals) },
    { EVENT_TALK, TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoIsEndingSoonIts) },
    { EVENT_TALK, TOLBI_INTERIORS_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheColossoFinalsAreStartingSoon) },
    { EVENT_TALK, TOLBI_INTERIORS_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeIllDoSomeResearchOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsAreWeMakingMoreMoneyNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoTheyDidExtendTheFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICantAffordToStayHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheAirInTheColosseumIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfSomebodyElseWouldWatchThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsAlreadyAtTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceWeDidntGetToSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoWhatWereBabisSoldiersDoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeWatchingTheTrialsIsToo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBeforeHeStartedTheLighthouseBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiHasBeenMakingStrangeReferences) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAllBuyWeaponsAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouCouldSayTheWarriorsIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomFindsOutIWent) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImTheOneWhoReallyWants) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThingsSettleDownImSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfTheMasterSlipsOutThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeWomenAreTheOnlyOnes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHasntTheMasterGoneToColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIDontHaveAnyParticularInterest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWantToSeeTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTrialBattlesWerentQuiteLike) },
    { EVENT_TALK, TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfYoureNotInterestedInThe) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_VIAL_BARREL, 0xf9a, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_MEDAL_BARREL, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_INTERIORS_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiInteriorsEventsAfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_NORTHWEST_TO_TOLBI },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, TOLBI_INTERIORS_INN_TO_TOLBI },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, TOLBI_INTERIORS_TO_TOLBI_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_NORTHEAST_TO_TOLBI },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_EAST_TO_TOLBI },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, TOLBI_INTERIORS_HOUSE_SOUTHWEST_TO_TOLBI },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, TOLBI_INTERIORS_SANCTUM_TO_TOLBI },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, TOLBI_INTERIORS_TO_TOLBI_WEST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, TOLBI_INTERIORS_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNowThatColossoIsOverWeve) },
    { EVENT_TALK, TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeHadALotOfGuests) },
    { EVENT_TALK, TOLBI_INTERIORS_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIRentedMyHouseToA) },
    { EVENT_TALK, TOLBI_INTERIORS_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAreAllInA) },
    { EVENT_TALK, TOLBI_INTERIORS_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasSoMovedByThis) },
    { EVENT_TALK, TOLBI_INTERIORS_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatLittleGuyJumpedStraightInto) },
    { EVENT_TALK, TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidYouKnowMtAlephErupted) },
    { EVENT_TALK, TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeAlephsEruptionCausedThoseSea) },
    { EVENT_TALK, TOLBI_INTERIORS_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsSaidTheyWouldRush) },
    { EVENT_TALK, TOLBI_INTERIORS_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereWontBeManyCustomersUntil) },
    { EVENT_TALK, TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIThoughtWedHaveSomeFree) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSpectatorsFromTheEastCant) },
    { EVENT_TALK, TOLBI_INTERIORS_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsBusyAllDayEveryDay) },
    { EVENT_TALK, TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToMedalWoman) },
    { EVENT_TALK, TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToChef) },
    { EVENT_TALK, TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWaiter) },
    { EVENT_TALK, TOLBI_INTERIORS_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWentToThePortBut) },
    { EVENT_TALK, TOLBI_INTERIORS_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoHasEndedButWeStill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSighMakingTheBedsHasWorn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIdLoveAVacationButWhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheShipsStartSailingAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardOfACaveThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsWhoFoughtInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDaringAndBraveryAreNiceAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSparklingStonesThatRainedFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIllBetTheMonstersWereFrightened) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItBothersMeABitThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentManyMerchantsAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenWithoutTheShipsICould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidAllTheWarriorsWhoFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGonnaWatchTheFinalsNext) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceTheShipsArentSailingThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeeHeeItsGreatTheInns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEveryoneIsWorkingOnBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_ReadMedalWomanMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsGrrrIDidntGetToSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeyChefSoWhatIfYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsITriedGoingToTheEastern) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoudThinkYouCouldGetThrough) },
    { EVENT_TALK, TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouAllGotToSeeColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_ANGRY_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToAngryPassenger) },
    { EVENT_TALK, TOLBI_INTERIORS_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThisGuyGetsMadTheres) },
    { EVENT_TALK, TOLBI_INTERIORS_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfThisReallyIs) },
    { EVENT_TALK, TOLBI_INTERIORS_BORED_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiGetsSoBoringOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_ANGRY_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoureSayingIfWeRowWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrewIsSayingThatIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImSickOfStayingAtThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_BORED_CHILD, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomHadJustBoughtMe) },
    { EVENT_TALK, TOLBI_INTERIORS_FELLOW_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToFellowPassenger) },
    { EVENT_TALK, TOLBI_INTERIORS_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGotToSeeColossoSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_FELLOW_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatVoyageAcrossTheSeaWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNoMatterWhatHappensImComing) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_VIAL_BARREL, 0xf9a, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_MEDAL_BARREL, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_INTERIORS_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_HIDDEN_MEDAL_BARREL, FLAG_TOLBI_INTERIORS_MEDAL_TAKEN, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gTolbiInteriorsDoorOpening[] = {
    MAP_CELL_STEP(2, 123, 1, 2, 6),
    MAP_CELL_STEP(4, 123, 1, 2, 6),
    MAP_CELL_STEPS_END,
};

struct MapCellAnim gTolbiInteriorsDoors[] = {
    {gTolbiInteriorsDoorOpening, 24, 100},
    {gTolbiInteriorsDoorOpening, 33, 100},
    {gTolbiInteriorsDoorOpening, 50, 99},
    {gTolbiInteriorsDoorOpening, 52, 109},
};
