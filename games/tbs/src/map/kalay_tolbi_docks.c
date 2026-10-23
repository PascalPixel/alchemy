/// @file
/// Kalay Docks and Tolbi Docks, the two ends of the ship's crossing of the
/// Karagol Sea, in three maps that share one overlay: MapId_KalayDocks,
/// MapId_KalayTolbiDocksInteriors and MapId_TolbiDocks. The getters pick
/// each map's tables by gGameState.mapId, and three story flags choose among
/// them: FLAG_TOLBI_SHIP_REACHED_TOLBI, whether the ship has made its crossing
/// (set on the world map, WorldMap_CompleteTransfer);
/// FLAG_KALAY_COLOSSO_TOUR_LEFT, whether the Colosso Tour's group has left
/// Kalay; and FLAG_COLOSSO_OVER, whether the Colosso finals have been fought.
///
/// Kalay Docks (KalayDocks): the first time the party walks in from the world
/// map (KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL)
/// KalayTolbiDocks_GaretSeesTheKaragol plays. Until the tour's
/// group has left Kalay, the ship is open to look around and the man at its
/// plank (LOCALID_CHECKING_TICKETS) checks no tickets; afterwards he stands in
/// the way and asks for one, takes it if the party has bought it
/// (FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) and lets the party aboard, and then
/// offers to let it ashore again until it boards
/// (FLAG_KALAY_DOCKS_WENT_ASHORE, cleared each time the party comes back off
/// the ship by KALAY_DOCKS_FROM_TOLBI_SHIP_DECK). A woman
/// (LOCALID_WOMAN_WITH_A_MESSAGE) asks the party to tell her daughter in Tolbi
/// that she cannot afford the tickets (FLAG_KALAY_DOCKS_CARRYING_MESSAGE).
/// She is the daughter's mother, and the man beside her
/// (LOCALID_MAN_AT_SILK_ROADS_END) her father: the Japanese text of
/// MsgKalayPassMessageDaughter has her name herself 母, "Mother", as she would
/// to her daughter, which the English renders `Her mother says that tickets
/// are too expensive`. Ground, a Venus Djinni (LOCALID_GROUND),
/// waits on the docks until it joins. Once the ship has crossed, the travellers
/// are gone and the dock hands wait for a ship that has not returned. The docks
/// lead to the world map (KALAY_DOCKS_TO_WORLD_MAP_SOUTH and
/// KALAY_DOCKS_TO_WORLD_MAP_NORTH), onto the ship
/// (KALAY_DOCKS_TO_TOLBI_SHIP_DECK, the Tolbi-bound Ship,
/// MapId_TolbiShipDeck) and through a door (coord event 10) into the building
/// beside them.
///
/// That building (KalayTolbiDocksInteriors) has a room at each dock: entered
/// from Kalay Docks (KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS) it is Kalay
/// Docks, from Tolbi Docks (KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS) it is
/// Tolbi Docks. In the Kalay room a woman sells the boarding ticket, 600 coins
/// for the four of them (FLAG_KALAY_DOCKS_TICKET_BOUGHT), until the ship has
/// sailed, when she says it will not leave again for the monsters at sea; in
/// the Tolbi room no tickets are sold at all, the sea being too dangerous.
///
/// Tolbi Docks (TolbiDocks) is where the ship lands after the crossing.
/// Stepping off (coord event 20, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED) the party
/// is met by a man on the docks who asks about the monster, with a reply for
/// each answer. The woman's daughter (LOCALID_WAITING_DAUGHTER) watches the sea
/// for her parents and takes the message (FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED).
/// Two stacks of crates slide along the pier under Move, a chest holds a Potion
/// and a box 35 coins, and the docks lead to the world map toward Tolbi
/// (TOLBI_DOCKS_TO_WORLD_MAP), back onto the ship
/// (TOLBI_DOCKS_TO_TOLBI_SHIP_DECK) and into the building (coord event 10).
/// After Colosso the Colosso Tour's passengers are stranded here, the ships not
/// sailing, and the first time the party comes (coord event 50,
/// FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED) it watches them demand to be taken home
/// and the tour guide's hired bodyguard knock the angriest flat;
/// KalayTolbiDocks_PrepareTolbiDocks keeps them where the scene left them. Only
/// the debug menus' map selector, which warps to any map and warp destination
/// by number, reaches KALAY_TOLBI_DOCKS_WARP_DEST_AFTER_COLOSSO, which sets
/// FLAG_COLOSSO_OVER as Babi Palace's BABI_PALACE_AFTER_COLOSSO does.
///
/// Open questions: none.

#include "game_state.h"
#include "djinn_ids.h"
#include "version.h"
#include "unit/map/kalay_tolbi_docks.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
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
#include "map_ids.h"
#include "types.h"
#include "script_layer.h"
#include "call.h"
#include "field_service.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// Kalay Docks' people, numbering the rows of
/// gKalayTolbiDocks_KalayObjectEventTemplates from LOCALID_FIRST_PLACED;
/// Garet's row takes his own object event slot. Once the ship has crossed
/// (gKalayTolbiDocks_KalayObjectEventTemplatesShipGone) the first four stand
/// off the map.
enum KalayDocksLocalId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who walks
  /// over to tell Garet the Karagol is no real ocean.
  /// ★ Named from the game: "Do you want to go to Tolbi?"
  /// (MsgKalayWantGoToTolbi)
  LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI = LOCALID_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "We can't take Silk Road, so everyone's in a
  /// pickle." (MsgKalayTolbiDocksWeCantTakeSilkRoadSo)
  LOCALID_WOMAN_IN_A_PICKLE,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), whose thoughts
  /// tell of a daughter who left Tolbi to meet him at the docks.
  /// ★ Named from the game: "You can't take Silk Road much further than
  /// this." (MsgKalayTolbiDocksYouCantTakeSilkRoadMuch)
  LOCALID_MAN_AT_SILK_ROADS_END,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS) whose daughter
  /// waits on the Tolbi Docks.
  /// ★ Named from the game: "When you reach Tolbi, please pass a message to
  /// my daughter." (MsgKalayPassMessageDaughter)
  LOCALID_WOMAN_WITH_A_MESSAGE,
  /// A man in a red bandana (SPRITE_MAN_IN_RED_BANDANA) who takes the tickets
  /// at the ship's plank. "Tickets, please."
  /// ★ Named from the game: "Tickets, please. Please show your tickets."
  /// (MsgKalayTicketsPlease)
  LOCALID_CHECKING_TICKETS,
  /// A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA).
  /// ★ Named from the game: "I'm glad I only work on the docks"
  /// (MsgKalayTolbiDocksImGladIOnlyWorkOn)
  LOCALID_DOCK_WORKER,
  /// A Venus Djinni, placed until it joins.
  /// ★ Named from the game: "Ground" (MsgDjinnName)
  LOCALID_GROUND
};

/// Tolbi Docks' people after the crossing, numbering the rows of
/// gKalayTolbiDocks_TolbiObjectEventTemplates from LOCALID_FIRST_PLACED.
enum TolbiDocksLocalId {
  /// A man in a yellow bandana (SPRITE_MAN_IN_YELLOW_BANDANA) who hears of
  /// the party's scary trip; after Colosso, "There are many here who want to
  /// return to Kalay since Colosso is over."
  /// ★ Named from the game: "I hear you had a scary trip."
  /// (MsgKalayHearScaryTrip)
  LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP = LOCALID_FIRST_PLACED,
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA); after Colosso,
  /// "Hanging around here like this and staring out at the sea is boring!"
  /// ★ Named from the game: "I think we're done sailing for a while."
  /// (MsgKalayTolbiDocksSighIThinkWereDoneSailing)
  LOCALID_MAN_DONE_SAILING,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "The tourists look a little upset. Did they get
  /// seasick?" (MsgKalayTouristsLookUpset)
  LOCALID_YOUNG_MAN_ASKING_IF_SEASICK,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who watches the sea
  /// for her parents.
  /// ★ Named from the game: "Silk Road was blocked, so I thought my parents
  /// might come by ship." (MsgKalaySilkRoadBlocked)
  LOCALID_WAITING_DAUGHTER,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "If you're going to Tolbi, keep heading west."
  /// (MsgKalayTolbiDocksIfYoureGoingToTolbiKeep)
  LOCALID_WOMAN_POINTING_WEST,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "It's been quite some time since the Colosso
  /// Tour passengers left for Tolbi."
  /// (MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince)
  LOCALID_MAN_ON_THE_TOUR_LONG_GONE,
  /// The two stacks of wooden crates Move pushes along the pier: SPRITE_CRATES,
  /// the crates of the Bilibin Barricade (bilibin.c).
  /// ★ Named from what the player sees: a tall stack of wooden crates on the
  /// pier (its render)
  LOCALID_FIRST_CRATES,
  /// ★ Named from what the player sees: the second stack of wooden crates
  /// (its render)
  LOCALID_SECOND_CRATES
};

/// Tolbi Docks once the Colosso Tour's passengers are stranded there: the rows
/// of gKalayTolbiDocks_TolbiObjectEventTemplatesStranded that hold other people
/// than gKalayTolbiDocks_TolbiObjectEventTemplates. The man who hears of the
/// scary trip, the one done sailing and the crates keep their rows.
enum TolbiDocksStrandedLocalId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), the
  /// passenger who charges the tour guide and is knocked flat.
  /// ★ Named from the game: "To heck with you! I'm staying right here until
  /// the ship leaves!" (MsgKalayTolbiDocksToHeckWithYouImStaying)
  LOCALID_ANGRY_YOUNG_MAN = LOCALID_FIRST_PLACED + 2,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Sigh. I wonder when we'll be able to return to
  /// Kalay." (MsgKalayTolbiDocksSighIWonderWhenWellBe)
  LOCALID_STRANDED_WOMAN,
  /// The other guide, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) as
  /// the tour guide is: "it's not our fault the ship won't sail, so we can't
  /// offer any refunds."
  /// ★ Named from the game: "Tour groups should take care of you from start to
  /// finish" (MsgKalayTolbiDocksTourGroupsShouldTakeCareOf)
  LOCALID_SECOND_TOUR_GUIDE,
  /// Refunded the angry young man's ticket.
  /// ★ Named from the game: "I returned the cost of your ticket to you."
  /// (MsgKalayReturnedTicketCost)
  LOCALID_TOUR_GUIDE,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN) hired by the tour guides.
  /// ★ Named from the game: "I might as well make some cash as a bodyguard."
  /// (MsgKalayTolbiDocksAsLongAsWereStuckHere)
  LOCALID_BODYGUARD = LOCALID_FIRST_PLACED + 8,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE). "Sigh. If I knew it
  /// would turn out like this, we wouldn't have come."
  /// ★ Named from the game: "Sigh. If I knew it would turn out like this"
  /// (MsgKalayTolbiDocksSighIfIKnewItWould)
  LOCALID_SIGHING_PASSENGER
};

/// The building's people, numbering the rows of
/// gKalayTolbiDocks_InteriorObjectEventTemplates from LOCALID_FIRST_PLACED: the
/// Kalay room's three, then the Tolbi room's. Once the ship has crossed
/// (gKalayTolbiDocks_InteriorObjectEventTemplatesShipGone) the Kalay room's two
/// travellers stand off the map.
enum KalayTolbiDocksInteriorLocalId {
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) in the Kalay room.
  /// ★ Named from the game: "Are you looking for tickets for boarding?"
  /// (MsgKalayLookingForTickets)
  LOCALID_SELLING_TICKETS = LOCALID_FIRST_PLACED,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "Isn't the replacement ship here yet?"
  /// (MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement)
  LOCALID_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP,
  /// An old woman (SPRITE_OLD_WOMAN), who wonders whether her husband is
  /// giving up on Colosso.
  /// ★ Named from the game: "Once we've crossed the Karagol, we'll reach
  /// Tolbi. Ohh, I'm excited."
  /// (MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell)
  LOCALID_OLD_WOMAN_EXCITED_FOR_TOLBI,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who sells no
  /// tickets in the Tolbi room. "I'm sorry, but we just can't sell any
  /// tickets right now."
  /// ★ Named from the game: "we just can't sell any tickets right now."
  /// (MsgKalayTolbiDocksImSorryButWeJustCant)
  LOCALID_SELLING_NO_TICKETS,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "We rented out our house for Colosso, and now
  /// the ship's not launching!" (MsgKalayTolbiDocksWeRentedOutOurHouseFor)
  LOCALID_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE,
  /// An old woman (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "If the ship won't sail, where will we sleep?"
  /// (MsgKalayTolbiDocksWeRentedOutOurHouseIf)
  LOCALID_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP
};

/// The building after Colosso
/// (gKalayTolbiDocks_InteriorObjectEventTemplatesStranded): the Tolbi room's
/// last two rows hold other people.
enum KalayTolbiDocksInteriorStrandedLocalId {
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). "Some rich
  /// guy was here throwing money around, but they still said no."
  /// ★ Named from the game: "Yeah...the ship's not going anywhere for now."
  /// (MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere)
  LOCALID_MAN_ON_THE_SHIP_GOING_NOWHERE = LOCALID_FIRST_PLACED + 4,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "The Karagol Sea looks so calm from here."
  /// (MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm)
  LOCALID_WOMAN_SAYING_THE_SEA_LOOKS_CALM
};

static void KalayTolbiDocks_PrepareKalayDocks(void);
static void KalayTolbiDocks_PrepareTolbiDocks(void);
static void KalayTolbiDocks_PrepareInterior(void);
static void KalayTolbiDocks_UpdateCrateCells(void);
static void KalayTolbiDocks_SettleCrates(s32 localId);
static void KalayTolbiDocks_GaretSeesTheKaragol(void);

/// Kalay Docks: talking to Ground, a Venus Djinni, makes it join the party.
static void KalayTolbiDocks_GroundJoins(void) {
  Djinn_JoinParty(LOCALID_GROUND, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
}

/// Selects the warp destinations of Kalay Docks, Tolbi Docks or the building
/// between.
const struct MapWarpDest *KalayTolbiDocks_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_KalayDocks) {
    return gKalayTolbiDocks_KalayWarpDests;
  }
  if (mapId == (s32)&MapId_TolbiDocks) {
    return gKalayTolbiDocks_TolbiWarpDests;
  }
  if (mapId == (s32)&MapId_KalayTolbiDocksInteriors) {
    return gKalayTolbiDocks_InteriorWarpDests;
  }
  return gKalayTolbiDocks_NoWarpDests;
}

const struct MapWarpRegion *KalayTolbiDocks_GetRegions(void) { return NULL; }

/// The warps of the three maps, each map's after its header word (a warp of
/// kind 0): both docks lead to the world map, onto the ship
/// (MapId_TolbiShipDeck) and into the building
/// (MapId_KalayTolbiDocksInteriors), whose rooms lead back to their docks.
const u32 *KalayTolbiDocks_GetWarps(void) { return gKalayTolbiDocks_Warps; }

/// Each map's people before the crossing, once the ship has crossed and not
/// come back (FLAG_TOLBI_SHIP_REACHED_TOLBI), and once the Colosso Tour's
/// passengers are stranded after Colosso (FLAG_COLOSSO_OVER).
const struct ObjectEventTemplate *
KalayTolbiDocks_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_KalayDocks) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayObjectEventTemplatesShipGone;
    }
    return gKalayTolbiDocks_KalayObjectEventTemplates;
  }

  if (mapId == (s32)&MapId_TolbiDocks) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiObjectEventTemplatesStranded;
    }
    return gKalayTolbiDocks_TolbiObjectEventTemplates;
  }

  if (mapId == (s32)&MapId_KalayTolbiDocksInteriors) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_InteriorObjectEventTemplatesStranded;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_InteriorObjectEventTemplatesShipGone;
    }
    return gKalayTolbiDocks_InteriorObjectEventTemplates;
  }

  return gKalayTolbiDocks_NoObjectEventTemplates;
}

/// Tolbi Docks, stepping off the ship (coord event 20, until
/// FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, which coming off the ship by
/// TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK clears): the leader walks a few steps and
/// a man on the docks hurries over to stand beside the party.
static void KalayTolbiDocks_GreetArrivals(void) {
  Engine_ScriptStart();
  GameFlag_Set(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 392, 296);
  ObjectEvent_SetSpeed(LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, 0x13333, 0x9999);
  ObjectEvent_WalkToAndWait(LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, 408, 296);
  ObjectEvent_FaceDirection(LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, FACING_WEST,
                            0);
  Engine_ScriptWait(20);
  Engine_ScriptEnd();
}

/// Selects each map's events for the same three times as its people.
const struct MapEvent *KalayTolbiDocks_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_KalayDocks) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayEventsShipGone;
    }
    return gKalayTolbiDocks_KalayEvents;
  }

  if (mapId == (s32)&MapId_TolbiDocks) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiEventsStranded;
    }
    return gKalayTolbiDocks_TolbiEvents;
  }

  if (mapId == (s32)&MapId_KalayTolbiDocksInteriors) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_InteriorEventsStranded;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_InteriorEventsShipGone;
    }
    return gKalayTolbiDocks_InteriorEvents;
  }

  return gKalayTolbiDocks_NoEvents;
}

/// Kalay Docks: a young man asks whether the party wants to go to Tolbi. Yes,
/// and he says that past Silk Road's end the way is across the Karagol Sea; no,
/// and he suggests a long look around the ship.
static void KalayTolbiDocks_AskAboutTolbiCrossing(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgKalayWantGoToTolbi);
  Engine_ScriptAskYesNo(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0);
  Engine_ScriptEnd();
}

/// Kalay Docks: a woman asks whether the party is taking the ship. If it is,
/// she asks it to tell her daughter in Tolbi that the tickets were too
/// expensive for the daughter's mother to come (sets
/// FLAG_KALAY_DOCKS_CARRYING_MESSAGE); afterwards she repeats the message.
static void KalayTolbiDocks_TalkToWomanWithAMessage(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) == 0) {
    Engine_ScriptSetMessage(MsgKalayGoingTakeShip);
    Script_OpenMessage(LOCALID_WOMAN_WITH_A_MESSAGE, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Script_ShowMessage(LOCALID_WOMAN_WITH_A_MESSAGE, 0);
      GameFlag_Set(FLAG_KALAY_DOCKS_CARRYING_MESSAGE);
    } else {
      gEventWork->message++;
      Script_ShowMessage(LOCALID_WOMAN_WITH_A_MESSAGE, 0);
    }
  } else {
    Engine_ScriptSetMessage(MsgKalayPassMessageDaughter);
    Script_ShowMessage(LOCALID_WOMAN_WITH_A_MESSAGE, 0);
  }
  Engine_ScriptEnd();
}

/// Kalay Docks: spoken to on the shore after the party has handed over its
/// ticket and gone ashore, the ticket taker asks it to board the ship when it
/// is finished.
static void KalayTolbiDocks_RemindBoardShip(void) {
#if !TBS_PATCH_3
  ((void)ObjectEvent_GetById(LOCALID_PARTY_LEADER));
  Engine_ScriptStart();
#endif
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
#if TBS_PATCH_3
      Engine_ScriptStart();
#endif
      Engine_ScriptSetMessage(MsgKalayFinishedOutHereBoardShip);
      Script_OpenMessage(LOCALID_CHECKING_TICKETS, 0);
      ObjectEvent_FaceDirection(LOCALID_CHECKING_TICKETS, FACING_SOUTH, 0);
#if TBS_PATCH_3
      Engine_ScriptEnd();
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
  s32 ticketOwner;
  s32 ticketSlot;
  struct ObjectEvent *leader;
  s16 cardinalFacing;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  cardinalFacing = ((leader->facing + 0x2000) & ~0x3fff);
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
      Engine_ScriptSetMessage(MsgKalayFinishedOutHereBoardShip);
      Script_OpenMessage(LOCALID_CHECKING_TICKETS, 0);
    } else {
      Engine_ScriptSetMessage(MsgKalayWantGoAshore);
      Script_OpenMessage(LOCALID_CHECKING_TICKETS, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 0) {
        Engine_ScriptWait(10);
        Engine_ScriptSetMessage(MsgKalayWantGoAshore + 1);
        Script_ShowMessage(LOCALID_CHECKING_TICKETS, 0);
        ObjectEvent_WalkToAndWait(LOCALID_CHECKING_TICKETS, 88, 1288);
        ObjectEvent_FaceDirection(LOCALID_CHECKING_TICKETS, FACING_SOUTH, 0);
        Engine_ScriptWait(20);
        GameFlag_Set(FLAG_KALAY_DOCKS_WENT_ASHORE);
      } else {
        Engine_ScriptSetMessage(MsgKalayWantGoAshore + 2);
        Script_ShowMessage(LOCALID_CHECKING_TICKETS, 0);
      }
    }
  } else {
    if ((u16)cardinalFacing != FACING_WEST) {
      return;
    }
    Engine_ScriptSetMessage(MsgKalayTicketsPlease);
    Script_ShowMessage(LOCALID_CHECKING_TICKETS, 0);
    // Hand over the actual inventory ticket before walking past the gate.
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
      ticketOwner = PartyInventory_FindOwner(ITEM_BOAT_TICKET);
      ticketSlot = Inventory_Find(ticketOwner, ITEM_BOAT_TICKET);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_CHECKING_TICKETS,
                                            ANIMATION_NOD);
      ObjectEvent_WalkToAndWait(LOCALID_CHECKING_TICKETS, 88, 1288);
      ObjectEvent_FaceDirection(LOCALID_CHECKING_TICKETS, FACING_SOUTH, 0);
      gEventWork->message++;
      Script_ShowMessage(LOCALID_CHECKING_TICKETS, 0);
      Inventory_Discard(ticketOwner, ticketSlot);
      GameFlag_Set(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER);
      leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, leader->x.part.pixel,
                                1304);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 72, 1304);
      ObjectEvent_WalkToAndWait(LOCALID_CHECKING_TICKETS, 88, 1304);
      ObjectEvent_FaceDirection(LOCALID_CHECKING_TICKETS, FACING_EAST, 0);
    } else {
      Script_ShowMessage(LOCALID_CHECKING_TICKETS, 0);
    }
  }
  Engine_ScriptEnd();
}

/// Kalay Docks, once the tour's group has left Kalay: the dock worker explains
/// the tickets: hand one over to board, give it to the ticket taker, or wait
/// on board for the ship to sail.
static void KalayTolbiDocks_TalkToDockWorker(void) {
  Engine_ScriptStart();

  if (Engine_GameFlagIsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    Engine_ScriptSetMessage(MsgKalayBoardWaitSetSail);
    Engine_ScriptOpenMessage(LOCALID_DOCK_WORKER, 0);
  } else if (Engine_GameFlagIsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Engine_ScriptSetMessage(MsgKalayHaveTicketsGiveHim);
    Engine_ScriptShowMessage(LOCALID_DOCK_WORKER, 0);
  } else {
    Engine_ScriptSetMessage(MsgKalayAfterHandingOverTicket);
    Engine_ScriptShowMessage(LOCALID_DOCK_WORKER, 0);
  }

  Engine_ScriptEnd();
}

/// The building's Kalay room, until the ship has crossed: the ticket seller
/// offers tickets for the four of them at 600 coins, with the party's coins
/// shown beside the question. Declining, the leader shakes his head; without
/// enough coins the seller refuses; otherwise the party gets its ticket and
/// pays (sets FLAG_KALAY_DOCKS_TICKET_BOUGHT).
static void KalayTolbiDocks_TalkToTicketSeller(void) {
  struct UiWindow *coinsWindow;
  struct GameState *state;
  s32 ticketPrice;

#if !TBS_PATCH_1
  /* FAKEMATCH(temporary): coinLabel keeps the first label in r5 and steps it
   * to the second; as two MSGNO arguments, 24 of 152 instructions differ in
   * tbs-ja. */
  s32 coinLabel;
#endif

  ticketPrice = 600;
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Engine_ScriptSetMessage(MsgKalayHandInTicketPlank);
    Script_ShowMessage(LOCALID_SELLING_TICKETS, 0);
  } else {
    Engine_ScriptSetMessage(MsgKalayLookingForTickets);
    Script_OpenMessage(LOCALID_SELLING_TICKETS, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 1) {
      Script_ShowMessageAndWait(LOCALID_SELLING_TICKETS, 0, 10);
    } else {
      gEventWork->message++;
      UiWork_PushValueSlot(ticketPrice, 5);
      Script_OpenMessage(LOCALID_SELLING_TICKETS, 0);
      // Keep the balance visible while the player confirms the purchase.
      coinsWindow = UiWindow_Create(TICKET_COUNTER_WINDOW_X, 8,
                                    TICKET_COUNTER_WIDTH, 4, 2);
#if TBS_PATCH_1
      UiText_DrawCharacterAtOffset(MSGNO(MsgYourCoins), coinsWindow, 0, 0);
#else
      coinLabel = MSGNO(MsgYourCoins);
      UiText_DrawCharacterAtOffset(coinLabel, coinsWindow, 0, 0);
#endif
      state = &gGameState;
#if LANGUAGE == LANGUAGE_JAPANESE
      coinLabel += MSGNO(MsgCoins) - MSGNO(MsgYourCoins);
#endif
      UiText_DrawNumberInWindow(state->coins, 6, coinsWindow, TICKET_COUNTER_X,
                                8);
#if LANGUAGE == LANGUAGE_JAPANESE
      UiText_DrawCharacterAtOffset(coinLabel, coinsWindow, 48, 8);
#endif
      if (Engine_ScriptChooseYesNo(-1, 0) == 1) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER,
                                              ANIMATION_SHAKE_HEAD);
        Engine_ScriptWait(10);
        Script_ShowMessage(LOCALID_SELLING_TICKETS, 0);
      } else if ((u32)ticketPrice > (u32)state->coins) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER,
                                              ANIMATION_NOD);
        Engine_ScriptWait(10);
        gEventWork->message++;
        Script_PlayCue(SE_MENU_CANCEL);
        Script_ShowMessage(LOCALID_SELLING_TICKETS, 0);
      } else {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER,
                                              ANIMATION_NOD);
        Engine_ScriptWait(10);
        gEventWork->message += 3;
        Script_ShowMessage(LOCALID_SELLING_TICKETS, 0);
        // Complete both inventory and coin changes only after confirmation.
        Engine_PartyGiveItem(ITEM_BOAT_TICKET, 0);
        GameFlag_Set(FLAG_KALAY_DOCKS_TICKET_BOUGHT);
        Party_AdjustCoins(-ticketPrice);
      }
    }
    Engine_ScriptEnd();
  }
}

/// Tolbi Docks: a man on the docks has heard the party was attacked by a
/// monster on the crossing, and asks whether that is true; told it was nothing,
/// he will not believe they fought the monsters alone, for Kaja killed the
/// Kraken.
static void KalayTolbiDocks_AskAboutRoughCrossing(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgKalayHearScaryTrip);
  Engine_ScriptAskYesNo(LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, 0);
  Engine_ScriptEnd();
}

/// Tolbi Docks: a young man asks whether the upset tourists got seasick.
static void KalayTolbiDocks_AskAboutSeasickTourists(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgKalayTouristsLookUpset);
  Engine_ScriptAskYesNo(LOCALID_YOUNG_MAN_ASKING_IF_SEASICK, 0);
  Engine_ScriptEnd();
}

/// Tolbi Docks: the daughter looks for her parents among the passengers. If the
/// party carries the message from her parents she asks for it and thanks them
/// once it is given; otherwise she turns back to the sea.
static void KalayTolbiDocks_TalkToWaitingDaughter(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED) != 0) {
    ObjectEvent_FaceObjectEvent(LOCALID_WAITING_DAUGHTER, LOCALID_PARTY_LEADER,
                                0);
    Engine_ScriptWait(20);
    Engine_ScriptSetMessage(MsgKalayThanksMessageParents);
    Script_ShowMessage(LOCALID_WAITING_DAUGHTER, 0);
    Engine_ScriptEnd();
  } else {
    Engine_ScriptWait(20);
    ObjectEvent_ShowEmote(LOCALID_WAITING_DAUGHTER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
    ObjectEvent_FaceObjectEvent(LOCALID_WAITING_DAUGHTER, LOCALID_PARTY_LEADER,
                                0);
    Engine_ScriptWait(20);
    Engine_ScriptSetMessage(MsgKalaySilkRoadBlocked);
    Script_ShowMessage(LOCALID_WAITING_DAUGHTER, 0);
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) != 0) {
      Engine_ScriptWait(20);
      ObjectEvent_ShowEmote(LOCALID_WAITING_DAUGHTER,
                            EMOTE_IN_FRONT | EMOTE_WINCE, 40);
      Script_OpenMessage(LOCALID_WAITING_DAUGHTER, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 0) {
        Engine_ScriptWait(20);
        Script_ShowMessage(LOCALID_WAITING_DAUGHTER, 0);
        GameFlag_Set(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED);
      } else {
        Engine_ScriptWait(10);
        gEventWork->message++;
        Script_ShowMessage(LOCALID_WAITING_DAUGHTER, 0);
        Engine_ScriptWait(10);
        ObjectEvent_FaceDirection(LOCALID_WAITING_DAUGHTER, FACING_EAST, 0);
        Engine_ScriptWait(30);
      }
    } else {
      Engine_ScriptWait(10);
      ObjectEvent_FaceDirection(LOCALID_WAITING_DAUGHTER, FACING_EAST, 0);
      Engine_ScriptWait(30);
    }
    Engine_ScriptEnd();
  }
}

/// Kalay Docks or Tolbi Docks: walking north into the door (coord event 10) of
/// the building beside them, the door opens and the leader walks in by
/// TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS, which in Kalay Docks is
/// KALAY_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS by the same number.
static void KalayTolbiDocks_OpenDoorAndLeave(void) {
  Engine_ScriptStart();
  Script_PlayCue(SE_DOOR_OPEN);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 3);
  if (gGameState.mapId == (s32)&MapId_KalayDocks) {
    ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 304, 1392);
    MapGrid_AnimateCells(gKalayTolbiDocks_KalayDoorCells, 78, 86);
  } else {
    if (gGameState.mapId == (s32)&MapId_TolbiDocks) {
      ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 248, 192);
      MapGrid_AnimateCells(gKalayTolbiDocks_TolbiDoorCells, 74, 9);
    }
  }
  Engine_ScriptWait(16);
  Engine_ScriptRequestWarp(TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS);
  Engine_ScriptEnd();
}

/// A warp destination no warp, table or code leads to, whose arrival sets
/// FLAG_COLOSSO_OVER; the debug menus' map selector reaches it.
enum { KALAY_TOLBI_DOCKS_WARP_DEST_AFTER_COLOSSO = 90 };

/// Prepares whichever of the three maps is entered. Arriving by
/// KALAY_TOLBI_DOCKS_WARP_DEST_AFTER_COLOSSO sets FLAG_COLOSSO_OVER, which
/// leaves the tour's passengers stranded on the Tolbi Docks.
s32 KalayTolbiDocks_Initialize(void) {
  if (gGameState.warpDest == KALAY_TOLBI_DOCKS_WARP_DEST_AFTER_COLOSSO) {
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
  }
  if (gGameState.mapId == (s32)&MapId_KalayDocks) {
    KalayTolbiDocks_PrepareKalayDocks();
  } else if (gGameState.mapId == (s32)&MapId_TolbiDocks) {
    KalayTolbiDocks_PrepareTolbiDocks();
  } else if (gGameState.mapId == (s32)&MapId_KalayTolbiDocksInteriors) {
    KalayTolbiDocks_PrepareInterior();
  }
  return 0;
}

/// On the first arrival at Kalay Docks from the world map, Garet runs to see
/// the Karagol. Coming back off the ship clears the shore leave, and once the
/// tour's group has left Kalay the ticket taker stands at the ship's plank
/// until the party goes ashore.
static void KalayTolbiDocks_PrepareKalayDocks(void) {
  if (gGameState.warpDest == KALAY_DOCKS_FROM_WORLD_MAP_SOUTH) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL) == 0) {
      GameFlag_Set(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL);
      KalayTolbiDocks_GaretSeesTheKaragol();
    }
  }
  if (gGameState.warpDest == KALAY_DOCKS_FROM_TOLBI_SHIP_DECK) {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      GameFlag_Clear(FLAG_KALAY_DOCKS_WENT_ASHORE);
    }
  }
  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) == 0) {
      ObjectEvent_SetPosition(LOCALID_CHECKING_TICKETS, PIXELS(88),
                              PIXELS(1304));
      ObjectEvent_FaceDirection(LOCALID_CHECKING_TICKETS, FACING_EAST, 0);
    }
  }
}

/// On Tolbi Docks the two stacks of crates settle into the pier's cells. The
/// passenger who met the party stays beside the ship, and after the
/// confrontation the guide, the bodyguard and the two passengers keep where it
/// left them, the angry one still lying on the ground.
static void KalayTolbiDocks_PrepareTolbiDocks(void) {
  struct ObjectEvent *objectEvent;
  u8 zero;

  KalayTolbiDocks_UpdateCrateCells();
  if (GameFlag_IsSet(FLAG_COLOSSO_OVER)) {
    Engine_ObjectEventSetChildValue(LOCALID_SECOND_TOUR_GUIDE,
                                    SPRITE_COLOR_RBG);
  }
  if (gGameState.warpDest == TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  if (gGameState.warpDest == TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK) {
    Engine_GameFlagClear(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED)) {
    ObjectEvent_SetPosition(LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, PIXELS(408),
                            PIXELS(296));
    ObjectEvent_FaceDirection(LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP,
                              FACING_WEST, 0);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED)) {
    ObjectEvent_SetPosition(LOCALID_TOUR_GUIDE, PIXELS(280), PIXELS(296));
    ObjectEvent_FaceDirection(LOCALID_TOUR_GUIDE, FACING_NORTH, 0);
    ObjectEvent_SetPosition(LOCALID_BODYGUARD, PIXELS(288), PIXELS(280));
    ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_NORTHEAST, 0);
    ObjectEvent_SetPosition(LOCALID_ANGRY_YOUNG_MAN, PIXELS(232), PIXELS(304));
    ObjectEvent_FaceDirection(LOCALID_ANGRY_YOUNG_MAN, FACING_SOUTH, 0);
    ObjectEvent_SetPosition(LOCALID_STRANDED_WOMAN, PIXELS(240), PIXELS(312));
    ObjectEvent_FaceDirection(LOCALID_STRANDED_WOMAN, FACING_NORTH, 0);
    objectEvent = ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN);
    objectEvent->collisionFlags = 0;
    objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    ((u8 *)objectEvent->sprite)[9] |= 12;
    ((u8 *)objectEvent->sprite)[38] = 0;
    zero = 0;
    objectEvent->sprite->rotation = 0xc000;
    objectEvent = ObjectEvent_GetById(LOCALID_STRANDED_WOMAN);
    objectEvent->priorityFlags = zero;
    ((u8 *)objectEvent->sprite)[9] |= 12;
    ((u8 *)objectEvent->sprite)[21] |= 12;
  }
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER)) {
    MapGrid_CopyCellAttributes(18, 18, 1, 1, 14, 18);
    MapGrid_CopyCellAttributes(18, 18, 1, 1, 15, 18);
  }
}

/// The building between the docks opens with a window transition and no area
/// name.
static void KalayTolbiDocks_PrepareInterior(void) {
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
}

/// Tolbi Docks, after Move pushes either stack of crates: the pier's cells
/// under both are rebuilt.
static void KalayTolbiDocks_UpdateCrateCells(void) {
  s32 firstCratesColumn;
  s32 secondCratesColumn;

  firstCratesColumn = ObjectEvent_GetById(LOCALID_FIRST_CRATES)->x.fixed >> 20;
  secondCratesColumn =
      ObjectEvent_GetById(LOCALID_SECOND_CRATES)->x.fixed >> 20;

  MapGrid_CopyCellAttributes(5, 12, 5, 1, 5, 11);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, secondCratesColumn, 11);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, firstCratesColumn, 11);

  KalayTolbiDocks_SettleCrates(LOCALID_FIRST_CRATES);
  KalayTolbiDocks_SettleCrates(LOCALID_SECOND_CRATES);
}

/// Stops a stack of crates' idle motion and sets its sprite priority.
static void KalayTolbiDocks_SettleCrates(s32 localId) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);

  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(localId), 0);
  Engine_ObjectEventSetSpritePriority(localId, 3);
  objectEvent->motionFlags = 0;
  objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
}

/// Kalay Docks, the first entry from the world map
/// (KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, until
/// FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL): Garet runs to the edge of the docks and
/// marvels at the sea. "Incredible! So this is the ocean Kraden was talking
/// about!" Kraden is the scholar who has told Garet of the ocean. The man who
/// asks travellers about Tolbi (LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI) walks
/// over, shaking his head: the Karagol is surrounded by land, so it is not a
/// real ocean. Garet means to see a real one someday, nods with the leader and
/// rejoins the party.
static void KalayTolbiDocks_GaretSeesTheKaragol(void) {
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  ObjectEvent_SetPosition(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, PIXELS(328),
                          PIXELS(1424));
  ObjectEvent_GetById(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI)->paused = 1;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 0, FACING_WEST);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_ScriptWait(20);
  // Follow Garet to the water before the man joins him.
  Engine_ScriptSetMessage(MsgKalayIncredibleOcean);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_SetSpeed(GARET, 0x19999, 0xcccc);
  ObjectEvent_WalkToAndWait(GARET, 232, 1424);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  Camera_MoveTo(PIXELS(184), -1, PIXELS(1440), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(10);
  Engine_ObjectEventJump(GARET, 6, 15);
  Engine_ObjectEventJump(GARET, 6, 40);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(1448), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  ObjectEvent_SetSpeed(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0x13333, 0x9999);
  ObjectEvent_WalkToAndWait(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, 264, 1424);
  ObjectEvent_FaceDirection(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, FACING_WEST,
                            0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  Engine_ScriptWait(20);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_ShowMessage(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  Engine_ScriptWait(50);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(30);
  ObjectEvent_SetSpeed(GARET, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(GARET, 264, 1464);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, FACING_SOUTH,
                            0);
  Engine_ScriptWait(30);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_SetSpeed(GARET, 0x13333, 0x9999);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  // Garet rejoins the leader and the man wanders again from where he stands.
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_ScriptWait(20);
  ObjectEvent_GetById(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI)->paused = 0;
  ObjectEvent_SetBuiltinAction(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, 2);
  objectEvent = ObjectEvent_GetById(LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI);
  {
    s32 pixelX = objectEvent->x.fixed / 0x10000;

    objectEvent->timer = pixelX;
  }
  {
    s32 pixelZ = objectEvent->z.fixed / 0x10000;

    objectEvent->work = pixelZ;
  }
  Engine_ScriptEnd();
}

/// Tolbi Docks after Colosso, the first time the party comes to the pier (coord
/// event 50, until FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED): it watches the tour
/// guide tell an angry young man his ticket was refunded. The passengers demand
/// to be taken home, the ships being stuck and the road blocked; when the angry
/// one charges, the guides' hired bodyguard steps in, and after a second try
/// the passenger is knocked flat and the bodyguard spins on the spot.
static void KalayTolbiDocks_PassengersConfrontTourGuide(void) {
  struct ObjectEvent *objectEvent;

  GameFlag_Set(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED);
  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  Engine_ScriptSetMessage(MsgKalayReturnedTicketCost);
  objectEvent = ObjectEvent_GetById(LOCALID_STRANDED_WOMAN);
  objectEvent->priorityFlags = 0;
  ((u8 *)objectEvent->sprite)[9] |= 12;
  ((u8 *)objectEvent->sprite)[21] |= 12;
  Camera_MoveTo(PIXELS(232), -1, PIXELS(304), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 216, 272);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Script_ShowMessage(LOCALID_TOUR_GUIDE, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_SCOWL,
                        50);
  ObjectEvent_FaceDirection(LOCALID_ANGRY_YOUNG_MAN, FACING_EAST, 0);
  Engine_ObjectEventJump(LOCALID_ANGRY_YOUNG_MAN, 4, 13);
  Engine_ObjectEventJump(LOCALID_ANGRY_YOUNG_MAN, 4, 30);
  Script_ShowMessage(LOCALID_ANGRY_YOUNG_MAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_STRANDED_WOMAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_STRANDED_WOMAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TOUR_GUIDE,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Engine_ScriptShowMessage(LOCALID_TOUR_GUIDE, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_ANGRY_YOUNG_MAN,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 55);
  ObjectEvent_SetSpeed(LOCALID_ANGRY_YOUNG_MAN, 0x20000, 0x10000);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, 16, 0);
  Engine_ObjectEventJump(LOCALID_ANGRY_YOUNG_MAN, 7, 0);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, 24, 0);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkBy(LOCALID_ANGRY_YOUNG_MAN, -16, 0);
  Script_PlayCue(SE_HIGH_JUMP);
  ObjectEvent_SetSpeed(LOCALID_TOUR_GUIDE, 0x26666, 0x13333);
  ObjectEvent_WalkByAndWait(LOCALID_TOUR_GUIDE, 16, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_ANGRY_YOUNG_MAN, ANIMATION_STAND);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetAttachedEffect(LOCALID_TOUR_GUIDE, ATTACHED_EFFECT_IN_FRONT |
                                                        ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_TOUR_GUIDE, 2);
  Script_PlayCue(SE_155);
  Engine_ScriptWait(10);
  Script_PlayCue(SE_155);
  Engine_ScriptWait(10);
  Script_PlayCue(SE_155);
  Engine_ScriptWait(10);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_TOUR_GUIDE, 0x6666, 0x3333);
  Engine_ObjectEventJump(LOCALID_TOUR_GUIDE, 6, 0);
  Script_PlayCue(SE_159);
  ObjectEvent_WalkByAndWait(LOCALID_TOUR_GUIDE, -8, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_ANGRY_YOUNG_MAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  ObjectEvent_SetSpeed(LOCALID_BODYGUARD, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_BODYGUARD, -8, 0);
  ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_BODYGUARD,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_BODYGUARD, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_ANGRY_YOUNG_MAN, FACING_NORTHEAST, 0);
  Engine_ScriptWait(35);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ANGRY_YOUNG_MAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_ANGRY_YOUNG_MAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(55);
  ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_BODYGUARD, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_STRANDED_WOMAN, FACING_NORTHEAST, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_STRANDED_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        50);
  Script_ShowMessage(LOCALID_STRANDED_WOMAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TOUR_GUIDE, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_TOUR_GUIDE, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_TOUR_GUIDE, FACING_NORTHWEST, 0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_TOUR_GUIDE, FACING_WEST, 0);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_TOUR_GUIDE, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_ANGRY_YOUNG_MAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_STRANDED_WOMAN, FACING_EAST, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ANGRY_YOUNG_MAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_ANGRY_YOUNG_MAN, 0);
  ObjectEvent_SetSpeed(LOCALID_ANGRY_YOUNG_MAN, 0x13333, 0x9999);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, 8, 0);
  ObjectEvent_SetSpeed(LOCALID_BODYGUARD, 0x20000, 0x10000);
  ObjectEvent_WalkByAndWait(LOCALID_BODYGUARD, -8, 16);
  ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_WEST, 0);
  Script_ShowMessage(LOCALID_BODYGUARD, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        50);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_ANGRY_YOUNG_MAN, 0xcccc, 0x6666);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, -8, 0);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ANGRY_YOUNG_MAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_ANGRY_YOUNG_MAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_BODYGUARD,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_BODYGUARD, 0);
  Engine_ScriptWait(10);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_ANGRY_YOUNG_MAN, 0xcccc, 0x6666);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, -16, 0);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_FaceDirection(LOCALID_ANGRY_YOUNG_MAN, FACING_EAST, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ANGRY_YOUNG_MAN,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_ANGRY_YOUNG_MAN, 0x1cccc, 0xe666);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, 8, 0);
  Engine_ObjectEventJump(LOCALID_ANGRY_YOUNG_MAN, 6, 0);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, 24, 0);
  Script_PlayCue(SE_IMPACT);
  Engine_ObjectEventJump(LOCALID_BODYGUARD, 6, 0);
  // The passenger bounces off the spinning bodyguard and lands on his back.
  ObjectEvent_SetActionScript(LOCALID_BODYGUARD,
                              gKalayTolbiDocks_BodyguardSpinScript);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventJump(LOCALID_ANGRY_YOUNG_MAN, 6, 0);
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, -12, 4);
  objectEvent = ObjectEvent_Get(LOCALID_ANGRY_YOUNG_MAN);
  objectEvent->collisionFlags = 0;
  objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  ((u8 *)objectEvent->sprite)[9] |= 12;
  ((u8 *)objectEvent->sprite)[38] = 0;
  objectEvent->sprite->rotation = 0xc000;
  ObjectEvent_WalkByAndWait(LOCALID_ANGRY_YOUNG_MAN, -12, 4);
  ObjectEvent_FaceDirection(LOCALID_ANGRY_YOUNG_MAN, FACING_SOUTH, 0);
  ObjectEvent_GetById(LOCALID_ANGRY_YOUNG_MAN)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  Script_PlayCue(SE_159);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_STRANDED_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        50);
  ObjectEvent_SetSpeed(LOCALID_STRANDED_WOMAN, 0x18000, 0xc000);
  ObjectEvent_WalkByAndWait(LOCALID_STRANDED_WOMAN, 24, 0);
  ObjectEvent_FaceDirection(LOCALID_STRANDED_WOMAN, FACING_NORTH, 0);
  Engine_ScriptWait(10);
  Script_ShowMessage(LOCALID_STRANDED_WOMAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_BODYGUARD,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_BODYGUARD, 0);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_BODYGUARD, 0xcccc, 0x6666);
  ObjectEvent_WalkByAndWait(LOCALID_BODYGUARD, -8, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_BODYGUARD, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ANGRY_YOUNG_MAN, 2);
  Engine_ScriptWait(20);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_EAST, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TOUR_GUIDE, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TOUR_GUIDE, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_BODYGUARD, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_BODYGUARD, 24, -24);
  ObjectEvent_WalkByAndWait(LOCALID_BODYGUARD, 8, 0);
  ObjectEvent_FaceDirection(LOCALID_BODYGUARD, FACING_NORTHEAST, 0);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_TOUR_GUIDE, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_TOUR_GUIDE, 0, -8);
  Engine_ScriptWait(10);
  Engine_ScriptEnd();
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

struct MapWarpDest gKalayTolbiDocks_NoWarpDests[] = {
    {0, CONDITION_ALWAYS, 128, 0, 128, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKalayTolbiDocks_KalayWarpDests[] = {
    {0, CONDITION_ALWAYS, 248, 0, 1368, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 288, 0, 1464,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 72, 0, 1304,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS, CONDITION_ALWAYS, 304, 0,
     1416, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_DOCKS_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 296, 0, 1048,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKalayTolbiDocks_TolbiWarpDests[] = {
    {0, CONDITION_ALWAYS, 392, 0, 296, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 408, 0, 296,
     FACING_WEST, 0, 0, 0, 448, 432, 0},
    {TOLBI_DOCKS_FROM_WORLD_MAP, CONDITION_ALWAYS, 24, 0, 232, FACING_EAST, 0,
     0, 0, 448, 432, 0},
    {TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS, CONDITION_ALWAYS, 240, 0,
     216, FACING_SOUTH, 0, 0, 0, 448, 432, 0},
    {MAP_TABLE_END},
};

/// The building's warp destinations, each into the room at the dock it is
/// entered from.
struct MapWarpDest gKalayTolbiDocks_InteriorWarpDests[] = {
    {0, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS, CONDITION_ALWAYS, 96, 0, 248,
     FACING_NORTH, 0, 32, 32, 272, 272, 0},
    {KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 384, 0,
     248, FACING_NORTH, 0, 320, 32, 560, 272, 0},
    {MAP_TABLE_END},
};

/// Kalay Docks' warps, then Tolbi Docks' and the building's, each after the
/// header word (a warp of kind 0) for its map.
u32 gKalayTolbiDocks_Warps[] = {
    MAP_WARPS(KalayDocks),
    MAP_WARP(KALAY_DOCKS_TO_WORLD_MAP_SOUTH, WorldMap,
             WORLD_MAP_FROM_KALAY_DOCKS_EAST),
    MAP_WARP(KALAY_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_KALAY_DOCKS),
    MAP_WARP(KALAY_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS,
             KalayTolbiDocksInteriors,
             KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS),
    MAP_WARP(KALAY_DOCKS_TO_WORLD_MAP_NORTH, WorldMap,
             WORLD_MAP_FROM_KALAY_DOCKS_WEST),
    MAP_WARPS(TolbiDocks),
    MAP_WARP(TOLBI_DOCKS_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI_DOCKS),
    MAP_WARP(TOLBI_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
    MAP_WARP(TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS,
             KalayTolbiDocksInteriors,
             KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS),
    MAP_WARPS(KalayTolbiDocksInteriors),
    MAP_WARP(KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS, KalayDocks,
             KALAY_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS),
    MAP_WARP(KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS, TolbiDocks,
             TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gKalayTolbiDocks_NoObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gKalayTolbiDocks_TolbiObjectEventTemplates[] = {
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(392), 0, PIXELS(344), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalayTolbiDocks_TolbiObjectEventTemplatesStranded[] = {
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(296), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(344), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(304), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(320), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(304), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(288), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalayTolbiDocks_KalayObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(1408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(1224), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(1304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(1336), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(1288), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(120), 0, PIXELS(1368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { VENUS_DJINNI, 0x35, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalayTolbiDocks_KalayObjectEventTemplatesShipGone[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(1352), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(1288), FACING_WEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { VENUS_DJINNI, 0x35, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalayTolbiDocks_InteriorObjectEventTemplates[] = {
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(192), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalayTolbiDocks_InteriorObjectEventTemplatesShipGone[] = {
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalayTolbiDocks_InteriorObjectEventTemplatesStranded[] = {
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gKalayTolbiDocks_NoEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_KalayEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutTolbiCrossing) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
    { MAP_EVENT_TALK, LOCALID_MAN_AT_SILK_ROADS_END, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_WITH_A_MESSAGE, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWomanWithAMessage) },
    { MAP_EVENT_TALK, LOCALID_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
    { MAP_EVENT_TALK, LOCALID_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
    { 0x400, LOCALID_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketTaker) },
    { 0x8400, LOCALID_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketTaker) },
#if !TBS_PATCH_3
    { MAP_EVENT_TALK, LOCALID_CHECKING_TICKETS, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#else
    { 0xc400, LOCALID_CHECKING_TICKETS, IF_FLAG_SET(FLAG_KALAY_DOCKS_WENT_ASHORE), EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#endif
    { MAP_EVENT_TALK, LOCALID_DOCK_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToDockWorker) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_AT_SILK_ROADS_END, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_WITH_A_MESSAGE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
    { MAP_EVENT_TALK, LOCALID_GROUND, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_GroundJoins) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_KalayEventsShipGone[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_TALK, LOCALID_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
    { MAP_EVENT_TALK, LOCALID_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
    { MAP_EVENT_TALK, LOCALID_GROUND, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_GroundJoins) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_TolbiEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { MAP_EVENT_COORD, 20, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, EVENT_SCRIPT(KalayTolbiDocks_GreetArrivals) },
    { MAP_EVENT_TALK, LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutRoughCrossing) },
    { MAP_EVENT_TALK, LOCALID_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ASKING_IF_SEASICK, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutSeasickTourists) },
    { MAP_EVENT_TALK, LOCALID_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYoureGoingToTolbiKeep) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_THE_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WAITING_DAUGHTER, IF_FLAG_SET(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED), MSGNO(MsgKalayTolbiDocksItWasAHardCrossingVery) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | LOCALID_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_ASKING_IF_SEASICK, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAfterSeeingTheLooksOnThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGetTheFeelingThatIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_THE_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCrossedTheSeaJustFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGuessWeWontBeSeeing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIThinkThatMissingShipMust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_TOLBI_DOCKS_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), 101, FLAG_TOLBI_DOCKS_COINS_BOX, FIND_COINS(35) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_TolbiEventsStranded[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { MAP_EVENT_COORD, 50, FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED, EVENT_SCRIPT(KalayTolbiDocks_PassengersConfrontTourGuide) },
    { MAP_EVENT_TALK, LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThereAreManyHereWhoWant) },
    { MAP_EVENT_TALK, LOCALID_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHangingAroundHereLikeThisAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfBothTheCaptainAndKaja) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfKajaAndTheOthersSay) },
    { MAP_EVENT_TALK, LOCALID_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTourGroupsShouldTakeCareOf) },
    { MAP_EVENT_TALK, LOCALID_ANGRY_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksToHeckWithYouImStaying) },
    { MAP_EVENT_TALK, LOCALID_STRANDED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIWonderWhenWellBe) },
    { MAP_EVENT_TALK, LOCALID_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellBeCountingOnYouIf) },
    { MAP_EVENT_TALK, LOCALID_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsWereStuckHere) },
    { MAP_EVENT_TALK, LOCALID_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIfIKnewItWould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfTheydJustLetTheShip) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ANGRY_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfImGoingToBeStuck) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRANDED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksGrrrWhyDontYouGetUp) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsYoureHereWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThisTourWasntWellOrganizedThats) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfGrandmaIsDoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_TOLBI_DOCKS_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), 101, FLAG_TOLBI_DOCKS_COINS_BOX, FIND_COINS(35) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_InteriorEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS },
    { MAP_EVENT_TALK, LOCALID_SELLING_TICKETS, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketSeller) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_TICKETS, FLAG_KALAY_DOCKS_TICKET_BOUGHT, MSGNO(MsgKalayTolbiDocksIFeelSortOfBadThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYouveAlreadyBoughtYourTicket) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontLeaveHereTill) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksLetsGetGoingImNotGetting) },
    { MAP_EVENT_TALK, LOCALID_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_InteriorEventsShipGone[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS },
    { MAP_EVENT_TALK, LOCALID_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { MAP_EVENT_TALK, LOCALID_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalayTolbiDocks_InteriorEventsStranded[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS },
    { MAP_EVENT_TALK, LOCALID_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { MAP_EVENT_TALK, LOCALID_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImStuckIWantToCross) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_THE_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_SAYING_THE_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveGotPassengersButNobodysSailing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_THE_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheCaptainMustBeAStubborn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_SAYING_THE_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheMarineBlueKaragolSeaIs) },
    { MAP_EVENTS_END },
};
// clang-format on
