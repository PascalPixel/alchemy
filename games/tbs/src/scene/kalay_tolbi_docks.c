/// @file
/// Kalay Docks and Tolbi Docks, the two ends of the ship's crossing of the
/// Karagol Sea, in three scenes that share one overlay: SceneId_KalayDocks,
/// SceneId_KalayTolbiDocksInteriors and SceneId_TolbiDocks. The getters pick
/// each scene's tables by gGameState.scene, and three story flags choose among
/// them: FLAG_TOLBI_SHIP_REACHED_TOLBI, whether the ship has made its crossing
/// (set on the world map, WorldMap_CompleteTransfer);
/// FLAG_KALAY_COLOSSO_TOUR_LEFT, whether the Colosso Tour's group has left
/// Kalay; and FLAG_COLOSSO_OVER, whether the Colosso finals have been fought.
///
/// Kalay Docks (KalayDocks) is where the party's road to Tolbi ends:
/// Silk Road is blocked, so everyone bound for Tolbi and its Colosso must cross
/// the Karagol Sea by ship, and the tickets are dear. The first time the party
/// walks in from the world map (KALAY_DOCKS_FROM_WORLD_MAP_SOUTH,
/// FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL) KalayTolbiDocks_GaretSeesTheKaragol
/// plays: Garet runs to the water ("So this is the ocean Kraden was talking
/// about!"), a man tells him the Karagol is surrounded by land and no true
/// ocean, and Garet means to see a real one someday. Until the tour's group
/// has left Kalay, the ship is open to look around and the man at its plank
/// (ACTOR_CHECKING_TICKETS) checks no tickets; afterwards he stands in the way
/// and asks for one, takes it if the party has bought it
/// (FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) and lets the party aboard, and then
/// offers to let it ashore again until it boards
/// (FLAG_KALAY_DOCKS_WENT_ASHORE, cleared each time the party comes back off
/// the ship by KALAY_DOCKS_FROM_TOLBI_SHIP_DECK). A father
/// (ACTOR_WORRIED_FATHER) asks the party to tell his daughter in Tolbi that
/// her mother cannot afford the tickets (FLAG_KALAY_DOCKS_CARRYING_MESSAGE).
/// Ground, a Venus Djinni (ACTOR_GROUND), waits on the docks until it joins.
/// Once the ship has crossed, the travellers are gone and the dock hands wait
/// for a ship that has not returned. The docks lead to the world map
/// (KALAY_DOCKS_TO_WORLD_MAP_SOUTH and KALAY_DOCKS_TO_WORLD_MAP_NORTH), onto
/// the ship (KALAY_DOCKS_TO_TOLBI_SHIP_DECK, the Tolbi-bound Ship,
/// SceneId_TolbiShipDeck) and through a door (trigger 10) into the building
/// beside them.
///
/// That building (KalayTolbiDocksInteriors) has a room at each dock: entered
/// from Kalay Docks (KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS) it is Kalay
/// Docks, from Tolbi Docks (KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS) it is
/// Tolbi Docks. In the Kalay room a man sells the boarding ticket, 600 coins
/// for the four of them (FLAG_KALAY_DOCKS_TICKET_BOUGHT), until the ship has
/// sailed, when he says it will not leave again for the monsters at sea; in the
/// Tolbi room no tickets are sold at all, the sea being too dangerous.
///
/// Tolbi Docks (TolbiDocks) is where the ship lands after the crossing,
/// on which the party helped the crew fight off the Kraken. Stepping off
/// (trigger 20, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED) the party is met by a
/// man on the docks who has heard the ship was attacked by a monster; if the
/// party makes light of it, he will not believe they fought the monsters alone:
/// "Everyone knows that Kaja was the one who killed the Kraken!" Kaja is the
/// sea dog whose crew the party helped against the monsters. The father's
/// daughter (ACTOR_WAITING_DAUGHTER) watches the sea for her parents and takes
/// his message (FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED). Two stacks of crates
/// slide along the pier under Move, a chest holds a Potion and a box 35 coins,
/// and the docks lead to the world map toward Tolbi (TOLBI_DOCKS_TO_WORLD_MAP),
/// back onto the ship (TOLBI_DOCKS_TO_TOLBI_SHIP_DECK) and into the building
/// (trigger 10). After Colosso the Colosso Tour's passengers are stranded
/// here, the ships not sailing, and the first time the party comes (trigger
/// 50, FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN) it watches them demand to be taken
/// home and the tour guide's hired bodyguard knock the angriest flat;
/// KalayTolbiDocks_PrepareTolbiDocks keeps them where the scene left them.

#include "game_state.h"
#include "djinn_ids.h"
#include "version.h"
#include "unit/scene/kalay_tolbi_docks.h"
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
#include "scene_ids.h"
#include "types.h"
#include "field_event.h"
#include "call.h"
#include "field_service.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// Kalay Docks' people, numbering the rows of gKalayTolbiDocks_KalayPlacements
/// from ACTOR_FIRST_PLACED; Garet's row takes his own actor slot. Once the
/// ship has crossed (gKalayTolbiDocks_KalayPlacementsShipGone) the first four
/// stand off the map.
enum KalayDocksActor {
  /// "Do you want to go to Tolbi?" He walks over to tell Garet the Karagol is
  /// no real ocean.
  ACTOR_VILLAGER_ASKING_ABOUT_TOLBI = ACTOR_FIRST_PLACED,
  /// "We can't take Silk Road, so everyone's in a pickle."
  ACTOR_VILLAGER_IN_A_PICKLE,
  /// "You can't take Silk Road much further than this." XXX: likely the
  /// waiting daughter's mother, because Mind Read finds "My daughter left
  /// Tolbi to meet us at the docks."
  ACTOR_VILLAGER_SILK_ROAD_ENDS,
  ACTOR_WORRIED_FATHER, ///< his daughter waits on the Tolbi Docks
  /// Takes the tickets at the ship's plank. "Tickets, please."
  /// ★ Named from the game: "Tickets, please. Please show your tickets."
  /// (MsgKalayTicketsPlease)
  ACTOR_CHECKING_TICKETS,
  ACTOR_DOCK_WORKER, ///< "I'm glad I only work on the docks"
  ACTOR_GROUND       ///< Ground, a Venus Djinni, placed until it joins
};

/// Tolbi Docks' people after the crossing, numbering the rows of
/// gKalayTolbiDocks_TolbiPlacements from ACTOR_FIRST_PLACED.
enum TolbiDocksActor {
  /// Hears of the party's scary trip; after Colosso, "There are many here who
  /// want to return to Kalay since Colosso is over."
  /// ★ Named from the game: "I hear you had a scary trip."
  /// (MsgKalayHearScaryTrip)
  ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP = ACTOR_FIRST_PLACED,
  /// "Sigh... I think we're done sailing for a while."; after Colosso,
  /// "Hanging around here like this and staring out at the sea is boring!"
  ACTOR_VILLAGER_DONE_SAILING,
  /// "The tourists look a little upset. Did they get seasick?"
  ACTOR_VILLAGER_ASKING_IF_SEASICK,
  ACTOR_WAITING_DAUGHTER,
  /// "If you're going to Tolbi, keep heading west. It's not too far."
  ACTOR_VILLAGER_POINTING_WEST,
  /// "It's been quite some time since the Colosso Tour passengers left for
  /// Tolbi."
  ACTOR_VILLAGER_TOUR_LONG_GONE,
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
/// of gKalayTolbiDocks_TolbiPlacementsStranded that hold other people than
/// gKalayTolbiDocks_TolbiPlacements. The villager who hears of the scary trip,
/// the one done sailing and the crates keep their rows.
enum TolbiDocksStrandedActor {
  /// "To heck with you! I'm staying right here until the ship leaves!"
  ACTOR_ANGRY_PASSENGER = ACTOR_FIRST_PLACED + 2,
  /// "Sigh. I wonder when we'll be able to return to Kalay."
  ACTOR_STRANDED_PASSENGER,
  /// The other guide, sprite 0xab as the tour guide's: "it's not our fault
  /// the ship won't sail, so we can't offer any refunds."
  ACTOR_SECOND_TOUR_GUIDE,
  /// Refunded the angry passenger's ticket.
  /// ★ Named from the game: "I returned the cost of your ticket to you."
  /// (MsgKalayReturnedTicketCost)
  ACTOR_TOUR_GUIDE,
  /// Hired by the tour guides.
  /// ★ Named from the game: "I might as well make some cash as a bodyguard."
  /// (MsgKalayTolbiDocksAsLongAsWereStuckHere)
  ACTOR_BODYGUARD = ACTOR_FIRST_PLACED + 8,
  /// "Sigh. If I knew it would turn out like this, we wouldn't have come."
  /// ★ Named from the game: "Sigh. If I knew it would turn out like this"
  /// (MsgKalayTolbiDocksSighIfIKnewItWould)
  ACTOR_SIGHING_PASSENGER
};

/// The building's people, numbering the rows of
/// gKalayTolbiDocks_InteriorPlacements from ACTOR_FIRST_PLACED: the Kalay
/// room's three, then the Tolbi room's. Once the ship has crossed
/// (gKalayTolbiDocks_InteriorPlacementsShipGone) the Kalay room's two
/// travellers stand off the map.
enum KalayTolbiDocksInteriorActor {
  /// In the Kalay room.
  /// ★ Named from the game: "Are you looking for tickets for boarding?"
  /// (MsgKalayLookingForTickets)
  ACTOR_SELLING_TICKETS = ACTOR_FIRST_PLACED,
  /// "Hmmm, that's strange. Isn't the replacement ship here yet?"
  ACTOR_VILLAGER_AWAITING_REPLACEMENT_SHIP,
  /// "Once we've crossed the Karagol, we'll reach Tolbi. Ohh, I'm excited."
  ACTOR_VILLAGER_EXCITED_FOR_TOLBI,
  /// Sells no tickets in the Tolbi room. "I'm sorry, but we just can't sell
  /// any tickets right now."
  /// ★ Named from the game: "we just can't sell any tickets right now."
  /// (MsgKalayTolbiDocksImSorryButWeJustCant)
  ACTOR_SELLING_NO_TICKETS,
  /// "We rented out our house for Colosso, and now the ship's not launching!"
  ACTOR_VILLAGER_RENTED_OUT_HOUSE,
  /// "We rented out our house. If the ship won't sail, where will we sleep?"
  ACTOR_VILLAGER_NOWHERE_TO_SLEEP
};

/// The building after Colosso (gKalayTolbiDocks_InteriorPlacementsStranded):
/// the Tolbi room's last two rows hold other people.
enum KalayTolbiDocksInteriorStrandedActor {
  /// "Yeah...the ship's not going anywhere for now. Some rich guy was here
  /// throwing money around, but they still said no."
  ACTOR_VILLAGER_SHIP_GOING_NOWHERE = ACTOR_FIRST_PLACED + 4,
  /// "The Karagol Sea looks so calm from here. How can it be dangerous?"
  ACTOR_VILLAGER_SEA_LOOKS_CALM
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

/// Selects the entrances of Kalay Docks, Tolbi Docks or the building between.
const struct SceneEntrance *KalayTolbiDocks_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayDocks) {
    return gKalayTolbiDocks_KalayEntrances;
  }
  if (scene == (s32)&SceneId_TolbiDocks) {
    return gKalayTolbiDocks_TolbiEntrances;
  }
  if (scene == (s32)&SceneId_KalayTolbiDocksInteriors) {
    return gKalayTolbiDocks_InteriorEntrances;
  }
  return gKalayTolbiDocks_NoEntrances;
}

const struct SceneRegion *KalayTolbiDocks_GetRegions(void) { return NULL; }

/// The exits of the three scenes, each scene's after its header word (an exit
/// of kind 0): both docks lead to the world map, onto the ship
/// (SceneId_TolbiShipDeck) and into the building
/// (SceneId_KalayTolbiDocksInteriors), whose rooms lead back to their docks.
const u32 *KalayTolbiDocks_GetExits(void) { return gKalayTolbiDocks_Exits; }

/// Each scene's people before the crossing, once the ship has crossed and not
/// come back (FLAG_TOLBI_SHIP_REACHED_TOLBI), and once the Colosso Tour's
/// passengers are stranded after Colosso (FLAG_COLOSSO_OVER).
const struct ScenePlacement *KalayTolbiDocks_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayDocks) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayPlacementsShipGone;
    }
    return gKalayTolbiDocks_KalayPlacements;
  }

  if (scene == (s32)&SceneId_TolbiDocks) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiPlacementsStranded;
    }
    return gKalayTolbiDocks_TolbiPlacements;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocksInteriors) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_InteriorPlacementsStranded;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_InteriorPlacementsShipGone;
    }
    return gKalayTolbiDocks_InteriorPlacements;
  }

  return gKalayTolbiDocks_NoPlacements;
}

/// Tolbi Docks, stepping off the ship (trigger 20, until
/// FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, which coming off the ship by
/// TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK clears): the leader walks a few steps and
/// a man on the docks hurries over to stand beside the party.
static void KalayTolbiDocks_GreetArrivals(void) {
  Engine_EventBegin();
  GameFlag_Set(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 392, 296);
  Actor_SetSpeed(ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, 0x13333, 0x9999);
  Actor_WalkToAndWait(ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, 408, 296);
  Actor_FaceDirection(ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventEnd();
}

/// Selects each scene's events for the same three times as its people.
const struct SceneEvent *KalayTolbiDocks_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayDocks) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayEventsShipGone;
    }
    return gKalayTolbiDocks_KalayEvents;
  }

  if (scene == (s32)&SceneId_TolbiDocks) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiEventsStranded;
    }
    return gKalayTolbiDocks_TolbiEvents;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocksInteriors) {
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

/// Kalay Docks: a man asks whether the party wants to go to Tolbi. Yes, and he
/// says that past Silk Road's end the way is across the Karagol Sea; no, and
/// he suggests a long look around the ship.
static void KalayTolbiDocks_AskAboutTolbiCrossing(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayWantGoToTolbi);
  Engine_EventAskYesNo(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, 0);
  Engine_EventEnd();
}

/// Kalay Docks: the father asks whether the party is taking the ship. If it
/// is, he asks it to tell his daughter in Tolbi that the tickets were too
/// expensive for her mother to come (sets FLAG_KALAY_DOCKS_CARRYING_MESSAGE);
/// afterwards he repeats the message.
static void KalayTolbiDocks_TalkToWorriedFather(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) == 0) {
    Engine_EventSetMessage(MsgKalayGoingTakeShip);
    Event_OpenMessage(ACTOR_WORRIED_FATHER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Event_ShowMessage(ACTOR_WORRIED_FATHER, 0);
      GameFlag_Set(FLAG_KALAY_DOCKS_CARRYING_MESSAGE);
    } else {
      gEventWork->message++;
      Event_ShowMessage(ACTOR_WORRIED_FATHER, 0);
    }
  } else {
    Engine_EventSetMessage(MsgKalayPassMessageDaughter);
    Event_ShowMessage(ACTOR_WORRIED_FATHER, 0);
  }
  Engine_EventEnd();
}

/// Kalay Docks: spoken to on the shore after the party has handed over its
/// ticket and gone ashore, the ticket taker asks it to board the ship when it
/// is finished.
static void KalayTolbiDocks_RemindBoardShip(void) {
#if !TBS_PATCH_3
  ((void)Object_GetById(ACTOR_PARTY_LEADER));
  Engine_EventBegin();
#endif
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
#if TBS_PATCH_3
      Engine_EventBegin();
#endif
      Engine_EventSetMessage(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(ACTOR_CHECKING_TICKETS, 0);
      Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
#if TBS_PATCH_3
      Engine_EventEnd();
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
  struct FieldActor *leader;
  s16 cardinalFacing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cardinalFacing = ((leader->facing + 0x2000) & ~0x3fff);
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
      Engine_EventSetMessage(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(ACTOR_CHECKING_TICKETS, 0);
    } else {
      Engine_EventSetMessage(MsgKalayWantGoAshore);
      Event_OpenMessage(ACTOR_CHECKING_TICKETS, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(MsgKalayWantGoAshore + 1);
        Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
        Actor_WalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1288);
        Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
        Engine_EventWait(20);
        GameFlag_Set(FLAG_KALAY_DOCKS_WENT_ASHORE);
      } else {
        Engine_EventSetMessage(MsgKalayWantGoAshore + 2);
        Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
      }
    }
  } else {
    if ((u16)cardinalFacing != FACING_WEST) {
      return;
    }
    Engine_EventSetMessage(MsgKalayTicketsPlease);
    Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
    // Hand over the actual inventory ticket before walking past the gate.
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
      ticketOwner = PartyInventory_FindOwner(ITEM_BOAT_TICKET);
      ticketSlot = Inventory_Find(ticketOwner, ITEM_BOAT_TICKET);
      Engine_ActorSetAnimationAndWait(ACTOR_CHECKING_TICKETS, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1288);
      Actor_FaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
      gEventWork->message++;
      Event_ShowMessage(ACTOR_CHECKING_TICKETS, 0);
      Inventory_Discard(ticketOwner, ticketSlot);
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
  Engine_EventEnd();
}

/// Kalay Docks, once the tour's group has left Kalay: the dock worker explains
/// the tickets: hand one over to board, give it to the ticket taker, or wait
/// on board for the ship to sail.
static void KalayTolbiDocks_TalkToDockWorker(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    Engine_EventSetMessage(MsgKalayBoardWaitSetSail);
    Engine_EventOpenMessage(ACTOR_DOCK_WORKER, 0);
  } else if (Engine_GameFlagIsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Engine_EventSetMessage(MsgKalayHaveTicketsGiveHim);
    Engine_EventShowMessage(ACTOR_DOCK_WORKER, 0);
  } else {
    Engine_EventSetMessage(MsgKalayAfterHandingOverTicket);
    Engine_EventShowMessage(ACTOR_DOCK_WORKER, 0);
  }

  Engine_EventEnd();
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
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Engine_EventSetMessage(MsgKalayHandInTicketPlank);
    Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
  } else {
    Engine_EventSetMessage(MsgKalayLookingForTickets);
    Event_OpenMessage(ACTOR_SELLING_TICKETS, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Event_ShowMessageAndWait(ACTOR_SELLING_TICKETS, 0, 10);
    } else {
      gEventWork->message++;
      UiWork_PushValueSlot(ticketPrice, 5);
      Event_OpenMessage(ACTOR_SELLING_TICKETS, 0);
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
      if (Engine_EventChooseYesNo(-1, 0) == 1) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
        Engine_EventWait(10);
        Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
      } else if ((u32)ticketPrice > (u32)state->coins) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(10);
        gEventWork->message++;
        Event_PlayCue(SE_MENU_CANCEL);
        Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
      } else {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(10);
        gEventWork->message += 3;
        Event_ShowMessage(ACTOR_SELLING_TICKETS, 0);
        // Complete both inventory and coin changes only after confirmation.
        Engine_PartyGiveItem(ITEM_BOAT_TICKET, 0);
        GameFlag_Set(FLAG_KALAY_DOCKS_TICKET_BOUGHT);
        Party_AdjustCoins(-ticketPrice);
      }
    }
    Engine_EventEnd();
  }
}

/// Tolbi Docks: a man on the docks has heard the party was attacked by a
/// monster on the crossing, and asks whether that is true; told it was nothing,
/// he will not believe they fought the monsters alone, for Kaja killed the
/// Kraken.
static void KalayTolbiDocks_AskAboutRoughCrossing(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayHearScaryTrip);
  Engine_EventAskYesNo(ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, 0);
  Engine_EventEnd();
}

/// Tolbi Docks: a man asks whether the upset tourists got seasick.
static void KalayTolbiDocks_AskAboutSeasickTourists(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayTouristsLookUpset);
  Engine_EventAskYesNo(ACTOR_VILLAGER_ASKING_IF_SEASICK, 0);
  Engine_EventEnd();
}

/// Tolbi Docks: the daughter looks for her parents among the passengers. If the
/// party carries her father's message she asks for it and thanks them once it
/// is given; otherwise she turns back to the sea.
static void KalayTolbiDocks_TalkToWaitingDaughter(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED) != 0) {
    Actor_FaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgKalayThanksMessageParents);
    Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
    Engine_EventEnd();
  } else {
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_WAITING_DAUGHTER, EMOTE_IN_FRONT, 50);
    Actor_FaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgKalaySilkRoadBlocked);
    Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) != 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(ACTOR_WAITING_DAUGHTER, 0x102, 40);
      Event_OpenMessage(ACTOR_WAITING_DAUGHTER, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
        GameFlag_Set(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED);
      } else {
        Engine_EventWait(10);
        gEventWork->message++;
        Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
        Engine_EventWait(30);
      }
    } else {
      Engine_EventWait(10);
      Actor_FaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
      Engine_EventWait(30);
    }
    Engine_EventEnd();
  }
}

/// Kalay Docks or Tolbi Docks: walking north into the door (trigger 10) of the
/// building beside them, the door opens and the leader walks in by
/// TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS, which in Kalay Docks is
/// KALAY_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS by the same number.
static void KalayTolbiDocks_OpenDoorAndLeave(void) {
  Engine_EventBegin();
  Event_PlayCue(SE_DOOR_OPEN);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  if (gGameState.scene == (s32)&SceneId_KalayDocks) {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 1392);
    Map_AnimateCells(gKalayTolbiDocks_KalayDoorCells, 78, 86);
  } else {
    if (gGameState.scene == (s32)&SceneId_TolbiDocks) {
      Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
      Map_AnimateCells(gKalayTolbiDocks_TolbiDoorCells, 74, 9);
    }
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS);
  Engine_EventEnd();
}

/// Prepares whichever of the three scenes is entered. Entrance 90 sets
/// FLAG_COLOSSO_OVER, which leaves the tour's passengers stranded on the Tolbi
/// Docks. XXX: no exit leads to entrance 90; likely a start for testing, since
/// Babi's Palace's entrances 90 and 91 set story flags the same way.
s32 KalayTolbiDocks_Initialize(void) {
  if (gGameState.entrance == 90) {
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
  }
  if (gGameState.scene == (s32)&SceneId_KalayDocks) {
    KalayTolbiDocks_PrepareKalayDocks();
  } else if (gGameState.scene == (s32)&SceneId_TolbiDocks) {
    KalayTolbiDocks_PrepareTolbiDocks();
  } else if (gGameState.scene == (s32)&SceneId_KalayTolbiDocksInteriors) {
    KalayTolbiDocks_PrepareInterior();
  }
  return 0;
}

/// On the first arrival at Kalay Docks from the world map, Garet runs to see
/// the Karagol. Coming back off the ship clears the shore leave, and once the
/// tour's group has left Kalay the ticket taker stands at the ship's plank
/// until the party goes ashore.
static void KalayTolbiDocks_PrepareKalayDocks(void) {
  if (gGameState.entrance == KALAY_DOCKS_FROM_WORLD_MAP_SOUTH) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL) == 0) {
      GameFlag_Set(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL);
      KalayTolbiDocks_GaretSeesTheKaragol();
    }
  }
  if (gGameState.entrance == KALAY_DOCKS_FROM_TOLBI_SHIP_DECK) {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      GameFlag_Clear(FLAG_KALAY_DOCKS_WENT_ASHORE);
    }
  }
  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) == 0) {
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
  struct FieldActor *actor;
  u8 zero;

  KalayTolbiDocks_UpdateCrateCells();
  if (GameFlag_IsSet(FLAG_COLOSSO_OVER)) {
    Engine_ActorSetChildValue(ACTOR_SECOND_TOUR_GUIDE, 2);
  }
  if (gGameState.entrance == TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  if (gGameState.entrance == TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK) {
    Engine_GameFlagClear(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED)) {
    Actor_SetPosition(ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, PIXELS(408),
                      PIXELS(296));
    Actor_FaceDirection(ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, FACING_WEST,
                        0);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN)) {
    Actor_SetPosition(ACTOR_TOUR_GUIDE, PIXELS(280), PIXELS(296));
    Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTH, 0);
    Actor_SetPosition(ACTOR_BODYGUARD, PIXELS(288), PIXELS(280));
    Actor_FaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
    Actor_SetPosition(ACTOR_ANGRY_PASSENGER, PIXELS(232), PIXELS(304));
    Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_SOUTH, 0);
    Actor_SetPosition(ACTOR_STRANDED_PASSENGER, PIXELS(240), PIXELS(312));
    Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_NORTH, 0);
    actor = Object_GetById(ACTOR_ANGRY_PASSENGER);
    actor->collisionFlags = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[38] = 0;
    zero = 0;
    actor->sprite->rotation = 0xc000;
    actor = Object_GetById(ACTOR_STRANDED_PASSENGER);
    actor->priorityFlags = zero;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[21] |= 12;
  }
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER)) {
    Map_CopyCellAttributes(18, 18, 1, 1, 14, 18);
    Map_CopyCellAttributes(18, 18, 1, 1, 15, 18);
  }
}

/// The building between the docks opens with a window transition and no area
/// name.
static void KalayTolbiDocks_PrepareInterior(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
}

/// Tolbi Docks, after Move pushes either stack of crates: the pier's cells
/// under both are rebuilt.
static void KalayTolbiDocks_UpdateCrateCells(void) {
  s32 firstCratesColumn;
  s32 secondCratesColumn;

  firstCratesColumn = Object_GetById(ACTOR_FIRST_CRATES)->x.fixed >> 20;
  secondCratesColumn = Object_GetById(ACTOR_SECOND_CRATES)->x.fixed >> 20;

  Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, secondCratesColumn, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, firstCratesColumn, 11);

  KalayTolbiDocks_SettleCrates(ACTOR_FIRST_CRATES);
  KalayTolbiDocks_SettleCrates(ACTOR_SECOND_CRATES);
}

/// Stops a stack of crates' idle motion and sets its sprite priority.
static void KalayTolbiDocks_SettleCrates(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);

  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  Engine_ActorSetSpritePriority(actorId, 3);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/// Kalay Docks, the first entry from the world map
/// (KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, until
/// FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL): Garet runs to the edge of the docks and
/// marvels at the sea. "Incredible! So this is the ocean Kraden was talking
/// about!" Kraden is the scholar who has told Garet of the ocean. The man who
/// asks travellers about Tolbi (ACTOR_VILLAGER_ASKING_ABOUT_TOLBI) walks over,
/// shaking his head: the Karagol is surrounded by land, so it is not a real
/// ocean. Garet means to see a real one someday, nods with the leader and
/// rejoins the party.
static void KalayTolbiDocks_GaretSeesTheKaragol(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Actor_SetPosition(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, PIXELS(328),
                    PIXELS(1424));
  Object_GetById(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI)->paused = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 0, FACING_WEST);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  // Follow Garet to the water before the man joins him.
  Engine_EventSetMessage(MsgKalayIncredibleOcean);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_SetSpeed(GARET, 0x19999, 0xcccc);
  Actor_WalkToAndWait(GARET, 232, 1424);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Camera_MoveTo(PIXELS(184), -1, PIXELS(1440), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 6, 15);
  Engine_ActorJump(GARET, 6, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(1448), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, EMOTE_IN_FRONT, 50);
  Actor_SetSpeed(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, 0x13333, 0x9999);
  Actor_WalkToAndWait(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, 264, 1424);
  Actor_FaceDirection(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI,
                                  ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Engine_EventWait(50);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkToAndWait(GARET, 264, 1464);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  // Garet rejoins the leader and the man wanders again from where he stands.
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventWait(20);
  Object_GetById(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI)->paused = 0;
  Actor_SetBuiltinAction(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, 2);
  actor = Object_GetById(ACTOR_VILLAGER_ASKING_ABOUT_TOLBI);
  {
    s32 pixelX = actor->x.fixed / 0x10000;

    actor->timer = pixelX;
  }
  {
    s32 pixelZ = actor->z.fixed / 0x10000;

    actor->work = pixelZ;
  }
  Engine_EventEnd();
}

/// Tolbi Docks after Colosso, the first time the party comes to the pier
/// (trigger 50, until FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN): it watches the tour
/// guide tell an angry passenger his ticket was refunded. The passengers
/// demand to be taken home, the ships being stuck and the road blocked; when
/// the angry one charges, the guides' hired bodyguard steps in, and after a
/// second try the passenger is knocked flat and the bodyguard spins on the
/// spot.
static void KalayTolbiDocks_PassengersConfrontTourGuide(void) {
  struct FieldActor *actor;

  GameFlag_Set(FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgKalayReturnedTicketCost);
  actor = Object_GetById(ACTOR_STRANDED_PASSENGER);
  actor->priorityFlags = 0;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[21] |= 12;
  Camera_MoveTo(PIXELS(232), -1, PIXELS(304), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ANGRY_PASSENGER, EMOTE_IN_FRONT | 7, 50);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_EAST, 0);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 4, 13);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 4, 30);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_STRANDED_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_STRANDED_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ANGRY_PASSENGER, EMOTE_IN_FRONT | 3, 55);
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0x20000, 0x10000);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 16, 0);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 7, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 24, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_ANGRY_PASSENGER, -16, 0);
  Event_PlayCue(SE_HIGH_JUMP);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x26666, 0x13333);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, 16, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_ANGRY_PASSENGER, ANIM_STAND);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | 2);
  Engine_ActorStartRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
  Event_PlayCue(SE_155);
  Engine_EventWait(10);
  Event_PlayCue(SE_155);
  Engine_EventWait(10);
  Event_PlayCue(SE_155);
  Engine_EventWait(10);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x6666, 0x3333);
  Engine_ActorJump(ACTOR_TOUR_GUIDE, 6, 0);
  Event_PlayCue(SE_159);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, -8, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | 2, 70);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 0);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_NORTHEAST, 0);
  Engine_EventWait(35);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(55);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_STRANDED_PASSENGER, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_STRANDED_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTHWEST, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0x13333, 0x9999);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 8, 0);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x20000, 0x10000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 16);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_WEST, 0);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ANGRY_PASSENGER, EMOTE_IN_FRONT | 2, 50);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -8, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -16, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_ANGRY_PASSENGER, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 8, 0);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 6, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 24, 0);
  Event_PlayCue(SE_133);
  Engine_ActorJump(ACTOR_BODYGUARD, 6, 0);
  // The passenger bounces off the spinning bodyguard and lands on his back.
  Actor_SetActionScript(ACTOR_BODYGUARD, gKalayTolbiDocks_BodyguardSpinScript);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 6, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -12, 4);
  actor = Actor_Get(ACTOR_ANGRY_PASSENGER);
  actor->collisionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[38] = 0;
  actor->sprite->rotation = 0xc000;
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -12, 4);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_SOUTH, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_PlayCue(SE_159);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_STRANDED_PASSENGER, EMOTE_IN_FRONT | 2, 50);
  Actor_SetSpeed(ACTOR_STRANDED_PASSENGER, 0x18000, 0xc000);
  Actor_WalkByAndWait(ACTOR_STRANDED_PASSENGER, 24, 0);
  Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_NORTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_STRANDED_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_EAST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, 24, -24);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, 8, 0);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, 0, -8);
  Engine_EventWait(10);
  Engine_EventEnd();
}

u16 gKalayTolbiDocks_KalayDoorCells[] = {
    MAP_CELL_STEP(98, 84, 2, 2, 5),
    MAP_CELL_STEP(98, 82, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gKalayTolbiDocks_TolbiDoorCells[] = {
    MAP_CELL_STEP(92, 9, 2, 2, 5),
    MAP_CELL_STEP(92, 7, 2, 2, 5),
    MAP_CELL_STEPS_END,
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
    {KALAY_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS, CONDITION_ALWAYS, 304, 0,
     1416, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
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
    {TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS, CONDITION_ALWAYS, 240, 0,
     216, FACING_SOUTH, 0, 0, 0, 448, 432, 0},
    {SCENE_TABLE_END},
};

/// The building's entrances, each into the room at the dock it is entered
/// from.
struct SceneEntrance gKalayTolbiDocks_InteriorEntrances[] = {
    {0, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS, CONDITION_ALWAYS, 96, 0, 248,
     FACING_NORTH, 0, 32, 32, 272, 272, 0},
    {KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 384, 0,
     248, FACING_NORTH, 0, 320, 32, 560, 272, 0},
    {SCENE_TABLE_END},
};

/// Kalay Docks' exits, then Tolbi Docks' and the building's, each after the
/// header word (an exit of kind 0) for its scene.
u32 gKalayTolbiDocks_Exits[] = {
    SCENE_EXITS(KalayDocks),
    SCENE_EXIT(KALAY_DOCKS_TO_WORLD_MAP_SOUTH, WorldMap,
               WORLD_MAP_FROM_KALAY_DOCKS_EAST),
    SCENE_EXIT(KALAY_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_KALAY_DOCKS),
    SCENE_EXIT(KALAY_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS,
               KalayTolbiDocksInteriors,
               KALAY_TOLBI_DOCKS_INTERIORS_FROM_KALAY_DOCKS),
    SCENE_EXIT(KALAY_DOCKS_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_KALAY_DOCKS_WEST),
    SCENE_EXITS(TolbiDocks),
    SCENE_EXIT(TOLBI_DOCKS_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI_DOCKS),
    SCENE_EXIT(TOLBI_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
    SCENE_EXIT(TOLBI_DOCKS_TO_KALAY_TOLBI_DOCKS_INTERIORS,
               KalayTolbiDocksInteriors,
               KALAY_TOLBI_DOCKS_INTERIORS_FROM_TOLBI_DOCKS),
    SCENE_EXITS(KalayTolbiDocksInteriors),
    SCENE_EXIT(KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS, KalayDocks,
               KALAY_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS),
    SCENE_EXIT(KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS, TolbiDocks,
               TOLBI_DOCKS_FROM_KALAY_TOLBI_DOCKS_INTERIORS),
    SCENE_EXITS_END,
};

struct ScenePlacement gKalayTolbiDocks_NoPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalayTolbiDocks_TolbiPlacements[] = {
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x93, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(344), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocks_TolbiPlacementsStranded[] = {
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(296), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x93, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(344), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(304), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(320), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(304), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(288), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocks_KalayPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(1408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(1224), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(1304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(1336), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(1288), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(120), 0, PIXELS(1368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x35, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocks_KalayPlacementsShipGone[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(1352), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(1288), FACING_WEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x5a, 0x35, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocks_InteriorPlacements[] = {
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(192), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocks_InteriorPlacementsShipGone[] = {
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocks_InteriorPlacementsStranded[] = {
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKalayTolbiDocks_NoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_KalayEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
    { EVENT_TALK, ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutTolbiCrossing) },
    { EVENT_TALK, ACTOR_VILLAGER_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
    { EVENT_TALK, ACTOR_VILLAGER_SILK_ROAD_ENDS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
    { EVENT_TALK, ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWorriedFather) },
    { EVENT_TALK, ACTOR_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
    { EVENT_TALK, ACTOR_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
    { 0x400, ACTOR_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketTaker) },
    { 0x8400, ACTOR_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketTaker) },
#if !TBS_PATCH_3
    { EVENT_TALK, ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#else
    { 0xc400, ACTOR_CHECKING_TICKETS, IF_FLAG_SET(FLAG_KALAY_DOCKS_WENT_ASHORE), EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#endif
    { EVENT_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToDockWorker) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SILK_ROAD_ENDS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
    { EVENT_TALK, ACTOR_GROUND, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_GroundJoins) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_KalayEventsShipGone[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
    { EVENT_TALK, ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
    { EVENT_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
    { EVENT_TALK, ACTOR_GROUND, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_GroundJoins) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_TolbiEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_TOUCH, 20, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, EVENT_SCRIPT(KalayTolbiDocks_GreetArrivals) },
    { EVENT_TALK, ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutRoughCrossing) },
    { EVENT_TALK, ACTOR_VILLAGER_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
    { EVENT_TALK, ACTOR_VILLAGER_ASKING_IF_SEASICK, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutSeasickTourists) },
    { EVENT_TALK, ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { EVENT_TALK, ACTOR_VILLAGER_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYoureGoingToTolbiKeep) },
    { EVENT_TALK, ACTOR_VILLAGER_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_DAUGHTER, IF_FLAG_SET(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED), MSGNO(MsgKalayTolbiDocksItWasAHardCrossingVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_IF_SEASICK, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAfterSeeingTheLooksOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGetTheFeelingThatIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCrossedTheSeaJustFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGuessWeWontBeSeeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIThinkThatMissingShipMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(ITEM_POTION) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_TolbiEventsStranded[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_TOUCH, 50, FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN, EVENT_SCRIPT(KalayTolbiDocks_PassengersConfrontTourGuide) },
    { EVENT_TALK, ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThereAreManyHereWhoWant) },
    { EVENT_TALK, ACTOR_VILLAGER_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHangingAroundHereLikeThisAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfBothTheCaptainAndKaja) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfKajaAndTheOthersSay) },
    { EVENT_TALK, ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTourGroupsShouldTakeCareOf) },
    { EVENT_TALK, ACTOR_ANGRY_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksToHeckWithYouImStaying) },
    { EVENT_TALK, ACTOR_STRANDED_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIWonderWhenWellBe) },
    { EVENT_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellBeCountingOnYouIf) },
    { EVENT_TALK, ACTOR_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsWereStuckHere) },
    { EVENT_TALK, ACTOR_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIfIKnewItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfTheydJustLetTheShip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANGRY_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfImGoingToBeStuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksGrrrWhyDontYouGetUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsYoureHereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThisTourWasntWellOrganizedThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfGrandmaIsDoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_CRATES, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateCrateCells) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(ITEM_POTION) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_InteriorEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS },
    { EVENT_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketSeller) },
    { EVENT_TALK, ACTOR_VILLAGER_AWAITING_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement) },
    { EVENT_TALK, ACTOR_VILLAGER_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, FLAG_KALAY_DOCKS_TICKET_BOUGHT, MSGNO(MsgKalayTolbiDocksIFeelSortOfBadThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYouveAlreadyBoughtYourTicket) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AWAITING_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontLeaveHereTill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksLetsGetGoingImNotGetting) },
    { EVENT_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { EVENT_TALK, ACTOR_VILLAGER_RENTED_OUT_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { EVENT_TALK, ACTOR_VILLAGER_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_RENTED_OUT_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_InteriorEventsShipGone[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS },
    { EVENT_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { EVENT_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { EVENT_TALK, ACTOR_VILLAGER_RENTED_OUT_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { EVENT_TALK, ACTOR_VILLAGER_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_RENTED_OUT_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_InteriorEventsStranded[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_KALAY_DOCKS },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KALAY_TOLBI_DOCKS_INTERIORS_TO_TOLBI_DOCKS },
    { EVENT_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { EVENT_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImStuckIWantToCross) },
    { EVENT_TALK, ACTOR_VILLAGER_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere) },
    { EVENT_TALK, ACTOR_VILLAGER_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveGotPassengersButNobodysSailing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheCaptainMustBeAStubborn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheMarineBlueKaragolSeaIs) },
    { SCENE_EVENTS_END },
};
// clang-format on
