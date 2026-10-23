/// @file
/// Tolbi-bound Ship (MapId_TolbiShipInteriors), below its decks: one of the
/// ship's three maps on its voyage from Kalay to Tolbi, told in
/// tolbi_ship_deck.c. The map is four rooms, each a camera box of the
/// warp destination table, whose warp destinations are named by room: the
/// passengers' hold (TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK
/// and the others named for it), the cargo hold
/// (TOLBI_SHIP_INTERIORS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK), the rowing deck
/// where the oarsmen row (the TOLBI_SHIP_INTERIORS_ROWING_DECK warp
/// destinations and TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP) and the captain's
/// quarters (the TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM warp destinations). Three
/// warps lead up to the deck,
/// TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK from the hold,
/// TOLBI_SHIP_INTERIORS_CRATE_HOLD_TO_TOLBI_SHIP_DECK from the cargo hold and
/// TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK from the captain's
/// quarters, and TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK and
/// TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD join the hold and the
/// rowing deck. The object event templates and map events are chosen by the
/// room and the voyage's stage; most of the voyage's cutscenes play here, at
/// the warp destinations from
/// TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF on, through
/// the shared operations of TolbiShipInteriors_RunSceneCommand. In story order:
///
/// - At Kalay, while the ship waits for its replacement from Tolbi. Once the
///   tour group has left Kalay (FLAG_KALAY_COLOSSO_TOUR_LEFT), walking into
///   the hold (TOLBI_SHIP_INTERIORS_CELL_HOLD_WARRIORS) has Ouranos, a warrior
///   bound for Colosso who "can't stand water", threaten to go ashore, and he
///   and his friend Sean go to make the captain sail
///   (FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN). The crew's talk of the delay
///   plays here (TOLBI_SHIP_INTERIORS_CELL_QUARTERS_CREW) if it has not on
///   deck. In the captain's quarters the warriors and passengers press the
///   captain, who has just decided to sail
///   (FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL); Ouranos and Sean offer to guard
///   the deck with Kaja, Sean thinks of the young warriors among the
///   passengers, and then the captain finds his lucky anchor charm gone and
///   will not sail without it. Giving him the Anchor Charm, found on the mast,
///   plays TolbiShipInteriors_ReturnLuckyAnchor: Sean names the party to guard
///   the oarsmen, and the party agrees (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED).
/// - Setting sail: on the rowing deck Kaja tells the oarsmen they sail without
///   the replacement (FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT), the party
///   guarding them below while he, Ouranos and Sean guard the deck, and that if
///   an oarsman falls the party must choose a passenger to row in his place,
///   keeping the two teams balanced. After the deck and the mast, the captain
///   casts off in the captain's quarters
///   (TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF,
///   raising the charm), a passenger is called to the oars
///   (TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS) and
///   the rowing deck rows
///   (TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING), which leads
///   to the world map for the next leg.
/// - The attacks, each called by the lookout on the mast or the deck; before
///   the first, the captain in the captain's quarters hears of monsters
///   (TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS) and
///   turns the ship
///   (TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN). Then
///   monsters are belowdecks
///   (TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK and
///   the second and third attacks'). While they are aboard the stairs are
///   barred (FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS) and the leader is turned back
///   from them; talking to a monster on the rowing deck fights it (the first,
///   second or third slot of the Tolbi ship's scripted battles, by the attack),
///   returning to TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE or
///   the second or third attack's, which record the attack fought off
///   (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, _SECOND_ATTACK_FOUGHT_OFF and
///   _THIRD_ATTACK_FOUGHT_OFF) and leave an oarsman hurt. The party then
///   chooses a passenger in the hold to row: the attacks fought off number the
///   seat being filled, and each seat has a group of nine flags, one for each
///   passenger (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS and the rest); the passenger
///   follows the party (FLAG_TOLBI_SHIP_ROWER_CHOSEN) to the rowing deck
///   (TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD), where Kaja takes
///   them on. Each passenger answers being asked in their own way, and their
///   rowing values on the mast decide the course.
/// - The Kraken: TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP, the ship rocking,
///   "there's a huge sea monster on the ship!" (FLAG_TOLBI_SHIP_KRAKEN_ON_DECK
///   shakes the scene until it is defeated on deck).
/// - The island: off course, a short scene in the captain's quarters
///   (TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND) leads by
///   the world map (TOLBI_SHIP_INTERIORS_TO_WORLD_MAP) to
///   TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND, where the rowers
///   rest at the island (FLAG_TOLBI_SHIP_AT_ISLAND) and the hold and captain's
///   quarters have their own talk. Back from it,
///   TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS has
///   Kaja switch the left and right teams, the left team's timing having taken
///   the ship off course, and the ship sets off for Tolbi.
/// - TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT, where a defeat at sea
///   returns: it clears the voyage's flags from
///   FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT to _KRAKEN_DEFEATED and the
///   rowers chosen, sets FLAG_TOLBI_SHIP_DEFEATED_AT_SEA and leads up to the
///   deck (TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK), so the
///   voyage starts over.
///
/// A travelling healer opens a Sanctum in the hold. At Tolbi
/// (FLAG_TOLBI_SHIP_REACHED_TOLBI) the captain's quarters and the hold have
/// their own object event templates.
///
/// Open questions:
/// - None.

#include "action_script.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "macros.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/map/tolbi_ship_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
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
#include "callback_scheduler.h"
#include "map_ids.h"
#include "inventory.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "field_runtime.h"

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
enum ShipSceneCommand {
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
/// rows in gTolbiShipInteriors_HoldObjectEventTemplatesAtSea and
/// gTolbiShipInteriors_RowingDeckObjectEventTemplates and of each seat's group
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
/// gTolbiShipInteriors_CaptainsQuartersObjectEventTemplates and
/// gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesWarriors from
/// LOCALID_FIRST_PLACED; the party's rows take their own slots. The captain,
/// Kaja and the crewman have the sprites they have on deck
/// (tolbi_ship_deck.c).
/// ★ Named from the game: "To the captain's quarters, of course."
/// (MsgTolbiShipIfWeArentGoingSet)
enum TolbiShipCaptainsQuartersLocalId {
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  QUARTERS_CAPTAIN = LOCALID_FIRST_PLACED,
  QUARTERS_KAJA, ///< a bald man with a beard (SPRITE_KAJA)
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who hid
  /// the captain's anchor charm.
  /// ★ Named from the game: "I have to confess that I hid it on top of the
  /// mast." (MsgTolbiShipInteriorsIWaitedTooLongIllGet)
  QUARTERS_CREWMAN,
  QUARTERS_OURANOS,
  QUARTERS_SEAN,
  QUARTERS_SECOND_GUIDE ///< PASSENGER_SECOND_GUIDE, drawn the same way
};

/// The hold while the ship waits at Kalay and after it reaches Tolbi,
/// numbering the rows of gTolbiShipInteriors_HoldObjectEventTemplates from
/// LOCALID_FIRST_PLACED: four of the passengers and the second guide, then the
/// warriors bound for Colosso, Ouranos and his friend Sean (Ouranos "can't
/// stand water"), the travelling healer and Kaja.
enum TolbiShipHoldLocalId {
  HOLD_WARRIOR = LOCALID_FIRST_PLACED, ///< PASSENGER_WARRIOR
  HOLD_TICKET_GRUMBLER,                ///< PASSENGER_TICKET_GRUMBLER
  HOLD_MUSCLE_ADMIRER,                 ///< PASSENGER_MUSCLE_ADMIRER
  HOLD_OLD_MAN,                        ///< PASSENGER_OLD_MAN
  HOLD_SECOND_GUIDE,                   ///< PASSENGER_SECOND_GUIDE
  HOLD_OURANOS,
  HOLD_SEAN,
  HOLD_HEALER, ///< a bearded man in a long robe
               ///< (SPRITE_BEARDED_MAN_IN_LONG_ROBE)
  HOLD_KAJA    ///< calls the first passenger to the oars. "Row those oars!"
};

/// The hold at sea, numbering the rows of
/// gTolbiShipInteriors_HoldObjectEventTemplatesAtSea from LOCALID_FIRST_PLACED:
/// the nine passengers in the order of enum TolbiShipPassenger, the healer and
/// Kaja.
enum TolbiShipSeaHoldLocalId {
  SEA_HOLD_FIRST_PASSENGER = LOCALID_FIRST_PLACED,
  SEA_HOLD_HEAD_GUIDE = SEA_HOLD_FIRST_PASSENGER + PASSENGER_HEAD_GUIDE,
  SEA_HOLD_SECOND_GUIDE = SEA_HOLD_FIRST_PASSENGER + PASSENGER_SECOND_GUIDE,
  SEA_HOLD_SWEATING_PASSENGER = SEA_HOLD_FIRST_PASSENGER + PASSENGER_SWEATING,
  SEA_HOLD_LADY = SEA_HOLD_FIRST_PASSENGER + PASSENGER_LADY,
  SEA_HOLD_STARED_AT_PASSENGER = SEA_HOLD_FIRST_PASSENGER + PASSENGER_STARED_AT,
  SEA_HOLD_WARRIOR = SEA_HOLD_FIRST_PASSENGER + PASSENGER_WARRIOR,
  SEA_HOLD_TICKET_GRUMBLER = SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  SEA_HOLD_MUSCLE_ADMIRER = SEA_HOLD_FIRST_PASSENGER + PASSENGER_MUSCLE_ADMIRER,
  SEA_HOLD_OLD_MAN = SEA_HOLD_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  SEA_HOLD_HEALER,
  SEA_HOLD_KAJA ///< calls a replacement rower to the oars
};

/// The rowing deck, numbering the rows of
/// gTolbiShipInteriors_RowingDeckObjectEventTemplates from
/// LOCALID_FIRST_PLACED; the monsters of each attack are placed after them.
enum TolbiShipRowingDeckLocalId {
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA), one of the crew
  /// by his bandana. "I've been sailing for many years, and I've never seen
  /// this island!"
  ROWING_CREWMAN = LOCALID_FIRST_PLACED,
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA) who keeps the
  /// oarsmen's time.
  /// ★ Named from the game: "Everyone rows in time with this drum."
  /// (MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis)
  ROWING_DRUMMER,
  /// The eight oarsmen, men in red bandanas (SPRITE_MAN_IN_RED_BANDANA), a seat
  /// each, two by two from the north, the west seat
  /// first; the passengers chosen take the first four seats
  /// (https://alchemy.pascalpixel.com/places/tolbi-ship-interiors/rowing-deck-seats.png).
  ROWING_FIRST_OARSMAN,
  /// The nine passengers in the order of enum TolbiShipPassenger.
  ROWING_FIRST_PASSENGER = ROWING_FIRST_OARSMAN + 8,
  ROWING_HEAD_GUIDE = ROWING_FIRST_PASSENGER + PASSENGER_HEAD_GUIDE,
  ROWING_SECOND_GUIDE = ROWING_FIRST_PASSENGER + PASSENGER_SECOND_GUIDE,
  ROWING_SWEATING_PASSENGER = ROWING_FIRST_PASSENGER + PASSENGER_SWEATING,
  ROWING_LADY = ROWING_FIRST_PASSENGER + PASSENGER_LADY,
  ROWING_STARED_AT_PASSENGER = ROWING_FIRST_PASSENGER + PASSENGER_STARED_AT,
  ROWING_WARRIOR = ROWING_FIRST_PASSENGER + PASSENGER_WARRIOR,
  ROWING_TICKET_GRUMBLER = ROWING_FIRST_PASSENGER + PASSENGER_TICKET_GRUMBLER,
  ROWING_MUSCLE_ADMIRER = ROWING_FIRST_PASSENGER + PASSENGER_MUSCLE_ADMIRER,
  ROWING_OLD_MAN = ROWING_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  /// "Our replacement never arrived, but we're shoving off just the same."
  ROWING_KAJA,
  /// The eight oars, one beside each seat, hidden while the oarsmen row.
  ROWING_FIRST_OAR,
  /// The monsters of each attack (gTolbiShipInteriors_FirstAttackMonsters and
  /// the two after it).
  ROWING_FIRST_MONSTER = ROWING_FIRST_OAR + 8
};

/// The coord events and searched containers below decks whose map events are
/// more than a warp.
enum TolbiShipInteriorsCell {
  /// The rowing deck's stairs up to the hold, barred while monsters are
  /// aboard.
  TOLBI_SHIP_INTERIORS_CELL_ROWING_DECK_STAIRS = 4,
  /// In the captain's quarters, where the crew's talk of the delay plays.
  TOLBI_SHIP_INTERIORS_CELL_QUARTERS_CREW = 10,
  /// Inside the hold, where Ouranos threatens to go ashore.
  TOLBI_SHIP_INTERIORS_CELL_HOLD_WARRIORS = 11,
  TOLBI_SHIP_INTERIORS_CRATE_HOLD_BARREL = 100,
  TOLBI_SHIP_INTERIORS_HOLD_BARREL = 101,
  TOLBI_SHIP_INTERIORS_QUARTERS_JAR = 102
};

/// A speaker: the object event, and in the top four bits where the message
/// window opens beside them.
#define SPEAKER(localId, window) ((window) << 12 | (localId))

// Defined below, in the order of the ROM.
static void TolbiShipInteriors_StepBesideLeader(s32 passengerId);
static s32 TolbiShipInteriors_IsSpotFree(s32 bucket);
static s32 TolbiShipInteriors_GetChosenOarsman(void);
static void TolbiShipInteriors_AskToRow(s32 speaker, s32 question,
                                        s32 seatFlag);
static void TolbiShipInteriors_DiscussLaunchingWithoutAnchor(void);
static void TolbiShipInteriors_SetUpCaptainsQuarters(void);
static void TolbiShipInteriors_SetUpHold(void);
static void TolbiShipInteriors_SetUpRowingDeck(void);
static void TolbiShipInteriors_SetOarCollision(void);
static void TolbiShipInteriors_AskForReplacementOarsmen(void);
static void TolbiShipInteriors_CastOff(void);
static void TolbiShipInteriors_AnnounceDeparture(void);
static void TolbiShipInteriors_KajaCallsToOars(void);
static void TolbiShipInteriors_KajaCallsReplacementToOars(void);
static void TolbiShipInteriors_ShowRowing(void);
static void TolbiShipInteriors_AskAboutMonsters(void);
static void TolbiShipInteriors_AnnounceTurn(void);
static void TolbiShipInteriors_StartFirstAttack(void);
static void TolbiShipInteriors_EndFirstAttack(void);
static void TolbiShipInteriors_IntroduceReplacementOarsman(s32 seat);
static struct ObjectEvent *TolbiShipInteriors_PlaceAnchorCharm(void);
static void TolbiShipInteriors_InstallRowingDeckObjectEvents(u32 action);
static void TolbiShipInteriors_Say(s32 speaker);
static void TolbiShipInteriors_Turn(s32 localId, u16 facing);
static void TolbiShipInteriors_PlaceObjectEvent(s32 localId, s32 x, s32 z,
                                                s32 facing);
static void TolbiShipInteriors_MoveCameraAndWait(s32 x, s32 y, s32 z,
                                                 u32 flags);
static void TolbiShipInteriors_RunSceneCommand(s32 step, u32 arg, u32 opt);
static s32 TolbiShipInteriors_FindFirstSetFlag(u32 group, s32 alternate);
static void TolbiShipInteriors_HideSelectedPassengers(void);
static void TolbiShipInteriors_HideSelectedPassenger(s32 localId,
                                                     s32 firstFlag);
static void TolbiShipInteriors_StartSecondAttack(void);
static void TolbiShipInteriors_ReportMonstersBelowdecks(void);
static void TolbiShipInteriors_ReactToMonsterAttack(void);
static void TolbiShipInteriors_EndSecondAttack(void);
static void TolbiShipInteriors_StartThirdAttack(void);
static void TolbiShipInteriors_EndThirdAttack(void);
static void TolbiShipInteriors_NoticeShipRocking(void);
static void TolbiShipInteriors_ShakeWhileKrakenAboard(void);
static void TolbiShipInteriors_ShowCaptainsQuartersBeforeIsland(void);
static void TolbiShipInteriors_ReachIsland(void);
static void TolbiShipInteriors_DismissReplacementRowers(void);
static void TolbiShipInteriors_SwapRowerSelections(s32 firstSeatFlags,
                                                   s32 secondSeatFlags);

/// An action-script call: stands the object event still at the origin, with no
/// velocity and no target. Returns 0.
static s32
TolbiShipInteriors_ResetObjectEvent(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetMode(objectEvent, 1);
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
static s32 TolbiShipInteriors_Bob(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 verticalMotionDirection;
  s32 descendingY;
  s32 ascendingY;

  if ((s16)objectEvent->work != 0) {
    descendingY =
        (objectEvent->y.fixed - ((u32)(Engine_RandomNext() << 15) >> 16)) -
        PIXELS(1) / 2;
    objectEvent->y.fixed = descendingY;
    if (descendingY < 0) {
      verticalMotionDirection = 0;
      objectEvent->work = verticalMotionDirection;
    }
  } else {
    ascendingY = objectEvent->y.fixed +
                 ((u32)(Engine_RandomNext() << 15) >> 16) + PIXELS(1) / 2;
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
static s32
TolbiShipInteriors_LookAround(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  u32 choice = (u32)(Engine_RandomNext(objectEvent) << 6) >> 16;

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
static void
TolbiShipInteriors_WaitThenAdvance(struct ObjectEvent *objectEvent) {
  u8 *counter = &objectEvent->riseCounter;
  s32 frame = *counter + 1;

  *counter = frame;
  if ((u8)frame > 80) {
    objectEvent->work += 1;
  }
}

/// Counts down the object event's walking time in riseBaseY. Returns 1 once the
/// time is up or the object event has reached its target, else 0.
static s32 TolbiShipInteriors_HasArrived(struct ObjectEvent *objectEvent) {
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

/// The update of Kaja (QUARTERS_KAJA) while the ship waits off the island: he
/// paces between two spots in the captain's quarters, looking about at each,
/// and signals the captain (QUARTERS_CAPTAIN) through his own riseEnabled at
/// three points of the round, but not while the captain is paused. work holds
/// the step.
static void
TolbiShipInteriors_PaceCaptainsQuarters(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *captain;

  captain = ObjectEvent_GetById(QUARTERS_CAPTAIN);
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
    ObjectEvent_SetMode(objectEvent, 2);
    Object_SetPosition(objectEvent, PIXELS(468), PIXELS(32), PIXELS(632));
    objectEvent->riseBaseY = 60;
    objectEvent->work++;
    break;
  case 5:
    if (TolbiShipInteriors_HasArrived(objectEvent) != 0) {
      ObjectEvent_SetMode(objectEvent, 1);
      objectEvent->riseCounter = 0;
      if (captain->paused == 0) {
        objectEvent->riseEnabled = 1;
      }
      objectEvent->work++;
    }
    break;
  case 7:
    if (captain->paused == 0) {
      ObjectEvent_SetMode(objectEvent, 3);
      objectEvent->riseEnabled = 2;
    }
    objectEvent->work++;
    objectEvent->riseCounter = 0;
    break;
  case 9:
    ObjectEvent_SetMode(objectEvent, 2);
    Object_SetPosition(objectEvent, PIXELS(480), PIXELS(32), PIXELS(600));
    objectEvent->riseBaseY = 60;
    objectEvent->work++;
    if (captain->paused == 0) {
      objectEvent->riseEnabled = 3;
    }
    break;
  case 10:
    if (TolbiShipInteriors_HasArrived(objectEvent) != 0) {
      ObjectEvent_SetMode(objectEvent, 1);
      objectEvent->riseCounter = 0;
      objectEvent->work++;
    }
    break;
  case 1:
  case 3:
  case 6:
  case 8:
  case 11:
    TolbiShipInteriors_WaitThenAdvance(objectEvent);
    break;
  case 12:
    objectEvent->work = 0;
    break;
  }
}

/// The update of the captain, who watches Kaja pace: turns north-northeast at
/// the first signal, shrugs at the second if still turned, and faces east at
/// the third. Does nothing while paused.
static void TolbiShipInteriors_WatchPacer(union FieldObject *fieldObject) {
  struct ObjectEvent *watcher = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *pacer;

  pacer = ObjectEvent_GetById(QUARTERS_KAJA);
  if (watcher->paused != 0)
    return;
  if (pacer->riseEnabled == 1) {
    watcher->facing = FACING_NORTH + FACING_STEP;
    watcher->riseCounter = 1;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 2) {
    if (watcher->riseCounter != 0)
      ObjectEvent_SetMode(watcher, 3);
    watcher->riseCounter = 0;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 3) {
    watcher->facing = 0;
    pacer->riseEnabled = 0;
  }
}

/// An action-script call: lays the object event flat on the floor, drawn
/// underfoot at priority 1 with its parts hidden. Returns 1.
static s32 TolbiShipInteriors_LayFlat(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *work = (struct ObjectEvent *)interpreter;
  struct AnimationObject *sprite = work->sprite;

  work->collisionFlags = 8;
  Engine_ObjectEventSetSpriteFlags(work, 0);
  sprite->image.priority = 1;
  sprite->shadow.priority = 1;
  work->priorityFlags = (work->priorityFlags & ~1) | 2;
  ObjectGroup_SetChildValue(work, 15);
  return 1;
}

const struct MapWarpDest *TolbiShipInteriors_SelectWarpDests(void) {
  if (gGameState.mapId == (s32)&MapId_TolbiShipInteriors) {
    return gTolbiShipInteriors_WarpDests;
  }
  return gTolbiShipInteriors_WarpDestsOther;
}

const struct MapWarpRegion *TolbiShipInteriors_GetRegions(void) { return 0; }

const u32 *TolbiShipInteriors_GetWarps(void) {
  return gTolbiShipInteriors_Warps;
}

/// A byte view keeps the native address arithmetic; direct row-field writes
/// choose different bases and registers. The layout still belongs to the row.
#define TOLBI_SHIP_TALK_FACING(bytes, row)                                     \
  ((bytes)[(row) * sizeof(struct ObjectEventTemplate) +                        \
           offsetof(struct ObjectEventTemplate, talkFacing)])

/// Picks the object event templates for the room entered and the voyage's
/// stage. Some stages change who turns to face the party when talked to
/// (talkFacing) in the shared tables.
const struct ObjectEventTemplate *
TolbiShipInteriors_SelectObjectEventTemplates(void) {
  u8 *templateBytes;

  switch (gGameState.warpDest) {
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gTolbiShipInteriors_HoldObjectEventTemplates;
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
               0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
        // Update talkFacing in the object event template rows, preserving their
        // write order.
        templateBytes = (u8 *)gTolbiShipInteriors_HoldObjectEventTemplatesAtSea;
        TOLBI_SHIP_TALK_FACING(templateBytes, 0) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 2) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 4) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 5) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 8) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 7) = TALK_FACING_KEEP;
        TOLBI_SHIP_TALK_FACING(templateBytes, 6) = TALK_FACING_PARTY_AND_BACK;
        TOLBI_SHIP_TALK_FACING(templateBytes, 3) = TALK_FACING_KEEP;
      }
      return gTolbiShipInteriors_HoldObjectEventTemplatesAtSea;
    } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
        gTolbiShipInteriors_HoldObjectEventTemplates[0].talkFacing =
            TALK_FACING_KEEP;
        gTolbiShipInteriors_HoldObjectEventTemplates[4].talkFacing =
            TALK_FACING_KEEP;
        gTolbiShipInteriors_HoldObjectEventTemplates[1].talkFacing =
            TALK_FACING_KEEP;
        gTolbiShipInteriors_HoldObjectEventTemplates[3].talkFacing =
            TALK_FACING_KEEP;
      }
      return gTolbiShipInteriors_HoldObjectEventTemplates;
    } else {
      return gTolbiShipInteriors_HoldObjectEventTemplatesEmpty;
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
  case TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
    return gTolbiShipInteriors_RowingDeckObjectEventTemplates;
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    templateBytes = (u8 *)gTolbiShipInteriors_RowingDeckObjectEventTemplates;
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
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesArrived;
    } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
        if (GameFlag_IsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
          gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesWarriors[1]
              .talkFacing = TALK_FACING_PARTY_AND_BACK;
        }
        if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
          if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
            gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesWarriors[0]
                .talkFacing = TALK_FACING_PARTY;
          }
        }
        return gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesWarriors;
      }
      return gTolbiShipInteriors_ObjectEventTemplatesNone;
    } else {
      return gTolbiShipInteriors_CaptainsQuartersObjectEventTemplates;
    }
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
    return gTolbiShipInteriors_CaptainsQuartersObjectEventTemplates;
    break;
  default:
    return gTolbiShipInteriors_ObjectEventTemplatesNone;
    break;
  }
  return (const struct ObjectEventTemplate *)templateBytes;
}

#undef TOLBI_SHIP_TALK_FACING

/// Chooses the room's interactions for the current stage of the voyage.
const struct MapEvent *TolbiShipInteriors_SelectEvents(void) {
  s32 warpDest = gGameState.warpDest;

  switch (warpDest) {
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gTolbiShipInteriors_HoldEventsAtIsland;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      return gTolbiShipInteriors_HoldEventsChoosingRowers;
    }
    return gTolbiShipInteriors_HoldEvents;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gTolbiShipInteriors_RowingDeckEventsArrived;
    }
    return gTolbiShipInteriors_RowingDeckEvents;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gTolbiShipInteriors_CaptainsQuartersEventsAtIsland;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gTolbiShipInteriors_CaptainsQuartersEventsArrived;
    }
    return gTolbiShipInteriors_CaptainsQuartersEvents;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    return gTolbiShipInteriors_RowingDeckEventsMonstersAboard;
  case TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP:
    return gTolbiShipInteriors_RowingDeckEventsKrakenAboard;
  default:
    break;
  }

  return gTolbiShipInteriors_EventsOther;
}

/// The crewman asks whether the party wants to see the captain too. This is in
/// the captain's quarters at Kalay, once the warriors want to see the captain;
/// on yes, the passengers' pressure on the captain plays
/// (FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL).
static void TolbiShipInteriors_TalkToCaptainsQuartersCrewman(void) {
  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipIfShipFromTolbiHad);
    Script_ShowMessage(QUARTERS_CREWMAN, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ScriptSetMessage(MsgTolbiShipNowWantSeeCaptainToo);
      Script_OpenMessage(QUARTERS_CREWMAN, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 0) {
        TolbiShipInteriors_DiscussLaunchingWithoutAnchor();
      } else {
        Engine_ObjectEventStartRepeatedMotion(QUARTERS_CREWMAN, 2);
        Script_ShowMessage(QUARTERS_CREWMAN, 0);
        ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP,
                                  0);
      }
    } else {
      Engine_ScriptSetMessage(MsgTolbiShipButWeCantSendShip);
      Script_ShowMessage(QUARTERS_CREWMAN, 0);
    }
  }
  Engine_ScriptEnd();
}

/// Sean, in the captain's quarters, asks whether the party came to ask the
/// captain to set sail.
static void TolbiShipInteriors_TalkToSeanInCaptainsQuarters(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiShipYouCameAskCaptainSet);
  Engine_ScriptAskYesNo(QUARTERS_SEAN, 0);
  Engine_ScriptEnd();
}

/// The captain, by the voyage's stage: too late to hire mercenaries, then his
/// bad luck at losing his lucky anchor, then uncertain what might happen, and
/// under sail the news of an injured oarsman. After finding the Anchor Charm,
/// talking to him also opens the main menu, so that the charm can be returned.
static void TolbiShipInteriors_TalkToCaptain(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipOarsmanWasInjured);
    TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
    ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP, 60);
    Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 4);
    TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
    Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipWeDontKnowMightHappen);
    Script_ShowMessage(QUARTERS_CAPTAIN, 0);
  } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipBadLuckLosingMyLucky);
    Script_ShowMessage(QUARTERS_CAPTAIN, 0);
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) == 0 &&
        GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND) != 0) {
      gFieldWork->menu = 1;
    }
  } else {
    Engine_ScriptSetMessage(MsgTolbiShipItsTooLateHireMercenaries);
    Script_ShowMessage(QUARTERS_CAPTAIN, 0);
  }
  Engine_ScriptEnd();
}

/// A passenger in the hold while the ship waits. Once the party has agreed to
/// protect the ship, she worries the proud warriors won't like being told to
/// row, and asks the party.
static void TolbiShipInteriors_TalkToWaitingWarrior(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_ObjectEventStartRepeatedMotion(HOLD_WARRIOR, 2);
    Engine_ScriptSetMessage(MsgTolbiShipTheseProudWarriorsNotGoing);
    TolbiShipInteriors_Say(HOLD_WARRIOR);
    ObjectEvent_FaceObjectEvent(HOLD_WARRIOR, LOCALID_PARTY_LEADER, 10);
    Script_OpenMessage(HOLD_WARRIOR, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Engine_ScriptWait(40);
      TolbiShipInteriors_Say(HOLD_WARRIOR);
      TolbiShipInteriors_Turn(HOLD_WARRIOR, FACING_SOUTHEAST + FACING_STEP);
      Script_ShowMessage(HOLD_WARRIOR, 0);
    } else {
      gFieldWork->message += 2;
      Script_ShowMessage(HOLD_WARRIOR, 0);
      ObjectEvent_FaceDirection(HOLD_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                                0);
    }
  } else {
    Engine_ScriptSetMessage(MsgTolbiShipLongerWeSitHereMore);
    Script_ShowMessage(HOLD_WARRIOR, 0);
  }
  Engine_ScriptEnd();
}

/// Another waiting passenger, once the party has agreed to protect the ship,
/// asks whether it will row. "Are you going to row?"
static void TolbiShipInteriors_TalkToWaitingMuscleAdmirer(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipYouGoingRow);
    Engine_ScriptAskYesNo(HOLD_MUSCLE_ADMIRER, 0);
  } else {
    Engine_ScriptSetMessage(MsgTolbiShipTheresWholeBunchReallyMuscular);
    Engine_ScriptShowMessage(HOLD_MUSCLE_ADMIRER, 0);
  }
  Engine_ScriptEnd();
}

/// Walks the rower back to the leader and hides its separate object event.
static inline void TolbiShipInteriors_RejoinParty(s32 rowerId) {
  struct ObjectEvent *leader;

  Engine_ObjectEventSetAnimation(rowerId, 2);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    Engine_ObjectEventSetDestination(rowerId, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(rowerId);
  Engine_ObjectEventSetPosition(rowerId, 0, 0);
}

/// A passenger in the hold under sail, when an oar needs a rower. If someone
/// has already been chosen this round, the chosen passenger steps beside the
/// leader and follows them out while this one says chosenMessage; otherwise
/// the party may ask this one to row (question) at the seat the attacks
/// fought off number. The Ask...ToRow functions are this with each
/// passenger's lines, except the stared-at passenger's and the second guide's,
/// which write it out in full.
#define TOLBI_SHIP_ASK_TO_ROW(speaker, chosenMessage, question, passenger)     \
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {               \
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();                       \
                                                                               \
    Engine_ScriptStart();                                                      \
    TolbiShipInteriors_StepBesideLeader(rowerId);                              \
    Engine_ScriptSetMessage(chosenMessage);                                    \
    TolbiShipInteriors_Say(speaker);                                           \
    TolbiShipInteriors_RejoinParty(rowerId);                                   \
    Engine_ScriptEnd();                                                        \
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {     \
    TolbiShipInteriors_AskToRow(                                               \
        speaker, question, FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + (passenger));  \
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) !=  \
             0) {                                                              \
    TolbiShipInteriors_AskToRow(                                               \
        speaker, question, FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + (passenger));   \
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != \
             0) {                                                              \
    TolbiShipInteriors_AskToRow(                                               \
        speaker, question, FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger));  \
  } else {                                                                     \
    TolbiShipInteriors_AskToRow(                                               \
        speaker, question, FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger));   \
  }

/// The head guide in the hold under sail.
static void TolbiShipInteriors_AskHeadGuideToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(SEA_HOLD_HEAD_GUIDE,
                        MsgTolbiShipLooksLikeYouveBeenChosen,
                        MsgTolbiShipNoooooNotGoing, PASSENGER_HEAD_GUIDE);
}

/// The sweating passenger in the hold under sail.
static void TolbiShipInteriors_AskSweatingPassengerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(SEA_HOLD_SWEATING_PASSENGER,
                        MsgTolbiShipHaHaHaChosenOarsman,
                        MsgTolbiShipJokingWantRow, PASSENGER_SWEATING);
}

/// The lady in the hold under sail, who is too seasick to talk once the ship
/// is at anchor.
static void TolbiShipInteriors_AskLadyToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    Engine_ScriptStart();
    Engine_ObjectEventSetAttachedEffect(
        SEA_HOLD_LADY, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(40);
    Engine_ScriptSetMessage(MsgTolbiShipDontFeelDontTalkMe);
    Engine_ScriptShowMessage(SEA_HOLD_LADY, 0);
    Engine_ScriptEnd();
  } else {
    TOLBI_SHIP_ASK_TO_ROW(SEA_HOLD_LADY,
                          MsgTolbiShipYouFinallyPickedSomeoneDidnt,
                          MsgTolbiShipDontTellGoing, PASSENGER_LADY);
  }
}

/// The stared-at passenger is asked to row, as the others are. It's written out
/// in full rather than through TolbiShipInteriors_AskToRow.
static void TolbiShipInteriors_AskStaredAtPassengerToRow(void) {
  s32 rowerId;
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_GetChosenOarsman();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_ScriptSetMessage(MsgTolbiShipGivesMeChillsThinkCould);
    TolbiShipInteriors_Say(SEA_HOLD_STARED_AT_PASSENGER);
    TolbiShipInteriors_RejoinParty(rowerId);
  } else {
    Engine_ObjectEventRunRepeatedMotion(SEA_HOLD_STARED_AT_PASSENGER, 2);
    Engine_ScriptWait(20);
    Engine_ScriptSetMessage(MsgTolbiShipOhhhhNooooGoingMakeMe);
    Script_OpenMessage(SEA_HOLD_STARED_AT_PASSENGER, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      TolbiShipInteriors_Say(SEA_HOLD_STARED_AT_PASSENGER);
      Engine_ObjectEventSetAnimation(SEA_HOLD_STARED_AT_PASSENGER, 2);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        ObjectEvent_SetDestination(SEA_HOLD_STARED_AT_PASSENGER,
                                   objectEvent->x.part.pixel,
                                   objectEvent->z.part.pixel);
      }
      Engine_ObjectEventWaitForMove(SEA_HOLD_STARED_AT_PASSENGER);
      ObjectEvent_SetPosition(SEA_HOLD_STARED_AT_PASSENGER, 0, 0);
      GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
      if (GameFlag_IsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + PASSENGER_STARED_AT);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + PASSENGER_STARED_AT);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
                 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + PASSENGER_STARED_AT);
      } else {
        GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_STARED_AT);
      }
    } else {
      gFieldWork->message++;
      TolbiShipInteriors_Say(SEA_HOLD_STARED_AT_PASSENGER);
    }
  }
  Engine_ScriptEnd();
}

/// The second guide in the hold under sail, written out in full as well.
static void TolbiShipInteriors_AskSecondGuideToRow(void) {
  s32 rowerId;
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_GetChosenOarsman();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_ScriptSetMessage(MsgTolbiShipIsaacYouveGotGoodEye);
    TolbiShipInteriors_Say(SEA_HOLD_SECOND_GUIDE);
    TolbiShipInteriors_RejoinParty(rowerId);
  } else {
    Engine_ScriptSetMessage(MsgTolbiShipHaHaHaRowingFeel);
    Engine_ScriptShowMessageAndWait(SEA_HOLD_SECOND_GUIDE, 0, 60);
    Engine_ObjectEventRunRepeatedMotion(SEA_HOLD_SECOND_GUIDE, 1);
    Script_OpenMessage(SEA_HOLD_SECOND_GUIDE, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      TolbiShipInteriors_Say(SEA_HOLD_SECOND_GUIDE);
      Engine_ObjectEventSetAnimation(SEA_HOLD_SECOND_GUIDE, 2);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        ObjectEvent_SetDestination(SEA_HOLD_SECOND_GUIDE,
                                   objectEvent->x.part.pixel,
                                   objectEvent->z.part.pixel);
      }
      Engine_ObjectEventWaitForMove(SEA_HOLD_SECOND_GUIDE);
      ObjectEvent_SetPosition(SEA_HOLD_SECOND_GUIDE, 0, 0);
      GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
      if (GameFlag_IsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
                 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      } else {
        GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                     PASSENGER_SECOND_GUIDE);
      }
    } else {
      gFieldWork->message++;
      TolbiShipInteriors_Say(SEA_HOLD_SECOND_GUIDE);
    }
  }
  Engine_ScriptEnd();
}

/// The warrior in the hold under sail.
static void TolbiShipInteriors_AskWarriorToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(SEA_HOLD_WARRIOR, MsgTolbiShipThatsWhoYouPickedOarsman,
                        MsgTolbiShipNotThinkingMaking, PASSENGER_WARRIOR);
}

/// The ticket grumbler in the hold under sail.
static void TolbiShipInteriors_AskTicketGrumblerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(
      SEA_HOLD_TICKET_GRUMBLER, MsgTolbiShipIfYouveChosenOarsmanLets,
      MsgTolbiShipOarsmanGiveBreak, PASSENGER_TICKET_GRUMBLER);
}

/// The muscle admirer in the hold under sail.
static void TolbiShipInteriors_AskMuscleAdmirerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(SEA_HOLD_MUSCLE_ADMIRER,
                        MsgTolbiShipYouveBeenChosenAsOarsman,
                        MsgTolbiShipPoorOarsmanFeel, PASSENGER_MUSCLE_ADMIRER);
}

/// The old man in the hold under sail.
static void TolbiShipInteriors_AskOldManToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(SEA_HOLD_OLD_MAN, MsgTolbiShipHoHoPersonGoingGet,
                        MsgTolbiShipPeopleAskingFrail, PASSENGER_OLD_MAN);
}

#undef TOLBI_SHIP_ASK_TO_ROW

/// Finds a placed object event within twelve pixels of the requested floor
/// position.
static struct ObjectEvent *TolbiShipInteriors_FindObjectEventNear(s32 x,
                                                                  s32 z) {
  struct EventWork *work;
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
  objectEvent = work->placedObjectEvents;
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
static void TolbiShipInteriors_StepBesideLeader(s32 passengerId) {
  struct ObjectEvent *leader;
  struct ObjectEvent *leaderPosition;
  s32 blocked;
  s32 direction;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  blocked = 1;
  Engine_ObjectEventSetSpritePriority(passengerId, 2);
  {
    struct ObjectEvent *passenger = ObjectEvent_GetById(passengerId);

    passenger->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | passenger->priorityFlags;
  }
  // Try the leader's right, then left, then the space behind.
  direction = (((leader->facing + 0x4000) & 0xf000) >> 12);
  if (TolbiShipInteriors_IsSpotFree(
          (((leader->facing + 0x4000) & 0xf000) >> 12)) != 0) {
    blocked = 0;
  }
  if (blocked != 0) {
    direction = (((leader->facing + -0x4000) & 0xf000) >> 12);
    if (TolbiShipInteriors_IsSpotFree(
            (((leader->facing + -0x4000) & 0xf000) >> 12)) != 0) {
      blocked = 0;
    }
    if (blocked != 0) {
      direction = (((leader->facing + 0x8000) & 0xf000) >> 12);
    }
  }
  leaderPosition = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leaderPosition != 0) {
    Engine_ObjectEventSetPosition(passengerId, leaderPosition->x.fixed,
                                  leaderPosition->z.fixed);
  }
  ObjectEvent_SetSpeed(passengerId, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ObjectEventSetAnimation(passengerId, 2);
  {
    s32 offset = gTolbiShipInteriors_TurnSteps[direction];

    Engine_ObjectEventSetDestinationOffset(passengerId, offset >> 16,
                                           (offset << 16) >> 16);
  }
  Engine_ObjectEventWaitForMove(passengerId);
  Engine_ObjectEventSetAnimation(passengerId, 1);
  TolbiShipInteriors_Turn(passengerId, leader->facing);
}

enum { KAJA_ROWING_SPEAKER = SPEAKER(ROWING_KAJA, 10) };

/// Returns 1 when the spot next to the leader in the given sixteenth of a turn
/// holds no object event and no wall, else 0.
static s32 TolbiShipInteriors_IsSpotFree(s32 bucket) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u32 offset = gTolbiShipInteriors_TurnSteps[bucket];
  s32 x = objectEvent->x.part.pixel + ((s32)offset >> 16);
  s32 z = objectEvent->z.part.pixel + (s16)offset;

  if (TolbiShipInteriors_FindObjectEventNear(x, z) != 0) {
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
static s32 TolbiShipInteriors_GetChosenOarsman(void) {
  s32 ret = 0;

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    ret = 3;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) !=
             0) {
    ret = 2;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
             0) {
    ret = 1;
  }

  return TolbiShipInteriors_FindFirstSetFlag(ret, 1);
}

/// What a passenger at the oars says, or thinks under Mind Read, by the round
/// they were chosen in: the first round's line, the second's, or a later
/// one's. The TalkTo...AtOars and Read...MindAtOars functions are this, or
/// TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER, with each passenger's lines.
#define TOLBI_SHIP_ROWER_LINE_BY_ROUND(passenger, localId, first, second,      \
                                       later)                                  \
  Engine_ScriptStart();                                                        \
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))   \
    Engine_ScriptSetMessage(first);                                            \
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS +           \
                                (passenger)))                                  \
    Engine_ScriptSetMessage(second);                                           \
  else                                                                         \
    Engine_ScriptSetMessage(later);                                            \
  Engine_ScriptShowMessage(localId, 0);                                        \
  Engine_ScriptEnd()

/// The same with one line for the first round and one for any later.
#define TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(passenger, localId, first, later) \
  Engine_ScriptStart();                                                        \
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))   \
    Engine_ScriptSetMessage(first);                                            \
  else                                                                         \
    Engine_ScriptSetMessage(later);                                            \
  Engine_ScriptShowMessage(localId, 0);                                        \
  Engine_ScriptEnd()

/// The head guide at the oars: Kaja's men are worried, monsters are
/// everywhere, or the ship is starting to list.
static void TolbiShipInteriors_TalkToHeadGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_HEAD_GUIDE, ROWING_HEAD_GUIDE,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The second guide at the oars, as the head guide.
static void TolbiShipInteriors_TalkToSecondGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SECOND_GUIDE, ROWING_SECOND_GUIDE,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The sweating passenger at the oars, as the head guide.
static void TolbiShipInteriors_TalkToSweatingPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SWEATING, ROWING_SWEATING_PASSENGER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The lady at the oars asks how many monsters are out there.
static void TolbiShipInteriors_TalkToLadyAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, ROWING_LADY, MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// The stared-at passenger at the oars, as the head guide.
static void TolbiShipInteriors_TalkToStaredAtPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_STARED_AT,
                                 ROWING_STARED_AT_PASSENGER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The warrior at the oars, as the head guide.
static void TolbiShipInteriors_TalkToWarriorAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_WARRIOR, ROWING_WARRIOR,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The ticket grumbler at the oars, as the head guide.
static void TolbiShipInteriors_TalkToTicketGrumblerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_TICKET_GRUMBLER,
                                 ROWING_TICKET_GRUMBLER,
                                 MsgTolbiShipThingHasKajaHisMen,
                                 MsgTolbiShipMonstersEverywhereImStuckRowing,
                                 MsgTolbiShipShipStartingListIfWe);
}

/// The muscle admirer at the oars, as the lady.
static void TolbiShipInteriors_TalkToMuscleAdmirerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// Mind Read on the head guide at the oars. "Just hurry up and slay that
/// beast!"
static void TolbiShipInteriors_ReadHeadGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_HEAD_GUIDE, ROWING_HEAD_GUIDE,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the second guide at the oars, as on the head guide.
static void TolbiShipInteriors_ReadSecondGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SECOND_GUIDE, ROWING_SECOND_GUIDE,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the sweating passenger at the oars, as on the head guide.
static void TolbiShipInteriors_ReadSweatingPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_SWEATING, ROWING_SWEATING_PASSENGER,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the lady at the oars: there are so many monsters at sea.
static void TolbiShipInteriors_ReadLadyMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, ROWING_LADY, MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

/// Mind Read on the stared-at passenger at the oars, as on the head guide.
static void TolbiShipInteriors_ReadStaredAtPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_STARED_AT, ROWING_STARED_AT_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the warrior at the oars, as on the head guide.
static void TolbiShipInteriors_ReadWarriorMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(PASSENGER_WARRIOR, ROWING_WARRIOR,
                                 MsgTolbiShipDontCareTakesJustHurry,
                                 MsgTolbiShipIfThoseMonstersComeBack,
                                 MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the ticket grumbler at the oars, as on the head guide.
static void TolbiShipInteriors_ReadTicketGrumblerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_ROUND(
      PASSENGER_TICKET_GRUMBLER, ROWING_TICKET_GRUMBLER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the muscle admirer at the oars, as on the lady.
static void TolbiShipInteriors_ReadMuscleAdmirerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

#undef TOLBI_SHIP_ROWER_LINE_BY_ROUND
#undef TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER

/// Asks a passenger to row. On yes they walk to the leader and leave with them,
/// and the seat's flag for this passenger (seatFlag) and
/// FLAG_TOLBI_SHIP_ROWER_CHOSEN are set; on no the next message refuses.
static void TolbiShipInteriors_AskToRow(s32 speaker, s32 question,
                                        s32 seatFlag) {
  struct ObjectEvent *leader;

  Engine_ScriptStart();
  Engine_ScriptSetMessage(question);
  Engine_ScriptOpenMessage(speaker, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    TolbiShipInteriors_Say(speaker);
    Engine_ObjectEventSetAnimation(speaker, 2);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      Engine_ObjectEventSetDestination(speaker, leader->x.part.pixel,
                                       leader->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(speaker);
    Engine_ObjectEventSetPosition(speaker, 0, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    Engine_GameFlagSet(seatFlag);
  } else {
    gFieldWork->message++;
    TolbiShipInteriors_Say(speaker);
  }
  Engine_ScriptEnd();
}

/// The travelling healer in the hold. From in front of him (the leader facing
/// roughly east) he opens his Sanctum; otherwise he talks by the voyage's
/// stage. Under sail, until the ship is docked, he is SEA_HOLD_HEALER, else
/// HOLD_HEALER.
static void TolbiShipInteriors_TalkToHealer(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if ((u16)(leader->facing - 0x2000) > 0xc000) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Engine_SanctumOpen(SEA_HOLD_HEALER);
    } else {
      Engine_SanctumOpen(HOLD_HEALER);
    }
  } else {
    Engine_ScriptStart();
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      Engine_ScriptSetMessage(MsgTolbiShipGoodShipHasArrivedSafely);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      Engine_ScriptSetMessage(MsgTolbiShipHateArguing);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
               0) {
      Engine_ScriptSetMessage(MsgTolbiShipShipsCrewReadyAnything);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) !=
               0) {
      Engine_ScriptSetMessage(MsgTolbiShipShipsCrewReadyForAnything);
    } else {
      Engine_ScriptSetMessage(MsgTolbiShipImSpreadingGoodwillWhereverTravel);
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Engine_ScriptShowMessage(SEA_HOLD_HEALER, 0);
    } else {
      Engine_ScriptShowMessage(HOLD_HEALER, 0);
    }
    Engine_ScriptEnd();
  }
}

/// Talking to one of the monsters on the rowing deck (ROWING_FIRST_MONSTER and
/// the two after it) starts the battle with them: a slot of the encounter zone
/// of the Tolbi ship's scripted battles, and the warp destination the party
/// comes back to, chosen by how far the voyage has gone.
static void TolbiShipInteriors_FightRowingDeckMonsters(void) {

  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  Battle_SetStart(BATTLE_START_SCRIPTED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) == 0) {
    FieldScript_SetNextDestination(
        (s32)&MapId_TolbiShipInteriors,
        TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) == 0) {
      FieldScript_SetNextDestination(
          (s32)&MapId_TolbiShipInteriors,
          TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE);
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 1);
    } else {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) == 0) {
        FieldScript_SetNextDestination(
            (s32)&MapId_TolbiShipInteriors,
            TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE);
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 2);
      }
    }
  }
  Engine_ScriptEnd();
}

/// While the monsters are aboard, the leader is walked back from the stairs.
/// "Where are you going, Isaac? Slay the beasts and save my mates!"
static void TolbiShipInteriors_TurnBackFromStairs(void) {
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS) != 0) {
    Engine_ScriptStart();
    Ui_SetMessageVoiceFromObjectEvent(ROWING_CREWMAN);
    UiText_ShowCenteredMessage(MsgTolbiShipWhereGoingSlay, 1, 8);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 408, 134);
    TolbiShipInteriors_Turn(LOCALID_PARTY_LEADER, FACING_SOUTH);
    Engine_ScriptEnd();
  }
}

/// Below decks at Kalay, once the warriors have gone to the captain
/// (TOLBI_SHIP_INTERIORS_CELL_QUARTERS_CREW): the captain, Kaja and the crewman
/// wonder what could have happened to the replacement ship, as on deck
/// (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY).
static void TolbiShipInteriors_WatchCrewWonderAtDelay(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0)
    return;

  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(448), -1, PIXELS(638),
                                       0x10000028u);
  Engine_ScriptSetMessage(MsgTolbiShipWonderCouldHaveHappened);

  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Turn(QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_SOUTH + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_KAJA);

  Engine_ObjectEventFaceDirection(QUARTERS_CAPTAIN, FACING_EAST, 20);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_WEST);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Turn(QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);

  Engine_GameFlagSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
  Engine_ScriptEnd();
}

/// In the hold at Kalay, once the tour group has left Kalay
/// (TOLBI_SHIP_INTERIORS_CELL_HOLD_WARRIORS): Ouranos threatens to go ashore if
/// the ship doesn't sail. The second guide can't say when it will, so Sean and
/// Ouranos set off to make the captain set sail, and the guide ("I don't want
/// any violence!") hurries after them past the party.
static void TolbiShipInteriors_WatchWarriorsStormOff(void) {
  s32 guideSpeaker;
  s32 seanSpeaker;
  const s32 *action;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
    // Not yet: the party has not heard of the delay.
  } else {
    Engine_ScriptStart();
    Field_EndBusyAndReveal();
    FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
    TolbiShipInteriors_MoveCameraAndWait(PIXELS(1463), -1, PIXELS(464),
                                         0x10000014);
    Engine_ObjectEventRunRepeatedMotion(HOLD_OURANOS, 1);
    Engine_ScriptSetMessage(MsgTolbiShipIfWeArentGoingSet);
    TolbiShipInteriors_Say(SPEAKER(HOLD_OURANOS, 2));
    TolbiShipInteriors_Turn(HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP);
    guideSpeaker = SPEAKER(HOLD_SECOND_GUIDE, 8);
    ObjectEvent_ShowEmote(HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SECOND_GUIDE, 2);
    TolbiShipInteriors_Say(guideSpeaker);
    Engine_ObjectEventRunRepeatedMotion(HOLD_SEAN, 1);
    Script_ShowMessageAndWait(SPEAKER(HOLD_SEAN, 10), 0, 20);
    TolbiShipInteriors_Turn(HOLD_SECOND_GUIDE, 0);
    seanSpeaker = SPEAKER(HOLD_SEAN, 10);
    ObjectEvent_ShowEmote(HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          40);
    ObjectEvent_ShowEmote(HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SEAN, 3);
    TolbiShipInteriors_Say(seanSpeaker);
    ObjectEvent_SetAttachedEffect(HOLD_SECOND_GUIDE, ATTACHED_EFFECT_IN_FRONT |
                                                         ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(40);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SECOND_GUIDE, 3);
    TolbiShipInteriors_Say(guideSpeaker);
    Engine_ObjectEventRunRepeatedMotion(HOLD_SEAN, 1);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SEAN, FACING_NORTHWEST + FACING_STEP);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP);
    ObjectEvent_ShowEmote(HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          30);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SECOND_GUIDE, 1);
    TolbiShipInteriors_Say(guideSpeaker);
    Engine_ObjectEventSetAnimationAndWait(HOLD_OURANOS, 4);
    TolbiShipInteriors_Say(SPEAKER(HOLD_OURANOS, 2));
    Engine_ObjectEventStartRepeatedMotion(HOLD_OURANOS, 2);
    TolbiShipInteriors_Say(SPEAKER(HOLD_OURANOS, 2));
    Engine_ObjectEventSetAnimation(HOLD_SECOND_GUIDE, 4);
    TolbiShipInteriors_Say(guideSpeaker);
    Engine_ObjectEventSetAnimationAndWait(HOLD_SEAN, 4);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SEAN, FACING_WEST);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SEAN, 2);
    Script_ShowMessageAndWait(seanSpeaker, 0, 20);
    ObjectEvent_FaceDirection(HOLD_SECOND_GUIDE, FACING_EAST, 0);
    ObjectEvent_ShowEmote(HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Script_ShowMessageAndWait(guideSpeaker, 0, 20);
    ObjectEvent_ShowEmote(HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    ObjectEvent_ShowEmote(HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SEAN, 2);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SEAN, FACING_NORTHWEST + FACING_STEP);
    Engine_ObjectEventRunRepeatedMotion(HOLD_SEAN, 1);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_OURANOS, FACING_SOUTHEAST + FACING_STEP);
    ObjectEvent_ShowEmote(HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          60);
    Engine_ObjectEventRunRepeatedMotion(HOLD_OURANOS, 1);
    TolbiShipInteriors_Say(HOLD_OURANOS);
    ObjectEvent_ShowEmote(HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ObjectEventRunRepeatedMotion(HOLD_SEAN, 1);
    TolbiShipInteriors_Say(seanSpeaker);
    ObjectEvent_FaceDirection(HOLD_SECOND_GUIDE, FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(HOLD_OURANOS, FACING_SOUTH + FACING_STEP, 40);
    ObjectEvent_FaceDirection(HOLD_SECOND_GUIDE, FACING_EAST, 0);
    TolbiShipInteriors_Turn(HOLD_OURANOS, FACING_SOUTHEAST + FACING_STEP);
    Engine_ObjectEventRunRepeatedMotion(HOLD_SECOND_GUIDE, 2);
    Script_ShowMessageAndWait(guideSpeaker, 0, 20);
    ObjectEvent_FaceDirection(HOLD_SEAN, FACING_SOUTH, 40);
    TolbiShipInteriors_Say(seanSpeaker);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SECOND_GUIDE, 2);
    Engine_ObjectEventRunRepeatedMotion(HOLD_OURANOS, 2);
    Engine_ScriptWait(60);
    Engine_ObjectEventRunRepeatedMotion(HOLD_OURANOS, 1);
    TolbiShipInteriors_Say(HOLD_OURANOS);
    Engine_ObjectEventSetAnimationAndWait(HOLD_SEAN, 3);
    TolbiShipInteriors_Say(seanSpeaker);
    ObjectEvent_ShowEmote(HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Engine_ObjectEventRunRepeatedMotion(HOLD_SECOND_GUIDE, 2);
    TolbiShipInteriors_Say(guideSpeaker);
    Engine_ObjectEventSetAnimationAndWait(HOLD_OURANOS, 3);
    TolbiShipInteriors_Say(HOLD_OURANOS);
    ObjectEvent_FaceDirection(HOLD_SEAN, FACING_NORTHWEST + FACING_STEP, 40);
    Engine_ObjectEventSetAnimation(HOLD_SEAN, 3);
    Engine_ObjectEventSetAnimationAndWait(HOLD_OURANOS, 3);
    ObjectEvent_SetSpeed(HOLD_SEAN, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_SetSpeed(HOLD_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
    action = gTolbiShipInteriors_LeaveHold;
    ObjectEvent_SetActionScript(HOLD_SEAN, action);
    ObjectEvent_SetActionScript(HOLD_OURANOS, action);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(HOLD_SECOND_GUIDE, FACING_SOUTH, 0);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(12) / 5, PIXELS(6) / 5);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &= 254;
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 184, 520);
    Engine_ScriptWait(1);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      objectEvent->steeringFlags =
          OBJ_EVENT_FACES_MOTION | objectEvent->steeringFlags;
    }
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ObjectEventJump(HOLD_SECOND_GUIDE, 4, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 20);
    Engine_ObjectEventStartRepeatedMotion(HOLD_SECOND_GUIDE, 2);
    TolbiShipInteriors_Say(HOLD_SECOND_GUIDE);
    ObjectEvent_SetSpeed(HOLD_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_SetActionScript(HOLD_SECOND_GUIDE, action);
    Engine_ScriptWait(40);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectEvent_RefreshSelectorById(HOLD_SECOND_GUIDE);
    GameFlag_Set(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
    Engine_ScriptEnd();
  }
}

/// In the captain's quarters at Kalay: the passengers press the captain to
/// launch; Ouranos and Sean offer to guard the deck, and the captain finds his
/// lucky anchor charm gone and refuses to sail without it.
static void TolbiShipInteriors_DiscussLaunchingWithoutAnchor(void) {
  s32 seanSpeech;
  s32 ouranosSpeech;
  s32 captainSpeech;

  // Gather the captain's quarters's attention around the argument about
  // launching.
  Script_PlaySong(MUS_028);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(456), -1, PIXELS(648),
                                       0x10000014);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 1);
  Engine_ScriptSetMessage(MsgTolbiShipYoureTryingLaunchShip);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_EAST, 0);
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_WEST, 40);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_KAJA, 2);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                            20);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 1);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                        20);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SECOND_GUIDE, 2);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_SEAN, 9));
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  Engine_ObjectEventSetAnimation(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_SEAN, 9));
  TolbiShipInteriors_Turn(QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_OURANOS, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SECOND_GUIDE, 1);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_SECOND_GUIDE, 3);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_SOUTH + FACING_STEP, 0);
  TolbiShipInteriors_Turn(QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP);
  // Two passengers step aside as the conversation continues.
  ObjectEvent_SetSpeed(QUARTERS_SECOND_GUIDE, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetSpeed(QUARTERS_SEAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(QUARTERS_SEAN, 444, 668);
  ObjectEvent_WalkToAndWait(QUARTERS_SECOND_GUIDE, 472, 668);
  Engine_ObjectEventWaitForMove(QUARTERS_SEAN);
  Engine_ObjectEventSetAnimation(QUARTERS_SEAN, 1);
  Engine_ScriptWait(80);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 1);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | QUARTERS_OURANOS, 0, 40);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 2);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventSetAnimation(QUARTERS_OURANOS, 4);
  Engine_ScriptWait(20);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  Engine_ObjectEventSetAnimation(QUARTERS_KAJA, 3);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_SetAttachedEffect(
      QUARTERS_SECOND_GUIDE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventJump(QUARTERS_SECOND_GUIDE, 2, 20);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_KAJA, 3);
  seanSpeech = SPEAKER(QUARTERS_SEAN, 1);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  TolbiShipInteriors_Turn(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SEAN, 1);
  TolbiShipInteriors_Say(seanSpeech);
  ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ObjectEventSetAnimation(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Engine_ObjectEventJump(QUARTERS_SECOND_GUIDE, 4, 0);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  Engine_ObjectEventSetAnimation(QUARTERS_KAJA, 3);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 1);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(QUARTERS_CAPTAIN, 0);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_OURANOS, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | QUARTERS_OURANOS, 0, 40);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_KAJA, 2);
  Script_ShowMessageAndWait(QUARTERS_KAJA, 0, 20);
  Engine_ObjectEventSetAnimation(QUARTERS_OURANOS, 3);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SEAN, 2);
  ouranosSpeech = SPEAKER(QUARTERS_OURANOS, 4);
  TolbiShipInteriors_Say(seanSpeech);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP, 20);
  TolbiShipInteriors_Say(ouranosSpeech);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SEAN, 2);
  TolbiShipInteriors_Say(seanSpeech);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_OURANOS, 3);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 1);
  TolbiShipInteriors_Say(ouranosSpeech);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 1);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_ShowEmote(QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(QUARTERS_KAJA, 3);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_ShowEmote(QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                        20);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_OURANOS, 2);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  Engine_ObjectEventSetAnimation(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_NORTH + FACING_STEP, 40);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SEAN, 1);
  Engine_ScriptWait(20);
  TolbiShipInteriors_Say(seanSpeech);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                            40);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 1);
  TolbiShipInteriors_Say(ouranosSpeech);
  Engine_ObjectEventSetAnimation(QUARTERS_SEAN, 3);
  Script_ShowMessageAndWait(seanSpeech, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 2);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Engine_ObjectEventSetAnimation(QUARTERS_SEAN, 3);
  TolbiShipInteriors_Say(seanSpeech);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_WEST, 20);
  Script_PlaySong(MUS_SILENCE);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
  captainSpeech = SPEAKER(QUARTERS_CAPTAIN, 8);
  ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        80);
  TolbiShipInteriors_Say(captainSpeech);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  ObjectEvent_SetAttachedEffect(QUARTERS_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventJump(QUARTERS_CAPTAIN, 4, 40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
  Script_ShowMessage(captainSpeech, 0);
  ObjectEvent_SetSpeed(QUARTERS_CAPTAIN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(QUARTERS_CAPTAIN, 475, 598);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_WEST, 0);
  ObjectEvent_SetSpeed(QUARTERS_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(QUARTERS_KAJA, 462, 618);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_NORTHWEST + FACING_STEP);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_KAJA, 2);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_KAJA, 8));
  ObjectEvent_ShowEmote(QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  TolbiShipInteriors_Say(QUARTERS_OURANOS);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  TolbiShipInteriors_Say(seanSpeech);
  ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                        20);
  Engine_ObjectEventJump(QUARTERS_CAPTAIN, 4, 0);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP, 20);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  Script_PlaySong(MUS_028);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 4);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  // Settle the passengers where the next visit to the captain's quarters needs
  // them.
  ObjectEvent_WalkToAndWait(QUARTERS_SEAN, 444, 628);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_SEAN, 9));
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_ShowEmote(QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_SCOWL,
                        40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SECOND_GUIDE, 2);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_KAJA, 4);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_KAJA, 1));
  TolbiShipInteriors_Turn(QUARTERS_SEAN, 0);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_EAST, 0);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(464), -1, PIXELS(680),
                                       0x10000000);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CREWMAN, 1);
  TolbiShipInteriors_Turn(QUARTERS_CREWMAN, 0);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_ShowEmote(QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_WEST, 20);
  ObjectEvent_ShowEmote(QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  Engine_ObjectEventJump(QUARTERS_CREWMAN, 4, 40);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CREWMAN, 1);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  ObjectEvent_SetSpeed(QUARTERS_SECOND_GUIDE, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(QUARTERS_SECOND_GUIDE, 438, 659);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP,
                            0);
  Script_PlaySong(AUDIO_CUE_FADE_BGM);
  ObjectEvent_SetSpeed(QUARTERS_CREWMAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(QUARTERS_CREWMAN, 488, 686);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                            0);
  Engine_ObjectEventWaitForMove(QUARTERS_SECOND_GUIDE);
  Engine_ObjectEventSetAnimation(QUARTERS_SECOND_GUIDE, 1);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                            0);
  Map_PlayMusic();
  GameFlag_Set(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL);
}

/// In the captain's quarters at Kalay, using the Anchor Charm on the captain:
/// he has his lucky anchor back, Sean names the party to guard the oarsmen, and
/// the party agrees to protect the ship
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED).
static void TolbiShipInteriors_ReturnLuckyAnchor(void) {
  struct ObjectEvent *objectEvent;
  s32 captainSpeech;
  s32 seanSpeech;
  s32 value;
  s32 captainAboveSpeech;
  const s32 *action;

  // Let the captain explain why the anchor matters and ask the party to help.
  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  ObjectEvent_FaceObjectEvent(QUARTERS_CAPTAIN, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        40);
  captainSpeech = SPEAKER(QUARTERS_CAPTAIN, 1);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 3);
  Engine_ScriptSetMessage(MsgTolbiShipItsMyLuckyAnchor);
  TolbiShipInteriors_Say(captainSpeech);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_KAJA, 1);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SEAN, 1);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_OURANOS, 1);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SECOND_GUIDE, 1);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CREWMAN, 1);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SEAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, FACING_NORTHWEST + FACING_STEP,
                            20);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  Script_OpenMessage(captainSpeech, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 2);
    TolbiShipInteriors_Say(SPEAKER(QUARTERS_KAJA, 9));
    ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    TolbiShipInteriors_Say(captainSpeech);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 1);
    TolbiShipInteriors_Say(SPEAKER(QUARTERS_KAJA, 9));
    Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
    TolbiShipInteriors_Say(SPEAKER(QUARTERS_CAPTAIN, 9));
  }
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        40);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(636), 1);
  ObjectEvent_SetSpeed(QUARTERS_SECOND_GUIDE, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(QUARTERS_SECOND_GUIDE, 472, 662);
  TolbiShipInteriors_Turn(QUARTERS_SECOND_GUIDE,
                          FACING_NORTHWEST + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_KAJA, 3);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_EAST, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_WEST, 20);
  Engine_ObjectEventSetAnimation(QUARTERS_OURANOS, 3);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_SECOND_GUIDE, 3);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SEAN, 1);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, FACING_SOUTHEAST + FACING_STEP);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | QUARTERS_SEAN, 0, 20);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectEvent_ShowEmote(QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  TolbiShipInteriors_Say(QUARTERS_OURANOS);
  seanSpeech = SPEAKER(QUARTERS_SEAN, 9);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  Engine_ObjectEventSetAnimation(QUARTERS_SEAN, 4);
  TolbiShipInteriors_Say(seanSpeech);
  TolbiShipInteriors_Turn(QUARTERS_SECOND_GUIDE,
                          FACING_NORTHWEST + FACING_STEP);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SECOND_GUIDE, 1);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  ObjectEvent_ShowEmote(QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 1);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_SEAN, 3);
  TolbiShipInteriors_Say(seanSpeech);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 2);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, FACING_NORTH + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_SEAN, 3);
  TolbiShipInteriors_Say(seanSpeech);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 2);
  TolbiShipInteriors_Turn(QUARTERS_OURANOS, FACING_NORTHWEST + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_OURANOS);
  TolbiShipInteriors_Turn(QUARTERS_SEAN, 0);
  TolbiShipInteriors_Say(seanSpeech);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, FACING_NORTH + FACING_STEP,
                            0);
  // Bring the rest of the party out beside Isaac for the agreement.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 486, 608);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(GARET, 486, 624);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(IVAN, 486, 640);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(IVAN);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(MIA, 486, 656);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 20);
  ObjectEvent_ShowEmote(QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  TolbiShipInteriors_Say(seanSpeech);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 1);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_KAJA, 1));
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP);
  Script_OpenMessage(QUARTERS_CAPTAIN, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
    TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
    Engine_ObjectEventSetAnimationAndWait(QUARTERS_SEAN, 3);
    TolbiShipInteriors_Say(seanSpeech);
    Engine_ObjectEventStartRepeatedMotion(QUARTERS_KAJA, 1);
    Script_ShowMessageAndWait(SPEAKER(QUARTERS_KAJA, 9), 0, 40);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 3;
    Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 3);
    Script_ShowMessageAndWait(QUARTERS_CAPTAIN, 0, 40);
  }
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SECOND_GUIDE, 1);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  value = 176;
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SECOND_GUIDE, 1);
  TolbiShipInteriors_Turn(QUARTERS_SECOND_GUIDE, (value << 8));
  Script_ShowMessageAndWait(QUARTERS_SECOND_GUIDE, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  ObjectEvent_SetSpeed(QUARTERS_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  captainAboveSpeech = SPEAKER(QUARTERS_CAPTAIN, 4);
  ObjectEvent_WalkToAndWait(QUARTERS_CAPTAIN, 472, 632);
  TolbiShipInteriors_Say(captainAboveSpeech);
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_SECOND_GUIDE, 2);
  TolbiShipInteriors_Say(QUARTERS_SECOND_GUIDE);
  Engine_ObjectEventSetAnimation(QUARTERS_CAPTAIN, 4);
  Script_ShowMessageAndWait(captainAboveSpeech, 0, 40);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_OURANOS, 1);
  TolbiShipInteriors_Turn(QUARTERS_OURANOS, (value << 8));
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_OURANOS, 1));
  ObjectEvent_ShowEmote(QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  ObjectEvent_SetSpeed(QUARTERS_CREWMAN, PIXELS(12) / 5, PIXELS(6) / 5);
  Engine_ObjectEventJump(QUARTERS_CREWMAN, 2, 0);
  ObjectEvent_WalkToAndWait(QUARTERS_CREWMAN, 462, 674);
  TolbiShipInteriors_Turn(QUARTERS_CREWMAN, (value << 8));
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CREWMAN, 2);
  TolbiShipInteriors_Say(QUARTERS_CREWMAN);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_KAJA, 4);
  TolbiShipInteriors_Say(QUARTERS_KAJA);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(captainAboveSpeech);
  ObjectEvent_ShowEmote(QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessageAndWait(QUARTERS_SECOND_GUIDE, 0, 40);
  TolbiShipInteriors_Turn(QUARTERS_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_KAJA, 2);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_KAJA, 1));
  TolbiShipInteriors_Turn(QUARTERS_SEAN, 0);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(QUARTERS_KAJA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(QUARTERS_OURANOS, (value << 8), 0);
  ObjectEvent_FaceDirection(QUARTERS_SECOND_GUIDE, (value << 8), 0);
  ObjectEvent_FaceDirection(QUARTERS_CREWMAN, (value << 8), 20);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SEAN, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | QUARTERS_SEAN, 0, 20);
  ObjectEvent_ShowEmote(QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP);
  Script_OpenMessage(SPEAKER_PORTRAIT_LEFT | QUARTERS_CAPTAIN, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
    TolbiShipInteriors_Say(SPEAKER(QUARTERS_CAPTAIN, 1));
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    TolbiShipInteriors_Say(SPEAKER(QUARTERS_CAPTAIN, 1));
  }
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_CAPTAIN, 1));
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_WEST);
  TolbiShipInteriors_Say(SPEAKER(QUARTERS_CAPTAIN, 4));
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 0, 0);
  Engine_ObjectEventSetAnimation(QUARTERS_SEAN, 3);
  Engine_ObjectEventSetAnimation(QUARTERS_OURANOS, 3);
  Engine_ObjectEventSetAnimation(QUARTERS_KAJA, 3);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CREWMAN, 2);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_SECOND_GUIDE, 2);
  Engine_ScriptWait(20);
  // Send the crew away and merge the party's separate object events again.
  action = gTolbiShipInteriors_LeaveCaptainsQuarters;
  ObjectEvent_SetActionScript(QUARTERS_CREWMAN, action);
  Engine_ScriptWait(4);
  ObjectEvent_SetActionScript(QUARTERS_OURANOS, action);
  Engine_ScriptWait(4);
  ObjectEvent_SetActionScript(QUARTERS_SEAN, action);
  Engine_ScriptWait(4);
  ObjectEvent_SetActionScript(QUARTERS_KAJA, action);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(IVAN);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(MIA, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(MIA);
  ObjectEvent_SetPosition(MIA, 0, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(IVAN, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(IVAN);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  ObjectEvent_SetActionScript(QUARTERS_SECOND_GUIDE, action);
  ObjectEvent_WalkToAndWait(QUARTERS_CAPTAIN, 456, 648);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, 0);
  PartyInventory_Discard(ITEM_ANCHOR_CHARM);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED);
  Engine_ScriptEnd();
}

/// Restores the room or plays the script selected by this warp destination.
s32 TolbiShipInteriors_ApplyEntryState(void) {
  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gGameState.warpDest) {
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      Engine_ObjectEventSetChildValue(SEA_HOLD_SECOND_GUIDE, 2);
    } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ObjectEventSetChildValue(HOLD_SECOND_GUIDE, 2);
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
  case TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
    Engine_ObjectEventSetChildValue(ROWING_SECOND_GUIDE, 2);
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ObjectEventSetChildValue(QUARTERS_SECOND_GUIDE, 2);
    }
    break;
  }
  switch (gGameState.warpDest) {
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK:
    TolbiShipInteriors_SetUpHold();
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD:
    TolbiShipInteriors_SetUpRowingDeck();
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    TolbiShipInteriors_SetUpCaptainsQuarters();
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      TolbiShipInteriors_AnnounceDeparture();
    } else {
      TolbiShipInteriors_CastOff();
    }
    break;
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      TolbiShipInteriors_KajaCallsReplacementToOars();
    } else {
      TolbiShipInteriors_KajaCallsToOars();
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
    TolbiShipInteriors_ShowRowing();
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
    TolbiShipInteriors_AskAboutMonsters();
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
    TolbiShipInteriors_AnnounceTurn();
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      Engine_ScriptStart();
      TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 1,
                                         0);
      TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_SHIVERING_OARSMAN, 0, 0);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER,
                                  gTolbiShipInteriors_MonsterBob);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 1,
                                  gTolbiShipInteriors_MonsterBob);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 2,
                                  gTolbiShipInteriors_MonsterBob);
      Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER, 3);
      Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER + 1, 3);
      Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER + 2, 3);
      Engine_ScriptEnd();
    } else {
      TolbiShipInteriors_StartFirstAttack();
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
    TolbiShipInteriors_EndFirstAttack();
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      Engine_ScriptStart();
      TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 2,
                                         0);
      TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_SHIVERING_OARSMAN, 0, 0);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER,
                                  gTolbiShipInteriors_MonsterBob);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 1,
                                  gTolbiShipInteriors_MonsterBob);
      Engine_ScriptEnd();
    } else {
      TolbiShipInteriors_StartSecondAttack();
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
    TolbiShipInteriors_EndSecondAttack();
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      Engine_ScriptStart();
      TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 3,
                                         0);
      TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_SHIVERING_OARSMAN, 0, 0);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER,
                                  gTolbiShipInteriors_MonsterBob);
      ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 1,
                                  gTolbiShipInteriors_MonsterBob);
      Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER, 3);
      Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER + 1, 3);
      Engine_ScriptEnd();
    } else {
      TolbiShipInteriors_StartThirdAttack();
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
    TolbiShipInteriors_EndThirdAttack();
    break;
  case TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK) != 0) {
        gTolbiShipInteriors_ShakeTimer = 0;
        FieldScheduler_AddCallback(TolbiShipInteriors_ShakeWhileKrakenAboard,
                                   TASK_PRIORITY_MAP);
        Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 5);
      }
    } else {
      TolbiShipInteriors_NoticeShipRocking();
    }
    break;
  case TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
    TolbiShipInteriors_ShowCaptainsQuartersBeforeIsland();
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      TolbiShipInteriors_ReachIsland();
    }
    break;
  case TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
    TolbiShipInteriors_DismissReplacementRowers();
    break;
  case TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT:
    TolbiShipInteriors_RunSceneCommand(
        SHIP_CLEAR_FLAG_RANGE, FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT,
        FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_ROWING_SELECTIONS, 0, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SHIP_DEFEATED_AT_SEA);
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK);
    break;
  }
  return 0;
}

/// Restores the captain's quarters's object events after the anchor and
/// departure conversations.
static void TolbiShipInteriors_SetUpCaptainsQuarters(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *other;
  s32 done;
  s32 x;
  s32 heading;
  u16 zero;

  if (!Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    TolbiShipInteriors_PlaceAnchorCharm();
    return;
  }
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    TolbiShipInteriors_PlaceAnchorCharm();
  }
  done = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  if (done != 0) {
    return;
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND)) {
    objectEvent = ObjectEvent_GetById(QUARTERS_KAJA);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_CAPTAIN, 456, 652, 0);
    x = 0x1e0;
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_KAJA, x, 600,
                                        FACING_NORTHWEST + FACING_STEP);
    ObjectEvent_SetSpeed(QUARTERS_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
    objectEvent->work = done;
    zero = 0;
    objectEvent->riseEnabled = zero;
    objectEvent->collisionFlags |= 128;
    objectEvent->update = TolbiShipInteriors_PaceCaptainsQuarters;
    other = ObjectEvent_GetById(QUARTERS_CAPTAIN);
    other->riseCounter = zero;
    other->update = TolbiShipInteriors_WatchPacer;
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      TolbiShipInteriors_PlaceObjectEvent(LOCALID_PARTY_LEADER, x, 666,
                                          FACING_NORTHWEST);
    }
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_CAPTAIN, 444, 614,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_CAPTAIN, 456, 648, 0);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL)) {
    heading = FACING_NORTHWEST + FACING_STEP;
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_CAPTAIN, 475, 598,
                                        FACING_WEST);
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_KAJA, 462, 618, heading);
    ObjectEvent_GetById(QUARTERS_SEAN)->facing = FACING_SOUTHEAST + FACING_STEP;
    ObjectEvent_GetById(QUARTERS_OURANOS)->facing = heading;
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_SECOND_GUIDE, 438, 659,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_CREWMAN, 488, 688, heading);
  }
}

/// Restores the passenger hold after recruitment and departure scripts.
static void TolbiShipInteriors_SetUpHold(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    Engine_ObjectEventSetPosition(HOLD_WARRIOR, 0, 0);
    Engine_ObjectEventSetPosition(HOLD_TICKET_GRUMBLER, 0, 0);
    Engine_ObjectEventSetPosition(HOLD_MUSCLE_ADMIRER, 0, 0);
    Engine_ObjectEventSetPosition(HOLD_OLD_MAN, 0, 0);
    Engine_ObjectEventSetPosition(HOLD_SECOND_GUIDE, 0, 0);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_HEAD_GUIDE, 152, 444,
                                        FACING_SOUTHEAST + FACING_STEP);
    ObjectEvent_SetActionScript(SEA_HOLD_HEAD_GUIDE,
                                gTolbiShipInteriors_LookAroundScript);
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_SWEATING_PASSENGER, 184, 480,
                                        0xb000);
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_STARED_AT_PASSENGER, 170, 488,
                                        0xb000);
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_WARRIOR, 136, 488,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_MUSCLE_ADMIRER, 120, 480,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_TICKET_GRUMBLER, 184, 526,
                                        FACING_NORTHWEST + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(SEA_HOLD_LADY, 136, 584, FACING_WEST);
    ObjectEvent_SetActionScript(SEA_HOLD_LADY, gTolbiShipInteriors_Shiver);
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    TolbiShipInteriors_HideSelectedPassengers();
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0 &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    Engine_ObjectEventSetPosition(HOLD_SECOND_GUIDE, 0, 0);
  }
}

/// Restores the rowing deck and introduces a newly recruited replacement.
static void TolbiShipInteriors_SetUpRowingDeck(void) {
  Engine_TaskWait(1);
  TolbiShipInteriors_SetOarCollision();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_CREWMAN, 412, 222, 12288);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_DRUMMER, 458, 161, 32768);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      ObjectEvent_SetPosition(ROWING_CREWMAN, PIXELS(472), PIXELS(152));
      Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 5);
      TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
    } else {
      if (GameFlag_IsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
        TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
        TolbiShipInteriors_IntroduceReplacementOarsman(3);
      } else {
        if (GameFlag_IsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
          TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
          TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_FIRST_OARSMEN, 3, 0);
          TolbiShipInteriors_IntroduceReplacementOarsman(2);
        } else {
          if (GameFlag_IsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
            TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0,
                                               0);
            TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_FIRST_OARSMEN, 2, 0);
            TolbiShipInteriors_IntroduceReplacementOarsman(1);
          } else {
            if (GameFlag_IsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
              TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0,
                                                 0);
              ObjectEvent_SetPosition(ROWING_FIRST_OARSMAN, 0, 0);
              TolbiShipInteriors_IntroduceReplacementOarsman(0);
            } else {
              Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 5);
              if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
                  GameFlag_IsSet(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT) ==
                      0) {
                TolbiShipInteriors_AskForReplacementOarsmen();
              }
            }
          }
        }
      }
    }
  }
}

/// Sets collision bit 3 on the eight oars beside the rowing seats.
static void TolbiShipInteriors_SetOarCollision(void) {
  u32 localId;

  for (localId = ROWING_FIRST_OAR; localId <= ROWING_FIRST_OAR + 7; localId++) {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
    u32 flags = objectEvent->collisionFlags;
    objectEvent->collisionFlags = flags | 0x08;
  }
}

/// Puts each recruited rower at its seat and hides the original occupant.
static void TolbiShipInteriors_PlaceFoundObjectEvents(s32 gated) {
  s32 localId;

  if (gated &&
      !Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((localId = TolbiShipInteriors_FindFirstSetFlag(0, 0)) != 0) {
    TolbiShipInteriors_PlaceObjectEvent(localId, 410, 172,
                                        FACING_NORTH + FACING_STEP);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN, 0, 0);
  }
  if (gated && !Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((localId = TolbiShipInteriors_FindFirstSetFlag(1, 0)) != 0) {
    TolbiShipInteriors_PlaceObjectEvent(localId, 470, 172,
                                        FACING_NORTHWEST + FACING_STEP);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 1, 0, 0);
  }
  if (gated && !Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED)) {
    return;
  }
  if ((localId = TolbiShipInteriors_FindFirstSetFlag(2, 0)) != 0) {
    TolbiShipInteriors_PlaceObjectEvent(localId, 410, 204,
                                        FACING_NORTH + FACING_STEP);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 2, 0, 0);
  }
  if (gated) {
    return;
  }
  if ((localId = TolbiShipInteriors_FindFirstSetFlag(3, 0)) != 0) {
    TolbiShipInteriors_PlaceObjectEvent(localId, 470, 204,
                                        FACING_NORTHWEST + FACING_STEP);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 3, 0, 0);
  }
}

/// On the rowing deck, once the charm is back: Kaja tells the oarsmen the ship
/// sails without its replacement and asks the party to bring a passenger to row
/// whenever an oarsman falls, keeping the teams balanced; leads to the deck's
/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY
/// (FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT).
static void TolbiShipInteriors_AskForReplacementOarsmen(void) {
  struct ObjectEvent *objectEvent;
  s32 mode;

  // Bring Isaac to the crewman who explains the shortage of rowers.
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(168), 0x1000001);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_KAJA, 440, 164,
                                      FACING_SOUTH + FACING_STEP);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_CREWMAN, 428, 190,
                                      FACING_NORTH + FACING_STEP);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_DRUMMER, 452, 190,
                                      FACING_NORTHWEST + FACING_STEP);
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 1);
  mode = 128;
  TolbiShipInteriors_PlaceObjectEvent(LOCALID_PARTY_LEADER, 440, 134,
                                      FACING_WEST);
  gFieldWork->transition = (mode << 1);
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 408, 134);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 408, 148);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 424, 148);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  Engine_ScriptSetMessage(MsgTolbiShipOurReplacementNeverArrivedBut);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventRunRepeatedMotion(ROWING_CREWMAN, 1);
  TolbiShipInteriors_Say(ROWING_CREWMAN);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  TolbiShipInteriors_Say(ROWING_KAJA);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_NORTH + FACING_STEP);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(GARET, 440, 148);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  objectEvent = ObjectEvent_GetById(GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(IVAN, 456, 148);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTH, 0);
  objectEvent = ObjectEvent_GetById(IVAN);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(MIA, 472, 148);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 20);
  // Let the full party respond to the request for replacement oarsmen.
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 60);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x4000, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTH + FACING_STEP, 20);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventStartRepeatedMotion(ROWING_DRUMMER, 1);
  ObjectEvent_ShowEmote(ROWING_DRUMMER, (mode << 1), 40);
  TolbiShipInteriors_Say(ROWING_DRUMMER);
  Engine_ObjectEventStartRepeatedMotion(GARET, 3);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventRunRepeatedMotion(ROWING_FIRST_OARSMAN, 1);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN, 3);
  TolbiShipInteriors_Say(ROWING_FIRST_OARSMAN);
  Engine_ObjectEventSetAnimation(ROWING_CREWMAN, 3);
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 3);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 1, 3);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 2, 3);
  Engine_ObjectEventSetAnimationAndWait(ROWING_FIRST_OARSMAN + 3, 3);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x4000, 20);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 4);
  TolbiShipInteriors_Say(ROWING_KAJA);
  ObjectEvent_ShowEmote(ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventStartRepeatedMotion(ROWING_CREWMAN, 1);
  TolbiShipInteriors_Say(ROWING_CREWMAN);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  TolbiShipInteriors_Say(ROWING_KAJA);
  ObjectEvent_FaceDirection(ROWING_CREWMAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(ROWING_DRUMMER, FACING_WEST, 40);
  ObjectEvent_ShowEmote(ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  Engine_ObjectEventSetAnimation(ROWING_KAJA, 3);
  Script_ShowMessageAndWait(ROWING_KAJA, 0, 20);
  Engine_ObjectEventSetAnimation(ROWING_CREWMAN, 3);
  Engine_ObjectEventSetAnimationAndWait(ROWING_DRUMMER, 3);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(ROWING_DRUMMER, (mode << 1), 20);
  TolbiShipInteriors_Turn(ROWING_DRUMMER, FACING_NORTHWEST + FACING_STEP);
  TolbiShipInteriors_Say(ROWING_DRUMMER);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_ShowEmote(ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(ROWING_KAJA, 0, 60);
  ObjectEvent_ShowEmote(ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_NORTHWEST + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  TolbiShipInteriors_Say(ROWING_KAJA);
  TolbiShipInteriors_RunSceneCommand(SHIP_SHOW_PARTY_EMOTE, 2, 80);
  TolbiShipInteriors_Turn(ROWING_CREWMAN, FACING_NORTH + FACING_STEP);
  Engine_ObjectEventStartRepeatedMotion(ROWING_CREWMAN, 2);
  TolbiShipInteriors_Say(ROWING_CREWMAN);
  Engine_ObjectEventSetAnimationAndWait(ROWING_DRUMMER, 3);
  Engine_ObjectEventStartRepeatedMotion(ROWING_DRUMMER, 2);
  TolbiShipInteriors_Say(ROWING_DRUMMER);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  TolbiShipInteriors_Say(ROWING_KAJA);
  ObjectEvent_SetSpeed(ROWING_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 158);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 148);
  ObjectEvent_FaceDirection(ROWING_KAJA, FACING_EAST, 20);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  TolbiShipInteriors_Say(ROWING_KAJA);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x8000, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 134);
  ObjectEvent_WalkTo(ROWING_KAJA, 440, 134);
  Engine_ScriptWait(40);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 10, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT);
}

/// Raises the anchor charm and announces the ship's first departure.
static void TolbiShipInteriors_CastOff(void) {
  struct ObjectEvent *anchorCharm;

  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 0, 1);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(QUARTERS_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(QUARTERS_CAPTAIN, 468, 614);
  ObjectEvent_WalkToAndWait(QUARTERS_CAPTAIN, 472, 596);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_WEST, 20);
  Engine_ObjectEventJump(QUARTERS_CAPTAIN, 4, 20);
  anchorCharm = TolbiShipInteriors_PlaceAnchorCharm();
  Engine_ScriptWait(20);
  Script_PlaySong(SE_214);
  Engine_ObjectSetScript(anchorCharm, gTolbiShipInteriors_AnchorCharmPulse);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(QUARTERS_CAPTAIN, 466, 624);
  TolbiShipInteriors_Turn(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
  Engine_ScriptSetMessage(MsgTolbiShipCastOff);
  Script_ShowMessageAndWait(QUARTERS_CAPTAIN, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 11, 0);
}

/// Announces departure after a replacement rower has joined the crew.
static void TolbiShipInteriors_AnnounceDeparture(void) {
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_SOUTH + FACING_STEP, 40);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
  Engine_ScriptSetMessage(MsgTolbiShipWereOff);
  Script_ShowMessageAndWait(QUARTERS_CAPTAIN, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 11, 0);
}

/// Kaja comes into the hold and calls the first passenger to the rowing deck.
/// "Row those oars!"
static void TolbiShipInteriors_KajaCallsToOars(void) {
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(HOLD_KAJA, PIXELS(150), PIXELS(586));
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCREEN, 0, 0);
  ObjectEvent_SetSpeed(HOLD_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(HOLD_KAJA, 168, 578);
  ObjectEvent_WalkToAndWait(HOLD_KAJA, 168, 554);
  ObjectEvent_FaceDirection(HOLD_KAJA, FACING_WEST, 20);
  Engine_ObjectEventStartRepeatedMotion(HOLD_KAJA, 2);
  Engine_ScriptSetMessage(MsgTolbiShipRowThoseOars);
  Script_ShowMessageAndWait(HOLD_KAJA, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 12, 0);
}

/// Kaja comes into the hold and calls a replacement rower back to the rowing
/// deck.
static void TolbiShipInteriors_KajaCallsReplacementToOars(void) {
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  TolbiShipInteriors_HideSelectedPassengers();
  ObjectEvent_SetPosition(SEA_HOLD_KAJA, PIXELS(150), PIXELS(586));
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCREEN, 0, 0);
  ObjectEvent_SetSpeed(SEA_HOLD_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(SEA_HOLD_KAJA, 168, 578);
  ObjectEvent_WalkToAndWait(SEA_HOLD_KAJA, 168, 554);
  ObjectEvent_FaceDirection(SEA_HOLD_KAJA, FACING_WEST, 20);
  Engine_ObjectEventStartRepeatedMotion(SEA_HOLD_KAJA, 2);
  Engine_ScriptSetMessage(MsgTolbiShipRowThoseOars);
  Script_ShowMessageAndWait(SEA_HOLD_KAJA, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 12, 0);
}

/// Restores the rowing deck for the current set of selected passengers.
static void TolbiShipInteriors_ShowRowing(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 5);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_OARS, 0, 0);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(0);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCREEN, 1, 20);
  Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Engine_CameraMoveTo(0x1b80000, -1, 0xb00000, 1);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 7);
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(SE_188);
  Engine_ScriptWait(30);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(16);
  Engine_ScriptWait(0x50);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(0);
  Engine_ScriptWait(0x3c);
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 7);
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(SE_188);
  Engine_ScriptWait(30);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(16);
  Engine_ScriptWait(0x50);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(0);
  Engine_ScriptWait(0x5a);
  Engine_AudioPlayCue(SE_188);
  Engine_ScriptWait(30);

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);

  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 0, 0);

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) !=
             0) {
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
             0) {
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
             0) {
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK);
  } else {
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK);
  }
}

/// Asks the crew about the monsters before returning belowdecks.
static void TolbiShipInteriors_AskAboutMonsters(void) {
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                            20);
  Engine_ObjectEventStartRepeatedMotion(QUARTERS_CAPTAIN, 2);
  Engine_ScriptSetMessage(MsgTolbiShipMonsters);
  Script_ShowMessageAndWait(QUARTERS_CAPTAIN, 0, 20);
  TolbiShipInteriors_RunSceneCommand(
      SHIP_CLOSE_SCREEN,
      TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS, 0);
}

/// Announces the ship's next turn and returns control to the rowing deck.
static void TolbiShipInteriors_AnnounceTurn(void) {
  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_CAPTAIN, 1);
  Engine_ScriptSetMessage(MsgTolbiShipImTurning);
  TolbiShipInteriors_Say(QUARTERS_CAPTAIN);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_NORTH + FACING_STEP, 40);
  TolbiShipInteriors_RunSceneCommand(
      SHIP_CLOSE_SCREEN,
      TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD, 0);
}

/// Stages the first monster encounter with the original rowers.
static void TolbiShipInteriors_StartFirstAttack(void) {
  const s32 *entryScript;

  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 1, 0);
  TolbiShipInteriors_SetOarCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                                     ROWING_FIRST_OARSMAN + 1,
                                     ROWING_FIRST_OARSMAN + 2);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN, 6);
  ObjectEvent_SetActionScript(ROWING_FIRST_OARSMAN + 2,
                              gTolbiShipInteriors_Shiver);
  entryScript = gTolbiShipInteriors_MonsterBob;
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER, entryScript);
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 1, entryScript);
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 2, entryScript);
  Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER, 3);
  Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER + 1, 3);
  Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER + 2, 3);
  TolbiShipInteriors_ReportMonstersBelowdecks();
  Engine_ScriptEnd();
}

/// Restores the original rowers after the first monster attack.
static void TolbiShipInteriors_EndFirstAttack(void) {
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                                     ROWING_FIRST_OARSMAN + 1,
                                     ROWING_FIRST_OARSMAN + 2);
  TolbiShipInteriors_ReactToMonsterAttack();
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  Engine_ScriptEnd();
}

/// Introduces the selected passenger and leads them to the open rowing seat.
static void TolbiShipInteriors_IntroduceReplacementOarsman(s32 seat) {
  s32 rowerId;
  struct ObjectEvent *objectEvent;
  s32 halfStep;
  s32 leftSeat;
  s32 x;

  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 0, 0);
  x = 0x1b0;
  halfStep = PIXELS(1) / 2;
  TolbiShipInteriors_PlaceObjectEvent(LOCALID_PARTY_LEADER, x, 134,
                                      FACING_WEST);
  TolbiShipInteriors_PlaceFoundObjectEvents(1);
  Engine_TaskWait(1);
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 406, 134);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 406, 152);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 421, 152);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceEachOther(ROWING_KAJA, LOCALID_PARTY_LEADER, 10);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_FindFirstSetFlag(seat, 0);
    Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
    Engine_ScriptWait(20);
    Engine_ObjectEventFaceEachOther(ROWING_KAJA, LOCALID_PARTY_LEADER, 10);
    Engine_ScriptSetMessage(MsgTolbiShipBackBroughtOarsman);
    TolbiShipInteriors_Say(KAJA_ROWING_SPEAKER);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    leftSeat = 0;
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), halfStep);
    Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, x, 168);
    Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetPosition(rowerId, objectEvent->x.fixed,
                                    objectEvent->z.fixed);
    }
    ObjectEvent_SetSpeed(rowerId, PIXELS(1), halfStep);
    ObjectEvent_WalkToAndWait(rowerId, 448, 168);
    ObjectEvent_FaceDirection(rowerId, FACING_NORTHWEST + FACING_STEP, 20);
    ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ObjectEventSetAnimation(ROWING_KAJA, 3);
    TolbiShipInteriors_Say(ROWING_KAJA);
    ObjectEvent_ShowEmote(rowerId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
    Engine_ObjectEventSetAnimation(ROWING_KAJA, 3);
    TolbiShipInteriors_Say(ROWING_KAJA);
    Engine_ObjectEventSetAnimationAndWait(rowerId, 3);
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
      ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 0);
      ObjectEvent_WalkToAndWait(rowerId, 470, 204);
      TolbiShipInteriors_Turn(rowerId, FACING_NORTHWEST + FACING_STEP);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) !=
               0) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 422, 154);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
      ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTH + FACING_STEP, 0);
      ObjectEvent_WalkToAndWait(rowerId, 410, 204);
      leftSeat = 1;
      TolbiShipInteriors_Turn(rowerId, FACING_NORTH + FACING_STEP);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
               0) {
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
      ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 0);
      ObjectEvent_WalkToAndWait(rowerId, 470, 172);
      TolbiShipInteriors_Turn(rowerId, FACING_NORTHWEST + FACING_STEP);
    } else {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 422, 154);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
      ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTH + FACING_STEP, 0);
      ObjectEvent_WalkToAndWait(rowerId, 410, 172);
      leftSeat = 1;
      TolbiShipInteriors_Turn(rowerId, FACING_NORTH + FACING_STEP);
    }
    Engine_ObjectEventFaceEachOther(ROWING_KAJA, LOCALID_PARTY_LEADER, 20);
    TolbiShipInteriors_Say(SPEAKER(ROWING_KAJA, 2));
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
    ObjectEvent_SetSpeed(ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
    if (leftSeat != 0) {
      ObjectEvent_WalkToAndWait(ROWING_KAJA, 428, 164);
      ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 164);
    }
    ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 134);
    Engine_ObjectEventWalkTo(ROWING_KAJA, 0x1b8, 134);
    Engine_ScriptWait(40);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 10, 0);
  } else {
    Engine_ScriptSetMessage(MsgTolbiShipBack);
    Script_ShowMessageAndWait(KAJA_ROWING_SPEAKER, 0, 40);
    Engine_ObjectEventShowEmote(ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION,
                                60);
    TolbiShipInteriors_Say(KAJA_ROWING_SPEAKER);
    Engine_ObjectEventSetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    ObjectEvent_ShowEmote(ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ObjectEventStartRepeatedMotion(ROWING_KAJA, 2);
    TolbiShipInteriors_Say(KAJA_ROWING_SPEAKER);
    Engine_ObjectEventShowEmote(ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                                40);
    TolbiShipInteriors_Say(KAJA_ROWING_SPEAKER);
    Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 4);
    TolbiShipInteriors_Say(KAJA_ROWING_SPEAKER);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ScriptWait(20);
    Engine_ScriptRequestWarp(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD);
  }
}

/// Creates the visible anchor charm once a visit (FLAG_MAP_TEMP_00 records
/// it) and retains it for departure scripts.
static struct ObjectEvent *TolbiShipInteriors_PlaceAnchorCharm(void) {
  s32 set = GameFlag_IsSet(FLAG_MAP_TEMP_00);
  struct ObjectEvent *objectEvent;
  struct AnimationObject *sprite;
  u8 *buffer;

  if (set != 0) {
    return gTolbiShipInteriors_AnchorObject;
  }
  objectEvent =
      Engine_ObjectCreate(SPRITE_BLANK, 0x1c70000, 0x40000, 0x2200000);
  objectEvent->motionFlags = set;
  objectEvent->retainSpriteCache = 1;
  sprite = objectEvent->sprite;
  sprite->count = set;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  buffer = Engine_HeapAllocate(17, 0x608);
  Engine_ItemLoadIcon(ITEM_ANCHOR_CHARM);
  Engine_VramLoad(sprite->slot, 128, buffer + 0x400);
  Engine_HeapRelease(17);
  Engine_GameFlagSet(FLAG_MAP_TEMP_00);
  gTolbiShipInteriors_AnchorObject = objectEvent;
  return objectEvent;
}

/// Restores each replacement rower or the original occupant of that seat.
static void TolbiShipInteriors_InstallRowingDeckObjectEvents(u32 action) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(0, 0);
    Engine_ObjectEventSetPosition(rowerId, PIXELS(410), PIXELS(172));
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION,
                                       ROWING_FIRST_OARSMAN, action);
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(1, 0);
    Engine_ObjectEventSetPosition(rowerId, PIXELS(470), PIXELS(172));
    ObjectEvent_GetById(rowerId)->scaleX = -Q16_ONE;
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 1, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                                       ROWING_FIRST_OARSMAN + 1, action);
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(2, 0);
    Engine_ObjectEventSetPosition(rowerId, PIXELS(410), PIXELS(204));
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 2, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION,
                                       ROWING_FIRST_OARSMAN + 2, action);
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(3, 0);
    Engine_ObjectEventSetPosition(rowerId, PIXELS(470), PIXELS(204));
    ObjectEvent_GetById(rowerId)->scaleX = -Q16_ONE;
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 3, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                                       ROWING_FIRST_OARSMAN + 3, action);
  }

  TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION,
                                     ROWING_FIRST_OARSMAN + 4, action);
  TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                                     ROWING_FIRST_OARSMAN + 5, action);
  TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION,
                                     ROWING_FIRST_OARSMAN + 6, action);
  TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                                     ROWING_FIRST_OARSMAN + 7, action);
}

/// Applies the seated pose and rowing action assigned to this rower.
static void TolbiShipInteriors_PoseObjectEvent(s32 id) {
  switch (id) {
  case ROWING_SECOND_GUIDE:
    Engine_ObjectEventSetAnimation(id, 6);
    ObjectEvent_SetActionById(id, 8);
    break;
  case ROWING_HEAD_GUIDE:
  case ROWING_SWEATING_PASSENGER:
    Engine_ObjectEventSetAnimation(id, 5);
    ObjectEvent_SetActionById(id, 16);
    break;
  case ROWING_STARED_AT_PASSENGER:
  case ROWING_WARRIOR:
    Engine_ObjectEventSetAnimation(id, 5);
    ObjectEvent_SetActionById(id, 20);
    break;
  case ROWING_TICKET_GRUMBLER:
    Engine_ObjectEventSetAnimation(id, 10);
    ObjectEvent_SetActionById(id, 8);
    break;
  case ROWING_LADY:
  case ROWING_MUSCLE_ADMIRER:
    Engine_ObjectEventSetAnimation(id, 5);
    ObjectEvent_SetActionById(id, 4);
    break;
  case ROWING_OLD_MAN:
    Engine_ObjectEventSetAnimation(id, 9);
    ObjectEvent_SetActionById(id, 4);
    break;
  }
}

static void TolbiShipInteriors_Say(s32 speaker) {
  Engine_ScriptShowMessage(speaker, 0);
  Engine_ScriptWait(10);
}

/// Turns an object event to a facing over 10 frames.
static void TolbiShipInteriors_Turn(s32 localId, u16 facing) {
  Engine_ObjectEventFaceDirection(localId, facing, 10);
}

/// Places an object event at pixel coordinates and sets its initial heading.
static void TolbiShipInteriors_PlaceObjectEvent(s32 localId, s32 x, s32 z,
                                                s32 facing) {
  Engine_ObjectEventSetPosition(localId, x << 16, z << 16, facing);
  ObjectEvent_GetById(localId)->facing = facing;
}

/// Moves the camera, optionally waits or redraws, then honors the packed delay.
static void TolbiShipInteriors_MoveCameraAndWait(s32 x, s32 y, s32 z,
                                                 u32 flags) {
  u32 selected;

  Engine_CameraMoveTo(x, y, z, ~flags & 1);
  selected = flags & 0x1111;
  if ((flags & 0x10000000) != 0)
    Engine_CameraWaitForMove();
  if ((flags & 0x01000000) != 0)
    Engine_MapGridRedraw();
  Engine_ScriptWait(selected);
}

/// Reuses the voyage's object event, camera, transition and selection
/// operations. Each command names how it interprets the two arguments below.
static void TolbiShipInteriors_RunSceneCommand(s32 step, u32 arg, u32 opt) {
  u32 slot;

  switch (step) {
  case SHIP_FACE_PARTY_EAST_AND_WEST:
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
    Engine_ObjectEventFaceDirection(IVAN, 0, 0);
    ObjectEvent_FaceDirection(MIA, FACING_WEST, opt);
    break;
  case SHIP_FACE_PARTY:
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, arg, 0);
    ObjectEvent_FaceDirection(GARET, arg, 0);
    ObjectEvent_FaceDirection(IVAN, arg, 0);
    ObjectEvent_FaceDirection(MIA, arg, opt);
    break;
  case SHIP_PLAY_PARTY_ANIMATION:
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
    if (arg != 0) {
      ObjectMotion_WaitForAnimationChange(3);
    }
    if (opt == 0) {
      break;
    }
    Engine_ScriptWait(opt);
    break;
  case SHIP_SHOW_PARTY_EMOTE:
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                             ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                            ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(opt);
    break;
  case SHIP_HIDE_FIRST_OARSMEN:
    for (slot = 0; slot < arg; slot++) {
      ObjectEvent_SetPosition(slot + ROWING_FIRST_OARSMAN, 0, 0);
    }
    break;
  case SHIP_START_ROWING_ANIMATION: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
  }
    Engine_ObjectEventSetAnimation(arg, 5);
    ObjectEvent_SetActionById(arg, opt);
    break;
  case SHIP_START_MIRRORED_ROWING_ANIMATION: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
    objectEvent->scaleX = -Q16_ONE;
  }
    Engine_ObjectEventSetAnimation(arg, 5);
    ObjectEvent_SetActionById(arg, opt);
    break;
  case SHIP_SET_SELECTED_ROWER_POSE: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
  }
    TolbiShipInteriors_PoseObjectEvent(arg);
    if (opt == 0) {
      ObjectEvent_SetActionById(arg, 0);
    }
    break;
  case SHIP_OPEN_SCREEN:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    Engine_ScriptOpenScreen();
    if (arg != 0) {
      Engine_ScriptWaitForScreen();
    }
    Engine_ScriptWait(0);
    break;
  case SHIP_CLOSE_SCREEN:
    Engine_ScriptCloseScreen();
    Engine_ScriptWaitForScreen();
    if (arg == 0) {
      break;
    }
    Engine_ScriptRequestWarp(arg);
    break;
  case SHIP_SETUP_ROWING_PARTY:
    TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
    TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
    TolbiShipInteriors_PlaceFoundObjectEvents(0);
    TolbiShipInteriors_PlaceObjectEvent(LOCALID_PARTY_LEADER, 432, 168,
                                        FACING_SOUTH);
    TolbiShipInteriors_PlaceObjectEvent(GARET, 448, 168, FACING_SOUTH);
    TolbiShipInteriors_PlaceObjectEvent(IVAN, 424, 152, FACING_SOUTH);
    TolbiShipInteriors_PlaceObjectEvent(MIA, 458, 152, FACING_SOUTH);
    break;
  case SHIP_RESTORE_ROWING_DECK_POSES:
    if (arg != 0) {
      struct ObjectEvent *objectEvent;

      Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 3, 1);
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 3);
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 3);
      objectEvent->scaleX = Q16_ONE;
    }
    {
      struct ObjectEvent *objectEvent;

      Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 4, 1);
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 4);
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
      Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 5, 1);
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 5);
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 5);
      objectEvent->scaleX = Q16_ONE;
      Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 6, 1);
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 6);
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
      Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN + 7, 1);
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 7);
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
      objectEvent = ObjectEvent_GetById(ROWING_FIRST_OARSMAN + 7);
      objectEvent->scaleX = Q16_ONE;
    }
    ObjectEvent_SetPosition(ROWING_FIRST_OAR, PIXELS(410), PIXELS(174));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 1, PIXELS(470), PIXELS(174));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 2, PIXELS(410), PIXELS(206));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 3, PIXELS(470), PIXELS(206));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 4, PIXELS(410), PIXELS(286));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 5, PIXELS(470), PIXELS(286));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 6, PIXELS(410), PIXELS(316));
    ObjectEvent_SetPosition(ROWING_FIRST_OAR + 7, PIXELS(470), PIXELS(316));
    Engine_TaskWait(1);
    if (arg != 0) {
      ObjectEvent_FaceDirection(ROWING_FIRST_OARSMAN + 3,
                                FACING_NORTHWEST + FACING_STEP, 0);
    }
    ObjectEvent_FaceDirection(ROWING_FIRST_OARSMAN + 4,
                              FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(ROWING_FIRST_OARSMAN + 5,
                              FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(ROWING_FIRST_OARSMAN + 6,
                              FACING_NORTH + FACING_STEP, 0);
    TolbiShipInteriors_Turn(ROWING_FIRST_OARSMAN + 7,
                            FACING_NORTHWEST + FACING_STEP);
    break;
  case SHIP_RESTORE_ROWER_POSE: {
    struct ObjectEvent *objectEvent;

    objectEvent = ObjectEvent_GetById(arg);
    Engine_ObjectEventSetAnimation(arg, 1);
    if (opt != 0) {
      objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
    } else {
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
    }
    objectEvent->scaleX = Q16_ONE;
  } break;
  case SHIP_HIDE_QUARTERS_CREW:
    ObjectEvent_SetPosition(QUARTERS_KAJA, 0, 0);
    ObjectEvent_SetPosition(QUARTERS_SEAN, 0, 0);
    ObjectEvent_SetPosition(QUARTERS_OURANOS, 0, 0);
    ObjectEvent_SetPosition(QUARTERS_SECOND_GUIDE, 0, 0);
    ObjectEvent_SetPosition(QUARTERS_CREWMAN, 0, 0);
    break;
  case SHIP_HIDE_OURANOS_AND_SEAN:
    ObjectEvent_SetPosition(HOLD_SEAN, 0, 0);
    ObjectEvent_SetPosition(HOLD_OURANOS, 0, 0);
    break;
  case SHIP_SETUP_CAPTAINS_QUARTERS:
    TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
    ObjectEvent_SetPosition(QUARTERS_KAJA, 0, 0);
    ObjectEvent_SetPosition(QUARTERS_CREWMAN, 0, 0);
    TolbiShipInteriors_PlaceObjectEvent(QUARTERS_CAPTAIN, 444, 614,
                                        FACING_NORTH + FACING_STEP);
    ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
    if (arg != 0) {
      TolbiShipInteriors_PlaceAnchorCharm();
    }
    TolbiShipInteriors_MoveCameraAndWait(PIXELS(448), PIXELS(32), PIXELS(624),
                                         0x1000001);
    if (opt == 0) {
      break;
    }
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(20);
    break;
  case SHIP_SETUP_RECRUITMENT_DECK:
    ObjectEvent_SetPosition(ROWING_CREWMAN, 0, 0);
    ObjectEvent_SetPosition(ROWING_DRUMMER, 0, 0);
    ObjectEvent_SetPosition(ROWING_KAJA, PIXELS(438), PIXELS(152));
    break;
  case SHIP_HIDE_OARS:
    slot = 0;
    do {
      ObjectEvent_SetPosition(slot + ROWING_FIRST_OAR, 0, 0);
      slot++;
    } while (slot <= 7);
    break;
  case SHIP_SETUP_PASSENGER_HOLD:
    TolbiShipInteriors_PlaceObjectEvent(HOLD_SECOND_GUIDE, 152, 532,
                                        FACING_NORTHWEST + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(HOLD_WARRIOR, 134, 490,
                                        FACING_SOUTHEAST + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(HOLD_TICKET_GRUMBLER, 166, 490,
                                        FACING_SOUTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(HOLD_MUSCLE_ADMIRER, 182, 504,
                                        FACING_SOUTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(HOLD_OLD_MAN, 118, 504,
                                        FACING_SOUTHEAST + FACING_STEP);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    break;
  case SHIP_SETUP_MONSTER_ENCOUNTER:
    TolbiShipInteriors_PlaceObjectEvent(ROWING_CREWMAN, 416, 328, 0);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_DRUMMER, 448, 352,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN, 454, 248,
                                        FACING_SOUTHEAST + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(arg, 408, 290, 0);
    TolbiShipInteriors_PlaceObjectEvent(opt, 408, 342, 0);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN + 3, 420, 356,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN + 4, 408, 304, 0);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN + 5, 418, 378,
                                        FACING_NORTH + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN + 6, 440, 262,
                                        FACING_SOUTHEAST + FACING_STEP);
    TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN + 7, 448, 378,
                                        FACING_NORTH + FACING_STEP);
    break;
  case SHIP_CLEAR_FLAG_RANGE:
    for (slot = arg; slot <= opt; slot++) {
      GameFlag_Clear(slot);
    }
    break;
  case SHIP_CLEAR_ROWING_SELECTIONS:
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_FLAG_RANGE,
                                       FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                       FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 8);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_FLAG_RANGE,
                                       FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                       FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 8);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_FLAG_RANGE,
                                       FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS,
                                       FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 8);
    GameFlag_Clear(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
    GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
    break;
  case SHIP_RESTORE_SHIVERING_OARSMAN:
    Engine_TaskWait(1);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
    ObjectEvent_SetActionScript(ROWING_FIRST_OARSMAN + 2,
                                gTolbiShipInteriors_Shiver);
    break;
  case SHIP_HIDE_PARTY_FOLLOWERS:
    Engine_ObjectEventDestroy(GARET);
    Engine_ObjectEventDestroy(IVAN);
    Engine_ObjectEventDestroy(MIA);
    break;
  case SHIP_RESET_CAMERA:
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    if (arg != 0) {
      FieldCamera_GetScriptObject()->motionFlags = 0;
    }
    break;
  case SHIP_PLACE_FOLLOWERS_AND_MONSTERS:
    FieldScript_CallWithLastActiveObjectId(
        gTolbiShipInteriors_PartyFollowerObjectEventTemplates);
    Engine_TaskWait(1);
    if (arg == 1) {
      FieldScript_CallWithLastActiveObjectId(
          gTolbiShipInteriors_FirstAttackMonsters);
      Engine_TaskWait(1);
    } else if (arg == 2) {
      FieldScript_CallWithLastActiveObjectId(
          gTolbiShipInteriors_SecondAttackMonsters);
      Engine_TaskWait(1);
    } else if (arg == 3) {
      FieldScript_CallWithLastActiveObjectId(
          gTolbiShipInteriors_ThirdAttackMonsters);
      Engine_TaskWait(1);
    }
    break;
  }
}

/// Finds the selected passenger or rowing-deck object event for a seat's flag
/// group.
static s32 TolbiShipInteriors_FindFirstSetFlag(u32 group, s32 alternate) {
  s32 flag = 0;
  s32 id = SEA_HOLD_FIRST_PASSENGER;
  u32 i;

  if (alternate == 0) {
    id = ROWING_FIRST_PASSENGER;
  }
  switch (group) {
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
    if (Engine_GameFlagIsSet(flag) != 0) {
      return id;
    }
  }
  return 0;
}

/// Hides the four passengers who are already filling rowing seats.
static void TolbiShipInteriors_HideSelectedPassengers(void) {
  TolbiShipInteriors_HideSelectedPassenger(SEA_HOLD_FIRST_PASSENGER,
                                           FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS);
  TolbiShipInteriors_HideSelectedPassenger(SEA_HOLD_FIRST_PASSENGER,
                                           FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  TolbiShipInteriors_HideSelectedPassenger(SEA_HOLD_FIRST_PASSENGER,
                                           FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS);
  TolbiShipInteriors_HideSelectedPassenger(SEA_HOLD_FIRST_PASSENGER,
                                           FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
}

/// Hides the passenger whose selection flag is set in this seat's flag group.
static void TolbiShipInteriors_HideSelectedPassenger(s32 localId,
                                                     s32 firstFlag) {
  unsigned int i = 0;

  do {
    if (Engine_GameFlagIsSet(firstFlag) != 0) {
      Engine_ObjectEventSetPosition(localId, 0, 0);
      break;
    }
    i++;
    localId++;
    firstFlag++;
  } while (i <= 8);
}

/// Stages the next monster encounter with one replacement rower.
static void TolbiShipInteriors_StartSecondAttack(void) {
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 2, 0);
  TolbiShipInteriors_SetOarCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                                     ROWING_FIRST_OARSMAN + 2);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN, 6);
  rowerScript = gTolbiShipInteriors_Shiver;
  ObjectEvent_SetActionScript(firstRower, rowerScript);
  Engine_ObjectEventDestroy(ROWING_FIRST_OARSMAN + 1);
  ObjectEvent_SetActionScript(ROWING_FIRST_OARSMAN + 2, rowerScript);
  entryScript = gTolbiShipInteriors_MonsterBob;
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER, entryScript);
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 1, entryScript);
  TolbiShipInteriors_ReportMonstersBelowdecks();
  Engine_ScriptEnd();
}

/// Gathers the party after the crew reports monsters belowdecks.
static void TolbiShipInteriors_ReportMonstersBelowdecks(void) {
  struct ObjectEvent *objectEvent;
  s32 message;

  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(176), 0x1000001);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(440), PIXELS(134));
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 5);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 0x198, 134);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 0x198, 152);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 0x1b0, 166);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(GARET);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(IVAN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkTo(IVAN, 424, 152);
  ObjectEvent_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkTo(GARET, 448, 168);
  ObjectEvent_SetSpeed(MIA, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkToAndWait(MIA, 458, 152);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_STAND);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 40);
  Ui_SetMessageVoiceFromObjectEvent(ROWING_FIRST_OARSMAN);
  message = MsgTolbiShipMonstersBelowdecks;
  UiText_ShowCenteredMessage(message, 1, 10);
  Engine_ScriptWait(10);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x4000, 20);
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(320),
                                       0x10000014);
  Engine_ObjectEventRunRepeatedMotion(ROWING_CREWMAN, 2);
  TolbiShipInteriors_Turn(ROWING_CREWMAN, FACING_NORTH + FACING_STEP);
  Engine_ScriptSetMessage(message + 1);
  TolbiShipInteriors_Say(ROWING_CREWMAN);
  ObjectEvent_FaceDirection(ROWING_CREWMAN, FACING_EAST, 20);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(134),
                                       0x10000000);
  ObjectEvent_SetActionScript(GARET,
                              gTolbiShipInteriors_FollowersRejoinByStairs);
  ObjectEvent_SetActionScript(IVAN,
                              gTolbiShipInteriors_FollowersRejoinByStairs);
  ObjectEvent_RunActionScriptAndWait(
      MIA, gTolbiShipInteriors_FollowersRejoinByStairs);
  Engine_ScriptWait(40);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
}

/// The crew checks on the party after the monster fight, then resumes rowing.
static void TolbiShipInteriors_ReactToMonsterAttack(void) {
  const s32 *action;

  TolbiShipInteriors_PlaceObjectEvent(LOCALID_PARTY_LEADER, 444, 300, 0);
  TolbiShipInteriors_PlaceObjectEvent(GARET, 458, 310, 0);
  TolbiShipInteriors_PlaceObjectEvent(IVAN, 444, 330, 0);
  TolbiShipInteriors_PlaceObjectEvent(MIA, 432, 310, 0);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_KAJA, 440, 134, FACING_WEST);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_FIRST_OARSMAN, 454, 248,
                                      FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN, 6);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(308), 0x1000001);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Engine_ScriptSetMessage(MsgTolbiShipHeyAreYouOk);
  TolbiShipInteriors_Say(ROWING_KAJA);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0xc000, 0);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  ObjectEvent_SetSpeed(ROWING_KAJA, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 134);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 152);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 424, 164);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 424, 222);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 424, 262);
  ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  TolbiShipInteriors_Say(ROWING_KAJA);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  TolbiShipInteriors_Say(ROWING_KAJA);
  TolbiShipInteriors_RunSceneCommand(SHIP_SHOW_PARTY_EMOTE, 2, 60);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0xe000, 60);
  ObjectEvent_FaceDirection(ROWING_KAJA, FACING_EAST, 40);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  Engine_ObjectEventSetAnimation(ROWING_KAJA, 2);
  ObjectEvent_MoveToAndWait(ROWING_KAJA, 0x1b0, 0x10c);
  ObjectEvent_MoveToAndWait(ROWING_KAJA, 0x1c4, 0x10c);
  Engine_ObjectEventSetAnimation(ROWING_KAJA, 1);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_NORTH + FACING_STEP);
  Engine_ObjectEventStartRepeatedMotion(ROWING_KAJA, 2);
  Script_ShowMessageAndWait(ROWING_KAJA, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0xc000, 20);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 4);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(ROWING_KAJA, 0, 80);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  Engine_ScriptWait(10);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_SOUTH + FACING_STEP);
  Script_OpenMessage(ROWING_KAJA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
    TolbiShipInteriors_Say(ROWING_KAJA);
  } else {
    Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 4);
    gFieldWork->message += 1;
    TolbiShipInteriors_Say(ROWING_KAJA);
    TolbiShipInteriors_RunSceneCommand(SHIP_SHOW_PARTY_EMOTE, 2, 40);
    Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
    Engine_ObjectEventSetAnimation(ROWING_KAJA, 3);
    TolbiShipInteriors_Say(ROWING_KAJA);
  }
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  action = gTolbiShipInteriors_FollowersRejoinMidDeck;
  ObjectEvent_SetActionScript(GARET, action);
  ObjectEvent_SetActionScript(IVAN, action);
  ObjectEvent_RunActionScriptAndWait(MIA, action);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 424, 272);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 424, 164);
  Engine_ScriptWait(60);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_ScriptRequestWarp(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD);
}

/// Restores one replacement rower after the next monster attack.
static void TolbiShipInteriors_EndSecondAttack(void) {
  s32 handle = TolbiShipInteriors_FindFirstSetFlag(0, 0);

  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_PlaceFoundObjectEvents(0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle,
                                     ROWING_FIRST_OARSMAN + 2);
  Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 1, 0, 0);
  TolbiShipInteriors_ReactToMonsterAttack();
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF);
  Engine_ScriptEnd();
}

/// Stages the next monster encounter with two replacement rowers.
static void TolbiShipInteriors_StartThirdAttack(void) {
  s32 secondRower;
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 3, 0);
  TolbiShipInteriors_SetOarCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                                     secondRower);
  Engine_ObjectEventSetAnimation(ROWING_FIRST_OARSMAN, 6);
  rowerScript = gTolbiShipInteriors_Shiver;
  ObjectEvent_SetActionScript(firstRower, rowerScript);
  Engine_ObjectEventDestroy(ROWING_FIRST_OARSMAN + 1);
  ObjectEvent_SetActionScript(secondRower, rowerScript);
  Engine_ObjectEventDestroy(ROWING_FIRST_OARSMAN + 2);
  entryScript = gTolbiShipInteriors_MonsterBob;
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER, entryScript);
  ObjectEvent_SetActionScript(ROWING_FIRST_MONSTER + 1, entryScript);
  Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER, 3);
  Engine_ObjectEventSetChildValue(ROWING_FIRST_MONSTER + 1, 3);
  TolbiShipInteriors_ReportMonstersBelowdecks();
  Engine_ScriptEnd();
}

/// Restores two replacement rowers after the next monster attack.
static void TolbiShipInteriors_EndThirdAttack(void) {
  s32 handle = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  s32 other = TolbiShipInteriors_FindFirstSetFlag(1, 0);

  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_PlaceFoundObjectEvents(0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle,
                                     other);
  Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 1, 0, 0);
  Engine_ObjectEventSetPosition(ROWING_FIRST_OARSMAN + 2, 0, 0);
  TolbiShipInteriors_ReactToMonsterAttack();
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF);
  Engine_ScriptEnd();
}

/// TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP, on the rowing deck: the ship rocks, a
/// huge sea monster is on the ship, and the party goes up on deck
/// (FLAG_TOLBI_SHIP_KRAKEN_ON_DECK).
static void TolbiShipInteriors_NoticeShipRocking(void) {
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  struct ObjectEvent *objectEvent;
  const s32 *action;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  thirdRower = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  // Restore the occupied rowing seats before showing the ship's movement.
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_OARS, 0, 0);
  ObjectEvent_SetPosition(ROWING_CREWMAN, PIXELS(472), PIXELS(152));
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 5);
  ObjectEvent_SetPosition(ROWING_KAJA, PIXELS(440), PIXELS(134));
  ObjectEvent_SetChildValue(ROWING_KAJA, 15);
  objectEvent = ObjectEvent_GetById(ROWING_KAJA);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(16);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCREEN, 1, 20);
  Script_PlaySong(MUS_SILENCE);
  Script_PlaySong(SE_181);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(10);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_ScriptWait(80);
  Script_PlaySong(SE_181);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(10);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Script_PlaySong(MUS_063);
  GameFlag_Set(FLAG_KEEP_MUSIC);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  TolbiShipInteriors_Turn(MIA, FACING_SOUTHWEST);
  // The party notices the rocking, then turns to the crew for an explanation.
  Engine_ScriptSetMessage(MsgTolbiShipWonderWhatsWrongShipShouldnt);
  Script_ShowMessageAndWait(MIA, 0, 40);
  TolbiShipInteriors_Say(ROWING_KAJA);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 40);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  TolbiShipInteriors_Turn(IVAN, FACING_SOUTHEAST);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  TolbiShipInteriors_Say(IVAN);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 20);
  ObjectEvent_SetChildValue(ROWING_KAJA, 0);
  objectEvent = ObjectEvent_GetById(ROWING_KAJA);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 1);
  ObjectEvent_SetSpeed(ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 430, 134);
  TolbiShipInteriors_Turn(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventStartRepeatedMotion(ROWING_KAJA, 2);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventStartRepeatedMotion(firstRower, 1);
  Engine_ObjectEventStartRepeatedMotion(secondRower, 1);
  Engine_ObjectEventStartRepeatedMotion(thirdRower, 1);
  Engine_ObjectEventRunRepeatedMotion(ROWING_FIRST_OARSMAN + 3, 1);
  ObjectEvent_SetAttachedEffect(firstRower, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(secondRower, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(thirdRower, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(ROWING_FIRST_OARSMAN + 3,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWING_DECK_POSES, 1, 0);
  ObjectEvent_FaceDirection(firstRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(secondRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 40);
  Engine_ObjectEventStartRepeatedMotion(ROWING_KAJA, 2);
  Script_ShowMessage(ROWING_KAJA, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 440, 134);
  ObjectEvent_SetPosition(ROWING_KAJA, 0, 0);
  TolbiShipInteriors_Turn(GARET, FACING_WEST);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  TolbiShipInteriors_Say(GARET);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  TolbiShipInteriors_Turn(MIA, FACING_WEST);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  action = gTolbiShipInteriors_FollowersRejoinBySeats;
  ObjectEvent_SetActionScript(GARET, action);
  ObjectEvent_SetActionScript(IVAN, action);
  ObjectEvent_RunActionScriptAndWait(MIA, action);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  gTolbiShipInteriors_ShakeTimer = 0;
  FieldScheduler_AddCallback(TolbiShipInteriors_ShakeWhileKrakenAboard,
                             TASK_PRIORITY_MAP);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  Engine_ObjectEventDestroy(ROWING_KAJA);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_ScriptEnd();
}

/// Plays the intermittent voyage cue and restores its effect after ten frames.
static void TolbiShipInteriors_ShakeWhileKrakenAboard(void) {
  s32 value;
  s32 *timer = &gTolbiShipInteriors_ShakeTimer;
  s32 remaining;
  if (*timer != 0) {
    remaining = *timer - 1;
    *timer = *timer - 1;
    if (remaining == 70)
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  } else {
    value = Engine_RandomNext();
    if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
      Engine_AudioPlayCue(SE_181);
      MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      *timer = 80;
    }
  }
}

/// TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND, in the
/// captain's quarters, after the ship has come off course: a short exchange,
/// then TOLBI_SHIP_INTERIORS_TO_WORLD_MAP leads by the world map to the island
/// (TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND).
static void TolbiShipInteriors_ShowCaptainsQuartersBeforeIsland(void) {
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 0);
  TolbiShipInteriors_PlaceObjectEvent(QUARTERS_KAJA, 468, 616, 32768);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCREEN, 1, 20);
  Engine_ObjectEventRunRepeatedMotion(QUARTERS_KAJA, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, 53248, 80);
  ObjectEvent_FaceDirection(QUARTERS_CAPTAIN, FACING_EAST, 20);
  Engine_ObjectEventSetAnimationAndWait(QUARTERS_CAPTAIN, 3);
  Engine_ScriptWait(20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCREEN,
                                     TOLBI_SHIP_INTERIORS_TO_WORLD_MAP, 0);
}

/// TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND, on the rowing deck
/// at the island: Kaja rests the rowers, and each chosen passenger answers
/// (FLAG_TOLBI_SHIP_AT_ISLAND).
static void TolbiShipInteriors_ReachIsland(void) {
  s32 fourthRower;
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  struct ObjectEvent *objectEvent;
  s32 reaction = 0;
  const s32 *progressB;
  const s32 *progressC;
  const s32 *progressA;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  thirdRower = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  fourthRower = TolbiShipInteriors_FindFirstSetFlag(3, 0);
  // Restore all four selected rowers and announce the island stop.
  Engine_ScriptStart();
  TolbiShipInteriors_SetOarCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_OARS, 0, 0);
  ObjectEvent_SetPosition(ROWING_CREWMAN, PIXELS(472), PIXELS(152));
  ObjectEvent_SetPosition(ROWING_KAJA, PIXELS(440), PIXELS(134));
  Engine_ObjectEventSetChildValue(ROWING_KAJA, 15);
  objectEvent = ObjectEvent_GetById(ROWING_KAJA);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  TolbiShipInteriors_InstallRowingDeckObjectEvents(16);
  Engine_ObjectEventSetAnimation(ROWING_DRUMMER, 5);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCREEN, 1, 20);
  Engine_ObjectEventSetChildValue(ROWING_KAJA, 0);
  objectEvent = ObjectEvent_GetById(ROWING_KAJA);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 1);
  ObjectEvent_SetSpeed(ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 132);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 142);
  ObjectEvent_FaceDirection(ROWING_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventStartRepeatedMotion(ROWING_KAJA, 2);
  Engine_ScriptSetMessage(MsgTolbiShipReachedIslandRowers);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ScriptWait(120);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, fourthRower, 1);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWING_DECK_POSES, 0, 0);
  ObjectEvent_FaceDirection(firstRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(secondRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_NORTH + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(fourthRower, FACING_NORTHWEST + FACING_STEP,
                                  60);
  // The first rower's identity selects the response to the unexpected stop.
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                           PASSENGER_OLD_MAN)) {
    reaction = 2;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                                  PASSENGER_MUSCLE_ADMIRER) ||
             Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                                  PASSENGER_LADY)) {
    reaction = 1;
  }
  Engine_ObjectEventRunRepeatedMotion(firstRower, 1);
  if (reaction == 1) {
    gFieldWork->message += 1;
  } else if (reaction == 2) {
    gFieldWork->message += 2;
  }
  Engine_ObjectEventStartRepeatedMotion(firstRower, 2);
  TolbiShipInteriors_Say(firstRower);
  Engine_ScriptSetMessage(MsgTolbiShipShipWentOff);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 4);
  TolbiShipInteriors_Say(ROWING_KAJA);
  ObjectEvent_ShowEmote(firstRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(secondRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(thirdRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ObjectEventShowEmote(fourthRower, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  TolbiShipInteriors_Say(ROWING_KAJA);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 132);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 444, 132);
  Engine_ObjectEventDestroy(ROWING_KAJA);
  Engine_ScriptWait(40);
  if (reaction == 0 && (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                                             PASSENGER_HEAD_GUIDE) ||
                        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
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
  TolbiShipInteriors_Turn(firstRower, 0);
  TolbiShipInteriors_Say(firstRower);
  ObjectEvent_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  progressB = gTolbiShipInteriors_LeaveByStairs;
  ObjectEvent_RunActionScriptAndWait(firstRower, progressB);
  ObjectEvent_FaceDirection(secondRower, FACING_SOUTH + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(thirdRower, FACING_EAST, 0);
  ObjectEvent_FaceDirection(fourthRower, FACING_WEST, 40);
  Engine_ObjectEventFaceDirection(secondRower, FACING_NORTH + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(thirdRower, FACING_NORTHWEST + FACING_STEP,
                                  0);
  Engine_ObjectEventFaceDirection(fourthRower, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetActionScript(thirdRower, progressB);
  Engine_ScriptWait(40);
  progressC = gTolbiShipInteriors_StepIntoAisle;
  ObjectEvent_RunActionScriptAndWait(secondRower, progressC);
  ObjectEvent_SetActionScript(secondRower, progressB);
  ObjectEvent_RunActionScriptAndWait(fourthRower, progressC);
  ObjectEvent_RunActionScriptAndWait(fourthRower, progressB);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  progressA = gTolbiShipInteriors_FollowersRejoinByStairs;
  ObjectEvent_SetActionScript(GARET, progressA);
  ObjectEvent_SetActionScript(IVAN, progressA);
  ObjectEvent_RunActionScriptAndWait(MIA, progressA);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  Engine_GameFlagClear(FLAG_TOLBI_SHIP_ROWING);
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_AT_ISLAND);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_ScriptEnd();
}

/// TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS, on the
/// rowing deck back from the island: Kaja switches the left and right teams,
/// the left team's timing having taken the ship off course; the chosen
/// passengers swap seats and the ship sets off again.
static void TolbiShipInteriors_DismissReplacementRowers(void) {
  s32 firstRower;
  s32 fourthRower;
  s32 thirdRower;
  s32 secondRower;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  thirdRower = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  fourthRower = TolbiShipInteriors_FindFirstSetFlag(3, 0);
  Engine_ScriptStart();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_CREWMAN, 472, 144,
                                      FACING_SOUTH + FACING_STEP);
  TolbiShipInteriors_PlaceObjectEvent(ROWING_KAJA, 408, 142,
                                      FACING_SOUTHEAST + FACING_STEP);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  Engine_ScriptSetMessage(MsgTolbiShipSorryEveryoneButWeNeed);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventStartRepeatedMotion(firstRower, 2);
  Engine_ObjectEventStartRepeatedMotion(secondRower, 2);
  Engine_ObjectEventStartRepeatedMotion(thirdRower, 2);
  Engine_ObjectEventRunRepeatedMotion(fourthRower, 2);
  Engine_ScriptWait(20);
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
  Engine_ObjectEventSetAnimation(firstRower, 1);
  Engine_ObjectEventSetAnimation(secondRower, 1);
  Engine_ObjectEventSetAnimation(thirdRower, 1);
  ObjectEvent_FaceDirection(secondRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(firstRower, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(fourthRower, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(thirdRower, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(ROWING_KAJA, 1);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventSetAnimation(firstRower, 3);
  Engine_ObjectEventSetAnimation(secondRower, 3);
  Engine_ObjectEventSetAnimation(thirdRower, 3);
  Engine_ObjectEventSetAnimationAndWait(fourthRower, 3);
  TolbiShipInteriors_Say(ROWING_KAJA);
  Engine_ObjectEventSetAnimation(firstRower, 3);
  Engine_ObjectEventSetAnimation(secondRower, 3);
  Engine_ObjectEventSetAnimation(thirdRower, 3);
  Engine_ObjectEventSetAnimationAndWait(fourthRower, 3);
  ObjectEvent_FaceDirection(ROWING_KAJA, FACING_EAST, 0);
  TolbiShipInteriors_Turn(LOCALID_PARTY_LEADER, FACING_WEST);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(ROWING_KAJA, 3);
  ObjectEvent_SetSpeed(ROWING_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 408, 132);
  ObjectEvent_WalkToAndWait(ROWING_KAJA, 444, 132);
  ObjectEvent_SetPosition(ROWING_KAJA, 0, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  TolbiShipInteriors_SwapRowerSelections(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                         FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  TolbiShipInteriors_SwapRowerSelections(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                         FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_AT_ISLAND);
  Engine_ScriptRequestWarp(
      TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY);
}

/// Exchanges the chosen passengers between two rowing seats.
static void TolbiShipInteriors_SwapRowerSelections(s32 firstSeatFlags,
                                                   s32 secondSeatFlags) {
  s32 firstPassenger = 0;
  s32 secondPassenger = 0;
  u32 i;

  for (i = 0; i <= 8; i++) {
    s32 flag = firstSeatFlags + i;
    if (Engine_GameFlagIsSet(flag) != 0) {
      Engine_GameFlagClear(flag);
      break;
    }
    firstPassenger++;
  }

  for (i = 0; i <= 8; i++) {
    s32 flag = secondSeatFlags + i;
    if (Engine_GameFlagIsSet(flag) != 0) {
      Engine_GameFlagClear(flag);
      break;
    }
    secondPassenger++;
  }

  Engine_GameFlagSet(secondSeatFlags + firstPassenger);
  Engine_GameFlagSet(firstSeatFlags + secondPassenger);
}

/// The step beside the leader for each of the 16 facings: the high 16 bits are
/// x, the low 16 bits z.
#define TURN_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gTolbiShipInteriors_TurnSteps[] = {
    TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(0, 16),
    TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(-16, 0),
    TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(0, -16),
    TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(16, 0),
};

/// The warriors walk out of the hold.
s32 gTolbiShipInteriors_LeaveHold[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(168), 0, PIXELS(500)),
    SC_SYNC,         SC_MOVE(PIXELS(168), 0, PIXELS(584)),
    SC_SYNC,         SC_CALL(TolbiShipInteriors_ResetObjectEvent),
    SC_END,
};

/// Those gathered in the captain's quarters walk out.
s32 gTolbiShipInteriors_LeaveCaptainsQuarters[] = {
    SC_SET(SW_SPEED, PIXELS(1)),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 2),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(674)),
    SC_SYNC,
    SC_MOVE(PIXELS(456), 0, PIXELS(712)),
    SC_SYNC,
    SC_CALL(TolbiShipInteriors_ResetObjectEvent),
    SC_END,
};

/// The Anchor Charm swells and shrinks twice as the captain raises it.
s32 gTolbiShipInteriors_AnchorCharmPulse[] = {
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
s32 gTolbiShipInteriors_FollowersRejoinByStairs[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(434), 0, PIXELS(166)),
    SC_SYNC,         SC_CALL(TolbiShipInteriors_ResetObjectEvent),
    SC_END,
};

/// The same, in the middle of the rowing deck.
s32 gTolbiShipInteriors_FollowersRejoinMidDeck[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(438), 0, PIXELS(300)),
    SC_SYNC,         SC_CALL(TolbiShipInteriors_ResetObjectEvent),
    SC_END,
};

/// The same, by the first row of seats.
s32 gTolbiShipInteriors_FollowersRejoinBySeats[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(432), 0, PIXELS(168)),
    SC_SYNC,         SC_CALL(TolbiShipInteriors_ResetObjectEvent),
    SC_END,
};

/// Squashes and stretches the object event over and over: a shiver.
s32 gTolbiShipInteriors_Shiver[] = {
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
s32 gTolbiShipInteriors_MonsterBob[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(TolbiShipInteriors_Bob),
};

/// A rower walks up the stairs and is freed.
s32 gTolbiShipInteriors_LeaveByStairs[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(408), 0, PIXELS(132)),
    SC_SYNC,         SC_MOVE(PIXELS(444), 0, PIXELS(132)),
    SC_SYNC,         SC_FREE,
};

/// A rower steps into the aisle.
s32 gTolbiShipInteriors_StepIntoAisle[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(416), 0, PIXELS(186)),
    SC_SYNC,
    SC_END,
};

/// A passenger glances about at random.
s32 gTolbiShipInteriors_LookAroundScript[] = {
    SC_CALL(TolbiShipInteriors_LookAround),
};

/// Lies flat on the floor of the hold.
s32 gTolbiShipInteriors_LieFlatScript[] = {
    SC_CALL(TolbiShipInteriors_LayFlat),
};

/// The Anchor Charm's object, once placed this visit.
struct ObjectEvent *gTolbiShipInteriors_AnchorObject = 0;

/// The other maps have no warp destinations.
struct MapWarpDest gTolbiShipInteriors_WarpDestsOther[] = {
    {MAP_TABLE_END},
};

/// The passengers' hold, the cargo hold, the rowing deck and the captain's
/// quarters, each warp destination named for its room.
struct MapWarpDest gTolbiShipInteriors_WarpDests[] = {
    {TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS,
     152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK, CONDITION_ALWAYS,
     160, 0, 552, FACING_EAST, 0, 30, 380, 270, 620, 0},
    {TOLBI_SHIP_INTERIORS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS,
     120, 0, 136, FACING_WEST, 0, 25, 80, 265, 350, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD, CONDITION_ALWAYS,
     408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS,
     440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF,
     CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS,
     CONDITION_ALWAYS, 152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING,
     CONDITION_ALWAYS, 440, 0, 342, FACING_NORTH, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS,
     CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN,
     CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP, CONDITION_ALWAYS, 408, 0, 136,
     FACING_WEST, 0, 305, 60, 565, 430, 0},
    {TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND,
     CONDITION_ALWAYS, 444, 0, 632, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND, CONDITION_ALWAYS,
     408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS,
     CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT, CONDITION_ALWAYS, 166, 0,
     570, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {MAP_TABLE_END},
};

u32 gTolbiShipInteriors_Warps[] = {
    MAP_WARPS(TolbiShipInteriors),
    MAP_WARP(TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK,
             TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD),
    MAP_WARP(TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK,
             TolbiShipInteriors,
             TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD),
    MAP_WARP(TOLBI_SHIP_INTERIORS_CRATE_HOLD_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CRATE_HOLD),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD,
             TolbiShipInteriors,
             TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK),
    MAP_WARP(TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK,
             TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY,
             TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY),
    MAP_WARP(
        TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS,
        TolbiShipInteriors,
        TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS),
    MAP_WARP(TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING,
             TolbiShipInteriors,
             TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK,
             WorldMap,
             WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK),
    MAP_WARP(TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS,
             TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS),
    MAP_WARP(
        TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD,
        TolbiShipDeck,
        TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD),
    MAP_WARP(
        TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK,
        WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK,
             WorldMap,
             WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN,
             WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN,
             TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN),
    MAP_WARP(TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND,
             WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND),
    MAP_WARP(TOLBI_SHIP_INTERIORS_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gTolbiShipInteriors_ObjectEventTemplatesNone[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gTolbiShipInteriors_CaptainsQuartersObjectEventTemplates[] = {
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
struct ObjectEventTemplate gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesWarriors[] = {
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
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct ObjectEventTemplate gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesArrived[] = {
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(462), 0, PIXELS(612), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

/// The hold before the party hears of the delay at Kalay.
struct ObjectEventTemplate gTolbiShipInteriors_HoldObjectEventTemplatesEmpty[] = {
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct ObjectEventTemplate gTolbiShipInteriors_HoldObjectEventTemplates[] = {
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
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The hold at sea, once the passengers row.
struct ObjectEventTemplate gTolbiShipInteriors_HoldObjectEventTemplatesAtSea[] = {
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
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The rowing deck, in the order of enum TolbiShipRowingDeckLocalId: the crewman
/// and the drummer, the oarsmen at the seats, the passengers who may take a
/// seat, Kaja and the eight oars beside the seats.
struct ObjectEventTemplate gTolbiShipInteriors_RowingDeckObjectEventTemplates[] = {
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

/// The party's followers, placed for a scene.
struct ObjectEventTemplate gTolbiShipInteriors_PartyFollowerObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The monsters of each attack, beside the seats.
struct ObjectEventTemplate gTolbiShipInteriors_FirstAttackMonsters[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTolbiShipInteriors_SecondAttackMonsters[] = {
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTolbiShipInteriors_ThirdAttackMonsters[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The cargo hold, and the cutscenes.
struct MapEvent gTolbiShipInteriors_EventsOther[] = {
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_CRATE_HOLD_TO_TOLBI_SHIP_DECK },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_SHIP_INTERIORS_CRATE_HOLD_BARREL, FLAG_TOLBI_SHIP_CRATE_HOLD_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct MapEvent gTolbiShipInteriors_HoldEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK },
    { MAP_EVENT_COORD, TOLBI_SHIP_INTERIORS_CELL_HOLD_WARRIORS, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, EVENT_SCRIPT(TolbiShipInteriors_WatchWarriorsStormOff) },
    { MAP_EVENT_TALK, HOLD_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToWaitingWarrior) },
    { MAP_EVENT_TALK, HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
    { MAP_EVENT_TALK, HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
    { MAP_EVENT_TALK, HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToWaitingMuscleAdmirer) },
    { MAP_EVENT_TALK, HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
    { MAP_EVENT_TALK, HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
    { MAP_EVENT_TALK, HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
    { MAP_EVENT_TALK, HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_WARRIOR, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMaybeIShouldJustForgetAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheyreJustTakingAdvantageOfUs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsDoTheyHonestlyThinkImGoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_MUSCLE_ADMIRER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSureItdBeEasierTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsWeLeftTheOldLadyBack) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellShootMaybeWeShouldHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAskingThePassengersToHelpOut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_HEALER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsEvenThoughImOpposedToViolence) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_HEALER, FLAG_TOLBI_SHIP_REACHED_TOLBI, MSGNO(MsgTolbiShipInteriorsICantRowButIAm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMadeItToColossoIn) },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_SHIP_INTERIORS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { MAP_EVENTS_END },
};

/// The hold at sea: talking to a passenger asks them to row.
struct MapEvent gTolbiShipInteriors_HoldEventsChoosingRowers[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK },
    { MAP_EVENT_TALK, SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
    { MAP_EVENT_TALK, SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskHeadGuideToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskSweatingPassengerToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskLadyToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskStaredAtPassengerToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskSecondGuideToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskWarriorToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskTicketGrumblerToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskMuscleAdmirerToRow) },
    { MAP_EVENT_TALK, SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskOldManToRow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_HEAD_GUIDE, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouMakeMeNervousJustStanding) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_SWEATING_PASSENGER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyLooksLikeTheyNeedSomeone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenInAColdSweat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_LADY, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsRowingIsAToughJobIsnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhThisPoorSapLooksSad) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_STARED_AT_PASSENGER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyThisGuysBeenStaringAt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhNoNoWhenOurEyes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_WARRIOR, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsOhOhNoHesNotComing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsOnlyNaturalThatINot) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsPleasePleaseImBeggingJustDont) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISnuckByThisTimeBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_MUSCLE_ADMIRER, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhatsWithYouGoBotherThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowThisWarriorIsntGonnaLet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhyAmIGettingSoWorked) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanJustMoveRightAlong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_SECOND_GUIDE, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCarefulNowWatchOutDontMake) },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_SHIP_INTERIORS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { MAP_EVENTS_END },
};

/// The hold while the ship waits off the island.
struct MapEvent gTolbiShipInteriors_HoldEventsAtIsland[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_TO_ROWING_DECK },
    { MAP_EVENT_TALK, SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
    { MAP_EVENT_TALK, SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
    { MAP_EVENT_TALK, SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
    { MAP_EVENT_TALK, SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
    { MAP_EVENT_TALK, SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
    { MAP_EVENT_TALK, SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
    { MAP_EVENT_TALK, SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
    { MAP_EVENT_TALK, SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
    { MAP_EVENT_TALK, SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
    { MAP_EVENT_TALK, SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskLadyToRow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_SHIP_INTERIORS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { MAP_EVENTS_END },
};

/// The captain's quarters, from the wait at Kalay until the ship reaches Tolbi. The Anchor
/// Charm is given back to the captain here.
struct MapEvent gTolbiShipInteriors_CaptainsQuartersEvents[] = {
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_TALK, QUARTERS_CAPTAIN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToCaptain) },
    { MAP_EVENT_TALK, QUARTERS_CREWMAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToCaptainsQuartersCrewman) },
    { MAP_EVENT_TALK, QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
    { MAP_EVENT_TALK, QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
    { MAP_EVENT_TALK, QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
    { MAP_EVENT_TALK, QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
    { MAP_EVENT_TALK, QUARTERS_SEAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToSeanInCaptainsQuarters) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsThisIsAChanceToMake) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheShipWontLeaveDockUntil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipInteriorsAsLongAsWeHaveThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeKnewTheRisksWhenWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CREWMAN, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsSendingOutAShipWhenIts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWaitedTooLongIllGet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIfWeDontSetSailNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThisIsRidiculousTheShipCant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThatThingCouldntHaveGottenLost) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWouldSteal) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipDoesntLeaveSoon) },
    { MAP_EVENT_COORD, TOLBI_SHIP_INTERIORS_CELL_QUARTERS_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(TolbiShipInteriors_WatchCrewWonderAtDelay) },
    { USE_ITEM_ON_OBJ_EVENT(ITEM_ANCHOR_CHARM), QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, EVENT_SCRIPT(TolbiShipInteriors_ReturnLuckyAnchor) },
    { BG_EVENT(BG_EVENT_JAR) | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_SHIP_INTERIORS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

/// The captain's quarters while the ship waits off the island.
struct MapEvent gTolbiShipInteriors_CaptainsQuartersEventsAtIsland[] = {
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_TALK, QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
    { MAP_EVENT_TALK, QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
    { BG_EVENT(BG_EVENT_JAR), TOLBI_SHIP_INTERIORS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct MapEvent gTolbiShipInteriors_CaptainsQuartersEventsArrived[] = {
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { MAP_EVENT_TALK, QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
    { BG_EVENT(BG_EVENT_JAR), TOLBI_SHIP_INTERIORS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

struct MapEvent gTolbiShipInteriors_RowingDeckEvents[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD },
    { MAP_EVENT_TALK, ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsWeCanLeaveAsSoonAs) },
    { MAP_EVENT_TALK, ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenSailingForManyYears) },
    { MAP_EVENT_TALK, ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis) },
    { MAP_EVENT_TALK, ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowIHaveABlisterOn) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRowingIsAMansJob) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereTheOnesWhoMakeThe) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOnceWeCastOffYouJust) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanOnlyDoHardWork) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHmmmIWonderWhenWereGoing) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfItsOkayTo) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsICantWaitUntilWeGet) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsStoppingAgainAndAgainLikeThis) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIHearTheresAnIsleIn) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLetsSeeIslandsInTheKaragol) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsFromTheSeaItLooksLike) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenWereRowingWeCanJust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEnoughIsEnoughLetsSetOff) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsCrazyToHaveInexperiencedPassengers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsILoveItWhenTheOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItWasHardForMeAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsEveryoneDownHereGetsSoCompetitive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseBigMusclesAreASign) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellSeeToItThatThey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouveGotToSaveUpLots) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIDontFeelLikeRowingAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISureHopeAnotherMonsterDoesnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsSpendYourMoneyWhileYoureYoung) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLooksLikeImGoingToHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsItSeemsLikeThereWouldBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCouldThisBeThatIslandThats) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHaHahIfYouWantTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMaybeItsTimeForMeTo) },
    { MAP_EVENTS_END },
};

/// The rowing deck while monsters are aboard: talking to one
/// (ROWING_FIRST_MONSTER and the two after it) fights it.
struct MapEvent gTolbiShipInteriors_RowingDeckEventsMonstersAboard[] = {
    { MAP_EVENT_COORD, TOLBI_SHIP_INTERIORS_CELL_ROWING_DECK_STAIRS, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TurnBackFromStairs) },
    { MAP_EVENT_TALK, ROWING_FIRST_MONSTER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_FightRowingDeckMonsters) },
    { MAP_EVENT_TALK, ROWING_FIRST_MONSTER + 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_FightRowingDeckMonsters) },
    { MAP_EVENT_TALK, ROWING_FIRST_MONSTER + 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_FightRowingDeckMonsters) },
    { MAP_EVENT_TALK, ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMonstersFromTheDeepAreAttacking) },
    { MAP_EVENT_TALK, ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCurseThoseBeastsTheyGotOne) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingOffTheMonstersIsYour) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFoulBeastIMayBeTrembling) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWerentSoTiredId) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCursesIWantToSmackThat) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhoIsThatThingGoingTo) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfYouDontDoABetter) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeOarsmenMayBeStrongBut) },
    { MAP_EVENT_TALK, ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSurroundedByDangerOnAll) },
    { MAP_EVENT_TALK, ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAhhhhSeaMonstersRealLiveSea) },
    { MAP_EVENT_TALK, ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhaHeyYouThisIsNo) },
    { MAP_EVENT_TALK, ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAahIHaveToRowThis) },
    { MAP_EVENT_TALK, ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHHeyIsaacShouldntYouBe) },
    { MAP_EVENT_TALK, ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYaaahhhIImNotAfraidOf) },
    { MAP_EVENT_TALK, ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheOarsmenAreInDireStraights) },
    { MAP_EVENT_TALK, ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIICantDoThisFighting) },
    { MAP_EVENT_TALK, ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhatHappensNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyArentYouDoingAnythingTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfWeLoseAnyMoreOarsmen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheSeaMonstersGetUs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImScaredThoseSeaMonstersAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingSeaMonstersIsForWarriors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheSeaMonstersAreSoTough) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhoElseThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseJustStayHereAndProtect) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeDontEvenKnowIfAttacking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeaMonstersAreTerriblyFerocious) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWereAWarriorI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfThisWarriorHas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSureInAFixSay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSoOutOfMyWits) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseSeaMonstersLookAwfullyStrong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseOhPleaseLetMeMake) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyDidSeaMonstersHaveTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIdRatherBeEatenByA) },
    { MAP_EVENTS_END },
};

/// The rowing deck while the Kraken rocks the ship.
struct MapEvent gTolbiShipInteriors_RowingDeckEventsKrakenAboard[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN },
    { MAP_EVENT_TALK, ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
    { MAP_EVENT_TALK, ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
    { MAP_EVENT_TALK, ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToHeadGuideAtOars) },
    { MAP_EVENT_TALK, ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToSecondGuideAtOars) },
    { MAP_EVENT_TALK, ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToSweatingPassengerAtOars) },
    { MAP_EVENT_TALK, ROWING_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToLadyAtOars) },
    { MAP_EVENT_TALK, ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToStaredAtPassengerAtOars) },
    { MAP_EVENT_TALK, ROWING_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToWarriorAtOars) },
    { MAP_EVENT_TALK, ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToTicketGrumblerAtOars) },
    { MAP_EVENT_TALK, ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToMuscleAdmirerAtOars) },
    { MAP_EVENT_TALK, ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadHeadGuideMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadSecondGuideMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadSweatingPassengerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_LADY, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadLadyMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadStaredAtPassengerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadWarriorMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadTicketGrumblerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadMuscleAdmirerMindAtOars) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
    { MAP_EVENTS_END },
};

/// The rowing deck once the ship has reached Tolbi.
struct MapEvent gTolbiShipInteriors_RowingDeckEventsArrived[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, TOLBI_SHIP_INTERIORS_ROWING_DECK_TO_PASSENGER_HOLD },
    { MAP_EVENT_TALK, ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWillBe) },
    { MAP_EVENT_TALK, ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouFolksReallyFoughtWell) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsALotOfOarsmenWereInjured) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAreWeSafeItsAllBecause) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenAnOarsmanAllMy) },
    { MAP_EVENT_TALK, ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouGoOnAheadJustWhen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeCantFindAnyoneToReplace) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImJustGladTheKrakenDidnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSureThePaysGoodButIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsaacAndHisPalsWereSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIFeelBetterNowIWas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImInNoRushAnywayThis) },
    { MAP_EVENTS_END },
};
// clang-format on

/// Frames left of the Kraken's current jolt; at 0 another may start.
s32 gTolbiShipInteriors_ShakeTimer;
