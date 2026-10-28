/// @file
/// Tolbi-bound Ship (SceneId_TolbiShipDeck), its deck: the ship that carries
/// the party and a tour group bound for Colosso across the Karagol Sea from
/// Kalay to Tolbi. The ship's other scenes are its mast (tolbi_ship_mast.c)
/// and below decks (ship_rooms.c). The voyage's stage, read from
/// its flags, chooses the deck's actor placements and events: at Kalay,
/// under sail once the first attack is fought off
/// (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF), while the oars move
/// (FLAG_TOLBI_SHIP_ROWING), at anchor off Crossbone Isle
/// (FLAG_TOLBI_SHIP_AT_ISLAND) and at Tolbi (FLAG_TOLBI_SHIP_REACHED_TOLBI).
/// The setup redraws the deck's cells for the stage, rocks the view and
/// scrolls the sea while under sail, rows the oars to the strokes of the
/// passengers at the seats (gTolbiShipDeckRowerStrokes), and plays the
/// event its entrance names. Each event leaves into the next, in
/// another of the ship's scenes or on the World Map, whose entrances sail
/// the ship on. In story order:
///
/// - At Kalay (TOLBI_SHIP_DECK_FROM_KALAY_DOCKS), while the ship waits for
///   its replacement from Tolbi. Once the tour group has left Kalay
///   (FLAG_KALAY_COLOSSO_TOUR_LEFT), walking up to the crew
///   (CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW) plays the captain, Kaja and a
///   crewman talking over the delay: the captain will sail trusting his anchor
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
///   the captain's quarters, where the captain turns the ship; monsters leap
///   aboard (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD), harry
///   Ouranos and Sean, the warriors bound for Colosso who guard the deck with
///   Kaja (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK), and drop onto
///   the deck (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK); each of
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

#include "script_operands.h"
#include "flags.h"
#include "sounds.h"
#include "version.h"
#include "sequence_parent_work.h"
#include "module/scene/tolbi_ship_deck.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/window/window.h"
#include "module/window/screens.h"
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
#include "event.h"
#include "scene_tables.h"
#include "call.h"
#include "event_inline.h"
#include "heap_slots.h"
#include "edition.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "map_grid_scroll.h"
#include "heap.h"
#include "fixed_math.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.

/// "There's a huge sea monster on the ship!": the deck shakes until the
/// Kraken is defeated (set in ship_rooms.c).
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

// The append service registers these borrowed crew tables for later talk
// lookups, but does not write them. Its mutable API therefore needs a cast
// at each call; the tables themselves remain const.

/// A speaker: the actor, with the speaker bits for where its message
/// window and portrait open.
#define SPEAKER(actorId, bits) ((bits) | (actorId))

/// Who is who on deck, numbering each actor placement table's rows from
/// ACTOR_FIRST_PLACED. Every table starts with the same twelve rows of oars,
/// and the events add their actors after a table's last row
/// (Event_CallWithLastActiveActorId), so each stage of the voyage has
/// its own names, which carry its prefix. At Kalay
/// (gTolbiShipDeckActorPlacementsWaiting) the captain talks the delay over
/// with Kaja, the deck's fighter, and a crewman, who asks Kaja how the oarsmen
/// are ("Say, Kaja, how are the oarsmen doing?"). At sea
/// gTolbiShipDeck_CrewActorPlacements places Kaja and the warriors Ouranos
/// and Sean, who help him guard the deck ("That'll be me, Ouranos, and Sean on
/// deck").
enum TolbiShipDeckActorId {
  /// Four oars, with the same sprite as the others, that keep the strokes of
  /// gTolbiShipDeckRowerStrokes, one each from 1 (the oar left still) to 4
  /// (fastest), drawn under the deck. Each oar at the sides copies the frames
  /// of its seat's stroke (TolbiShipDeck_SwingOar); at rest the first stands in
  /// for the first oar.
  /// ★ Named from what the player sees: an oar dipping into the water (its
  /// render)
  ACTOR_FIRST_STROKE_OAR = ACTOR_FIRST_PLACED,
  /// The eight oars at the ship's sides: four at x 132, then four at x 300,
  /// which are drawn mirrored (SPRITE_OAR). The rowing deck's map shows them
  /// out while the ship is rowed
  /// (https://alchemy.pascalpixel.com/places/tolbi-ship-deck_tolbi-ship-rowing/map-0.png).
  /// ★ Named from what the player sees: an oar dipping into the water (its
  /// render)
  ACTOR_FIRST_OAR = ACTOR_FIRST_STROKE_OAR + 4,

  // At Kalay: gTolbiShipDeckActorPlacements, then
  // gTolbiShipDeckActorPlacementsWaiting.
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who
  /// hid the captain's anchor charm on the mast and denies everything.
  /// ★ Named from the game: "I didn't do anything..."
  /// (MsgTolbiShipDidntDoAnything)
  ACTOR_KALAY_CREWMAN = ACTOR_FIRST_OAR + 8,
  /// One of the crew, a man in a yellow bandana (SPRITE_MAN_IN_YELLOW_BANDANA),
  /// who answers the tour guide beside him. When the ship leaves he calls
  /// "Preparations... Ready!" (MsgTolbiShipPreparationsReady).
  /// ★ Named from the game: "I told you, we're leaving soon. We'll set sail
  /// when our replacement arrives!" (MsgTolbiShipToldWereLeavingSoonSet)
  ACTOR_KALAY_CREW_MEMBER,
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  /// ★ Named from the game: "Now we can set sail. Right, Captain?"
  /// (MsgTolbiShipItsMyLuckyAnchor)
  ACTOR_KALAY_CAPTAIN,
  /// Kaja, a bald man with a beard (SPRITE_KAJA).
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  ACTOR_KALAY_KAJA,
  /// The tour group's guide, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK)
  /// as on the Tolbi Docks. Mind Read: "I just hope we don't have to refund
  /// their money at the end of the trip."
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  ACTOR_KALAY_TOUR_GUIDE,
  /// Three of the tour group's passengers, named by what they say first: a
  /// chef (SPRITE_CHEF).
  /// ★ Named from the game: "I wonder what's going on. We've been waiting an
  /// awfully long time." (MsgTolbiShipDeckIWonderWhatsGoingOnWeve)
  ACTOR_KALAY_PASSENGER_WAITING_LONG,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN).
  /// ★ Named from the game: "I wonder if I'm the only one here who's going to
  /// Tolbi to see Colosso..." (MsgTolbiShipDeckIWonderIfImTheOnly)
  ACTOR_KALAY_PASSENGER_SEEING_COLOSSO,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN).
  /// ★ Named from the game: "The Karagol is like a humongous lake, surrounded
  /// by land." (MsgTolbiShipDeckTheKaragolIsLikeAHumongous)
  ACTOR_KALAY_PASSENGER_KARAGOL,

  // At sea: gTolbiShipDeck_CrewActorPlacements after the oars, then each
  // event's own rows.
  /// ★ Named from the game: "Say, Kaja, how are the oarsmen doing?"
  /// (MsgTolbiShipWonderCouldHaveHappened)
  ACTOR_SEA_KAJA = ACTOR_FIRST_OAR + 8,
  /// Ouranos, a young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR).
  /// ★ Named from the game: "You know, my friend Ouranos here is scared of
  /// ships." (MsgTolbiShipIfWeArentGoingSet)
  ACTOR_SEA_OURANOS,
  /// Sean, a young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR).
  /// ★ Named from the game: "Well, it's just like Sean said."
  /// (MsgTolbiShipItsMyLuckyAnchor)
  ACTOR_SEA_SEAN,
  /// With ACTOR_KALAY_CREW_MEMBER's sprite. At anchor, Mind Read hears "I
  /// want to be a sailor like Kaja"; on later departures he calls
  /// "Preparations... Ready!"
  /// ★ Named from the game: "I have a job to do. I can't row!"
  /// (MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering)
  ACTOR_SEA_CREW_MEMBER,
  /// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD
  /// (gTolbiShipDeck_MonstersBoardActorPlacements): the first attack's
  /// three monsters, all of one sprite, which leap aboard and go below by the
  /// first door, where gShipRooms_FirstAttackMonsters places them.
  /// ★ Named from what the player sees: a jellyfish monster
  /// (SPRITE_JELLYFISH_MONSTER) leaping onto the deck from the sea (its
  /// render)
  ACTOR_SEA_FIRST_ATTACK_MONSTER,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK
  /// (gTolbiShipDeck_SecondAttackActorPlacements): four monsters of one
  /// sprite hover by the deck, the first two diving at Sean and the others at
  /// Ouranos.
  /// ★ Named from what the player sees: a pink winged monster
  /// (SPRITE_PINK_WINGED_MONSTER) hovering by the rail (its render)
  ACTOR_SEA_FIRST_HOVERING_MONSTER = ACTOR_SEA_CREW_MEMBER + 1,
  /// Then the second attack's three, two of the hovering monsters' sprite and
  /// one other, as gShipRooms_SecondAttackMonsters places them, come
  /// aboard and go below.
  /// ★ Named from what the player sees: two pink winged monsters and a lizard
  /// monster (SPRITE_LIZARD_MONSTER) coming aboard (its render)
  ACTOR_SEA_SECOND_ATTACK_MONSTER = ACTOR_SEA_FIRST_HOVERING_MONSTER + 4,
  /// The splash that swells for 24 frames at the rail where the last of them
  /// leaps aboard out of the sea (SPRITE_SPLASH).
  /// ★ Named from what the player sees: a spray of water at the rail (its
  /// render)
  ACTOR_SEA_SPLASH = ACTOR_SEA_SECOND_ATTACK_MONSTER + 3,
  /// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK
  /// (gTolbiShipDeck_ThirdAttackActorPlacements): four monsters with the
  /// first attack's sprite, that leap at Sean, the first two, and at Ouranos.
  /// ★ Named from what the player sees: a jellyfish monster leaping at Sean
  /// across the deck (its render)
  ACTOR_SEA_FIRST_LEAPING_MONSTER = ACTOR_SEA_CREW_MEMBER + 1,
  /// Then the third attack's three, two with the first attack's sprite and one
  /// other, as gShipRooms_ThirdAttackMonsters places them, which go
  /// below; the last drops onto the deck.
  /// ★ Named from what the player sees: two jellyfish monsters and a blue
  /// winged monster (SPRITE_BLUE_WINGED_MONSTER) going below (its render)
  ACTOR_SEA_THIRD_ATTACK_MONSTER = ACTOR_SEA_FIRST_LEAPING_MONSTER + 4,
  /// The falling monster's shadow, standing in for its own while it drops: a
  /// blank sprite (SPRITE_BLANK) draws only its shadow, which grows from a
  /// tenth of its size where the monster will land and goes the moment the
  /// monster lands and its own shadow is switched on.
  /// ★ Named from what the player sees: a shadow on the deck growing where the
  /// falling monster will land (its render)
  ACTOR_SEA_FALLING_MONSTER_SHADOW = ACTOR_SEA_THIRD_ATTACK_MONSTER + 3,
  /// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN and at anchor
  /// (gTolbiShipDeck_KrakenActorPlacements, after Garet, Ivan and Mia in
  /// their own slots): the Kraken, which the party charges; at anchor the same
  /// rows place it only for it to be released at once.
  /// ★ Named from the game: "You did good against that Kraken."
  /// (MsgTolbiShipThankIsaacDidGoodAgainst)
  ACTOR_SEA_KRAKEN = ACTOR_SEA_CREW_MEMBER + 1,
  /// An invisible actor (SPRITE_BLANK) the camera follows across the
  /// deck.
  ACTOR_INVISIBLE_SEA_CAMERA_TARGET,
  /// At anchor, back from the island
  /// (gTolbiShipDeck_TourGuideActorPlacements): the tour guide comes up
  /// from below. Kaja: "Tell our tourists they can enjoy a nice, long break
  /// after we arrive."
  /// ★ Named from the game: "The passengers are going stir crazy!"
  /// (MsgTolbiShipHowWasIsaacDidExplore)
  ACTOR_TOUR_GUIDE_AT_ANCHOR,

  // At Tolbi: gTolbiShipDeckActorPlacementsArrived.
  /// ★ Named from the game: "You lookin' for Sean and Ouranos?"
  /// (MsgTolbiShipDeckYouLookinForSeanAndOuranos)
  ACTOR_ARRIVED_KAJA = ACTOR_FIRST_OAR + 8,
  /// A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA). Mind Read: "I'm
  /// just going to hang out in Tolbi and enjoy myself for a while."
  /// ★ Named from the game: "Are you headed for Colosso, too?"
  /// (MsgTolbiShipHeadedColosso)
  ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO,
  /// With ACTOR_KALAY_CREW_MEMBER's sprite.
  /// ★ Named from the game: "This group that came on our last voyage pulled
  /// some strange stunts." (MsgTolbiShipDeckThisGroupThatCameOnOur)
  ACTOR_ARRIVED_CREW_MEMBER,
  /// The first of the nine passengers who could promise to row, in the order of
  /// each seat's group of flags (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS and the
  /// others) and of gTolbiShipDeckRowerStrokes: the tour group's two guides,
  /// the chef, the young woman in green, the bald, muscular man, the woman
  /// with a sword, the young man with brown hair, the woman with fair hair and
  /// an old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "I'm the tour group's guide!"
  /// (MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll)
  ACTOR_ARRIVED_FIRST_PASSENGER,
  /// An invisible actor (SPRITE_BLANK) the camera follows as the rowers
  /// come up.
  ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET = ACTOR_ARRIVED_FIRST_PASSENGER + 9
};

/// The deck's step triggers at Kalay, beside the ways below, whose cell
/// behaviors carry the numbers of their exits (TolbiShipDeck_GoBelow).
enum TolbiShipDeckCellBehavior {
  /// Beside the crew, where they talk over the delay.
  CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW = 10,
  /// East of the second door, which the crewman slipped through.
  CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_DOOR = 11,
  /// West of that door.
  CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_DOOR = 12
};

// Shared code, included where it sits in the ROM.
#include "shared/actor_pulse.h"

/// The door to the passengers' hold has been drawn open this visit.
/// ★ Named from the game: "I saw some warriors in among the passengers
/// belowdecks." (MsgTolbiTolbiShipThenTheyAreDangerous)
#define FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN FLAG_SCENE_TEMP_71
/// The door to the captain's quarters has been drawn open this visit.
/// ★ Named from the game: "Where else? To the captain's quarters, of course."
/// (MsgTolbiShipIfWeArentGoingSet)
#define FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN FLAG_SCENE_TEMP_72

/// An actor script call: shows the actor's sprite. Returns 0 to end the
/// call.
static s32 TolbiShipDeck_ShowSprite(struct Actor *interpreter) {
  struct Actor *a = interpreter;
  Actor_SetSpriteFlags(a, 1);
  return 0;
}

// The diving scripts own the actor's scratch fields: riseBaseY is
// sideways drift in 16.16 pixels per frame, riseCounter the dive step (0
// hovering), riseEnabled the return side, timer the drift direction and work
// the bob direction. Using the actor's fields keeps one layout for the
// whole slot.

/// The update of a sea monster hovering by Ouranos's side of the deck while the
/// monsters attack. It drifts and bobs at random; one frame in a hundred it
/// dives at Ouranos, who turns and jumps (sometimes with an emote), then it
/// returns to its side. riseEnabled picks which side it returns to.
/// Returns 1 to keep running.
static s32 TolbiShipDeck_HoverAndDiveAtOuranos(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  if (actor->riseCounter != 0) {
    switch (actor->riseCounter) {
    case 1:
      actor->speedLimit = PIXELS(4);
      actor->acceleration = PIXELS(2);
      FieldActor_SetMoveTarget(actor, PIXELS(268), PIXELS(20), PIXELS(692));
      actor->riseCounter++;
      break;
    case 2:
      actor->riseCounter++;
      break;
    case 3:
      if (actor->targetX == ACTOR_NO_TARGET &&
          actor->targetY == actor->targetX &&
          actor->targetZ == actor->targetY) {
        actor->riseCounter++;
        Event_PlaySound(SE_ENEMY_CRY);
        if (actor->riseEnabled != 0)
          Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH + FACING_STEP, 0);
        else
          Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTHWEST + FACING_STEP,
                              0);
        if (((u32)Import_RandomNext() << 2) >> 16 != 0) {
          Actor_Get(ACTOR_SEA_OURANOS)->velocityY = PIXELS(2);
        } else {
          Actor_ShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                          0);
          Actor_Get(ACTOR_SEA_OURANOS)->velocityY = PIXELS(6);
        }
      }
      break;
    case 4:
      actor->riseCounter++;
      break;
    case 5:
      if (actor->riseEnabled != 0)
        FieldActor_SetMoveTarget(actor, PIXELS(282), 0, PIXELS(658));
      else
        FieldActor_SetMoveTarget(actor, PIXELS(254), 0, PIXELS(668));
      actor->riseCounter++;
      break;
    case 6:
      actor->riseCounter++;
      break;
    case 7:
      if (actor->targetX == ACTOR_NO_TARGET &&
          actor->targetY == actor->targetX &&
          actor->targetZ == actor->targetY) {
        actor->speedLimit = PIXELS(2);
        actor->acceleration = PIXELS(1);
        actor->timer = 0;
        actor->userValue = 0;
        actor->riseCounter++;
        actor->riseBaseY = 0;
      }
      break;
    case 8:
      actor->riseCounter = 0;
      break;
    }
  } else {
    if (actor->timer != 0) {
      actor->riseBaseY -= ((u32)Import_RandomNext() << 12) >> 16;
      if (actor->riseBaseY < -PIXELS(1) / 4)
        actor->timer = 0;
    } else {
      actor->riseBaseY += ((u32)Import_RandomNext() << 12) >> 16;
      if (actor->riseBaseY > PIXELS(1) / 4)
        actor->timer = 1;
    }
    if (actor->x.fixed > PIXELS(248) && actor->x.fixed < PIXELS(292))
      actor->x.fixed += actor->riseBaseY;
    if ((s16)actor->userValue != 0) {
      actor->y.fixed = actor->y.fixed -
                       (((u32)Import_RandomNext() << 15) >> 16) - PIXELS(1) / 2;
      if (actor->y.fixed < 0)
        actor->userValue = 0;
    } else {
      actor->y.fixed = actor->y.fixed +
                       (((u32)Import_RandomNext() << 15) >> 16) + PIXELS(1) / 2;
      if (actor->y.fixed > PIXELS(8))
        actor->userValue = 1;
    }
  }
  if (((u32)Import_RandomNext() * 100) >> 16 == 0)
    actor->riseCounter = 1;
  return 1;
}

/// The same for the monster that dives at Sean.
static s32 TolbiShipDeck_HoverAndDiveAtSean(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  if (actor->riseCounter != 0) {
    switch (actor->riseCounter) {
    case 1:
      actor->speedLimit = PIXELS(4);
      actor->acceleration = PIXELS(2);
      FieldActor_SetMoveTarget(actor, PIXELS(256), PIXELS(20), PIXELS(640));
      actor->riseCounter++;
      break;
    case 2:
      actor->riseCounter++;
      break;
    case 3:
      if (actor->targetX == ACTOR_NO_TARGET &&
          actor->targetY == actor->targetX &&
          actor->targetZ == actor->targetY) {
        actor->riseCounter++;
        Event_PlaySound(SE_ENEMY_CRY);
        if (actor->riseEnabled != 0)
          Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
        else
          Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTHWEST + FACING_STEP,
                              0);
        if (((u32)Import_RandomNext() << 2) >> 16 != 0) {
          Actor_Get(ACTOR_SEA_SEAN)->velocityY = PIXELS(2);
        } else {
          Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
          Actor_Get(ACTOR_SEA_SEAN)->velocityY = PIXELS(6);
        }
      }
      break;
    case 4:
      actor->riseCounter++;
      break;
    case 5:
      if (actor->riseEnabled != 0)
        FieldActor_SetMoveTarget(actor, PIXELS(264), 0, PIXELS(600));
      else
        FieldActor_SetMoveTarget(actor, PIXELS(242), 0, PIXELS(604));
      actor->riseCounter++;
      break;
    case 6:
      actor->riseCounter++;
      break;
    case 7:
      if (actor->targetX == ACTOR_NO_TARGET &&
          actor->targetY == actor->targetX &&
          actor->targetZ == actor->targetY) {
        actor->speedLimit = PIXELS(2);
        actor->acceleration = PIXELS(1);
        actor->timer = 0;
        actor->userValue = 0;
        actor->riseCounter++;
        actor->riseBaseY = 0;
      }
      break;
    case 8:
      actor->riseCounter = 0;
      break;
    }
  } else {
    if (actor->timer != 0) {
      actor->riseBaseY -= ((u32)Import_RandomNext() << 12) >> 16;
      if (actor->riseBaseY < -PIXELS(1) / 4)
        actor->timer = 0;
    } else {
      actor->riseBaseY += ((u32)Import_RandomNext() << 12) >> 16;
      if (actor->riseBaseY > PIXELS(1) / 4)
        actor->timer = 1;
    }
    if (actor->x.fixed > PIXELS(232) && actor->x.fixed < PIXELS(272))
      actor->x.fixed += actor->riseBaseY;
    if ((s16)actor->userValue != 0) {
      actor->y.fixed = actor->y.fixed -
                       (((u32)Import_RandomNext() << 15) >> 16) - PIXELS(1) / 2;
      if (actor->y.fixed < 0)
        actor->userValue = 0;
    } else {
      actor->y.fixed = actor->y.fixed +
                       (((u32)Import_RandomNext() << 15) >> 16) + PIXELS(1) / 2;
      if (actor->y.fixed > PIXELS(8))
        actor->userValue = 1;
    }
  }
  if (((u32)Import_RandomNext() * 100) >> 16 == 0)
    actor->riseCounter = 1;
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

/// Bobs the actor between 4 and 12 pixels high by a random amount
/// each frame; the actor's work halfword says which way it is going.
/// Returns 1.
static s32 TolbiShipDeck_Bob(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s16 *flag = (s16 *)&actor->userValue;
  s32 val;
  s32 tmp;

  // The one store of the new direction matches only through these jumps;
  // written as two stores, their constants load from the literal pool.
  if (*flag != 0) {
    val = actor->y.fixed - (((u32)(Import_RandomNext() << 15)) >> 16) -
          PIXELS(1) / 2;
    actor->y.fixed = val;
    if (val >= PIXELS(4))
      goto done;
    tmp = 0;
  } else {
    val = actor->y.fixed + (((u32)(Import_RandomNext() << 15)) >> 16) +
          PIXELS(1) / 2;
    actor->y.fixed = val;
    if (val <= PIXELS(12))
      goto done;
    tmp = 1;
  }
  *flag = tmp;
done:
  return 1;
}

/// Now and then (two chances in 32) turns the actor to look
/// north-northeast or north-northwest. Returns 1.
static s32 TolbiShipDeck_LookAround(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  u32 roll = ((u32)(Import_RandomNext() << 5)) >> 16;

  if (roll == 6)
    actor->facing = FACING_NORTH + FACING_STEP;
  else if (roll == 9)
    actor->facing = FACING_NORTHWEST + FACING_STEP;
  return 1;
}

/// The update of a monster on deck that, at random, leaps at Ouranos: it jumps
/// to him, he turns and jumps back (sometimes with an emote), and it lands on
/// one of two spots chosen by riseEnabled. The actor's timer holds the
/// step. Returns 1.
static s32 TolbiShipDeck_LeapAtOuranos(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  switch (actor->timer) {
  case 0:
    if ((((u32)Import_RandomNext() * 40) >> 16) == 0) {
      actor->timer++;
    }
    break;
  case 1:
    actor->timer++;
    break;
  case 2:
    actor->velocityY = PIXELS(4);
    actor->speedLimit = PIXELS(4);
    actor->acceleration = PIXELS(2);
    FieldActor_SetMoveTarget(actor, PIXELS(264), 0, PIXELS(662));
    actor->timer++;
    break;
  case 3:
    actor->timer++;
    break;
  case 4:
    if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
        actor->targetZ == actor->targetY) {
      actor->timer++;
      Sound_Play(SE_JUMP);
      if (actor->riseEnabled != 0) {
        Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTHWEST + FACING_STEP,
                            0);
      } else {
        Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_SOUTH + FACING_STEP, 0);
      }
      if (((u32)Import_RandomNext() << 2) >> 16 != 0) {
        Actor_GetById(ACTOR_SEA_OURANOS)->velocityY = PIXELS(2);
      } else {
        Actor_ShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                        0);
        Actor_GetById(ACTOR_SEA_OURANOS)->velocityY = PIXELS(6);
      }
    }
    break;
  case 5:
    actor->timer++;
    break;
  case 6:
    actor->timer++;
    actor->velocityY = PIXELS(4);
    actor->speedLimit = PIXELS(2);
    actor->acceleration = PIXELS(1);
    if (actor->riseEnabled != 0) {
      FieldActor_SetMoveTarget(actor, PIXELS(252), 0, PIXELS(646));
    } else {
      FieldActor_SetMoveTarget(actor, PIXELS(256), 0, PIXELS(686));
    }
    break;
  case 7:
    actor->timer++;
    break;
  case 8:
    if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
        actor->targetZ == actor->targetY) {
      actor->timer++;
    }
    break;
  case 9:
    actor->timer = 0;
    break;
  }
  return 1;
}

/// The same for the monster that leaps at Sean.
static s32 TolbiShipDeck_LeapAtSean(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  switch (actor->timer) {
  case 0:
    if ((((u32)Import_RandomNext() * 40) >> 16) == 0) {
      actor->timer++;
    }
    break;
  case 1:
    actor->timer++;
    break;
  case 2:
    actor->velocityY = PIXELS(4);
    actor->speedLimit = PIXELS(4);
    actor->acceleration = PIXELS(2);
    FieldActor_SetMoveTarget(actor, PIXELS(176), 0, PIXELS(696));
    actor->timer++;
    break;
  case 3:
    actor->timer++;
    break;
  case 4:
    if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
        actor->targetZ == actor->targetY) {
      actor->timer++;
      Sound_Play(SE_JUMP);
      if (actor->riseEnabled != 0) {
        Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
      } else {
        Actor_ArmCallback(ACTOR_SEA_SEAN, FACING_EAST, 0);
      }
      if (((u32)Import_RandomNext() << 2) >> 16 != 0) {
        Actor_GetById(ACTOR_SEA_SEAN)->velocityY = PIXELS(2);
      } else {
        Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
        Actor_GetById(ACTOR_SEA_SEAN)->velocityY = PIXELS(6);
      }
    }
    break;
  case 5:
    actor->timer++;
    break;
  case 6:
    actor->timer++;
    actor->velocityY = PIXELS(4);
    actor->speedLimit = PIXELS(2);
    actor->acceleration = PIXELS(1);
    if (actor->riseEnabled != 0) {
      FieldActor_SetMoveTarget(actor, PIXELS(184), 0, PIXELS(672));
    } else {
      FieldActor_SetMoveTarget(actor, PIXELS(202), 0, PIXELS(692));
    }
    break;
  case 7:
    actor->timer++;
    break;
  case 8:
    if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
        actor->targetZ == actor->targetY) {
      actor->timer++;
    }
    break;
  case 9:
    actor->timer = 0;
    break;
  }
  return 1;
}

/// Makes the actor hop at random, one frame in 40 on average. Returns 1.
static s32 TolbiShipDeck_RandomlyStartActorHop(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  if ((((u32)(Import_RandomNext() * 40)) >> 16) == 0)
    actor->velocityY = PIXELS(4);
  return 1;
}

/// The update that shrinks an enlarged actor by 1/32 a frame until it is
/// back to normal size.
static void TolbiShipDeck_ShrinkToNormalSize(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  if (actor->scaleX > Q16_ONE) {
    actor->scaleX -= Q16_ONE / 32;
    actor->scaleY -= Q16_ONE / 32;
  }
}

/// Turns the actor east, northeast-by-north or southeast-by-south at
/// random intervals of 80 to 160 frames. riseCounter counts down the wait.
/// Returns 1.
static s32 TolbiShipDeck_IdleTurn(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  if (actor->riseCounter != 0) {
    actor->riseCounter--;
  } else {
    u32 roll = (u32)(Import_RandomNext() * 300) >> 16;

    if (roll > 200) {
      actor->facing = FACING_NORTH + FACING_STEP;
    } else if (roll > 100) {
      actor->facing = FACING_SOUTH + FACING_STEP;
    } else {
      actor->facing = FACING_EAST;
    }
    actor->riseCounter = ((u32)(Import_RandomNext() * 80) >> 16) + 80;
  }
  return 1;
}

const struct SceneEntrance *TolbiShipDeck_GetEntrances(void) {
  return gTolbiShipDeckEntrances;
}

const struct SceneExitRegion *TolbiShipDeck_GetRegions(void) {
  return gTolbiShipDeckRegions;
}

const u32 *TolbiShipDeck_GetExits(void) { return gTolbiShipDeckExits; }

/// The talk facing of a Kalay actor's row in
/// gTolbiShipDeckActorPlacementsWaiting.
#define KALAY_TALK_FACING(actorId)                                             \
  STORE_CONST(                                                                 \
      gTolbiShipDeckActorPlacementsWaiting[(actorId) - ACTOR_FIRST_PLACED]     \
          .talkFacing)

const struct ActorPlacement *TolbiShipDeck_GetActorPlacements(void) {
  s32 afterFirstAttack;

  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckActorPlacementsArrived;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWING))
    return gTolbiShipDeckActorPlacementsRowing;
  afterFirstAttack = GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  if (afterFirstAttack != 0)
    return gTolbiShipDeckActorPlacementsAtSea;
  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
      KALAY_TALK_FACING(ACTOR_KALAY_CREW_MEMBER) = afterFirstAttack;
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_WAITING_LONG) = 2;
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO) = 2;
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN)) {
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_WAITING_LONG) = 1;
      KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO) = 1;
    }
    return gTolbiShipDeckActorPlacementsWaiting;
  }
  return gTolbiShipDeckActorPlacements;
}

#undef KALAY_TALK_FACING

const struct Trigger *TolbiShipDeck_GetTriggers(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckTriggersArrived;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND))
    return gTolbiShipDeckTriggersAtIsland;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF))
    return gTolbiShipDeckTriggersAtSea;
  return gTolbiShipDeckTriggers;
}

/// Talking to ACTOR_KALAY_CREW_MEMBER on deck at Kalay. What he says follows
/// the delay: short with the tour guide at first, alarmed once the warriors
/// storm off to the captain, relieved once the party has agreed to protect the
/// ship. While alarmed he paces at random intervals.
static void TolbiShipDeck_TalkToCrewMember(void) {
  struct Actor *crewMember;
  s32 random;
  s32 frames;

  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Event_SetMessageId(MsgTolbiShipNowWeHaveProtectShip);
    Event_ShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Actor_RunRepeatedMotion(ACTOR_KALAY_CREW_MEMBER, 2);
      Event_SetMessageId(MsgTolbiShipSomebodyStopThem);
      Event_ShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
      crewMember = Actor_GetById(ACTOR_KALAY_CREW_MEMBER);
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
      Event_ShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
    }
  }
  Event_End();
}

/// The tour guide on deck at Kalay talks about the ship's delay. The guide has
/// the same three moods as TolbiShipDeck_TalkToCrewMember. "If we don't leave
/// soon, Colosso will start without us!"
static void TolbiShipDeck_TalkToTourGuide(void) {
  struct Actor *guide;
  s32 random;
  s32 frames;

  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Event_SetMessageId(MsgTolbiShipHaveMakeThemPromiseHelp);
    Event_ShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Actor_RunRepeatedMotion(ACTOR_KALAY_TOUR_GUIDE, 2);
      Event_SetMessageId(MsgTolbiShipTheyCantPlanningMutiny);
      Event_ShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
      guide = Actor_GetById(ACTOR_KALAY_TOUR_GUIDE);
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
  Event_AskYesNoAndShowResponse(ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, 0);
  Event_End();
}

/// Opens the first door to below deck, once a visit.
static void TolbiShipDeck_OpenFirstDoor(void) {
  if (Event_GetFlag(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN) == 0) {
    Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(30, 94, 13, 94, 1, 3);
    GameFlag_Set(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN);
  }
}

/// Opens the second door to below deck, once a visit.
static void TolbiShipDeck_OpenSecondDoor(void) {
  if (Event_GetFlag(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN) == 0) {
    Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(30, 108, 13, 108, 1, 2);
    GameFlag_Set(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN);
  }
}

/// Walking into one of the ways below deck, whose cell behaviors are
/// numbered as their exits. The doors to the passengers' hold and the captain's
/// quarters open and the leader steps through; the hatch to the cargo hold just
/// plays its sound. Leaves by the exit numbered as the cell behavior.
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
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    FieldActor_CenterAndWalk(ACTOR_PARTY_LEADER, 1, -10);
    Event_Wait(10);
  } else {
    Sound_Play(SE_MAP_EXIT);
  }
  Import_EventSetExitRequest(work->stepTrigger);
}

#if TBS_PATCH_2
/// In the German, Spanish, French and Italian editions cell behavior 2 is a
/// plain step trigger: the leader goes below only while facing west, after
/// walking onto the hatch.
static void TolbiShipDeck_GoBelowWhileFacingWest(void) {
  if ((u16)(Actor_GetById(ACTOR_PARTY_LEADER)->facing - 0x6001) <= 0x3ffe) {
    Event_Start();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 214, 664);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    TolbiShipDeck_GoBelow();
  }
}
#endif

/// The captain decides to set sail without the replacement ship. On deck at
/// Kalay, once the tour group has left Kalay (FLAG_KALAY_COLOSSO_TOUR_LEFT;
/// CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW), the captain, Kaja and the crewman
/// wonder what happened to the replacement ship, and the captain decides to set
/// sail anyway, trusting his anchor charm; the crewman runs off through the
/// second door. Plays once (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY).
static void TolbiShipDeck_WatchCrewDecideToSail(void) {
  struct Actor *record;

  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    Event_Start();
    Field_EndBusyAndReveal();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_KALAY_CREWMAN, 10);
    FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    FieldCamera_MoveTo(PIXELS(190), -1, PIXELS(708), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
    Event_SetMessageId(MsgTolbiShipWonderCouldHaveHappened);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
    Actor_FaceDirection(ACTOR_KALAY_CAPTAIN, FACING_SOUTH + FACING_STEP, 0);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_RunRepeatedMotion(ACTOR_KALAY_CREWMAN, 1);
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTHWEST + FACING_STEP);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    TolbiShipDeck_Turn(ACTOR_KALAY_KAJA, FACING_SOUTHEAST + FACING_STEP);
    Import_ActorSetAnimation(ACTOR_KALAY_KAJA, 3);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_FaceDirection(ACTOR_KALAY_CAPTAIN, FACING_WEST, 20);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    TolbiShipDeck_Turn(ACTOR_KALAY_KAJA, FACING_EAST);
    Actor_SetAnimationAndWait(ACTOR_KALAY_KAJA, 4);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    40);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH + FACING_STEP);
    Import_ActorSetAnimation(ACTOR_KALAY_KAJA, 3);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CREWMAN, 3);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    TolbiShipDeck_Turn(ACTOR_KALAY_CAPTAIN, FACING_SOUTH + FACING_STEP);
    Event_SetMessageId(MsgTolbiShipTheresNothingWeCanDo);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    Actor_ShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    FieldEvent_ShowMessageThenWait(
        SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE), 0, 20);
    Actor_ShowEmote(ACTOR_KALAY_KAJA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
    TolbiShipDeck_Turn(ACTOR_KALAY_CAPTAIN, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CAPTAIN, 3);
    FieldEvent_ShowMessageThenWait(
        SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE), 0, 20);
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
    TolbiShipDeck_Turn(ACTOR_KALAY_CAPTAIN, FACING_SOUTH + FACING_STEP);
    Import_ActorSetAnimation(ACTOR_KALAY_CAPTAIN, 4);
    TolbiShipDeck_Say(ACTOR_KALAY_CAPTAIN);
    Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_KALAY_KAJA, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_KALAY_KAJA, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_NORTH + FACING_STEP, 20);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 2);
    Event_Wait(20);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    Actor_SetAttachedEffect(ACTOR_KALAY_KAJA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_KALAY_CREWMAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
    FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    FieldCamera_MoveTo(PIXELS(182), -1, PIXELS(760), 1);
    Actor_SetSpeed(ACTOR_KALAY_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetScript(ACTOR_KALAY_KAJA, gTolbiShipDeck_KajaLeavesActions);
    Actor_SetSpeed(ACTOR_KALAY_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetScript(ACTOR_KALAY_CAPTAIN,
                    gTolbiShipDeck_CaptainStepsAsideActions);
    Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 182, 0x2f8);
    Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    60);
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH + FACING_STEP);
    FieldEvent_ShowMessageThenWait(ACTOR_KALAY_CREWMAN, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_KALAY_CREWMAN, 3);
    Actor_Jump(ACTOR_KALAY_CREWMAN, 4, 0);
    Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_SOUTHEAST + FACING_STEP,
                        40);
    FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(790), 1);
    {
      struct Actor *record = Actor_GetById(ACTOR_KALAY_CREWMAN);
      record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
    }
    Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 182, 0x30e);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 192, 0x328);
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 0x328);
    TolbiShipDeck_Turn(ACTOR_KALAY_CREWMAN, FACING_NORTH + FACING_STEP);
    Actor_RunRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
    TolbiShipDeck_OpenSecondDoor();
    Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 0x31e);
    Actor_SetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
    record = Actor_GetById(ACTOR_KALAY_CREWMAN);
    record->scaleX = 0x10000;
    record = Actor_GetById(ACTOR_KALAY_CREWMAN);
    record->scaleY = 0x10000;
    GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
    Event_End();
  }
}

/// On deck at Kalay, approaching from the east side of the door
/// (CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_DOOR): the crewman comes out,
/// protests and slips away round the party to the bow. "I didn't do
/// anything..." Runs until the party has confronted the crewman.
static void TolbiShipDeck_CatchCrewmanFromEast(void) {
  u8 bits;

  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Event_Start();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(2) / 5, PIXELS(1) / 5);
      Actor_GetById(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 232, 0x330);
      Event_Wait(1);
      bits = 1;
      {
        struct Actor *record = Actor_GetById(ACTOR_KALAY_CREWMAN);
        u8 value = record->steeringFlags;

        record->steeringFlags = value | bits;
      }
      Event_Wait(20);
      Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
      Actor_GetById(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 244, 0x324);
      Event_Wait(1);
      {
        struct Actor *record = Actor_GetById(ACTOR_KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Event_Wait(20);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(16) / 5, PIXELS(8) / 5);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 248, 0x30a);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 248, 0x2bc);
      Actor_SetPosition(ACTOR_KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Event_End();
    }
  }
}

/// The same, approaching from the west side
/// (CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_DOOR).
static void TolbiShipDeck_CatchCrewmanFromWest(void) {
  u8 bits;

  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Event_Start();
      Field_EndBusyAndReveal();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(2) / 5, PIXELS(1) / 5);
      Actor_GetById(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 202, 0x330);
      Event_Wait(1);
      bits = 1;
      {
        struct Actor *record = Actor_GetById(ACTOR_KALAY_CREWMAN);
        u8 value = record->steeringFlags;

        record->steeringFlags = value | bits;
      }
      Event_Wait(20);
      Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(ACTOR_KALAY_CREWMAN);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
      Actor_GetById(ACTOR_KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 192, 0x324);
      Event_Wait(1);
      {
        struct Actor *record = Actor_GetById(ACTOR_KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Event_Wait(20);
      Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(16) / 5, PIXELS(8) / 5);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 180, 0x30a);
      Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 180, 0x2bc);
      Actor_SetPosition(ACTOR_KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_FaceDirection(ACTOR_KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Event_End();
    }
  }
}

/// The start both of those share: the camera pans to the door, which opens, and
/// the crewman steps out and denies everything. Records the confrontation.
static void TolbiShipDeck_ConfrontCrewman(void) {
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(824), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  TolbiShipDeck_OpenSecondDoor();
  MapGrid_CopyCellsTo(30, 108, 13, 108, 1, 2);
  Event_Wait(10);
  Actor_SetPosition(ACTOR_KALAY_CREWMAN, PIXELS(216), PIXELS(800));
  Actor_SetSpeed(ACTOR_KALAY_CREWMAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 0x32e);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_KALAY_CREWMAN, 10);
  Actor_SetAnimationAndWait(ACTOR_KALAY_CREWMAN, 4);
  Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
  Actor_ShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceActor(ACTOR_KALAY_CREWMAN, ACTOR_PARTY_LEADER, 20);
  Actor_StartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
  Event_SetMessageId(MsgTolbiShipDidntDoAnything);
  FieldEvent_ShowMessageThenWait(ACTOR_KALAY_CREWMAN, 0, 20);
  FieldEffect_SpawnAttached(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                            0);
  GameFlag_Set(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT);
}

/// Sets up the scene on entry: clears FLAG_WORLD_MAP_LEFT, sets the
/// transition, starts the deck's sway at a random angle and stops the sea's
/// scroll, redraws the map grid and applies the voyage's state.
s32 TolbiShipDeck_Initialize(void) {
  void **base = gMapGridWork;
  struct MapGridLayerScroll *layer =
      &((struct MapGridScrollWork *)base[0])->layers[MAPGRID_LAYER_GROUND];

  GameFlag_Clear(FLAG_WORLD_MAP_LEFT);
  ((struct FieldWork *)base[HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID])->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  layer->speedY = 0;
  sSwayAngleX = (u16)Import_RandomNext();
  sSwayAngleY = (u16)Import_RandomNext();
  sSeaScroll[0] = 0;
  sSeaScroll[1] = 0;
  sSeaSpeed[0] = 0;
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  TolbiShipDeck_ApplyEntryState();
  return 0;
}

/// Applies the voyage's state: which cells and oars are drawn, whether the deck
/// rocks and the sea scrolls, and which event the entrance starts
/// (the entrances from TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY
/// on are the voyage's events, chained by their exits).
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
    Actor_ConfigureSpriteColor(ACTOR_ARRIVED_FIRST_PASSENGER + 1,
                               SPRITE_COLOR_RBG);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
  } else if ((flag = GameFlag_Get(FLAG_TOLBI_SHIP_ROWING)) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_SetUpOars();
    Task_AddFrame(TolbiShipDeck_RowOars, TASK_PASS_FRAME);
    sSeaScroll[1] = 0x200000;
    sSeaSpeed[1] = 0x13333;
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
  switch (gSequenceParentWork.entrance) {
  case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST:
    Actor_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority = 1;
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
    ((struct MapGridScrollWork *)gMapGridWork[0])->minX = PIXELS(65);
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

/// At Kalay while the ship waits: places the passengers by what has happened so
/// far.
static void TolbiShipDeck_PlaceWaitingPassengers(void) {
  struct Actor *actor;
  s32 flag;

  Actor_SetSpritePriority(ACTOR_KALAY_PASSENGER_KARAGOL, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_KAJA, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_CAPTAIN, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 1);
  Actor_SetSpritePriority(ACTOR_KALAY_TOUR_GUIDE, 1);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY) != 0) {
    Import_ActorSetPosition(ACTOR_KALAY_CAPTAIN, PIXELS(162), PIXELS(666));
    Actor_GetById(ACTOR_KALAY_CAPTAIN)->facing = FACING_WEST;
    Import_ActorSetPosition(ACTOR_KALAY_KAJA, 0, 0);
    Import_ActorSetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
  }
  flag = GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
  if (flag != 0) {
    Import_ActorSetPosition(ACTOR_KALAY_CREW_MEMBER, PIXELS(264), PIXELS(702));
    Actor_GetById(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_SOUTH + FACING_STEP;
    actor = Actor_GetById(ACTOR_KALAY_CREW_MEMBER);
    actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    Actor_SetScript(ACTOR_KALAY_CREW_MEMBER,
                    gTolbiShipDeck_SquashAndStretchActions);
    Import_ActorSetPosition(ACTOR_KALAY_TOUR_GUIDE, PIXELS(248), PIXELS(680));
    actor = Actor_Get(ACTOR_KALAY_TOUR_GUIDE);
    actor->timer = Math_RemainderUnsigned(Import_RandomNext(), 90) + 60;
    Actor_SetScript(ACTOR_KALAY_TOUR_GUIDE,
                    gTolbiShipDeck_SquashAndStretchActions);
    Import_ActorSetPosition(ACTOR_KALAY_CAPTAIN, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0) {
      Actor_SetPosition(ACTOR_KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_GetById(ACTOR_KALAY_CREWMAN)->facing = flag;
    }
  }
}

/// At Kalay once the anchor charm is back
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED): ACTOR_KALAY_CREW_MEMBER, the tour
/// guide and the three passengers take new places, and the crewman, the captain
/// and Kaja leave the deck.
static void TolbiShipDeck_PlacePassengersAgreedToRow(void) {
  Import_ActorSetPosition(ACTOR_KALAY_CREW_MEMBER, PIXELS(262), PIXELS(706));
  Actor_GetById(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_SOUTH + FACING_STEP;
  Import_ActorSetPosition(ACTOR_KALAY_TOUR_GUIDE, PIXELS(164), PIXELS(648));
  Actor_GetById(ACTOR_KALAY_TOUR_GUIDE)->facing = FACING_EAST;
  Actor_SetSpritePriority(ACTOR_KALAY_TOUR_GUIDE, 1);
  Import_ActorSetPosition(ACTOR_KALAY_PASSENGER_WAITING_LONG, PIXELS(198),
                          PIXELS(665));
  Actor_GetById(ACTOR_KALAY_PASSENGER_WAITING_LONG)->facing = FACING_WEST;
  Actor_SetSpritePriority(ACTOR_KALAY_PASSENGER_WAITING_LONG, 1);
  Import_ActorSetPosition(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, PIXELS(188),
                          PIXELS(678));
  Actor_GetById(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO)->facing =
      FACING_NORTHWEST + FACING_STEP;
  Import_ActorSetPosition(ACTOR_KALAY_PASSENGER_KARAGOL, PIXELS(186),
                          PIXELS(635));
  Actor_GetById(ACTOR_KALAY_PASSENGER_KARAGOL)->facing =
      FACING_SOUTH + FACING_STEP;
  Import_ActorSetPosition(ACTOR_KALAY_CAPTAIN, 0, 0);
  Import_ActorSetPosition(ACTOR_KALAY_KAJA, 0, 0);
  Import_ActorSetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
}

/// Under sail: the crew take their places, the two lookouts start their rounds
/// and, when the party keeps its position, the leader stands on
/// deck.
static void TolbiShipDeck_SetUpUnderSail(void) {
  struct Actor *record;
  u8 bits;

  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(204), PIXELS(521));
  record = Actor_GetById(ACTOR_SEA_SEAN);
  record->y.fixed = PIXELS(16);
  bits = 128;
  {
    struct Actor *record = Actor_GetById(ACTOR_SEA_SEAN);
    u8 value = record->collisionFlags;

    record->collisionFlags = value | bits;
  }
  Actor_SetSpeed(ACTOR_SEA_SEAN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_SeanPacesActions);
  {
    struct Actor *record = Actor_GetById(ACTOR_SEA_OURANOS);

    bits |= record->collisionFlags;
    record->collisionFlags = bits;
  }
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_OuranosPatrolsActions);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Event_End();
}

/// After the Kraken: as under sail, with Sean at the helm.
static void TolbiShipDeck_SetUpAfterKraken(void) {
  struct Actor *record;

  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(268), PIXELS(678));
  record = Actor_GetById(ACTOR_SEA_SEAN);
  record->facing = FACING_EAST;
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_SeanLooksAboutActions);
  {
    struct Actor *record = Actor_GetById(ACTOR_SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_OuranosPatrolsActions);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Event_End();
}

/// At anchor off the island: the crew stand where they were left, by whether
/// the party has agreed to scout the island, and entering by
/// TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS means the party is back from it.
static void TolbiShipDeck_SetUpAtAnchor(void) {
  struct Actor *record;
  s32 facing;

  ((struct MapGridScrollWork *)gMapGridWork[0])->minX = PIXELS(65);
  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_KrakenActorPlacements);
  wait(1);
  Actor_ReleaseById(ACTOR_SEA_KRAKEN);
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  facing = 192; // 192 << 6 is 0x3000, south-southeast
  record = Actor_GetById(ACTOR_SEA_CREW_MEMBER);
  record->facing = (facing << 6);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED) != 0) {
    Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(162), PIXELS(634));
    record = Actor_GetById(ACTOR_SEA_SEAN);
    record->facing = (facing << 6);
    Actor_SetPosition(ACTOR_SEA_OURANOS, PIXELS(162), PIXELS(676));
    record = Actor_GetById(ACTOR_SEA_OURANOS);
    record->facing = FACING_NORTH + FACING_STEP;
  } else {
    Import_ActorSetPosition(ACTOR_SEA_SEAN, PIXELS(160), PIXELS(652));
    record = Actor_GetById(ACTOR_SEA_SEAN);
    record->facing = (facing << 6);
    Actor_SetPosition(ACTOR_SEA_OURANOS, PIXELS(166), PIXELS(668));
    record = Actor_GetById(ACTOR_SEA_OURANOS);
    record->facing = FACING_NORTHWEST + FACING_STEP;
  }
  if (gSequenceParentWork.entrance == TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) {
    TolbiShipDeck_ReturnFromIsland();
  }
  Event_End();
}

/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY, setting sail from Kalay
/// before the party is on deck: Kaja (ACTOR_KALAY_KAJA) brings word to
/// ACTOR_KALAY_CREW_MEMBER that all is ready.
static void TolbiShipDeck_DepartFromKalay(void) {
  struct Actor *record;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetPosition(ACTOR_KALAY_CREWMAN, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_CAPTAIN, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_TOUR_GUIDE, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_WAITING_LONG, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_PASSENGER_KARAGOL, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_KALAY_KAJA, 0, 0);
  record = Actor_GetById(ACTOR_KALAY_KAJA);
  record->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_KALAY_CREW_MEMBER, PIXELS(232), PIXELS(650));
  record = Actor_GetById(ACTOR_KALAY_CREW_MEMBER);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(636), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  TolbiShipDeck_AnnouncePreparationsReady(ACTOR_KALAY_KAJA,
                                          ACTOR_KALAY_CREW_MEMBER);
}

/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY on later departures, with
/// the party aboard: Kaja brings word to ACTOR_SEA_CREW_MEMBER.
static void TolbiShipDeck_DepartWithParty(void) {
  struct Actor *record;

  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(636));
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(record, 0);
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
  record = Actor_GetById(ACTOR_SEA_KAJA);
  record->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, PIXELS(232), PIXELS(650));
  record = Actor_GetById(ACTOR_SEA_CREW_MEMBER);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  wait(1);
  TolbiShipDeck_AnnouncePreparationsReady(ACTOR_SEA_KAJA,
                                          ACTOR_SEA_CREW_MEMBER);
}

/// The messenger comes up through the first door to report, the crew member
/// waiting there calls the ship ready, and the event moves on to the next stage
/// of the voyage. "Preparations... Ready!"
static void TolbiShipDeck_AnnouncePreparationsReady(s32 messenger,
                                                    s32 crewMember) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(messenger, PIXELS(216), PIXELS(588));
  Actor_SetSpeed(messenger, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(messenger, 216, 0x258);
  Actor_WalkToAndWait(messenger, 218, 0x25c);
  Actor_WalkToAndWait(messenger, 234, 0x25c);
  Actor_WalkToAndWait(messenger, 236, 0x26a);
  Actor_FaceDirection(messenger, FACING_SOUTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(messenger, 3);
  Event_Wait(20);
  TolbiShipDeck_Turn(crewMember, FACING_SOUTH + FACING_STEP);
  Actor_Jump(crewMember, 4, 40);
  Actor_StartRepeatedMotion(crewMember, 2);
  Event_SetMessageId(MsgTolbiShipPreparationsReady);
  FieldEvent_ShowMessageThenWait(crewMember, 0, 20);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR);
}

/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS: sea monsters are
/// sighted. "Aye, Captain! Sea monsters!"
static void TolbiShipDeck_SightSeaMonsters(void) {
  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipAyeCaptainSeaMonsters);
  Event_ShowMessageThenWait(ACTOR_SEA_KAJA, 0, 10);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_SOUTH + FACING_STEP);
  Actor_Jump(ACTOR_SEA_SEAN, 4, 20);
  Actor_StartRepeatedMotion(ACTOR_SEA_SEAN, 2);
  Event_ShowMessageThenWait(
      SPEAKER(ACTOR_SEA_SEAN, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT), 0,
      20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_TURN);
}

/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD: the monsters leap
/// aboard and someone cries out. "It's no use. It's too late!"
static void TolbiShipDeck_MonstersBoard(void) {
  s32 zero;
  const s32 *shared;

  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_MonstersBoardActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_OURANOS, PIXELS(248), PIXELS(728));
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 242, 692);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 196, 678);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 182, 654);
  Actor_RunRepeatedMotion(ACTOR_SEA_OURANOS, 2);
  Event_SetMessageId(MsgTolbiShipNoUseLate);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_OURANOS,
                            SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(12) / 5, PIXELS(6) / 5);
  FieldActor_SetDestination(ACTOR_PARTY_LEADER, 154, 609);
  Sound_Play(SE_ENEMY_CRY);
  zero = 0;
  Actor_GetById(ACTOR_SEA_FIRST_ATTACK_MONSTER)->timer = zero;
  Actor_GetById(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1)->timer = zero;
  Actor_GetById(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2)->timer = zero;
  Actor_SetPosition(ACTOR_SEA_FIRST_ATTACK_MONSTER, PIXELS(32), PIXELS(484));
  Actor_SetPosition(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, PIXELS(84),
                    PIXELS(496));
  Actor_SetPosition(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, PIXELS(16),
                    PIXELS(596));
  Actor_SetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, PIXELS(12) / 5,
                 PIXELS(6) / 5);
  Actor_SetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, PIXELS(12) / 5,
                 PIXELS(6) / 5);
  shared = gTolbiShipDeck_MonsterLeapsAboardActions;
  Actor_SetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER, shared);
  Actor_SetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, shared);
  Actor_SetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, shared);
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_ATTACK_MONSTER, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1,
                             SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2,
                             SPRITE_COLOR_BGR);
  do {
    wait(1);
  } while (Actor_GetById(ACTOR_SEA_FIRST_ATTACK_MONSTER)->timer == 0);
  TolbiShipDeck_OpenFirstDoor();
  /* FAKEMATCH(one-pass): the block sets r0 = 21 straight after the call above;
   * as a plain call, 1 of 163 instructions differ in tbs-en. */
  do {
    Actor_WalkTo(ACTOR_SEA_OURANOS, 196, 612);
  } while (0);
  Actor_RefreshSelectorById(ACTOR_SEA_FIRST_ATTACK_MONSTER);
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

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK: the monsters harry the
/// crew while more of them climb aboard.
static void TolbiShipDeck_MonstersAttackCrew(void) {

  Event_Start();
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_SecondAttackActorPlacements);
  wait(1);
  Import_ActorSetAnimation(ACTOR_SEA_SPLASH, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER), 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1), 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2), 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3), 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_SECOND_ATTACK_MONSTER), 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1), 1);
  Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(256), PIXELS(640));
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_LookAroundActions);
  Actor_SetPosition(ACTOR_SEA_OURANOS, PIXELS(268), PIXELS(692));
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_LookAroundActions);
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER, PIXELS(242), PIXELS(604));
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1, PIXELS(264),
                    PIXELS(600));
  Actor_SetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2, PIXELS(254),
                    PIXELS(668));
  Import_ActorSetPosition(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3, PIXELS(282),
                          PIXELS(658));
  Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER)->riseEnabled = 0;
  Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1)->riseEnabled = 1;
  Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2)->riseEnabled = 0;
  Actor_GetById(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3)->riseEnabled = 1;
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER,
                  gTolbiShipDeck_HoverAndDiveAtSeanActions);
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1,
                  gTolbiShipDeck_HoverAndDiveAtSeanActions);
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2,
                  gTolbiShipDeck_HoverAndDiveAtOuranosActions);
  Actor_SetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3,
                  gTolbiShipDeck_HoverAndDiveAtOuranosActions);
  Import_ActorSetPosition(ACTOR_SEA_KAJA, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(0x190);
  Actor_SetPosition(ACTOR_SEA_SECOND_ATTACK_MONSTER, PIXELS(254), PIXELS(740));
  Actor_SetPosition(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, PIXELS(24),
                    PIXELS(586));
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, PIXELS(8) / 5,
                 PIXELS(4) / 5);
  Actor_SetDestination(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 172, 0x284);
  FieldActor_MoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER, 200, 0x294);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetDestination(ACTOR_PARTY_LEADER, 174, 0x26c);
  Actor_SetPositionAndCommit(ACTOR_SEA_SECOND_ATTACK_MONSTER, 180, 0x244);
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER, gTolbiShipDeck_BobActions);
  Actor_SetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1,
                  gTolbiShipDeck_BobActions);
  Sound_Play(SE_LOG_FALL);
  Actor_SetPosition(ACTOR_SEA_SPLASH, PIXELS(134), PIXELS(594));
  Actor_SetScript(ACTOR_SEA_SPLASH, gTolbiShipDeck_SwellActions);
  Event_Wait(10);
  Actor_SetPosition(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(134),
                    PIXELS(584));
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(4), PIXELS(2));
  Actor_GetById(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2)->velocityY = PIXELS(8);
  Actor_SetPositionAndCommit(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 186, 0x264);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2), 1);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, PIXELS(2), PIXELS(1));
  Actor_SetPositionAndCommit(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 216, 0x258);
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

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK: a great shape rises from
/// the sea and a monster drops onto the deck.
static void TolbiShipDeck_MonsterLandsOnDeck(void) {
  struct Actor *monster;
  struct Actor *record;
  s32 frame;
  const s32 *sailorActions;

  Event_Start();
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(record, 0);
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_ThirdAttackActorPlacements);
  wait(1);
  Import_ActorSetPosition(ACTOR_SEA_SEAN, PIXELS(176), PIXELS(696));
  record = Actor_GetById(ACTOR_SEA_SEAN);
  record->facing = FACING_NORTH + FACING_STEP;
  Import_ActorSetPosition(ACTOR_SEA_OURANOS, PIXELS(264), PIXELS(662));
  record = Actor_GetById(ACTOR_SEA_OURANOS);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER, PIXELS(184), PIXELS(672));
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1, PIXELS(202),
                    PIXELS(692));
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2, PIXELS(252),
                    PIXELS(646));
  Actor_SetPosition(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3, PIXELS(256),
                    PIXELS(686));
  Actor_SetPosition(ACTOR_SEA_THIRD_ATTACK_MONSTER, PIXELS(172), PIXELS(632));
  Import_ActorSetPosition(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, PIXELS(256),
                          PIXELS(622));
  Actor_GetById(ACTOR_SEA_FIRST_LEAPING_MONSTER)->riseEnabled = 0;
  Actor_GetById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1)->riseEnabled = 1;
  Actor_GetById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2)->riseEnabled = 0;
  Actor_GetById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3)->riseEnabled = 2;
  Import_ActorSetPosition(ACTOR_SEA_KAJA, 0, 0);
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
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_LEAPING_MONSTER, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1,
                             SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2,
                             SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3,
                             SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_THIRD_ATTACK_MONSTER, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1,
                             SPRITE_COLOR_BGR);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(80);
  Sound_Play(SE_BEAST_CRY);
  // The shadow grows where the monster will land, then it drops from 80
  // pixels up.
  monster = Actor_GetById(ACTOR_SEA_FALLING_MONSTER_SHADOW);
  monster->scaleX = Q16_ONE / 10;
  monster->scaleY = Q16_ONE / 10;
  monster->x.fixed = PIXELS(194);
  monster->z.fixed = PIXELS(642);
  for (frame = 0; (u32)frame < 16; frame++) {
    monster->scaleX += 3 * Q16_ONE / 50;
    monster->scaleY += 3 * Q16_ONE / 50;
    wait(1);
  }
  monster = Actor_GetById(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2);
  monster->scaleX = 11 * Q16_ONE / 10;
  monster->scaleY = 11 * Q16_ONE / 10;
  monster->x.fixed = PIXELS(194);
  monster->y.fixed = PIXELS(80);
  monster->z.fixed = PIXELS(642);
  monster->facing = FACING_SOUTH + FACING_STEP;
  monster->bounce = 2 * Q16_ONE / 5;
  monster->gravity = PIXELS(2);
  Event_Wait(80);
  Sound_Play(SE_BEAST_CRY);
  Import_ActorSetPosition(ACTOR_SEA_FALLING_MONSTER_SHADOW, 0, 0);
  record = Actor_GetById(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2);
  Actor_SetSpriteFlags(record, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  monster = Actor_GetById(ACTOR_PARTY_LEADER);
  monster->motionFlags = 0;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 216, 0x264);
  Actor_SetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, PIXELS(8) / 5,
                 PIXELS(4) / 5);
  FieldActor_MoveToAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 196, 0x258);
  FieldActor_MoveToAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 216, 0x258);
  Actor_Stop(ACTOR_SEA_THIRD_ATTACK_MONSTER);
  wait(1);
  Actor_SetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER, PIXELS(8) / 5, PIXELS(4) / 5);
  sailorActions = gTolbiShipDeck_MonsterGoesBelowActions;
  Actor_SetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER, sailorActions);
  TolbiShipDeck_Turn(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2,
                     FACING_NORTH + FACING_STEP);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_SetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, sailorActions);
  Actor_Stop(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1);
  wait(1);
  Actor_SetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, PIXELS(8) / 5,
                 PIXELS(4) / 5);
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

/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN: the party jumps down to
/// the deck, gathers and charges; the battle with the Kraken begins, returning
/// to TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE.
static void TolbiShipDeck_JumpIntoKrakenBattle(void) {
  struct Actor *actor;

  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_KrakenActorPlacements);
  wait(1);
  FieldCamera_FollowActor(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 1);
  wait(1);
  Import_ActorSetAnimation(ACTOR_SEA_OURANOS, 5);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_OURANOS), 0);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_SOUTH;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(216), PIXELS(586));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
  Event_Wait(20);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
  Sound_Play(SE_BOOM);
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  Event_Wait(20);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_GetById(ACTOR_INVISIBLE_SEA_CAMERA_TARGET)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, PIXELS(2), PIXELS(1));
  Actor_ResetAndSetPosition(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 216, 612);
  Sound_Play(SE_MONSTER_ROAR);
  Actor_SetSpritePriority(ACTOR_SEA_SEAN, 2);
  Import_ActorSetAnimation(ACTOR_SEA_SEAN, 5);
  actor = Actor_GetById(ACTOR_SEA_SEAN);
  actor->velocityY = PIXELS(8);
  actor->gravity = 7 * PIXELS(1) / 10;
  actor->scaleX = 0x1a000;
  actor->scaleY = 0x1a000;
  actor->update = TolbiShipDeck_ShrinkToNormalSize;
  actor->bounce = 0x8000;
  Actor_SetSpeed(ACTOR_SEA_SEAN, PIXELS(6), PIXELS(3));
  Actor_SetPositionAndCommit(ACTOR_SEA_SEAN, 182, 0x26a);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_SEAN), 0);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 80);
  Actor_SetDestination(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 232, 564);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 250, 584);
  Actor_WalkTo(ACTOR_GARET, 240, 600);
  Actor_WalkTo(ACTOR_IVAN, 254, 600);
  Actor_SetPositionAndReset(ACTOR_MIA, 248, 616);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_ArmCallback(ACTOR_MIA, FACING_NORTH, 20);
  Sound_Play(SE_MONSTER_ROAR);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEffect_SetAttached(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 564);
  Actor_WalkTo(ACTOR_GARET, 248, 564);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_IVAN, 248, 564);
  Actor_WalkTo(ACTOR_MIA, 248, 564);
  Event_Wait(20);
  gSequenceParentWork.restartScene = (s32)&SceneId_ShipRooms;
  gSequenceParentWork.restartEntrance = SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(
      (s32)&SceneId_TolbiShipDeck,
      TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 3);
  Event_End();
}

/// TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE: Isaac is thanked for the
/// Kraken. "Thank you, Isaac. You did good against that Kraken."
static void TolbiShipDeck_ThankForKraken(void) {
  struct Actor *record;

  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, PIXELS(182), PIXELS(618));
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, PIXELS(238), PIXELS(626));
  Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(268), PIXELS(678));
  record = Actor_GetById(ACTOR_SEA_SEAN);
  record->facing = FACING_EAST;
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_SeanLooksAboutActions);
  {
    struct Actor *record = Actor_GetById(ACTOR_SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_OuranosPatrolsActions);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SEA_KAJA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 0x224);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipThankIsaacDidGoodAgainst);
  TolbiShipDeck_Say(ACTOR_SEA_KAJA);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_SOUTH + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_SEA_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_SEA_KAJA, 0, 40);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Say(ACTOR_SEA_KAJA);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, 3);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 0x258);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 216, 0x258);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 216, 0x244);
  Actor_SetPosition(ACTOR_SEA_KAJA, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  Event_End();
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED: land is sighted. "Hey, I
/// can see land!"
static void TolbiShipDeck_SightLand(void) {
  struct Actor *record;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(record, 0);
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_KAJA, PIXELS(196), PIXELS(502));
  record = Actor_GetById(ACTOR_SEA_KAJA);
  record->facing = FACING_NORTHWEST;
  Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(184), PIXELS(524));
  record = Actor_GetById(ACTOR_SEA_SEAN);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  Actor_SetSpritePriority(ACTOR_SEA_OURANOS, 1);
  Actor_SetPosition(ACTOR_SEA_OURANOS, PIXELS(184), PIXELS(632));
  record = Actor_GetById(ACTOR_SEA_OURANOS);
  record->facing = FACING_NORTHWEST + FACING_STEP;
  // UB: a u32 lvalue aliases the pointer, which keeps the facing stores ahead
  // of its load; the defined build reads it directly.
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
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, 3);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 180, 0x222);
  Actor_ArmCallback(ACTOR_SEA_OURANOS, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_SEA_OURANOS, 1);
  TolbiShipDeck_Say(ACTOR_SEA_OURANOS);
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI);
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
/// groups, the passengers (ACTOR_ARRIVED_FIRST_PASSENGER and the eight after)
/// in a fixed order of preference.
static void TolbiShipDeck_RowersDisembark(void) {
  s32 roster[4];
  s32 matched[4];
  s32 category[4];
  u32 index;
  s32 found;
  s32 actorId;
  s32 member;
  struct Actor *record;

  found = 0;
  for (index = 0; index <= 3; index++) {
    roster[index] = TolbiShipDeck_GetRowerAtSeat(index);
    category[index] = 0;
  }

  member = ACTOR_ARRIVED_FIRST_PASSENGER;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = ACTOR_ARRIVED_FIRST_PASSENGER + 1;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = ACTOR_ARRIVED_FIRST_PASSENGER + 2;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = ACTOR_ARRIVED_FIRST_PASSENGER + 4;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  if (found != 4) {
    member = ACTOR_ARRIVED_FIRST_PASSENGER + 5;
    for (index = 0; index <= 3; index++) {
      if (roster[index] == member) {
        matched[found] = member;
        category[found] = found;
        found++;
        break;
      }
    }
    if (found != 4) {
      member = ACTOR_ARRIVED_FIRST_PASSENGER + 6;
      for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
          matched[found] = member;
          category[found] = found;
          found++;
          break;
        }
      }
      if (found != 4) {
        member = ACTOR_ARRIVED_FIRST_PASSENGER + 3;
        for (index = 0; index <= 3; index++) {
          if (roster[index] == member) {
            matched[found] = member;
            category[found] = ROWER_REPLY_ROWING_MORE;
            found++;
            break;
          }
        }
        if (found != 4) {
          member = ACTOR_ARRIVED_FIRST_PASSENGER + 7;
          for (index = 0; index <= 3; index++) {
            if (roster[index] == member) {
              matched[found] = member;
              category[found] = ROWER_REPLY_LOST_AT_SEA;
              found++;
              break;
            }
          }
          if (found != 4) {
            member = ACTOR_ARRIVED_FIRST_PASSENGER + 8;
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

  Event_Start();
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Import_ActorSetPosition(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET,
                            record->x.fixed, record->z.fixed);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 1);

  TolbiShipDeck_OpenFirstDoor();

  Event_Wait(10);

  for (index = 0; index <= 3; index++) {
    actorId = matched[index];
    Actor_SetPosition(actorId, 216 << 16, 146 << 18);
    if (category[index] == ROWER_REPLY_OLD_MAN) {
      Actor_SetSpeed(actorId, PIXELS(4) / 5, PIXELS(2) / 5);
    } else {
      Actor_SetSpeed(actorId, 128 << 9, 128 << 8);
    }
    Actor_WalkToAndWait(actorId, 216, 150 << 2);
    Actor_WalkToAndWait(actorId, 192, 0x26a);
    Actor_SetPositionAndReset(actorId, 192, 164 << 2);

    switch (category[index]) {
    case ROWER_REPLY_FINALLY_LUCKY:
      FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Event_SetMessageId(MsgTolbiShipMadeFinallyLucky);
      break;
    case ROWER_REPLY_OTHERS_ALREADY:
      TolbiShipDeck_Turn(actorId, 208 << 8);
      FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Event_SetMessageId(MsgTolbiShipOtherPassengersAlready);
      break;
    case ROWER_REPLY_DONE_ROWING:
      FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Event_SetMessageId(MsgTolbiShipDontRowAnymore);
      break;
    case ROWER_REPLY_NO_WORKOUT:
      Actor_RunRepeatedMotion(actorId, 1);
      Event_SetMessageId(MsgTolbiShipOhhhHaventWorkout);
      break;
    case ROWER_REPLY_ROWING_MORE:
      Actor_SetAnimationAndWait(actorId, 3);
      Event_SetMessageId(MsgTolbiShipRowingShipMore);
      break;
    case ROWER_REPLY_LOST_AT_SEA:
      Import_ActorSetAnimation(actorId, 4);
      Event_SetMessageId(MsgTolbiShipTotallyLostOcean);
      break;
    case ROWER_REPLY_OLD_MAN:
      Actor_SetAnimationAndWait(actorId, 4);
      FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
      Event_SetMessageId(MsgTolbiShipMakeOldMan);
      break;
    default:
      break;
    }

    TolbiShipDeck_Say(actorId);
    Actor_SetScript(actorId, gTolbiShipDeck_RowerLeavesActions);
  }

  Actor_RefreshSelectorById(actorId);
  Event_Wait(40);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, 216 << 16, 146 << 18);
  wait(1);
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 150 << 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 190, 153 << 2);
  Event_SetMessageId(MsgTolbiShipThanksHardWork);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, 192 << 8, 0);

  Actor_GetById(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 128 << 10, 128 << 9);
  Actor_SetDestination(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 196, 141 << 2);

  Actor_SetSpeed(ACTOR_ARRIVED_KAJA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_ARRIVED_KAJA, 182, 0x22b);
  Actor_FaceDirection(ACTOR_ARRIVED_KAJA, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_ARRIVED_KAJA, 1);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_SetAnimationAndWait(ACTOR_ARRIVED_KAJA, 3);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_ArmCallback(ACTOR_ARRIVED_KAJA, 128 << 8, 40);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  TolbiShipDeck_Turn(ACTOR_ARRIVED_KAJA, FACING_SOUTHEAST + FACING_STEP);
  TolbiShipDeck_Say(ACTOR_ARRIVED_KAJA);
  Actor_SetAnimationAndWait(ACTOR_ARRIVED_KAJA, 3);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_ARRIVED_KAJA, 188, 512);
  Actor_ArmCallback(ACTOR_ARRIVED_KAJA, FACING_SOUTHEAST + FACING_STEP, 0);
  Import_ActorSetPosition(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 0, 0);

  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Returns the passenger (ACTOR_ARRIVED_FIRST_PASSENGER and the eight after)
/// who promised to row at a seat (0 to 3), or 0 if the seat is empty. Each seat
/// has a group of nine flags, one for each passenger.
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

#define TOLBI_SHIP_DECK_SEA_WRAP PIXELS(32)

/// The task that rocks the deck while under sail: sways the camera on two
/// random-walking angles and offsets the BG3 sea pattern, wrapping at 32
/// pixels.
static void TolbiShipDeck_RockDeck(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  s32 *camera = grid->origin;
  s32 dx = Trig_Cos(sSwayAngleX);
  s32 dy = Trig_Sin(sSwayAngleY);
  struct MapGridLayerScroll *layer;

  *camera++ += dx >> 1;
  *camera += dy;
  sSwayAngleX += (u32)(Import_RandomNext() * 3 << 7) >> 16;
  sSwayAngleY += (u32)(Import_RandomNext() << 9) >> 16;
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

/// Under sail: shows the stroke oars (ACTOR_FIRST_STROKE_OAR) and the oars at
/// the sides (ACTOR_FIRST_OAR, the right-hand ones mirrored), records each
/// side oar's resting depth and chooses their strokes.
static void TolbiShipDeck_SetUpOars(void) {
  Actor_GetById(ACTOR_FIRST_STROKE_OAR)->collisionFlags = 0;
  Actor_GetById(ACTOR_FIRST_STROKE_OAR + 1)->collisionFlags = 0;
  Actor_GetById(ACTOR_FIRST_STROKE_OAR + 2)->collisionFlags = 0;
  Actor_GetById(ACTOR_FIRST_STROKE_OAR + 3)->collisionFlags = 0;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR + 3);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 3);
  sOarDepth[0] = Actor_GetById(ACTOR_FIRST_OAR)->z.fixed;
  sOarDepth[1] = Actor_GetById(ACTOR_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = Actor_GetById(ACTOR_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = Actor_GetById(ACTOR_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 7);
  Actor_GetById(ACTOR_FIRST_OAR + 4)->scaleX = -Q16_ONE;
  Actor_GetById(ACTOR_FIRST_OAR + 5)->scaleX = -Q16_ONE;
  Actor_GetById(ACTOR_FIRST_OAR + 6)->scaleX = -Q16_ONE;
  Actor_GetById(ACTOR_FIRST_OAR + 7)->scaleX = -Q16_ONE;
  sOarDepth[4] = Actor_GetById(ACTOR_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = Actor_GetById(ACTOR_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = Actor_GetById(ACTOR_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = Actor_GetById(ACTOR_FIRST_OAR + 7)->z.fixed;
  TolbiShipDeck_ChooseOarStrokes();
}

/// Shows an oar drawn under the deck, with no collision.
static void TolbiShipDeck_ShowOar(s32 actorId) {
  struct Actor *oar = Actor_GetById(actorId);

  if (oar != 0) {
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
    if (GameFlag_Get(base + i) != 0)
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
static void TolbiShipDeck_AdvanceStroke(s32 actorId, s32 slot) {
  u16 value = sOarAngle[slot];

  if (value >= 0x6801 && value <= 0x6fff) {
    sStrokePhase[slot] += 0x70;
    Import_ActorSetAnimation(actorId, 3);
  } else if (value >= 0xe801 && value <= 0xefff) {
    sStrokePhase[slot] += 0xe0;
    Import_ActorSetAnimation(actorId, 3);
  } else if (value >= 0x7001 && value <= 0xefff) {
    sStrokePhase[slot] += 0x1c0;
    Import_ActorSetAnimation(actorId, 2);
  } else {
    sStrokePhase[slot] += 0x300;
    Import_ActorSetAnimation(actorId, 1);
  }
}

/// Swings one oar to its angle: unless flags bit 1 is set, the angle follows
/// the stroke oar whose stroke the oar takes; bit 0 marks a right-hand oar. The
/// oar's tilt (its sprite's rotation) and depth follow the angle.
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags) {
  struct Actor *actor = Actor_GetById(id);
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
    sprite->rotation = Trig_Sin(sOarAngle[slot] + 0x8000) >> 5;
    actor->z.fixed = sOarDepth[slot] - (sine << 2) - (sine << 1);
  } else {
    sine = Trig_Sin(sOarAngle[slot] + 0x8000);
    sprite->rotation = Trig_Sin(sOarAngle[slot]) >> 5;
    actor->z.fixed = sOarDepth[slot] + (sine << 2) + (sine << 1);
  }
}

/// When not under sail: the oars lie at rest, all copying the first stroke oar,
/// which stands where the leader is while they copy it and then takes the first
/// oar's place.
static void TolbiShipDeck_RestOars(void) {
  struct Actor *actor;
  u32 i;

  for (i = 0; i <= 7; i++)
    sOarAngle[i] = 0xc000;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_STROKE_OAR);
  Import_ActorSetPosition(ACTOR_FIRST_STROKE_OAR + 1, 0, 0);
  Import_ActorSetPosition(ACTOR_FIRST_STROKE_OAR + 2, 0, 0);
  Import_ActorSetPosition(ACTOR_FIRST_STROKE_OAR + 3, 0, 0);
  Import_ActorSetPosition(ACTOR_FIRST_OAR, 0, 0);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 1);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 2);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 3);
  sOarStroke[0] = 0;
  sOarStroke[1] = 0;
  sOarStroke[2] = 0;
  sOarStroke[3] = 0;
  sOarDepth[0] = Actor_GetById(ACTOR_FIRST_STROKE_OAR)->z.fixed;
  sOarDepth[1] = Actor_GetById(ACTOR_FIRST_OAR + 1)->z.fixed;
  sOarDepth[2] = Actor_GetById(ACTOR_FIRST_OAR + 2)->z.fixed;
  sOarDepth[3] = Actor_GetById(ACTOR_FIRST_OAR + 3)->z.fixed;
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 4);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 5);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 6);
  TolbiShipDeck_ShowOar(ACTOR_FIRST_OAR + 7);
  Actor_GetById(ACTOR_FIRST_OAR + 4)->scaleX = -Q16_ONE;
  Actor_GetById(ACTOR_FIRST_OAR + 5)->scaleX = -Q16_ONE;
  Actor_GetById(ACTOR_FIRST_OAR + 6)->scaleX = -Q16_ONE;
  Actor_GetById(ACTOR_FIRST_OAR + 7)->scaleX = -Q16_ONE;
  sOarStroke[4] = 0;
  sOarStroke[5] = 0;
  sOarStroke[6] = 0;
  sOarStroke[7] = 0;
  sOarDepth[4] = Actor_GetById(ACTOR_FIRST_OAR + 4)->z.fixed;
  sOarDepth[5] = Actor_GetById(ACTOR_FIRST_OAR + 5)->z.fixed;
  sOarDepth[6] = Actor_GetById(ACTOR_FIRST_OAR + 6)->z.fixed;
  sOarDepth[7] = Actor_GetById(ACTOR_FIRST_OAR + 7)->z.fixed;
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL)
    Import_ActorSetPosition(ACTOR_FIRST_STROKE_OAR, actor->x.fixed,
                            actor->z.fixed);
  wait(1);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 1, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 2, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 3, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 4, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 5, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 6, ACTOR_FIRST_STROKE_OAR);
  Actor_CopyGraphics(ACTOR_FIRST_OAR + 7, ACTOR_FIRST_STROKE_OAR);
  Actor_GetById(ACTOR_FIRST_STROKE_OAR)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 1)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 2)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 3)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 4)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 5)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 6)->retainSpriteCache = 1;
  Actor_GetById(ACTOR_FIRST_OAR + 7)->retainSpriteCache = 1;
  wait(1);
  Import_ActorSetPosition(ACTOR_FIRST_STROKE_OAR, PIXELS(132), PIXELS(632));
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
  Event_ShowMessageAndWait(speaker, 0);
  Event_Wait(10);
}

/// Turns an actor to a facing over 10 frames.
static void TolbiShipDeck_Turn(s32 actorId, s32 facing) {
  Actor_ArmCallback(actorId, facing, 10);
}

/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, after the lookout's call:
/// Ouranos thinks they have reached Tolbi, but Kaja sees the coast is not
/// Tolbi's and the ship is off course, and the crew head for an island to get
/// their bearings before Kaja goes below.
/// TOLBI_SHIP_DECK_TO_SHIP_ROOMS_ISLAND leads on below decks, toward
/// the island.
static void TolbiShipDeck_SightIsland(void) {
  Event_Start();
  sSeaScroll[0] = 0x40000;
  sSeaSpeed[0] = -0x8000;
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_CrewActorPlacements);
  wait(1);
  Actor_SetPosition(ACTOR_SEA_OURANOS, PIXELS(182), PIXELS(618));
  Actor_Get(ACTOR_SEA_OURANOS)->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_SEA_KAJA, PIXELS(218), PIXELS(516));
  Actor_Get(ACTOR_SEA_KAJA)->facing = FACING_NORTHWEST + FACING_STEP;
  Actor_SetPosition(ACTOR_SEA_SEAN, PIXELS(204), PIXELS(526));
  Actor_Get(ACTOR_SEA_SEAN)->facing = FACING_NORTHWEST + FACING_STEP;
  Actor_SetPosition(ACTOR_SEA_CREW_MEMBER, 0, 0);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 182, 0x214);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTHWEST + FACING_STEP, 40);
  Event_SetMessageId(MsgTolbiShipFinallyReachedTolbi);
  TolbiShipDeck_Say(ACTOR_SEA_OURANOS);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 2);
  Import_ActorSetAnimation(ACTOR_SEA_KAJA, 4);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Import_ActorSetAnimation(ACTOR_SEA_KAJA, 3);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
  Actor_SetAttachedEffect(ACTOR_SEA_OURANOS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SEA_SEAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 194, 0x1f4);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetSpeed(ACTOR_SEA_SEAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_SEAN, 192, 0x206);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_SEA_KAJA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 210, 0x1fc);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_NORTHWEST + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_SEA_OURANOS, 1);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_OURANOS, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT));
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, 3);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_NORTH + FACING_STEP);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_WEST, 20);
  Import_ActorSetAnimation(ACTOR_SEA_KAJA, 4);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT));
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 204, 0x218);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 0x224);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 0x250);
  Actor_WalkTo(ACTOR_SEA_KAJA, 182, 0x298);
  Event_Wait(40);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_ISLAND);
}

/// At anchor off the island, talking to Sean or Ouranos: the party is asked to
/// scout the island for the arguing tour group; Garet and Ivan may argue
/// before the party agrees (FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED).
static void TolbiShipDeck_OfferToScoutIsland(void) {
  struct Actor *leader;
  u8 bits;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkTo(ACTOR_GARET, 194, 0x280);
  Actor_WalkTo(ACTOR_IVAN, 198, 0x28e);
  Actor_WalkToAndWait(ACTOR_MIA, 194, 0x2a0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  TolbiShipDeck_Turn(ACTOR_MIA, FACING_WEST);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_EAST);
  Event_SetMessageId(MsgTolbiShipIsaacTalkedPassengersDidntTour);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  TolbiShipDeck_Turn(ACTOR_SEA_OURANOS, FACING_NORTH + FACING_STEP);
  FieldEvent_ShowMessageThenWait(ACTOR_SEA_OURANOS, 0, 40);
  Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_RunRepeatedMotion(ACTOR_SEA_SEAN, 1);
  Event_OpenMessage(ACTOR_SEA_SEAN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    TolbiShipDeck_Say(ACTOR_IVAN);
    TolbiShipDeck_Turn(ACTOR_MIA, FACING_NORTHWEST);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    TolbiShipDeck_Say(ACTOR_MIA);
    TolbiShipDeck_Turn(ACTOR_GARET, FACING_SOUTHWEST);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_OpenMessage(ACTOR_GARET, 0);
    for (;;) {
      if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_SetMessageId(MsgTolbiShipIsaacDontTalkLikeShouldnt);
        Event_OpenMessage(ACTOR_IVAN, 0);
      } else
        break;
    }
  }
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SEA_SEAN, 3);
  Event_SetMessageId(MsgTolbiShipSeeYoureGoingGoFor);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_SetSpeed(ACTOR_SEA_SEAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(1), PIXELS(1) / 2);
  Actor_GetById(ACTOR_SEA_SEAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SEA_SEAN, 162, 0x27a);
  Event_Wait(1);
  bits = ACTOR_FACES_MOTION;
  {
    struct Actor *actor = Actor_GetById(ACTOR_SEA_SEAN);
    u8 value = actor->steeringFlags;

    actor->steeringFlags = value | bits;
  }
  Actor_GetById(ACTOR_SEA_OURANOS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 162, 0x2a4);
  Event_Wait(1);
  {
    struct Actor *actor = Actor_GetById(ACTOR_SEA_OURANOS);

    bits |= actor->steeringFlags;
    actor->steeringFlags = bits;
  }
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  TolbiShipDeck_Turn(ACTOR_SEA_OURANOS, FACING_NORTH + FACING_STEP);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_WalkTo(ACTOR_GARET, 180, 0x28e);
  Actor_WalkTo(ACTOR_IVAN, 180, 0x28e);
  Actor_WalkToAndWait(ACTOR_MIA, 180, 0x28e);
  Actor_ReleaseById(ACTOR_GARET);
  Actor_ReleaseById(ACTOR_IVAN);
  Actor_ReleaseById(ACTOR_MIA);
  GameFlag_Set(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED);
  Event_End();
}

/// Back on deck from the island: on yes, the crew make ready to sail and the
/// event moves on. "How was it, Isaac? Did you explore the island
/// thoroughly?"
static void TolbiShipDeck_ReturnFromIsland(void) {
  Event_Start();
  Event_CallWithLastActiveActorId(
      (struct ActorPlacement *)gTolbiShipDeck_TourGuideActorPlacements);
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
  Actor_ShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_SEA_SEAN, 1);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_SOUTH + FACING_STEP);
  Event_SetMessageId(MsgTolbiShipHowWasIsaacDidExplore);
  Event_OpenMessage(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT));
    Event_End();
    return;
  }
  gFieldWork->message++;
  Event_AskYesNo(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT), 0);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(ACTOR_TOUR_GUIDE_AT_ANCHOR, PIXELS(216), PIXELS(588));
  Actor_SetSpeed(ACTOR_TOUR_GUIDE_AT_ANCHOR, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_TOUR_GUIDE_AT_ANCHOR, 216, 0x254);
  Actor_WalkToAndWait(ACTOR_TOUR_GUIDE_AT_ANCHOR, 188, 0x268);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_NORTH + FACING_STEP, 0);
  TolbiShipDeck_Turn(ACTOR_TOUR_GUIDE_AT_ANCHOR, FACING_SOUTH + FACING_STEP);
  Actor_Jump(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2, 0);
  Import_ActorSetAnimation(ACTOR_TOUR_GUIDE_AT_ANCHOR, 4);
  Event_ShowMessage(ACTOR_TOUR_GUIDE_AT_ANCHOR, 0);
  Actor_SetPosition(ACTOR_SEA_KAJA, PIXELS(180), PIXELS(777));
  Actor_SetSpeed(ACTOR_SEA_KAJA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 180, 0x298);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_NORTH + FACING_STEP, 0);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SEA_SEAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE_AT_ANCHOR, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  60);
  Actor_RunRepeatedMotion(ACTOR_SEA_KAJA, 1);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
  Actor_StartRepeatedMotion(ACTOR_SEA_OURANOS, 2);
  TolbiShipDeck_Say(ACTOR_SEA_OURANOS);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_SOUTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_SEA_KAJA, 3);
  TolbiShipDeck_Say(
      SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
  Actor_Jump(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2, 20);
  Import_ActorSetAnimation(ACTOR_TOUR_GUIDE_AT_ANCHOR, 4);
  TolbiShipDeck_Say(ACTOR_TOUR_GUIDE_AT_ANCHOR);
  Actor_WalkToAndWait(ACTOR_SEA_KAJA, 182, 0x280);
  Actor_FaceDirection(ACTOR_SEA_KAJA, FACING_NORTH + FACING_STEP, 0);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_BELOW));
  Actor_ShowEmote(ACTOR_TOUR_GUIDE_AT_ANCHOR,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_StartRepeatedMotion(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2);
  TolbiShipDeck_Say(ACTOR_TOUR_GUIDE_AT_ANCHOR);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, 3);
  TolbiShipDeck_Turn(ACTOR_SEA_SEAN, FACING_EAST);
  Actor_RunRepeatedMotion(ACTOR_SEA_SEAN, 1);
  TolbiShipDeck_Say(ACTOR_SEA_SEAN);
  Actor_SetSpeed(ACTOR_SEA_SEAN, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetScript(ACTOR_SEA_SEAN, gTolbiShipDeck_GoBelowActions);
  Actor_SetSpeed(ACTOR_SEA_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_SEA_OURANOS, 168, 0x278);
  Actor_SetScript(ACTOR_SEA_OURANOS, gTolbiShipDeck_GoBelowActions);
  Event_Wait(80);
  Actor_SetScript(ACTOR_TOUR_GUIDE_AT_ANCHOR, gTolbiShipDeck_GoBelowActions);
  Event_Wait(40);
  TolbiShipDeck_Turn(ACTOR_SEA_KAJA, FACING_WEST);
  TolbiShipDeck_Say(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SEA_KAJA, 3);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SWITCH_TEAMS);
}

/// Places the leader on the deck facing south, with the camera on them.
static void TolbiShipDeck_PlaceLeaderOnDeck(void) {
  struct Actor *record;

  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(676), 0);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(676));
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  record->facing = FACING_SOUTH;
  wait(1);
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
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD: a first-attack
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

/// The second attack's hovering monsters (ACTOR_SEA_FIRST_HOVERING_MONSTER):
/// their scratch fields cleared, then TolbiShipDeck_HoverAndDiveAtOuranos or
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

/// ACTOR_SEA_SPLASH: it swells from 7/10 of its size for 24 frames, then
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

/// The third attack's leaping monsters (ACTOR_SEA_FIRST_LEAPING_MONSTER):
/// their timer cleared, then TolbiShipDeck_LeapAtOuranos or
/// TolbiShipDeck_LeapAtSean each frame.
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
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(266), 0, PIXELS(703), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

// UB: the matching compiler places this writable overlay table with the
// constants. The modern build gives its talkFacing stores a mutable object.
#ifdef UBFIX
struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[] = {
#else
const struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[] = {
#endif
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
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
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeckActorPlacementsRowing[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeckActorPlacementsAtSea[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_CrewActorPlacements[] = {
    { SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(512), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(634), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(212), 0, PIXELS(521), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_TourGuideActorPlacements[] = {
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_MonstersBoardActorPlacements[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_SecondAttackActorPlacements[] = {
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
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_ThirdAttackActorPlacements[] = {
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeck_KrakenActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SCENE_TABLE_END },
};

const struct ActorPlacement gTolbiShipDeckActorPlacementsArrived[] = {
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
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
    { TRIGGERS_END },
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
    { TRIGGERS_END },
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
    { TRIGGERS_END },
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
    { TRIGGERS_END },
};
// clang-format on

/// How hard each passenger rows, from ACTOR_ARRIVED_FIRST_PASSENGER on: 1
/// leaves the oar still, 2 to 4 row ever faster.
const s32 gTolbiShipDeckRowerStrokes[] = {3, 2, 3, 1, 4, 4, 2, 1, 1};
