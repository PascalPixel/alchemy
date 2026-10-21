/// @file
/// The buildings of Suhalla (SceneId_SuhallaInteriors): the village's houses,
/// its one shop and its inn, which the area names call Suhalla, and Suhalla
/// Sanctum, as the same scene is named at SUHALLA_INTERIORS_SANCTUM and
/// SUHALLA_INTERIORS_SANCTUM_NORTH.
///
/// - Suhalla, the houses, shop and inn behind the village's doors
///   (SUHALLA_TO_INN, SUHALLA_TO_HOUSE_WEST, SUHALLA_TO_HOUSE_NORTHEAST and
///   SUHALLA_TO_HOUSE_EAST lead to the entrances of the same numbers, and
///   leaving by them goes back out). Doors join the inn to its two guest
///   rooms, and a ladder to its loft.
/// - Suhalla Sanctum, behind the village's wide door (SUHALLA_TO_SANCTUM, to
///   SUHALLA_INTERIORS_SANCTUM), whose priest offers the sanctum's services.
///
/// The rooms' people and talk come in two sets, chosen by whether Babi has
/// held his audience after Colosso (FLAG_BABI_SPOKE_AFTER_COLOSSO). Before
/// it, Suhalla talks of Lalivero across the Nol River, under Tolbi's rule and
/// out of reach, and of the travelers the sandstorms will send its inn once
/// Tolbi's festival ends. The shopkeeper, the innkeeper and the priest serve
/// the party across their counters and otherwise chat by the same flag.
///
/// At that audience Babi sent the party to Venus Lighthouse with Iodem, who
/// commands his soldiers and travels with the party; Sheba, the holy child of
/// Lalivero whom Babi held as a hostage, had already left Tolbi for Lalivero
/// with soldiers bound for Babi Lighthouse. Afterwards two of those soldiers
/// lie wounded in the house of a man who, his child says, aids anyone in
/// trouble; the inn is full of travelers the storms blew back, and two
/// villagers talk of the death storms in the middle of the Suhalla; one says
/// there was a saying about them she cannot remember, and Mind Read finds half
/// of it, `"Ye who seek..." Something about eyes of truth...` Stepping into
/// that house (cell code 50) runs a scripted scene once
/// (Event_IodemQuestionsWoundedSoldiers, FLAG_IODEM_MET_WOUNDED_SOLDIERS):
/// Iodem steps out of the party to question the soldiers, who woke in these
/// beds and do not know where Sheba is, only that she was not in the sandstorm,
/// and who warn that monsters cause the storms, that others got through them
/// with water and slew the beasts. Iodem asks whether these others are the ones
/// the party is following and, whatever the answer, resolves to find Sheba and
/// to cross the Suhalla Desert after them, then rejoins the party. Iodem only
/// does this before he has reached Babi Lighthouse
/// (FLAG_IODEM_AT_BABI_LIGHTHOUSE); what he says in the desert and at Suhalla
/// Gate later depends on FLAG_IODEM_MET_WOUNDED_SOLDIERS.
///
/// Searching the ovens finds a Smoke Bomb, once
/// (FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB), and shows what is cooking, except
/// at the Smoke Bomb's oven: its find always answers first, so the cactus steak
/// line there is unused.

#include "game_state.h"
#include "field_event.h"
#include "unit/import.h"
#include "unit/scene/suhalla_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"

/// The actors, numbering the rows of gSuhallaInteriorsPlacements and of
/// gSuhallaInteriorsPlacementsAfterAudience from ACTOR_FIRST_PLACED; the two
/// tables agree up to ACTOR_PRIEST. Where an actor's talk changes after
/// Babi's audience, the note quotes it before.
enum SuhallaInteriorsActor {
  /// The child of the man who aids anyone in trouble. "My father loves people
  /// so much, he can't help but aid anyone in trouble."
  ACTOR_HELPFUL_FATHERS_CHILD = ACTOR_FIRST_PLACED,
  /// The man who aids anyone in trouble, in whose house the wounded soldiers
  /// lie after the audience. "Let me know if you have any problems. I'll do
  /// anything I can to help."
  ACTOR_HELPFUL_FATHER,
  /// "Tolbi probably ignores small villages like Suhalla, doesn't it?" After
  /// the audience: "Do not approach the large cyclones in the middle of the
  /// Suhalla."
  ACTOR_VILLAGER_IGNORED_BY_TOLBI,
  /// In the old woman's sprite (as in vale_gate.c). "If you head east across
  /// Suhalla, Lalivero is so close you can see it across the Nol River..."
  /// After the audience she half remembers a saying about the death storms.
  ACTOR_OLD_WOMAN,
  ACTOR_SHOPKEEPER,
  ACTOR_INNKEEPER,
  /// A woman at the inn, in the sprite of Kalay's innkeeper's wife. "I wonder
  /// if any customers can get through those sandstorms..."
  ACTOR_INN_WOMAN,
  /// Travelers the sandstorms blew back, out of sight until the audience. "We
  /// went into the Suhalla to go build the lighthouse, but the storms caught
  /// us."
  ACTOR_LIGHTHOUSE_BUILDER,
  /// "The sandstorms are so bad, we've given up trying to reach Babi
  /// Lighthouse."
  ACTOR_SECOND_LIGHTHOUSE_BUILDER,
  /// "I set out for Babi Lighthouse to open a business there, but then I was
  /// blown back here again!"
  ACTOR_STRANDED_TRAVELER,
  ACTOR_PRIEST, ///< keeps Suhalla Sanctum
  ACTOR_IODEM,
  /// The wounded soldiers, lying in the beds of a house after Babi's audience.
  ACTOR_SOLDIER_SOUTH,
  ACTOR_SOLDIER_NORTH
};

enum { SHOP_SUHALLA = 31, INN_SUHALLA = 10 };

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSuhallaInteriorsEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) {
  return (const struct SceneRegion *)gSuhallaInteriorsRegions;
}

const u32 *Scene_GetExits(void) { return gSuhallaInteriorsExits; }

/// Suhalla's people, joined after Babi's audience by Iodem, the wounded
/// soldiers and the travelers the sandstorms brought.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaInteriorsPlacementsAfterAudience;
  }
  return gSuhallaInteriorsPlacements;
}

/// Suhalla's shopkeeper. Spoken to facing north, across the counter, the
/// shopkeeper opens the shop; otherwise says Suhalla's is the only store, or,
/// after Babi's audience, asks whether the party is surprised to find so few
/// shops.
static void Event_TalkToShopkeeper(s32 actorId) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_ShopOpen(SHOP_SUHALLA, actorId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    s32 question = MsgSuhallaArentSurprisedFind;
    Engine_EventSetMessage(question);
    Engine_EventOpenMessage(actorId, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventWait(10);
      Engine_EventSetMessage(question + 1);
    } else {
      Engine_EventSetMessage(question + 2);
    }
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaOursOnlyStore);
    Engine_EventShowMessage(actorId, 0);
  }
}

/// Suhalla's innkeeper. Spoken to facing north, across the counter, the
/// innkeeper offers a room; otherwise has nothing to do until Tolbi's festival
/// ends, or, after Babi's audience, has been busy since the sandstorms began.
static void Event_TalkToInnkeeper(s32 actorId) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_InnCheckIn(INN_SUHALLA, actorId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    Engine_EventSetMessage(MsgSuhallaBusyEverSince);
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaSighNothinMaybe);
    Engine_EventShowMessage(actorId, 0);
  }
}

/// Suhalla Sanctum's priest. Spoken to facing north, across the counter, the
/// priest offers the sanctum's services; otherwise talks of Lalivero under
/// Tolbi's rule, or, after Babi's audience, wonders why there are so many
/// sandstorms.
static void Event_TalkToPriest(s32 actorId) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_ShopConfirmAct(actorId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    Engine_EventSetMessage(MsgSuhallaWonderWhySandstorms);
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaGetSickThinkingAboutLalivero);
    Engine_EventShowMessage(actorId, 0);
  }
}

/// In Suhalla after Babi's audience, a traveler the storms blew back asks
/// whether the party was blown here on the way to Babi Lighthouse: yes, and
/// the traveler means to give up and follow the northern mountain back to
/// Tolbi; no, and tells of setting out to open a business at the lighthouse.
static void Event_AskIfBlownHere(s32 actorId) {
  s32 question = MsgSuhallaSupposeFolkBlown;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

/// The doors of Suhalla and Suhalla Sanctum, and the doors between the inn and
/// its guest rooms: the party leaves by the touched exit.
static void Event_LeaveByExit(void) {
  struct EventWork *state = gEventWork;
  s16 *touchedTrigger = (s16 *)&state->touchedTrigger;

  Engine_EventRequestExit(*touchedTrigger);
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

/// In Suhalla after Babi's audience, stepping into the house where the two
/// wounded soldiers lie (cell code 50), once: Iodem steps out of the party and
/// questions them. "Iodem..." They were escorting Sheba to Lalivero when a
/// sandstorm hit them, and tell of others who stopped the cyclones with water
/// and slew the monsters causing them. Iodem asks whether these are the ones
/// the party follows, resolves either way to search the desert for Sheba and
/// pursue them, and rejoins the party. Sets
/// FLAG_IODEM_MET_WOUNDED_SOLDIERS either way; once Iodem has reached Babi
/// Lighthouse (FLAG_IODEM_AT_BABI_LIGHTHOUSE), the scene does not play.
static void Event_IodemQuestionsWoundedSoldiers(void) {
  struct FieldActor *leader;
  GameFlag_Set(FLAG_IODEM_MET_WOUNDED_SOLDIERS);
  if (GameFlag_IsSet(FLAG_IODEM_AT_BABI_LIGHTHOUSE) == 0) {
    Event_PlayCue(MUS_030);
    Event_Begin();
    // Frame the speakers and bring the leader over before the dialogue.
    Camera_MoveTo(PIXELS(368), -1, PIXELS(104), 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    FieldMotion_LaunchFromFocusedObject(ACTOR_IODEM, 0, -16, FACING_NORTH);
    Actor_WaitForMove(ACTOR_IODEM);
    Camera_WaitForMove();
    Event_SetMessage(MsgSuhallaIodem);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IODEM, 78643, 39321);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_IODEM, 78643, 39321);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -24);
    Actor_WalkByAndWait(ACTOR_IODEM, 48, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_SOLDIER_SOUTH, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_NORTH, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 7, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 78643, 39321);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 104);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IODEM, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_NORTH, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IODEM, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SOUTH, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_NORTH, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SOUTH, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 50);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SOUTH, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 80);
    Actor_ShowEmote(ACTOR_SOLDIER_NORTH, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 20);
    Actor_WalkByAndWait(ACTOR_IODEM, -12, 0);
    Event_Wait(20);
    // Both answers converge on the following line after different waits.
    Event_OpenMessage(ACTOR_IODEM, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Event_ShowMessage(ACTOR_IODEM, 0);
      gEventWork->message++;
    } else {
      Event_Wait(10);
      gEventWork->message++;
      Event_ShowMessage(ACTOR_IODEM, 0);
    }
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, 3);
    Event_Wait(30);
    Event_PlayCue(MUS_030);
    Actor_SetSpeed(ACTOR_IODEM, 78643, 39321);
    Actor_SetAnimation(ACTOR_IODEM, 2);
    // Iodem walks into the leader and is hidden: he travels in the party.
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_IODEM);
    Actor_SetPosition(ACTOR_IODEM, 0, 0);
    Event_Wait(10);
    // Back to the place's own music.
    Audio_PlayCueFromEventWork();
    Event_End();
  }
}

/// The doors and inner exits, people and ovens of Suhalla's buildings and
/// Suhalla Sanctum, before or after Babi's audience.
const struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaInteriorsEventsAfterAudience;
  }
  return gSuhallaInteriorsEvents;
}

/// Sets Suhalla's buildings up as the party enters: the shopkeeper and
/// innkeeper behind their counters, and the two wounded soldiers lying in their
/// beds. SUHALLA_INTERIORS_AFTER_BABI_AUDIENCE sets
/// FLAG_BABI_SPOKE_AFTER_COLOSSO.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  if (gGameState.entrance == SUHALLA_INTERIORS_AFTER_BABI_AUDIENCE)
    Engine_GameFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gEventWork->transitionFrames = 24;
  // Nearer to talk to, across their counters.
  Object_GetById(ACTOR_SHOPKEEPER)->collisionFlags |= 4;
  Object_GetById(ACTOR_INNKEEPER)->collisionFlags |= 4;
  // Each soldier lies a quarter turn over, with no shadow.
  actor = Object_GetById(ACTOR_SOLDIER_SOUTH);
  actor->sprite->flags = 0;
  actor->sprite->rotation = 0x4000;
  actor->sprite->image.priority = 1;
  actor = Object_GetById(ACTOR_SOLDIER_NORTH);
  actor->sprite->flags = 0;
  actor->sprite->rotation = 0x4000;
  actor->motionFlags = ACTOR_FALLS;
  actor->y.fixed = 0;
  return 0;
}

struct SceneEntrance gSuhallaInteriorsEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SUHALLA_INTERIORS_INN, CONDITION_ALWAYS, 256, 0, 456, FACING_NORTH, 0, 0,
     280, 336, 480, 0},
    {SUHALLA_INTERIORS_SANCTUM, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0,
     312, 528, 552, 752, 0},
    {SUHALLA_INTERIORS_HOUSE_WEST, CONDITION_ALWAYS, 640, 0, 168, FACING_NORTH,
     0, 496, 16, 736, 192, 0},
    {SUHALLA_INTERIORS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 112, 0, 168,
     FACING_NORTH, 0, 0, 16, 240, 208, 0},
    {SUHALLA_INTERIORS_HOUSE_EAST, CONDITION_ALWAYS, 352, 0, 216, FACING_NORTH,
     0, 256, 16, 496, 232, 0},
    {SUHALLA_INTERIORS_SANCTUM_NORTH, CONDITION_ALWAYS, 432, 0, 630,
     FACING_NORTH, 0, 312, 528, 552, 752, 0},
    {SUHALLA_INTERIORS_HOUSE_WEST_UNUSED, CONDITION_ALWAYS, 680, 0, 120,
     FACING_WEST, 0, 496, 16, 736, 192, 0},
    {SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_WEST, CONDITION_ALWAYS, 104, 0, 360,
     FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_EAST, CONDITION_ALWAYS, 152, 0, 360,
     FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {SUHALLA_INTERIORS_GUEST_ROOM_WEST_FROM_INN, CONDITION_ALWAYS, 472, 0, 344,
     FACING_SOUTH, 0, 344, 280, 584, 440, 0},
    {SUHALLA_INTERIORS_GUEST_ROOM_EAST_FROM_INN, CONDITION_ALWAYS, 712, 0, 344,
     FACING_SOUTH, 0, 600, 280, 840, 440, 0},
    {SUHALLA_INTERIORS_INN_FROM_LOFT, CONDITION_ALWAYS, 88, 0, 352,
     FACING_SOUTH, 0, 0, 280, 336, 480, 0},
    {SUHALLA_INTERIORS_LOFT_FROM_INN, CONDITION_ALWAYS, 88, 0, 584,
     FACING_NORTH, 0, 16, 528, 256, 688, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gSuhallaInteriorsRegions[] = {
    83, 28, 356, 91, 36, 364, -1, 14,
    83, -20, 596, 91, -12, 604, -1, 15,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSuhallaInteriorsExits[] = {
    SCENE_EXITS(SuhallaInteriors),
    SCENE_EXIT(SUHALLA_INTERIORS_INN_TO_SUHALLA, Suhalla, SUHALLA_FROM_INN),
    SCENE_EXIT(SUHALLA_INTERIORS_SANCTUM_TO_SUHALLA, Suhalla, SUHALLA_FROM_SANCTUM),
    SCENE_EXIT(SUHALLA_INTERIORS_HOUSE_WEST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_WEST),
    SCENE_EXIT(SUHALLA_INTERIORS_HOUSE_NORTHEAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(SUHALLA_INTERIORS_HOUSE_EAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_EAST),
    SCENE_EXIT(SUHALLA_INTERIORS_INN_TO_GUEST_ROOM_WEST, SuhallaInteriors, SUHALLA_INTERIORS_GUEST_ROOM_WEST_FROM_INN),
    SCENE_EXIT(SUHALLA_INTERIORS_INN_TO_GUEST_ROOM_EAST, SuhallaInteriors, SUHALLA_INTERIORS_GUEST_ROOM_EAST_FROM_INN),
    SCENE_EXIT(SUHALLA_INTERIORS_GUEST_ROOM_WEST_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_WEST),
    SCENE_EXIT(SUHALLA_INTERIORS_GUEST_ROOM_EAST_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_EAST),
    SCENE_EXIT(SUHALLA_INTERIORS_INN_TO_LOFT, SuhallaInteriors, SUHALLA_INTERIORS_LOFT_FROM_INN),
    SCENE_EXIT(SUHALLA_INTERIORS_LOFT_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN_FROM_LOFT),
    SCENE_EXITS_END,
};

struct ScenePlacement gSuhallaInteriorsPlacements[] = {
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(368), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaInteriorsPlacementsAfterAudience[] = {
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(184), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(184), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(688), 0, PIXELS(384), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(408), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(136), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x39, FLAG_IODEM_MET_WOUNDED_SOLDIERS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSuhallaInteriorsEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TALK, ACTOR_HELPFUL_FATHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsMyFatherLovesPeopleSoMuch) },
    { EVENT_TALK, ACTOR_HELPFUL_FATHER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsLetMeKnowIfYouHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HELPFUL_FATHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenISeePeopleInTrouble) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HELPFUL_FATHER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWantToHelpPeopleSo) },
    { EVENT_TALK, ACTOR_VILLAGER_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIfYouHeadEastAcrossSuhalla) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSuhallaIsADesertVillageI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsGoodThingItsNotEasyTo) },
    { EVENT_TALK, ACTOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAVillageThisSmallCantSupport) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWonderIfAnyCustomersCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceTheFestivalEndsTheTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets) },
    { EVENT_TALK, ACTOR_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheHolyChildActorWillSave) },
    { SEARCH(SEARCH_OVEN), 100, FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    // Unused: this oven's item row answers first (see the scenes README).
    { SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSuhallaInteriorsEventsAfterAudience[] = {
    { EVENT_TOUCH, 50, FLAG_IODEM_MET_WOUNDED_SOLDIERS, EVENT_SCRIPT(Event_IodemQuestionsWoundedSoldiers) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TALK, ACTOR_HELPFUL_FATHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheWoundedAreSleepingSoDont) },
    { EVENT_TALK, ACTOR_HELPFUL_FATHER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIAmAmazedTheseSoldiersSurvived) },
    { EVENT_TALK, ACTOR_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhyWereTheyHeadedTowardEastern) },
    { EVENT_TALK, ACTOR_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhereIsActorWhatHasHappened) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HELPFUL_FATHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HELPFUL_FATHER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhoCouldHaveDoneThisTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAColumnOfWaterShotUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsActorWasNotFarBehindUs) },
    { EVENT_TALK, ACTOR_VILLAGER_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDoNotApproachTheLargeCyclones) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDeathStormsHuhThereWasA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceYouGetSweptUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsYeWhoSeekSomethingAboutEyes) },
    { EVENT_TALK, ACTOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeveHadMoreCustomersLatelyBecause) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsItWillTakeTimeForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSendingCustomersOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsPleaseGreatSandstormsSendUsMore) },
    { EVENT_TALK, ACTOR_LIGHTHOUSE_BUILDER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeWentIntoTheSuhallaTo) },
    { EVENT_TALK, ACTOR_SECOND_LIGHTHOUSE_BUILDER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSoBadWeve) },
    { EVENT_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIfBlownHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIGHTHOUSE_BUILDER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsEvenIfWeCantGoIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_LIGHTHOUSE_BUILDER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSoShallWeGiveUpOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenThatCycloneThrewUsInto) },
    { EVENT_TALK, ACTOR_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIShudderToThinkSandstormsWill) },
    { SEARCH(SEARCH_OVEN), 100, FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    // Unused: this oven's item row answers first (see the scenes README).
    { SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { SCENE_EVENTS_END },
};
// clang-format on
