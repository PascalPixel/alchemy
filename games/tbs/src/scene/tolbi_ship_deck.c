/// @file
/// Tolbi-bound Ship (SceneId_TolbiShipDeck), its deck: the ship that carries
/// the party across the Karagol Sea from Kalay to Tolbi, with Silk Road blocked
/// and a tour group of passengers bound for Colosso. Its other two scenes are
/// the mast (tolbi_ship_mast.c, SceneId_TolbiShipMast) and below decks
/// (tolbi_ship_interiors.c, SceneId_TolbiShipInteriors). The deck's placements
/// and events follow the voyage's stage, read from its flags: at Kalay, at sea
/// once the first attack is fought off
/// (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF), while the oars move
/// (FLAG_TOLBI_SHIP_ROWING), at anchor off the island
/// (FLAG_TOLBI_SHIP_AT_ISLAND) and at Tolbi (FLAG_TOLBI_SHIP_REACHED_TOLBI).
/// The setup draws the deck for that stage (the cells as under way, at anchor),
/// rocks it and scrolls the sea while under sail, sets the oars rowing, and
/// plays the cutscene its entrance names. The entrances from
/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY on are the voyage's
/// cutscenes on deck; each leaves by an exit into the next cutscene in another
/// of the ship's scenes or on the world map (SceneId_WorldMap, whose entrances
/// for the ship's legs sail it on), and the chain comes back here. In story
/// order:
///
/// - Boarding at Kalay: Kalay Docks (SceneId_KalayDocks) lead aboard at
///   TOLBI_SHIP_DECK_FROM_KALAY_DOCKS, and TOLBI_SHIP_DECK_TO_KALAY_DOCKS leads
///   back. Walking into one of the ways below deck (the two doors and the
///   hatch) leads below by
///   TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD,
///   TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CARGO_HOLD and
///   TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN, and the ladder up
///   the mast by TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST. The ship is waiting for
///   its replacement from Tolbi. Once the tour group has left Kalay
///   (FLAG_KALAY_COLOSSO_TOUR_LEFT) the passengers on deck grow impatient, and
///   walking up to the crew (touch cell 10) plays the captain, Kaja, the deck's
///   fighter, and a crewman talking over the delay: the captain will sail
///   trusting his anchor charm, the crewman calls it superstition and slips
///   below ("Not if I have anything to say about it."),
///   FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY. Coming up to the door he took
///   (touch cells 11 and 12) brings him out denying everything
///   (FLAG_TOLBI_SHIP_CREWMAN_CAUGHT); Mind Read shows he hid the charm on
///   top of the mast. Below decks the captain then refuses to sail without it,
///   and once it is given back (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) the
///   passengers who promised to row stand on deck.
/// - Setting sail without the replacement, the party having agreed below to
///   guard the oarsmen: TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY has
///   Kaja bring the word that all is ready ("Preparations... Ready!") and leads
///   to the lookout on the mast; later departures, with the party aboard, play
///   the same with Kaja reporting.
/// - The monster attacks: sea monsters are sighted
///   (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS, "Aye, Captain!
///   Sea monsters!"), they leap aboard
///   (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD), they harry
///   Ouranos and Sean, the warriors bound for Colosso who guard the deck with
///   Kaja (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED), and a great
///   shape rises from the sea and a monster drops onto the deck
///   (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_MONSTER_LANDS). The sighting leads
///   to the cabin, where the captain turns the ship
///   (TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN_FROM_TOLBI_SHIP_DECK_TURN); the other
///   three each lead below to the fights among the oars
///   (TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK and
///   the second and third attacks'). At
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN the party jumps down to
///   the deck and fights the Kraken (the voyage's encounter zone), returning to
///   TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE, where Kaja thanks Isaac
///   (FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) and another oarsman is lost.
/// - Off course: the lookout's call of land on the mast leads either to
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED, where land is sighted
///   and the ship comes in to Tolbi, or, by how the passengers chosen to row
///   pull (tolbi_ship_mast.c), to
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, where the coast proves
///   not to be Tolbi's and the crew head for an island to get their bearings.
///   At anchor off it (the island is Crossbone Isle, SceneId_CrossboneIsle,
///   reached across the world map by
///   TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE) Sean asks the party to scout
///   it for the tour group (FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED); back aboard
///   at TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS, the crew have found the course to
///   Tolbi, the rowers below switch sides, and the ship sails on, to
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED this time.
/// - At Tolbi: TOLBI_SHIP_DECK_FROM_WORLD_MAP, where the passengers who rowed
///   come up one by one and the leader is thanked;
///   TOLBI_SHIP_DECK_TO_TOLBI_DOCKS leads ashore to Tolbi Docks
///   (SceneId_TolbiDocks).

#include "script.h"
#include "flags.h"
#include "songs.h"
#include "version.h"
#include "game_state.h"
#include "unit/scene/tolbi_ship_deck.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "field_service.h"
#include "global_cells.h"
#include "edition.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "map_scroll.h"
#include "heap_state.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

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
static void TolbiShipDeck_ShowOar(s32 a);
static void TolbiShipDeck_ChooseOarStrokes(void);
static s32 TolbiShipDeck_GetSeatStroke(u32 seat);
static void TolbiShipDeck_RowOars(void);
static void TolbiShipDeck_AdvanceStroke(s32 actor, s32 slot);
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags);
static void TolbiShipDeck_RestOars(void);
static void TolbiShipDeck_CopyUnderwayCells(void);
static void TolbiShipDeck_CopyAnchoredCells(void);
static void TolbiShipDeck_Say(s32 a);
static void TolbiShipDeck_Turn(s32 a, s32 b);
static void TolbiShipDeck_SightIsland(void);
static void TolbiShipDeck_ReturnFromIsland(void);
static void TolbiShipDeck_PlaceLeaderOnDeck(void);

// The append service registers these borrowed crew tables for later talk
// lookups, but does not write them. Its mutable API therefore needs a cast
// at each call; the tables themselves remain const.

/// A speaker: the actor, and in the top four bits where the message window
/// opens beside them (4, SPEAKER_WINDOW_ABOVE, is above).
#define SPEAKER(actor, window) ((window) << 12 | (actor))

/// Who is who on deck, numbering each placement table's rows from
/// ACTOR_FIRST_PLACED. Every table starts with the same twelve rows of oars,
/// and the cutscenes add their actors after a table's last row
/// (Event_CallWithLastActiveObjectId), so each stage of the voyage has its own
/// names, which carry its prefix. At Kalay (gTolbiShipDeckPlacementsWaiting)
/// the captain talks the delay over with Kaja, the deck's fighter, and a
/// crewman, who asks Kaja how the oarsmen are ("Say, Kaja, how are the oarsmen
/// doing?"). At sea gTolbiShipDeck_CrewScript places Kaja and the warriors
/// Ouranos and Sean, who help him guard the deck ("That'll be me, Ouranos, and
/// Sean on deck").
enum TolbiShipDeckActor {
  /// Four oars, with the same sprite as the others, that keep the strokes of
  /// gTolbiShipDeckRowerStrokes, one each from 1 (the oar left still) to 4
  /// (fastest), drawn under the deck. Each oar at the sides copies the frames
  /// of its seat's stroke (TolbiShipDeck_SwingOar); at rest the first stands in
  /// for the first oar.
  ACTOR_FIRST_STROKE_OAR = ACTOR_FIRST_PLACED,
  /// The eight oars at the ship's sides: four at x 132, then four at x 300,
  /// which are drawn mirrored.
  ACTOR_FIRST_OAR = ACTOR_FIRST_STROKE_OAR + 4,

  // At Kalay: gTolbiShipDeckPlacements, then gTolbiShipDeckPlacementsWaiting.
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who
  /// hid the captain's anchor charm on the mast and denies
  /// everything. "I didn't do anything..."
  KALAY_CREWMAN = ACTOR_FIRST_OAR + 8,
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

  // At sea: gTolbiShipDeck_CrewScript after the oars, then each cutscene's
  // own rows.
  SEA_KAJA = ACTOR_FIRST_OAR + 8,
  SEA_OURANOS,
  SEA_SEAN,
  /// With KALAY_CREW_MEMBER's sprite: "I have a job to do. I can't row!"; at
  /// anchor, Mind Read hears "I want to be a sailor like Kaja". On later
  /// departures he calls "Preparations... Ready!"
  SEA_CREW_MEMBER,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD
  /// (gTolbiShipDeck_CrewScriptB): the first attack's three monsters, all of
  /// one sprite, which leap aboard and go below by the first door, where
  /// gTolbiShipInteriors_FirstAttackMonsters places them.
  SEA_FIRST_ATTACK_MONSTER,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED
  /// (gTolbiShipDeck_CrewScriptC): four monsters of one sprite hover by the
  /// deck, the first two diving at Sean and the others at Ouranos.
  SEA_FIRST_HOVERING_MONSTER = SEA_CREW_MEMBER + 1,
  /// Then the second attack's three, two of the hovering monsters' sprite and
  /// one other, as gTolbiShipInteriors_SecondAttackMonsters places them, come
  /// aboard and go below.
  SEA_SECOND_ATTACK_MONSTER = SEA_FIRST_HOVERING_MONSTER + 4,
  /// A sprite that swells for 24 frames at the rail where the last of them
  /// leaps aboard. XXX: what the player sees in it is unknown; a render of
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED would settle it.
  SEA_SWELLING_SPRITE = SEA_SECOND_ATTACK_MONSTER + 3,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_MONSTER_LANDS
  /// (gTolbiShipDeck_CrewScriptD): four monsters with the first attack's
  /// sprite, that leap at Sean, the first two, and at Ouranos.
  SEA_FIRST_LEAPING_MONSTER = SEA_CREW_MEMBER + 1,
  /// Then the third attack's three, two with the first attack's sprite and one
  /// other, as gTolbiShipInteriors_ThirdAttackMonsters places them, which go
  /// below; the last drops onto the deck.
  SEA_THIRD_ATTACK_MONSTER = SEA_FIRST_LEAPING_MONSTER + 4,
  /// A sprite that grows from a tenth of its size where the last of them drops.
  /// XXX: likely its shadow, because it grows before the monster falls onto it.
  SEA_LANDING_SPOT = SEA_THIRD_ATTACK_MONSTER + 3,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN and at anchor
  /// (gTolbiShipDeck_CrewScriptE, after Garet, Ivan and Mia in their own
  /// slots): the actor the party charges. XXX: likely the Kraken, because the
  /// battle that follows is the Kraken's and at anchor the same rows place it
  /// only for it to be destroyed at once.
  SEA_KRAKEN = SEA_CREW_MEMBER + 1,
  /// An invisible actor the camera follows across the deck.
  SEA_CAMERA_TARGET,
  /// At anchor, back from the island (gTolbiShipDeck_ClosingCrewScript): the
  /// tour guide comes up from below. "The passengers are going stir crazy!"
  /// Kaja: "Tell our tourists they can enjoy a nice, long break after we
  /// arrive."
  ANCHOR_TOUR_GUIDE,

  // At Tolbi: gTolbiShipDeckPlacementsArrived.
  ARRIVED_KAJA = ACTOR_FIRST_OAR + 8, ///< "You lookin' for Sean and Ouranos?"
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
  /// An invisible actor the camera follows as the rowers come up.
  ARRIVED_CAMERA_TARGET = ARRIVED_FIRST_PASSENGER + 9
};

// Shared code, included where it sits in the ROM.
#include "common/object_pulse.h"

/// An action-script call: shows the actor's sprite. Returns 0 to end the call.
static s32 TolbiShipDeck_ShowSprite(struct ScriptInterpreter *interpreter) {
  struct FieldActor *a = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(a, 1);
  return 0;
}

// The diving scripts own the actor's scratch fields: riseBaseY is sideways
// drift in 16.16 pixels per frame, riseCounter the dive step (0 hovering),
// riseEnabled the return side, timer the drift direction and work the bob
// direction. Using the actor's fields keeps one layout for the whole slot.

/// The update of a sea monster hovering by Ouranos's side of the deck while the
/// monsters attack. It drifts and bobs at random; one frame in a hundred it
/// dives at Ouranos, who turns and jumps (sometimes with an emote), then it
/// returns to its side. riseEnabled picks which side it returns to.
/// Returns 1 to keep running.
static s32
TolbiShipDeck_HoverAndDiveAtOuranos(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  if (obj->riseCounter != 0) {
    switch (obj->riseCounter) {
    case 1:
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      FieldObject_SetPosition(obj, PIXELS(268), PIXELS(20), PIXELS(692));
      obj->riseCounter++;
      break;
    case 2:
      obj->riseCounter++;
      break;
    case 3:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        obj->riseCounter++;
        Event_PlayCue(SE_146);
        if (obj->riseEnabled != 0)
          Actor_FaceDirection(SEA_OURANOS, 0xd000, 0);
        else
          Actor_FaceDirection(SEA_OURANOS, 0xb000, 0);
        if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
          Actor_Get(SEA_OURANOS)->velocityY = 0x20000;
        } else {
          Actor_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
          Actor_Get(SEA_OURANOS)->velocityY = 0x60000;
        }
      }
      break;
    case 4:
      obj->riseCounter++;
      break;
    case 5:
      if (obj->riseEnabled != 0)
        FieldObject_SetPosition(obj, PIXELS(282), 0, PIXELS(658));
      else
        FieldObject_SetPosition(obj, PIXELS(254), 0, PIXELS(668));
      obj->riseCounter++;
      break;
    case 6:
      obj->riseCounter++;
      break;
    case 7:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        obj->timer = 0;
        obj->work = 0;
        obj->riseCounter++;
        obj->riseBaseY = 0;
      }
      break;
    case 8:
      obj->riseCounter = 0;
      break;
    }
  } else {
    if (obj->timer != 0) {
      obj->riseBaseY -= ((u32)Engine_RandomNext() << 12) >> 16;
      if (obj->riseBaseY < -0x4000)
        obj->timer = 0;
    } else {
      obj->riseBaseY += ((u32)Engine_RandomNext() << 12) >> 16;
      if (obj->riseBaseY > 0x4000)
        obj->timer = 1;
    }
    if (obj->x.fixed > 0xf80000 && obj->x.fixed < 0x1240000)
      obj->x.fixed += obj->riseBaseY;
    if ((s16)obj->work != 0) {
      obj->y.fixed =
          obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
      if (obj->y.fixed < 0)
        obj->work = 0;
    } else {
      obj->y.fixed =
          obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
      if (obj->y.fixed > 0x80000)
        obj->work = 1;
    }
  }
  if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
    obj->riseCounter = 1;
  return 1;
}

/// The same for the monster that dives at Sean.
static s32
TolbiShipDeck_HoverAndDiveAtSean(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  if (obj->riseCounter != 0) {
    switch (obj->riseCounter) {
    case 1:
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      FieldObject_SetPosition(obj, PIXELS(256), PIXELS(20), PIXELS(640));
      obj->riseCounter++;
      break;
    case 2:
      obj->riseCounter++;
      break;
    case 3:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        obj->riseCounter++;
        Event_PlayCue(SE_146);
        if (obj->riseEnabled != 0)
          Actor_FaceDirection(SEA_SEAN, 0xd000, 0);
        else
          Actor_FaceDirection(SEA_SEAN, 0xb000, 0);
        if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
          Actor_Get(SEA_SEAN)->velocityY = 0x20000;
        } else {
          Actor_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
          Actor_Get(SEA_SEAN)->velocityY = 0x60000;
        }
      }
      break;
    case 4:
      obj->riseCounter++;
      break;
    case 5:
      if (obj->riseEnabled != 0)
        FieldObject_SetPosition(obj, PIXELS(264), 0, PIXELS(600));
      else
        FieldObject_SetPosition(obj, PIXELS(242), 0, PIXELS(604));
      obj->riseCounter++;
      break;
    case 6:
      obj->riseCounter++;
      break;
    case 7:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        obj->timer = 0;
        obj->work = 0;
        obj->riseCounter++;
        obj->riseBaseY = 0;
      }
      break;
    case 8:
      obj->riseCounter = 0;
      break;
    }
  } else {
    if (obj->timer != 0) {
      obj->riseBaseY -= ((u32)Engine_RandomNext() << 12) >> 16;
      if (obj->riseBaseY < -0x4000)
        obj->timer = 0;
    } else {
      obj->riseBaseY += ((u32)Engine_RandomNext() << 12) >> 16;
      if (obj->riseBaseY > 0x4000)
        obj->timer = 1;
    }
    if (obj->x.fixed > 0xe80000 && obj->x.fixed < 0x1100000)
      obj->x.fixed += obj->riseBaseY;
    if ((s16)obj->work != 0) {
      obj->y.fixed =
          obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
      if (obj->y.fixed < 0)
        obj->work = 0;
    } else {
      obj->y.fixed =
          obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
      if (obj->y.fixed > 0x80000)
        obj->work = 1;
    }
  }
  if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
    obj->riseCounter = 1;
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

/// Bobs the actor between 4 and 12 pixels high by a random amount each frame;
/// the actor's work halfword says which way it is going. Returns 1.
static s32 TolbiShipDeck_Bob(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 *flag = (s16 *)&actor->work;
  s32 val;
  s32 tmp;

  if (*flag != 0) {
    val = actor->y.fixed - (((u32)(Engine_RandomNext() << 15)) >> 16) - 0x8000;
    actor->y.fixed = val;
    if (val >= 0x40000)
      goto done;
    tmp = 0;
  } else {
    val = actor->y.fixed + (((u32)(Engine_RandomNext() << 15)) >> 16) + 0x8000;
    actor->y.fixed = val;
    if (val <= 0xC0000)
      goto done;
    tmp = 1;
  }
  *flag = tmp;
done:
  return 1;
}

/// Now and then (two chances in 32) turns the actor to look north-northeast or
/// north-northwest. Returns 1.
static s32 TolbiShipDeck_LookAround(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  u32 roll = ((u32)(Engine_RandomNext() << 5)) >> 16;

  if (roll == 6)
    actor->facing = 0xd000;
  else if (roll == 9)
    actor->facing = 0xb000;
  return 1;
}

/// The update of a monster on deck that, at random, leaps at Ouranos: it jumps
/// to him, he turns and jumps back (sometimes with an emote), and it lands on
/// one of two spots chosen by riseEnabled. The actor's timer holds the step.
/// Returns 1.
static s32 TolbiShipDeck_LeapAtOuranos(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  switch (obj->timer) {
  case 0:
    if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
      obj->timer++;
    }
    break;
  case 1:
    obj->timer++;
    break;
  case 2:
    obj->velocityY = 0x40000;
    obj->speed = 0x40000;
    obj->acceleration = 0x20000;
    FieldObject_SetPosition(obj, PIXELS(264), 0, PIXELS(662));
    obj->timer++;
    break;
  case 3:
    obj->timer++;
    break;
  case 4:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
      Engine_AudioPlayCue(SE_JUMP);
      if (obj->riseEnabled != 0) {
        Actor_FaceDirection(SEA_OURANOS, 0xb000, 0);
      } else {
        Actor_FaceDirection(SEA_OURANOS, 0x5000, 0);
      }
      if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
        Object_GetById(SEA_OURANOS)->velocityY = 0x20000;
      } else {
        Actor_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
        Object_GetById(SEA_OURANOS)->velocityY = 0x60000;
      }
    }
    break;
  case 5:
    obj->timer++;
    break;
  case 6:
    obj->timer++;
    obj->velocityY = 0x40000;
    obj->speed = 0x20000;
    obj->acceleration = 0x10000;
    if (obj->riseEnabled != 0) {
      FieldObject_SetPosition(obj, PIXELS(252), 0, PIXELS(646));
    } else {
      FieldObject_SetPosition(obj, PIXELS(256), 0, PIXELS(686));
    }
    break;
  case 7:
    obj->timer++;
    break;
  case 8:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
    }
    break;
  case 9:
    obj->timer = 0;
    break;
  }
  return 1;
}

/// The same for the monster that leaps at Sean.
static s32 TolbiShipDeck_LeapAtSean(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  switch (obj->timer) {
  case 0:
    if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
      obj->timer++;
    }
    break;
  case 1:
    obj->timer++;
    break;
  case 2:
    obj->velocityY = 0x40000;
    obj->speed = 0x40000;
    obj->acceleration = 0x20000;
    FieldObject_SetPosition(obj, PIXELS(176), 0, PIXELS(696));
    obj->timer++;
    break;
  case 3:
    obj->timer++;
    break;
  case 4:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
      Engine_AudioPlayCue(SE_JUMP);
      if (obj->riseEnabled != 0) {
        Actor_FaceDirection(SEA_SEAN, 0xd000, 0);
      } else {
        Engine_ActorFaceDirection(SEA_SEAN, FACING_EAST, 0);
      }
      if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
        Object_GetById(SEA_SEAN)->velocityY = 0x20000;
      } else {
        Actor_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
        Object_GetById(SEA_SEAN)->velocityY = 0x60000;
      }
    }
    break;
  case 5:
    obj->timer++;
    break;
  case 6:
    obj->timer++;
    obj->velocityY = 0x40000;
    obj->speed = 0x20000;
    obj->acceleration = 0x10000;
    if (obj->riseEnabled != 0) {
      FieldObject_SetPosition(obj, PIXELS(184), 0, PIXELS(672));
    } else {
      FieldObject_SetPosition(obj, PIXELS(202), 0, PIXELS(692));
    }
    break;
  case 7:
    obj->timer++;
    break;
  case 8:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
    }
    break;
  case 9:
    obj->timer = 0;
    break;
  }
  return 1;
}

/// Makes the actor hop at random, one frame in 40 on average. Returns 1.
static s32
TolbiShipDeck_RandomlyStartActorHop(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if ((((u32)(Engine_RandomNext() * 40)) >> 16) == 0)
    actor->velocityY = 0x40000;
  return 1;
}

/// The update that shrinks an enlarged actor by 1/32 a frame until it is back
/// to normal size.
static void TolbiShipDeck_ShrinkToNormalSize(union FieldObject *object) {
  struct FieldActor *actor = (struct FieldActor *)object;
  if (actor->scaleX > 0x10000) {
    actor->scaleX += 0xFFFFF800;
    actor->scaleY += 0xFFFFF800;
  }
}

/// Turns the actor east, northeast-by-north or southeast-by-south at random
/// intervals of 80 to 160 frames. riseCounter counts down the wait. Returns 1.
static s32 TolbiShipDeck_IdleTurn(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (actor->riseCounter != 0) {
    actor->riseCounter--;
  } else {
    u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

    if (roll > 200) {
      actor->facing = 0xd000;
    } else if (roll > 100) {
      actor->facing = 0x5000;
    } else {
      actor->facing = 0;
    }
    actor->riseCounter = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
  }
  return 1;
}

const struct SceneEntrance *TolbiShipDeck_GetEntrances(void) {
  return gTolbiShipDeckEntrances;
}

const struct SceneRegion *TolbiShipDeck_GetRegions(void) {
  return gTolbiShipDeckRegions;
}

const u32 *TolbiShipDeck_GetExits(void) { return gTolbiShipDeckExits; }

/// The talk facing of a Kalay actor's row in gTolbiShipDeckPlacementsWaiting.
#define KALAY_TALK_FACING(actor)                                               \
  STORE_CONST(gTolbiShipDeckPlacementsWaiting[(actor) - ACTOR_FIRST_PLACED]    \
                  .talkFacing)

const struct ScenePlacement *TolbiShipDeck_GetPlacements(void) {
  s32 afterFirstAttack;

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckPlacementsArrived;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING))
    return gTolbiShipDeckPlacementsRowing;
  afterFirstAttack =
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  if (afterFirstAttack != 0)
    return gTolbiShipDeckPlacementsAtSea;
  if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
      KALAY_TALK_FACING(KALAY_CREW_MEMBER) = afterFirstAttack;
      KALAY_TALK_FACING(KALAY_PASSENGER_WAITING_LONG) = 2;
      KALAY_TALK_FACING(KALAY_PASSENGER_SEEING_COLOSSO) = 2;
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN)) {
      KALAY_TALK_FACING(KALAY_PASSENGER_WAITING_LONG) = 1;
      KALAY_TALK_FACING(KALAY_PASSENGER_SEEING_COLOSSO) = 1;
    }
    return gTolbiShipDeckPlacementsWaiting;
  }
  return gTolbiShipDeckPlacements;
}

#undef KALAY_TALK_FACING

const struct SceneEvent *TolbiShipDeck_GetEvents(void) {
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
  struct FieldActor *crewMember;
  s32 random;
  s32 wait;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipNowWeHaveProtectShip);
    Event_ShowMessage(KALAY_CREW_MEMBER, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ActorRunRepeatedMotion(KALAY_CREW_MEMBER, 2);
      Engine_EventSetMessage(MsgTolbiShipSomebodyStopThem);
      Event_ShowMessage(KALAY_CREW_MEMBER, 0);
      crewMember = Object_GetById(KALAY_CREW_MEMBER);
      random = Random_Next();
      wait = ((u32)(90 * random) >> 16) + 60;
      crewMember->timer = wait;
      Actor_SetActionScript(KALAY_CREW_MEMBER,
                            gTolbiShipDeck_RandomActorActions);
    } else {
      Actor_ShowEmote(KALAY_CREW_MEMBER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Engine_ActorStartRepeatedMotion(KALAY_CREW_MEMBER, 3);
      Engine_EventSetMessage(MsgTolbiShipToldWereLeavingSoonSet);
      Event_ShowMessage(KALAY_CREW_MEMBER, 0);
    }
  }
  Engine_EventEnd();
}

/// The tour guide on deck at Kalay talks about the ship's delay. The guide has
/// the same three moods as TolbiShipDeck_TalkToCrewMember. "If we don't leave
/// soon, Colosso will start without us!"
static void TolbiShipDeck_TalkToTourGuide(void) {
  struct FieldActor *guide;
  s32 random;
  s32 wait;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipHaveMakeThemPromiseHelp);
    Event_ShowMessage(KALAY_TOUR_GUIDE, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ActorRunRepeatedMotion(KALAY_TOUR_GUIDE, 2);
      Engine_EventSetMessage(MsgTolbiShipTheyCantPlanningMutiny);
      Event_ShowMessage(KALAY_TOUR_GUIDE, 0);
      guide = Object_GetById(KALAY_TOUR_GUIDE);
      random = Random_Next();
      wait = ((u32)(90 * random) >> 16) + 60;
      guide->timer = wait;
      Actor_SetActionScript(KALAY_TOUR_GUIDE,
                            gTolbiShipDeck_RandomActorActions);
    } else {
      Actor_ShowEmote(KALAY_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Engine_ActorStartRepeatedMotion(KALAY_TOUR_GUIDE, 3);
      Engine_EventSetMessage(MsgTolbiShipIfWeDontLeaveSoon);
      Event_ShowMessage(KALAY_TOUR_GUIDE, 0);
    }
  }
  Engine_EventEnd();
}

/// At Tolbi, ARRIVED_ASKS_ABOUT_COLOSSO asks whether the party is headed for
/// Colosso too.
static void TolbiShipDeck_AskHeadedForColosso(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiShipHeadedColosso);
  Engine_EventAskYesNo(ARRIVED_ASKS_ABOUT_COLOSSO, 0);
  Engine_EventEnd();
}

/// Opens the first door to below deck, once a visit.
static void TolbiShipDeck_OpenFirstDoor(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_71) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
    GameFlag_Set(FLAG_SCENE_TEMP_71);
  }
}

/// Opens the second door to below deck, once a visit.
static void TolbiShipDeck_OpenSecondDoor(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_72) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
    GameFlag_Set(FLAG_SCENE_TEMP_72);
  }
}

/// Walking into one of the ways below deck, whose triggers are numbered as
/// their exits. The doors to the passengers' hold and the captain's cabin open
/// and the leader steps through; the hatch to the cargo hold just plays its
/// sound. Leaves by the exit numbered as the trigger.
static void TolbiShipDeck_GoBelow(void) {

  s32 throughDoor;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  throughDoor = 0;
  switch (work->touchedTrigger) {
  case TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD:
    throughDoor = 1;
    TolbiShipDeck_OpenFirstDoor();
    break;
  case TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN:
    throughDoor = 1;
    TolbiShipDeck_OpenSecondDoor();
    break;
  }
  if (throughDoor != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    FieldObjectMotion_CenterOnColumn(ACTOR_PARTY_LEADER, 1, -10);
    Engine_EventWait(10);
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
  }
  Engine_EventRequestExit(work->touchedTrigger);
}

#if TBS_PATCH_2
/// In the German, Spanish, French and Italian editions trigger 2 is a plain
/// touch: the leader goes below only while facing west, after walking onto the
/// hatch.
static void TolbiShipDeck_GoBelowWhileFacingWest(void) {
  if ((u16)(Object_GetById(ACTOR_PARTY_LEADER)->facing - 0x6001) <= 0x3ffe) {
    Engine_EventBegin();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 214, 664);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    TolbiShipDeck_GoBelow();
  }
}
#endif

/// The captain decides to set sail without the replacement ship. On deck at
/// Kalay, once the tour group has left Kalay (FLAG_KALAY_COLOSSO_TOUR_LEFT;
/// touch cell 10), the captain, Kaja and the crewman wonder what happened to
/// the replacement ship, and the captain decides to set sail anyway, trusting
/// his anchor charm; the crewman runs off through the second door. Plays once
/// (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY).
static void TolbiShipDeck_WatchCrewDecideToSail(void) {
  struct FieldActor *record;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    Engine_EventBegin();
    Field_EndBusyAndReveal();
    Actor_FaceActor(ACTOR_PARTY_LEADER, KALAY_CREWMAN, 10);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(PIXELS(190), -1, PIXELS(708), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(KALAY_CAPTAIN, 1);
    Engine_EventSetMessage(MsgTolbiShipWonderCouldHaveHappened);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Actor_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Engine_ActorRunRepeatedMotion(KALAY_CAPTAIN, 1);
    Actor_FaceDirection(KALAY_CAPTAIN, 0x5000, 0);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Engine_ActorRunRepeatedMotion(KALAY_CREWMAN, 1);
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xb000);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    TolbiShipDeck_Turn(KALAY_KAJA, 0x3000);
    Engine_ActorSetAnimation(KALAY_KAJA, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    Actor_ShowEmote(KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_FaceDirection(KALAY_CAPTAIN, FACING_WEST, 20);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    TolbiShipDeck_Turn(KALAY_KAJA, FACING_EAST);
    Engine_ActorSetAnimationAndWait(KALAY_KAJA, 4);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    Actor_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Engine_ActorSetAnimationAndWait(KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xd000);
    Engine_ActorSetAnimation(KALAY_KAJA, 3);
    Engine_ActorSetAnimationAndWait(KALAY_CREWMAN, 3);
    Engine_EventWait(60);
    Actor_ShowEmote(KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    TolbiShipDeck_Turn(KALAY_CAPTAIN, 0x5000);
    Engine_EventSetMessage(MsgTolbiShipTheresNothingWeCanDo);
    Engine_ActorStartRepeatedMotion(KALAY_CAPTAIN, 1);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Actor_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Actor_ShowEmote(KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessageAndWait(SPEAKER(KALAY_CAPTAIN, 4), 0, 20);
    Actor_ShowEmote(KALAY_KAJA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    TolbiShipDeck_Turn(KALAY_CAPTAIN, FACING_WEST);
    Engine_ActorSetAnimationAndWait(KALAY_CAPTAIN, 3);
    Event_ShowMessageAndWait(SPEAKER(KALAY_CAPTAIN, 4), 0, 20);
    Actor_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    TolbiShipDeck_Turn(KALAY_CAPTAIN, 0x5000);
    Engine_ActorSetAnimation(KALAY_CAPTAIN, 4);
    TolbiShipDeck_Say(KALAY_CAPTAIN);
    Actor_FaceDirection(KALAY_CREWMAN, 0xb000, 0);
    Actor_FaceDirection(KALAY_KAJA, 0x3000, 40);
    Actor_FaceDirection(KALAY_KAJA, FACING_EAST, 0);
    Actor_FaceDirection(KALAY_CREWMAN, 0xd000, 20);
    Engine_ActorRunRepeatedMotion(KALAY_CAPTAIN, 2);
    Engine_EventWait(20);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Actor_SetAttachedEffect(KALAY_KAJA, 0x102);
    Actor_SetAttachedEffect(KALAY_CREWMAN, 0x102);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(PIXELS(182), -1, PIXELS(760), 1);
    Actor_SetSpeed(KALAY_KAJA, 0xcccc, 0x6666);
    Actor_SetActionScript(KALAY_KAJA, gTolbiShipDeck_PresentationActionsA);
    Actor_SetSpeed(KALAY_CAPTAIN, 0xcccc, 0x6666);
    Actor_SetActionScript(KALAY_CAPTAIN, gTolbiShipDeck_PresentationActionsB);
    Actor_SetSpeed(KALAY_CREWMAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(KALAY_CREWMAN, 182, 0x2f8);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    Actor_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xd000);
    Event_ShowMessageAndWait(KALAY_CREWMAN, 0, 20);
    Engine_ActorSetAnimationAndWait(KALAY_CREWMAN, 3);
    Engine_ActorJump(KALAY_CREWMAN, 4, 0);
    Actor_FaceDirection(KALAY_CREWMAN, 0x3000, 40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(216), -1, PIXELS(790), 1);
    {
      struct FieldActor *record = Object_GetById(KALAY_CREWMAN);
      record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
    }
    Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
    Actor_WalkToAndWait(KALAY_CREWMAN, 182, 0x30e);
    Actor_WalkToAndWait(KALAY_CREWMAN, 192, 0x328);
    Actor_WalkToAndWait(KALAY_CREWMAN, 216, 0x328);
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xd000);
    Engine_ActorRunRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_OpenSecondDoor();
    Actor_WalkToAndWait(KALAY_CREWMAN, 216, 0x31e);
    Actor_SetPosition(KALAY_CREWMAN, 0, 0);
    record = Object_GetById(KALAY_CREWMAN);
    record->scaleX = 0x10000;
    record = Object_GetById(KALAY_CREWMAN);
    record->scaleY = 0x10000;
    GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
    Engine_EventEnd();
  }
}

/// On deck at Kalay, approaching from the east side of the door (touch cell
/// 11): the crewman comes out, protests and slips away round the party to the
/// bow. "I didn't do anything..." Runs until the party has confronted the
/// crewman.
static void TolbiShipDeck_CatchCrewmanFromEast(void) {
  u8 bits;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Engine_EventBegin();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(KALAY_CREWMAN, 0x6666, 0x3333);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 232, 0x330);
      Engine_EventWait(1);
      bits = 1;
      {
        struct FieldActor *record = Object_GetById(KALAY_CREWMAN);
        u8 value = record->steeringFlags;

        record->steeringFlags = value | bits;
      }
      Engine_EventWait(20);
      Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(KALAY_CREWMAN);
      Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 244, 0x324);
      Engine_EventWait(1);
      {
        struct FieldActor *record = Object_GetById(KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Engine_EventWait(20);
      Actor_SetSpeed(KALAY_CREWMAN, 0x33333, 0x19999);
      Actor_WalkToAndWait(KALAY_CREWMAN, 248, 0x30a);
      Actor_WalkToAndWait(KALAY_CREWMAN, 248, 0x2bc);
      Actor_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_FaceDirection(KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Engine_EventEnd();
    }
  }
}

/// The same, approaching from the west side (touch cell 12).
static void TolbiShipDeck_CatchCrewmanFromWest(void) {
  u8 bits;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Engine_EventBegin();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(KALAY_CREWMAN, 0x6666, 0x3333);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 202, 0x330);
      Engine_EventWait(1);
      bits = 1;
      {
        struct FieldActor *record = Object_GetById(KALAY_CREWMAN);
        u8 value = record->steeringFlags;

        record->steeringFlags = value | bits;
      }
      Engine_EventWait(20);
      Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(KALAY_CREWMAN);
      Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 192, 0x324);
      Engine_EventWait(1);
      {
        struct FieldActor *record = Object_GetById(KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Engine_EventWait(20);
      Actor_SetSpeed(KALAY_CREWMAN, 0x33333, 0x19999);
      Actor_WalkToAndWait(KALAY_CREWMAN, 180, 0x30a);
      Actor_WalkToAndWait(KALAY_CREWMAN, 180, 0x2bc);
      Actor_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_FaceDirection(KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Engine_EventEnd();
    }
  }
}

/// The start both of those share: the camera pans to the door, which opens, and
/// the crewman steps out and denies everything. Records the confrontation.
static void TolbiShipDeck_ConfrontCrewman(void) {
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(824), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  TolbiShipDeck_OpenSecondDoor();
  Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
  Engine_EventWait(10);
  Actor_SetPosition(KALAY_CREWMAN, PIXELS(216), PIXELS(800));
  Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
  Actor_WalkToAndWait(KALAY_CREWMAN, 216, 0x32e);
  Actor_FaceActor(ACTOR_PARTY_LEADER, KALAY_CREWMAN, 10);
  Engine_ActorSetAnimationAndWait(KALAY_CREWMAN, 4);
  Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
  Actor_ShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceActor(KALAY_CREWMAN, ACTOR_PARTY_LEADER, 20);
  Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
  Engine_EventSetMessage(MsgTolbiShipDidntDoAnything);
  Event_ShowMessageAndWait(KALAY_CREWMAN, 0, 20);
  Engine_ActorShowEmote(KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT);
}

/// Sets up the scene on entry: clears FLAG_NOT_ON_MAP, sets the transition,
/// starts the deck's sway at a random angle and stops the sea's scroll, redraws
/// the map and applies the voyage's state.
s32 TolbiShipDeck_Initialize(void) {
  void **base = gMapWork;
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)base[0])->layers[MAP_LAYER_GROUND];

  Engine_GameFlagClear(FLAG_NOT_ON_MAP);
  ((struct EventWork *)base[HEAP_SLOT_EVENT - HEAP_SLOT_MAP])->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  layer->speedY = 0;
  sSwayAngleX = (u16)Engine_RandomNext();
  sSwayAngleY = (u16)Engine_RandomNext();
  sSeaScroll[0] = 0;
  sSeaScroll[1] = 0;
  sSeaSpeed[0] = 0;
  Engine_MapRedraw();
  Engine_TaskWait(1);
  TolbiShipDeck_ApplyEntryState();
  return 0;
}

/// Applies the voyage's state: which cells and oars are drawn, whether the deck
/// rocks and the sea scrolls, and which cutscene the entrance starts (the
/// entrances from TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY on are the
/// voyage's cutscenes, chained by their exits).
static void TolbiShipDeck_ApplyEntryState(void) {
  s32 flag;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_71);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_72);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
    Engine_ActorSetChildValue(ARRIVED_FIRST_PASSENGER + 1, 2);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
  } else if ((flag = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING)) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_SetUpOars();
    Task_AddCallback(TolbiShipDeck_RowOars, TASK_PRIORITY_SCENE);
    sSeaScroll[1] = 0x200000;
    sSeaSpeed[1] = 0x13333;
    FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, TASK_PRIORITY_SCENE);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
             0) {
    TolbiShipDeck_CopyUnderwayCells();
    sSeaScroll[1] = flag;
    sSeaSpeed[1] = flag;
    FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, TASK_PRIORITY_SCENE);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING) == 0) {
    TolbiShipDeck_RestOars();
  }
  switch (gGameState.entrance) {
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST:
    Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority = 1;
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
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED:
    TolbiShipDeck_MonstersAttackCrew();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_MONSTER_LANDS:
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
    ((struct MapScrollWork *)gMapWork[0])->minX = PIXELS(65);
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
  struct FieldActor *actor;
  s32 flag;

  Engine_ActorSetSpritePriority(KALAY_PASSENGER_KARAGOL, 1);
  Engine_ActorSetSpritePriority(KALAY_KAJA, 1);
  Engine_ActorSetSpritePriority(KALAY_CAPTAIN, 1);
  Engine_ActorSetSpritePriority(KALAY_PASSENGER_SEEING_COLOSSO, 1);
  Engine_ActorSetSpritePriority(KALAY_TOUR_GUIDE, 1);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY) != 0) {
    Engine_ActorSetPosition(KALAY_CAPTAIN, PIXELS(162), PIXELS(666));
    Object_GetById(KALAY_CAPTAIN)->facing = FACING_WEST;
    Engine_ActorSetPosition(KALAY_KAJA, 0, 0);
    Engine_ActorSetPosition(KALAY_CREWMAN, 0, 0);
  }
  flag = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
  if (flag != 0) {
    Engine_ActorSetPosition(KALAY_CREW_MEMBER, PIXELS(264), PIXELS(702));
    Object_GetById(KALAY_CREW_MEMBER)->facing = 0x5000;
    actor = Object_GetById(KALAY_CREW_MEMBER);
    actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    Actor_SetActionScript(KALAY_CREW_MEMBER, gTolbiShipDeck_RandomActorActions);
    Engine_ActorSetPosition(KALAY_TOUR_GUIDE, PIXELS(248), PIXELS(680));
    actor = Actor_Get(KALAY_TOUR_GUIDE);
    actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    Actor_SetActionScript(KALAY_TOUR_GUIDE, gTolbiShipDeck_RandomActorActions);
    Engine_ActorSetPosition(KALAY_CAPTAIN, 0, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0) {
      Actor_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Object_GetById(KALAY_CREWMAN)->facing = flag;
    }
  }
}

/// At Kalay once the anchor charm is back
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED): KALAY_CREW_MEMBER, the tour guide
/// and the three passengers take new places, and the crewman, the captain and
/// Kaja leave the deck.
static void TolbiShipDeck_PlacePassengersAgreedToRow(void) {
  Engine_ActorSetPosition(KALAY_CREW_MEMBER, PIXELS(262), PIXELS(706));
  Object_GetById(KALAY_CREW_MEMBER)->facing = 0x5000;
  Engine_ActorSetPosition(KALAY_TOUR_GUIDE, PIXELS(164), PIXELS(648));
  Object_GetById(KALAY_TOUR_GUIDE)->facing = 0;
  Engine_ActorSetSpritePriority(KALAY_TOUR_GUIDE, 1);
  Engine_ActorSetPosition(KALAY_PASSENGER_WAITING_LONG, PIXELS(198),
                          PIXELS(665));
  Object_GetById(KALAY_PASSENGER_WAITING_LONG)->facing = FACING_WEST;
  Engine_ActorSetSpritePriority(KALAY_PASSENGER_WAITING_LONG, 1);
  Engine_ActorSetPosition(KALAY_PASSENGER_SEEING_COLOSSO, PIXELS(188),
                          PIXELS(678));
  Object_GetById(KALAY_PASSENGER_SEEING_COLOSSO)->facing = 0xb000;
  Engine_ActorSetPosition(KALAY_PASSENGER_KARAGOL, PIXELS(186), PIXELS(635));
  Object_GetById(KALAY_PASSENGER_KARAGOL)->facing = 0x5000;
  Engine_ActorSetPosition(KALAY_CAPTAIN, 0, 0);
  Engine_ActorSetPosition(KALAY_KAJA, 0, 0);
  Engine_ActorSetPosition(KALAY_CREWMAN, 0, 0);
}

/// Under sail: the crew take their places, the two lookouts start their rounds
/// and, when the party keeps its position, the leader stands on
/// deck.
static void TolbiShipDeck_SetUpUnderSail(void) {
  struct FieldActor *record;
  u8 bits;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_KAJA, 0, 0);
  Actor_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  Actor_SetPosition(SEA_SEAN, PIXELS(204), PIXELS(521));
  record = Object_GetById(SEA_SEAN);
  record->y.fixed = PIXELS(16);
  bits = 128;
  {
    struct FieldActor *record = Object_GetById(SEA_SEAN);
    u8 value = record->collisionFlags;

    record->collisionFlags = value | bits;
  }
  Actor_SetSpeed(SEA_SEAN, 0x9999, 0x4ccc);
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_LeadActionsA);
  {
    struct FieldActor *record = Object_GetById(SEA_OURANOS);

    bits |= record->collisionFlags;
    record->collisionFlags = bits;
  }
  Actor_SetSpeed(SEA_OURANOS, 0xcccc, 0x6666);
  Actor_SetActionScript(SEA_OURANOS, gTolbiShipDeck_LeadActionsB);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Engine_EventEnd();
}

/// After the Kraken: as under sail, with Sean at the helm.
static void TolbiShipDeck_SetUpAfterKraken(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_KAJA, 0, 0);
  Actor_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  Actor_SetPosition(SEA_SEAN, PIXELS(268), PIXELS(678));
  record = Object_GetById(SEA_SEAN);
  record->facing = 0;
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_LeadActionsC);
  {
    struct FieldActor *record = Object_GetById(SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  Actor_SetSpeed(SEA_OURANOS, 0xcccc, 0x6666);
  Actor_SetActionScript(SEA_OURANOS, gTolbiShipDeck_LeadActionsB);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Engine_EventEnd();
}

/// At anchor off the island: the crew stand where they were left, by whether
/// the party has agreed to scout the island, and entering by
/// TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS means the party is back from it.
static void TolbiShipDeck_SetUpAtAnchor(void) {
  struct FieldActor *record;
  s32 facing;

  ((struct MapScrollWork *)gMapWork[0])->minX = PIXELS(65);
  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScriptE);
  Engine_TaskWait(1);
  Engine_ActorDestroy(SEA_KRAKEN);
  Actor_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  facing = 192; // 192 << 6 is 0x3000, south-southeast
  record = Object_GetById(SEA_CREW_MEMBER);
  record->facing = (facing << 6);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED) != 0) {
    Actor_SetPosition(SEA_SEAN, PIXELS(162), PIXELS(634));
    record = Object_GetById(SEA_SEAN);
    record->facing = (facing << 6);
    Actor_SetPosition(SEA_OURANOS, PIXELS(162), PIXELS(676));
    record = Object_GetById(SEA_OURANOS);
    record->facing = 0xd000;
  } else {
    Engine_ActorSetPosition(SEA_SEAN, PIXELS(160), PIXELS(652));
    record = Object_GetById(SEA_SEAN);
    record->facing = (facing << 6);
    Actor_SetPosition(SEA_OURANOS, PIXELS(166), PIXELS(668));
    record = Object_GetById(SEA_OURANOS);
    record->facing = 0xb000;
  }
  if (gGameState.entrance == TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) {
    TolbiShipDeck_ReturnFromIsland();
  }
  Engine_EventEnd();
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY, setting sail from Kalay
/// before the party is on deck: Kaja (KALAY_KAJA) brings word to
/// KALAY_CREW_MEMBER that all is ready.
static void TolbiShipDeck_DepartFromKalay(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(KALAY_CREWMAN, 0, 0);
  Actor_SetPosition(KALAY_CAPTAIN, 0, 0);
  Actor_SetPosition(KALAY_TOUR_GUIDE, 0, 0);
  Actor_SetPosition(KALAY_PASSENGER_WAITING_LONG, 0, 0);
  Actor_SetPosition(KALAY_PASSENGER_SEEING_COLOSSO, 0, 0);
  Actor_SetPosition(KALAY_PASSENGER_KARAGOL, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(KALAY_KAJA, 0, 0);
  record = Object_GetById(KALAY_KAJA);
  record->facing = 0x3000;
  Actor_SetPosition(KALAY_CREW_MEMBER, PIXELS(232), PIXELS(650));
  record = Object_GetById(KALAY_CREW_MEMBER);
  record->facing = 0xb000;
  Camera_MoveTo(PIXELS(232), -1, PIXELS(636), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  TolbiShipDeck_AnnouncePreparationsReady(KALAY_KAJA, KALAY_CREW_MEMBER);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_READY on later departures, with
/// the party aboard: Kaja brings word to SEA_CREW_MEMBER.
static void TolbiShipDeck_DepartWithParty(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(636));
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_ActorStop(SEA_SEAN);
  Engine_ActorStop(SEA_OURANOS);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_SEAN, 0, 0);
  Actor_SetPosition(SEA_OURANOS, 0, 0);
  Actor_SetPosition(SEA_KAJA, 0, 0);
  record = Object_GetById(SEA_KAJA);
  record->facing = 0x3000;
  Actor_SetPosition(SEA_CREW_MEMBER, PIXELS(232), PIXELS(650));
  record = Object_GetById(SEA_CREW_MEMBER);
  record->facing = 0xb000;
  Engine_TaskWait(1);
  TolbiShipDeck_AnnouncePreparationsReady(SEA_KAJA, SEA_CREW_MEMBER);
}

/// The messenger comes up through the first door to report, the crew member
/// waiting there calls the ship ready, and the scene moves on to the next stage
/// of the voyage. "Preparations... Ready!"
static void TolbiShipDeck_AnnouncePreparationsReady(s32 messenger,
                                                    s32 crewMember) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(messenger, PIXELS(216), PIXELS(588));
  Actor_SetSpeed(messenger, 0xcccc, 0x6666);
  Actor_WalkToAndWait(messenger, 216, 0x258);
  Actor_WalkToAndWait(messenger, 218, 0x25c);
  Actor_WalkToAndWait(messenger, 234, 0x25c);
  Actor_WalkToAndWait(messenger, 236, 0x26a);
  Actor_FaceDirection(messenger, 0x5000, 20);
  Engine_ActorSetAnimationAndWait(messenger, 3);
  Engine_EventWait(20);
  TolbiShipDeck_Turn(crewMember, 0x5000);
  Engine_ActorJump(crewMember, 4, 40);
  Engine_ActorStartRepeatedMotion(crewMember, 2);
  Engine_EventSetMessage(MsgTolbiShipPreparationsReady);
  Event_ShowMessageAndWait(crewMember, 0, 20);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_SEA_MONSTERS: sea monsters are
/// sighted. "Aye, Captain! Sea monsters!"
static void TolbiShipDeck_SightSeaMonsters(void) {
  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(SEA_KAJA, 1);
  Engine_EventSetMessage(MsgTolbiShipAyeCaptainSeaMonsters);
  Engine_EventShowMessageAndWait(SEA_KAJA, 0, 10);
  TolbiShipDeck_Turn(SEA_SEAN, 0x5000);
  Engine_ActorJump(SEA_SEAN, 4, 20);
  Engine_ActorStartRepeatedMotion(SEA_SEAN, 2);
  Engine_EventShowMessageAndWait(SPEAKER(SEA_SEAN, 6), 0, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_TURN);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_MONSTERS_BOARD: the monsters leap
/// aboard and someone cries out. "It's no use. It's too late!"
static void TolbiShipDeck_MonstersBoard(void) {
  s32 zero;
  const s32 *shared;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScriptB);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_OURANOS, PIXELS(248), PIXELS(728));
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Actor_SetSpeed(SEA_OURANOS, 104857, 52428);
  Actor_WalkToAndWait(SEA_OURANOS, 242, 692);
  Actor_WalkToAndWait(SEA_OURANOS, 196, 678);
  Actor_WalkToAndWait(SEA_OURANOS, 182, 654);
  Engine_ActorRunRepeatedMotion(SEA_OURANOS, 2);
  Engine_EventSetMessage(MsgTolbiShipNoUseLate);
  TolbiShipDeck_Say(SPEAKER(SEA_OURANOS, 10));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 157286, 78643);
  FieldObjectMotion_ResetAndSetPosition(ACTOR_PARTY_LEADER, 154, 609);
  Engine_AudioPlayCue(SE_146);
  zero = 0;
  Object_GetById(SEA_FIRST_ATTACK_MONSTER)->timer = zero;
  Object_GetById(SEA_FIRST_ATTACK_MONSTER + 1)->timer = zero;
  Object_GetById(SEA_FIRST_ATTACK_MONSTER + 2)->timer = zero;
  Actor_SetPosition(SEA_FIRST_ATTACK_MONSTER, PIXELS(32), PIXELS(484));
  Actor_SetPosition(SEA_FIRST_ATTACK_MONSTER + 1, PIXELS(84), PIXELS(496));
  Actor_SetPosition(SEA_FIRST_ATTACK_MONSTER + 2, PIXELS(16), PIXELS(596));
  Actor_SetSpeed(SEA_FIRST_ATTACK_MONSTER, 157286, 78643);
  Actor_SetSpeed(SEA_FIRST_ATTACK_MONSTER + 1, 157286, 78643);
  Actor_SetSpeed(SEA_FIRST_ATTACK_MONSTER + 2, 157286, 78643);
  shared = gTolbiShipDeck_DeckEventActions;
  Actor_SetActionScript(SEA_FIRST_ATTACK_MONSTER, shared);
  Actor_SetActionScript(SEA_FIRST_ATTACK_MONSTER + 1, shared);
  Actor_SetActionScript(SEA_FIRST_ATTACK_MONSTER + 2, shared);
  Engine_ActorSetChildValue(SEA_FIRST_ATTACK_MONSTER, 3);
  Engine_ActorSetChildValue(SEA_FIRST_ATTACK_MONSTER + 1, 3);
  Engine_ActorSetChildValue(SEA_FIRST_ATTACK_MONSTER + 2, 3);
  do {
    Engine_TaskWait(1);
  } while (Object_GetById(SEA_FIRST_ATTACK_MONSTER)->timer == 0);
  TolbiShipDeck_OpenFirstDoor();
  /* FAKEMATCH(one-pass): the block sets r0 = 21 straight after the call above;
   * as a plain call, 1 of 163 instructions differ in tbs-en. */
  do {
    Actor_WalkTo(SEA_OURANOS, 196, 612);
  } while (0);
  Object_RefreshSelectorById(SEA_FIRST_ATTACK_MONSTER);
  Engine_EventWait(10);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(10);
  ObjectSystem_SuspendProjectionAndShowSprites();
  Ui_SetMessageVoiceFromActor(SEA_OURANOS);
  UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_FIRST_ATTACK);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED: the monsters harry the
/// crew while more of them climb aboard.
static void TolbiShipDeck_MonstersAttackCrew(void) {

  Engine_EventBegin();
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScriptC);
  Engine_TaskWait(1);
  Engine_ActorSetAnimation(SEA_SWELLING_SPRITE, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_FIRST_HOVERING_MONSTER), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_FIRST_HOVERING_MONSTER + 1), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_FIRST_HOVERING_MONSTER + 2), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_FIRST_HOVERING_MONSTER + 3), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_SECOND_ATTACK_MONSTER), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_SECOND_ATTACK_MONSTER + 1), 1);
  Actor_SetPosition(SEA_SEAN, PIXELS(256), PIXELS(640));
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_CrewActionsD);
  Actor_SetPosition(SEA_OURANOS, PIXELS(268), PIXELS(692));
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_CrewActionsD);
  Actor_SetPosition(SEA_FIRST_HOVERING_MONSTER, PIXELS(242), PIXELS(604));
  Actor_SetPosition(SEA_FIRST_HOVERING_MONSTER + 1, PIXELS(264), PIXELS(600));
  Actor_SetPosition(SEA_FIRST_HOVERING_MONSTER + 2, PIXELS(254), PIXELS(668));
  Engine_ActorSetPosition(SEA_FIRST_HOVERING_MONSTER + 3, PIXELS(282),
                          PIXELS(658));
  Object_GetById(SEA_FIRST_HOVERING_MONSTER)->riseEnabled = 0;
  Object_GetById(SEA_FIRST_HOVERING_MONSTER + 1)->riseEnabled = 1;
  Object_GetById(SEA_FIRST_HOVERING_MONSTER + 2)->riseEnabled = 0;
  Object_GetById(SEA_FIRST_HOVERING_MONSTER + 3)->riseEnabled = 1;
  Actor_SetActionScript(SEA_FIRST_HOVERING_MONSTER,
                        gTolbiShipDeck_CrewActionsB);
  Actor_SetActionScript(SEA_FIRST_HOVERING_MONSTER + 1,
                        gTolbiShipDeck_CrewActionsB);
  Actor_SetActionScript(SEA_FIRST_HOVERING_MONSTER + 2,
                        gTolbiShipDeck_CrewActionsA);
  Actor_SetActionScript(SEA_FIRST_HOVERING_MONSTER + 3,
                        gTolbiShipDeck_CrewActionsA);
  Engine_ActorSetPosition(SEA_KAJA, 0, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(0x190);
  Actor_SetPosition(SEA_SECOND_ATTACK_MONSTER, PIXELS(254), PIXELS(740));
  Actor_SetPosition(SEA_SECOND_ATTACK_MONSTER + 1, PIXELS(24), PIXELS(586));
  Actor_SetSpeed(SEA_SECOND_ATTACK_MONSTER, 0x19999, 0xcccc);
  Actor_SetSpeed(SEA_SECOND_ATTACK_MONSTER + 1, 0x19999, 0xcccc);
  Actor_SetDestination(SEA_SECOND_ATTACK_MONSTER + 1, 172, 0x284);
  FieldObject_MotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER, 200, 0x294);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 174, 0x26c);
  Engine_ObjectMotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER, 180,
                                          0x244);
  Engine_AudioPlayCue(SE_146);
  Actor_SetActionScript(SEA_SECOND_ATTACK_MONSTER, gTolbiShipDeck_CrewActionsC);
  Actor_SetActionScript(SEA_SECOND_ATTACK_MONSTER + 1,
                        gTolbiShipDeck_CrewActionsC);
  Engine_AudioPlayCue(SE_240);
  Actor_SetPosition(SEA_SWELLING_SPRITE, PIXELS(134), PIXELS(594));
  Actor_SetActionScript(SEA_SWELLING_SPRITE, gTolbiShipDeck_CrewActionsE);
  Engine_EventWait(10);
  Actor_SetPosition(SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(134), PIXELS(584));
  Actor_SetSpeed(SEA_SECOND_ATTACK_MONSTER + 2, 0x40000, 0x20000);
  Object_GetById(SEA_SECOND_ATTACK_MONSTER + 2)->velocityY = 0x80000;
  Engine_ObjectMotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER + 2, 186,
                                          0x264);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_SECOND_ATTACK_MONSTER + 2), 1);
  Engine_EventWait(10);
  Actor_SetSpeed(SEA_SECOND_ATTACK_MONSTER + 2, 0x20000, 0x10000);
  Engine_ObjectMotionSetPositionAndCommit(SEA_SECOND_ATTACK_MONSTER + 2, 216,
                                          0x258);
  TolbiShipDeck_Turn(SEA_SECOND_ATTACK_MONSTER + 2, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_SetActionScript(SEA_SECOND_ATTACK_MONSTER + 2,
                        gTolbiShipDeck_SailorActions);
  Engine_EventWait(10);
  Actor_SetActionScript(SEA_SECOND_ATTACK_MONSTER,
                        gTolbiShipDeck_SailorActions);
  Engine_EventWait(10);
  Actor_RunActionScriptAndWait(SEA_SECOND_ATTACK_MONSTER + 1,
                               gTolbiShipDeck_SailorActions);
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_147);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_ActorStop(SEA_FIRST_HOVERING_MONSTER);
  Engine_ActorStop(SEA_FIRST_HOVERING_MONSTER + 1);
  Engine_ActorStop(SEA_FIRST_HOVERING_MONSTER + 2);
  Engine_ActorStop(SEA_FIRST_HOVERING_MONSTER + 3);
  Engine_EventWait(10);
  ObjectSystem_SuspendProjectionAndShowSprites();
  Ui_SetMessageVoiceFromActor(SEA_OURANOS);
  UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_EventRequestExit(
      TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SECOND_ATTACK);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_MONSTER_LANDS: a great shape rises from
/// the sea and a monster drops onto the deck.
static void TolbiShipDeck_MonsterLandsOnDeck(void) {
  struct FieldActor *monster;
  struct FieldActor *record;
  s32 frame;
  const s32 *sailorActions;

  Engine_EventBegin();
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(record, 0);
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScriptD);
  Engine_TaskWait(1);
  Engine_ActorSetPosition(SEA_SEAN, PIXELS(176), PIXELS(696));
  record = Object_GetById(SEA_SEAN);
  record->facing = 0xd000;
  Engine_ActorSetPosition(SEA_OURANOS, PIXELS(264), PIXELS(662));
  record = Object_GetById(SEA_OURANOS);
  record->facing = 0xb000;
  Actor_SetPosition(SEA_FIRST_LEAPING_MONSTER, PIXELS(184), PIXELS(672));
  Actor_SetPosition(SEA_FIRST_LEAPING_MONSTER + 1, PIXELS(202), PIXELS(692));
  Actor_SetPosition(SEA_FIRST_LEAPING_MONSTER + 2, PIXELS(252), PIXELS(646));
  Actor_SetPosition(SEA_FIRST_LEAPING_MONSTER + 3, PIXELS(256), PIXELS(686));
  Actor_SetPosition(SEA_THIRD_ATTACK_MONSTER, PIXELS(172), PIXELS(632));
  Engine_ActorSetPosition(SEA_THIRD_ATTACK_MONSTER + 1, PIXELS(256),
                          PIXELS(622));
  Object_GetById(SEA_FIRST_LEAPING_MONSTER)->riseEnabled = 0;
  Object_GetById(SEA_FIRST_LEAPING_MONSTER + 1)->riseEnabled = 1;
  Object_GetById(SEA_FIRST_LEAPING_MONSTER + 2)->riseEnabled = 0;
  Object_GetById(SEA_FIRST_LEAPING_MONSTER + 3)->riseEnabled = 2;
  Engine_ActorSetPosition(SEA_KAJA, 0, 0);
  Actor_SetActionScript(SEA_FIRST_LEAPING_MONSTER, gTolbiShipDeck_DeckActionsA);
  Actor_SetActionScript(SEA_FIRST_LEAPING_MONSTER + 1,
                        gTolbiShipDeck_DeckActionsA);
  Actor_SetActionScript(SEA_FIRST_LEAPING_MONSTER + 2,
                        gTolbiShipDeck_DeckActionsB);
  Actor_SetActionScript(SEA_FIRST_LEAPING_MONSTER + 3,
                        gTolbiShipDeck_DeckActionsB);
  Actor_SetActionScript(SEA_THIRD_ATTACK_MONSTER, gTolbiShipDeck_DeckActionsC);
  Actor_SetActionScript(SEA_THIRD_ATTACK_MONSTER + 1,
                        gTolbiShipDeck_DeckActionsC);
  Engine_ActorSetChildValue(SEA_FIRST_LEAPING_MONSTER, 3);
  Engine_ActorSetChildValue(SEA_FIRST_LEAPING_MONSTER + 1, 3);
  Engine_ActorSetChildValue(SEA_FIRST_LEAPING_MONSTER + 2, 3);
  Engine_ActorSetChildValue(SEA_FIRST_LEAPING_MONSTER + 3, 3);
  Engine_ActorSetChildValue(SEA_THIRD_ATTACK_MONSTER, 3);
  Engine_ActorSetChildValue(SEA_THIRD_ATTACK_MONSTER + 1, 3);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  Engine_AudioPlayCue(SE_147);
  monster = Object_GetById(SEA_LANDING_SPOT);
  monster->scaleX = 0x1999;
  monster->scaleY = 0x1999;
  monster->x.fixed = PIXELS(194);
  monster->z.fixed = PIXELS(642);
  for (frame = 0; (u32)frame < 16; frame++) {
    monster->scaleX += 0xf5c;
    monster->scaleY += 0xf5c;
    Engine_TaskWait(1);
  }
  monster = Object_GetById(SEA_THIRD_ATTACK_MONSTER + 2);
  monster->scaleX = 0x11999;
  monster->scaleY = 0x11999;
  monster->x.fixed = PIXELS(194);
  monster->y.fixed = PIXELS(80);
  monster->z.fixed = PIXELS(642);
  monster->facing = 0x5000;
  monster->bounce = 0x6666;
  monster->gravity = 0x20000;
  Engine_EventWait(80);
  Engine_AudioPlayCue(SE_147);
  Engine_ActorSetPosition(SEA_LANDING_SPOT, 0, 0);
  record = Object_GetById(SEA_THIRD_ATTACK_MONSTER + 2);
  Engine_ActorSetSpriteFlags(record, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
  monster = Object_GetById(ACTOR_PARTY_LEADER);
  monster->motionFlags = 0;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 216, 0x264);
  Actor_SetSpeed(SEA_THIRD_ATTACK_MONSTER + 2, 0x19999, 0xcccc);
  FieldObject_MotionSetPositionAndCommit(SEA_THIRD_ATTACK_MONSTER + 2, 196,
                                         0x258);
  FieldObject_MotionSetPositionAndCommit(SEA_THIRD_ATTACK_MONSTER + 2, 216,
                                         0x258);
  Engine_ActorStop(SEA_THIRD_ATTACK_MONSTER);
  Engine_TaskWait(1);
  Actor_SetSpeed(SEA_THIRD_ATTACK_MONSTER, 0x19999, 0xcccc);
  sailorActions = gTolbiShipDeck_SailorActions;
  Actor_SetActionScript(SEA_THIRD_ATTACK_MONSTER, sailorActions);
  TolbiShipDeck_Turn(SEA_THIRD_ATTACK_MONSTER + 2, 0xd000);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_SetActionScript(SEA_THIRD_ATTACK_MONSTER + 2, sailorActions);
  Engine_ActorStop(SEA_THIRD_ATTACK_MONSTER + 1);
  Engine_TaskWait(1);
  Actor_SetSpeed(SEA_THIRD_ATTACK_MONSTER + 1, 0x19999, 0xcccc);
  Actor_RunActionScriptAndWait(SEA_THIRD_ATTACK_MONSTER + 1, sailorActions);
  Engine_EventWait(20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_ActorStop(SEA_FIRST_LEAPING_MONSTER);
  Engine_ActorStop(SEA_FIRST_LEAPING_MONSTER + 1);
  Engine_ActorStop(SEA_FIRST_LEAPING_MONSTER + 2);
  Engine_ActorStop(SEA_FIRST_LEAPING_MONSTER + 3);
  Engine_ActorStop(SEA_THIRD_ATTACK_MONSTER);
  Engine_ActorStop(SEA_THIRD_ATTACK_MONSTER + 1);
  Engine_EventWait(10);
  ObjectSystem_SuspendProjectionAndShowSprites();
  Ui_SetMessageVoiceFromActor(SEA_OURANOS);
  UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_THIRD_ATTACK);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_KRAKEN: the party jumps down to
/// the deck, gathers and charges; the battle with the Kraken begins, returning
/// to TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE.
static void TolbiShipDeck_JumpIntoKrakenBattle(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScriptE);
  Engine_TaskWait(1);
  Engine_CameraFollowActor(SEA_CAMERA_TARGET, 1);
  Engine_TaskWait(1);
  Engine_ActorSetAnimation(SEA_OURANOS, 5);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_OURANOS), 0);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_SOUTH;
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(216), PIXELS(586));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
  Engine_EventWait(20);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
  Engine_AudioPlayCue(SE_181);
  Map_SetShake(0x40000, 0x40000, 0x10000);
  Engine_EventWait(20);
  Map_SetShake(-1, -1, 0xe666);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Object_GetById(SEA_CAMERA_TARGET)->motionFlags = 0;
  Actor_SetSpeed(SEA_CAMERA_TARGET, 0x20000, 0x10000);
  Engine_ActorSetDestination(SEA_CAMERA_TARGET, 216, 612);
  Engine_AudioPlayCue(SE_149);
  Engine_ActorSetSpritePriority(SEA_SEAN, 2);
  Engine_ActorSetAnimation(SEA_SEAN, 5);
  actor = Object_GetById(SEA_SEAN);
  actor->velocityY = 0x80000;
  actor->gravity = 0xb333;
  actor->scaleX = 0x1a000;
  actor->scaleY = 0x1a000;
  actor->update = TolbiShipDeck_ShrinkToNormalSize;
  actor->bounce = 0x8000;
  Actor_SetSpeed(SEA_SEAN, 0x60000, 0x30000);
  Engine_ObjectMotionSetPositionAndCommit(SEA_SEAN, 182, 0x26a);
  Engine_ActorSetSpriteFlags(Object_GetById(SEA_SEAN), 0);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 80);
  Actor_SetDestination(SEA_CAMERA_TARGET, 232, 564);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 250, 584);
  Actor_WalkTo(GARET, 240, 600);
  Actor_WalkTo(IVAN, 254, 600);
  Engine_ActorWalkToAndWait(MIA, 248, 616);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorFaceDirection(MIA, FACING_NORTH, 20);
  Engine_AudioPlayCue(SE_149);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_ActorSetAttachedEffect(MIA, 0x102);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_SetSpeed(GARET, 0xcccc, 0x6666);
  Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
  Actor_SetSpeed(MIA, 0xcccc, 0x6666);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 564);
  Actor_WalkTo(GARET, 248, 564);
  Engine_EventWait(20);
  Actor_WalkTo(IVAN, 248, 564);
  Actor_WalkTo(MIA, 248, 564);
  Engine_EventWait(20);
  gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
  gGameState.savedEntrance = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
  Battle_SetStart(BATTLE_START_SCENE);
  Event_SetNextDestination((s32)&SceneId_TolbiShipDeck,
                           TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 3);
  Engine_EventEnd();
}

/// TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE: Isaac is thanked for the
/// Kraken. "Thank you, Isaac. You did good against that Kraken."
static void TolbiShipDeck_ThankForKraken(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_KAJA, PIXELS(182), PIXELS(618));
  Actor_SetPosition(SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  Actor_SetPosition(SEA_SEAN, PIXELS(268), PIXELS(678));
  record = Object_GetById(SEA_SEAN);
  record->facing = 0;
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_LeadActionsC);
  {
    struct FieldActor *record = Object_GetById(SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  Actor_SetSpeed(SEA_OURANOS, 0xcccc, 0x6666);
  Actor_SetActionScript(SEA_OURANOS, gTolbiShipDeck_LeadActionsB);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetSpeed(SEA_KAJA, 0x19999, 0xcccc);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x224);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_WEST);
  Engine_ActorRunRepeatedMotion(SEA_KAJA, 1);
  Engine_EventSetMessage(MsgTolbiShipThankIsaacDidGoodAgainst);
  TolbiShipDeck_Say(SEA_KAJA);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(SEA_KAJA, 0x5000, 20);
  Actor_ShowEmote(SEA_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(SEA_KAJA, 0, 40);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Say(SEA_KAJA);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(SEA_KAJA, 3);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x258);
  Actor_WalkToAndWait(SEA_KAJA, 216, 0x258);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_WalkToAndWait(SEA_KAJA, 216, 0x244);
  Actor_SetPosition(SEA_KAJA, 0, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  Engine_EventEnd();
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED: land is sighted. "Hey, I
/// can see land!"
static void TolbiShipDeck_SightLand(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(record, 0);
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_KAJA, PIXELS(196), PIXELS(502));
  record = Object_GetById(SEA_KAJA);
  record->facing = FACING_NORTHWEST;
  Actor_SetPosition(SEA_SEAN, PIXELS(184), PIXELS(524));
  record = Object_GetById(SEA_SEAN);
  record->facing = 0xb000;
  Engine_ActorSetSpritePriority(SEA_OURANOS, 1);
  Actor_SetPosition(SEA_OURANOS, PIXELS(184), PIXELS(632));
  record = Object_GetById(SEA_OURANOS);
  record->facing = 0xb000;
  // UB: a u32 lvalue aliases the pointer, which keeps the facing stores ahead
  // of its load; the defined build reads it directly.
#ifdef UBFIX
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#else
  ((struct EventWork *)*(u32 *)&gEventWork)->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#endif
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorJump(SEA_SEAN, 4, 10);
  Engine_ActorJump(SEA_SEAN, 6, 20);
  Engine_EventSetMessage(MsgTolbiShipCanSeeLand);
  TolbiShipDeck_Say(SEA_SEAN);
  Engine_ActorSetAnimationAndWait(SEA_KAJA, 3);
  Actor_SetSpeed(SEA_OURANOS, 0x30000, 0x18000);
  Actor_WalkToAndWait(SEA_OURANOS, 180, 0x222);
  Engine_ActorFaceDirection(SEA_OURANOS, 0xb000, 40);
  Engine_ActorRunRepeatedMotion(SEA_OURANOS, 1);
  TolbiShipDeck_Say(SEA_OURANOS);
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI);
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
  s32 actor;
  s32 member;
  struct FieldActor *record;

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

  Engine_EventBegin();
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Engine_ActorSetPosition(ARRIVED_CAMERA_TARGET, record->x.fixed,
                            record->z.fixed);
  }
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_CameraFollowActor(ARRIVED_CAMERA_TARGET, 1);

  TolbiShipDeck_OpenFirstDoor();

  Engine_EventWait(10);

  for (index = 0; index <= 3; index++) {
    actor = matched[index];
    Actor_SetPosition(actor, 216 << 16, 146 << 18);
    if (category[index] == ROWER_REPLY_OLD_MAN) {
      Actor_SetSpeed(actor, 0xcccc, 0x6666);
    } else {
      Actor_SetSpeed(actor, 128 << 9, 128 << 8);
    }
    Actor_WalkToAndWait(actor, 216, 150 << 2);
    Actor_WalkToAndWait(actor, 192, 0x26a);
    Engine_ActorWalkToAndWait(actor, 192, 164 << 2);

    switch (category[index]) {
    case ROWER_REPLY_FINALLY_LUCKY:
      Engine_ActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Engine_EventSetMessage(MsgTolbiShipMadeFinallyLucky);
      break;
    case ROWER_REPLY_OTHERS_ALREADY:
      TolbiShipDeck_Turn(actor, 208 << 8);
      Engine_ActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Engine_EventSetMessage(MsgTolbiShipOtherPassengersAlready);
      break;
    case ROWER_REPLY_DONE_ROWING:
      Engine_ActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Engine_EventSetMessage(MsgTolbiShipDontRowAnymore);
      break;
    case ROWER_REPLY_NO_WORKOUT:
      Engine_ActorRunRepeatedMotion(actor, 1);
      Engine_EventSetMessage(MsgTolbiShipOhhhHaventWorkout);
      break;
    case ROWER_REPLY_ROWING_MORE:
      Engine_ActorSetAnimationAndWait(actor, 3);
      Engine_EventSetMessage(MsgTolbiShipRowingShipMore);
      break;
    case ROWER_REPLY_LOST_AT_SEA:
      Engine_ActorSetAnimation(actor, 4);
      Engine_EventSetMessage(MsgTolbiShipTotallyLostOcean);
      break;
    case ROWER_REPLY_OLD_MAN:
      Engine_ActorSetAnimationAndWait(actor, 4);
      Engine_ActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
      Engine_EventSetMessage(MsgTolbiShipMakeOldMan);
      break;
    default:
      break;
    }

    TolbiShipDeck_Say(actor);
    Actor_SetActionScript(actor, gTolbiShipDeck_RosterActions);
  }

  Object_RefreshSelectorById(actor);
  Engine_EventWait(40);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 216 << 16, 146 << 18);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 150 << 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 190, 153 << 2);
  Engine_EventSetMessage(MsgTolbiShipThanksHardWork);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 192 << 8, 0);

  Object_GetById(ARRIVED_CAMERA_TARGET)->motionFlags = 0;
  Actor_SetSpeed(ARRIVED_CAMERA_TARGET, 128 << 10, 128 << 9);
  Actor_SetDestination(ARRIVED_CAMERA_TARGET, 196, 141 << 2);

  Actor_SetSpeed(ARRIVED_KAJA, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ARRIVED_KAJA, 182, 0x22b);
  Actor_FaceDirection(ARRIVED_KAJA, 192 << 6, 20);
  Engine_ActorRunRepeatedMotion(ARRIVED_KAJA, 1);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ActorSetAnimationAndWait(ARRIVED_KAJA, 3);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ActorFaceDirection(ARRIVED_KAJA, 128 << 8, 40);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  TolbiShipDeck_Turn(ARRIVED_KAJA, 192 << 6);
  TolbiShipDeck_Say(ARRIVED_KAJA);
  Engine_ActorSetAnimationAndWait(ARRIVED_KAJA, 3);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ARRIVED_KAJA, 188, 128 << 2);
  Engine_ActorFaceDirection(ARRIVED_KAJA, 192 << 6, 0);
  Engine_ActorSetPosition(ARRIVED_CAMERA_TARGET, 0, 0);

  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
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
  struct MapScrollWork *map = gMapWork[0];
  s32 *camera = map->origin;
  s32 dx = Engine_MathCos(sSwayAngleX);
  s32 dy = Engine_MathSin(sSwayAngleY);
  struct MapLayerScroll *layer;

  *camera++ += dx >> 1;
  *camera += dy;
  sSwayAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
  sSwayAngleY += (u32)(Engine_RandomNext() << 9) >> 16;
  sSwayAngleX &= 0xffff;
  sSwayAngleY &= 0xffff;
  layer = &map->layers[MAP_LAYER_GROUND];
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

/// Under sail: shows the stroke oars (ACTOR_FIRST_STROKE_OAR) and the oars at
/// the sides (ACTOR_FIRST_OAR, the right-hand ones mirrored), records each
/// side oar's resting depth and chooses their strokes.
static void TolbiShipDeck_SetUpOars(void) {
  Object_GetById(ACTOR_FIRST_STROKE_OAR)->collisionFlags = 0;
  Object_GetById(ACTOR_FIRST_STROKE_OAR + 1)->collisionFlags = 0;
  Object_GetById(ACTOR_FIRST_STROKE_OAR + 2)->collisionFlags = 0;
  Object_GetById(ACTOR_FIRST_STROKE_OAR + 3)->collisionFlags = 0;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 3);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 3);
  sOarDepth[0] = Object_GetById(ACTOR_FIRST_OAR)->z.fixed;
  sOarDepth[1] = Object_GetById(ACTOR_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = Object_GetById(ACTOR_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = Object_GetById(ACTOR_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 7);
  Object_GetById(ACTOR_FIRST_OAR + 4)->scaleX = 0xffff0000;
  Object_GetById(ACTOR_FIRST_OAR + 5)->scaleX = 0xffff0000;
  Object_GetById(ACTOR_FIRST_OAR + 6)->scaleX = 0xffff0000;
  Object_GetById(ACTOR_FIRST_OAR + 7)->scaleX = 0xffff0000;
  sOarDepth[4] = Object_GetById(ACTOR_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = Object_GetById(ACTOR_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = Object_GetById(ACTOR_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = Object_GetById(ACTOR_FIRST_OAR + 7)->z.fixed;
  TolbiShipDeck_ChooseOarStrokes();
}

/// Shows an oar drawn under the deck, with no collision.
static void TolbiShipDeck_ShowOar(s32 a) {
  struct FieldActor *p = Object_GetById(a);

  if (p != 0) {
    Engine_ActorSetSpritePriority(a, 3);
    Engine_ActorSetSpriteFlags(p, 0);
    p->collisionFlags = 0;
    {
      s32 c;
      c = 2 | p->priorityFlags;
      p->priorityFlags = c;
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
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR, 0);
  sStrokePhase[0] = 0;
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR + 1, 1);
  sStrokePhase[1] += 0x80;
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR + 2, 2);
  sStrokePhase[2] += 0x100;
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR + 3, 3);
  sStrokePhase[3] += 0x200;
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR, 0, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 1, 1, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 2, 2, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 3, 3, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 4, 4, 1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 5, 5, 1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 6, 6, 1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 7, 7, 1);
}

/// Advances one stroke's phase: slowly through the catch and the finish of the
/// stroke, quicker through the drive, quickest on the return; its stroke oar's
/// animation follows.
static void TolbiShipDeck_AdvanceStroke(s32 actor, s32 slot) {
  u16 value = sOarAngle[slot];

  if (value >= 0x6801 && value <= 0x6fff) {
    sStrokePhase[slot] += 0x70;
    Engine_ActorSetAnimation(actor, 3);
  } else if (value >= 0xe801 && value <= 0xefff) {
    sStrokePhase[slot] += 0xe0;
    Engine_ActorSetAnimation(actor, 3);
  } else if (value >= 0x7001 && value <= 0xefff) {
    sStrokePhase[slot] += 0x1c0;
    Engine_ActorSetAnimation(actor, 2);
  } else {
    sStrokePhase[slot] += 0x300;
    Engine_ActorSetAnimation(actor, 1);
  }
}

/// Swings one oar to its angle: unless flags bit 1 is set, the angle follows
/// the stroke oar whose stroke the oar takes; bit 0 marks a right-hand oar. The
/// oar's tilt (its sprite's rotation) and depth follow the angle.
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags) {
  struct FieldActor *actor = Object_GetById(id);
  struct AnimationObject *sprite = actor->sprite;
  s32 sine;

  if ((flags & 2) == 0) {
    switch (sOarStroke[slot]) {
    case 1:
      sOarAngle[slot] = sStrokePhase[0];
      ObjectVisual_CopyAttributes(id, ACTOR_FIRST_STROKE_OAR);
      break;
    case 2:
      sOarAngle[slot] = sStrokePhase[1];
      ObjectVisual_CopyAttributes(id, ACTOR_FIRST_STROKE_OAR + 1);
      break;
    case 3:
      sOarAngle[slot] = sStrokePhase[2];
      ObjectVisual_CopyAttributes(id, ACTOR_FIRST_STROKE_OAR + 2);
      break;
    case 4:
      sOarAngle[slot] = sStrokePhase[3];
      ObjectVisual_CopyAttributes(id, ACTOR_FIRST_STROKE_OAR + 3);
      break;
    }
  }
  if (flags & 1) {
    sine = Engine_MathSin(sOarAngle[slot]);
    sprite->rotation = Engine_MathSin(sOarAngle[slot] + 0x8000) >> 5;
    actor->z.fixed = sOarDepth[slot] - (sine << 2) - (sine << 1);
  } else {
    sine = Engine_MathSin(sOarAngle[slot] + 0x8000);
    sprite->rotation = Engine_MathSin(sOarAngle[slot]) >> 5;
    actor->z.fixed = sOarDepth[slot] + (sine << 2) + (sine << 1);
  }
}

/// When not under sail: the oars lie at rest, all copying the first stroke oar,
/// which stands where the leader is while they copy it and then takes the first
/// oar's place.
static void TolbiShipDeck_RestOars(void) {
  struct FieldActor *actor;
  u32 i;

  for (i = 0; i <= 7; i++)
    sOarAngle[i] = 0xc000;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR);
  Engine_ActorSetPosition(ACTOR_FIRST_STROKE_OAR + 1, 0, 0);
  Engine_ActorSetPosition(ACTOR_FIRST_STROKE_OAR + 2, 0, 0);
  Engine_ActorSetPosition(ACTOR_FIRST_STROKE_OAR + 3, 0, 0);
  Engine_ActorSetPosition(ACTOR_FIRST_OAR, 0, 0);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 3);
  sOarStroke[0] = 0;
  sOarStroke[1] = 0;
  sOarStroke[2] = 0;
  sOarStroke[3] = 0;
  sOarDepth[0] = Object_GetById(ACTOR_FIRST_STROKE_OAR)->z.fixed;
  sOarDepth[1] = Object_GetById(ACTOR_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = Object_GetById(ACTOR_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = Object_GetById(ACTOR_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 7);
  Object_GetById(ACTOR_FIRST_OAR + 4)->scaleX = 0xffff0000;
  Object_GetById(ACTOR_FIRST_OAR + 5)->scaleX = 0xffff0000;
  Object_GetById(ACTOR_FIRST_OAR + 6)->scaleX = 0xffff0000;
  Object_GetById(ACTOR_FIRST_OAR + 7)->scaleX = 0xffff0000;
  sOarStroke[4] = 0;
  sOarStroke[5] = 0;
  sOarStroke[6] = 0;
  sOarStroke[7] = 0;
  sOarDepth[4] = Object_GetById(ACTOR_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = Object_GetById(ACTOR_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = Object_GetById(ACTOR_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = Object_GetById(ACTOR_FIRST_OAR + 7)->z.fixed;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL)
    Engine_ActorSetPosition(ACTOR_FIRST_STROKE_OAR, actor->x.fixed,
                            actor->z.fixed);
  Engine_TaskWait(1);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 1, ACTOR_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 2, ACTOR_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 3, ACTOR_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 4, ACTOR_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 5, ACTOR_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 6, ACTOR_FIRST_STROKE_OAR);
  ObjectVisual_CopyAttributes(ACTOR_FIRST_OAR + 7, ACTOR_FIRST_STROKE_OAR);
  Object_GetById(ACTOR_FIRST_STROKE_OAR)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 1)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 2)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 3)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 4)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 5)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 6)->retainSpriteCache = 1;
  Object_GetById(ACTOR_FIRST_OAR + 7)->retainSpriteCache = 1;
  Engine_TaskWait(1);
  Engine_ActorSetPosition(ACTOR_FIRST_STROKE_OAR, PIXELS(132), PIXELS(632));
  Engine_TaskWait(1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_STROKE_OAR, 0, 2);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 1, 1, 2);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 2, 2, 2);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 3, 3, 2);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 4, 4, 3);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 5, 5, 3);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 6, 6, 3);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 7, 7, 3);
}

/// Redraws a block of the deck's cells and attributes, as it is whenever the
/// ship has left the Kalay dock.
static void TolbiShipDeck_CopyUnderwayCells(void) {
  Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
  Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
  Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
  Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
  {

    Map_CopyCellAttributes(17, 38, 5, 2, 17, 40);
  }
}

/// Redraws two more blocks of cells, as they are at anchor.
static void TolbiShipDeck_CopyAnchoredCells(void) {
  Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
  Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

/// Shows the current message from a speaker and waits 10 frames.
static void TolbiShipDeck_Say(s32 a) {
  Engine_EventShowMessage(a, 0);
  Engine_EventWait(10);
}

/// Turns an actor to a facing over 10 frames.
static void TolbiShipDeck_Turn(s32 a, s32 b) {
  Engine_ActorFaceDirection(a, b, 10);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, after the lookout's call:
/// Ouranos thinks they have reached Tolbi, but Kaja sees the coast is not
/// Tolbi's and the ship is off course, and the crew head for an island to get
/// their bearings before Kaja goes below.
/// TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND leads on below decks, toward
/// the island.
static void TolbiShipDeck_SightIsland(void) {
  Engine_EventBegin();
  sSeaScroll[0] = 0x40000;
  sSeaSpeed[0] = -0x8000;
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_OURANOS, PIXELS(182), PIXELS(618));
  Actor_Get(SEA_OURANOS)->facing = FACING_NORTH;
  Actor_SetPosition(SEA_KAJA, PIXELS(218), PIXELS(516));
  Actor_Get(SEA_KAJA)->facing = 0xb000;
  Actor_SetPosition(SEA_SEAN, PIXELS(204), PIXELS(526));
  Actor_Get(SEA_SEAN)->facing = 0xb000;
  Actor_SetPosition(SEA_CREW_MEMBER, 0, 0);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(SEA_OURANOS, 0xcccc, 0x6666);
  Actor_WalkToAndWait(SEA_OURANOS, 182, 0x214);
  Actor_FaceDirection(SEA_OURANOS, 0xb000, 40);
  Engine_EventSetMessage(MsgTolbiShipFinallyReachedTolbi);
  TolbiShipDeck_Say(SEA_OURANOS);
  Engine_ActorRunRepeatedMotion(SEA_KAJA, 2);
  Engine_ActorSetAnimation(SEA_KAJA, 4);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 6));
  Actor_FaceDirection(SEA_OURANOS, 0xd000, 0);
  Actor_FaceDirection(SEA_SEAN, 0xd000, 0);
  Actor_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorSetAnimation(SEA_KAJA, 3);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 6));
  Actor_SetAttachedEffect(SEA_OURANOS, 0x102);
  Actor_SetAttachedEffect(SEA_SEAN, 0x102);
  Engine_EventWait(80);
  Actor_ShowEmote(SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(SEA_OURANOS, 0x19999, 0xcccc);
  Actor_WalkToAndWait(SEA_OURANOS, 194, 0x1f4);
  Actor_FaceDirection(SEA_OURANOS, 0xb000, 20);
  Actor_SetSpeed(SEA_SEAN, 0xcccc, 0x6666);
  Actor_WalkToAndWait(SEA_SEAN, 192, 0x206);
  Actor_FaceDirection(SEA_SEAN, 0xb000, 0);
  Actor_SetSpeed(SEA_KAJA, 0x10000, 0x8000);
  Actor_WalkToAndWait(SEA_KAJA, 210, 0x1fc);
  TolbiShipDeck_Turn(SEA_KAJA, 0xb000);
  Engine_ActorRunRepeatedMotion(SEA_OURANOS, 1);
  TolbiShipDeck_Say(SPEAKER(SEA_OURANOS, 5));
  Engine_ActorSetAnimationAndWait(SEA_KAJA, 3);
  TolbiShipDeck_Turn(SEA_SEAN, 0xd000);
  TolbiShipDeck_Say(SPEAKER(SEA_SEAN, 9));
  Actor_FaceDirection(SEA_KAJA, FACING_WEST, 20);
  Engine_ActorSetAnimation(SEA_KAJA, 4);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 10));
  Actor_WalkToAndWait(SEA_KAJA, 204, 0x218);
  Actor_FaceDirection(SEA_SEAN, 0xb000, 0);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x224);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x250);
  Actor_WalkTo(SEA_KAJA, 182, 0x298);
  Engine_EventWait(40);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND);
}

/// At anchor off the island, talking to Sean or Ouranos: the party is asked to
/// scout the island for the arguing tour group; Garet and Ivan may argue
/// before the party agrees (FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED).
static void TolbiShipDeck_OfferToScoutIsland(void) {
  struct FieldActor *leader;
  u8 bits;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_WalkTo(GARET, 194, 0x280);
  Actor_WalkTo(IVAN, 198, 0x28e);
  Actor_WalkToAndWait(MIA, 194, 0x2a0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  TolbiShipDeck_Turn(MIA, FACING_WEST);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_EAST);
  Engine_EventSetMessage(MsgTolbiShipIsaacTalkedPassengersDidntTour);
  TolbiShipDeck_Say(SEA_SEAN);
  TolbiShipDeck_Turn(SEA_OURANOS, 0xd000);
  Event_ShowMessageAndWait(SEA_OURANOS, 0, 40);
  Actor_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Engine_ActorRunRepeatedMotion(SEA_SEAN, 1);
  Event_OpenMessage(SEA_SEAN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    TolbiShipDeck_Say(IVAN);
    TolbiShipDeck_Turn(MIA, FACING_NORTHWEST);
    Engine_ActorSetAnimation(MIA, ANIM_NOD);
    TolbiShipDeck_Say(MIA);
    TolbiShipDeck_Turn(GARET, FACING_SOUTHWEST);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Event_OpenMessage(GARET, 0);
    for (;;) {
      if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_ActorRunRepeatedMotion(IVAN, 1);
        Engine_EventSetMessage(MsgTolbiShipIsaacDontTalkLikeShouldnt);
        Event_OpenMessage(IVAN, 0);
      } else
        break;
    }
  }
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(SEA_SEAN, 3);
  Engine_EventSetMessage(MsgTolbiShipSeeYoureGoingGoFor);
  TolbiShipDeck_Say(SEA_SEAN);
  Actor_SetSpeed(SEA_SEAN, 0x10000, 0x8000);
  Actor_SetSpeed(SEA_OURANOS, 0x10000, 0x8000);
  Object_GetById(SEA_SEAN)->steeringFlags &= 254;
  Actor_WalkToAndWait(SEA_SEAN, 162, 0x27a);
  Engine_EventWait(1);
  bits = ACTOR_FACES_MOTION;
  {
    struct FieldActor *actor = Object_GetById(SEA_SEAN);
    u8 value = actor->steeringFlags;

    actor->steeringFlags = value | bits;
  }
  Object_GetById(SEA_OURANOS)->steeringFlags &= 254;
  Actor_WalkToAndWait(SEA_OURANOS, 162, 0x2a4);
  Engine_EventWait(1);
  {
    struct FieldActor *actor = Object_GetById(SEA_OURANOS);

    bits |= actor->steeringFlags;
    actor->steeringFlags = bits;
  }
  Actor_FaceDirection(SEA_SEAN, 0x3000, 0);
  TolbiShipDeck_Turn(SEA_OURANOS, 0xd000);
  TolbiShipDeck_Say(SEA_SEAN);
  Actor_WalkTo(GARET, 180, 0x28e);
  Actor_WalkTo(IVAN, 180, 0x28e);
  Actor_WalkToAndWait(MIA, 180, 0x28e);
  Engine_ActorDestroy(GARET);
  Engine_ActorDestroy(IVAN);
  Engine_ActorDestroy(MIA);
  GameFlag_Set(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED);
  Engine_EventEnd();
}

/// Back on deck from the island: on yes, the crew make ready to sail and the
/// scene moves on. "How was it, Isaac? Did you explore the island
/// thoroughly?"
static void TolbiShipDeck_ReturnFromIsland(void) {
  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(
      (struct ScenePlacement *)gTolbiShipDeck_ClosingCrewScript);
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
  Actor_ShowEmote(SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorRunRepeatedMotion(SEA_SEAN, 1);
  TolbiShipDeck_Turn(SEA_SEAN, 0x5000);
  Engine_EventSetMessage(MsgTolbiShipHowWasIsaacDidExplore);
  Event_OpenMessage(SPEAKER(SEA_SEAN, 2), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    TolbiShipDeck_Say(SPEAKER(SEA_SEAN, 2));
    Engine_EventEnd();
    return;
  }
  gEventWork->message++;
  Event_AskYesNo(SPEAKER(SEA_SEAN, 2), 0);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(ANCHOR_TOUR_GUIDE, PIXELS(216), PIXELS(588));
  Actor_SetSpeed(ANCHOR_TOUR_GUIDE, 0x13333, 0x9999);
  Actor_WalkToAndWait(ANCHOR_TOUR_GUIDE, 216, 0x254);
  Actor_WalkToAndWait(ANCHOR_TOUR_GUIDE, 188, 0x268);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(SEA_OURANOS, 0xd000, 0);
  Actor_FaceDirection(SEA_SEAN, 0xd000, 0);
  TolbiShipDeck_Turn(ANCHOR_TOUR_GUIDE, 0x5000);
  Engine_ActorJump(ANCHOR_TOUR_GUIDE, 2, 0);
  Engine_ActorSetAnimation(ANCHOR_TOUR_GUIDE, 4);
  Event_ShowMessage(ANCHOR_TOUR_GUIDE, 0);
  Actor_SetPosition(SEA_KAJA, PIXELS(180), PIXELS(777));
  Actor_SetSpeed(SEA_KAJA, 0x10000, 0x8000);
  Actor_WalkToAndWait(SEA_KAJA, 180, 0x298);
  Actor_FaceDirection(SEA_KAJA, 0xd000, 0);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 2));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(SEA_SEAN, 0x3000, 0);
  Actor_ShowEmote(ANCHOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorRunRepeatedMotion(SEA_KAJA, 1);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 2));
  Engine_ActorStartRepeatedMotion(SEA_OURANOS, 2);
  TolbiShipDeck_Say(SEA_OURANOS);
  Actor_FaceDirection(SEA_KAJA, 0x5000, 20);
  Engine_ActorSetAnimation(SEA_KAJA, 3);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 6));
  Engine_ActorJump(ANCHOR_TOUR_GUIDE, 2, 20);
  Engine_ActorSetAnimation(ANCHOR_TOUR_GUIDE, 4);
  TolbiShipDeck_Say(ANCHOR_TOUR_GUIDE);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x280);
  Actor_FaceDirection(SEA_KAJA, 0xd000, 0);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 8));
  Actor_ShowEmote(ANCHOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Engine_ActorStartRepeatedMotion(ANCHOR_TOUR_GUIDE, 2);
  TolbiShipDeck_Say(ANCHOR_TOUR_GUIDE);
  Engine_ActorSetAnimationAndWait(SEA_KAJA, 3);
  TolbiShipDeck_Turn(SEA_SEAN, FACING_EAST);
  Engine_ActorRunRepeatedMotion(SEA_SEAN, 1);
  TolbiShipDeck_Say(SEA_SEAN);
  Actor_SetSpeed(SEA_SEAN, 0x19999, 0xcccc);
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_LeaveActions);
  Actor_SetSpeed(SEA_OURANOS, 0x19999, 0xcccc);
  Actor_WalkToAndWait(SEA_OURANOS, 168, 0x278);
  Actor_SetActionScript(SEA_OURANOS, gTolbiShipDeck_LeaveActions);
  Engine_EventWait(80);
  Actor_SetActionScript(ANCHOR_TOUR_GUIDE, gTolbiShipDeck_LeaveActions);
  Engine_EventWait(40);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_WEST);
  TolbiShipDeck_Say(SPEAKER(SEA_KAJA, 2));
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHEAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(SEA_KAJA, 3);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SWITCH_TEAMS);
}

/// Places the leader on the deck facing south, with the camera on them.
static void TolbiShipDeck_PlaceLeaderOnDeck(void) {
  struct FieldActor *record;

  Camera_MoveTo(PIXELS(232), -1, PIXELS(676), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(676));
  record = Object_GetById(ACTOR_PARTY_LEADER);
  record->facing = FACING_SOUTH;
  Engine_TaskWait(1);
}

const s32 gTolbiShipDeck_PresentationActionsA[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(202), 0, PIXELS(600)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

const s32 gTolbiShipDeck_PresentationActionsB[] = {
    SC_WAIT(20), SC_ARG(2), SC_MOVE(PIXELS(162), 0, PIXELS(666)),
    SC_SYNC,     SC_ARG(1), SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

const s32 gTolbiShipDeck_RandomActorActions[] = {
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_DeckEventActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(140), 0, PIXELS(586)),
    SC_SYNC,
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, 0x80000),
    SC_MOVE(PIXELS(184), 0, PIXELS(598)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_SET(SW_VELOCITY_Y, 0x40000),
    SC_CALL(TolbiShipDeck_ShowSprite),
    SC_MOVE(PIXELS(216), 0, PIXELS(596)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

const s32 gTolbiShipDeck_LeadActionsA[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(228), 0, PIXELS(521)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(40),
    SC_ARG(4),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(204), 0, PIXELS(521)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(40),
    SC_ARG(4),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_LeadActionsB[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(672)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(70),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(672)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(30),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(70),
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(636)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, 0x5000),
    SC_WAIT(30),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(636)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(70),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_CrewActionsA[] = {
    SC_SET(SW_MOTION, 0), SC_SET(0x23, 0),
    SC_SET(0x24, 0),      SC_SET(0x27, 0),
    SC_SET(0x17, 0),      SC_CALL(TolbiShipDeck_HoverAndDiveAtOuranos),
};

const s32 gTolbiShipDeck_CrewActionsB[] = {
    SC_SET(SW_MOTION, 0), SC_SET(0x23, 0),
    SC_SET(0x24, 0),      SC_SET(0x27, 0),
    SC_SET(0x17, 0),      SC_CALL(TolbiShipDeck_HoverAndDiveAtSean),
};

const s32 gTolbiShipDeck_CrewActionsC[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(TolbiShipDeck_Bob),
};

const s32 gTolbiShipDeck_CrewActionsD[] = {
    SC_CALL(TolbiShipDeck_LookAround),
};

const s32 gTolbiShipDeck_CrewActionsE[] = {
    SC_ARG(1),
    SC_SET(SW_SCALE_X, 0xb333),
    SC_SET(SW_SCALE_Y, 0xb333),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 0x147a),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

const s32 gTolbiShipDeck_SailorActions[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

const s32 gTolbiShipDeck_DeckActionsB[] = {
    SC_SET(0x23, 0),
    SC_CALL(TolbiShipDeck_LeapAtOuranos),
};

const s32 gTolbiShipDeck_DeckActionsA[] = {
    SC_SET(0x23, 0),
    SC_CALL(TolbiShipDeck_LeapAtSean),
};

const s32 gTolbiShipDeck_DeckActionsC[] = {
    SC_CALL(TolbiShipDeck_RandomlyStartActorHop),
};

const s32 gTolbiShipDeck_RosterActions[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(82), 0, PIXELS(656)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

const s32 gTolbiShipDeck_LeaveActions[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(186), 0, PIXELS(604)),
    SC_SYNC,         SC_MOVE(PIXELS(216), 0, PIXELS(604)),
    SC_SYNC,         SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

const s32 gTolbiShipDeck_LeadActionsC[] = {
    SC_SET(0x27, 0),
    SC_CALL(TolbiShipDeck_IdleTurn),
};

const struct SceneEntrance gTolbiShipDeckEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 179, 0, 653, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, CONDITION_ALWAYS,
     216, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CARGO_HOLD, CONDITION_ALWAYS,
     232, 0, 664, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN, CONDITION_ALWAYS,
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
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED, CONDITION_ALWAYS, 232,
     0, 654, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_MONSTER_LANDS, CONDITION_ALWAYS, 216,
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
    {SCENE_TABLE_END},
};

// clang-format off
const struct SceneRegion gTolbiShipDeckRegions[] = {
    {208, 24, 634, 224, 40, 650, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST},
    {SCENE_TABLE_END},
};

const u32 gTolbiShipDeckExits[] = {
    SCENE_EXITS(TolbiShipDeck),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_PASSENGER_HOLD, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CARGO_HOLD, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CARGO_HOLD_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_KALAY_DOCKS, KalayDocks, KALAY_DOCKS_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST, TolbiShipMast, TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_DOCKS, TolbiDocks, TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE, WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR, TolbiShipMast, TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_TURN, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN_FROM_TOLBI_SHIP_DECK_TURN),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_FIRST_ATTACK, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SECOND_ATTACK, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_THIRD_ATTACK, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI, WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_ISLAND, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN_FROM_TOLBI_SHIP_DECK_ISLAND),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_INTERIORS_SWITCH_TEAMS, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS),
    SCENE_EXITS_END,
};

const struct ScenePlacement gTolbiShipDeckPlacements[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(266), 0, PIXELS(703), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

// UB: the matching compiler places this writable overlay table with the
// constants. The modern build gives its talkFacing stores a mutable object.
#ifdef UBFIX
struct ScenePlacement gTolbiShipDeckPlacementsWaiting[] = {
#else
const struct ScenePlacement gTolbiShipDeckPlacementsWaiting[] = {
#endif
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(183), 0, PIXELS(720), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(712), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(198), 0, PIXELS(706), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(172), 0, PIXELS(706), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(230), 0, PIXELS(602), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(236), 0, PIXELS(621), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(556), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsRowing[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsAtSea[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScript[] = {
    { SPRITE_KAJA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(512), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(634), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(521), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_ClosingCrewScript[] = {
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptB[] = {
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptC[] = {
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptD[] = {
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc5, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptE[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_KAJA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(178), 0, PIXELS(642), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(534), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc2, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(540), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(640), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsArrived[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_KAJA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(188), 0, PIXELS(512), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(162), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(162), 0, PIXELS(676), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gTolbiShipDeckEvents[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_KALAY_DOCKS },
    { EVENT_TALK, KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckThereAreALotOfPassengers) },
    { EVENT_TALK, KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckOhAtTheRiskOfSounding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckTheReplacementShipFromTolbiShould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIHidTheAnchorCharmNo) },
    { EVENT_TALK, KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeApologizeForAnyTroubleThat) },
    { EVENT_TALK, KALAY_CREW_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_TalkToCrewMember) },
    { EVENT_TALK, KALAY_TOUR_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_TalkToTourGuide) },
    { EVENT_TALK, KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderWhatsGoingOnWeve) },
    { EVENT_TALK, KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseWarriorsGotTiredOfWaiting) },
    { EVENT_TALK, KALAY_PASSENGER_WAITING_LONG, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIfWeJustPromiseToHelp) },
    { EVENT_TALK, KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderIfImTheOnly) },
    { EVENT_TALK, KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckIWonderIfThoseTwoAre) },
    { EVENT_TALK, KALAY_PASSENGER_SEEING_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIllAgreeToAnythingEvenRowing) },
    { EVENT_TALK, KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolIsLikeAHumongous) },
    { EVENT_TALK, KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHaHahThoseMercenariesAreGoing) },
    { EVENT_TALK, KALAY_PASSENGER_KARAGOL, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThatSeaDogKajaIsA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThereIsNothingToFearAs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_CREW_MEMBER, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckWhenAreWeGoingWhenAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_CREW_MEMBER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckUsingForceToLaunchTheShip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAreYouSeriousAboutSettingSail) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_TOUR_GUIDE, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckHmmmThisIsSoFrustratingWhy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_TOUR_GUIDE, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckTheyWereJustAboutToLaunch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItProbablyWontMakeAnyDifference) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTravelingByShipIsFasterThan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseThugsAreTooImpatientTrying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_WAITING_LONG, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUltimatelyTheyreJustBeingThreatenedInto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckMostOfThePeopleGoingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHeeHeeIfWeCanGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_SEEING_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckBesidesRowingIsAMansJob) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolSeaIsPrettyFoggy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseGuysSeemPrettySureOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), KALAY_PASSENGER_KARAGOL, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUsHelpWithTheRowingGet) },
    { EVENT_TOUCH, 10, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(TolbiShipDeck_WatchCrewDecideToSail) },
    { EVENT_TOUCH, 11, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT_SCRIPT(TolbiShipDeck_CatchCrewmanFromEast) },
    { EVENT_TOUCH, 12, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT_SCRIPT(TolbiShipDeck_CatchCrewmanFromWest) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsAtSea[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_TALK, SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering) },
    { EVENT_TALK, SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItLooksLikeAnotherOarsmanWas) },
    { EVENT_TALK, SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIFeelSorryForTheOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAnOarsmanWasInjuredByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeHaveNoIdeaWhenThose) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUhnnnWhenILookAtThe) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsArrived[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_DOCKS },
    { EVENT_TALK, ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouLookinForSeanAndOuranos) },
    { EVENT_TALK, ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_AskHeadedForColosso) },
    { EVENT_TALK, ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisGroupThatCameOnOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantSeanAndOuranosTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckImJustGoingToHangOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWonderWhatHappenedToThat) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsAtIsland[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE },
    { EVENT_TALK, SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckKajaAndTheCaptainAreProfessionals) },
    { EVENT_TALK, SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT_SCRIPT(TolbiShipDeck_OfferToScoutIsland) },
    { EVENT_TALK, SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckTheresNoTellingWhatKindOf) },
    { EVENT_TALK, SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT_SCRIPT(TolbiShipDeck_OfferToScoutIsland) },
    { EVENT_TALK, SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeveDoneAGoodJobOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantToBeASailor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckIGiveUpWhatAreWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisIslandGivesMeTheCreeps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckThatsEnoughBickering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWhatWeCanDoNowIs) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// How hard each passenger rows, from ARRIVED_FIRST_PASSENGER on: 1 leaves the
/// oar still, 2 to 4 row ever faster.
const s32 gTolbiShipDeckRowerStrokes[] = {3, 2, 3, 1, 4, 4, 2, 1, 1};
