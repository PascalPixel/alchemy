/// @file
/// Tolbi-bound Ship (SceneId_TolbiShipDeck), its deck: the ship that carries
/// the party and a tour group bound for Colosso across the Karagol Sea from
/// Kalay to Tolbi.
///
/// The ship's other scenes are its mast (tolbi_ship_mast.c) and below decks
/// (ship_rooms.c).
///
/// The voyage's stage, read from its flags, chooses the deck's actor
/// placements and events. The stages are:
///
/// - at Kalay,
/// - under sail once the first attack is fought off
///   (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF),
/// - while the oars move (FLAG_TOLBI_SHIP_ROWING),
/// - at anchor off Crossbone Isle (FLAG_TOLBI_SHIP_AT_ISLAND),
/// - and at Tolbi (FLAG_TOLBI_SHIP_REACHED_TOLBI).
///
/// The setup redraws the deck's cells for the stage. While under sail it
/// rocks the view and scrolls the sea. It rows the oars to the strokes of the
/// passengers at the seats (gTolbiShipDeckRowerStrokes), and plays the event
/// its entrance names.
///
/// Each event leaves into the next, in another of the ship's scenes or on the
/// World Map, whose entrances sail the ship on. In story order:
///
/// - At Kalay (TOLBI_SHIP_DECK_FROM_KALAY_DOCKS), while the ship waits for
///   its replacement from Tolbi. Once the tour group has left Kalay
///   (FLAG_KALAY_COLOSSO_TOUR_LEFT), walking up to the crew
///   (CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW) plays the captain, Kaja and a
///   crewman talking over the delay. The captain will sail trusting his anchor
///   charm, and the crewman slips below
///   (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY). Coming up to his door brings him
///   out denying everything (FLAG_TOLBI_SHIP_CREWMAN_CAUGHT), and Mind Read
///   shows he hid the charm on the mast. Once it is back
///   (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) the passengers who promised to row
///   stand on deck.
/// - Setting sail (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY): Kaja
///   brings word that all is ready ("Preparations... Ready!"), which leads
///   to the lookout on the mast.
/// - The attacks: sea monsters are sighted
///   (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS), which leads to
///   the captain's quarters, where the captain turns the ship. Then monsters
///   leap aboard (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD), harry
///   Ouranos and Sean, the warriors bound for Colosso who guard the deck with
///   Kaja (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK), and drop onto
///   the deck (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK). Each of
///   these three leads below to the fight among the oars. At
///   TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN the party jumps down to
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
///   leads ashore to Tolbi Docks (SceneId_TolbiDocks).

#include "ability_ids.h"
#include "macros.h"
#include "sounds.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/tolbi_ship.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

extern const s32 gTolbiShipDeck_KajaLeavesActions[];
extern const s32 gTolbiShipDeck_CaptainStepsAsideActions[];
extern const s32 gTolbiShipDeck_SquashAndStretchActions[];
extern const s32 gTolbiShipDeck_MonsterLeapsAboardActions[];
extern const s32 gTolbiShipDeck_SeanPacesActions[];
extern const s32 gTolbiShipDeck_OuranosPatrolsActions[];
extern const s32 gTolbiShipDeck_HoverAndDiveAtOuranosActions[];
extern const s32 gTolbiShipDeck_HoverAndDiveAtSeanActions[];
extern const s32 gTolbiShipDeck_BobActions[];
extern const s32 gTolbiShipDeck_LookAroundActions[];
extern const s32 gTolbiShipDeck_SwellActions[];
extern const s32 gTolbiShipDeck_MonsterGoesBelowActions[];
extern const s32 gTolbiShipDeck_LeapAtOuranosActions[];
extern const s32 gTolbiShipDeck_LeapAtSeanActions[];
extern const s32 gTolbiShipDeck_HopActions[];
extern const s32 gTolbiShipDeck_RowerLeavesActions[];
extern const s32 gTolbiShipDeck_GoBelowActions[];
extern const s32 gTolbiShipDeck_SeanLooksAboutActions[];
extern const struct SceneEntrance gTolbiShipDeckEntrances[];
extern const struct SceneExitRegion gTolbiShipDeckRegions[];
extern const u32 gTolbiShipDeckExits[];
extern const struct ActorPlacement gTolbiShipDeckActorPlacements[];
#ifdef UBFIX
extern struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[];
#else
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[];
#endif
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsRowing[];
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsAtSea[];
extern const struct ActorPlacement gTolbiShipDeck_CrewActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_TourGuideActorPlacements[];
extern const struct ActorPlacement
    gTolbiShipDeck_MonstersBoardActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_SecondAttackActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_ThirdAttackActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_KrakenActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsArrived[];
extern const struct Trigger gTolbiShipDeckTriggers[];
extern const struct Trigger gTolbiShipDeckTriggersAtSea[];
extern const struct Trigger gTolbiShipDeckTriggersArrived[];
extern const struct Trigger gTolbiShipDeckTriggersAtIsland[];
extern const s32 gTolbiShipDeckRowerStrokes[];

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
static void TolbiShipDeck_ShowOar(s32 actorId);
static void TolbiShipDeck_ChooseOarStrokes(void);
static s32 TolbiShipDeck_GetSeatStroke(u32 seat);
static void TolbiShipDeck_RowOars(void);
static void TolbiShipDeck_AdvanceStroke(s32 actorId, s32 slot);
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags);
static void TolbiShipDeck_RestOars(void);
static void TolbiShipDeck_CopyUnderwayCells(void);
static void TolbiShipDeck_CopyAnchoredCells(void);
static void TolbiShipDeck_Say(s32 speaker);
static void TolbiShipDeck_Turn(s32 actorId, s32 facing);
static void TolbiShipDeck_SightIsland(void);
static void TolbiShipDeck_ReturnFromIsland(void);
static void TolbiShipDeck_PlaceLeaderOnDeck(void);

/// Who is who on deck, numbering each actor placement table's rows from
/// ACTOR_FIRST_PLACED.
///
/// Every table starts with the same twelve rows of oars, and the events add
/// their actors after a table's last row (Event_AppendActorPlacements). So
/// each stage of the voyage has its own names, which carry its prefix.
///
/// At Kalay (gTolbiShipDeckActorPlacementsWaiting) the captain talks the
/// delay over with Kaja, the deck's fighter, and a crewman, who asks Kaja how
/// the oarsmen are ("Say, Kaja, how are the oarsmen doing?").
///
/// At sea gTolbiShipDeck_CrewActorPlacements places Kaja and the warriors
/// Ouranos and Sean, who help him guard the deck ("That'll be me, Ouranos,
/// and Sean on deck").
enum TolbiShipDeckActorId {
  /// Four oars, with the same sprite as the others, that keep the strokes of
  /// gTolbiShipDeckRowerStrokes.
  ///
  /// There is one each from 1 (the oar left still) to 4 (fastest), drawn under
  /// the deck. Each oar at the sides copies the frames of its seat's stroke
  /// (TolbiShipDeck_SwingOar). At rest the first stands in for the first oar.
  ACTOR_FIRST_STROKE_OAR = ACTOR_FIRST_PLACED,
  /// The eight oars at the ship's sides (SPRITE_OAR).
  ///
  /// There are four at x 132, then four at x 300, which are drawn mirrored.
  /// The rowing deck's map shows them out while the ship is rowed
  /// (https://alchemy.pascalpixel.com/places/tolbi-ship-deck_tolbi-ship-rowing/map-0.png).
  ACTOR_FIRST_OAR = ACTOR_FIRST_STROKE_OAR + 4,

  // At Kalay: gTolbiShipDeckActorPlacements, then
  // gTolbiShipDeckActorPlacementsWaiting.
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who
  /// hid the captain's anchor charm on the mast and denies everything.
  ACTOR_KALAY_CREWMAN = ACTOR_FIRST_OAR + 8,
  /// One of the crew, a man in a yellow bandana (SPRITE_MAN_IN_YELLOW_BANDANA),
  /// who answers the tour guide beside him.
  ///
  /// When the ship leaves he calls "Preparations... Ready!"
  /// (MsgTolbiShipPreparationsReady).
  ACTOR_KALAY_CREW_MEMBER,
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  ACTOR_KALAY_CAPTAIN,
  /// Kaja, a bald man with a beard (SPRITE_KAJA).
  ACTOR_KALAY_KAJA,
  /// The tour group's guide, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK)
  /// as on the Tolbi Docks.
  ///
  /// Mind Read hears "I just hope we don't have to refund their money at the
  /// end of the trip."
  ACTOR_KALAY_TOUR_GUIDE,
  /// Three of the tour group's passengers, named by what they say first.
  ///
  /// This one is a chef (SPRITE_CHEF).
  ACTOR_KALAY_PASSENGER_WAITING_LONG,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN).
  ACTOR_KALAY_PASSENGER_SEEING_COLOSSO,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN).
  ACTOR_KALAY_PASSENGER_KARAGOL,

  // At sea: gTolbiShipDeck_CrewActorPlacements after the oars, then each
  // event's own rows.
  ACTOR_SEA_KAJA = ACTOR_FIRST_OAR + 8,
  /// Ouranos, a young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR).
  ACTOR_SEA_OURANOS,
  /// Sean, a young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR).
  ACTOR_SEA_SEAN,
  /// With ACTOR_KALAY_CREW_MEMBER's sprite.
  ///
  /// At anchor, Mind Read hears "I want to be a sailor like Kaja". On later
  /// departures he calls "Preparations... Ready!"
  ACTOR_SEA_CREW_MEMBER,
  /// The first attack's three monsters, all of one sprite.
  ///
  /// They belong to TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD
  /// (gTolbiShipDeck_MonstersBoardActorPlacements). They leap aboard and go
  /// below by the first door, where gShipRooms_FirstAttackMonsters places
  /// them.
  ACTOR_SEA_FIRST_ATTACK_MONSTER,
  /// Four monsters of one sprite that hover by the deck.
  ///
  /// They belong to TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK
  /// (gTolbiShipDeck_SecondAttackActorPlacements). The first two dive at Sean
  /// and the others at Ouranos.
  ACTOR_SEA_FIRST_HOVERING_MONSTER = ACTOR_SEA_CREW_MEMBER + 1,
  /// Then the second attack's three come aboard and go below.
  ///
  /// Two are of the hovering monsters' sprite and one other, as
  /// gShipRooms_SecondAttackMonsters places them.
  ACTOR_SEA_SECOND_ATTACK_MONSTER = ACTOR_SEA_FIRST_HOVERING_MONSTER + 4,
  /// The splash that swells for 24 frames at the rail where the last of them
  /// leaps aboard out of the sea (SPRITE_SPLASH).
  ACTOR_SEA_SPLASH = ACTOR_SEA_SECOND_ATTACK_MONSTER + 3,
  /// Four monsters with the first attack's sprite, that leap at Sean, the
  /// first two, and at Ouranos.
  ///
  /// They belong to TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK
  /// (gTolbiShipDeck_ThirdAttackActorPlacements).
  ACTOR_SEA_FIRST_LEAPING_MONSTER = ACTOR_SEA_CREW_MEMBER + 1,
  /// Then the third attack's three, which go below.
  ///
  /// Two have the first attack's sprite and one other, as
  /// gShipRooms_ThirdAttackMonsters places them. The last drops onto the deck.
  ACTOR_SEA_THIRD_ATTACK_MONSTER = ACTOR_SEA_FIRST_LEAPING_MONSTER + 4,
  /// The falling monster's shadow, standing in for its own while it drops.
  ///
  /// A blank sprite (SPRITE_BLANK) draws only its shadow, which grows from a
  /// tenth of its size where the monster will land. It goes the moment the
  /// monster lands and its own shadow is switched on.
  ACTOR_SEA_FALLING_MONSTER_SHADOW = ACTOR_SEA_THIRD_ATTACK_MONSTER + 3,
  /// The Kraken, which the party charges.
  ///
  /// It belongs to TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN and at anchor
  /// (gTolbiShipDeck_KrakenActorPlacements, after Garet, Ivan and Mia in
  /// their own slots). At anchor the same rows place it only for it to be
  /// released at once.
  ACTOR_SEA_KRAKEN = ACTOR_SEA_CREW_MEMBER + 1,
  /// An invisible actor (SPRITE_BLANK) the camera follows across the
  /// deck.
  ACTOR_INVISIBLE_SEA_CAMERA_TARGET,
  /// The tour guide, who comes up from below at anchor, back from the island
  /// (gTolbiShipDeck_TourGuideActorPlacements).
  ///
  /// Kaja says "Tell our tourists they can enjoy a nice, long break after we
  /// arrive."
  ACTOR_TOUR_GUIDE_AT_ANCHOR,

  // At Tolbi: gTolbiShipDeckActorPlacementsArrived.
  ACTOR_ARRIVED_KAJA = ACTOR_FIRST_OAR + 8,
  /// A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA).
  ///
  /// Mind Read hears "I'm just going to hang out in Tolbi and enjoy myself for
  /// a while."
  ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO,
  /// With ACTOR_KALAY_CREW_MEMBER's sprite.
  ACTOR_ARRIVED_CREW_MEMBER,
  /// The first of the nine passengers who could promise to row.
  ///
  /// They come in the order of each seat's group of flags
  /// (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS and the others) and of
  /// gTolbiShipDeckRowerStrokes. They are the tour group's two guides, the
  /// chef, the young woman in green, the bald, muscular man, the woman with a
  /// sword, the young man with brown hair, the woman with fair hair and an old
  /// man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  ACTOR_ARRIVED_FIRST_PASSENGER,
  /// An invisible actor (SPRITE_BLANK) the camera follows as the rowers
  /// come up.
  ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET = ACTOR_ARRIVED_FIRST_PASSENGER + 9
};

/// Sean or Ouranos lying on his back, arms flung out
/// (young_man_with_blue_hair.png, young_man_with_red_hair.png), beyond enum
/// ActorAnimation.
enum { ANIM_LYING_FLAT = 5 };

/// The deck's step triggers at Kalay, beside the ways below, whose cell
/// behaviors carry the numbers of their exits (TolbiShipDeck_GoBelow).
enum TolbiShipDeckCellBehavior {
  /// Beside the crew, where they talk over the delay.
  CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW = 10,
  /// East of the second door, which the crewman slipped through.
  CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_DOOR = 11,
  CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_DOOR = 12
};

#include "shared/actor_pulse.c"

/// The door to the passengers' hold has been drawn open this visit.
#define FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN FLAG_SCENE_TEMP_71
/// The door to the captain's quarters has been drawn open this visit.
#define FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN FLAG_SCENE_TEMP_72

/// An actor script call that shows the actor's sprite.
///
/// Returns 0 to end the call.
static s32 TolbiShipDeck_ShowSprite(struct Actor *a) {
  Actor_SetSpriteFlags(a, SPRITE_FLAG_SHADOW);
  return 0;
}

// The diving scripts own the actor's scratch fields: riseBaseY is
// sideways drift in 16.16 pixels per frame, riseCounter the dive step (0
// hovering), riseEnabled the return side, timer the drift direction and work
// the bob direction. Using the actor's fields keeps one layout for the
// whole slot.

/// Defines `name`, the update of a sea monster hovering by one warrior's side
/// of the deck while the monsters attack.
///
/// It drifts between minX and maxX and bobs, at random. One frame in a hundred
/// it dives at the warrior to (diveX, diveZ), 20 pixels up, and the warrior
/// turns and jumps (sometimes with an emote). Then it returns to its side,
/// (eastX, eastZ) or (westX, westZ) as riseEnabled picks.
///
/// Returns 1 to keep running.
#define TOLBI_SHIP_DECK_HOVER_AND_DIVE(name, warrior, diveX, diveZ, eastX,     \
                                       eastZ, westX, westZ, minX, maxX)        \
  static s32 name(struct Actor *actor) {                                       \
    if (actor->riseCounter != 0) {                                             \
      switch (actor->riseCounter) {                                            \
      case 1:                                                                  \
        actor->speedLimit = px(4);                                             \
        actor->acceleration = px(2);                                           \
        FieldActor_SetMoveTarget(actor, diveX, 20, diveZ);                     \
        actor->riseCounter++;                                                  \
        break;                                                                 \
      case 2:                                                                  \
        actor->riseCounter++;                                                  \
        break;                                                                 \
      case 3:                                                                  \
        if (actor->targetX == ACTOR_NO_TARGET &&                               \
            actor->targetY == actor->targetX &&                                \
            actor->targetZ == actor->targetY) {                                \
          actor->riseCounter++;                                                \
          Sound_Play(SE_ENEMY_CRY);                                            \
          if (actor->riseEnabled != 0)                                         \
            Actor_FaceDirection(warrior, FACING_NORTH_NORTHEAST, 0);           \
          else                                                                 \
            Actor_FaceDirection(warrior, FACING_NORTH_NORTHWEST, 0);           \
          if (Random_Below(4) != 0) {                                          \
            Actor_Get(warrior)->velocityY = px(2);                             \
          } else {                                                             \
            Actor_ShowEmote(warrior, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);    \
            Actor_Get(warrior)->velocityY = px(6);                             \
          }                                                                    \
        }                                                                      \
        break;                                                                 \
      case 4:                                                                  \
        actor->riseCounter++;                                                  \
        break;                                                                 \
      case 5:                                                                  \
        if (actor->riseEnabled != 0)                                           \
          FieldActor_SetMoveTarget(actor, eastX, 0, eastZ);                    \
        else                                                                   \
          FieldActor_SetMoveTarget(actor, westX, 0, westZ);                    \
        actor->riseCounter++;                                                  \
        break;                                                                 \
      case 6:                                                                  \
        actor->riseCounter++;                                                  \
        break;                                                                 \
      case 7:                                                                  \
        if (actor->targetX == ACTOR_NO_TARGET &&                               \
            actor->targetY == actor->targetX &&                                \
            actor->targetZ == actor->targetY) {                                \
          actor->speedLimit = px(2);                                           \
          actor->acceleration = px(1);                                         \
          actor->timer = 0;                                                    \
          actor->userValue = 0;                                                \
          actor->riseCounter++;                                                \
          actor->riseBaseY = 0;                                                \
        }                                                                      \
        break;                                                                 \
      case 8:                                                                  \
        actor->riseCounter = 0;                                                \
        break;                                                                 \
      }                                                                        \
    } else {                                                                   \
      if (actor->timer != 0) {                                                 \
        actor->riseBaseY -= Random_Below(4096);                                \
        if (actor->riseBaseY < -px(1) / 4)                                     \
          actor->timer = 0;                                                    \
      } else {                                                                 \
        actor->riseBaseY += Random_Below(4096);                                \
        if (actor->riseBaseY > px(1) / 4)                                      \
          actor->timer = 1;                                                    \
      }                                                                        \
      if (actor->x.fixed > px(minX) && actor->x.fixed < px(maxX))              \
        actor->x.fixed += actor->riseBaseY;                                    \
      if ((s16)actor->userValue != 0) {                                        \
        actor->y.fixed = actor->y.fixed - (Random_Below(32768)) - px(1) / 2;   \
        if (actor->y.fixed < 0)                                                \
          actor->userValue = 0;                                                \
      } else {                                                                 \
        actor->y.fixed = actor->y.fixed + (Random_Below(32768)) + px(1) / 2;   \
        if (actor->y.fixed > px(8))                                            \
          actor->userValue = 1;                                                \
      }                                                                        \
    }                                                                          \
    if (Random_Below(100) == 0)                                                \
      actor->riseCounter = 1;                                                  \
    return 1;                                                                  \
  }

/// A monster hovering on Ouranos's side, diving at him.
TOLBI_SHIP_DECK_HOVER_AND_DIVE(TolbiShipDeck_HoverAndDiveAtOuranos,
                               ACTOR_SEA_OURANOS, 268, 692, 282, 658, 254, 668,
                               248, 292)

/// A monster hovering on Sean's side, diving at him.
TOLBI_SHIP_DECK_HOVER_AND_DIVE(TolbiShipDeck_HoverAndDiveAtSean, ACTOR_SEA_SEAN,
                               256, 640, 264, 600, 242, 604, 232, 272)

static u16 sStrokePhase[4] ALIGNED(16);

static s32 sSwayAngleY;
UNUSED static s32 sUnused0; ///< never used; keeps the statics' layout

static u16 sOarAngle[8];

static s32 sSeaScroll[2];

static s32 sSwayAngleX;
UNUSED static s32 sUnused1; ///< never used; keeps the statics' layout

static s32 sSeaSpeed[2];

// The oar strokes and depths come after the file's other statics, the strokes
// on a 16-byte boundary.
static u32 sOarStroke[8] __attribute__((aligned(16)));
static s32 sOarDepth[8];

/// Bobs the actor between 4 and 12 pixels high by a random amount
/// each frame.
static s32 TolbiShipDeck_Bob(struct Actor *actor) {
  if ((s16)actor->userValue != 0) {
    actor->y.fixed -= (Random_Below(32768)) + px(1) / 2;
    if (actor->y.fixed < px(4)) {
      actor->userValue = 0;
    }
  } else {
    actor->y.fixed += (Random_Below(32768)) + px(1) / 2;
    if (actor->y.fixed > px(12)) {
      actor->userValue = 1;
    }
  }
  return 1;
}

/// Now and then (two chances in 32) turns the actor to look
/// north-northeast or north-northwest.
///
/// Returns 1.
static s32 TolbiShipDeck_LookAround(struct Actor *actor) {
  u32 roll = Random_Below(32);

  if (roll == 6)
    actor->facing = FACING_NORTH_NORTHEAST;
  else if (roll == 9)
    actor->facing = FACING_NORTH_NORTHWEST;
  return 1;
}

/// Defines `name`, the update of a monster on deck that, at random, leaps at
/// a warrior.
///
/// It jumps to him at (atX, atZ), he turns and jumps back (sometimes with
/// an emote), and it lands. While riseEnabled is set he turns to setFacing and
/// it lands on (setX, setZ), else clearFacing and (clearX, clearZ).
///
/// The actor's timer holds the step. Returns 1.
#define TOLBI_SHIP_DECK_LEAP_AT(name, warrior, atX, atZ, setFacing,            \
                                clearFacing, setX, setZ, clearX, clearZ)       \
  static s32 name(struct Actor *actor) {                                       \
    switch (actor->timer) {                                                    \
    case 0:                                                                    \
      if ((Random_Below(40)) == 0) {                                           \
        actor->timer++;                                                        \
      }                                                                        \
      break;                                                                   \
    case 1:                                                                    \
      actor->timer++;                                                          \
      break;                                                                   \
    case 2:                                                                    \
      actor->velocityY = px(4);                                                \
      actor->speedLimit = px(4);                                               \
      actor->acceleration = px(2);                                             \
      FieldActor_SetMoveTarget(actor, atX, 0, atZ);                            \
      actor->timer++;                                                          \
      break;                                                                   \
    case 3:                                                                    \
      actor->timer++;                                                          \
      break;                                                                   \
    case 4:                                                                    \
      if (actor->targetX == ACTOR_NO_TARGET &&                                 \
          actor->targetY == actor->targetX &&                                  \
          actor->targetZ == actor->targetY) {                                  \
        actor->timer++;                                                        \
        Sound_Play(SE_JUMP);                                                   \
        if (actor->riseEnabled != 0) {                                         \
          Actor_FaceDirection(warrior, setFacing, 0);                          \
        } else {                                                               \
          Actor_FaceDirection(warrior, clearFacing, 0);                        \
        }                                                                      \
        if (Random_Below(4) != 0) {                                            \
          Actor_Get(warrior)->velocityY = px(2);                               \
        } else {                                                               \
          Actor_ShowEmote(warrior, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);      \
          Actor_Get(warrior)->velocityY = px(6);                               \
        }                                                                      \
      }                                                                        \
      break;                                                                   \
    case 5:                                                                    \
      actor->timer++;                                                          \
      break;                                                                   \
    case 6:                                                                    \
      actor->timer++;                                                          \
      actor->velocityY = px(4);                                                \
      actor->speedLimit = px(2);                                               \
      actor->acceleration = px(1);                                             \
      if (actor->riseEnabled != 0) {                                           \
        FieldActor_SetMoveTarget(actor, setX, 0, setZ);                        \
      } else {                                                                 \
        FieldActor_SetMoveTarget(actor, clearX, 0, clearZ);                    \
      }                                                                        \
      break;                                                                   \
    case 7:                                                                    \
      actor->timer++;                                                          \
      break;                                                                   \
    case 8:                                                                    \
      if (actor->targetX == ACTOR_NO_TARGET &&                                 \
          actor->targetY == actor->targetX &&                                  \
          actor->targetZ == actor->targetY) {                                  \
        actor->timer++;                                                        \
      }                                                                        \
      break;                                                                   \
    case 9:                                                                    \
      actor->timer = 0;                                                        \
      break;                                                                   \
    }                                                                          \
    return 1;                                                                  \
  }

TOLBI_SHIP_DECK_LEAP_AT(TolbiShipDeck_LeapAtOuranos, ACTOR_SEA_OURANOS, 264,
                        662, FACING_NORTH_NORTHWEST, FACING_SOUTH_SOUTHWEST,
                        252, 646, 256, 686)
TOLBI_SHIP_DECK_LEAP_AT(TolbiShipDeck_LeapAtSean, ACTOR_SEA_SEAN, 176, 696,
                        FACING_NORTH_NORTHEAST, FACING_EAST, 184, 672, 202, 692)

/// Makes the actor hop at random, one frame in 40 on average.
///
/// Returns 1.
static s32 TolbiShipDeck_RandomlyStartActorHop(struct Actor *actor) {
  if ((Random_Below(40)) == 0)
    actor->velocityY = px(4);
  return 1;
}

/// The update that shrinks an enlarged actor by 1/32 a frame until it is
/// back to normal size.
static void TolbiShipDeck_ShrinkToNormalSize(struct Actor *actor) {
  if (actor->scaleX > Q16_ONE) {
    actor->scaleX -= Q16_ONE / 32;
    actor->scaleY -= Q16_ONE / 32;
  }
}

/// Turns the actor east, northeast-by-north or southeast-by-south at
/// random intervals of 80 to 160 frames.
static s32 TolbiShipDeck_IdleTurn(struct Actor *actor) {
  if (actor->riseCounter != 0) {
    actor->riseCounter--;
  } else {
    u32 roll = Random_Below(300);

    if (roll > 200) {
      actor->facing = FACING_NORTH_NORTHEAST;
    } else if (roll > 100) {
      actor->facing = FACING_SOUTH_SOUTHWEST;
    } else {
      actor->facing = FACING_EAST;
    }
    actor->riseCounter = (Random_Below(80)) + 80;
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTolbiShipDeckEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gTolbiShipDeckRegions;
}

const u32 *Scene_GetExits(void) { return gTolbiShipDeckExits; }

/// The talk facing of a Kalay actor's row in
/// gTolbiShipDeckActorPlacementsWaiting.
#define KALAY_TALK_FACING(actorId)                                             \
  STORE_CONST(                                                                 \
      gTolbiShipDeckActorPlacementsWaiting[(actorId) - ACTOR_FIRST_PLACED]     \
          .talkFacing)

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckActorPlacementsArrived;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWING))
    return gTolbiShipDeckActorPlacementsRowing;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF))
    return gTolbiShipDeckActorPlacementsAtSea;
  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
      KALAY_TALK_FACING(ACTOR_KALAY_CREW_MEMBER) = TALK_FACING_PARTY;
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_WAITING_LONG) = TALK_FACING_KEEP;
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO) =
          TALK_FACING_KEEP;
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN)) {
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_WAITING_LONG) =
          TALK_FACING_PARTY_AND_BACK;
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO) =
          TALK_FACING_PARTY_AND_BACK;
    }
    return gTolbiShipDeckActorPlacementsWaiting;
  }
  return gTolbiShipDeckActorPlacements;
}

#undef KALAY_TALK_FACING

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckTriggersArrived;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND))
    return gTolbiShipDeckTriggersAtIsland;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF))
    return gTolbiShipDeckTriggersAtSea;
  return gTolbiShipDeckTriggers;
}

/// Talking to ACTOR_KALAY_CREW_MEMBER on deck at Kalay.
static void TolbiShipDeck_TalkToCrewMember(void) {
  struct Actor *crewMember;
  s32 random;
  s32 frames;

  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Event_SetMessageId(MsgTolbiShipNowWeHaveProtectShip);
    // "Now that we have you to protect the ship, we ought to be..."
    Event_ShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Actor_RunRepeatedMotion(ACTOR_KALAY_CREW_MEMBER, 2);
      Event_SetMessageId(MsgTolbiShipSomebodyStopThem);
      // "Somebody stop them..."
      Event_ShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
      crewMember = Actor_Get(ACTOR_KALAY_CREW_MEMBER);
      random = Random_Next();
      frames = ((u32)(90 * random) >> 16) + 60;
      crewMember->timer = frames;
      Actor_SetScript(ACTOR_KALAY_CREW_MEMBER,
                      gTolbiShipDeck_SquashAndStretchActions);
    } else {
      Actor_ShowEmote(ACTOR_KALAY_CREW_MEMBER,
                      EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Actor_StartRepeatedMotion(ACTOR_KALAY_CREW_MEMBER, 3);
      Event_SetMessageId(MsgTolbiShipToldWereLeavingSoonSet);
      // "I told you, we're leaving soon. We'll set sail when our..."
      Event_ShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
    }
  }
  Event_End();
}

/// The tour guide on deck at Kalay talks about the ship's delay.
static void TolbiShipDeck_TalkToTourGuide(void) {
  struct Actor *guide;
  s32 random;
  s32 frames;

  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Event_SetMessageId(MsgTolbiShipHaveMakeThemPromiseHelp);
    // "We'll have to make them promise to help row before we can..."
    Event_ShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Actor_RunRepeatedMotion(ACTOR_KALAY_TOUR_GUIDE, 2);
      Event_SetMessageId(MsgTolbiShipTheyCantPlanningMutiny);
      // "They can't be planning a mutiny..."
      Event_ShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
      guide = Actor_Get(ACTOR_KALAY_TOUR_GUIDE);
      random = Random_Next();
      frames = ((u32)(90 * random) >> 16) + 60;
      guide->timer = frames;
      Actor_SetScript(ACTOR_KALAY_TOUR_GUIDE,
                      gTolbiShipDeck_SquashAndStretchActions);
    } else {
      Actor_ShowEmote(ACTOR_KALAY_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      0);
      Actor_StartRepeatedMotion(ACTOR_KALAY_TOUR_GUIDE, 3);
      Event_SetMessageId(MsgTolbiShipIfWeDontLeaveSoon);
      // "If we don't leave soon, Colosso will start without us! Will..."
      Event_ShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
    }
  }
  Event_End();
}

/// At Tolbi, ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO asks whether the party is
/// headed for Colosso too.
static void TolbiShipDeck_AskHeadedForColosso(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiShipHeadedColosso);
  // "Are you headed for Colosso, too?"
  // Yes: "I thought so. Most people coming at this time of year..."
  // No: "You came all this way, and you're not going to Colosso? That..."
  Event_AskYesNo(ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, 0);
  Event_End();
}

/// Opens the first door to below deck, once a visit.
static void TolbiShipDeck_OpenFirstDoor(void) {
  if (Event_GetFlag(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN) == 0) {
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(30, 94, 13, 94, 1, 3);
    GameFlag_Set(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN);
  }
}

/// Opens the second door to below deck, once a visit.
static void TolbiShipDeck_OpenSecondDoor(void) {
  if (Event_GetFlag(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN) == 0) {
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(30, 108, 13, 108, 1, 2);
    GameFlag_Set(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN);
  }
}

/// Walking into one of the ways below deck, whose cell behaviors are
/// numbered as their exits.
static void TolbiShipDeck_GoBelow(void) {
  s32 throughDoor;
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  throughDoor = 0;
  switch (work->stepTrigger) {
  case TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD:
    throughDoor = 1;
    TolbiShipDeck_OpenFirstDoor();
    break;
  case TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM:
    throughDoor = 1;
    TolbiShipDeck_OpenSecondDoor();
    break;
  }
  if (throughDoor != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 1, -10);
    Event_Wait(10);
  } else {
    Sound_Play(SE_MAP_EXIT);
  }
  Import_EventSetExitRequest(work->stepTrigger);
}

#if TBS_PATCH_2
/// In the German, Spanish, French and Italian editions cell behavior 2 is a
/// plain step trigger.
static void TolbiShipDeck_GoBelowWhileFacingWest(void) {
  if ((u16)(Actor_Get(ACTOR_PARTY_LEADER)->facing - (FACING_SOUTHWEST + 1)) <
      FACING_NORTHWEST - (FACING_SOUTHWEST + 1)) {
    Event_Start();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 214, 664);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    TolbiShipDeck_GoBelow();
  }
}
#endif

/// The captain decides to set sail without the replacement ship.
static void TolbiShipDeck_WatchCrewDecideToSail(void) {
  struct Actor *actor;

  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    Event_Start();
    Field_EndBusyAndReveal();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_KALAY_CREWMAN, 10);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(190, CAMERA_KEEP, 708, 1);
    FieldCamera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
    Event_SetMessageId(MsgTolbiShipWonderCouldHaveHappened);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
    Actor_FaceDirection(ACTOR_KALAY_CAPTAIN, FACING_SOUTH_SOUTHWEST, 0);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_RunRepeatedMotion(ACTOR_KALAY_CREWMAN, 1);
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHWEST);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    TolbiShipDeck_Turn(ACTOR_KALAY_KAJA, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimation(ACTOR_KALAY_KAJA, ANIM_NOD);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_FaceDirection(ACTOR_KALAY_CAPTAIN, FACING_WEST, 20);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    TolbiShipDeck_Turn(ACTOR_KALAY_KAJA, FACING_EAST);
    Actor_SetAnimationAndWait(ACTOR_KALAY_KAJA, ANIM_SHAKE_HEAD);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    40);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CAPTAIN, ANIM_NOD);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST);
    Actor_SetAnimation(ACTOR_KALAY_KAJA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CREWMAN, ANIM_NOD);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    TolbiShipDeck_Turn(ACTOR_KALAY_CAPTAIN, FACING_SOUTH_SOUTHWEST);
    Event_SetMessageId(MsgTolbiShipTheresNothingWeCanDo);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    Actor_ShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessageThenWait(
        SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE), 0, 20);
    Actor_ShowEmote(ACTOR_KALAY_KAJA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
    TolbiShipDeck_Turn(ACTOR_KALAY_CAPTAIN, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CAPTAIN, ANIM_NOD);
    Event_ShowMessageThenWait(
        SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE), 0, 20);
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    TolbiShipDeck_Turn(ACTOR_KALAY_CAPTAIN, FACING_SOUTH_SOUTHWEST);
    Actor_SetAnimation(ACTOR_KALAY_CAPTAIN, ANIM_SHAKE_HEAD);
    TolbiShipDeck_Say(ACTOR_KALAY_CAPTAIN);
    Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_KALAY_KAJA, FACING_SOUTH_SOUTHEAST, 40);
    Actor_FaceDirection(ACTOR_KALAY_KAJA, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST, 20);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 2);
    Event_Wait(20);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_SetAttachedEffect(ACTOR_KALAY_KAJA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_KALAY_CREWMAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CAPTAIN, ANIM_NOD);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    FieldCamera_SetSpeed(0.8);
    FieldCamera_MoveTo(182, CAMERA_KEEP, 760, 1);
    Actor_SetSpeed(ACTOR_KALAY_KAJA, 0.8);
    Actor_SetScript(ACTOR_KALAY_KAJA, gTolbiShipDeck_KajaLeavesActions);
    Actor_SetSpeed(ACTOR_KALAY_CAPTAIN, 0.8);
    Actor_SetScript(ACTOR_KALAY_CAPTAIN,
                    gTolbiShipDeck_CaptainStepsAsideActions);
    Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 0.8);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 182, 760);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    60);
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST);
    Event_ShowMessageThenWait(ACTOR_KALAY_CREWMAN, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CREWMAN, ANIM_NOD);
    Actor_Jump(ACTOR_KALAY_CREWMAN, 4, 0);
    Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_SOUTH_SOUTHEAST, 40);
    FieldCamera_SetSpeed(1);
    FieldCamera_MoveTo(216, CAMERA_KEEP, 790, 1);
    Actor_Get(ACTOR_KALAY_CREWMAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 182, 782);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 192, 808);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 808);
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_OpenSecondDoor();
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 798);
    Actor_SetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
    actor = Actor_Get(ACTOR_KALAY_CREWMAN);
    actor->scaleX = Q16_ONE;
    actor = Actor_Get(ACTOR_KALAY_CREWMAN);
    actor->scaleY = Q16_ONE;
    GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
    Event_End();
  }
}

/// On deck at Kalay, the crewman comes out as the party approaches from the
/// east side of the door (CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_DOOR).
static void TolbiShipDeck_CatchCrewmanFromEast(void) {
  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Event_Start();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 0.4);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 232, 816);
      Event_Wait(1);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags |= ACTOR_FACES_MOTION;
      Event_Wait(20);
      Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 244, 804);
      Event_Wait(1);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags |= ACTOR_FACES_MOTION;
      Event_Wait(20);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 3.2);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 248, 778);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 248, 700);
      Actor_SetPosition(ACTOR_KALAY_CREWMAN, 246, 512);
      Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Event_End();
    }
  }
}

/// The same, approaching from the west side
/// (CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_DOOR).
static void TolbiShipDeck_CatchCrewmanFromWest(void) {
  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Event_Start();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 0.4);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 202, 816);
      Event_Wait(1);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags |= ACTOR_FACES_MOTION;
      Event_Wait(20);
      Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 192, 804);
      Event_Wait(1);
      Actor_Get(ACTOR_KALAY_CREWMAN)->steeringFlags |= ACTOR_FACES_MOTION;
      Event_Wait(20);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 3.2);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 180, 778);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 180, 700);
      Actor_SetPosition(ACTOR_KALAY_CREWMAN, 246, 512);
      Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Event_End();
    }
  }
}

/// The start both of those share.
static void TolbiShipDeck_ConfrontCrewman(void) {
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(216, CAMERA_KEEP, 824, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  TolbiShipDeck_OpenSecondDoor();
  MapGrid_CopyCellsTo(30, 108, 13, 108, 1, 2);
  Event_Wait(10);
  Actor_SetPosition(ACTOR_KALAY_CREWMAN, 216, 800);
  Actor_SetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
  Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 814);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_KALAY_CREWMAN, 10);
  Actor_SetAnimationAndWait(ACTOR_KALAY_CREWMAN, ANIM_SHAKE_HEAD);
  Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
  Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceActor(ACTOR_KALAY_CREWMAN, ACTOR_PARTY_LEADER, 20);
  Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
  Event_SetMessageId(MsgTolbiShipDidntDoAnything);
  // "I didn't do anything..."
  Event_ShowMessageThenWait(ACTOR_KALAY_CREWMAN, 0, 20);
  Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT);
}

/// Sets up the scene on entry.
s32 Scene_Initialize(void) {
  struct MapGridLayerScroll *sea =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_GROUND];

  GameFlag_Clear(FLAG_WORLD_MAP_LEFT);
  FIELD_WORK_FROM(gMapGridWork, HEAP_SLOT_MAP_GRID)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  sea->speedY = 0;
  sSwayAngleX = (u16)Random_Next();
  sSwayAngleY = (u16)Random_Next();
  sSeaScroll[0] = 0;
  sSeaScroll[1] = 0;
  sSeaSpeed[0] = 0;
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  TolbiShipDeck_ApplyEntryState();
  return 0;
}

/// Applies the voyage's state.
static void TolbiShipDeck_ApplyEntryState(void) {
  s32 flag;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN);
    GameFlag_Clear(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN);
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
    Actor_SetSpriteColorById(ACTOR_ARRIVED_FIRST_PASSENGER + 1,
                             SPRITE_COLOR_RBG);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
  } else if ((flag = GameFlag_Get(FLAG_TOLBI_SHIP_ROWING)) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_SetUpOars();
    Task_AddFrame(TolbiShipDeck_RowOars, TASK_PASS_FRAME);
    sSeaScroll[1] = px(32);
    sSeaSpeed[1] = px(6) / 5;
    Task_AddFrame(TolbiShipDeck_RockDeck, TASK_PASS_FRAME);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    sSeaScroll[1] = flag;
    sSeaSpeed[1] = flag;
    Task_AddFrame(TolbiShipDeck_RockDeck, TASK_PASS_FRAME);
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWING) == 0) {
    TolbiShipDeck_RestOars();
  }
  switch (gMainRoutineWork.entrance) {
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST:
    Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority = 1;
    break;
  case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      TolbiShipDeck_DepartWithParty();
    } else {
      TolbiShipDeck_DepartFromKalay();
    }
    return;
  case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS:
    TolbiShipDeck_SightSeaMonsters();
    return;
  case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD:
    TolbiShipDeck_MonstersBoard();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK:
    TolbiShipDeck_MonstersAttackCrew();
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK:
    TolbiShipDeck_MonsterLandsOnDeck();
    return;
  case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN:
    TolbiShipDeck_JumpIntoKrakenBattle();
    return;
  case TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      TolbiShipDeck_SetUpAfterKraken();
    } else {
      TolbiShipDeck_ThankForKraken();
    }
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED:
    TolbiShipDeck_SightLand();
    return;
  case TOLBI_SHIP_DECK_FROM_WORLD_MAP:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      TolbiShipDeck_RowersDisembark();
    }
    return;
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE:
    TolbiShipDeck_SightIsland();
    return;
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->minX = px(65);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_SetUpAtAnchor();
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipDeck_SetUpAfterKraken();
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    TolbiShipDeck_SetUpUnderSail();
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    TolbiShipDeck_PlacePassengersAgreedToRow();
  } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    TolbiShipDeck_PlaceWaitingPassengers();
  }
}

/// At Kalay while the ship waits, places the passengers by what has happened
/// so far.
static void TolbiShipDeck_PlaceWaitingPassengers(void) {
  struct Actor *actor;
  s32 flag;

  Actor_SetSpritePriority(ACTOR_KALAY_PASSENGER_KARAGOL, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_KAJA, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_CAPTAIN, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_TOUR_GUIDE, 1);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY) != 0) {
    Actor_SetPosition(ACTOR_KALAY_CAPTAIN, 162, 666);
    Actor_Get(ACTOR_KALAY_CAPTAIN)->facing = FACING_WEST;
    Actor_SetPosition(ACTOR_KALAY_KAJA, 0, 0);
    Actor_SetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
  }
  flag = GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
  if (flag != 0) {
    Actor_SetPosition(ACTOR_KALAY_CREW_MEMBER, 264, 702);
    Actor_Get(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_SOUTH_SOUTHWEST;
    actor = Actor_Get(ACTOR_KALAY_CREW_MEMBER);
    actor->timer = Math_RemainderUnsigned(Random_Next(), 90) + 60;
    Actor_SetScript(ACTOR_KALAY_CREW_MEMBER,
                    gTolbiShipDeck_SquashAndStretchActions);
    Actor_SetPosition(ACTOR_KALAY_TOUR_GUIDE, 248, 680);
    actor = Actor_Get(ACTOR_KALAY_TOUR_GUIDE);
    actor->timer = Math_RemainderUnsigned(Random_Next(), 90) + 60;
    Actor_SetScript(ACTOR_KALAY_TOUR_GUIDE,
                    gTolbiShipDeck_SquashAndStretchActions);
    Actor_SetPosition(ACTOR_KALAY_CAPTAIN, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0) {
      Actor_SetPosition(ACTOR_KALAY_CREWMAN, 246, 512);
      Actor_Get(ACTOR_KALAY_CREWMAN)->facing = flag;
    }
  }
}

/// At Kalay once the anchor charm is back
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED), moves the people on deck.
static void TolbiShipDeck_PlacePassengersAgreedToRow(void) {
  Actor_SetPosition(ACTOR_KALAY_CREW_MEMBER, 262, 706);
  Actor_Get(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_SOUTH_SOUTHWEST;
  Actor_SetPosition(ACTOR_KALAY_TOUR_GUIDE, 164, 648);
  Actor_Get(ACTOR_KALAY_TOUR_GUIDE)->facing = FACING_EAST;
  Actor_SetSpritePriority(ACTOR_KALAY_TOUR_GUIDE, 1);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_WAITING_LONG, 198, 665);
  Actor_Get(ACTOR_KALAY_PASSENGER_WAITING_LONG)->facing = FACING_WEST;
  Actor_SetSpritePriority(ACTOR_KALAY_PASSENGER_WAITING_LONG, 1);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 188, 678);
  Actor_Get(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO)->facing =
      FACING_NORTH_NORTHWEST;
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_KARAGOL, 186, 635);
  Actor_Get(ACTOR_KALAY_PASSENGER_KARAGOL)->facing = FACING_SOUTH_SOUTHWEST;
  Actor_SetPosition(ACTOR_KALAY_CAPTAIN, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_KAJA, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
}

/// Under sail, the crew take their places, the two lookouts start their rounds
/// and, when the party keeps its position, the leader stands on deck.
static void TolbiShipDeck_SetUpUnderSail(void) {
  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 238, 626);
  Actor_SetPosition(ACTOR_SEA_SEAN, 204, 521);
  Actor_Get(ACTOR_SEA_SEAN)->y.fixed = px(16);
  Actor_Get(ACTOR_SEA_SEAN)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  Actor_SetSpeed(ACTOR_SEA_SEAN, 0.6);
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_SeanPacesActions);
  Actor_Get(ACTOR_SEA_OURANOS)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 0.8);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_OuranosPatrolsActions);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Event_End();
}

/// After the Kraken, the deck is as under sail, with Sean at the helm.
static void TolbiShipDeck_SetUpAfterKraken(void) {
  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 238, 626);
  Actor_SetPosition(ACTOR_SEA_SEAN, 268, 678);
  Actor_Get(ACTOR_SEA_SEAN)->facing = FACING_EAST;
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_SeanLooksAboutActions);
  Actor_Get(ACTOR_SEA_OURANOS)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 0.8);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_OuranosPatrolsActions);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Event_End();
}

/// At anchor off the island, the crew stand where they were left, by whether
/// the party has agreed to scout the island.
static void TolbiShipDeck_SetUpAtAnchor(void) {
  struct Actor *actor;

  ((struct MapGridScrollWork *)gMapGridWork[0])->minX = px(65);
  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_KrakenActorPlacements);
  wait(1);
  Actor_ReleaseById(ACTOR_SEA_KRAKEN);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 238, 626);
  actor = Actor_Get(ACTOR_SEA_CREW_MEMBER);
  actor->facing = FACING_SOUTH_SOUTHEAST;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED) != 0) {
    Actor_SetPosition(ACTOR_SEA_SEAN, 162, 634);
    actor = Actor_Get(ACTOR_SEA_SEAN);
    actor->facing = FACING_SOUTH_SOUTHEAST;
    Actor_SetPosition(ACTOR_SEA_OURANOS, 162, 676);
    actor = Actor_Get(ACTOR_SEA_OURANOS);
    actor->facing = FACING_NORTH_NORTHEAST;
  } else {
    Actor_SetPosition(ACTOR_SEA_SEAN, 160, 652);
    actor = Actor_Get(ACTOR_SEA_SEAN);
    actor->facing = FACING_SOUTH_SOUTHEAST;
    Actor_SetPosition(ACTOR_SEA_OURANOS, 166, 668);
    actor = Actor_Get(ACTOR_SEA_OURANOS);
    actor->facing = FACING_NORTH_NORTHWEST;
  }
  if (gMainRoutineWork.entrance == TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) {
    TolbiShipDeck_ReturnFromIsland();
  }
  Event_End();
}

/// Setting sail from Kalay before the party is on deck, Kaja
/// (ACTOR_KALAY_KAJA) brings word to ACTOR_KALAY_CREW_MEMBER that all is
/// ready.
static void TolbiShipDeck_DepartFromKalay(void) {
  struct Actor *actor;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_CAPTAIN, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_TOUR_GUIDE, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_WAITING_LONG, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_KARAGOL, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_KAJA, 0, 0);
  actor = Actor_Get(ACTOR_KALAY_KAJA);
  actor->facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPosition(ACTOR_KALAY_CREW_MEMBER, 232, 650);
  actor = Actor_Get(ACTOR_KALAY_CREW_MEMBER);
  actor->facing = FACING_NORTH_NORTHWEST;
  FieldCamera_MoveTo(232, CAMERA_KEEP, 636, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  TolbiShipDeck_AnnouncePreparationsReady(ACTOR_KALAY_KAJA,
                                          ACTOR_KALAY_CREW_MEMBER);
}

/// On later departures, with the party aboard, Kaja brings word to
/// ACTOR_SEA_CREW_MEMBER.
static void TolbiShipDeck_DepartWithParty(void) {
  struct Actor *actor;

  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 232, 636);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  wait(1);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_Stop(ACTOR_SEA_SEAN);
  Actor_Stop(ACTOR_SEA_OURANOS);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_SEAN, 0, 0);
  Actor_SetPosition(ACTOR_SEA_OURANOS, 0, 0);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  actor = Actor_Get(ACTOR_SEA_KAJA);
  actor->facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 232, 650);
  actor = Actor_Get(ACTOR_SEA_CREW_MEMBER);
  actor->facing = FACING_NORTH_NORTHWEST;
  wait(1);
  TolbiShipDeck_AnnouncePreparationsReady(ACTOR_SEA_KAJA,
                                          ACTOR_SEA_CREW_MEMBER);
}

/// The messenger comes up through the first door to report, and the crew
/// member waiting there calls the ship ready.
static void TolbiShipDeck_AnnouncePreparationsReady(s32 messenger,
                                                    s32 crewMember) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(messenger, 216, 588);
  Actor_SetSpeed(messenger, 0.8);
  Actor_WalkToAndWait(messenger, 216, 600);
  Actor_WalkToAndWait(messenger, 218, 604);
  Actor_WalkToAndWait(messenger, 234, 604);
  Actor_WalkToAndWait(messenger, 236, 618);
  Actor_FaceDirection(messenger, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(messenger, ANIM_NOD);
  Event_Wait(20);
  TolbiShipDeck_Turn(crewMember, FACING_SOUTH_SOUTHWEST);
  Actor_Jump(crewMember, 4, 40);
  Actor_StartRepeatedMotion(crewMember, 2);
  Event_SetMessageId(MsgTolbiShipPreparationsReady);
  // "Preparations... Ready!"
  Event_ShowMessageThenWait(crewMember, 0, 20);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR);
}

/// Sea monsters are sighted.
static void TolbiShipDeck_SightSeaMonsters(void) {
  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipAyeCaptainSeaMonsters);
  // "Aye, Captain! Sea monsters!"
  Event_ShowMessageThenWait(ACTOR_SEA_KAJA, 0, 10);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHWEST);
  Actor_Jump(ACTOR_SEA_SEAN, 4, 20);
  Actor_StartRepeatedMotion(ACTOR_SEA_SEAN, 2);
  Event_ShowMessageThenWait(
      SPEAKER(ACTOR_SEA_SEAN, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT), 0,
      20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_TURN);
}

/// The monsters leap aboard and someone cries out.
static void TolbiShipDeck_MonstersBoard(void) {
  const s32 *shared;

  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_MonstersBoardActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_OURANOS, 248, 728);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 1.6);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 242, 692);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 196, 678);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 182, 654);
  Actor_RunRepeatedMotion(ACTOR_SEA_OURANOS, 2);
  Event_SetMessageId(MsgTolbiShipNoUseLate);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_OURANOS,
                            SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.4);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 154, 609);
  Sound_Play(SE_ENEMY_CRY);
  Actor_Get(ACTOR_SEA_FIRST_ATTACK_MONSTER)->timer = 0;
  Actor_Get(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1)->timer = 0;
  Actor_Get(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2)->timer = 0;
  Actor_SetPosition(ACTOR_SEA_FIRST_ATTACK_MONSTER, 32, 484);
  Actor_SetPosition(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, 84, 496);
  Actor_SetPosition(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, 16, 596);
  Actor_SetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER, 2.4);
  Actor_SetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, 2.4);
  Actor_SetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, 2.4);
  shared = gTolbiShipDeck_MonsterLeapsAboardActions;
  Actor_SetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER, shared);
  Actor_SetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, shared);
  Actor_SetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, shared);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_ATTACK_MONSTER, SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1,
                           SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2,
                           SPRITE_COLOR_BGR);
  do {
    wait(1);
  } while (Actor_Get(ACTOR_SEA_FIRST_ATTACK_MONSTER)->timer == 0);
  TolbiShipDeck_OpenFirstDoor();
  /* FAKEMATCH(dead): the walk in a once-only block loads its actor id straight
   * after the call above. */
  do {
    Actor_WalkTo(ACTOR_SEA_OURANOS, 196, 612);
  } while (0);
  Actor_WaitForScriptEndById(ACTOR_SEA_FIRST_ATTACK_MONSTER);
  Event_Wait(10);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  ActorSystem_SuspendProjectionAndShowSprites();
  Message_SetVoiceFromActor(ACTOR_SEA_OURANOS);
  FieldMessage_ShowCentered(MsgTolbiShipArrgh, 1, 0);
  ActorSystem_ResumeProjectionAndHideLayers();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_FIRST_ATTACK);
}

/// The monsters harry the crew while more of them climb aboard.
static void TolbiShipDeck_MonstersAttackCrew(void) {
  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_SecondAttackActorPlacements);
  wait(1);
  Actor_SetAnimation(ACTOR_SEA_SPLASH, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_SECOND_ATTACK_MONSTER),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1),
                       SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_SEA_SEAN, 256, 640);
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_LookAroundActions);
  Actor_SetPosition(ACTOR_SEA_OURANOS, 268, 692);
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_LookAroundActions);
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER, 242, 604);
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1, 264, 600);
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2, 254, 668);
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3, 282, 658);
  Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER)->riseEnabled = 0;
  Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1)->riseEnabled = 1;
  Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2)->riseEnabled = 0;
  Actor_Get(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3)->riseEnabled = 1;
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER,
                  gTolbiShipDeck_HoverAndDiveAtSeanActions);
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1,
                  gTolbiShipDeck_HoverAndDiveAtSeanActions);
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2,
                  gTolbiShipDeck_HoverAndDiveAtOuranosActions);
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3,
                  gTolbiShipDeck_HoverAndDiveAtOuranosActions);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(400);
  Actor_SetPosition(ACTOR_SEA_SECOND_ATTACK_MONSTER, 254, 740);
  Actor_SetPosition(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 24, 586);
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER, 1.6);
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 1.6);
  Actor_SetDestination(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 172, 644);
  Actor_MoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER, 200, 660);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 4);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 174, 620);
  Actor_MoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER, 180, 580);
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER, gTolbiShipDeck_BobActions);
  Actor_SetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1,
                  gTolbiShipDeck_BobActions);
  Sound_Play(SE_LOG_FALL);
  Actor_SetPosition(ACTOR_SEA_SPLASH, 134, 594);
  Actor_SetScript(ACTOR_SEA_SPLASH, gTolbiShipDeck_SwellActions);
  Event_Wait(10);
  Actor_SetPosition(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 134, 584);
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 4);
  Actor_Get(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2)->velocityY = px(8);
  Actor_MoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 186, 612);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2),
                       SPRITE_FLAG_SHADOW);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 2);
  Actor_MoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 216, 600);
  TolbiShipDeck_Turn(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_SetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2,
                  gTolbiShipDeck_MonsterGoesBelowActions);
  Event_Wait(10);
  Actor_SetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER,
                  gTolbiShipDeck_MonsterGoesBelowActions);
  Event_Wait(10);
  Actor_RunScriptAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1,
                         gTolbiShipDeck_MonsterGoesBelowActions);
  Event_Wait(20);
  Sound_Play(SE_BEAST_CRY);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_Stop(ACTOR_SEA_FIRST_HOVERING_MONSTER);
  Actor_Stop(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1);
  Actor_Stop(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2);
  Actor_Stop(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3);
  Event_Wait(10);
  ActorSystem_SuspendProjectionAndShowSprites();
  Message_SetVoiceFromActor(ACTOR_SEA_OURANOS);
  FieldMessage_ShowCentered(MsgTolbiShipArrgh, 1, 0);
  ActorSystem_ResumeProjectionAndHideLayers();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SECOND_ATTACK);
}

/// A great shape rises from the sea and a monster drops onto the deck.
static void TolbiShipDeck_MonsterLandsOnDeck(void) {
  struct Actor *monster;
  struct Actor *actor;
  u32 frame;
  const s32 *sailorActions;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_ThirdAttackActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_SEAN, 176, 696);
  actor = Actor_Get(ACTOR_SEA_SEAN);
  actor->facing = FACING_NORTH_NORTHEAST;
  Actor_SetPosition(ACTOR_SEA_OURANOS, 264, 662);
  actor = Actor_Get(ACTOR_SEA_OURANOS);
  actor->facing = FACING_NORTH_NORTHWEST;
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER, 184, 672);
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1, 202, 692);
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2, 252, 646);
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3, 256, 686);
  Actor_SetPosition(ACTOR_SEA_THIRD_ATTACK_MONSTER, 172, 632);
  Actor_SetPosition(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, 256, 622);
  Actor_Get(ACTOR_SEA_FIRST_LEAPING_MONSTER)->riseEnabled = 0;
  Actor_Get(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1)->riseEnabled = 1;
  Actor_Get(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2)->riseEnabled = 0;
  Actor_Get(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3)->riseEnabled = 2;
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  Actor_SetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER,
                  gTolbiShipDeck_LeapAtSeanActions);
  Actor_SetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1,
                  gTolbiShipDeck_LeapAtSeanActions);
  Actor_SetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2,
                  gTolbiShipDeck_LeapAtOuranosActions);
  Actor_SetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3,
                  gTolbiShipDeck_LeapAtOuranosActions);
  Actor_SetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER, gTolbiShipDeck_HopActions);
  Actor_SetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1,
                  gTolbiShipDeck_HopActions);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_LEAPING_MONSTER, SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1,
                           SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2,
                           SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3,
                           SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_THIRD_ATTACK_MONSTER, SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1,
                           SPRITE_COLOR_BGR);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(80);
  Sound_Play(SE_BEAST_CRY);
  // The shadow grows where the monster will land, then it drops from 80
  // pixels up.
  monster = Actor_Get(ACTOR_SEA_FALLING_MONSTER_SHADOW);
  monster->scaleX = Q16_ONE / 10;
  monster->scaleY = Q16_ONE / 10;
  monster->x.fixed = px(194);
  monster->z.fixed = px(642);
  for (frame = 0; frame < 16; frame++) {
    monster->scaleX += 3 * Q16_ONE / 50;
    monster->scaleY += 3 * Q16_ONE / 50;
    wait(1);
  }
  monster = Actor_Get(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2);
  monster->scaleX = 11 * Q16_ONE / 10;
  monster->scaleY = 11 * Q16_ONE / 10;
  monster->x.fixed = px(194);
  monster->y.fixed = px(80);
  monster->z.fixed = px(642);
  monster->facing = FACING_SOUTH_SOUTHWEST;
  monster->bounce = 2 * Q16_ONE / 5;
  monster->gravity = px(2);
  Event_Wait(80);
  Sound_Play(SE_BEAST_CRY);
  Actor_SetPosition(ACTOR_SEA_FALLING_MONSTER_SHADOW, 0, 0);
  actor = Actor_Get(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.6);
  monster = Actor_Get(ACTOR_PARTY_LEADER);
  monster->motionFlags = 0;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 216, 612);
  Actor_SetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 1.6);
  Actor_MoveToAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 196, 600);
  Actor_MoveToAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 216, 600);
  Actor_Stop(ACTOR_SEA_THIRD_ATTACK_MONSTER);
  wait(1);
  Actor_SetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER, 1.6);
  sailorActions = gTolbiShipDeck_MonsterGoesBelowActions;
  Actor_SetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER, sailorActions);
  TolbiShipDeck_Turn(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2,
                     FACING_NORTH_NORTHEAST);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_SetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, sailorActions);
  Actor_Stop(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1);
  wait(1);
  Actor_SetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, 1.6);
  Actor_RunScriptAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, sailorActions);
  Event_Wait(20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_Stop(ACTOR_SEA_FIRST_LEAPING_MONSTER);
  Actor_Stop(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1);
  Actor_Stop(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2);
  Actor_Stop(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3);
  Actor_Stop(ACTOR_SEA_THIRD_ATTACK_MONSTER);
  Actor_Stop(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1);
  Event_Wait(10);
  ActorSystem_SuspendProjectionAndShowSprites();
  Message_SetVoiceFromActor(ACTOR_SEA_OURANOS);
  FieldMessage_ShowCentered(MsgTolbiShipArrgh, 1, 0);
  ActorSystem_ResumeProjectionAndHideLayers();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_THIRD_ATTACK);
}

/// The party jumps down to the deck, gathers and charges the Kraken.
static void TolbiShipDeck_JumpIntoKrakenBattle(void) {
  struct Actor *actor;

  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_KrakenActorPlacements);
  wait(1);
  FieldCamera_FollowActor(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 1);
  wait(1);
  Actor_SetAnimation(ACTOR_SEA_OURANOS, ANIM_LYING_FLAT);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_OURANOS), 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->facing = FACING_SOUTH;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 216, 586);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
  Event_Wait(20);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
  Sound_Play(SE_BOOM);
  MapGrid_SetShake(4, 4, 1);
  Event_Wait(20);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_Get(ACTOR_INVISIBLE_SEA_CAMERA_TARGET)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 2);
  Actor_SetDestination(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 216, 612);
  Sound_Play(SE_MONSTER_ROAR);
  Actor_SetSpritePriority(ACTOR_SEA_SEAN, 2);
  Actor_SetAnimation(ACTOR_SEA_SEAN, ANIM_LYING_FLAT);
  actor = Actor_Get(ACTOR_SEA_SEAN);
  actor->velocityY = px(8);
  actor->gravity = px(7) / 10;
  actor->scaleX = 13 * Q16_ONE / 8;
  actor->scaleY = 13 * Q16_ONE / 8;
  actor->update = TolbiShipDeck_ShrinkToNormalSize;
  actor->bounce = Q16_ONE / 2;
  Actor_SetSpeed(ACTOR_SEA_SEAN, 6);
  Actor_MoveToAndWait(ACTOR_SEA_SEAN, 182, 618);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SEA_SEAN), 0);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 80);
  Actor_SetDestination(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 232, 564);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 250, 584);
  Actor_WalkTo(ACTOR_GARET, 240, 600);
  Actor_WalkTo(ACTOR_IVAN, 254, 600);
  Actor_WalkToAndWait(ACTOR_MIA, 248, 616);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Sound_Play(SE_MONSTER_ROAR);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 564);
  Actor_WalkTo(ACTOR_GARET, 248, 564);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_IVAN, 248, 564);
  Actor_WalkTo(ACTOR_MIA, 248, 564);
  Event_Wait(20);
  gMainRoutineWork.restartScene = SCENE_SHIP_ROOMS;
  gMainRoutineWork.restartEntrance = SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(
      SCENE_TOLBI_SHIP_DECK, TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 3);
  Event_End();
}

/// Isaac is thanked for the Kraken.
static void TolbiShipDeck_ThankForKraken(void) {
  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, 182, 618);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 238, 626);
  Actor_SetPosition(ACTOR_SEA_SEAN, 268, 678);
  Actor_Get(ACTOR_SEA_SEAN)->facing = FACING_EAST;
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_SeanLooksAboutActions);
  Actor_Get(ACTOR_SEA_OURANOS)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 0.8);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_OuranosPatrolsActions);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SEA_KAJA, 1.6);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 548);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipThankIsaacDidGoodAgainst);
  TolbiShipDeck_Say(ACTOR_SEA_KAJA);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_SOUTH_SOUTHWEST, 20);
  Actor_ShowEmote(ACTOR_SEA_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageThenWait(ACTOR_SEA_KAJA, 0, 40);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Say(ACTOR_SEA_KAJA);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 600);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 216, 600);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 216, 580);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  Event_End();
}

/// Land is sighted.
static void TolbiShipDeck_SightLand(void) {
  struct Actor *actor;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, 196, 502);
  actor = Actor_Get(ACTOR_SEA_KAJA);
  actor->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_SEA_SEAN, 184, 524);
  actor = Actor_Get(ACTOR_SEA_SEAN);
  actor->facing = FACING_NORTH_NORTHWEST;
  Actor_SetSpritePriority(ACTOR_SEA_OURANOS, 1);
  Actor_SetPosition(ACTOR_SEA_OURANOS, 184, 632);
  actor = Actor_Get(ACTOR_SEA_OURANOS);
  actor->facing = FACING_NORTH_NORTHWEST;
  // UB: the field work's pointer is read through a u32 lvalue, which keeps
  // the facing store ahead of its load; UBFIX reads it as declared.
#ifdef UBFIX
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#else
  ((struct FieldWork *)*(u32 *)&gFieldWork)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_Jump(ACTOR_SEA_SEAN, 4, 10);
  Actor_Jump(ACTOR_SEA_SEAN, 6, 20);
  Event_SetMessageId(MsgTolbiShipCanSeeLand);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 3);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 180, 546);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHWEST, 40);
  Actor_RunRepeatedMotion(ACTOR_SEA_OURANOS, 1);
  TolbiShipDeck_Say(ACTOR_SEA_OURANOS);
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI);
}

/// What each rower says coming up at Tolbi, named by its message.
///
/// The first four found take the first four replies in the order found. Three
/// passengers have their own.
enum TolbiShipDeckRowerReply {
  ROWER_REPLY_FINALLY_LUCKY = 0,  ///< MsgTolbiShipMadeFinallyLucky
  ROWER_REPLY_OTHERS_ALREADY = 1, ///< MsgTolbiShipOtherPassengersAlready
  ROWER_REPLY_DONE_ROWING = 2,    ///< MsgTolbiShipDontRowAnymore
  ROWER_REPLY_NO_WORKOUT = 3,     ///< MsgTolbiShipOhhhHaventWorkout
  ROWER_REPLY_ROWING_MORE = 10,   ///< MsgTolbiShipRowingShipMore
  ROWER_REPLY_LOST_AT_SEA = 11,   ///< MsgTolbiShipTotallyLostOcean
  ROWER_REPLY_OLD_MAN = 20        ///< MsgTolbiShipMakeOldMan
};

/// Takes `passenger` as the next to come up, with `reply`, when one of the
/// four seats holds them.
#define TOLBI_SHIP_DECK_TAKE_ROWER(passenger, reply)                           \
  do {                                                                         \
    member = (passenger);                                                      \
    for (index = 0; index <= 3; index++) {                                     \
      if (roster[index] == member) {                                           \
        matched[found] = member;                                               \
        category[found] = (reply);                                             \
        found++;                                                               \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

/// At Tolbi, the four passengers who rowed come up from below in turn and
/// each says something.
static void TolbiShipDeck_RowersDisembark(void) {
  s32 roster[4];
  s32 matched[4];
  s32 category[4];
  u32 index;
  s32 found;
  s32 actorId;
  s32 member;
  struct Actor *actor;

  found = 0;
  for (index = 0; index <= 3; index++) {
    roster[index] = TolbiShipDeck_GetRowerAtSeat(index);
    category[index] = 0;
  }

  // The first four in this order take the first four replies; three others
  // have their own, for when fewer than four of the first rowed.
  TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER, found);
  TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 1, found);
  TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 2, found);
  TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 4, found);
  if (found != 4)
    TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 5, found);
  if (found != 4)
    TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 6, found);
  if (found != 4)
    TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 3,
                               ROWER_REPLY_ROWING_MORE);
  if (found != 4)
    TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 7,
                               ROWER_REPLY_LOST_AT_SEA);
  if (found != 4)
    TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 8,
                               ROWER_REPLY_OLD_MAN);

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET,
                           actor->x.fixed, actor->z.fixed);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 1);

  TolbiShipDeck_OpenFirstDoor();

  Event_Wait(10);

  for (index = 0; index <= 3; index++) {
    actorId = matched[index];
    Actor_SetPosition(actorId, 216, 584);
    if (category[index] == ROWER_REPLY_OLD_MAN) {
      Actor_SetSpeed(actorId, 0.8);
    } else {
      Actor_SetSpeed(actorId, 1);
    }
    Actor_WalkToAndWait(actorId, 216, 150 << 2);
    Actor_WalkToAndWait(actorId, 192, 618);
    Actor_WalkToAndWait(actorId, 192, 164 << 2);

    switch (category[index]) {
    case ROWER_REPLY_FINALLY_LUCKY:
      Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Event_SetMessageId(MsgTolbiShipMadeFinallyLucky);
      break;
    case ROWER_REPLY_OTHERS_ALREADY:
      TolbiShipDeck_Turn(actorId, 208 << 8);
      Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Event_SetMessageId(MsgTolbiShipOtherPassengersAlready);
      break;
    case ROWER_REPLY_DONE_ROWING:
      Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Event_SetMessageId(MsgTolbiShipDontRowAnymore);
      break;
    case ROWER_REPLY_NO_WORKOUT:
      Actor_RunRepeatedMotion(actorId, 1);
      Event_SetMessageId(MsgTolbiShipOhhhHaventWorkout);
      break;
    case ROWER_REPLY_ROWING_MORE:
      Actor_SetAnimationAndWait(actorId, ANIM_NOD);
      Event_SetMessageId(MsgTolbiShipRowingShipMore);
      break;
    case ROWER_REPLY_LOST_AT_SEA:
      Actor_SetAnimation(actorId, ANIM_SHAKE_HEAD);
      Event_SetMessageId(MsgTolbiShipTotallyLostOcean);
      break;
    case ROWER_REPLY_OLD_MAN:
      Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
      Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
      Event_SetMessageId(MsgTolbiShipMakeOldMan);
      break;
    default:
      break;
    }

    TolbiShipDeck_Say(actorId);
    Actor_SetScript(actorId, gTolbiShipDeck_RowerLeavesActions);
  }

  Actor_WaitForScriptEndById(actorId);
  Event_Wait(40);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 216, 584);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 150 << 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 190, 153 << 2);
  Event_SetMessageId(MsgTolbiShipThanksHardWork);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 192 << 8, 0);

  Actor_Get(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 2);
  Actor_SetDestination(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 196, 141 << 2);

  Actor_SetSpeed(ACTOR_ARRIVED_KAJA, 0.8);
  Actor_WalkToAndWait(ACTOR_ARRIVED_KAJA, 182, 555);
  Actor_FaceDirection(ACTOR_ARRIVED_KAJA, FACING_SOUTH_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_ARRIVED_KAJA, 1);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_SetAnimationAndWait(ACTOR_ARRIVED_KAJA, ANIM_NOD);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_FaceDirection(ACTOR_ARRIVED_KAJA, 128 << 8, 40);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  TolbiShipDeck_Turn(ACTOR_ARRIVED_KAJA, FACING_SOUTH_SOUTHEAST);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_SetAnimationAndWait(ACTOR_ARRIVED_KAJA, ANIM_NOD);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_ARRIVED_KAJA, 188, 512);
  Actor_FaceDirection(ACTOR_ARRIVED_KAJA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 0, 0);

  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Returns the passenger (ACTOR_ARRIVED_FIRST_PASSENGER and the eight after)
/// who promised to row at a seat (0 to 3), or 0 if the seat is empty.
static s32 TolbiShipDeck_GetRowerAtSeat(u32 seat) {
  s32 flag = 0;
  s32 passenger = ACTOR_ARRIVED_FIRST_PASSENGER;
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
    if (GameFlag_Get(flag) != 0)
      return passenger;
    flag++;
    passenger++;
  }
  return 0;
}

#define TOLBI_SHIP_DECK_SEA_WRAP px(32)

/// The task that rocks the deck while under sail.
static void TolbiShipDeck_RockDeck(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  s32 *origin = grid->origin;
  s32 dx = Trig_Cos(sSwayAngleX);
  s32 dy = Trig_Sin(sSwayAngleY);
  struct MapGridLayerScroll *layer;

  *origin++ += dx >> 1;
  *origin += dy;
  sSwayAngleX += (u32)(Random_Next() * 3 << 7) >> 16;
  sSwayAngleY += Random_Below(512);
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

/// Under sail, shows the stroke oars (ACTOR_FIRST_STROKE_OAR) and the oars at
/// the sides (ACTOR_FIRST_OAR, the right-hand ones mirrored).
static void TolbiShipDeck_SetUpOars(void) {
  Actor_Get(ACTOR_FIRST_STROKE_OAR)->collisionFlags = 0;
  Actor_Get(ACTOR_FIRST_STROKE_OAR + 1)->collisionFlags = 0;
  Actor_Get(ACTOR_FIRST_STROKE_OAR + 2)->collisionFlags = 0;
  Actor_Get(ACTOR_FIRST_STROKE_OAR + 3)->collisionFlags = 0;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 3);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 3);
  sOarDepth[0] = Actor_Get(ACTOR_FIRST_OAR)->z.fixed;
  sOarDepth[1] = Actor_Get(ACTOR_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = Actor_Get(ACTOR_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = Actor_Get(ACTOR_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 7);
  Actor_Get(ACTOR_FIRST_OAR + 4)->scaleX = -Q16_ONE;
  Actor_Get(ACTOR_FIRST_OAR + 5)->scaleX = -Q16_ONE;
  Actor_Get(ACTOR_FIRST_OAR + 6)->scaleX = -Q16_ONE;
  Actor_Get(ACTOR_FIRST_OAR + 7)->scaleX = -Q16_ONE;
  sOarDepth[4] = Actor_Get(ACTOR_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = Actor_Get(ACTOR_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = Actor_Get(ACTOR_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = Actor_Get(ACTOR_FIRST_OAR + 7)->z.fixed;
  TolbiShipDeck_ChooseOarStrokes();
}

/// Shows an oar drawn under the deck, with no collision.
static void TolbiShipDeck_ShowOar(s32 actorId) {
  struct Actor *oar = Actor_Get(actorId);

  if (oar != NULL) {
    Actor_SetSpritePriority(actorId, 3);
    Actor_SetSpriteFlags(oar, 0);
    oar->collisionFlags = 0;
    {
      s32 flags;
      flags = 2 | oar->priorityFlags;
      oar->priorityFlags = flags;
    }
  }
}

/// Gives each oar the stroke of its seat's rower.
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
    if (GameFlag_Get(base + i) != 0)
      return gTolbiShipDeckRowerStrokes[i];
  }
  return 0;
}

/// The task that rows, which advances the four strokes and swings the eight
/// oars at the sides, each following its stroke's phase.
static void TolbiShipDeck_RowOars(void) {
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR, 0);
  sStrokePhase[0] = 0;
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR + 1, 1);
  sStrokePhase[1] += 128;
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR + 2, 2);
  sStrokePhase[2] += 256;
  TolbiShipDeck_AdvanceStroke(ACTOR_FIRST_STROKE_OAR + 3, 3);
  sStrokePhase[3] += 512;
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR, 0, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 1, 1, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 2, 2, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 3, 3, 0);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 4, 4, 1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 5, 5, 1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 6, 6, 1);
  TolbiShipDeck_SwingOar(ACTOR_FIRST_OAR + 7, 7, 1);
}

/// Advances one stroke's phase.
static void TolbiShipDeck_AdvanceStroke(s32 actorId, s32 slot) {
  u16 value = sOarAngle[slot];

  if (value >= 0x6801 && value <= 0x6fff) {
    sStrokePhase[slot] += 112;
    Actor_SetAnimation(actorId, 3);
  } else if (value >= 0xe801 && value <= 0xefff) {
    sStrokePhase[slot] += 224;
    Actor_SetAnimation(actorId, 3);
  } else if (value >= 0x7001 && value <= 0xefff) {
    sStrokePhase[slot] += 448;
    Actor_SetAnimation(actorId, 2);
  } else {
    sStrokePhase[slot] += 768;
    Actor_SetAnimation(actorId, 1);
  }
}

/// Swings one oar to its angle.
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags) {
  struct Actor *actor = Actor_Get(id);
  struct Sprite *sprite = actor->sprite;
  s32 sine;

  if ((flags & 2) == 0) {
    switch (sOarStroke[slot]) {
    case 1:
      sOarAngle[slot] = sStrokePhase[0];
      Actor_CopyGraphics(id, ACTOR_FIRST_STROKE_OAR);
      break;
    case 2:
      sOarAngle[slot] = sStrokePhase[1];
      Actor_CopyGraphics(id, ACTOR_FIRST_STROKE_OAR + 1);
      break;
    case 3:
      sOarAngle[slot] = sStrokePhase[2];
      Actor_CopyGraphics(id, ACTOR_FIRST_STROKE_OAR + 2);
      break;
    case 4:
      sOarAngle[slot] = sStrokePhase[3];
      Actor_CopyGraphics(id, ACTOR_FIRST_STROKE_OAR + 3);
      break;
    }
  }
  if (flags & 1) {
    sine = Trig_Sin(sOarAngle[slot]);
    sprite->rotation = Trig_Sin(sOarAngle[slot] + TRIG_TURN / 2) >> 5;
    actor->z.fixed = sOarDepth[slot] - (sine << 2) - (sine << 1);
  } else {
    sine = Trig_Sin(sOarAngle[slot] + TRIG_TURN / 2);
    sprite->rotation = Trig_Sin(sOarAngle[slot]) >> 5;
    actor->z.fixed = sOarDepth[slot] + (sine << 2) + (sine << 1);
  }
}

/// When not under sail, the oars lie at rest, all copying the first stroke
/// oar.
static void TolbiShipDeck_RestOars(void) {
  struct Actor *actor;
  u32 i;

  for (i = 0; i <= 7; i++)
    sOarAngle[i] = TRIG_TURN * 3 / 4;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR);
  Actor_SetPosition(ACTOR_FIRST_STROKE_OAR + 1, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_STROKE_OAR + 2, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_STROKE_OAR + 3, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_OAR, 0, 0);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 3);
  sOarStroke[0] = 0;
  sOarStroke[1] = 0;
  sOarStroke[2] = 0;
  sOarStroke[3] = 0;
  sOarDepth[0] = Actor_Get(ACTOR_FIRST_STROKE_OAR)->z.fixed;
  sOarDepth[1] = Actor_Get(ACTOR_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = Actor_Get(ACTOR_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = Actor_Get(ACTOR_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 7);
  Actor_Get(ACTOR_FIRST_OAR + 4)->scaleX = -Q16_ONE;
  Actor_Get(ACTOR_FIRST_OAR + 5)->scaleX = -Q16_ONE;
  Actor_Get(ACTOR_FIRST_OAR + 6)->scaleX = -Q16_ONE;
  Actor_Get(ACTOR_FIRST_OAR + 7)->scaleX = -Q16_ONE;
  sOarStroke[4] = 0;
  sOarStroke[5] = 0;
  sOarStroke[6] = 0;
  sOarStroke[7] = 0;
  sOarDepth[4] = Actor_Get(ACTOR_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = Actor_Get(ACTOR_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = Actor_Get(ACTOR_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = Actor_Get(ACTOR_FIRST_OAR + 7)->z.fixed;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL)
    Actor_SetPositionFixed(ACTOR_FIRST_STROKE_OAR, actor->x.fixed,
                           actor->z.fixed);
  wait(1);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 1, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 2, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 3, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 4, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 5, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 6, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 7, ACTOR_FIRST_STROKE_OAR);
  Actor_Get(ACTOR_FIRST_STROKE_OAR)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 1)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 2)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 3)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 4)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 5)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 6)->retainSpriteCache = 1;
  Actor_Get(ACTOR_FIRST_OAR + 7)->retainSpriteCache = 1;
  wait(1);
  Actor_SetPosition(ACTOR_FIRST_STROKE_OAR, 132, 632);
  wait(1);
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
  MapGrid_CopyCellsTo(78, 39, 78, 40, 5, 1);
  MapGrid_CopyCellsTo(78, 39, 78, 41, 5, 1);
  MapGrid_CopyCellsTo(78, 39, 79, 42, 4, 1);
  MapGrid_CopyCellsTo(78, 39, 82, 43, 1, 1);
  MapGrid_CopyCellAttributes(17, 38, 5, 2, 17, 40);
}

/// Redraws two more blocks of cells, as they are at anchor.
static void TolbiShipDeck_CopyAnchoredCells(void) {
  MapGrid_CopyCellsTo(66, 61, 64, 40, 5, 4);
  MapGrid_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

/// Shows the current message from a speaker and waits 10 frames.
static void TolbiShipDeck_Say(s32 speaker) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(10);
}

/// Turns an actor to a facing over 10 frames.
static void TolbiShipDeck_Turn(s32 actorId, s32 facing) {
  Actor_FaceDirection(actorId, facing, 10);
}

/// After the lookout's call, Kaja sees the coast is not Tolbi's and the ship
/// is off course.
static void TolbiShipDeck_SightIsland(void) {
  Event_Start();
  sSeaScroll[0] = px(4);
  sSeaSpeed[0] = -px(1) / 2;
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_OURANOS, 182, 618);
  Actor_Get(ACTOR_SEA_OURANOS)->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_SEA_KAJA, 218, 516);
  Actor_Get(ACTOR_SEA_KAJA)->facing = FACING_NORTH_NORTHWEST;
  Actor_SetPosition(ACTOR_SEA_SEAN, 204, 526);
  Actor_Get(ACTOR_SEA_SEAN)->facing = FACING_NORTH_NORTHWEST;
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 0, 0);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 0.8);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 182, 532);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHWEST, 40);
  Event_SetMessageId(MsgTolbiShipFinallyReachedTolbi);
  TolbiShipDeck_Say(ACTOR_SEA_OURANOS);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 2);
  Actor_SetAnimation(ACTOR_SEA_KAJA, ANIM_SHAKE_HEAD);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimation(ACTOR_SEA_KAJA, ANIM_NOD);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
  Actor_SetAttachedEffect(ACTOR_SEA_OURANOS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SEA_SEAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 1.6);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 194, 500);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHWEST, 20);
  Actor_SetSpeed(ACTOR_SEA_SEAN, 0.8);
  Actor_WalkToAndWait(ACTOR_SEA_SEAN, 192, 518);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_SEA_KAJA, 1);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 210, 508);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_NORTH_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_SEA_OURANOS, 1);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_OURANOS, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT));
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_NORTH_NORTHEAST);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_SEA_KAJA, ANIM_SHAKE_HEAD);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT));
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 204, 536);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 548);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 592);
  Actor_WalkTo(ACTOR_SEA_KAJA, 182, 664);
  Event_Wait(40);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_ISLAND);
}

/// At anchor off the island, talking to Sean or Ouranos, the party is asked
/// to scout the island for the arguing tour group.
static void TolbiShipDeck_OfferToScoutIsland(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 654);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Actor_WalkTo(ACTOR_GARET, 194, 640);
  Actor_WalkTo(ACTOR_IVAN, 198, 654);
  Actor_WalkToAndWait(ACTOR_MIA, 194, 672);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  TolbiShipDeck_Turn(ACTOR_MIA, FACING_WEST);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_EAST);
  Event_SetMessageId(MsgTolbiShipIsaacTalkedPassengersDidntTour);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  TolbiShipDeck_Turn(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST);
  Event_ShowMessageThenWait(ACTOR_SEA_OURANOS, 0, 40);
  Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_RunRepeatedMotion(ACTOR_SEA_SEAN, 1);
  Event_OpenMessage(ACTOR_SEA_SEAN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    TolbiShipDeck_Say(ACTOR_IVAN);
    TolbiShipDeck_Turn(ACTOR_MIA, FACING_NORTHWEST);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    TolbiShipDeck_Say(ACTOR_MIA);
    TolbiShipDeck_Turn(ACTOR_GARET, FACING_SOUTHWEST);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_OpenMessage(ACTOR_GARET, 0);
    for (;;) {
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_SetMessageId(MsgTolbiShipIsaacDontTalkLikeShouldnt);
        // "Isaac, don't talk like that. Shouldn't we listen to them?"
        Event_OpenMessage(ACTOR_IVAN, 0);
      } else
        break;
    }
  }
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SEA_SEAN, ANIM_NOD);
  Event_SetMessageId(MsgTolbiShipSeeYoureGoingGoFor);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_SetSpeed(ACTOR_SEA_SEAN, 1);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 1);
  Actor_Get(ACTOR_SEA_SEAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SEA_SEAN, 162, 634);
  Event_Wait(1);
  Actor_Get(ACTOR_SEA_SEAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SEA_OURANOS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 162, 676);
  Event_Wait(1);
  Actor_Get(ACTOR_SEA_OURANOS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHEAST, 0);
  TolbiShipDeck_Turn(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_WalkTo(ACTOR_GARET, 180, 654);
  Actor_WalkTo(ACTOR_IVAN, 180, 654);
  Actor_WalkToAndWait(ACTOR_MIA, 180, 654);
  Actor_ReleaseById(ACTOR_GARET);
  Actor_ReleaseById(ACTOR_IVAN);
  Actor_ReleaseById(ACTOR_MIA);
  GameFlag_Set(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED);
  Event_End();
}

/// Back on deck from the island, the party is asked whether it explored it.
static void TolbiShipDeck_ReturnFromIsland(void) {
  Event_Start();
  Event_AppendActorPlacements(
      (struct ActorPlacement *)gTolbiShipDeck_TourGuideActorPlacements);
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 656);
  Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_SEA_SEAN, 1);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHWEST);
  Event_SetMessageId(MsgTolbiShipHowWasIsaacDidExplore);
  // "How was it, Isaac? Did you explore the island thoroughly?"
  Event_OpenMessage(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT));
    Event_End();
    return;
  }
  gFieldWork->message++;
  Event_AskYesNo(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT), 0);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(ACTOR_TOUR_GUIDE_AT_ANCHOR, 216, 588);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE_AT_ANCHOR, 1.2);
  Actor_WalkToAndWait(ACTOR_TOUR_GUIDE_AT_ANCHOR, 216, 596);
  Actor_WalkToAndWait(ACTOR_TOUR_GUIDE_AT_ANCHOR, 188, 616);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHEAST, 0);
  TolbiShipDeck_Turn(ACTOR_TOUR_GUIDE_AT_ANCHOR, FACING_SOUTH_SOUTHWEST);
  Actor_Jump(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2, 0);
  Actor_SetAnimation(ACTOR_TOUR_GUIDE_AT_ANCHOR, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_TOUR_GUIDE_AT_ANCHOR, 0);
  Actor_SetPosition(ACTOR_SEA_KAJA, 180, 777);
  Actor_SetSpeed(ACTOR_SEA_KAJA, 1);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 180, 664);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_NORTH_NORTHEAST, 0);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE_AT_ANCHOR, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  60);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 1);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
  Actor_StartRepeatedMotion(ACTOR_SEA_OURANOS, 2);
  TolbiShipDeck_Say(ACTOR_SEA_OURANOS);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimation(ACTOR_SEA_KAJA, ANIM_NOD);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
  Actor_Jump(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2, 20);
  Actor_SetAnimation(ACTOR_TOUR_GUIDE_AT_ANCHOR, ANIM_SHAKE_HEAD);
  TolbiShipDeck_Say(ACTOR_TOUR_GUIDE_AT_ANCHOR);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 640);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_NORTH_NORTHEAST, 0);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_BELOW));
  Actor_ShowEmote(ACTOR_TOUR_GUIDE_AT_ANCHOR,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_StartRepeatedMotion(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2);
  TolbiShipDeck_Say(ACTOR_TOUR_GUIDE_AT_ANCHOR);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_EAST);
  Actor_RunRepeatedMotion(ACTOR_SEA_SEAN, 1);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_SetSpeed(ACTOR_SEA_SEAN, 1.6);
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_GoBelowActions);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, 1.6);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 168, 632);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_GoBelowActions);
  Event_Wait(80);
  Actor_SetScript(ACTOR_TOUR_GUIDE_AT_ANCHOR, gTolbiShipDeck_GoBelowActions);
  Event_Wait(40);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_WEST);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SWITCH_TEAMS);
}

/// Places the leader on the deck facing south, with the camera on them.
static void TolbiShipDeck_PlaceLeaderOnDeck(void) {
  FieldCamera_MoveTo(232, CAMERA_KEEP, 676, 0);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 232, 676);
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_SOUTH;
  wait(1);
}

/// At Kalay, as the crew's talk of the delay ends, Kaja walks off and is
/// hidden.
const s32 gTolbiShipDeck_KajaLeavesActions[] = {
    SC_ANIMATION(ANIM_WALK),  SC_MOVE(202, 0, 600), SC_SYNC,
    SC_ANIMATION(ANIM_STAND), SC_POSITION(0, 0, 0), SC_END,
};

/// At the same moment the captain steps aside and turns west.
const s32 gTolbiShipDeck_CaptainStepsAsideActions[] = {
    SC_WAIT(20), SC_ANIMATION(ANIM_WALK),  SC_MOVE(162, 0, 666),
    SC_SYNC,     SC_ANIMATION(ANIM_STAND), SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

/// The crew member's and the tour guide's impatience at Kalay.
///
/// They squash and stretch on the spot, the wait between in their timer.
const s32 gTolbiShipDeck_SquashAndStretchActions[] = {
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A first-attack monster runs in, leaps over the rail onto the deck and
/// goes below by the first door.
///
/// It is used at TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD.
const s32 gTolbiShipDeck_MonsterLeapsAboardActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(140, 0, 586),
    SC_SYNC,
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, px(8)),
    SC_MOVE(184, 0, 598),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_CALL(TolbiShipDeck_ShowSprite),
    SC_MOVE(216, 0, 596),
    SC_SYNC,
    SC_MOVE(216, 0, 586),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// Sean on guard at sea, pacing between two spots on the deck and looking
/// out at each.
const s32 gTolbiShipDeck_SeanPacesActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(228, 0, 521),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(40),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(40),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(204, 0, 521),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(40),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Ouranos on guard at sea, walking the four corners of a square on the deck
/// and looking out at each.
const s32 gTolbiShipDeck_OuranosPatrolsActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 672),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(50),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(196, 0, 672),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(70),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(196, 0, 636),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 636),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The second attack's hovering monsters (ACTOR_SEA_FIRST_HOVERING_MONSTER).
///
/// Their scratch fields are cleared, then TolbiShipDeck_HoverAndDiveAtOuranos
/// or TolbiShipDeck_HoverAndDiveAtSean runs each frame.
const s32 gTolbiShipDeck_HoverAndDiveAtOuranosActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_TIMER, 0),
    SC_SET(SW_USER_VALUE, 0),
    SC_SET(SW_RISE_COUNTER, 0),
    SC_SET(23, 0),
    SC_CALL(TolbiShipDeck_HoverAndDiveAtOuranos),
};

const s32 gTolbiShipDeck_HoverAndDiveAtSeanActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_TIMER, 0),
    SC_SET(SW_USER_VALUE, 0),
    SC_SET(SW_RISE_COUNTER, 0),
    SC_SET(23, 0),
    SC_CALL(TolbiShipDeck_HoverAndDiveAtSean),
};

/// The second attack's boarding monsters, bobbing in the air
/// (TolbiShipDeck_Bob).
const s32 gTolbiShipDeck_BobActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_USER_VALUE, 0),
    SC_CALL(TolbiShipDeck_Bob),
};

const s32 gTolbiShipDeck_LookAroundActions[] = {
    SC_CALL(TolbiShipDeck_LookAround),
};

/// ACTOR_SEA_SPLASH swells from 7/10 of its size for 24 frames, then is
/// hidden.
const s32 gTolbiShipDeck_SwellActions[] = {
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 10),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 2 * Q16_ONE / 25),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// A monster on deck goes below by the first door and is hidden.
const s32 gTolbiShipDeck_MonsterGoesBelowActions[] = {
    SC_MOVE(216, 0, 586),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// The third attack's leaping monsters (ACTOR_SEA_FIRST_LEAPING_MONSTER).
///
/// Their timer is cleared, then TolbiShipDeck_LeapAtOuranos or
/// TolbiShipDeck_LeapAtSean runs each frame.
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
    SC_CALL(TolbiShipDeck_RandomlyStartActorHop),
};

/// At Tolbi, a passenger who rowed walks away once they have spoken, and is
/// hidden.
const s32 gTolbiShipDeck_RowerLeavesActions[] = {
    SC_ANIMATION(ANIM_WALK),  SC_MOVE(82, 0, 656),  SC_SYNC,
    SC_ANIMATION(ANIM_STAND), SC_POSITION(0, 0, 0), SC_END,
};

/// Back from the island, Sean, Ouranos and the tour guide go below by the
/// first door, one after another.
const s32 gTolbiShipDeck_GoBelowActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(186, 0, 604),
    SC_SYNC,
    SC_MOVE(216, 0, 604),
    SC_SYNC,
    SC_MOVE(216, 0, 586),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Sean standing by, turning now and then (TolbiShipDeck_IdleTurn).
const s32 gTolbiShipDeck_SeanLooksAboutActions[] = {
    SC_SET(SW_RISE_COUNTER, 0),
    SC_CALL(TolbiShipDeck_IdleTurn),
};

const struct SceneEntrance gTolbiShipDeckEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 179, 0, 653, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, 216, 0,
     600, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, 232, 0, 664,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, 216, 0,
     808, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST, CONDITION_ALWAYS, 216, 4, 648,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_KALAY_DOCKS, CONDITION_ALWAYS, 312, -4, 656,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 120, -4, 656,
     FACING_EAST, 0, 65, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY, CONDITION_ALWAYS, 232, 0, 664,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS, CONDITION_ALWAYS, 212, 0,
     521, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD, CONDITION_ALWAYS, 192, 0,
     650, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK, CONDITION_ALWAYS, 232,
     0, 654, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK, CONDITION_ALWAYS, 216,
     0, 657, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN, CONDITION_ALWAYS, 216, 0, 640,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
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
const struct SceneExitRegion gTolbiShipDeckRegions[] = {
    {208, 24, 634, 224, 40, 650, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST},
    {SCENE_TABLE_END},
};

const u32 gTolbiShipDeckExits[] = {
    SCENE_EXITS(TolbiShipDeck),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, ShipRooms, SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, ShipRooms, SHIP_ROOMS_CRATE_HOLD_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_KALAY_DOCKS, KalayDocks, KALAY_DOCKS_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST, TolbiShipMast, TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_DOCKS, TolbiDocks, TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE, WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR, TolbiShipMast, TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_TURN, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_FIRST_ATTACK, ShipRooms, SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SECOND_ATTACK, ShipRooms, SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_THIRD_ATTACK, ShipRooms, SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI, WorldMap, WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_ISLAND, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND),
    SCENE_EXIT(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SWITCH_TEAMS, ShipRooms, SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS),
    SCENE_EXITS_END,
};

const struct ActorPlacement gTolbiShipDeckActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        266, 0, 703, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

// UB: a table the event writes (its talkFacing), defined const and kept with
// the constants; UBFIX makes it writable.
#ifdef UBFIX
struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[] = {
#else
const struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[] = {
#endif
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        183, 0, 720, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 712, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        198, 0, 706, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        172, 0, 706, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        230, 0, 602, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        236, 0, 621, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        182, 0, 556, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeckActorPlacementsRowing[] = {
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 657, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 657, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 657, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 657, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeckActorPlacementsAtSea[] = {
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_CrewActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 512, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 634, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        212, 0, 521, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_TourGuideActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_MonstersBoardActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_SecondAttackActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_ThirdAttackActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_KrakenActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        178, 0, 642, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        182, 0, 534, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRAKEN, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 540, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 640, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeckActorPlacementsArrived[] = {
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 632, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 664, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        300, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        188, 0, 512, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        162, 0, 634, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        162, 0, 676, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

const struct Trigger gTolbiShipDeckTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_KALAY_DOCKS },
    { TRIGGER_TALK, ACTOR_KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckThereAreALotOfPassengers) },
    { TRIGGER_TALK, ACTOR_KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckOhAtTheRiskOfSounding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckTheReplacementShipFromTolbiShould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIHidTheAnchorCharmNo) },
    { TRIGGER_TALK, ACTOR_KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeApologizeForAnyTroubleThat) },
    { TRIGGER_TALK, ACTOR_KALAY_CREW_MEMBER, CONDITION_ALWAYS, EVENT(TolbiShipDeck_TalkToCrewMember) },
    { TRIGGER_TALK, ACTOR_KALAY_TOUR_GUIDE, CONDITION_ALWAYS, EVENT(TolbiShipDeck_TalkToTourGuide) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderWhatsGoingOnWeve) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseWarriorsGotTiredOfWaiting) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_WAITING_LONG, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIfWeJustPromiseToHelp) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderIfImTheOnly) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckIWonderIfThoseTwoAre) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIllAgreeToAnythingEvenRowing) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolIsLikeAHumongous) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHaHahThoseMercenariesAreGoing) },
    { TRIGGER_TALK, ACTOR_KALAY_PASSENGER_KARAGOL, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThatSeaDogKajaIsA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThereIsNothingToFearAs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREW_MEMBER, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckWhenAreWeGoingWhenAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREW_MEMBER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckUsingForceToLaunchTheShip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAreYouSeriousAboutSettingSail) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_TOUR_GUIDE, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckHmmmThisIsSoFrustratingWhy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_TOUR_GUIDE, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckTheyWereJustAboutToLaunch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItProbablyWontMakeAnyDifference) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTravelingByShipIsFasterThan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_WAITING_LONG, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseThugsAreTooImpatientTrying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_WAITING_LONG, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUltimatelyTheyreJustBeingThreatenedInto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckMostOfThePeopleGoingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHeeHeeIfWeCanGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckBesidesRowingIsAMansJob) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolSeaIsPrettyFoggy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_KARAGOL, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseGuysSeemPrettySureOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_PASSENGER_KARAGOL, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUsHelpWithTheRowingGet) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT(TolbiShipDeck_WatchCrewDecideToSail) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_DOOR, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT(TolbiShipDeck_CatchCrewmanFromEast) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_DOOR, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT(TolbiShipDeck_CatchCrewmanFromWest) },
    { TRIGGER_TABLE_END },
};

const struct Trigger gTolbiShipDeckTriggersAtSea[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
    { TRIGGER_TALK, ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering) },
    { TRIGGER_TALK, ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItLooksLikeAnotherOarsmanWas) },
    { TRIGGER_TALK, ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIFeelSorryForTheOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAnOarsmanWasInjuredByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeHaveNoIdeaWhenThose) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUhnnnWhenILookAtThe) },
    { TRIGGER_TABLE_END },
};

const struct Trigger gTolbiShipDeckTriggersArrived[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_DOCKS },
    { TRIGGER_TALK, ACTOR_ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouLookinForSeanAndOuranos) },
    { TRIGGER_TALK, ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, EVENT(TolbiShipDeck_AskHeadedForColosso) },
    { TRIGGER_TALK, ACTOR_ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisGroupThatCameOnOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantSeanAndOuranosTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckImJustGoingToHangOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWonderWhatHappenedToThat) },
    { TRIGGER_TABLE_END },
};

const struct Trigger gTolbiShipDeckTriggersAtIsland[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(TolbiShipDeck_GoBelow) },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE },
    { TRIGGER_TALK, ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckKajaAndTheCaptainAreProfessionals) },
    { TRIGGER_TALK, ACTOR_SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT(TolbiShipDeck_OfferToScoutIsland) },
    { TRIGGER_TALK, ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckTheresNoTellingWhatKindOf) },
    { TRIGGER_TALK, ACTOR_SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT(TolbiShipDeck_OfferToScoutIsland) },
    { TRIGGER_TALK, ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeveDoneAGoodJobOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantToBeASailor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckIGiveUpWhatAreWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisIslandGivesMeTheCreeps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckThatsEnoughBickering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWhatWeCanDoNowIs) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// How hard each passenger rows, from ACTOR_ARRIVED_FIRST_PASSENGER on.
///
/// 1 leaves the oar still, 2 to 4 row ever faster.
const s32 gTolbiShipDeckRowerStrokes[] = {3, 2, 3, 1, 4, 4, 2, 1, 1};
