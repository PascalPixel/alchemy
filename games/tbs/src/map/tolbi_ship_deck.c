/// @file
/// Tolbi-bound Ship (MapId_TolbiShipDeck), its deck: the ship that carries
/// the party and a tour group bound for Colosso across the Karagol Sea from
/// Kalay to Tolbi. The ship's other maps are its mast (tolbi_ship_mast.c)
/// and below decks (tolbi_ship_interiors.c). The voyage's stage, read from
/// its flags, chooses the deck's object event templates and events: at Kalay,
/// under sail once the first attack is fought off
/// (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF), while the oars move
/// (FLAG_TOLBI_SHIP_ROWING), at anchor off Crossbone Isle
/// (FLAG_TOLBI_SHIP_AT_ISLAND) and at Tolbi (FLAG_TOLBI_SHIP_REACHED_TOLBI).
/// The setup redraws the deck's cells for the stage, rocks the view and
/// scrolls the sea while under sail, rows the oars to the strokes of the
/// passengers at the seats (gTolbiShipDeckRowerStrokes), and plays the
/// cutscene its warp destination names. Each cutscene leaves into the next, in
/// another of the ship's maps or on the world map, whose warp destinations sail
/// the ship on. In story order:
///
/// - At Kalay (TOLBI_SHIP_DECK_FROM_KALAY_DOCKS), while the ship waits for
///   its replacement from Tolbi. Once the tour group has left Kalay
///   (FLAG_KALAY_COLOSSO_TOUR_LEFT), walking up to the crew
///   (TOLBI_SHIP_DECK_CELL_BY_CREW) plays the captain, Kaja and a crewman
///   talking over the delay: the captain will sail trusting his anchor charm,
///   and the crewman slips below (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY).
///   Coming up to his door brings him out denying everything
///   (FLAG_TOLBI_SHIP_CREWMAN_CAUGHT), and Mind Read shows he hid the charm
///   on the mast. Once it is back (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) the
///   passengers who promised to row stand on deck.
/// - Setting sail (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY): Kaja
///   brings word that all is ready ("Preparations... Ready!"), which leads
///   to the lookout on the mast.
/// - The attacks: sea monsters are sighted
///   (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS), which leads to
///   the captain's quarters, where the captain turns the ship; monsters leap
///   aboard (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD), harry
///   Ouranos and Sean, the warriors bound for Colosso who guard the deck with
///   Kaja (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK), and drop onto
///   the deck (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK); each of
///   these three leads below to the fight among the oars. At
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN the party jumps down to
///   the deck and fights the Kraken, and returns to
///   TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE, where Kaja thanks Isaac
///   (FLAG_TOLBI_SHIP_KRAKEN_DEFEATED).
/// - Land: the lookout's call leads either to
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED and Tolbi, or, by how
///   the passengers chosen to row pull (tolbi_ship_mast.c), to
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, where the coast is not
///   Tolbi's and the crew make for an island. At anchor off it, Crossbone Isle
///   (reached by TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE), Sean asks the
///   party to scout it (FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED). Back aboard
///   (TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) the rowers switch sides below and the
///   ship sails on to Tolbi.
/// - At Tolbi (TOLBI_SHIP_DECK_FROM_WORLD_MAP): the passengers who rowed come
///   up one by one to thank the leader, and TOLBI_SHIP_DECK_TO_TOLBI_DOCKS
///   leads ashore to Tolbi Docks (MapId_TolbiDocks).
///
/// Open questions:
/// - None.

#include "action_script.h"
#include "flags.h"
#include "songs.h"
#include "version.h"
#include "game_state.h"
#include "unit/map/tolbi_ship_deck.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
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
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "call.h"
#include "field_service.h"
#include "global_cells.h"
#include "edition.h"
#include "map_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "map_grid_scroll.h"
#include "heap_state.h"
#include "fixed_math.h"

// Map section flags: Map_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another map section, and other map sections use the numbers for their
// own.

/// "There's a huge sea monster on the ship!": the deck shakes until the
/// Kraken is defeated (set in tolbi_ship_interiors.c).
/// ★ Named from the game: "there's a huge sea monster on the ship!"
/// (MsgTolbiShipWonderWhatsWrongShipShouldnt)
#define FLAG_TOLBI_SHIP_KRAKEN_ON_DECK 0x302

// Defined below, in the order of the ROM.
static void TolbiShipDeck_ConfrontCrewman(void);
static void TolbiShipDeck_ApplyEntryState(void);
static void TolbiShipDeck_PlaceWaitingPassengers(void);
static void TolbiShipDeck_PlacePassengersAgreedToRow(void);
static void TolbiShipDeck_SetUpUnderSail(void);
static void TolbiShipDeck_SetUpAfterKraken(void);
static void TolbiShipDeck_SetUpAtAnchor(void);
static void TolbiShipDeck_DepartFromKalay(void);
static void TolbiShipDeck_DepartWithParty(void);
static void TolbiShipDeck_AnnouncePreparationsReady(s32 messenger,
                                                    s32 crewMember);
static void TolbiShipDeck_SightSeaMonsters(void);
static void TolbiShipDeck_MonstersBoard(void);
static void TolbiShipDeck_MonstersAttackCrew(void);
static void TolbiShipDeck_MonsterLandsOnDeck(void);
static void TolbiShipDeck_JumpIntoKrakenBattle(void);
static void TolbiShipDeck_ThankForKraken(void);
static void TolbiShipDeck_SightLand(void);
static void TolbiShipDeck_RowersDisembark(void);
static s32 TolbiShipDeck_GetRowerAtSeat(u32 seat);
static void TolbiShipDeck_RockDeck(void);
static void TolbiShipDeck_SetUpOars(void);
static void TolbiShipDeck_ShowOar(s32 localId);
static void TolbiShipDeck_ChooseOarStrokes(void);
static s32 TolbiShipDeck_GetSeatStroke(u32 seat);
static void TolbiShipDeck_RowOars(void);
static void TolbiShipDeck_AdvanceStroke(s32 localId, s32 slot);
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags);
static void TolbiShipDeck_RestOars(void);
static void TolbiShipDeck_CopyUnderwayCells(void);
static void TolbiShipDeck_CopyAnchoredCells(void);
static void TolbiShipDeck_Say(s32 speaker);
static void TolbiShipDeck_Turn(s32 localId, s32 facing);
static void TolbiShipDeck_SightIsland(void);
static void TolbiShipDeck_ReturnFromIsland(void);
static void TolbiShipDeck_PlaceLeaderOnDeck(void);

// The append service registers these borrowed crew tables for later talk
// lookups, but does not write them. Its mutable API therefore needs a cast
// at each call; the tables themselves remain const.

/// A speaker: the object event, and in the top four bits where the message
/// window opens beside them (4, SPEAKER_WINDOW_ABOVE, is above).
#define SPEAKER(localId, window) ((window) << 12 | (localId))

/// Who is who on deck, numbering each object event template table's rows from
/// LOCALID_FIRST_PLACED. Every table starts with the same twelve rows of oars,
/// and the cutscenes add their object events after a table's last row
/// (FieldScript_CallWithLastActiveObjectId), so each stage of the voyage has
/// its own names, which carry its prefix. At Kalay
/// (gTolbiShipDeckObjectEventTemplatesWaiting) the captain talks the delay over
/// with Kaja, the deck's fighter, and a crewman, who asks Kaja how the oarsmen
/// are ("Say, Kaja, how are the oarsmen doing?"). At sea
/// gTolbiShipDeck_CrewObjectEventTemplates places Kaja and the warriors Ouranos
/// and Sean, who help him guard the deck ("That'll be me, Ouranos, and Sean on
/// deck").
enum TolbiShipDeckLocalId {
  /// Four oars, with the same sprite as the others, that keep the strokes of
  /// gTolbiShipDeckRowerStrokes, one each from 1 (the oar left still) to 4
  /// (fastest), drawn under the deck. Each oar at the sides copies the frames
  /// of its seat's stroke (TolbiShipDeck_SwingOar); at rest the first stands in
  /// for the first oar.
  /// ★ Named from what the player sees: an oar dipping into the water (its
  /// render)
  LOCALID_FIRST_STROKE_OAR = LOCALID_FIRST_PLACED,
  /// The eight oars at the ship's sides: four at x 132, then four at x 300,
  /// which are drawn mirrored (SPRITE_OAR). The rowing deck's map shows them
  /// out while the ship is rowed
  /// (https://alchemy.pascalpixel.com/places/tolbi-ship-deck_tolbi-ship-rowing/map-0.png).
  /// ★ Named from what the player sees: an oar dipping into the water (its
  /// render)
  LOCALID_FIRST_OAR = LOCALID_FIRST_STROKE_OAR + 4,

  // At Kalay: gTolbiShipDeckObjectEventTemplates, then
  // gTolbiShipDeckObjectEventTemplatesWaiting.
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who
  /// hid the captain's anchor charm on the mast and denies
  /// everything. "I didn't do anything..."
  KALAY_CREWMAN = LOCALID_FIRST_OAR + 8,
  /// One of the crew, a man in a yellow bandana (SPRITE_MAN_IN_YELLOW_BANDANA),
  /// who answers the tour guide beside him: "I told you, we're
  /// leaving soon. We'll set sail when our replacement arrives!" When the ship
  /// leaves he calls "Preparations... Ready!"
  KALAY_CREW_MEMBER,
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  KALAY_CAPTAIN,
  /// Kaja, a bald man with a beard (SPRITE_KAJA).
  KALAY_KAJA,
  /// The tour group's guide, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK)
  /// as on the Tolbi Docks. Mind Read: "I
  /// just hope we don't have to refund their money at the end of the trip."
  KALAY_TOUR_GUIDE,
  /// Three of the tour group's passengers, named by what they say first: a
  /// chef (SPRITE_CHEF), "I wonder what's going on. We've been waiting an
  /// awfully long time."
  KALAY_PASSENGER_WAITING_LONG,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN). "I wonder if I'm
  /// the only one here who's going to Tolbi to see Colosso..."
  KALAY_PASSENGER_SEEING_COLOSSO,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN). "The Karagol is like a
  /// humongous lake, surrounded by land."
  KALAY_PASSENGER_KARAGOL,

  // At sea: gTolbiShipDeck_CrewObjectEventTemplates after the oars, then each
  // cutscene's
  // own rows.
  SEA_KAJA = LOCALID_FIRST_OAR + 8,
  SEA_OURANOS,
  SEA_SEAN,
  /// With KALAY_CREW_MEMBER's sprite: "I have a job to do. I can't row!"; at
  /// anchor, Mind Read hears "I want to be a sailor like Kaja". On later
  /// departures he calls "Preparations... Ready!"
  SEA_CREW_MEMBER,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD
  /// (gTolbiShipDeck_MonstersBoardObjectEventTemplates): the first attack's
  /// three
  /// monsters, all of
  /// one sprite, which leap aboard and go below by the first door, where
  /// gTolbiShipInteriors_FirstAttackMonsters places them.
  SEA_FIRST_ATTACK_MONSTER,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK
  /// (gTolbiShipDeck_SecondAttackObjectEventTemplates): four monsters of one
  /// sprite hover
  /// by the
  /// deck, the first two diving at Sean and the others at Ouranos.
  SEA_FIRST_HOVERING_MONSTER = SEA_CREW_MEMBER + 1,
  /// Then the second attack's three, two of the hovering monsters' sprite and
  /// one other, as gTolbiShipInteriors_SecondAttackMonsters places them, come
  /// aboard and go below.
  SEA_SECOND_ATTACK_MONSTER = SEA_FIRST_HOVERING_MONSTER + 4,
  /// The splash that swells for 24 frames at the rail where the last of them
  /// leaps aboard out of the sea (SPRITE_SPLASH).
  /// ★ Named from what the player sees: a spray of water at the rail (its
  /// render)
  SEA_SPLASH = SEA_SECOND_ATTACK_MONSTER + 3,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK
  /// (gTolbiShipDeck_ThirdAttackObjectEventTemplates): four monsters with the
  /// first
  /// attack's
  /// sprite, that leap at Sean, the first two, and at Ouranos.
  SEA_FIRST_LEAPING_MONSTER = SEA_CREW_MEMBER + 1,
  /// Then the third attack's three, two with the first attack's sprite and one
  /// other, as gTolbiShipInteriors_ThirdAttackMonsters places them, which go
  /// below; the last drops onto the deck.
  SEA_THIRD_ATTACK_MONSTER = SEA_FIRST_LEAPING_MONSTER + 4,
  /// The falling monster's shadow, standing in for its own while it drops: a
  /// blank sprite (SPRITE_BLANK) draws only its shadow, which grows from a
  /// tenth of its size where the monster will land and goes the moment the
  /// monster lands and its own shadow is switched on.
  SEA_FALLING_MONSTER_SHADOW = SEA_THIRD_ATTACK_MONSTER + 3,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN and at anchor
  /// (gTolbiShipDeck_KrakenObjectEventTemplates, after Garet, Ivan and Mia in
  /// their own
  /// slots): the Kraken, which the party charges; at anchor the same rows
  /// place it only for it to be destroyed at once.
  /// ★ Named from the game: "You did good against that Kraken."
  /// (MsgTolbiShipThankIsaacDidGoodAgainst)
  SEA_KRAKEN = SEA_CREW_MEMBER + 1,
  /// An invisible object event the camera follows across the deck.
  SEA_CAMERA_TARGET,
  /// At anchor, back from the island
  /// (gTolbiShipDeck_TourGuideObjectEventTemplates): the
  /// tour guide comes up from below. "The passengers are going stir crazy!"
  /// Kaja: "Tell our tourists they can enjoy a nice, long break after we
  /// arrive."
  ANCHOR_TOUR_GUIDE,

  // At Tolbi: gTolbiShipDeckObjectEventTemplatesArrived.
  ARRIVED_KAJA = LOCALID_FIRST_OAR + 8, ///< "You lookin' for Sean and Ouranos?"
  /// A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA). "Are you headed for
  /// Colosso, too?" Mind Read: "I'm just going to hang out
  /// in Tolbi and enjoy myself for a while."
  ARRIVED_ASKS_ABOUT_COLOSSO,
  /// With KALAY_CREW_MEMBER's sprite: "This group that came on our last voyage
  /// pulled some strange stunts."
  ARRIVED_CREW_MEMBER,
  /// The first of the nine passengers who could promise to row, in the order of
  /// each seat's group of flags (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS and the
  /// others) and of gTolbiShipDeckRowerStrokes: the tour group's two guides,
  /// the
  /// chef, the young woman in green, the bald, muscular man, the woman with a
  /// sword, the young man with brown hair, the woman with fair hair and an old
  /// man with a cane (SPRITE_OLD_MAN_WITH_CANE): "You make an old man row the
  /// ship".
  ARRIVED_FIRST_PASSENGER,
  /// An invisible object event the camera follows as the rowers come up.
  ARRIVED_CAMERA_TARGET = ARRIVED_FIRST_PASSENGER + 9
};

/// The deck's coord events at Kalay, beside the ways below, whose metatile
/// behaviors carry the numbers of their warps (TolbiShipDeck_GoBelow).
enum TolbiShipDeckCell {
  /// Beside the crew, where they talk over the delay.
  TOLBI_SHIP_DECK_CELL_BY_CREW = 10,
  /// East of the second door, which the crewman slipped through.
  TOLBI_SHIP_DECK_CELL_EAST_OF_DOOR = 11,
  /// West of that door.
  TOLBI_SHIP_DECK_CELL_WEST_OF_DOOR = 12
};

// Shared code, included where it sits in the ROM.
#include "common/object_pulse.h"

/// An action-script call: shows the object event's sprite. Returns 0 to end the
/// call.
static s32 TolbiShipDeck_ShowSprite(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *a = (struct ObjectEvent *)interpreter;
  Engine_ObjectEventSetSpriteFlags(a, 1);
  return 0;
}

// The diving scripts own the object event's scratch fields: riseBaseY is
// sideways drift in 16.16 pixels per frame, riseCounter the dive step (0
// hovering), riseEnabled the return side, timer the drift direction and work
// the bob direction. Using the object event's fields keeps one layout for the
// whole slot.

/// The update of a sea monster hovering by Ouranos's side of the deck while the
/// monsters attack. It drifts and bobs at random; one frame in a hundred it
/// dives at Ouranos, who turns and jumps (sometimes with an emote), then it
/// returns to its side. riseEnabled picks which side it returns to.
/// Returns 1 to keep running.
static s32
TolbiShipDeck_HoverAndDiveAtOuranos(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if (objectEvent->riseCounter != 0) {
    switch (objectEvent->riseCounter) {
    case 1:
      objectEvent->speed = PIXELS(4);
      objectEvent->acceleration = PIXELS(2);
      Object_SetPosition(objectEvent, PIXELS(268), PIXELS(20), PIXELS(692));
      objectEvent->riseCounter++;
      break;
    case 2:
      objectEvent->riseCounter++;
      break;
    case 3:
      if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
          objectEvent->targetY == objectEvent->targetX &&
          objectEvent->targetZ == objectEvent->targetY) {
        objectEvent->riseCounter++;
        Script_PlaySong(SE_146);
        if (objectEvent->riseEnabled != 0)
          ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTH + FACING_STEP, 0);
        else
          ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTHWEST + FACING_STEP,
                                    0);
        if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
          ObjectEvent_Get(SEA_OURANOS)->velocityY = PIXELS(2);
        } else {
          ObjectEvent_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                                0);
          ObjectEvent_Get(SEA_OURANOS)->velocityY = PIXELS(6);
        }
      }
      break;
    case 4:
      objectEvent->riseCounter++;
      break;
    case 5:
      if (objectEvent->riseEnabled != 0)
        Object_SetPosition(objectEvent, PIXELS(282), 0, PIXELS(658));
      else
        Object_SetPosition(objectEvent, PIXELS(254), 0, PIXELS(668));
      objectEvent->riseCounter++;
      break;
    case 6:
      objectEvent->riseCounter++;
      break;
    case 7:
      if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
          objectEvent->targetY == objectEvent->targetX &&
          objectEvent->targetZ == objectEvent->targetY) {
        objectEvent->speed = PIXELS(2);
        objectEvent->acceleration = PIXELS(1);
        objectEvent->timer = 0;
        objectEvent->work = 0;
        objectEvent->riseCounter++;
        objectEvent->riseBaseY = 0;
      }
      break;
    case 8:
      objectEvent->riseCounter = 0;
      break;
    }
  } else {
    if (objectEvent->timer != 0) {
      objectEvent->riseBaseY -= ((u32)Engine_RandomNext() << 12) >> 16;
      if (objectEvent->riseBaseY < -PIXELS(1) / 4)
        objectEvent->timer = 0;
    } else {
      objectEvent->riseBaseY += ((u32)Engine_RandomNext() << 12) >> 16;
      if (objectEvent->riseBaseY > PIXELS(1) / 4)
        objectEvent->timer = 1;
    }
    if (objectEvent->x.fixed > PIXELS(248) &&
        objectEvent->x.fixed < PIXELS(292))
      objectEvent->x.fixed += objectEvent->riseBaseY;
    if ((s16)objectEvent->work != 0) {
      objectEvent->y.fixed = objectEvent->y.fixed -
                             (((u32)Engine_RandomNext() << 15) >> 16) -
                             PIXELS(1) / 2;
      if (objectEvent->y.fixed < 0)
        objectEvent->work = 0;
    } else {
      objectEvent->y.fixed = objectEvent->y.fixed +
                             (((u32)Engine_RandomNext() << 15) >> 16) +
                             PIXELS(1) / 2;
      if (objectEvent->y.fixed > PIXELS(8))
        objectEvent->work = 1;
    }
  }
  if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
    objectEvent->riseCounter = 1;
  return 1;
}

/// The same for the monster that dives at Sean.
static s32
TolbiShipDeck_HoverAndDiveAtSean(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if (objectEvent->riseCounter != 0) {
    switch (objectEvent->riseCounter) {
    case 1:
      objectEvent->speed = PIXELS(4);
      objectEvent->acceleration = PIXELS(2);
      Object_SetPosition(objectEvent, PIXELS(256), PIXELS(20), PIXELS(640));
      objectEvent->riseCounter++;
      break;
    case 2:
      objectEvent->riseCounter++;
      break;
    case 3:
      if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
          objectEvent->targetY == objectEvent->targetX &&
          objectEvent->targetZ == objectEvent->targetY) {
        objectEvent->riseCounter++;
        Script_PlaySong(SE_146);
        if (objectEvent->riseEnabled != 0)
          ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
        else
          ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTHWEST + FACING_STEP,
                                    0);
        if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
          ObjectEvent_Get(SEA_SEAN)->velocityY = PIXELS(2);
        } else {
          ObjectEvent_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
          ObjectEvent_Get(SEA_SEAN)->velocityY = PIXELS(6);
        }
      }
      break;
    case 4:
      objectEvent->riseCounter++;
      break;
    case 5:
      if (objectEvent->riseEnabled != 0)
        Object_SetPosition(objectEvent, PIXELS(264), 0, PIXELS(600));
      else
        Object_SetPosition(objectEvent, PIXELS(242), 0, PIXELS(604));
      objectEvent->riseCounter++;
      break;
    case 6:
      objectEvent->riseCounter++;
      break;
    case 7:
      if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
          objectEvent->targetY == objectEvent->targetX &&
          objectEvent->targetZ == objectEvent->targetY) {
        objectEvent->speed = PIXELS(2);
        objectEvent->acceleration = PIXELS(1);
        objectEvent->timer = 0;
        objectEvent->work = 0;
        objectEvent->riseCounter++;
        objectEvent->riseBaseY = 0;
      }
      break;
    case 8:
      objectEvent->riseCounter = 0;
      break;
    }
  } else {
    if (objectEvent->timer != 0) {
      objectEvent->riseBaseY -= ((u32)Engine_RandomNext() << 12) >> 16;
      if (objectEvent->riseBaseY < -PIXELS(1) / 4)
        objectEvent->timer = 0;
    } else {
      objectEvent->riseBaseY += ((u32)Engine_RandomNext() << 12) >> 16;
      if (objectEvent->riseBaseY > PIXELS(1) / 4)
        objectEvent->timer = 1;
    }
    if (objectEvent->x.fixed > PIXELS(232) &&
        objectEvent->x.fixed < PIXELS(272))
      objectEvent->x.fixed += objectEvent->riseBaseY;
    if ((s16)objectEvent->work != 0) {
      objectEvent->y.fixed = objectEvent->y.fixed -
                             (((u32)Engine_RandomNext() << 15) >> 16) -
                             PIXELS(1) / 2;
      if (objectEvent->y.fixed < 0)
        objectEvent->work = 0;
    } else {
      objectEvent->y.fixed = objectEvent->y.fixed +
                             (((u32)Engine_RandomNext() << 15) >> 16) +
                             PIXELS(1) / 2;
      if (objectEvent->y.fixed > PIXELS(8))
        objectEvent->work = 1;
    }
  }
  if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
    objectEvent->riseCounter = 1;
  return 1;
}

static u16 sStrokePhase[4];

static s32 sSwayAngleY;
UNUSED static s32 sUnused0; ///< never used; keeps the statics' layout

static u16 sOarAngle[8];

static s32 sSeaScroll[2];

static s32 sSwayAngleX;
UNUSED static s32 sUnused1; ///< never used; keeps the statics' layout

static s32 sSeaSpeed[2];

// The ROM places these after the file's other statics, the strokes first, on
// the 16-byte boundary the assembler gives a 32-byte common symbol.
static u32 sOarStroke[8] __attribute__((aligned(16)));
static s32 sOarDepth[8];

/// Bobs the object event between 4 and 12 pixels high by a random amount
/// each frame; the object event's work halfword says which way it is going.
/// Returns 1.
static s32 TolbiShipDeck_Bob(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 *flag = (s16 *)&objectEvent->work;
  s32 val;
  s32 tmp;

  // The one store of the new direction matches only through these jumps;
  // written as two stores, their constants load from the literal pool.
  if (*flag != 0) {
    val = objectEvent->y.fixed - (((u32)(Engine_RandomNext() << 15)) >> 16) -
          PIXELS(1) / 2;
    objectEvent->y.fixed = val;
    if (val >= PIXELS(4))
      goto done;
    tmp = 0;
  } else {
    val = objectEvent->y.fixed + (((u32)(Engine_RandomNext() << 15)) >> 16) +
          PIXELS(1) / 2;
    objectEvent->y.fixed = val;
    if (val <= PIXELS(12))
      goto done;
    tmp = 1;
  }
  *flag = tmp;
done:
  return 1;
}

/// Now and then (two chances in 32) turns the object event to look
/// north-northeast or north-northwest. Returns 1.
static s32 TolbiShipDeck_LookAround(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  u32 roll = ((u32)(Engine_RandomNext() << 5)) >> 16;

  if (roll == 6)
    objectEvent->facing = FACING_NORTH + FACING_STEP;
  else if (roll == 9)
    objectEvent->facing = FACING_NORTHWEST + FACING_STEP;
  return 1;
}

/// The update of a monster on deck that, at random, leaps at Ouranos: it jumps
/// to him, he turns and jumps back (sometimes with an emote), and it lands on
/// one of two spots chosen by riseEnabled. The object event's timer holds the
/// step. Returns 1.
static s32
TolbiShipDeck_LeapAtOuranos(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  switch (objectEvent->timer) {
  case 0:
    if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
      objectEvent->timer++;
    }
    break;
  case 1:
    objectEvent->timer++;
    break;
  case 2:
    objectEvent->velocityY = PIXELS(4);
    objectEvent->speed = PIXELS(4);
    objectEvent->acceleration = PIXELS(2);
    Object_SetPosition(objectEvent, PIXELS(264), 0, PIXELS(662));
    objectEvent->timer++;
    break;
  case 3:
    objectEvent->timer++;
    break;
  case 4:
    if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
        objectEvent->targetY == objectEvent->targetX &&
        objectEvent->targetZ == objectEvent->targetY) {
      objectEvent->timer++;
      Engine_AudioPlayCue(SE_JUMP);
      if (objectEvent->riseEnabled != 0) {
        ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTHWEST + FACING_STEP,
                                  0);
      } else {
        ObjectEvent_FaceDirection(SEA_OURANOS, FACING_SOUTH + FACING_STEP, 0);
      }
      if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
        ObjectEvent_GetById(SEA_OURANOS)->velocityY = PIXELS(2);
      } else {
        ObjectEvent_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                              0);
        ObjectEvent_GetById(SEA_OURANOS)->velocityY = PIXELS(6);
      }
    }
    break;
  case 5:
    objectEvent->timer++;
    break;
  case 6:
    objectEvent->timer++;
    objectEvent->velocityY = PIXELS(4);
    objectEvent->speed = PIXELS(2);
    objectEvent->acceleration = PIXELS(1);
    if (objectEvent->riseEnabled != 0) {
      Object_SetPosition(objectEvent, PIXELS(252), 0, PIXELS(646));
    } else {
      Object_SetPosition(objectEvent, PIXELS(256), 0, PIXELS(686));
    }
    break;
  case 7:
    objectEvent->timer++;
    break;
  case 8:
    if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
        objectEvent->targetY == objectEvent->targetX &&
        objectEvent->targetZ == objectEvent->targetY) {
      objectEvent->timer++;
    }
    break;
  case 9:
    objectEvent->timer = 0;
    break;
  }
  return 1;
}

/// The same for the monster that leaps at Sean.
static s32 TolbiShipDeck_LeapAtSean(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  switch (objectEvent->timer) {
  case 0:
    if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
      objectEvent->timer++;
    }
    break;
  case 1:
    objectEvent->timer++;
    break;
  case 2:
    objectEvent->velocityY = PIXELS(4);
    objectEvent->speed = PIXELS(4);
    objectEvent->acceleration = PIXELS(2);
    Object_SetPosition(objectEvent, PIXELS(176), 0, PIXELS(696));
    objectEvent->timer++;
    break;
  case 3:
    objectEvent->timer++;
    break;
  case 4:
    if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
        objectEvent->targetY == objectEvent->targetX &&
        objectEvent->targetZ == objectEvent->targetY) {
      objectEvent->timer++;
      Engine_AudioPlayCue(SE_JUMP);
      if (objectEvent->riseEnabled != 0) {
        ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
      } else {
        Engine_ObjectEventFaceDirection(SEA_SEAN, FACING_EAST, 0);
      }
      if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
        ObjectEvent_GetById(SEA_SEAN)->velocityY = PIXELS(2);
      } else {
        ObjectEvent_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
        ObjectEvent_GetById(SEA_SEAN)->velocityY = PIXELS(6);
      }
    }
    break;
  case 5:
    objectEvent->timer++;
    break;
  case 6:
    objectEvent->timer++;
    objectEvent->velocityY = PIXELS(4);
    objectEvent->speed = PIXELS(2);
    objectEvent->acceleration = PIXELS(1);
    if (objectEvent->riseEnabled != 0) {
      Object_SetPosition(objectEvent, PIXELS(184), 0, PIXELS(672));
    } else {
      Object_SetPosition(objectEvent, PIXELS(202), 0, PIXELS(692));
    }
    break;
  case 7:
    objectEvent->timer++;
    break;
  case 8:
    if (objectEvent->targetX == OBJ_EVENT_NO_TARGET &&
        objectEvent->targetY == objectEvent->targetX &&
        objectEvent->targetZ == objectEvent->targetY) {
      objectEvent->timer++;
    }
    break;
  case 9:
    objectEvent->timer = 0;
    break;
  }
  return 1;
}

/// Makes the object event hop at random, one frame in 40 on average. Returns 1.
static s32 TolbiShipDeck_RandomlyStartObjectEventHop(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if ((((u32)(Engine_RandomNext() * 40)) >> 16) == 0)
    objectEvent->velocityY = PIXELS(4);
  return 1;
}

/// The update that shrinks an enlarged object event by 1/32 a frame until it is
/// back to normal size.
static void TolbiShipDeck_ShrinkToNormalSize(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  if (objectEvent->scaleX > Q16_ONE) {
    objectEvent->scaleX -= Q16_ONE / 32;
    objectEvent->scaleY -= Q16_ONE / 32;
  }
}

/// Turns the object event east, northeast-by-north or southeast-by-south at
/// random intervals of 80 to 160 frames. riseCounter counts down the wait.
/// Returns 1.
static s32 TolbiShipDeck_IdleTurn(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if (objectEvent->riseCounter != 0) {
    objectEvent->riseCounter--;
  } else {
    u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

    if (roll > 200) {
      objectEvent->facing = FACING_NORTH + FACING_STEP;
    } else if (roll > 100) {
      objectEvent->facing = FACING_SOUTH + FACING_STEP;
    } else {
      objectEvent->facing = FACING_EAST;
    }
    objectEvent->riseCounter = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
  }
  return 1;
}

const struct MapWarpDest *TolbiShipDeck_GetWarpDests(void) {
  return gTolbiShipDeckWarpDests;
}

const struct MapWarpRegion *TolbiShipDeck_GetRegions(void) {
  return gTolbiShipDeckRegions;
}

const u32 *TolbiShipDeck_GetWarps(void) { return gTolbiShipDeckWarps; }

/// The talk facing of a Kalay object event's row in
/// gTolbiShipDeckObjectEventTemplatesWaiting.
#define KALAY_TALK_FACING(localId)                                             \
  STORE_CONST(gTolbiShipDeckObjectEventTemplatesWaiting[(localId) -            \
                                                        LOCALID_FIRST_PLACED]  \
                  .talkFacing)

const struct ObjectEventTemplate *TolbiShipDeck_GetObjectEventTemplates(void) {
  s32 afterFirstAttack;

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckObjectEventTemplatesArrived;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING))
    return gTolbiShipDeckObjectEventTemplatesRowing;
  afterFirstAttack =
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  if (afterFirstAttack != 0)
    return gTolbiShipDeckObjectEventTemplatesAtSea;
  if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
      KALAY_TALK_FACING(KALAY_CREW_MEMBER) = afterFirstAttack;
      KALAY_TALK_FACING(KALAY_PASSENGER_WAITING_LONG) = 2;
      KALAY_TALK_FACING(KALAY_PASSENGER_SEEING_COLOSSO) = 2;
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN)) {
      KALAY_TALK_FACING(KALAY_PASSENGER_WAITING_LONG) = 1;
      KALAY_TALK_FACING(KALAY_PASSENGER_SEEING_COLOSSO) = 1;
    }
    return gTolbiShipDeckObjectEventTemplatesWaiting;
  }
  return gTolbiShipDeckObjectEventTemplates;
}

#undef KALAY_TALK_FACING

const struct MapEvent *TolbiShipDeck_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckEventsArrived;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND))
    return gTolbiShipDeckEventsAtIsland;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF))
    return gTolbiShipDeckEventsAtSea;
  return gTolbiShipDeckEvents;
}

/// Talking to KALAY_CREW_MEMBER on deck at Kalay. What he says follows the
/// delay: short with the tour guide at first, alarmed once the warriors storm
/// off to the captain, relieved once the party has agreed to protect the ship.
/// While alarmed he paces at random intervals.
static void TolbiShipDeck_TalkToCrewMember(void) {
  struct ObjectEvent *crewMember;
  s32 random;
  s32 wait;

  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipNowWeHaveProtectShip);
    Script_ShowMessage(KALAY_CREW_MEMBER, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ObjectEventRunRepeatedMotion(KALAY_CREW_MEMBER, 2);
      Engine_ScriptSetMessage(MsgTolbiShipSomebodyStopThem);
      Script_ShowMessage(KALAY_CREW_MEMBER, 0);
      crewMember = ObjectEvent_GetById(KALAY_CREW_MEMBER);
      random = Random_Next();
      wait = ((u32)(90 * random) >> 16) + 60;
      crewMember->timer = wait;
      ObjectEvent_SetActionScript(KALAY_CREW_MEMBER,
                                  gTolbiShipDeck_SquashAndStretchActions);
    } else {
      ObjectEvent_ShowEmote(KALAY_CREW_MEMBER,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Engine_ObjectEventStartRepeatedMotion(KALAY_CREW_MEMBER, 3);
      Engine_ScriptSetMessage(MsgTolbiShipToldWereLeavingSoonSet);
      Script_ShowMessage(KALAY_CREW_MEMBER, 0);
    }
  }
  Engine_ScriptEnd();
}

/// The tour guide on deck at Kalay talks about the ship's delay. The guide has
/// the same three moods as TolbiShipDeck_TalkToCrewMember. "If we don't leave
/// soon, Colosso will start without us!"
static void TolbiShipDeck_TalkToTourGuide(void) {
  struct ObjectEvent *guide;
  s32 random;
  s32 wait;

  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_ScriptSetMessage(MsgTolbiShipHaveMakeThemPromiseHelp);
    Script_ShowMessage(KALAY_TOUR_GUIDE, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ObjectEventRunRepeatedMotion(KALAY_TOUR_GUIDE, 2);
      Engine_ScriptSetMessage(MsgTolbiShipTheyCantPlanningMutiny);
      Script_ShowMessage(KALAY_TOUR_GUIDE, 0);
      guide = ObjectEvent_GetById(KALAY_TOUR_GUIDE);
      random = Random_Next();
      wait = ((u32)(90 * random) >> 16) + 60;
      guide->timer = wait;
      ObjectEvent_SetActionScript(KALAY_TOUR_GUIDE,
                                  gTolbiShipDeck_SquashAndStretchActions);
    } else {
      ObjectEvent_ShowEmote(KALAY_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                            0);
      Engine_ObjectEventStartRepeatedMotion(KALAY_TOUR_GUIDE, 3);
      Engine_ScriptSetMessage(MsgTolbiShipIfWeDontLeaveSoon);
      Script_ShowMessage(KALAY_TOUR_GUIDE, 0);
    }
  }
  Engine_ScriptEnd();
}

/// At Tolbi, ARRIVED_ASKS_ABOUT_COLOSSO asks whether the party is headed for
/// Colosso too.
static void TolbiShipDeck_AskHeadedForColosso(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiShipHeadedColosso);
  Engine_ScriptAskYesNo(ARRIVED_ASKS_ABOUT_COLOSSO, 0);
  Engine_ScriptEnd();
}

/// Opens the first door to below deck, once a visit.
static void TolbiShipDeck_OpenFirstDoor(void) {
  if (GameFlag_IsSet(FLAG_MAP_TEMP_71) == 0) {
    Script_PlaySong(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(30, 94, 13, 94, 1, 3);
    GameFlag_Set(FLAG_MAP_TEMP_71);
  }
}

/// Opens the second door to below deck, once a visit.
static void TolbiShipDeck_OpenSecondDoor(void) {
  if (GameFlag_IsSet(FLAG_MAP_TEMP_72) == 0) {
    Script_PlaySong(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(30, 108, 13, 108, 1, 2);
    GameFlag_Set(FLAG_MAP_TEMP_72);
  }
}

/// Walking into one of the ways below deck, whose metatile behaviors are
/// numbered as their warps. The doors to the passengers' hold and the captain's
/// quarters open and the leader steps through; the hatch to the cargo hold just
/// plays its sound. Leaves by the warp numbered as the metatile behavior.
static void TolbiShipDeck_GoBelow(void) {

  s32 throughDoor;
  struct EventWork *work;

  work = gFieldWork;
  Engine_ScriptStart();
  throughDoor = 0;
  switch (work->coordEvent) {
  case TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD:
    throughDoor = 1;
    TolbiShipDeck_OpenFirstDoor();
    break;
  case TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM:
    throughDoor = 1;
    TolbiShipDeck_OpenSecondDoor();
    break;
  }
  if (throughDoor != 0) {
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    FieldObjectMotion_CenterOnColumn(LOCALID_PARTY_LEADER, 1, -10);
    Engine_ScriptWait(10);
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
  }
  Engine_ScriptRequestWarp(work->coordEvent);
}

#if TBS_PATCH_2
/// In the German, Spanish, French and Italian editions metatile behavior 2 is a
/// plain coord event: the leader goes below only while facing west, after
/// walking onto the hatch.
static void TolbiShipDeck_GoBelowWhileFacingWest(void) {
  if ((u16)(ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing - 0x6001) <=
      0x3ffe) {
    Engine_ScriptStart();
    ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 214, 664);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    TolbiShipDeck_GoBelow();
  }
}
#endif

/// The captain decides to set sail without the replacement ship. On deck at
/// Kalay, once the tour group has left Kalay (FLAG_KALAY_COLOSSO_TOUR_LEFT;
/// TOLBI_SHIP_DECK_CELL_BY_CREW), the captain, Kaja and the crewman wonder what
/// happened to the replacement ship, and the captain decides to set sail
/// anyway, trusting his anchor charm; the crewman runs off through the second
/// door. Plays once (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY).
static void TolbiShipDeck_WatchCrewDecideToSail(void) {
  struct ObjectEvent *record;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    Engine_ScriptStart();
    Field_EndBusyAndReveal();
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, KALAY_CREWMAN, 10);
    FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    FieldCamera_MoveTo(PIXELS(190), -1, PIXELS(708), 1);
    Engine_CameraWaitForMove();
    Engine_ScriptWait(40);
    Engine_ObjectEventRunRepeatedMotion(KALAY_CAPTAIN, 1);
    Engine_ScriptSetMessage(MsgTolbiShipWonderCouldHaveHappened);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    ObjectEvent_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Engine_ObjectEventRunRepeatedMotion(KALAY_CAPTAIN, 1);
    ObjectEvent_FaceDirection(KALAY_CAPTAIN, FACING_SOUTH + FACING_STEP, 0);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Engine_ObjectEventRunRepeatedMotion(KALAY_CREWMAN, 1);
    TolbiShipDeck_Turn(KALAY_CREWMAN, FACING_NORTHWEST + FACING_STEP);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    TolbiShipDeck_Turn(KALAY_KAJA, FACING_SOUTHEAST + FACING_STEP);
    Engine_ObjectEventSetAnimation(KALAY_KAJA, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    ObjectEvent_ShowEmote(KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    ObjectEvent_FaceDirection(KALAY_CAPTAIN, FACING_WEST, 20);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    TolbiShipDeck_Turn(KALAY_KAJA, FACING_EAST);
    Engine_ObjectEventSetAnimationAndWait(KALAY_KAJA, 4);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    ObjectEvent_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Engine_ObjectEventSetAnimationAndWait(KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    TolbiShipDeck_Turn(KALAY_CREWMAN, FACING_NORTH + FACING_STEP);
    Engine_ObjectEventSetAnimation(KALAY_KAJA, 3);
    Engine_ObjectEventSetAnimationAndWait(KALAY_CREWMAN, 3);
    Engine_ScriptWait(60);
    ObjectEvent_ShowEmote(KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    TolbiShipDeck_Turn(KALAY_CAPTAIN, FACING_SOUTH + FACING_STEP);
    Engine_ScriptSetMessage(MsgTolbiShipTheresNothingWeCanDo);
    Engine_ObjectEventStartRepeatedMotion(KALAY_CAPTAIN, 1);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    ObjectEvent_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    ObjectEvent_ShowEmote(KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Script_ShowMessageAndWait(SPEAKER(KALAY_CAPTAIN, 4), 0, 20);
    ObjectEvent_ShowEmote(KALAY_KAJA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    TolbiShipDeck_Turn(KALAY_CAPTAIN, FACING_WEST);
    Engine_ObjectEventSetAnimationAndWait(KALAY_CAPTAIN, 3);
    Script_ShowMessageAndWait(SPEAKER(KALAY_CAPTAIN, 4), 0, 20);
    ObjectEvent_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    TolbiShipDeck_Turn(KALAY_CAPTAIN, FACING_SOUTH + FACING_STEP);
    Engine_ObjectEventSetAnimation(KALAY_CAPTAIN, 4);
    TolbiShipDeck_Say(KALAY_CAPTAIN);
    ObjectEvent_FaceDirection(KALAY_CREWMAN, FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(KALAY_KAJA, FACING_SOUTHEAST + FACING_STEP, 40);
    ObjectEvent_FaceDirection(KALAY_KAJA, FACING_EAST, 0);
    ObjectEvent_FaceDirection(KALAY_CREWMAN, FACING_NORTH + FACING_STEP, 20);
    Engine_ObjectEventRunRepeatedMotion(KALAY_CAPTAIN, 2);
    Engine_ScriptWait(20);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    ObjectEvent_SetAttachedEffect(KALAY_KAJA, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(KALAY_CREWMAN, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(40);
    Engine_ObjectEventSetAnimationAndWait(KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    FieldCamera_MoveTo(PIXELS(182), -1, PIXELS(760), 1);
    ObjectEvent_SetSpeed(KALAY_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetActionScript(KALAY_KAJA, gTolbiShipDeck_KajaLeavesActions);
    ObjectEvent_SetSpeed(KALAY_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetActionScript(KALAY_CAPTAIN,
                                gTolbiShipDeck_CaptainStepsAsideActions);
    ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 182, 0x2f8);
    Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
    ObjectEvent_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          60);
    TolbiShipDeck_Turn(KALAY_CREWMAN, FACING_NORTH + FACING_STEP);
    Script_ShowMessageAndWait(KALAY_CREWMAN, 0, 20);
    Engine_ObjectEventSetAnimationAndWait(KALAY_CREWMAN, 3);
    Engine_ObjectEventJump(KALAY_CREWMAN, 4, 0);
    ObjectEvent_FaceDirection(KALAY_CREWMAN, FACING_SOUTHEAST + FACING_STEP,
                              40);
    FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(790), 1);
    {
      struct ObjectEvent *record = ObjectEvent_GetById(KALAY_CREWMAN);
      record->priorityFlags =
          OBJ_EVENT_PRIORITY_AUTOMATIC | record->priorityFlags;
    }
    ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 182, 0x30e);
    ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 192, 0x328);
    ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 216, 0x328);
    TolbiShipDeck_Turn(KALAY_CREWMAN, FACING_NORTH + FACING_STEP);
    Engine_ObjectEventRunRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_OpenSecondDoor();
    ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 216, 0x31e);
    ObjectEvent_SetPosition(KALAY_CREWMAN, 0, 0);
    record = ObjectEvent_GetById(KALAY_CREWMAN);
    record->scaleX = 0x10000;
    record = ObjectEvent_GetById(KALAY_CREWMAN);
    record->scaleY = 0x10000;
    GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
    Engine_ScriptEnd();
  }
}

/// On deck at Kalay, approaching from the east side of the door
/// (TOLBI_SHIP_DECK_CELL_EAST_OF_DOOR): the crewman comes out, protests and
/// slips away round the party to the bow. "I didn't do anything..." Runs until
/// the party has confronted the crewman.
static void TolbiShipDeck_CatchCrewmanFromEast(void) {
  u8 bits;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Engine_ScriptStart();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(2) / 5, PIXELS(1) / 5);
      ObjectEvent_GetById(KALAY_CREWMAN)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 232, 0x330);
      Engine_ScriptWait(1);
      bits = 1;
      {
        struct ObjectEvent *record = ObjectEvent_GetById(KALAY_CREWMAN);
        u8 value = record->steeringFlags;

        record->steeringFlags = value | bits;
      }
      Engine_ScriptWait(20);
      Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(KALAY_CREWMAN);
      ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
      ObjectEvent_GetById(KALAY_CREWMAN)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 244, 0x324);
      Engine_ScriptWait(1);
      {
        struct ObjectEvent *record = ObjectEvent_GetById(KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Engine_ScriptWait(20);
      ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(16) / 5, PIXELS(8) / 5);
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 248, 0x30a);
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 248, 0x2bc);
      ObjectEvent_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      ObjectEvent_FaceDirection(KALAY_CREWMAN, FACING_EAST, 0);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Engine_ScriptEnd();
    }
  }
}

/// The same, approaching from the west side
/// (TOLBI_SHIP_DECK_CELL_WEST_OF_DOOR).
static void TolbiShipDeck_CatchCrewmanFromWest(void) {
  u8 bits;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Engine_ScriptStart();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(2) / 5, PIXELS(1) / 5);
      ObjectEvent_GetById(KALAY_CREWMAN)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 202, 0x330);
      Engine_ScriptWait(1);
      bits = 1;
      {
        struct ObjectEvent *record = ObjectEvent_GetById(KALAY_CREWMAN);
        u8 value = record->steeringFlags;

        record->steeringFlags = value | bits;
      }
      Engine_ScriptWait(20);
      Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(KALAY_CREWMAN);
      ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
      ObjectEvent_GetById(KALAY_CREWMAN)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 192, 0x324);
      Engine_ScriptWait(1);
      {
        struct ObjectEvent *record = ObjectEvent_GetById(KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Engine_ScriptWait(20);
      ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(16) / 5, PIXELS(8) / 5);
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 180, 0x30a);
      ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 180, 0x2bc);
      ObjectEvent_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      ObjectEvent_FaceDirection(KALAY_CREWMAN, FACING_EAST, 0);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Engine_ScriptEnd();
    }
  }
}

/// The start both of those share: the camera pans to the door, which opens, and
/// the crewman steps out and denies everything. Records the confrontation.
static void TolbiShipDeck_ConfrontCrewman(void) {
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(824), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  TolbiShipDeck_OpenSecondDoor();
  MapGrid_CopyCellsTo(30, 108, 13, 108, 1, 2);
  Engine_ScriptWait(10);
  ObjectEvent_SetPosition(KALAY_CREWMAN, PIXELS(216), PIXELS(800));
  ObjectEvent_SetSpeed(KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(KALAY_CREWMAN, 216, 0x32e);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, KALAY_CREWMAN, 10);
  Engine_ObjectEventSetAnimationAndWait(KALAY_CREWMAN, 4);
  Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
  ObjectEvent_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_FaceObjectEvent(KALAY_CREWMAN, LOCALID_PARTY_LEADER, 20);
  Engine_ObjectEventStartRepeatedMotion(KALAY_CREWMAN, 2);
  Engine_ScriptSetMessage(MsgTolbiShipDidntDoAnything);
  Script_ShowMessageAndWait(KALAY_CREWMAN, 0, 20);
  Engine_ObjectEventShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT);
}

/// Sets up the map on entry: clears FLAG_WORLD_MAP_LEFT, sets the
/// transition, starts the deck's sway at a random angle and stops the sea's
/// scroll, redraws the map grid and applies the voyage's state.
s32 TolbiShipDeck_Initialize(void) {
  void **base = gMapGridWork;
  struct MapGridLayerScroll *layer =
      &((struct MapGridScrollWork *)base[0])->layers[MAPGRID_LAYER_GROUND];

  Engine_GameFlagClear(FLAG_WORLD_MAP_LEFT);
  ((struct EventWork *)base[HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID])->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  layer->speedY = 0;
  sSwayAngleX = (u16)Engine_RandomNext();
  sSwayAngleY = (u16)Engine_RandomNext();
  sSeaScroll[0] = 0;
  sSeaScroll[1] = 0;
  sSeaSpeed[0] = 0;
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  TolbiShipDeck_ApplyEntryState();
  return 0;
}

/// Applies the voyage's state: which cells and oars are drawn, whether the deck
/// rocks and the sea scrolls, and which cutscene the warp destination starts
/// (the warp destinations from TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY
/// on are the voyage's cutscenes, chained by their warps).
static void TolbiShipDeck_ApplyEntryState(void) {
  s32 flag;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_MAP_TEMP_71);
    Engine_GameFlagClear(FLAG_MAP_TEMP_72);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
    Engine_ObjectEventSetChildValue(ARRIVED_FIRST_PASSENGER + 1, 2);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
  } else if ((flag = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING)) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_SetUpOars();
    Task_AddCallback(TolbiShipDeck_RowOars, TASK_PRIORITY_MAP);
    sSeaScroll[1] = 0x200000;
    sSeaSpeed[1] = 0x13333;
    FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, TASK_PRIORITY_MAP);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
             0) {
    TolbiShipDeck_CopyUnderwayCells();
    sSeaScroll[1] = flag;
    sSeaSpeed[1] = flag;
    FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, TASK_PRIORITY_MAP);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING) == 0) {
    TolbiShipDeck_RestOars();
  }
  switch (gGameState.warpDest) {
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST:
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite->image.priority = 1;
    break;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      TolbiShipDeck_DepartWithParty();
    } else {
      TolbiShipDeck_DepartFromKalay();
    }
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS:
    TolbiShipDeck_SightSeaMonsters();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD:
    TolbiShipDeck_MonstersBoard();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK:
    TolbiShipDeck_MonstersAttackCrew();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK:
    TolbiShipDeck_MonsterLandsOnDeck();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN:
    TolbiShipDeck_JumpIntoKrakenBattle();
    return;
  case TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      TolbiShipDeck_SetUpAfterKraken();
    } else {
      TolbiShipDeck_ThankForKraken();
    }
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED:
    TolbiShipDeck_SightLand();
    return;
  case TOLBI_SHIP_DECK_FROM_WORLD_MAP:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      TolbiShipDeck_RowersDisembark();
    }
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE:
    TolbiShipDeck_SightIsland();
    return;
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->minX = PIXELS(65);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_SetUpAtAnchor();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipDeck_SetUpAfterKraken();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
             0) {
    TolbiShipDeck_SetUpUnderSail();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    TolbiShipDeck_PlacePassengersAgreedToRow();
  } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    TolbiShipDeck_PlaceWaitingPassengers();
  }
}

/// At Kalay while the ship waits: places the passengers by what has happened so
/// far.
static void TolbiShipDeck_PlaceWaitingPassengers(void) {
  struct ObjectEvent *objectEvent;
  s32 flag;

  Engine_ObjectEventSetSpritePriority(KALAY_PASSENGER_KARAGOL, 1);
  Engine_ObjectEventSetSpritePriority(KALAY_KAJA, 1);
  Engine_ObjectEventSetSpritePriority(KALAY_CAPTAIN, 1);
  Engine_ObjectEventSetSpritePriority(KALAY_PASSENGER_SEEING_COLOSSO, 1);
  Engine_ObjectEventSetSpritePriority(KALAY_TOUR_GUIDE, 1);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY) != 0) {
    Engine_ObjectEventSetPosition(KALAY_CAPTAIN, PIXELS(162), PIXELS(666));
    ObjectEvent_GetById(KALAY_CAPTAIN)->facing = FACING_WEST;
    Engine_ObjectEventSetPosition(KALAY_KAJA, 0, 0);
    Engine_ObjectEventSetPosition(KALAY_CREWMAN, 0, 0);
  }
  flag = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
  if (flag != 0) {
    Engine_ObjectEventSetPosition(KALAY_CREW_MEMBER, PIXELS(264), PIXELS(702));
    ObjectEvent_GetById(KALAY_CREW_MEMBER)->facing = FACING_SOUTH + FACING_STEP;
    objectEvent = ObjectEvent_GetById(KALAY_CREW_MEMBER);
    objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    ObjectEvent_SetActionScript(KALAY_CREW_MEMBER,
                                gTolbiShipDeck_SquashAndStretchActions);
    Engine_ObjectEventSetPosition(KALAY_TOUR_GUIDE, PIXELS(248), PIXELS(680));
    objectEvent = ObjectEvent_Get(KALAY_TOUR_GUIDE);
    objectEvent->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    ObjectEvent_SetActionScript(KALAY_TOUR_GUIDE,
                                gTolbiShipDeck_SquashAndStretchActions);
    Engine_ObjectEventSetPosition(KALAY_CAPTAIN, 0, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0) {
      ObjectEvent_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      ObjectEvent_GetById(KALAY_CREWMAN)->facing = flag;
    }
  }
}

/// At Kalay once the anchor charm is back
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED): KALAY_CREW_MEMBER, the tour guide
/// and the three passengers take new places, and the crewman, the captain and
/// Kaja leave the deck.
static void TolbiShipDeck_PlacePassengersAgreedToRow(void) {
  Engine_ObjectEventSetPosition(KALAY_CREW_MEMBER, PIXELS(262), PIXELS(706));
  ObjectEvent_GetById(KALAY_CREW_MEMBER)->facing = FACING_SOUTH + FACING_STEP;
  Engine_ObjectEventSetPosition(KALAY_TOUR_GUIDE, PIXELS(164), PIXELS(648));
  ObjectEvent_GetById(KALAY_TOUR_GUIDE)->facing = FACING_EAST;
  Engine_ObjectEventSetSpritePriority(KALAY_TOUR_GUIDE, 1);
  Engine_ObjectEventSetPosition(KALAY_PASSENGER_WAITING_LONG, PIXELS(198),
                                PIXELS(665));
  ObjectEvent_GetById(KALAY_PASSENGER_WAITING_LONG)->facing = FACING_WEST;
  Engine_ObjectEventSetSpritePriority(KALAY_PASSENGER_WAITING_LONG, 1);
  Engine_ObjectEventSetPosition(KALAY_PASSENGER_SEEING_COLOSSO, PIXELS(188),
                                PIXELS(678));
  ObjectEvent_GetById(KALAY_PASSENGER_SEEING_COLOSSO)->facing =
      FACING_NORTHWEST + FACING_STEP;
  Engine_ObjectEventSetPosition(KALAY_PASSENGER_KARAGOL, PIXELS(186),
                                PIXELS(635));
  ObjectEvent_GetById(KALAY_PASSENGER_KARAGOL)->facing =
      FACING_SOUTH + FACING_STEP;
  Engine_ObjectEventSetPosition(KALAY_CAPTAIN, 0, 0);
  Engine_ObjectEventSetPosition(KALAY_KAJA, 0, 0);
  Engine_ObjectEventSetPosition(KALAY_CREWMAN, 0, 0);
}

/// Under sail: the crew take their places, the two lookouts start their rounds
/// and, when the party keeps its position, the leader stands on
/// deck.
static void TolbiShipDeck_SetUpUnderSail(void) {
  struct ObjectEvent *record;
  u8 bits;

  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_KAJA, 0, 0);
  ObjectEvent_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  ObjectEvent_SetPosition(SEA_SEAN, PIXELS(204), PIXELS(521));
  record = ObjectEvent_GetById(SEA_SEAN);
  record->y.fixed = PIXELS(16);
  bits = 128;
  {
    struct ObjectEvent *record = ObjectEvent_GetById(SEA_SEAN);
    u8 value = record->collisionFlags;

    record->collisionFlags = value | bits;
  }
  ObjectEvent_SetSpeed(SEA_SEAN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetActionScript(SEA_SEAN, gTolbiShipDeck_SeanPacesActions);
  {
    struct ObjectEvent *record = ObjectEvent_GetById(SEA_OURANOS);

    bits |= record->collisionFlags;
    record->collisionFlags = bits;
  }
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetActionScript(SEA_OURANOS,
                              gTolbiShipDeck_OuranosPatrolsActions);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Engine_ScriptEnd();
}

/// After the Kraken: as under sail, with Sean at the helm.
static void TolbiShipDeck_SetUpAfterKraken(void) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_KAJA, 0, 0);
  ObjectEvent_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  ObjectEvent_SetPosition(SEA_SEAN, PIXELS(268), PIXELS(678));
  record = ObjectEvent_GetById(SEA_SEAN);
  record->facing = FACING_EAST;
  ObjectEvent_SetActionScript(SEA_SEAN, gTolbiShipDeck_SeanLooksAboutActions);
  {
    struct ObjectEvent *record = ObjectEvent_GetById(SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetActionScript(SEA_OURANOS,
                              gTolbiShipDeck_OuranosPatrolsActions);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Engine_ScriptEnd();
}

/// At anchor off the island: the crew stand where they were left, by whether
/// the party has agreed to scout the island, and entering by
/// TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS means the party is back from it.
static void TolbiShipDeck_SetUpAtAnchor(void) {
  struct ObjectEvent *record;
  s32 facing;

  ((struct MapGridScrollWork *)gMapGridWork[0])->minX = PIXELS(65);
  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_KrakenObjectEventTemplates);
  Engine_TaskWait(1);
  Engine_ObjectEventDestroy(SEA_KRAKEN);
  ObjectEvent_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  facing = 192; // 192 << 6 is 0x3000, south-southeast
  record = ObjectEvent_GetById(SEA_CREW_MEMBER);
  record->facing = (facing << 6);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED) != 0) {
    ObjectEvent_SetPosition(SEA_SEAN, PIXELS(162), PIXELS(634));
    record = ObjectEvent_GetById(SEA_SEAN);
    record->facing = (facing << 6);
    ObjectEvent_SetPosition(SEA_OURANOS, PIXELS(162), PIXELS(676));
    record = ObjectEvent_GetById(SEA_OURANOS);
    record->facing = FACING_NORTH + FACING_STEP;
  } else {
    Engine_ObjectEventSetPosition(SEA_SEAN, PIXELS(160), PIXELS(652));
    record = ObjectEvent_GetById(SEA_SEAN);
    record->facing = (facing << 6);
    ObjectEvent_SetPosition(SEA_OURANOS, PIXELS(166), PIXELS(668));
    record = ObjectEvent_GetById(SEA_OURANOS);
    record->facing = FACING_NORTHWEST + FACING_STEP;
  }
  if (gGameState.warpDest == TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) {
    TolbiShipDeck_ReturnFromIsland();
  }
  Engine_ScriptEnd();
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY, setting sail from Kalay
/// before the party is on deck: Kaja (KALAY_KAJA) brings word to
/// KALAY_CREW_MEMBER that all is ready.
static void TolbiShipDeck_DepartFromKalay(void) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(KALAY_CREWMAN, 0, 0);
  ObjectEvent_SetPosition(KALAY_CAPTAIN, 0, 0);
  ObjectEvent_SetPosition(KALAY_TOUR_GUIDE, 0, 0);
  ObjectEvent_SetPosition(KALAY_PASSENGER_WAITING_LONG, 0, 0);
  ObjectEvent_SetPosition(KALAY_PASSENGER_SEEING_COLOSSO, 0, 0);
  ObjectEvent_SetPosition(KALAY_PASSENGER_KARAGOL, 0, 0);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(KALAY_KAJA, 0, 0);
  record = ObjectEvent_GetById(KALAY_KAJA);
  record->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_SetPosition(KALAY_CREW_MEMBER, PIXELS(232), PIXELS(650));
  record = ObjectEvent_GetById(KALAY_CREW_MEMBER);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(636), 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  TolbiShipDeck_AnnouncePreparationsReady(KALAY_KAJA, KALAY_CREW_MEMBER);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY on later departures, with
/// the party aboard: Kaja brings word to SEA_CREW_MEMBER.
static void TolbiShipDeck_DepartWithParty(void) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(232), PIXELS(636));
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 15);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(record, 0);
  Engine_TaskWait(1);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  Engine_ObjectEventStop(SEA_SEAN);
  Engine_ObjectEventStop(SEA_OURANOS);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_SEAN, 0, 0);
  ObjectEvent_SetPosition(SEA_OURANOS, 0, 0);
  ObjectEvent_SetPosition(SEA_KAJA, 0, 0);
  record = ObjectEvent_GetById(SEA_KAJA);
  record->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_SetPosition(SEA_CREW_MEMBER, PIXELS(232), PIXELS(650));
  record = ObjectEvent_GetById(SEA_CREW_MEMBER);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  Engine_TaskWait(1);
  TolbiShipDeck_AnnouncePreparationsReady(SEA_KAJA, SEA_CREW_MEMBER);
}

/// The messenger comes up through the first door to report, the crew member
/// waiting there calls the ship ready, and the scene moves on to the next stage
/// of the voyage. "Preparations... Ready!"
static void TolbiShipDeck_AnnouncePreparationsReady(s32 messenger,
                                                    s32 crewMember) {
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  TolbiShipDeck_OpenFirstDoor();
  ObjectEvent_SetPosition(messenger, PIXELS(216), PIXELS(588));
  ObjectEvent_SetSpeed(messenger, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(messenger, 216, 0x258);
  ObjectEvent_WalkToAndWait(messenger, 218, 0x25c);
  ObjectEvent_WalkToAndWait(messenger, 234, 0x25c);
  ObjectEvent_WalkToAndWait(messenger, 236, 0x26a);
  ObjectEvent_FaceDirection(messenger, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(messenger, 3);
  Engine_ScriptWait(20);
  TolbiShipDeck_Turn(crewMember, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventJump(crewMember, 4, 40);
  Engine_ObjectEventStartRepeatedMotion(crewMember, 2);
  Engine_ScriptSetMessage(MsgTolbiShipPreparationsReady);
  Script_ShowMessageAndWait(crewMember, 0, 20);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS: sea monsters are
/// sighted. "Aye, Captain! Sea monsters!"
static void TolbiShipDeck_SightSeaMonsters(void) {
  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                                   0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(SEA_KAJA, 1);
  Engine_ScriptSetMessage(MsgTolbiShipAyeCaptainSeaMonsters);
  Engine_ScriptShowMessageAndWait(SEA_KAJA, 0, 10);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventJump(SEA_SEAN, 4, 20);
  Engine_ObjectEventStartRepeatedMotion(SEA_SEAN, 2);
  Engine_ScriptShowMessageAndWait(SPEAKER(SEA_SEAN, 6), 0, 20);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_TURN);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD: the monsters leap
/// aboard and someone cries out. "It's no use. It's too late!"
static void TolbiShipDeck_MonstersBoard(void) {
  s32 zero;
  const s32 *shared;

  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)
          gTolbiShipDeck_MonstersBoardObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_OURANOS, PIXELS(248), PIXELS(728));
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                                   0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 242, 692);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 196, 678);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 182, 654);
  Engine_ObjectEventRunRepeatedMotion(SEA_OURANOS, 2);
  Engine_ScriptSetMessage(MsgTolbiShipNoUseLate);
  TolbiShipDeck_Say(SPEAKER(SEA_OURANOS, 10));
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(12) / 5, PIXELS(6) / 5);
  FieldObjectMotion_ResetAndSetPosition(LOCALID_PARTY_LEADER, 154, 609);
  Engine_AudioPlayCue(SE_146);
  zero = 0;
  ObjectEvent_GetById(SEA_FIRST_ATTACK_MONSTER)->timer = zero;
  ObjectEvent_GetById(SEA_FIRST_ATTACK_MONSTER + 1)->timer = zero;
  ObjectEvent_GetById(SEA_FIRST_ATTACK_MONSTER + 2)->timer = zero;
  ObjectEvent_SetPosition(SEA_FIRST_ATTACK_MONSTER, PIXELS(32), PIXELS(484));
  ObjectEvent_SetPosition(SEA_FIRST_ATTACK_MONSTER + 1, PIXELS(84),
                          PIXELS(496));
  ObjectEvent_SetPosition(SEA_FIRST_ATTACK_MONSTER + 2, PIXELS(16),
                          PIXELS(596));
  ObjectEvent_SetSpeed(SEA_FIRST_ATTACK_MONSTER, PIXELS(12) / 5, PIXELS(6) / 5);
  ObjectEvent_SetSpeed(SEA_FIRST_ATTACK_MONSTER + 1, PIXELS(12) / 5,
                       PIXELS(6) / 5);
  ObjectEvent_SetSpeed(SEA_FIRST_ATTACK_MONSTER + 2, PIXELS(12) / 5,
                       PIXELS(6) / 5);
  shared = gTolbiShipDeck_MonsterLeapsAboardActions;
  ObjectEvent_SetActionScript(SEA_FIRST_ATTACK_MONSTER, shared);
  ObjectEvent_SetActionScript(SEA_FIRST_ATTACK_MONSTER + 1, shared);
  ObjectEvent_SetActionScript(SEA_FIRST_ATTACK_MONSTER + 2, shared);
  Engine_ObjectEventSetChildValue(SEA_FIRST_ATTACK_MONSTER, 3);
  Engine_ObjectEventSetChildValue(SEA_FIRST_ATTACK_MONSTER + 1, 3);
  Engine_ObjectEventSetChildValue(SEA_FIRST_ATTACK_MONSTER + 2, 3);
  do {
    Engine_TaskWait(1);
  } while (ObjectEvent_GetById(SEA_FIRST_ATTACK_MONSTER)->timer == 0);
  TolbiShipDeck_OpenFirstDoor();
  /* FAKEMATCH(one-pass): the block sets r0 = 21 straight after the call above;
   * as a plain call, 1 of 163 instructions differ in tbs-en. */
  do {
    ObjectEvent_WalkTo(SEA_OURANOS, 196, 612);
  } while (0);
  ObjectEvent_RefreshSelectorById(SEA_FIRST_ATTACK_MONSTER);
  Engine_ScriptWait(10);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(10);
  ObjectSystem_SuspendProjectionAndShowSprites();
  Message_SetVoiceFromObjectEvent(SEA_OURANOS);
  FieldMessage_ShowCentered(MsgTolbiShipArrgh, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_ScriptRequestWarp(
      TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_FIRST_ATTACK);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK: the monsters harry the
/// crew while more of them climb aboard.
static void TolbiShipDeck_MonstersAttackCrew(void) {

  Engine_ScriptStart();
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                                   0);
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)
          gTolbiShipDeck_SecondAttackObjectEventTemplates);
  Engine_TaskWait(1);
  Engine_ObjectEventSetAnimation(SEA_SPLASH, 0);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER), 1);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER + 1), 1);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER + 2), 1);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER + 3), 1);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_SECOND_ATTACK_MONSTER), 1);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_SECOND_ATTACK_MONSTER + 1), 1);
  ObjectEvent_SetPosition(SEA_SEAN, PIXELS(256), PIXELS(640));
  ObjectEvent_SetActionScript(SEA_SEAN, gTolbiShipDeck_LookAroundActions);
  ObjectEvent_SetPosition(SEA_OURANOS, PIXELS(268), PIXELS(692));
  ObjectEvent_SetActionScript(SEA_SEAN, gTolbiShipDeck_LookAroundActions);
  ObjectEvent_SetPosition(SEA_FIRST_HOVERING_MONSTER, PIXELS(242), PIXELS(604));
  ObjectEvent_SetPosition(SEA_FIRST_HOVERING_MONSTER + 1, PIXELS(264),
                          PIXELS(600));
  ObjectEvent_SetPosition(SEA_FIRST_HOVERING_MONSTER + 2, PIXELS(254),
                          PIXELS(668));
  Engine_ObjectEventSetPosition(SEA_FIRST_HOVERING_MONSTER + 3, PIXELS(282),
                                PIXELS(658));
  ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER)->riseEnabled = 0;
  ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER + 1)->riseEnabled = 1;
  ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER + 2)->riseEnabled = 0;
  ObjectEvent_GetById(SEA_FIRST_HOVERING_MONSTER + 3)->riseEnabled = 1;
  ObjectEvent_SetActionScript(SEA_FIRST_HOVERING_MONSTER,
                              gTolbiShipDeck_HoverAndDiveAtSeanActions);
  ObjectEvent_SetActionScript(SEA_FIRST_HOVERING_MONSTER + 1,
                              gTolbiShipDeck_HoverAndDiveAtSeanActions);
  ObjectEvent_SetActionScript(SEA_FIRST_HOVERING_MONSTER + 2,
                              gTolbiShipDeck_HoverAndDiveAtOuranosActions);
  ObjectEvent_SetActionScript(SEA_FIRST_HOVERING_MONSTER + 3,
                              gTolbiShipDeck_HoverAndDiveAtOuranosActions);
  Engine_ObjectEventSetPosition(SEA_KAJA, 0, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(0x190);
  ObjectEvent_SetPosition(SEA_SECOND_ATTACK_MONSTER, PIXELS(254), PIXELS(740));
  ObjectEvent_SetPosition(SEA_SECOND_ATTACK_MONSTER + 1, PIXELS(24),
                          PIXELS(586));
  ObjectEvent_SetSpeed(SEA_SECOND_ATTACK_MONSTER, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(SEA_SECOND_ATTACK_MONSTER + 1, PIXELS(8) / 5,
                       PIXELS(4) / 5);
  ObjectEvent_SetDestination(SEA_SECOND_ATTACK_MONSTER + 1, 172, 0x284);
  Object_MotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER, 200, 0x294);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4), PIXELS(2));
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 174, 0x26c);
  Engine_ObjectMotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER, 180,
                                          0x244);
  Engine_AudioPlayCue(SE_146);
  ObjectEvent_SetActionScript(SEA_SECOND_ATTACK_MONSTER,
                              gTolbiShipDeck_BobActions);
  ObjectEvent_SetActionScript(SEA_SECOND_ATTACK_MONSTER + 1,
                              gTolbiShipDeck_BobActions);
  Engine_AudioPlayCue(SE_240);
  ObjectEvent_SetPosition(SEA_SPLASH, PIXELS(134), PIXELS(594));
  ObjectEvent_SetActionScript(SEA_SPLASH, gTolbiShipDeck_SwellActions);
  Engine_ScriptWait(10);
  ObjectEvent_SetPosition(SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(134),
                          PIXELS(584));
  ObjectEvent_SetSpeed(SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(4), PIXELS(2));
  ObjectEvent_GetById(SEA_SECOND_ATTACK_MONSTER + 2)->velocityY = PIXELS(8);
  Engine_ObjectMotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER + 2, 186,
                                          0x264);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(SEA_SECOND_ATTACK_MONSTER + 2), 1);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(2), PIXELS(1));
  Engine_ObjectMotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER + 2, 216,
                                          0x258);
  TolbiShipDeck_Turn(SEA_SECOND_ATTACK_MONSTER + 2, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Engine_ScriptWait(10);
  ObjectEvent_SetActionScript(SEA_SECOND_ATTACK_MONSTER + 2,
                              gTolbiShipDeck_MonsterGoesBelowActions);
  Engine_ScriptWait(10);
  ObjectEvent_SetActionScript(SEA_SECOND_ATTACK_MONSTER,
                              gTolbiShipDeck_MonsterGoesBelowActions);
  Engine_ScriptWait(10);
  ObjectEvent_RunActionScriptAndWait(SEA_SECOND_ATTACK_MONSTER + 1,
                                     gTolbiShipDeck_MonsterGoesBelowActions);
  Engine_ScriptWait(20);
  Engine_AudioPlayCue(SE_147);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ObjectEventStop(SEA_FIRST_HOVERING_MONSTER);
  Engine_ObjectEventStop(SEA_FIRST_HOVERING_MONSTER + 1);
  Engine_ObjectEventStop(SEA_FIRST_HOVERING_MONSTER + 2);
  Engine_ObjectEventStop(SEA_FIRST_HOVERING_MONSTER + 3);
  Engine_ScriptWait(10);
  ObjectSystem_SuspendProjectionAndShowSprites();
  Message_SetVoiceFromObjectEvent(SEA_OURANOS);
  FieldMessage_ShowCentered(MsgTolbiShipArrgh, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_ScriptRequestWarp(
      TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SECOND_ATTACK);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK: a great shape rises from
/// the sea and a monster drops onto the deck.
static void TolbiShipDeck_MonsterLandsOnDeck(void) {
  struct ObjectEvent *monster;
  struct ObjectEvent *record;
  s32 frame;
  const s32 *sailorActions;

  Engine_ScriptStart();
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 15);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(record, 0);
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)
          gTolbiShipDeck_ThirdAttackObjectEventTemplates);
  Engine_TaskWait(1);
  Engine_ObjectEventSetPosition(SEA_SEAN, PIXELS(176), PIXELS(696));
  record = ObjectEvent_GetById(SEA_SEAN);
  record->facing = FACING_NORTH + FACING_STEP;
  Engine_ObjectEventSetPosition(SEA_OURANOS, PIXELS(264), PIXELS(662));
  record = ObjectEvent_GetById(SEA_OURANOS);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  ObjectEvent_SetPosition(SEA_FIRST_LEAPING_MONSTER, PIXELS(184), PIXELS(672));
  ObjectEvent_SetPosition(SEA_FIRST_LEAPING_MONSTER + 1, PIXELS(202),
                          PIXELS(692));
  ObjectEvent_SetPosition(SEA_FIRST_LEAPING_MONSTER + 2, PIXELS(252),
                          PIXELS(646));
  ObjectEvent_SetPosition(SEA_FIRST_LEAPING_MONSTER + 3, PIXELS(256),
                          PIXELS(686));
  ObjectEvent_SetPosition(SEA_THIRD_ATTACK_MONSTER, PIXELS(172), PIXELS(632));
  Engine_ObjectEventSetPosition(SEA_THIRD_ATTACK_MONSTER + 1, PIXELS(256),
                                PIXELS(622));
  ObjectEvent_GetById(SEA_FIRST_LEAPING_MONSTER)->riseEnabled = 0;
  ObjectEvent_GetById(SEA_FIRST_LEAPING_MONSTER + 1)->riseEnabled = 1;
  ObjectEvent_GetById(SEA_FIRST_LEAPING_MONSTER + 2)->riseEnabled = 0;
  ObjectEvent_GetById(SEA_FIRST_LEAPING_MONSTER + 3)->riseEnabled = 2;
  Engine_ObjectEventSetPosition(SEA_KAJA, 0, 0);
  ObjectEvent_SetActionScript(SEA_FIRST_LEAPING_MONSTER,
                              gTolbiShipDeck_LeapAtSeanActions);
  ObjectEvent_SetActionScript(SEA_FIRST_LEAPING_MONSTER + 1,
                              gTolbiShipDeck_LeapAtSeanActions);
  ObjectEvent_SetActionScript(SEA_FIRST_LEAPING_MONSTER + 2,
                              gTolbiShipDeck_LeapAtOuranosActions);
  ObjectEvent_SetActionScript(SEA_FIRST_LEAPING_MONSTER + 3,
                              gTolbiShipDeck_LeapAtOuranosActions);
  ObjectEvent_SetActionScript(SEA_THIRD_ATTACK_MONSTER,
                              gTolbiShipDeck_HopActions);
  ObjectEvent_SetActionScript(SEA_THIRD_ATTACK_MONSTER + 1,
                              gTolbiShipDeck_HopActions);
  Engine_ObjectEventSetChildValue(SEA_FIRST_LEAPING_MONSTER, 3);
  Engine_ObjectEventSetChildValue(SEA_FIRST_LEAPING_MONSTER + 1, 3);
  Engine_ObjectEventSetChildValue(SEA_FIRST_LEAPING_MONSTER + 2, 3);
  Engine_ObjectEventSetChildValue(SEA_FIRST_LEAPING_MONSTER + 3, 3);
  Engine_ObjectEventSetChildValue(SEA_THIRD_ATTACK_MONSTER, 3);
  Engine_ObjectEventSetChildValue(SEA_THIRD_ATTACK_MONSTER + 1, 3);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(80);
  Engine_AudioPlayCue(SE_147);
  // The shadow grows where the monster will land, then it drops from 80
  // pixels up.
  monster = ObjectEvent_GetById(SEA_FALLING_MONSTER_SHADOW);
  monster->scaleX = Q16_ONE / 10;
  monster->scaleY = Q16_ONE / 10;
  monster->x.fixed = PIXELS(194);
  monster->z.fixed = PIXELS(642);
  for (frame = 0; (u32)frame < 16; frame++) {
    monster->scaleX += 3 * Q16_ONE / 50;
    monster->scaleY += 3 * Q16_ONE / 50;
    Engine_TaskWait(1);
  }
  monster = ObjectEvent_GetById(SEA_THIRD_ATTACK_MONSTER + 2);
  monster->scaleX = 11 * Q16_ONE / 10;
  monster->scaleY = 11 * Q16_ONE / 10;
  monster->x.fixed = PIXELS(194);
  monster->y.fixed = PIXELS(80);
  monster->z.fixed = PIXELS(642);
  monster->facing = FACING_SOUTH + FACING_STEP;
  monster->bounce = 2 * Q16_ONE / 5;
  monster->gravity = PIXELS(2);
  Engine_ScriptWait(80);
  Engine_AudioPlayCue(SE_147);
  Engine_ObjectEventSetPosition(SEA_FALLING_MONSTER_SHADOW, 0, 0);
  record = ObjectEvent_GetById(SEA_THIRD_ATTACK_MONSTER + 2);
  Engine_ObjectEventSetSpriteFlags(record, 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  monster = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  monster->motionFlags = 0;
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 216, 0x264);
  ObjectEvent_SetSpeed(SEA_THIRD_ATTACK_MONSTER + 2, PIXELS(8) / 5,
                       PIXELS(4) / 5);
  Object_MotionSetPositionAndCommit(SEA_THIRD_ATTACK_MONSTER + 2, 196, 0x258);
  Object_MotionSetPositionAndCommit(SEA_THIRD_ATTACK_MONSTER + 2, 216, 0x258);
  Engine_ObjectEventStop(SEA_THIRD_ATTACK_MONSTER);
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(SEA_THIRD_ATTACK_MONSTER, PIXELS(8) / 5, PIXELS(4) / 5);
  sailorActions = gTolbiShipDeck_MonsterGoesBelowActions;
  ObjectEvent_SetActionScript(SEA_THIRD_ATTACK_MONSTER, sailorActions);
  TolbiShipDeck_Turn(SEA_THIRD_ATTACK_MONSTER + 2, FACING_NORTH + FACING_STEP);
  TolbiShipDeck_OpenFirstDoor();
  Engine_ScriptWait(10);
  ObjectEvent_SetActionScript(SEA_THIRD_ATTACK_MONSTER + 2, sailorActions);
  Engine_ObjectEventStop(SEA_THIRD_ATTACK_MONSTER + 1);
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(SEA_THIRD_ATTACK_MONSTER + 1, PIXELS(8) / 5,
                       PIXELS(4) / 5);
  ObjectEvent_RunActionScriptAndWait(SEA_THIRD_ATTACK_MONSTER + 1,
                                     sailorActions);
  Engine_ScriptWait(20);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ObjectEventStop(SEA_FIRST_LEAPING_MONSTER);
  Engine_ObjectEventStop(SEA_FIRST_LEAPING_MONSTER + 1);
  Engine_ObjectEventStop(SEA_FIRST_LEAPING_MONSTER + 2);
  Engine_ObjectEventStop(SEA_FIRST_LEAPING_MONSTER + 3);
  Engine_ObjectEventStop(SEA_THIRD_ATTACK_MONSTER);
  Engine_ObjectEventStop(SEA_THIRD_ATTACK_MONSTER + 1);
  Engine_ScriptWait(10);
  ObjectSystem_SuspendProjectionAndShowSprites();
  Message_SetVoiceFromObjectEvent(SEA_OURANOS);
  FieldMessage_ShowCentered(MsgTolbiShipArrgh, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_ScriptRequestWarp(
      TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_THIRD_ATTACK);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN: the party jumps down to
/// the deck, gathers and charges; the battle with the Kraken begins, returning
/// to TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE.
static void TolbiShipDeck_JumpIntoKrakenBattle(void) {
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_KrakenObjectEventTemplates);
  Engine_TaskWait(1);
  Engine_CameraFollowObjectEvent(SEA_CAMERA_TARGET, 1);
  Engine_TaskWait(1);
  Engine_ObjectEventSetAnimation(SEA_OURANOS, 5);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(SEA_OURANOS), 0);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->facing = FACING_SOUTH;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  TolbiShipDeck_OpenFirstDoor();
  Engine_ScriptWait(10);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(216), PIXELS(586));
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 216, 598);
  Engine_ScriptWait(20);
  TolbiShipDeck_Turn(LOCALID_PARTY_LEADER, FACING_SOUTHWEST);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 194, 624);
  Engine_AudioPlayCue(SE_181);
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  Engine_ScriptWait(20);
  MapGrid_SetShake(-1, -1, 0xe666);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  ObjectEvent_GetById(SEA_CAMERA_TARGET)->motionFlags = 0;
  ObjectEvent_SetSpeed(SEA_CAMERA_TARGET, PIXELS(2), PIXELS(1));
  Engine_ObjectEventSetDestination(SEA_CAMERA_TARGET, 216, 612);
  Engine_AudioPlayCue(SE_149);
  Engine_ObjectEventSetSpritePriority(SEA_SEAN, 2);
  Engine_ObjectEventSetAnimation(SEA_SEAN, 5);
  objectEvent = ObjectEvent_GetById(SEA_SEAN);
  objectEvent->velocityY = PIXELS(8);
  objectEvent->gravity = 7 * PIXELS(1) / 10;
  objectEvent->scaleX = 0x1a000;
  objectEvent->scaleY = 0x1a000;
  objectEvent->update = TolbiShipDeck_ShrinkToNormalSize;
  objectEvent->bounce = 0x8000;
  ObjectEvent_SetSpeed(SEA_SEAN, PIXELS(6), PIXELS(3));
  Engine_ObjectMotionSetPositionAndCommit(SEA_SEAN, 182, 0x26a);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(SEA_SEAN), 0);
  TolbiShipDeck_Turn(LOCALID_PARTY_LEADER, FACING_NORTHWEST);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 6, 80);
  ObjectEvent_SetDestination(SEA_CAMERA_TARGET, 232, 564);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 204, 610);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 208, 598);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 248, 598);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(GARET, objectEvent->x.fixed,
                                  objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(IVAN, objectEvent->x.fixed,
                                  objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(MIA, objectEvent->x.fixed,
                                  objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 250, 584);
  ObjectEvent_WalkTo(GARET, 240, 600);
  ObjectEvent_WalkTo(IVAN, 254, 600);
  Engine_ObjectEventWalkToAndWait(MIA, 248, 616);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ObjectEventFaceDirection(MIA, FACING_NORTH, 20);
  Engine_AudioPlayCue(SE_149);
  Engine_ScriptWait(40);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventSetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                               ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 248, 564);
  ObjectEvent_WalkTo(GARET, 248, 564);
  Engine_ScriptWait(20);
  ObjectEvent_WalkTo(IVAN, 248, 564);
  ObjectEvent_WalkTo(MIA, 248, 564);
  Engine_ScriptWait(20);
  gGameState.savedMap = (s32)&MapId_TolbiShipInteriors;
  gGameState.savedWarpDest = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_TolbiShipDeck,
                                 TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 3);
  Engine_ScriptEnd();
}

/// TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE: Isaac is thanked for the
/// Kraken. "Thank you, Isaac. You did good against that Kraken."
static void TolbiShipDeck_ThankForKraken(void) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_KAJA, PIXELS(182), PIXELS(618));
  ObjectEvent_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  ObjectEvent_SetPosition(SEA_SEAN, PIXELS(268), PIXELS(678));
  record = ObjectEvent_GetById(SEA_SEAN);
  record->facing = FACING_EAST;
  ObjectEvent_SetActionScript(SEA_SEAN, gTolbiShipDeck_SeanLooksAboutActions);
  {
    struct ObjectEvent *record = ObjectEvent_GetById(SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetActionScript(SEA_OURANOS,
                              gTolbiShipDeck_OuranosPatrolsActions);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(SEA_KAJA, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 182, 0x224);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Turn(LOCALID_PARTY_LEADER, FACING_WEST);
  Engine_ObjectEventRunRepeatedMotion(SEA_KAJA, 1);
  Engine_ScriptSetMessage(MsgTolbiShipThankIsaacDidGoodAgainst);
  TolbiShipDeck_Say(SEA_KAJA);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(SEA_KAJA, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_ShowEmote(SEA_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(SEA_KAJA, 0, 40);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Say(SEA_KAJA);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(SEA_KAJA, 3);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 182, 0x258);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 216, 0x258);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Engine_ScriptWait(10);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 216, 0x244);
  ObjectEvent_SetPosition(SEA_KAJA, 0, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  Engine_ScriptEnd();
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED: land is sighted. "Hey, I
/// can see land!"
static void TolbiShipDeck_SightLand(void) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 15);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(record, 0);
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_KAJA, PIXELS(196), PIXELS(502));
  record = ObjectEvent_GetById(SEA_KAJA);
  record->facing = FACING_NORTHWEST;
  ObjectEvent_SetPosition(SEA_SEAN, PIXELS(184), PIXELS(524));
  record = ObjectEvent_GetById(SEA_SEAN);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  Engine_ObjectEventSetSpritePriority(SEA_OURANOS, 1);
  ObjectEvent_SetPosition(SEA_OURANOS, PIXELS(184), PIXELS(632));
  record = ObjectEvent_GetById(SEA_OURANOS);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  // UB: a u32 lvalue aliases the pointer, which keeps the facing stores ahead
  // of its load; the defined build reads it directly.
#ifdef UBFIX
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#else
  ((struct EventWork *)*(u32 *)&gFieldWork)->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#endif
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  Engine_ObjectEventJump(SEA_SEAN, 4, 10);
  Engine_ObjectEventJump(SEA_SEAN, 6, 20);
  Engine_ScriptSetMessage(MsgTolbiShipCanSeeLand);
  TolbiShipDeck_Say(SEA_SEAN);
  Engine_ObjectEventSetAnimationAndWait(SEA_KAJA, 3);
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(3), PIXELS(3) / 2);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 180, 0x222);
  Engine_ObjectEventFaceDirection(SEA_OURANOS, FACING_NORTHWEST + FACING_STEP,
                                  40);
  Engine_ObjectEventRunRepeatedMotion(SEA_OURANOS, 1);
  TolbiShipDeck_Say(SEA_OURANOS);
  Engine_ScriptRequestWarp(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI);
}

/// What each rower says coming up at Tolbi, named by its message: the first
/// four found take the first four replies in the order found; three passengers
/// have their own.
enum TolbiShipDeckRowerReply {
  ROWER_REPLY_FINALLY_LUCKY = 0,  ///< MsgTolbiShipMadeFinallyLucky
  ROWER_REPLY_OTHERS_ALREADY = 1, ///< MsgTolbiShipOtherPassengersAlready
  ROWER_REPLY_DONE_ROWING = 2,    ///< MsgTolbiShipDontRowAnymore
  ROWER_REPLY_NO_WORKOUT = 3,     ///< MsgTolbiShipOhhhHaventWorkout
  ROWER_REPLY_ROWING_MORE = 10,   ///< MsgTolbiShipRowingShipMore
  ROWER_REPLY_LOST_AT_SEA = 11,   ///< MsgTolbiShipTotallyLostOcean
  ROWER_REPLY_OLD_MAN = 20        ///< MsgTolbiShipMakeOldMan
};

/// TOLBI_SHIP_DECK_FROM_WORLD_MAP, at Tolbi: the four passengers who rowed come
/// up from below in turn and each says something, then the leader comes up and
/// is thanked for the hard work. Who rowed is read from the four seats' flag
/// groups, the passengers (ARRIVED_FIRST_PASSENGER and the eight after) in a
/// fixed order of preference.
static void TolbiShipDeck_RowersDisembark(void) {
  s32 roster[4];
  s32 matched[4];
  s32 category[4];
  u32 index;
  s32 found;
  s32 localId;
  s32 member;
  struct ObjectEvent *record;

  found = 0;
  for (index = 0; index <= 3; index++) {
    roster[index] = TolbiShipDeck_GetRowerAtSeat(index);
    category[index] = 0;
  }

  member = ARRIVED_FIRST_PASSENGER;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = ARRIVED_FIRST_PASSENGER + 1;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = ARRIVED_FIRST_PASSENGER + 2;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = ARRIVED_FIRST_PASSENGER + 4;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  if (found != 4) {
    member = ARRIVED_FIRST_PASSENGER + 5;
    for (index = 0; index <= 3; index++) {
      if (roster[index] == member) {
        matched[found] = member;
        category[found] = found;
        found++;
        break;
      }
    }
    if (found != 4) {
      member = ARRIVED_FIRST_PASSENGER + 6;
      for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
          matched[found] = member;
          category[found] = found;
          found++;
          break;
        }
      }
      if (found != 4) {
        member = ARRIVED_FIRST_PASSENGER + 3;
        for (index = 0; index <= 3; index++) {
          if (roster[index] == member) {
            matched[found] = member;
            category[found] = ROWER_REPLY_ROWING_MORE;
            found++;
            break;
          }
        }
        if (found != 4) {
          member = ARRIVED_FIRST_PASSENGER + 7;
          for (index = 0; index <= 3; index++) {
            if (roster[index] == member) {
              matched[found] = member;
              category[found] = ROWER_REPLY_LOST_AT_SEA;
              found++;
              break;
            }
          }
          if (found != 4) {
            member = ARRIVED_FIRST_PASSENGER + 8;
            for (index = 0; index <= 3; index++) {
              if (roster[index] == member) {
                matched[found] = member;
                category[found] = ROWER_REPLY_OLD_MAN;
                break;
              }
            }
          }
        }
      }
    }
  }

  Engine_ScriptStart();
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                                   0);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (record != 0) {
    Engine_ObjectEventSetPosition(ARRIVED_CAMERA_TARGET, record->x.fixed,
                                  record->z.fixed);
  }
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  Engine_CameraFollowObjectEvent(ARRIVED_CAMERA_TARGET, 1);

  TolbiShipDeck_OpenFirstDoor();

  Engine_ScriptWait(10);

  for (index = 0; index <= 3; index++) {
    localId = matched[index];
    ObjectEvent_SetPosition(localId, 216 << 16, 146 << 18);
    if (category[index] == ROWER_REPLY_OLD_MAN) {
      ObjectEvent_SetSpeed(localId, PIXELS(4) / 5, PIXELS(2) / 5);
    } else {
      ObjectEvent_SetSpeed(localId, 128 << 9, 128 << 8);
    }
    ObjectEvent_WalkToAndWait(localId, 216, 150 << 2);
    ObjectEvent_WalkToAndWait(localId, 192, 0x26a);
    Engine_ObjectEventWalkToAndWait(localId, 192, 164 << 2);

    switch (category[index]) {
    case ROWER_REPLY_FINALLY_LUCKY:
      Engine_ObjectEventShowEmote(localId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Engine_ScriptSetMessage(MsgTolbiShipMadeFinallyLucky);
      break;
    case ROWER_REPLY_OTHERS_ALREADY:
      TolbiShipDeck_Turn(localId, 208 << 8);
      Engine_ObjectEventShowEmote(localId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Engine_ScriptSetMessage(MsgTolbiShipOtherPassengersAlready);
      break;
    case ROWER_REPLY_DONE_ROWING:
      Engine_ObjectEventShowEmote(localId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Engine_ScriptSetMessage(MsgTolbiShipDontRowAnymore);
      break;
    case ROWER_REPLY_NO_WORKOUT:
      Engine_ObjectEventRunRepeatedMotion(localId, 1);
      Engine_ScriptSetMessage(MsgTolbiShipOhhhHaventWorkout);
      break;
    case ROWER_REPLY_ROWING_MORE:
      Engine_ObjectEventSetAnimationAndWait(localId, 3);
      Engine_ScriptSetMessage(MsgTolbiShipRowingShipMore);
      break;
    case ROWER_REPLY_LOST_AT_SEA:
      Engine_ObjectEventSetAnimation(localId, 4);
      Engine_ScriptSetMessage(MsgTolbiShipTotallyLostOcean);
      break;
    case ROWER_REPLY_OLD_MAN:
      Engine_ObjectEventSetAnimationAndWait(localId, 4);
      Engine_ObjectEventShowEmote(localId, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
      Engine_ScriptSetMessage(MsgTolbiShipMakeOldMan);
      break;
    default:
      break;
    }

    TolbiShipDeck_Say(localId);
    ObjectEvent_SetActionScript(localId, gTolbiShipDeck_RowerLeavesActions);
  }

  ObjectEvent_RefreshSelectorById(localId);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 216 << 16, 146 << 18);
  Engine_TaskWait(1);
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                                   1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 216, 150 << 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 190, 153 << 2);
  Engine_ScriptSetMessage(MsgTolbiShipThanksHardWork);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, 192 << 8, 0);

  ObjectEvent_GetById(ARRIVED_CAMERA_TARGET)->motionFlags = 0;
  ObjectEvent_SetSpeed(ARRIVED_CAMERA_TARGET, 128 << 10, 128 << 9);
  ObjectEvent_SetDestination(ARRIVED_CAMERA_TARGET, 196, 141 << 2);

  ObjectEvent_SetSpeed(ARRIVED_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(ARRIVED_KAJA, 182, 0x22b);
  ObjectEvent_FaceDirection(ARRIVED_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(ARRIVED_KAJA, 1);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ObjectEventSetAnimationAndWait(ARRIVED_KAJA, 3);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ObjectEventFaceDirection(ARRIVED_KAJA, 128 << 8, 40);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  TolbiShipDeck_Turn(ARRIVED_KAJA, FACING_SOUTHEAST + FACING_STEP);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ObjectEventSetAnimationAndWait(ARRIVED_KAJA, 3);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_WalkToAndWait(ARRIVED_KAJA, 188, 512);
  Engine_ObjectEventFaceDirection(ARRIVED_KAJA, FACING_SOUTHEAST + FACING_STEP,
                                  0);
  Engine_ObjectEventSetPosition(ARRIVED_CAMERA_TARGET, 0, 0);

  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_ScriptEnd();
}

/// Returns the passenger (ARRIVED_FIRST_PASSENGER and the eight after) who
/// promised to row at a seat (0 to 3), or 0 if the seat is empty. Each seat has
/// a group of nine flags, one for each passenger.
static s32 TolbiShipDeck_GetRowerAtSeat(u32 seat) {
  s32 flag = 0;
  s32 passenger = ARRIVED_FIRST_PASSENGER;
  u32 i;

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
  for (i = 0; i < 9; i++) {
    if (Engine_GameFlagIsSet(flag) != 0)
      return passenger;
    flag++;
    passenger++;
  }
  return 0;
}

#define TOLBI_SHIP_DECK_SEA_WRAP PIXELS(32)

/// The callback that rocks the deck while under sail: sways the camera on two
/// random-walking angles and offsets the BG3 sea pattern, wrapping at 32
/// pixels.
static void TolbiShipDeck_RockDeck(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  s32 *camera = grid->origin;
  s32 dx = Engine_MathCos(sSwayAngleX);
  s32 dy = Engine_MathSin(sSwayAngleY);
  struct MapGridLayerScroll *layer;

  *camera++ += dx >> 1;
  *camera += dy;
  sSwayAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
  sSwayAngleY += (u32)(Engine_RandomNext() << 9) >> 16;
  sSwayAngleX &= 0xffff;
  sSwayAngleY &= 0xffff;
  layer = &grid->layers[MAPGRID_LAYER_GROUND];
  layer->offsetX = sSeaScroll[0];
  sSeaScroll[0] -= sSeaSpeed[0];
  if (sSeaScroll[0] < 0) {
    sSeaScroll[0] += TOLBI_SHIP_DECK_SEA_WRAP;
  }
  if (sSeaScroll[0] > TOLBI_SHIP_DECK_SEA_WRAP) {
    sSeaScroll[0] -= TOLBI_SHIP_DECK_SEA_WRAP;
  }
  layer->offsetY = sSeaScroll[1];
  sSeaScroll[1] -= sSeaSpeed[1];
  if (sSeaScroll[1] < 0) {
    sSeaScroll[1] += TOLBI_SHIP_DECK_SEA_WRAP;
  }
}

/// Under sail: shows the stroke oars (LOCALID_FIRST_STROKE_OAR) and the oars at
/// the sides (LOCALID_FIRST_OAR, the right-hand ones mirrored), records each
/// side oar's resting depth and chooses their strokes.
static void TolbiShipDeck_SetUpOars(void) {
  ObjectEvent_GetById(LOCALID_FIRST_STROKE_OAR)->collisionFlags = 0;
  ObjectEvent_GetById(LOCALID_FIRST_STROKE_OAR + 1)->collisionFlags = 0;
  ObjectEvent_GetById(LOCALID_FIRST_STROKE_OAR + 2)->collisionFlags = 0;
  ObjectEvent_GetById(LOCALID_FIRST_STROKE_OAR + 3)->collisionFlags = 0;
  TolbiShipDeck_ShowOar(LOCALID_FIRST_STROKE_OAR);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_STROKE_OAR + 1);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_STROKE_OAR + 2);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_STROKE_OAR + 3);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 3);
  sOarDepth[0] = ObjectEvent_GetById(LOCALID_FIRST_OAR)->z.fixed;
  sOarDepth[1] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 7);
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 4)->scaleX = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 5)->scaleX = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 6)->scaleX = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 7)->scaleX = -Q16_ONE;
  sOarDepth[4] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 7)->z.fixed;
  TolbiShipDeck_ChooseOarStrokes();
}

/// Shows an oar drawn under the deck, with no collision.
static void TolbiShipDeck_ShowOar(s32 localId) {
  struct ObjectEvent *oar = ObjectEvent_GetById(localId);

  if (oar != 0) {
    Engine_ObjectEventSetSpritePriority(localId, 3);
    Engine_ObjectEventSetSpriteFlags(oar, 0);
    oar->collisionFlags = 0;
    {
      s32 flags;
      flags = 2 | oar->priorityFlags;
      oar->priorityFlags = flags;
    }
  }
}

/// Gives each oar the stroke of its seat's rower; empty seats take stroke 3.
/// The first loop's stores are all overwritten below.
static void TolbiShipDeck_ChooseOarStrokes(void) {
  u32 *mode = sOarStroke;
  u32 i;

  for (i = 0; i <= 3; i++) {
    if (TolbiShipDeck_GetRowerAtSeat(i) != 0)
      *mode = TolbiShipDeck_GetSeatStroke(i);
    else
      *mode = 3;
    mode++;
  }
  if (TolbiShipDeck_GetRowerAtSeat(0) != 0)
    sOarStroke[0] = TolbiShipDeck_GetSeatStroke(0);
  else
    sOarStroke[0] = 3;
  if (TolbiShipDeck_GetRowerAtSeat(2) != 0)
    sOarStroke[1] = TolbiShipDeck_GetSeatStroke(2);
  else
    sOarStroke[1] = 3;
  sOarStroke[2] = 3;
  sOarStroke[3] = 3;
  if (TolbiShipDeck_GetRowerAtSeat(1) != 0)
    sOarStroke[4] = TolbiShipDeck_GetSeatStroke(1);
  else
    sOarStroke[4] = 3;
  if (TolbiShipDeck_GetRowerAtSeat(3) != 0)
    sOarStroke[5] = TolbiShipDeck_GetSeatStroke(3);
  else
    sOarStroke[5] = 3;
  sOarStroke[6] = 3;
  sOarStroke[7] = 3;
}

/// Returns the stroke (1 to 4) of the rower at a seat, or 0 if the seat is
/// empty.
static s32 TolbiShipDeck_GetSeatStroke(u32 seat) {
  s32 base = 0;
  u32 i;

  switch (seat) {
  case 0:
    base = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    base = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    base = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    base = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (i = 0; i < 9; i++) {
    if (Engine_GameFlagIsSet(base + i) != 0)
      return gTolbiShipDeckRowerStrokes[i];
  }
  return 0;
}

/// The task that rows: advances the four strokes and swings the eight oars at
/// the sides, each following its stroke's phase.
static void TolbiShipDeck_RowOars(void) {
  TolbiShipDeck_AdvanceStroke(LOCALID_FIRST_STROKE_OAR, 0);
  sStrokePhase[0] = 0;
  TolbiShipDeck_AdvanceStroke(LOCALID_FIRST_STROKE_OAR + 1, 1);
  sStrokePhase[1] += 0x80;
  TolbiShipDeck_AdvanceStroke(LOCALID_FIRST_STROKE_OAR + 2, 2);
  sStrokePhase[2] += 0x100;
  TolbiShipDeck_AdvanceStroke(LOCALID_FIRST_STROKE_OAR + 3, 3);
  sStrokePhase[3] += 0x200;
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR, 0, 0);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 1, 1, 0);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 2, 2, 0);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 3, 3, 0);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 4, 4, 1);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 5, 5, 1);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 6, 6, 1);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 7, 7, 1);
}

/// Advances one stroke's phase: slowly through the catch and the finish of the
/// stroke, quicker through the drive, quickest on the return; its stroke oar's
/// animation follows.
static void TolbiShipDeck_AdvanceStroke(s32 localId, s32 slot) {
  u16 value = sOarAngle[slot];

  if (value >= 0x6801 && value <= 0x6fff) {
    sStrokePhase[slot] += 0x70;
    Engine_ObjectEventSetAnimation(localId, 3);
  } else if (value >= 0xe801 && value <= 0xefff) {
    sStrokePhase[slot] += 0xe0;
    Engine_ObjectEventSetAnimation(localId, 3);
  } else if (value >= 0x7001 && value <= 0xefff) {
    sStrokePhase[slot] += 0x1c0;
    Engine_ObjectEventSetAnimation(localId, 2);
  } else {
    sStrokePhase[slot] += 0x300;
    Engine_ObjectEventSetAnimation(localId, 1);
  }
}

/// Swings one oar to its angle: unless flags bit 1 is set, the angle follows
/// the stroke oar whose stroke the oar takes; bit 0 marks a right-hand oar. The
/// oar's tilt (its sprite's rotation) and depth follow the angle.
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);
  struct AnimationObject *sprite = objectEvent->sprite;
  s32 sine;

  if ((flags & 2) == 0) {
    switch (sOarStroke[slot]) {
    case 1:
      sOarAngle[slot] = sStrokePhase[0];
      ObjectVisual_CopyAttributes(id, LOCALID_FIRST_STROKE_OAR);
      break;
    case 2:
      sOarAngle[slot] = sStrokePhase[1];
      ObjectVisual_CopyAttributes(id, LOCALID_FIRST_STROKE_OAR + 1);
      break;
    case 3:
      sOarAngle[slot] = sStrokePhase[2];
      ObjectVisual_CopyAttributes(id, LOCALID_FIRST_STROKE_OAR + 2);
      break;
    case 4:
      sOarAngle[slot] = sStrokePhase[3];
      ObjectVisual_CopyAttributes(id, LOCALID_FIRST_STROKE_OAR + 3);
      break;
    }
  }
  if (flags & 1) {
    sine = Engine_MathSin(sOarAngle[slot]);
    sprite->rotation = Engine_MathSin(sOarAngle[slot] + 0x8000) >> 5;
    objectEvent->z.fixed = sOarDepth[slot] - (sine << 2) - (sine << 1);
  } else {
    sine = Engine_MathSin(sOarAngle[slot] + 0x8000);
    sprite->rotation = Engine_MathSin(sOarAngle[slot]) >> 5;
    objectEvent->z.fixed = sOarDepth[slot] + (sine << 2) + (sine << 1);
  }
}

/// When not under sail: the oars lie at rest, all copying the first stroke oar,
/// which stands where the leader is while they copy it and then takes the first
/// oar's place.
static void TolbiShipDeck_RestOars(void) {
  struct ObjectEvent *objectEvent;
  u32 i;

  for (i = 0; i <= 7; i++)
    sOarAngle[i] = 0xc000;
  TolbiShipDeck_ShowOar(LOCALID_FIRST_STROKE_OAR);
  Engine_ObjectEventSetPosition(LOCALID_FIRST_STROKE_OAR + 1, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_FIRST_STROKE_OAR + 2, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_FIRST_STROKE_OAR + 3, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_FIRST_OAR, 0, 0);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 3);
  sOarStroke[0] = 0;
  sOarStroke[1] = 0;
  sOarStroke[2] = 0;
  sOarStroke[3] = 0;
  sOarDepth[0] = ObjectEvent_GetById(LOCALID_FIRST_STROKE_OAR)->z.fixed;
  sOarDepth[1] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(LOCALID_FIRST_OAR + 7);
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 4)->scaleX = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 5)->scaleX = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 6)->scaleX = -Q16_ONE;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 7)->scaleX = -Q16_ONE;
  sOarStroke[4] = 0;
  sOarStroke[5] = 0;
  sOarStroke[6] = 0;
  sOarStroke[7] = 0;
  sOarDepth[4] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = ObjectEvent_GetById(LOCALID_FIRST_OAR + 7)->z.fixed;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL)
    Engine_ObjectEventSetPosition(LOCALID_FIRST_STROKE_OAR,
                                  objectEvent->x.fixed, objectEvent->z.fixed);
  Engine_TaskWait(1);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 1, LOCALID_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 2, LOCALID_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 3, LOCALID_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 4, LOCALID_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 5, LOCALID_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 6, LOCALID_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(LOCALID_FIRST_OAR + 7, LOCALID_FIRST_STROKE_OAR);
  ObjectEvent_GetById(LOCALID_FIRST_STROKE_OAR)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 1)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 2)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 3)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 4)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 5)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 6)->retainSpriteCache = 1;
  ObjectEvent_GetById(LOCALID_FIRST_OAR + 7)->retainSpriteCache = 1;
  Engine_TaskWait(1);
  Engine_ObjectEventSetPosition(LOCALID_FIRST_STROKE_OAR, PIXELS(132),
                                PIXELS(632));
  Engine_TaskWait(1);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_STROKE_OAR, 0, 2);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 1, 1, 2);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 2, 2, 2);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 3, 3, 2);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 4, 4, 3);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 5, 5, 3);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 6, 6, 3);
  TolbiShipDeck_SwingOar(LOCALID_FIRST_OAR + 7, 7, 3);
}

/// Redraws a block of the deck's cells and attributes, as it is whenever the
/// ship has left the Kalay dock.
static void TolbiShipDeck_CopyUnderwayCells(void) {
  MapGrid_CopyCellsTo(78, 39, 78, 40, 5, 1);
  MapGrid_CopyCellsTo(78, 39, 78, 41, 5, 1);
  MapGrid_CopyCellsTo(78, 39, 79, 42, 4, 1);
  MapGrid_CopyCellsTo(78, 39, 82, 43, 1, 1);
  {

    MapGrid_CopyCellAttributes(17, 38, 5, 2, 17, 40);
  }
}

/// Redraws two more blocks of cells, as they are at anchor.
static void TolbiShipDeck_CopyAnchoredCells(void) {
  MapGrid_CopyCellsTo(66, 61, 64, 40, 5, 4);
  MapGrid_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

/// Shows the current message from a speaker and waits 10 frames.
static void TolbiShipDeck_Say(s32 speaker) {
  Engine_ScriptShowMessage(speaker, 0);
  Engine_ScriptWait(10);
}

/// Turns an object event to a facing over 10 frames.
static void TolbiShipDeck_Turn(s32 localId, s32 facing) {
  Engine_ObjectEventFaceDirection(localId, facing, 10);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, after the lookout's call:
/// Ouranos thinks they have reached Tolbi, but Kaja sees the coast is not
/// Tolbi's and the ship is off course, and the crew head for an island to get
/// their bearings before Kaja goes below.
/// TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND leads on below decks, toward
/// the island.
static void TolbiShipDeck_SightIsland(void) {
  Engine_ScriptStart();
  sSeaScroll[0] = 0x40000;
  sSeaSpeed[0] = -0x8000;
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)gTolbiShipDeck_CrewObjectEventTemplates);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(SEA_OURANOS, PIXELS(182), PIXELS(618));
  ObjectEvent_Get(SEA_OURANOS)->facing = FACING_NORTH;
  ObjectEvent_SetPosition(SEA_KAJA, PIXELS(218), PIXELS(516));
  ObjectEvent_Get(SEA_KAJA)->facing = FACING_NORTHWEST + FACING_STEP;
  ObjectEvent_SetPosition(SEA_SEAN, PIXELS(204), PIXELS(526));
  ObjectEvent_Get(SEA_SEAN)->facing = FACING_NORTHWEST + FACING_STEP;
  ObjectEvent_SetPosition(SEA_CREW_MEMBER, 0, 0);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 182, 0x214);
  ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ScriptSetMessage(MsgTolbiShipFinallyReachedTolbi);
  TolbiShipDeck_Say(SEA_OURANOS);
  Engine_ObjectEventRunRepeatedMotion(SEA_KAJA, 2);
  Engine_ObjectEventSetAnimation(SEA_KAJA, 4);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 6));
  ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventSetAnimation(SEA_KAJA, 3);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 6));
  ObjectEvent_SetAttachedEffect(SEA_OURANOS, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(SEA_SEAN, ATTACHED_EFFECT_IN_FRONT |
                                              ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(80);
  ObjectEvent_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 194, 0x1f4);
  ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_SetSpeed(SEA_SEAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(SEA_SEAN, 192, 0x206);
  ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_SetSpeed(SEA_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 210, 0x1fc);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_NORTHWEST + FACING_STEP);
  Engine_ObjectEventRunRepeatedMotion(SEA_OURANOS, 1);
  TolbiShipDeck_Say(SPEAKER(SEA_OURANOS, 5));
  Engine_ObjectEventSetAnimationAndWait(SEA_KAJA, 3);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_NORTH + FACING_STEP);
  TolbiShipDeck_Say(SPEAKER(SEA_SEAN, 9));
  ObjectEvent_FaceDirection(SEA_KAJA, FACING_WEST, 20);
  Engine_ObjectEventSetAnimation(SEA_KAJA, 4);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 10));
  ObjectEvent_WalkToAndWait(SEA_KAJA, 204, 0x218);
  ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 182, 0x224);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 182, 0x250);
  ObjectEvent_WalkTo(SEA_KAJA, 182, 0x298);
  Engine_ScriptWait(40);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND);
}

/// At anchor off the island, talking to Sean or Ouranos: the party is asked to
/// scout the island for the arguing tour group; Garet and Ivan may argue
/// before the party agrees (FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED).
static void TolbiShipDeck_OfferToScoutIsland(void) {
  struct ObjectEvent *leader;
  u8 bits;

  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 180, 0x28e);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkTo(GARET, 194, 0x280);
  ObjectEvent_WalkTo(IVAN, 198, 0x28e);
  ObjectEvent_WalkToAndWait(MIA, 194, 0x2a0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_STAND);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 0);
  TolbiShipDeck_Turn(MIA, FACING_WEST);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_EAST);
  Engine_ScriptSetMessage(MsgTolbiShipIsaacTalkedPassengersDidntTour);
  TolbiShipDeck_Say(SEA_SEAN);
  TolbiShipDeck_Turn(SEA_OURANOS, FACING_NORTH + FACING_STEP);
  Script_ShowMessageAndWait(SEA_OURANOS, 0, 40);
  ObjectEvent_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Engine_ObjectEventRunRepeatedMotion(SEA_SEAN, 1);
  Script_OpenMessage(SEA_SEAN, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    TolbiShipDeck_Say(IVAN);
    TolbiShipDeck_Turn(MIA, FACING_NORTHWEST);
    Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
    TolbiShipDeck_Say(MIA);
    TolbiShipDeck_Turn(GARET, FACING_SOUTHWEST);
    Engine_ObjectEventRunRepeatedMotion(GARET, 1);
    Script_OpenMessage(GARET, 0);
    for (;;) {
      if (Engine_ScriptChooseYesNo(0, 0) == 1) {
        Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
        Engine_ScriptSetMessage(MsgTolbiShipIsaacDontTalkLikeShouldnt);
        Script_OpenMessage(IVAN, 0);
      } else
        break;
    }
  }
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(SEA_SEAN, 3);
  Engine_ScriptSetMessage(MsgTolbiShipSeeYoureGoingGoFor);
  TolbiShipDeck_Say(SEA_SEAN);
  ObjectEvent_SetSpeed(SEA_SEAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_GetById(SEA_SEAN)->steeringFlags &= 254;
  ObjectEvent_WalkToAndWait(SEA_SEAN, 162, 0x27a);
  Engine_ScriptWait(1);
  bits = OBJ_EVENT_FACES_MOTION;
  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(SEA_SEAN);
    u8 value = objectEvent->steeringFlags;

    objectEvent->steeringFlags = value | bits;
  }
  ObjectEvent_GetById(SEA_OURANOS)->steeringFlags &= 254;
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 162, 0x2a4);
  Engine_ScriptWait(1);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(SEA_OURANOS);

    bits |= objectEvent->steeringFlags;
    objectEvent->steeringFlags = bits;
  }
  ObjectEvent_FaceDirection(SEA_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  TolbiShipDeck_Turn(SEA_OURANOS, FACING_NORTH + FACING_STEP);
  TolbiShipDeck_Say(SEA_SEAN);
  ObjectEvent_WalkTo(GARET, 180, 0x28e);
  ObjectEvent_WalkTo(IVAN, 180, 0x28e);
  ObjectEvent_WalkToAndWait(MIA, 180, 0x28e);
  Engine_ObjectEventDestroy(GARET);
  Engine_ObjectEventDestroy(IVAN);
  Engine_ObjectEventDestroy(MIA);
  GameFlag_Set(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED);
  Engine_ScriptEnd();
}

/// Back on deck from the island: on yes, the crew make ready to sail and the
/// scene moves on. "How was it, Isaac? Did you explore the island
/// thoroughly?"
static void TolbiShipDeck_ReturnFromIsland(void) {
  Engine_ScriptStart();
  FieldScript_CallWithLastActiveObjectId(
      (struct ObjectEventTemplate *)
          gTolbiShipDeck_TourGuideObjectEventTemplates);
  Engine_TaskWait(1);
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 148, 0x290);
  ObjectEvent_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventRunRepeatedMotion(SEA_SEAN, 1);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_SOUTH + FACING_STEP);
  Engine_ScriptSetMessage(MsgTolbiShipHowWasIsaacDidExplore);
  Script_OpenMessage(SPEAKER(SEA_SEAN, 2), 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    TolbiShipDeck_Say(SPEAKER(SEA_SEAN, 2));
    Engine_ScriptEnd();
    return;
  }
  gFieldWork->message++;
  Script_AskYesNo(SPEAKER(SEA_SEAN, 2), 0);
  TolbiShipDeck_OpenFirstDoor();
  ObjectEvent_SetPosition(ANCHOR_TOUR_GUIDE, PIXELS(216), PIXELS(588));
  ObjectEvent_SetSpeed(ANCHOR_TOUR_GUIDE, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(ANCHOR_TOUR_GUIDE, 216, 0x254);
  ObjectEvent_WalkToAndWait(ANCHOR_TOUR_GUIDE, 188, 0x268);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(SEA_OURANOS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
  TolbiShipDeck_Turn(ANCHOR_TOUR_GUIDE, FACING_SOUTH + FACING_STEP);
  Engine_ObjectEventJump(ANCHOR_TOUR_GUIDE, 2, 0);
  Engine_ObjectEventSetAnimation(ANCHOR_TOUR_GUIDE, 4);
  Script_ShowMessage(ANCHOR_TOUR_GUIDE, 0);
  ObjectEvent_SetPosition(SEA_KAJA, PIXELS(180), PIXELS(777));
  ObjectEvent_SetSpeed(SEA_KAJA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 180, 0x298);
  ObjectEvent_FaceDirection(SEA_KAJA, FACING_NORTH + FACING_STEP, 0);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 2));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(SEA_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_ShowEmote(ANCHOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventRunRepeatedMotion(SEA_KAJA, 1);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 2));
  Engine_ObjectEventStartRepeatedMotion(SEA_OURANOS, 2);
  TolbiShipDeck_Say(SEA_OURANOS);
  ObjectEvent_FaceDirection(SEA_KAJA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(SEA_KAJA, 3);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 6));
  Engine_ObjectEventJump(ANCHOR_TOUR_GUIDE, 2, 20);
  Engine_ObjectEventSetAnimation(ANCHOR_TOUR_GUIDE, 4);
  TolbiShipDeck_Say(ANCHOR_TOUR_GUIDE);
  ObjectEvent_WalkToAndWait(SEA_KAJA, 182, 0x280);
  ObjectEvent_FaceDirection(SEA_KAJA, FACING_NORTH + FACING_STEP, 0);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 8));
  ObjectEvent_ShowEmote(ANCHOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        20);
  Engine_ObjectEventStartRepeatedMotion(ANCHOR_TOUR_GUIDE, 2);
  TolbiShipDeck_Say(ANCHOR_TOUR_GUIDE);
  Engine_ObjectEventSetAnimationAndWait(SEA_KAJA, 3);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_EAST);
  Engine_ObjectEventRunRepeatedMotion(SEA_SEAN, 1);
  TolbiShipDeck_Say(SEA_SEAN);
  ObjectEvent_SetSpeed(SEA_SEAN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetActionScript(SEA_SEAN, gTolbiShipDeck_GoBelowActions);
  ObjectEvent_SetSpeed(SEA_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(SEA_OURANOS, 168, 0x278);
  ObjectEvent_SetActionScript(SEA_OURANOS, gTolbiShipDeck_GoBelowActions);
  Engine_ScriptWait(80);
  ObjectEvent_SetActionScript(ANCHOR_TOUR_GUIDE, gTolbiShipDeck_GoBelowActions);
  Engine_ScriptWait(40);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_WEST);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 2));
  TolbiShipDeck_Turn(LOCALID_PARTY_LEADER, FACING_NORTHEAST);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(SEA_KAJA, 3);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(
      TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SWITCH_TEAMS);
}

/// Places the leader on the deck facing south, with the camera on them.
static void TolbiShipDeck_PlaceLeaderOnDeck(void) {
  struct ObjectEvent *record;

  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(676), 0);
  Engine_MapGridRedraw();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(232), PIXELS(676));
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  record->facing = FACING_SOUTH;
  Engine_TaskWait(1);
}

/// At Kalay, as the crew's talk of the delay ends: Kaja walks off and is
/// hidden.
const s32 gTolbiShipDeck_KajaLeavesActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(202), 0, PIXELS(600)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// At the same moment the captain steps aside and turns west.
const s32 gTolbiShipDeck_CaptainStepsAsideActions[] = {
    SC_WAIT(20), SC_ANIMATION(2), SC_MOVE(PIXELS(162), 0, PIXELS(666)),
    SC_SYNC,     SC_ANIMATION(1), SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

/// The crew member's and the tour guide's impatience at Kalay: they squash
/// and stretch on the spot, the wait between in their timer.
const s32 gTolbiShipDeck_SquashAndStretchActions[] = {
    SC_CALL(Map_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD: a first-attack
/// monster runs in, leaps over the rail onto the deck and goes below by the
/// first door.
const s32 gTolbiShipDeck_MonsterLeapsAboardActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(140), 0, PIXELS(586)),
    SC_SYNC,
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_MOVE(PIXELS(184), 0, PIXELS(598)),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_CALL(TolbiShipDeck_ShowSprite),
    SC_MOVE(PIXELS(216), 0, PIXELS(596)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// Sean on guard at sea, pacing between two spots on the deck and looking
/// out at each.
const s32 gTolbiShipDeck_SeanPacesActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(228), 0, PIXELS(521)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(40),
    SC_ANIMATION(4),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(204), 0, PIXELS(521)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(40),
    SC_ANIMATION(4),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Ouranos on guard at sea, walking the four corners of a square on the deck
/// and looking out at each.
const s32 gTolbiShipDeck_OuranosPatrolsActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(672)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(50),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(672)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(70),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(636)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(30),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(636)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The second attack's hovering monsters (SEA_FIRST_HOVERING_MONSTER): their
/// scratch fields cleared, then TolbiShipDeck_HoverAndDiveAtOuranos or
/// TolbiShipDeck_HoverAndDiveAtSean each frame.
const s32 gTolbiShipDeck_HoverAndDiveAtOuranosActions[] = {
    SC_SET(SW_MOTION, 0), SC_SET(SW_TIMER, 0),
    SC_SET(0x24, 0),      SC_SET(0x27, 0),
    SC_SET(0x17, 0),      SC_CALL(TolbiShipDeck_HoverAndDiveAtOuranos),
};

const s32 gTolbiShipDeck_HoverAndDiveAtSeanActions[] = {
    SC_SET(SW_MOTION, 0), SC_SET(SW_TIMER, 0),
    SC_SET(0x24, 0),      SC_SET(0x27, 0),
    SC_SET(0x17, 0),      SC_CALL(TolbiShipDeck_HoverAndDiveAtSean),
};

/// The second attack's boarding monsters, bobbing in the air
/// (TolbiShipDeck_Bob).
const s32 gTolbiShipDeck_BobActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(TolbiShipDeck_Bob),
};

const s32 gTolbiShipDeck_LookAroundActions[] = {
    SC_CALL(TolbiShipDeck_LookAround),
};

/// SEA_SPLASH: it swells from 7/10 of its size for 24 frames, then
/// is hidden.
const s32 gTolbiShipDeck_SwellActions[] = {
    SC_ANIMATION(1),
    SC_SET(SW_SCALE_X, 0xb333),
    SC_SET(SW_SCALE_Y, 0xb333),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 0x147a),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// A monster on deck goes below by the first door and is hidden.
const s32 gTolbiShipDeck_MonsterGoesBelowActions[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// The third attack's leaping monsters (SEA_FIRST_LEAPING_MONSTER): their
/// timer cleared, then TolbiShipDeck_LeapAtOuranos or TolbiShipDeck_LeapAtSean
/// each frame.
const s32 gTolbiShipDeck_LeapAtOuranosActions[] = {
    SC_SET(SW_TIMER, 0),
    SC_CALL(TolbiShipDeck_LeapAtOuranos),
};

const s32 gTolbiShipDeck_LeapAtSeanActions[] = {
    SC_SET(SW_TIMER, 0),
    SC_CALL(TolbiShipDeck_LeapAtSean),
};

/// The third attack's boarding monsters, hopping at random.
const s32 gTolbiShipDeck_HopActions[] = {
    SC_CALL(TolbiShipDeck_RandomlyStartObjectEventHop),
};

/// At Tolbi, a passenger who rowed walks away once they have spoken, and is
/// hidden.
const s32 gTolbiShipDeck_RowerLeavesActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(82), 0, PIXELS(656)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// Back from the island, Sean, Ouranos and the tour guide go below by the
/// first door, one after another.
const s32 gTolbiShipDeck_GoBelowActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(186), 0, PIXELS(604)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(604)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Sean standing by, turning now and then (TolbiShipDeck_IdleTurn).
const s32 gTolbiShipDeck_SeanLooksAboutActions[] = {
    SC_SET(0x27, 0),
    SC_CALL(TolbiShipDeck_IdleTurn),
};

const struct MapWarpDest gTolbiShipDeckWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 179, 0, 653, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, CONDITION_ALWAYS,
     216, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS,
     232, 0, 664, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM, CONDITION_ALWAYS,
     216, 0, 808, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST, CONDITION_ALWAYS, 216, 4, 648,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_KALAY_DOCKS, CONDITION_ALWAYS, 312, -4, 656,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 120, -4, 656,
     FACING_EAST, 0, 65, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY, CONDITION_ALWAYS, 232, 0,
     664, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS, CONDITION_ALWAYS,
     212, 0, 521, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD, CONDITION_ALWAYS,
     192, 0, 650, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK, CONDITION_ALWAYS, 232,
     0, 654, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK, CONDITION_ALWAYS, 216,
     0, 657, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN, CONDITION_ALWAYS, 216, 0,
     640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE, CONDITION_ALWAYS, 216, 0,
     540, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED, CONDITION_ALWAYS, 216,
     0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_WORLD_MAP, CONDITION_ALWAYS, 196, 0, 652,
     FACING_SOUTH, 0, 65, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, CONDITION_ALWAYS, 212, 0,
     521, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

// clang-format off
const struct MapWarpRegion gTolbiShipDeckRegions[] = {
    {208, 24, 634, 224, 40, 650, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST},
    {MAP_TABLE_END},
};

const u32 gTolbiShipDeckWarps[] = {
    MAP_WARPS(TolbiShipDeck),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_KALAY_DOCKS, KalayDocks, KALAY_DOCKS_FROM_TOLBI_SHIP_DECK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST, TolbiShipMast, TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_DOCKS, TolbiDocks, TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE, WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR, TolbiShipMast, TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_TURN, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_FIRST_ATTACK, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SECOND_ATTACK, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_THIRD_ATTACK, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK),
    MAP_WARP(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI, WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND),
    MAP_WARP(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SWITCH_TEAMS, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS),
    MAP_WARPS_END,
};

const struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplates[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(266), 0, PIXELS(703), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

// UB: the matching compiler places this writable overlay table with the
// constants. The modern build gives its talkFacing stores a mutable object.
#ifdef UBFIX
struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplatesWaiting[] = {
#else
const struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplatesWaiting[] = {
#endif
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(183), 0, PIXELS(720), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(712), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(198), 0, PIXELS(706), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(172), 0, PIXELS(706), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(230), 0, PIXELS(602), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(236), 0, PIXELS(621), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(182), 0, PIXELS(556), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplatesRowing[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplatesAtSea[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeck_CrewObjectEventTemplates[] = {
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(512), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(634), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(212), 0, PIXELS(521), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeck_TourGuideObjectEventTemplates[] = {
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeck_MonstersBoardObjectEventTemplates[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeck_SecondAttackObjectEventTemplates[] = {
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeck_ThirdAttackObjectEventTemplates[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeck_KrakenObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(178), 0, PIXELS(642), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(182), 0, PIXELS(534), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_KRAKEN, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(540), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(640), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplatesArrived[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(188), 0, PIXELS(512), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(162), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(162), 0, PIXELS(676), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
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
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

const struct MapEvent gTolbiShipDeckEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { COORD_EVENT_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_KALAY_DOCKS },
    { MAP_EVENT_TALK, KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckThereAreALotOfPassengers) },
    { MAP_EVENT_TALK, KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckOhAtTheRiskOfSounding) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckTheReplacementShipFromTolbiShould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIHidTheAnchorCharmNo) },
    { MAP_EVENT_TALK, KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeApologizeForAnyTroubleThat) },
    { MAP_EVENT_TALK, KALAY_CREW_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_TalkToCrewMember) },
    { MAP_EVENT_TALK, KALAY_TOUR_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_TalkToTourGuide) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderWhatsGoingOnWeve) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseWarriorsGotTiredOfWaiting) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_WAITING_LONG, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIfWeJustPromiseToHelp) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderIfImTheOnly) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckIWonderIfThoseTwoAre) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_SEEING_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIllAgreeToAnythingEvenRowing) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolIsLikeAHumongous) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHaHahThoseMercenariesAreGoing) },
    { MAP_EVENT_TALK, KALAY_PASSENGER_KARAGOL, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThatSeaDogKajaIsA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThereIsNothingToFearAs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_CREW_MEMBER, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckWhenAreWeGoingWhenAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_CREW_MEMBER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckUsingForceToLaunchTheShip) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAreYouSeriousAboutSettingSail) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_TOUR_GUIDE, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckHmmmThisIsSoFrustratingWhy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_TOUR_GUIDE, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckTheyWereJustAboutToLaunch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItProbablyWontMakeAnyDifference) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTravelingByShipIsFasterThan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseThugsAreTooImpatientTrying) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_WAITING_LONG, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUltimatelyTheyreJustBeingThreatenedInto) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckMostOfThePeopleGoingTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHeeHeeIfWeCanGet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_SEEING_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckBesidesRowingIsAMansJob) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolSeaIsPrettyFoggy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseGuysSeemPrettySureOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), KALAY_PASSENGER_KARAGOL, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUsHelpWithTheRowingGet) },
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_CELL_BY_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(TolbiShipDeck_WatchCrewDecideToSail) },
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_CELL_EAST_OF_DOOR, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT_SCRIPT(TolbiShipDeck_CatchCrewmanFromEast) },
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_CELL_WEST_OF_DOOR, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT_SCRIPT(TolbiShipDeck_CatchCrewmanFromWest) },
    { MAP_EVENTS_END },
};

const struct MapEvent gTolbiShipDeckEventsAtSea[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { COORD_EVENT_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { MAP_EVENT_TALK, SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering) },
    { MAP_EVENT_TALK, SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItLooksLikeAnotherOarsmanWas) },
    { MAP_EVENT_TALK, SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIFeelSorryForTheOarsmen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAnOarsmanWasInjuredByA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeHaveNoIdeaWhenThose) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUhnnnWhenILookAtThe) },
    { MAP_EVENTS_END },
};

const struct MapEvent gTolbiShipDeckEventsArrived[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { COORD_EVENT_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_DOCKS },
    { MAP_EVENT_TALK, ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouLookinForSeanAndOuranos) },
    { MAP_EVENT_TALK, ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_AskHeadedForColosso) },
    { MAP_EVENT_TALK, ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisGroupThatCameOnOur) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantSeanAndOuranosTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckImJustGoingToHangOut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWonderWhatHappenedToThat) },
    { MAP_EVENTS_END },
};

const struct MapEvent gTolbiShipDeckEventsAtIsland[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { COORD_EVENT_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { MAP_EVENT_COORD, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CRATE_HOLD, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE },
    { MAP_EVENT_TALK, SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckKajaAndTheCaptainAreProfessionals) },
    { MAP_EVENT_TALK, SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT_SCRIPT(TolbiShipDeck_OfferToScoutIsland) },
    { MAP_EVENT_TALK, SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckTheresNoTellingWhatKindOf) },
    { MAP_EVENT_TALK, SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT_SCRIPT(TolbiShipDeck_OfferToScoutIsland) },
    { MAP_EVENT_TALK, SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeveDoneAGoodJobOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantToBeASailor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckIGiveUpWhatAreWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisIslandGivesMeTheCreeps) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckThatsEnoughBickering) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWhatWeCanDoNowIs) },
    { MAP_EVENTS_END },
};
// clang-format on

/// How hard each passenger rows, from ARRIVED_FIRST_PASSENGER on: 1 leaves the
/// oar still, 2 to 4 row ever faster.
const s32 gTolbiShipDeckRowerStrokes[] = {3, 2, 3, 1, 4, 4, 2, 1, 1};
