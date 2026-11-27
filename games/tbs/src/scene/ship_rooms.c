/* Ship rooms (SceneIdShipRooms): the rooms below the decks of the Tolbi-bound
   Ship, on its voyage across the Karagol Sea from Kalay to Tolbi. The deck and
   the mast are tolbi_ship_deck.c and the mast's own scene; the voyage moves
   between all three.

   The map is four rooms, one camera box each:

   - The passengers' room, the hold where the Colosso tour group waits, down
	 the stairs from the deck. A ladder behind a railing on its west side goes
	 down to the rowing deck, a travelling Healer stands in its south-east
	 corner, and a barrel beside the stairs holds 23 coins.
   - The cargo hold, down its own stairs in the middle of the ship, where a
	 barrel holds a Lucky Medal.
   - The rowing deck under the passengers' room: eight oarsmen in red
	 bandanas at their seats, two by two, a drummer keeping time and a
	 crewman.
   - The captain's cabin, where a jar holds a Nut.

   In story order:

   - At Kalay the ship waits for its relief from Tolbi. Walking into the
	 passengers' room once the tour group is aboard, Isaac overhears Ouranos,
	 a huge warrior bound for Colosso who "can't stand water", threaten to go
	 ashore; he and his friend Sean storm off to make the captain sail
	 (scShipRoomsWatchWarriorsStormOff). If Isaac has not yet overheard the
	 captain, Kaja and a crewman talk over the delay on deck, he hears it on
	 stepping into the cabin (scShipRoomsWatchCrewWonderAtDelay). With the
	 warriors there, the captain agrees to sail, then finds his lucky anchor
	 charm gone and will not leave without it
	 (scShipRoomsDiscussLaunchingWithoutAnchor). The crewman hid it on top of
	 the mast. Giving the Anchor Charm back (scShipRoomsReturnLuckyAnchor) gets
	 Isaac's party asked to guard the oarsmen, and on the rowing deck Kaja
	 tells them the ship sails without its relief.
   - The voyage: the captain raises the charm and casts off, Kaja calls the
	 oarsmen to row, and three times monsters come aboard and attack the
	 rowing deck. Isaac fights them there, and after each battle an oarsman
	 lies hurt, so Isaac must ask a passenger in the passengers' room to row
	 in his place. Each answers in his or her own way, and whoever agrees
	 follows Isaac down to the empty seat. Kaja warns that the two teams of
	 rowers must be balanced; the choices decide whether the ship keeps its
	 course.
   - The Kraken: the ship rocks badly, and the crew calls Isaac up on deck to
	 fight a huge sea monster.
   - If the rowers were badly chosen the ship strays off course to an island,
	 Crossbone Isle, and waits there while the tour group argues about going
	 ashore. Back on board, Kaja has the left and right teams change sides and
	 the ship sets off for Tolbi.
   - A defeat at sea brings the party back to the passengers' room with the
	 voyage's flags cleared, to start the crossing over.

   Which actors stand in each room, and which talk and search rows they have,
   depend on the room entered and the stage of the voyage. The cutscenes are
   played from scInit by the entrance the other ship scenes send the party in
   by, and share the operations of scShipRoomsRunScriptCommand. */

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "scene/tolbi_ship.h"
#include "scene/walk_into_leader.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

struct ACTOR;

extern Sint32		  ShipRoomsLeaveHold[];
extern Sint32		  ShipRoomsLeaveCaptainsQuarters[];
extern Sint32		  ShipRoomsAnchorCharmPulse[];
extern Sint32		  ShipRoomsCompanionsJoinLeaderAtFront[];
extern Sint32		  ShipRoomsCompanionsJoinLeaderByBackSeats[];
extern Sint32		  ShipRoomsCompanionsJoinLeaderByFrontSeats[];
extern Sint32		  ShipRoomsTremble[];
extern Sint32		  ShipRoomsMonsterFloat[];
extern Sint32		  ShipRoomsGoUpstairs[];
extern Sint32		  ShipRoomsStepOutFromSeat[];
extern Sint32		  ShipRoomsLookAroundScript[];
extern ACTOR		 *ShipRoomsAnchorCharm;
extern SCENE_ENTRANCE ShipRoomsEntrancesOther[];
extern SCENE_ENTRANCE ShipRoomsEntrances[];
extern Uint32		  ShipRoomsExits[];
extern ACTOR_PLACE	  ShipRoomsActorPlacementsNone[];
extern ACTOR_PLACE	  ShipRoomsCaptainsQuartersActorPlacements[];
extern ACTOR_PLACE	  ShipRoomsCaptainsQuartersActorPlacementsWarriors[];
extern ACTOR_PLACE	  ShipRoomsCaptainsQuartersActorPlacementsArrived[];
extern ACTOR_PLACE	  ShipRoomsHoldActorPlacementsEmpty[];
extern ACTOR_PLACE	  ShipRoomsHoldActorPlacements[];
extern ACTOR_PLACE	  ShipRoomsHoldActorPlacementsAtSea[];
extern ACTOR_PLACE	  ShipRoomsRowingDeckActorPlacements[];
extern ACTOR_PLACE	  ShipRoomsCompanionActorPlacements[];
extern ACTOR_PLACE	  ShipRoomsFirstAttackMonsters[];
extern ACTOR_PLACE	  ShipRoomsSecondAttackMonsters[];
extern ACTOR_PLACE	  ShipRoomsThirdAttackMonsters[];
extern CUE			  ShipRoomsCuesOther[];
extern CUE			  ShipRoomsHoldCues[];
extern CUE			  ShipRoomsHoldCuesChoosingRowers[];
extern CUE			  ShipRoomsHoldCuesAtIsland[];
extern CUE			  ShipRoomsCaptainsQuartersCues[];
extern CUE			  ShipRoomsCaptainsQuartersCuesAtIsland[];
extern CUE			  ShipRoomsCaptainsQuartersCuesArrived[];
extern CUE			  ShipRoomsRowingDeckCues[];
extern CUE			  ShipRoomsRowingDeckCuesMonstersBelowdecks[];
extern CUE			  ShipRoomsRowingDeckCuesKrakenOnDeck[];
extern CUE			  ShipRoomsRowingDeckCuesArrived[];

/* The visible anchor charm has been created this visit. */
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED FLAG_SCENE_TEMP_00

/* Operations shared by the captain's quarters, passenger hold and rowing-deck
   scripts (scShipRoomsRunScriptCommand), with what each takes as its arg and
   opt. */
typedef enum SHIP_SCRIPT_COMMAND {
	/* The leader and Ivan face east, Garet and Mia west; then wait opt
	   ticks. */
	SHIP_FACE_PARTY_EAST_AND_WEST,
	/* The party faces the heading arg; then wait opt ticks. */
	SHIP_FACE_PARTY,
	/* The party nods, waiting for Mia's nod to end if arg; then opt ticks. */
	SHIP_PLAY_PARTY_ANIMATION,
	/* Each of the party shows its attached effect once; then wait opt ticks
	   (arg is not read). */
	SHIP_SHOW_PARTY_EMOTE,
	/* The first arg oarsmen leave their seats. */
	SHIP_HIDE_FIRST_OARSMEN,
	/* Actor arg rows, at animation speed opt. */
	SHIP_START_ROWING_ANIMATION,
	/* Actor arg rows facing the other way, at animation speed opt. */
	SHIP_START_MIRRORED_ROWING_ANIMATION,
	/* Actor arg takes its rowing pose, held still unless opt. */
	SHIP_SET_SELECTED_ROWER_POSE,
	/* The screen opens, waiting for it if arg (opt is not read). */
	SHIP_OPEN_SCREEN,
	/* The screen closes, then the party takes exit arg if it is not 0. */
	SHIP_CLOSE_SCREEN,
	/* The companions are placed, the chosen passengers take their seats and the
	   party stands together on the rowing deck, facing south. */
	SHIP_SETUP_ROWING_PARTY,
	/* The oarsmen of the fourth seat on stand at their seats, the fourth only
	   if arg, and the oars are put back beside the seats. */
	SHIP_RESTORE_ROWING_DECK_POSES,
	/* Actor arg stands, facing south-southeast if opt, else south-southwest. */
	SHIP_RESTORE_ROWER_POSE,
	SHIP_HIDE_QUARTERS_CREW,
	SHIP_HIDE_OURANOS_AND_SEAN,
	/* The view goes to the captain in his cabin, with the anchor charm if arg,
	   opening the screen if opt. */
	SHIP_SETUP_CAPTAINS_QUARTERS,
	/* Only Kaja stands on the rowing deck, by the stairs. */
	SHIP_SETUP_RECRUITMENT_DECK,
	SHIP_HIDE_OARS,
	/* Five passengers take their places in the hold, and Ouranos and Sean
	   go. */
	SHIP_SETUP_PASSENGER_HOLD,
	/* The crew and oarsmen take their places as the monsters strike, actors arg
	   and opt among them at the west seats. */
	SHIP_SETUP_MONSTER_ENCOUNTER,
	/* Clears the flags arg to opt. */
	SHIP_CLEAR_FLAG_RANGE,
	/* Clears every seat's chosen rower and the attacks' flags. */
	SHIP_CLEAR_ROWING_SELECTIONS,
	/* The companions go and the third oarsman trembles. */
	SHIP_RESTORE_TREMBLING_OARSMAN,
	SHIP_RELEASE_COMPANIONS,
	/* The camera stays where it is, its actor held still if arg. */
	SHIP_RESET_CAMERA,
	/* The companions are placed, with attack arg's monsters (1 to 3). */
	SHIP_PLACE_COMPANIONS_AND_MONSTERS
} SHIP_SCRIPT_COMMAND;

/* The nine passengers who may be called to the oars.

   Their order is that of their rows in ShipRoomsHoldActorPlacementsAtSea and
   ShipRoomsRowingDeckActorPlacements and of each seat's group of flags
   (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + passenger).

   The game gives them no names, so each is named by what they say, or by what
   the guides call themselves. */
typedef enum TOLBI_SHIP_PASSENGER {
	/* A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), the tour's guide. */
	PASSENGER_HEAD_GUIDE,
	/* The tour's other guide, a man carrying a pack drawn as kalay.c's
	   ACTOR_SECOND_GUIDE (child value 2). */
	PASSENGER_SECOND_GUIDE,
	/* A chef (SPRITE_CHEF). */
	PASSENGER_SWEATING,
	/* A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN). */
	PASSENGER_LADY,
	/* A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN), who knows he will be
	   chosen. */
	PASSENGER_STARED_AT,
	/* A woman with a sword (SPRITE_PERSON_WITH_SWORD). */
	PASSENGER_WARRIOR,
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	PASSENGER_TICKET_GRUMBLER,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	PASSENGER_MUSCLE_ADMIRER,
	/* An old man with a cane (SPRITE_OLD_MAN_WITH_CANE). */
	PASSENGER_OLD_MAN,
	PASSENGER_COUNT
} TOLBI_SHIP_PASSENGER;

/* The captain's quarters, numbering the rows of
   ShipRoomsCaptainsQuartersActorPlacements and
   ShipRoomsCaptainsQuartersActorPlacementsWarriors from ACTOR_FIRST_PLACED.

   The party's rows take their own slots. The captain, Kaja and the crewman have
   the sprites they have on deck (tolbi_ship_deck.c). */
typedef enum TOLBI_SHIP_CAPTAINS_QUARTERS_ACTOR_ID {
	/* The captain, an old man with a white beard in green
	   (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN). */
	ACTOR_QUARTERS_CAPTAIN = ACTOR_FIRST_PLACED,
	/* A bald man with a beard (SPRITE_KAJA). */
	ACTOR_QUARTERS_KAJA,
	/* The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who
	   hid the captain's anchor charm. */
	ACTOR_QUARTERS_CREWMAN,
	ACTOR_QUARTERS_OURANOS,
	ACTOR_QUARTERS_SEAN,
	/* PASSENGER_SECOND_GUIDE, drawn the same way. */
	ACTOR_QUARTERS_SECOND_GUIDE
} TOLBI_SHIP_CAPTAINS_QUARTERS_ACTOR_ID;

/* The hold while the ship waits at Kalay and after it reaches Tolbi, numbering
   the rows of ShipRoomsHoldActorPlacements from ACTOR_FIRST_PLACED.

   They are four of the passengers and the second guide, then the warriors bound
   for Colosso, Ouranos and his friend Sean, the travelling healer and Kaja. */
typedef enum TOLBI_SHIP_HOLD_ACTOR_ID {
	/* PASSENGER_WARRIOR. */
	ACTOR_HOLD_WARRIOR = ACTOR_FIRST_PLACED,
	/* PASSENGER_TICKET_GRUMBLER. */
	ACTOR_HOLD_TICKET_GRUMBLER,
	/* PASSENGER_MUSCLE_ADMIRER. */
	ACTOR_HOLD_MUSCLE_ADMIRER,
	/* PASSENGER_OLD_MAN. */
	ACTOR_HOLD_OLD_MAN,
	/* PASSENGER_SECOND_GUIDE. */
	ACTOR_HOLD_SECOND_GUIDE,
	ACTOR_HOLD_OURANOS,
	ACTOR_HOLD_SEAN,
	/* A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE) who opens
	   his Sanctum (scShipRoomsTalkToHealer). */
	ACTOR_HOLD_HEALER,
	/* Calls the first passenger to the oars. */
	ACTOR_HOLD_KAJA
} TOLBI_SHIP_HOLD_ACTOR_ID;

/* The hold at sea, numbering the rows of ShipRoomsHoldActorPlacementsAtSea
   from ACTOR_FIRST_PLACED.

   They are the nine passengers in the order of TOLBI_SHIP_PASSENGER, the healer
   and Kaja. Each passenger's evidence is its TOLBI_SHIP_PASSENGER line. */
typedef enum TOLBI_SHIP_SEA_HOLD_ACTOR_ID {
	ACTOR_SEA_HOLD_FIRST_PASSENGER = ACTOR_FIRST_PLACED,
	ACTOR_SEA_HOLD_HEAD_GUIDE =
		ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_HEAD_GUIDE,
	ACTOR_SEA_HOLD_SECOND_GUIDE =
		ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_SECOND_GUIDE,
	ACTOR_SEA_HOLD_SWEATING_PASSENGER =
		ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_SWEATING,
	ACTOR_SEA_HOLD_LADY = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_LADY,
	ACTOR_SEA_HOLD_STARED_AT_PASSENGER =
		ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_STARED_AT,
	ACTOR_SEA_HOLD_WARRIOR = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_WARRIOR,
	ACTOR_SEA_HOLD_TICKET_GRUMBLER =
		ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_TICKET_GRUMBLER,
	ACTOR_SEA_HOLD_MUSCLE_ADMIRER =
		ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_MUSCLE_ADMIRER,
	ACTOR_SEA_HOLD_OLD_MAN = ACTOR_SEA_HOLD_FIRST_PASSENGER + PASSENGER_OLD_MAN,
	ACTOR_SEA_HOLD_HEALER,
	/* Calls a replacement rower to the oars. */
	ACTOR_SEA_HOLD_KAJA
} TOLBI_SHIP_SEA_HOLD_ACTOR_ID;

/* The seats on the rowing deck, an oarsman and an oar to each. */
#define ROWING_DECK_SEATS 8

/* The rowing deck, numbering the rows of ShipRoomsRowingDeckActorPlacements
   from ACTOR_FIRST_PLACED.

   The monsters of each attack are placed after them. */
typedef enum TOLBI_SHIP_ROWING_DECK_ACTOR_ID {
	/* A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA), one of the crew
	   by his bandana. */
	ACTOR_ROWING_CREWMAN = ACTOR_FIRST_PLACED,
	/* A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA) who keeps the
	   oarsmen's time. */
	ACTOR_ROWING_DRUMMER,
	/* The eight oarsmen, men in red bandanas (SPRITE_MAN_IN_RED_BANDANA), a
	   seat each.

	   The seats run two by two from the north, the west seat first. The
	   passengers chosen take the first four seats. */
	ACTOR_ROWING_FIRST_OARSMAN,
	/* The nine passengers in the order of TOLBI_SHIP_PASSENGER. */
	ACTOR_ROWING_FIRST_PASSENGER =
		ACTOR_ROWING_FIRST_OARSMAN + ROWING_DECK_SEATS,
	ACTOR_ROWING_HEAD_GUIDE =
		ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_HEAD_GUIDE,
	ACTOR_ROWING_SECOND_GUIDE =
		ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_SECOND_GUIDE,
	ACTOR_ROWING_SWEATING_PASSENGER =
		ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_SWEATING,
	ACTOR_ROWING_LADY = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_LADY,
	ACTOR_ROWING_STARED_AT_PASSENGER =
		ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_STARED_AT,
	ACTOR_ROWING_WARRIOR = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_WARRIOR,
	ACTOR_ROWING_TICKET_GRUMBLER =
		ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_TICKET_GRUMBLER,
	ACTOR_ROWING_MUSCLE_ADMIRER =
		ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_MUSCLE_ADMIRER,
	ACTOR_ROWING_OLD_MAN = ACTOR_ROWING_FIRST_PASSENGER + PASSENGER_OLD_MAN,
	/* Kaja: "Our replacement never arrived, but we're shoving off just the
	   same." */
	ACTOR_ROWING_KAJA,
	/* The eight oars, one beside each seat, hidden while the oarsmen row. */
	ACTOR_ROWING_FIRST_OAR,
	/* The monsters of each attack (ShipRoomsFirstAttackMonsters and the two
	   after it). */
	ACTOR_ROWING_FIRST_MONSTER = ACTOR_ROWING_FIRST_OAR + ROWING_DECK_SEATS
} TOLBI_SHIP_ROWING_DECK_ACTOR_ID;

/* Animations of the rowing deck's sprites beyond ACTOR_ANIMATION, as their
   animation tables and ticks under graphics/sprite draw them. */
typedef enum SHIP_ROOMS_ANIMATION {
	/* Pulling an oar from a seat, as the oarsmen's sprite sheet
	   (man_in_red_bandana.png) and most passengers' sprite sheets draw it. */
	ANIM_ROW = 5,
	/* The tour guide's sprite sheet rowing with a pink oar in place of
	   ANIM_ROW's brown one (man_carrying_a_pack.png). */
	ANIM_SECOND_GUIDE_ROW = 6,
	/* An oarsman lying on his back, arms flung out (man_in_red_bandana.png):
	   "An oarsman was injured by a sea monster?" */
	ANIM_OARSMAN_INJURED = 6,
	/* The drummer standing with his drum at his side
	   (man_in_green_bandana.png). */
	ANIM_DRUMMER_HOLD_DRUM = 5,
	/* The drummer, seen from behind, striking his drum once, then
	   ANIM_DRUMMER_HOLD_DRUM (man_in_green_bandana.png). */
	ANIM_DRUMMER_BEAT = 7
} SHIP_ROOMS_ANIMATION;

/* The step cues and searched containers below decks whose subjects are more
   than an exit. */
typedef enum SHIP_ROOMS_METATILE_BEHAVIOR {
	/* The rowing deck's stairs up to the passengers' room, metatile (27, 8),
	   barred while monsters are aboard. */
	METATILE_BEHAVIOR_SHIP_ROOMS_ROWING_DECK_STAIRS = 4,
	/* Just inside the captain's cabin door, metatiles (28, 42), (29, 42), (27,
	   43) and (29, 43), where the crew's talk of the delay plays. */
	METATILE_BEHAVIOR_SHIP_ROOMS_QUARTERS_CREW = 10,
	/* Across the passengers' room north of the stairs, metatiles (9..11, 32),
	   where Ouranos threatens to go ashore. */
	METATILE_BEHAVIOR_SHIP_ROOMS_HOLD_WARRIORS = 11,
	/* The cargo hold's barrel holding a Lucky Medal, metatile (10, 19). */
	METATILE_BEHAVIOR_SHIP_ROOMS_CRATE_HOLD_BARREL = 100,
	/* The barrel beside the passengers' room stairs holding 23 coins, metatile
	   (9, 35). */
	METATILE_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL = 101,
	/* The jar in the captain's cabin holding a Nut, metatile (26, 40). */
	METATILE_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR = 102
} SHIP_ROOMS_METATILE_BEHAVIOR;

/* Kaja's pacing in the captain's quarters off the island
   (scShipRoomsPaceCaptainsQuarters), one step to each value of his userValue.
   He looks about where he stands, then walks south-southwest and back. */
typedef enum KAJA_PACE_STEP {
	KAJA_PACE_FACE_NORTH_NORTHWEST,
	KAJA_PACE_FIRST_PAUSE,
	KAJA_PACE_FACE_EAST,
	KAJA_PACE_SECOND_PAUSE,
	KAJA_PACE_WALK_SOUTH,
	KAJA_PACE_ARRIVE_SOUTH, /* cues KAJA_BEAT_LOOK */
	KAJA_PACE_THIRD_PAUSE,
	KAJA_PACE_NOD, /* cues KAJA_BEAT_NOD */
	KAJA_PACE_FOURTH_PAUSE,
	KAJA_PACE_WALK_BACK, /* cues KAJA_BEAT_FACE_EAST */
	KAJA_PACE_ARRIVE_BACK,
	KAJA_PACE_LAST_PAUSE,
	KAJA_PACE_REPEAT
} KAJA_PACE_STEP;

/* The ticks each of Kaja's pauses counts past before his next step. */
enum { KAJA_PACE_PAUSE_TICKS = 80 };

/* What Kaja's pacing cues the captain to do (scShipRoomsWatchKaja), held in
   Kaja's sceneByte until the captain takes it; only while the captain is not
   paused. */
typedef enum KAJA_PACE_BEAT {
	KAJA_BEAT_NONE,
	KAJA_BEAT_LOOK,		/* the captain turns north-northeast, to Kaja */
	KAJA_BEAT_NOD,		/* the captain nods, if he turned to Kaja */
	KAJA_BEAT_FACE_EAST /* the captain turns back east */
} KAJA_PACE_BEAT;

/* scShipRoomsMoveCameraAndWait's flags, above the ticks it waits after in the
   low bits (SHIP_CAMERA_WAIT_TICKS). */
#define SHIP_CAMERA_WAIT_TICKS		 0xffff
#define SHIP_CAMERA_JUMP			 0x1 /* jump there at once rather than glide */
/* Then recenter the layers on the camera (fdMapGridCenterOnFollowedPoint). */
#define SHIP_CAMERA_RECENTER_LAYERS	 0x01000000
#define SHIP_CAMERA_WAIT_FOR_ARRIVAL 0x10000000

static void	  scShipRoomsStepBesideLeader(Sint32 passengerId);
static Bool	  scShipRoomsIsSpotFree(Sint32 direction);
static Sint32 scShipRoomsGetChosenOarsman(void);
static void	  scShipRoomsAskToRow(
	  Sint32 speaker, Sint32 question, Sint32 seatFlg);
static void	  scShipRoomsDiscussLaunchingWithoutAnchor(void);
static void	  scShipRoomsSetUpCaptainsQuarters(void);
static void	  scShipRoomsSetUpHold(void);
static void	  scShipRoomsSetUpRowingDeck(void);
static void	  scShipRoomsSetOarCollision(void);
static void	  scShipRoomsAskForReplacementOarsmen(void);
static void	  scShipRoomsCastOff(void);
static void	  scShipRoomsAnnounceDeparture(void);
static void	  scShipRoomsKajaCallsToOars(void);
static void	  scShipRoomsKajaCallsReplacementToOars(void);
static void	  scShipRoomsShowRowing(void);
static void	  scShipRoomsAskAboutMonsters(void);
static void	  scShipRoomsAnnounceTurn(void);
static void	  scShipRoomsStartFirstAttack(void);
static void	  scShipRoomsEndFirstAttack(void);
static void	  scShipRoomsIntroduceReplacementOarsman(Sint32 seat);
static ACTOR *scShipRoomsPlaceAnchorCharm(void);
static void	  scShipRoomsInstallRowingDeckActors(Uint32 anmStep);
static void	  scShipRoomsSay(Sint32 speaker);
static void	  scShipRoomsTurn(Sint32 actorId, Uint16 facing);
static void	  scShipRoomsPlaceActor(
	  Sint32 actorId, Sint32 x, Sint32 z, Sint32 facing);
static void scShipRoomsMoveCameraAndWaitFX(
	FIXED x, FIXED y, FIXED z, Uint32 flg);
/* Moves the camera to a point in whole pixels, CAMERA_KEEP keeping an axis,
   then waits as its flags say (scShipRoomsMoveCameraAndWaitFX). */
#define scShipRoomsMoveCameraAndWait(x, y, z, flags) \
	scShipRoomsMoveCameraAndWaitFX(SCRIPT_CAMERA_FIXED(x), \
		SCRIPT_CAMERA_FIXED(y), SCRIPT_CAMERA_FIXED(z), flags)
static void	  scShipRoomsRunScriptCommand(Sint32 step, Uint32 arg, Uint32 opt);
static Sint32 scShipRoomsFindSeatRower(Uint32 seat, Sint32 inHold);
static void	  scShipRoomsHideSelectedPassengers(void);
static void	  scShipRoomsHideSelectedPassenger(Sint32 actorId, Sint32 firstFlg);
static void	  scShipRoomsStartSecondAttack(void);
static void	  scShipRoomsReportMonstersBelowdecks(void);
static void	  scShipRoomsFindOarsmanMissing(void);
static void	  scShipRoomsEndSecondAttack(void);
static void	  scShipRoomsStartThirdAttack(void);
static void	  scShipRoomsEndThirdAttack(void);
static void	  scShipRoomsNoticeShipRocking(void);
static void	  scShipRoomsShakeWhileKrakenOnDeck(void);
static void	  scShipRoomsShowCaptainsQuartersBeforeIsland(void);
static void	  scShipRoomsReachIsland(void);
static void	  scShipRoomsSwitchLeftAndRightTeams(void);
static void	  scShipRoomsSwapRowerSelections(
	  Sint32 firstSeatFlg, Sint32 secondSeatFlg);

/* An actor script call that stands the actor still at the origin, with no
   velocity and no target, once, before its script goes on. */
static Sint32 scShipRoomsResetActor(ACTOR *actor)
{
	fdActorSelectAnimation(actor, ANIM_STAND);
	actor->x.fixed = 0;
	actor->y.fixed = 0;
	actor->z.fixed = 0;
	SET_VELOCITY(actor, 0, 0, 0);
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtX = ACTOR_NO_TARGET;
	return 0;
}

/* Floats the actor up and down between the floor and 8 pixels up, by a random
   half pixel to a pixel each tick. The actor's userValue says which way it is
   going, 1 down and 0 up. It runs every tick for as long as its script
   does. */
static Sint32 scShipRoomsFloat(ACTOR *actor)
{
	if(actor->userValue != 0) {
		actor->y.fixed -= syRandomBelow(toFIXED(1) / 2) + toFIXED(1) / 2;
		if(actor->y.fixed < 0) {
			actor->userValue = 0;
		}
	} else {
		actor->y.fixed += syRandomBelow(toFIXED(1) / 2) + toFIXED(1) / 2;
		if(actor->y.fixed > toFIXED(8)) {
			actor->userValue = 1;
		}
	}
	return 1;
}

/* Now and then (two chances in 64) turns the actor to face south-southeast or
   south-southwest, every tick for as long as its script runs. */
static Sint32 scShipRoomsLookAround(ACTOR *actor)
{
	Uint32 choice = syRandomBelow(64);

	if(choice == 6) {
		actor->facing = FACING_SOUTH_SOUTHEAST;
	} else if(choice == 9) {
		actor->facing = FACING_SOUTH_SOUTHWEST;
	}
	return 1;
}

/* Counts the ticks of one of Kaja's pauses in sceneCnt and moves him to his
   next step once it passes KAJA_PACE_PAUSE_TICKS. */
static void scShipRoomsWaitThenAdvance(ACTOR *actor)
{
	if(++actor->sceneCnt > KAJA_PACE_PAUSE_TICKS) {
		actor->userValue += 1;
	}
}

/* Counts down the actor's walking time in sceneWord.

   Returns 1 once the time is up or the actor has reached its target, else 0. */
static Bool scShipRoomsHasArrived(ACTOR *actor)
{
	if(actor->sceneWord != 0) {
		actor->sceneWord--;
	} else {
		return TRUE;
	}
	if(actor->tgtX == ACTOR_NO_TARGET && actor->tgtY == ACTOR_NO_TARGET &&
		actor->tgtZ == ACTOR_NO_TARGET) {
		return TRUE;
	}
	return FALSE;
}

/* The update of Kaja (ACTOR_QUARTERS_KAJA) while the ship waits off the island:
   he paces the captain's quarters (KAJA_PACE_STEP), giving each walk 60 ticks
   to arrive. */
static void scShipRoomsPaceCaptainsQuarters(ACTOR *actor)
{
	ACTOR *captain;

	captain = fdActorGet(ACTOR_QUARTERS_CAPTAIN);
	switch(actor->userValue) {
	case KAJA_PACE_FACE_NORTH_NORTHWEST:
		actor->facing = FACING_NORTH_NORTHWEST;
		actor->userValue++;
		actor->sceneCnt = 0;
		break;
	case KAJA_PACE_FACE_EAST:
		actor->facing = FACING_EAST;
		actor->userValue++;
		actor->sceneCnt = 0;
		break;
	case KAJA_PACE_WALK_SOUTH:
		fdActorSelectAnimation(actor, ANIM_WALK);
		syFieldActorSetMoveTarget(actor, 468, 32, 632);
		actor->sceneWord = 60;
		actor->userValue++;
		break;
	case KAJA_PACE_ARRIVE_SOUTH:
		if(scShipRoomsHasArrived(actor) != 0) {
			fdActorSelectAnimation(actor, ANIM_STAND);
			actor->sceneCnt = 0;
			if(captain->paused == 0) {
				actor->sceneByte = KAJA_BEAT_LOOK;
			}
			actor->userValue++;
		}
		break;
	case KAJA_PACE_NOD:
		if(captain->paused == 0) {
			fdActorSelectAnimation(actor, ANIM_NOD);
			actor->sceneByte = KAJA_BEAT_NOD;
		}
		actor->userValue++;
		actor->sceneCnt = 0;
		break;
	case KAJA_PACE_WALK_BACK:
		fdActorSelectAnimation(actor, ANIM_WALK);
		syFieldActorSetMoveTarget(actor, 480, 32, 600);
		actor->sceneWord = 60;
		actor->userValue++;
		if(captain->paused == 0) {
			actor->sceneByte = KAJA_BEAT_FACE_EAST;
		}
		break;
	case KAJA_PACE_ARRIVE_BACK:
		if(scShipRoomsHasArrived(actor) != 0) {
			fdActorSelectAnimation(actor, ANIM_STAND);
			actor->sceneCnt = 0;
			actor->userValue++;
		}
		break;
	case KAJA_PACE_FIRST_PAUSE:
	case KAJA_PACE_SECOND_PAUSE:
	case KAJA_PACE_THIRD_PAUSE:
	case KAJA_PACE_FOURTH_PAUSE:
	case KAJA_PACE_LAST_PAUSE:
		scShipRoomsWaitThenAdvance(actor);
		break;
	case KAJA_PACE_REPEAT:
		actor->userValue = KAJA_PACE_FACE_NORTH_NORTHWEST;
		break;
	}
}

/* The update of the captain while Kaja paces: he takes Kaja's cues (enum
   KAJA_PACE_BEAT), keeping in sceneCnt whether he has turned to Kaja. */
static void scShipRoomsWatchKaja(ACTOR *captain)
{
	ACTOR *kaja;

	kaja = fdActorGet(ACTOR_QUARTERS_KAJA);
	if(captain->paused != 0)
		return;
	if(kaja->sceneByte == KAJA_BEAT_LOOK) {
		captain->facing = FACING_NORTH_NORTHEAST;
		captain->sceneCnt = 1;
		kaja->sceneByte = KAJA_BEAT_NONE;
	} else if(kaja->sceneByte == KAJA_BEAT_NOD) {
		if(captain->sceneCnt != 0)
			fdActorSelectAnimation(captain, ANIM_NOD);
		captain->sceneCnt = 0;
		kaja->sceneByte = KAJA_BEAT_NONE;
	} else if(kaja->sceneByte == KAJA_BEAT_FACE_EAST) {
		captain->facing = FACING_EAST;
		kaja->sceneByte = KAJA_BEAT_NONE;
	}
}

/* An actor script call that hides the actor: never a talk or Psynergy target,
   without its shadow, drawn underfoot in SPRITE_COLOR_HIDDEN, every tick for
   as long as its script runs. */
static Sint32 scShipRoomsHide(ACTOR *work)
{
	SPR *sprite = work->spr;

	work->collisionFlg = ACTOR_NOT_TARGETED;
	fdActorSetSprFlags(work, 0);
	sprite->image.pri = 1;
	sprite->shadow.pri = 1;
	work->priFlg =
		(work->priFlg & ~ACTOR_PRIORITY_AUTOMATIC) | ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprCol(work, SPRITE_COLOR_HIDDEN);
	return 1;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	if(MainRoutineStateWork.next == SCENE_SHIP_ROOMS) {
		return ShipRoomsEntrances;
	}
	return ShipRoomsEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return ShipRoomsExits;
}

/* The talk facing of an actor's row in a room's actor placements. */
#define TALK_FACING_OF(placements, actorId) \
	((placements)[(actorId) - ACTOR_FIRST_PLACED].talkFacing)

/* Picks the actor placements for the room entered and the voyage's stage. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	switch(MainRoutineStateWork.entrance) {
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return ShipRoomsHoldActorPlacements;
		} else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
				/* At the island the passengers no longer turn to the party. */
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_HEAD_GUIDE) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_SWEATING_PASSENGER) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_STARED_AT_PASSENGER) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_WARRIOR) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_OLD_MAN) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_MUSCLE_ADMIRER) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_TICKET_GRUMBLER) =
					TALK_FACING_PARTY_AND_BACK;
				TALK_FACING_OF(ShipRoomsHoldActorPlacementsAtSea,
					ACTOR_SEA_HOLD_LADY) = TALK_FACING_KEEP;
			}
			return ShipRoomsHoldActorPlacementsAtSea;
		} else if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
			if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
				TALK_FACING_OF(ShipRoomsHoldActorPlacements,
					ACTOR_HOLD_WARRIOR) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacements,
					ACTOR_HOLD_SECOND_GUIDE) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacements,
					ACTOR_HOLD_TICKET_GRUMBLER) = TALK_FACING_KEEP;
				TALK_FACING_OF(ShipRoomsHoldActorPlacements,
					ACTOR_HOLD_OLD_MAN) = TALK_FACING_KEEP;
			}
			return ShipRoomsHoldActorPlacements;
		} else {
			return ShipRoomsHoldActorPlacementsEmpty;
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
		return ShipRoomsRowingDeckActorPlacements;
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_CREWMAN) = TALK_FACING_KEEP;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_DRUMMER) = TALK_FACING_KEEP;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 1) = TALK_FACING_PARTY_AND_BACK;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 2) = TALK_FACING_KEEP;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 3) = TALK_FACING_KEEP;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 4) = TALK_FACING_KEEP;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 5) = TALK_FACING_KEEP;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 6) = TALK_FACING_PARTY_AND_BACK;
		TALK_FACING_OF(ShipRoomsRowingDeckActorPlacements,
			ACTOR_ROWING_FIRST_OARSMAN + 7) = TALK_FACING_KEEP;
		return ShipRoomsRowingDeckActorPlacements;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return ShipRoomsCaptainsQuartersActorPlacementsArrived;
		} else if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
			if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
				if(fdScriptGetFlag(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
					TALK_FACING_OF(
						ShipRoomsCaptainsQuartersActorPlacementsWarriors,
						ACTOR_QUARTERS_KAJA) = TALK_FACING_PARTY_AND_BACK;
				}
				if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
					if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
						TALK_FACING_OF(
							ShipRoomsCaptainsQuartersActorPlacementsWarriors,
							ACTOR_QUARTERS_CAPTAIN) = TALK_FACING_PARTY;
					}
				}
				return ShipRoomsCaptainsQuartersActorPlacementsWarriors;
			}
			return ShipRoomsActorPlacementsNone;
		} else {
			return ShipRoomsCaptainsQuartersActorPlacements;
		}
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
		return ShipRoomsCaptainsQuartersActorPlacements;
		break;
	default:
		return ShipRoomsActorPlacementsNone;
		break;
	}
}

/* Chooses the room's interactions for the current stage of the voyage. */
const CUE *scGetCues(void)
{
	Sint32 entrance = MainRoutineStateWork.entrance;

	switch(entrance) {
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
		if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
			return ShipRoomsHoldCuesAtIsland;
		}
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
			gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
			return ShipRoomsHoldCuesChoosingRowers;
		}
		return ShipRoomsHoldCues;
	case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
	case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return ShipRoomsRowingDeckCuesArrived;
		}
		return ShipRoomsRowingDeckCues;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
		if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
			return ShipRoomsCaptainsQuartersCuesAtIsland;
		}
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return ShipRoomsCaptainsQuartersCuesArrived;
		}
		return ShipRoomsCaptainsQuartersCues;
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
		return ShipRoomsRowingDeckCuesMonstersBelowdecks;
	case SHIP_ROOMS_FROM_WORLD_MAP:
		return ShipRoomsRowingDeckCuesKrakenOnDeck;
	default:
		break;
	}

	return ShipRoomsCuesOther;
}

/* The crewman in the blue bandana, in the captain's cabin at Kalay, asks "Now
   what? You want to see the captain too?" once Ouranos and Sean have stormed
   in; a yes plays scShipRoomsDiscussLaunchingWithoutAnchor.

   Before then he says "But we can't send the ship out without hiring some
   warriors," and once the captain has decided to sail, "If the ship from Tolbi
   had come, I'd feel differently... Hmmm... I wonder where that anchor charm
   went." A no gets "Well, you don't have any business here then. Off with you
   now!" */
static void scShipRoomsTalkToCaptainsQuartersCrewman(void)
{
	fdScriptStart();
	fdEndBusyAndReveal();
	if(gmFlagGet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
		fdScriptSetMessageId(MsgTolbiShipIfShipFromTolbiHad);
		/* "If the ship from Tolbi had come, I'd feel differently, but..." */
		fdScriptShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
	} else {
		if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
			fdScriptSetMessageId(MsgTolbiShipNowWantSeeCaptainToo);
			/* "Now what? You want to see the captain too?" */
			fdScriptOpenMessage(ACTOR_QUARTERS_CREWMAN, 0);
			if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) ==
				YES) {
				scShipRoomsDiscussLaunchingWithoutAnchor();
			} else {
				fdActorStartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
				/* "Well, you don't have any business here then. Off with you
				   now!" */
				fdScriptShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
				fdActorFaceDirection(
					ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHEAST, 0);
			}
		} else {
			fdScriptSetMessageId(MsgTolbiShipButWeCantSendShip);
			/* "But we can't send the ship out without hiring some warriors." */
			fdScriptShowMessage(ACTOR_QUARTERS_CREWMAN, 0);
		}
	}
	fdScriptEnd();
}

/* Sean, in the captain's cabin while the charm is lost, asks "You came to ask
   the captain to set sail, right?" A yes gets "If we had worked together, we'd
   be out at sea right now..."; a no, "Heh heh... In that case, why don't you
   take a tour of the ship?" */
static void scShipRoomsTalkToSeanInCaptainsQuarters(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiShipYouCameAskCaptainSet);
	/* "You came to ask the captain to set sail, right?" Yes: "If we had worked
	   together, we'd be out at sea right now. But..." No: "Heh heh... In that
	   case, why don't you take a tour of the..." */
	fdScriptAskYesNo(ACTOR_QUARTERS_SEAN, 0);
	fdScriptEnd();
}

/* The captain, an old man with a white beard in his cabin, says "It's too late
   to hire mercenaries now... There's no time" at first, and "Oh, of all the bad
   luck...losing my lucky charm! This ship can't leave until we find it!" once
   it is lost.

   If Isaac is carrying the Anchor Charm by then, the field menu opens so that
   he can hand it over. With the charm back he says "We don't know what might
   happen before we arrive. Be ready for anything," and after the first attack
   he asks "An oarsman was injured?", shakes his head and says "There's no time
   to worry. We must escape the waters of the Karagol!" */
static void scShipRoomsTalkToCaptain(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
		fdScriptSetMessageId(MsgTolbiShipOarsmanWasInjured);
		/* "An oarsman was injured?" */
		scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
		fdActorFaceDirection(
			ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST, 60);
		fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_SHAKE_HEAD);
		/* "There's no time to worry..." */
		scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
		fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	} else if(fdScriptGetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		fdScriptSetMessageId(MsgTolbiShipWeDontKnowMightHappen);
		/* "We don't know what might happen before we arrive. Be ready..." */
		fdScriptShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
		fdScriptSetMessageId(MsgTolbiShipBadLuckLosingMyLucky);
		/* "Oh, of all the bad luck...losing my lucky charm! This ship..." */
		fdScriptShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
		if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) == 0 &&
			gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND) != 0) {
			FieldStateWork->fieldMenu = 1;
		}
	} else {
		fdScriptSetMessageId(MsgTolbiShipItsTooLateHireMercenaries);
		/* "It's too late to hire mercenaries now... There's no time." */
		fdScriptShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
	}
	fdScriptEnd();
}

/* The woman with a sword, a passenger in the passengers' room at Kalay, says
   "The longer we sit here, the more time we have to think..." until the Anchor
   Charm is back.

   Then she says "These proud warriors are not going to like being told to row,"
   and asks Isaac "What do you think? You guys wanna trade jobs?" A yes gets
   "You look so scary... You say you want to trade, but it's not that easy," and
   she turns away: "I see. Sure, why don't you take over the rowing." A no gets
   "I figured you'd say that. Fine. Rowing is fine with me." */
static void scShipRoomsTalkToWaitingWarrior(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		fdActorStartRepeatedMotion(ACTOR_HOLD_WARRIOR, 2);
		fdScriptSetMessageId(MsgTolbiShipTheseProudWarriorsNotGoing);
		/* "These proud warriors are not going to like being told to row." */
		scShipRoomsSay(ACTOR_HOLD_WARRIOR);
		fdActorFaceActor(ACTOR_HOLD_WARRIOR, ACTOR_PARTY_LEADER, 10);
		/* "What do you think? You guys wanna trade jobs?" */
		fdScriptOpenMessage(ACTOR_HOLD_WARRIOR, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
			fdScriptWait(40);
			/* "You look so scary... You say you want to trade, but it's
			   not..." */
			scShipRoomsSay(ACTOR_HOLD_WARRIOR);
			scShipRoomsTurn(ACTOR_HOLD_WARRIOR, FACING_SOUTH_SOUTHEAST);
			/* "I see. Sure, why don't you take over the rowing." */
			fdScriptShowMessage(ACTOR_HOLD_WARRIOR, 0);
		} else {
			FieldStateWork->msg += 2;
			/* "I figured you'd say that. Fine. Rowing is fine with me." */
			fdScriptShowMessage(ACTOR_HOLD_WARRIOR, 0);
			fdActorFaceDirection(ACTOR_HOLD_WARRIOR, FACING_SOUTH_SOUTHEAST, 0);
		}
	} else {
		fdScriptSetMessageId(MsgTolbiShipLongerWeSitHereMore);
		/* "The longer we sit here, the more time we have to think. I..." */
		fdScriptShowMessage(ACTOR_HOLD_WARRIOR, 0);
	}
	fdScriptEnd();
}

/* The woman with fair hair, a passenger in the passengers' room at Kalay,
   admires "a whole bunch of really muscular men on the floor below", and once
   the Anchor Charm is back asks Isaac "Are you going to row?"

   A yes gets "Hmmm. So, you have to row, even though you're a fighter, huh?"; a
   no, "Oh. Helping to protect the ship suits you better, doesn't it?" */
static void scShipRoomsTalkToWaitingMuscleAdmirer(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		fdScriptSetMessageId(MsgTolbiShipYouGoingRow);
		/* "Are you going to row?" Yes: "Hmmm. So, you have to row, even though
		   you're a fighter, huh?" No: "Oh. Helping to protect the ship suits
		   you better, doesn't..." */
		fdScriptAskYesNo(ACTOR_HOLD_MUSCLE_ADMIRER, 0);
	} else {
		fdScriptSetMessageId(MsgTolbiShipTheresWholeBunchReallyMuscular);
		/* "There's a whole bunch of really muscular men on the floor..." */
		fdScriptShowMessage(ACTOR_HOLD_MUSCLE_ADMIRER, 0);
	}
	fdScriptEnd();
}

/* A passenger in the hold under sail, when an oar needs a rower.

   If someone has already been chosen for the open seat, they step beside the
   leader and follow them out while this one says chosenMessage. Otherwise the
   party may ask this one to row (question) at the seat the attacks fought off
   number.

   The Ask...ToRow functions are this with each passenger's lines, except the
   stared-at passenger's and the second guide's, which write it out in full. */
#define TOLBI_SHIP_ASK_TO_ROW(speaker, chosenMessage, question, passenger) \
	if(gmFlagGet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) { \
		Sint32 rowerId = scShipRoomsGetChosenOarsman(); \
\
		fdScriptStart(); \
		scShipRoomsStepBesideLeader(rowerId); \
		fdScriptSetMessageId(chosenMessage); \
		scShipRoomsSay(speaker); \
		scWalkIntoLeader(rowerId); \
		fdScriptEnd(); \
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) { \
		scShipRoomsAskToRow(speaker, question, \
			FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + (passenger)); \
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) { \
		scShipRoomsAskToRow(speaker, question, \
			FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + (passenger)); \
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) { \
		scShipRoomsAskToRow(speaker, question, \
			FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger)); \
	} else { \
		scShipRoomsAskToRow(speaker, question, \
			FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger)); \
	}

/* Isaac asks the tour guide, the man with a pack in the passengers' room, to
   row after an attack, and he wails "Nooooo! You're not going to make me row,
   are you?"

   A yes gets "I've never done much physical labor, but if I must, then I guess
   I must," and he follows Isaac. A no gets "I'm just a businessman. I'm not
   strong enough to be of any use." Once someone else has been chosen, the
   chosen one steps out beside Isaac and the guide says "Looks like you've been
   chosen to help row, doesn't it? So, get on with the rowing!" */
static void scShipRoomsAskHeadGuideToRow(void)
{
	TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_HEAD_GUIDE,
		MsgTolbiShipLooksLikeYouveBeenChosen, MsgTolbiShipNoooooNotGoing,
		PASSENGER_HEAD_GUIDE);
}

/* Isaac asks the chef in the passengers' room to row after an attack: "You...
   You must be joking. You want me to row?"

   A yes gets "You jerk! What do you have against me?", and he follows Isaac. A
   no gets "Ye-Yeah. Besides, a spatula suits me better than an oar." Once
   someone else has been chosen he laughs, "Ha ha ha! Chosen to be an oarsman,
   huh?" */
static void scShipRoomsAskSweatingPassengerToRow(void)
{
	TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_SWEATING_PASSENGER,
		MsgTolbiShipHaHaHaChosenOarsman, MsgTolbiShipJokingWantRow,
		PASSENGER_SWEATING);
}

/* Isaac asks the young woman in green in the passengers' room to row after an
   attack: "Don't tell me you're going to ask a lady to row?"

   A yes gets "What? I don't believe this. You're evil! A monster! I won't
   forget this!", and she follows Isaac. A no gets "You startled me, sir. Don't
   frighten me like that!" Once someone else has been chosen she says "So you
   finally picked someone, didn't you? Show your stuff!" Off the island she is
   seasick: she trembles and says "I don't feel so well.. Don't talk to
   me..." */
static void scShipRoomsAskLadyToRow(void)
{
	if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
		fdScriptStart();
		fdActorSetAttachedEffect(ACTOR_SEA_HOLD_LADY,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		fdScriptSetMessageId(MsgTolbiShipDontFeelDontTalkMe);
		/* "I don't feel so well.. Don't talk to me..." */
		fdScriptShowMessage(ACTOR_SEA_HOLD_LADY, 0);
		fdScriptEnd();
	} else {
		TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_LADY,
			MsgTolbiShipYouFinallyPickedSomeoneDidnt, MsgTolbiShipDontTellGoing,
			PASSENGER_LADY);
	}
}

/* Isaac asks the bald, muscular man in the passengers' room to row after an
   attack, and he flinches: "What? Ohhhh, noooo... Are you going to make me
   row?"

   A yes gets "Huh? You DO want me to row? Okay, okay. I'm going already...",
   and he follows Isaac. A no gets "What? You're not going to pick me? You
   really are a nice guy!" Once someone else has been chosen he says "It gives
   me chills to think that could have been me." The steps are the other
   passengers' (TOLBI_SHIP_ASK_TO_ROW), written out with a flinch first. */
static void scShipRoomsAskStaredAtPassengerToRow(void)
{
	Sint32 rowerId;

	fdScriptStart();
	if(fdScriptGetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
		rowerId = scShipRoomsGetChosenOarsman();
		scShipRoomsStepBesideLeader(rowerId);
		fdScriptSetMessageId(MsgTolbiShipGivesMeChillsThinkCould);
		/* "It gives me chills to think that could have been me." */
		scShipRoomsSay(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
		scWalkIntoLeader(rowerId);
	} else {
		fdActorRunRepeatedMotion(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 2);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgTolbiShipOhhhhNooooGoingMakeMe);
		/* "What? Ohhhh, noooo... Are you going to make me row?" */
		fdScriptOpenMessage(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
			/* "Huh? You DO want me to row? Okay, okay. I'm going already..." */
			scShipRoomsSay(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
			scWalkIntoLeader(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
			gmFlagSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
			if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
				gmFlagSet(
					FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + PASSENGER_STARED_AT);
			} else if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
				gmFlagSet(
					FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + PASSENGER_STARED_AT);
			} else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
				0) {
				gmFlagSet(
					FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + PASSENGER_STARED_AT);
			} else {
				gmFlagSet(
					FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_STARED_AT);
			}
		} else {
			FieldStateWork->msg++;
			/* "What? You're not going to pick me? You really are a nice
			   guy!" */
			scShipRoomsSay(ACTOR_SEA_HOLD_STARED_AT_PASSENGER);
		}
	}
	fdScriptEnd();
}

/* Isaac asks the tour's other guide, in the passengers' room, to row after an
   attack. He laughs, "Ha ha ha. Rowing, right? Feel free to choose whomever you
   like," then starts: "What? Me? You must be joking! Really?"

   A yes gets "Awwwww! Oh, this is just too much to handle," and he follows
   Isaac. A no gets "Phew! Don't frighten me like that!" Once someone else has
   been chosen he says "Hey, Isaac. You've got a good eye!" */
static void scShipRoomsAskSecondGuideToRow(void)
{
	Sint32 rowerId;

	fdScriptStart();
	if(fdScriptGetFlag(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
		rowerId = scShipRoomsGetChosenOarsman();
		scShipRoomsStepBesideLeader(rowerId);
		fdScriptSetMessageId(MsgTolbiShipIsaacYouveGotGoodEye);
		/* "Hey, Isaac. You've got a good eye!" */
		scShipRoomsSay(ACTOR_SEA_HOLD_SECOND_GUIDE);
		scWalkIntoLeader(rowerId);
	} else {
		fdScriptSetMessageId(MsgTolbiShipHaHaHaRowingFeel);
		/* "Ha ha ha. Rowing, right? Feel free to choose whomever you like." */
		fdScriptShowMessageThenWait(ACTOR_SEA_HOLD_SECOND_GUIDE, 0, 60);
		fdActorRunRepeatedMotion(ACTOR_SEA_HOLD_SECOND_GUIDE, 1);
		/* "What? Me? You must be joking! Really?" */
		fdScriptOpenMessage(ACTOR_SEA_HOLD_SECOND_GUIDE, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
			/* "Awwwww! Oh, this is just too much to handle." */
			scShipRoomsSay(ACTOR_SEA_HOLD_SECOND_GUIDE);
			scWalkIntoLeader(ACTOR_SEA_HOLD_SECOND_GUIDE);
			gmFlagSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
			if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
				gmFlagSet(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS +
					PASSENGER_SECOND_GUIDE);
			} else if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
				gmFlagSet(
					FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + PASSENGER_SECOND_GUIDE);
			} else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
				0) {
				gmFlagSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS +
					PASSENGER_SECOND_GUIDE);
			} else {
				gmFlagSet(
					FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_SECOND_GUIDE);
			}
		} else {
			FieldStateWork->msg++;
			/* "Phew! Don't frighten me like that!" */
			scShipRoomsSay(ACTOR_SEA_HOLD_SECOND_GUIDE);
		}
	}
	fdScriptEnd();
}

/* Isaac asks the woman with a sword in the passengers' room to row after an
   attack: "Hey! You're not thinking of making me an oarsman, are you?"

   A yes gets "Grrr... I can't believe this guy is actually choosing me...", and
   she follows Isaac. A no gets "Heh-heh! Not picking me is a smart choice,
   buddy." Once someone else has been chosen she says "That's who you picked to
   be an oarsman? Good choice! ...Looks very...oarsmanly." */
static void scShipRoomsAskWarriorToRow(void)
{
	TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_WARRIOR,
		MsgTolbiShipThatsWhoYouPickedOarsman, MsgTolbiShipNotThinkingMaking,
		PASSENGER_WARRIOR);
}

/* Isaac asks the young man with brown hair in the passengers' room, who
   grumbled about the price of the tickets, to row after an attack: "What? Me?
   An oarsman? Give me a break! Seriously?"

   A yes gets "Oh, you are serious... Fine! I'll do it...", and he follows
   Isaac. A no gets "Hahhh... You scared me so bad, my heart nearly stopped!"
   Once someone else has been chosen he says "If you've chosen an oarsman, let's
   get to rowing!" */
static void scShipRoomsAskTicketGrumblerToRow(void)
{
	TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_TICKET_GRUMBLER,
		MsgTolbiShipIfYouveChosenOarsmanLets, MsgTolbiShipOarsmanGiveBreak,
		PASSENGER_TICKET_GRUMBLER);
}

/* Isaac asks the woman with fair hair in the passengers' room, who admired the
   oarsmen's muscles, to row after an attack: "That poor oarsman. I feel so
   sorry for him. What? You want me to take his place?"

   A yes gets "How can you ask a young lady to take his place? It's not
   fair!!!", and she follows Isaac. A no gets "That's right... You can't ask a
   proper lady to do such a filthy job!" Once someone else has been chosen she
   says "You've been chosen as an oarsman? Aye, now that's an honor!" */
static void scShipRoomsAskMuscleAdmirerToRow(void)
{
	TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_MUSCLE_ADMIRER,
		MsgTolbiShipYouveBeenChosenAsOarsman, MsgTolbiShipPoorOarsmanFeel,
		PASSENGER_MUSCLE_ADMIRER);
}

/* Isaac asks the old man with a cane in the passengers' room to row after an
   attack: "Out of all these people, you're asking a frail old man like me to
   row?"

   A yes gets "You'll regret this when I collapse from exhaustion or
   something!", and he follows Isaac. A no gets "Th... That's right... Nobody
   would ever choose me." Once someone else has been chosen he asks "Ho ho! Is
   that person going to get us to Tolbi?" */
static void scShipRoomsAskOldManToRow(void)
{
	TOLBI_SHIP_ASK_TO_ROW(ACTOR_SEA_HOLD_OLD_MAN,
		MsgTolbiShipHoHoPersonGoingGet, MsgTolbiShipPeopleAskingFrail,
		PASSENGER_OLD_MAN);
}

#undef TOLBI_SHIP_ASK_TO_ROW

/* Finds a placed actor within twelve pixels of the requested floor position. */
static ACTOR *scShipRoomsFindActorNear(Sint32 x, Sint32 z)
{
	FIELD_STATE *state;
	ACTOR	   **actor;
	ACTOR		*cur;
	Uint32		 i;
	Sint32		 actorX;
	Sint32		 actorZ;
	Sint32		 left;
	Sint32		 top;
	Sint32		 right;
	Sint32		 bottom;

	state = FieldStateWork;
	i = ACTOR_FIRST_PLACED;
	left = x - 12;
	right = x + 12;
	top = z - 12;
	bottom = z + 12;
	actor = &state->actors[ACTOR_FIRST_PLACED];
	for(; i <= ACTOR_LAST_ID; i++) {
		cur = *actor++;
		actorX = cur->x.part[0].integer;
		actorZ = cur->z.part[0].integer;
		if(left < actorX && right > actorX && top < actorZ && bottom > actorZ)
			return cur;
	}
	return 0;
}

/* The chosen oarsman steps out beside the leader, on the leader's right if that
   spot is free, else on the left, else behind, and turns the way the leader
   faces. */
static void scShipRoomsStepBesideLeader(Sint32 passengerId)
{
	ACTOR *leader;
	Sint32 direction;
	Sint32 blocked;
	ACTOR *passenger;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	blocked = 1;
	fdActorSetSprPriority(passengerId, 2);
	passenger = fdActorGet(passengerId);

	passenger->priFlg = ACTOR_PRIORITY_AUTOMATIC | passenger->priFlg;
	/* Try the leader's right, then left, then the space behind. */
	direction = FACING_INDEX(FACING_SIXTEENTH(leader->facing + DEGtoANG(90)));
	if(scShipRoomsIsSpotFree(direction) != 0) {
		blocked = 0;
	}
	if(blocked != 0) {
		direction =
			FACING_INDEX(FACING_SIXTEENTH(leader->facing - DEGtoANG(90)));
		if(scShipRoomsIsSpotFree(direction) != 0) {
			blocked = 0;
		}
		if(blocked != 0) {
			direction =
				FACING_INDEX(FACING_SIXTEENTH(leader->facing + DEGtoANG(180)));
		}
	}
	{
		ACTOR *start = fdActorGet(ACTOR_PARTY_LEADER);

		if(start != NULL) {
			fdActorSetPosFX(passengerId, start->x.fixed, start->z.fixed);
		}
	}
	fdActorSetSpeed(passengerId, 1.6);
	fdActorSetAnimation(passengerId, ANIM_WALK);
	{
		Sint32 step = PushedActorDirectionSteps[direction];

		fdActorSetDestinationOffset(
			passengerId, DIRECTION_STEP_X(step), DIRECTION_STEP_Z(step));
	}
	fdActorWaitForMoveById(passengerId);
	fdActorSetAnimation(passengerId, ANIM_STAND);
	scShipRoomsTurn(passengerId, leader->facing);
}

/* Kaja rowing speaks with the window below her and her portrait on the
   right. */
enum {
	KAJA_ROWING_SPEAKER = SPEAKER(
		ACTOR_ROWING_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT)
};

/* Returns 1 when the spot next to the leader in the given sixteenth of a turn
   holds no actor and no wall, else 0. */
static Bool scShipRoomsIsSpotFree(Sint32 direction)
{
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	Uint32 step = PushedActorDirectionSteps[direction];
	Sint32 x = actor->x.part[0].integer + DIRECTION_STEP_X(step);
	Sint32 z = actor->z.part[0].integer + DIRECTION_STEP_Z(step);

	if(scShipRoomsFindActorNear(x, z) != 0) {
		return FALSE;
	}

	{
		FIELD_POS point;
		point.x = toFIXED(x);
		point.y = actor->y.fixed;
		point.z = toFIXED(z);

		if(syFieldActorCheckMovementCollision(actor, &point) != 0) {
			return FALSE;
		}
	}

	return TRUE;
}

/* Returns the passenger chosen to row at the open seat, which the attacks
   fought off number. */
static Sint32 scShipRoomsGetChosenOarsman(void)
{
	Sint32 seat = 0;

	if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
		seat = 3;
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
		seat = 2;
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
		seat = 1;
	}

	return scShipRoomsFindSeatRower(seat, 1);
}

/* What a passenger at the oars says, or thinks under Mind Read, while the
   Kraken is on deck, by the seat they were chosen for.

   That is the first seat's line, the second's, or a later one's. The
   TalkTo...AtOars and Read...MindAtOars functions are this, or
   TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER, with each passenger's lines. */
#define TOLBI_SHIP_ROWER_LINE_BY_SEAT( \
	passenger, actorId, first, second, later) \
	fdScriptStart(); \
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger))) \
		fdScriptSetMessageId(first); \
	else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + (passenger))) \
		fdScriptSetMessageId(second); \
	else \
		fdScriptSetMessageId(later); \
	fdScriptShowMessage(actorId, 0); \
	fdScriptEnd()

/* The same with one line for the first seat and one for any later. */
#define TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(passenger, actorId, first, later) \
	fdScriptStart(); \
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + (passenger))) \
		fdScriptSetMessageId(first); \
	else \
		fdScriptSetMessageId(later); \
	fdScriptShowMessage(actorId, 0); \
	fdScriptEnd()

/* The tour guide, rowing while the Kraken is on deck, says "That thing has Kaja
   and his men worried. It's going to be a tough fight" if he was the first
   passenger chosen.

   Chosen second he says "Monsters everywhere, and I'm stuck rowing... I should
   have stayed home!", and chosen third, "The ship is starting to list... If we
   don't do something, she'll sink!" Every passenger at the oars but the two
   women in green and fair hair says the same three lines. */
static void scShipRoomsTalkToHeadGuideAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_HEAD_GUIDE, ACTOR_ROWING_HEAD_GUIDE,
		MsgTolbiShipThingHasKajaHisMen,
		MsgTolbiShipMonstersEverywhereImStuckRowing,
		MsgTolbiShipShipStartingListIfWe);
}

/* The tour's other guide, rowing while the Kraken is on deck, says "That thing
   has Kaja and his men worried..." if he was chosen first, "Monsters
   everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
   list..." if third, as the tour guide does. */
static void scShipRoomsTalkToSecondGuideAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_SECOND_GUIDE,
		ACTOR_ROWING_SECOND_GUIDE, MsgTolbiShipThingHasKajaHisMen,
		MsgTolbiShipMonstersEverywhereImStuckRowing,
		MsgTolbiShipShipStartingListIfWe);
}

/* The chef, rowing while the Kraken is on deck, says "That thing has Kaja and
   his men worried..." if he was chosen first, "Monsters everywhere, and I'm
   stuck rowing..." if second, and "The ship is starting to list..." if third,
   as the tour guide does. */
static void scShipRoomsTalkToSweatingPassengerAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_SWEATING,
		ACTOR_ROWING_SWEATING_PASSENGER, MsgTolbiShipThingHasKajaHisMen,
		MsgTolbiShipMonstersEverywhereImStuckRowing,
		MsgTolbiShipShipStartingListIfWe);
}

/* The young woman in green, rowing while the Kraken is on deck, asks "How many
   monsters are out there?" if she was the first passenger chosen, and otherwise
   grumbles "Another monster? This isn't the first-class passage I paid for!" */
static void scShipRoomsTalkToLadyAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(PASSENGER_LADY, ACTOR_ROWING_LADY,
		MsgTolbiShipHowManyMonstersOutThere,
		MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/* The bald, muscular man, rowing while the Kraken is on deck, says "That thing
   has Kaja and his men worried..." if he was chosen first, "Monsters
   everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
   list..." if third, as the tour guide does. */
static void scShipRoomsTalkToStaredAtPassengerAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_STARED_AT,
		ACTOR_ROWING_STARED_AT_PASSENGER, MsgTolbiShipThingHasKajaHisMen,
		MsgTolbiShipMonstersEverywhereImStuckRowing,
		MsgTolbiShipShipStartingListIfWe);
}

/* The woman with a sword, rowing while the Kraken is on deck, says "That thing
   has Kaja and his men worried..." if she was chosen first, "Monsters
   everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
   list..." if third, as the tour guide does. */
static void scShipRoomsTalkToWarriorAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_WARRIOR, ACTOR_ROWING_WARRIOR,
		MsgTolbiShipThingHasKajaHisMen,
		MsgTolbiShipMonstersEverywhereImStuckRowing,
		MsgTolbiShipShipStartingListIfWe);
}

/* The young man with brown hair, rowing while the Kraken is on deck, says "That
   thing has Kaja and his men worried..." if he was chosen first, "Monsters
   everywhere, and I'm stuck rowing..." if second, and "The ship is starting to
   list..." if third, as the tour guide does. */
static void scShipRoomsTalkToTicketGrumblerAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_TICKET_GRUMBLER,
		ACTOR_ROWING_TICKET_GRUMBLER, MsgTolbiShipThingHasKajaHisMen,
		MsgTolbiShipMonstersEverywhereImStuckRowing,
		MsgTolbiShipShipStartingListIfWe);
}

/* The woman with fair hair, rowing while the Kraken is on deck, asks "How many
   monsters are out there?" if she was the first passenger chosen, and otherwise
   grumbles "Another monster? This isn't the first-class passage I paid for!",
   as the woman in green does. */
static void scShipRoomsTalkToMuscleAdmirerAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(PASSENGER_MUSCLE_ADMIRER,
		ACTOR_ROWING_MUSCLE_ADMIRER, MsgTolbiShipHowManyMonstersOutThere,
		MsgTolbiShipAnotherMonsterIsntFirstClass);
}

/* Mind Read on the tour guide, rowing while the Kraken is on deck, hears "I
   don't care what it takes, just hurry up and slay that beast!" if he was the
   first passenger chosen.

   Chosen second he thinks "If those monsters come back...go teach 'em another
   lesson!", and chosen third, "The boat's rocking so much. I'm certain we're
   doomed!" Every passenger at the oars but the two women in green and fair hair
   thinks the same three thoughts. */
static void scShipRoomsReadHeadGuideMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_HEAD_GUIDE, ACTOR_ROWING_HEAD_GUIDE,
		MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
		MsgTolbiShipBoatsRockingMuchImCertain);
}

/* Mind Read on the tour's other guide at the oars, while the Kraken is on deck,
   hears "I don't care what it takes, just hurry up and slay that beast!", "If
   those monsters come back...go teach 'em another lesson!" or "The boat's
   rocking so much. I'm certain we're doomed!", by whether he was chosen first,
   second or third. */
static void scShipRoomsReadSecondGuideMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_SECOND_GUIDE,
		ACTOR_ROWING_SECOND_GUIDE, MsgTolbiShipDontCareTakesJustHurry,
		MsgTolbiShipIfThoseMonstersComeBack,
		MsgTolbiShipBoatsRockingMuchImCertain);
}

/* Mind Read on the chef at the oars, while the Kraken is on deck, hears "I
   don't care what it takes, just hurry up and slay that beast!", "If those
   monsters come back...go teach 'em another lesson!" or "The boat's rocking so
   much. I'm certain we're doomed!", by whether he was chosen first, second or
   third. */
static void scShipRoomsReadSweatingPassengerMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_SWEATING,
		ACTOR_ROWING_SWEATING_PASSENGER, MsgTolbiShipDontCareTakesJustHurry,
		MsgTolbiShipIfThoseMonstersComeBack,
		MsgTolbiShipBoatsRockingMuchImCertain);
}

/* Mind Read on the young woman in green at the oars, while the Kraken is on
   deck, hears "I had no idea there were so many monsters out at sea..." if she
   was the first passenger chosen, and otherwise "We're surrounded by monsters,
   and still there's no sign of land!" */
static void scShipRoomsReadLadyMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(PASSENGER_LADY, ACTOR_ROWING_LADY,
		MsgTolbiShipHadIdeaThereWereMany,
		MsgTolbiShipWereSurroundedByMonstersStill);
}

/* Mind Read on the bald, muscular man at the oars, while the Kraken is on deck,
   hears "I don't care what it takes, just hurry up and slay that beast!", "If
   those monsters come back...go teach 'em another lesson!" or "The boat's
   rocking so much. I'm certain we're doomed!", by whether he was chosen first,
   second or third. */
static void scShipRoomsReadStaredAtPassengerMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_STARED_AT,
		ACTOR_ROWING_STARED_AT_PASSENGER, MsgTolbiShipDontCareTakesJustHurry,
		MsgTolbiShipIfThoseMonstersComeBack,
		MsgTolbiShipBoatsRockingMuchImCertain);
}

/* Mind Read on the woman with a sword at the oars, while the Kraken is on deck,
   hears "I don't care what it takes, just hurry up and slay that beast!", "If
   those monsters come back...go teach 'em another lesson!" or "The boat's
   rocking so much. I'm certain we're doomed!", by whether she was chosen first,
   second or third. */
static void scShipRoomsReadWarriorMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_WARRIOR, ACTOR_ROWING_WARRIOR,
		MsgTolbiShipDontCareTakesJustHurry, MsgTolbiShipIfThoseMonstersComeBack,
		MsgTolbiShipBoatsRockingMuchImCertain);
}

/* Mind Read on the young man with brown hair at the oars, while the Kraken is
   on deck, hears "I don't care what it takes, just hurry up and slay that
   beast!", "If those monsters come back...go teach 'em another lesson!" or "The
   boat's rocking so much. I'm certain we're doomed!", by whether he was chosen
   first, second or third. */
static void scShipRoomsReadTicketGrumblerMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_BY_SEAT(PASSENGER_TICKET_GRUMBLER,
		ACTOR_ROWING_TICKET_GRUMBLER, MsgTolbiShipDontCareTakesJustHurry,
		MsgTolbiShipIfThoseMonstersComeBack,
		MsgTolbiShipBoatsRockingMuchImCertain);
}

/* Mind Read on the woman with fair hair at the oars, while the Kraken is on
   deck, hears "I had no idea there were so many monsters out at sea..." if she
   was the first passenger chosen, and otherwise "We're surrounded by monsters,
   and still there's no sign of land!" */
static void scShipRoomsReadMuscleAdmirerMindAtOars(void)
{
	TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER(PASSENGER_MUSCLE_ADMIRER,
		ACTOR_ROWING_MUSCLE_ADMIRER, MsgTolbiShipHadIdeaThereWereMany,
		MsgTolbiShipWereSurroundedByMonstersStill);
}

#undef TOLBI_SHIP_ROWER_LINE_BY_SEAT
#undef TOLBI_SHIP_ROWER_LINE_FIRST_OR_LATER

/* Asks a passenger to row. */
static void scShipRoomsAskToRow(Sint32 speaker, Sint32 question, Sint32 seatFlg)
{
	fdScriptStart();
	fdScriptSetMessageId(question);
	fdScriptOpenMessage(speaker, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
		scShipRoomsSay(speaker);
		scWalkIntoLeader(speaker);
		gmFlagSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
		gmFlagSet(seatFlg);
	} else {
		FieldStateWork->msg++;
		scShipRoomsSay(speaker);
	}
	fdScriptEnd();
}

/* The travelling Healer, the bearded man in a long robe in the south-east
   corner of the passengers' room, opens his Sanctum when Isaac faces him from
   the west.

   Spoken to from any other side, he says "I'm spreading goodwill wherever I
   travel" at Kalay, "The ship's crew is ready for anything, and I'll be right
   here for them. Ask the others to do the rowing" once the voyage is under way,
   "I hate arguing..." off the island and "It is good that the ship has arrived
   safely" at Tolbi. */
static void scShipRoomsTalkToHealer(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader->facing < FACING_SOUTHEAST || leader->facing > FACING_NORTHEAST) {
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
			gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
			mnSanctumRun(ACTOR_SEA_HOLD_HEALER);
		} else {
			mnSanctumRun(ACTOR_HOLD_HEALER);
		}
	} else {
		fdScriptStart();
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			fdScriptSetMessageId(MsgTolbiShipGoodShipHasArrivedSafely);
		} else if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
			fdScriptSetMessageId(MsgTolbiShipHateArguing);
		} else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			fdScriptSetMessageId(MsgTolbiShipShipsCrewReadyAnything);
		} else if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
			fdScriptSetMessageId(MsgTolbiShipShipsCrewReadyForAnything);
		} else {
			fdScriptSetMessageId(MsgTolbiShipImSpreadingGoodwillWhereverTravel);
		}
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0 &&
			gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
			fdScriptShowMessage(ACTOR_SEA_HOLD_HEALER, 0);
		} else {
			fdScriptShowMessage(ACTOR_HOLD_HEALER, 0);
		}
		fdScriptEnd();
	}
}

/* Isaac walks up to one of the monsters floating over the rowing deck after an
   attack and talks to it, and the battle starts: three Man o' Wars the first
   time, a Lizard Fighter and two Rabid Bats the second, a Virago and two Man o'
   Wars the third.

   The battle is the Tolbi ship's encounter table's first, second or third
   scripted battle, and afterwards the party comes back to the rowing deck for
   scShipRoomsEndFirstAttack or the one after it. */
static void scShipRoomsFightRowingDeckMonsters(void)
{
	fdScriptStart();
	fdEndBusyAndReveal();
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	gmFlagClear(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) == 0) {
		fdScriptSetAfterBattleDestination(
			SCENE_SHIP_ROOMS, SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 0);
	} else {
		if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) == 0) {
			fdScriptSetAfterBattleDestination(SCENE_SHIP_ROOMS,
				SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE);
			fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 1);
		} else {
			if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) == 0) {
				fdScriptSetAfterBattleDestination(SCENE_SHIP_ROOMS,
					SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE);
				fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 2);
			}
		}
	}
	fdScriptEnd();
}

/* Walking onto the stairs up from the rowing deck (metatile (27, 8)) while
   monsters are aboard has the crewman call "Where are you going, Isaac? Slay
   the beasts and save my mates!", and Isaac walks back off the stairs. */
static void scShipRoomsCallLeaderBackToBeasts(void)
{
	if(gmFlagGet(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS) != 0) {
		fdScriptStart();
		fdMessageSetVoiceFromActor(ACTOR_ROWING_CREWMAN);
		/* "Where are you going, Isaac? Slay the beasts and save my mates!" */
		mnFieldMessageShowCentered(
			MsgTolbiShipWhereGoingSlay, CENTERED_MESSAGE_WINDOW, 8);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
		scShipRoomsTurn(ACTOR_PARTY_LEADER, FACING_SOUTH);
		fdScriptEnd();
	}
}

/* Stepping inside the door of the captain's cabin at Kalay (metatiles (28, 42),
   (29, 42), (27, 43) and (29, 43)) after Ouranos and Sean have gone there,
   Isaac hears the captain, the crewman and Kaja talk of the delay, if he has
   not heard it on deck already.

   "I wonder what could have happened," the captain says of the ship from Tolbi.
   Kaja says the oarsmen are fearless, but if monsters appear at sea he can't
   handle them alone; the captain can let no one but Kaja fight, since the
   oarsmen will have their hands full: "Hmmm. What to do..." It plays once. */
static void scShipRoomsWatchCrewWonderAtDelay(void)
{
	if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0)
		return;

	fdScriptStart();
	fdEndBusyAndReveal();
	fdCameraSetSpeed(1.6);
	scShipRoomsMoveCameraAndWait(
		448, CAMERA_KEEP, 638, SHIP_CAMERA_WAIT_FOR_ARRIVAL | 40);
	fdScriptSetMessageId(MsgTolbiShipWonderCouldHaveHappened);

	/* "I wonder what could have happened." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	/* "You mean you still intend to cross this dangerous sea?" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST);
	/* "It would be too big a loss to let so many passengers get away..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	scShipRoomsTurn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHEAST);
	/* "Say, Kaja, how are the oarsmen doing?" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST);
	/* "They're fearless. There's no problem there. It's just that..." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);

	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_EAST, 20);
	/* ""It's just that"...what?" */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_WEST);
	/* "If a whole bunch of...them appear while we're crossing the sea..." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	/* "I'm sorry, but you'll have to..." */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	/* "Remember, the oarsmen will have their hands full rowing..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	scShipRoomsTurn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST);
	/* "Hmmm. What to do..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);

	gmFlagSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
	fdScriptEnd();
}

/* Walking north into the passengers' room at Kalay (metatiles (9..11, 32)) once
   the tour group is aboard, Isaac overhears Ouranos threaten "If we aren't
   going to set sail right away, I'm going to go ashore!"

   The tour's other guide begs him to be patient. Sean explains that his friend
   Ouranos, huge as he is, can't stand water, and the two decide to go to the
   captain's quarters and make him set sail, ignoring the guide's "I don't want
   any violence!" They march out, the guide hurries after them, and it plays
   once. */
static void scShipRoomsWatchWarriorsStormOff(void)
{
	Sint32		  guideSpeaker;
	Sint32		  seanSpeaker;
	const Sint32 *action;

	/* Not before the party has heard of the delay. */
	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0)
		return;

	fdScriptStart();
	fdEndBusyAndReveal();
	fdCameraSetSpeed(2.4);
	/* The hold's camera box is one screen wide, so the camera stays at x 150
	   whatever x is asked for. */
	scShipRoomsMoveCameraAndWait(
		1463, CAMERA_KEEP, 464, SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
	fdActorRunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
	fdScriptSetMessageId(MsgTolbiShipIfWeArentGoingSet);
	/* "If we aren't going to set sail right away, I'm going to go..." */
	scShipRoomsSay(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
	scShipRoomsTurn(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH_NORTHEAST);
	guideSpeaker = SPEAKER(ACTOR_HOLD_SECOND_GUIDE, SPEAKER_WINDOW_BELOW);
	fdActorShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
	/* "Yeah, but... we're going to set sail real soon, so please be..." */
	scShipRoomsSay(guideSpeaker);
	fdActorRunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
	/* "When is "real soon"?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER(ACTOR_HOLD_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT),
		0, 20);
	scShipRoomsTurn(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST);
	seanSpeaker =
		SPEAKER(ACTOR_HOLD_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT);
	fdActorShowEmote(
		ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SEAN, 3);
	/* "I'm asking how long real soon is going to be!" */
	scShipRoomsSay(seanSpeaker);
	fdActorSetAttachedEffect(ACTOR_HOLD_SECOND_GUIDE,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 3);
	/* "You're wondering when. . . Well..." */
	scShipRoomsSay(guideSpeaker);
	fdActorRunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
	/* "We're planning to go see Colosso." */
	scShipRoomsSay(seanSpeaker);
	scShipRoomsTurn(ACTOR_HOLD_SEAN, FACING_NORTH_NORTHWEST);
	/* "You know, my friend Ouranos here is scared of ships." */
	scShipRoomsSay(seanSpeaker);
	scShipRoomsTurn(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH_NORTHEAST);
	fdActorShowEmote(
		ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 1);
	/* "Ouranos? But he's huge!" */
	scShipRoomsSay(guideSpeaker);
	fdActorSetAnimationAndWait(ACTOR_HOLD_OURANOS, ANIM_SHAKE_HEAD);
	/* "It's not that I'm scared. I just can't stand water..." */
	scShipRoomsSay(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
	fdActorStartRepeatedMotion(ACTOR_HOLD_OURANOS, 2);
	/* "I'd rather be bitten by poisonous spiders than sail across the..." */
	scShipRoomsSay(SPEAKER(ACTOR_HOLD_OURANOS, SPEAKER_PORTRAIT_RIGHT));
	fdActorSetAnimation(ACTOR_HOLD_SECOND_GUIDE, ANIM_SHAKE_HEAD);
	/* "Hmmm. Spiders, huh?" */
	scShipRoomsSay(guideSpeaker);
	fdActorSetAnimationAndWait(ACTOR_HOLD_SEAN, ANIM_SHAKE_HEAD);
	/* "And even Ouranos is ready to cross the sea right now." */
	scShipRoomsSay(seanSpeaker);
	scShipRoomsTurn(ACTOR_HOLD_SEAN, FACING_WEST);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SEAN, 2);
	/* "Well, if we're not going now, then when are we going to set..." */
	fdScriptShowMessageThenWait(seanSpeaker, 0, 20);
	fdActorFaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	/* "That's... not an easy question." */
	fdScriptShowMessageThenWait(guideSpeaker, 0, 20);
	fdActorShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorShowEmote(ACTOR_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SEAN, 2);
	/* "You mean you wouldn't know even if I asked?" */
	scShipRoomsSay(seanSpeaker);
	scShipRoomsTurn(ACTOR_HOLD_SEAN, FACING_NORTH_NORTHWEST);
	fdActorRunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
	/* "We're going, Ouranos." */
	scShipRoomsSay(seanSpeaker);
	scShipRoomsTurn(ACTOR_HOLD_OURANOS, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_HOLD_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(
		ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
	/* "Going? Where?" */
	scShipRoomsSay(ACTOR_HOLD_OURANOS);
	fdActorShowEmote(ACTOR_HOLD_SEAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorRunRepeatedMotion(ACTOR_HOLD_SEAN, 1);
	/* "Where else? To the captain's quarters, of course." */
	scShipRoomsSay(seanSpeaker);
	fdActorFaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_HOLD_OURANOS, FACING_SOUTH_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_EAST, 0);
	scShipRoomsTurn(ACTOR_HOLD_OURANOS, FACING_SOUTH_SOUTHEAST);
	fdActorRunRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
	/* "What do you think you're going to do in the captain's quarters?" */
	fdScriptShowMessageThenWait(guideSpeaker, 0, 20);
	fdActorFaceDirection(ACTOR_HOLD_SEAN, FACING_SOUTH, 40);
	/* "Make him set sail." */
	scShipRoomsSay(seanSpeaker);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
	fdActorRunRepeatedMotion(ACTOR_HOLD_OURANOS, 2);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_HOLD_OURANOS, 1);
	/* "And what if he refuses?" */
	scShipRoomsSay(ACTOR_HOLD_OURANOS);
	fdActorSetAnimationAndWait(ACTOR_HOLD_SEAN, ANIM_NOD);
	/* "In that case, we'll just have to do our best to persuade him..." */
	scShipRoomsSay(seanSpeaker);
	fdActorShowEmote(ACTOR_HOLD_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorRunRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
	/* "That sounds crazy!" */
	scShipRoomsSay(guideSpeaker);
	fdActorSetAnimationAndWait(ACTOR_HOLD_OURANOS, ANIM_NOD);
	/* "That sounds great! Count me in." */
	scShipRoomsSay(ACTOR_HOLD_OURANOS);
	fdActorFaceDirection(ACTOR_HOLD_SEAN, FACING_NORTH_NORTHWEST, 40);
	fdActorSetAnimation(ACTOR_HOLD_SEAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_HOLD_OURANOS, ANIM_NOD);
	fdActorSetSpeed(ACTOR_HOLD_SEAN, 1.6);
	fdActorSetSpeed(ACTOR_HOLD_OURANOS, 1.6);
	action = ShipRoomsLeaveHold;
	fdActorSetScript(ACTOR_HOLD_SEAN, action);
	fdActorSetScript(ACTOR_HOLD_OURANOS, action);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_HOLD_SECOND_GUIDE, FACING_SOUTH, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2.4);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 184, 520);
	fdScriptWait(1);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorJump(ACTOR_HOLD_SECOND_GUIDE, 4, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_HOLD_SECOND_GUIDE, 2);
	/* "I don't want any violence!" */
	scShipRoomsSay(ACTOR_HOLD_SECOND_GUIDE);
	fdActorSetSpeed(ACTOR_HOLD_SECOND_GUIDE, 1.6);
	fdActorSetScript(ACTOR_HOLD_SECOND_GUIDE, action);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorWaitForScriptEndById(ACTOR_HOLD_SECOND_GUIDE);
	gmFlagSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
	fdScriptEnd();
}

/* Ouranos, Sean and the tour's other guide press the captain in his cabin to
   set sail, and he finds his lucky anchor charm gone; it plays when Isaac tells
   the crewman he wants to see the captain too.

   Kaja, angry, says they can't sail in this dangerous sea, but the captain has
   just decided to sail. Kaja tells of monsters never before seen in the
   Karagol, Ouranos and Sean offer to guard the deck, and Sean remembers some
   young warriors among the passengers. Then the captain finds the anchor gone
   and refuses to leave without it, the crewman mutters "(Maybe we shouldn't
   have taken it.)", and Kaja sends the party out: "This ship goes nowhere!!" */
static void scShipRoomsDiscussLaunchingWithoutAnchor(void)
{
	Sint32 seanSpeech;
	Sint32 ouranosSpeech;
	Sint32 captainSpeech;

	sySndPlay(MUS_DUNGEON);
	fdCameraSetSpeed(2.4);
	scShipRoomsMoveCameraAndWait(
		456, CAMERA_KEEP, 648, SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
	fdScriptSetMessageId(MsgTolbiShipYoureTryingLaunchShip);
	/* "You're trying to launch the ship?" */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 40);
	fdActorShowEmote(
		ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
	/* "We can't set sail in this dangerous sea!" */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	/* "What was that?" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
	/* "We don't care about that!.." */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "But the captain decided to set sail just a second ago." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
	/* "So...we're taking off?" */
	scShipRoomsSay(SPEAKER(
		ACTOR_QUARTERS_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	fdActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "Yes. Preparations are being made right now." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST);
	/* "Well...maybe we were a bit rash, running up here like that." */
	scShipRoomsSay(SPEAKER(
		ACTOR_QUARTERS_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
	scShipRoomsTurn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_SECOND_GUIDE, ANIM_NOD);
	/* "Please, launch the ship when you're ready." */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorFaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHWEST, 0);
	scShipRoomsTurn(ACTOR_QUARTERS_OURANOS, FACING_SOUTH_SOUTHWEST);
	/* Two passengers step aside as the conversation continues. */
	fdActorSetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, 0.4);
	fdActorSetSpeed(ACTOR_QUARTERS_SEAN, 0.8);
	fdActorWalkTo(ACTOR_QUARTERS_SEAN, 444, 668);
	fdActorWalkToAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 472, 668);
	fdActorWaitForMoveById(ACTOR_QUARTERS_SEAN);
	fdActorSetAnimation(ACTOR_QUARTERS_SEAN, ANIM_STAND);
	fdScriptWait(80);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	fdScriptWait(20);
	/* "..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_QUARTERS_OURANOS, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
	/* "Dangerous sea... That's what you said, isn't it?" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "It's dangerous out there..." */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorSetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
	/* "Yeah, I did... The sea is dangerous now. There are monsters..." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
	fdActorSetAttachedEffect(ACTOR_QUARTERS_SECOND_GUIDE,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorJump(ACTOR_QUARTERS_SECOND_GUIDE, 2, 20);
	/* "Monsters, you say?" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_NOD);
	seanSpeech = SPEAKER(ACTOR_QUARTERS_SEAN, SPEAKER_PORTRAIT_LEFT);
	/* "Lately, we've been sighting creatures never before seen in the..." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	scShipRoomsTurn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
	/* "Dangerous ones?" */
	scShipRoomsSay(seanSpeech);
	fdActorShowEmote(
		ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "Actually, some even came onboard the ship and attacked the crew." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorJump(ACTOR_QUARTERS_SECOND_GUIDE, 4, 0);
	/* "That IS dangerous!" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorSetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
	/* "We suspect that our replacement ship might have been attacked." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	/* "With such dangerous creatures around... You might need warriors!" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorShowEmote(ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "It is certainly dangerous, but the ship is okay. After all, we-" */
	fdScriptShowMessage(ACTOR_QUARTERS_CAPTAIN, 0);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
	/* "If there are monsters, I'll be glad to help out." */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_OURANOS, 0, 40);
	fdActorShowEmote(
		ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
	/* "Oh, really?" */
	fdScriptShowMessageThenWait(ACTOR_QUARTERS_KAJA, 0, 20);
	fdActorSetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmote(
		ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SEAN, 2);
	ouranosSpeech = SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_WINDOW_ABOVE);
	/* "Hey, Ouranos, I thought-" */
	scShipRoomsSay(seanSpeech);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_SOUTH_SOUTHWEST, 20);
	/* "We have to make it to Colosso... If we're late, we can't enter." */
	scShipRoomsSay(ouranosSpeech);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SEAN, 2);
	/* "That's true, but..." */
	scShipRoomsSay(seanSpeech);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	/* "Hey, maybe you can give us a hand, too, Sean!" */
	scShipRoomsSay(ouranosSpeech);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
	/* "Really?" */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorShowEmote(
		ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
	/* "Are you really going to help out?" */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorShowEmote(
		ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
	/* "I'm a man of my word. If I said I'll help, I'll help." */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "With you guys helping me to keep the deck safe, the ship should..." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	fdActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "So, can we set off now?" */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHEAST, 40);
	/* "No... Now, we need someone to protect the oarsmen..." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
	fdScriptWait(20);
	/* "Protect the oarsmen, huh? You guys need some warriors..." */
	scShipRoomsSay(seanSpeech);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 40);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	/* "You have an idea?" */
	scShipRoomsSay(ouranosSpeech);
	fdActorSetAnimation(ACTOR_QUARTERS_SEAN, ANIM_NOD);
	/* "I saw some warriors in among the passengers belowdecks." */
	fdScriptShowMessageThenWait(seanSpeech, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
	/* "Do you think they'd help us out?" */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorSetAnimation(ACTOR_QUARTERS_SEAN, ANIM_NOD);
	/* "If they're from Angara, they will." */
	scShipRoomsSay(seanSpeech);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "Hmmm... If they could help out, we'd be unbeatable..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 20);
	sySndPlay(MUS_SILENCE);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
	captainSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_BELOW);
	fdActorShowEmote(
		ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
	/* "It's gone..." */
	scShipRoomsSay(captainSpeech);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(
		ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(
		ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdActorSetAttachedEffect(ACTOR_QUARTERS_CAPTAIN,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorJump(ACTOR_QUARTERS_CAPTAIN, 4, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
	/* "It's vanished!" */
	fdScriptShowMessage(captainSpeech, 0);
	fdActorSetSpeed(ACTOR_QUARTERS_CAPTAIN, 1.6);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 475, 598);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
	fdActorSetSpeed(ACTOR_QUARTERS_KAJA, 1);
	fdActorWalkToAndWait(ACTOR_QUARTERS_KAJA, 462, 618);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHWEST);
	fdActorShowEmote(
		ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
	/* "What!? The captain's beloved anchor!" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_WINDOW_BELOW));
	fdActorShowEmote(
		ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "The anchor... Our good luck charm?" */
	scShipRoomsSay(ACTOR_QUARTERS_OURANOS);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	/* "Oh, that? What's the big deal?.." */
	scShipRoomsSay(seanSpeech);
	fdActorShowEmote(
		ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorJump(ACTOR_QUARTERS_CAPTAIN, 4, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST, 20);
	/* "What do you mean, "Oh, that"!?" */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	sySndPlay(MUS_DUNGEON);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
	/* "That anchor's been keeping my ship safe for years!" */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "It's just decorative, right?" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_SHAKE_HEAD);
	/* "Even if we find some warriors, we won't make it without that..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorWalkToAndWait(ACTOR_QUARTERS_SEAN, 444, 628);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
	/* "But you just said we were leaving..." */
	scShipRoomsSay(SPEAKER(
		ACTOR_QUARTERS_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "That was when I thought we still had the anchor!" */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorShowEmote(ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Why all the fuss over a little trinket?" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
	/* ""Little trinket"!? Weren't you listening to what he said?" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_SHAKE_HEAD);
	/* "It's not just a decoration! It's a good luck charm!" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_EAST);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	/* "This ship doesn't leave until we find it." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdCameraSetSpeed(1.2);
	scShipRoomsMoveCameraAndWait(
		464, CAMERA_KEEP, 680, SHIP_CAMERA_WAIT_FOR_ARRIVAL);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
	scShipRoomsTurn(ACTOR_QUARTERS_CREWMAN, FACING_EAST);
	/* "(What's the problem? We've got some warriors to help us...)" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "(Maybe we shouldn't have taken it.)" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_WEST, 20);
	fdActorShowEmote(
		ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_QUARTERS_CREWMAN, 4, 40);
	/* "Hey! Hey, you! What are you doing here?" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
	/* "You heard the captain- This ship goes nowhere!!" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	fdActorSetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	fdActorWalkTo(ACTOR_QUARTERS_SECOND_GUIDE, 438, 659);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST, 0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_QUARTERS_CREWMAN, 1);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CREWMAN, 488, 686);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 0);
	fdActorWaitForMoveById(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorSetAnimation(ACTOR_QUARTERS_SECOND_GUIDE, ANIM_STAND);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
	gmPlayMusic();
	gmFlagSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL);
}

/* Isaac gives the captain the Anchor Charm from the top of the mast: "Hey! It's
   my lucky anchor!" Then the captain asks the party to guard the oarsmen.

   Sean names the party as the young warriors he had in mind, and Isaac agrees
   to help (a no only gets him talked round). Then passengers must promise to
   row if an oarsman falls, which the tour's guide calls crazy, and the captain
   asks Isaac's name before sending everyone to their stations. */
static void scShipRoomsReturnLuckyAnchor(void)
{
	ACTOR		 *actor;
	Sint32		  captainSpeech;
	Sint32		  seanSpeech;
	Sint32		  captainAboveSpeech;
	const Sint32 *action;

	fdScriptStart();
	fdEndBusyAndReveal();
	fdActorFaceActor(ACTOR_QUARTERS_CAPTAIN, ACTOR_PARTY_LEADER, 0);
	fdActorShowEmote(
		ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	captainSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
	fdScriptSetMessageId(MsgTolbiShipItsMyLuckyAnchor);
	/* "Hey! It's my lucky anchor!" */
	scShipRoomsSay(captainSpeech);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 1);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	/* "Well, look at that! Did you find our charm for us?" */
	fdScriptOpenMessage(captainSpeech, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
		fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
		/* "It was atop the mast? Why there, of all places?" */
		scShipRoomsSay(SPEAKER(
			ACTOR_QUARTERS_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
		fdActorShowEmote(
			ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
		/* "I... I don't know what to say... Thank you..." */
		scShipRoomsSay(captainSpeech);
		FieldStateWork->msg += 2;
	} else {
		FieldStateWork->msg += 2;
		fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
		/* "You climbed up the mast to see the Crow's Nest, and there it..." */
		scShipRoomsSay(SPEAKER(
			ACTOR_QUARTERS_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
		fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
		/* "Thank you so much! Now, I can finally launch this ship." */
		scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_CAPTAIN,
			SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
	}
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(472, CAMERA_KEEP, 636, 1);
	fdActorSetSpeed(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	fdActorWalkToAndWait(ACTOR_QUARTERS_SECOND_GUIDE, 472, 662);
	scShipRoomsTurn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST);
	/* "Boy, that's a relief! Now we can set sail. Right, Captain?" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_NOD);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_SECOND_GUIDE, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_SECOND_GUIDE, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_SOUTH_SOUTHEAST);
	/* "Mmm, not just yet..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_SEAN, 0, 20);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST, 20);
	fdActorShowEmote(
		ACTOR_QUARTERS_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Not yet? What's the problem this time?" */
	scShipRoomsSay(ACTOR_QUARTERS_OURANOS);
	seanSpeech = SPEAKER(
		ACTOR_QUARTERS_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
	fdActorSetAnimation(ACTOR_QUARTERS_SEAN, ANIM_SHAKE_HEAD);
	/* "The Karagol Sea is thick with monsters..." */
	scShipRoomsSay(seanSpeech);
	scShipRoomsTurn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	/* "But you don't..." */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorShowEmote(
		ACTOR_QUARTERS_KAJA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHEAST);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
	/* "Well, it's just like Sean said. The best we can do is protect the
	   deck." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_SEAN, ANIM_NOD);
	/* "We need someone to protect the oarsmen." */
	scShipRoomsSay(seanSpeech);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
	/* "Sean, you look like you have someone in mind to help us..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_SEAN, ANIM_NOD);
	/* "Yeah." */
	scShipRoomsSay(seanSpeech);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 2);
	scShipRoomsTurn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST);
	/* "Someone to help? Who?" */
	scShipRoomsSay(ACTOR_QUARTERS_OURANOS);
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_EAST);
	/* "Those warriors, that's who!" */
	scShipRoomsSay(seanSpeech);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHEAST, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 486, 608);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkToAndWait(ACTOR_GARET, 486, 624);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	actor = fdActorGet(ACTOR_GARET);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorWalkToAndWait(ACTOR_IVAN, 486, 640);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	actor = fdActorGet(ACTOR_IVAN);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkToAndWait(ACTOR_MIA, 486, 656);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	fdActorShowEmote(ACTOR_QUARTERS_SEAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "They may look young, but each of them is a force to be reckoned
	   with." */
	scShipRoomsSay(seanSpeech);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
	/* "When you put it that way, sure. Why not?" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
	/* "I can't argue with that..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST);
	/* "How about it? You want to join forces with us and help out the ship?" */
	fdScriptOpenMessage(ACTOR_QUARTERS_CAPTAIN, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == NO) {
		fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
		/* "You lack confidence, but Sean has a lot of faith in you." */
		scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
		fdActorSetAnimationAndWait(ACTOR_QUARTERS_SEAN, ANIM_NOD);
		/* "With you at our side, we'll be fine. I'm sure you can protect the
		   ship." */
		scShipRoomsSay(seanSpeech);
		fdActorStartRepeatedMotion(ACTOR_QUARTERS_KAJA, 1);
		/* "If you want to reach Tolbi, you'll give us a hand so we can set
		   sail." */
		fdScriptShowMessageThenWait(
			SPEAKER(ACTOR_QUARTERS_KAJA,
				SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT),
			0, 40);
		FieldStateWork->msg += 1;
	} else {
		FieldStateWork->msg += 3;
		fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 3);
		/* "So, you'll help us? That means we can finally set sail." */
		fdScriptShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 40);
	}
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	/* "Oh, good! That means the ship is taking off!" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
	/* "There's just one thing left to do..." */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 1);
	scShipRoomsTurn(ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST);
	/* "Oh, no... Now what?" */
	fdScriptShowMessageThenWait(ACTOR_QUARTERS_SECOND_GUIDE, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	fdActorSetSpeed(ACTOR_QUARTERS_CAPTAIN, 0.8);
	captainAboveSpeech = SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 472, 632);
	/* "I need some volunteers to help with the rowing... just in case." */
	scShipRoomsSay(captainAboveSpeech);
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
	/* "What? Are you going to ask the passengers to help row the ship?" */
	scShipRoomsSay(ACTOR_QUARTERS_SECOND_GUIDE);
	fdActorSetAnimation(ACTOR_QUARTERS_CAPTAIN, ANIM_SHAKE_HEAD);
	/* "The ship won't move without oarsmen, so..." */
	fdScriptShowMessageThenWait(captainAboveSpeech, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_OURANOS, 1);
	scShipRoomsTurn(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST);
	/* "Don't worry--we'll do that, too!" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_OURANOS, SPEAKER_PORTRAIT_LEFT));
	fdActorShowEmote(ACTOR_QUARTERS_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	fdActorSetSpeed(ACTOR_QUARTERS_CREWMAN, 2.4);
	fdActorJump(ACTOR_QUARTERS_CREWMAN, 2, 0);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CREWMAN, 462, 674);
	scShipRoomsTurn(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
	/* "Wa-Wait a minute... You're supposed to protect the ship!" */
	scShipRoomsSay(ACTOR_QUARTERS_CREWMAN);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_KAJA, ANIM_SHAKE_HEAD);
	/* "We'll need to ask the other passengers." */
	scShipRoomsSay(ACTOR_QUARTERS_KAJA);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "These passengers are part of your tour, so why don't you go ask
	   them?" */
	scShipRoomsSay(captainAboveSpeech);
	fdActorShowEmote(
		ACTOR_QUARTERS_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "This is crazy..." */
	fdScriptShowMessageThenWait(ACTOR_QUARTERS_SECOND_GUIDE, 0, 40);
	scShipRoomsTurn(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHEAST);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
	/* "We need you to help out. Even with the crazy stuff." */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_KAJA, SPEAKER_PORTRAIT_LEFT));
	scShipRoomsTurn(ACTOR_QUARTERS_SEAN, FACING_EAST);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_KAJA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_OURANOS, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_QUARTERS_SECOND_GUIDE, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_QUARTERS_CREWMAN, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SEAN, 1);
	/* "Well then, let's all go to our stations while we wait for the
	   launch." */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_SEAN, 0, 20);
	fdActorShowEmote(
		ACTOR_QUARTERS_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST);
	/* "Come to think of it, I don't know your name. You want to tell me?" */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_QUARTERS_CAPTAIN, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
		fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
		/* "Ah, Isaac. Well then, Isaac, I'm looking forward to working with
		   you." */
		scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
		FieldStateWork->msg += 1;
	} else {
		FieldStateWork->msg += 1;
		/* "Well, I'll need to call you something. Hmm... You look like
		   Isaac." */
		scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
	}
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	/* "Well then, Isaac..." */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_PORTRAIT_LEFT));
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_WEST);
	/* "Well, everybody, let's get this ship to sea!" */
	scShipRoomsSay(SPEAKER(ACTOR_QUARTERS_CAPTAIN, SPEAKER_WINDOW_ABOVE));
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 0, 0);
	fdActorSetAnimation(ACTOR_QUARTERS_SEAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_QUARTERS_OURANOS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_QUARTERS_KAJA, ANIM_NOD);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CREWMAN, 2);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_SECOND_GUIDE, 2);
	fdScriptWait(20);
	action = ShipRoomsLeaveCaptainsQuarters;
	fdActorSetScript(ACTOR_QUARTERS_CREWMAN, action);
	fdScriptWait(4);
	fdActorSetScript(ACTOR_QUARTERS_OURANOS, action);
	fdScriptWait(4);
	fdActorSetScript(ACTOR_QUARTERS_SEAN, action);
	fdScriptWait(4);
	fdActorSetScript(ACTOR_QUARTERS_KAJA, action);
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_IVAN);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_GARET);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	scWalkIntoLeader(ACTOR_GARET);
	fdActorSetScript(ACTOR_QUARTERS_SECOND_GUIDE, action);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 456, 648);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_EAST);
	gmPartyInventoryDiscard(ITEM_ANCHOR_CHARM);
	gmFlagSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED);
	fdScriptEnd();
}

/* Restores the room or plays the script selected by this entrance. */
Sint32 scInit(void)
{
	sySynch(1);
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	switch(MainRoutineStateWork.entrance) {
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
			gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			fdActorSetSprColById(ACTOR_SEA_HOLD_SECOND_GUIDE, SPRITE_COLOR_RBG);
		} else if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
			fdActorSetSprColById(ACTOR_HOLD_SECOND_GUIDE, SPRITE_COLOR_RBG);
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
		fdActorSetSprColById(ACTOR_ROWING_SECOND_GUIDE, SPRITE_COLOR_RBG);
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
			gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
			fdActorSetSprColById(ACTOR_QUARTERS_SECOND_GUIDE, SPRITE_COLOR_RBG);
		}
		break;
	}
	switch(MainRoutineStateWork.entrance) {
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_TOLBI_SHIP_DECK:
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK:
		scShipRoomsSetUpHold();
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD:
		scShipRoomsSetUpRowingDeck();
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK:
		scShipRoomsSetUpCaptainsQuarters();
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF:
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			scShipRoomsAnnounceDeparture();
		} else {
			scShipRoomsCastOff();
		}
		break;
	case SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS:
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			scShipRoomsKajaCallsReplacementToOars();
		} else {
			scShipRoomsKajaCallsToOars();
		}
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_PASSENGER_HOLD_ROWING:
		scShipRoomsShowRowing();
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS:
		scShipRoomsAskAboutMonsters();
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_TURN:
		scShipRoomsAnnounceTurn();
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_FIRST_ATTACK:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			fdScriptStart();
			scShipRoomsRunScriptCommand(
				SHIP_PLACE_COMPANIONS_AND_MONSTERS, 1, 0);
			scShipRoomsRunScriptCommand(SHIP_RESTORE_TREMBLING_OARSMAN, 0, 0);
			fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER, ShipRoomsMonsterFloat);
			fdActorSetScript(
				ACTOR_ROWING_FIRST_MONSTER + 1, ShipRoomsMonsterFloat);
			fdActorSetScript(
				ACTOR_ROWING_FIRST_MONSTER + 2, ShipRoomsMonsterFloat);
			fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
			fdActorSetSprColById(
				ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
			fdActorSetSprColById(
				ACTOR_ROWING_FIRST_MONSTER + 2, SPRITE_COLOR_BGR);
			fdScriptEnd();
		} else {
			scShipRoomsStartFirstAttack();
		}
		break;
	case SHIP_ROOMS_ROWING_DECK_AFTER_FIRST_ATTACK_BATTLE:
		scShipRoomsEndFirstAttack();
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SECOND_ATTACK:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			fdScriptStart();
			scShipRoomsRunScriptCommand(
				SHIP_PLACE_COMPANIONS_AND_MONSTERS, 2, 0);
			scShipRoomsRunScriptCommand(SHIP_RESTORE_TREMBLING_OARSMAN, 0, 0);
			fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER, ShipRoomsMonsterFloat);
			fdActorSetScript(
				ACTOR_ROWING_FIRST_MONSTER + 1, ShipRoomsMonsterFloat);
			fdScriptEnd();
		} else {
			scShipRoomsStartSecondAttack();
		}
		break;
	case SHIP_ROOMS_ROWING_DECK_AFTER_SECOND_ATTACK_BATTLE:
		scShipRoomsEndSecondAttack();
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_THIRD_ATTACK:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			fdScriptStart();
			scShipRoomsRunScriptCommand(
				SHIP_PLACE_COMPANIONS_AND_MONSTERS, 3, 0);
			scShipRoomsRunScriptCommand(SHIP_RESTORE_TREMBLING_OARSMAN, 0, 0);
			fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER, ShipRoomsMonsterFloat);
			fdActorSetScript(
				ACTOR_ROWING_FIRST_MONSTER + 1, ShipRoomsMonsterFloat);
			fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
			fdActorSetSprColById(
				ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
			fdScriptEnd();
		} else {
			scShipRoomsStartThirdAttack();
		}
		break;
	case SHIP_ROOMS_ROWING_DECK_AFTER_THIRD_ATTACK_BATTLE:
		scShipRoomsEndThirdAttack();
		break;
	case SHIP_ROOMS_FROM_WORLD_MAP:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK) != 0) {
				ShipRoomsShakeTimer = 0;
				sySetTickEvent(
					scShipRoomsShakeWhileKrakenOnDeck, EVENT_PASS_TICK);
				fdActorSetAnimation(
					ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
			}
		} else {
			scShipRoomsNoticeShipRocking();
		}
		break;
	case SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_DECK_ISLAND:
		scShipRoomsShowCaptainsQuartersBeforeIsland();
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scShipRoomsReachIsland();
		}
		break;
	case SHIP_ROOMS_ROWING_DECK_FROM_TOLBI_SHIP_DECK_SWITCH_TEAMS:
		scShipRoomsSwitchLeftAndRightTeams();
		break;
	case SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT:
		scShipRoomsRunScriptCommand(SHIP_CLEAR_FLAG_RANGE,
			FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT,
			FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
		scShipRoomsRunScriptCommand(SHIP_CLEAR_ROWING_SELECTIONS, 0, 0);
		gmFlagSet(FLAG_TOLBI_SHIP_DEFEATED_AT_SEA);
		syImportScriptSetExitRequest(
			SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK);
		break;
	}
	return 0;
}

/* Restores the captain's quarters' actors after the anchor and departure
   conversations. */
static void scShipRoomsSetUpCaptainsQuarters(void)
{
	ACTOR *actor;
	ACTOR *other;
	Sint32 done;

	if(!gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
		scShipRoomsPlaceAnchorCharm();
		return;
	}
	if(fdScriptGetFlag(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
		scShipRoomsPlaceAnchorCharm();
	}
	done = gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI);
	if(done != 0) {
		return;
	}
	if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND)) {
		actor = fdActorGet(ACTOR_QUARTERS_KAJA);
		scShipRoomsRunScriptCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
		scShipRoomsPlaceActor(ACTOR_QUARTERS_CAPTAIN, 456, 652, FACING_EAST);
		scShipRoomsPlaceActor(
			ACTOR_QUARTERS_KAJA, 480, 600, FACING_NORTH_NORTHWEST);
		fdActorSetSpeed(ACTOR_QUARTERS_KAJA, 0.8);
		actor->userValue = KAJA_PACE_FACE_NORTH_NORTHWEST;
		actor->sceneByte = KAJA_BEAT_NONE;
		actor->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
		actor->update = scShipRoomsPaceCaptainsQuarters;
		other = fdActorGet(ACTOR_QUARTERS_CAPTAIN);
		other->sceneCnt = 0;
		other->update = scShipRoomsWatchKaja;
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			scShipRoomsPlaceActor(
				ACTOR_PARTY_LEADER, 480, 666, FACING_NORTHWEST);
		}
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF)) {
		scShipRoomsPlaceActor(
			ACTOR_QUARTERS_CAPTAIN, 444, 614, FACING_NORTH_NORTHEAST);
		scShipRoomsRunScriptCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
		scShipRoomsPlaceActor(ACTOR_QUARTERS_CAPTAIN, 456, 648, FACING_EAST);
		scShipRoomsRunScriptCommand(SHIP_HIDE_QUARTERS_CREW, 0, 0);
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL)) {
		scShipRoomsPlaceActor(ACTOR_QUARTERS_CAPTAIN, 475, 598, FACING_WEST);
		scShipRoomsPlaceActor(
			ACTOR_QUARTERS_KAJA, 462, 618, FACING_NORTH_NORTHWEST);
		fdActorGet(ACTOR_QUARTERS_SEAN)->facing = FACING_SOUTH_SOUTHEAST;
		fdActorGet(ACTOR_QUARTERS_OURANOS)->facing = FACING_NORTH_NORTHWEST;
		scShipRoomsPlaceActor(
			ACTOR_QUARTERS_SECOND_GUIDE, 438, 659, FACING_NORTH_NORTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_QUARTERS_CREWMAN, 488, 688, FACING_NORTH_NORTHWEST);
	}
}

/* Restores the passenger hold after recruitment and departure scripts. */
static void scShipRoomsSetUpHold(void)
{
	if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
		fdActorSetPos(ACTOR_HOLD_WARRIOR, 0, 0);
		fdActorSetPos(ACTOR_HOLD_TICKET_GRUMBLER, 0, 0);
		fdActorSetPos(ACTOR_HOLD_MUSCLE_ADMIRER, 0, 0);
		fdActorSetPos(ACTOR_HOLD_OLD_MAN, 0, 0);
		fdActorSetPos(ACTOR_HOLD_SECOND_GUIDE, 0, 0);
		scShipRoomsRunScriptCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
		return;
	}

	if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
		scShipRoomsPlaceActor(
			ACTOR_SEA_HOLD_HEAD_GUIDE, 152, 444, FACING_SOUTH_SOUTHEAST);
		fdActorSetScript(ACTOR_SEA_HOLD_HEAD_GUIDE, ShipRoomsLookAroundScript);
		scShipRoomsPlaceActor(ACTOR_SEA_HOLD_SWEATING_PASSENGER, 184, 480,
			FACING_NORTH_NORTHWEST);
		scShipRoomsPlaceActor(ACTOR_SEA_HOLD_STARED_AT_PASSENGER, 170, 488,
			FACING_NORTH_NORTHWEST);
		scShipRoomsPlaceActor(
			ACTOR_SEA_HOLD_WARRIOR, 136, 488, FACING_NORTH_NORTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_SEA_HOLD_MUSCLE_ADMIRER, 120, 480, FACING_NORTH_NORTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_SEA_HOLD_TICKET_GRUMBLER, 184, 526, FACING_NORTH_NORTHWEST);
		scShipRoomsPlaceActor(ACTOR_SEA_HOLD_LADY, 136, 584, FACING_WEST);
		fdActorSetScript(ACTOR_SEA_HOLD_LADY, ShipRoomsTremble);
		return;
	}

	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
		scShipRoomsHideSelectedPassengers();
		return;
	}

	if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		scShipRoomsRunScriptCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
		return;
	}

	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0 &&
		gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
		scShipRoomsRunScriptCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
		fdActorSetPos(ACTOR_HOLD_SECOND_GUIDE, 0, 0);
	}
}

/* Restores the rowing deck and introduces a newly recruited replacement. */
static void scShipRoomsSetUpRowingDeck(void)
{
	sySynch(1);
	scShipRoomsSetOarCollision();
	if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
		scShipRoomsRunScriptCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_CREWMAN, 412, 222, FACING_SOUTH_SOUTHEAST);
		scShipRoomsPlaceActor(ACTOR_ROWING_DRUMMER, 458, 161, FACING_WEST);
	} else {
		if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
			fdActorSetPos(ACTOR_ROWING_CREWMAN, 472, 152);
			fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
			scShipRoomsRunScriptCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
		} else {
			if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
				scShipRoomsRunScriptCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
				scShipRoomsRunScriptCommand(SHIP_HIDE_FIRST_OARSMEN, 4, 0);
				scShipRoomsIntroduceReplacementOarsman(3);
			} else {
				if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
					scShipRoomsRunScriptCommand(
						SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
					scShipRoomsRunScriptCommand(SHIP_HIDE_FIRST_OARSMEN, 3, 0);
					scShipRoomsIntroduceReplacementOarsman(2);
				} else {
					if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) !=
						0) {
						scShipRoomsRunScriptCommand(
							SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
						scShipRoomsRunScriptCommand(
							SHIP_HIDE_FIRST_OARSMEN, 2, 0);
						scShipRoomsIntroduceReplacementOarsman(1);
					} else {
						if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) !=
							0) {
							scShipRoomsRunScriptCommand(
								SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
							fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
							scShipRoomsIntroduceReplacementOarsman(0);
						} else {
							fdActorSetAnimation(
								ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
							if(gmFlagGet(
								   FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) !=
									0 &&
								gmFlagGet(
									FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT) ==
									0) {
								scShipRoomsAskForReplacementOarsmen();
							}
						}
					}
				}
			}
		}
	}
}

/* Makes the eight oars beside the seats never a talk or Psynergy target. */
static void scShipRoomsSetOarCollision(void)
{
	Uint32 actorId;

	for(actorId = ACTOR_ROWING_FIRST_OAR;
		actorId < ACTOR_ROWING_FIRST_OAR + ROWING_DECK_SEATS; actorId++) {
		ACTOR *actor = fdActorGet(actorId);
		Uint32 flg = actor->collisionFlg;
		actor->collisionFlg = flg | ACTOR_NOT_TARGETED;
	}
}

/* Puts each chosen passenger at the seat they took, in place of its oarsman; if
   gated, only those whose seat's attack has since been fought off. */
static void scShipRoomsSeatChosenRowers(Sint32 gated)
{
	Sint32 actorId;

	if(gated && !gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF)) {
		return;
	}
	if((actorId = scShipRoomsFindSeatRower(0, 0)) != 0) {
		scShipRoomsPlaceActor(actorId, 410, 172, FACING_NORTH_NORTHEAST);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
	}
	if(gated && !gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF)) {
		return;
	}
	if((actorId = scShipRoomsFindSeatRower(1, 0)) != 0) {
		scShipRoomsPlaceActor(actorId, 470, 172, FACING_NORTH_NORTHWEST);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
	}
	if(gated && !gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED)) {
		return;
	}
	if((actorId = scShipRoomsFindSeatRower(2, 0)) != 0) {
		scShipRoomsPlaceActor(actorId, 410, 204, FACING_NORTH_NORTHEAST);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
	}
	if(gated) {
		return;
	}
	if((actorId = scShipRoomsFindSeatRower(3, 0)) != 0) {
		scShipRoomsPlaceActor(actorId, 470, 204, FACING_NORTH_NORTHWEST);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 3, 0, 0);
	}
}

/* On the rowing deck, once the charm is back, Kaja tells the oarsmen "Our
   replacement never arrived, but we're shoving off just the same."

   He, Ouranos and Sean will guard the deck and Isaac's party the oarsmen. If an
   oarsman falls, Isaac is to choose a passenger and bring him down, keeping the
   two teams of rowers balanced: "Right. You have your orders." */
static void scShipRoomsAskForReplacementOarsmen(void)
{
	ACTOR *actor;

	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsMoveCameraAndWait(
		440, CAMERA_KEEP, 168, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	scShipRoomsPlaceActor(ACTOR_ROWING_KAJA, 440, 164, FACING_SOUTH_SOUTHWEST);
	scShipRoomsPlaceActor(
		ACTOR_ROWING_CREWMAN, 428, 190, FACING_NORTH_NORTHEAST);
	scShipRoomsPlaceActor(
		ACTOR_ROWING_DRUMMER, 452, 190, FACING_NORTH_NORTHWEST);
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_STAND);
	scShipRoomsPlaceActor(ACTOR_PARTY_LEADER, 440, 134, FACING_WEST);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 408, 148);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 424, 148);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	fdScriptSetMessageId(MsgTolbiShipOurReplacementNeverArrivedBut);
	/* "Our replacement never arrived, but we're shoving off just the..." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorRunRepeatedMotion(ACTOR_ROWING_CREWMAN, 1);
	/* "What about the sea monsters?.." */
	scShipRoomsSay(ACTOR_ROWING_CREWMAN);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	/* "That'll be me, Ouranos, and Sean on deck, and..." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_NORTH_NORTHEAST);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorWalkToAndWait(ACTOR_GARET, 440, 148);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	actor = fdActorGet(ACTOR_GARET);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorWalkToAndWait(ACTOR_IVAN, 456, 148);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	actor = fdActorGet(ACTOR_IVAN);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorWalkToAndWait(ACTOR_MIA, 472, 148);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 60);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
	fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST, 20);
	/* "Isaac and his friends protecting you down here." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorStartRepeatedMotion(ACTOR_ROWING_DRUMMER, 1);
	fdActorShowEmote(
		ACTOR_ROWING_DRUMMER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Can we really depend on them?" */
	scShipRoomsSay(ACTOR_ROWING_DRUMMER);
	fdActorStartRepeatedMotion(ACTOR_GARET, 3);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	/* "Isaac and his friends have seen more combat than even I have..." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorRunRepeatedMotion(ACTOR_ROWING_FIRST_OARSMAN, 1);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_NOD);
	/* "If you say so, Kaja. We trust your judgment." */
	scShipRoomsSay(ACTOR_ROWING_FIRST_OARSMAN);
	fdActorSetAnimation(ACTOR_ROWING_CREWMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 1, ANIM_NOD);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 2, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ROWING_FIRST_OARSMAN + 3, ANIM_NOD);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
	/* "And what will you do if any of you becomes incapacitated?" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorStartRepeatedMotion(ACTOR_ROWING_CREWMAN, 1);
	/* "We're not sure... What happens then??" */
	scShipRoomsSay(ACTOR_ROWING_CREWMAN);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	/* "Another passenger will help row the ship." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorFaceDirection(ACTOR_ROWING_CREWMAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ROWING_DRUMMER, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_ROWING_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	fdActorSetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
	/* "Don't worry about it! I had a word with the passengers." */
	fdScriptShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 20);
	fdActorSetAnimation(ACTOR_ROWING_CREWMAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ROWING_DRUMMER, ANIM_NOD);
	fdScriptWait(40);
	fdActorShowEmote(
		ACTOR_ROWING_DRUMMER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scShipRoomsTurn(ACTOR_ROWING_DRUMMER, FACING_NORTH_NORTHWEST);
	/* "Who will go and give the order when the time comes?" */
	scShipRoomsSay(ACTOR_ROWING_DRUMMER);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Hmmm. Good point..." */
	fdScriptShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 60);
	fdActorShowEmote(ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_NORTH_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	/* "You should select a replacement and bring him here." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	scShipRoomsRunScriptCommand(SHIP_SHOW_PARTY_EMOTE, 2, 80);
	scShipRoomsTurn(ACTOR_ROWING_CREWMAN, FACING_NORTH_NORTHEAST);
	fdActorStartRepeatedMotion(ACTOR_ROWING_CREWMAN, 2);
	/* "Rowing requires teamwork. Be sure to balance the two teams." */
	scShipRoomsSay(ACTOR_ROWING_CREWMAN);
	fdActorSetAnimationAndWait(ACTOR_ROWING_DRUMMER, ANIM_NOD);
	fdActorStartRepeatedMotion(ACTOR_ROWING_DRUMMER, 2);
	/* "That's right. If we're not balanced, we won't make much..." */
	scShipRoomsSay(ACTOR_ROWING_DRUMMER);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	/* "Right. You have your orders." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorSetSpeed(ACTOR_ROWING_KAJA, 0.8);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 158);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 148);
	fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 20);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	/* "I trust you'll all do your best." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_WEST, 20);
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
	fdActorWalkTo(ACTOR_ROWING_KAJA, 440, 134);
	fdScriptWait(40);
	scShipRoomsRunScriptCommand(
		SHIP_CLOSE_SCREEN, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
	gmFlagSet(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT);
}

/* In his cabin the captain walks up to the Anchor Charm, which swells and
   shrinks as he raises it, and calls "All right. Cast off!" */
static void scShipRoomsCastOff(void)
{
	ACTOR *anchorCharm;

	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 0, 1);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_QUARTERS_CAPTAIN, 0.8);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 468, 614);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 472, 596);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_WEST, 20);
	fdActorJump(ACTOR_QUARTERS_CAPTAIN, 4, 20);
	anchorCharm = scShipRoomsPlaceAnchorCharm();
	fdScriptWait(20);
	sySndPlay(SE_RISING_CHIMES);
	fdActorStartScript(anchorCharm, ShipRoomsAnchorCharmPulse);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_QUARTERS_CAPTAIN, 466, 624);
	scShipRoomsTurn(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
	fdScriptSetMessageId(MsgTolbiShipCastOff);
	/* "All right. Cast off!" */
	fdScriptShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN,
		SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/* After a new rower has taken his seat, the captain calls from his cabin "All
   right, we're off!" */
static void scShipRoomsAnnounceDeparture(void)
{
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHWEST, 40);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
	fdScriptSetMessageId(MsgTolbiShipWereOff);
	/* "All right, we're off!" */
	fdScriptShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN,
		SHIP_ROOMS_CAPTAINS_ROOM_TO_PASSENGER_HOLD_CALL_TO_OARS, 0);
}

/* Kaja comes into the passengers' room as the ship casts off and calls "Row
   those oars!" */
static void scShipRoomsKajaCallsToOars(void)
{
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 0, 0);
	scShipRoomsRunScriptCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_HOLD_KAJA, 150, 586);
	scShipRoomsMoveCameraAndWait(
		156, CAMERA_KEEP, 536, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	scShipRoomsRunScriptCommand(SHIP_OPEN_SCREEN, 0, 0);
	fdActorSetSpeed(ACTOR_HOLD_KAJA, 0.8);
	fdActorWalkToAndWait(ACTOR_HOLD_KAJA, 168, 578);
	fdActorWalkToAndWait(ACTOR_HOLD_KAJA, 168, 554);
	fdActorFaceDirection(ACTOR_HOLD_KAJA, FACING_WEST, 20);
	fdActorStartRepeatedMotion(ACTOR_HOLD_KAJA, 2);
	fdScriptSetMessageId(MsgTolbiShipRowThoseOars);
	/* "Row those oars!" */
	fdScriptShowMessageThenWait(ACTOR_HOLD_KAJA, 0, 20);
	scShipRoomsRunScriptCommand(
		SHIP_CLOSE_SCREEN, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/* Kaja comes into the passengers' room when the ship sets off again with its
   new rower and calls "Row those oars!" */
static void scShipRoomsKajaCallsReplacementToOars(void)
{
	fdScriptStart();
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	scShipRoomsHideSelectedPassengers();
	fdActorSetPos(ACTOR_SEA_HOLD_KAJA, 150, 586);
	scShipRoomsMoveCameraAndWait(
		156, CAMERA_KEEP, 536, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	scShipRoomsRunScriptCommand(SHIP_OPEN_SCREEN, 0, 0);
	fdActorSetSpeed(ACTOR_SEA_HOLD_KAJA, 0.8);
	fdActorWalkToAndWait(ACTOR_SEA_HOLD_KAJA, 168, 578);
	fdActorWalkToAndWait(ACTOR_SEA_HOLD_KAJA, 168, 554);
	fdActorFaceDirection(ACTOR_SEA_HOLD_KAJA, FACING_WEST, 20);
	fdActorStartRepeatedMotion(ACTOR_SEA_HOLD_KAJA, 2);
	fdScriptSetMessageId(MsgTolbiShipRowThoseOars);
	/* "Row those oars!" */
	fdScriptShowMessageThenWait(ACTOR_SEA_HOLD_KAJA, 0, 20);
	scShipRoomsRunScriptCommand(
		SHIP_CLOSE_SCREEN, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK_ROWING, 0);
}

/* The oarsmen and the passengers chosen row to the drum, and the ship sails on
   across the World Map to its next attack, the Kraken or land. */
static void scShipRoomsShowRowing(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	scShipRoomsRunScriptCommand(SHIP_HIDE_OARS, 0, 0);
	scShipRoomsInstallRowingDeckActors(0);
	scShipRoomsRunScriptCommand(SHIP_OPEN_SCREEN, 1, 20);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(440, CAMERA_KEEP, 176, 1);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_BEAT);
	fdScriptWait(30);
	sySndPlay(SE_THUMP);
	fdScriptWait(30);
	scShipRoomsInstallRowingDeckActors(ANIM_STEP_NORMAL);
	fdScriptWait(80);
	scShipRoomsInstallRowingDeckActors(0);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_BEAT);
	fdScriptWait(30);
	sySndPlay(SE_THUMP);
	fdScriptWait(30);
	scShipRoomsInstallRowingDeckActors(ANIM_STEP_NORMAL);
	fdScriptWait(80);
	scShipRoomsInstallRowingDeckActors(0);
	fdScriptWait(90);
	sySndPlay(SE_THUMP);
	fdScriptWait(30);

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);

	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN, 0, 0);

	if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
		syImportScriptSetExitRequest(
			SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_LAND);
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
		syImportScriptSetExitRequest(
			SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_KRAKEN);
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
		syImportScriptSetExitRequest(
			SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_THIRD_ATTACK);
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
		syImportScriptSetExitRequest(
			SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_SECOND_ATTACK);
	} else {
		syImportScriptSetExitRequest(
			SHIP_ROOMS_ROWING_DECK_TO_WORLD_MAP_SAIL_TO_FIRST_ATTACK);
	}
}

/* The lookout calls down into the captain's cabin "Avast! Monsters sighted!"
   "Monsters?" "Aye, Captain! Sea monsters!", and the party goes up on deck. */
static void scShipRoomsAskAboutMonsters(void)
{
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorStartRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 2);
	fdScriptSetMessageId(MsgTolbiShipMonsters);
	/* "Monsters?" */
	fdScriptShowMessageThenWait(ACTOR_QUARTERS_CAPTAIN, 0, 20);
	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN,
		SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_SEA_MONSTERS, 0);
}

/* Told to turn the rudder hard to starboard, the captain answers from his cabin
   "All right, I'm turning!", and the scene goes back up on deck as the monsters
   board. */
static void scShipRoomsAnnounceTurn(void)
{
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	scShipRoomsRunScriptCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 1);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_CAPTAIN, 1);
	fdScriptSetMessageId(MsgTolbiShipImTurning);
	/* "All right, I'm turning!" */
	scShipRoomsSay(ACTOR_QUARTERS_CAPTAIN);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST, 40);
	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN,
		SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK_MONSTERS_BOARD, 0);
}

/* Monsters are on the rowing deck in the first attack: three floating monsters
   beside the seats and an oarsman down, before
   scShipRoomsReportMonstersBelowdecks. */
static void scShipRoomsStartFirstAttack(void)
{
	const Sint32 *entryScript;

	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 1, 0);
	scShipRoomsSetOarCollision();
	scShipRoomsRunScriptCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
		ACTOR_ROWING_FIRST_OARSMAN + 1, ACTOR_ROWING_FIRST_OARSMAN + 2);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
	fdActorSetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, ShipRoomsTremble);
	entryScript = ShipRoomsMonsterFloat;
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER + 2, entryScript);
	fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER + 2, SPRITE_COLOR_BGR);
	scShipRoomsReportMonstersBelowdecks();
	fdScriptEnd();
}

/* Back on the rowing deck after the first battle: the monsters are gone, and
   Kaja finds an oarsman hurt (scShipRoomsFindOarsmanMissing). */
static void scShipRoomsEndFirstAttack(void)
{
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
	scShipRoomsRunScriptCommand(SHIP_SETUP_MONSTER_ENCOUNTER,
		ACTOR_ROWING_FIRST_OARSMAN + 1, ACTOR_ROWING_FIRST_OARSMAN + 2);
	scShipRoomsFindOarsmanMissing();
	gmFlagSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF);
	fdScriptEnd();
}

/* Back on the rowing deck after choosing a rower, Kaja greets Isaac, "Isaac,
   you're back! And you brought an oarsman with you!", and the passenger takes
   the empty seat.

   Coming down without one gets "Hey! Where's the replacement for our injured
   oarsman?... I'm sorry, but you're going to have to choose someone!", and
   Isaac is sent back up to the passengers' room. */
static void scShipRoomsIntroduceReplacementOarsman(Sint32 seat)
{
	Sint32 rowerId;
	ACTOR *actor;
	FIXED  halfStep;
	Sint32 leftSeat;

	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 0, 0);
	halfStep = toFIXED(1) / 2;
	scShipRoomsPlaceActor(ACTOR_PARTY_LEADER, 432, 134, FACING_WEST);
	scShipRoomsSeatChosenRowers(1);
	sySynch(1);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 406, 134);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 406, 152);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 421, 152);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 10);
	if(gmFlagGet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
		rowerId = scShipRoomsFindSeatRower(seat, 0);
		fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
		fdScriptWait(20);
		fdActorFaceEachOther(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 10);
		fdScriptSetMessageId(MsgTolbiShipBackBroughtOarsman);
		/* "Isaac, you're back! And you brought an oarsman with you!" */
		scShipRoomsSay(KAJA_ROWING_SPEAKER);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		leftSeat = 0;
		fdActorSetSpeedFX(ACTOR_PARTY_LEADER, toFIXED(1), halfStep);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 432, 168);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetPosFX(rowerId, actor->x.fixed, actor->z.fixed);
		}
		fdActorSetSpeedFX(rowerId, toFIXED(1), halfStep);
		fdActorWalkToAndWait(rowerId, 448, 168);
		fdActorFaceDirection(rowerId, FACING_NORTH_NORTHWEST, 20);
		fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 20);
		fdActorSetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
		/* "Well, you're going to have to row for us." */
		scShipRoomsSay(ACTOR_ROWING_KAJA);
		fdActorShowEmote(rowerId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
		fdActorSetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
		/* "All right. Take your place." */
		scShipRoomsSay(ACTOR_ROWING_KAJA);
		fdActorSetAnimationAndWait(rowerId, ANIM_NOD);
		if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
			fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 0);
			fdActorWalkToAndWait(rowerId, 470, 204);
			scShipRoomsTurn(rowerId, FACING_NORTH_NORTHWEST);
		} else if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 422, 154);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
			fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST, 0);
			fdActorWalkToAndWait(rowerId, 410, 204);
			leftSeat = 1;
			scShipRoomsTurn(rowerId, FACING_NORTH_NORTHEAST);
		} else if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
			fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 0);
			fdActorWalkToAndWait(rowerId, 470, 172);
			scShipRoomsTurn(rowerId, FACING_NORTH_NORTHWEST);
		} else {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 422, 154);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
			fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST, 0);
			fdActorWalkToAndWait(rowerId, 410, 172);
			leftSeat = 1;
			scShipRoomsTurn(rowerId, FACING_NORTH_NORTHEAST);
		}
		fdActorFaceEachOther(ACTOR_ROWING_KAJA, ACTOR_PARTY_LEADER, 20);
		/* "Now, we'll need you to protect this area again, Isaac." */
		scShipRoomsSay(SPEAKER(ACTOR_ROWING_KAJA, SPEAKER_PORTRAIT_RIGHT));
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
		fdActorSetSpeed(ACTOR_ROWING_KAJA, 1);
		if(leftSeat != 0) {
			fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 428, 164);
			fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 164);
		}
		fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
		fdActorWalkTo(ACTOR_ROWING_KAJA, 440, 134);
		fdScriptWait(40);
		scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN,
			SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY, 0);
	} else {
		fdScriptSetMessageId(MsgTolbiShipBack);
		/* "Isaac, you're back." */
		syFieldScriptShowMessageThenWait(KAJA_ROWING_SPEAKER, 0, 40);
		fdActorShowEmote(
			ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		/* "Hey! Where's the replacement for our injured oarsman?" */
		scShipRoomsSay(KAJA_ROWING_SPEAKER);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(60);
		fdActorShowEmote(
			ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorStartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
		/* "What? You didn't choose an oarsman?" */
		scShipRoomsSay(KAJA_ROWING_SPEAKER);
		fdActorShowEmote(
			ACTOR_ROWING_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		/* "This ship won't go anywhere without enough oarsmen!" */
		scShipRoomsSay(KAJA_ROWING_SPEAKER);
		fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
		/* "I'm sorry, but you're going to have to choose someone!" */
		scShipRoomsSay(KAJA_ROWING_SPEAKER);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		syImportScriptSetExitRequest(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
	}
}

/* Creates the visible anchor charm once a visit
   (FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED records it) and retains it for departure
   scripts. */
static ACTOR *scShipRoomsPlaceAnchorCharm(void)
{
	Sint32		   set = fdScriptGetFlag(FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED);
	ACTOR		  *actor;
	SPR			  *spr;
	ICON_TRANSFER *glyph;

	if(set != 0) {
		return ShipRoomsAnchorCharm;
	}
	actor = syFieldActorCreate(
		SPRITE_BLANK, toFIXED(455), toFIXED(4), toFIXED(544));
	actor->motionFlg = set;
	actor->retainSprCache = 1;
	spr = actor->spr;
	spr->cnt = set;
	spr->image.fullCol = 0;
	spr->image.pal = 0;
	glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItem(ITEM_ANCHOR_CHARM);
	syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	gmFlagSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_PLACED);
	ShipRoomsAnchorCharm = actor;
	return actor;
}

/* Seats the rowers: at each of the first four seats the passenger chosen for
   it, the oarsman put away, or else the oarsman, and the oarsmen at the other
   four, all rowing at anmStep (0 holds them still; a passenger rows at
   their own pace, scShipRoomsPoseActor). */
static void scShipRoomsInstallRowingDeckActors(Uint32 anmStep)
{
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
		Sint32 rowerId = scShipRoomsFindSeatRower(0, 0);
		fdActorSetPos(rowerId, 410, 172);
		scShipRoomsRunScriptCommand(
			SHIP_SET_SELECTED_ROWER_POSE, rowerId, anmStep);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
	} else {
		scShipRoomsRunScriptCommand(
			SHIP_START_ROWING_ANIMATION, ACTOR_ROWING_FIRST_OARSMAN, anmStep);
	}

	if(gmFlagGet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF) != 0) {
		Sint32 rowerId = scShipRoomsFindSeatRower(1, 0);
		fdActorSetPos(rowerId, 470, 172);
		fdActorGet(rowerId)->scaleX = -toFIXED(1);
		scShipRoomsRunScriptCommand(
			SHIP_SET_SELECTED_ROWER_POSE, rowerId, anmStep);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
	} else {
		scShipRoomsRunScriptCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
			ACTOR_ROWING_FIRST_OARSMAN + 1, anmStep);
	}

	if(gmFlagGet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF) != 0) {
		Sint32 rowerId = scShipRoomsFindSeatRower(2, 0);
		fdActorSetPos(rowerId, 410, 204);
		scShipRoomsRunScriptCommand(
			SHIP_SET_SELECTED_ROWER_POSE, rowerId, anmStep);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
	} else {
		scShipRoomsRunScriptCommand(SHIP_START_ROWING_ANIMATION,
			ACTOR_ROWING_FIRST_OARSMAN + 2, anmStep);
	}

	if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
		Sint32 rowerId = scShipRoomsFindSeatRower(3, 0);
		fdActorSetPos(rowerId, 470, 204);
		fdActorGet(rowerId)->scaleX = -toFIXED(1);
		scShipRoomsRunScriptCommand(
			SHIP_SET_SELECTED_ROWER_POSE, rowerId, anmStep);
		fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 3, 0, 0);
	} else {
		scShipRoomsRunScriptCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
			ACTOR_ROWING_FIRST_OARSMAN + 3, anmStep);
	}

	scShipRoomsRunScriptCommand(
		SHIP_START_ROWING_ANIMATION, ACTOR_ROWING_FIRST_OARSMAN + 4, anmStep);
	scShipRoomsRunScriptCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
		ACTOR_ROWING_FIRST_OARSMAN + 5, anmStep);
	scShipRoomsRunScriptCommand(
		SHIP_START_ROWING_ANIMATION, ACTOR_ROWING_FIRST_OARSMAN + 6, anmStep);
	scShipRoomsRunScriptCommand(SHIP_START_MIRRORED_ROWING_ANIMATION,
		ACTOR_ROWING_FIRST_OARSMAN + 7, anmStep);
}

/* Sets a passenger at the oars rowing at a pace of their own beside the
   oarsmen's ANIM_STEP_NORMAL: the head guide and the sweating passenger as
   fast, the stared-at passenger and the warrior a quarter faster, the second
   guide and the ticket grumbler at half, and the lady, the muscle admirer and
   the old man at a quarter. */
static void scShipRoomsPoseActor(Sint32 id)
{
	switch(id) {
	case ACTOR_ROWING_SECOND_GUIDE:
		fdActorSetAnimation(id, ANIM_SECOND_GUIDE_ROW);
		fdActorSetAnimationSpeedById(id, ANIM_STEP_NORMAL / 2);
		break;
	case ACTOR_ROWING_HEAD_GUIDE:
	case ACTOR_ROWING_SWEATING_PASSENGER:
		fdActorSetAnimation(id, ANIM_ROW);
		fdActorSetAnimationSpeedById(id, ANIM_STEP_NORMAL);
		break;
	case ACTOR_ROWING_STARED_AT_PASSENGER:
	case ACTOR_ROWING_WARRIOR:
		fdActorSetAnimation(id, ANIM_ROW);
		fdActorSetAnimationSpeedById(id, ANIM_STEP_NORMAL * 5 / 4);
		break;
	case ACTOR_ROWING_TICKET_GRUMBLER:
		fdActorSetAnimation(id, YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_ROW);
		fdActorSetAnimationSpeedById(id, ANIM_STEP_NORMAL / 2);
		break;
	case ACTOR_ROWING_LADY:
	case ACTOR_ROWING_MUSCLE_ADMIRER:
		fdActorSetAnimation(id, ANIM_ROW);
		fdActorSetAnimationSpeedById(id, ANIM_STEP_NORMAL / 4);
		break;
	case ACTOR_ROWING_OLD_MAN:
		fdActorSetAnimation(id, OLD_MAN_WITH_CANE_ANIMATION_ROW);
		fdActorSetAnimationSpeedById(id, ANIM_STEP_NORMAL / 4);
		break;
	}
}

/* Shows the speaker's next line, then waits 10 ticks. */
static void scShipRoomsSay(Sint32 speaker)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(10);
}

/* Turns an actor to a facing over 10 ticks. */
static void scShipRoomsTurn(Sint32 actorId, Uint16 facing)
{
	fdActorFaceDirection(actorId, facing, 10);
}

/* Places an actor at pixel coordinates and sets its initial heading. */
static void scShipRoomsPlaceActor(
	Sint32 actorId, Sint32 x, Sint32 z, Sint32 facing)
{
	syImportActorSetPos(actorId, toFIXED(x), toFIXED(z), facing);
	fdActorGet(actorId)->facing = facing;
}

/* Moves the camera, gliding unless flags hold SHIP_CAMERA_JUMP, waits for it to
   arrive and recenters the layers as they say, then waits the ticks in their
   low bits. */
static void scShipRoomsMoveCameraAndWaitFX(
	FIXED x, FIXED y, FIXED z, Uint32 flg)
{
	Uint32 ticks;

	fdCameraMoveWithinMapBounds(x, y, z, ~flg & SHIP_CAMERA_JUMP);
	/* BUG: the mask keeps only bits 0, 4, 8 and 12 of the wait, so the callers'
	   waits of 20 and 40 ticks become 16 and none; SHIP_CAMERA_JUMP also adds
	   a tick. */
#if FIX_SHIP_CAMERA_WAIT_MASK
	ticks = flg & SHIP_CAMERA_WAIT_TICKS;
#else
	ticks = flg & 0x1111;
#endif
	if((flg & SHIP_CAMERA_WAIT_FOR_ARRIVAL) != 0)
		fdCameraWaitForMove();
	if((flg & SHIP_CAMERA_RECENTER_LAYERS) != 0)
		fdMapGridCenterOnFollowedPoint();
	fdScriptWait(ticks);
}

/* Reuses the voyage's actor, camera, transition and selection operations. `opt`
   is most often a wait in ticks (the party's last turn, a nod, an emote);
   other steps take it as an animation speed, a second actor or the last of a
   range of flags. */
static void scShipRoomsRunScriptCommand(Sint32 step, Uint32 arg, Uint32 opt)
{
	Uint32 slot;

	switch(step) {
	case SHIP_FACE_PARTY_EAST_AND_WEST:
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_MIA, FACING_WEST, opt);
		break;
	case SHIP_FACE_PARTY:
		fdActorFaceDirection(ACTOR_PARTY_LEADER, arg, 0);
		fdActorFaceDirection(ACTOR_GARET, arg, 0);
		fdActorFaceDirection(ACTOR_IVAN, arg, 0);
		fdActorFaceDirection(ACTOR_MIA, arg, opt);
		break;
	case SHIP_PLAY_PARTY_ANIMATION:
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
		if(arg != 0) {
			fdActorWaitForAnimationChange(ACTOR_MIA);
		}
		if(opt == 0) {
			break;
		}
		fdScriptWait(opt);
		break;
	case SHIP_SHOW_PARTY_EMOTE:
		/* arg is not read. */
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(
			ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(
			ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(
			ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(opt);
		break;
	case SHIP_HIDE_FIRST_OARSMEN:
		for(slot = 0; slot < arg; slot++) {
			fdActorSetPos(slot + ACTOR_ROWING_FIRST_OARSMAN, 0, 0);
		}
		break;
	case SHIP_START_ROWING_ANIMATION: {
		ACTOR *actor;

		actor = fdActorGet(arg);
		actor->facing = FACING_SOUTH_SOUTHWEST;
	}
		fdActorSetAnimation(arg, ANIM_ROW);
		fdActorSetAnimationSpeedById(arg, opt);
		break;
	case SHIP_START_MIRRORED_ROWING_ANIMATION: {
		ACTOR *actor;

		actor = fdActorGet(arg);
		actor->facing = FACING_SOUTH_SOUTHWEST;
		actor->scaleX = -toFIXED(1);
	}
		fdActorSetAnimation(arg, ANIM_ROW);
		fdActorSetAnimationSpeedById(arg, opt);
		break;
	case SHIP_SET_SELECTED_ROWER_POSE: {
		ACTOR *actor;

		actor = fdActorGet(arg);
		actor->facing = FACING_SOUTH_SOUTHWEST;
	}
		scShipRoomsPoseActor(arg);
		if(opt == 0) {
			fdActorSetAnimationSpeedById(arg, 0);
		}
		break;
	case SHIP_OPEN_SCREEN:
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
		fdScriptStartScreenTransition();
		if(arg != 0) {
			fdScriptWaitTransitionTicks();
		}
		/* opt is not read: the 20 ticks some callers pass are not waited. */
		fdScriptWait(0);
		break;
	case SHIP_CLOSE_SCREEN:
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		if(arg == 0) {
			break;
		}
		syImportScriptSetExitRequest(arg);
		break;
	case SHIP_SETUP_ROWING_PARTY:
		scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
		scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
		scShipRoomsSeatChosenRowers(0);
		scShipRoomsPlaceActor(ACTOR_PARTY_LEADER, 432, 168, FACING_SOUTH);
		scShipRoomsPlaceActor(ACTOR_GARET, 448, 168, FACING_SOUTH);
		scShipRoomsPlaceActor(ACTOR_IVAN, 424, 152, FACING_SOUTH);
		scShipRoomsPlaceActor(ACTOR_MIA, 458, 152, FACING_SOUTH);
		break;
	case SHIP_RESTORE_ROWING_DECK_POSES:
		if(arg != 0) {
			ACTOR *actor;

			fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 3, ANIM_STAND);
			fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 3)->facing =
				FACING_SOUTH_SOUTHEAST;
			actor = fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 3);
			actor->scaleX = toFIXED(1);
		}
		{
			ACTOR *actor;

			fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 4, ANIM_STAND);
			fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 4)->facing =
				FACING_SOUTH_SOUTHWEST;
			fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 5, ANIM_STAND);
			fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 5)->facing =
				FACING_SOUTH_SOUTHEAST;
			fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 5)->scaleX = toFIXED(1);
			fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 6, ANIM_STAND);
			fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 6)->facing =
				FACING_SOUTH_SOUTHWEST;
			fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN + 7, ANIM_STAND);
			fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 7)->facing =
				FACING_SOUTH_SOUTHEAST;
			actor = fdActorGet(ACTOR_ROWING_FIRST_OARSMAN + 7);
			actor->scaleX = toFIXED(1);
		}
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR, 410, 174);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 1, 470, 174);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 2, 410, 206);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 3, 470, 206);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 4, 410, 286);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 5, 470, 286);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 6, 410, 316);
		fdActorSetPos(ACTOR_ROWING_FIRST_OAR + 7, 470, 316);
		sySynch(1);
		if(arg != 0) {
			fdActorFaceDirection(
				ACTOR_ROWING_FIRST_OARSMAN + 3, FACING_NORTH_NORTHWEST, 0);
		}
		fdActorFaceDirection(
			ACTOR_ROWING_FIRST_OARSMAN + 4, FACING_NORTH_NORTHEAST, 0);
		fdActorFaceDirection(
			ACTOR_ROWING_FIRST_OARSMAN + 5, FACING_NORTH_NORTHWEST, 0);
		fdActorFaceDirection(
			ACTOR_ROWING_FIRST_OARSMAN + 6, FACING_NORTH_NORTHEAST, 0);
		scShipRoomsTurn(ACTOR_ROWING_FIRST_OARSMAN + 7, FACING_NORTH_NORTHWEST);
		break;
	case SHIP_RESTORE_ROWER_POSE: {
		ACTOR *actor;

		actor = fdActorGet(arg);
		fdActorSetAnimation(arg, ANIM_STAND);
		if(opt != 0) {
			actor->facing = FACING_SOUTH_SOUTHEAST;
		} else {
			actor->facing = FACING_SOUTH_SOUTHWEST;
		}
		actor->scaleX = toFIXED(1);
	} break;
	case SHIP_HIDE_QUARTERS_CREW:
		fdActorSetPos(ACTOR_QUARTERS_KAJA, 0, 0);
		fdActorSetPos(ACTOR_QUARTERS_SEAN, 0, 0);
		fdActorSetPos(ACTOR_QUARTERS_OURANOS, 0, 0);
		fdActorSetPos(ACTOR_QUARTERS_SECOND_GUIDE, 0, 0);
		fdActorSetPos(ACTOR_QUARTERS_CREWMAN, 0, 0);
		break;
	case SHIP_HIDE_OURANOS_AND_SEAN:
		fdActorSetPos(ACTOR_HOLD_SEAN, 0, 0);
		fdActorSetPos(ACTOR_HOLD_OURANOS, 0, 0);
		break;
	case SHIP_SETUP_CAPTAINS_QUARTERS:
		scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
		fdActorSetPos(ACTOR_QUARTERS_KAJA, 0, 0);
		fdActorSetPos(ACTOR_QUARTERS_CREWMAN, 0, 0);
		scShipRoomsPlaceActor(
			ACTOR_QUARTERS_CAPTAIN, 444, 614, FACING_NORTH_NORTHEAST);
		fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
		if(arg != 0) {
			scShipRoomsPlaceAnchorCharm();
		}
		scShipRoomsMoveCameraAndWait(
			448, 32, 624, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
		if(opt == 0) {
			break;
		}
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptWait(20);
		break;
	case SHIP_SETUP_RECRUITMENT_DECK:
		fdActorSetPos(ACTOR_ROWING_CREWMAN, 0, 0);
		fdActorSetPos(ACTOR_ROWING_DRUMMER, 0, 0);
		fdActorSetPos(ACTOR_ROWING_KAJA, 438, 152);
		break;
	case SHIP_HIDE_OARS:
		for(slot = 0; slot < ROWING_DECK_SEATS; slot++) {
			fdActorSetPos(slot + ACTOR_ROWING_FIRST_OAR, 0, 0);
		}
		break;
	case SHIP_SETUP_PASSENGER_HOLD:
		scShipRoomsPlaceActor(
			ACTOR_HOLD_SECOND_GUIDE, 152, 532, FACING_NORTH_NORTHWEST);
		scShipRoomsPlaceActor(
			ACTOR_HOLD_WARRIOR, 134, 490, FACING_SOUTH_SOUTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_HOLD_TICKET_GRUMBLER, 166, 490, FACING_SOUTH_SOUTHWEST);
		scShipRoomsPlaceActor(
			ACTOR_HOLD_MUSCLE_ADMIRER, 182, 504, FACING_SOUTH_SOUTHWEST);
		scShipRoomsPlaceActor(
			ACTOR_HOLD_OLD_MAN, 118, 504, FACING_SOUTH_SOUTHEAST);
		scShipRoomsRunScriptCommand(SHIP_HIDE_OURANOS_AND_SEAN, 0, 0);
		break;
	case SHIP_SETUP_MONSTER_ENCOUNTER:
		scShipRoomsPlaceActor(ACTOR_ROWING_CREWMAN, 416, 328, FACING_EAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_DRUMMER, 448, 352, FACING_NORTH_NORTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_FIRST_OARSMAN, 454, 248, FACING_SOUTH_SOUTHEAST);
		scShipRoomsPlaceActor(arg, 408, 290, FACING_EAST);
		scShipRoomsPlaceActor(opt, 408, 342, FACING_EAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_FIRST_OARSMAN + 3, 420, 356, FACING_NORTH_NORTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_FIRST_OARSMAN + 4, 408, 304, FACING_EAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_FIRST_OARSMAN + 5, 418, 378, FACING_NORTH_NORTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_FIRST_OARSMAN + 6, 440, 262, FACING_SOUTH_SOUTHEAST);
		scShipRoomsPlaceActor(
			ACTOR_ROWING_FIRST_OARSMAN + 7, 448, 378, FACING_NORTH_NORTHEAST);
		break;
	case SHIP_CLEAR_FLAG_RANGE:
		for(slot = arg; slot <= opt; slot++) {
			gmFlagClear(slot);
		}
		break;
	case SHIP_CLEAR_ROWING_SELECTIONS:
		scShipRoomsRunScriptCommand(SHIP_CLEAR_FLAG_RANGE,
			FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
			FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + PASSENGER_COUNT - 1);
		scShipRoomsRunScriptCommand(SHIP_CLEAR_FLAG_RANGE,
			FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
			FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + PASSENGER_COUNT - 1);
		scShipRoomsRunScriptCommand(SHIP_CLEAR_FLAG_RANGE,
			FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS,
			FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + PASSENGER_COUNT - 1);
		gmFlagClear(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
		gmFlagClear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
		gmFlagClear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
		break;
	case SHIP_RESTORE_TREMBLING_OARSMAN:
		sySynch(1);
		scShipRoomsRunScriptCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
		fdActorSetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, ShipRoomsTremble);
		break;
	case SHIP_RELEASE_COMPANIONS:
		fdActorReleaseById(ACTOR_GARET);
		fdActorReleaseById(ACTOR_IVAN);
		fdActorReleaseById(ACTOR_MIA);
		break;
	case SHIP_RESET_CAMERA:
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		sySynch(1);
		if(arg != 0) {
			fdCameraGetActor()->motionFlg = 0;
		}
		break;
	case SHIP_PLACE_COMPANIONS_AND_MONSTERS:
		fdScriptAppendActorPlacements(ShipRoomsCompanionActorPlacements);
		sySynch(1);
		if(arg == 1) {
			fdScriptAppendActorPlacements(ShipRoomsFirstAttackMonsters);
			sySynch(1);
		} else if(arg == 2) {
			fdScriptAppendActorPlacements(ShipRoomsSecondAttackMonsters);
			sySynch(1);
		} else if(arg == 3) {
			fdScriptAppendActorPlacements(ShipRoomsThirdAttackMonsters);
			sySynch(1);
		}
		break;
	}
}

/* The passenger chosen for a seat (0 to 3): their actor in the hold at sea if
   inHold, else on the rowing deck; 0 if no one has been chosen. */
static Sint32 scShipRoomsFindSeatRower(Uint32 seat, Sint32 inHold)
{
	Sint32 flg = 0;
	Sint32 id = ACTOR_SEA_HOLD_FIRST_PASSENGER;
	Uint32 i;

	if(inHold == 0) {
		id = ACTOR_ROWING_FIRST_PASSENGER;
	}
	switch(seat) {
	case 0:
		flg = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
		break;
	case 1:
		flg = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
		break;
	case 2:
		flg = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
		break;
	case 3:
		flg = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
		break;
	}
	for(i = 0; i < PASSENGER_COUNT; i++, flg++, id++) {
		if(gmFlagGet(flg) != 0) {
			return id;
		}
	}
	return 0;
}

/* Hides the four passengers who are already filling rowing seats. */
static void scShipRoomsHideSelectedPassengers(void)
{
	scShipRoomsHideSelectedPassenger(
		ACTOR_SEA_HOLD_FIRST_PASSENGER, FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS);
	scShipRoomsHideSelectedPassenger(
		ACTOR_SEA_HOLD_FIRST_PASSENGER, FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
	scShipRoomsHideSelectedPassenger(
		ACTOR_SEA_HOLD_FIRST_PASSENGER, FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS);
	scShipRoomsHideSelectedPassenger(
		ACTOR_SEA_HOLD_FIRST_PASSENGER, FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
}

/* Hides the passenger whose selection flag is set in this seat's flag group. */
static void scShipRoomsHideSelectedPassenger(Sint32 actorId, Sint32 firstFlg)
{
	unsigned int i = 0;

	do {
		if(gmFlagGet(firstFlg) != 0) {
			fdActorSetPos(actorId, 0, 0);
			break;
		}
		i++;
		actorId++;
		firstFlg++;
	} while(i < PASSENGER_COUNT);
}

/* Monsters are on the rowing deck in the second attack, the first passenger
   chosen trembling at the oars. */
static void scShipRoomsStartSecondAttack(void)
{
	Sint32		  firstRower;
	const Sint32 *rowerScript;
	const Sint32 *entryScript;

	firstRower = scShipRoomsFindSeatRower(0, 0);
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 2, 0);
	scShipRoomsSetOarCollision();
	scShipRoomsRunScriptCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
		ACTOR_ROWING_FIRST_OARSMAN + 2);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
	rowerScript = ShipRoomsTremble;
	fdActorSetScript(firstRower, rowerScript);
	fdActorReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 1);
	fdActorSetScript(ACTOR_ROWING_FIRST_OARSMAN + 2, rowerScript);
	entryScript = ShipRoomsMonsterFloat;
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
	scShipRoomsReportMonstersBelowdecks();
	fdScriptEnd();
}

/* An oarsman cries "Monsters belowdecks!", Garet, Ivan and Mia run up beside
   Isaac, and the crewman begs "Isaac! Take care of these beasts and save my
   mates!" The three fall in behind Isaac, and the stairs up stay barred until
   the monsters are beaten. */
static void scShipRoomsReportMonstersBelowdecks(void)
{
	ACTOR *actor;
	Sint32 msg;

	scShipRoomsMoveCameraAndWait(
		440, CAMERA_KEEP, 176, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	fdActorSetPos(ACTOR_PARTY_LEADER, 440, 134);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 408, 134);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 408, 152);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 432, 166);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_GARET);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	sySynch(1);
	fdActorSetSpeed(ACTOR_IVAN, 1.6);
	fdActorWalkTo(ACTOR_IVAN, 424, 152);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorWalkTo(ACTOR_GARET, 448, 168);
	fdActorSetSpeed(ACTOR_MIA, 2);
	fdActorWalkToAndWait(ACTOR_MIA, 458, 152);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 40);
	fdMessageSetVoiceFromActor(ACTOR_ROWING_FIRST_OARSMAN);
	msg = MsgTolbiShipMonstersBelowdecks;
	/* "Monsters belowdecks!" */
	mnFieldMessageShowCentered(msg, CENTERED_MESSAGE_WINDOW, 10);
	fdScriptWait(10);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_SOUTH, 20);
	fdCameraSetSpeed(3.6);
	scShipRoomsMoveCameraAndWait(
		440, CAMERA_KEEP, 320, SHIP_CAMERA_WAIT_FOR_ARRIVAL | 20);
	fdActorRunRepeatedMotion(ACTOR_ROWING_CREWMAN, 2);
	scShipRoomsTurn(ACTOR_ROWING_CREWMAN, FACING_NORTH_NORTHEAST);
	fdScriptSetMessageId(msg + 1);
	/* "Isaac! Take care of these beasts and save my mates!" */
	scShipRoomsSay(ACTOR_ROWING_CREWMAN);
	fdActorFaceDirection(ACTOR_ROWING_CREWMAN, FACING_EAST, 20);
	scShipRoomsMoveCameraAndWait(
		440, CAMERA_KEEP, 134, SHIP_CAMERA_WAIT_FOR_ARRIVAL);
	fdActorSetScript(ACTOR_GARET, ShipRoomsCompanionsJoinLeaderAtFront);
	fdActorSetScript(ACTOR_IVAN, ShipRoomsCompanionsJoinLeaderAtFront);
	fdActorRunScriptAndWait(ACTOR_MIA, ShipRoomsCompanionsJoinLeaderAtFront);
	fdScriptWait(40);
	gmFlagSet(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
	scShipRoomsRunScriptCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
}

/* Back on the rowing deck after the battle, Kaja finds an oarsman missing and
   sends the party for a passenger to row in his place. */
static void scShipRoomsFindOarsmanMissing(void)
{
	const Sint32 *action;

	scShipRoomsPlaceActor(ACTOR_PARTY_LEADER, 444, 300, FACING_EAST);
	scShipRoomsPlaceActor(ACTOR_GARET, 458, 310, FACING_EAST);
	scShipRoomsPlaceActor(ACTOR_IVAN, 444, 330, FACING_EAST);
	scShipRoomsPlaceActor(ACTOR_MIA, 432, 310, FACING_EAST);
	scShipRoomsPlaceActor(ACTOR_ROWING_KAJA, 440, 134, FACING_WEST);
	scShipRoomsPlaceActor(
		ACTOR_ROWING_FIRST_OARSMAN, 454, 248, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
	scShipRoomsMoveCameraAndWait(
		440, CAMERA_KEEP, 308, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
	fdScriptSetMessageId(MsgTolbiShipHeyAreYouOk);
	/* "Hey, are you OK?" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_NORTH, 0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(440, CAMERA_KEEP, 176, 1);
	fdActorSetSpeed(ACTOR_ROWING_KAJA, 1.6);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 134);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 152);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 424, 164);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(440, CAMERA_KEEP, 300, 1);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 424, 222);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 424, 262);
	fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	/* "Looks like you took care of those sea monsters." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	/* "We're missing an oarsman!" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	scShipRoomsRunScriptCommand(SHIP_SHOW_PARTY_EMOTE, 2, 60);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_NORTHEAST, 60);
	fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 40);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	fdActorSetAnimation(ACTOR_ROWING_KAJA, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_ROWING_KAJA, 432, 268);
	fdActorMoveToAndWait(ACTOR_ROWING_KAJA, 452, 268);
	fdActorSetAnimation(ACTOR_ROWING_KAJA, ANIM_STAND);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_NORTH_NORTHEAST);
	fdActorStartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
	/* "Oh no! He's unconscious!" */
	fdScriptShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 20);
	scShipRoomsRunScriptCommand(SHIP_FACE_PARTY, FACING_NORTH, 20);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
	fdScriptWait(40);
	/* "It looks like the sea monsters got one of our rowers..." */
	fdScriptShowMessageThenWait(ACTOR_ROWING_KAJA, 0, 80);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	fdScriptWait(10);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHWEST);
	/* "There's no way around it..." */
	fdScriptOpenMessage(ACTOR_ROWING_KAJA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == YES) {
		fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
		/* "Hop to it!" */
		scShipRoomsSay(ACTOR_ROWING_KAJA);
	} else {
		fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
		/* "Look, I know you don't want to have to do this, but you..." */
		scShipRoomsSay(ACTOR_ROWING_KAJA);
		scShipRoomsRunScriptCommand(SHIP_SHOW_PARTY_EMOTE, 2, 40);
		fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
		fdActorSetAnimation(ACTOR_ROWING_KAJA, ANIM_NOD);
		/* "Now, bring a passenger who can help row." */
		scShipRoomsSay(ACTOR_ROWING_KAJA);
	}
	scShipRoomsRunScriptCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
	action = ShipRoomsCompanionsJoinLeaderByBackSeats;
	fdActorSetScript(ACTOR_GARET, action);
	fdActorSetScript(ACTOR_IVAN, action);
	fdActorRunScriptAndWait(ACTOR_MIA, action);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(440, CAMERA_KEEP, 176, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 424, 272);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 424, 164);
	fdScriptWait(60);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN, 0, 0);
	gmFlagClear(FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS);
	gmFlagClear(FLAG_TOLBI_SHIP_ROWING);
	syImportScriptSetExitRequest(SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD);
}

/* Back on the rowing deck after the second battle, with the first rower chosen
   in his seat, Kaja finds another oarsman hurt. */
static void scShipRoomsEndSecondAttack(void)
{
	Sint32 firstRower = scShipRoomsFindSeatRower(0, 0);

	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
	scShipRoomsSeatChosenRowers(0);
	scShipRoomsRunScriptCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
		ACTOR_ROWING_FIRST_OARSMAN + 2);
	fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
	scShipRoomsFindOarsmanMissing();
	gmFlagSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF);
	fdScriptEnd();
}

/* Monsters are on the rowing deck in the third attack, with two of the
   passengers chosen at the oars. */
static void scShipRoomsStartThirdAttack(void)
{
	Sint32		  secondRower;
	Sint32		  firstRower;
	const Sint32 *rowerScript;
	const Sint32 *entryScript;

	firstRower = scShipRoomsFindSeatRower(0, 0);
	secondRower = scShipRoomsFindSeatRower(1, 0);
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 3, 0);
	scShipRoomsSetOarCollision();
	scShipRoomsRunScriptCommand(
		SHIP_SETUP_MONSTER_ENCOUNTER, firstRower, secondRower);
	fdActorSetAnimation(ACTOR_ROWING_FIRST_OARSMAN, ANIM_OARSMAN_INJURED);
	rowerScript = ShipRoomsTremble;
	fdActorSetScript(firstRower, rowerScript);
	fdActorReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 1);
	fdActorSetScript(secondRower, rowerScript);
	fdActorReleaseById(ACTOR_ROWING_FIRST_OARSMAN + 2);
	entryScript = ShipRoomsMonsterFloat;
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER, entryScript);
	fdActorSetScript(ACTOR_ROWING_FIRST_MONSTER + 1, entryScript);
	fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_ROWING_FIRST_MONSTER + 1, SPRITE_COLOR_BGR);
	scShipRoomsReportMonstersBelowdecks();
	fdScriptEnd();
}

/* Back on the rowing deck after the third battle, Kaja finds another oarsman
   hurt. */
static void scShipRoomsEndThirdAttack(void)
{
	Sint32 firstRower = scShipRoomsFindSeatRower(0, 0);
	Sint32 secondRower = scShipRoomsFindSeatRower(1, 0);

	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_RESET_CAMERA, 1, 0);
	scShipRoomsRunScriptCommand(SHIP_PLACE_COMPANIONS_AND_MONSTERS, 0, 0);
	scShipRoomsSeatChosenRowers(0);
	scShipRoomsRunScriptCommand(
		SHIP_SETUP_MONSTER_ENCOUNTER, firstRower, secondRower);
	fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 1, 0, 0);
	fdActorSetPos(ACTOR_ROWING_FIRST_OARSMAN + 2, 0, 0);
	scShipRoomsFindOarsmanMissing();
	gmFlagSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF);
	fdScriptEnd();
}

/* The ship rocks badly as it sails on, and Mia wonders "I wonder what's
   wrong... The ship shouldn't be rocking this badly."

   Kaja runs down calling Isaac up on deck, "there's a huge sea monster on the
   ship!", Garet says "Let's go bag us some sea monsters," and the party falls
   in behind Isaac. The ship shakes now and then while the Kraken is on deck
   (scShipRoomsShakeWhileKrakenOnDeck). */
static void scShipRoomsNoticeShipRocking(void)
{
	Sint32		  secondRower;
	Sint32		  thirdRower;
	Sint32		  firstRower;
	const Sint32 *action;

	firstRower = scShipRoomsFindSeatRower(0, 0);
	secondRower = scShipRoomsFindSeatRower(1, 0);
	thirdRower = scShipRoomsFindSeatRower(2, 0);
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
	scShipRoomsRunScriptCommand(SHIP_HIDE_OARS, 0, 0);
	fdActorSetPos(ACTOR_ROWING_CREWMAN, 472, 152);
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
	fdActorSetPos(ACTOR_ROWING_KAJA, 440, 134);
	fdActorSetSprColById(ACTOR_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_ROWING_KAJA), 0);
	scShipRoomsInstallRowingDeckActors(ANIM_STEP_NORMAL);
	scShipRoomsMoveCameraAndWait(
		438, CAMERA_KEEP, 174, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	scShipRoomsRunScriptCommand(SHIP_OPEN_SCREEN, 1, 20);
	sySndPlay(MUS_SILENCE);
	sySndPlay(SE_BOOM);
	fdMapGridSetShake(2, 2, 1);
	fdScriptWait(10);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptWait(80);
	sySndPlay(SE_BOOM);
	fdMapGridSetShake(2, 2, 1);
	fdScriptWait(10);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	sySndPlay(MUS_MOUNT_ALEPH_ERUPTS);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	scShipRoomsTurn(ACTOR_MIA, FACING_SOUTHWEST);
	/* The party notices the rocking, then turns to the crew for an
	   explanation. */
	fdScriptSetMessageId(MsgTolbiShipWonderWhatsWrongShipShouldnt);
	/* "I wonder what's wrong... The ship shouldn't be rocking this..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 40);
	/* "Isaac!" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	scShipRoomsTurn(ACTOR_IVAN, FACING_SOUTHEAST);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	/* "Isaac, up on deck!" */
	scShipRoomsSay(ACTOR_IVAN);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
	fdActorSetSprColById(ACTOR_ROWING_KAJA, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_ROWING_KAJA), SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_ROWING_KAJA, 1);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 430, 134);
	scShipRoomsTurn(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST);
	fdActorStartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
	/* "Isaac, there's a huge sea monster on the ship!" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorStartRepeatedMotion(firstRower, 1);
	fdActorStartRepeatedMotion(secondRower, 1);
	fdActorStartRepeatedMotion(thirdRower, 1);
	fdActorRunRepeatedMotion(ACTOR_ROWING_FIRST_OARSMAN + 3, 1);
	fdActorSetAttachedEffect(
		firstRower, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		secondRower, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		thirdRower, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_ROWING_FIRST_OARSMAN + 3,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWING_DECK_POSES, 1, 0);
	fdActorFaceDirection(firstRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(secondRower, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(thirdRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorStartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
	/* "If we don't do something... we'll sink! You have to help us!" */
	fdScriptShowMessage(ACTOR_ROWING_KAJA, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 440, 134);
	fdActorSetPos(ACTOR_ROWING_KAJA, 0, 0);
	scShipRoomsTurn(ACTOR_GARET, FACING_WEST);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	/* "If this ship sinks, everything we've done will mean nothing..." */
	scShipRoomsSay(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	scShipRoomsTurn(ACTOR_MIA, FACING_WEST);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	action = ShipRoomsCompanionsJoinLeaderByFrontSeats;
	fdActorSetScript(ACTOR_GARET, action);
	fdActorSetScript(ACTOR_IVAN, action);
	fdActorRunScriptAndWait(ACTOR_MIA, action);
	gmFlagSet(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
	ShipRoomsShakeTimer = 0;
	sySetTickEvent(scShipRoomsShakeWhileKrakenOnDeck, EVENT_PASS_TICK);
	scShipRoomsRunScriptCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
	fdActorReleaseById(ACTOR_ROWING_KAJA);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagClear(FLAG_TOLBI_SHIP_ROWING);
	fdScriptEnd();
}

/* While the Kraken is on deck, now and then (one chance in 120 a tick) plays a
   rumble with a shake, easing the shake after ten ticks; no other rumble
   starts for 80 ticks. */
static void scShipRoomsShakeWhileKrakenOnDeck(void)
{
	if(ShipRoomsShakeTimer != 0) {
		if(--ShipRoomsShakeTimer == 70)
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	} else if((syRandomBelow(120)) == 0) {
		sySndPlay(SE_BOOM);
		fdMapGridSetShake(2, 2, 1);
		ShipRoomsShakeTimer = 80;
	}
}

/* A glimpse of the captain's cabin, Kaja beside the captain, once the ship has
   come off course, before it sails on across the World Map to the island. */
static void scShipRoomsShowCaptainsQuartersBeforeIsland(void)
{
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_SETUP_CAPTAINS_QUARTERS, 1, 0);
	scShipRoomsPlaceActor(ACTOR_QUARTERS_KAJA, 468, 616, FACING_WEST);
	scShipRoomsRunScriptCommand(SHIP_OPEN_SCREEN, 1, 20);
	fdActorRunRepeatedMotion(ACTOR_QUARTERS_KAJA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_NORTH_NORTHEAST, 80);
	fdActorFaceDirection(ACTOR_QUARTERS_CAPTAIN, FACING_EAST, 20);
	fdActorSetAnimationAndWait(ACTOR_QUARTERS_CAPTAIN, ANIM_NOD);
	fdScriptWait(20);
	scShipRoomsRunScriptCommand(SHIP_CLOSE_SCREEN, SHIP_ROOMS_TO_WORLD_MAP, 0);
}

/* How the passenger at the first seat answers Kaja at the island
   (scShipRoomsReachIsland), each with lines of their own. */
typedef enum ISLAND_REACTION {
	ISLAND_REACTION_OTHERS,
	/* The lady or the muscle admirer. */
	ISLAND_REACTION_LADY_OR_ADMIRER,
	ISLAND_REACTION_OLD_MAN,
	/* A guide, who says the same as the others at first. */
	ISLAND_REACTION_GUIDE
} ISLAND_REACTION;

/* Kaja comes down to the rowing deck: "We've reached the island! Rowers, rest
   those weary arms!"

   The passengers at the oars ask whether this is Tolbi, Kaja admits the ship
   went off course and stopped to get its bearings, and the oarsmen go out to
   take their break. */
static void scShipRoomsReachIsland(void)
{
	Sint32		  fourthRower;
	Sint32		  secondRower;
	Sint32		  thirdRower;
	Sint32		  firstRower;
	Sint32		  reaction = ISLAND_REACTION_OTHERS;
	const Sint32 *goUpstairs;
	const Sint32 *stepOut;
	const Sint32 *joinLeader;

	firstRower = scShipRoomsFindSeatRower(0, 0);
	secondRower = scShipRoomsFindSeatRower(1, 0);
	thirdRower = scShipRoomsFindSeatRower(2, 0);
	fourthRower = scShipRoomsFindSeatRower(3, 0);
	fdScriptStart();
	scShipRoomsSetOarCollision();
	scShipRoomsRunScriptCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
	scShipRoomsRunScriptCommand(SHIP_HIDE_OARS, 0, 0);
	fdActorSetPos(ACTOR_ROWING_CREWMAN, 472, 152);
	fdActorSetPos(ACTOR_ROWING_KAJA, 440, 134);
	fdActorSetSprColById(ACTOR_ROWING_KAJA, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_ROWING_KAJA), 0);
	scShipRoomsInstallRowingDeckActors(ANIM_STEP_NORMAL);
	fdActorSetAnimation(ACTOR_ROWING_DRUMMER, ANIM_DRUMMER_HOLD_DRUM);
	scShipRoomsMoveCameraAndWait(
		438, CAMERA_KEEP, 174, SHIP_CAMERA_RECENTER_LAYERS | SHIP_CAMERA_JUMP);
	scShipRoomsRunScriptCommand(SHIP_OPEN_SCREEN, 1, 20);
	fdActorSetSprColById(ACTOR_ROWING_KAJA, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_ROWING_KAJA), SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_ROWING_KAJA, 1);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 142);
	fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_SOUTH_SOUTHEAST, 20);
	fdActorStartRepeatedMotion(ACTOR_ROWING_KAJA, 2);
	fdScriptSetMessageId(MsgTolbiShipReachedIslandRowers);
	/* "We've reached the island! Rowers, rest those weary arms!" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdScriptWait(120);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWER_POSE, fourthRower, 1);
	scShipRoomsRunScriptCommand(SHIP_RESTORE_ROWING_DECK_POSES, 0, 0);
	fdActorFaceDirection(firstRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(secondRower, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(thirdRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(fourthRower, FACING_NORTH_NORTHWEST, 60);
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_OLD_MAN)) {
		reaction = ISLAND_REACTION_OLD_MAN;
	} else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS +
				  PASSENGER_MUSCLE_ADMIRER) ||
		gmFlagGet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_LADY)) {
		reaction = ISLAND_REACTION_LADY_OR_ADMIRER;
	}
	fdActorRunRepeatedMotion(firstRower, 1);
	if(reaction == ISLAND_REACTION_LADY_OR_ADMIRER) {
		FieldStateWork->msg += 1;
	} else if(reaction == ISLAND_REACTION_OLD_MAN) {
		FieldStateWork->msg += 2;
	}
	fdActorStartRepeatedMotion(firstRower, 2);
	/* "Island? Isn't this Tolbi?", the lady's or the muscle admirer's "What do
	   you mean, island? Aren't we in Tolbi?", or the old man's "Uh... Tolbi's
	   not an island, is it?" */
	scShipRoomsSay(firstRower);
	fdScriptSetMessageId(MsgTolbiShipShipWentOff);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_SHAKE_HEAD);
	/* "The ship went off course, and we're..." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorShowEmote(firstRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(secondRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(thirdRower, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(fourthRower, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Raise your oars and rest for a little while." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 444, 132);
	fdActorReleaseById(ACTOR_ROWING_KAJA);
	fdScriptWait(40);
	if(reaction == ISLAND_REACTION_OTHERS &&
		(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_HEAD_GUIDE) ||
			gmFlagGet(
				FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + PASSENGER_SECOND_GUIDE))) {
		reaction = ISLAND_REACTION_GUIDE;
	}
	if(reaction == ISLAND_REACTION_OTHERS) {
		FieldStateWork->msg += 1;
	} else if(reaction == ISLAND_REACTION_LADY_OR_ADMIRER) {
		FieldStateWork->msg += 2;
	} else if(reaction == ISLAND_REACTION_OLD_MAN) {
		FieldStateWork->msg += 3;
	}
	scShipRoomsTurn(firstRower, FACING_EAST);
	/* A guide's "I'm going to check on the other passengers.", or the others'
	   "If the ship isn't going anywhere, I'm not waiting down here.", the
	   lady's or the muscle admirer's "If I'm not rowing, I ain't staying here."
	   or the old man's "As long as we're stopping, I don't have to stay in
	   here." */
	scShipRoomsSay(firstRower);
	fdActorSetSpeed(firstRower, 1);
	goUpstairs = ShipRoomsGoUpstairs;
	fdActorRunScriptAndWait(firstRower, goUpstairs);
	fdActorFaceDirection(secondRower, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(thirdRower, FACING_EAST, 0);
	fdActorFaceDirection(fourthRower, FACING_WEST, 40);
	fdActorFaceDirection(secondRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(thirdRower, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(fourthRower, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetSpeed(secondRower, 1);
	fdActorSetSpeed(thirdRower, 1);
	fdActorSetSpeed(fourthRower, 1);
	fdActorSetScript(thirdRower, goUpstairs);
	fdScriptWait(40);
	stepOut = ShipRoomsStepOutFromSeat;
	fdActorRunScriptAndWait(secondRower, stepOut);
	fdActorSetScript(secondRower, goUpstairs);
	fdActorRunScriptAndWait(fourthRower, stepOut);
	fdActorRunScriptAndWait(fourthRower, goUpstairs);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	joinLeader = ShipRoomsCompanionsJoinLeaderAtFront;
	fdActorSetScript(ACTOR_GARET, joinLeader);
	fdActorSetScript(ACTOR_IVAN, joinLeader);
	fdActorRunScriptAndWait(ACTOR_MIA, joinLeader);
	scShipRoomsRunScriptCommand(SHIP_RELEASE_COMPANIONS, 0, 0);
	gmFlagClear(FLAG_TOLBI_SHIP_ROWING);
	gmFlagSet(FLAG_TOLBI_SHIP_AT_ISLAND);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	fdScriptEnd();
}

/* Back from the island, Kaja has the rowers change sides: "Sorry, everyone, but
   we need to switch the left and right teams! We've strayed off course because
   the left team's timing is off."

   The passengers chosen swap seats, and the ship sets off one last time for
   Tolbi. */
static void scShipRoomsSwitchLeftAndRightTeams(void)
{
	Sint32 firstRower;
	Sint32 fourthRower;
	Sint32 thirdRower;
	Sint32 secondRower;

	firstRower = scShipRoomsFindSeatRower(0, 0);
	secondRower = scShipRoomsFindSeatRower(1, 0);
	thirdRower = scShipRoomsFindSeatRower(2, 0);
	fourthRower = scShipRoomsFindSeatRower(3, 0);
	fdScriptStart();
	scShipRoomsRunScriptCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
	scShipRoomsPlaceActor(
		ACTOR_ROWING_CREWMAN, 472, 144, FACING_SOUTH_SOUTHWEST);
	scShipRoomsPlaceActor(ACTOR_ROWING_KAJA, 408, 142, FACING_SOUTH_SOUTHEAST);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	fdScriptSetMessageId(MsgTolbiShipSorryEveryoneButWeNeed);
	/* "Sorry, everyone , but we need to switch the left and right..." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorStartRepeatedMotion(firstRower, 2);
	fdActorStartRepeatedMotion(secondRower, 2);
	fdActorStartRepeatedMotion(thirdRower, 2);
	fdActorRunRepeatedMotion(fourthRower, 2);
	fdScriptWait(20);
	fdActorFaceDirection(firstRower, FACING_EAST, 0);
	fdActorFaceDirection(secondRower, FACING_WEST, 0);
	fdActorFaceDirection(thirdRower, FACING_EAST, 0);
	fdActorFaceDirection(fourthRower, FACING_WEST, 40);
	fdActorSetSpeed(firstRower, 1);
	fdActorSetSpeed(secondRower, 1);
	fdActorSetSpeed(thirdRower, 1);
	fdActorSetSpeed(fourthRower, 1);
	fdActorWalkTo(firstRower, 470, 172);
	fdActorWalkTo(secondRower, 410, 172);
	fdActorWalkTo(thirdRower, 470, 204);
	fdActorWalkToAndWait(fourthRower, 410, 204);
	fdActorSetAnimation(firstRower, ANIM_STAND);
	fdActorSetAnimation(secondRower, ANIM_STAND);
	fdActorSetAnimation(thirdRower, ANIM_STAND);
	fdActorFaceDirection(secondRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(firstRower, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(fourthRower, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(thirdRower, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_ROWING_KAJA, 1);
	/* "One last push should get us there..." */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorSetAnimation(firstRower, ANIM_NOD);
	fdActorSetAnimation(secondRower, ANIM_NOD);
	fdActorSetAnimation(thirdRower, ANIM_NOD);
	fdActorSetAnimationAndWait(fourthRower, ANIM_NOD);
	/* "All right! Let's aim for Tolbi and set off one last time!" */
	scShipRoomsSay(ACTOR_ROWING_KAJA);
	fdActorSetAnimation(firstRower, ANIM_NOD);
	fdActorSetAnimation(secondRower, ANIM_NOD);
	fdActorSetAnimation(thirdRower, ANIM_NOD);
	fdActorSetAnimationAndWait(fourthRower, ANIM_NOD);
	fdActorFaceDirection(ACTOR_ROWING_KAJA, FACING_EAST, 0);
	scShipRoomsTurn(ACTOR_PARTY_LEADER, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ROWING_KAJA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_ROWING_KAJA, 1);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 408, 132);
	fdActorWalkToAndWait(ACTOR_ROWING_KAJA, 444, 132);
	fdActorSetPos(ACTOR_ROWING_KAJA, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	scShipRoomsSwapRowerSelections(
		FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS, FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
	scShipRoomsSwapRowerSelections(
		FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS, FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
	gmFlagClear(FLAG_TOLBI_SHIP_AT_ISLAND);
	syImportScriptSetExitRequest(
		SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_READY);
}

/* Exchanges the chosen passengers between two rowing seats. */
static void scShipRoomsSwapRowerSelections(
	Sint32 firstSeatFlg, Sint32 secondSeatFlg)
{
	Sint32 firstPassenger = 0;
	Sint32 secondPassenger = 0;
	Uint32 i;

	for(i = 0; i < PASSENGER_COUNT; i++) {
		Sint32 flg = firstSeatFlg + i;
		if(gmFlagGet(flg) != 0) {
			gmFlagClear(flg);
			break;
		}
		firstPassenger++;
	}

	for(i = 0; i < PASSENGER_COUNT; i++) {
		Sint32 flg = secondSeatFlg + i;
		if(gmFlagGet(flg) != 0) {
			gmFlagClear(flg);
			break;
		}
		secondPassenger++;
	}

	gmFlagSet(secondSeatFlg + firstPassenger);
	gmFlagSet(firstSeatFlg + secondPassenger);
}

#include "shared/pushed_actor_direction_steps.c"

/* The warriors walk out of the hold. */
Sint32 ShipRoomsLeaveHold[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 500),
	SC_SYNC,
	SC_MOVE(168, 0, 584),
	SC_SYNC,
	SC_CALL(scShipRoomsResetActor),
	SC_END,
};

/* Those gathered in the captain's quarters walk out. */
Sint32 ShipRoomsLeaveCaptainsQuarters[] = {
	SC_SPEED(1),
	SC_ACCELERATION(0.5),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(456, 0, 674),
	SC_SYNC,
	SC_MOVE(456, 0, 712),
	SC_SYNC,
	SC_CALL(scShipRoomsResetActor),
	SC_END,
};

/* The Anchor Charm swells and shrinks twice as the captain raises it. */
Sint32 ShipRoomsAnchorCharmPulse[] = {
	SC_SET(SW_SCALE_X, toFIXED(11) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(11) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(9) / 8),
	SC_SET(SW_SCALE_Y, toFIXED(9) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_LOOP(2, SC_TOP),
	SC_END,
};

/* Garet, Ivan or Mia walks into the leader, who stands at the front of the
   rowing deck below the stairs, and is put away. */
Sint32 ShipRoomsCompanionsJoinLeaderAtFront[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(434, 0, 166),
	SC_SYNC,
	SC_CALL(scShipRoomsResetActor),
	SC_END,
};

/* The same, the leader standing between the back seats. */
Sint32 ShipRoomsCompanionsJoinLeaderByBackSeats[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(438, 0, 300),
	SC_SYNC,
	SC_CALL(scShipRoomsResetActor),
	SC_END,
};

/* The same, the leader standing between the front seats, two pixels from where
   ShipRoomsCompanionsJoinLeaderAtFront ends. */
Sint32 ShipRoomsCompanionsJoinLeaderByFrontSeats[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(432, 0, 168),
	SC_SYNC,
	SC_CALL(scShipRoomsResetActor),
	SC_END,
};

/* Squashes the actor and lets it spring back, over and over: a rower trembling
   at the monsters, or the lady seasick in the hold. */
Sint32 ShipRoomsTremble[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_ADD(SW_SCALE_X, -toFIXED(5) / 16),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A monster on the rowing deck floats up and down in place
   (scShipRoomsFloat). */
Sint32 ShipRoomsMonsterFloat[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_USER_VALUE, 0),
	SC_CALL(scShipRoomsFloat),
};

/* A rower walks up the stairs and is put away. */
Sint32 ShipRoomsGoUpstairs[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(408, 0, 132),
	SC_SYNC,
	SC_MOVE(444, 0, 132),
	SC_SYNC,
	SC_RELEASE,
};

/* A rower steps out from the front west seats into the walk between them. */
Sint32 ShipRoomsStepOutFromSeat[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(416, 0, 186),
	SC_SYNC,
	SC_END,
};

/* A passenger glances about at random. */
Sint32 ShipRoomsLookAroundScript[] = {
	SC_CALL(scShipRoomsLookAround),
};

/* Hides the actor at once (scShipRoomsHide). */
Sint32 ShipRoomsHideScript[] = {
	SC_CALL(scShipRoomsHide),
};

/* The Anchor Charm's actor, once placed this visit. */
ACTOR *ShipRoomsAnchorCharm = 0;

/* The other scenes have no entrances. */
SCENE_ENTRANCE ShipRoomsEntrancesOther[] = {
	{SCENE_TABLE_END},
};

/* The passengers' hold, the cargo hold, the rowing deck and the captain's
   quarters, each entrance named for its room. */
SCENE_ENTRANCE ShipRoomsEntrances[] = {
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

Uint32 ShipRoomsExits[] = {
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
		ShipRooms, SHIP_ROOMS_PASSENGER_HOLD_FROM_CAPTAINS_ROOM_CALL_TO_OARS),
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

ACTOR_PLACE ShipRoomsActorPlacementsNone[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE ShipRoomsCaptainsQuartersActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		446, 0, 620, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KAJA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		472, 0, 626, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		454, 0, 640, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The captain's quarters once the warriors have come to see the captain, */
/* with Garet, Ivan and Mia. */
ACTOR_PLACE ShipRoomsCaptainsQuartersActorPlacementsWarriors[] = {
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

/* The captain's quarters once the ship has reached Tolbi. */
ACTOR_PLACE ShipRoomsCaptainsQuartersActorPlacementsArrived[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		462, 0, 612, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

/* The hold before the party hears of the delay at Kalay: no one but a blue */
/* orb in the middle, hidden at once (ShipRoomsHideScript), which the hold's */
/* other placements end with too. */
ACTOR_PLACE ShipRoomsHoldActorPlacementsEmpty[] = {
	/* XXX (no evidence): what the hidden orb is for; it is never seen, solid */
	/* or a target, and no code of the scene reads it. */
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, (Sint32)ShipRoomsHideScript,
		150, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The hold while the ship waits at Kalay, and after it reaches Tolbi. */
ACTOR_PLACE ShipRoomsHoldActorPlacements[] = {
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
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, (Sint32)ShipRoomsHideScript,
		150, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The hold at sea, once the passengers row. */
ACTOR_PLACE ShipRoomsHoldActorPlacementsAtSea[] = {
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
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, (Sint32)ShipRoomsHideScript,
		150, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The rowing deck, in the order of TOLBI_SHIP_ROWING_DECK_ACTOR_ID. */
/* */
/* They are the crewman and the drummer, the oarsmen at the seats, the */
/* passengers who may take a seat, Kaja and the eight oars beside the seats. */
ACTOR_PLACE ShipRoomsRowingDeckActorPlacements[] = {
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

/* Garet, Ivan and Mia, placed for a script. */
ACTOR_PLACE ShipRoomsCompanionActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The monsters of each attack, beside the seats. */
ACTOR_PLACE ShipRoomsFirstAttackMonsters[] = {
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		474, 0, 300, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		458, 0, 310, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		474, 0, 332, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ShipRoomsSecondAttackMonsters[] = {
	ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		474, 0, 300, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PINK_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		474, 0, 332, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LIZARD_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		458, 0, 310, FACING_WEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ShipRoomsThirdAttackMonsters[] = {
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		474, 0, 300, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		474, 0, 332, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		458, 0, 310, FACING_WEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The cargo hold, and the scripts. */
CUE ShipRoomsCuesOther[] = {
	{ CUE_EXIT, 3, CONDITION_ALWAYS, SHIP_ROOMS_CRATE_HOLD_TO_TOLBI_SHIP_DECK },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_SHIP_ROOMS_CRATE_HOLD_BARREL, FLAG_TOLBI_SHIP_CRATE_HOLD_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ CUE_TABLE_END },
};

/* The hold while the ship waits at Kalay, and after it reaches Tolbi. */
CUE ShipRoomsHoldCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
	{ CUE_STEP, METATILE_BEHAVIOR_SHIP_ROOMS_HOLD_WARRIORS, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, SCRIPT_REF(scShipRoomsWatchWarriorsStormOff) },
	{ CUE_TALK, ACTOR_HOLD_WARRIOR, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToWaitingWarrior) },
	{ CUE_TALK, ACTOR_HOLD_TICKET_GRUMBLER, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
	{ CUE_TALK, ACTOR_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
	{ CUE_TALK, ACTOR_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToWaitingMuscleAdmirer) },
	{ CUE_TALK, ACTOR_HOLD_OLD_MAN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
	{ CUE_TALK, ACTOR_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
	{ CUE_TALK, ACTOR_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
	{ CUE_TALK, ACTOR_HOLD_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToHealer) },
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
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
	{ CUE_TABLE_END },
};

/* The hold at sea, where talking to a passenger asks them to row. */
CUE ShipRoomsHoldCuesChoosingRowers[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
	{ CUE_TALK, ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
	{ CUE_TALK, ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskHeadGuideToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskSweatingPassengerToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskLadyToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskStaredAtPassengerToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskSecondGuideToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskWarriorToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskTicketGrumblerToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskMuscleAdmirerToRow) },
	{ CUE_TALK, ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskOldManToRow) },
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
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
	{ CUE_TABLE_END },
};

/* The hold while the ship waits off the island. */
CUE ShipRoomsHoldCuesAtIsland[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_TOLBI_SHIP_DECK },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, SHIP_ROOMS_PASSENGER_HOLD_TO_ROWING_DECK },
	{ CUE_TALK, ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
	{ CUE_TALK, ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
	{ CUE_TALK, ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
	{ CUE_TALK, ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
	{ CUE_TALK, ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
	{ CUE_TALK, ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
	{ CUE_TALK, ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
	{ CUE_TALK, ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
	{ CUE_TALK, ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
	{ CUE_TALK, ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsAskLadyToRow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_HOLD_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_SHIP_ROOMS_HOLD_BARREL, FLAG_TOLBI_SHIP_HOLD_COINS, FIND_COINS(23) },
	{ CUE_TABLE_END },
};

/* The captain's quarters, from the wait at Kalay until the ship reaches */
/* Tolbi. */
/* */
/* The Anchor Charm is given back to the captain here. */
CUE ShipRoomsCaptainsQuartersCues[] = {
	{ CUE_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
	{ CUE_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToCaptain) },
	{ CUE_TALK, ACTOR_QUARTERS_CREWMAN, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToCaptainsQuartersCrewman) },
	{ CUE_TALK, ACTOR_QUARTERS_KAJA, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
	{ CUE_TALK, ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
	{ CUE_TALK, ACTOR_QUARTERS_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
	{ CUE_TALK, ACTOR_QUARTERS_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
	{ CUE_TALK, ACTOR_QUARTERS_SEAN, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToSeanInCaptainsQuarters) },
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
	{ CUE_STEP, METATILE_BEHAVIOR_SHIP_ROOMS_QUARTERS_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, SCRIPT_REF(scShipRoomsWatchCrewWonderAtDelay) },
	{ USE_ITEM_ON_ACTOR(ITEM_ANCHOR_CHARM), ACTOR_QUARTERS_CAPTAIN, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, SCRIPT_REF(scShipRoomsReturnLuckyAnchor) },
	{ EXAMINE_CUE(EXAMINE_JAR) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
	{ CUE_TABLE_END },
};

/* The captain's quarters while the ship waits off the island. */
CUE ShipRoomsCaptainsQuartersCuesAtIsland[] = {
	{ CUE_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
	{ CUE_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
	{ CUE_TALK, ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
	{ CUE_TABLE_END },
};

/* The captain's quarters once the ship has reached Tolbi. */
CUE ShipRoomsCaptainsQuartersCuesArrived[] = {
	{ CUE_EXIT, 5, CONDITION_ALWAYS, SHIP_ROOMS_CAPTAINS_ROOM_TO_TOLBI_SHIP_DECK },
	{ CUE_TALK, ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_QUARTERS_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SHIP_ROOMS_QUARTERS_JAR, FLAG_TOLBI_SHIP_QUARTERS_NUT, FIND_ITEM(ITEM_NUT) },
	{ CUE_TABLE_END },
};

CUE ShipRoomsRowingDeckCues[] = {
	{ CUE_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD },
	{ CUE_TALK, ACTOR_ROWING_CREWMAN, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsWeCanLeaveAsSoonAs) },
	{ CUE_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenSailingForManyYears) },
	{ CUE_TALK, ACTOR_ROWING_DRUMMER, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis) },
	{ CUE_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowIHaveABlisterOn) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRowingIsAMansJob) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereTheOnesWhoMakeThe) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOnceWeCastOffYouJust) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanOnlyDoHardWork) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHmmmIWonderWhenWereGoing) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfItsOkayTo) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsICantWaitUntilWeGet) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsStoppingAgainAndAgainLikeThis) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIHearTheresAnIsleIn) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLetsSeeIslandsInTheKaragol) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsFromTheSeaItLooksLike) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenWereRowingWeCanJust) },
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
	{ CUE_TABLE_END },
};

/* The rowing deck while monsters are belowdecks, where talking to one */
/* (ACTOR_ROWING_FIRST_MONSTER and the two after it) fights it. */
CUE ShipRoomsRowingDeckCuesMonstersBelowdecks[] = {
	{ CUE_STEP, METATILE_BEHAVIOR_SHIP_ROOMS_ROWING_DECK_STAIRS, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsCallLeaderBackToBeasts) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_MONSTER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsFightRowingDeckMonsters) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_MONSTER + 1, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsFightRowingDeckMonsters) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_MONSTER + 2, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsFightRowingDeckMonsters) },
	{ CUE_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMonstersFromTheDeepAreAttacking) },
	{ CUE_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCurseThoseBeastsTheyGotOne) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 1, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingOffTheMonstersIsYour) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 2, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFoulBeastIMayBeTrembling) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWerentSoTiredId) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCursesIWantToSmackThat) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhoIsThatThingGoingTo) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfYouDontDoABetter) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeOarsmenMayBeStrongBut) },
	{ CUE_TALK, ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSurroundedByDangerOnAll) },
	{ CUE_TALK, ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAhhhhSeaMonstersRealLiveSea) },
	{ CUE_TALK, ACTOR_ROWING_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhaHeyYouThisIsNo) },
	{ CUE_TALK, ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAahIHaveToRowThis) },
	{ CUE_TALK, ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHHeyIsaacShouldntYouBe) },
	{ CUE_TALK, ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYaaahhhIImNotAfraidOf) },
	{ CUE_TALK, ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheOarsmenAreInDireStraights) },
	{ CUE_TALK, ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIICantDoThisFighting) },
	{ CUE_TALK, ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhatHappensNow) },
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
	{ CUE_TABLE_END },
};

/* The rowing deck while the Kraken rocks the ship. */
CUE ShipRoomsRowingDeckCuesKrakenOnDeck[] = {
	{ CUE_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_TOLBI_SHIP_DECK_KRAKEN },
	{ CUE_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
	{ CUE_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
	{ CUE_TALK, ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToHeadGuideAtOars) },
	{ CUE_TALK, ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToSecondGuideAtOars) },
	{ CUE_TALK, ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToSweatingPassengerAtOars) },
	{ CUE_TALK, ACTOR_ROWING_LADY, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToLadyAtOars) },
	{ CUE_TALK, ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToStaredAtPassengerAtOars) },
	{ CUE_TALK, ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToWarriorAtOars) },
	{ CUE_TALK, ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToTicketGrumblerAtOars) },
	{ CUE_TALK, ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsTalkToMuscleAdmirerAtOars) },
	{ CUE_TALK, ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 3, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_HEAD_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadHeadGuideMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SECOND_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadSecondGuideMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_SWEATING_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadSweatingPassengerMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_LADY, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadLadyMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_STARED_AT_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadStaredAtPassengerMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_WARRIOR, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadWarriorMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_TICKET_GRUMBLER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadTicketGrumblerMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_MUSCLE_ADMIRER, CONDITION_ALWAYS, SCRIPT_REF(scShipRoomsReadMuscleAdmirerMindAtOars) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
	{ CUE_TABLE_END },
};

/* The rowing deck once the ship has reached Tolbi. */
CUE ShipRoomsRowingDeckCuesArrived[] = {
	{ CUE_EXIT, 4, CONDITION_ALWAYS, SHIP_ROOMS_ROWING_DECK_TO_PASSENGER_HOLD },
	{ CUE_TALK, ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWillBe) },
	{ CUE_TALK, ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouFolksReallyFoughtWell) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsALotOfOarsmenWereInjured) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAreWeSafeItsAllBecause) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenAnOarsmanAllMy) },
	{ CUE_TALK, ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouGoOnAheadJustWhen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeCantFindAnyoneToReplace) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImJustGladTheKrakenDidnt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 4, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSureThePaysGoodButIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 5, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsaacAndHisPalsWereSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 6, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIFeelBetterNowIWas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROWING_FIRST_OARSMAN + 7, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImInNoRushAnywayThis) },
	{ CUE_TABLE_END },
};
/* clang-format on */

/* Ticks left of the Kraken's current jolt. At 0 another may start. */
Sint32 ShipRoomsShakeTimer;
