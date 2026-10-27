/// @file
/// Ship rooms (SceneId_ShipRooms), a sheet of rooms drawn as a ship's cabins,
/// all of them below the decks of the Tolbi-bound Ship: one of the ship's three
/// maps on its voyage from Kalay to Tolbi, told in tolbi_ship_deck.c. The map
/// is four rooms, each a camera box of the entrance table, whose
/// entrances are named by room: the passengers' hold
/// (SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK and the others named for
/// it), the cargo hold (SHIP_ROOMS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK), the rowing
/// deck where the oarsmen row (the SHIP_ROOMS_ROWING_DECK entrances and
/// SHIP_ROOMS_FROM_WORLD_MAP) and the captain's quarters (the
/// SHIP_ROOMS_CAPTAINS_ROOM entrances). Three exits lead up to the
/// deck, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK from the hold,
/// SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK from the cargo hold and
/// SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK from the captain's quarters, and
/// SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK and
/// SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD join the hold and the rowing deck.
/// The actor templates and triggers are chosen by the room and the
/// voyage's stage; most of the voyage's events play here, at the
/// entrances from SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF on,
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
/// their own actor templates.

#include "action_script.h"
#include "flags.h"
#include "songs.h"
#include "heap.h"
#include "audio_engine_symbols.h"
#include "macros.h"
#include "save_block.h"
#include "unit/import.h"
#include "unit/scene/ship_rooms.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "types.h"
#include "script_layer.h"
#include "scene_tables.h"
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
#include "scene_ids.h"
#include "inventory.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "scene_header.h"

/// The visible anchor charm has been created this visit.
/// ★ Named from the game: "Anchor Charm" (MsgItemName)
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN FLAG_SCENE_TEMP_00

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
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
/// rows in gShipRooms_HoldActorTemplatesAtSea and
/// gShipRooms_RowingDeckActorTemplates and of each seat's group
/// of flags (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + passenger). The game gives
/// them no names, so each is named by what they say, or by what the guides call
/// themselves.
enum TolbiShipPassenger {
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), the tour's guide.
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  PASSENGER_HEAD_GUIDE,
  /// The tour's other guide, a man carrying a pack drawn as kalay.c's
  /// ACTOR_SECOND_GUIDE (child value 2).
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
/// gShipRooms_CaptainsQuartersActorTemplates and
/// gShipRooms_CaptainsQuartersActorTemplatesWarriors from
/// ACTOR_FIRST_PLACED; the party's rows take their own slots. The captain,
/// Kaja and the crewman have the sprites they have on deck
/// (tolbi_ship_deck.c).
/// ★ Named from the game: "To the captain's quarters, of course."
/// (MsgTolbiShipIfWeArentGoingSet)
enum TolbiShipCaptainsQuartersActorId {
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  /// ★ Named from the game: "Now we can set sail. Right, Captain?"
  /// (MsgTolbiShipItsMyLuckyAnchor)
  ACTOR_QUARTERS_CAPTAIN = ACTOR_FIRST_PLACED,
  /// A bald man with a beard (SPRITE_KAJA).
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  ACTOR_QUARTERS_KAJA,
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who hid
  /// the captain's anchor charm.
  /// ★ Named from the game: "I have to confess that I hid it on top of the
  /// mast." (MsgTolbiShipInteriorsIWaitedTooLongIllGet)
  ACTOR_QUARTERS_CREWMAN,
  /// ★ Named from the game: "You know, my friend Ouranos here is scared of
  /// ships." (MsgTolbiShipIfWeArentGoingSet)
  ACTOR_QUARTERS_OURANOS,
  /// ★ Named from the game: "Well, it's just like Sean said."
  /// (MsgTolbiShipItsMyLuckyAnchor)
  ACTOR_QUARTERS_SEAN,
  /// PASSENGER_SECOND_GUIDE, drawn the same way.
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  ACTOR_QUARTERS_SECOND_GUIDE
};

/// The hold while the ship waits at Kalay and after it reaches Tolbi,
/// numbering the rows of gShipRooms_HoldActorTemplates from
/// ACTOR_FIRST_PLACED: four of the passengers and the second guide, then the
/// warriors bound for Colosso, Ouranos and his friend Sean, the travelling
/// healer and Kaja.
enum TolbiShipHoldActorId {
  /// PASSENGER_WARRIOR.
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  ACTOR_HOLD_WARRIOR = ACTOR_FIRST_PLACED,
  /// PASSENGER_TICKET_GRUMBLER.
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  ACTOR_HOLD_TICKET_GRUMBLER,
  /// PASSENGER_MUSCLE_ADMIRER.
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  ACTOR_HOLD_MUSCLE_ADMIRER,
  /// PASSENGER_OLD_MAN.
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  ACTOR_HOLD_OLD_MAN,
  /// PASSENGER_SECOND_GUIDE.
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  ACTOR_HOLD_SECOND_GUIDE,
  /// ★ Named from the game: "It's not that I'm scared. I just can't stand
  /// water..." (MsgTolbiShipIfWeArentGoingSet)
  ACTOR_HOLD_OURANOS,
  /// ★ Named from the game: "Hey, maybe you can give us a hand, too, Sean!"
  /// (MsgTolbiTolbiShipThenTheyAreDangerous)
  ACTOR_HOLD_SEAN,
  /// A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE) who opens
  /// his Sanctum (ShipRooms_TalkToHealer).
  /// ★ Named from the game: "I'm spreading goodwill wherever I travel."
  /// (MsgTolbiShipImSpreadingGoodwillWhereverTravel)
  ACTOR_HOLD_HEALER,
  /// Calls the first passenger to the oars.
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  ACTOR_HOLD_KAJA
};

/// The hold at sea, numbering the rows of
/// gShipRooms_HoldActorTemplatesAtSea from ACTOR_FIRST_PLACED:
/// the nine passengers in the order of enum TolbiShipPassenger, the healer and
/// Kaja. Each passenger's evidence is its enum TolbiShipPassenger line.
enum TolbiShipSeaHoldActorId {
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  ACTOR_SEA_HOLD_FIRST_PASSENGER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  ACTOR_SEA_HOLD_HEAD_GUIDE = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_HEAD_GUIDE,
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  ACTOR_SEA_HOLD_SECOND_GUIDE = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_SECOND_GUIDE,
  /// ★ Named from the game: "I've been in a cold sweat"
  /// (MsgTolbiShipInteriorsIveBeenInAColdSweat)
  ACTOR_SEA_HOLD_SWEATING_PASSENGER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_SWEATING,
  /// ★ Named from the game: "you're going to ask a lady to row?"
  /// (MsgTolbiShipDontTellGoing)
  ACTOR_SEA_HOLD_LADY = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_LADY,
  /// ★ Named from the game: "When our eyes met, I knew that was it."
  /// (MsgTolbiShipInteriorsOhNoNoWhenOurEyes)
  ACTOR_SEA_HOLD_STARED_AT_PASSENGER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_STARED_AT,
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  ACTOR_SEA_HOLD_WARRIOR = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_WARRIOR,
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  ACTOR_SEA_HOLD_TICKET_GRUMBLER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  ACTOR_SEA_HOLD_MUSCLE_ADMIRER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_MUSCLE_ADMIRER,
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  ACTOR_SEA_HOLD_OLD_MAN = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  /// ★ Named from the game: "The ship's crew is ready for anything, and I'll
  /// be right here for them." (MsgTolbiShipShipsCrewReadyAnything)
  ACTOR_SEA_HOLD_HEALER,
  /// Calls a replacement rower to the oars.
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  ACTOR_SEA_HOLD_KAJA
};

/// The rowing deck, numbering the rows of
/// gShipRooms_RowingDeckActorTemplates from
/// ACTOR_FIRST_PLACED; the monsters of each attack are placed after them.
enum TolbiShipRowingDeckActorId {
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA), one of the crew
  /// by his bandana.
  /// ★ Named from the game: "I've been sailing for many years, and I've never
  /// seen this island!" (MsgTolbiShipInteriorsIveBeenSailingForManyYears)
  ACTOR_ROWING_CREWMAN = ACTOR_FIRST_PLACED,
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA) who keeps the
  /// oarsmen's time.
  /// ★ Named from the game: "Everyone rows in time with this drum."
  /// (MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis)
  ACTOR_ROWING_DRUMMER,
  /// The eight oarsmen, men in red bandanas (SPRITE_MAN_IN_RED_BANDANA), a seat
  /// each, two by two from the north, the west seat
  /// first; the passengers chosen take the first four seats.
  /// ★ Named from the game: "These big muscles are a sign that we're
  /// oarsmen." (MsgTolbiShipInteriorsTheseBigMusclesAreASign)
  ACTOR_ROWING_FIRST_OARSMAN,
  /// The nine passengers in the order of enum TolbiShipPassenger.
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  ACTOR_ROWING_FIRST_PASSENGER = ACTOR_ROWING_FIRST_OARSMAN + 8,
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  ACTOR_ROWING_HEAD_GUIDE = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_HEAD_GUIDE,
  /// ★ Named from the game: "We had enough trouble just getting passengers!"
  /// (MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting)
  ACTOR_ROWING_SECOND_GUIDE = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_SECOND_GUIDE,
  /// ★ Named from the game: "I've been in a cold sweat"
  /// (MsgTolbiShipInteriorsIveBeenInAColdSweat)
  ACTOR_ROWING_SWEATING_PASSENGER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_SWEATING,
  /// ★ Named from the game: "you're going to ask a lady to row?"
  /// (MsgTolbiShipDontTellGoing)
  ACTOR_ROWING_LADY = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_LADY,
  /// ★ Named from the game: "When our eyes met, I knew that was it."
  /// (MsgTolbiShipInteriorsOhNoNoWhenOurEyes)
  ACTOR_ROWING_STARED_AT_PASSENGER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_STARED_AT,
  /// ★ Named from the game: "we'll help out as warriors."
  /// (MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen)
  ACTOR_ROWING_WARRIOR = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_WARRIOR,
  /// ★ Named from the game: "All that money on tickets, and now we have to
  /// row the ship? Madness!" (MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd)
  ACTOR_ROWING_TICKET_GRUMBLER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  /// ★ Named from the game: "Muscles rippling everywhere... looks great!"
  /// (MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf)
  ACTOR_ROWING_MUSCLE_ADMIRER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_MUSCLE_ADMIRER,
  /// ★ Named from the game: "a frail old man like me"
  /// (MsgTolbiShipPeopleAskingFrail)
  ACTOR_ROWING_OLD_MAN = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  /// Kaja: "Our replacement never arrived, but we're shoving off just the
  /// same."
  /// ★ Named from the game: "If you say so, Kaja. We trust your judgment."
  /// (MsgTolbiShipOurReplacementNeverArrivedBut)
  ACTOR_ROWING_KAJA,
  /// The eight oars, one beside each seat, hidden while the oarsmen row.
  /// ★ Named from what the player sees: an oar resting beside an empty seat
  /// (SPRITE_RESTING_OAR) (its render)
  ACTOR_ROWING_FIRST_OAR,
  /// The monsters of each attack (gShipRooms_FirstAttackMonsters and
  /// the two after it).
  /// ★ Named from what the player sees: three jellyfish monsters
  /// (SPRITE_JELLYFISH_MONSTER) among the oarsmen (its render)
  ACTOR_ROWING_FIRST_MONSTER = ACTOR_ROWING_FIRST_OAR + 8
};

/// The step triggers and searched containers below decks whose subjects are
/// more than an exit.
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

/// A speaker: the actor, with the speaker bits for where its message
/// window and portrait open.
#define SPEAKER(actorId, bits) ((bits) | (actorId))

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
static struct Actor *ShipRooms_PlaceAnchorCharm(void);
static void ShipRooms_InstallRowingDeckActors(u32 action);
static void ShipRooms_Say(s32 speaker);
static void ShipRooms_Turn(s32 actorId, u16 facing);
static void ShipRooms_PlaceActor(s32 actorId, s32 x, s32 z, s32 facing);
static void ShipRooms_MoveCameraAndWait(s32 x, s32 y, s32 z, u32 flags);
static void ShipRooms_RunEventCommand(s32 step, u32 arg, u32 opt);
static s32 ShipRooms_FindFirstSetFlag(u32 seat, s32 alternate);
static void ShipRooms_HideSelectedPassengers(void);
static void ShipRooms_HideSelectedPassenger(s32 actorId, s32 firstFlag);
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

/// An action-script call: stands the actor still at the origin, with no
/// velocity and no target. Returns 0.
static s32 ShipRooms_ResetActor(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SelectAnimation(actor, 1);
  actor->x.fixed = 0;
  actor->y.fixed = 0;
  actor->z.fixed = 0;
  SET_VELOCITY(actor, 0, 0, 0);
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetX = ACTOR_NO_TARGET;
  return 0;
}

/// Bobs the actor between the floor and 8 pixels up by a random amount
/// each frame. Returns 1.
static s32 ShipRooms_Bob(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s16 verticalMotionDirection;
  s32 descendingY;
  s32 ascendingY;

  if ((s16)actor->userValue != 0) {
    descendingY = (actor->y.fixed - ((u32)(Import_RandomNext() << 15) >> 16)) -
                  PIXELS(1) / 2;
    actor->y.fixed = descendingY;
    if (descendingY < 0) {
      verticalMotionDirection = 0;
      actor->userValue = verticalMotionDirection;
    }
  } else {
    ascendingY = actor->y.fixed + ((u32)(Import_RandomNext() << 15) >> 16) +
                 PIXELS(1) / 2;
    actor->y.fixed = ascendingY;
    if (ascendingY > PIXELS(8)) {
      verticalMotionDirection = 1;
      actor->userValue = verticalMotionDirection;
    }
  }
  return 1;
}

/// Now and then (two chances in 64) turns the actor to face
/// south-southeast or south-southwest. Returns 1.
static s32 ShipRooms_LookAround(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  u32 choice = (u32)(Import_RandomNext(actor) << 6) >> 16;

  if (choice == 6) {
    s32 facing = FACING_SOUTHEAST + FACING_STEP;
    actor->facing = facing;
  } else if (choice == 9) {
    s32 facing = FACING_SOUTH + FACING_STEP;
    actor->facing = facing;
  }
  return 1;
}

/// Counts the frames of a pause in riseCounter and moves the pacing
/// actor to its next step after 80 frames.
static void ShipRooms_WaitThenAdvance(struct Actor *actor) {
  u8 *counter = &actor->riseCounter;
  s32 frame = *counter + 1;

  *counter = frame;
  if ((u8)frame > 80) {
    actor->userValue += 1;
  }
}

/// Counts down the actor's walking time in riseBaseY. Returns 1 once the
/// time is up or the actor has reached its target, else 0.
static s32 ShipRooms_HasArrived(struct Actor *actor) {
  s32 remaining = actor->riseBaseY;

  if (remaining != 0) {
    actor->riseBaseY = remaining - 1;
  } else {
    return 1;
  }
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
      actor->targetZ == actor->targetY) {
    return 1;
  }
  return 0;
}

/// The update of Kaja (ACTOR_QUARTERS_KAJA) while the ship waits off the
/// island: he paces between two spots in the captain's quarters, looking about
/// at each, and signals the captain (ACTOR_QUARTERS_CAPTAIN) through his own
/// riseEnabled at three points of the round, but not while the captain is
/// paused. work holds the step.
static void ShipRooms_PaceCaptainsQuarters(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *captain;

  captain = Actor_GetById(ACTOR_QUARTERS_CAPTAIN);
  switch ((s16)actor->userValue) {
  case 0:
    actor->facing = FACING_NORTHWEST + FACING_STEP;
    actor->userValue++;
    actor->riseCounter = 0;
    break;
  case 2:
    actor->facing = 0;
    actor->userValue++;
    actor->riseCounter = 0;
    break;
  case 4:
    Actor_SelectAnimation(actor, 2);
    FieldActor_SetMoveTarget(actor, PIXELS(468), PIXELS(32), PIXELS(632));
    actor->riseBaseY = 60;
    actor->userValue++;
    break;
  case 5:
    if (ShipRooms_HasArrived(actor) != 0) {
      Actor_SelectAnimation(actor, 1);
      actor->riseCounter = 0;
      if (captain->paused == 0) {
        actor->riseEnabled = 1;
      }
      actor->userValue++;
    }
    break;
  case 7:
    if (captain->paused == 0) {
      Actor_SelectAnimation(actor, 3);
      actor->riseEnabled = 2;
    }
    actor->userValue++;
    actor->riseCounter = 0;
    break;
  case 9:
    Actor_SelectAnimation(actor, 2);
    FieldActor_SetMoveTarget(actor, PIXELS(480), PIXELS(32), PIXELS(600));
    actor->riseBaseY = 60;
    actor->userValue++;
    if (captain->paused == 0) {
      actor->riseEnabled = 3;
    }
    break;
  case 10:
    if (ShipRooms_HasArrived(actor) != 0) {
      Actor_SelectAnimation(actor, 1);
      actor->riseCounter = 0;
      actor->userValue++;
    }
    break;
  case 1:
  case 3:
  case 6:
  case 8:
  case 11:
    ShipRooms_WaitThenAdvance(actor);
    break;
  case 12:
    actor->userValue = 0;
    break;
  }
}

/// The update of the captain, who watches Kaja pace: turns north-northeast at
/// the first signal, shrugs at the second if still turned, and faces east at
/// the third. Does nothing while paused.
static void ShipRooms_WatchPacer(struct Actor *actor) {
  struct Actor *watcher = (struct Actor *)actor;
  struct Actor *pacer;

  pacer = Actor_GetById(ACTOR_QUARTERS_KAJA);
  if (watcher->paused != 0)
    return;
  if (pacer->riseEnabled == 1) {
    watcher->facing = FACING_NORTH + FACING_STEP;
    watcher->riseCounter = 1;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 2) {
    if (watcher->riseCounter != 0)
      Actor_SelectAnimation(watcher, 3);
    watcher->riseCounter = 0;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 3) {
    watcher->facing = 0;
    pacer->riseEnabled = 0;
  }
}

/// An action-script call: lays the actor flat on the floor, drawn
/// underfoot at priority 1 with its parts hidden. Returns 1.
static s32 ShipRooms_LayFlat(struct Actor *interpreter) {
  struct Actor *work = interpreter;
  struct Sprite *sprite = work->sprite;

  work->collisionFlags = 8;
  Actor_SetSpriteFlags(work, 0);
  sprite->image.priority = 1;
  sprite->shadow.priority = 1;
  work->priorityFlags = (work->priorityFlags & ~1) | 2;
  Actor_SetSpriteColor(work, SPRITE_COLOR_HIDDEN);
  return 1;
}

const struct SceneEntrance *ShipRooms_SelectEntrances(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_ShipRooms) {
    return gShipRooms_Entrances;
  }
  return gShipRooms_EntrancesOther;
}

const struct SceneExitRegion *ShipRooms_GetRegions(void) { return 0; }

const u32 *ShipRooms_GetExits(void) { return gShipRooms_Exits; }

/// A byte view keeps the native address arithmetic; direct row-field writes
/// choose different bases and registers. The layout still belongs to the row.
#define TOLBI_SHIP_TALK_FACING(bytes, row)                                     \
  ((bytes)[(row) * sizeof(struct ActorTemplate) +                              \
           offsetof(struct ActorTemplate, talkFacing)])

/// Picks the actor templates for the room entered and the voyage's
/// stage. Some stages change who turns to face the party when talked to
/// (talkFacing) in the shared tables.
const struct ActorTemplate *ShipRooms_SelectActorTemplates(void) {
  u8 *templateBytes;

  switch (gSaveBlock.entrance) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_HoldActorTemplates;
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
        // Update talkFacing in the actor template rows, preserving their
        // write order.
        templateBytes = (u8 *)gShipRooms_HoldActorTemplatesAtSea;
        TOLBI_SHIP_TALK_FACING(templateBytes, 0) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 2) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 4) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 5) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 8) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 7) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 6) = TALK_FACING_PARTY_AND_BACK;
        TOLBI_SHIP_TALK_FACING(templateBytes, 3) = TALK_FACING_KEEP;
      }
      return gShipRooms_HoldActorTemplatesAtSea;
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
        gShipRooms_HoldActorTemplates[0].talkFacing = TALK_FACING_KEEP;
        gShipRooms_HoldActorTemplates[4].talkFacing = TALK_FACING_KEEP;
        gShipRooms_HoldActorTemplates[1].talkFacing = TALK_FACING_KEEP;
        gShipRooms_HoldActorTemplates[3].talkFacing = TALK_FACING_KEEP;
      }
      return gShipRooms_HoldActorTemplates;
    } else {
      return gShipRooms_HoldActorTemplatesEmpty;
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
    return gShipRooms_RowingDeckActorTemplates;
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    templateBytes = (u8 *)gShipRooms_RowingDeckActorTemplates;
    TOLBI_SHIP_TALK_FACING(templateBytes, 0) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 1) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 3) = TALK_FACING_PARTY_AND_BACK;
    TOLBI_SHIP_TALK_FACING(templateBytes, 4) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 5) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 6) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 7) = TALK_FACING_KEEP;
    TOLBI_SHIP_TALK_FACING(templateBytes, 8) = TALK_FACING_PARTY_AND_BACK;
    TOLBI_SHIP_TALK_FACING(templateBytes, 9) = TALK_FACING_KEEP;
    return (const struct ActorTemplate *)templateBytes;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_CaptainsQuartersActorTemplatesArrived;
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
        if (Script_GetFlag(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
          gShipRooms_CaptainsQuartersActorTemplatesWarriors[1].talkFacing =
              TALK_FACING_PARTY_AND_BACK;
        }
        if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
          if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
            gShipRooms_CaptainsQuartersActorTemplatesWarriors[0].talkFacing =
                TALK_FACING_PARTY;
          }
        }
        return gShipRooms_CaptainsQuartersActorTemplatesWarriors;
      }
      return gShipRooms_ActorTemplatesNone;
    } else {
      return gShipRooms_CaptainsQuartersActorTemplates;
    }
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
    return gShipRooms_CaptainsQuartersActorTemplates;
    break;
  default:
    return gShipRooms_ActorTemplatesNone;
    break;
  }
  return (const struct ActorTemplate *)templateBytes;
}

#undef TOLBI_SHIP_TALK_FACING

/// Chooses the room's interactions for the current stage of the voyage.
const struct Trigger *ShipRooms_SelectTriggers(void) {
  s32 entrance = gSaveBlock.entrance;

  switch (entrance) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gShipRooms_HoldTriggersAtIsland;
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      return gShipRooms_HoldTriggersChoosingRowers;
    }
    return gShipRooms_HoldTriggers;
  case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_RowingDeckTriggersArrived;
    }
    return gShipRooms_RowingDeckTriggers;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gShipRooms_CaptainsQuartersTriggersAtIsland;
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_CaptainsQuartersTriggersArrived;
    }
    return gShipRooms_CaptainsQuartersTriggers;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    return gShipRooms_RowingDeckTriggersMonstersAboard;
  case SHIP_ROOMS_FROM_WORLD_MAP:
    return gShipRooms_RowingDeckTriggersKrakenAboard;
  default:
    break;
  }

  return gShipRooms_TriggersOther;
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
    Script_ShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipNowWantSeeCaptainToo);
      Script_OpenMessage(ACTOR_QUARTERS_CREWMAN, 0);
      if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
        ShipRooms_DiscussLaunchingWithoutAnchor();
      } else {
        Actor_StartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
        Script_ShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
        Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP,
                            0);
      }
    } else {
      FieldScript_SetMessageId(MsgTolbiShipButWeCantSendShip);
      Script_ShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
    }
  }
  FieldScript_End();
}

/// Sean, in the captain's quarters, asks whether the party came to ask the
/// captain to set sail.
static void ShipRooms_TalkToSeanInCaptainsQuarters(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgTolbiShipYouCameAskCaptainSet);
  FieldScript_AskYesNoAndShowResponse(ACTOR_QUARTERS_SEAN, 0);
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
    ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
    Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP, 60);
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 4);
    ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  } else if (Script_GetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipWeDontKnowMightHappen);
    Script_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipBadLuckLosingMyLucky);
    Script_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
    if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) == 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND) != 0) {
      gFieldWork->fieldMenu = 1;
    }
  } else {
    FieldScript_SetMessageId(MsgTolbiShipItsTooLateHireMercenaries);
    Script_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  }
  FieldScript_End();
}

/// A passenger in the hold while the ship waits. Once the party has agreed to
/// protect the ship, she worries the proud warriors won't like being told to
/// row, and asks the party.
static void ShipRooms_TalkToWaitingWarrior(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Actor_StartRepeatedMotion(ACTOR_HOLD_WARRIOR, 2);
    FieldScript_SetMessageId(MsgTolbiShipTheseProudWarriorsNotGoing);
    ShipRooms_Say(ACTOR_HOLD_WARRIOR);
    Actor_FaceActor(ACTOR_HOLD_WARRIOR, ACTOR_PARTY_LEADER, 10);
    Script_OpenMessage(ACTOR_HOLD_WARRIOR, 0);
    if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(40);
      ShipRooms_Say(ACTOR_HOLD_WARRIOR);
      ShipRooms_Turn(ACTOR_HOLD_WARRIOR, FACING_SOUTHEAST + FACING_STEP);
      Script_ShowMessage(ACTOR_HOLD_WARRIOR, 0);
    } else {
      gFieldWork->message += 2;
      Script_ShowMessage(ACTOR_HOLD_WARRIOR, 0);
      Actor_FaceDirection(ACTOR_HOLD_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                          0);
    }
  } else {
    FieldScript_SetMessageId(MsgTolbiShipLongerWeSitHereMore);
    Script_ShowMessage(ACTOR_HOLD_WARRIOR, 0);
  }
  FieldScript_End();
}

/// Another waiting passenger, once the party has agreed to protect the ship,
/// asks whether it will row. "Are you going to row?"
static void ShipRooms_TalkToWaitingMuscleAdmirer(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    FieldScript_SetMessageId(MsgTolbiShipYouGoingRow);
    FieldScript_AskYesNoAndShowResponse(ACTOR_HOLD_MUSCLE_ADMIRER, 0);
  } else {
    FieldScript_SetMessageId(MsgTolbiShipTheresWholeBunchReallyMuscular);
    FieldScript_ShowMessageAndWait(ACTOR_HOLD_MUSCLE_ADMIRER, 0);
  }
  FieldScript_End();
}

/// Walks the rower back to the leader and hides its separate actor.
static inline void ShipRooms_RejoinParty(s32 rowerId) {
  struct Actor *leader;

  Import_ActorSetAnimation(rowerId, 2);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(rowerId, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(rowerId);
  Import_ActorSetPosition(rowerId, 0, 0);
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
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_HEAD_GUIDE,
                        MsgTolbiShipLooksLikeYouveBeenChosen,
                        MsgTolbiShipNoooooNotGoing, PASSENGER_HEAD_GUIDE);
}

/// The sweating passenger in the hold under sail.
static void ShipRooms_AskSweatingPassengerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_SWEATING_PASSENGER,
                        MsgTolbiShipHaHaHaChosenOarsman,
                        MsgTolbiShipJokingWantRow, PASSENGER_SWEATING);
}

/// The lady in the hold under sail, who is too seasick to talk once the ship
/// is at anchor.
static void ShipRooms_AskLadyToRow(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    FieldScript_Start();
    FieldEffect_SetAttached(ACTOR_SEA_HOLD_LADY,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    FieldScript_SetMessageId(MsgTolbiShipDontFeelDontTalkMe);
    FieldScript_ShowMessageAndWait(ACTOR_SEA_HOLD_LADY, 0);
    FieldScript_End();
  } else {
    TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_LADY,
                          MsgTolbiShipYouFinallyPickedSomeoneDidnt,
                          MsgTolbiShipDontTellGoing, PASSENGER_LADY);
  }
}

/// The stared-at passenger is asked to row, as the others are. It's written out
/// in full rather than through ShipRooms_AskToRow.
static void ShipRooms_AskStaredAtPassengerToRow(void) {
  s32 rowerId;
  struct Actor *actor;

  FieldScript_Start();
  if (Script_GetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_GetChosenOarsman();
    ShipRooms_StepBesideLeader(rowerId);
    FieldScript_SetMessageId(MsgTolbiShipGivesMeChillsThinkCould);
    ShipRooms_Say(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
    ShipRooms_RejoinParty(rowerId);
  } else {
    Actor_RunRepeatedMotion(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 2);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgTolbiShipOhhhhNooooGoingMakeMe);
    Script_OpenMessage(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 0);
    if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
      ShipRooms_Say(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
      Import_ActorSetAnimation(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 2);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Actor_SetDestination(ACTOR_SEA_HOLD_STARED_AT_PASSENGER,
                             actor->x.part.pixel, actor->z.part.pixel);
      }
      Actor_WaitForMoveById(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
      Actor_SetPosition(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 0, 0);
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
      ShipRooms_Say(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
    }
  }
  FieldScript_End();
}

/// The second guide in the hold under sail, written out in full as well.
static void ShipRooms_AskSecondGuideToRow(void) {
  s32 rowerId;
  struct Actor *actor;

  FieldScript_Start();
  if (Script_GetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_GetChosenOarsman();
    ShipRooms_StepBesideLeader(rowerId);
    FieldScript_SetMessageId(MsgTolbiShipIsaacYouveGotGoodEye);
    ShipRooms_Say(ACTOR_SEA_HOLD_SECOND_GUIDE);
    ShipRooms_RejoinParty(rowerId);
  } else {
    FieldScript_SetMessageId(MsgTolbiShipHaHaHaRowingFeel);
    FieldScript_ShowMessageThenWait(ACTOR_SEA_HOLD_SECOND_GUIDE, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_SEA_HOLD_SECOND_GUIDE, 1);
    Script_OpenMessage(ACTOR_SEA_HOLD_SECOND_GUIDE, 0);
    if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
      ShipRooms_Say(ACTOR_SEA_HOLD_SECOND_GUIDE);
      Import_ActorSetAnimation(ACTOR_SEA_HOLD_SECOND_GUIDE, 2);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Actor_SetDestination(ACTOR_SEA_HOLD_SECOND_GUIDE, actor->x.part.pixel,
                             actor->z.part.pixel);
      }
      Actor_WaitForMoveById(ACTOR_SEA_HOLD_SECOND_GUIDE);
      Actor_SetPosition(ACTOR_SEA_HOLD_SECOND_GUIDE, 0, 0);
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
      ShipRooms_Say(ACTOR_SEA_HOLD_SECOND_GUIDE);
    }
  }
  FieldScript_End();
}

/// The warrior in the hold under sail.
static void ShipRooms_AskWarriorToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_WARRIOR,
                        MsgTolbiShipThatsWhoYouPickedOarsman,
                        MsgTolbiShipNotThinkingMaking, PASSENGER_WARRIOR);
}

/// The ticket grumbler in the hold under sail.
static void ShipRooms_AskTicketGrumblerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(
      ACTOR_SEA_HOLD_TICKET_GRUMBLER, MsgTolbiShipIfYouveChosenOarsmanLets,
      MsgTolbiShipOarsmanGiveBreak, PASSENGER_TICKET_GRUMBLER);
}

/// The muscle admirer in the hold under sail.
static void ShipRooms_AskMuscleAdmirerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_MUSCLE_ADMIRER,
                        MsgTolbiShipYouveBeenChosenAsOarsman,
                        MsgTolbiShipPoorOarsmanFeel, PASSENGER_MUSCLE_ADMIRER);
}

/// The old man in the hold under sail.
static void ShipRooms_AskOldManToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_OLD_MAN, MsgTolbiShipHoHoPersonGoingGet,
                        MsgTolbiShipPeopleAskingFrail, PASSENGER_OLD_MAN);
}

#undef TOLBI_SHIP_ASK_TO_ROW

/// Finds a placed actor within twelve pixels of the requested floor
/// position.
static struct Actor *ShipRooms_FindActorNear(s32 x, s32 z) {
  struct FieldWork *work;
  struct Actor **actor;
  struct Actor *current;
  u32 i;
  s32 actorX;
  s32 actorZ;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  work = gFieldWork;
  i = ACTOR_FIRST_PLACED;
  left = x - 12;
  right = x + 12;
  top = z - 12;
  bottom = z + 12;
  actor = &work->actors[ACTOR_FIRST_PLACED];
  while (i <= 65) {
    current = *actor++;
    actorX = current->x.part.pixel;
    actorZ = current->z.part.pixel;
    if (left < actorX && right > actorX && top < actorZ && bottom > actorZ)
      return current;
    i++;
  }
  return 0;
}

/// The chosen oarsman steps out beside the leader, on the leader's right if
/// that spot is free, else on the left, else behind, and turns the way the
/// leader faces.
static void ShipRooms_StepBesideLeader(s32 passengerId) {
  struct Actor *leader;
  struct Actor *leaderPosition;
  s32 blocked;
  s32 direction;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  blocked = 1;
  Actor_SetSpritePriority(passengerId, 2);
  {
    struct Actor *passenger = Actor_GetById(passengerId);

    passenger->priorityFlags =
        ACTOR_PRIORITY_AUTOMATIC | passenger->priorityFlags;
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
  leaderPosition = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leaderPosition != 0) {
    Import_ActorSetPosition(passengerId, leaderPosition->x.fixed,
                            leaderPosition->z.fixed);
  }
  Actor_SetSpeed(passengerId, PIXELS(8) / 5, PIXELS(4) / 5);
  Import_ActorSetAnimation(passengerId, 2);
  {
    s32 offset = gShipRooms_TurnSteps[direction];

    Actor_OffsetPositionAndResetMotion(passengerId, offset >> 16,
                                       (offset << 16) >> 16);
  }
  Actor_WaitForMoveById(passengerId);
  Import_ActorSetAnimation(passengerId, 1);
  ShipRooms_Turn(passengerId, leader->facing);
}

enum {
  KAJA_ROWING_SPEAKER =
      SPEAKER(ACTOR_ROWING_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT)
};

/// Returns 1 when the spot next to the leader in the given sixteenth of a turn
/// holds no actor and no wall, else 0.
static s32 ShipRooms_IsSpotFree(s32 bucket) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  u32 offset = gShipRooms_TurnSteps[bucket];
  s32 x = actor->x.part.pixel + ((s32)offset >> 16);
  s32 z = actor->z.part.pixel + (s16)offset;

  if (ShipRooms_FindActorNear(x, z) != 0) {
    return 0;
  }

  {
    struct FieldPosition point;
    point.x = x << 16;
    point.y = actor->y.fixed;
    point.z = z << 16;

    if (FieldActor_CheckMovementCollision(actor, &point) != 0) {
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
#define TOLBI_SHIP_ROWER_LINE_BY_ROUND(passenger, actorId, first, second,      \
                                       later)                                  \
  FieldScript_Start();                                                         \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))           \
    FieldScript_SetMessageId(first);                                           \
  else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger)))     \
    FieldScript_SetMessageId(second);                                          \
  else                                                                         \
    FieldScript_SetMessageId(later);                                           \
  FieldScript_ShowMessageAndWait(actorId, 0);                                  \
  FieldScript_End()

/// The same with one line for the first round and one for any later.
#define TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(passenger, actorId, first, later) \
  FieldScript_Start();                                                         \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))           \
    FieldScript_SetMessageId(first);                                           \
  else                                                                         \
    FieldScript_SetMessageId(later);                                           \
  FieldScript_ShowMessageAndWait(actorId, 0);                                  \
  FieldScript_End()

/// The head guide at the oars: Kaja's men are worried, monsters are
/// everywhere, or the ship is starting to list.
static void ShipRooms_TalkToHeadGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_HEAD_GUIDE, ACTOR_ROWING_HEAD_GUIDE,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The second guide at the oars, as the head guide.
static void ShipRooms_TalkToSecondGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SECOND_GUIDE,
                                 ACTOR_ROWING_SECOND_GUIDE,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The sweating passenger at the oars, as the head guide.
static void ShipRooms_TalkToSweatingPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SWEATING,
                                 ACTOR_ROWING_SWEATING_PASSENGER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The lady at the oars asks how many monsters are out there.
static void ShipRooms_TalkToLadyAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, ACTOR_ROWING_LADY, MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// The stared-at passenger at the oars, as the head guide.
static void ShipRooms_TalkToStaredAtPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_STARED_AT,
                                 ACTOR_ROWING_STARED_AT_PASSENGER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The warrior at the oars, as the head guide.
static void ShipRooms_TalkToWarriorAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_WARRIOR, ACTOR_ROWING_WARRIOR,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The ticket grumbler at the oars, as the head guide.
static void ShipRooms_TalkToTicketGrumblerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_TICKET_GRUMBLER,
                                 ACTOR_ROWING_TICKET_GRUMBLER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The muscle admirer at the oars, as the lady.
static void ShipRooms_TalkToMuscleAdmirerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, ACTOR_ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// Mind Read on the head guide at the oars. "Just hurry up and slay that
/// beast!"
static void ShipRooms_ReadHeadGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_HEAD_GUIDE, ACTOR_ROWING_HEAD_GUIDE,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the second guide at the oars, as on the head guide.
static void ShipRooms_ReadSecondGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_SECOND_GUIDE, ACTOR_ROWING_SECOND_GUIDE,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the sweating passenger at the oars, as on the head guide.
static void ShipRooms_ReadSweatingPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_SWEATING, ACTOR_ROWING_SWEATING_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the lady at the oars: there are so many monsters at sea.
static void ShipRooms_ReadLadyMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, ACTOR_ROWING_LADY, MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

/// Mind Read on the stared-at passenger at the oars, as on the head guide.
static void ShipRooms_ReadStaredAtPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_STARED_AT, ACTOR_ROWING_STARED_AT_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the warrior at the oars, as on the head guide.
static void ShipRooms_ReadWarriorMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_WARRIOR, ACTOR_ROWING_WARRIOR,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the ticket grumbler at the oars, as on the head guide.
static void ShipRooms_ReadTicketGrumblerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_TICKET_GRUMBLER, ACTOR_ROWING_TICKET_GRUMBLER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the muscle admirer at the oars, as on the lady.
static void ShipRooms_ReadMuscleAdmirerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, ACTOR_ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

#undef TOLBI_SHIP_ROWER_LINE_BY_ROUND
#undef TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER

/// Asks a passenger to row. On yes they walk to the leader and leave with them,
/// and the seat's flag for this passenger (seatFlag) and
/// FLAG_TOLBI_SHIP_ROWER_CHOSEN are set; on no the next message refuses.
static void ShipRooms_AskToRow(s32 speaker, s32 question, s32 seatFlag) {
  struct Actor *leader;

  FieldScript_Start();
  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtActor(speaker, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    ShipRooms_Say(speaker);
    Import_ActorSetAnimation(speaker, 2);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(speaker, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(speaker);
    Import_ActorSetPosition(speaker, 0, 0);
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
/// stage. Under sail, until the ship is docked, he is ACTOR_SEA_HOLD_HEALER,
/// else ACTOR_HOLD_HEALER.
static void ShipRooms_TalkToHealer(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if ((u16)(leader->facing - 0x2000) > 0xc000) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Sanctum_Run(ACTOR_SEA_HOLD_HEALER);
    } else {
      Sanctum_Run(ACTOR_HOLD_HEALER);
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
      FieldScript_ShowMessageAndWait(ACTOR_SEA_HOLD_HEALER, 0);
    } else {
      FieldScript_ShowMessageAndWait(ACTOR_HOLD_HEALER, 0);
    }
    FieldScript_End();
  }
}

/// Talking to one of the monsters on the rowing deck
/// (ACTOR_ROWING_FIRST_MONSTER and the two after it) starts the battle with
/// them: a slot of the encounter zone of the Tolbi ship's scripted battles, and
/// the entrance the party comes back to, chosen by how far the voyage
/// has gone.
static void ShipRooms_FightRowingDeckMonsters(void) {

  FieldScript_Start();
  Field_EndBusyAndReveal();
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) == 0) {
    FieldScript_SetNextDestination(
        (s32)&SceneId_ShipRooms,
        SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) == 0) {
      FieldScript_SetNextDestination(
          (s32)&SceneId_ShipRooms,
          SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE);
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 1);
    } else {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) == 0) {
        FieldScript_SetNextDestination(
            (s32)&SceneId_ShipRooms,
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
    Message_SetVoiceFromActor(ACTOR_ROWING_CREWMAN);
    FieldMessage_ShowCentered(MsgTolbiShipWhereGoingSlay, 1, 8);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
    ShipRooms_Turn(ACTOR_PARTY_LEADER, FACING_SOUTH);
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

  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);

  Actor_ArmCallback(ACTOR_QUARTERS_CAPTAIN, FACING_EAST, 20);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_WEST);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);

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
    Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
    FieldScript_SetMessageId(MsgTolbiShipIfWeArentGoingSet);
    ShipRooms_Say(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
    ShipRooms_Turn(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP);
    guideSpeaker = SPEAKER(ACTOR_HOLD_SECOND_GUIDE, SPEAKER_WINDOW_BELOW);
    Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
    ShipRooms_Say(guideSpeaker);
    Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
    Script_ShowMessageAndWait(
        SPEAKER(ACTOR_HOLD_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT),
        0, 20);
    ShipRooms_Turn(ACTOR_HOLD_SECOND_GUIDE, 0);
    seanSpeaker =
        SPEAKER(ACTOR_HOLD_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT);
    Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    40);
    Actor_ShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SEAN, 3);
    ShipRooms_Say(seanSpeaker);
    Actor_SetAttachedEffect(ACTOR_HOLD_SECOND_GUIDE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 3);
    ShipRooms_Say(guideSpeaker);
    Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(ACTOR_HOLD_SEAN, FACING_NORTHWEST + FACING_STEP);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP);
    Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    30);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 1);
    ShipRooms_Say(guideSpeaker);
    Actor_SetAnimationAndWait(ACTOR_HOLD_OURANOS, 4);
    ShipRooms_Say(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
    Actor_StartRepeatedMotion(ACTOR_HOLD_OURANOS, 2);
    ShipRooms_Say(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
    Import_ActorSetAnimation(ACTOR_HOLD_SECOND_GUIDE, 4);
    ShipRooms_Say(guideSpeaker);
    Actor_SetAnimationAndWait(ACTOR_HOLD_SEAN, 4);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(ACTOR_HOLD_SEAN, FACING_WEST);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SEAN, 2);
    Script_ShowMessageAndWait(seanSpeaker, 0, 20);
    Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST, 0);
    Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Script_ShowMessageAndWait(guideSpeaker, 0, 20);
    Actor_ShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_ShowEmote(ACTOR_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SEAN, 2);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(ACTOR_HOLD_SEAN, FACING_NORTHWEST + FACING_STEP);
    Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
    ShipRooms_Say(seanSpeaker);
    ShipRooms_Turn(ACTOR_HOLD_OURANOS, FACING_SOUTHEAST + FACING_STEP);
    Actor_ShowEmote(ACTOR_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    60);
    Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
    ShipRooms_Say(ACTOR_HOLD_OURANOS);
    Actor_ShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
    ShipRooms_Say(seanSpeaker);
    Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_HOLD_OURANOS, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST, 0);
    ShipRooms_Turn(ACTOR_HOLD_OURANOS, FACING_SOUTHEAST + FACING_STEP);
    Actor_RunRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
    Script_ShowMessageAndWait(guideSpeaker, 0, 20);
    Actor_FaceDirection(ACTOR_HOLD_SEAN, FACING_SOUTH, 40);
    ShipRooms_Say(seanSpeaker);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
    Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 2);
    FieldScript_WaitFrames(60);
    Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
    ShipRooms_Say(ACTOR_HOLD_OURANOS);
    Actor_SetAnimationAndWait(ACTOR_HOLD_SEAN, 3);
    ShipRooms_Say(seanSpeaker);
    Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_RunRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
    ShipRooms_Say(guideSpeaker);
    Actor_SetAnimationAndWait(ACTOR_HOLD_OURANOS, 3);
    ShipRooms_Say(ACTOR_HOLD_OURANOS);
    Actor_FaceDirection(ACTOR_HOLD_SEAN, FACING_NORTHWEST + FACING_STEP, 40);
    Import_ActorSetAnimation(ACTOR_HOLD_SEAN, 3);
    Actor_SetAnimationAndWait(ACTOR_HOLD_OURANOS, 3);
    Actor_SetSpeed(ACTOR_HOLD_SEAN, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetSpeed(ACTOR_HOLD_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
    action = gShipRooms_LeaveHold;
    Actor_SetScript(ACTOR_HOLD_SEAN, action);
    Actor_SetScript(ACTOR_HOLD_OURANOS, action);
    FieldScript_WaitFrames(20);
    Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_SOUTH, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(12) / 5, PIXELS(6) / 5);
    Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 520);
    FieldScript_WaitFrames(1);
    {
      struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
      actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_Jump(ACTOR_HOLD_SECOND_GUIDE, 4, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
    Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
    ShipRooms_Say(ACTOR_HOLD_SECOND_GUIDE);
    Actor_SetSpeed(ACTOR_HOLD_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetScript(ACTOR_HOLD_SECOND_GUIDE, action);
    FieldScript_WaitFrames(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_RefreshSelectorById(ACTOR_HOLD_SECOND_GUIDE);
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
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  FieldScript_SetMessageId(MsgTolbiShipYoureTryingLaunchShip);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                      20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                  20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Import_ActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_OURANOS, 3);
  FieldScript_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 3);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH + FACING_STEP, 0);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP);
  // Two passengers step aside as the conversation continues.
  Actor_SetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_QUARTERS_SEAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_QUARTERS_SEAN, 444, 668);
  Actor_WalkToAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 472, 668);
  Actor_WaitForMoveById(ACTOR_QUARTERS_SEAN);
  Import_ActorSetAnimation(ACTOR_QUARTERS_SEAN, 1);
  FieldScript_WaitFrames(80);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_QUARTERS_OURANOS, 0,
                            40);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Import_ActorSetAnimation(ACTOR_QUARTERS_OURANOS, 4);
  FieldScript_WaitFrames(20);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Import_ActorSetAnimation(ACTOR_QUARTERS_KAJA, 3);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                      0);
  Actor_SetAttachedEffect(ACTOR_QUARTERS_SECOND_GUIDE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_QUARTERS_SECOND_GUIDE, 2, 20);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, 3);
  seanSpeech = SPEAKER(ACTOR_QUARTERS_SEAN, SPEAKER_PORTRAIT_LEFT);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  ShipRooms_Say(seanSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Import_ActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                  40);
  Actor_Jump(ACTOR_QUARTERS_SECOND_GUIDE, 4, 0);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Import_ActorSetAnimation(ACTOR_QUARTERS_KAJA, 3);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_OURANOS, 0,
                            40);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP, 20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  Script_ShowMessageAndWait(ACTOR_QUARTERS_KAJA, 0, 20);
  Import_ActorSetAnimation(ACTOR_QUARTERS_OURANOS, 3);
  FieldScript_WaitFrames(20);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SEAN, 2);
  ouranosSpeech = SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_WINDOW_ABOVE);
  ShipRooms_Say(seanSpeech);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP, 20);
  ShipRooms_Say(ouranosSpeech);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SEAN, 2);
  ShipRooms_Say(seanSpeech);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_OURANOS, 3);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  ShipRooms_Say(ouranosSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_QUARTERS_KAJA, 3);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Import_ActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH + FACING_STEP, 40);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  FieldScript_WaitFrames(20);
  ShipRooms_Say(seanSpeech);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                      40);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  ShipRooms_Say(ouranosSpeech);
  Import_ActorSetAnimation(ACTOR_QUARTERS_SEAN, 3);
  Script_ShowMessageAndWait(seanSpeech, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Import_ActorSetAnimation(ACTOR_QUARTERS_SEAN, 3);
  ShipRooms_Say(seanSpeech);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 20);
  Script_PlaySong(MUS_SILENCE);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  captainSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_BELOW);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  80);
  ShipRooms_Say(captainSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  0);
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE,
                      FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_QUARTERS_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_QUARTERS_CAPTAIN, 4, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  Script_ShowMessage(captainSpeech, 0);
  Actor_SetSpeed(ACTOR_QUARTERS_CAPTAIN, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 475, 598);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
  Actor_SetSpeed(ACTOR_QUARTERS_KAJA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_QUARTERS_KAJA, 462, 618);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_WINDOW_BELOW));
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ShipRooms_Say(ACTOR_QUARTERS_OURANOS);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  ShipRooms_Say(seanSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  20);
  Actor_Jump(ACTOR_QUARTERS_CAPTAIN, 4, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP, 20);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Script_PlaySong(MUS_DUNGEON);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  60);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 4);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  // Settle the passengers where the next visit to the captain's quarters needs
  // them.
  Actor_WalkToAndWait(ACTOR_QUARTERS_SEAN, 444, 628);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_SCOWL,
                  40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, 4);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, 0);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  ShipRooms_MoveCameraAndWait(PIXELS(464), -1, PIXELS(680),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, 0);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_Jump(ACTOR_QUARTERS_CREWMAN, 4, 40);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_SetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_QUARTERS_SECOND_GUIDE, 438, 659);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP,
                      0);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_QUARTERS_CREWMAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CREWMAN, 488, 686);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                      0);
  Actor_WaitForMoveById(ACTOR_QUARTERS_SECOND_GUIDE);
  Import_ActorSetAnimation(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                      0);
  Scene_PlayMusic();
  GameFlag_Set(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL);
}

/// In the captain's quarters at Kalay, using the Anchor Charm on the captain:
/// he has his lucky anchor back, Sean names the party to guard the oarsmen, and
/// the party agrees to protect the ship
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED).
static void ShipRooms_ReturnLuckyAnchor(void) {
  struct Actor *actor;
  s32 captainSpeech;
  s32 seanSpeech;
  s32 value;
  s32 captainAboveSpeech;
  const s32 *action;

  // Let the captain explain why the anchor matters and ask the party to help.
  FieldScript_Start();
  Field_EndBusyAndReveal();
  Actor_FaceActor(ACTOR_QUARTERS_CAPTAIN, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  captainSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
  FieldScript_SetMessageId(MsgTolbiShipItsMyLuckyAnchor);
  ShipRooms_Say(captainSpeech);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                      20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Script_OpenMessage(captainSpeech, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
    Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    ShipRooms_Say(captainSpeech);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  }
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                  40);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(636), 1);
  Actor_SetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 472, 662);
  ShipRooms_Turn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, 3);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 20);
  Import_ActorSetAnimation(ACTOR_QUARTERS_OURANOS, 3);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 3);
  FieldScript_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_SEAN, 0, 20);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP,
                      20);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ShipRooms_Say(ACTOR_QUARTERS_OURANOS);
  seanSpeech = SPEAKER(ACTOR_QUARTERS_SEAN,
                       SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  Import_ActorSetAnimation(ACTOR_QUARTERS_SEAN, 4);
  ShipRooms_Say(seanSpeech);
  ShipRooms_Turn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SEAN, 3);
  ShipRooms_Say(seanSpeech);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SEAN, 3);
  ShipRooms_Say(seanSpeech);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_OURANOS);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, 0);
  ShipRooms_Say(seanSpeech);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                      0);
  // Bring the rest of the party out beside Isaac for the agreement.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 486, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_GARET, 486, 624);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  actor = Actor_GetById(ACTOR_GARET);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_IVAN, 486, 640);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  actor = Actor_GetById(ACTOR_IVAN);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_MIA, 486, 656);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  ShipRooms_Say(seanSpeech);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  Script_OpenMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 1) {
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
    ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_SEAN, 3);
    ShipRooms_Say(seanSpeech);
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
    Script_ShowMessageAndWait(
        SPEAKER(ACTOR_QUARTERS_KAJA,
                SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT),
        0, 40);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 3;
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
    Script_ShowMessageAndWait(ACTOR_QUARTERS_CAPTAIN, 0, 40);
  }
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  value = 176;
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_SECOND_GUIDE, (value << 8));
  Script_ShowMessageAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  Actor_SetSpeed(ACTOR_QUARTERS_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  captainAboveSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 472, 632);
  ShipRooms_Say(captainAboveSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Import_ActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, 4);
  Script_ShowMessageAndWait(captainAboveSpeech, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, (value << 8));
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_SetSpeed(ACTOR_QUARTERS_CREWMAN, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_Jump(ACTOR_QUARTERS_CREWMAN, 2, 0);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CREWMAN, 462, 674);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, (value << 8));
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, 4);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(captainAboveSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                  40);
  Script_ShowMessageAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 0, 40);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, (value << 8), 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, (value << 8), 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, (value << 8), 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_SEAN, 0, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP);
  Script_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_CAPTAIN, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
  }
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_WEST);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE));
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 0, 0);
  Import_ActorSetAnimation(ACTOR_QUARTERS_SEAN, 3);
  Import_ActorSetAnimation(ACTOR_QUARTERS_OURANOS, 3);
  Import_ActorSetAnimation(ACTOR_QUARTERS_KAJA, 3);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  FieldScript_WaitFrames(20);
  // Send the crew away and merge the party's separate actors again.
  action = gShipRooms_LeaveCaptainsQuarters;
  Actor_SetScript(ACTOR_QUARTERS_CREWMAN, action);
  FieldScript_WaitFrames(4);
  Actor_SetScript(ACTOR_QUARTERS_OURANOS, action);
  FieldScript_WaitFrames(4);
  Actor_SetScript(ACTOR_QUARTERS_SEAN, action);
  FieldScript_WaitFrames(4);
  Actor_SetScript(ACTOR_QUARTERS_KAJA, action);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_IVAN);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_GetById(ACTOR_GARET);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetScript(ACTOR_QUARTERS_SECOND_GUIDE, action);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 456, 648);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, 0);
  PartyInventory_Discard(ITEM_ANCHOR_CHARM);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED);
  FieldScript_End();
}

/// Restores the room or plays the script selected by this entrance.
s32 ShipRooms_ApplyEntryState(void) {
  WaitFrames(1);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gSaveBlock.entrance) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      Actor_ConfigureSpriteColor(ACTOR_SEA_HOLD_SECOND_GUIDE, SPRITE_COLOR_RBG);
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Actor_ConfigureSpriteColor(ACTOR_HOLD_SECOND_GUIDE, SPRITE_COLOR_RBG);
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
    Actor_ConfigureSpriteColor(ACTOR_ROWING_SECOND_GUIDE, SPRITE_COLOR_RBG);
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Actor_ConfigureSpriteColor(ACTOR_QUARTERS_SECOND_GUIDE, SPRITE_COLOR_RBG);
    }
    break;
  }
  switch (gSaveBlock.entrance) {
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
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, gShipRooms_MonsterBob);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, gShipRooms_MonsterBob);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 2, gShipRooms_MonsterBob);
      Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
      Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER + 1,
                                 SPRITE_COLOR_BGR);
      Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER + 2,
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
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, gShipRooms_MonsterBob);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, gShipRooms_MonsterBob);
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
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, gShipRooms_MonsterBob);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, gShipRooms_MonsterBob);
      Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
      Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER + 1,
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
        Task_AddFrame(ShipRooms_ShakeWhileKrakenAboard, TASK_PASS_FRAME);
        Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 5);
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
    Import_ScriptRequestExit(SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK);
    break;
  }
  return 0;
}

/// Restores the captain's quarters's actors after the anchor and
/// departure conversations.
static void ShipRooms_SetUpCaptainsQuarters(void) {
  struct Actor *actor;
  struct Actor *other;
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
    actor = Actor_GetById(ACTOR_QUARTERS_KAJA);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 456, 652, FACING_EAST);
    x = 0x1e0;
    ShipRooms_PlaceActor(ACTOR_QUARTERS_KAJA, x, 600,
                         FACING_NORTHWEST + FACING_STEP);
    Actor_SetSpeed(ACTOR_QUARTERS_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
    actor->userValue = done;
    zero = 0;
    actor->riseEnabled = zero;
    actor->collisionFlags |= 128;
    actor->update = ShipRooms_PaceCaptainsQuarters;
    other = Actor_GetById(ACTOR_QUARTERS_CAPTAIN);
    other->riseCounter = zero;
    other->update = ShipRooms_WatchPacer;
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, x, 666, FACING_NORTHWEST);
    }
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 444, 614,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 456, 648, FACING_EAST);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL)) {
    heading = FACING_NORTHWEST + FACING_STEP;
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 475, 598, FACING_WEST);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_KAJA, 462, 618, heading);
    Actor_GetById(ACTOR_QUARTERS_SEAN)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_GetById(ACTOR_QUARTERS_OURANOS)->facing = heading;
    ShipRooms_PlaceActor(ACTOR_QUARTERS_SECOND_GUIDE, 438, 659,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CREWMAN, 488, 688, heading);
  }
}

/// Restores the passenger hold after recruitment and departure scripts.
static void ShipRooms_SetUpHold(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    Import_ActorSetPosition(ACTOR_HOLD_WARRIOR, 0, 0);
    Import_ActorSetPosition(ACTOR_HOLD_TICKET_GRUMBLER, 0, 0);
    Import_ActorSetPosition(ACTOR_HOLD_MUSCLE_ADMIRER, 0, 0);
    Import_ActorSetPosition(ACTOR_HOLD_OLD_MAN, 0, 0);
    Import_ActorSetPosition(ACTOR_HOLD_SECOND_GUIDE, 0, 0);
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    return;
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_HEAD_GUIDE, 152, 444,
                         FACING_SOUTHEAST + FACING_STEP);
    Actor_SetScript(ACTOR_SEA_HOLD_HEAD_GUIDE, gShipRooms_LookAroundScript);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_SWEATING_PASSENGER, 184, 480,
                         FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 170, 488,
                         FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_WARRIOR, 136, 488,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_MUSCLE_ADMIRER, 120, 480,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_TICKET_GRUMBLER, 184, 526,
                         FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_LADY, 136, 584, FACING_WEST);
    Actor_SetScript(ACTOR_SEA_HOLD_LADY, gShipRooms_Shiver);
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
    Import_ActorSetPosition(ACTOR_HOLD_SECOND_GUIDE, 0, 0);
  }
}

/// Restores the rowing deck and introduces a newly recruited replacement.
static void ShipRooms_SetUpRowingDeck(void) {
  WaitFrames(1);
  ShipRooms_SetOarCollision();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
    ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 412, 222,
                         FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_ROWING_DRUMMER, 458, 161, FACING_WEST);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      Actor_SetPosition(ACTOR_ROWING_CREWMAN, PIXELS(472), PIXELS(152));
      Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 5);
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
              Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
              ShipRooms_IntroduceReplacementOarsman(0);
            } else {
              Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 5);
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
  u32 actorId;

  for (actorId = ACTOR_ROWING_FIRST_OAR; actorId <= ACTOR_ROWING_FIRST_OAR + 7;
       actorId++) {
    struct Actor *actor = Actor_GetById(actorId);
    u32 flags = actor->collisionFlags;
    actor->collisionFlags = flags | 0x08;
  }
}

/// Puts each recruited rower at its seat and hides the original occupant.
static void ShipRooms_PlaceFoundActors(s32 gated) {
  s32 actorId;

  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((actorId = ShipRooms_FindFirstSetFlag(0, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 410, 172, FACING_NORTH + FACING_STEP);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
  }
  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((actorId = ShipRooms_FindFirstSetFlag(1, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 470, 172, FACING_NORTHWEST + FACING_STEP);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  }
  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED)) {
    return;
  }
  if ((actorId = ShipRooms_FindFirstSetFlag(2, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 410, 204, FACING_NORTH + FACING_STEP);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
  }
  if (gated) {
    return;
  }
  if ((actorId = ShipRooms_FindFirstSetFlag(3, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 470, 204, FACING_NORTHWEST + FACING_STEP);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 3, 0, 0);
  }
}

/// On the rowing deck, once the charm is back: Kaja tells the oarsmen the ship
/// sails without its replacement and asks the party to bring a passenger to row
/// whenever an oarsman falls, keeping the teams balanced; leads to the deck's
/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY
/// (FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT).
static void ShipRooms_AskForReplacementOarsmen(void) {
  struct Actor *actor;
  s32 mode;

  // Bring Isaac to the crewman who explains the shortage of rowers.
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(168),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_PlaceActor(ACTOR_ROWING_KAJA, 440, 164, FACING_SOUTH + FACING_STEP);
  ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 428, 190,
                       FACING_NORTH + FACING_STEP);
  ShipRooms_PlaceActor(ACTOR_ROWING_DRUMMER, 452, 190,
                       FACING_NORTHWEST + FACING_STEP);
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 1);
  mode = 128;
  ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 440, 134, FACING_WEST);
  gFieldWork->transition = (mode << 1);
  FieldScript_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 148);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 424, 148);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  FieldScript_SetMessageId(MsgTolbiShipOurReplacementNeverArrivedBut);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_RunRepeatedMotion(ACTOR_ROWING_CREWMAN, 1);
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_NORTH + FACING_STEP);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_GARET, 440, 148);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  actor = Actor_GetById(ACTOR_GARET);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_IVAN, 456, 148);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  actor = Actor_GetById(ACTOR_IVAN);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_MIA, 472, 148);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
  // Let the full party respond to the request for replacement oarsmen.
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 60);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH + FACING_STEP, 20);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_StartRepeatedMotion(ACTOR_ROWING_DRUMMER, 1);
  Actor_ShowEmote(ACTOR_ROWING_DRUMMER, (mode << 1), 40);
  ShipRooms_Say(ACTOR_ROWING_DRUMMER);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_RunRepeatedMotion(ACTOR_ROWING_FIRST_OARSMAN, 1);
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, 3);
  ShipRooms_Say(ACTOR_ROWING_FIRST_OARSMAN);
  Import_ActorSetAnimation(ACTOR_ROWING_CREWMAN, 3);
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 3);
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 1, 3);
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 2, 3);
  Actor_SetAnimationAndWait(ACTOR_ROWING_FIRST_OARSMAN + 3, 3);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 4);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_ShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_StartRepeatedMotion(ACTOR_ROWING_CREWMAN, 1);
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_FaceDirection(ACTOR_ROWING_CREWMAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ROWING_DRUMMER, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Import_ActorSetAnimation(ACTOR_ROWING_KAJA, 3);
  Script_ShowMessageAndWait(ACTOR_ROWING_KAJA, 0, 20);
  Import_ActorSetAnimation(ACTOR_ROWING_CREWMAN, 3);
  Actor_SetAnimationAndWait(ACTOR_ROWING_DRUMMER, 3);
  FieldScript_WaitFrames(40);
  Actor_ShowEmote(ACTOR_ROWING_DRUMMER, (mode << 1), 20);
  ShipRooms_Turn(ACTOR_ROWING_DRUMMER, FACING_NORTHWEST + FACING_STEP);
  ShipRooms_Say(ACTOR_ROWING_DRUMMER);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(ACTOR_ROWING_KAJA, 0, 60);
  Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_NORTHWEST + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 80);
  ShipRooms_Turn(ACTOR_ROWING_CREWMAN, FACING_NORTH + FACING_STEP);
  Actor_StartRepeatedMotion(ACTOR_ROWING_CREWMAN, 2);
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_SetAnimationAndWait(ACTOR_ROWING_DRUMMER, 3);
  Actor_StartRepeatedMotion(ACTOR_ROWING_DRUMMER, 2);
  ShipRooms_Say(ACTOR_ROWING_DRUMMER);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTH + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 158);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 148);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_WEST, 20);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
  Actor_WalkTo(ACTOR_ROWING_KAJA, 440, 134);
  FieldScript_WaitFrames(40);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT);
}

/// Raises the anchor charm and announces the ship's first departure.
static void ShipRooms_CastOff(void) {
  struct Actor *anchorCharm;

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 0, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  FieldScript_WaitFrames(20);
  Actor_SetSpeed(ACTOR_QUARTERS_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 468, 614);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 472, 596);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 20);
  Actor_Jump(ACTOR_QUARTERS_CAPTAIN, 4, 20);
  anchorCharm = ShipRooms_PlaceAnchorCharm();
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_RISING_CHIMES);
  Actor_StartScript(anchorCharm, gShipRooms_AnchorCharmPulse);
  FieldScript_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
  FieldScript_WaitFrames(20);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 466, 624);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  FieldScript_SetMessageId(MsgTolbiShipCastOff);
  Script_ShowMessageAndWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/// Announces departure after a replacement rower has joined the crew.
static void ShipRooms_AnnounceDeparture(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  FieldScript_SetMessageId(MsgTolbiShipWereOff);
  Script_ShowMessageAndWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
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
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_KAJA, PIXELS(150), PIXELS(586));
  ShipRooms_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 0, 0);
  Actor_SetSpeed(ACTOR_HOLD_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_HOLD_KAJA, 168, 578);
  Actor_WalkToAndWait(ACTOR_HOLD_KAJA, 168, 554);
  Actor_FaceDirection(ACTOR_HOLD_KAJA, FACING_WEST, 20);
  Actor_StartRepeatedMotion(ACTOR_HOLD_KAJA, 2);
  FieldScript_SetMessageId(MsgTolbiShipRowThoseOars);
  Script_ShowMessageAndWait(ACTOR_HOLD_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/// Kaja comes into the hold and calls a replacement rower back to the rowing
/// deck.
static void ShipRooms_KajaCallsReplacementToOars(void) {
  FieldScript_Start();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  ShipRooms_HideSelectedPassengers();
  Actor_SetPosition(ACTOR_SEA_HOLD_KAJA, PIXELS(150), PIXELS(586));
  ShipRooms_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 0, 0);
  Actor_SetSpeed(ACTOR_SEA_HOLD_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_HOLD_KAJA, 168, 578);
  Actor_WalkToAndWait(ACTOR_SEA_HOLD_KAJA, 168, 554);
  Actor_FaceDirection(ACTOR_SEA_HOLD_KAJA, FACING_WEST, 20);
  Actor_StartRepeatedMotion(ACTOR_SEA_HOLD_KAJA, 2);
  FieldScript_SetMessageId(MsgTolbiShipRowThoseOars);
  Script_ShowMessageAndWait(ACTOR_SEA_HOLD_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/// Restores the rowing deck for the current set of selected passengers.
static void ShipRooms_ShowRowing(void) {
  FieldScript_Start();
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 5);
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  ShipRooms_InstallRowingDeckActors(0);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveWithinMapBounds(0x1b80000, -1, 0xb00000, 1);
  FieldScript_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 7);
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SE_THUMP);
  FieldScript_WaitFrames(30);
  ShipRooms_InstallRowingDeckActors(16);
  FieldScript_WaitFrames(0x50);
  ShipRooms_InstallRowingDeckActors(0);
  FieldScript_WaitFrames(0x3c);
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 7);
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SE_THUMP);
  FieldScript_WaitFrames(30);
  ShipRooms_InstallRowingDeckActors(16);
  FieldScript_WaitFrames(0x50);
  ShipRooms_InstallRowingDeckActors(0);
  FieldScript_WaitFrames(0x5a);
  Sound_PlaySong(SE_THUMP);
  FieldScript_WaitFrames(30);

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);

  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, 0, 0);

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    Import_ScriptRequestExit(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    Import_ScriptRequestExit(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    Import_ScriptRequestExit(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    Import_ScriptRequestExit(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK);
  } else {
    Import_ScriptRequestExit(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK);
  }
}

/// Asks the crew about the monsters before returning belowdecks.
static void ShipRooms_AskAboutMonsters(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  FieldScript_WaitFrames(10);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  FieldScript_SetMessageId(MsgTolbiShipMonsters);
  Script_ShowMessageAndWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
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
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  FieldScript_SetMessageId(MsgTolbiShipImTurning);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP, 40);
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
                            ACTOR_ROWING_FIRST_OARSMAN + 1,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, 6);
  Actor_SetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, gShipRooms_Shiver);
  entryScript = gShipRooms_MonsterBob;
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 2, entryScript);
  Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER + 2, SPRITE_COLOR_BGR);
  ShipRooms_ReportMonstersBelowdecks();
  FieldScript_End();
}

/// Restores the original rowers after the first monster attack.
static void ShipRooms_EndFirstAttack(void) {
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                            ACTOR_ROWING_FIRST_OARSMAN + 1,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  ShipRooms_ReactToMonsterAttack();
  GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  FieldScript_End();
}

/// Introduces the selected passenger and leads them to the open rowing seat.
static void ShipRooms_IntroduceReplacementOarsman(s32 seat) {
  s32 rowerId;
  struct Actor *actor;
  s32 halfStep;
  s32 leftSeat;
  s32 x;

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 0, 0);
  x = 0x1b0;
  halfStep = PIXELS(1) / 2;
  ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, x, 134, FACING_WEST);
  ShipRooms_PlaceFoundActors(1);
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 406, 134);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 406, 152);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 421, 152);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  FieldScript_WaitFrames(20);
  Actor_LinkPair(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 10);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_FindFirstSetFlag(seat, 0);
    Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
    FieldScript_WaitFrames(20);
    Actor_LinkPair(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 10);
    FieldScript_SetMessageId(MsgTolbiShipBackBroughtOarsman);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    leftSeat = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), halfStep);
    Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, x, 168);
    Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Import_ActorSetPosition(rowerId, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(rowerId, PIXELS(1), halfStep);
    Actor_WalkToAndWait(rowerId, 448, 168);
    Actor_FaceDirection(rowerId, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
    Import_ActorSetAnimation(ACTOR_ROWING_KAJA, 3);
    ShipRooms_Say(ACTOR_ROWING_KAJA);
    Actor_ShowEmote(rowerId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
    Import_ActorSetAnimation(ACTOR_ROWING_KAJA, 3);
    ShipRooms_Say(ACTOR_ROWING_KAJA);
    Actor_SetAnimationAndWait(rowerId, 3);
    if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 0);
      Actor_WalkToAndWait(rowerId, 470, 204);
      ShipRooms_Turn(rowerId, FACING_NORTHWEST + FACING_STEP);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 422, 154);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH + FACING_STEP, 0);
      Actor_WalkToAndWait(rowerId, 410, 204);
      leftSeat = 1;
      ShipRooms_Turn(rowerId, FACING_NORTH + FACING_STEP);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 0);
      Actor_WalkToAndWait(rowerId, 470, 172);
      ShipRooms_Turn(rowerId, FACING_NORTHWEST + FACING_STEP);
    } else {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 422, 154);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH + FACING_STEP, 0);
      Actor_WalkToAndWait(rowerId, 410, 172);
      leftSeat = 1;
      ShipRooms_Turn(rowerId, FACING_NORTH + FACING_STEP);
    }
    Actor_LinkPair(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 20);
    ShipRooms_Say(SPEAKER(ACTOR_ROWING_KAJA, SPEAKER_PORTRAIT_RIGHT));
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
    Actor_SetSpeed(ACTOR_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
    if (leftSeat != 0) {
      Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 428, 164);
      Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 164);
    }
    Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
    Actor_ResetAndSetPositionInMode2(ACTOR_ROWING_KAJA, 0x1b8, 134);
    FieldScript_WaitFrames(40);
    ShipRooms_RunEventCommand(
        SHIP_CLOSE_SCREEN, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
  } else {
    FieldScript_SetMessageId(MsgTolbiShipBack);
    Script_ShowMessageAndWait(KAJA_ROWING_SPEAKER, 0, 40);
    FieldEffect_SpawnAttached(ACTOR_ROWING_KAJA,
                              EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    FieldEffect_SetAttached(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(60);
    Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    FieldEffect_SpawnAttached(ACTOR_ROWING_KAJA,
                              EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 4);
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(20);
    Import_ScriptRequestExit(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
  }
}

/// Creates the visible anchor charm once a visit
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN records it) and retains it for departure
/// scripts.
static struct Actor *ShipRooms_PlaceAnchorCharm(void) {
  s32 set = Script_GetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN);
  struct Actor *actor;
  struct Sprite *sprite;
  u8 *buffer;

  if (set != 0) {
    return gShipRooms_AnchorObject;
  }
  actor = FieldActor_Create(SPRITE_BLANK, 0x1c70000, 0x40000, 0x2200000);
  actor->motionFlags = set;
  actor->retainSpriteCache = 1;
  sprite = actor->sprite;
  sprite->count = set;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  buffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  Icon_DrawItem(ITEM_ANCHOR_CHARM);
  VramCache_Load(sprite->slot, 128, buffer + 0x400);
  FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_DRAWN);
  gShipRooms_AnchorObject = actor;
  return actor;
}

/// Restores each replacement rower or the original occupant of that seat.
static void ShipRooms_InstallRowingDeckActors(u32 action) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(0, 0);
    Import_ActorSetPosition(rowerId, PIXELS(410), PIXELS(172));
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN, action);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(1, 0);
    Import_ActorSetPosition(rowerId, PIXELS(470), PIXELS(172));
    Actor_GetById(rowerId)->scaleX = -Q16_ONE;
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN + 1, action);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(2, 0);
    Import_ActorSetPosition(rowerId, PIXELS(410), PIXELS(204));
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN + 2, action);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    s32 rowerId = ShipRooms_FindFirstSetFlag(3, 0);
    Import_ActorSetPosition(rowerId, PIXELS(470), PIXELS(204));
    Actor_GetById(rowerId)->scaleX = -Q16_ONE;
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId, action);
    Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 3, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN + 3, action);
  }

  ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 4, action);
  ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 5, action);
  ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 6, action);
  ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 7, action);
}

/// Applies the seated pose and rowing action assigned to this rower.
static void ShipRooms_PoseActor(s32 id) {
  switch (id) {
  case ACTOR_ROWING_SECOND_GUIDE:
    Import_ActorSetAnimation(id, 6);
    Actor_SetAnimationSpeedById(id, 8);
    break;
  case ACTOR_ROWING_HEAD_GUIDE:
  case ACTOR_ROWING_SWEATING_PASSENGER:
    Import_ActorSetAnimation(id, 5);
    Actor_SetAnimationSpeedById(id, 16);
    break;
  case ACTOR_ROWING_STARED_AT_PASSENGER:
  case ACTOR_ROWING_WARRIOR:
    Import_ActorSetAnimation(id, 5);
    Actor_SetAnimationSpeedById(id, 20);
    break;
  case ACTOR_ROWING_TICKET_GRUMBLER:
    Import_ActorSetAnimation(id, 10);
    Actor_SetAnimationSpeedById(id, 8);
    break;
  case ACTOR_ROWING_LADY:
  case ACTOR_ROWING_MUSCLE_ADMIRER:
    Import_ActorSetAnimation(id, 5);
    Actor_SetAnimationSpeedById(id, 4);
    break;
  case ACTOR_ROWING_OLD_MAN:
    Import_ActorSetAnimation(id, 9);
    Actor_SetAnimationSpeedById(id, 4);
    break;
  }
}

static void ShipRooms_Say(s32 speaker) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(10);
}

/// Turns an actor to a facing over 10 frames.
static void ShipRooms_Turn(s32 actorId, u16 facing) {
  Actor_ArmCallback(actorId, facing, 10);
}

/// Places an actor at pixel coordinates and sets its initial heading.
static void ShipRooms_PlaceActor(s32 actorId, s32 x, s32 z, s32 facing) {
  Import_ActorSetPosition(actorId, x << 16, z << 16, facing);
  Actor_GetById(actorId)->facing = facing;
}

/// Moves the camera, optionally waits or redraws, then honors the packed delay.
static void ShipRooms_MoveCameraAndWait(s32 x, s32 y, s32 z, u32 flags) {
  u32 selected;

  FieldCamera_MoveWithinMapBounds(x, y, z, ~flags & SHIP_CAMERA_JUMP);
  // BUG: the mask keeps only bits 0, 4, 8 and 12 of the wait, so the callers'
  // waits of 20 and 40 frames become 16 and none; SHIP_CAMERA_JUMP also adds
  // a frame.
#ifdef BUGFIX
  selected = flags & 0xffff;
#else
  selected = flags & 0x1111;
#endif
  if ((flags & SHIP_CAMERA_WAIT_FOR_ARRIVAL) != 0)
    FieldCamera_WaitForArrival();
  if ((flags & SHIP_CAMERA_RECENTER_LAYERS) != 0)
    MapGrid_CenterOnFollowedPoint();
  FieldScript_WaitFrames(selected);
}

/// Reuses the voyage's actor, camera, transition and selection
/// operations. Each command names how it interprets the two arguments below.
static void ShipRooms_RunEventCommand(s32 step, u32 arg, u32 opt) {
  u32 slot;

  switch (step) {
  case SHIP_FACE_PARTY_EAST_AND_WEST:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_ArmCallback(ACTOR_IVAN, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, opt);
    break;
  case SHIP_FACE_PARTY:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, arg, 0);
    Actor_FaceDirection(ACTOR_GARET, arg, 0);
    Actor_FaceDirection(ACTOR_IVAN, arg, 0);
    Actor_FaceDirection(ACTOR_MIA, arg, opt);
    break;
  case SHIP_PLAY_PARTY_ANIMATION:
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    if (arg != 0) {
      Actor_WaitForAnimationChange(3);
    }
    if (opt == 0) {
      break;
    }
    FieldScript_WaitFrames(opt);
    break;
  case SHIP_SHOW_PARTY_EMOTE:
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_IVAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_MIA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(opt);
    break;
  case SHIP_HIDE_FIRST_OARSMEN:
    for (slot = 0; slot < arg; slot++) {
      Actor_SetPosition(slot + ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
    }
    break;
  case SHIP_START_ROWING_ANIMATION: {
    struct Actor *actor;

    actor = Actor_GetById(arg);
    actor->facing = FACING_SOUTH + FACING_STEP;
  }
    Import_ActorSetAnimation(arg, 5);
    Actor_SetAnimationSpeedById(arg, opt);
    break;
  case SHIP_START_MIRRORED_ROWING_ANIMATION: {
    struct Actor *actor;

    actor = Actor_GetById(arg);
    actor->facing = FACING_SOUTH + FACING_STEP;
    actor->scaleX = -Q16_ONE;
  }
    Import_ActorSetAnimation(arg, 5);
    Actor_SetAnimationSpeedById(arg, opt);
    break;
  case SHIP_SET_SELECTED_ROWER_POSE: {
    struct Actor *actor;

    actor = Actor_GetById(arg);
    actor->facing = FACING_SOUTH + FACING_STEP;
  }
    ShipRooms_PoseActor(arg);
    if (opt == 0) {
      Actor_SetAnimationSpeedById(arg, 0);
    }
    break;
  case SHIP_OPEN_SCREEN:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
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
    Import_ScriptRequestExit(arg);
    break;
  case SHIP_SETUP_ROWING_PARTY:
    ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
    ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
    ShipRooms_PlaceFoundActors(0);
    ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 432, 168, FACING_SOUTH);
    ShipRooms_PlaceActor(ACTOR_GARET, 448, 168, FACING_SOUTH);
    ShipRooms_PlaceActor(ACTOR_IVAN, 424, 152, FACING_SOUTH);
    ShipRooms_PlaceActor(ACTOR_MIA, 458, 152, FACING_SOUTH);
    break;
  case SHIP_RESTORE_ROWING_DECK_POSES:
    if (arg != 0) {
      struct Actor *actor;

      Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 3, 1);
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 3);
      actor->facing = FACING_SOUTHEAST + FACING_STEP;
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 3);
      actor->scaleX = Q16_ONE;
    }
    {
      struct Actor *actor;

      Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 4, 1);
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 4);
      actor->facing = FACING_SOUTH + FACING_STEP;
      Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 5, 1);
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 5);
      actor->facing = FACING_SOUTHEAST + FACING_STEP;
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 5);
      actor->scaleX = Q16_ONE;
      Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 6, 1);
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 6);
      actor->facing = FACING_SOUTH + FACING_STEP;
      Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 7, 1);
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 7);
      actor->facing = FACING_SOUTHEAST + FACING_STEP;
      actor = Actor_GetById(ACTOR_ROWING_FIRST_OARSMAN + 7);
      actor->scaleX = Q16_ONE;
    }
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR, PIXELS(410), PIXELS(174));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 1, PIXELS(470), PIXELS(174));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 2, PIXELS(410), PIXELS(206));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 3, PIXELS(470), PIXELS(206));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 4, PIXELS(410), PIXELS(286));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 5, PIXELS(470), PIXELS(286));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 6, PIXELS(410), PIXELS(316));
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 7, PIXELS(470), PIXELS(316));
    WaitFrames(1);
    if (arg != 0) {
      Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 3,
                          FACING_NORTHWEST + FACING_STEP, 0);
    }
    Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 4,
                        FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 5,
                        FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 6,
                        FACING_NORTH + FACING_STEP, 0);
    ShipRooms_Turn(ACTOR_ROWING_FIRST_OARSMAN + 7,
                   FACING_NORTHWEST + FACING_STEP);
    break;
  case SHIP_RESTORE_ROWER_POSE: {
    struct Actor *actor;

    actor = Actor_GetById(arg);
    Import_ActorSetAnimation(arg, 1);
    if (opt != 0) {
      actor->facing = FACING_SOUTHEAST + FACING_STEP;
    } else {
      actor->facing = FACING_SOUTH + FACING_STEP;
    }
    actor->scaleX = Q16_ONE;
  } break;
  case SHIP_HIDE_QUARTERS_CREW:
    Actor_SetPosition(ACTOR_QUARTERS_KAJA, 0, 0);
    Actor_SetPosition(ACTOR_QUARTERS_SEAN, 0, 0);
    Actor_SetPosition(ACTOR_QUARTERS_OURANOS, 0, 0);
    Actor_SetPosition(ACTOR_QUARTERS_SECOND_GUIDE, 0, 0);
    Actor_SetPosition(ACTOR_QUARTERS_CREWMAN, 0, 0);
    break;
  case SHIP_HIDE_OURANOS_AND_SEAN:
    Actor_SetPosition(ACTOR_HOLD_SEAN, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_OURANOS, 0, 0);
    break;
  case SHIP_SETUP_CAPTAINS_QUARTERS:
    ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
    Actor_SetPosition(ACTOR_QUARTERS_KAJA, 0, 0);
    Actor_SetPosition(ACTOR_QUARTERS_CREWMAN, 0, 0);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 444, 614,
                         FACING_NORTH + FACING_STEP);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    if (arg != 0) {
      ShipRooms_PlaceAnchorCharm();
    }
    ShipRooms_MoveCameraAndWait(PIXELS(448), PIXELS(32), PIXELS(624),
                                SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
    if (opt == 0) {
      break;
    }
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    FieldScript_WaitFrames(20);
    break;
  case SHIP_SETUP_RECRUITMENT_DECK:
    Actor_SetPosition(ACTOR_ROWING_CREWMAN, 0, 0);
    Actor_SetPosition(ACTOR_ROWING_DRUMMER, 0, 0);
    Actor_SetPosition(ACTOR_ROWING_KAJA, PIXELS(438), PIXELS(152));
    break;
  case SHIP_HIDE_OARS:
    slot = 0;
    do {
      Actor_SetPosition(slot + ACTOR_ROWING_FIRST_OAR, 0, 0);
      slot++;
    } while (slot <= 7);
    break;
  case SHIP_SETUP_PASSENGER_HOLD:
    ShipRooms_PlaceActor(ACTOR_HOLD_SECOND_GUIDE, 152, 532,
                         FACING_NORTHWEST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_HOLD_WARRIOR, 134, 490,
                         FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_HOLD_TICKET_GRUMBLER, 166, 490,
                         FACING_SOUTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_HOLD_MUSCLE_ADMIRER, 182, 504,
                         FACING_SOUTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_HOLD_OLD_MAN, 118, 504,
                         FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    break;
  case SHIP_SETUP_MONSTER_ENCOUNTER:
    ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 416, 328, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_DRUMMER, 448, 352,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN, 454, 248,
                         FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceActor(arg, 408, 290, FACING_EAST);
    ShipRooms_PlaceActor(opt, 408, 342, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 3, 420, 356,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 4, 408, 304, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 5, 418, 378,
                         FACING_NORTH + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 6, 440, 262,
                         FACING_SOUTHEAST + FACING_STEP);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 7, 448, 378,
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
    Actor_SetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, gShipRooms_Shiver);
    break;
  case SHIP_HIDE_PARTY_FOLLOWERS:
    Actor_ReleaseById(ACTOR_GARET);
    Actor_ReleaseById(ACTOR_IVAN);
    Actor_ReleaseById(ACTOR_MIA);
    break;
  case SHIP_RESET_CAMERA:
    FieldCamera_MoveTo(-1, -1, -1, 0);
    WaitFrames(1);
    if (arg != 0) {
      FieldCamera_GetActor()->motionFlags = 0;
    }
    break;
  case SHIP_PLACE_FOLLOWERS_AND_MONSTERS:
    Event_CallWithLastActiveActorId(gShipRooms_PartyFollowerActorTemplates);
    WaitFrames(1);
    if (arg == 1) {
      Event_CallWithLastActiveActorId(gShipRooms_FirstAttackMonsters);
      WaitFrames(1);
    } else if (arg == 2) {
      Event_CallWithLastActiveActorId(gShipRooms_SecondAttackMonsters);
      WaitFrames(1);
    } else if (arg == 3) {
      Event_CallWithLastActiveActorId(gShipRooms_ThirdAttackMonsters);
      WaitFrames(1);
    }
    break;
  }
}

/// Finds the selected passenger or rowing-deck actor for a seat's flag
/// group.
static s32 ShipRooms_FindFirstSetFlag(u32 seat, s32 alternate) {
  s32 flag = 0;
  s32 id = ACTOR_SEA_HOLD_FIRST_PASSENGER;
  u32 i;

  if (alternate == 0) {
    id = ACTOR_ROWING_FIRST_PASSENGER;
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
  ShipRooms_HideSelectedPassenger(ACTOR_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS);
  ShipRooms_HideSelectedPassenger(ACTOR_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  ShipRooms_HideSelectedPassenger(ACTOR_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS);
  ShipRooms_HideSelectedPassenger(ACTOR_SEA_HOLD_FIRST_PASSENGER,
                                  FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
}

/// Hides the passenger whose selection flag is set in this seat's flag group.
static void ShipRooms_HideSelectedPassenger(s32 actorId, s32 firstFlag) {
  unsigned int i = 0;

  do {
    if (GameFlag_Get(firstFlag) != 0) {
      Import_ActorSetPosition(actorId, 0, 0);
      break;
    }
    i++;
    actorId++;
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
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, 6);
  rowerScript = gShipRooms_Shiver;
  Actor_SetScript(firstRower, rowerScript);
  Actor_ReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 1);
  Actor_SetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, rowerScript);
  entryScript = gShipRooms_MonsterBob;
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
  ShipRooms_ReportMonstersBelowdecks();
  FieldScript_End();
}

/// Gathers the party after the crew reports monsters belowdecks.
static void ShipRooms_ReportMonstersBelowdecks(void) {
  struct Actor *actor;
  s32 message;

  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(176),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(440), PIXELS(134));
  FieldScript_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 152);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x1b0, 166);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_GARET);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  WaitFrames(1);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(ACTOR_IVAN, 424, 152);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(ACTOR_GARET, 448, 168);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_MIA, 458, 152);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 40);
  Message_SetVoiceFromActor(ACTOR_ROWING_FIRST_OARSMAN);
  message = MsgTolbiShipMonstersBelowdecks;
  FieldMessage_ShowCentered(message, 1, 10);
  FieldScript_WaitFrames(10);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(320),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_CREWMAN, 2);
  ShipRooms_Turn(ACTOR_ROWING_CREWMAN, FACING_NORTH + FACING_STEP);
  FieldScript_SetMessageId(message + 1);
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_FaceDirection(ACTOR_ROWING_CREWMAN, FACING_EAST, 20);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(134),
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL);
  Actor_SetScript(ACTOR_GARET, gShipRooms_FollowersRejoinByStairs);
  Actor_SetScript(ACTOR_IVAN, gShipRooms_FollowersRejoinByStairs);
  Actor_RunScriptAndWait(ACTOR_MIA, gShipRooms_FollowersRejoinByStairs);
  FieldScript_WaitFrames(40);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  ShipRooms_RunEventCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
}

/// The crew checks on the party after the monster fight, then resumes rowing.
static void ShipRooms_ReactToMonsterAttack(void) {
  const s32 *action;

  ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 444, 300, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_GARET, 458, 310, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_IVAN, 444, 330, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_MIA, 432, 310, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_ROWING_KAJA, 440, 134, FACING_WEST);
  ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN, 454, 248,
                       FACING_SOUTHEAST + FACING_STEP);
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, 6);
  ShipRooms_MoveCameraAndWait(PIXELS(440), -1, PIXELS(308),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  FieldScript_SetMessageId(MsgTolbiShipHeyAreYouOk);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTH, 0);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 152);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 424, 164);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 424, 222);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 424, 262);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 60);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTHEAST, 60);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 40);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Import_ActorSetAnimation(ACTOR_ROWING_KAJA, 2);
  Actor_MoveToAndWait(ACTOR_ROWING_KAJA, 0x1b0, 0x10c);
  Actor_MoveToAndWait(ACTOR_ROWING_KAJA, 0x1c4, 0x10c);
  Import_ActorSetAnimation(ACTOR_ROWING_KAJA, 1);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_NORTH + FACING_STEP);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  Script_ShowMessageAndWait(ACTOR_ROWING_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 4);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(ACTOR_ROWING_KAJA, 0, 80);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  FieldScript_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  FieldScript_WaitFrames(10);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTH + FACING_STEP);
  Script_OpenMessage(ACTOR_ROWING_KAJA, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
    ShipRooms_Say(ACTOR_ROWING_KAJA);
  } else {
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 4);
    gFieldWork->message += 1;
    ShipRooms_Say(ACTOR_ROWING_KAJA);
    ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 40);
    Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
    Import_ActorSetAnimation(ACTOR_ROWING_KAJA, 3);
    ShipRooms_Say(ACTOR_ROWING_KAJA);
  }
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  action = gShipRooms_FollowersRejoinMidDeck;
  Actor_SetScript(ACTOR_GARET, action);
  Actor_SetScript(ACTOR_IVAN, action);
  Actor_RunScriptAndWait(ACTOR_MIA, action);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 424, 272);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 424, 164);
  FieldScript_WaitFrames(60);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Import_ScriptRequestExit(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
}

/// Restores one replacement rower after the next monster attack.
static void ShipRooms_EndSecondAttack(void) {
  s32 handle = ShipRooms_FindFirstSetFlag(0, 0);

  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  ShipRooms_PlaceFoundActors(0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
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
  Import_ActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, 6);
  rowerScript = gShipRooms_Shiver;
  Actor_SetScript(firstRower, rowerScript);
  Actor_ReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 1);
  Actor_SetScript(secondRower, rowerScript);
  Actor_ReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 2);
  entryScript = gShipRooms_MonsterBob;
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
  Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
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
  ShipRooms_PlaceFoundActors(0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle, other);
  Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  Import_ActorSetPosition(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
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
  struct Actor *actor;
  const s32 *action;

  firstRower = ShipRooms_FindFirstSetFlag(0, 0);
  secondRower = ShipRooms_FindFirstSetFlag(1, 0);
  thirdRower = ShipRooms_FindFirstSetFlag(2, 0);
  // Restore the occupied rowing seats before showing the ship's movement.
  FieldScript_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  Actor_SetPosition(ACTOR_ROWING_CREWMAN, PIXELS(472), PIXELS(152));
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 5);
  Actor_SetPosition(ACTOR_ROWING_KAJA, PIXELS(440), PIXELS(134));
  Actor_SetSpriteColorById(ACTOR_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
  actor = Actor_GetById(ACTOR_ROWING_KAJA);
  Actor_SetSpriteFlags(actor, 0);
  ShipRooms_InstallRowingDeckActors(16);
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
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ShipRooms_Turn(ACTOR_MIA, FACING_SOUTHWEST);
  // The party notices the rocking, then turns to the crew for an explanation.
  FieldScript_SetMessageId(MsgTolbiShipWonderWhatsWrongShipShouldnt);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 40);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ShipRooms_Turn(ACTOR_IVAN, FACING_SOUTHEAST);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  ShipRooms_Say(ACTOR_IVAN);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  FieldScript_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetSpriteColorById(ACTOR_ROWING_KAJA, SPRITE_COLOR_NORMAL);
  actor = Actor_GetById(ACTOR_ROWING_KAJA);
  Actor_SetSpriteFlags(actor, 1);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 430, 134);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_StartRepeatedMotion(firstRower, 1);
  Actor_StartRepeatedMotion(secondRower, 1);
  Actor_StartRepeatedMotion(thirdRower, 1);
  Actor_RunRepeatedMotion(ACTOR_ROWING_FIRST_OARSMAN + 3, 1);
  Actor_SetAttachedEffect(firstRower,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(secondRower,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(thirdRower,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_ROWING_FIRST_OARSMAN + 3,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWING_DECK_POSES, 1, 0);
  Actor_FaceDirection(firstRower, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(secondRower, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  Script_ShowMessage(ACTOR_ROWING_KAJA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 440, 134);
  Actor_SetPosition(ACTOR_ROWING_KAJA, 0, 0);
  ShipRooms_Turn(ACTOR_GARET, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  ShipRooms_Say(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  ShipRooms_Turn(ACTOR_MIA, FACING_WEST);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  action = gShipRooms_FollowersRejoinBySeats;
  Actor_SetScript(ACTOR_GARET, action);
  Actor_SetScript(ACTOR_IVAN, action);
  Actor_RunScriptAndWait(ACTOR_MIA, action);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  gShipRooms_ShakeTimer = 0;
  Task_AddFrame(ShipRooms_ShakeWhileKrakenAboard, TASK_PASS_FRAME);
  ShipRooms_RunEventCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  Actor_ReleaseById(ACTOR_ROWING_KAJA);
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
  ShipRooms_PlaceActor(ACTOR_QUARTERS_KAJA, 468, 616, FACING_WEST);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, 3);
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
  struct Actor *actor;
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
  Actor_SetPosition(ACTOR_ROWING_CREWMAN, PIXELS(472), PIXELS(152));
  Actor_SetPosition(ACTOR_ROWING_KAJA, PIXELS(440), PIXELS(134));
  Actor_ConfigureSpriteColor(ACTOR_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
  actor = Actor_GetById(ACTOR_ROWING_KAJA);
  Actor_SetSpriteFlags(actor, 0);
  ShipRooms_InstallRowingDeckActors(16);
  Import_ActorSetAnimation(ACTOR_ROWING_DRUMMER, 5);
  ShipRooms_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174),
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  Actor_ConfigureSpriteColor(ACTOR_ROWING_KAJA, SPRITE_COLOR_NORMAL);
  actor = Actor_GetById(ACTOR_ROWING_KAJA);
  Actor_SetSpriteFlags(actor, 1);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 142);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  FieldScript_SetMessageId(MsgTolbiShipReachedIslandRowers);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  FieldScript_WaitFrames(120);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, fourthRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWING_DECK_POSES, 0, 0);
  Actor_FaceDirection(firstRower, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(secondRower, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(fourthRower, FACING_NORTHWEST + FACING_STEP, 60);
  // The first rower's identity selects the response to the unexpected stop.
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_OLD_MAN)) {
    reaction = 2;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                          PASSENGER_MUSCLE_ADMIRER) ||
             GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_LADY)) {
    reaction = 1;
  }
  Actor_RunRepeatedMotion(firstRower, 1);
  if (reaction == 1) {
    gFieldWork->message += 1;
  } else if (reaction == 2) {
    gFieldWork->message += 2;
  }
  Actor_StartRepeatedMotion(firstRower, 2);
  ShipRooms_Say(firstRower);
  FieldScript_SetMessageId(MsgTolbiShipShipWentOff);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 4);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_ShowEmote(firstRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(secondRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(thirdRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldEffect_SpawnAttached(fourthRower, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 444, 132);
  Actor_ReleaseById(ACTOR_ROWING_KAJA);
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
  Actor_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  progressB = gShipRooms_LeaveByStairs;
  Actor_RunScriptAndWait(firstRower, progressB);
  Actor_FaceDirection(secondRower, FACING_SOUTH + FACING_STEP, 0);
  Actor_ArmCallback(thirdRower, FACING_EAST, 0);
  Actor_FaceDirection(fourthRower, FACING_WEST, 40);
  Actor_ArmCallback(secondRower, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(thirdRower, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ArmCallback(fourthRower, FACING_SOUTH + FACING_STEP, 20);
  Actor_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetScript(thirdRower, progressB);
  FieldScript_WaitFrames(40);
  progressC = gShipRooms_StepIntoAisle;
  Actor_RunScriptAndWait(secondRower, progressC);
  Actor_SetScript(secondRower, progressB);
  Actor_RunScriptAndWait(fourthRower, progressC);
  Actor_RunScriptAndWait(fourthRower, progressB);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  progressA = gShipRooms_FollowersRejoinByStairs;
  Actor_SetScript(ACTOR_GARET, progressA);
  Actor_SetScript(ACTOR_IVAN, progressA);
  Actor_RunScriptAndWait(ACTOR_MIA, progressA);
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
  ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 472, 144,
                       FACING_SOUTH + FACING_STEP);
  ShipRooms_PlaceActor(ACTOR_ROWING_KAJA, 408, 142,
                       FACING_SOUTHEAST + FACING_STEP);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  FieldScript_SetMessageId(MsgTolbiShipSorryEveryoneButWeNeed);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_StartRepeatedMotion(firstRower, 2);
  Actor_StartRepeatedMotion(secondRower, 2);
  Actor_StartRepeatedMotion(thirdRower, 2);
  Actor_RunRepeatedMotion(fourthRower, 2);
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(firstRower, FACING_EAST, 0);
  Actor_FaceDirection(secondRower, FACING_WEST, 0);
  Actor_FaceDirection(thirdRower, FACING_EAST, 0);
  Actor_FaceDirection(fourthRower, FACING_WEST, 40);
  Actor_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(firstRower, 470, 172);
  Actor_WalkTo(secondRower, 410, 172);
  Actor_WalkTo(thirdRower, 470, 204);
  Actor_WalkToAndWait(fourthRower, 410, 204);
  Import_ActorSetAnimation(firstRower, 1);
  Import_ActorSetAnimation(secondRower, 1);
  Import_ActorSetAnimation(thirdRower, 1);
  Actor_FaceDirection(secondRower, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(firstRower, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(fourthRower, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Import_ActorSetAnimation(firstRower, 3);
  Import_ActorSetAnimation(secondRower, 3);
  Import_ActorSetAnimation(thirdRower, 3);
  Actor_SetAnimationAndWait(fourthRower, 3);
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Import_ActorSetAnimation(firstRower, 3);
  Import_ActorSetAnimation(secondRower, 3);
  Import_ActorSetAnimation(thirdRower, 3);
  Actor_SetAnimationAndWait(fourthRower, 3);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 0);
  ShipRooms_Turn(ACTOR_PARTY_LEADER, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, 3);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 444, 132);
  Actor_SetPosition(ACTOR_ROWING_KAJA, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  ShipRooms_SwapRowerSelections(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  ShipRooms_SwapRowerSelections(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_AT_ISLAND);
  Import_ScriptRequestExit(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY);
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
    SC_SYNC,         SC_CALL(ShipRooms_ResetActor),
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
    SC_CALL(ShipRooms_ResetActor),
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
    SC_SYNC,         SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// The same, in the middle of the rowing deck.
s32 gShipRooms_FollowersRejoinMidDeck[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(438), 0, PIXELS(300)),
    SC_SYNC,         SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// The same, by the first row of seats.
s32 gShipRooms_FollowersRejoinBySeats[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(432), 0, PIXELS(168)),
    SC_SYNC,         SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// Squashes and stretches the actor over and over: a shiver.
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
    SC_SYNC,         SC_RELEASE,
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
struct Actor *gShipRooms_AnchorObject = 0;

/// The other maps have no entrances.
struct SceneEntrance gShipRooms_EntrancesOther[] = {
    {SCENE_TABLE_END},
};

/// The passengers' hold, the cargo hold, the rowing deck and the captain's
/// quarters, each entrance named for its room.
struct SceneEntrance gShipRooms_Entrances[] = {
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
    {SCENE_TABLE_END},
};

u32 gShipRooms_Exits[] = {
    SCENE_EXITS(ShipRooms),
    SCENE_EXIT(SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_PASSENGER_HOLD),
    SCENE_EXIT(SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK, ShipRooms,
               SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD),
    SCENE_EXIT(SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_CRATE_HOLD),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD, ShipRooms,
               SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK),
    SCENE_EXIT(SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_CAPTAINS_ROOM),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY),
    SCENE_EXIT(SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS,
               ShipRooms,
               SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS),
    SCENE_EXIT(SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, ShipRooms,
               SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK,
               WorldMap, WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK),
    SCENE_EXIT(SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS,
               TolbiShipDeck, TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS),
    SCENE_EXIT(SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD,
               TolbiShipDeck, TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK,
               WorldMap, WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK,
               WorldMap, WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN, WorldMap,
               WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN),
    SCENE_EXIT(SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND, WorldMap,
               WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND),
    SCENE_EXIT(SHIP_ROOMS_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SHIP_ROOMS),
    SCENE_EXITS_END,
};

struct ActorTemplate gShipRooms_ActorTemplatesNone[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gShipRooms_CaptainsQuartersActorTemplates[] = {
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(446), 0, PIXELS(620), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(626), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(454), 0, PIXELS(640), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// The captain's quarters once the warriors have come to see the captain, with the party's
/// followers.
struct ActorTemplate gShipRooms_CaptainsQuartersActorTemplatesWarriors[] = {
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
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct ActorTemplate gShipRooms_CaptainsQuartersActorTemplatesArrived[] = {
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(462), 0, PIXELS(612), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

/// The hold before the party hears of the delay at Kalay.
struct ActorTemplate gShipRooms_HoldActorTemplatesEmpty[] = {
    { 0xf4, CONDITION_ALWAYS, (s32)gShipRooms_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct ActorTemplate gShipRooms_HoldActorTemplates[] = {
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
    { SCENE_TABLE_END },
};

/// The hold at sea, once the passengers row.
struct ActorTemplate gShipRooms_HoldActorTemplatesAtSea[] = {
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
    { SCENE_TABLE_END },
};

/// The rowing deck, in the order of enum TolbiShipRowingDeckActorId: the crewman
/// and the drummer, the oarsmen at the seats, the passengers who may take a
/// seat, Kaja and the eight oars beside the seats.
struct ActorTemplate gShipRooms_RowingDeckActorTemplates[] = {
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
        PIXELS(470), 0, PIXELS(174), FACING_WEST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(206), FACING_EAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(206), FACING_WEST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(286), FACING_EAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(286), FACING_WEST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(410), 0, PIXELS(316), FACING_EAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(470), 0, PIXELS(316), FACING_WEST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

/// The party's followers, placed for an event.
struct ActorTemplate gShipRooms_PartyFollowerActorTemplates[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// The monsters of each attack, beside the seats.
struct ActorTemplate gShipRooms_FirstAttackMonsters[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ActorTemplate gShipRooms_SecondAttackMonsters[] = {
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gShipRooms_ThirdAttackMonsters[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// The cargo hold, and the events.
struct Trigger gShipRooms_TriggersOther[] = {
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_SHIP_ROOMS_CRATE_HOLD_BARREL, FLAG_TOLBI_SHIP_CRATE_HOLD_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct Trigger gShipRooms_HoldTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { TRIGGER_STEP, MB_SHIP_ROOMS_HOLD_WARRIORS, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, EVENT_SCRIPT(ShipRooms_WatchWarriorsStormOff) },
    { TRIGGER_TALK, ACTOR_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToWaitingWarrior) },
    { TRIGGER_TALK, ACTOR_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
    { TRIGGER_TALK, ACTOR_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
    { TRIGGER_TALK, ACTOR_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToWaitingMuscleAdmirer) },
    { TRIGGER_TALK, ACTOR_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
    { TRIGGER_TALK, ACTOR_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
    { TRIGGER_TALK, ACTOR_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
    { TRIGGER_TALK, ACTOR_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_WARRIOR, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMaybeIShouldJustForgetAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheyreJustTakingAdvantageOfUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsDoTheyHonestlyThinkImGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_MUSCLE_ADMIRER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSureItdBeEasierTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsWeLeftTheOldLadyBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellShootMaybeWeShouldHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAskingThePassengersToHelpOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_HEALER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsEvenThoughImOpposedToViolence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_HEALER, FLAG_TOLBI_SHIP_REACHED_TOLBI, MSGNO(MsgTolbiShipInteriorsICantRowButIAm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMadeItToColossoIn) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { TRIGGERS_END },
};

/// The hold at sea: talking to a passenger asks them to row.
struct Trigger gShipRooms_HoldTriggersChoosingRowers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskHeadGuideToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskSweatingPassengerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskLadyToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskStaredAtPassengerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskSecondGuideToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskWarriorToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskTicketGrumblerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskMuscleAdmirerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskOldManToRow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEAD_GUIDE, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouMakeMeNervousJustStanding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SWEATING_PASSENGER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyLooksLikeTheyNeedSomeone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenInAColdSweat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_LADY, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsRowingIsAToughJobIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhThisPoorSapLooksSad) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_STARED_AT_PASSENGER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyThisGuysBeenStaringAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhNoNoWhenOurEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_WARRIOR, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsOhOhNoHesNotComing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsOnlyNaturalThatINot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsPleasePleaseImBeggingJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISnuckByThisTimeBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_MUSCLE_ADMIRER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhatsWithYouGoBotherThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowThisWarriorIsntGonnaLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhyAmIGettingSoWorked) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanJustMoveRightAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SECOND_GUIDE, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCarefulNowWatchOutDontMake) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { TRIGGERS_END },
};

/// The hold while the ship waits off the island.
struct Trigger gShipRooms_HoldTriggersAtIsland[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_AskLadyToRow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { TRIGGERS_END },
};

/// The captain's quarters, from the wait at Kalay until the ship reaches Tolbi. The Anchor
/// Charm is given back to the captain here.
struct Trigger gShipRooms_CaptainsQuartersTriggers[] = {
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { TRIGGER_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToCaptain) },
    { TRIGGER_TALK, ACTOR_QUARTERS_CREWMAN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToCaptainsQuartersCrewman) },
    { TRIGGER_TALK, ACTOR_QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
    { TRIGGER_TALK, ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
    { TRIGGER_TALK, ACTOR_QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
    { TRIGGER_TALK, ACTOR_QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
    { TRIGGER_TALK, ACTOR_QUARTERS_SEAN, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToSeanInCaptainsQuarters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsThisIsAChanceToMake) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheShipWontLeaveDockUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipInteriorsAsLongAsWeHaveThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeKnewTheRisksWhenWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CREWMAN, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsSendingOutAShipWhenIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWaitedTooLongIllGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIfWeDontSetSailNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThisIsRidiculousTheShipCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThatThingCouldntHaveGottenLost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWouldSteal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipDoesntLeaveSoon) },
    { TRIGGER_STEP, MB_SHIP_ROOMS_QUARTERS_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(ShipRooms_WatchCrewWonderAtDelay) },
    { USE_ITEM_ON_ACTOR(ITEM_ANCHOR_CHARM), ACTOR_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, EVENT_SCRIPT(ShipRooms_ReturnLuckyAnchor) },
    { EXAMINE_TRIGGER(EXAMINE_JAR) | TRIGGER_FLAG_FACING | FACING_NORTH, MB_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGERS_END },
};

/// The captain's quarters while the ship waits off the island.
struct Trigger gShipRooms_CaptainsQuartersTriggersAtIsland[] = {
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { TRIGGER_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
    { TRIGGER_TALK, ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), MB_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGERS_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct Trigger gShipRooms_CaptainsQuartersTriggersArrived[] = {
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { TRIGGER_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), MB_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGERS_END },
};

struct Trigger gShipRooms_RowingDeckTriggers[] = {
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD },
    { TRIGGER_TALK, ACTOR_ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsWeCanLeaveAsSoonAs) },
    { TRIGGER_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenSailingForManyYears) },
    { TRIGGER_TALK, ACTOR_ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis) },
    { TRIGGER_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowIHaveABlisterOn) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRowingIsAMansJob) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereTheOnesWhoMakeThe) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOnceWeCastOffYouJust) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanOnlyDoHardWork) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHmmmIWonderWhenWereGoing) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfItsOkayTo) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsICantWaitUntilWeGet) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsStoppingAgainAndAgainLikeThis) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIHearTheresAnIsleIn) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLetsSeeIslandsInTheKaragol) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsFromTheSeaItLooksLike) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenWereRowingWeCanJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEnoughIsEnoughLetsSetOff) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsCrazyToHaveInexperiencedPassengers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsILoveItWhenTheOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItWasHardForMeAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsEveryoneDownHereGetsSoCompetitive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseBigMusclesAreASign) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellSeeToItThatThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouveGotToSaveUpLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIDontFeelLikeRowingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISureHopeAnotherMonsterDoesnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsSpendYourMoneyWhileYoureYoung) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLooksLikeImGoingToHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsItSeemsLikeThereWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCouldThisBeThatIslandThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHaHahIfYouWantTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMaybeItsTimeForMeTo) },
    { TRIGGERS_END },
};

/// The rowing deck while monsters are aboard: talking to one
/// (ACTOR_ROWING_FIRST_MONSTER and the two after it) fights it.
struct Trigger gShipRooms_RowingDeckTriggersMonstersAboard[] = {
    { TRIGGER_STEP, MB_SHIP_ROOMS_ROWING_DECK_STAIRS, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TurnBackFromStairs) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_MONSTER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_FightRowingDeckMonsters) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_MONSTER + 1, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_FightRowingDeckMonsters) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_MONSTER + 2, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_FightRowingDeckMonsters) },
    { TRIGGER_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMonstersFromTheDeepAreAttacking) },
    { TRIGGER_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCurseThoseBeastsTheyGotOne) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingOffTheMonstersIsYour) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFoulBeastIMayBeTrembling) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWerentSoTiredId) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCursesIWantToSmackThat) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhoIsThatThingGoingTo) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfYouDontDoABetter) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeOarsmenMayBeStrongBut) },
    { TRIGGER_TALK, ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSurroundedByDangerOnAll) },
    { TRIGGER_TALK, ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAhhhhSeaMonstersRealLiveSea) },
    { TRIGGER_TALK, ACTOR_ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhaHeyYouThisIsNo) },
    { TRIGGER_TALK, ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAahIHaveToRowThis) },
    { TRIGGER_TALK, ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHHeyIsaacShouldntYouBe) },
    { TRIGGER_TALK, ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYaaahhhIImNotAfraidOf) },
    { TRIGGER_TALK, ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheOarsmenAreInDireStraights) },
    { TRIGGER_TALK, ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIICantDoThisFighting) },
    { TRIGGER_TALK, ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhatHappensNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyArentYouDoingAnythingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfWeLoseAnyMoreOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheSeaMonstersGetUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImScaredThoseSeaMonstersAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingSeaMonstersIsForWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheSeaMonstersAreSoTough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhoElseThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseJustStayHereAndProtect) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeDontEvenKnowIfAttacking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeaMonstersAreTerriblyFerocious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWereAWarriorI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfThisWarriorHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSureInAFixSay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSoOutOfMyWits) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseSeaMonstersLookAwfullyStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseOhPleaseLetMeMake) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyDidSeaMonstersHaveTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIdRatherBeEatenByA) },
    { TRIGGERS_END },
};

/// The rowing deck while the Kraken rocks the ship.
struct Trigger gShipRooms_RowingDeckTriggersKrakenAboard[] = {
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN },
    { TRIGGER_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
    { TRIGGER_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
    { TRIGGER_TALK, ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToHeadGuideAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToSecondGuideAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToSweatingPassengerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToLadyAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToStaredAtPassengerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToWarriorAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToTicketGrumblerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_TalkToMuscleAdmirerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadHeadGuideMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadSecondGuideMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadSweatingPassengerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadLadyMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadStaredAtPassengerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadWarriorMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadTicketGrumblerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(ShipRooms_ReadMuscleAdmirerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
    { TRIGGERS_END },
};

/// The rowing deck once the ship has reached Tolbi.
struct Trigger gShipRooms_RowingDeckTriggersArrived[] = {
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD },
    { TRIGGER_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWillBe) },
    { TRIGGER_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouFolksReallyFoughtWell) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsALotOfOarsmenWereInjured) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAreWeSafeItsAllBecause) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenAnOarsmanAllMy) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouGoOnAheadJustWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeCantFindAnyoneToReplace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImJustGladTheKrakenDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSureThePaysGoodButIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsaacAndHisPalsWereSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIFeelBetterNowIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImInNoRushAnywayThis) },
    { TRIGGERS_END },
};
// clang-format on

/// Frames left of the Kraken's current jolt; at 0 another may start.
s32 gShipRooms_ShakeTimer;
