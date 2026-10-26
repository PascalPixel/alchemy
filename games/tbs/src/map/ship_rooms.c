/// @file
/// Ship rooms (MapId_ShipRooms), a sheet of rooms drawn as a ship's cabins, all
/// of them below the decks of the Tolbi-bound Ship: one of the ship's three
/// maps on its voyage from Kalay to Tolbi, told in tolbi_ship_deck.c. The map
/// is four rooms, each a camera box of the warp destination table, whose warp
/// destinations are named by room: the passengers' hold
/// (SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK and the others named for
/// it), the cargo hold (SHIP_ROOMS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK), the rowing
/// deck where the oarsmen row (the SHIP_ROOMS_ROWING_DECK warp destinations and
/// SHIP_ROOMS_FROM_WORLD_MAP) and the captain's quarters (the
/// SHIP_ROOMS_CAPTAINS_ROOM warp destinations). Three warps lead up to the
/// deck, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK from the hold,
/// SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK from the cargo hold and
/// SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK from the captain's quarters, and
/// SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK and
/// SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD join the hold and the rowing deck.
/// The object event templates and map events are chosen by the room and the
/// voyage's stage; most of the voyage's events play here, at the warp
/// destinations from SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF on,
/// through the shared operations of ShipRooms_RunEventCommand. In story order:
///
/// - At Kalay, while the ship waits for its replacement from Tolbi. Once the
///   tour group has left Kalay (FLAG_KALAY_COLOSSO_TOUR_LEFT), walking into
///   the hold (MB_SHIP_ROOMS_HOLD_WARRIORS) has Ouranos, a warrior
///   bound for Colosso who "can't stand water", threaten to go ashore, and he
///   and his friend Sean go to make the captain sail
///   (FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN). The crew's talk of the delay
///   plays here (MB_SHIP_ROOMS_QUARTERS_CREW) if it has not on
///   deck. In the captain's quarters the warriors and passengers press the
///   captain, who has just decided to sail
///   (FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL); Ouranos and Sean offer to guard
///   the deck with Kaja, Sean thinks of the young warriors among the
///   passengers, and then the captain finds his lucky anchor charm gone and
///   will not sail without it. Giving him the Anchor Charm, found on the mast,
///   plays ShipRooms_ReturnLuckyAnchor: Sean names the party to guard
///   the oarsmen, and the party agrees (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED).
/// - Setting sail: on the rowing deck Kaja tells the oarsmen they sail without
///   the replacement (FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT), the party
///   guarding them below while he, Ouranos and Sean guard the deck, and that if
///   an oarsman falls the party must choose a passenger to row in his place,
///   keeping the two teams balanced. After the deck and the mast, the captain
///   casts off in the captain's quarters
///   (SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF,
///   raising the charm), a passenger is called to the oars
///   (SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS) and
///   the rowing deck rows
///   (SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING), which leads
///   to the world map for the next leg.
/// - The attacks, each called by the lookout on the mast or the deck; before
///   the first, the captain in the captain's quarters hears of monsters
///   (SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS) and
///   turns the ship
///   (SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN). Then
///   monsters are belowdecks
///   (SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK and
///   the second and third attacks'). While they are aboard the stairs are
///   barred (FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS) and the leader is turned back
///   from them; talking to a monster on the rowing deck fights it (the first,
///   second or third slot of the Tolbi ship's scripted battles, by the attack),
///   returning to SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE or
///   the second or third attack's, which record the attack fought off
///   (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, _SECOND_ATTACK_FOUGHT_OFF and
///   _THIRD_ATTACK_FOUGHT_OFF) and leave an oarsman hurt. The party then
///   chooses a passenger in the hold to row: the attacks fought off number the
///   seat being filled, and each seat has a group of nine flags, one for each
///   passenger (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS and the rest); the passenger
///   follows the party (FLAG_TOLBI_SHIP_ROWER_CHOSEN) to the rowing deck
///   (SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD), where Kaja takes
///   them on. Each passenger answers being asked in their own way, and their
///   rowing values on the mast decide the course.
/// - The Kraken: SHIP_ROOMS_FROM_WORLD_MAP, the ship rocking,
///   "there's a huge sea monster on the ship!" (FLAG_TOLBI_SHIP_KRAKEN_ON_DECK
///   shakes the event until it is defeated on deck).
/// - The island: off course, a short event in the captain's quarters
///   (SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND) leads by
///   the world map (SHIP_ROOMS_TO_WORLD_MAP) to
///   SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND, where the rowers
///   rest at the island (FLAG_TOLBI_SHIP_AT_ISLAND) and the hold and captain's
///   quarters have their own talk. Back from it,
///   SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS has
///   Kaja switch the left and right teams, the left team's timing having taken
///   the ship off course, and the ship sets off for Tolbi.
/// - SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT, where a defeat at sea
///   returns: it clears the voyage's flags from
///   FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT to _KRAKEN_DEFEATED and the
///   rowers chosen, sets FLAG_TOLBI_SHIP_DEFEATED_AT_SEA and leads up to the
///   deck (SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK), so the
///   voyage starts over.
///
/// A travelling healer opens a Sanctum in the hold. At Tolbi
/// (FLAG_TOLBI_SHIP_REACHED_TOLBI) the captain's quarters and the hold have
/// their own object event templates.

#include "action_script.h"
#include "flags.h"
#include "songs.h"
#include "heap.h"
#include "audio_engine_symbols.h"
#include "macros.h"
#include "save_block.h"
#include "unit/import.h"
#include "unit/map/ship_rooms.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "task.h"
#include "map_ids.h"
#include "inventory.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "map_header.h"

/// The visible anchor charm has been created this visit.
/// ★ Named from the game: "Anchor Charm" (MsgItemName)
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN FLAG_MAP_TEMP_00

// Map section flags: Map_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another map section, and other map sections use the numbers for their
// own.
/// A passenger chosen to row follows the party to the rowing deck.
/// ★ Named from the game: "Looks like you've been chosen to help row"
/// (MsgTolbiShipLooksLikeYouveBeenChosen)
#define FLAG_TOLBI_SHIP_ROWER_CHOSEN 0x300
/// The hold's stairs are barred while monsters are on the rowing deck.
/// ★ Named from the game: "Monsters belowdecks!"
/// (MsgTolbiShipMonstersBelowdecks)
#define FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS 0x301
/// The Kraken is on deck: the deck shakes until it is defeated
/// (tolbi_ship_deck.c reads it too).
/// ★ Named from the game: "there's a huge sea monster on the ship!"
/// (MsgTolbiShipWonderWhatsWrongShipShouldnt)
#define FLAG_TOLBI_SHIP_KRAKEN_ON_DECK 0x302

/// Operations shared by the captain's quarters, passenger hold and rowing-deck
/// scripts.
enum ShipEventCommand {
  SHIP_FACE_PARTY_EAST_AND_WEST,
  SHIP_FACE_PARTY,
  SHIP_PLAY_PARTY_ANIMATION,
  SHIP_SHOW_PARTY_EMOTE,
  SHIP_HIDE_FIRST_OARSMEN,
  SHIP_START_ROWING_ANIMATION,
  SHIP_START_MIRRORED_ROWING_ANIMATION,
  SHIP_SET_SELECTED_ROWER_POSE,
  SHIP_OPEN_SCREEN,
  SHIP_CLOSE_SCREEN,
  SHIP_SETUP_ROWING_PARTY,
  SHIP_RESTORE_ROWING_DECK_POSES,
  SHIP_RESTORE_ROWER_POSE,
  SHIP_HIDE_QUARTERS_CREW,
  SHIP_HIDE_OURANOS_AND_SEAN,
  SHIP_SETUP_CAPTAINS_QUARTERS,
  SHIP_SETUP_RECRUITMENT_DECK,
  SHIP_HIDE_OARS,
  SHIP_SETUP_PASSENGER_HOLD,
  SHIP_SETUP_MONSTER_ENCOUNTER,
  SHIP_CLEAR_FLAG_RANGE,
  SHIP_CLEAR_ROWING_SELECTIONS,
  SHIP_RESTORE_SHIVERING_OARSMAN,
  SHIP_HIDE_PARTY_FOLLOWERS,
  SHIP_RESET_CAMERA,
  SHIP_PLACE_FOLLOWERS_AND_MONSTERS
};

/// The nine passengers who may be called to the oars, in the order of their
/// rows in gShipRooms_HoldObjectEventTemplatesAtSea and
/// gShipRooms_RowingDeckObjectEventTemplates and of each seat's group
/// of flags (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + passenger). The game gives
/// them no names, so each is named by what they say, or by what the guides call
/// themselves.
enum TolbiShipPassenger {
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), the tour's guide.
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  PASSENGER_HEAD_GUIDE,
  /// The tour's other guide, a man carrying a pack drawn as kalay.c's
  /// LOCALID_SECOND_GUIDE (child value 2).
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  PASSENGER_SECOND_GUIDE,
  /// A chef (SPRITE_CHEF).
  /// ★ Named from the game: "I've been in a cold sweat"
  /// (MsgTolbiShipInteriorsIveBeenInAColdSweat)
  PASSENGER_SWEATING,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN).
  /// ★ Named from the game: "you're going to ask a lady to row?"
  /// (MsgTolbiShipDontTellGoing)
  PASSENGER_LADY,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN), who knows he will be
  /// chosen.
  /// ★ Named from the game: "When our eyes met, I knew that was it."
  /// (MsgTolbiShipInteriorsOhNoNoWhenOurEyes)
  PASSENGER_STARED_AT,
  /// A woman with a sword (SPRITE_PERSON_WITH_SWORD).
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  PASSENGER_WARRIOR,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  PASSENGER_TICKET_GRUMBLER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  PASSENGER_MUSCLE_ADMIRER,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  PASSENGER_OLD_MAN
};

/// The captain's quarters, numbering the rows of
/// gShipRooms_CaptainsQuartersObjectEventTemplates and
/// gShipRooms_CaptainsQuartersObjectEventTemplatesWarriors from
/// LOCALID_FIRST_PLACED; the party's rows take their own slots. The captain,
/// Kaja and the crewman have the sprites they have on deck
/// (tolbi_ship_deck.c).
/// ★ Named from the game: "To the captain's quarters, of course."
/// (MsgTolbiShipIfWeArentGoingSet)
enum TolbiShipCaptainsQuartersLocalId {
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  /// ★ Named from the game: "Now we can set sail. Right, Captain?"
  /// (MsgTolbiShipItsMyLuckyAnchor)
  LOCALID_QUARTERS_CAPTAIN = LOCALID_FIRST_PLACED,
  /// A bald man with a beard (SPRITE_KAJA).
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  LOCALID_QUARTERS_KAJA,
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who hid
  /// the captain's anchor charm.
  /// ★ Named from the game: "I have to confess that I hid it on top of the
  /// mast." (MsgTolbiShipInteriorsIWaitedTooLongIllGet)
  LOCALID_QUARTERS_CREWMAN,
  /// ★ Named from the game: "You know, my friend Ouranos here is scared of
  /// ships." (MsgTolbiShipIfWeArentGoingSet)
  LOCALID_QUARTERS_OURANOS,
  /// ★ Named from the game: "Well, it's just like Sean said."
  /// (MsgTolbiShipItsMyLuckyAnchor)
  LOCALID_QUARTERS_SEAN,
  /// PASSENGER_SECOND_GUIDE, drawn the same way.
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  LOCALID_QUARTERS_SECOND_GUIDE
};

/// The hold while the ship waits at Kalay and after it reaches Tolbi,
/// numbering the rows of gShipRooms_HoldObjectEventTemplates from
/// LOCALID_FIRST_PLACED: four of the passengers and the second guide, then the
/// warriors bound for Colosso, Ouranos and his friend Sean, the travelling
/// healer and Kaja.
enum TolbiShipHoldLocalId {
  /// PASSENGER_WARRIOR.
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  LOCALID_HOLD_WARRIOR = LOCALID_FIRST_PLACED,
  /// PASSENGER_TICKET_GRUMBLER.
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  LOCALID_HOLD_TICKET_GRUMBLER,
  /// PASSENGER_MUSCLE_ADMIRER.
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  LOCALID_HOLD_MUSCLE_ADMIRER,
  /// PASSENGER_OLD_MAN.
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  LOCALID_HOLD_OLD_MAN,
  /// PASSENGER_SECOND_GUIDE.
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  LOCALID_HOLD_SECOND_GUIDE,
  /// ★ Named from the game: "It's not that I'm scared. I just can't stand
  /// water..." (MsgTolbiShipIfWeArentGoingSet)
  LOCALID_HOLD_OURANOS,
  /// ★ Named from the game: "Hey, maybe you can give us a hand, too, Sean!"
  /// (MsgTolbiTolbiShipThenTheyAreDangerous)
  LOCALID_HOLD_SEAN,
  /// A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE) who opens
  /// his Sanctum (ShipRooms_TalkToHealer).
  /// ★ Named from the game: "I'm spreading goodwill wherever I travel."
  /// (MsgTolbiShipImSpreadingGoodwillWhereverTravel)
  LOCALID_HOLD_HEALER,
  /// Calls the first passenger to the oars.
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  LOCALID_HOLD_KAJA
};

/// The hold at sea, numbering the rows of
/// gShipRooms_HoldObjectEventTemplatesAtSea from LOCALID_FIRST_PLACED:
/// the nine passengers in the order of enum TolbiShipPassenger, the healer and
/// Kaja. Each passenger's evidence is its enum TolbiShipPassenger line.
enum TolbiShipSeaHoldLocalId {
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  LOCALID_SEA_HOLD_FIRST_PASSENGER = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  LOCALID_SEA_HOLD_HEAD_GUIDE = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_HEAD_GUIDE,
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  LOCALID_SEA_HOLD_SECOND_GUIDE = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_SECOND_GUIDE,
  /// ★ Named from the game: "I've been in a cold sweat"
  /// (MsgTolbiShipInteriorsIveBeenInAColdSweat)
  LOCALID_SEA_HOLD_SWEATING_PASSENGER = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_SWEATING,
  /// ★ Named from the game: "you're going to ask a lady to row?"
  /// (MsgTolbiShipDontTellGoing)
  LOCALID_SEA_HOLD_LADY = LOCALID_SEA_HOLD_FIRST_PASSENGER + PASSENGER_LADY,
  /// ★ Named from the game: "When our eyes met, I knew that was it."
  /// (MsgTolbiShipInteriorsOhNoNoWhenOurEyes)
  LOCALID_SEA_HOLD_STARED_AT_PASSENGER = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_STARED_AT,
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  LOCALID_SEA_HOLD_WARRIOR = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_WARRIOR,
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  LOCALID_SEA_HOLD_TICKET_GRUMBLER = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  LOCALID_SEA_HOLD_MUSCLE_ADMIRER = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_MUSCLE_ADMIRER,
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  LOCALID_SEA_HOLD_OLD_MAN = LOCALID_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_OLD_MAN,
  /// ★ Named from the game: "The ship's crew is ready for anything, and I'll
  /// be right here for them." (MsgTolbiShipShipsCrewReadyAnything)
  LOCALID_SEA_HOLD_HEALER,
  /// Calls a replacement rower to the oars.
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  LOCALID_SEA_HOLD_KAJA
};

/// The rowing deck, numbering the rows of
/// gShipRooms_RowingDeckObjectEventTemplates from
/// LOCALID_FIRST_PLACED; the monsters of each attack are placed after them.
enum TolbiShipRowingDeckLocalId {
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA), one of the crew
  /// by his bandana.
  /// ★ Named from the game: "I've been sailing for many years, and I've never
  /// seen this island!" (MsgTolbiShipInteriorsIveBeenSailingForManyYears)
  LOCALID_ROWING_CREWMAN = LOCALID_FIRST_PLACED,
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA) who keeps the
  /// oarsmen's time.
  /// ★ Named from the game: "Everyone rows in time with this drum."
  /// (MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis)
  LOCALID_ROWING_DRUMMER,
  /// The eight oarsmen, men in red bandanas (SPRITE_MAN_IN_RED_BANDANA), a seat
  /// each, two by two from the north, the west seat
  /// first; the passengers chosen take the first four seats.
  /// ★ Named from the game: "These big muscles are a sign that we're
  /// oarsmen." (MsgTolbiShipInteriorsTheseBigMusclesAreASign)
  LOCALID_ROWING_FIRST_OARSMAN,
  /// The nine passengers in the order of enum TolbiShipPassenger.
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  LOCALID_ROWING_FIRST_PASSENGER = LOCALID_ROWING_FIRST_OARSMAN + 8,
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  LOCALID_ROWING_HEAD_GUIDE = LOCALID_ROWING_FIRST_PASSENGER +
      PASSENGER_HEAD_GUIDE,
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  LOCALID_ROWING_SECOND_GUIDE = LOCALID_ROWING_FIRST_PASSENGER +
      PASSENGER_SECOND_GUIDE,
  /// ★ Named from the game: "I've been in a cold sweat"
  /// (MsgTolbiShipInteriorsIveBeenInAColdSweat)
  LOCALID_ROWING_SWEATING_PASSENGER = LOCALID_ROWING_FIRST_PASSENGER +
      PASSENGER_SWEATING,
  /// ★ Named from the game: "you're going to ask a lady to row?"
  /// (MsgTolbiShipDontTellGoing)
  LOCALID_ROWING_LADY = LOCALID_ROWING_FIRST_PASSENGER + PASSENGER_LADY,
  /// ★ Named from the game: "When our eyes met, I knew that was it."
  /// (MsgTolbiShipInteriorsOhNoNoWhenOurEyes)
  LOCALID_ROWING_STARED_AT_PASSENGER = LOCALID_ROWING_FIRST_PASSENGER +
      PASSENGER_STARED_AT,
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  LOCALID_ROWING_WARRIOR = LOCALID_ROWING_FIRST_PASSENGER + PASSENGER_WARRIOR,
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  LOCALID_ROWING_TICKET_GRUMBLER = LOCALID_ROWING_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  LOCALID_ROWING_MUSCLE_ADMIRER = LOCALID_ROWING_FIRST_PASSENGER +
      PASSENGER_MUSCLE_ADMIRER,
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  LOCALID_ROWING_OLD_MAN = LOCALID_ROWING_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  /// Kaja: "Our replacement never arrived, but we're shoving off just the
  /// same."
  /// ★ Named from the game: "If you say so, Kaja. We trust your judgment."
  /// (MsgTolbiShipOurReplacementNeverArrivedBut)
  LOCALID_ROWING_KAJA,
  /// The eight oars, one beside each seat, hidden while the oarsmen row.
  /// ★ Named from what the player sees: an oar resting beside an empty seat
  /// (SPRITE_RESTING_OAR) (its render)
  LOCALID_ROWING_FIRST_OAR,
  /// The monsters of each attack (gShipRooms_FirstAttackMonsters and
  /// the two after it).
  /// ★ Named from what the player sees: three jellyfish monsters
  /// (SPRITE_JELLYFISH_MONSTER) among the oarsmen (its render)
  LOCALID_ROWING_FIRST_MONSTER = LOCALID_ROWING_FIRST_OAR + 8
};

/// The coord events and searched containers below decks whose map events are
/// more than a warp.
enum ShipRoomsMetatileBehavior {
  /// The rowing deck's stairs up to the hold, barred while monsters are
  /// aboard.
  MB_SHIP_ROOMS_ROWING_DECK_STAIRS = 4,
  /// In the captain's quarters, where the crew's talk of the delay plays.
  MB_SHIP_ROOMS_QUARTERS_CREW = 10,
  /// Inside the hold, where Ouranos threatens to go ashore.
  MB_SHIP_ROOMS_HOLD_WARRIORS = 11,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_SHIP_ROOMS_CRATE_HOLD_BARREL = 100,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_SHIP_ROOMS_HOLD_BARREL = 101,
  MB_SHIP_ROOMS_QUARTERS_JAR = 102
};

/// A speaker: the object event, with the speaker bits for where its message
/// window and portrait open.
#define SPEAKER(localId, bits) ((bits) | (localId))

/// ShipRooms_MoveCameraAndWait's flags, above the frames it waits
/// after in the low bits.
#define SHIP_CAMERA_JUMP 0x1 ///< jump there at once rather than glide
/// Then recenter the layers on the camera (MapGrid_CenterOnFollowedPoint).
#define SHIP_CAMERA_RECENTER_LAYERS 0x01000000
#define SHIP_CAMERA_WAIT_FOR_ARRIVAL 0x10000000

// Defined below, in the order of the ROM.
static void ShipRooms_StepBesideLeader(s32 passengerId);
static s32 ShipRooms_IsSpotFree(s32 bucket);
static s32 ShipRooms_GetChosenOarsman(void);
static void ShipRooms_AskToRow(s32 speaker, s32 question, s32 seatFlag);
static void ShipRooms_DiscussLaunchingWithoutAnchor(void);
static void ShipRooms_SetUpCaptainsQuarters(void);
static void ShipRooms_SetUpHold(void);
static void ShipRooms_SetUpRowingDeck(void);
static void ShipRooms_SetOarCollision(void);
static void ShipRooms_AskForReplacementOarsmen(void);
static void ShipRooms_CastOff(void);
static void ShipRooms_AnnounceDeparture(void);
static void ShipRooms_KajaCallsToOars(void);
static void ShipRooms_KajaCallsReplacementToOars(void);
static void ShipRooms_ShowRowing(void);
static void ShipRooms_AskAboutMonsters(void);
static void ShipRooms_AnnounceTurn(void);
static void ShipRooms_StartFirstAttack(void);
static void ShipRooms_EndFirstAttack(void);
static void ShipRooms_IntroduceReplacementOarsman(s32 seat);
static struct ObjectEvent *ShipRooms_PlaceAnchorCharm(void);
static void ShipRooms_InstallRowingDeckObjectEvents(u32 action);
static void ShipRooms_Say(s32 speaker);
static void ShipRooms_Turn(s32 localId, u16 facing);
static void ShipRooms_PlaceObjectEvent(s32 localId, s32 x, s32 z, s32 facing);
static void ShipRooms_MoveCameraAndWait(s32 x, s32 y, s32 z, u32 flags);
static void ShipRooms_RunEventCommand(s32 step, u32 arg, u32 opt);
static s32 ShipRooms_FindFirstSetFlag(u32 seat, s32 alternate);
static void ShipRooms_HideSelectedPassengers(void);
static void ShipRooms_HideSelectedPassenger(s32 localId, s32 firstFlag);
static void ShipRooms_StartSecondAttack(void);
static void ShipRooms_ReportMonstersBelowdecks(void);
static void ShipRooms_ReactToMonsterAttack(void);
static void ShipRooms_EndSecondAttack(void);
static void ShipRooms_StartThirdAttack(void);
static void ShipRooms_EndThirdAttack(void);
static void ShipRooms_NoticeShipRocking(void);
static void ShipRooms_ShakeWhileKrakenAboard(void);
static void ShipRooms_ShowCaptainsQuartersBeforeIsland(void);
static void ShipRooms_ReachIsland(void);
static void ShipRooms_DismissReplacementRowers(void);
static void ShipRooms_SwapRowerSelections(s32 firstSeatFlags,
                                          s32 secondSeatFlags);

/// An action-script call: stands the object event still at the origin, with no
/// velocity and no target. Returns 0.
static s32 ShipRooms_ResetObjectEvent(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  Object_SelectAnimation(objectEvent, 1);
  objectEvent->x.fixed = 0;
  objectEvent->y.fixed = 0;
  objectEvent->z.fixed = 0;
  SET_VELOCITY(objectEvent, 0, 0, 0);
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  return 0;
}

/// Bobs the object event between the floor and 8 pixels up by a random amount
/// each frame. Returns 1.
static s32 ShipRooms_Bob(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 verticalMotionDirection;
  s32 descendingY;
  s32 ascendingY;

  if ((s16)objectEvent->work != 0) {
    descendingY =
        (objectEvent->y.fixed - ((u32)(Import_RandomNext() << 15) >> 16)) -
        PIXELS(1) / 2;
    objectEvent->y.fixed = descendingY;
    if (descendingY < 0) {
      verticalMotionDirection = 0;
      objectEvent->work = verticalMotionDirection;
    }
  } else {
    ascendingY = objectEvent->y.fixed +
                 ((u32)(Import_RandomNext() << 15) >> 16) + PIXELS(1) / 2;
    objectEvent->y.fixed = ascendingY;
    if (ascendingY > PIXELS(8)) {
      verticalMotionDirection = 1;
      objectEvent->work = verticalMotionDirection;
    }
  }
  return 1;
}

/// Now and then (two chances in 64) turns the object event to face
/// south-southeast or south-southwest. Returns 1.
static s32 ShipRooms_LookAround(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  u32 choice = (u32)(Import_RandomNext(objectEvent) << 6) >> 16;

  if (choice == 6) {
    s32 facing = FACING_SOUTHEAST + FACING_STEP;
    objectEvent->facing = facing;
  } else if (choice == 9) {
    s32 facing = FACING_SOUTH + FACING_STEP;
    objectEvent->facing = facing;
  }
  return 1;
}

/// Counts the frames of a pause in riseCounter and moves the pacing object
/// event to its next step after 80 frames.
static void ShipRooms_WaitThenAdvance(struct ObjectEvent *objectEvent) {
  u8 *counter = &objectEvent->riseCounter;
  s32 frame = *counter + 1;

  *counter = frame;
  if ((u8)frame > 80) {
    objectEvent->work += 1;
  }
}

/// Counts down the object event's walking time in riseBaseY. Returns 1 once the
/// time is up or the object event has reached its target, else 0.
static s32 ShipRooms_HasArrived(struct ObjectEvent *objectEvent) {
  s32 remaining = objectEvent->riseBaseY;

  if (remaining != 0) {
    objectEvent->riseBaseY = remaining - 1;
  } else {
    return 1;
  }
  if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
      objectEvent->targetY == objectEvent->targetX &&
      objectEvent->targetZ == objectEvent->targetY) {
    return 1;
  }
  return 0;
}

/// The update of Kaja (LOCALID_QUARTERS_KAJA) while the ship waits off the
/// island: he paces between two spots in the captain's quarters, looking about
/// at each, and signals the captain (LOCALID_QUARTERS_CAPTAIN) through his own
/// riseEnabled at three points of the round, but not while the captain is
/// paused. work holds the step.
static void
ShipRooms_PaceCaptainsQuarters(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *captain;

  captain = ObjectEvent_GetById(LOCALID_QUARTERS_CAPTAIN);
  switch ((s16)objectEvent->work) {
  case 0:
    objectEvent->facing = FACING_NORTHWEST + FACING_STEP;
    objectEvent->work++;
    objectEvent->riseCounter = 0;
    break;
  case 2:
    objectEvent->facing = 0;
    objectEvent->work++;
    objectEvent->riseCounter = 0;
    break;
  case 4:
    Object_SelectAnimation(objectEvent, 2);
    Object_SetPosition(objectEvent, PIXELS(468), PIXELS(32), PIXELS(632));
    objectEvent->riseBaseY = 60;
    objectEvent->work++;
    break;
  case 5:
    if (ShipRooms_HasArrived(objectEvent) != 0) {
      Object_SelectAnimation(objectEvent, 1);
      objectEvent->riseCounter = 0;
      if (captain->paused == 0) {
        objectEvent->riseEnabled = 1;
      }
      objectEvent->work++;
    }
    break;
  case 7:
    if (captain->paused == 0) {
      Object_SelectAnimation(objectEvent, 3);
      objectEvent->riseEnabled = 2;
    }
    objectEvent->work++;
    objectEvent->riseCounter = 0;
    break;
  case 9:
    Object_SelectAnimation(objectEvent, 2);
    Object_SetPosition(objectEvent, PIXELS(480), PIXELS(32), PIXELS(600));
    objectEvent->riseBaseY = 60;
    objectEvent->work++;
    if (captain->paused == 0) {
      objectEvent->riseEnabled = 3;
    }
    break;
  case 10:
    if (ShipRooms_HasArrived(objectEvent) != 0) {
      Object_SelectAnimation(objectEvent, 1);
      objectEvent->riseCounter = 0;
      objectEvent->work++;
    }
    break;
  case 1:
  case 3:
  case 6:
  case 8:
  case 11:
    ShipRooms_WaitThenAdvance(objectEvent);
    break;
  case 12:
    objectEvent->work = 0;
    break;
  }
}

/// The update of the captain, who watches Kaja pace: turns north-northeast at
/// the first signal, shrugs at the second if still turned, and faces east at
/// the third. Does nothing while paused.
static void ShipRooms_WatchPacer(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *watcher = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *pacer;

  pacer = ObjectEvent_GetById(LOCALID_QUARTERS_KAJA);
  if (watcher->paused != 0)
    return;
  if (pacer->riseEnabled == 1) {
    watcher->facing = FACING_NORTH + FACING_STEP;
    watcher->riseCounter = 1;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 2) {
    if (watcher->riseCounter != 0)
      Object_SelectAnimation(watcher, 3);
    watcher->riseCounter = 0;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 3) {
    watcher->facing = 0;
    pacer->riseEnabled = 0;
  }
}

/// An action-script call: lays the object event flat on the floor, drawn
/// underfoot at priority 1 with its parts hidden. Returns 1.
static s32 ShipRooms_LayFlat(struct Object *interpreter) {
  struct ObjectEvent *work = (struct ObjectEvent *)interpreter;
  struct Sprite *sprite = work->sprite;

  work->collisionFlags = 8;
  ObjectEvent_SetSpriteFlags(work, 0);
  sprite->image.priority = 1;
  sprite->shadow.priority = 1;
  work->priorityFlags = (work->priorityFlags & ~1) | 2;
  ObjectEvent_SetSpriteColor(work, SPRITE_COLOR_HIDDEN);
  return 1;
}

const struct MapWarpDest *ShipRooms_SelectWarpDests(void) {
  if (gSaveBlock.mapId == (s32)&MapId_ShipRooms) {
    return gShipRooms_WarpDests;
  }
  return gShipRooms_WarpDestsOther;
}

const struct MapWarpRegion *ShipRooms_GetRegions(void) { return 0; }

const u32 *ShipRooms_GetWarps(void) { return gShipRooms_Warps; }

/// A byte view keeps the native address arithmetic; direct row-field writes
/// choose different bases and registers. The layout still belongs to the row.
#define TOLBI_SHIP_TALK_FACING(bytes, row)                                     \
  ((bytes)[(row) * sizeof(struct ObjectEventTemplate) +                        \
           offsetof(struct ObjectEventTemplate, talkFacing)])

/// Picks the object event templates for the room entered and the voyage's
/// stage. Some stages change who turns to face the party when talked to
/// (talkFacing) in the shared tables.
const struct ObjectEventTemplate *ShipRooms_SelectObjectEventTemplates(void) {
  u8 *templateBytes;

  switch (gSaveBlock.warpDest) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_HoldObjectEventTemplates;
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
        // Update talkFacing in the object event template rows, preserving their
        // write order.
        templateBytes = (u8 *)gShipRooms_HoldObjectEventTemplatesAtSea;
        TOLBI_SHIP_TALK_FACING(templateBytes, 0) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 2) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 4) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 5) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 8) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 7) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 6) = TALK_FACING_PARTY_AND_BACK;
        TOLBI_SHIP_TALK_FACING(templateBytes, 3) = TALK_FACING_KEEP;
      }
      return gShipRooms_HoldObjectEventTemplatesAtSea;
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
        gShipRooms_HoldObjectEventTemplates[0].talkFacing = TALK_FACING_KEEP;
        gShipRooms_HoldObjectEventTemplates[4].talkFacing = TALK_FACING_KEEP;
        gShipRooms_HoldObjectEventTemplates[1].talkFacing = TALK_FACING_KEEP;
        gShipRooms_HoldObjectEventTemplates[3].talkFacing = TALK_FACING_KEEP;
      }
      return gShipRooms_HoldObjectEventTemplates;
    } else {
      return gShipRooms_HoldObjectEventTemplatesEmpty;
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
  case SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
  case SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
  case SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
  case SHIP_ROOMS_FROM_WORLD_MAP:
  case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
    return gShipRooms_RowingDeckObjectEventTemplates;
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    templateBytes = (u8 *)gShipRooms_RowingDeckObjectEventTemplates;
    TOLBI_SHIP_TALK_FACING(templateBytes, 0) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 1) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 3) = TALK_FACING_PARTY_AND_BACK;
    TOLBI_SHIP_TALK_FACING(templateBytes, 4) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 5) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 6) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 7) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 8) = TALK_FACING_PARTY_AND_BACK;
    TOLBI_SHIP_TALK_FACING(templateBytes, 9) = TALK_FACING_KEEP;
    return (const struct ObjectEventTemplate *)templateBytes;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_CaptainsQuartersObjectEventTemplatesArrived;
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
        if (Script_GetFlag(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
          gShipRooms_CaptainsQuartersObjectEventTemplatesWarriors[1]
              .talkFacing = TALK_FACING_PARTY_AND_BACK;
        }
        if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
          if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
            gShipRooms_CaptainsQuartersObjectEventTemplatesWarriors[0]
                .talkFacing = TALK_FACING_PARTY;
          }
        }
        return gShipRooms_CaptainsQuartersObjectEventTemplatesWarriors;
      }
      return gShipRooms_ObjectEventTemplatesNone;
    } else {
      return gShipRooms_CaptainsQuartersObjectEventTemplates;
    }
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
    return gShipRooms_CaptainsQuartersObjectEventTemplates;
    break;
  default:
    return gShipRooms_ObjectEventTemplatesNone;
    break;
  }
  return (const struct ObjectEventTemplate *)templateBytes;
}

#undef TOLBI_SHIP_TALK_FACING

/// Chooses the room's interactions for the current stage of the voyage.
const struct MapEvent *ShipRooms_SelectEvents(void) {
  s32 warpDest = gSaveBlock.warpDest;

  switch (warpDest) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gShipRooms_HoldEventsAtIsland;
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      return gShipRooms_HoldEventsChoosingRowers;
    }
    return gShipRooms_HoldEvents;
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_RowingDeckEventsArrived;
    }
    return gShipRooms_RowingDeckEvents;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gShipRooms_CaptainsQuartersEventsAtIsland;
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_CaptainsQuartersEventsArrived;
    }
    return gShipRooms_CaptainsQuartersEvents;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    return gShipRooms_RowingDeckEventsMonstersAboard;
  case SHIP_ROOMS_FROM_WORLD_MAP:
    return gShipRooms_RowingDeckEventsKrakenAboard;
  default:
    break;
  }

  return gShipRooms_EventsOther;
}

/// The crewman asks whether the party wants to see the captain too. This is in
/// the captain's quarters at Kalay, once the warriors want to see the captain;
/// on yes, the passengers' pressure on the captain plays
/// (FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL).
static void ShipRooms_TalkToCaptainsQuartersCrewman(void) {
  FieldScript_Start();
  Field_EndBusyAndReveal();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipIfShipFromTolbiHad);
    Script_ShowMessage(LOCALID_QUARTERS_CREWMAN, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipNowWantSeeCaptainToo);
      Script_OpenMessage(LOCALID_QUARTERS_CREWMAN, 0);
      if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
        ShipRooms_DiscussLaunchingWithoutAnchor();
      } else {
        ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CREWMAN, 2);
        Script_ShowMessage(LOCALID_QUARTERS_CREWMAN, 0);
        ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN,
                                  FACING_NORTH + FACING_STEP, 0);
      }
    } else {
      FieldScript_SetMessageId(MsgTolbiShipButWeCantSendShip);
      Script_ShowMessage(LOCALID_QUARTERS_CREWMAN, 0);
    }
  }
  FieldScript_End();
}

/// Sean, in the captain's quarters, asks whether the party came to ask the
/// captain to set sail.
static void ShipRooms_TalkToSeanInCaptainsQuarters(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgTolbiShipYouCameAskCaptainSet);
  FieldScript_AskYesNoAndShowResponse(LOCALID_QUARTERS_SEAN, 0);
  FieldScript_End();
}

/// The captain, by the voyage's stage: too late to hire mercenaries, then his
/// bad luck at losing his lucky anchor, then uncertain what might happen, and
/// under sail the news of an injured oarsman. After finding the Anchor Charm,
/// talking to him also opens the main menu, so that the charm can be returned.
static void ShipRooms_TalkToCaptain(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipOarsmanWasInjured);
    ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
    ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                              FACING_NORTH + FACING_STEP, 60);
    ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 4);
    ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
    ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  } else if (Script_GetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipWeDontKnowMightHappen);
    Script_ShowMessage(LOCALID_QUARTERS_CAPTAIN, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipBadLuckLosingMyLucky);
    Script_ShowMessage(LOCALID_QUARTERS_CAPTAIN, 0);
    if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) == 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND) != 0) {
      gFieldWork->menu = 1;
    }
  } else {
    FieldScript_SetMessageId(MsgTolbiShipItsTooLateHireMercenaries);
    Script_ShowMessage(LOCALID_QUARTERS_CAPTAIN, 0);
  }
  FieldScript_End();
}

/// A passenger in the hold while the ship waits. Once the party has agreed to
/// protect the ship, she worries the proud warriors won't like being told to
/// row, and asks the party.
static void ShipRooms_TalkToWaitingWarrior(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_WARRIOR, 2);
    FieldScript_SetMessageId(MsgTolbiShipTheseProudWarriorsNotGoing);
    ShipRooms_Say(LOCALID_HOLD_WARRIOR);
    ObjectEvent_FaceObjectEvent(LOCALID_HOLD_WARRIOR, LOCALID_PARTY_LEADER, 10);
    Script_OpenMessage(LOCALID_HOLD_WARRIOR, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(40);
      ShipRooms_Say(LOCALID_HOLD_WARRIOR);
      ShipRooms_Turn(LOCALID_HOLD_WARRIOR, FACING_SOUTHEAST + FACING_STEP);
      Script_ShowMessage(LOCALID_HOLD_WARRIOR, 0);
    } else {
      gFieldWork->message += 2;
      Script_ShowMessage(LOCALID_HOLD_WARRIOR, 0);
      ObjectEvent_FaceDirection(LOCALID_HOLD_WARRIOR,
                                FACING_SOUTHEAST + FACING_STEP, 0);
    }
  } else {
    FieldScript_SetMessageId(MsgTolbiShipLongerWeSitHereMore);
    Script_ShowMessage(LOCALID_HOLD_WARRIOR, 0);
  }
  FieldScript_End();
}

/// Another waiting passenger, once the party has agreed to protect the ship,
/// asks whether it will row. "Are you going to row?"
static void ShipRooms_TalkToWaitingMuscleAdmirer(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipYouGoingRow);
    FieldScript_AskYesNoAndShowResponse(LOCALID_HOLD_MUSCLE_ADMIRER, 0);
  } else {
    FieldScript_SetMessageId(MsgTolbiShipTheresWholeBunchReallyMuscular);
    FieldScript_ShowMessageAndWait(LOCALID_HOLD_MUSCLE_ADMIRER, 0);
  }
  FieldScript_End();
}

/// Walks the rower back to the leader and hides its separate object event.
static inline void ShipRooms_RejoinParty(s32 rowerId) {
  struct ObjectEvent *leader;

  Import_ObjectEventSetAnimation(rowerId, 2);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(rowerId, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(rowerId);
  Import_ObjectEventSetPosition(rowerId, 0, 0);
}

/// A passenger in the hold under sail, when an oar needs a rower. If someone
/// has already been chosen this round, the chosen passenger steps beside the
/// leader and follows them out while this one says chosenMessage; otherwise
/// the party may ask this one to row (question) at the seat the attacks
/// fought off number. The Ask...ToRow functions are this with each
/// passenger's lines, except the stared-at passenger's and the second guide's,
/// which write it out in full.
#define TOLBI_SHIP_ASK_TO_ROW(speaker, chosenMessage, question, passenger)     \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {                       \
    s32 rowerId = ShipRooms_GetChosenOarsman();                                \
                                                                               \
    FieldScript_Start();                                                       \
    ShipRooms_StepBesideLeader(rowerId);                                       \
    FieldScript_SetMessageId(chosenMessage);                                   \
    ShipRooms_Say(speaker);                                                    \
    ShipRooms_RejoinParty(rowerId);                                            \
    FieldScript_End();                                                         \
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {             \
    ShipRooms_AskToRow(speaker, question,                                      \
                       FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + (passenger));      \
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {     \
    ShipRooms_AskToRow(speaker, question,                                      \
                       FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + (passenger));       \
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {    \
    ShipRooms_AskToRow(speaker, question,                                      \
                       FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger));      \
  } else {                                                                     \
    ShipRooms_AskToRow(speaker, question,                                      \
                       FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger));       \
  }

/// The head guide in the hold under sail.
static void ShipRooms_AskHeadGuideToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(LOCALID_SEA_HOLD_HEAD_GUIDE,
                        MsgTolbiShipLooksLikeYouveBeenChosen,
                        MsgTolbiShipNoooooNotGoing, PASSENGER_HEAD_GUIDE);
}

/// The sweating passenger in the hold under sail.
static void ShipRooms_AskSweatingPassengerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(LOCALID_SEA_HOLD_SWEATING_PASSENGER,
                        MsgTolbiShipHaHaHaChosenOarsman,
                        MsgTolbiShipJokingWantRow, PASSENGER_SWEATING);
}

/// The lady in the hold under sail, who is too seasick to talk once the ship
/// is at anchor.
static void ShipRooms_AskLadyToRow(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    FieldScript_Start();
    FieldEffect_ConfigureLinkedObject(
        LOCALID_SEA_HOLD_LADY, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    FieldScript_SetMessageId(MsgTolbiShipDontFeelDontTalkMe);
    FieldScript_ShowMessageAndWait(LOCALID_SEA_HOLD_LADY, 0);
    FieldScript_End();
  } else {
    TOLBI_SHIP_ASK_TO_ROW(LOCALID_SEA_HOLD_LADY,
                          MsgTolbiShipYouFinallyPickedSomeoneDidnt,
                          MsgTolbiShipDontTellGoing, PASSENGER_LADY);
  }
}

/// The stared-at passenger is asked to row, as the others are. It's written out
/// in full rather than through ShipRooms_AskToRow.
static void ShipRooms_AskStaredAtPassengerToRow(void) {
  s32 rowerId;
  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  if (Script_GetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_GetChosenOarsman();
    ShipRooms_StepBesideLeader(rowerId);
    FieldScript_SetMessageId(MsgTolbiShipGivesMeChillsThinkCould);
    ShipRooms_Say(LOCALID_SEA_HOLD_STARED_AT_PASSENGER);
    ShipRooms_RejoinParty(rowerId);
  } else {
    ObjectMotion_SetVarCbAndRefresh(LOCALID_SEA_HOLD_STARED_AT_PASSENGER, 2);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgTolbiShipOhhhhNooooGoingMakeMe);
    Script_OpenMessage(LOCALID_SEA_HOLD_STARED_AT_PASSENGER, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      ShipRooms_Say(LOCALID_SEA_HOLD_STARED_AT_PASSENGER);
      Import_ObjectEventSetAnimation(LOCALID_SEA_HOLD_STARED_AT_PASSENGER, 2);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        ObjectEvent_SetDestination(LOCALID_SEA_HOLD_STARED_AT_PASSENGER,
                                   objectEvent->x.part.pixel,
                                   objectEvent->z.part.pixel);
      }
      ObjectMotion_CommitCurrentPositionAndActivate(
          LOCALID_SEA_HOLD_STARED_AT_PASSENGER);
      ObjectEvent_SetPosition(LOCALID_SEA_HOLD_STARED_AT_PASSENGER, 0, 0);
      GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
      if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + PASSENGER_STARED_AT);
      } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + PASSENGER_STARED_AT);
      } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + PASSENGER_STARED_AT);
      } else {
        GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_STARED_AT);
      }
    } else {
      gFieldWork->message++;
      ShipRooms_Say(LOCALID_SEA_HOLD_STARED_AT_PASSENGER);
    }
  }
  FieldScript_End();
}

/// The second guide in the hold under sail, written out in full as well.
static void ShipRooms_AskSecondGuideToRow(void) {
  s32 rowerId;
  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  if (Script_GetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_GetChosenOarsman();
    ShipRooms_StepBesideLeader(rowerId);
    FieldScript_SetMessageId(MsgTolbiShipIsaacYouveGotGoodEye);
    ShipRooms_Say(LOCALID_SEA_HOLD_SECOND_GUIDE);
    ShipRooms_RejoinParty(rowerId);
  } else {
    FieldScript_SetMessageId(MsgTolbiShipHaHaHaRowingFeel);
    FieldScript_ShowMessageThenWait(LOCALID_SEA_HOLD_SECOND_GUIDE, 0, 60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_SEA_HOLD_SECOND_GUIDE, 1);
    Script_OpenMessage(LOCALID_SEA_HOLD_SECOND_GUIDE, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      ShipRooms_Say(LOCALID_SEA_HOLD_SECOND_GUIDE);
      Import_ObjectEventSetAnimation(LOCALID_SEA_HOLD_SECOND_GUIDE, 2);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        ObjectEvent_SetDestination(LOCALID_SEA_HOLD_SECOND_GUIDE,
                                   objectEvent->x.part.pixel,
                                   objectEvent->z.part.pixel);
      }
      ObjectMotion_CommitCurrentPositionAndActivate(
          LOCALID_SEA_HOLD_SECOND_GUIDE);
      ObjectEvent_SetPosition(LOCALID_SEA_HOLD_SECOND_GUIDE, 0, 0);
      GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
      if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      } else {
        GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      }
    } else {
      gFieldWork->message++;
      ShipRooms_Say(LOCALID_SEA_HOLD_SECOND_GUIDE);
    }
  }
  FieldScript_End();
}

/// The warrior in the hold under sail.
static void ShipRooms_AskWarriorToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(LOCALID_SEA_HOLD_WARRIOR,
                        MsgTolbiShipThatsWhoYouPickedOarsman,
                        MsgTolbiShipNotThinkingMaking, PASSENGER_WARRIOR);
}

/// The ticket grumbler in the hold under sail.
static void ShipRooms_AskTicketGrumblerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(
      LOCALID_SEA_HOLD_TICKET_GRUMBLER, MsgTolbiShipIfYouveChosenOarsmanLets,
      MsgTolbiShipOarsmanGiveBreak, PASSENGER_TICKET_GRUMBLER);
}

/// The muscle admirer in the hold under sail.
static void ShipRooms_AskMuscleAdmirerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(LOCALID_SEA_HOLD_MUSCLE_ADMIRER,
                        MsgTolbiShipYouveBeenChosenAsOarsman,
                        MsgTolbiShipPoorOarsmanFeel, PASSENGER_MUSCLE_ADMIRER);
}

/// The old man in the hold under sail.
static void ShipRooms_AskOldManToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(LOCALID_SEA_HOLD_OLD_MAN,
                        MsgTolbiShipHoHoPersonGoingGet,
                        MsgTolbiShipPeopleAskingFrail, PASSENGER_OLD_MAN);
}

#undef TOLBI_SHIP_ASK_TO_ROW

/// Finds a placed object event within twelve pixels of the requested floor
/// position.
static struct ObjectEvent *ShipRooms_FindObjectEventNear(s32 x, s32 z) {
  struct FieldWork *work;
  struct ObjectEvent **objectEvent;
  struct ObjectEvent *current;
  u32 i;
  s32 objectEventX;
  s32 objectEventZ;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  work = gFieldWork;
  i = LOCALID_FIRST_PLACED;
  left = x - 12;
  right = x + 12;
  top = z - 12;
  bottom = z + 12;
  objectEvent = &work->objectEvents[LOCALID_FIRST_PLACED];
  while (i <= 65) {
    current = *objectEvent++;
    objectEventX = current->x.part.pixel;
    objectEventZ = current->z.part.pixel;
    if (left < objectEventX && right > objectEventX && top < objectEventZ &&
        bottom > objectEventZ)
      return current;
    i++;
  }
  return 0;
}

/// The chosen oarsman steps out beside the leader, on the leader's right if
/// that spot is free, else on the left, else behind, and turns the way the
/// leader faces.
static void ShipRooms_StepBesideLeader(s32 passengerId) {
  struct ObjectEvent *leader;
  struct ObjectEvent *leaderPosition;
  s32 blocked;
  s32 direction;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  blocked = 1;
  ObjectMotion_SetActionVariant(passengerId, 2);
  {
    struct ObjectEvent *passenger = ObjectEvent_GetById(passengerId);

    passenger->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | passenger->priorityFlags;
  }
  // Try the leader's right, then left, then the space behind.
  direction = (((leader->facing + 0x4000) & 0xf000) >> 12);
  if (ShipRooms_IsSpotFree((((leader->facing + 0x4000) & 0xf000) >> 12)) != 0) {
    blocked = 0;
  }
  if (blocked != 0) {
    direction = (((leader->facing + -0x4000) & 0xf000) >> 12);
    if (ShipRooms_IsSpotFree((((leader->facing + -0x4000) & 0xf000) >> 12)) !=
        0) {
      blocked = 0;
    }
    if (blocked != 0) {
      direction = (((leader->facing + 0x8000) & 0xf000) >> 12);
    }
  }
  leaderPosition = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leaderPosition != 0) {
    Import_ObjectEventSetPosition(passengerId, leaderPosition->x.fixed,
                                  leaderPosition->z.fixed);
  }
  ObjectEvent_SetSpeed(passengerId, PIXELS(8) / 5, PIXELS(4) / 5);
  Import_ObjectEventSetAnimation(passengerId, 2);
  {
    s32 offset = gShipRooms_TurnSteps[direction];

    ObjectMotion_OffsetPositionAndResetMotion(passengerId, offset >> 16,
                                              (offset << 16) >> 16);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(passengerId);
  Import_ObjectEventSetAnimation(passengerId, 1);
  ShipRooms_Turn(passengerId, leader->facing);
}

enum {
  KAJA_ROWING_SPEAKER = SPEAKER(LOCALID_ROWING_KAJA,
                                SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT)
};

/// Returns 1 when the spot next to the leader in the given sixteenth of a turn
/// holds no object event and no wall, else 0.
static s32 ShipRooms_IsSpotFree(s32 bucket) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u32 offset = gShipRooms_TurnSteps[bucket];
  s32 x = objectEvent->x.part.pixel + ((s32)offset >> 16);
  s32 z = objectEvent->z.part.pixel + (s16)offset;

  if (ShipRooms_FindObjectEventNear(x, z) != 0) {
    return 0;
  }

  {
    struct FieldPosition point;
    point.x = x << 16;
    point.y = objectEvent->y.fixed;
    point.z = z << 16;

    if (FieldObject_CheckMovementCollision(objectEvent, &point) != 0) {
      return 0;
    }
  }

  return 1;
}

/// Returns the passenger chosen to row at the open seat, which the attacks
/// fought off number.
static s32 ShipRooms_GetChosenOarsman(void) {
  s32 ret = 0;

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    ret = 3;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    ret = 2;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    ret = 1;
  }

  return ShipRooms_FindFirstSetFlag(ret, 1);
}

/// What a passenger at the oars says, or thinks under Mind Read, by the round
/// they were chosen in: the first round's line, the second's, or a later
/// one's. The TalkTo...AtOars and Read...MindAtOars functions are this, or
/// TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER, with each passenger's lines.
#define TOLBI_SHIP_ROWER_LINE_BY_ROUND(passenger, localId, first, second,      \
                                       later)                                  \
  FieldScript_Start();                                                         \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))           \
    FieldScript_SetMessageId(first);                                           \
  else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger)))     \
    FieldScript_SetMessageId(second);                                          \
  else                                                                         \
    FieldScript_SetMessageId(later);                                           \
  FieldScript_ShowMessageAndWait(localId, 0);                                  \
  FieldScript_End()

/// The same with one line for the first round and one for any later.
#define TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(passenger, localId, first, later) \
  FieldScript_Start();                                                         \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))           \
    FieldScript_SetMessageId(first);                                           \
  else                                                                         \
    FieldScript_SetMessageId(later);                                           \
  FieldScript_ShowMessageAndWait(localId, 0);                                  \
  FieldScript_End()

/// The head guide at the oars: Kaja's men are worried, monsters are
/// everywhere, or the ship is starting to list.
static void ShipRooms_TalkToHeadGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_HEAD_GUIDE,
                                 LOCALID_ROWING_HEAD_GUIDE,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The second guide at the oars, as the head guide.
static void ShipRooms_TalkToSecondGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SECOND_GUIDE,
                                 LOCALID_ROWING_SECOND_GUIDE,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The sweating passenger at the oars, as the head guide.
static void ShipRooms_TalkToSweatingPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SWEATING,
                                 LOCALID_ROWING_SWEATING_PASSENGER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The lady at the oars asks how many monsters are out there.
static void ShipRooms_TalkToLadyAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, LOCALID_ROWING_LADY, MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// The stared-at passenger at the oars, as the head guide.
static void ShipRooms_TalkToStaredAtPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_STARED_AT,
                                 LOCALID_ROWING_STARED_AT_PASSENGER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The warrior at the oars, as the head guide.
static void ShipRooms_TalkToWarriorAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_WARRIOR, LOCALID_ROWING_WARRIOR,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The ticket grumbler at the oars, as the head guide.
static void ShipRooms_TalkToTicketGrumblerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_TICKET_GRUMBLER,
                                 LOCALID_ROWING_TICKET_GRUMBLER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The muscle admirer at the oars, as the lady.
static void ShipRooms_TalkToMuscleAdmirerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, LOCALID_ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// Mind Read on the head guide at the oars. "Just hurry up and slay that
/// beast!"
static void ShipRooms_ReadHeadGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_HEAD_GUIDE, LOCALID_ROWING_HEAD_GUIDE,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the second guide at the oars, as on the head guide.
static void ShipRooms_ReadSecondGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_SECOND_GUIDE, LOCALID_ROWING_SECOND_GUIDE,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the sweating passenger at the oars, as on the head guide.
static void ShipRooms_ReadSweatingPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_SWEATING, LOCALID_ROWING_SWEATING_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the lady at the oars: there are so many monsters at sea.
static void ShipRooms_ReadLadyMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, LOCALID_ROWING_LADY, MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

/// Mind Read on the stared-at passenger at the oars, as on the head guide.
static void ShipRooms_ReadStaredAtPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_STARED_AT, LOCALID_ROWING_STARED_AT_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the warrior at the oars, as on the head guide.
static void ShipRooms_ReadWarriorMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_WARRIOR, LOCALID_ROWING_WARRIOR,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the ticket grumbler at the oars, as on the head guide.
static void ShipRooms_ReadTicketGrumblerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_TICKET_GRUMBLER, LOCALID_ROWING_TICKET_GRUMBLER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the muscle admirer at the oars, as on the lady.
static void ShipRooms_ReadMuscleAdmirerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, LOCALID_ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

#undef TOLBI_SHIP_ROWER_LINE_BY_ROUND
#undef TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER

/// Asks a passenger to row. On yes they walk to the leader and leave with them,
/// and the seat's flag for this passenger (seatFlag) and
/// FLAG_TOLBI_SHIP_ROWER_CHOSEN are set; on no the next message refuses.
static void ShipRooms_AskToRow(s32 speaker, s32 question, s32 seatFlag) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtObject(speaker, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ShipRooms_Say(speaker);
    Import_ObjectEventSetAnimation(speaker, 2);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectMotion_ResetAndSetPosition(speaker, leader->x.part.pixel,
                                       leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(speaker);
    Import_ObjectEventSetPosition(speaker, 0, 0);
    GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    GameFlag_Set(seatFlag);
  } else {
    gFieldWork->message++;
    ShipRooms_Say(speaker);
  }
  FieldScript_End();
}

/// The travelling healer in the hold. From in front of him (the leader facing
/// roughly east) he opens his Sanctum; otherwise he talks by the voyage's
/// stage. Under sail, until the ship is docked, he is LOCALID_SEA_HOLD_HEALER,
/// else LOCALID_HOLD_HEALER.
static void ShipRooms_TalkToHealer(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if ((u16)(leader->facing - 0x2000) > 0xc000) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Sanctum_Run(LOCALID_SEA_HOLD_HEALER);
    } else {
      Sanctum_Run(LOCALID_HOLD_HEALER);
    }
  } else {
    FieldScript_Start();
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipGoodShipHasArrivedSafely);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipHateArguing);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipShipsCrewReadyAnything);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipShipsCrewReadyForAnything);
    } else {
      FieldScript_SetMessageId(MsgTolbiShipImSpreadingGoodwillWhereverTravel);
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      FieldScript_ShowMessageAndWait(LOCALID_SEA_HOLD_HEALER, 0);
    } else {
      FieldScript_ShowMessageAndWait(LOCALID_HOLD_HEALER, 0);
    }
    FieldScript_End();
  }
}

/// Talking to one of the monsters on the rowing deck
/// (LOCALID_ROWING_FIRST_MONSTER and the two after it) starts the battle with
/// them: a slot of the encounter zone of the Tolbi ship's scripted battles, and
/// the warp destination the party comes back to, chosen by how far the voyage
/// has gone.
static void ShipRooms_FightRowingDeckMonsters(void) {

  FieldScript_Start();
  Field_EndBusyAndReveal();
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) == 0) {
    FieldScript_SetNextDestination(
        (s32)&MapId_ShipRooms,
        SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) == 0) {
      FieldScript_SetNextDestination(
          (s32)&MapId_ShipRooms,
          SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE);
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 1);
    } else {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) == 0) {
        FieldScript_SetNextDestination(
            (s32)&MapId_ShipRooms,
            SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE);
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 2);
      }
    }
  }
  FieldScript_End();
}

/// While the monsters are aboard, the leader is walked back from the stairs.
/// "Where are you going, Isaac? Slay the beasts and save my mates!"
static void ShipRooms_TurnBackFromStairs(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS) != 0) {
    FieldScript_Start();
    Message_SetVoiceFromObjectEvent(LOCALID_ROWING_CREWMAN);
    FieldMessage_ShowCentered(MsgTolbiShipWhereGoingSlay, 1, 8);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 408, 134);
    ShipRooms_Turn(LOCALID_PARTY_LEADER, FACING_SOUTH);
    FieldScript_End();
  }
}

/// Below decks at Kalay, once the warriors have gone to the captain
/// (MB_SHIP_ROOMS_QUARTERS_CREW): the captain, Kaja and the crewman
/// wonder what could have happened to the replacement ship, as on deck
/// (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY).
static void ShipRooms_WatchCrewWonderAtDelay(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0)
    return;

  FieldScript_Start();
  Field_EndBusyAndReveal();
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  ShipRooms_MoveCameraAndWait(PIXELS(448), -1, PIXELS(638),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 40);
  FieldScript_SetMessageId(MsgTolbiShipWonderCouldHaveHappened);

  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Turn(LOCALID_QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);

  ObjectMotion_ArmCallback(LOCALID_QUARTERS_CAPTAIN, FACING_EAST, 20);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_WEST);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Turn(LOCALID_QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);

  GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
  FieldScript_End();
}

/// In the hold at Kalay, once the tour group has left Kalay
/// (MB_SHIP_ROOMS_HOLD_WARRIORS): Ouranos threatens to go ashore if
/// the ship doesn't sail. The second guide can't say when it will, so Sean and
/// Ouranos set off to make the captain set sail, and the guide ("I don't want
/// any violence!") hurries after them past the party.
static void ShipRooms_WatchWarriorsStormOff(void) {
  s32 guideSpeaker;
  s32 seanSpeaker;
  const s32 *action;

  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
    // Not yet: the party has not heard of the delay.
  } else {
    FieldScript_Start();
    Field_EndBusyAndReveal();
    FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
    ShipRooms_MoveCameraAndWait(PIXELS(1463), -1, PIXELS(464),
                                SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_OURANOS, 1);
    FieldScript_SetMessageId(MsgTolbiShipIfWeArentGoingSet);
    ShipRooms_Say(SPEAKER(LOCALID_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
    ShipRooms_Turn(LOCALID_HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP);
    guideSpeaker = SPEAKER(LOCALID_HOLD_SECOND_GUIDE, SPEAKER_WINDOW_BELOW);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 20);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SECOND_GUIDE, 2);
    ShipRooms_Say(guideSpeaker);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_SEAN, 1);
    Script_ShowMessageAndWait(
        SPEAKER(LOCALID_HOLD_SEAN,
                SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT),
        0, 20);
    ShipRooms_Turn(LOCALID_HOLD_SECOND_GUIDE, 0);
    seanSpeaker = SPEAKER(LOCALID_HOLD_SEAN,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                          40);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SEAN, 3);
    ShipRooms_Say(seanSpeaker);
    ObjectEvent_SetAttachedEffect(LOCALID_HOLD_SECOND_GUIDE,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SECOND_GUIDE, 3);
    ShipRooms_Say(guideSpeaker);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_SEAN, 1);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(LOCALID_HOLD_SEAN, FACING_NORTHWEST + FACING_STEP);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(LOCALID_HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SECOND_GUIDE, 1);
    ShipRooms_Say(guideSpeaker);
    ObjectMotion_SetAnimationAndWait(LOCALID_HOLD_OURANOS, 4);
    ShipRooms_Say(SPEAKER(LOCALID_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_OURANOS, 2);
    ShipRooms_Say(SPEAKER(LOCALID_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
    Import_ObjectEventSetAnimation(LOCALID_HOLD_SECOND_GUIDE, 4);
    ShipRooms_Say(guideSpeaker);
    ObjectMotion_SetAnimationAndWait(LOCALID_HOLD_SEAN, 4);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(LOCALID_HOLD_SEAN, FACING_WEST);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SEAN, 2);
    Script_ShowMessageAndWait(seanSpeaker, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_HOLD_SECOND_GUIDE, FACING_EAST, 0);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Script_ShowMessageAndWait(guideSpeaker, 0, 20);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                          0);
    ObjectEvent_ShowEmote(LOCALID_HOLD_OURANOS,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SEAN, 2);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(LOCALID_HOLD_SEAN, FACING_NORTHWEST + FACING_STEP);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_SEAN, 1);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(LOCALID_HOLD_OURANOS, FACING_SOUTHEAST + FACING_STEP);
    ObjectEvent_ShowEmote(LOCALID_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_OURANOS, 1);
    ShipRooms_Say(LOCALID_HOLD_OURANOS);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                          40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_SEAN, 1);
    ShipRooms_Say(seanSpeaker);
    ObjectEvent_FaceDirection(LOCALID_HOLD_SECOND_GUIDE,
                              FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_HOLD_OURANOS, FACING_SOUTH + FACING_STEP,
                              40);
    ObjectEvent_FaceDirection(LOCALID_HOLD_SECOND_GUIDE, FACING_EAST, 0);
    ShipRooms_Turn(LOCALID_HOLD_OURANOS, FACING_SOUTHEAST + FACING_STEP);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_SECOND_GUIDE, 2);
    Script_ShowMessageAndWait(guideSpeaker, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_HOLD_SEAN, FACING_SOUTH, 40);
    ShipRooms_Say(seanSpeaker);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SECOND_GUIDE, 2);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_OURANOS, 2);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_OURANOS, 1);
    ShipRooms_Say(LOCALID_HOLD_OURANOS);
    ObjectMotion_SetAnimationAndWait(LOCALID_HOLD_SEAN, 3);
    ShipRooms_Say(seanSpeaker);
    ObjectEvent_ShowEmote(LOCALID_HOLD_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HOLD_SECOND_GUIDE, 2);
    ShipRooms_Say(guideSpeaker);
    ObjectMotion_SetAnimationAndWait(LOCALID_HOLD_OURANOS, 3);
    ShipRooms_Say(LOCALID_HOLD_OURANOS);
    ObjectEvent_FaceDirection(LOCALID_HOLD_SEAN, FACING_NORTHWEST + FACING_STEP,
                              40);
    Import_ObjectEventSetAnimation(LOCALID_HOLD_SEAN, 3);
    ObjectMotion_SetAnimationAndWait(LOCALID_HOLD_OURANOS, 3);
    ObjectEvent_SetSpeed(LOCALID_HOLD_SEAN, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_SetSpeed(LOCALID_HOLD_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
    action = gShipRooms_LeaveHold;
    ObjectEvent_SetActionScript(LOCALID_HOLD_SEAN, action);
    ObjectEvent_SetActionScript(LOCALID_HOLD_OURANOS, action);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_HOLD_SECOND_GUIDE, FACING_SOUTH, 0);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(12) / 5, PIXELS(6) / 5);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 184, 520);
    FieldScript_WaitFrames(1);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      objectEvent->steeringFlags =
          OBJ_EVENT_FACES_MOTION | objectEvent->steeringFlags;
    }
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectMotion_Launch(LOCALID_HOLD_SECOND_GUIDE, 4, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 20);
    ObjectMotion_SetVariantCallback(LOCALID_HOLD_SECOND_GUIDE, 2);
    ShipRooms_Say(LOCALID_HOLD_SECOND_GUIDE);
    ObjectEvent_SetSpeed(LOCALID_HOLD_SECOND_GUIDE, PIXELS(8) / 5,
                         PIXELS(4) / 5);
    ObjectEvent_SetActionScript(LOCALID_HOLD_SECOND_GUIDE, action);
    FieldScript_WaitFrames(40);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectEvent_RefreshSelectorById(LOCALID_HOLD_SECOND_GUIDE);
    GameFlag_Set(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
    FieldScript_End();
  }
}

/// In the captain's quarters at Kalay: the passengers press the captain to
/// launch; Ouranos and Sean offer to guard the deck, and the captain finds his
/// lucky anchor charm gone and refuses to sail without it.
static void ShipRooms_DiscussLaunchingWithoutAnchor(void) {
  s32 seanSpeech;
  s32 ouranosSpeech;
  s32 captainSpeech;

  // Gather the captain's quarters's attention around the argument about
  // launching.
  Script_PlaySong(MUS_DUNGEON);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  ShipRooms_MoveCameraAndWait(PIXELS(456), -1, PIXELS(648),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 1);
  FieldScript_SetMessageId(MsgTolbiShipYoureTryingLaunchShip);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE, FACING_WEST, 40);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_KAJA, 2);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE,
                            FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 1);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_HEART,
                        20);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(LOCALID_QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_OURANOS, 3);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SECOND_GUIDE, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_SECOND_GUIDE, 3);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN, FACING_SOUTH + FACING_STEP,
                            0);
  ShipRooms_Turn(LOCALID_QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP);
  // Two passengers step aside as the conversation continues.
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_SECOND_GUIDE, PIXELS(2) / 5,
                       PIXELS(1) / 5);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_SEAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(LOCALID_QUARTERS_SEAN, 444, 668);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_SECOND_GUIDE, 472, 668);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_QUARTERS_SEAN);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_SEAN, 1);
  FieldScript_WaitFrames(80);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_QUARTERS_OURANOS, 0,
                            40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 2);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTH + FACING_STEP, 0);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_OURANOS, 4);
  FieldScript_WaitFrames(20);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_KAJA, 3);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_SetAttachedEffect(LOCALID_QUARTERS_SECOND_GUIDE,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectMotion_Launch(LOCALID_QUARTERS_SECOND_GUIDE, 2, 20);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_KAJA, 3);
  seanSpeech = SPEAKER(LOCALID_QUARTERS_SEAN, SPEAKER_PORTRAIT_LEFT);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ShipRooms_Turn(LOCALID_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SEAN, 1);
  ShipRooms_Say(seanSpeech);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ObjectMotion_Launch(LOCALID_QUARTERS_SECOND_GUIDE, 4, 0);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_KAJA, 3);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 1);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_QUARTERS_CAPTAIN, 0);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_OURANOS, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_QUARTERS_OURANOS, 0,
                            40);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP,
                            20);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_KAJA, 2);
  Script_ShowMessageAndWait(LOCALID_QUARTERS_KAJA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_OURANOS, 3);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SEAN, 2);
  ouranosSpeech = SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_WINDOW_ABOVE);
  ShipRooms_Say(seanSpeech);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_SOUTH + FACING_STEP, 20);
  ShipRooms_Say(ouranosSpeech);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SEAN, 2);
  ShipRooms_Say(seanSpeech);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_OURANOS, 3);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 1);
  ShipRooms_Say(ouranosSpeech);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 1);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_OURANOS,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTH + FACING_STEP, 20);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_KAJA, 3);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_OURANOS,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_OURANOS, 2);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA, FACING_NORTH + FACING_STEP,
                            40);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SEAN, 1);
  FieldScript_WaitFrames(20);
  ShipRooms_Say(seanSpeech);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN,
                            FACING_NORTHWEST + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 1);
  ShipRooms_Say(ouranosSpeech);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_SEAN, 3);
  Script_ShowMessageAndWait(seanSpeech, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 2);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_SEAN, 3);
  ShipRooms_Say(seanSpeech);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN, FACING_WEST, 20);
  Script_PlaySong(MUS_SILENCE);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
  captainSpeech = SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_WINDOW_BELOW);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  ShipRooms_Say(captainSpeech);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_OURANOS,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CREWMAN,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_QUARTERS_CAPTAIN,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectMotion_Launch(LOCALID_QUARTERS_CAPTAIN, 4, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
  Script_ShowMessage(captainSpeech, 0);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_CAPTAIN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CAPTAIN, 475, 598);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN, FACING_WEST, 0);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_KAJA, 462, 618);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_NORTHWEST + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_KAJA, 2);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_KAJA, SPEAKER_WINDOW_BELOW));
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_OURANOS,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ShipRooms_Say(LOCALID_QUARTERS_OURANOS);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        20);
  ShipRooms_Say(seanSpeech);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectMotion_Launch(LOCALID_QUARTERS_CAPTAIN, 4, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_SOUTH + FACING_STEP, 20);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  Script_PlaySong(MUS_DUNGEON);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 4);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  // Settle the passengers where the next visit to the captain's quarters needs
  // them.
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_SEAN, 444, 628);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_SOUTH + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE,
                        60);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_KAJA, 4);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS, FACING_EAST, 0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  ShipRooms_MoveCameraAndWait(PIXELS(464), -1, PIXELS(680),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CREWMAN, 1);
  ShipRooms_Turn(LOCALID_QUARTERS_CREWMAN, 0);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN, FACING_WEST, 20);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CREWMAN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_QUARTERS_CREWMAN, 4, 40);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CREWMAN, 1);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_SECOND_GUIDE, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_QUARTERS_SECOND_GUIDE, 438, 659);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTHWEST + FACING_STEP, 0);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_CREWMAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CREWMAN, 488, 686);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_QUARTERS_SECOND_GUIDE);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_SECOND_GUIDE, 1);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE,
                            FACING_NORTH + FACING_STEP, 0);
  Map_PlayMusic();
  GameFlag_Set(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL);
}

/// In the captain's quarters at Kalay, using the Anchor Charm on the captain:
/// he has his lucky anchor back, Sean names the party to guard the oarsmen, and
/// the party agrees to protect the ship
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED).
static void ShipRooms_ReturnLuckyAnchor(void) {
  struct ObjectEvent *objectEvent;
  s32 captainSpeech;
  s32 seanSpeech;
  s32 value;
  s32 captainAboveSpeech;
  const s32 *action;

  // Let the captain explain why the anchor matters and ask the party to help.
  FieldScript_Start();
  Field_EndBusyAndReveal();
  ObjectEvent_FaceObjectEvent(LOCALID_QUARTERS_CAPTAIN, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  captainSpeech = SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 3);
  FieldScript_SetMessageId(MsgTolbiShipItsMyLuckyAnchor);
  ShipRooms_Say(captainSpeech);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_KAJA, 1);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SEAN, 1);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_OURANOS, 1);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SECOND_GUIDE, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CREWMAN, 1);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN,
                            FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  Script_OpenMessage(captainSpeech, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 2);
    ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_KAJA,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
    ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN,
                          EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    ShipRooms_Say(captainSpeech);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 1);
    ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_KAJA,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
    ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
    ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_CAPTAIN,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  }
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(636), 1);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_SECOND_GUIDE, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_SECOND_GUIDE, 472, 662);
  ShipRooms_Turn(LOCALID_QUARTERS_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_KAJA, 3);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE, FACING_WEST, 20);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_OURANOS, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_SECOND_GUIDE, 3);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SEAN, 1);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_QUARTERS_SEAN, 0,
                            20);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_OURANOS,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ShipRooms_Say(LOCALID_QUARTERS_OURANOS);
  seanSpeech = SPEAKER(LOCALID_QUARTERS_SEAN,
                       SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_SEAN, 4);
  ShipRooms_Say(seanSpeech);
  ShipRooms_Turn(LOCALID_QUARTERS_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_KAJA,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 1);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_SEAN, 3);
  ShipRooms_Say(seanSpeech);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 2);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_SEAN, 3);
  ShipRooms_Say(seanSpeech);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 2);
  ShipRooms_Turn(LOCALID_QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_OURANOS);
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, 0);
  ShipRooms_Say(seanSpeech);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE,
                            FACING_NORTH + FACING_STEP, 0);
  // Bring the rest of the party out beside Isaac for the agreement.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 486, 608);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 486, 624);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_IVAN, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 486, 640);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_MIA, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_MIA, 486, 656);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 20);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  ShipRooms_Say(seanSpeech);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 1);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  Script_OpenMessage(LOCALID_QUARTERS_CAPTAIN, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
    ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
    ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_SEAN, 3);
    ShipRooms_Say(seanSpeech);
    ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_KAJA, 1);
    Script_ShowMessageAndWait(
        SPEAKER(LOCALID_QUARTERS_KAJA,
                SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT),
        0, 40);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 3;
    ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 3);
    Script_ShowMessageAndWait(LOCALID_QUARTERS_CAPTAIN, 0, 40);
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  value = 176;
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Turn(LOCALID_QUARTERS_SECOND_GUIDE, (value << 8));
  Script_ShowMessageAndWait(LOCALID_QUARTERS_SECOND_GUIDE, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  captainAboveSpeech = SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CAPTAIN, 472, 632);
  ShipRooms_Say(captainAboveSpeech);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(LOCALID_QUARTERS_SECOND_GUIDE);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_CAPTAIN, 4);
  Script_ShowMessageAndWait(captainAboveSpeech, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_OURANOS, 1);
  ShipRooms_Turn(LOCALID_QUARTERS_OURANOS, (value << 8));
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        20);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_CREWMAN, PIXELS(12) / 5, PIXELS(6) / 5);
  ObjectMotion_Launch(LOCALID_QUARTERS_CREWMAN, 2, 0);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CREWMAN, 462, 674);
  ShipRooms_Turn(LOCALID_QUARTERS_CREWMAN, (value << 8));
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CREWMAN, 2);
  ShipRooms_Say(LOCALID_QUARTERS_CREWMAN);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_KAJA, 4);
  ShipRooms_Say(LOCALID_QUARTERS_KAJA);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(captainAboveSpeech);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessageAndWait(LOCALID_QUARTERS_SECOND_GUIDE, 0, 40);
  ShipRooms_Turn(LOCALID_QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_KAJA, 2);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(LOCALID_QUARTERS_SEAN, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_OURANOS, (value << 8), 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_SECOND_GUIDE, (value << 8), 0);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CREWMAN, (value << 8), 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SEAN, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_QUARTERS_SEAN, 0,
                            20);
  ObjectEvent_ShowEmote(LOCALID_QUARTERS_CAPTAIN,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP);
  Script_OpenMessage(SPEAKER_PORTRAIT_LEFT | LOCALID_QUARTERS_CAPTAIN, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
    ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
  }
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_WEST);
  ShipRooms_Say(SPEAKER(LOCALID_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE));
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_SEAN, 3);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_OURANOS, 3);
  Import_ObjectEventSetAnimation(LOCALID_QUARTERS_KAJA, 3);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CREWMAN, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_SECOND_GUIDE, 2);
  FieldScript_WaitFrames(20);
  // Send the crew away and merge the party's separate object events again.
  action = gShipRooms_LeaveCaptainsQuarters;
  ObjectEvent_SetActionScript(LOCALID_QUARTERS_CREWMAN, action);
  FieldScript_WaitFrames(4);
  ObjectEvent_SetActionScript(LOCALID_QUARTERS_OURANOS, action);
  FieldScript_WaitFrames(4);
  ObjectEvent_SetActionScript(LOCALID_QUARTERS_SEAN, action);
  FieldScript_WaitFrames(4);
  ObjectEvent_SetActionScript(LOCALID_QUARTERS_KAJA, action);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_MIA, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_IVAN, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_SetActionScript(LOCALID_QUARTERS_SECOND_GUIDE, action);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CAPTAIN, 456, 648);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, 0);
  PartyInventory_Discard(ITEM_ANCHOR_CHARM);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED);
  FieldScript_End();
}

/// Restores the room or plays the script selected by this warp destination.
s32 ShipRooms_ApplyEntryState(void) {
  WaitFrames(1);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gSaveBlock.warpDest) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      ObjectEvent_ConfigureSpriteColor(LOCALID_SEA_HOLD_SECOND_GUIDE,
                                       SPRITE_COLOR_RBG);
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      ObjectEvent_ConfigureSpriteColor(LOCALID_HOLD_SECOND_GUIDE,
                                       SPRITE_COLOR_RBG);
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
  case SHIP_ROOMS_FROM_WORLD_MAP:
  case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
    ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_SECOND_GUIDE,
                                     SPRITE_COLOR_RBG);
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      ObjectEvent_ConfigureSpriteColor(LOCALID_QUARTERS_SECOND_GUIDE,
                                       SPRITE_COLOR_RBG);
    }
    break;
  }
  switch (gSaveBlock.warpDest) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
    ShipRooms_SetUpHold();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
    ShipRooms_SetUpRowingDeck();
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    ShipRooms_SetUpCaptainsQuarters();
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      ShipRooms_AnnounceDeparture();
    } else {
      ShipRooms_CastOff();
    }
    break;
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      ShipRooms_KajaCallsReplacementToOars();
    } else {
      ShipRooms_KajaCallsToOars();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
    ShipRooms_ShowRowing();
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
    ShipRooms_AskAboutMonsters();
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
    ShipRooms_AnnounceTurn();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      FieldScript_Start();
      ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 1, 0);
      ShipRooms_RunEventCommand(SHIP_RESTORE_SHIVERING_OARSMAN, 0, 0);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER,
                                  gShipRooms_MonsterBob);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 1,
                                  gShipRooms_MonsterBob);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 2,
                                  gShipRooms_MonsterBob);
      ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER,
                                       SPRITE_COLOR_BGR);
      ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER + 1,
                                       SPRITE_COLOR_BGR);
      ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER + 2,
                                       SPRITE_COLOR_BGR);
      FieldScript_End();
    } else {
      ShipRooms_StartFirstAttack();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
    ShipRooms_EndFirstAttack();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      FieldScript_Start();
      ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 2, 0);
      ShipRooms_RunEventCommand(SHIP_RESTORE_SHIVERING_OARSMAN, 0, 0);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER,
                                  gShipRooms_MonsterBob);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 1,
                                  gShipRooms_MonsterBob);
      FieldScript_End();
    } else {
      ShipRooms_StartSecondAttack();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
    ShipRooms_EndSecondAttack();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      FieldScript_Start();
      ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 3, 0);
      ShipRooms_RunEventCommand(SHIP_RESTORE_SHIVERING_OARSMAN, 0, 0);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER,
                                  gShipRooms_MonsterBob);
      ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 1,
                                  gShipRooms_MonsterBob);
      ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER,
                                       SPRITE_COLOR_BGR);
      ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER + 1,
                                       SPRITE_COLOR_BGR);
      FieldScript_End();
    } else {
      ShipRooms_StartThirdAttack();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
    ShipRooms_EndThirdAttack();
    break;
  case SHIP_ROOMS_FROM_WORLD_MAP:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK) != 0) {
        gShipRooms_ShakeTimer = 0;
        FieldScheduler_AddCallback(ShipRooms_ShakeWhileKrakenAboard,
                                   TASK_PRIORITY_MAP);
        Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 5);
      }
    } else {
      ShipRooms_NoticeShipRocking();
    }
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
    ShipRooms_ShowCaptainsQuartersBeforeIsland();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      ShipRooms_ReachIsland();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
    ShipRooms_DismissReplacementRowers();
    break;
  case SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT:
    ShipRooms_RunEventCommand(SHIP_CLEAR_FLAG_RANGE,
                              FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT,
                              FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
    ShipRooms_RunEventCommand(SHIP_CLEAR_ROWING_SELECTIONS, 0, 0);
    GameFlag_Set(FLAG_TOLBI_SHIP_DEFEATED_AT_SEA);
    Import_ScriptRequestWarp(SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK);
    break;
  }
  return 0;
}

/// Restores the captain's quarters's object events after the anchor and
/// departure conversations.
static void ShipRooms_SetUpCaptainsQuarters(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *other;
  s32 done;
  s32 x;
  s32 heading;
  u16 zero;

  if (!GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    ShipRooms_PlaceAnchorCharm();
    return;
  }
  if (Script_GetFlag(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    ShipRooms_PlaceAnchorCharm();
  }
  done = GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  if (done != 0) {
    return;
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND)) {
    objectEvent = ObjectEvent_GetById(LOCALID_QUARTERS_KAJA);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_CAPTAIN, 456, 652, FACING_EAST);
    x = 0x1e0;
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_KAJA, x, 600,
                               FACING_NORTHWEST + FACING_STEP);
    ObjectEvent_SetSpeed(LOCALID_QUARTERS_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
    objectEvent->work = done;
    zero = 0;
    objectEvent->riseEnabled = zero;
    objectEvent->collisionFlags |= 128;
    objectEvent->update = ShipRooms_PaceCaptainsQuarters;
    other = ObjectEvent_GetById(LOCALID_QUARTERS_CAPTAIN);
    other->riseCounter = zero;
    other->update = ShipRooms_WatchPacer;
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ShipRooms_PlaceObjectEvent(LOCALID_PARTY_LEADER, x, 666,
                                 FACING_NORTHWEST);
    }
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_CAPTAIN, 444, 614,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_CAPTAIN, 456, 648, FACING_EAST);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL)) {
    heading = FACING_NORTHWEST + FACING_STEP;
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_CAPTAIN, 475, 598, FACING_WEST);
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_KAJA, 462, 618, heading);
    ObjectEvent_GetById(LOCALID_QUARTERS_SEAN)->facing =
        FACING_SOUTHEAST + FACING_STEP;
    ObjectEvent_GetById(LOCALID_QUARTERS_OURANOS)->facing = heading;
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_SECOND_GUIDE, 438, 659,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_CREWMAN, 488, 688, heading);
  }
}

/// Restores the passenger hold after recruitment and departure scripts.
static void ShipRooms_SetUpHold(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    Import_ObjectEventSetPosition(LOCALID_HOLD_WARRIOR, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_HOLD_TICKET_GRUMBLER, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_HOLD_MUSCLE_ADMIRER, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_HOLD_OLD_MAN, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_HOLD_SECOND_GUIDE, 0, 0);
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    return;
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_HEAD_GUIDE, 152, 444,
                               FACING_SOUTHEAST + FACING_STEP);
    ObjectEvent_SetActionScript(LOCALID_SEA_HOLD_HEAD_GUIDE,
                                gShipRooms_LookAroundScript);
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_SWEATING_PASSENGER, 184, 480,
                               FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_STARED_AT_PASSENGER, 170, 488,
                               FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_WARRIOR, 136, 488,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_MUSCLE_ADMIRER, 120, 480,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_TICKET_GRUMBLER, 184, 526,
                               FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_SEA_HOLD_LADY, 136, 584, FACING_WEST);
    ObjectEvent_SetActionScript(LOCALID_SEA_HOLD_LADY, gShipRooms_Shiver);
    return;
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    ShipRooms_HideSelectedPassengers();
    return;
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    ShipRooms_RunEventCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
    return;
  }

  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0 &&
      GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_HOLD_SECOND_GUIDE, 0, 0);
  }
}

/// Restores the rowing deck and introduces a newly recruited replacement.
static void ShipRooms_SetUpRowingDeck(void) {
  WaitFrames(1);
  ShipRooms_SetOarCollision();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_CREWMAN, 412, 222,
                               FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_DRUMMER, 458, 161, FACING_WEST);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      ObjectEvent_SetPosition(LOCALID_ROWING_CREWMAN, PIXELS(472), PIXELS(152));
      Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 5);
      ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
    } else {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        ShipRooms_RunEventCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
        ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
        ShipRooms_IntroduceReplacementOarsman(3);
      } else {
        if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
          ShipRooms_RunEventCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
          ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 3, 0);
          ShipRooms_IntroduceReplacementOarsman(2);
        } else {
          if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
            ShipRooms_RunEventCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
            ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 2, 0);
            ShipRooms_IntroduceReplacementOarsman(1);
          } else {
            if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
              ShipRooms_RunEventCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
              ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OARSMAN, 0, 0);
              ShipRooms_IntroduceReplacementOarsman(0);
            } else {
              Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 5);
              if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
                  GameFlag_Get(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT) ==
                      0) {
                ShipRooms_AskForReplacementOarsmen();
              }
            }
          }
        }
      }
    }
  }
}

/// Sets collision bit 3 on the eight oars beside the rowing seats.
static void ShipRooms_SetOarCollision(void) {
  u32 localId;

  for (localId = LOCALID_ROWING_FIRST_OAR;
       localId <= LOCALID_ROWING_FIRST_OAR + 7; localId++) {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
    u32 flags = objectEvent->collisionFlags;
    objectEvent->collisionFlags = flags | 0x08;
  }
}

/// Puts each recruited rower at its seat and hides the original occupant.
static void ShipRooms_PlaceFoundObjectEvents(s32 gated) {
  s32 localId;

  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((localId = ShipRooms_FindFirstSetFlag(0, 0)) != 0) {
    ShipRooms_PlaceObjectEvent(localId, 410, 172, FACING_NORTH + FACING_STEP);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN, 0, 0);
  }
  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((localId = ShipRooms_FindFirstSetFlag(1, 0)) != 0) {
    ShipRooms_PlaceObjectEvent(localId, 470, 172,
                               FACING_NORTHWEST + FACING_STEP);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 1, 0, 0);
  }
  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED)) {
    return;
  }
  if ((localId = ShipRooms_FindFirstSetFlag(2, 0)) != 0) {
    ShipRooms_PlaceObjectEvent(localId, 410, 204, FACING_NORTH + FACING_STEP);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 2, 0, 0);
  }
  if (gated) {
    return;
  }
  if ((localId = ShipRooms_FindFirstSetFlag(3, 0)) != 0) {
    ShipRooms_PlaceObjectEvent(localId, 470, 204,
                               FACING_NORTHWEST + FACING_STEP);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 3, 0, 0);
  }
}

/// On the rowing deck, once the charm is back: Kaja tells the oarsmen the ship
/// sails without its replacement and asks the party to bring a passenger to row
/// whenever an oarsman falls, keeping the teams balanced; leads to the deck's
/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY
/// (FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT).
static void ShipRooms_AskForReplacementOarsmen(void) {
  struct ObjectEvent *objectEvent;
  s32 mode;

  // Bring Isaac to the crewman who explains the shortage of rowers.
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(168),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_KAJA, 440, 164,
                             FACING_SOUTH + FACING_STEP);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_CREWMAN, 428, 190,
                             FACING_NORTH + FACING_STEP);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_DRUMMER, 452, 190,
                             FACING_NORTHWEST + FACING_STEP);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 1);
  mode = 128;
  ShipRooms_PlaceObjectEvent(LOCALID_PARTY_LEADER, 440, 134, FACING_WEST);
  gFieldWork->transition = (mode << 1);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 408, 134);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 408, 148);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 424, 148);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  FieldScript_SetMessageId(MsgTolbiShipOurReplacementNeverArrivedBut);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_CREWMAN, 1);
  ShipRooms_Say(LOCALID_ROWING_CREWMAN);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_NORTH + FACING_STEP);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 440, 148);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_IVAN, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 456, 148);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_MIA, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_MIA, 472, 148);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 20);
  // Let the full party respond to the request for replacement oarsmen.
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 60);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_SOUTH + FACING_STEP,
                            20);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_DRUMMER, 1);
  ObjectEvent_ShowEmote(LOCALID_ROWING_DRUMMER, (mode << 1), 40);
  ShipRooms_Say(LOCALID_ROWING_DRUMMER);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 3);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_FIRST_OARSMAN, 1);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN, 3);
  ShipRooms_Say(LOCALID_ROWING_FIRST_OARSMAN);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_CREWMAN, 3);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 3);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 1, 3);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 2, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_FIRST_OARSMAN + 3, 3);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 4);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectEvent_ShowEmote(LOCALID_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        60);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_CREWMAN, 1);
  ShipRooms_Say(LOCALID_ROWING_CREWMAN);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectEvent_FaceDirection(LOCALID_ROWING_CREWMAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_ROWING_DRUMMER, FACING_WEST, 40);
  ObjectEvent_ShowEmote(LOCALID_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  ObjectEvent_ShowEmote(LOCALID_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_KAJA, 3);
  Script_ShowMessageAndWait(LOCALID_ROWING_KAJA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_CREWMAN, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_DRUMMER, 3);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_ROWING_DRUMMER, (mode << 1), 20);
  ShipRooms_Turn(LOCALID_ROWING_DRUMMER, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(LOCALID_ROWING_DRUMMER);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  Script_ShowMessageAndWait(LOCALID_ROWING_KAJA, 0, 60);
  ObjectEvent_ShowEmote(LOCALID_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                        20);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_NORTHWEST + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 80);
  ShipRooms_Turn(LOCALID_ROWING_CREWMAN, FACING_NORTH + FACING_STEP);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_CREWMAN, 2);
  ShipRooms_Say(LOCALID_ROWING_CREWMAN);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_DRUMMER, 3);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_DRUMMER, 2);
  ShipRooms_Say(LOCALID_ROWING_DRUMMER);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_SOUTH + FACING_STEP);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectEvent_SetSpeed(LOCALID_ROWING_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 158);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 148);
  ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_EAST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_WEST, 20);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 134);
  ObjectEvent_WalkTo(LOCALID_ROWING_KAJA, 440, 134);
  FieldScript_WaitFrames(40);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT);
}

/// Raises the anchor charm and announces the ship's first departure.
static void ShipRooms_CastOff(void) {
  struct ObjectEvent *anchorCharm;

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 0, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_QUARTERS_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CAPTAIN, 468, 614);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CAPTAIN, 472, 596);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN, FACING_WEST, 20);
  ObjectMotion_Launch(LOCALID_QUARTERS_CAPTAIN, 4, 20);
  anchorCharm = ShipRooms_PlaceAnchorCharm();
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_RISING_CHIMES);
  ObjectEvent_SetScript(anchorCharm, gShipRooms_AnchorCharmPulse);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkToAndWait(LOCALID_QUARTERS_CAPTAIN, 466, 624);
  ShipRooms_Turn(LOCALID_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
  FieldScript_SetMessageId(MsgTolbiShipCastOff);
  Script_ShowMessageAndWait(LOCALID_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/// Announces departure after a replacement rower has joined the crew.
static void ShipRooms_AnnounceDeparture(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_SOUTH + FACING_STEP, 40);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
  FieldScript_SetMessageId(MsgTolbiShipWereOff);
  Script_ShowMessageAndWait(LOCALID_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/// Kaja comes into the hold and calls the first passenger to the rowing deck.
/// "Row those oars!"
static void ShipRooms_KajaCallsToOars(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 0, 0);
  ShipRooms_RunEventCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_HOLD_KAJA, PIXELS(150), PIXELS(586));
  ShipRooms_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 0, 0);
  ObjectEvent_SetSpeed(LOCALID_HOLD_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_HOLD_KAJA, 168, 578);
  ObjectEvent_WalkToAndWait(LOCALID_HOLD_KAJA, 168, 554);
  ObjectEvent_FaceDirection(LOCALID_HOLD_KAJA, FACING_WEST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_HOLD_KAJA, 2);
  FieldScript_SetMessageId(MsgTolbiShipRowThoseOars);
  Script_ShowMessageAndWait(LOCALID_HOLD_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/// Kaja comes into the hold and calls a replacement rower back to the rowing
/// deck.
static void ShipRooms_KajaCallsReplacementToOars(void) {
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ShipRooms_HideSelectedPassengers();
  ObjectEvent_SetPosition(LOCALID_SEA_HOLD_KAJA, PIXELS(150), PIXELS(586));
  ShipRooms_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 0, 0);
  ObjectEvent_SetSpeed(LOCALID_SEA_HOLD_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_SEA_HOLD_KAJA, 168, 578);
  ObjectEvent_WalkToAndWait(LOCALID_SEA_HOLD_KAJA, 168, 554);
  ObjectEvent_FaceDirection(LOCALID_SEA_HOLD_KAJA, FACING_WEST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_SEA_HOLD_KAJA, 2);
  FieldScript_SetMessageId(MsgTolbiShipRowThoseOars);
  Script_ShowMessageAndWait(LOCALID_SEA_HOLD_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/// Restores the rowing deck for the current set of selected passengers.
static void ShipRooms_ShowRowing(void) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 5);
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  Import_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  ShipRooms_InstallRowingDeckObjectEvents(0);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveWithinMapBounds(0x1b80000, -1, 0xb00000, 1);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 7);
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SE_THUMP);
  FieldScript_WaitFrames(30);
  ShipRooms_InstallRowingDeckObjectEvents(16);
  FieldScript_WaitFrames(0x50);
  ShipRooms_InstallRowingDeckObjectEvents(0);
  FieldScript_WaitFrames(0x3c);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 7);
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SE_THUMP);
  FieldScript_WaitFrames(30);
  ShipRooms_InstallRowingDeckObjectEvents(16);
  FieldScript_WaitFrames(0x50);
  ShipRooms_InstallRowingDeckObjectEvents(0);
  FieldScript_WaitFrames(0x5a);
  Sound_PlaySong(SE_THUMP);
  FieldScript_WaitFrames(30);

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);

  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, 0, 0);

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    Import_ScriptRequestWarp(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    Import_ScriptRequestWarp(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    Import_ScriptRequestWarp(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    Import_ScriptRequestWarp(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK);
  } else {
    Import_ScriptRequestWarp(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK);
  }
}

/// Asks the crew about the monsters before returning belowdecks.
static void ShipRooms_AskAboutMonsters(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_QUARTERS_CAPTAIN, 2);
  FieldScript_SetMessageId(MsgTolbiShipMonsters);
  Script_ShowMessageAndWait(LOCALID_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS, 0);
}

/// Announces the ship's next turn and returns control to the rowing deck.
static void ShipRooms_AnnounceTurn(void) {
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_CAPTAIN, 1);
  FieldScript_SetMessageId(MsgTolbiShipImTurning);
  ShipRooms_Say(LOCALID_QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_NORTH + FACING_STEP, 40);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD, 0);
}

/// Stages the first monster encounter with the original rowers.
static void ShipRooms_StartFirstAttack(void) {
  const s32 *entryScript;

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 1, 0);
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                            LOCALID_ROWING_FIRST_OARSMAN + 1,
                            LOCALID_ROWING_FIRST_OARSMAN + 2);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN, 6);
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_OARSMAN + 2,
                              gShipRooms_Shiver);
  entryScript = gShipRooms_MonsterBob;
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER, entryScript);
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 1, entryScript);
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 2, entryScript);
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER,
                                   SPRITE_COLOR_BGR);
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER + 1,
                                   SPRITE_COLOR_BGR);
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER + 2,
                                   SPRITE_COLOR_BGR);
  ShipRooms_ReportMonstersBelowdecks();
  FieldScript_End();
}

/// Restores the original rowers after the first monster attack.
static void ShipRooms_EndFirstAttack(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                            LOCALID_ROWING_FIRST_OARSMAN + 1,
                            LOCALID_ROWING_FIRST_OARSMAN + 2);
  ShipRooms_ReactToMonsterAttack();
  GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  FieldScript_End();
}

/// Introduces the selected passenger and leads them to the open rowing seat.
static void ShipRooms_IntroduceReplacementOarsman(s32 seat) {
  s32 rowerId;
  struct ObjectEvent *objectEvent;
  s32 halfStep;
  s32 leftSeat;
  s32 x;

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 0, 0);
  x = 0x1b0;
  halfStep = PIXELS(1) / 2;
  ShipRooms_PlaceObjectEvent(LOCALID_PARTY_LEADER, x, 134, FACING_WEST);
  ShipRooms_PlaceFoundObjectEvents(1);
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 406, 134);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 406, 152);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 421, 152);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_LinkPair(LOCALID_ROWING_KAJA, LOCALID_PARTY_LEADER, 10);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_FindFirstSetFlag(seat, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
    FieldScript_WaitFrames(20);
    ObjectEvent_LinkPair(LOCALID_ROWING_KAJA, LOCALID_PARTY_LEADER, 10);
    FieldScript_SetMessageId(MsgTolbiShipBackBroughtOarsman);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    leftSeat = 0;
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), halfStep);
    ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, x, 168);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Import_ObjectEventSetPosition(rowerId, objectEvent->x.fixed,
                                    objectEvent->z.fixed);
    }
    ObjectEvent_SetSpeed(rowerId, PIXELS(1), halfStep);
    ObjectEvent_WalkToAndWait(rowerId, 448, 168);
    ObjectEvent_FaceDirection(rowerId, FACING_NORTHWEST + FACING_STEP, 20);
    ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA,
                              FACING_SOUTHEAST + FACING_STEP, 20);
    Import_ObjectEventSetAnimation(LOCALID_ROWING_KAJA, 3);
    ShipRooms_Say(LOCALID_ROWING_KAJA);
    ObjectEvent_ShowEmote(rowerId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
    Import_ObjectEventSetAnimation(LOCALID_ROWING_KAJA, 3);
    ShipRooms_Say(LOCALID_ROWING_KAJA);
    ObjectMotion_SetAnimationAndWait(rowerId, 3);
    if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
      ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA,
                                FACING_SOUTHEAST + FACING_STEP, 0);
      ObjectEvent_WalkToAndWait(rowerId, 470, 204);
      ShipRooms_Turn(rowerId, FACING_NORTHWEST + FACING_STEP);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 422, 154);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
      ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_SOUTH + FACING_STEP,
                                0);
      ObjectEvent_WalkToAndWait(rowerId, 410, 204);
      leftSeat = 1;
      ShipRooms_Turn(rowerId, FACING_NORTH + FACING_STEP);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
      ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA,
                                FACING_SOUTHEAST + FACING_STEP, 0);
      ObjectEvent_WalkToAndWait(rowerId, 470, 172);
      ShipRooms_Turn(rowerId, FACING_NORTHWEST + FACING_STEP);
    } else {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 422, 154);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
      ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_SOUTH + FACING_STEP,
                                0);
      ObjectEvent_WalkToAndWait(rowerId, 410, 172);
      leftSeat = 1;
      ShipRooms_Turn(rowerId, FACING_NORTH + FACING_STEP);
    }
    ObjectEvent_LinkPair(LOCALID_ROWING_KAJA, LOCALID_PARTY_LEADER, 20);
    ShipRooms_Say(SPEAKER(LOCALID_ROWING_KAJA, SPEAKER_PORTRAIT_RIGHT));
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
    ObjectEvent_SetSpeed(LOCALID_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
    if (leftSeat != 0) {
      ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 428, 164);
      ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 164);
    }
    ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 134);
    ObjectMotion_ResetAndSetPositionInMode2(LOCALID_ROWING_KAJA, 0x1b8, 134);
    FieldScript_WaitFrames(40);
    ShipRooms_RunEventCommand(
        SHIP_CLOSE_SCREEN, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
  } else {
    FieldScript_SetMessageId(MsgTolbiShipBack);
    Script_ShowMessageAndWait(KAJA_ROWING_SPEAKER, 0, 40);
    FieldEffect_SpawnLinked(LOCALID_ROWING_KAJA,
                            EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    FieldEffect_ConfigureLinkedObject(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(60);
    ObjectEvent_ShowEmote(LOCALID_ROWING_KAJA,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    ObjectMotion_SetVariantCallback(LOCALID_ROWING_KAJA, 2);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    FieldEffect_SpawnLinked(LOCALID_ROWING_KAJA,
                            EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 4);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(20);
    Import_ScriptRequestWarp(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
  }
}

/// Creates the visible anchor charm once a visit
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN records it) and retains it for departure
/// scripts.
static struct ObjectEvent *ShipRooms_PlaceAnchorCharm(void) {
  s32 set = Script_GetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN);
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  u8 *buffer;

  if (set != 0) {
    return gShipRooms_AnchorObject;
  }
  objectEvent = ObjectEvent_Create(SPRITE_BLANK, 0x1c70000, 0x40000, 0x2200000);
  objectEvent->motionFlags = set;
  objectEvent->retainSpriteCache = 1;
  sprite = objectEvent->sprite;
  sprite->count = set;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  buffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  Icon_DrawItem(ITEM_ANCHOR_CHARM);
  VramCache_Load(sprite->slot, 128, buffer + 0x400);
  FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN);
  gShipRooms_AnchorObject = objectEvent;
  return objectEvent;
}

/// Restores each replacement rower or the original occupant of that seat.
static void ShipRooms_InstallRowingDeckObjectEvents(u32 action) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(0, 0);
    Import_ObjectEventSetPosition(rowerId, PIXELS(410), PIXELS(172));
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                              LOCALID_ROWING_FIRST_OARSMAN, action);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(1, 0);
    Import_ObjectEventSetPosition(rowerId, PIXELS(470), PIXELS(172));
    ObjectEvent_GetById(rowerId)->scaleX = -Q16_ONE;
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 1, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                              LOCALID_ROWING_FIRST_OARSMAN + 1, action);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(2, 0);
    Import_ObjectEventSetPosition(rowerId, PIXELS(410), PIXELS(204));
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 2, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                              LOCALID_ROWING_FIRST_OARSMAN + 2, action);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(3, 0);
    Import_ObjectEventSetPosition(rowerId, PIXELS(470), PIXELS(204));
    ObjectEvent_GetById(rowerId)->scaleX = -Q16_ONE;
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 3, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                              LOCALID_ROWING_FIRST_OARSMAN + 3, action);
  }

  ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                            LOCALID_ROWING_FIRST_OARSMAN + 4, action);
  ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                            LOCALID_ROWING_FIRST_OARSMAN + 5, action);
  ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                            LOCALID_ROWING_FIRST_OARSMAN + 6, action);
  ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                            LOCALID_ROWING_FIRST_OARSMAN + 7, action);
}

/// Applies the seated pose and rowing action assigned to this rower.
static void ShipRooms_PoseObjectEvent(s32 id) {
  switch (id) {
  case LOCALID_ROWING_SECOND_GUIDE:
    Import_ObjectEventSetAnimation(id, 6);
    ObjectEvent_SetActionById(id, 8);
    break;
  case LOCALID_ROWING_HEAD_GUIDE:
  case LOCALID_ROWING_SWEATING_PASSENGER:
    Import_ObjectEventSetAnimation(id, 5);
    ObjectEvent_SetActionById(id, 16);
    break;
  case LOCALID_ROWING_STARED_AT_PASSENGER:
  case LOCALID_ROWING_WARRIOR:
    Import_ObjectEventSetAnimation(id, 5);
    ObjectEvent_SetActionById(id, 20);
    break;
  case LOCALID_ROWING_TICKET_GRUMBLER:
    Import_ObjectEventSetAnimation(id, 10);
    ObjectEvent_SetActionById(id, 8);
    break;
  case LOCALID_ROWING_LADY:
  case LOCALID_ROWING_MUSCLE_ADMIRER:
    Import_ObjectEventSetAnimation(id, 5);
    ObjectEvent_SetActionById(id, 4);
    break;
  case LOCALID_ROWING_OLD_MAN:
    Import_ObjectEventSetAnimation(id, 9);
    ObjectEvent_SetActionById(id, 4);
    break;
  }
}

static void ShipRooms_Say(s32 speaker) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(10);
}

/// Turns an object event to a facing over 10 frames.
static void ShipRooms_Turn(s32 localId, u16 facing) {
  ObjectMotion_ArmCallback(localId, facing, 10);
}

/// Places an object event at pixel coordinates and sets its initial heading.
static void ShipRooms_PlaceObjectEvent(s32 localId, s32 x, s32 z, s32 facing) {
  Import_ObjectEventSetPosition(localId, x << 16, z << 16, facing);
  ObjectEvent_GetById(localId)->facing = facing;
}

/// Moves the camera, optionally waits or redraws, then honors the packed delay.
static void ShipRooms_MoveCameraAndWait(s32 x, s32 y, s32 z, u32 flags) {
  u32 selected;

  FieldCamera_MoveWithinMapBounds(x, y, z, ~flags & SHIP_CAMERA_JUMP);
  // XXX: likely a mask of 0xffff was meant, because the callers pass waits of
  // 20 and 40 frames, which this mask cuts to 16 and none; SHIP_CAMERA_JUMP
  // also adds a frame.
  selected = flags & 0x1111;
  if ((flags & SHIP_CAMERA_WAIT_FOR_ARRIVAL) != 0)
    FieldCamera_WaitForArrival();
  if ((flags & SHIP_CAMERA_RECENTER_LAYERS) != 0)
    MapGrid_CenterOnFollowedPoint();
  FieldScript_WaitFrames(selected);
}

/// Reuses the voyage's object event, camera, transition and selection
/// operations. Each command names how it interprets the two arguments below.
static void ShipRooms_RunEventCommand(s32 step, u32 arg, u32 opt) {
  u32 slot;

  switch (step) {
  case SHIP_FACE_PARTY_EAST_AND_WEST:
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
    ObjectMotion_ArmCallback(LOCALID_IVAN, 0, 0);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, opt);
    break;
  case SHIP_FACE_PARTY:
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, arg, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, arg, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, arg, 0);
    ObjectEvent_FaceDirection(LOCALID_MIA, arg, opt);
    break;
  case SHIP_PLAY_PARTY_ANIMATION:
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
    if (arg != 0) {
      ObjectMotion_WaitForAnimationChange(3);
    }
    if (opt == 0) {
      break;
    }
    FieldScript_WaitFrames(opt);
    break;
  case SHIP_SHOW_PARTY_EMOTE:
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_IVAN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_MIA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(opt);
    break;
  case SHIP_HIDE_FIRST_OARSMEN:
    for (slot = 0; slot < arg; slot++) {
      ObjectEvent_SetPosition(slot + LOCALID_ROWING_FIRST_OARSMAN, 0, 0);
    }
    break;
  case SHIP_START_ROWING_ANIMATION: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
  }
    Import_ObjectEventSetAnimation(arg, 5);
    ObjectEvent_SetActionById(arg, opt);
    break;
  case SHIP_START_MIRRORED_ROWING_ANIMATION: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
    objectEvent->scaleX = -Q16_ONE;
  }
    Import_ObjectEventSetAnimation(arg, 5);
    ObjectEvent_SetActionById(arg, opt);
    break;
  case SHIP_SET_SELECTED_ROWER_POSE: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
  }
    ShipRooms_PoseObjectEvent(arg);
    if (opt == 0) {
      ObjectEvent_SetActionById(arg, 0);
    }
    break;
  case SHIP_OPEN_SCREEN:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    FieldScript_StartScreenTransition();
    if (arg != 0) {
      FieldScript_WaitTransitionFrames();
    }
    FieldScript_WaitFrames(0);
    break;
  case SHIP_CLOSE_SCREEN:
    FieldScript_FinishScreenTransition();
    FieldScript_WaitTransitionFrames();
    if (arg == 0) {
      break;
    }
    Import_ScriptRequestWarp(arg);
    break;
  case SHIP_SETUP_ROWING_PARTY:
    ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
    ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
    ShipRooms_PlaceFoundObjectEvents(0);
    ShipRooms_PlaceObjectEvent(LOCALID_PARTY_LEADER, 432, 168, FACING_SOUTH);
    ShipRooms_PlaceObjectEvent(LOCALID_GARET, 448, 168, FACING_SOUTH);
    ShipRooms_PlaceObjectEvent(LOCALID_IVAN, 424, 152, FACING_SOUTH);
    ShipRooms_PlaceObjectEvent(LOCALID_MIA, 458, 152, FACING_SOUTH);
    break;
  case SHIP_RESTORE_ROWING_DECK_POSES:
    if (arg != 0) {
      struct ObjectEvent *objectEvent;

      Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 3, 1);
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 3);
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 3);
      objectEvent->scaleX = Q16_ONE;
    }
    {
      struct ObjectEvent *objectEvent;

      Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 4, 1);
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 4);
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
      Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 5, 1);
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 5);
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 5);
      objectEvent->scaleX = Q16_ONE;
      Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 6, 1);
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 6);
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
      Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN + 7, 1);
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 7);
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
      objectEvent = ObjectEvent_GetById(LOCALID_ROWING_FIRST_OARSMAN + 7);
      objectEvent->scaleX = Q16_ONE;
    }
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR, PIXELS(410), PIXELS(174));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 1, PIXELS(470),
                            PIXELS(174));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 2, PIXELS(410),
                            PIXELS(206));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 3, PIXELS(470),
                            PIXELS(206));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 4, PIXELS(410),
                            PIXELS(286));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 5, PIXELS(470),
                            PIXELS(286));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 6, PIXELS(410),
                            PIXELS(316));
    ObjectEvent_SetPosition(LOCALID_ROWING_FIRST_OAR + 7, PIXELS(470),
                            PIXELS(316));
    WaitFrames(1);
    if (arg != 0) {
      ObjectEvent_FaceDirection(LOCALID_ROWING_FIRST_OARSMAN + 3,
                                FACING_NORTHWEST + FACING_STEP, 0);
    }
    ObjectEvent_FaceDirection(LOCALID_ROWING_FIRST_OARSMAN + 4,
                              FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_ROWING_FIRST_OARSMAN + 5,
                              FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_ROWING_FIRST_OARSMAN + 6,
                              FACING_NORTH + FACING_STEP, 0);
    ShipRooms_Turn(LOCALID_ROWING_FIRST_OARSMAN + 7,
                   FACING_NORTHWEST + FACING_STEP);
    break;
  case SHIP_RESTORE_ROWER_POSE: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    Import_ObjectEventSetAnimation(arg, 1);
    if (opt != 0) {
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
    } else {
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
    }
    objectEvent->scaleX = Q16_ONE;
  } break;
  case SHIP_HIDE_QUARTERS_CREW:
    ObjectEvent_SetPosition(LOCALID_QUARTERS_KAJA, 0, 0);
    ObjectEvent_SetPosition(LOCALID_QUARTERS_SEAN, 0, 0);
    ObjectEvent_SetPosition(LOCALID_QUARTERS_OURANOS, 0, 0);
    ObjectEvent_SetPosition(LOCALID_QUARTERS_SECOND_GUIDE, 0, 0);
    ObjectEvent_SetPosition(LOCALID_QUARTERS_CREWMAN, 0, 0);
    break;
  case SHIP_HIDE_OURANOS_AND_SEAN:
    ObjectEvent_SetPosition(LOCALID_HOLD_SEAN, 0, 0);
    ObjectEvent_SetPosition(LOCALID_HOLD_OURANOS, 0, 0);
    break;
  case SHIP_SETUP_CAPTAINS_QUARTERS:
    ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
    ObjectEvent_SetPosition(LOCALID_QUARTERS_KAJA, 0, 0);
    ObjectEvent_SetPosition(LOCALID_QUARTERS_CREWMAN, 0, 0);
    ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_CAPTAIN, 444, 614,
                               FACING_NORTH + FACING_STEP);
    ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
    if (arg != 0) {
      ShipRooms_PlaceAnchorCharm();
    }
    ShipRooms_MoveCameraAndWait(PIXELS(448), PIXELS(32), PIXELS(624),
                                SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
    if (opt == 0) {
      break;
    }
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    FieldScript_WaitFrames(20);
    break;
  case SHIP_SETUP_RECRUITMENT_DECK:
    ObjectEvent_SetPosition(LOCALID_ROWING_CREWMAN, 0, 0);
    ObjectEvent_SetPosition(LOCALID_ROWING_DRUMMER, 0, 0);
    ObjectEvent_SetPosition(LOCALID_ROWING_KAJA, PIXELS(438), PIXELS(152));
    break;
  case SHIP_HIDE_OARS:
    slot = 0;
    do {
      ObjectEvent_SetPosition(slot + LOCALID_ROWING_FIRST_OAR, 0, 0);
      slot++;
    } while (slot <= 7);
    break;
  case SHIP_SETUP_PASSENGER_HOLD:
    ShipRooms_PlaceObjectEvent(LOCALID_HOLD_SECOND_GUIDE, 152, 532,
                               FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_HOLD_WARRIOR, 134, 490,
                               FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_HOLD_TICKET_GRUMBLER, 166, 490,
                               FACING_SOUTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_HOLD_MUSCLE_ADMIRER, 182, 504,
                               FACING_SOUTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_HOLD_OLD_MAN, 118, 504,
                               FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    break;
  case SHIP_SETUP_MONSTER_ENCOUNTER:
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_CREWMAN, 416, 328, FACING_EAST);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_DRUMMER, 448, 352,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN, 454, 248,
                               FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(arg, 408, 290, FACING_EAST);
    ShipRooms_PlaceObjectEvent(opt, 408, 342, FACING_EAST);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN + 3, 420, 356,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN + 4, 408, 304,
                               FACING_EAST);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN + 5, 418, 378,
                               FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN + 6, 440, 262,
                               FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN + 7, 448, 378,
                               FACING_NORTH + FACING_STEP);
    break;
  case SHIP_CLEAR_FLAG_RANGE:
    for (slot = arg; slot <= opt; slot++) {
      GameFlag_Clear(slot);
    }
    break;
  case SHIP_CLEAR_ROWING_SELECTIONS:
    ShipRooms_RunEventCommand(SHIP_CLEAR_FLAG_RANGE,
                              FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                              FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 8);
    ShipRooms_RunEventCommand(SHIP_CLEAR_FLAG_RANGE,
                              FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                              FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 8);
    ShipRooms_RunEventCommand(SHIP_CLEAR_FLAG_RANGE,
                              FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS,
                              FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 8);
    GameFlag_Clear(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
    GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
    break;
  case SHIP_RESTORE_SHIVERING_OARSMAN:
    WaitFrames(1);
    ShipRooms_RunEventCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
    ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_OARSMAN + 2,
                                gShipRooms_Shiver);
    break;
  case SHIP_HIDE_PARTY_FOLLOWERS:
    ObjectEvent_DestroyById(LOCALID_GARET);
    ObjectEvent_DestroyById(LOCALID_IVAN);
    ObjectEvent_DestroyById(LOCALID_MIA);
    break;
  case SHIP_RESET_CAMERA:
    FieldCamera_MoveTo(-1, -1, -1, 0);
    WaitFrames(1);
    if (arg != 0) {
      FieldCamera_GetScriptObject()->motionFlags = 0;
    }
    break;
  case SHIP_PLACE_FOLLOWERS_AND_MONSTERS:
    FieldScript_CallWithLastActiveObjectId(
        gShipRooms_PartyFollowerObjectEventTemplates);
    WaitFrames(1);
    if (arg == 1) {
      FieldScript_CallWithLastActiveObjectId(gShipRooms_FirstAttackMonsters);
      WaitFrames(1);
    } else if (arg == 2) {
      FieldScript_CallWithLastActiveObjectId(gShipRooms_SecondAttackMonsters);
      WaitFrames(1);
    } else if (arg == 3) {
      FieldScript_CallWithLastActiveObjectId(gShipRooms_ThirdAttackMonsters);
      WaitFrames(1);
    }
    break;
  }
}

/// Finds the selected passenger or rowing-deck object event for a seat's flag
/// group.
static s32 ShipRooms_FindFirstSetFlag(u32 seat, s32 alternate) {
  s32 flag = 0;
  s32 id = LOCALID_SEA_HOLD_FIRST_PASSENGER;
  u32 i;

  if (alternate == 0) {
    id = LOCALID_ROWING_FIRST_PASSENGER;
  }
  switch (seat) {
  case 0:
    flag = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    flag = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    flag = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    flag = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (i = 0; i <= 8; i++, flag++, id++) {
    if (GameFlag_Get(flag) != 0) {
      return id;
    }
  }
  return 0;
}

/// Hides the four passengers who are already filling rowing seats.
static void ShipRooms_HideSelectedPassengers(void) {
  ShipRooms_HideSelectedPassenger(LOCALID_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS);
  ShipRooms_HideSelectedPassenger(LOCALID_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  ShipRooms_HideSelectedPassenger(LOCALID_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS);
  ShipRooms_HideSelectedPassenger(LOCALID_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
}

/// Hides the passenger whose selection flag is set in this seat's flag group.
static void ShipRooms_HideSelectedPassenger(s32 localId, s32 firstFlag) {
  unsigned int i = 0;

  do {
    if (GameFlag_Get(firstFlag) != 0) {
      Import_ObjectEventSetPosition(localId, 0, 0);
      break;
    }
    i++;
    localId++;
    firstFlag++;
  } while (i <= 8);
}

/// Stages the next monster encounter with one replacement rower.
static void ShipRooms_StartSecondAttack(void) {
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = ShipRooms_FindFirstSetFlag(0, 0);
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 2, 0);
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                            LOCALID_ROWING_FIRST_OARSMAN + 2);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN, 6);
  rowerScript = gShipRooms_Shiver;
  ObjectEvent_SetActionScript(firstRower, rowerScript);
  ObjectEvent_DestroyById(LOCALID_ROWING_FIRST_OARSMAN + 1);
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_OARSMAN + 2, rowerScript);
  entryScript = gShipRooms_MonsterBob;
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER, entryScript);
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 1, entryScript);
  ShipRooms_ReportMonstersBelowdecks();
  FieldScript_End();
}

/// Gathers the party after the crew reports monsters belowdecks.
static void ShipRooms_ReportMonstersBelowdecks(void) {
  struct ObjectEvent *objectEvent;
  s32 message;

  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(176),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(440), PIXELS(134));
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_RUN);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 0x198, 134);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 0x198, 152);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 0x1b0, 166);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_IVAN, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_MIA, objectEvent->x.fixed,
                            objectEvent->z.fixed);
  }
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkTo(LOCALID_IVAN, 424, 152);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkTo(LOCALID_GARET, 448, 168);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkToAndWait(LOCALID_MIA, 458, 152);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 40);
  Message_SetVoiceFromObjectEvent(LOCALID_ROWING_FIRST_OARSMAN);
  message = MsgTolbiShipMonstersBelowdecks;
  FieldMessage_ShowCentered(message, 1, 10);
  FieldScript_WaitFrames(10);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(320),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_CREWMAN, 2);
  ShipRooms_Turn(LOCALID_ROWING_CREWMAN, FACING_NORTH + FACING_STEP);
  FieldScript_SetMessageId(message + 1);
  ShipRooms_Say(LOCALID_ROWING_CREWMAN);
  ObjectEvent_FaceDirection(LOCALID_ROWING_CREWMAN, FACING_EAST, 20);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(134),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL);
  ObjectEvent_SetActionScript(LOCALID_GARET,
                              gShipRooms_FollowersRejoinByStairs);
  ObjectEvent_SetActionScript(LOCALID_IVAN, gShipRooms_FollowersRejoinByStairs);
  ObjectEvent_RunActionScriptAndWait(LOCALID_MIA,
                                     gShipRooms_FollowersRejoinByStairs);
  FieldScript_WaitFrames(40);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  ShipRooms_RunEventCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
}

/// The crew checks on the party after the monster fight, then resumes rowing.
static void ShipRooms_ReactToMonsterAttack(void) {
  const s32 *action;

  ShipRooms_PlaceObjectEvent(LOCALID_PARTY_LEADER, 444, 300, FACING_EAST);
  ShipRooms_PlaceObjectEvent(LOCALID_GARET, 458, 310, FACING_EAST);
  ShipRooms_PlaceObjectEvent(LOCALID_IVAN, 444, 330, FACING_EAST);
  ShipRooms_PlaceObjectEvent(LOCALID_MIA, 432, 310, FACING_EAST);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_KAJA, 440, 134, FACING_WEST);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_FIRST_OARSMAN, 454, 248,
                             FACING_SOUTHEAST + FACING_STEP);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN, 6);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(308),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTHEAST, 40);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  FieldScript_SetMessageId(MsgTolbiShipHeyAreYouOk);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTH, 0);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  ObjectEvent_SetSpeed(LOCALID_ROWING_KAJA, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 134);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 152);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 424, 164);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 424, 222);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 424, 262);
  ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP,
                            20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 60);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTHEAST, 60);
  ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_EAST, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_KAJA, 2);
  ObjectEvent_MoveToAndWait(LOCALID_ROWING_KAJA, 0x1b0, 0x10c);
  ObjectEvent_MoveToAndWait(LOCALID_ROWING_KAJA, 0x1c4, 0x10c);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_KAJA, 1);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_NORTH + FACING_STEP);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_KAJA, 2);
  Script_ShowMessageAndWait(LOCALID_ROWING_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 4);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(LOCALID_ROWING_KAJA, 0, 80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  FieldScript_WaitFrames(10);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_SOUTH + FACING_STEP);
  Script_OpenMessage(LOCALID_ROWING_KAJA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
    ShipRooms_Say(LOCALID_ROWING_KAJA);
  } else {
    ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 4);
    gFieldWork->message += 1;
    ShipRooms_Say(LOCALID_ROWING_KAJA);
    ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
    Import_ObjectEventSetAnimation(LOCALID_ROWING_KAJA, 3);
    ShipRooms_Say(LOCALID_ROWING_KAJA);
  }
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  action = gShipRooms_FollowersRejoinMidDeck;
  ObjectEvent_SetActionScript(LOCALID_GARET, action);
  ObjectEvent_SetActionScript(LOCALID_IVAN, action);
  ObjectEvent_RunActionScriptAndWait(LOCALID_MIA, action);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 424, 272);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 424, 164);
  FieldScript_WaitFrames(60);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Import_ScriptRequestWarp(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
}

/// Restores one replacement rower after the next monster attack.
static void ShipRooms_EndSecondAttack(void) {
  s32 handle = ShipRooms_FindFirstSetFlag(0, 0);

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_PlaceFoundObjectEvents(0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle,
                            LOCALID_ROWING_FIRST_OARSMAN + 2);
  Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 1, 0, 0);
  ShipRooms_ReactToMonsterAttack();
  GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF);
  FieldScript_End();
}

/// Stages the next monster encounter with two replacement rowers.
static void ShipRooms_StartThirdAttack(void) {
  s32 secondRower;
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = ShipRooms_FindFirstSetFlag(0, 0);
  secondRower = ShipRooms_FindFirstSetFlag(1, 0);
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 3, 0);
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                            secondRower);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_FIRST_OARSMAN, 6);
  rowerScript = gShipRooms_Shiver;
  ObjectEvent_SetActionScript(firstRower, rowerScript);
  ObjectEvent_DestroyById(LOCALID_ROWING_FIRST_OARSMAN + 1);
  ObjectEvent_SetActionScript(secondRower, rowerScript);
  ObjectEvent_DestroyById(LOCALID_ROWING_FIRST_OARSMAN + 2);
  entryScript = gShipRooms_MonsterBob;
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER, entryScript);
  ObjectEvent_SetActionScript(LOCALID_ROWING_FIRST_MONSTER + 1, entryScript);
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER,
                                   SPRITE_COLOR_BGR);
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_FIRST_MONSTER + 1,
                                   SPRITE_COLOR_BGR);
  ShipRooms_ReportMonstersBelowdecks();
  FieldScript_End();
}

/// Restores two replacement rowers after the next monster attack.
static void ShipRooms_EndThirdAttack(void) {
  s32 handle = ShipRooms_FindFirstSetFlag(0, 0);
  s32 other = ShipRooms_FindFirstSetFlag(1, 0);

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_PlaceFoundObjectEvents(0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle, other);
  Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 1, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_ROWING_FIRST_OARSMAN + 2, 0, 0);
  ShipRooms_ReactToMonsterAttack();
  GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF);
  FieldScript_End();
}

/// SHIP_ROOMS_FROM_WORLD_MAP, on the rowing deck: the ship rocks, a
/// huge sea monster is on the ship, and the party goes up on deck
/// (FLAG_TOLBI_SHIP_KRAKEN_ON_DECK).
static void ShipRooms_NoticeShipRocking(void) {
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  struct ObjectEvent *objectEvent;
  const s32 *action;

  firstRower = ShipRooms_FindFirstSetFlag(0, 0);
  secondRower = ShipRooms_FindFirstSetFlag(1, 0);
  thirdRower = ShipRooms_FindFirstSetFlag(2, 0);
  // Restore the occupied rowing seats before showing the ship's movement.
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  ObjectEvent_SetPosition(LOCALID_ROWING_CREWMAN, PIXELS(472), PIXELS(152));
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 5);
  ObjectEvent_SetPosition(LOCALID_ROWING_KAJA, PIXELS(440), PIXELS(134));
  ObjectEvent_SetSpriteColorById(LOCALID_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
  objectEvent = ObjectEvent_GetById(LOCALID_ROWING_KAJA);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ShipRooms_InstallRowingDeckObjectEvents(16);
  ShipRooms_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  Script_PlaySong(MUS_SILENCE);
  Script_PlaySong(SE_BOOM);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(10);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  FieldScript_WaitFrames(80);
  Script_PlaySong(SE_BOOM);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(10);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Script_PlaySong(MUS_ERUPTION);
  GameFlag_Set(FLAG_KEEP_MUSIC);
  ObjectEvent_SetAttachedEffect(LOCALID_MIA, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ShipRooms_Turn(LOCALID_MIA, FACING_SOUTHWEST);
  // The party notices the rocking, then turns to the crew for an explanation.
  FieldScript_SetMessageId(MsgTolbiShipWonderWhatsWrongShipShouldnt);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 40);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHEAST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ShipRooms_Turn(LOCALID_IVAN, FACING_SOUTHEAST);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  ShipRooms_Say(LOCALID_IVAN);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTHWEST, 20);
  ObjectEvent_SetSpriteColorById(LOCALID_ROWING_KAJA, SPRITE_COLOR_NORMAL);
  objectEvent = ObjectEvent_GetById(LOCALID_ROWING_KAJA);
  ObjectEvent_SetSpriteFlags(objectEvent, 1);
  ObjectEvent_SetSpeed(LOCALID_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 430, 134);
  ShipRooms_Turn(LOCALID_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_KAJA, 2);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectMotion_SetVariantCallback(firstRower, 1);
  ObjectMotion_SetVariantCallback(secondRower, 1);
  ObjectMotion_SetVariantCallback(thirdRower, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_FIRST_OARSMAN + 3, 1);
  ObjectEvent_SetAttachedEffect(firstRower, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(secondRower, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(thirdRower, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_ROWING_FIRST_OARSMAN + 3,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWING_DECK_POSES, 1, 0);
  ObjectEvent_FaceDirection(firstRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(secondRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 40);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_KAJA, 2);
  Script_ShowMessage(LOCALID_ROWING_KAJA, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 20);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 440, 134);
  ObjectEvent_SetPosition(LOCALID_ROWING_KAJA, 0, 0);
  ShipRooms_Turn(LOCALID_GARET, FACING_WEST);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  ShipRooms_Say(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_EAST, 0);
  ShipRooms_Turn(LOCALID_MIA, FACING_WEST);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(1), PIXELS(1) / 2);
  action = gShipRooms_FollowersRejoinBySeats;
  ObjectEvent_SetActionScript(LOCALID_GARET, action);
  ObjectEvent_SetActionScript(LOCALID_IVAN, action);
  ObjectEvent_RunActionScriptAndWait(LOCALID_MIA, action);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  gShipRooms_ShakeTimer = 0;
  FieldScheduler_AddCallback(ShipRooms_ShakeWhileKrakenAboard,
                             TASK_PRIORITY_MAP);
  ShipRooms_RunEventCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  ObjectEvent_DestroyById(LOCALID_ROWING_KAJA);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  FieldScript_End();
}

/// Now and then plays a rumble with a shake, easing the shake after ten frames.
static void ShipRooms_ShakeWhileKrakenAboard(void) {
  s32 value;
  s32 *timer = &gShipRooms_ShakeTimer;
  s32 remaining;
  if (*timer != 0) {
    remaining = *timer - 1;
    *timer = *timer - 1;
    if (remaining == 70)
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  } else {
    value = Import_RandomNext();
    if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
      Sound_PlaySong(SE_BOOM);
      MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      *timer = 80;
    }
  }
}

/// SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND, in the
/// captain's quarters, after the ship has come off course: a short exchange,
/// then SHIP_ROOMS_TO_WORLD_MAP leads by the world map to the island
/// (SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND).
static void ShipRooms_ShowCaptainsQuartersBeforeIsland(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 0);
  ShipRooms_PlaceObjectEvent(LOCALID_QUARTERS_KAJA, 468, 616, FACING_WEST);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_QUARTERS_KAJA, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN,
                            FACING_NORTH + FACING_STEP, 80);
  ObjectEvent_FaceDirection(LOCALID_QUARTERS_CAPTAIN, FACING_EAST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_QUARTERS_CAPTAIN, 3);
  FieldScript_WaitFrames(20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, SHIP_ROOMS_TO_WORLD_MAP, 0);
}

/// SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND, on the rowing deck
/// at the island: Kaja rests the rowers, and each chosen passenger answers
/// (FLAG_TOLBI_SHIP_AT_ISLAND).
static void ShipRooms_ReachIsland(void) {
  s32 fourthRower;
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  struct ObjectEvent *objectEvent;
  s32 reaction = 0;
  const s32 *progressB;
  const s32 *progressC;
  const s32 *progressA;

  firstRower = ShipRooms_FindFirstSetFlag(0, 0);
  secondRower = ShipRooms_FindFirstSetFlag(1, 0);
  thirdRower = ShipRooms_FindFirstSetFlag(2, 0);
  fourthRower = ShipRooms_FindFirstSetFlag(3, 0);
  // Restore all four selected rowers and announce the island stop.
  FieldScript_Start();
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  ObjectEvent_SetPosition(LOCALID_ROWING_CREWMAN, PIXELS(472), PIXELS(152));
  ObjectEvent_SetPosition(LOCALID_ROWING_KAJA, PIXELS(440), PIXELS(134));
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
  objectEvent = ObjectEvent_GetById(LOCALID_ROWING_KAJA);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ShipRooms_InstallRowingDeckObjectEvents(16);
  Import_ObjectEventSetAnimation(LOCALID_ROWING_DRUMMER, 5);
  ShipRooms_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  ObjectEvent_ConfigureSpriteColor(LOCALID_ROWING_KAJA, SPRITE_COLOR_NORMAL);
  objectEvent = ObjectEvent_GetById(LOCALID_ROWING_KAJA);
  ObjectEvent_SetSpriteFlags(objectEvent, 1);
  ObjectEvent_SetSpeed(LOCALID_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 132);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 142);
  ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP,
                            20);
  ObjectMotion_SetVariantCallback(LOCALID_ROWING_KAJA, 2);
  FieldScript_SetMessageId(MsgTolbiShipReachedIslandRowers);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  FieldScript_WaitFrames(120);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, fourthRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWING_DECK_POSES, 0, 0);
  ObjectEvent_FaceDirection(firstRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(secondRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_NORTH + FACING_STEP, 0);
  ObjectMotion_ArmCallback(fourthRower, FACING_NORTHWEST + FACING_STEP, 60);
  // The first rower's identity selects the response to the unexpected stop.
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_OLD_MAN)) {
    reaction = 2;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                          PASSENGER_MUSCLE_ADMIRER) ||
             GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_LADY)) {
    reaction = 1;
  }
  ObjectMotion_SetVarCbAndRefresh(firstRower, 1);
  if (reaction == 1) {
    gFieldWork->message += 1;
  } else if (reaction == 2) {
    gFieldWork->message += 2;
  }
  ObjectMotion_SetVariantCallback(firstRower, 2);
  ShipRooms_Say(firstRower);
  FieldScript_SetMessageId(MsgTolbiShipShipWentOff);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 4);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectEvent_ShowEmote(firstRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(secondRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(thirdRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldEffect_SpawnLinked(fourthRower, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 132);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 444, 132);
  ObjectEvent_DestroyById(LOCALID_ROWING_KAJA);
  FieldScript_WaitFrames(40);
  if (reaction == 0 &&
      (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_HEAD_GUIDE) ||
       GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                    PASSENGER_SECOND_GUIDE))) {
    reaction = 3;
  }
  if (reaction == 0) {
    gFieldWork->message += 1;
  } else if (reaction == 1) {
    gFieldWork->message += 2;
  } else if (reaction == 2) {
    gFieldWork->message += 3;
  }
  // Send the rowers and party back through their departure scripts.
  ShipRooms_Turn(firstRower, 0);
  ShipRooms_Say(firstRower);
  ObjectEvent_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  progressB = gShipRooms_LeaveByStairs;
  ObjectEvent_RunActionScriptAndWait(firstRower, progressB);
  ObjectEvent_FaceDirection(secondRower, FACING_SOUTH + FACING_STEP, 0);
  ObjectMotion_ArmCallback(thirdRower, FACING_EAST, 0);
  ObjectEvent_FaceDirection(fourthRower, FACING_WEST, 40);
  ObjectMotion_ArmCallback(secondRower, FACING_NORTH + FACING_STEP, 0);
  ObjectMotion_ArmCallback(thirdRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectMotion_ArmCallback(fourthRower, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetActionScript(thirdRower, progressB);
  FieldScript_WaitFrames(40);
  progressC = gShipRooms_StepIntoAisle;
  ObjectEvent_RunActionScriptAndWait(secondRower, progressC);
  ObjectEvent_SetActionScript(secondRower, progressB);
  ObjectEvent_RunActionScriptAndWait(fourthRower, progressC);
  ObjectEvent_RunActionScriptAndWait(fourthRower, progressB);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(1), PIXELS(1) / 2);
  progressA = gShipRooms_FollowersRejoinByStairs;
  ObjectEvent_SetActionScript(LOCALID_GARET, progressA);
  ObjectEvent_SetActionScript(LOCALID_IVAN, progressA);
  ObjectEvent_RunActionScriptAndWait(LOCALID_MIA, progressA);
  ShipRooms_RunEventCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  GameFlag_Set(FLAG_TOLBI_SHIP_AT_ISLAND);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  FieldScript_End();
}

/// SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS, on the
/// rowing deck back from the island: Kaja switches the left and right teams,
/// the left team's timing having taken the ship off course; the chosen
/// passengers swap seats and the ship sets off again.
static void ShipRooms_DismissReplacementRowers(void) {
  s32 firstRower;
  s32 fourthRower;
  s32 thirdRower;
  s32 secondRower;

  firstRower = ShipRooms_FindFirstSetFlag(0, 0);
  secondRower = ShipRooms_FindFirstSetFlag(1, 0);
  thirdRower = ShipRooms_FindFirstSetFlag(2, 0);
  fourthRower = ShipRooms_FindFirstSetFlag(3, 0);
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_CREWMAN, 472, 144,
                             FACING_SOUTH + FACING_STEP);
  ShipRooms_PlaceObjectEvent(LOCALID_ROWING_KAJA, 408, 142,
                             FACING_SOUTHEAST + FACING_STEP);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  FieldScript_SetMessageId(MsgTolbiShipSorryEveryoneButWeNeed);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  ObjectMotion_SetVariantCallback(firstRower, 2);
  ObjectMotion_SetVariantCallback(secondRower, 2);
  ObjectMotion_SetVariantCallback(thirdRower, 2);
  ObjectMotion_SetVarCbAndRefresh(fourthRower, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(firstRower, FACING_EAST, 0);
  ObjectEvent_FaceDirection(secondRower, FACING_WEST, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_EAST, 0);
  ObjectEvent_FaceDirection(fourthRower, FACING_WEST, 40);
  ObjectEvent_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(firstRower, 470, 172);
  ObjectEvent_WalkTo(secondRower, 410, 172);
  ObjectEvent_WalkTo(thirdRower, 470, 204);
  ObjectEvent_WalkToAndWait(fourthRower, 410, 204);
  Import_ObjectEventSetAnimation(firstRower, 1);
  Import_ObjectEventSetAnimation(secondRower, 1);
  Import_ObjectEventSetAnimation(thirdRower, 1);
  ObjectEvent_FaceDirection(secondRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(firstRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(fourthRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ROWING_KAJA, 1);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  Import_ObjectEventSetAnimation(firstRower, 3);
  Import_ObjectEventSetAnimation(secondRower, 3);
  Import_ObjectEventSetAnimation(thirdRower, 3);
  ObjectMotion_SetAnimationAndWait(fourthRower, 3);
  ShipRooms_Say(LOCALID_ROWING_KAJA);
  Import_ObjectEventSetAnimation(firstRower, 3);
  Import_ObjectEventSetAnimation(secondRower, 3);
  Import_ObjectEventSetAnimation(thirdRower, 3);
  ObjectMotion_SetAnimationAndWait(fourthRower, 3);
  ObjectEvent_FaceDirection(LOCALID_ROWING_KAJA, FACING_EAST, 0);
  ShipRooms_Turn(LOCALID_PARTY_LEADER, FACING_WEST);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_ROWING_KAJA, 3);
  ObjectEvent_SetSpeed(LOCALID_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 408, 132);
  ObjectEvent_WalkToAndWait(LOCALID_ROWING_KAJA, 444, 132);
  ObjectEvent_SetPosition(LOCALID_ROWING_KAJA, 0, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  ShipRooms_SwapRowerSelections(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  ShipRooms_SwapRowerSelections(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_AT_ISLAND);
  Import_ScriptRequestWarp(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY);
}

/// Exchanges the chosen passengers between two rowing seats.
static void ShipRooms_SwapRowerSelections(s32 firstSeatFlags,
                                          s32 secondSeatFlags) {
  s32 firstPassenger = 0;
  s32 secondPassenger = 0;
  u32 i;

  for (i = 0; i <= 8; i++) {
    s32 flag = firstSeatFlags + i;
    if (GameFlag_Get(flag) != 0) {
      GameFlag_Clear(flag);
      break;
    }
    firstPassenger++;
  }

  for (i = 0; i <= 8; i++) {
    s32 flag = secondSeatFlags + i;
    if (GameFlag_Get(flag) != 0) {
      GameFlag_Clear(flag);
      break;
    }
    secondPassenger++;
  }

  GameFlag_Set(secondSeatFlags + firstPassenger);
  GameFlag_Set(firstSeatFlags + secondPassenger);
}

/// The step beside the leader for each of the 16 facings: the high 16 bits are
/// x, the low 16 bits z.
#define TURN_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gShipRooms_TurnSteps[] = {
    TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(0, 16),
    TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(-16, 0),
    TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(0, -16),
    TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(16, 0),
};

/// The warriors walk out of the hold.
s32 gShipRooms_LeaveHold[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(168), 0, PIXELS(500)),
    SC_SYNC,         SC_MOVE(PIXELS(168), 0, PIXELS(584)),
    SC_SYNC,         SC_CALL(ShipRooms_ResetObjectEvent),
    SC_END,
};

/// Those gathered in the captain's quarters walk out.
s32 gShipRooms_LeaveCaptainsQuarters[] = {
    SC_SET(SW_SPEED, PIXELS(1)),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 2),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(674)),
    SC_SYNC,
    SC_MOVE(PIXELS(456), 0, PIXELS(712)),
    SC_SYNC,
    SC_CALL(ShipRooms_ResetObjectEvent),
    SC_END,
};

/// The Anchor Charm swells and shrinks twice as the captain raises it.
s32 gShipRooms_AnchorCharmPulse[] = {
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

/// The party's followers walk into the leader by the rowing deck's stairs.
s32 gShipRooms_FollowersRejoinByStairs[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(434), 0, PIXELS(166)),
    SC_SYNC,         SC_CALL(ShipRooms_ResetObjectEvent),
    SC_END,
};

/// The same, in the middle of the rowing deck.
s32 gShipRooms_FollowersRejoinMidDeck[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(438), 0, PIXELS(300)),
    SC_SYNC,         SC_CALL(ShipRooms_ResetObjectEvent),
    SC_END,
};

/// The same, by the first row of seats.
s32 gShipRooms_FollowersRejoinBySeats[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(432), 0, PIXELS(168)),
    SC_SYNC,         SC_CALL(ShipRooms_ResetObjectEvent),
    SC_END,
};

/// Squashes and stretches the object event over and over: a shiver.
s32 gShipRooms_Shiver[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A monster in the hold bobs in place.
s32 gShipRooms_MonsterBob[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(ShipRooms_Bob),
};

/// A rower walks up the stairs and is freed.
s32 gShipRooms_LeaveByStairs[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(408), 0, PIXELS(132)),
    SC_SYNC,         SC_MOVE(PIXELS(444), 0, PIXELS(132)),
    SC_SYNC,         SC_FREE,
};

/// A rower steps into the aisle.
s32 gShipRooms_StepIntoAisle[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(416), 0, PIXELS(186)),
    SC_SYNC,
    SC_END,
};

/// A passenger glances about at random.
s32 gShipRooms_LookAroundScript[] = {
    SC_CALL(ShipRooms_LookAround),
};

/// Lies flat on the floor of the hold.
s32 gShipRooms_LieFlatScript[] = {
    SC_CALL(ShipRooms_LayFlat),
};

/// The Anchor Charm's object, once placed this visit.
struct ObjectEvent *gShipRooms_AnchorObject = 0;

/// The other maps have no warp destinations.
struct MapWarpDest gShipRooms_WarpDestsOther[] = {
    {MAP_TABLE_END},
};

/// The passengers' hold, the cargo hold, the rowing deck and the captain's
/// quarters, each warp destination named for its room.
struct MapWarpDest gShipRooms_WarpDests[] = {
    {SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 152, 0,
     584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK, CONDITION_ALWAYS, 160, 0, 552,
     FACING_EAST, 0, 30, 380, 270, 620, 0},
    {SHIP_ROOMS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 120, 0, 136,
     FACING_WEST, 0, 25, 80, 265, 350, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD, CONDITION_ALWAYS, 408, 0, 136,
     FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 440, 0,
     728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF, CONDITION_ALWAYS,
     440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS,
     CONDITION_ALWAYS, 152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING, CONDITION_ALWAYS, 440,
     0, 342, FACING_NORTH, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS, CONDITION_ALWAYS,
     440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN, CONDITION_ALWAYS, 440,
     0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK, CONDITION_ALWAYS,
     408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE, CONDITION_ALWAYS, 408, 0,
     136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE, CONDITION_ALWAYS, 408,
     0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK, CONDITION_ALWAYS,
     408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE, CONDITION_ALWAYS, 408, 0,
     136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_FROM_WORLD_MAP, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0,
     305, 60, 565, 430, 0},
    {SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND, CONDITION_ALWAYS,
     444, 0, 632, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND, CONDITION_ALWAYS, 408, 0,
     136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS, CONDITION_ALWAYS,
     408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT, CONDITION_ALWAYS, 166, 0, 570,
     FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {MAP_TABLE_END},
};

u32 gShipRooms_Warps[] = {
    MAP_WARPS(ShipRooms),
    MAP_WARP(SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_PASSENGER_HOLD),
    MAP_WARP(SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK, ShipRooms,
             SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD),
    MAP_WARP(SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_CRATE_HOLD),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD, ShipRooms,
             SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK),
    MAP_WARP(SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_CAPTAINS_ROOM),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY),
    MAP_WARP(SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, ShipRooms,
             SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS),
    MAP_WARP(SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, ShipRooms,
             SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK, WorldMap,
             WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK),
    MAP_WARP(SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS,
             TolbiShipDeck, TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS),
    MAP_WARP(SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD,
             TolbiShipDeck, TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK,
             WorldMap, WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK, WorldMap,
             WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN, WorldMap,
             WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN),
    MAP_WARP(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND, WorldMap,
             WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND),
    MAP_WARP(SHIP_ROOMS_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SHIP_ROOMS),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gShipRooms_ObjectEventTemplatesNone[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gShipRooms_CaptainsQuartersObjectEventTemplates[] = {
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(446), 0, PIXELS(620), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(626), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(454), 0, PIXELS(640), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The captain's quarters once the warriors have come to see the captain, with the party's
/// followers.
struct ObjectEventTemplate gShipRooms_CaptainsQuartersObjectEventTemplatesWarriors[] = {
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(482), 0, PIXELS(604), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(464), 0, PIXELS(690), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(648), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(444), 0, PIXELS(628), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(660), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct ObjectEventTemplate gShipRooms_CaptainsQuartersObjectEventTemplatesArrived[] = {
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(462), 0, PIXELS(612), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

/// The hold before the party hears of the delay at Kalay.
struct ObjectEventTemplate gShipRooms_HoldObjectEventTemplatesEmpty[] = {
    { 0xf4, CONDITION_ALWAYS, (s32)gShipRooms_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct ObjectEventTemplate gShipRooms_HoldObjectEventTemplates[] = {
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(504), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(540), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(540), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(520), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(130), 0, PIXELS(456), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(142), 0, PIXELS(446), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(156), 0, PIXELS(466), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(586), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { 0xf4, CONDITION_ALWAYS, (s32)gShipRooms_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The hold at sea, once the passengers row.
struct ObjectEventTemplate gShipRooms_HoldObjectEventTemplatesAtSea[] = {
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(138), 0, PIXELS(446), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(532), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(182), 0, PIXELS(492), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(492), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(480), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(526), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(172), 0, PIXELS(448), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(180), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(122), 0, PIXELS(516), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(586), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { 0xf4, CONDITION_ALWAYS, (s32)gShipRooms_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The rowing deck, in the order of enum TolbiShipRowingDeckLocalId: the crewman
/// and the drummer, the oarsmen at the seats, the passengers who may take a
/// seat, Kaja and the eight oars beside the seats.
struct ObjectEventTemplate gShipRooms_RowingDeckObjectEventTemplates[] = {
    { SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(438), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(438), 0, PIXELS(374), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(410), 0, PIXELS(172), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(470), 0, PIXELS(172), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(410), 0, PIXELS(204), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(470), 0, PIXELS(204), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(410), 0, PIXELS(284), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(470), 0, PIXELS(284), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(410), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(470), 0, PIXELS(314), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(174), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(174), FACING_WEST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(206), FACING_EAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(206), FACING_WEST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(286), FACING_EAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(286), FACING_WEST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(316), FACING_EAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(316), FACING_WEST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

/// The party's followers, placed for an event.
struct ObjectEventTemplate gShipRooms_PartyFollowerObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The monsters of each attack, beside the seats.
struct ObjectEventTemplate gShipRooms_FirstAttackMonsters[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gShipRooms_SecondAttackMonsters[] = {
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gShipRooms_ThirdAttackMonsters[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The cargo hold, and the events.
struct MapEvent gShipRooms_EventsOther[] = {
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK },
    { BG_EVENT(BG_EVENT_BARREL), MB_SHIP_ROOMS_CRATE_HOLD_BARREL, FLAG_TOLBI_SHIP_CRATE_HOLD_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct MapEvent gShipRooms_HoldEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { MAP_EVENT_COORD, MB_SHIP_ROOMS_HOLD_WARRIORS, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, EVENT_SCRIPT(ShipRooms_WatchWarriorsStormOff) },
    { MAP_EVENT_TALK, LOCALID_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToWaitingWarrior) },
    { MAP_EVENT_TALK, LOCALID_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
    { MAP_EVENT_TALK, LOCALID_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
    { MAP_EVENT_TALK, LOCALID_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToWaitingMuscleAdmirer) },
    { MAP_EVENT_TALK, LOCALID_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
    { MAP_EVENT_TALK, LOCALID_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
    { MAP_EVENT_TALK, LOCALID_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
    { MAP_EVENT_TALK, LOCALID_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_WARRIOR, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMaybeIShouldJustForgetAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheyreJustTakingAdvantageOfUs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsDoTheyHonestlyThinkImGoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_MUSCLE_ADMIRER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSureItdBeEasierTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsWeLeftTheOldLadyBack) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellShootMaybeWeShouldHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAskingThePassengersToHelpOut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_HEALER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsEvenThoughImOpposedToViolence) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_HEALER, FLAG_TOLBI_SHIP_REACHED_TOLBI, MSGNO(MsgTolbiShipInteriorsICantRowButIAm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMadeItToColossoIn) },
    { BG_EVENT(BG_EVENT_BARREL), MB_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { MAP_EVENTS_END },
};

/// The hold at sea: talking to a passenger asks them to row.
struct MapEvent gShipRooms_HoldEventsChoosingRowers[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskHeadGuideToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskSweatingPassengerToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskLadyToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskStaredAtPassengerToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskSecondGuideToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskWarriorToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskTicketGrumblerToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskMuscleAdmirerToRow) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskOldManToRow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_HEAD_GUIDE, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouMakeMeNervousJustStanding) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_SWEATING_PASSENGER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyLooksLikeTheyNeedSomeone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenInAColdSweat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_LADY, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsRowingIsAToughJobIsnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhThisPoorSapLooksSad) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_STARED_AT_PASSENGER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyThisGuysBeenStaringAt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhNoNoWhenOurEyes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_WARRIOR, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsOhOhNoHesNotComing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsOnlyNaturalThatINot) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsPleasePleaseImBeggingJustDont) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISnuckByThisTimeBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_MUSCLE_ADMIRER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhatsWithYouGoBotherThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowThisWarriorIsntGonnaLet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhyAmIGettingSoWorked) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanJustMoveRightAlong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_SECOND_GUIDE, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCarefulNowWatchOutDontMake) },
    { BG_EVENT(BG_EVENT_BARREL), MB_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { MAP_EVENTS_END },
};

/// The hold while the ship waits off the island.
struct MapEvent gShipRooms_HoldEventsAtIsland[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
    { MAP_EVENT_TALK, LOCALID_SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskLadyToRow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
    { BG_EVENT(BG_EVENT_BARREL), MB_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { MAP_EVENTS_END },
};

/// The captain's quarters, from the wait at Kalay until the ship reaches Tolbi. The Anchor
/// Charm is given back to the captain here.
struct MapEvent gShipRooms_CaptainsQuartersEvents[] = {
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_CAPTAIN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToCaptain) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_CREWMAN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToCaptainsQuartersCrewman) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_SEAN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToSeanInCaptainsQuarters) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsThisIsAChanceToMake) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheShipWontLeaveDockUntil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipInteriorsAsLongAsWeHaveThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeKnewTheRisksWhenWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CREWMAN, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsSendingOutAShipWhenIts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWaitedTooLongIllGet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIfWeDontSetSailNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThisIsRidiculousTheShipCant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThatThingCouldntHaveGottenLost) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWouldSteal) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipDoesntLeaveSoon) },
    { MAP_EVENT_COORD, MB_SHIP_ROOMS_QUARTERS_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(ShipRooms_WatchCrewWonderAtDelay) },
    { USE_ITEM_ON_OBJ_EVENT(ITEM_ANCHOR_CHARM), LOCALID_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, EVENT_SCRIPT(ShipRooms_ReturnLuckyAnchor) },
    { BG_EVENT(BG_EVENT_JAR) | MAP_EVENT_FLAG_FACING | FACING_NORTH, MB_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

/// The captain's quarters while the ship waits off the island.
struct MapEvent gShipRooms_CaptainsQuartersEventsAtIsland[] = {
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
    { BG_EVENT(BG_EVENT_JAR), MB_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct MapEvent gShipRooms_CaptainsQuartersEventsArrived[] = {
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_TALK, LOCALID_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
    { BG_EVENT(BG_EVENT_JAR), MB_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

struct MapEvent gShipRooms_RowingDeckEvents[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD },
    { MAP_EVENT_TALK, LOCALID_ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsWeCanLeaveAsSoonAs) },
    { MAP_EVENT_TALK, LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenSailingForManyYears) },
    { MAP_EVENT_TALK, LOCALID_ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis) },
    { MAP_EVENT_TALK, LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowIHaveABlisterOn) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRowingIsAMansJob) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereTheOnesWhoMakeThe) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOnceWeCastOffYouJust) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanOnlyDoHardWork) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHmmmIWonderWhenWereGoing) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfItsOkayTo) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsICantWaitUntilWeGet) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsStoppingAgainAndAgainLikeThis) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIHearTheresAnIsleIn) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLetsSeeIslandsInTheKaragol) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsFromTheSeaItLooksLike) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenWereRowingWeCanJust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEnoughIsEnoughLetsSetOff) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsCrazyToHaveInexperiencedPassengers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsILoveItWhenTheOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItWasHardForMeAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsEveryoneDownHereGetsSoCompetitive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseBigMusclesAreASign) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellSeeToItThatThey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouveGotToSaveUpLots) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIDontFeelLikeRowingAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISureHopeAnotherMonsterDoesnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsSpendYourMoneyWhileYoureYoung) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLooksLikeImGoingToHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsItSeemsLikeThereWouldBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCouldThisBeThatIslandThats) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHaHahIfYouWantTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMaybeItsTimeForMeTo) },
    { MAP_EVENTS_END },
};

/// The rowing deck while monsters are aboard: talking to one
/// (LOCALID_ROWING_FIRST_MONSTER and the two after it) fights it.
struct MapEvent gShipRooms_RowingDeckEventsMonstersAboard[] = {
    { MAP_EVENT_COORD, MB_SHIP_ROOMS_ROWING_DECK_STAIRS, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TurnBackFromStairs) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_MONSTER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_FightRowingDeckMonsters) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_MONSTER + 1, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_FightRowingDeckMonsters) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_MONSTER + 2, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_FightRowingDeckMonsters) },
    { MAP_EVENT_TALK, LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMonstersFromTheDeepAreAttacking) },
    { MAP_EVENT_TALK, LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCurseThoseBeastsTheyGotOne) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingOffTheMonstersIsYour) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFoulBeastIMayBeTrembling) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWerentSoTiredId) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCursesIWantToSmackThat) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhoIsThatThingGoingTo) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfYouDontDoABetter) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeOarsmenMayBeStrongBut) },
    { MAP_EVENT_TALK, LOCALID_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSurroundedByDangerOnAll) },
    { MAP_EVENT_TALK, LOCALID_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAhhhhSeaMonstersRealLiveSea) },
    { MAP_EVENT_TALK, LOCALID_ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhaHeyYouThisIsNo) },
    { MAP_EVENT_TALK, LOCALID_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAahIHaveToRowThis) },
    { MAP_EVENT_TALK, LOCALID_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHHeyIsaacShouldntYouBe) },
    { MAP_EVENT_TALK, LOCALID_ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYaaahhhIImNotAfraidOf) },
    { MAP_EVENT_TALK, LOCALID_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheOarsmenAreInDireStraights) },
    { MAP_EVENT_TALK, LOCALID_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIICantDoThisFighting) },
    { MAP_EVENT_TALK, LOCALID_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhatHappensNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyArentYouDoingAnythingTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfWeLoseAnyMoreOarsmen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheSeaMonstersGetUs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImScaredThoseSeaMonstersAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingSeaMonstersIsForWarriors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheSeaMonstersAreSoTough) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhoElseThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseJustStayHereAndProtect) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeDontEvenKnowIfAttacking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeaMonstersAreTerriblyFerocious) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWereAWarriorI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfThisWarriorHas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSureInAFixSay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSoOutOfMyWits) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseSeaMonstersLookAwfullyStrong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseOhPleaseLetMeMake) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyDidSeaMonstersHaveTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIdRatherBeEatenByA) },
    { MAP_EVENTS_END },
};

/// The rowing deck while the Kraken rocks the ship.
struct MapEvent gShipRooms_RowingDeckEventsKrakenAboard[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN },
    { MAP_EVENT_TALK, LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
    { MAP_EVENT_TALK, LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
    { MAP_EVENT_TALK, LOCALID_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHeadGuideAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToSecondGuideAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToSweatingPassengerAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToLadyAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToStaredAtPassengerAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToWarriorAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToTicketGrumblerAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToMuscleAdmirerAtOars) },
    { MAP_EVENT_TALK, LOCALID_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadHeadGuideMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadSecondGuideMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadSweatingPassengerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadLadyMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadStaredAtPassengerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadWarriorMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadTicketGrumblerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadMuscleAdmirerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
    { MAP_EVENTS_END },
};

/// The rowing deck once the ship has reached Tolbi.
struct MapEvent gShipRooms_RowingDeckEventsArrived[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD },
    { MAP_EVENT_TALK, LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWillBe) },
    { MAP_EVENT_TALK, LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouFolksReallyFoughtWell) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsALotOfOarsmenWereInjured) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAreWeSafeItsAllBecause) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenAnOarsmanAllMy) },
    { MAP_EVENT_TALK, LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouGoOnAheadJustWhen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeCantFindAnyoneToReplace) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImJustGladTheKrakenDidnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSureThePaysGoodButIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsaacAndHisPalsWereSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIFeelBetterNowIWas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImInNoRushAnywayThis) },
    { MAP_EVENTS_END },
};
// clang-format on

/// Frames left of the Kraken's current jolt; at 0 another may start.
s32 gShipRooms_ShakeTimer;
