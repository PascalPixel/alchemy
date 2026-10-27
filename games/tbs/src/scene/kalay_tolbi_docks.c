/// @file
/// Kalay Docks and Tolbi Docks, the two ends of the ship's crossing of the
/// Karagol Sea, in three scenes that share one overlay: SceneId_KalayDocks,
/// SceneId_TolbiDocks and SceneId_CottageRoomsE, the sheet that holds a room at
/// each. The getters pick each scene's tables by gCommon.sceneId, and
/// three story flags choose among them: FLAG_TOLBI_SHIP_REACHED_TOLBI, whether
/// the ship has made its crossing (set on the World Map,
/// WorldMap_CompleteTransfer); FLAG_KALAY_COLOSSO_TOUR_LEFT, whether the
/// Colosso Tour's group has left Kalay; and FLAG_COLOSSO_OVER, whether the
/// Colosso finals have been fought.
///
/// Kalay Docks (KalayDocks): the first time the party walks in from the World
/// Map (KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL)
/// KalayTolbiDocks_GaretSeesTheKaragol plays. Until the tour's
/// group has left Kalay, the ship is open to look around and the man at its
/// plank (ACTOR_CHECKING_TICKETS) checks no tickets; afterwards he stands in
/// the way and asks for one, takes it if the party has bought it
/// (FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) and lets the party aboard, and then
/// offers to let it ashore again until it boards
/// (FLAG_KALAY_DOCKS_WENT_ASHORE, cleared each time the party comes back off
/// the ship by KALAY_DOCKS_FROM_TOLBI_SHIP_DECK). A woman
/// (ACTOR_WOMAN_WITH_A_MESSAGE) asks the party to tell her daughter in Tolbi
/// that she cannot afford the tickets (FLAG_KALAY_DOCKS_CARRYING_MESSAGE).
/// She is the daughter's mother, and the man beside her
/// (ACTOR_MAN_AT_SILK_ROADS_END) her father: the Japanese text of
/// MsgKalayPassMessageDaughter has her name herself 母, "Mother", as she would
/// to her daughter, which the English renders `Her mother says that tickets
/// are too expensive`. Ground, a Venus Djinni (ACTOR_GROUND),
/// waits on the docks until it joins. Once the ship has crossed, the travellers
/// are gone and the dock hands wait for a ship that has not returned. The docks
/// lead to the World Map (KALAY_DOCKS_TO_WORLD_MAP_SOUTH and
/// KALAY_DOCKS_TO_WORLD_MAP_NORTH), onto the ship
/// (KALAY_DOCKS_TO_TOLBI_SHIP_DECK, the Tolbi-bound Ship,
/// SceneId_TolbiShipDeck) and through a door (step trigger 10) into the
/// building beside them, whose room is on CottageRoomsE.
///
/// Cottage rooms E (CottageRoomsE) is a sheet of rooms drawn as cottages: the
/// game packs rooms of several places that share a tile style onto one map, and
/// this one holds a room at each dock, in story order by the exits that lead
/// in: the room at Kalay Docks (COTTAGE_ROOMS_E_FROM_KALAY_DOCKS), whose area
/// name is Kalay Docks, and the room at Tolbi Docks
/// (COTTAGE_ROOMS_E_FROM_TOLBI_DOCKS), whose area name is Tolbi Docks. In the
/// Kalay room a woman sells the boarding ticket, 600 coins for the four of them
/// (FLAG_KALAY_DOCKS_TICKET_BOUGHT), until the ship has sailed, when she says
/// it will not leave again for the monsters at sea; in the Tolbi room no
/// tickets are sold at all, the sea being too dangerous.
///
/// Tolbi Docks (TolbiDocks) is where the ship lands after the crossing.
/// Stepping off (step trigger 20, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED) the party
/// is met by a man on the docks who asks about the monster, with a reply for
/// each answer. The woman's daughter (ACTOR_WAITING_DAUGHTER) watches the sea
/// for her parents and takes the message (FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED).
/// Two stacks of crates slide along the pier under Move, a chest holds a Potion
/// and a box 35 coins, and the docks lead to the World Map toward Tolbi
/// (TOLBI_DOCKS_TO_WORLD_MAP), back onto the ship
/// (TOLBI_DOCKS_TO_TOLBI_SHIP_DECK) and into the building beside them (
/// step trigger 10). After Colosso the Colosso Tour's passengers are stranded
/// here, the ships not sailing, and the first time the party comes (step
/// trigger 50, FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED) it watches them demand to be
/// taken home and the tour guide's hired bodyguard knock the angriest flat;
/// KalayTolbiDocks_PrepareTolbiDocks keeps them where the event left them. Only
/// the debug menus' scene selector, which exits to any scene and entrance
/// by number, reaches KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO, which sets
/// FLAG_COLOSSO_OVER as Babi Palace's BABI_PALACE_AFTER_COLOSSO does.

#include "common.h"
#include "djinn_ids.h"
#include "version.h"
#include "unit/scene/kalay_tolbi_docks.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "tbs_edition.h"
#include "kalay_tolbi_docks.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "event_layer.h"
#include "call.h"
#include "event_inline.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "actor_script.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// Kalay Docks' people, numbering the rows of
/// gKalayTolbiDocks_KalayActorTemplates from ACTOR_FIRST_PLACED;
/// Garet's row takes his own actor slot. Once the ship has crossed
/// (gKalayTolbiDocks_KalayActorTemplatesShipGone) the first four stand
/// off the map.
enum KalayDocksActorId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who walks
  /// over to tell Garet the Karagol is no real ocean.
  /// ★ Named from the game: "Do you want to go to Tolbi?"
  /// (MsgKalayWantGoToTolbi)
  ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "We can't take Silk Road, so everyone's in a
  /// pickle." (MsgKalayTolbiDocksWeCantTakeSilkRoadSo)
  ACTOR_WOMAN_IN_A_PICKLE,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), whose thoughts
  /// tell of a daughter who left Tolbi to meet him at the docks.
  /// ★ Named from the game: "You can't take Silk Road much further than
  /// this." (MsgKalayTolbiDocksYouCantTakeSilkRoadMuch)
  ACTOR_MAN_AT_SILK_ROADS_END,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS) whose daughter
  /// waits on the Tolbi Docks.
  /// ★ Named from the game: "When you reach Tolbi, please pass a message to
  /// my daughter." (MsgKalayPassMessageDaughter)
  ACTOR_WOMAN_WITH_A_MESSAGE,
  /// A man in a red bandana (SPRITE_MAN_IN_RED_BANDANA) who takes the tickets
  /// at the ship's plank. "Tickets, please."
  /// ★ Named from the game: "Tickets, please. Please show your tickets."
  /// (MsgKalayTicketsPlease)
  ACTOR_CHECKING_TICKETS,
  /// A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA).
  /// ★ Named from the game: "I'm glad I only work on the docks"
  /// (MsgKalayTolbiDocksImGladIOnlyWorkOn)
  ACTOR_DOCK_WORKER,
  /// A Venus Djinni, placed until it joins.
  /// ★ Named from the game: "Ground" (MsgDjinnName)
  ACTOR_GROUND
};

/// Tolbi Docks' people after the crossing, numbering the rows of
/// gKalayTolbiDocks_TolbiActorTemplates from ACTOR_FIRST_PLACED.
enum TolbiDocksActorId {
  /// A man in a yellow bandana (SPRITE_MAN_IN_YELLOW_BANDANA) who hears of
  /// the party's scary trip; after Colosso, "There are many here who want to
  /// return to Kalay since Colosso is over."
  /// ★ Named from the game: "I hear you had a scary trip."
  /// (MsgKalayHearScaryTrip)
  ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP = ACTOR_FIRST_PLACED,
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA); after Colosso,
  /// "Hanging around here like this and staring out at the sea is boring!"
  /// ★ Named from the game: "I think we're done sailing for a while."
  /// (MsgKalayTolbiDocksSighIThinkWereDoneSailing)
  ACTOR_MAN_DONE_SAILING,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "The tourists look a little upset. Did they get
  /// seasick?" (MsgKalayTouristsLookUpset)
  ACTOR_YOUNG_MAN_ASKING_IF_SEASICK,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who watches the sea
  /// for her parents.
  /// ★ Named from the game: "Silk Road was blocked, so I thought my parents
  /// might come by ship." (MsgKalaySilkRoadBlocked)
  ACTOR_WAITING_DAUGHTER,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "If you're going to Tolbi, keep heading west."
  /// (MsgKalayTolbiDocksIfYoureGoingToTolbiKeep)
  ACTOR_WOMAN_POINTING_WEST,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "It's been quite some time since the Colosso
  /// Tour passengers left for Tolbi."
  /// (MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince)
  ACTOR_MAN_ON_THE_TOUR_LONG_GONE,
  /// The two stacks of wooden crates Move pushes along the pier: SPRITE_CRATES,
  /// the crates of the Bilibin Barricade (bilibin.c).
  /// ★ Named from what the player sees: a tall stack of wooden crates on the
  /// pier (its render)
  ACTOR_FIRST_CRATES,
  /// ★ Named from what the player sees: the second stack of wooden crates
  /// (its render)
  ACTOR_SECOND_CRATES
};

/// Tolbi Docks once the Colosso Tour's passengers are stranded there: the rows
/// of gKalayTolbiDocks_TolbiActorTemplatesStranded that hold other people
/// than gKalayTolbiDocks_TolbiActorTemplates. The man who hears of the
/// scary trip, the one done sailing and the crates keep their rows.
enum TolbiDocksStrandedActorId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), the
  /// passenger who charges the tour guide and is knocked flat.
  /// ★ Named from the game: "To heck with you! I'm staying right here until
  /// the ship leaves!" (MsgKalayTolbiDocksToHeckWithYouImStaying)
  ACTOR_ANGRY_YOUNG_MAN = ACTOR_FIRST_PLACED + 2,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Sigh. I wonder when we'll be able to return to
  /// Kalay." (MsgKalayTolbiDocksSighIWonderWhenWellBe)
  ACTOR_STRANDED_WOMAN,
  /// The other guide, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) as
  /// the tour guide is: "it's not our fault the ship won't sail, so we can't
  /// offer any refunds."
  /// ★ Named from the game: "Tour groups should take care of you from start to
  /// finish" (MsgKalayTolbiDocksTourGroupsShouldTakeCareOf)
  ACTOR_SECOND_TOUR_GUIDE,
  /// Refunded the angry young man's ticket.
  /// ★ Named from the game: "I returned the cost of your ticket to you."
  /// (MsgKalayReturnedTicketCost)
  ACTOR_TOUR_GUIDE,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN) hired by the tour guides.
  /// ★ Named from the game: "I might as well make some cash as a bodyguard."
  /// (MsgKalayTolbiDocksAsLongAsWereStuckHere)
  ACTOR_BODYGUARD = ACTOR_FIRST_PLACED + 8,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE). "Sigh. If I knew it
  /// would turn out like this, we wouldn't have come."
  /// ★ Named from the game: "Sigh. If I knew it would turn out like this"
  /// (MsgKalayTolbiDocksSighIfIKnewItWould)
  ACTOR_SIGHING_PASSENGER
};

/// The people of the two dock rooms, numbering the rows of
/// gKalayTolbiDocks_InteriorActorTemplates from ACTOR_FIRST_PLACED: the
/// Kalay room's three, then the Tolbi room's. Once the ship has crossed
/// (gKalayTolbiDocks_InteriorActorTemplatesShipGone) the Kalay room's two
/// travellers stand off the map.
enum KalayTolbiDocksInteriorActorId {
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) in the Kalay room.
  /// ★ Named from the game: "Are you looking for tickets for boarding?"
  /// (MsgKalayLookingForTickets)
  ACTOR_SELLING_TICKETS = ACTOR_FIRST_PLACED,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "Isn't the replacement ship here yet?"
  /// (MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement)
  ACTOR_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP,
  /// An old woman (SPRITE_OLD_WOMAN), who wonders whether her husband is
  /// giving up on Colosso.
  /// ★ Named from the game: "Once we've crossed the Karagol, we'll reach
  /// Tolbi. Ohh, I'm excited."
  /// (MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell)
  ACTOR_OLD_WOMAN_EXCITED_FOR_TOLBI,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who sells no
  /// tickets in the Tolbi room. "I'm sorry, but we just can't sell any
  /// tickets right now."
  /// ★ Named from the game: "we just can't sell any tickets right now."
  /// (MsgKalayTolbiDocksImSorryButWeJustCant)
  ACTOR_SELLING_NO_TICKETS,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "We rented out our house for Colosso, and now
  /// the ship's not launching!" (MsgKalayTolbiDocksWeRentedOutOurHouseFor)
  ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE,
  /// An old woman (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "If the ship won't sail, where will we sleep?"
  /// (MsgKalayTolbiDocksWeRentedOutOurHouseIf)
  ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP
};

/// The dock rooms after Colosso
/// (gKalayTolbiDocks_InteriorActorTemplatesStranded): the Tolbi room's
/// last two rows hold other people.
enum KalayTolbiDocksInteriorStrandedActorId {
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). "Some rich
  /// guy was here throwing money around, but they still said no."
  /// ★ Named from the game: "Yeah...the ship's not going anywhere for now."
  /// (MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere)
  ACTOR_MAN_ON_THE_SHIP_GOING_NOWHERE = ACTOR_FIRST_PLACED + 4,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "The Karagol Sea looks so calm from here."
  /// (MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm)
  ACTOR_WOMAN_SAYING_THE_SEA_LOOKS_CALM
};

static void KalayTolbiDocks_PrepareKalayDocks(void);
static void KalayTolbiDocks_PrepareTolbiDocks(void);
static void KalayTolbiDocks_PrepareInterior(void);
static void KalayTolbiDocks_UpdateCrateCells(void);
static void KalayTolbiDocks_SettleCrates(s32 actorId);
static void KalayTolbiDocks_GaretSeesTheKaragol(void);

/// Kalay Docks: talking to Ground, a Venus Djinni, makes it join the party.
static void KalayTolbiDocks_GroundJoins(void) {
  Djinn_JoinParty(ACTOR_GROUND, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
}

/// Selects the entrances of Kalay Docks, Tolbi Docks or the rooms
/// on CottageRoomsE.
const struct SceneEntrance *KalayTolbiDocks_GetEntrances(void) {
  s16 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_KalayDocks) {
    return gKalayTolbiDocks_KalayEntrances;
  }
  if (sceneId == (s32)&SceneId_TolbiDocks) {
    return gKalayTolbiDocks_TolbiEntrances;
  }
  if (sceneId == (s32)&SceneId_CottageRoomsE) {
    return gKalayTolbiDocks_InteriorEntrances;
  }
  return gKalayTolbiDocks_NoEntrances;
}

const struct SceneExitRegion *KalayTolbiDocks_GetRegions(void) { return NULL; }

/// The exits of the three scenes, each scene's after its header word (an exit
/// of kind 0): both docks lead to the World Map, onto the ship
/// (SceneId_TolbiShipDeck) and into their rooms
/// (SceneId_CottageRoomsE), which lead back to their docks.
const u32 *KalayTolbiDocks_GetExits(void) { return gKalayTolbiDocks_Exits; }

/// Each scene's people before the crossing, once the ship has crossed and not
/// come back (FLAG_TOLBI_SHIP_REACHED_TOLBI), and once the Colosso Tour's
/// passengers are stranded after Colosso (FLAG_COLOSSO_OVER).
const struct ActorTemplate *KalayTolbiDocks_GetActorTemplates(void) {
  s16 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_KalayDocks) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayActorTemplatesShipGone;
    }
    return gKalayTolbiDocks_KalayActorTemplates;
  }

  if (sceneId == (s32)&SceneId_TolbiDocks) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiActorTemplatesStranded;
    }
    return gKalayTolbiDocks_TolbiActorTemplates;
  }

  if (sceneId == (s32)&SceneId_CottageRoomsE) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_InteriorActorTemplatesStranded;
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_InteriorActorTemplatesShipGone;
    }
    return gKalayTolbiDocks_InteriorActorTemplates;
  }

  return gKalayTolbiDocks_NoActorTemplates;
}

/// Tolbi Docks, stepping off the ship (step trigger 20, until
/// FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, which coming off the ship by
/// TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK clears): the leader walks a few steps and
/// a man on the docks hurries over to stand beside the party.
static void KalayTolbiDocks_GreetArrivals(void) {
  Event_Start();
  GameFlag_Set(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 392, 296);
  Actor_SetSpeed(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 0x13333, 0x9999);
  Actor_WalkToAndWait(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 408, 296);
  Actor_FaceDirection(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, FACING_WEST, 0);
  Event_WaitTicks(20);
  Event_End();
}

/// Selects each scene's events for the same three times as its people.
const struct Trigger *KalayTolbiDocks_GetTriggers(void) {
  s16 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_KalayDocks) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayTriggersShipGone;
    }
    return gKalayTolbiDocks_KalayTriggers;
  }

  if (sceneId == (s32)&SceneId_TolbiDocks) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiTriggersStranded;
    }
    return gKalayTolbiDocks_TolbiTriggers;
  }

  if (sceneId == (s32)&SceneId_CottageRoomsE) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_InteriorTriggersStranded;
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_InteriorTriggersShipGone;
    }
    return gKalayTolbiDocks_InteriorTriggers;
  }

  return gKalayTolbiDocks_NoTriggers;
}

/// Kalay Docks: a young man asks whether the party wants to go to Tolbi. Yes,
/// and he says that past Silk Road's end the way is across the Karagol Sea; no,
/// and he suggests a long look around the ship.
static void KalayTolbiDocks_AskAboutTolbiCrossing(void) {
  Event_Start();
  Event_SetMessageId(MsgKalayWantGoToTolbi);
  Event_AskYesNoAndShowResponse(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0);
  Event_End();
}

/// Kalay Docks: a woman asks whether the party is taking the ship. If it is,
/// she asks it to tell her daughter in Tolbi that the tickets were too
/// expensive for the daughter's mother to come (sets
/// FLAG_KALAY_DOCKS_CARRYING_MESSAGE); afterwards she repeats the message.
static void KalayTolbiDocks_TalkToWomanWithAMessage(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) == 0) {
    Event_SetMessageId(MsgKalayGoingTakeShip);
    Event_OpenMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_ShowMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
      GameFlag_Set(FLAG_KALAY_DOCKS_CARRYING_MESSAGE);
    } else {
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
    }
  } else {
    Event_SetMessageId(MsgKalayPassMessageDaughter);
    Event_ShowMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
  }
  Event_End();
}

/// Kalay Docks: spoken to on the shore after the party has handed over its
/// ticket and gone ashore, the ticket taker asks it to board the ship when it
/// is finished.
static void KalayTolbiDocks_RemindBoardShip(void) {
#if !TBS_PATCH_3
  ((void)Actor_GetById(ACTOR_PARTY_LEADER));
  Event_Start();
#endif
  if (GameFlag_Get(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_Get(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
#if TBS_PATCH_3
      Event_Start();
#endif
      Event_SetMessageId(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(ACTOR_CHECKING_TICKETS, 0);
      Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
#if TBS_PATCH_3
      Event_End();
#endif
    }
  }
}

/// Kalay Docks: the ticket taker at the ship's plank. Facing him from the east,
/// the party is asked for its tickets: with one bought he steps aside, takes it
/// (sets FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) and lets the party aboard.
/// Once aboard, he offers to let the party go ashore again
/// (FLAG_KALAY_DOCKS_WENT_ASHORE).
static void KalayTolbiDocks_TalkToTicketTaker(void) {
  s32 ticketHolder;
  s32 ticketSlot;
  struct Actor *leader;
  s16 cardinalFacing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cardinalFacing = ((leader->facing + 0x2000) & ~0x3fff);
  Event_Start();
  if (Event_GetFlag(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (Event_GetFlag(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
      Event_SetMessageId(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(ACTOR_CHECKING_TICKETS, 0);
    } else {
      Event_SetMessageId(MsgKalayWantGoAshore);
      Event_OpenMessage(ACTOR_CHECKING_TICKETS, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
        Event_WaitTicks(10);
        Event_SetMessageId(MsgKalayWantGoAshore + 1);
        Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
        Actor_WalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1288);
        Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
        Event_WaitTicks(20);
        GameFlag_Set(FLAG_KALAY_DOCKS_WENT_ASHORE);
      } else {
        Event_SetMessageId(MsgKalayWantGoAshore + 2);
        Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
      }
    }
  } else {
    if ((u16)cardinalFacing != FACING_WEST) {
      return;
    }
    Event_SetMessageId(MsgKalayTicketsPlease);
    Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
    // Hand over the actual inventory ticket before walking past the gate.
    if (GameFlag_Get(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
      ticketHolder = PartyInventory_FindMember(ITEM_BOAT_TICKET);
      ticketSlot = Inventory_Find(ticketHolder, ITEM_BOAT_TICKET);
      Actor_SetAnimationAndWait(ACTOR_CHECKING_TICKETS, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1288);
      Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
      Inventory_Discard(ticketHolder, ticketSlot);
      GameFlag_Set(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel, 1304);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 1304);
      Actor_WalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1304);
      Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_EAST, 0);
    } else {
      Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
    }
  }
  Event_End();
}

/// Kalay Docks, once the tour's group has left Kalay: the dock worker explains
/// the tickets: hand one over to board, give it to the ticket taker, or wait
/// on board for the ship to sail.
static void KalayTolbiDocks_TalkToDockWorker(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    Event_SetMessageId(MsgKalayBoardWaitSetSail);
    FieldMessage_ShowAtActor(ACTOR_DOCK_WORKER, 0);
  } else if (GameFlag_Get(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Event_SetMessageId(MsgKalayHaveTicketsGiveHim);
    Event_ShowMessageAndWait(ACTOR_DOCK_WORKER, 0);
  } else {
    Event_SetMessageId(MsgKalayAfterHandingOverTicket);
    Event_ShowMessageAndWait(ACTOR_DOCK_WORKER, 0);
  }

  Event_End();
}

/// The room at Kalay Docks, until the ship has crossed: the ticket seller
/// offers tickets for the four of them at 600 coins, with the party's coins
/// shown beside the question. Declining, the leader shakes his head; without
/// enough coins the seller refuses; otherwise the party gets its ticket and
/// pays (sets FLAG_KALAY_DOCKS_TICKET_BOUGHT).
static void KalayTolbiDocks_TalkToTicketSeller(void) {
  struct Window *coinsWindow;
  struct Common *state;
  s32 ticketPrice;

#if !TBS_PATCH_1
  /* FAKEMATCH(temporary): coinLabel keeps the first label in r5 and steps it
   * to the second; as two MSGNO arguments, 24 of 152 instructions differ in
   * tbs-ja. */
  s32 coinLabel;
#endif

  ticketPrice = 600;
  Event_Start();
  if (Event_GetFlag(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Event_SetMessageId(MsgKalayHandInTicketPlank);
    Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
  } else {
    Event_SetMessageId(MsgKalayLookingForTickets);
    Event_OpenMessage(ACTOR_SELLING_TICKETS, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      FieldEvent_ShowMessageThenWait(ACTOR_SELLING_TICKETS, 0, 10);
    } else {
      gFieldWork->message++;
      MessageArgument_Push(ticketPrice, 5);
      Event_OpenMessage(ACTOR_SELLING_TICKETS, 0);
      // Keep the balance visible while the player confirms the purchase.
      coinsWindow =
          Window_Create(TICKET_COUNTER_WINDOW_X, 8, TICKET_COUNTER_WIDTH, 4, 2);
#if TBS_PATCH_1
      Text_DrawMessageAsTiles(MSGNO(MsgYourCoins), coinsWindow, 0, 0);
#else
      coinLabel = MSGNO(MsgYourCoins);
      Text_DrawMessageAsTiles(coinLabel, coinsWindow, 0, 0);
#endif
      state = &gCommon;
#if LANGUAGE == LANGUAGE_JAPANESE
      coinLabel += MSGNO(MsgCoins) - MSGNO(MsgYourCoins);
#endif
      Text_DrawNumberInWindow(state->coins, 6, coinsWindow, TICKET_COUNTER_X,
                              8);
#if LANGUAGE == LANGUAGE_JAPANESE
      Text_DrawMessageAsTiles(coinLabel, coinsWindow, 48, 8);
#endif
      if (Event_AskYesNoWithActorAnimation(-1, 0) == 1) {
        Window_Close(coinsWindow, 2);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
        Event_WaitTicks(10);
        Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
      } else if ((u32)ticketPrice > (u32)state->coins) {
        Window_Close(coinsWindow, 2);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Event_WaitTicks(10);
        gFieldWork->message++;
        Event_PlaySong(SE_MENU_CANCEL);
        Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
      } else {
        Window_Close(coinsWindow, 2);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Event_WaitTicks(10);
        gFieldWork->message += 3;
        Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
        // Complete both inventory and coin changes only after confirmation.
        PartyInventory_GiveItem(ITEM_BOAT_TICKET, 0);
        Event_SetFlag(FLAG_KALAY_DOCKS_TICKET_BOUGHT);
        Party_AdjustCoins(-ticketPrice);
      }
    }
    Event_End();
  }
}

/// Tolbi Docks: a man on the docks has heard the party was attacked by a
/// monster on the crossing, and asks whether that is true; told it was nothing,
/// he will not believe they fought the monsters alone, for Kaja killed the
/// Kraken.
static void KalayTolbiDocks_AskAboutRoughCrossing(void) {
  Event_Start();
  Event_SetMessageId(MsgKalayHearScaryTrip);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 0);
  Event_End();
}

/// Tolbi Docks: a young man asks whether the upset tourists got seasick.
static void KalayTolbiDocks_AskAboutSeasickTourists(void) {
  Event_Start();
  Event_SetMessageId(MsgKalayTouristsLookUpset);
  Event_AskYesNoAndShowResponse(ACTOR_YOUNG_MAN_ASKING_IF_SEASICK, 0);
  Event_End();
}

/// Tolbi Docks: the daughter looks for her parents among the passengers. If the
/// party carries the message from her parents she asks for it and thanks them
/// once it is given; otherwise she turns back to the sea.
static void KalayTolbiDocks_TalkToWaitingDaughter(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED) != 0) {
    Actor_FaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
    Event_WaitTicks(20);
    Event_SetMessageId(MsgKalayThanksMessageParents);
    Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
    Event_End();
  } else {
    Event_WaitTicks(20);
    Actor_ShowEmote(ACTOR_WAITING_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    50);
    Actor_FaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
    Event_WaitTicks(20);
    Event_SetMessageId(MsgKalaySilkRoadBlocked);
    Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
    if (GameFlag_Get(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) != 0) {
      Event_WaitTicks(20);
      Actor_ShowEmote(ACTOR_WAITING_DAUGHTER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
      Event_OpenMessage(ACTOR_WAITING_DAUGHTER, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
        Event_WaitTicks(20);
        Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
        GameFlag_Set(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED);
      } else {
        Event_WaitTicks(10);
        gFieldWork->message++;
        Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
        Event_WaitTicks(10);
        Actor_FaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
        Event_WaitTicks(30);
      }
    } else {
      Event_WaitTicks(10);
      Actor_FaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
      Event_WaitTicks(30);
    }
    Event_End();
  }
}

/// Kalay Docks or Tolbi Docks: walking north into the door (step trigger 10) of
/// the building beside them, the door opens and the leader walks in by
/// TOLBI_DOCKS_TO_COTTAGE_ROOMS_E, which in Kalay Docks is
/// KALAY_DOCKS_TO_COTTAGE_ROOMS_E by the same number.
static void KalayTolbiDocks_OpenDoorAndLeave(void) {
  Event_Start();
  Event_PlaySong(SE_DOOR_OPEN);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  if (gCommon.sceneId == (s32)&SceneId_KalayDocks) {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 1392);
    MapGrid_AnimateCells(gKalayTolbiDocks_KalayDoorCells, 78, 86);
  } else {
    if (gCommon.sceneId == (s32)&SceneId_TolbiDocks) {
      Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
      MapGrid_AnimateCells(gKalayTolbiDocks_TolbiDoorCells, 74, 9);
    }
  }
  Event_WaitTicks(16);
  Import_EventSetExitRequest(TOLBI_DOCKS_TO_COTTAGE_ROOMS_E);
  Event_End();
}

/// An entrance no exit, table or code leads to, whose arrival sets
/// FLAG_COLOSSO_OVER; the debug menus' scene selector reaches it.
enum { KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO = 90 };

/// Prepares whichever of the three scenes is entered. Arriving by
/// KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO sets FLAG_COLOSSO_OVER, which
/// leaves the tour's passengers stranded on the Tolbi Docks.
s32 KalayTolbiDocks_Initialize(void) {
  if (gCommon.entrance == KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO) {
    GameFlag_Set(FLAG_COLOSSO_OVER);
  }
  if (gCommon.sceneId == (s32)&SceneId_KalayDocks) {
    KalayTolbiDocks_PrepareKalayDocks();
  } else if (gCommon.sceneId == (s32)&SceneId_TolbiDocks) {
    KalayTolbiDocks_PrepareTolbiDocks();
  } else if (gCommon.sceneId == (s32)&SceneId_CottageRoomsE) {
    KalayTolbiDocks_PrepareInterior();
  }
  return 0;
}

/// On the first arrival at Kalay Docks from the World Map, Garet runs to see
/// the Karagol. Coming back off the ship clears the shore leave, and once the
/// tour's group has left Kalay the ticket taker stands at the ship's plank
/// until the party goes ashore.
static void KalayTolbiDocks_PrepareKalayDocks(void) {
  if (gCommon.entrance == KALAY_DOCKS_FROM_WORLD_MAP_SOUTH) {
    if (Event_GetFlag(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL) == 0) {
      GameFlag_Set(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL);
      KalayTolbiDocks_GaretSeesTheKaragol();
    }
  }
  if (gCommon.entrance == KALAY_DOCKS_FROM_TOLBI_SHIP_DECK) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      GameFlag_Clear(FLAG_KALAY_DOCKS_WENT_ASHORE);
    }
  }
  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_Get(FLAG_KALAY_DOCKS_WENT_ASHORE) == 0) {
      Actor_SetPosition(ACTOR_CHECKING_TICKETS, PIXELS(88), PIXELS(1304));
      Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_EAST, 0);
    }
  }
}

/// On Tolbi Docks the two stacks of crates settle into the pier's cells. The
/// passenger who met the party stays beside the ship, and after the
/// confrontation the guide, the bodyguard and the two passengers keep where it
/// left them, the angry one still lying on the ground.
static void KalayTolbiDocks_PrepareTolbiDocks(void) {
  struct Actor *actor;
  u8 zero;

  KalayTolbiDocks_UpdateCrateCells();
  if (Event_GetFlag(FLAG_COLOSSO_OVER)) {
    Actor_ConfigureSpriteColor(ACTOR_SECOND_TOUR_GUIDE, SPRITE_COLOR_RBG);
  }
  if (gCommon.entrance == TOLBI_DOCKS_FROM_COTTAGE_ROOMS_E) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  if (gCommon.entrance == TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK) {
    GameFlag_Clear(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
  }
  if (GameFlag_Get(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED)) {
    Actor_SetPosition(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, PIXELS(408),
                      PIXELS(296));
    Actor_FaceDirection(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, FACING_WEST, 0);
  }
  if (GameFlag_Get(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED)) {
    Actor_SetPosition(ACTOR_TOUR_GUIDE, PIXELS(280), PIXELS(296));
    Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTH, 0);
    Actor_SetPosition(ACTOR_BODYGUARD, PIXELS(288), PIXELS(280));
    Actor_FaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
    Actor_SetPosition(ACTOR_ANGRY_YOUNG_MAN, PIXELS(232), PIXELS(304));
    Actor_FaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_SOUTH, 0);
    Actor_SetPosition(ACTOR_STRANDED_WOMAN, PIXELS(240), PIXELS(312));
    Actor_FaceDirection(ACTOR_STRANDED_WOMAN, FACING_NORTH, 0);
    actor = Actor_GetById(ACTOR_ANGRY_YOUNG_MAN);
    actor->collisionFlags = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[38] = 0;
    zero = 0;
    actor->sprite->rotation = 0xc000;
    actor = Actor_GetById(ACTOR_STRANDED_WOMAN);
    actor->priorityFlags = zero;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[21] |= 12;
  }
  if (GameFlag_Get(FLAG_COLOSSO_OVER)) {
    MapGrid_CopyCellAttributes(18, 18, 1, 1, 14, 18);
    MapGrid_CopyCellAttributes(18, 18, 1, 1, 15, 18);
  }
}

/// The rooms at the docks open with a window transition and no area
/// name.
static void KalayTolbiDocks_PrepareInterior(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}

/// Tolbi Docks, after Move pushes either stack of crates: the pier's cells
/// under both are rebuilt.
static void KalayTolbiDocks_UpdateCrateCells(void) {
  s32 firstCratesColumn;
  s32 secondCratesColumn;

  firstCratesColumn = Actor_GetById(ACTOR_FIRST_CRATES)->x.fixed >> 20;
  secondCratesColumn = Actor_GetById(ACTOR_SECOND_CRATES)->x.fixed >> 20;

  MapGrid_CopyCellAttributes(5, 12, 5, 1, 5, 11);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, secondCratesColumn, 11);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, firstCratesColumn, 11);

  KalayTolbiDocks_SettleCrates(ACTOR_FIRST_CRATES);
  KalayTolbiDocks_SettleCrates(ACTOR_SECOND_CRATES);
}

/// Stops a stack of crates' idle motion and sets its sprite priority.
static void KalayTolbiDocks_SettleCrates(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);

  Actor_SetSpriteFlags(Actor_GetById(actorId), 0);
  Actor_SetSpritePriority(actorId, 3);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/// Kalay Docks, the first entry from the World Map
/// (KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, until
/// FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL): Garet runs to the edge of the docks and
/// marvels at the sea. "Incredible! So this is the ocean Kraden was talking
/// about!" Kraden is the scholar who has told Garet of the ocean. The man who
/// asks travellers about Tolbi (ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI) walks
/// over, shaking his head: the Karagol is surrounded by land, so it is not a
/// real ocean. Garet means to see a real one someday, nods with the leader and
/// rejoins the party.
static void KalayTolbiDocks_GaretSeesTheKaragol(void) {
  struct Actor *actor;

  Event_Start();
  Field_EndBusyAndReveal();
  Actor_SetPosition(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, PIXELS(328),
                    PIXELS(1424));
  Actor_GetById(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI)->paused = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitTicks(20);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 0, FACING_WEST);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Event_WaitTicks(20);
  // Follow Garet to the water before the man joins him.
  Event_SetMessageId(MsgKalayIncredibleOcean);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_GARET, 0x19999, 0xcccc);
  Actor_WalkToAndWait(ACTOR_GARET, 232, 1424);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(1440), 1);
  FieldCamera_WaitForArrival();
  Event_WaitTicks(10);
  Actor_Jump(ACTOR_GARET, 6, 15);
  Actor_Jump(ACTOR_GARET, 6, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitTicks(20);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(1448), 1);
  FieldCamera_WaitForArrival();
  Event_WaitTicks(20);
  Actor_ShowEmote(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_SetSpeed(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0x13333, 0x9999);
  Actor_WalkToAndWait(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 264, 1424);
  Actor_FaceDirection(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, FACING_WEST, 0);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_WaitTicks(20);
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI,
                            ANIM_SHAKE_HEAD);
  Event_WaitTicks(10);
  Event_ShowMessage(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0);
  Event_WaitTicks(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_WaitTicks(30);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Event_WaitTicks(50);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitTicks(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_WaitTicks(30);
  Actor_SetSpeed(ACTOR_GARET, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_GARET, 264, 1464);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, FACING_SOUTH, 0);
  Event_WaitTicks(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitTicks(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitTicks(30);
  Actor_SetSpeed(ACTOR_GARET, 0x13333, 0x9999);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  // Garet rejoins the leader and the man wanders again from where he stands.
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_WaitTicks(20);
  Actor_GetById(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI)->paused = 0;
  Actor_SetMovementTypeById(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 2);
  actor = Actor_GetById(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI);
  {
    s32 pixelX = actor->x.fixed / 0x10000;

    actor->timer = pixelX;
  }
  {
    s32 pixelZ = actor->z.fixed / 0x10000;

    actor->userValue = pixelZ;
  }
  Event_End();
}

/// Tolbi Docks after Colosso, the first time the party comes to the pier (
/// step trigger 50, until FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED): it watches the
/// tour guide tell an angry young man his ticket was refunded. The passengers
/// demand to be taken home, the ships being stuck and the road blocked; when
/// the angry one charges, the guides' hired bodyguard steps in, and after a
/// second try the passenger is knocked flat and the bodyguard spins on the
/// spot.
static void KalayTolbiDocks_PassengersConfrontTourGuide(void) {
  struct Actor *actor;

  GameFlag_Set(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgKalayReturnedTicketCost);
  actor = Actor_GetById(ACTOR_STRANDED_WOMAN);
  actor->priorityFlags = 0;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[21] |= 12;
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(304), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  FieldCamera_WaitForArrival();
  Event_WaitTicks(20);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Event_WaitTicks(10);
  Actor_ShowEmote(ACTOR_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
  Actor_FaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_EAST, 0);
  Actor_Jump(ACTOR_ANGRY_YOUNG_MAN, 4, 13);
  Actor_Jump(ACTOR_ANGRY_YOUNG_MAN, 4, 30);
  Event_ShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
  Event_WaitTicks(10);
  Actor_RunRepeatedMotion(ACTOR_STRANDED_WOMAN, 2);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_STRANDED_WOMAN, 0);
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  Event_ShowMessageAndWait(ACTOR_TOUR_GUIDE, 0);
  Event_WaitTicks(10);
  Actor_ShowEmote(ACTOR_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 55);
  Actor_SetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0x20000, 0x10000);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 16, 0);
  Actor_Jump(ACTOR_ANGRY_YOUNG_MAN, 7, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 24, 0);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_ANGRY_YOUNG_MAN, -16, 0);
  Event_PlaySong(SE_HIGH_JUMP);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x26666, 0x13333);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, 16, 0);
  Event_WaitTicks(10);
  Import_ActorSetAnimation(ACTOR_ANGRY_YOUNG_MAN, ANIM_STAND);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(ACTOR_TOUR_GUIDE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
  Event_PlaySong(SE_SWITCH);
  Event_WaitTicks(10);
  Event_PlaySong(SE_SWITCH);
  Event_WaitTicks(10);
  Event_PlaySong(SE_SWITCH);
  Event_WaitTicks(10);
  Event_WaitTicks(20);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x6666, 0x3333);
  Actor_Jump(ACTOR_TOUR_GUIDE, 6, 0);
  Event_PlaySong(SE_DOOR_CLOSE);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, -8, 0);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
  Event_WaitTicks(10);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 0);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitTicks(30);
  Actor_SetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_NORTHEAST, 0);
  Event_WaitTicks(35);
  Actor_RunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTHEAST, 0);
  Event_WaitTicks(55);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitTicks(30);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_STRANDED_WOMAN, FACING_NORTHEAST, 0);
  Event_WaitTicks(20);
  Actor_ShowEmote(ACTOR_STRANDED_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessage(ACTOR_STRANDED_WOMAN, 0);
  Event_WaitTicks(10);
  Actor_RunRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTHWEST, 0);
  Event_WaitTicks(60);
  Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_WEST, 0);
  Event_WaitTicks(30);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_STRANDED_WOMAN, FACING_EAST, 0);
  Event_WaitTicks(20);
  Actor_RunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
  Actor_SetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0x13333, 0x9999);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 8, 0);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x20000, 0x10000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 16);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_WEST, 0);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Event_WaitTicks(10);
  Actor_ShowEmote(ACTOR_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -8, 0);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_WaitTicks(20);
  Actor_RunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Event_WaitTicks(10);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -16, 0);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_EAST, 0);
  Event_WaitTicks(20);
  Actor_SetAnimationAndWait(ACTOR_ANGRY_YOUNG_MAN, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  Actor_SetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 8, 0);
  Actor_Jump(ACTOR_ANGRY_YOUNG_MAN, 6, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 24, 0);
  Event_PlaySong(SE_IMPACT);
  Actor_Jump(ACTOR_BODYGUARD, 6, 0);
  // The passenger bounces off the spinning bodyguard and lands on his back.
  Actor_SetScript(ACTOR_BODYGUARD, gKalayTolbiDocks_BodyguardSpinScript);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Jump(ACTOR_ANGRY_YOUNG_MAN, 6, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -12, 4);
  actor = Actor_Get(ACTOR_ANGRY_YOUNG_MAN);
  actor->collisionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[38] = 0;
  actor->sprite->rotation = 0xc000;
  Actor_WalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -12, 4);
  Actor_FaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_SOUTH, 0);
  Actor_GetById(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_PlaySong(SE_DOOR_CLOSE);
  Event_WaitTicks(20);
  Actor_ShowEmote(ACTOR_STRANDED_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetSpeed(ACTOR_STRANDED_WOMAN, 0x18000, 0xc000);
  Actor_WalkByAndWait(ACTOR_STRANDED_WOMAN, 24, 0);
  Actor_FaceDirection(ACTOR_STRANDED_WOMAN, FACING_NORTH, 0);
  Event_WaitTicks(10);
  Event_ShowMessage(ACTOR_STRANDED_WOMAN, 0);
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Event_WaitTicks(20);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 0);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Event_WaitTicks(10);
  Actor_RunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
  Event_WaitTicks(20);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_EAST, 0);
  Event_WaitTicks(40);
  Actor_SetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
  Event_WaitTicks(20);
  Event_WaitTicks(10);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, 24, -24);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, 8, 0);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
  Event_WaitTicks(20);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, 0, -8);
  Event_WaitTicks(10);
  Event_End();
}

u16 gKalayTolbiDocks_KalayDoorCells[] = {
    MAPGRID_ANIMATION_STEP(98, 84, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(98, 82, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gKalayTolbiDocks_TolbiDoorCells[] = {
    MAPGRID_ANIMATION_STEP(92, 9, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(92, 7, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

s32 gKalayTolbiDocks_BodyguardSpinScript[] = {
    SC_ADD(SW_FACING, -0x2000),
    SC_WAIT(2),
    SC_LOOP(8, SC_TOP),
    SC_END,
};

struct SceneEntrance gKalayTolbiDocks_NoEntrances[] = {
    {0, CONDITION_ALWAYS, 128, 0, 128, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocks_KalayEntrances[] = {
    {0, CONDITION_ALWAYS, 248, 0, 1368, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 288, 0, 1464,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 72, 0, 1304,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_COTTAGE_ROOMS_E, CONDITION_ALWAYS, 304, 0, 1416,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 296, 0, 1048,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocks_TolbiEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 296, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 408, 0, 296,
     FACING_WEST, 0, 0, 0, 448, 432, 0},
    {TOLBI_DOCKS_FROM_WORLD_MAP, CONDITION_ALWAYS, 24, 0, 232, FACING_EAST, 0,
     0, 0, 448, 432, 0},
    {TOLBI_DOCKS_FROM_COTTAGE_ROOMS_E, CONDITION_ALWAYS, 240, 0, 216,
     FACING_SOUTH, 0, 0, 0, 448, 432, 0},
    {SCENE_TABLE_END},
};

/// CottageRoomsE's entrances, each into the room at the dock it is
/// entered from.
struct SceneEntrance gKalayTolbiDocks_InteriorEntrances[] = {
    {0, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {COTTAGE_ROOMS_E_FROM_KALAY_DOCKS, CONDITION_ALWAYS, 96, 0, 248,
     FACING_NORTH, 0, 32, 32, 272, 272, 0},
    {COTTAGE_ROOMS_E_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 384, 0, 248,
     FACING_NORTH, 0, 320, 32, 560, 272, 0},
    {SCENE_TABLE_END},
};

/// Kalay Docks' exits, then Tolbi Docks' and CottageRoomsE's, each after the
/// header word (an exit of kind 0) for its scene.
u32 gKalayTolbiDocks_Exits[] = {
    SCENE_EXITS(KalayDocks),
    SCENE_EXIT(KALAY_DOCKS_TO_WORLD_MAP_SOUTH, WorldMap,
               WORLD_MAP_FROM_KALAY_DOCKS_EAST),
    SCENE_EXIT(KALAY_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_KALAY_DOCKS),
    SCENE_EXIT(KALAY_DOCKS_TO_COTTAGE_ROOMS_E, CottageRoomsE,
               COTTAGE_ROOMS_E_FROM_KALAY_DOCKS),
    SCENE_EXIT(KALAY_DOCKS_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_KALAY_DOCKS_WEST),
    SCENE_EXITS(TolbiDocks),
    SCENE_EXIT(TOLBI_DOCKS_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI_DOCKS),
    SCENE_EXIT(TOLBI_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
    SCENE_EXIT(TOLBI_DOCKS_TO_COTTAGE_ROOMS_E, CottageRoomsE,
               COTTAGE_ROOMS_E_FROM_TOLBI_DOCKS),
    SCENE_EXITS(CottageRoomsE),
    SCENE_EXIT(COTTAGE_ROOMS_E_TO_KALAY_DOCKS, KalayDocks,
               KALAY_DOCKS_FROM_COTTAGE_ROOMS_E),
    SCENE_EXIT(COTTAGE_ROOMS_E_TO_TOLBI_DOCKS, TolbiDocks,
               TOLBI_DOCKS_FROM_COTTAGE_ROOMS_E),
    SCENE_EXITS_END,
};

struct ActorTemplate gKalayTolbiDocks_NoActorTemplates[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gKalayTolbiDocks_TolbiActorTemplates[] = {
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(392), 0, PIXELS(344), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayTolbiDocks_TolbiActorTemplatesStranded[] = {
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(296), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(344), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(304), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(320), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(304), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(288), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayTolbiDocks_KalayActorTemplates[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(1408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(1224), FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(1304), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(1336), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(1288), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(120), 0, PIXELS(1368), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { VENUS_DJINNI, 0x35, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayTolbiDocks_KalayActorTemplatesShipGone[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(1352), FACING_EAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(1288), FACING_WEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { VENUS_DJINNI, 0x35, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayTolbiDocks_InteriorActorTemplates[] = {
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(192), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayTolbiDocks_InteriorActorTemplatesShipGone[] = {
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayTolbiDocks_InteriorActorTemplatesStranded[] = {
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(200), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gKalayTolbiDocks_NoTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_KalayTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_OpenDoorAndLeave) },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_AskAboutTolbiCrossing) },
    { TRIGGER_TALK, ACTOR_WOMAN_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
    { TRIGGER_TALK, ACTOR_MAN_AT_SILK_ROADS_END, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
    { TRIGGER_TALK, ACTOR_WOMAN_WITH_A_MESSAGE, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_TalkToWomanWithAMessage) },
    { TRIGGER_TALK, ACTOR_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
    { TRIGGER_TALK, ACTOR_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
    { TALK_TRIGGER_FACING(FACING_EAST), ACTOR_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT(KalayTolbiDocks_TalkToTicketTaker) },
    { TALK_TRIGGER_FACING(FACING_WEST), ACTOR_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT(KalayTolbiDocks_TalkToTicketTaker) },
#if !TBS_PATCH_3
    { TRIGGER_TALK, ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_RemindBoardShip) },
#else
    { TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_CHECKING_TICKETS, IF_FLAG_SET(FLAG_KALAY_DOCKS_WENT_ASHORE), EVENT(KalayTolbiDocks_RemindBoardShip) },
#endif
    { TRIGGER_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_TalkToDockWorker) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_SILK_ROADS_END, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WITH_A_MESSAGE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
    { TRIGGER_TALK, ACTOR_GROUND, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_GroundJoins) },
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_KalayTriggersShipGone[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_OpenDoorAndLeave) },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
    { TRIGGER_TALK, ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
    { TRIGGER_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
    { TRIGGER_TALK, ACTOR_GROUND, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_GroundJoins) },
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_TolbiTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_OpenDoorAndLeave) },
    { TRIGGER_STEP, 20, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, EVENT(KalayTolbiDocks_GreetArrivals) },
    { TRIGGER_TALK, ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_AskAboutRoughCrossing) },
    { TRIGGER_TALK, ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_IF_SEASICK, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_AskAboutSeasickTourists) },
    { TRIGGER_TALK, ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { TRIGGER_TALK, ACTOR_WOMAN_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYoureGoingToTolbiKeep) },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_DAUGHTER, IF_FLAG_SET(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED), MSGNO(MsgKalayTolbiDocksItWasAHardCrossingVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_IF_SEASICK, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAfterSeeingTheLooksOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGetTheFeelingThatIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCrossedTheSeaJustFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGuessWeWontBeSeeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIThinkThatMissingShipMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CRATES, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_UpdateCrateCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_CRATES, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_UpdateCrateCells) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_TOLBI_DOCKS_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), 101, FLAG_TOLBI_DOCKS_COINS_BOX, FIND_COINS(35) },
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_TolbiTriggersStranded[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_OpenDoorAndLeave) },
    { TRIGGER_STEP, 50, FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED, EVENT(KalayTolbiDocks_PassengersConfrontTourGuide) },
    { TRIGGER_TALK, ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThereAreManyHereWhoWant) },
    { TRIGGER_TALK, ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHangingAroundHereLikeThisAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfBothTheCaptainAndKaja) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfKajaAndTheOthersSay) },
    { TRIGGER_TALK, ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTourGroupsShouldTakeCareOf) },
    { TRIGGER_TALK, ACTOR_ANGRY_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksToHeckWithYouImStaying) },
    { TRIGGER_TALK, ACTOR_STRANDED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIWonderWhenWellBe) },
    { TRIGGER_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellBeCountingOnYouIf) },
    { TRIGGER_TALK, ACTOR_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsWereStuckHere) },
    { TRIGGER_TALK, ACTOR_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIfIKnewItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfTheydJustLetTheShip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANGRY_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfImGoingToBeStuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksGrrrWhyDontYouGetUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsYoureHereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThisTourWasntWellOrganizedThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfGrandmaIsDoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CRATES, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_UpdateCrateCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_CRATES, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_UpdateCrateCells) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_TOLBI_DOCKS_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), 101, FLAG_TOLBI_DOCKS_COINS_BOX, FIND_COINS(35) },
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_InteriorTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_KALAY_DOCKS },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_TOLBI_DOCKS },
    { TRIGGER_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, EVENT(KalayTolbiDocks_TalkToTicketSeller) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, FLAG_KALAY_DOCKS_TICKET_BOUGHT, MSGNO(MsgKalayTolbiDocksIFeelSortOfBadThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYouveAlreadyBoughtYourTicket) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontLeaveHereTill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksLetsGetGoingImNotGetting) },
    { TRIGGER_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_InteriorTriggersShipGone[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_KALAY_DOCKS },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_TOLBI_DOCKS },
    { TRIGGER_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { TRIGGER_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { TRIGGERS_END },
};

struct Trigger gKalayTolbiDocks_InteriorTriggersStranded[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_KALAY_DOCKS },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_TOLBI_DOCKS },
    { TRIGGER_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { TRIGGER_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImStuckIWantToCross) },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere) },
    { TRIGGER_TALK, ACTOR_WOMAN_SAYING_THE_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveGotPassengersButNobodysSailing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheCaptainMustBeAStubborn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SAYING_THE_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheMarineBlueKaragolSeaIs) },
    { TRIGGERS_END },
};
// clang-format on
