/// @file
/// Ship rooms (SceneId_ShipRooms): the rooms below the decks of the
/// Tolbi-bound Ship, on its voyage across the Karagol Sea from Kalay to Tolbi.
/// The deck and the mast are tolbi_ship_deck.c and the mast's own scene; the
/// voyage moves between all three.
///
/// The map is four rooms, one camera box each:
///
/// - The passengers' room, the hold where the Colosso tour group waits, down
///   the stairs from the deck. A ladder behind a railing on its west side goes
///   down to the rowing deck, a travelling Healer stands in its south-east
///   corner, and a barrel beside the stairs holds 23 coins.
/// - The cargo hold, down its own stairs in the middle of the ship, where a
///   barrel holds a Lucky Medal.
/// - The rowing deck under the passengers' room: eight oarsmen in red
///   bandanas at their seats, two by two, a drummer keeping time and a
///   crewman.
/// - The captain's cabin, where a jar holds a Nut.
///
/// In story order:
///
/// - At Kalay the ship waits for its relief from Tolbi. Walking into the
///   passengers' room once the tour group is aboard, Isaac overhears Ouranos,
///   a huge warrior bound for Colosso who "can't stand water", threaten to go
///   ashore; he and his friend Sean storm off to make the captain sail
///   (ShipRooms_WatchWarriorsStormOff). If Isaac has not yet overheard the
///   captain, Kaja and a crewman talk over the delay on deck, he hears it on
///   stepping into the cabin (ShipRooms_WatchCrewWonderAtDelay). With the
///   warriors there, the captain agrees to sail, then finds his lucky anchor
///   charm gone and will not leave without it
///   (ShipRooms_DiscussLaunchingWithoutAnchor). The crewman hid it on top of
///   the mast. Giving the Anchor Charm back (ShipRooms_ReturnLuckyAnchor) gets
///   Isaac's party asked to guard the oarsmen, and on the rowing deck Kaja
///   tells them the ship sails without its relief.
/// - The voyage: the captain raises the charm and casts off, Kaja calls the
///   oarsmen to row, and three times monsters come aboard and attack the
///   rowing deck. Isaac fights them there, and after each battle an oarsman
///   lies hurt, so Isaac must ask a passenger in the passengers' room to row
///   in his place. Each answers in his or her own way, and whoever agrees
///   follows Isaac down to the empty seat. Kaja warns that the two teams of
///   rowers must be balanced; the choices decide whether the ship keeps its
///   course.
/// - The Kraken: the ship rocks badly, and the crew calls Isaac up on deck to
///   fight a huge sea monster.
/// - If the rowers were badly chosen the ship strays off course to an island,
///   Crossbone Isle, and waits there while the tour group argues about going
///   ashore. Back on board, Kaja has the left and right teams change sides and
///   the ship sets off for Tolbi.
/// - A defeat at sea brings the party back to the passengers' room with the
///   voyage's flags cleared, to start the crossing over.
///
/// Which actors stand in each room, and which talk and search rows they have,
/// depend on the room entered and the stage of the voyage. The cutscenes are
/// played from Scene_Initialize by the entrance the other ship scenes send the
/// party in by, and share the operations of ShipRooms_RunEventCommand.

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "sounds.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "game/inventory.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "scene/tolbi_ship.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

struct Actor;

extern s32 gPushedActor_DirectionSteps[];
extern s32 gShipRooms_LeaveHold[];
extern s32 gShipRooms_LeaveCaptainsQuarters[];
extern s32 gShipRooms_AnchorCharmPulse[];
extern s32 gShipRooms_CompanionsJoinLeaderAtFront[];
extern s32 gShipRooms_CompanionsJoinLeaderByBackSeats[];
extern s32 gShipRooms_CompanionsJoinLeaderByFrontSeats[];
extern s32 gShipRooms_Tremble[];
extern s32 gShipRooms_MonsterFloat[];
extern s32 gShipRooms_GoUpstairs[];
extern s32 gShipRooms_StepOutFromSeat[];
extern s32 gShipRooms_LookAroundScript[];
extern struct Actor *gShipRooms_AnchorCharm;
extern struct SceneEntrance gShipRooms_EntrancesOther[];
extern struct SceneEntrance gShipRooms_Entrances[];
extern u32 gShipRooms_Exits[];
extern struct ActorPlacement gShipRooms_ActorPlacementsNone[];
extern struct ActorPlacement gShipRooms_CaptainsQuartersActorPlacements[];
extern struct ActorPlacement
    gShipRooms_CaptainsQuartersActorPlacementsWarriors[];
extern struct ActorPlacement
    gShipRooms_CaptainsQuartersActorPlacementsArrived[];
extern struct ActorPlacement gShipRooms_HoldActorPlacementsEmpty[];
extern struct ActorPlacement gShipRooms_HoldActorPlacements[];
extern struct ActorPlacement gShipRooms_HoldActorPlacementsAtSea[];
extern struct ActorPlacement gShipRooms_RowingDeckActorPlacements[];
extern struct ActorPlacement gShipRooms_CompanionActorPlacements[];
extern struct ActorPlacement gShipRooms_FirstAttackMonsters[];
extern struct ActorPlacement gShipRooms_SecondAttackMonsters[];
extern struct ActorPlacement gShipRooms_ThirdAttackMonsters[];
extern struct Trigger gShipRooms_TriggersOther[];
extern struct Trigger gShipRooms_HoldTriggers[];
extern struct Trigger gShipRooms_HoldTriggersChoosingRowers[];
extern struct Trigger gShipRooms_HoldTriggersAtIsland[];
extern struct Trigger gShipRooms_CaptainsQuartersTriggers[];
extern struct Trigger gShipRooms_CaptainsQuartersTriggersAtIsland[];
extern struct Trigger gShipRooms_CaptainsQuartersTriggersArrived[];
extern struct Trigger gShipRooms_RowingDeckTriggers[];
extern struct Trigger gShipRooms_RowingDeckTriggersMonstersBelowdecks[];
extern struct Trigger gShipRooms_RowingDeckTriggersKrakenOnDeck[];
extern struct Trigger gShipRooms_RowingDeckTriggersArrived[];

/// The visible anchor charm has been created this visit.
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED FLAG_SCENE_TEMP_00

/// Operations shared by the captain's quarters, passenger hold and rowing-deck
/// events (ShipRooms_RunEventCommand), with what each takes as its arg and opt.
enum ShipEventCommand {
  /// The leader and Ivan face east, Garet and Mia west; then wait opt frames.
  SHIP_FACE_PARTY_EAST_AND_WEST,
  /// The party faces the heading arg; then wait opt frames.
  SHIP_FACE_PARTY,
  /// The party nods, waiting for Mia's nod to end if arg; then opt frames.
  SHIP_PLAY_PARTY_ANIMATION,
  /// Each of the party shows its attached effect once; then wait opt frames
  /// (arg is not read).
  SHIP_SHOW_PARTY_EMOTE,
  /// The first arg oarsmen leave their seats.
  SHIP_HIDE_FIRST_OARSMEN,
  /// Actor arg rows, at animation speed opt.
  SHIP_START_ROWING_ANIMATION,
  /// Actor arg rows facing the other way, at animation speed opt.
  SHIP_START_MIRRORED_ROWING_ANIMATION,
  /// Actor arg takes its rowing pose, held still unless opt.
  SHIP_SET_SELECTED_ROWER_POSE,
  /// The screen opens, waiting for it if arg (opt is not read).
  SHIP_OPEN_SCREEN,
  /// The screen closes, then the party takes exit arg if it is not 0.
  SHIP_CLOSE_SCREEN,
  /// The companions are placed, the chosen passengers take their seats and the
  /// party stands together on the rowing deck, facing south.
  SHIP_SETUP_ROWING_PARTY,
  /// The oarsmen of the fourth seat on stand at their seats, the fourth only if
  /// arg, and the oars are put back beside the seats.
  SHIP_RESTORE_ROWING_DECK_POSES,
  /// Actor arg stands, facing south-southeast if opt, else south-southwest.
  SHIP_RESTORE_ROWER_POSE,
  SHIP_HIDE_QUARTERS_CREW,
  SHIP_HIDE_OURANOS_AND_SEAN,
  /// The view goes to the captain in his cabin, with the anchor charm if arg,
  /// opening the screen if opt.
  SHIP_SETUP_CAPTAINS_QUARTERS,
  /// Only Kaja stands on the rowing deck, by the stairs.
  SHIP_SETUP_RECRUITMENT_DECK,
  SHIP_HIDE_OARS,
  /// Five passengers take their places in the hold, and Ouranos and Sean go.
  SHIP_SETUP_PASSENGER_HOLD,
  /// The crew and oarsmen take their places as the monsters strike, actors arg
  /// and opt among them at the west seats.
  SHIP_SETUP_MONSTER_ENCOUNTER,
  /// Clears the flags arg to opt.
  SHIP_CLEAR_FLAG_RANGE,
  /// Clears every seat's chosen rower and the attacks' flags.
  SHIP_CLEAR_ROWING_SELECTIONS,
  /// The companions go and the third oarsman trembles.
  SHIP_RESTORE_TREMBLING_OARSMAN,
  SHIP_RELEASE_COMPANIONS,
  /// The camera stays where it is, its actor held still if arg.
  SHIP_RESET_CAMERA,
  /// The companions are placed, with attack arg's monsters (1 to 3).
  SHIP_PLACE_COMPANIONS_AND_MONSTERS
};

/// The nine passengers who may be called to the oars.
///
/// Their order is that of their rows in gShipRooms_HoldActorPlacementsAtSea and
/// gShipRooms_RowingDeckActorPlacements and of each seat's group of flags
/// (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + passenger).
///
/// The game gives them no names, so each is named by what they say, or by what
/// the guides call themselves.
enum TolbiShipPassenger {
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), the tour's guide.
  PASSENGER_HEAD_GUIDE,
  /// The tour's other guide, a man carrying a pack drawn as kalay.c's
  /// ACTOR_SECOND_GUIDE (child value 2).
  PASSENGER_SECOND_GUIDE,
  /// A chef (SPRITE_CHEF).
  PASSENGER_SWEATING,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN).
  PASSENGER_LADY,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN), who knows he will be
  /// chosen.
  PASSENGER_STARED_AT,
  /// A woman with a sword (SPRITE_PERSON_WITH_SWORD).
  PASSENGER_WARRIOR,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  PASSENGER_TICKET_GRUMBLER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  PASSENGER_MUSCLE_ADMIRER,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  PASSENGER_OLD_MAN,
  PASSENGER_COUNT
};

/// The captain's quarters, numbering the rows of
/// gShipRooms_CaptainsQuartersActorPlacements and
/// gShipRooms_CaptainsQuartersActorPlacementsWarriors from ACTOR_FIRST_PLACED.
///
/// The party's rows take their own slots. The captain, Kaja and the crewman
/// have the sprites they have on deck (tolbi_ship_deck.c).
enum TolbiShipCaptainsQuartersActorId {
  /// The captain, an old man with a white beard in green
  /// (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN).
  ACTOR_QUARTERS_CAPTAIN = ACTOR_FIRST_PLACED,
  /// A bald man with a beard (SPRITE_KAJA).
  ACTOR_QUARTERS_KAJA,
  /// The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who hid
  /// the captain's anchor charm.
  ACTOR_QUARTERS_CREWMAN,
  ACTOR_QUARTERS_OURANOS,
  ACTOR_QUARTERS_SEAN,
  /// PASSENGER_SECOND_GUIDE, drawn the same way.
  ACTOR_QUARTERS_SECOND_GUIDE
};

/// The hold while the ship waits at Kalay and after it reaches Tolbi, numbering
/// the rows of gShipRooms_HoldActorPlacements from ACTOR_FIRST_PLACED.
///
/// They are four of the passengers and the second guide, then the warriors
/// bound for Colosso, Ouranos and his friend Sean, the travelling healer and
/// Kaja.
enum TolbiShipHoldActorId {
  /// PASSENGER_WARRIOR.
  ACTOR_HOLD_WARRIOR = ACTOR_FIRST_PLACED,
  /// PASSENGER_TICKET_GRUMBLER.
  ACTOR_HOLD_TICKET_GRUMBLER,
  /// PASSENGER_MUSCLE_ADMIRER.
  ACTOR_HOLD_MUSCLE_ADMIRER,
  /// PASSENGER_OLD_MAN.
  ACTOR_HOLD_OLD_MAN,
  /// PASSENGER_SECOND_GUIDE.
  ACTOR_HOLD_SECOND_GUIDE,
  ACTOR_HOLD_OURANOS,
  ACTOR_HOLD_SEAN,
  /// A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE) who opens
  /// his Sanctum (ShipRooms_TalkToHealer).
  ACTOR_HOLD_HEALER,
  /// Calls the first passenger to the oars.
  ACTOR_HOLD_KAJA
};

/// The hold at sea, numbering the rows of gShipRooms_HoldActorPlacementsAtSea
/// from ACTOR_FIRST_PLACED.
///
/// They are the nine passengers in the order of enum TolbiShipPassenger, the
/// healer and Kaja. Each passenger's evidence is its enum TolbiShipPassenger
/// line.
enum TolbiShipSeaHoldActorId {
  ACTOR_SEA_HOLD_FIRST_PASSENGER = ACTOR_FIRST_PLACED,
  ACTOR_SEA_HOLD_HEAD_GUIDE = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_HEAD_GUIDE,
  ACTOR_SEA_HOLD_SECOND_GUIDE = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_SECOND_GUIDE,
  ACTOR_SEA_HOLD_SWEATING_PASSENGER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_SWEATING,
  ACTOR_SEA_HOLD_LADY = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_LADY,
  ACTOR_SEA_HOLD_STARED_AT_PASSENGER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_STARED_AT,
  ACTOR_SEA_HOLD_WARRIOR = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_WARRIOR,
  ACTOR_SEA_HOLD_TICKET_GRUMBLER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  ACTOR_SEA_HOLD_MUSCLE_ADMIRER = ACTOR_SEA_HOLD_FIRST_PASSENGER +
      PASSENGER_MUSCLE_ADMIRER,
  ACTOR_SEA_HOLD_OLD_MAN = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  ACTOR_SEA_HOLD_HEALER,
  /// Calls a replacement rower to the oars.
  ACTOR_SEA_HOLD_KAJA
};

/// The seats on the rowing deck, an oarsman and an oar to each.
#define ROWING_DECK_SEATS 8

/// The rowing deck, numbering the rows of gShipRooms_RowingDeckActorPlacements
/// from ACTOR_FIRST_PLACED.
///
/// The monsters of each attack are placed after them.
enum TolbiShipRowingDeckActorId {
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA), one of the crew
  /// by his bandana.
  ACTOR_ROWING_CREWMAN = ACTOR_FIRST_PLACED,
  /// A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA) who keeps the
  /// oarsmen's time.
  ACTOR_ROWING_DRUMMER,
  /// The eight oarsmen, men in red bandanas (SPRITE_MAN_IN_RED_BANDANA), a seat
  /// each.
  ///
  /// The seats run two by two from the north, the west seat first. The
  /// passengers chosen take the first four seats.
  ACTOR_ROWING_FIRST_OARSMAN,
  /// The nine passengers in the order of enum TolbiShipPassenger.
  ACTOR_ROWING_FIRST_PASSENGER = ACTOR_ROWING_FIRST_OARSMAN + ROWING_DECK_SEATS,
  ACTOR_ROWING_HEAD_GUIDE = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_HEAD_GUIDE,
  ACTOR_ROWING_SECOND_GUIDE = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_SECOND_GUIDE,
  ACTOR_ROWING_SWEATING_PASSENGER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_SWEATING,
  ACTOR_ROWING_LADY = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_LADY,
  ACTOR_ROWING_STARED_AT_PASSENGER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_STARED_AT,
  ACTOR_ROWING_WARRIOR = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_WARRIOR,
  ACTOR_ROWING_TICKET_GRUMBLER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_TICKET_GRUMBLER,
  ACTOR_ROWING_MUSCLE_ADMIRER = ACTOR_ROWING_FIRST_PASSENGER +
      PASSENGER_MUSCLE_ADMIRER,
  ACTOR_ROWING_OLD_MAN = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_OLD_MAN,
  /// Kaja: "Our replacement never arrived, but we're shoving off just the
  /// same."
  ACTOR_ROWING_KAJA,
  /// The eight oars, one beside each seat, hidden while the oarsmen row.
  ACTOR_ROWING_FIRST_OAR,
  /// The monsters of each attack (gShipRooms_FirstAttackMonsters and
  /// the two after it).
  ACTOR_ROWING_FIRST_MONSTER = ACTOR_ROWING_FIRST_OAR + ROWING_DECK_SEATS
};

/// Animations of the rowing deck's sprites beyond enum ActorAnimation, as
/// their animation tables and frames under graphics/sprite draw them.
enum ShipRoomsAnimation {
  /// Pulling an oar from a seat, as the oarsmen's sprite sheet
  /// (man_in_red_bandana.png) and most
  /// passengers' sprite sheets draw it.
  ANIM_ROW = 5,
  /// The tour guide's sprite sheet rowing with a pink oar in place of
  /// ANIM_ROW's brown one (man_carrying_a_pack.png).
  ANIM_SECOND_GUIDE_ROW = 6,
  /// An oarsman lying on his back, arms flung out (man_in_red_bandana.png): "An
  /// oarsman was injured by a sea monster?"
  ANIM_OARSMAN_INJURED = 6,
  /// The drummer standing with his drum at his side (man_in_green_bandana.png).
  ANIM_DRUMMER_HOLD_DRUM = 5,
  /// The drummer, seen from behind, striking his drum once, then
  /// ANIM_DRUMMER_HOLD_DRUM (man_in_green_bandana.png).
  ANIM_DRUMMER_BEAT = 7
};

/// The step triggers and searched containers below decks whose subjects are
/// more than an exit.
enum ShipRoomsCellBehavior {
  /// The rowing deck's stairs up to the passengers' room, cell (27, 8), barred
  /// while monsters are aboard.
  CELL_BEHAVIOR_SHIP_ROOMS_ROWING_DECK_STAIRS = 4,
  /// Just inside the captain's cabin door, cells (28, 42), (29, 42), (27, 43)
  /// and (29, 43), where the crew's talk of the delay plays.
  CELL_BEHAVIOR_SHIP_ROOMS_QUARTERS_CREW = 10,
  /// Across the passengers' room north of the stairs, cells (9..11, 32), where
  /// Ouranos threatens to go ashore.
  CELL_BEHAVIOR_SHIP_ROOMS_HOLD_WARRIORS = 11,
  /// The cargo hold's barrel holding a Lucky Medal, cell (10, 19).
  CELL_BEHAVIOR_SHIP_ROOMS_CRATE_HOLD_BARREL = 100,
  /// The barrel beside the passengers' room stairs holding 23 coins, cell
  /// (9, 35).
  CELL_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL = 101,
  /// The jar in the captain's cabin holding a Nut, cell (26, 40).
  CELL_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR = 102
};

/// Kaja's pacing in the captain's quarters off the island
/// (ShipRooms_PaceCaptainsQuarters), one step to each value of his userValue.
/// He looks about where he stands, then walks south-southwest and back.
enum KajaPaceStep {
  KAJA_PACE_FACE_NORTH_NORTHWEST,
  KAJA_PACE_FIRST_PAUSE,
  KAJA_PACE_FACE_EAST,
  KAJA_PACE_SECOND_PAUSE,
  KAJA_PACE_WALK_SOUTH,
  KAJA_PACE_ARRIVE_SOUTH, ///< cues KAJA_CUE_LOOK
  KAJA_PACE_THIRD_PAUSE,
  KAJA_PACE_NOD, ///< cues KAJA_CUE_NOD
  KAJA_PACE_FOURTH_PAUSE,
  KAJA_PACE_WALK_BACK, ///< cues KAJA_CUE_FACE_EAST
  KAJA_PACE_ARRIVE_BACK,
  KAJA_PACE_LAST_PAUSE,
  KAJA_PACE_REPEAT
};

/// The frames each of Kaja's pauses counts past before his next step.
enum { KAJA_PACE_PAUSE_FRAMES = 80 };

/// What Kaja's pacing cues the captain to do (ShipRooms_WatchKaja), held in
/// Kaja's sceneByte until the captain takes it; only while the captain is
/// not paused.
enum KajaPaceCue {
  KAJA_CUE_NONE,
  KAJA_CUE_LOOK,     ///< the captain turns north-northeast, to Kaja
  KAJA_CUE_NOD,      ///< the captain nods, if he turned to Kaja
  KAJA_CUE_FACE_EAST ///< the captain turns back east
};

/// ShipRooms_MoveCameraAndWait's flags, above the frames it waits
/// after in the low bits (SHIP_CAMERA_WAIT_FRAMES).
#define SHIP_CAMERA_WAIT_FRAMES 0xffff
#define SHIP_CAMERA_JUMP 0x1 ///< jump there at once rather than glide
/// Then recenter the layers on the camera (MapGrid_CenterOnFollowedPoint).
#define SHIP_CAMERA_RECENTER_LAYERS 0x01000000
#define SHIP_CAMERA_WAIT_FOR_ARRIVAL 0x10000000

static void ShipRooms_StepBesideLeader(s32 passengerId);
static s32 ShipRooms_IsSpotFree(s32 direction);
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
static void ShipRooms_InstallRowingDeckActors(u32 animationStep);
static void ShipRooms_Say(s32 speaker);
static void ShipRooms_Turn(s32 actorId, u16 facing);
static void ShipRooms_PlaceActor(s32 actorId, s32 x, s32 z, s32 facing);
static void ShipRooms_MoveCameraAndWaitFX(FIXED x, FIXED y, FIXED z, u32 flags);
/// Moves the camera to a point in whole pixels, CAMERA_KEEP keeping an axis,
/// then waits as its flags say (ShipRooms_MoveCameraAndWaitFX).
#define ShipRooms_MoveCameraAndWait(x, y, z, flags)                            \
  ShipRooms_MoveCameraAndWaitFX(EVENT_CAMERA_FIXED(x), EVENT_CAMERA_FIXED(y),  \
                                EVENT_CAMERA_FIXED(z), flags)
static void ShipRooms_RunEventCommand(s32 step, u32 arg, u32 opt);
static s32 ShipRooms_FindSeatRower(u32 seat, s32 inHold);
static void ShipRooms_HideSelectedPassengers(void);
static void ShipRooms_HideSelectedPassenger(s32 actorId, s32 firstFlag);
static void ShipRooms_StartSecondAttack(void);
static void ShipRooms_ReportMonstersBelowdecks(void);
static void ShipRooms_FindOarsmanMissing(void);
static void ShipRooms_EndSecondAttack(void);
static void ShipRooms_StartThirdAttack(void);
static void ShipRooms_EndThirdAttack(void);
static void ShipRooms_NoticeShipRocking(void);
static void ShipRooms_ShakeWhileKrakenOnDeck(void);
static void ShipRooms_ShowCaptainsQuartersBeforeIsland(void);
static void ShipRooms_ReachIsland(void);
static void ShipRooms_SwitchLeftAndRightTeams(void);
static void ShipRooms_SwapRowerSelections(s32 firstSeatFlags,
                                          s32 secondSeatFlags);

/// An actor script call that stands the actor still at the origin, with no
/// velocity and no target, once, before its script goes on.
static s32 ShipRooms_ResetActor(struct Actor *actor) {
  Actor_SelectAnimation(actor, ANIM_STAND);
  actor->x.fixed = 0;
  actor->y.fixed = 0;
  actor->z.fixed = 0;
  SET_VELOCITY(actor, 0, 0, 0);
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetX = ACTOR_NO_TARGET;
  return 0;
}

/// Floats the actor up and down between the floor and 8 pixels up, by a random
/// half pixel to a pixel each frame. The actor's userValue says which way it is
/// going, 1 down and 0 up. It runs every frame for as long as its script
/// does.
static s32 ShipRooms_Float(struct Actor *actor) {
  if ((s16)actor->userValue != 0) {
    actor->y.fixed -= Random_Below(toFIXED(1) / 2) + toFIXED(1) / 2;
    if (actor->y.fixed < 0) {
      actor->userValue = 0;
    }
  } else {
    actor->y.fixed += Random_Below(toFIXED(1) / 2) + toFIXED(1) / 2;
    if (actor->y.fixed > toFIXED(8)) {
      actor->userValue = 1;
    }
  }
  return 1;
}

/// Now and then (two chances in 64) turns the actor to face south-southeast or
/// south-southwest, every frame for as long as its script runs.
static s32 ShipRooms_LookAround(struct Actor *actor) {
  u32 choice = Random_Below(64);

  if (choice == 6) {
    actor->facing = FACING_SOUTH_SOUTHEAST;
  } else if (choice == 9) {
    actor->facing = FACING_SOUTH_SOUTHWEST;
  }
  return 1;
}

/// Counts the frames of one of Kaja's pauses in sceneCounter and moves him to
/// his next step once it passes KAJA_PACE_PAUSE_FRAMES.
static void ShipRooms_WaitThenAdvance(struct Actor *actor) {
  if (++actor->sceneCounter > KAJA_PACE_PAUSE_FRAMES) {
    actor->userValue += 1;
  }
}

/// Counts down the actor's walking time in sceneWord.
///
/// Returns 1 once the time is up or the actor has reached its target, else 0.
static s32 ShipRooms_HasArrived(struct Actor *actor) {
  if (actor->sceneWord != 0) {
    actor->sceneWord--;
  } else {
    return 1;
  }
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == ACTOR_NO_TARGET &&
      actor->targetZ == ACTOR_NO_TARGET) {
    return 1;
  }
  return 0;
}

/// The update of Kaja (ACTOR_QUARTERS_KAJA) while the ship waits off the
/// island: he paces the captain's quarters (enum KajaPaceStep), giving each
/// walk 60 frames to arrive.
static void ShipRooms_PaceCaptainsQuarters(struct Actor *actor) {
  struct Actor *captain;

  captain = Actor_Get(ACTOR_QUARTERS_CAPTAIN);
  switch ((s16)actor->userValue) {
  case KAJA_PACE_FACE_NORTH_NORTHWEST:
    actor->facing = FACING_NORTH_NORTHWEST;
    actor->userValue++;
    actor->sceneCounter = 0;
    break;
  case KAJA_PACE_FACE_EAST:
    actor->facing = FACING_EAST;
    actor->userValue++;
    actor->sceneCounter = 0;
    break;
  case KAJA_PACE_WALK_SOUTH:
    Actor_SelectAnimation(actor, ANIM_WALK);
    FieldActor_SetMoveTarget(actor, 468, 32, 632);
    actor->sceneWord = 60;
    actor->userValue++;
    break;
  case KAJA_PACE_ARRIVE_SOUTH:
    if (ShipRooms_HasArrived(actor) != 0) {
      Actor_SelectAnimation(actor, ANIM_STAND);
      actor->sceneCounter = 0;
      if (captain->paused == 0) {
        actor->sceneByte = KAJA_CUE_LOOK;
      }
      actor->userValue++;
    }
    break;
  case KAJA_PACE_NOD:
    if (captain->paused == 0) {
      Actor_SelectAnimation(actor, ANIM_NOD);
      actor->sceneByte = KAJA_CUE_NOD;
    }
    actor->userValue++;
    actor->sceneCounter = 0;
    break;
  case KAJA_PACE_WALK_BACK:
    Actor_SelectAnimation(actor, ANIM_WALK);
    FieldActor_SetMoveTarget(actor, 480, 32, 600);
    actor->sceneWord = 60;
    actor->userValue++;
    if (captain->paused == 0) {
      actor->sceneByte = KAJA_CUE_FACE_EAST;
    }
    break;
  case KAJA_PACE_ARRIVE_BACK:
    if (ShipRooms_HasArrived(actor) != 0) {
      Actor_SelectAnimation(actor, ANIM_STAND);
      actor->sceneCounter = 0;
      actor->userValue++;
    }
    break;
  case KAJA_PACE_FIRST_PAUSE:
  case KAJA_PACE_SECOND_PAUSE:
  case KAJA_PACE_THIRD_PAUSE:
  case KAJA_PACE_FOURTH_PAUSE:
  case KAJA_PACE_LAST_PAUSE:
    ShipRooms_WaitThenAdvance(actor);
    break;
  case KAJA_PACE_REPEAT:
    actor->userValue = KAJA_PACE_FACE_NORTH_NORTHWEST;
    break;
  }
}

/// The update of the captain while Kaja paces: he takes Kaja's cues (enum
/// KajaPaceCue), keeping in sceneCounter whether he has turned to Kaja.
static void ShipRooms_WatchKaja(struct Actor *captain) {
  struct Actor *kaja;

  kaja = Actor_Get(ACTOR_QUARTERS_KAJA);
  if (captain->paused != 0)
    return;
  if (kaja->sceneByte == KAJA_CUE_LOOK) {
    captain->facing = FACING_NORTH_NORTHEAST;
    captain->sceneCounter = 1;
    kaja->sceneByte = KAJA_CUE_NONE;
  } else if (kaja->sceneByte == KAJA_CUE_NOD) {
    if (captain->sceneCounter != 0)
      Actor_SelectAnimation(captain, ANIM_NOD);
    captain->sceneCounter = 0;
    kaja->sceneByte = KAJA_CUE_NONE;
  } else if (kaja->sceneByte == KAJA_CUE_FACE_EAST) {
    captain->facing = FACING_EAST;
    kaja->sceneByte = KAJA_CUE_NONE;
  }
}

/// An actor script call that hides the actor: never a talk or Psynergy
/// target, without its shadow, drawn underfoot in SPRITE_COLOR_HIDDEN, every
/// frame for as long as its script runs.
static s32 ShipRooms_Hide(struct Actor *work) {
  struct Sprite *sprite = work->sprite;

  work->collisionFlags = ACTOR_NOT_TARGETED;
  Actor_SetSpriteFlags(work, 0);
  sprite->image.priority = 1;
  sprite->shadow.priority = 1;
  work->priorityFlags = (work->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) |
                        ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteColor(work, SPRITE_COLOR_HIDDEN);
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineState.next == SCENE_SHIP_ROOMS) {
    return gShipRooms_Entrances;
  }
  return gShipRooms_EntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gShipRooms_Exits; }

/// The talk facing of an actor's row in a room's actor placements.
#define TALK_FACING_OF(placements, actorId)                                    \
  ((placements)[(actorId) - ACTOR_FIRST_PLACED].talkFacing)

/// Picks the actor placements for the room entered and the voyage's stage.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  switch (gMainRoutineState.entrance) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_HoldActorPlacements;
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
        // At the island the passengers no longer turn to the party.
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_HEAD_GUIDE) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_SWEATING_PASSENGER) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_STARED_AT_PASSENGER) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_WARRIOR) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_OLD_MAN) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_MUSCLE_ADMIRER) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_TICKET_GRUMBLER) =
            TALK_FACING_PARTY_AND_BACK;
        TALK_FACING_OF(gShipRooms_HoldActorPlacementsAtSea,
                       ACTOR_SEA_HOLD_LADY) = TALK_FACING_KEEP;
      }
      return gShipRooms_HoldActorPlacementsAtSea;
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
        TALK_FACING_OF(gShipRooms_HoldActorPlacements, ACTOR_HOLD_WARRIOR) =
            TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacements,
                       ACTOR_HOLD_SECOND_GUIDE) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacements,
                       ACTOR_HOLD_TICKET_GRUMBLER) = TALK_FACING_KEEP;
        TALK_FACING_OF(gShipRooms_HoldActorPlacements, ACTOR_HOLD_OLD_MAN) =
            TALK_FACING_KEEP;
      }
      return gShipRooms_HoldActorPlacements;
    } else {
      return gShipRooms_HoldActorPlacementsEmpty;
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
    return gShipRooms_RowingDeckActorPlacements;
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements, ACTOR_ROWING_CREWMAN) =
        TALK_FACING_KEEP;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements, ACTOR_ROWING_DRUMMER) =
        TALK_FACING_KEEP;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 1) = TALK_FACING_PARTY_AND_BACK;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 2) = TALK_FACING_KEEP;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 3) = TALK_FACING_KEEP;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 4) = TALK_FACING_KEEP;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 5) = TALK_FACING_KEEP;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 6) = TALK_FACING_PARTY_AND_BACK;
    TALK_FACING_OF(gShipRooms_RowingDeckActorPlacements,
                   ACTOR_ROWING_FIRST_OARSMAN + 7) = TALK_FACING_KEEP;
    return gShipRooms_RowingDeckActorPlacements;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gShipRooms_CaptainsQuartersActorPlacementsArrived;
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
        if (Event_GetFlag(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
          TALK_FACING_OF(gShipRooms_CaptainsQuartersActorPlacementsWarriors,
                         ACTOR_QUARTERS_KAJA) = TALK_FACING_PARTY_AND_BACK;
        }
        if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
          if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
            TALK_FACING_OF(gShipRooms_CaptainsQuartersActorPlacementsWarriors,
                           ACTOR_QUARTERS_CAPTAIN) = TALK_FACING_PARTY;
          }
        }
        return gShipRooms_CaptainsQuartersActorPlacementsWarriors;
      }
      return gShipRooms_ActorPlacementsNone;
    } else {
      return gShipRooms_CaptainsQuartersActorPlacements;
    }
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
    return gShipRooms_CaptainsQuartersActorPlacements;
    break;
  default:
    return gShipRooms_ActorPlacementsNone;
    break;
  }
}

/// Chooses the room's interactions for the current stage of the voyage.
const struct Trigger *Scene_GetTriggers(void) {
  s32 entrance = gMainRoutineState.entrance;

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
    return gShipRooms_RowingDeckTriggersMonstersBelowdecks;
  case SHIP_ROOMS_FROM_WORLD_MAP:
    return gShipRooms_RowingDeckTriggersKrakenOnDeck;
  default:
    break;
  }

  return gShipRooms_TriggersOther;
}

/// The crewman in the blue bandana, in the captain's cabin at Kalay, asks "Now
/// what? You want to see the captain too?" once Ouranos and Sean have stormed
/// in; a yes plays ShipRooms_DiscussLaunchingWithoutAnchor.
///
/// Before then he says "But we can't send the ship out without hiring some
/// warriors," and once the captain has decided to sail, "If the ship from Tolbi
/// had come, I'd feel differently... Hmmm... I wonder where that anchor charm
/// went." A no gets "Well, you don't have any business here then. Off with you
/// now!"
static void ShipRooms_TalkToCaptainsQuartersCrewman(void) {
  Event_Start();
  Field_EndBusyAndReveal();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    Event_SetMessageId(MsgTolbiShipIfShipFromTolbiHad);
    // "If the ship from Tolbi had come, I'd feel differently, but..."
    Event_ShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Event_SetMessageId(MsgTolbiShipNowWantSeeCaptainToo);
      // "Now what? You want to see the captain too?"
      Event_OpenMessage(ACTOR_QUARTERS_CREWMAN, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
        ShipRooms_DiscussLaunchingWithoutAnchor();
      } else {
        Actor_StartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
        // "Well, you don't have any business here then. Off with you now!"
        Event_ShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
        Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHEAST, 0);
      }
    } else {
      Event_SetMessageId(MsgTolbiShipButWeCantSendShip);
      // "But we can't send the ship out without hiring some warriors."
      Event_ShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
    }
  }
  Event_End();
}

/// Sean, in the captain's cabin while the charm is lost, asks "You came to ask
/// the captain to set sail, right?" A yes gets "If we had worked together, we'd
/// be out at sea right now..."; a no, "Heh heh... In that case, why don't you
/// take a tour of the ship?"
static void ShipRooms_TalkToSeanInCaptainsQuarters(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiShipYouCameAskCaptainSet);
  // "You came to ask the captain to set sail, right?"
  // Yes: "If we had worked together, we'd be out at sea right now. But..."
  // No: "Heh heh... In that case, why don't you take a tour of the..."
  Event_AskYesNo(ACTOR_QUARTERS_SEAN, 0);
  Event_End();
}

/// The captain, an old man with a white beard in his cabin, says "It's too
/// late to hire mercenaries now... There's no time" at first, and "Oh, of all
/// the bad luck...losing my lucky charm! This ship can't leave until we find
/// it!" once it is lost.
///
/// If Isaac is carrying the Anchor Charm by then, the field menu opens so that
/// he can hand it over. With the charm back he says "We don't know what might
/// happen before we arrive. Be ready for anything," and after the first attack
/// he asks "An oarsman was injured?", shakes his head and says "There's no time
/// to worry. We must escape the waters of the Karagol!"
static void ShipRooms_TalkToCaptain(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    Event_SetMessageId(MsgTolbiShipOarsmanWasInjured);
    // "An oarsman was injured?"
    ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
    Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST, 60);
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_SHAKE_HEAD);
    // "There's no time to worry..."
    ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  } else if (Event_GetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Event_SetMessageId(MsgTolbiShipWeDontKnowMightHappen);
    // "We don't know what might happen before we arrive. Be ready..."
    Event_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    Event_SetMessageId(MsgTolbiShipBadLuckLosingMyLucky);
    // "Oh, of all the bad luck...losing my lucky charm! This ship..."
    Event_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
    if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) == 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND) != 0) {
      gFieldState->fieldMenu = 1;
    }
  } else {
    Event_SetMessageId(MsgTolbiShipItsTooLateHireMercenaries);
    // "It's too late to hire mercenaries now... There's no time."
    Event_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  }
  Event_End();
}

/// The woman with a sword, a passenger in the passengers' room at Kalay, says
/// "The longer we sit here, the more time we have to think..." until the
/// Anchor Charm is back.
///
/// Then she says "These proud warriors are not going to like being told to
/// row," and asks Isaac "What do you think? You guys wanna trade jobs?" A yes
/// gets "You look so scary... You say you want to trade, but it's not that
/// easy," and she turns away: "I see. Sure, why don't you take over the
/// rowing." A no gets "I figured you'd say that. Fine. Rowing is fine with
/// me."
static void ShipRooms_TalkToWaitingWarrior(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Actor_StartRepeatedMotion(ACTOR_HOLD_WARRIOR, 2);
    Event_SetMessageId(MsgTolbiShipTheseProudWarriorsNotGoing);
    // "These proud warriors are not going to like being told to row."
    ShipRooms_Say(ACTOR_HOLD_WARRIOR);
    Actor_FaceActor(ACTOR_HOLD_WARRIOR, ACTOR_PARTY_LEADER, 10);
    // "What do you think? You guys wanna trade jobs?"
    Event_OpenMessage(ACTOR_HOLD_WARRIOR, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
      Event_Wait(40);
      // "You look so scary... You say you want to trade, but it's not..."
      ShipRooms_Say(ACTOR_HOLD_WARRIOR);
      ShipRooms_Turn(ACTOR_HOLD_WARRIOR, FACING_SOUTH_SOUTHEAST);
      // "I see. Sure, why don't you take over the rowing."
      Event_ShowMessage(ACTOR_HOLD_WARRIOR, 0);
    } else {
      gFieldState->message += 2;
      // "I figured you'd say that. Fine. Rowing is fine with me."
      Event_ShowMessage(ACTOR_HOLD_WARRIOR, 0);
      Actor_FaceDirection(ACTOR_HOLD_WARRIOR, FACING_SOUTH_SOUTHEAST, 0);
    }
  } else {
    Event_SetMessageId(MsgTolbiShipLongerWeSitHereMore);
    // "The longer we sit here, the more time we have to think. I..."
    Event_ShowMessage(ACTOR_HOLD_WARRIOR, 0);
  }
  Event_End();
}

/// The woman with fair hair, a passenger in the passengers' room at Kalay,
/// admires "a whole bunch of really muscular men on the floor below", and once
/// the Anchor Charm is back asks Isaac "Are you going to row?"
///
/// A yes gets "Hmmm. So, you have to row, even though you're a fighter, huh?";
/// a no, "Oh. Helping to protect the ship suits you better, doesn't it?"
static void ShipRooms_TalkToWaitingMuscleAdmirer(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Event_SetMessageId(MsgTolbiShipYouGoingRow);
    // "Are you going to row?"
    // Yes: "Hmmm. So, you have to row, even though you're a fighter, huh?"
    // No: "Oh. Helping to protect the ship suits you better, doesn't..."
    Event_AskYesNo(ACTOR_HOLD_MUSCLE_ADMIRER, 0);
  } else {
    Event_SetMessageId(MsgTolbiShipTheresWholeBunchReallyMuscular);
    // "There's a whole bunch of really muscular men on the floor..."
    Event_ShowMessage(ACTOR_HOLD_MUSCLE_ADMIRER, 0);
  }
  Event_End();
}

/// A passenger in the hold under sail, when an oar needs a rower.
///
/// If someone has already been chosen for the open seat, they step beside the
/// leader and follow them out while this one says chosenMessage.
/// Otherwise the party may ask this one to row (question) at the seat the
/// attacks fought off number.
///
/// The Ask...ToRow functions are this with each passenger's lines, except the
/// stared-at passenger's and the second guide's, which write it out in full.
#define TOLBI_SHIP_ASK_TO_ROW(speaker, chosenMessage, question, passenger)     \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {                       \
    s32 rowerId = ShipRooms_GetChosenOarsman();                                \
                                                                               \
    Event_Start();                                                             \
    ShipRooms_StepBesideLeader(rowerId);                                       \
    Event_SetMessageId(chosenMessage);                                         \
    ShipRooms_Say(speaker);                                                    \
    Scene_WalkIntoLeader(rowerId);                                             \
    Event_End();                                                               \
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

/// Isaac asks the tour guide, the man with a pack in the passengers' room, to
/// row after an attack, and he wails "Nooooo! You're not going to make me row,
/// are you?"
///
/// A yes gets "I've never done much physical labor, but if I must, then I guess
/// I must," and he follows Isaac. A no gets "I'm just a businessman. I'm not
/// strong enough to be of any use." Once someone else has been chosen, the
/// chosen one steps out beside Isaac and the guide says "Looks like you've been
/// chosen to help row, doesn't it? So, get on with the rowing!"
static void ShipRooms_AskHeadGuideToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_HEAD_GUIDE,
                        MsgTolbiShipLooksLikeYouveBeenChosen,
                        MsgTolbiShipNoooooNotGoing, PASSENGER_HEAD_GUIDE);
}

/// Isaac asks the chef in the passengers' room to row after an attack: "You...
/// You must be joking. You want me to row?"
///
/// A yes gets "You jerk! What do you have against me?", and he follows Isaac. A
/// no gets "Ye-Yeah. Besides, a spatula suits me better than an oar." Once
/// someone else has been chosen he laughs, "Ha ha ha! Chosen to be an oarsman,
/// huh?"
static void ShipRooms_AskSweatingPassengerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_SWEATING_PASSENGER,
                        MsgTolbiShipHaHaHaChosenOarsman,
                        MsgTolbiShipJokingWantRow, PASSENGER_SWEATING);
}

/// Isaac asks the young woman in green in the passengers' room to row after an
/// attack: "Don't tell me you're going to ask a lady to row?"
///
/// A yes gets "What? I don't believe this. You're evil! A monster! I won't
/// forget this!", and she follows Isaac. A no gets "You startled me, sir. Don't
/// frighten me like that!" Once someone else has been chosen she says "So you
/// finally picked someone, didn't you? Show your stuff!" Off the island she
/// is seasick: she trembles and says "I don't feel so well.. Don't talk to
/// me..."
static void ShipRooms_AskLadyToRow(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    Event_Start();
    Actor_SetAttachedEffect(ACTOR_SEA_HOLD_LADY,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Event_SetMessageId(MsgTolbiShipDontFeelDontTalkMe);
    // "I don't feel so well.. Don't talk to me..."
    Event_ShowMessage(ACTOR_SEA_HOLD_LADY, 0);
    Event_End();
  } else {
    TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_LADY,
                          MsgTolbiShipYouFinallyPickedSomeoneDidnt,
                          MsgTolbiShipDontTellGoing, PASSENGER_LADY);
  }
}

/// Isaac asks the bald, muscular man in the passengers' room to row after an
/// attack, and he flinches: "What? Ohhhh, noooo... Are you going to make me
/// row?"
///
/// A yes gets "Huh? You DO want me to row? Okay, okay. I'm going already...",
/// and he follows Isaac. A no gets "What? You're not going to pick me? You
/// really are a nice guy!" Once someone else has been chosen he says "It gives
/// me chills to think that could have been me." The steps are the other
/// passengers' (TOLBI_SHIP_ASK_TO_ROW), written out with a flinch first.
static void ShipRooms_AskStaredAtPassengerToRow(void) {
  s32 rowerId;

  Event_Start();
  if (Event_GetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_GetChosenOarsman();
    ShipRooms_StepBesideLeader(rowerId);
    Event_SetMessageId(MsgTolbiShipGivesMeChillsThinkCould);
    // "It gives me chills to think that could have been me."
    ShipRooms_Say(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
    Scene_WalkIntoLeader(rowerId);
  } else {
    Actor_RunRepeatedMotion(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 2);
    Event_Wait(20);
    Event_SetMessageId(MsgTolbiShipOhhhhNooooGoingMakeMe);
    // "What? Ohhhh, noooo... Are you going to make me row?"
    Event_OpenMessage(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
      // "Huh? You DO want me to row? Okay, okay. I'm going already..."
      ShipRooms_Say(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
      Scene_WalkIntoLeader(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
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
      gFieldState->message++;
      // "What? You're not going to pick me? You really are a nice guy!"
      ShipRooms_Say(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
    }
  }
  Event_End();
}

/// Isaac asks the tour's other guide, in the passengers' room, to row after an
/// attack. He laughs, "Ha ha ha. Rowing, right? Feel free to choose whomever
/// you like," then starts: "What? Me? You must be joking! Really?"
///
/// A yes gets "Awwwww! Oh, this is just too much to handle," and he follows
/// Isaac. A no gets "Phew! Don't frighten me like that!" Once someone else has
/// been chosen he says "Hey, Isaac. You've got a good eye!"
static void ShipRooms_AskSecondGuideToRow(void) {
  s32 rowerId;

  Event_Start();
  if (Event_GetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_GetChosenOarsman();
    ShipRooms_StepBesideLeader(rowerId);
    Event_SetMessageId(MsgTolbiShipIsaacYouveGotGoodEye);
    // "Hey, Isaac. You've got a good eye!"
    ShipRooms_Say(ACTOR_SEA_HOLD_SECOND_GUIDE);
    Scene_WalkIntoLeader(rowerId);
  } else {
    Event_SetMessageId(MsgTolbiShipHaHaHaRowingFeel);
    // "Ha ha ha. Rowing, right? Feel free to choose whomever you like."
    Event_ShowMessageThenWait(ACTOR_SEA_HOLD_SECOND_GUIDE, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_SEA_HOLD_SECOND_GUIDE, 1);
    // "What? Me? You must be joking! Really?"
    Event_OpenMessage(ACTOR_SEA_HOLD_SECOND_GUIDE, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
      // "Awwwww! Oh, this is just too much to handle."
      ShipRooms_Say(ACTOR_SEA_HOLD_SECOND_GUIDE);
      Scene_WalkIntoLeader(ACTOR_SEA_HOLD_SECOND_GUIDE);
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
      gFieldState->message++;
      // "Phew! Don't frighten me like that!"
      ShipRooms_Say(ACTOR_SEA_HOLD_SECOND_GUIDE);
    }
  }
  Event_End();
}

/// Isaac asks the woman with a sword in the passengers' room to row after an
/// attack: "Hey! You're not thinking of making me an oarsman, are you?"
///
/// A yes gets "Grrr... I can't believe this guy is actually choosing me...",
/// and she follows Isaac. A no gets "Heh-heh! Not picking me is a smart choice,
/// buddy." Once someone else has been chosen she says "That's who you picked to
/// be an oarsman? Good choice! ...Looks very...oarsmanly."
static void ShipRooms_AskWarriorToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_WARRIOR,
                        MsgTolbiShipThatsWhoYouPickedOarsman,
                        MsgTolbiShipNotThinkingMaking, PASSENGER_WARRIOR);
}

/// Isaac asks the young man with brown hair in the passengers' room, who
/// grumbled about the price of the tickets, to row after an attack: "What? Me?
/// An oarsman? Give me a break! Seriously?"
///
/// A yes gets "Oh, you are serious... Fine! I'll do it...", and he follows
/// Isaac. A no gets "Hahhh... You scared me so bad, my heart nearly stopped!"
/// Once someone else has been chosen he says "If you've chosen an oarsman,
/// let's get to rowing!"
static void ShipRooms_AskTicketGrumblerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(
      ACTOR_SEA_HOLD_TICKET_GRUMBLER, MsgTolbiShipIfYouveChosenOarsmanLets,
      MsgTolbiShipOarsmanGiveBreak, PASSENGER_TICKET_GRUMBLER);
}

/// Isaac asks the woman with fair hair in the passengers' room, who admired
/// the oarsmen's muscles, to row after an attack: "That poor oarsman. I feel so
/// sorry for him. What? You want me to take his place?"
///
/// A yes gets "How can you ask a young lady to take his place? It's not
/// fair!!!", and she follows Isaac. A no gets "That's right... You can't ask a
/// proper lady to do such a filthy job!" Once someone else has been chosen she
/// says "You've been chosen as an oarsman? Aye, now that's an honor!"
static void ShipRooms_AskMuscleAdmirerToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_MUSCLE_ADMIRER,
                        MsgTolbiShipYouveBeenChosenAsOarsman,
                        MsgTolbiShipPoorOarsmanFeel, PASSENGER_MUSCLE_ADMIRER);
}

/// Isaac asks the old man with a cane in the passengers' room to row after an
/// attack: "Out of all these people, you're asking a frail old man like me to
/// row?"
///
/// A yes gets "You'll regret this when I collapse from exhaustion or
/// something!", and he follows Isaac. A no gets "Th... That's right... Nobody
/// would ever choose me." Once someone else has been chosen he asks "Ho ho! Is
/// that person going to get us to Tolbi?"
static void ShipRooms_AskOldManToRow(void) {
  TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_OLD_MAN, MsgTolbiShipHoHoPersonGoingGet,
                        MsgTolbiShipPeopleAskingFrail, PASSENGER_OLD_MAN);
}

#undef TOLBI_SHIP_ASK_TO_ROW

/// Finds a placed actor within twelve pixels of the requested floor
/// position.
static struct Actor *ShipRooms_FindActorNear(s32 x, s32 z) {
  struct FieldState *state;
  struct Actor **actor;
  struct Actor *current;
  u32 i;
  s32 actorX;
  s32 actorZ;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  state = gFieldState;
  i = ACTOR_FIRST_PLACED;
  left = x - 12;
  right = x + 12;
  top = z - 12;
  bottom = z + 12;
  actor = &state->actors[ACTOR_FIRST_PLACED];
  for (; i <= ACTOR_LAST_ID; i++) {
    current = *actor++;
    actorX = current->x.part[0].integer;
    actorZ = current->z.part[0].integer;
    if (left < actorX && right > actorX && top < actorZ && bottom > actorZ)
      return current;
  }
  return 0;
}

/// The chosen oarsman steps out beside the leader, on the leader's right if
/// that spot is free, else on the left, else behind, and turns the way the
/// leader faces.
static void ShipRooms_StepBesideLeader(s32 passengerId) {
  struct Actor *leader;
  s32 direction;
  s32 blocked;
  struct Actor *passenger;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  blocked = 1;
  Actor_SetSpritePriority(passengerId, 2);
  passenger = Actor_Get(passengerId);

  passenger->priorityFlags =
      ACTOR_PRIORITY_AUTOMATIC | passenger->priorityFlags;
  // Try the leader's right, then left, then the space behind.
  direction = FACING_INDEX(FACING_SIXTEENTH(leader->facing + DEGtoANG(90)));
  if (ShipRooms_IsSpotFree(direction) != 0) {
    blocked = 0;
  }
  if (blocked != 0) {
    direction = FACING_INDEX(FACING_SIXTEENTH(leader->facing - DEGtoANG(90)));
    if (ShipRooms_IsSpotFree(direction) != 0) {
      blocked = 0;
    }
    if (blocked != 0) {
      direction =
          FACING_INDEX(FACING_SIXTEENTH(leader->facing + DEGtoANG(180)));
    }
  }
  {
    struct Actor *start = Actor_Get(ACTOR_PARTY_LEADER);

    if (start != NULL) {
      Actor_SetPositionFX(passengerId, start->x.fixed, start->z.fixed);
    }
  }
  Actor_SetSpeed(passengerId, 1.6);
  Actor_SetAnimation(passengerId, ANIM_WALK);
  {
    s32 step = gPushedActor_DirectionSteps[direction];

    Actor_SetDestinationOffset(passengerId, DIRECTION_STEP_X(step),
                               DIRECTION_STEP_Z(step));
  }
  Actor_WaitForMoveById(passengerId);
  Actor_SetAnimation(passengerId, ANIM_STAND);
  ShipRooms_Turn(passengerId, leader->facing);
}

/// Kaja rowing speaks with the window below her and her portrait on the right.
enum {
  KAJA_ROWING_SPEAKER =
      SPEAKER(ACTOR_ROWING_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT)
};

/// Returns 1 when the spot next to the leader in the given sixteenth of a turn
/// holds no actor and no wall, else 0.
static s32 ShipRooms_IsSpotFree(s32 direction) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  u32 step = gPushedActor_DirectionSteps[direction];
  s32 x = actor->x.part[0].integer + DIRECTION_STEP_X(step);
  s32 z = actor->z.part[0].integer + DIRECTION_STEP_Z(step);

  if (ShipRooms_FindActorNear(x, z) != 0) {
    return 0;
  }

  {
    struct FieldPosition point;
    point.x = toFIXED(x);
    point.y = actor->y.fixed;
    point.z = toFIXED(z);

    if (FieldActor_CheckMovementCollision(actor, &point) != 0) {
      return 0;
    }
  }

  return 1;
}

/// Returns the passenger chosen to row at the open seat, which the attacks
/// fought off number.
static s32 ShipRooms_GetChosenOarsman(void) {
  s32 seat = 0;

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    seat = 3;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    seat = 2;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    seat = 1;
  }

  return ShipRooms_FindSeatRower(seat, 1);
}

/// What a passenger at the oars says, or thinks under Mind Read, while the
/// Kraken is on deck, by the seat they were chosen for.
///
/// That is the first seat's line, the second's, or a later one's. The
/// TalkTo...AtOars and Read...MindAtOars functions are this, or
/// TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER, with each passenger's lines.
#define TOLBI_SHIP_ROWER_LINE_BY_SEAT(passenger, actorId, first, second,       \
                                      later)                                   \
  Event_Start();                                                               \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))           \
    Event_SetMessageId(first);                                                 \
  else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger)))     \
    Event_SetMessageId(second);                                                \
  else                                                                         \
    Event_SetMessageId(later);                                                 \
  Event_ShowMessage(actorId, 0);                                               \
  Event_End()

/// The same with one line for the first seat and one for any later.
#define TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(passenger, actorId, first, later) \
  Event_Start();                                                               \
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)))           \
    Event_SetMessageId(first);                                                 \
  else                                                                         \
    Event_SetMessageId(later);                                                 \
  Event_ShowMessage(actorId, 0);                                               \
  Event_End()

/// The tour guide, rowing while the Kraken is on deck, says "That thing has
/// Kaja and his men worried. It's going to be a tough fight" if he was the
/// first passenger chosen.
///
/// Chosen second he says "Monsters everywhere, and I'm stuck rowing... I
/// should have stayed home!", and chosen third, "The ship is starting to
/// list... If we don't do something, she'll sink!" Every passenger at the oars
/// but the two women in green and fair hair says the same three lines.
static void ShipRooms_TalkToHeadGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_HEAD_GUIDE, ACTOR_ROWING_HEAD_GUIDE,
                                MsgTolbiShipThingHasKajaHisMen,
                                MsgTolbiShipMonstersEverywhereImStuckRowing,
                                MsgTolbiShipShipStartingListIfWe);
}

/// The tour's other guide, rowing while the Kraken is on deck, says "That thing
/// has Kaja and his men worried..." if he was chosen first, "Monsters
/// everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
/// list..." if third, as the tour guide does.
static void ShipRooms_TalkToSecondGuideAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_SECOND_GUIDE,
                                ACTOR_ROWING_SECOND_GUIDE,
                                MsgTolbiShipThingHasKajaHisMen,
                                MsgTolbiShipMonstersEverywhereImStuckRowing,
                                MsgTolbiShipShipStartingListIfWe);
}

/// The chef, rowing while the Kraken is on deck, says "That thing has Kaja and
/// his men worried..." if he was chosen first, "Monsters everywhere, and I'm
/// stuck rowing..." if second, and "The ship is starting to list..." if third,
/// as the tour guide does.
static void ShipRooms_TalkToSweatingPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_SWEATING,
                                ACTOR_ROWING_SWEATING_PASSENGER,
                                MsgTolbiShipThingHasKajaHisMen,
                                MsgTolbiShipMonstersEverywhereImStuckRowing,
                                MsgTolbiShipShipStartingListIfWe);
}

/// The young woman in green, rowing while the Kraken is on deck, asks "How many
/// monsters are out there?" if she was the first passenger chosen, and
/// otherwise grumbles "Another monster? This isn't the first-class passage I
/// paid for!"
static void ShipRooms_TalkToLadyAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, ACTOR_ROWING_LADY, MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// The bald, muscular man, rowing while the Kraken is on deck, says "That thing
/// has Kaja and his men worried..." if he was chosen first, "Monsters
/// everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
/// list..." if third, as the tour guide does.
static void ShipRooms_TalkToStaredAtPassengerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_STARED_AT,
                                ACTOR_ROWING_STARED_AT_PASSENGER,
                                MsgTolbiShipThingHasKajaHisMen,
                                MsgTolbiShipMonstersEverywhereImStuckRowing,
                                MsgTolbiShipShipStartingListIfWe);
}

/// The woman with a sword, rowing while the Kraken is on deck, says "That thing
/// has Kaja and his men worried..." if she was chosen first, "Monsters
/// everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
/// list..." if third, as the tour guide does.
static void ShipRooms_TalkToWarriorAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_WARRIOR, ACTOR_ROWING_WARRIOR,
                                MsgTolbiShipThingHasKajaHisMen,
                                MsgTolbiShipMonstersEverywhereImStuckRowing,
                                MsgTolbiShipShipStartingListIfWe);
}

/// The young man with brown hair, rowing while the Kraken is on deck, says
/// "That thing has Kaja and his men worried..." if he was chosen first,
/// "Monsters everywhere, and I'm stuck rowing..." if second, and "The ship is
/// starting to list..." if third, as the tour guide does.
static void ShipRooms_TalkToTicketGrumblerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_TICKET_GRUMBLER,
                                ACTOR_ROWING_TICKET_GRUMBLER,
                                MsgTolbiShipThingHasKajaHisMen,
                                MsgTolbiShipMonstersEverywhereImStuckRowing,
                                MsgTolbiShipShipStartingListIfWe);
}

/// The woman with fair hair, rowing while the Kraken is on deck, asks "How
/// many monsters are out there?" if she was the first passenger chosen, and
/// otherwise grumbles "Another monster? This isn't the first-class passage I
/// paid for!", as the woman in green does.
static void ShipRooms_TalkToMuscleAdmirerAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, ACTOR_ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHowManyMonstersOutThere,
      MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/// Mind Read on the tour guide, rowing while the Kraken is on deck, hears "I
/// don't care what it takes, just hurry up and slay that beast!" if he was the
/// first passenger chosen.
///
/// Chosen second he thinks "If those monsters come back...go teach 'em another
/// lesson!", and chosen third, "The boat's rocking so much. I'm certain we're
/// doomed!" Every passenger at the oars but the two women in green and fair
/// hair thinks the same three thoughts.
static void ShipRooms_ReadHeadGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_HEAD_GUIDE, ACTOR_ROWING_HEAD_GUIDE,
                                MsgTolbiShipDontCareTakesJustHurry,
                                MsgTolbiShipIfThoseMonstersComeBack,
                                MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the tour's other guide at the oars, while the Kraken is on
/// deck, hears "I don't care what it takes, just hurry up and slay that
/// beast!", "If those monsters come back...go teach 'em another lesson!" or
/// "The boat's rocking so much. I'm certain we're doomed!", by whether he was
/// chosen first, second or third.
static void ShipRooms_ReadSecondGuideMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(
      PASSENGER_SECOND_GUIDE, ACTOR_ROWING_SECOND_GUIDE,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the chef at the oars, while the Kraken is on deck, hears "I
/// don't care what it takes, just hurry up and slay that beast!", "If those
/// monsters come back...go teach 'em another lesson!" or "The boat's rocking so
/// much. I'm certain we're doomed!", by whether he was chosen first, second or
/// third.
static void ShipRooms_ReadSweatingPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(
      PASSENGER_SWEATING, ACTOR_ROWING_SWEATING_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the young woman in green at the oars, while the Kraken is on
/// deck, hears "I had no idea there were so many monsters out at sea..." if
/// she was the first passenger chosen, and otherwise "We're surrounded by
/// monsters, and still there's no sign of land!"
static void ShipRooms_ReadLadyMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_LADY, ACTOR_ROWING_LADY, MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

/// Mind Read on the bald, muscular man at the oars, while the Kraken is on
/// deck, hears "I don't care what it takes, just hurry up and slay that
/// beast!", "If those monsters come back...go teach 'em another lesson!" or
/// "The boat's rocking so much. I'm certain we're doomed!", by whether he was
/// chosen first, second or third.
static void ShipRooms_ReadStaredAtPassengerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(
      PASSENGER_STARED_AT, ACTOR_ROWING_STARED_AT_PASSENGER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the woman with a sword at the oars, while the Kraken is on
/// deck, hears "I don't care what it takes, just hurry up and slay that
/// beast!", "If those monsters come back...go teach 'em another lesson!" or
/// "The boat's rocking so much. I'm certain we're doomed!", by whether she was
/// chosen first, second or third.
static void ShipRooms_ReadWarriorMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_WARRIOR, ACTOR_ROWING_WARRIOR,
                                MsgTolbiShipDontCareTakesJustHurry,
                                MsgTolbiShipIfThoseMonstersComeBack,
                                MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the young man with brown hair at the oars, while the Kraken is
/// on deck, hears "I don't care what it takes, just hurry up and slay that
/// beast!", "If those monsters come back...go teach 'em another lesson!" or
/// "The boat's rocking so much. I'm certain we're doomed!", by whether he was
/// chosen first, second or third.
static void ShipRooms_ReadTicketGrumblerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_BY_SEAT(
      PASSENGER_TICKET_GRUMBLER, ACTOR_ROWING_TICKET_GRUMBLER,
      MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
      MsgTolbiShipBoatsRockingMuchImCertain);
}

/// Mind Read on the woman with fair hair at the oars, while the Kraken is on
/// deck, hears "I had no idea there were so many monsters out at sea..." if
/// she was the first passenger chosen, and otherwise "We're surrounded by
/// monsters, and still there's no sign of land!"
static void ShipRooms_ReadMuscleAdmirerMindAtOars(void) {
  TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(
      PASSENGER_MUSCLE_ADMIRER, ACTOR_ROWING_MUSCLE_ADMIRER,
      MsgTolbiShipHadIdeaThereWereMany,
      MsgTolbiShipWereSurroundedByMonstersStill);
}

#undef TOLBI_SHIP_ROWER_LINE_BY_SEAT
#undef TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER

/// Asks a passenger to row.
static void ShipRooms_AskToRow(s32 speaker, s32 question, s32 seatFlag) {
  Event_Start();
  Event_SetMessageId(question);
  Event_OpenMessage(speaker, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
    ShipRooms_Say(speaker);
    Scene_WalkIntoLeader(speaker);
    GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    GameFlag_Set(seatFlag);
  } else {
    gFieldState->message++;
    ShipRooms_Say(speaker);
  }
  Event_End();
}

/// The travelling Healer, the bearded man in a long robe in the south-east
/// corner of the passengers' room, opens his Sanctum when Isaac faces him from
/// the west.
///
/// Spoken to from any other side, he says "I'm spreading goodwill wherever I
/// travel" at Kalay, "The ship's crew is ready for anything, and I'll be right
/// here for them. Ask the others to do the rowing" once the voyage is under
/// way, "I hate arguing..." off the island and "It is good that the ship has
/// arrived safely" at Tolbi.
static void ShipRooms_TalkToHealer(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->facing < FACING_SOUTHEAST || leader->facing > FACING_NORTHEAST) {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Sanctum_Run(ACTOR_SEA_HOLD_HEALER);
    } else {
      Sanctum_Run(ACTOR_HOLD_HEALER);
    }
  } else {
    Event_Start();
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      Event_SetMessageId(MsgTolbiShipGoodShipHasArrivedSafely);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      Event_SetMessageId(MsgTolbiShipHateArguing);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      Event_SetMessageId(MsgTolbiShipShipsCrewReadyAnything);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
      Event_SetMessageId(MsgTolbiShipShipsCrewReadyForAnything);
    } else {
      Event_SetMessageId(MsgTolbiShipImSpreadingGoodwillWhereverTravel);
    }
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Event_ShowMessage(ACTOR_SEA_HOLD_HEALER, 0);
    } else {
      Event_ShowMessage(ACTOR_HOLD_HEALER, 0);
    }
    Event_End();
  }
}

/// Isaac walks up to one of the monsters floating over the rowing deck after
/// an attack and talks to it, and the battle starts: three Man o' Wars the
/// first time, a Lizard Fighter and two Rabid Bats the second, a Virago and
/// two Man o' Wars the third.
///
/// The battle is the Tolbi ship's encounter table's first, second or third
/// scripted battle, and afterwards the party comes back to the rowing deck for
/// ShipRooms_EndFirstAttack or the one after it.
static void ShipRooms_FightRowingDeckMonsters(void) {
  Event_Start();
  Field_EndBusyAndReveal();
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) == 0) {
    Event_SetAfterBattleDestination(
        SCENE_SHIP_ROOMS, SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 0);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) == 0) {
      Event_SetAfterBattleDestination(
          SCENE_SHIP_ROOMS, SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE);
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 1);
    } else {
      if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) == 0) {
        Event_SetAfterBattleDestination(
            SCENE_SHIP_ROOMS, SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE);
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 2);
      }
    }
  }
  Event_End();
}

/// Walking onto the stairs up from the rowing deck (cell (27, 8)) while
/// monsters are aboard has the crewman call "Where are you going, Isaac? Slay
/// the beasts and save my mates!", and Isaac walks back off the stairs.
static void ShipRooms_CallLeaderBackToBeasts(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS) != 0) {
    Event_Start();
    Message_SetVoiceFromActor(ACTOR_ROWING_CREWMAN);
    FieldMessage_ShowCentered(MsgTolbiShipWhereGoingSlay,
                              CENTERED_MESSAGE_WINDOW, 8);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
    ShipRooms_Turn(ACTOR_PARTY_LEADER, FACING_SOUTH);
    Event_End();
  }
}

/// Stepping inside the door of the captain's cabin at Kalay (cells (28, 42),
/// (29, 42), (27, 43) and (29, 43)) after Ouranos and Sean have gone there,
/// Isaac hears the captain, the crewman and Kaja talk of the delay, if he has
/// not heard it on deck already.
///
/// "I wonder what could have happened," the captain says of the ship from
/// Tolbi. Kaja says the oarsmen are fearless, but if monsters appear at sea he
/// can't handle them alone; the captain can let no one but Kaja fight, since
/// the oarsmen will have their hands full: "Hmmm. What to do..." It plays once.
static void ShipRooms_WatchCrewWonderAtDelay(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0)
    return;

  Event_Start();
  Field_EndBusyAndReveal();
  FieldCamera_SetSpeed(1.6);
  ShipRooms_MoveCameraAndWait(448, CAMERA_KEEP, 638,
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 40);
  Event_SetMessageId(MsgTolbiShipWonderCouldHaveHappened);

  // "I wonder what could have happened."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  // "You mean you still intend to cross this dangerous sea?"
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST);
  // "It would be too big a loss to let so many passengers get away..."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHEAST);
  // "Say, Kaja, how are the oarsmen doing?"
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST);
  // "They're fearless. There's no problem there. It's just that..."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);

  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_EAST, 20);
  // ""It's just that"...what?"
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_WEST);
  // "If a whole bunch of...them appear while we're crossing the sea..."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  // "I'm sorry, but you'll have to..."
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  // "Remember, the oarsmen will have their hands full rowing..."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST);
  // "Hmmm. What to do..."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);

  GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
  Event_End();
}

/// Walking north into the passengers' room at Kalay (cells (9..11, 32)) once
/// the tour group is aboard, Isaac overhears Ouranos threaten "If we aren't
/// going to set sail right away, I'm going to go ashore!"
///
/// The tour's other guide begs him to be patient. Sean explains that his
/// friend Ouranos, huge as he is, can't stand water, and the two decide to go
/// to the captain's quarters and make him set sail, ignoring the guide's "I
/// don't want any violence!" They march out, the guide hurries after them, and
/// it plays once.
static void ShipRooms_WatchWarriorsStormOff(void) {
  s32 guideSpeaker;
  s32 seanSpeaker;
  const s32 *action;

  // Not before the party has heard of the delay.
  if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0)
    return;

  Event_Start();
  Field_EndBusyAndReveal();
  FieldCamera_SetSpeed(2.4);
  // The hold's camera box is one screen wide, so the camera stays at x 150
  // whatever x is asked for.
  ShipRooms_MoveCameraAndWait(1463, CAMERA_KEEP, 464,
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
  Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
  Event_SetMessageId(MsgTolbiShipIfWeArentGoingSet);
  // "If we aren't going to set sail right away, I'm going to go..."
  ShipRooms_Say(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
  ShipRooms_Turn(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH_NORTHEAST);
  guideSpeaker = SPEAKER(ACTOR_HOLD_SECOND_GUIDE, SPEAKER_WINDOW_BELOW);
  Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
  // "Yeah, but... we're going to set sail real soon, so please be..."
  ShipRooms_Say(guideSpeaker);
  Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
  // "When is "real soon"?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER(ACTOR_HOLD_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT),
      0, 20);
  ShipRooms_Turn(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST);
  seanSpeaker =
      SPEAKER(ACTOR_HOLD_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT);
  Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_ShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SEAN, 3);
  // "I'm asking how long real soon is going to be!"
  ShipRooms_Say(seanSpeaker);
  Actor_SetAttachedEffect(ACTOR_HOLD_SECOND_GUIDE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 3);
  // "You're wondering when. . . Well..."
  ShipRooms_Say(guideSpeaker);
  Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
  // "We're planning to go see Colosso."
  ShipRooms_Say(seanSpeaker);
  ShipRooms_Turn(ACTOR_HOLD_SEAN, FACING_NORTH_NORTHWEST);
  // "You know, my friend Ouranos here is scared of ships."
  ShipRooms_Say(seanSpeaker);
  ShipRooms_Turn(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH_NORTHEAST);
  Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 1);
  // "Ouranos? But he's huge!"
  ShipRooms_Say(guideSpeaker);
  Actor_SetAnimationAndWait(ACTOR_HOLD_OURANOS, ANIM_SHAKE_HEAD);
  // "It's not that I'm scared. I just can't stand water..."
  ShipRooms_Say(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
  Actor_StartRepeatedMotion(ACTOR_HOLD_OURANOS, 2);
  // "I'd rather be bitten by poisonous spiders than sail across the..."
  ShipRooms_Say(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
  Actor_SetAnimation(ACTOR_HOLD_SECOND_GUIDE, ANIM_SHAKE_HEAD);
  // "Hmmm. Spiders, huh?"
  ShipRooms_Say(guideSpeaker);
  Actor_SetAnimationAndWait(ACTOR_HOLD_SEAN, ANIM_SHAKE_HEAD);
  // "And even Ouranos is ready to cross the sea right now."
  ShipRooms_Say(seanSpeaker);
  ShipRooms_Turn(ACTOR_HOLD_SEAN, FACING_WEST);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SEAN, 2);
  // "Well, if we're not going now, then when are we going to set..."
  Event_ShowMessageThenWait(seanSpeaker, 0, 20);
  Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  // "That's... not an easy question."
  Event_ShowMessageThenWait(guideSpeaker, 0, 20);
  Actor_ShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ShowEmote(ACTOR_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SEAN, 2);
  // "You mean you wouldn't know even if I asked?"
  ShipRooms_Say(seanSpeaker);
  ShipRooms_Turn(ACTOR_HOLD_SEAN, FACING_NORTH_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
  // "We're going, Ouranos."
  ShipRooms_Say(seanSpeaker);
  ShipRooms_Turn(ACTOR_HOLD_OURANOS, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
  // "Going? Where?"
  ShipRooms_Say(ACTOR_HOLD_OURANOS);
  Actor_ShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_RunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
  // "Where else? To the captain's quarters, of course."
  ShipRooms_Say(seanSpeaker);
  Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_HOLD_OURANOS, FACING_SOUTH_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST, 0);
  ShipRooms_Turn(ACTOR_HOLD_OURANOS, FACING_SOUTH_SOUTHEAST);
  Actor_RunRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
  // "What do you think you're going to do in the captain's quarters?"
  Event_ShowMessageThenWait(guideSpeaker, 0, 20);
  Actor_FaceDirection(ACTOR_HOLD_SEAN, FACING_SOUTH, 40);
  // "Make him set sail."
  ShipRooms_Say(seanSpeaker);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
  Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 2);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
  // "And what if he refuses?"
  ShipRooms_Say(ACTOR_HOLD_OURANOS);
  Actor_SetAnimationAndWait(ACTOR_HOLD_SEAN, ANIM_NOD);
  // "In that case, we'll just have to do our best to persuade him..."
  ShipRooms_Say(seanSpeaker);
  Actor_ShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_RunRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
  // "That sounds crazy!"
  ShipRooms_Say(guideSpeaker);
  Actor_SetAnimationAndWait(ACTOR_HOLD_OURANOS, ANIM_NOD);
  // "That sounds great! Count me in."
  ShipRooms_Say(ACTOR_HOLD_OURANOS);
  Actor_FaceDirection(ACTOR_HOLD_SEAN, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimation(ACTOR_HOLD_SEAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_HOLD_OURANOS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_HOLD_SEAN, 1.6);
  Actor_SetSpeed(ACTOR_HOLD_OURANOS, 1.6);
  action = gShipRooms_LeaveHold;
  Actor_SetScript(ACTOR_HOLD_SEAN, action);
  Actor_SetScript(ACTOR_HOLD_OURANOS, action);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_SOUTH, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.4);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 520);
  Event_Wait(1);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_Jump(ACTOR_HOLD_SECOND_GUIDE, 4, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
  // "I don't want any violence!"
  ShipRooms_Say(ACTOR_HOLD_SECOND_GUIDE);
  Actor_SetSpeed(ACTOR_HOLD_SECOND_GUIDE, 1.6);
  Actor_SetScript(ACTOR_HOLD_SECOND_GUIDE, action);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_WaitForScriptEndById(ACTOR_HOLD_SECOND_GUIDE);
  GameFlag_Set(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
  Event_End();
}

/// Ouranos, Sean and the tour's other guide press the captain in his cabin to
/// set sail, and he finds his lucky anchor charm gone; it plays when Isaac
/// tells the crewman he wants to see the captain too.
///
/// Kaja, angry, says they can't sail in this dangerous sea, but the captain has
/// just decided to sail. Kaja tells of monsters never before seen in the
/// Karagol, Ouranos and Sean offer to guard the deck, and Sean remembers some
/// young warriors among the passengers. Then the captain finds the anchor gone
/// and refuses to leave without it, the crewman mutters "(Maybe we shouldn't
/// have taken it.)", and Kaja sends the party out: "This ship goes nowhere!!"
static void ShipRooms_DiscussLaunchingWithoutAnchor(void) {
  s32 seanSpeech;
  s32 ouranosSpeech;
  s32 captainSpeech;

  Sound_Play(MUS_DUNGEON);
  FieldCamera_SetSpeed(2.4);
  ShipRooms_MoveCameraAndWait(456, CAMERA_KEEP, 648,
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipYoureTryingLaunchShip);
  // "You're trying to launch the ship?"
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  // "We can't set sail in this dangerous sea!"
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  // "What was that?"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                  20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  // "We don't care about that!.."
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "But the captain decided to set sail just a second ago."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
  // "So...we're taking off?"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Actor_SetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  // "Yes. Preparations are being made right now."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST);
  // "Well...maybe we were a bit rash, running up here like that."
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SECOND_GUIDE, ANIM_NOD);
  // "Please, launch the ship when you're ready."
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHWEST, 0);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_SOUTH_SOUTHWEST);
  // Two passengers step aside as the conversation continues.
  Actor_SetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, 0.4);
  Actor_SetSpeed(ACTOR_QUARTERS_SEAN, 0.8);
  Actor_WalkTo(ACTOR_QUARTERS_SEAN, 444, 668);
  Actor_WalkToAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 472, 668);
  Actor_WaitForMoveById(ACTOR_QUARTERS_SEAN);
  Actor_SetAnimation(ACTOR_QUARTERS_SEAN, ANIM_STAND);
  Event_Wait(80);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  Event_Wait(20);
  // "..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_QUARTERS_OURANOS,
                                 0, 40);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
  // "Dangerous sea... That's what you said, isn't it?"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "It's dangerous out there..."
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_SetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
  // "Yeah, I did... The sea is dangerous now. There are monsters..."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
  Actor_SetAttachedEffect(ACTOR_QUARTERS_SECOND_GUIDE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_QUARTERS_SECOND_GUIDE, 2, 20);
  // "Monsters, you say?"
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_NOD);
  seanSpeech = SPEAKER(ACTOR_QUARTERS_SEAN, SPEAKER_PORTRAIT_LEFT);
  // "Lately, we've been sighting creatures never before seen in the..."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  // "Dangerous ones?"
  ShipRooms_Say(seanSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_SetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  // "Actually, some even came onboard the ship and attacked the crew."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                  40);
  Actor_Jump(ACTOR_QUARTERS_SECOND_GUIDE, 4, 0);
  // "That IS dangerous!"
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_SetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
  // "We suspect that our replacement ship might have been attacked."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  // "With such dangerous creatures around... You might need warriors!"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "It is certainly dangerous, but the ship is okay. After all, we-"
  Event_ShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  // "If there are monsters, I'll be glad to help out."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_OURANOS, 0,
                            40);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  // "Oh, really?"
  Event_ShowMessageThenWait(ACTOR_QUARTERS_KAJA, 0, 20);
  Actor_SetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SEAN, 2);
  ouranosSpeech = SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_WINDOW_ABOVE);
  // "Hey, Ouranos, I thought-"
  ShipRooms_Say(seanSpeech);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_SOUTH_SOUTHWEST, 20);
  // "We have to make it to Colosso... If we're late, we can't enter."
  ShipRooms_Say(ouranosSpeech);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SEAN, 2);
  // "That's true, but..."
  ShipRooms_Say(seanSpeech);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  // "Hey, maybe you can give us a hand, too, Sean!"
  ShipRooms_Say(ouranosSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  // "Really?"
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
  // "Are you really going to help out?"
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  // "I'm a man of my word. If I said I'll help, I'll help."
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "With you guys helping me to keep the deck safe, the ship should..."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Actor_SetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  // "So, can we set off now?"
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHEAST, 40);
  // "No... Now, we need someone to protect the oarsmen..."
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  Event_Wait(20);
  // "Protect the oarsmen, huh? You guys need some warriors..."
  ShipRooms_Say(seanSpeech);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 40);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  // "You have an idea?"
  ShipRooms_Say(ouranosSpeech);
  Actor_SetAnimation(ACTOR_QUARTERS_SEAN, ANIM_NOD);
  // "I saw some warriors in among the passengers belowdecks."
  Event_ShowMessageThenWait(seanSpeech, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  // "Do you think they'd help us out?"
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_SetAnimation(ACTOR_QUARTERS_SEAN, ANIM_NOD);
  // "If they're from Angara, they will."
  ShipRooms_Say(seanSpeech);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  // "Hmmm... If they could help out, we'd be unbeatable..."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 20);
  Sound_Play(MUS_SILENCE);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  captainSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_BELOW);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  80);
  // "It's gone..."
  ShipRooms_Say(captainSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  0);
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_QUARTERS_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_QUARTERS_CAPTAIN, 4, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  // "It's vanished!"
  Event_ShowMessage(captainSpeech, 0);
  Actor_SetSpeed(ACTOR_QUARTERS_CAPTAIN, 1.6);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 475, 598);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
  Actor_SetSpeed(ACTOR_QUARTERS_KAJA, 1);
  Actor_WalkToAndWait(ACTOR_QUARTERS_KAJA, 462, 618);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHWEST);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  // "What!? The captain's beloved anchor!"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_WINDOW_BELOW));
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "The anchor... Our good luck charm?"
  ShipRooms_Say(ACTOR_QUARTERS_OURANOS);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  // "Oh, that? What's the big deal?.."
  ShipRooms_Say(seanSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  20);
  Actor_Jump(ACTOR_QUARTERS_CAPTAIN, 4, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST, 20);
  // "What do you mean, "Oh, that"!?"
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Sound_Play(MUS_DUNGEON);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
  // "That anchor's been keeping my ship safe for years!"
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  60);
  // "It's just decorative, right?"
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_SHAKE_HEAD);
  // "Even if we find some warriors, we won't make it without that..."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_WalkToAndWait(ACTOR_QUARTERS_SEAN, 444, 628);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
  // "But you just said we were leaving..."
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_SEAN,
                        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  // "That was when I thought we still had the anchor!"
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "Why all the fuss over a little trinket?"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_SCOWL,
                  40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  // ""Little trinket"!? Weren't you listening to what he said?"
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_SHAKE_HEAD);
  // "It's not just a decoration! It's a good luck charm!"
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_EAST);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  // "This ship doesn't leave until we find it."
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldCamera_SetSpeed(1.2);
  ShipRooms_MoveCameraAndWait(464, CAMERA_KEEP, 680,
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, FACING_EAST);
  // "(What's the problem? We've got some warriors to help us...)"
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "(Maybe we shouldn't have taken it.)"
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_Jump(ACTOR_QUARTERS_CREWMAN, 4, 40);
  // "Hey! Hey, you! What are you doing here?"
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
  // "You heard the captain- This ship goes nowhere!!"
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  Actor_SetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_WalkTo(ACTOR_QUARTERS_SECOND_GUIDE, 438, 659);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_QUARTERS_CREWMAN, 1);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CREWMAN, 488, 686);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_SetAnimation(ACTOR_QUARTERS_SECOND_GUIDE, ANIM_STAND);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
  Scene_PlayMusic();
  GameFlag_Set(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL);
}

/// Isaac gives the captain the Anchor Charm from the top of the mast: "Hey!
/// It's my lucky anchor!" Then the captain asks the party to guard the
/// oarsmen.
///
/// Sean names the party as the young warriors he had in mind, and Isaac agrees
/// to help (a no only gets him talked round). Then passengers must promise to
/// row if an oarsman falls, which the tour's guide calls crazy, and the captain
/// asks Isaac's name before sending everyone to their stations.
static void ShipRooms_ReturnLuckyAnchor(void) {
  struct Actor *actor;
  s32 captainSpeech;
  s32 seanSpeech;
  s32 captainAboveSpeech;
  const s32 *action;

  Event_Start();
  Field_EndBusyAndReveal();
  Actor_FaceActor(ACTOR_QUARTERS_CAPTAIN, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  captainSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
  Event_SetMessageId(MsgTolbiShipItsMyLuckyAnchor);
  // "Hey! It's my lucky anchor!"
  ShipRooms_Say(captainSpeech);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  // "Well, look at that! Did you find our charm for us?"
  Event_OpenMessage(captainSpeech, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
    Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
    // "It was atop the mast? Why there, of all places?"
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
    Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "I... I don't know what to say... Thank you..."
    ShipRooms_Say(captainSpeech);
    gFieldState->message += 2;
  } else {
    gFieldState->message += 2;
    Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
    // "You climbed up the mast to see the Crow's Nest, and there it..."
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
    // "Thank you so much! Now, I can finally launch this ship."
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN,
                          SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
  }
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                  40);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(472, CAMERA_KEEP, 636, 1);
  Actor_SetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  Actor_WalkToAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 472, 662);
  ShipRooms_Turn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SECOND_GUIDE, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_SEAN, 0,
                                 20);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ShipRooms_Say(ACTOR_QUARTERS_OURANOS);
  seanSpeech = SPEAKER(ACTOR_QUARTERS_SEAN,
                       SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(ACTOR_QUARTERS_SEAN, ANIM_SHAKE_HEAD);
  ShipRooms_Say(seanSpeech);
  ShipRooms_Turn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_ShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHEAST);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SEAN, ANIM_NOD);
  ShipRooms_Say(seanSpeech);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_SEAN, ANIM_NOD);
  ShipRooms_Say(seanSpeech);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST);
  ShipRooms_Say(ACTOR_QUARTERS_OURANOS);
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_EAST);
  ShipRooms_Say(seanSpeech);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 486, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkToAndWait(ACTOR_GARET, 486, 624);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  actor = Actor_Get(ACTOR_GARET);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_WalkToAndWait(ACTOR_IVAN, 486, 640);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  actor = Actor_Get(ACTOR_IVAN);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_WalkToAndWait(ACTOR_MIA, 486, 656);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  ShipRooms_Say(seanSpeech);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST);
  Event_OpenMessage(ACTOR_QUARTERS_CAPTAIN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == NO) {
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
    ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_SEAN, ANIM_NOD);
    ShipRooms_Say(seanSpeech);
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
    Event_ShowMessageThenWait(
        SPEAKER(ACTOR_QUARTERS_KAJA,
                SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT),
        0, 40);
    gFieldState->message += 1;
  } else {
    gFieldState->message += 3;
    Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
    Event_ShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 40);
  }
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST);
  Event_ShowMessageThenWait(ACTOR_QUARTERS_SECOND_GUIDE, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  Actor_SetSpeed(ACTOR_QUARTERS_CAPTAIN, 0.8);
  captainAboveSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 472, 632);
  ShipRooms_Say(captainAboveSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  ShipRooms_Say(ACTOR_QUARTERS_SECOND_GUIDE);
  Actor_SetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(captainAboveSpeech, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
  ShipRooms_Turn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
  Actor_ShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_SetSpeed(ACTOR_QUARTERS_CREWMAN, 2.4);
  Actor_Jump(ACTOR_QUARTERS_CREWMAN, 2, 0);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CREWMAN, 462, 674);
  ShipRooms_Turn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
  ShipRooms_Say(ACTOR_QUARTERS_CREWMAN);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_SHAKE_HEAD);
  ShipRooms_Say(ACTOR_QUARTERS_KAJA);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  ShipRooms_Say(captainAboveSpeech);
  Actor_ShowEmote(ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                  40);
  Event_ShowMessageThenWait(ACTOR_QUARTERS_SECOND_GUIDE, 0, 40);
  ShipRooms_Turn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHEAST);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_SEAN, FACING_EAST);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_SEAN, 0, 20);
  Actor_ShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_CAPTAIN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
    Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
    gFieldState->message += 1;
  } else {
    gFieldState->message += 1;
    ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
  }
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_WEST);
  ShipRooms_Say(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE));
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 0, 0);
  Actor_SetAnimation(ACTOR_QUARTERS_SEAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
  Event_Wait(20);
  action = gShipRooms_LeaveCaptainsQuarters;
  Actor_SetScript(ACTOR_QUARTERS_CREWMAN, action);
  Event_Wait(4);
  Actor_SetScript(ACTOR_QUARTERS_OURANOS, action);
  Event_Wait(4);
  Actor_SetScript(ACTOR_QUARTERS_SEAN, action);
  Event_Wait(4);
  Actor_SetScript(ACTOR_QUARTERS_KAJA, action);
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_IVAN);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_GARET);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Actor_SetScript(ACTOR_QUARTERS_SECOND_GUIDE, action);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 456, 648);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_EAST);
  PartyInventory_Discard(ITEM_ANCHOR_CHARM);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED);
  Event_End();
}

/// Restores the room or plays the event selected by this entrance.
s32 Scene_Initialize(void) {
  wait(1);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gMainRoutineState.entrance) {
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
  case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      Actor_SetSpriteColorById(ACTOR_SEA_HOLD_SECOND_GUIDE, SPRITE_COLOR_RBG);
    } else if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Actor_SetSpriteColorById(ACTOR_HOLD_SECOND_GUIDE, SPRITE_COLOR_RBG);
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
    Actor_SetSpriteColorById(ACTOR_ROWING_SECOND_GUIDE, SPRITE_COLOR_RBG);
    break;
  case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Actor_SetSpriteColorById(ACTOR_QUARTERS_SECOND_GUIDE, SPRITE_COLOR_RBG);
    }
    break;
  }
  switch (gMainRoutineState.entrance) {
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
      Event_Start();
      ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 1, 0);
      ShipRooms_RunEventCommand(SHIP_RESTORE_TREMBLING_OARSMAN, 0, 0);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, gShipRooms_MonsterFloat);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, gShipRooms_MonsterFloat);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 2, gShipRooms_MonsterFloat);
      Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
      Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER + 1,
                               SPRITE_COLOR_BGR);
      Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER + 2,
                               SPRITE_COLOR_BGR);
      Event_End();
    } else {
      ShipRooms_StartFirstAttack();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
    ShipRooms_EndFirstAttack();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      Event_Start();
      ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 2, 0);
      ShipRooms_RunEventCommand(SHIP_RESTORE_TREMBLING_OARSMAN, 0, 0);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, gShipRooms_MonsterFloat);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, gShipRooms_MonsterFloat);
      Event_End();
    } else {
      ShipRooms_StartSecondAttack();
    }
    break;
  case SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
    ShipRooms_EndSecondAttack();
    break;
  case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      Event_Start();
      ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 3, 0);
      ShipRooms_RunEventCommand(SHIP_RESTORE_TREMBLING_OARSMAN, 0, 0);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, gShipRooms_MonsterFloat);
      Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, gShipRooms_MonsterFloat);
      Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
      Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER + 1,
                               SPRITE_COLOR_BGR);
      Event_End();
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
        Task_AddFrame(ShipRooms_ShakeWhileKrakenOnDeck, TASK_PASS_FRAME);
        Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
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
    ShipRooms_SwitchLeftAndRightTeams();
    break;
  case SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT:
    ShipRooms_RunEventCommand(SHIP_CLEAR_FLAG_RANGE,
                              FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT,
                              FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
    ShipRooms_RunEventCommand(SHIP_CLEAR_ROWING_SELECTIONS, 0, 0);
    GameFlag_Set(FLAG_TOLBI_SHIP_DEFEATED_AT_SEA);
    Import_EventSetExitRequest(SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK);
    break;
  }
  return 0;
}

/// Restores the captain's quarters' actors after the anchor and
/// departure conversations.
static void ShipRooms_SetUpCaptainsQuarters(void) {
  struct Actor *actor;
  struct Actor *other;
  s32 done;

  if (!GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    ShipRooms_PlaceAnchorCharm();
    return;
  }
  if (Event_GetFlag(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    ShipRooms_PlaceAnchorCharm();
  }
  done = GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  if (done != 0) {
    return;
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND)) {
    actor = Actor_Get(ACTOR_QUARTERS_KAJA);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 456, 652, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_KAJA, 480, 600, FACING_NORTH_NORTHWEST);
    Actor_SetSpeed(ACTOR_QUARTERS_KAJA, 0.8);
    actor->userValue = KAJA_PACE_FACE_NORTH_NORTHWEST;
    actor->sceneByte = KAJA_CUE_NONE;
    actor->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
    actor->update = ShipRooms_PaceCaptainsQuarters;
    other = Actor_Get(ACTOR_QUARTERS_CAPTAIN);
    other->sceneCounter = 0;
    other->update = ShipRooms_WatchKaja;
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 480, 666, FACING_NORTHWEST);
    }
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 444, 614,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 456, 648, FACING_EAST);
    ShipRooms_RunEventCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL)) {
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CAPTAIN, 475, 598, FACING_WEST);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_KAJA, 462, 618, FACING_NORTH_NORTHWEST);
    Actor_Get(ACTOR_QUARTERS_SEAN)->facing = FACING_SOUTH_SOUTHEAST;
    Actor_Get(ACTOR_QUARTERS_OURANOS)->facing = FACING_NORTH_NORTHWEST;
    ShipRooms_PlaceActor(ACTOR_QUARTERS_SECOND_GUIDE, 438, 659,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_PlaceActor(ACTOR_QUARTERS_CREWMAN, 488, 688,
                         FACING_NORTH_NORTHWEST);
  }
}

/// Restores the passenger hold after recruitment and departure scripts.
static void ShipRooms_SetUpHold(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    Actor_SetPosition(ACTOR_HOLD_WARRIOR, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_TICKET_GRUMBLER, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_MUSCLE_ADMIRER, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_OLD_MAN, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_SECOND_GUIDE, 0, 0);
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    return;
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_HEAD_GUIDE, 152, 444,
                         FACING_SOUTH_SOUTHEAST);
    Actor_SetScript(ACTOR_SEA_HOLD_HEAD_GUIDE, gShipRooms_LookAroundScript);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_SWEATING_PASSENGER, 184, 480,
                         FACING_NORTH_NORTHWEST);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 170, 488,
                         FACING_NORTH_NORTHWEST);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_WARRIOR, 136, 488,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_MUSCLE_ADMIRER, 120, 480,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_TICKET_GRUMBLER, 184, 526,
                         FACING_NORTH_NORTHWEST);
    ShipRooms_PlaceActor(ACTOR_SEA_HOLD_LADY, 136, 584, FACING_WEST);
    Actor_SetScript(ACTOR_SEA_HOLD_LADY, gShipRooms_Tremble);
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
    Actor_SetPosition(ACTOR_HOLD_SECOND_GUIDE, 0, 0);
  }
}

/// Restores the rowing deck and introduces a newly recruited replacement.
static void ShipRooms_SetUpRowingDeck(void) {
  wait(1);
  ShipRooms_SetOarCollision();
  if (GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    ShipRooms_RunEventCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
    ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 412, 222,
                         FACING_SOUTH_SOUTHEAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_DRUMMER, 458, 161, FACING_WEST);
  } else {
    if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      Actor_SetPosition(ACTOR_ROWING_CREWMAN, 472, 152);
      Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
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
              Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
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

/// Makes the eight oars beside the seats never a talk or Psynergy target.
static void ShipRooms_SetOarCollision(void) {
  u32 actorId;

  for (actorId = ACTOR_ROWING_FIRST_OAR;
       actorId < ACTOR_ROWING_FIRST_OAR + ROWING_DECK_SEATS; actorId++) {
    struct Actor *actor = Actor_Get(actorId);
    u32 flags = actor->collisionFlags;
    actor->collisionFlags = flags | ACTOR_NOT_TARGETED;
  }
}

/// Puts each chosen passenger at the seat they took, in place of its oarsman;
/// if gated, only those whose seat's attack has since been fought off.
static void ShipRooms_SeatChosenRowers(s32 gated) {
  s32 actorId;

  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((actorId = ShipRooms_FindSeatRower(0, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 410, 172, FACING_NORTH_NORTHEAST);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
  }
  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF)) {
    return;
  }
  if ((actorId = ShipRooms_FindSeatRower(1, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 470, 172, FACING_NORTH_NORTHWEST);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  }
  if (gated && !GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED)) {
    return;
  }
  if ((actorId = ShipRooms_FindSeatRower(2, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 410, 204, FACING_NORTH_NORTHEAST);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
  }
  if (gated) {
    return;
  }
  if ((actorId = ShipRooms_FindSeatRower(3, 0)) != 0) {
    ShipRooms_PlaceActor(actorId, 470, 204, FACING_NORTH_NORTHWEST);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 3, 0, 0);
  }
}

/// On the rowing deck, once the charm is back, Kaja tells the oarsmen "Our
/// replacement never arrived, but we're shoving off just the same."
///
/// He, Ouranos and Sean will guard the deck and Isaac's party the oarsmen. If
/// an oarsman falls, Isaac is to choose a passenger and bring him down,
/// keeping the two teams of rowers balanced: "Right. You have your orders."
static void ShipRooms_AskForReplacementOarsmen(void) {
  struct Actor *actor;

  Event_Start();
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_MoveCameraAndWait(440, CAMERA_KEEP, 168,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_PlaceActor(ACTOR_ROWING_KAJA, 440, 164, FACING_SOUTH_SOUTHWEST);
  ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 428, 190, FACING_NORTH_NORTHEAST);
  ShipRooms_PlaceActor(ACTOR_ROWING_DRUMMER, 452, 190, FACING_NORTH_NORTHWEST);
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_STAND);
  ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 440, 134, FACING_WEST);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 148);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 424, 148);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipOurReplacementNeverArrivedBut);
  // "Our replacement never arrived, but we're shoving off just the..."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_RunRepeatedMotion(ACTOR_ROWING_CREWMAN, 1);
  // "What about the sea monsters?.."
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  // "That'll be me, Ouranos, and Sean on deck, and..."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_NORTH_NORTHEAST);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 440, 148);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  actor = Actor_Get(ACTOR_GARET);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkToAndWait(ACTOR_IVAN, 456, 148);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  actor = Actor_Get(ACTOR_IVAN);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_WalkToAndWait(ACTOR_MIA, 472, 148);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 60);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST, 20);
  // "Isaac and his friends protecting you down here."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_StartRepeatedMotion(ACTOR_ROWING_DRUMMER, 1);
  Actor_ShowEmote(ACTOR_ROWING_DRUMMER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Can we really depend on them?"
  ShipRooms_Say(ACTOR_ROWING_DRUMMER);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  // "Isaac and his friends have seen more combat than even I have..."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_RunRepeatedMotion(ACTOR_ROWING_FIRST_OARSMAN, 1);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_NOD);
  // "If you say so, Kaja. We trust your judgment."
  ShipRooms_Say(ACTOR_ROWING_FIRST_OARSMAN);
  Actor_SetAnimation(ACTOR_ROWING_CREWMAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 1, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 2, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ROWING_FIRST_OARSMAN + 3, ANIM_NOD);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
  // "And what will you do if any of you becomes incapacitated?"
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_ShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_StartRepeatedMotion(ACTOR_ROWING_CREWMAN, 1);
  // "We're not sure... What happens then??"
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  // "Another passenger will help row the ship."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_FaceDirection(ACTOR_ROWING_CREWMAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ROWING_DRUMMER, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Actor_SetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
  // "Don't worry about it! I had a word with the passengers."
  Event_ShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 20);
  Actor_SetAnimation(ACTOR_ROWING_CREWMAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ROWING_DRUMMER, ANIM_NOD);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_ROWING_DRUMMER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ShipRooms_Turn(ACTOR_ROWING_DRUMMER, FACING_NORTH_NORTHWEST);
  // "Who will go and give the order when the time comes?"
  ShipRooms_Say(ACTOR_ROWING_DRUMMER);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Hmmm. Good point..."
  Event_ShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 60);
  Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  // "You should select a replacement and bring him here."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 80);
  ShipRooms_Turn(ACTOR_ROWING_CREWMAN, FACING_NORTH_NORTHEAST);
  Actor_StartRepeatedMotion(ACTOR_ROWING_CREWMAN, 2);
  // "Rowing requires teamwork. Be sure to balance the two teams."
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_SetAnimationAndWait(ACTOR_ROWING_DRUMMER, ANIM_NOD);
  Actor_StartRepeatedMotion(ACTOR_ROWING_DRUMMER, 2);
  // "That's right. If we're not balanced, we won't make much..."
  ShipRooms_Say(ACTOR_ROWING_DRUMMER);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  // "Right. You have your orders."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, 0.8);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 158);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 148);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  // "I trust you'll all do your best."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_WEST, 20);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
  Actor_WalkTo(ACTOR_ROWING_KAJA, 440, 134);
  Event_Wait(40);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT);
}

/// In his cabin the captain walks up to the Anchor Charm, which swells and
/// shrinks as he raises it, and calls "All right. Cast off!"
static void ShipRooms_CastOff(void) {
  struct Actor *anchorCharm;

  Event_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 0, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_QUARTERS_CAPTAIN, 0.8);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 468, 614);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 472, 596);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 20);
  Actor_Jump(ACTOR_QUARTERS_CAPTAIN, 4, 20);
  anchorCharm = ShipRooms_PlaceAnchorCharm();
  Event_Wait(20);
  Sound_Play(SE_RISING_CHIMES);
  Actor_StartScript(anchorCharm, gShipRooms_AnchorCharmPulse);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 466, 624);
  ShipRooms_Turn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  Event_SetMessageId(MsgTolbiShipCastOff);
  // "All right. Cast off!"
  Event_ShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/// After a new rower has taken his seat, the captain calls from his cabin "All
/// right, we're off!"
static void ShipRooms_AnnounceDeparture(void) {
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST, 40);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  Event_SetMessageId(MsgTolbiShipWereOff);
  // "All right, we're off!"
  Event_ShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/// Kaja comes into the passengers' room as the ship casts off and calls "Row
/// those oars!"
static void ShipRooms_KajaCallsToOars(void) {
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 0, 0);
  ShipRooms_RunEventCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_KAJA, 150, 586);
  ShipRooms_MoveCameraAndWait(156, CAMERA_KEEP, 536,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 0, 0);
  Actor_SetSpeed(ACTOR_HOLD_KAJA, 0.8);
  Actor_WalkToAndWait(ACTOR_HOLD_KAJA, 168, 578);
  Actor_WalkToAndWait(ACTOR_HOLD_KAJA, 168, 554);
  Actor_FaceDirection(ACTOR_HOLD_KAJA, FACING_WEST, 20);
  Actor_StartRepeatedMotion(ACTOR_HOLD_KAJA, 2);
  Event_SetMessageId(MsgTolbiShipRowThoseOars);
  // "Row those oars!"
  Event_ShowMessageThenWait(ACTOR_HOLD_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/// Kaja comes into the passengers' room when the ship sets off again with its
/// new rower and calls "Row those oars!"
static void ShipRooms_KajaCallsReplacementToOars(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  ShipRooms_HideSelectedPassengers();
  Actor_SetPosition(ACTOR_SEA_HOLD_KAJA, 150, 586);
  ShipRooms_MoveCameraAndWait(156, CAMERA_KEEP, 536,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 0, 0);
  Actor_SetSpeed(ACTOR_SEA_HOLD_KAJA, 0.8);
  Actor_WalkToAndWait(ACTOR_SEA_HOLD_KAJA, 168, 578);
  Actor_WalkToAndWait(ACTOR_SEA_HOLD_KAJA, 168, 554);
  Actor_FaceDirection(ACTOR_SEA_HOLD_KAJA, FACING_WEST, 20);
  Actor_StartRepeatedMotion(ACTOR_SEA_HOLD_KAJA, 2);
  Event_SetMessageId(MsgTolbiShipRowThoseOars);
  // "Row those oars!"
  Event_ShowMessageThenWait(ACTOR_SEA_HOLD_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN,
                            SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/// The oarsmen and the passengers chosen row to the drum, and the ship sails
/// on across the World Map to its next attack, the Kraken or land.
static void ShipRooms_ShowRowing(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  ShipRooms_InstallRowingDeckActors(0);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 176, 1);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_BEAT);
  Event_Wait(30);
  Sound_Play(SE_THUMP);
  Event_Wait(30);
  ShipRooms_InstallRowingDeckActors(ANIM_STEP_NORMAL);
  Event_Wait(80);
  ShipRooms_InstallRowingDeckActors(0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_BEAT);
  Event_Wait(30);
  Sound_Play(SE_THUMP);
  Event_Wait(30);
  ShipRooms_InstallRowingDeckActors(ANIM_STEP_NORMAL);
  Event_Wait(80);
  ShipRooms_InstallRowingDeckActors(0);
  Event_Wait(90);
  Sound_Play(SE_THUMP);
  Event_Wait(30);

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);

  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, 0, 0);

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    Import_EventSetExitRequest(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    Import_EventSetExitRequest(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    Import_EventSetExitRequest(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK);
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    Import_EventSetExitRequest(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK);
  } else {
    Import_EventSetExitRequest(
        SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK);
  }
}

/// The lookout calls down into the captain's cabin "Avast! Monsters
/// sighted!" "Monsters?" "Aye, Captain! Sea monsters!", and the party goes up
/// on deck.
static void ShipRooms_AskAboutMonsters(void) {
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_StartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
  Event_SetMessageId(MsgTolbiShipMonsters);
  // "Monsters?"
  Event_ShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS, 0);
}

/// Told to turn the rudder hard to starboard, the captain answers from his
/// cabin "All right, I'm turning!", and the scene goes back up on deck as the
/// monsters board.
static void ShipRooms_AnnounceTurn(void) {
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
  Event_SetMessageId(MsgTolbiShipImTurning);
  // "All right, I'm turning!"
  ShipRooms_Say(ACTOR_QUARTERS_CAPTAIN);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST, 40);
  ShipRooms_RunEventCommand(
      SHIP_CLOSE_SCREEN,
      SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD, 0);
}

/// Monsters are on the rowing deck in the first attack: three floating
/// monsters beside the seats and an oarsman down, before ShipRooms_
/// ReportMonstersBelowdecks.
static void ShipRooms_StartFirstAttack(void) {
  const s32 *entryScript;

  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 1, 0);
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                            ACTOR_ROWING_FIRST_OARSMAN + 1,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
  Actor_SetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, gShipRooms_Tremble);
  entryScript = gShipRooms_MonsterFloat;
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 2, entryScript);
  Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER + 2, SPRITE_COLOR_BGR);
  ShipRooms_ReportMonstersBelowdecks();
  Event_End();
}

/// Back on the rowing deck after the first battle: the monsters are gone, and
/// Kaja finds an oarsman hurt (ShipRooms_FindOarsmanMissing).
static void ShipRooms_EndFirstAttack(void) {
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
                            ACTOR_ROWING_FIRST_OARSMAN + 1,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  ShipRooms_FindOarsmanMissing();
  GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
  Event_End();
}

/// Back on the rowing deck after choosing a rower, Kaja greets Isaac, "Isaac,
/// you're back! And you brought an oarsman with you!", and the passenger
/// takes the empty seat.
///
/// Coming down without one gets "Hey! Where's the replacement for our injured
/// oarsman?... I'm sorry, but you're going to have to choose someone!", and
/// Isaac is sent back up to the passengers' room.
static void ShipRooms_IntroduceReplacementOarsman(s32 seat) {
  s32 rowerId;
  struct Actor *actor;
  FIXED halfStep;
  s32 leftSeat;

  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 0, 0);
  halfStep = toFIXED(1) / 2;
  ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 432, 134, FACING_WEST);
  ShipRooms_SeatChosenRowers(1);
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 406, 134);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 406, 152);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 421, 152);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 10);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = ShipRooms_FindSeatRower(seat, 0);
    Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 10);
    Event_SetMessageId(MsgTolbiShipBackBroughtOarsman);
    // "Isaac, you're back! And you brought an oarsman with you!"
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    leftSeat = 0;
    Actor_SetSpeedFX(ACTOR_PARTY_LEADER, toFIXED(1), halfStep);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 432, 168);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPositionFX(rowerId, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeedFX(rowerId, toFIXED(1), halfStep);
    Actor_WalkToAndWait(rowerId, 448, 168);
    Actor_FaceDirection(rowerId, FACING_NORTH_NORTHWEST, 20);
    Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 20);
    Actor_SetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
    // "Well, you're going to have to row for us."
    ShipRooms_Say(ACTOR_ROWING_KAJA);
    Actor_ShowEmote(rowerId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
    Actor_SetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
    // "All right. Take your place."
    ShipRooms_Say(ACTOR_ROWING_KAJA);
    Actor_SetAnimationAndWait(rowerId, ANIM_NOD);
    if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 0);
      Actor_WalkToAndWait(rowerId, 470, 204);
      ShipRooms_Turn(rowerId, FACING_NORTH_NORTHWEST);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 422, 154);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST, 0);
      Actor_WalkToAndWait(rowerId, 410, 204);
      leftSeat = 1;
      ShipRooms_Turn(rowerId, FACING_NORTH_NORTHEAST);
    } else if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 0);
      Actor_WalkToAndWait(rowerId, 470, 172);
      ShipRooms_Turn(rowerId, FACING_NORTH_NORTHWEST);
    } else {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 422, 154);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST, 0);
      Actor_WalkToAndWait(rowerId, 410, 172);
      leftSeat = 1;
      ShipRooms_Turn(rowerId, FACING_NORTH_NORTHEAST);
    }
    Actor_FaceEachOther(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 20);
    // "Now, we'll need you to protect this area again, Isaac."
    ShipRooms_Say(SPEAKER(ACTOR_ROWING_KAJA, SPEAKER_PORTRAIT_RIGHT));
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
    Actor_SetSpeed(ACTOR_ROWING_KAJA, 1);
    if (leftSeat != 0) {
      Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 428, 164);
      Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 164);
    }
    Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
    Actor_WalkTo(ACTOR_ROWING_KAJA, 440, 134);
    Event_Wait(40);
    ShipRooms_RunEventCommand(
        SHIP_CLOSE_SCREEN, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
  } else {
    Event_SetMessageId(MsgTolbiShipBack);
    // "Isaac, you're back."
    FieldEvent_ShowMessageThenWait(KAJA_ROWING_SPEAKER, 0, 40);
    Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Hey! Where's the replacement for our injured oarsman?"
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
    // "What? You didn't choose an oarsman?"
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_ShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "This ship won't go anywhere without enough oarsmen!"
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
    // "I'm sorry, but you're going to have to choose someone!"
    ShipRooms_Say(KAJA_ROWING_SPEAKER);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Import_EventSetExitRequest(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
  }
}

/// Creates the visible anchor charm once a visit
/// (FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED records it) and retains it for
/// departure scripts.
static struct Actor *ShipRooms_PlaceAnchorCharm(void) {
  s32 set = Event_GetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED);
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *glyph;

  if (set != 0) {
    return gShipRooms_AnchorCharm;
  }
  actor =
      FieldActor_Create(SPRITE_BLANK, toFIXED(455), toFIXED(4), toFIXED(544));
  actor->motionFlags = set;
  actor->retainSpriteCache = 1;
  sprite = actor->sprite;
  sprite->count = set;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawItem(ITEM_ANCHOR_CHARM);
  VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED);
  gShipRooms_AnchorCharm = actor;
  return actor;
}

/// Seats the rowers: at each of the first four seats the passenger chosen for
/// it, the oarsman put away, or else the oarsman, and the oarsmen at the other
/// four, all rowing at animationStep (0 holds them still; a passenger rows at
/// their own pace, ShipRooms_PoseActor).
static void ShipRooms_InstallRowingDeckActors(u32 animationStep) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindSeatRower(0, 0);
    Actor_SetPosition(rowerId, 410, 172);
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                              animationStep);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN, animationStep);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindSeatRower(1, 0);
    Actor_SetPosition(rowerId, 470, 172);
    Actor_Get(rowerId)->scaleX = -FIXED_ONE;
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                              animationStep);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN + 1, animationStep);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
    s32 rowerId = ShipRooms_FindSeatRower(2, 0);
    Actor_SetPosition(rowerId, 410, 204);
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                              animationStep);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN + 2, animationStep);
  }

  if (GameFlag_Get(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    s32 rowerId = ShipRooms_FindSeatRower(3, 0);
    Actor_SetPosition(rowerId, 470, 204);
    Actor_Get(rowerId)->scaleX = -FIXED_ONE;
    ShipRooms_RunEventCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                              animationStep);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 3, 0, 0);
  } else {
    ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                              ACTOR_ROWING_FIRST_OARSMAN + 3, animationStep);
  }

  ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 4, animationStep);
  ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 5, animationStep);
  ShipRooms_RunEventCommand(SHIP_START_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 6, animationStep);
  ShipRooms_RunEventCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
                            ACTOR_ROWING_FIRST_OARSMAN + 7, animationStep);
}

/// Sets a passenger at the oars rowing at a pace of their own beside the
/// oarsmen's ANIM_STEP_NORMAL: the head guide and the sweating
/// passenger as fast, the stared-at passenger and the warrior a quarter faster,
/// the second guide and the ticket grumbler at half, and the lady, the muscle
/// admirer and the old man at a quarter.
static void ShipRooms_PoseActor(s32 id) {
  switch (id) {
  case ACTOR_ROWING_SECOND_GUIDE:
    Actor_SetAnimation(id, ANIM_SECOND_GUIDE_ROW);
    Actor_SetAnimationSpeedById(id, ANIM_STEP_NORMAL / 2);
    break;
  case ACTOR_ROWING_HEAD_GUIDE:
  case ACTOR_ROWING_SWEATING_PASSENGER:
    Actor_SetAnimation(id, ANIM_ROW);
    Actor_SetAnimationSpeedById(id, ANIM_STEP_NORMAL);
    break;
  case ACTOR_ROWING_STARED_AT_PASSENGER:
  case ACTOR_ROWING_WARRIOR:
    Actor_SetAnimation(id, ANIM_ROW);
    Actor_SetAnimationSpeedById(id, ANIM_STEP_NORMAL * 5 / 4);
    break;
  case ACTOR_ROWING_TICKET_GRUMBLER:
    Actor_SetAnimation(id, YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_ROW);
    Actor_SetAnimationSpeedById(id, ANIM_STEP_NORMAL / 2);
    break;
  case ACTOR_ROWING_LADY:
  case ACTOR_ROWING_MUSCLE_ADMIRER:
    Actor_SetAnimation(id, ANIM_ROW);
    Actor_SetAnimationSpeedById(id, ANIM_STEP_NORMAL / 4);
    break;
  case ACTOR_ROWING_OLD_MAN:
    Actor_SetAnimation(id, OLD_MAN_WITH_CANE_ANIMATION_ROW);
    Actor_SetAnimationSpeedById(id, ANIM_STEP_NORMAL / 4);
    break;
  }
}

/// Shows the speaker's next line, then waits 10 frames.
static void ShipRooms_Say(s32 speaker) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(10);
}

/// Turns an actor to a facing over 10 frames.
static void ShipRooms_Turn(s32 actorId, u16 facing) {
  Actor_FaceDirection(actorId, facing, 10);
}

/// Places an actor at pixel coordinates and sets its initial heading.
static void ShipRooms_PlaceActor(s32 actorId, s32 x, s32 z, s32 facing) {
  Import_ActorSetPosition(actorId, toFIXED(x), toFIXED(z), facing);
  Actor_Get(actorId)->facing = facing;
}

/// Moves the camera, gliding unless flags hold SHIP_CAMERA_JUMP, waits for it
/// to arrive and recenters the layers as they say, then waits the frames in
/// their low bits.
static void ShipRooms_MoveCameraAndWaitFX(FIXED x, FIXED y, FIXED z,
                                          u32 flags) {
  u32 frames;

  FieldCamera_MoveWithinMapBounds(x, y, z, ~flags & SHIP_CAMERA_JUMP);
  // BUG: the mask keeps only bits 0, 4, 8 and 12 of the wait, so the callers'
  // waits of 20 and 40 frames become 16 and none; SHIP_CAMERA_JUMP also adds
  // a frame.
#ifdef BUGFIX
  frames = flags & SHIP_CAMERA_WAIT_FRAMES;
#else
  frames = flags & 0x1111;
#endif
  if ((flags & SHIP_CAMERA_WAIT_FOR_ARRIVAL) != 0)
    FieldCamera_WaitForMove();
  if ((flags & SHIP_CAMERA_RECENTER_LAYERS) != 0)
    MapGrid_CenterOnFollowedPoint();
  Event_Wait(frames);
}

/// Reuses the voyage's actor, camera, transition and selection operations.
/// `opt` is most often a wait in frames (the party's last turn, a nod, an
/// emote); other steps take it as an animation speed, a second actor or the
/// last of a range of flags.
static void ShipRooms_RunEventCommand(s32 step, u32 arg, u32 opt) {
  u32 slot;

  switch (step) {
  case SHIP_FACE_PARTY_EAST_AND_WEST:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, opt);
    break;
  case SHIP_FACE_PARTY:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, arg, 0);
    Actor_FaceDirection(ACTOR_GARET, arg, 0);
    Actor_FaceDirection(ACTOR_IVAN, arg, 0);
    Actor_FaceDirection(ACTOR_MIA, arg, opt);
    break;
  case SHIP_PLAY_PARTY_ANIMATION:
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    if (arg != 0) {
      Actor_WaitForAnimationChange(ACTOR_MIA);
    }
    if (opt == 0) {
      break;
    }
    Event_Wait(opt);
    break;
  case SHIP_SHOW_PARTY_EMOTE:
    // arg is not read.
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_IVAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_MIA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(opt);
    break;
  case SHIP_HIDE_FIRST_OARSMEN:
    for (slot = 0; slot < arg; slot++) {
      Actor_SetPosition(slot + ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
    }
    break;
  case SHIP_START_ROWING_ANIMATION: {
    struct Actor *actor;

    actor = Actor_Get(arg);
    actor->facing = FACING_SOUTH_SOUTHWEST;
  }
    Actor_SetAnimation(arg, ANIM_ROW);
    Actor_SetAnimationSpeedById(arg, opt);
    break;
  case SHIP_START_MIRRORED_ROWING_ANIMATION: {
    struct Actor *actor;

    actor = Actor_Get(arg);
    actor->facing = FACING_SOUTH_SOUTHWEST;
    actor->scaleX = -FIXED_ONE;
  }
    Actor_SetAnimation(arg, ANIM_ROW);
    Actor_SetAnimationSpeedById(arg, opt);
    break;
  case SHIP_SET_SELECTED_ROWER_POSE: {
    struct Actor *actor;

    actor = Actor_Get(arg);
    actor->facing = FACING_SOUTH_SOUTHWEST;
  }
    ShipRooms_PoseActor(arg);
    if (opt == 0) {
      Actor_SetAnimationSpeedById(arg, 0);
    }
    break;
  case SHIP_OPEN_SCREEN:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    Event_StartScreenTransition();
    if (arg != 0) {
      Event_WaitTransitionFrames();
    }
    // opt is not read: the 20 frames some callers pass are not waited.
    Event_Wait(0);
    break;
  case SHIP_CLOSE_SCREEN:
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    if (arg == 0) {
      break;
    }
    Import_EventSetExitRequest(arg);
    break;
  case SHIP_SETUP_ROWING_PARTY:
    ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
    ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
    ShipRooms_SeatChosenRowers(0);
    ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 432, 168, FACING_SOUTH);
    ShipRooms_PlaceActor(ACTOR_GARET, 448, 168, FACING_SOUTH);
    ShipRooms_PlaceActor(ACTOR_IVAN, 424, 152, FACING_SOUTH);
    ShipRooms_PlaceActor(ACTOR_MIA, 458, 152, FACING_SOUTH);
    break;
  case SHIP_RESTORE_ROWING_DECK_POSES:
    if (arg != 0) {
      struct Actor *actor;

      Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 3, ANIM_STAND);
      Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 3)->facing =
          FACING_SOUTH_SOUTHEAST;
      actor = Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 3);
      actor->scaleX = FIXED_ONE;
    }
    {
      struct Actor *actor;

      Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 4, ANIM_STAND);
      Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 4)->facing =
          FACING_SOUTH_SOUTHWEST;
      Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 5, ANIM_STAND);
      Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 5)->facing =
          FACING_SOUTH_SOUTHEAST;
      Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 5)->scaleX = FIXED_ONE;
      Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 6, ANIM_STAND);
      Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 6)->facing =
          FACING_SOUTH_SOUTHWEST;
      Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 7, ANIM_STAND);
      Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 7)->facing =
          FACING_SOUTH_SOUTHEAST;
      actor = Actor_Get(ACTOR_ROWING_FIRST_OARSMAN + 7);
      actor->scaleX = FIXED_ONE;
    }
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR, 410, 174);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 1, 470, 174);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 2, 410, 206);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 3, 470, 206);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 4, 410, 286);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 5, 470, 286);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 6, 410, 316);
    Actor_SetPosition(ACTOR_ROWING_FIRST_OAR + 7, 470, 316);
    wait(1);
    if (arg != 0) {
      Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 3,
                          FACING_NORTH_NORTHWEST, 0);
    }
    Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 4, FACING_NORTH_NORTHEAST,
                        0);
    Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 5, FACING_NORTH_NORTHWEST,
                        0);
    Actor_FaceDirection(ACTOR_ROWING_FIRST_OARSMAN + 6, FACING_NORTH_NORTHEAST,
                        0);
    ShipRooms_Turn(ACTOR_ROWING_FIRST_OARSMAN + 7, FACING_NORTH_NORTHWEST);
    break;
  case SHIP_RESTORE_ROWER_POSE: {
    struct Actor *actor;

    actor = Actor_Get(arg);
    Actor_SetAnimation(arg, ANIM_STAND);
    if (opt != 0) {
      actor->facing = FACING_SOUTH_SOUTHEAST;
    } else {
      actor->facing = FACING_SOUTH_SOUTHWEST;
    }
    actor->scaleX = FIXED_ONE;
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
                         FACING_NORTH_NORTHEAST);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    if (arg != 0) {
      ShipRooms_PlaceAnchorCharm();
    }
    ShipRooms_MoveCameraAndWait(448, 32, 624,
                                SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
    if (opt == 0) {
      break;
    }
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    break;
  case SHIP_SETUP_RECRUITMENT_DECK:
    Actor_SetPosition(ACTOR_ROWING_CREWMAN, 0, 0);
    Actor_SetPosition(ACTOR_ROWING_DRUMMER, 0, 0);
    Actor_SetPosition(ACTOR_ROWING_KAJA, 438, 152);
    break;
  case SHIP_HIDE_OARS:
    for (slot = 0; slot < ROWING_DECK_SEATS; slot++) {
      Actor_SetPosition(slot + ACTOR_ROWING_FIRST_OAR, 0, 0);
    }
    break;
  case SHIP_SETUP_PASSENGER_HOLD:
    ShipRooms_PlaceActor(ACTOR_HOLD_SECOND_GUIDE, 152, 532,
                         FACING_NORTH_NORTHWEST);
    ShipRooms_PlaceActor(ACTOR_HOLD_WARRIOR, 134, 490, FACING_SOUTH_SOUTHEAST);
    ShipRooms_PlaceActor(ACTOR_HOLD_TICKET_GRUMBLER, 166, 490,
                         FACING_SOUTH_SOUTHWEST);
    ShipRooms_PlaceActor(ACTOR_HOLD_MUSCLE_ADMIRER, 182, 504,
                         FACING_SOUTH_SOUTHWEST);
    ShipRooms_PlaceActor(ACTOR_HOLD_OLD_MAN, 118, 504, FACING_SOUTH_SOUTHEAST);
    ShipRooms_RunEventCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
    break;
  case SHIP_SETUP_MONSTER_ENCOUNTER:
    ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 416, 328, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_DRUMMER, 448, 352,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN, 454, 248,
                         FACING_SOUTH_SOUTHEAST);
    ShipRooms_PlaceActor(arg, 408, 290, FACING_EAST);
    ShipRooms_PlaceActor(opt, 408, 342, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 3, 420, 356,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 4, 408, 304, FACING_EAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 5, 418, 378,
                         FACING_NORTH_NORTHEAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 6, 440, 262,
                         FACING_SOUTH_SOUTHEAST);
    ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN + 7, 448, 378,
                         FACING_NORTH_NORTHEAST);
    break;
  case SHIP_CLEAR_FLAG_RANGE:
    for (slot = arg; slot <= opt; slot++) {
      GameFlag_Clear(slot);
    }
    break;
  case SHIP_CLEAR_ROWING_SELECTIONS:
    ShipRooms_RunEventCommand(
        SHIP_CLEAR_FLAG_RANGE, FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
        FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + PASSENGER_COUNT - 1);
    ShipRooms_RunEventCommand(
        SHIP_CLEAR_FLAG_RANGE, FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
        FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + PASSENGER_COUNT - 1);
    ShipRooms_RunEventCommand(
        SHIP_CLEAR_FLAG_RANGE, FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS,
        FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + PASSENGER_COUNT - 1);
    GameFlag_Clear(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
    GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
    break;
  case SHIP_RESTORE_TREMBLING_OARSMAN:
    wait(1);
    ShipRooms_RunEventCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
    Actor_SetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, gShipRooms_Tremble);
    break;
  case SHIP_RELEASE_COMPANIONS:
    Actor_ReleaseById(ACTOR_GARET);
    Actor_ReleaseById(ACTOR_IVAN);
    Actor_ReleaseById(ACTOR_MIA);
    break;
  case SHIP_RESET_CAMERA:
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    wait(1);
    if (arg != 0) {
      FieldCamera_GetActor()->motionFlags = 0;
    }
    break;
  case SHIP_PLACE_COMPANIONS_AND_MONSTERS:
    Event_AppendActorPlacements(gShipRooms_CompanionActorPlacements);
    wait(1);
    if (arg == 1) {
      Event_AppendActorPlacements(gShipRooms_FirstAttackMonsters);
      wait(1);
    } else if (arg == 2) {
      Event_AppendActorPlacements(gShipRooms_SecondAttackMonsters);
      wait(1);
    } else if (arg == 3) {
      Event_AppendActorPlacements(gShipRooms_ThirdAttackMonsters);
      wait(1);
    }
    break;
  }
}

/// The passenger chosen for a seat (0 to 3): their actor in the hold at sea if
/// inHold, else on the rowing deck; 0 if no one has been chosen.
static s32 ShipRooms_FindSeatRower(u32 seat, s32 inHold) {
  s32 flag = 0;
  s32 id = ACTOR_SEA_HOLD_FIRST_PASSENGER;
  u32 i;

  if (inHold == 0) {
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
  for (i = 0; i < PASSENGER_COUNT; i++, flag++, id++) {
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
      Actor_SetPosition(actorId, 0, 0);
      break;
    }
    i++;
    actorId++;
    firstFlag++;
  } while (i < PASSENGER_COUNT);
}

/// Monsters are on the rowing deck in the second attack, the first passenger
/// chosen trembling at the oars.
static void ShipRooms_StartSecondAttack(void) {
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = ShipRooms_FindSeatRower(0, 0);
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 2, 0);
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
  rowerScript = gShipRooms_Tremble;
  Actor_SetScript(firstRower, rowerScript);
  Actor_ReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 1);
  Actor_SetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, rowerScript);
  entryScript = gShipRooms_MonsterFloat;
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
  ShipRooms_ReportMonstersBelowdecks();
  Event_End();
}

/// An oarsman cries "Monsters belowdecks!", Garet, Ivan and Mia run up beside
/// Isaac, and the crewman begs "Isaac! Take care of these beasts and save my
/// mates!" The three fall in behind Isaac, and the stairs up stay barred until
/// the monsters are beaten.
static void ShipRooms_ReportMonstersBelowdecks(void) {
  struct Actor *actor;
  s32 message;

  ShipRooms_MoveCameraAndWait(440, CAMERA_KEEP, 176,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 440, 134);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.6);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 408, 134);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 408, 152);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 432, 166);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_GARET);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  wait(1);
  Actor_SetSpeed(ACTOR_IVAN, 1.6);
  Actor_WalkTo(ACTOR_IVAN, 424, 152);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_WalkTo(ACTOR_GARET, 448, 168);
  Actor_SetSpeed(ACTOR_MIA, 2);
  Actor_WalkToAndWait(ACTOR_MIA, 458, 152);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 40);
  Message_SetVoiceFromActor(ACTOR_ROWING_FIRST_OARSMAN);
  message = MsgTolbiShipMonstersBelowdecks;
  FieldMessage_ShowCentered(message, CENTERED_MESSAGE_WINDOW, 10);
  Event_Wait(10);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
  FieldCamera_SetSpeed(3.6);
  ShipRooms_MoveCameraAndWait(440, CAMERA_KEEP, 320,
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_CREWMAN, 2);
  ShipRooms_Turn(ACTOR_ROWING_CREWMAN, FACING_NORTH_NORTHEAST);
  Event_SetMessageId(message + 1);
  // "Isaac! Take care of these beasts and save my mates!"
  ShipRooms_Say(ACTOR_ROWING_CREWMAN);
  Actor_FaceDirection(ACTOR_ROWING_CREWMAN, FACING_EAST, 20);
  ShipRooms_MoveCameraAndWait(440, CAMERA_KEEP, 134,
                              SHIP_CAMERA_WAIT_FOR_ARRIVAL);
  Actor_SetScript(ACTOR_GARET, gShipRooms_CompanionsJoinLeaderAtFront);
  Actor_SetScript(ACTOR_IVAN, gShipRooms_CompanionsJoinLeaderAtFront);
  Actor_RunScriptAndWait(ACTOR_MIA, gShipRooms_CompanionsJoinLeaderAtFront);
  Event_Wait(40);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  ShipRooms_RunEventCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}

/// Back on the rowing deck after the battle, Kaja finds an oarsman missing and
/// sends the party for a passenger to row in his place.
static void ShipRooms_FindOarsmanMissing(void) {
  const s32 *action;

  ShipRooms_PlaceActor(ACTOR_PARTY_LEADER, 444, 300, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_GARET, 458, 310, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_IVAN, 444, 330, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_MIA, 432, 310, FACING_EAST);
  ShipRooms_PlaceActor(ACTOR_ROWING_KAJA, 440, 134, FACING_WEST);
  ShipRooms_PlaceActor(ACTOR_ROWING_FIRST_OARSMAN, 454, 248,
                       FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
  ShipRooms_MoveCameraAndWait(440, CAMERA_KEEP, 308,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Event_SetMessageId(MsgTolbiShipHeyAreYouOk);
  // "Hey, are you OK?"
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTH, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 176, 1);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, 1.6);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 152);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 424, 164);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 300, 1);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 424, 222);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 424, 262);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  // "Looks like you took care of those sea monsters."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  // "We're missing an oarsman!"
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 60);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTHEAST, 60);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 40);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Actor_SetAnimation(ACTOR_ROWING_KAJA, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ROWING_KAJA, 432, 268);
  Actor_MoveToAndWait(ACTOR_ROWING_KAJA, 452, 268);
  Actor_SetAnimation(ACTOR_ROWING_KAJA, ANIM_STAND);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_NORTH_NORTHEAST);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  // "Oh no! He's unconscious!"
  Event_ShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 20);
  ShipRooms_RunEventCommand(SHIP_FACE_PARTY, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
  Event_Wait(40);
  // "It looks like the sea monsters got one of our rowers..."
  Event_ShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 80);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  Event_Wait(10);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST);
  // "There's no way around it..."
  Event_OpenMessage(ACTOR_ROWING_KAJA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
    // "Hop to it!"
    ShipRooms_Say(ACTOR_ROWING_KAJA);
  } else {
    Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
    gFieldState->message += 1;
    // "Look, I know you don't want to have to do this, but you..."
    ShipRooms_Say(ACTOR_ROWING_KAJA);
    ShipRooms_RunEventCommand(SHIP_SHOW_PARTY_EMOTE, 2, 40);
    Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
    Actor_SetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
    // "Now, bring a passenger who can help row."
    ShipRooms_Say(ACTOR_ROWING_KAJA);
  }
  ShipRooms_RunEventCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  action = gShipRooms_CompanionsJoinLeaderByBackSeats;
  Actor_SetScript(ACTOR_GARET, action);
  Actor_SetScript(ACTOR_IVAN, action);
  Actor_RunScriptAndWait(ACTOR_MIA, action);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 176, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 424, 272);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 424, 164);
  Event_Wait(60);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Import_EventSetExitRequest(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
}

/// Back on the rowing deck after the second battle, with the first rower
/// chosen in his seat, Kaja finds another oarsman hurt.
static void ShipRooms_EndSecondAttack(void) {
  s32 firstRower = ShipRooms_FindSeatRower(0, 0);

  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
  ShipRooms_SeatChosenRowers(0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                            ACTOR_ROWING_FIRST_OARSMAN + 2);
  Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  ShipRooms_FindOarsmanMissing();
  GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF);
  Event_End();
}

/// Monsters are on the rowing deck in the third attack, with two of the
/// passengers chosen at the oars.
static void ShipRooms_StartThirdAttack(void) {
  s32 secondRower;
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = ShipRooms_FindSeatRower(0, 0);
  secondRower = ShipRooms_FindSeatRower(1, 0);
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 3, 0);
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                            secondRower);
  Actor_SetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
  rowerScript = gShipRooms_Tremble;
  Actor_SetScript(firstRower, rowerScript);
  Actor_ReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 1);
  Actor_SetScript(secondRower, rowerScript);
  Actor_ReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 2);
  entryScript = gShipRooms_MonsterFloat;
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
  Actor_SetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
  Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
  Actor_SetSpriteColorById(ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
  ShipRooms_ReportMonstersBelowdecks();
  Event_End();
}

/// Back on the rowing deck after the third battle, Kaja finds another oarsman
/// hurt.
static void ShipRooms_EndThirdAttack(void) {
  s32 firstRower = ShipRooms_FindSeatRower(0, 0);
  s32 secondRower = ShipRooms_FindSeatRower(1, 0);

  Event_Start();
  ShipRooms_RunEventCommand(SHIP_RESET_CAMERA, 1, 0);
  ShipRooms_RunEventCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
  ShipRooms_SeatChosenRowers(0);
  ShipRooms_RunEventCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                            secondRower);
  Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
  Actor_SetPosition(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
  ShipRooms_FindOarsmanMissing();
  GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF);
  Event_End();
}

/// The ship rocks badly as it sails on, and Mia wonders "I wonder what's
/// wrong... The ship shouldn't be rocking this badly."
///
/// Kaja runs down calling Isaac up on deck, "there's a huge sea monster on the
/// ship!", Garet says "Let's go bag us some sea monsters," and the party falls
/// in behind Isaac. The ship shakes now and then while the Kraken is on deck
/// (ShipRooms_ShakeWhileKrakenOnDeck).
static void ShipRooms_NoticeShipRocking(void) {
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  const s32 *action;

  firstRower = ShipRooms_FindSeatRower(0, 0);
  secondRower = ShipRooms_FindSeatRower(1, 0);
  thirdRower = ShipRooms_FindSeatRower(2, 0);
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  Actor_SetPosition(ACTOR_ROWING_CREWMAN, 472, 152);
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
  Actor_SetPosition(ACTOR_ROWING_KAJA, 440, 134);
  Actor_SetSpriteColorById(ACTOR_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ROWING_KAJA), 0);
  ShipRooms_InstallRowingDeckActors(ANIM_STEP_NORMAL);
  ShipRooms_MoveCameraAndWait(438, CAMERA_KEEP, 174,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_BOOM);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(80);
  Sound_Play(SE_BOOM);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Sound_Play(MUS_MOUNT_ALEPH_ERUPTS);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  ShipRooms_Turn(ACTOR_MIA, FACING_SOUTHWEST);
  // The party notices the rocking, then turns to the crew for an explanation.
  Event_SetMessageId(MsgTolbiShipWonderWhatsWrongShipShouldnt);
  // "I wonder what's wrong... The ship shouldn't be rocking this..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  // "Isaac!"
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
  // "Isaac, up on deck!"
  ShipRooms_Say(ACTOR_IVAN);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetSpriteColorById(ACTOR_ROWING_KAJA, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ROWING_KAJA), SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, 1);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 430, 134);
  ShipRooms_Turn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  // "Isaac, there's a huge sea monster on the ship!"
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
  Event_Wait(40);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWING_DECK_POSES, 1, 0);
  Actor_FaceDirection(firstRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(secondRower, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  // "If we don't do something... we'll sink! You have to help us!"
  Event_ShowMessage(ACTOR_ROWING_KAJA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 440, 134);
  Actor_SetPosition(ACTOR_ROWING_KAJA, 0, 0);
  ShipRooms_Turn(ACTOR_GARET, FACING_WEST);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "If this ship sinks, everything we've done will mean nothing..."
  ShipRooms_Say(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  ShipRooms_Turn(ACTOR_MIA, FACING_WEST);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  action = gShipRooms_CompanionsJoinLeaderByFrontSeats;
  Actor_SetScript(ACTOR_GARET, action);
  Actor_SetScript(ACTOR_IVAN, action);
  Actor_RunScriptAndWait(ACTOR_MIA, action);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
  gShipRooms_ShakeTimer = 0;
  Task_AddFrame(ShipRooms_ShakeWhileKrakenOnDeck, TASK_PASS_FRAME);
  ShipRooms_RunEventCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
  Actor_ReleaseById(ACTOR_ROWING_KAJA);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Event_End();
}

/// While the Kraken is on deck, now and then (one chance in 120 a frame) plays
/// a rumble with a shake, easing the shake after ten frames; no other rumble
/// starts for 80 frames.
static void ShipRooms_ShakeWhileKrakenOnDeck(void) {
  if (gShipRooms_ShakeTimer != 0) {
    if (--gShipRooms_ShakeTimer == 70)
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  } else if ((Random_Below(120)) == 0) {
    Sound_Play(SE_BOOM);
    MapGrid_SetShake(2, 2, 1);
    gShipRooms_ShakeTimer = 80;
  }
}

/// A glimpse of the captain's cabin, Kaja beside the captain, once the ship
/// has come off course, before it sails on across the World Map to the
/// island.
static void ShipRooms_ShowCaptainsQuartersBeforeIsland(void) {
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 0);
  ShipRooms_PlaceActor(ACTOR_QUARTERS_KAJA, 468, 616, FACING_WEST);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  Actor_RunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST, 80);
  Actor_FaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
  Event_Wait(20);
  ShipRooms_RunEventCommand(SHIP_CLOSE_SCREEN, SHIP_ROOMS_TO_WORLD_MAP, 0);
}

/// How the passenger at the first seat answers Kaja at the island
/// (ShipRooms_ReachIsland), each with lines of their own.
enum IslandReaction {
  ISLAND_REACTION_OTHERS,
  /// The lady or the muscle admirer.
  ISLAND_REACTION_LADY_OR_ADMIRER,
  ISLAND_REACTION_OLD_MAN,
  /// A guide, who says the same as the others at first.
  ISLAND_REACTION_GUIDE
};

/// Kaja comes down to the rowing deck: "We've reached the island! Rowers,
/// rest those weary arms!"
///
/// The passengers at the oars ask whether this is Tolbi, Kaja admits the ship
/// went off course and stopped to get its bearings, and the oarsmen go out to
/// take their break.
static void ShipRooms_ReachIsland(void) {
  s32 fourthRower;
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  s32 reaction = ISLAND_REACTION_OTHERS;
  const s32 *goUpstairs;
  const s32 *stepOut;
  const s32 *joinLeader;

  firstRower = ShipRooms_FindSeatRower(0, 0);
  secondRower = ShipRooms_FindSeatRower(1, 0);
  thirdRower = ShipRooms_FindSeatRower(2, 0);
  fourthRower = ShipRooms_FindSeatRower(3, 0);
  Event_Start();
  ShipRooms_SetOarCollision();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_RunEventCommand(SHIP_HIDE_OARS, 0, 0);
  Actor_SetPosition(ACTOR_ROWING_CREWMAN, 472, 152);
  Actor_SetPosition(ACTOR_ROWING_KAJA, 440, 134);
  Actor_SetSpriteColorById(ACTOR_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ROWING_KAJA), 0);
  ShipRooms_InstallRowingDeckActors(ANIM_STEP_NORMAL);
  Actor_SetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
  ShipRooms_MoveCameraAndWait(438, CAMERA_KEEP, 174,
                              SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
  ShipRooms_RunEventCommand(SHIP_OPEN_SCREEN, 1, 20);
  Actor_SetSpriteColorById(ACTOR_ROWING_KAJA, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ROWING_KAJA), SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, 1);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 142);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 20);
  Actor_StartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
  Event_SetMessageId(MsgTolbiShipReachedIslandRowers);
  // "We've reached the island! Rowers, rest those weary arms!"
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Event_Wait(120);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWER_POSE, fourthRower, 1);
  ShipRooms_RunEventCommand(SHIP_RESTORE_ROWING_DECK_POSES, 0, 0);
  Actor_FaceDirection(firstRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(secondRower, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(fourthRower, FACING_NORTH_NORTHWEST, 60);
  if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_OLD_MAN)) {
    reaction = ISLAND_REACTION_OLD_MAN;
  } else if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                          PASSENGER_MUSCLE_ADMIRER) ||
             GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_LADY)) {
    reaction = ISLAND_REACTION_LADY_OR_ADMIRER;
  }
  Actor_RunRepeatedMotion(firstRower, 1);
  if (reaction == ISLAND_REACTION_LADY_OR_ADMIRER) {
    gFieldState->message += 1;
  } else if (reaction == ISLAND_REACTION_OLD_MAN) {
    gFieldState->message += 2;
  }
  Actor_StartRepeatedMotion(firstRower, 2);
  // "Island? Isn't this Tolbi?", the lady's or the muscle admirer's "What do
  // you mean, island? Aren't we in Tolbi?", or the old man's "Uh... Tolbi's
  // not an island, is it?"
  ShipRooms_Say(firstRower);
  Event_SetMessageId(MsgTolbiShipShipWentOff);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
  // "The ship went off course, and we're..."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_ShowEmote(firstRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(secondRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(thirdRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(fourthRower, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "Raise your oars and rest for a little while."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 444, 132);
  Actor_ReleaseById(ACTOR_ROWING_KAJA);
  Event_Wait(40);
  if (reaction == ISLAND_REACTION_OTHERS &&
      (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_HEAD_GUIDE) ||
       GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
                    PASSENGER_SECOND_GUIDE))) {
    reaction = ISLAND_REACTION_GUIDE;
  }
  if (reaction == ISLAND_REACTION_OTHERS) {
    gFieldState->message += 1;
  } else if (reaction == ISLAND_REACTION_LADY_OR_ADMIRER) {
    gFieldState->message += 2;
  } else if (reaction == ISLAND_REACTION_OLD_MAN) {
    gFieldState->message += 3;
  }
  ShipRooms_Turn(firstRower, FACING_EAST);
  // A guide's "I'm going to check on the other passengers.", or the others'
  // "If the ship isn't going anywhere, I'm not waiting down here.", the lady's
  // or the muscle admirer's "If I'm not rowing, I ain't staying here." or the
  // old man's "As long as we're stopping, I don't have to stay in here."
  ShipRooms_Say(firstRower);
  Actor_SetSpeed(firstRower, 1);
  goUpstairs = gShipRooms_GoUpstairs;
  Actor_RunScriptAndWait(firstRower, goUpstairs);
  Actor_FaceDirection(secondRower, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(thirdRower, FACING_EAST, 0);
  Actor_FaceDirection(fourthRower, FACING_WEST, 40);
  Actor_FaceDirection(secondRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(fourthRower, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetSpeed(secondRower, 1);
  Actor_SetSpeed(thirdRower, 1);
  Actor_SetSpeed(fourthRower, 1);
  Actor_SetScript(thirdRower, goUpstairs);
  Event_Wait(40);
  stepOut = gShipRooms_StepOutFromSeat;
  Actor_RunScriptAndWait(secondRower, stepOut);
  Actor_SetScript(secondRower, goUpstairs);
  Actor_RunScriptAndWait(fourthRower, stepOut);
  Actor_RunScriptAndWait(fourthRower, goUpstairs);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  joinLeader = gShipRooms_CompanionsJoinLeaderAtFront;
  Actor_SetScript(ACTOR_GARET, joinLeader);
  Actor_SetScript(ACTOR_IVAN, joinLeader);
  Actor_RunScriptAndWait(ACTOR_MIA, joinLeader);
  ShipRooms_RunEventCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  GameFlag_Set(FLAG_TOLBI_SHIP_AT_ISLAND);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Back from the island, Kaja has the rowers change sides: "Sorry, everyone,
/// but we need to switch the left and right teams! We've strayed off course
/// because the left team's timing is off."
///
/// The passengers chosen swap seats, and the ship sets off one last time for
/// Tolbi.
static void ShipRooms_SwitchLeftAndRightTeams(void) {
  s32 firstRower;
  s32 fourthRower;
  s32 thirdRower;
  s32 secondRower;

  firstRower = ShipRooms_FindSeatRower(0, 0);
  secondRower = ShipRooms_FindSeatRower(1, 0);
  thirdRower = ShipRooms_FindSeatRower(2, 0);
  fourthRower = ShipRooms_FindSeatRower(3, 0);
  Event_Start();
  ShipRooms_RunEventCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  ShipRooms_PlaceActor(ACTOR_ROWING_CREWMAN, 472, 144, FACING_SOUTH_SOUTHWEST);
  ShipRooms_PlaceActor(ACTOR_ROWING_KAJA, 408, 142, FACING_SOUTH_SOUTHEAST);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  Event_SetMessageId(MsgTolbiShipSorryEveryoneButWeNeed);
  // "Sorry, everyone , but we need to switch the left and right..."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_StartRepeatedMotion(firstRower, 2);
  Actor_StartRepeatedMotion(secondRower, 2);
  Actor_StartRepeatedMotion(thirdRower, 2);
  Actor_RunRepeatedMotion(fourthRower, 2);
  Event_Wait(20);
  Actor_FaceDirection(firstRower, FACING_EAST, 0);
  Actor_FaceDirection(secondRower, FACING_WEST, 0);
  Actor_FaceDirection(thirdRower, FACING_EAST, 0);
  Actor_FaceDirection(fourthRower, FACING_WEST, 40);
  Actor_SetSpeed(firstRower, 1);
  Actor_SetSpeed(secondRower, 1);
  Actor_SetSpeed(thirdRower, 1);
  Actor_SetSpeed(fourthRower, 1);
  Actor_WalkTo(firstRower, 470, 172);
  Actor_WalkTo(secondRower, 410, 172);
  Actor_WalkTo(thirdRower, 470, 204);
  Actor_WalkToAndWait(fourthRower, 410, 204);
  Actor_SetAnimation(firstRower, ANIM_STAND);
  Actor_SetAnimation(secondRower, ANIM_STAND);
  Actor_SetAnimation(thirdRower, ANIM_STAND);
  Actor_FaceDirection(secondRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(firstRower, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(fourthRower, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(thirdRower, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
  // "One last push should get us there..."
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_SetAnimation(firstRower, ANIM_NOD);
  Actor_SetAnimation(secondRower, ANIM_NOD);
  Actor_SetAnimation(thirdRower, ANIM_NOD);
  Actor_SetAnimationAndWait(fourthRower, ANIM_NOD);
  // "All right! Let's aim for Tolbi and set off one last time!"
  ShipRooms_Say(ACTOR_ROWING_KAJA);
  Actor_SetAnimation(firstRower, ANIM_NOD);
  Actor_SetAnimation(secondRower, ANIM_NOD);
  Actor_SetAnimation(thirdRower, ANIM_NOD);
  Actor_SetAnimationAndWait(fourthRower, ANIM_NOD);
  Actor_FaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 0);
  ShipRooms_Turn(ACTOR_PARTY_LEADER, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ROWING_KAJA, 1);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
  Actor_WalkToAndWait(ACTOR_ROWING_KAJA, 444, 132);
  Actor_SetPosition(ACTOR_ROWING_KAJA, 0, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  ShipRooms_SwapRowerSelections(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  ShipRooms_SwapRowerSelections(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_AT_ISLAND);
  Import_EventSetExitRequest(SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY);
}

/// Exchanges the chosen passengers between two rowing seats.
static void ShipRooms_SwapRowerSelections(s32 firstSeatFlags,
                                          s32 secondSeatFlags) {
  s32 firstPassenger = 0;
  s32 secondPassenger = 0;
  u32 i;

  for (i = 0; i < PASSENGER_COUNT; i++) {
    s32 flag = firstSeatFlags + i;
    if (GameFlag_Get(flag) != 0) {
      GameFlag_Clear(flag);
      break;
    }
    firstPassenger++;
  }

  for (i = 0; i < PASSENGER_COUNT; i++) {
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

#include "shared/pushed_actor_direction_steps.c"

/// The warriors walk out of the hold.
s32 gShipRooms_LeaveHold[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 500),
    SC_SYNC,
    SC_MOVE(168, 0, 584),
    SC_SYNC,
    SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// Those gathered in the captain's quarters walk out.
s32 gShipRooms_LeaveCaptainsQuarters[] = {
    SC_SPEED(1),
    SC_ACCELERATION(0.5),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(456, 0, 674),
    SC_SYNC,
    SC_MOVE(456, 0, 712),
    SC_SYNC,
    SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// The Anchor Charm swells and shrinks twice as the captain raises it.
s32 gShipRooms_AnchorCharmPulse[] = {
    SC_SET(SW_SCALE_X, 11 * FIXED_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * FIXED_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * FIXED_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(2),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

/// Garet, Ivan or Mia walks into the leader, who stands at the front of the
/// rowing deck below the stairs, and is put away.
s32 gShipRooms_CompanionsJoinLeaderAtFront[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(434, 0, 166),
    SC_SYNC,
    SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// The same, the leader standing between the back seats.
s32 gShipRooms_CompanionsJoinLeaderByBackSeats[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(438, 0, 300),
    SC_SYNC,
    SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// The same, the leader standing between the front seats, two pixels from
/// where gShipRooms_CompanionsJoinLeaderAtFront ends.
s32 gShipRooms_CompanionsJoinLeaderByFrontSeats[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(432, 0, 168),
    SC_SYNC,
    SC_CALL(ShipRooms_ResetActor),
    SC_END,
};

/// Squashes the actor and lets it spring back, over and over: a rower
/// trembling at the monsters, or the lady seasick in the hold.
s32 gShipRooms_Tremble[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_SCALE_X, -5 * FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A monster on the rowing deck floats up and down in place
/// (ShipRooms_Float).
s32 gShipRooms_MonsterFloat[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_USER_VALUE, 0),
    SC_CALL(ShipRooms_Float),
};

/// A rower walks up the stairs and is put away.
s32 gShipRooms_GoUpstairs[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(408, 0, 132),
    SC_SYNC,
    SC_MOVE(444, 0, 132),
    SC_SYNC,
    SC_RELEASE,
};

/// A rower steps out from the front west seats into the walk between them.
s32 gShipRooms_StepOutFromSeat[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(416, 0, 186),
    SC_SYNC,
    SC_END,
};

/// A passenger glances about at random.
s32 gShipRooms_LookAroundScript[] = {
    SC_CALL(ShipRooms_LookAround),
};

/// Hides the actor at once (ShipRooms_Hide).
s32 gShipRooms_HideScript[] = {
    SC_CALL(ShipRooms_Hide),
};

/// The Anchor Charm's actor, once placed this visit.
struct Actor *gShipRooms_AnchorCharm = 0;

/// The other scenes have no entrances.
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

struct ActorPlacement gShipRooms_ActorPlacementsNone[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gShipRooms_CaptainsQuartersActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        446, 0, 620, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 626, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        454, 0, 640, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The captain's quarters once the warriors have come to see the captain, with
/// Garet, Ivan and Mia.
struct ActorPlacement gShipRooms_CaptainsQuartersActorPlacementsWarriors[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        482, 0, 604, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 624, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        464, 0, 690, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        456, 0, 648, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        444, 0, 628, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 660, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct ActorPlacement gShipRooms_CaptainsQuartersActorPlacementsArrived[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        462, 0, 612, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

/// The hold before the party hears of the delay at Kalay: no one but a blue
/// orb in the middle, hidden at once (gShipRooms_HideScript), which the hold's
/// other placements end with too.
struct ActorPlacement gShipRooms_HoldActorPlacementsEmpty[] = {
    // XXX (no evidence): what the hidden orb is for; it is never seen, solid
    // or a target, and no code of the scene reads it.
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, (s32)gShipRooms_HideScript,
        150, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct ActorPlacement gShipRooms_HoldActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 504, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 540, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 540, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 520, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        130, 0, 456, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        142, 0, 446, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        156, 0, 466, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 586, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, (s32)gShipRooms_HideScript,
        150, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The hold at sea, once the passengers row.
struct ActorPlacement gShipRooms_HoldActorPlacementsAtSea[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        138, 0, 446, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 532, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        182, 0, 492, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 492, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 480, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 526, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        172, 0, 448, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        180, 0, 464, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        122, 0, 516, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 586, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, (s32)gShipRooms_HideScript,
        150, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The rowing deck, in the order of enum TolbiShipRowingDeckActorId.
///
/// They are the crewman and the drummer, the oarsmen at the seats, the
/// passengers who may take a seat, Kaja and the eight oars beside the seats.
struct ActorPlacement gShipRooms_RowingDeckActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        438, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        438, 0, 374, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        410, 0, 172, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        470, 0, 172, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        410, 0, 204, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        470, 0, 204, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        410, 0, 284, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        470, 0, 284, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        410, 0, 314, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        470, 0, 314, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
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
    ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        410, 0, 174, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        470, 0, 174, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        410, 0, 206, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        470, 0, 206, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        410, 0, 286, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        470, 0, 286, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        410, 0, 316, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RESTING_OAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        470, 0, 316, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

/// Garet, Ivan and Mia, placed for an event.
struct ActorPlacement gShipRooms_CompanionActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The monsters of each attack, beside the seats.
struct ActorPlacement gShipRooms_FirstAttackMonsters[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 300, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        458, 0, 310, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 332, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gShipRooms_SecondAttackMonsters[] = {
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 300, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 332, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        458, 0, 310, FACING_WEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gShipRooms_ThirdAttackMonsters[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 300, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        474, 0, 332, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        458, 0, 310, FACING_WEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The cargo hold, and the events.
struct Trigger gShipRooms_TriggersOther[] = {
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_SHIP_ROOMS_CRATE_HOLD_BARREL, FLAG_TOLBI_SHIP_CRATE_HOLD_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct Trigger gShipRooms_HoldTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { TRIGGER_STEP, CELL_BEHAVIOR_SHIP_ROOMS_HOLD_WARRIORS, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, EVENT(ShipRooms_WatchWarriorsStormOff) },
    { TRIGGER_TALK, ACTOR_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToWaitingWarrior) },
    { TRIGGER_TALK, ACTOR_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
    { TRIGGER_TALK, ACTOR_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
    { TRIGGER_TALK, ACTOR_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToWaitingMuscleAdmirer) },
    { TRIGGER_TALK, ACTOR_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
    { TRIGGER_TALK, ACTOR_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
    { TRIGGER_TALK, ACTOR_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
    { TRIGGER_TALK, ACTOR_HOLD_HEALER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToHealer) },
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
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { TRIGGER_TABLE_END },
};

/// The hold at sea, where talking to a passenger asks them to row.
struct Trigger gShipRooms_HoldTriggersChoosingRowers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, EVENT(ShipRooms_AskHeadGuideToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT(ShipRooms_AskSweatingPassengerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT(ShipRooms_AskLadyToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT(ShipRooms_AskStaredAtPassengerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, EVENT(ShipRooms_AskSecondGuideToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, EVENT(ShipRooms_AskWarriorToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT(ShipRooms_AskTicketGrumblerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT(ShipRooms_AskMuscleAdmirerToRow) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, EVENT(ShipRooms_AskOldManToRow) },
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
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { TRIGGER_TABLE_END },
};

/// The hold while the ship waits off the island.
struct Trigger gShipRooms_HoldTriggersAtIsland[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
    { TRIGGER_TALK, ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, EVENT(ShipRooms_AskLadyToRow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
    { TRIGGER_TABLE_END },
};

/// The captain's quarters, from the wait at Kalay until the ship reaches Tolbi.
///
/// The Anchor Charm is given back to the captain here.
struct Trigger gShipRooms_CaptainsQuartersTriggers[] = {
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { TRIGGER_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToCaptain) },
    { TRIGGER_TALK, ACTOR_QUARTERS_CREWMAN, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToCaptainsQuartersCrewman) },
    { TRIGGER_TALK, ACTOR_QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
    { TRIGGER_TALK, ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
    { TRIGGER_TALK, ACTOR_QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
    { TRIGGER_TALK, ACTOR_QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
    { TRIGGER_TALK, ACTOR_QUARTERS_SEAN, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToSeanInCaptainsQuarters) },
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
    { TRIGGER_STEP, CELL_BEHAVIOR_SHIP_ROOMS_QUARTERS_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT(ShipRooms_WatchCrewWonderAtDelay) },
    { USE_ITEM_ON_ACTOR(ITEM_ANCHOR_CHARM), ACTOR_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, EVENT(ShipRooms_ReturnLuckyAnchor) },
    { EXAMINE_TRIGGER(EXAMINE_JAR) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGER_TABLE_END },
};

/// The captain's quarters while the ship waits off the island.
struct Trigger gShipRooms_CaptainsQuartersTriggersAtIsland[] = {
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { TRIGGER_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
    { TRIGGER_TALK, ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGER_TABLE_END },
};

/// The captain's quarters once the ship has reached Tolbi.
struct Trigger gShipRooms_CaptainsQuartersTriggersArrived[] = {
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
    { TRIGGER_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};

/// The rowing deck while monsters are belowdecks, where talking to one
/// (ACTOR_ROWING_FIRST_MONSTER and the two after it) fights it.
struct Trigger gShipRooms_RowingDeckTriggersMonstersBelowdecks[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_SHIP_ROOMS_ROWING_DECK_STAIRS, CONDITION_ALWAYS, EVENT(ShipRooms_CallLeaderBackToBeasts) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_MONSTER, CONDITION_ALWAYS, EVENT(ShipRooms_FightRowingDeckMonsters) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_MONSTER + 1, CONDITION_ALWAYS, EVENT(ShipRooms_FightRowingDeckMonsters) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_MONSTER + 2, CONDITION_ALWAYS, EVENT(ShipRooms_FightRowingDeckMonsters) },
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
    { TRIGGER_TABLE_END },
};

/// The rowing deck while the Kraken rocks the ship.
struct Trigger gShipRooms_RowingDeckTriggersKrakenOnDeck[] = {
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN },
    { TRIGGER_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
    { TRIGGER_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
    { TRIGGER_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
    { TRIGGER_TALK, ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToHeadGuideAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToSecondGuideAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToSweatingPassengerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_LADY, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToLadyAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToStaredAtPassengerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToWarriorAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToTicketGrumblerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT(ShipRooms_TalkToMuscleAdmirerAtOars) },
    { TRIGGER_TALK, ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, EVENT(ShipRooms_ReadHeadGuideMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, EVENT(ShipRooms_ReadSecondGuideMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, EVENT(ShipRooms_ReadSweatingPassengerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_LADY, CONDITION_ALWAYS, EVENT(ShipRooms_ReadLadyMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, EVENT(ShipRooms_ReadStaredAtPassengerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, EVENT(ShipRooms_ReadWarriorMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, EVENT(ShipRooms_ReadTicketGrumblerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, EVENT(ShipRooms_ReadMuscleAdmirerMindAtOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};
// clang-format on

/// Frames left of the Kraken's current jolt. At 0 another may start.
s32 gShipRooms_ShakeTimer;
