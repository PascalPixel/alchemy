/* @file */
/* Tolbi-bound Ship (SceneId_TolbiShipDeck), its deck: the ship that carries */
/* the party and a tour group bound for Colosso across the Karagol Sea from */
/* Kalay to Tolbi. */
/* */
/* The ship's other scenes are its mast (tolbi_ship_mast.c) and below decks */
/* (ship_rooms.c). */
/* */
/* The deck runs north and south. At its north end a ladder climbs to a carved
 */
/* door, the way to the passengers' hold; a small stairway in the middle of */
/* the deck leads down, walking west, to the hold of crates and barrels; and */
/* the door of the cabin at the south end is the captain's quarters. At Kalay */
/* a gangplank on the east side leads back to the dock, and at Tolbi one on */
/* the west side leads ashore. */
/* */
/* The voyage's stage, read from its flags, chooses the deck's actor */
/* placements and events. The stages are: */
/* */
/* - at Kalay, */
/* - under sail once the first attack is fought off */
/*   (FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF), */
/* - while the oars move (FLAG_TOLBI_SHIP_ROWING), */
/* - at anchor off Crossbone Isle (FLAG_TOLBI_SHIP_AT_ISLAND), */
/* - and at Tolbi (FLAG_TOLBI_SHIP_REACHED_TOLBI). */
/* */
/* The setup redraws the deck's cells for the stage. While under sail it */
/* rocks the view and scrolls the sea. It rows the oars to the strokes of the */
/* passengers at the seats (TolbiShipDeckRowerStrokes), and plays the event */
/* its entrance names. */
/* */
/* Each event leaves into the next, in another of the ship's scenes or on the */
/* World Map, whose entrances sail the ship on. In story order: */
/* */
/* - At Kalay (TOLBI_SHIP_DECK_FROM_KALAY_DOCKS), while the ship waits for */
/*   its replacement from Tolbi. Once the tour group has left Kalay */
/*   (FLAG_KALAY_COLOSSO_TOUR_LEFT), walking up to the crew */
/*   (CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW) plays the captain, Kaja and a */
/*   crewman talking over the delay. The captain will sail trusting his anchor
 */
/*   charm, and the crewman slips below */
/*   (FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY). Coming up to his door brings him
 */
/*   out denying everything (FLAG_TOLBI_SHIP_CREWMAN_CAUGHT), and Mind Read */
/*   shows he hid the charm on the mast. Once it is back */
/*   (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) the passengers who promised to row
 */
/*   stand on deck. */
/* - Setting sail (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY): Kaja */
/*   brings word that all is ready ("Preparations... Ready!"), which leads */
/*   to the lookout on the mast. */
/* - The attacks: sea monsters are sighted */
/*   (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS), which leads to */
/*   the captain's quarters, where the captain turns the ship. Then monsters */
/*   leap aboard (TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD), harry */
/*   Ouranos and Sean, the warriors bound for Colosso who guard the deck with */
/*   Kaja (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK), and drop onto */
/*   the deck (TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK). Each of */
/*   these three leads below to the fight among the oars. At */
/*   TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN the party jumps down to */
/*   the deck and fights the Kraken, and returns to */
/*   TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE, where Kaja thanks Isaac */
/*   (FLAG_TOLBI_SHIP_KRAKEN_DEFEATED). */
/* - Land: the lookout's call leads either to */
/*   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED and Tolbi, or, by how */
/*   the passengers chosen to row pull (tolbi_ship_mast.c), to */
/*   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, where the coast is not */
/*   Tolbi's and the crew make for an island. At anchor off it, Crossbone Isle
 */
/*   (reached by TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE), Sean asks the */
/*   party to scout it (FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED). Back aboard */
/*   (TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) the rowers switch sides below and the */
/*   ship sails on to Tolbi. */
/* - At Tolbi (TOLBI_SHIP_DECK_FROM_WORLD_MAP): the passengers who rowed come */
/*   up one by one to thank the leader, and TOLBI_SHIP_DECK_TO_TOLBI_DOCKS */
/*   leads ashore to Tolbi Docks (SceneId_TolbiDocks). */

#include "ability_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/squash_and_stretch.h"
#include "scene/tolbi_ship.h"
#include "text/msg_ids.h"

extern const s32 TolbiShipDeck_KajaLeavesActions[];
extern const s32 TolbiShipDeck_CaptainStepsAsideActions[];
extern const s32 TolbiShipDeck_SquashAndStretchActions[];
extern const s32 TolbiShipDeck_MonsterLeapsAboardActions[];
extern const s32 TolbiShipDeck_SeanPacesActions[];
extern const s32 TolbiShipDeck_OuranosPatrolsActions[];
extern const s32 TolbiShipDeck_HoverAndDiveAtOuranosActions[];
extern const s32 TolbiShipDeck_HoverAndDiveAtSeanActions[];
extern const s32 TolbiShipDeck_BobActions[];
extern const s32 TolbiShipDeck_LookAroundActions[];
extern const s32 TolbiShipDeck_SwellActions[];
extern const s32 TolbiShipDeck_MonsterGoesBelowActions[];
extern const s32 TolbiShipDeck_LeapAtOuranosActions[];
extern const s32 TolbiShipDeck_LeapAtSeanActions[];
extern const s32 TolbiShipDeck_HopActions[];
extern const s32 TolbiShipDeck_RowerLeavesActions[];
extern const s32 TolbiShipDeck_GoBelowActions[];
extern const s32 TolbiShipDeck_SeanLooksAboutActions[];
extern const SCENE_ENTRANCE TolbiShipDeckEntrances[];
extern const SCENE_EXIT_REGION TolbiShipDeckRegions[];
extern const u32 TolbiShipDeckExits[];
extern const ACTOR_PLACE TolbiShipDeckActorPlacements[];
#ifdef UBFIX
extern ACTOR_PLACE TolbiShipDeckActorPlacementsWaiting[];
#else
extern const ACTOR_PLACE TolbiShipDeckActorPlacementsWaiting[];
#endif
extern const ACTOR_PLACE TolbiShipDeckActorPlacementsRowing[];
extern const ACTOR_PLACE TolbiShipDeckActorPlacementsAtSea[];
extern const ACTOR_PLACE TolbiShipDeck_CrewActorPlacements[];
extern const ACTOR_PLACE TolbiShipDeck_TourGuideActorPlacements[];
extern const ACTOR_PLACE TolbiShipDeck_MonstersBoardActorPlacements[];
extern const ACTOR_PLACE TolbiShipDeck_SecondAttackActorPlacements[];
extern const ACTOR_PLACE TolbiShipDeck_ThirdAttackActorPlacements[];
extern const ACTOR_PLACE TolbiShipDeck_KrakenActorPlacements[];
extern const ACTOR_PLACE TolbiShipDeckActorPlacementsArrived[];
extern const TRIGGER TolbiShipDeckTriggers[];
extern const TRIGGER TolbiShipDeckTriggersAtSea[];
extern const TRIGGER TolbiShipDeckTriggersArrived[];
extern const TRIGGER TolbiShipDeckTriggersAtIsland[];
extern const s32 TolbiShipDeckRowerStrokes[];

static void scTolbiShipDeckConfrontCrewman(void);
static void scTolbiShipDeckApplyEntryState(void);
static void scTolbiShipDeckPlaceWaitingPassengers(void);
static void scTolbiShipDeckPlacePassengersAgreedToRow(void);
static void scTolbiShipDeckSetUpUnderSail(void);
static void scTolbiShipDeckSetUpAfterKraken(void);
static void scTolbiShipDeckSetUpAtAnchor(void);
static void scTolbiShipDeckDepartFromKalay(void);
static void scTolbiShipDeckDepartWithParty(void);
static void scTolbiShipDeckAnnouncePreparationsReady(s32 messenger,
													 s32 crewMember);
static void scTolbiShipDeckSightSeaMonsters(void);
static void scTolbiShipDeckMonstersBoard(void);
static void scTolbiShipDeckMonstersAttackCrew(void);
static void scTolbiShipDeckMonsterLandsOnDeck(void);
static void scTolbiShipDeckJumpIntoKrakenBattle(void);
static void scTolbiShipDeckThankForKraken(void);
static void scTolbiShipDeckSightLand(void);
static void scTolbiShipDeckRowersDisembark(void);
static s32 scTolbiShipDeckGetRowerAtSeat(u32 seat);
static void scTolbiShipDeckRockDeck(void);
static void scTolbiShipDeckSetUpOars(void);
static void scTolbiShipDeckShowOar(s32 actorId);
static void scTolbiShipDeckChooseOarStrokes(void);
static s32 scTolbiShipDeckGetSeatStroke(u32 seat);
static void scTolbiShipDeckRowOars(void);
static void scTolbiShipDeckAdvanceStroke(s32 actorId, s32 slot);
static void scTolbiShipDeckSwingOar(s32 id, s32 slot, s32 flags);

/* How scTolbiShipDeckSwingOar swings an oar: on the starboard side, mirrored;
 */
/* and at rest, at the angle it has rather than its stroke's. */
enum { OAR_SWING_MIRRORED = 1, OAR_SWING_AT_REST = 2 };

/* How hard a rower pulls, which sets how fast his oars go round: the oar left
 */
/* still, then ever faster. A seat no one rows still has its oars swing */
/* steadily. */
typedef enum OAR_STROKE {
	OAR_STROKE_STILL = 1,
	OAR_STROKE_SLOW,
	OAR_STROKE_STEADY,
	OAR_STROKE_FAST
} OAR_STROKE;

/* SPRITE_OAR's animations (oar.tsv): the dry blade in the air, the blade */
/* splashing in the water, and the blade dipping in with a splash. */
enum {
	OAR_ANIMATION_IN_AIR = 1,
	OAR_ANIMATION_IN_WATER = 2,
	OAR_ANIMATION_DIPPING = 3
};
static void scTolbiShipDeckRestOars(void);
static void scTolbiShipDeckCopyUnderwayCells(void);
static void scTolbiShipDeckCopyAnchoredCells(void);
static void scTolbiShipDeckSay(s32 speaker);
static void scTolbiShipDeckTurn(s32 actorId, s32 facing);
static void scTolbiShipDeckSightIsland(void);
static void scTolbiShipDeckReturnFromIsland(void);
static void scTolbiShipDeckPlaceLeaderOnDeck(void);

/* Who is who on deck, numbering each actor placement table's rows from */
/* ACTOR_FIRST_PLACED. */
/* */
/* Every table starts with the same twelve rows of oars, and the events add */
/* their actors after a table's last row (fdEventAppendActorPlacements). So */
/* each stage of the voyage has its own names, which carry its prefix. */
/* */
/* At Kalay (TolbiShipDeckActorPlacementsWaiting) the captain talks the */
/* delay over with Kaja, the deck's fighter, and a crewman, who asks Kaja how */
/* the oarsmen are ("Say, Kaja, how are the oarsmen doing?"). */
/* */
/* At sea TolbiShipDeck_CrewActorPlacements places Kaja and the warriors */
/* Ouranos and Sean, who help him guard the deck ("That'll be me, Ouranos, */
/* and Sean on deck"). */
typedef enum TOLBI_SHIP_DECK_ACTOR_ID {
	/* Four oars, with the same sprite as the others, that keep the strokes of
	 */
	/* TolbiShipDeckRowerStrokes. */
	/* */
	/* There is one for each OAR_STROKE, from the still oar to the fastest,
	 */
	/* drawn under the deck. Each oar at the sides copies the frames of its */
	/* seat's stroke */
	/* (scTolbiShipDeckSwingOar). At rest the first stands in for the first oar.
	 */
	ACTOR_FIRST_STROKE_OAR = ACTOR_FIRST_PLACED,
	/* The eight oars at the ship's sides (SPRITE_OAR). */
	/* */
	/* There are four at x 132, then four at x 300, which are drawn mirrored. */
	/* They stand out from the hull while the ship is rowed. */
	ACTOR_FIRST_OAR = ACTOR_FIRST_STROKE_OAR + 4,

	/* At Kalay: TolbiShipDeckActorPlacements, then */
	/* TolbiShipDeckActorPlacementsWaiting. */
	/* The crewman, a man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA), who */
	/* hid the captain's anchor charm on the mast and denies everything. */
	ACTOR_KALAY_CREWMAN = ACTOR_FIRST_OAR + 8,
	/* One of the crew, a man in a yellow bandana
	   (SPRITE_MAN_IN_YELLOW_BANDANA), */
	/* who answers the tour guide beside him. */
	/* */
	/* When the ship leaves he calls "Preparations... Ready!" */
	/* (MsgTolbiShipPreparationsReady). */
	ACTOR_KALAY_CREW_MEMBER,
	/* The captain, an old man with a white beard in green */
	/* (SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN). */
	ACTOR_KALAY_CAPTAIN,
	/* Kaja, a bald man with a beard (SPRITE_KAJA). */
	ACTOR_KALAY_KAJA,
	/* The tour group's guide, a man carrying a pack
	   (SPRITE_MAN_CARRYING_A_PACK) */
	/* as on the Tolbi Docks. */
	/* */
	/* Mind Read hears "I just hope we don't have to refund their money at the
	 */
	/* end of the trip." */
	ACTOR_KALAY_TOUR_GUIDE,
	/* Three of the tour group's passengers, named by what they say first. */
	/* */
	/* This one is a chef (SPRITE_CHEF). */
	ACTOR_KALAY_PASSENGER_WAITING_LONG,
	/* A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN). */
	ACTOR_KALAY_PASSENGER_SEEING_COLOSSO,
	/* A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN). */
	ACTOR_KALAY_PASSENGER_KARAGOL,

	/* At sea: TolbiShipDeck_CrewActorPlacements after the oars, then each */
	/* event's own rows. */
	ACTOR_SEA_KAJA = ACTOR_FIRST_OAR + 8,
	/* Ouranos, a young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR). */
	ACTOR_SEA_OURANOS,
	/* Sean, a young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR). */
	ACTOR_SEA_SEAN,
	/* With ACTOR_KALAY_CREW_MEMBER's sprite. */
	/* */
	/* At anchor, Mind Read hears "I want to be a sailor like Kaja". On later */
	/* departures he calls "Preparations... Ready!" */
	ACTOR_SEA_CREW_MEMBER,
	/* The first attack's three monsters, all of one sprite. */
	/* */
	/* They belong to TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD */
	/* (TolbiShipDeck_MonstersBoardActorPlacements). They leap aboard and go */
	/* below by the carved door at the north end, where */
	/* ShipRooms_FirstAttackMonsters places */
	/* them. */
	ACTOR_SEA_FIRST_ATTACK_MONSTER,
	/* Four monsters of one sprite that hover by the deck. */
	/* */
	/* They belong to TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK */
	/* (TolbiShipDeck_SecondAttackActorPlacements). The first two dive at Sean
	 */
	/* and the others at Ouranos. */
	ACTOR_SEA_FIRST_HOVERING_MONSTER = ACTOR_SEA_CREW_MEMBER + 1,
	/* Then the second attack's three come aboard and go below. */
	/* */
	/* Two are of the hovering monsters' sprite and one other, as */
	/* ShipRooms_SecondAttackMonsters places them. */
	ACTOR_SEA_SECOND_ATTACK_MONSTER = ACTOR_SEA_FIRST_HOVERING_MONSTER + 4,
	/* The splash that swells for 24 frames at the rail where the last of them
	 */
	/* leaps aboard out of the sea (SPRITE_SPLASH). */
	ACTOR_SEA_SPLASH = ACTOR_SEA_SECOND_ATTACK_MONSTER + 3,
	/* Four monsters with the first attack's sprite, that leap at Sean, the */
	/* first two, and at Ouranos. */
	/* */
	/* They belong to TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK */
	/* (TolbiShipDeck_ThirdAttackActorPlacements). */
	ACTOR_SEA_FIRST_LEAPING_MONSTER = ACTOR_SEA_CREW_MEMBER + 1,
	/* Then the third attack's three, which go below. */
	/* */
	/* Two have the first attack's sprite and one other, as */
	/* ShipRooms_ThirdAttackMonsters places them. The last drops onto the deck.
	 */
	ACTOR_SEA_THIRD_ATTACK_MONSTER = ACTOR_SEA_FIRST_LEAPING_MONSTER + 4,
	/* The falling monster's shadow, standing in for its own while it drops. */
	/* */
	/* A blank sprite (SPRITE_BLANK) draws only its shadow, which grows from a
	 */
	/* tenth of its size where the monster will land. It goes the moment the */
	/* monster lands and its own shadow is switched on. */
	ACTOR_SEA_FALLING_MONSTER_SHADOW = ACTOR_SEA_THIRD_ATTACK_MONSTER + 3,
	/* The Kraken, which the party charges. */
	/* */
	/* It belongs to TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN and at anchor */
	/* (TolbiShipDeck_KrakenActorPlacements, after Garet, Ivan and Mia in */
	/* their own slots). At anchor the same rows place it only for it to be */
	/* released at once. */
	ACTOR_SEA_KRAKEN = ACTOR_SEA_CREW_MEMBER + 1,
	/* An invisible actor (SPRITE_BLANK) the camera follows across the */
	/* deck. */
	ACTOR_INVISIBLE_SEA_CAMERA_TARGET,
	/* The tour guide, who comes up from below at anchor, back from the island
	 */
	/* (TolbiShipDeck_TourGuideActorPlacements). */
	/* */
	/* Kaja says "Tell our tourists they can enjoy a nice, long break after we
	 */
	/* arrive." */
	ACTOR_TOUR_GUIDE_AT_ANCHOR,

	/* At Tolbi: TolbiShipDeckActorPlacementsArrived. */
	ACTOR_ARRIVED_KAJA = ACTOR_FIRST_OAR + 8,
	/* A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA). */
	/* */
	/* Mind Read hears "I'm just going to hang out in Tolbi and enjoy myself for
	 */
	/* a while." */
	ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO,
	/* With ACTOR_KALAY_CREW_MEMBER's sprite. */
	ACTOR_ARRIVED_CREW_MEMBER,
	/* The first of the nine passengers who could promise to row. */
	/* */
	/* They come in the order of each seat's group of flags */
	/* (FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS and the others) and of */
	/* TolbiShipDeckRowerStrokes. They are the tour group's two guides, the */
	/* chef, the young woman in green, the bald, muscular man, the woman with a
	 */
	/* sword, the young man with brown hair, the woman with fair hair and an old
	 */
	/* man with a cane (SPRITE_OLD_MAN_WITH_CANE). */
	ACTOR_ARRIVED_FIRST_PASSENGER,
	/* An invisible actor (SPRITE_BLANK) the camera follows as the rowers */
	/* come up. */
	ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET = ACTOR_ARRIVED_FIRST_PASSENGER + 9
} TOLBI_SHIP_DECK_ACTOR_ID;

/* Sean or Ouranos lying on his back, arms flung out */
/* (young_man_with_blue_hair.png, young_man_with_red_hair.png), beyond enum */
/* ACTOR_ANIMATION. */
enum { ANIM_LYING_FLAT = 5 };

/* The cells of the deck at Kalay that start the scenes with the crew. The */
/* ways below are cells numbered as their exits instead */
/* (scTolbiShipDeckGoBelow). */
typedef enum TOLBI_SHIP_DECK_CELL_BEHAVIOR {
	/* The deck beside the stairway in the middle and south-east of it, cells */
	/* (12, 39) to (15, 46), where Isaac comes upon the crew talking over the */
	/* delay. */
	CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW = 10,
	/* Beside the door of the cabin at the south end, which the crewman slipped
	 */
	/* through: west of it, cells (12, 50) and (12, 51)... */
	CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_CABIN_DOOR = 11,
	/* ...and east of it, cells (14, 50) and (14, 51). */
	CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_CABIN_DOOR = 12
} TOLBI_SHIP_DECK_CELL_BEHAVIOR;

#include "shared/squash_and_stretch.c"

/* The door to the passengers' hold has been drawn open this visit. */
#define FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN FLAG_SCENE_TEMP_71
/* The door to the captain's quarters has been drawn open this visit. */
#define FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN FLAG_SCENE_TEMP_72

/* An actor script call that shows the actor's sprite. */
/* */
/* Returns 0 to end the call. */
static s32 scTolbiShipDeckShowSpr(ACTOR *a) {
	fdActorSetSprFlags(a, SPRITE_FLAG_SHADOW);
	return 0;
}

/* The diving scripts own the actor's scratch fields: sceneWord is */
/* sideways drift in 16.16 pixels per frame, sceneCnt the dive step */
/* (TOLBI_SHIP_DECK_DIVE_STEP), sceneByte the return side, timer the drift */
/* direction and */
/* userValue the bob direction. */

/* The steps of a hovering monster's dive (TOLBI_SHIP_DECK_HOVER_AND_DIVE), in
 */
/* sceneCnt. After each move is started one frame passes before the */
/* arrival is tested. */
typedef enum TOLBI_SHIP_DECK_DIVE_STEP {
	DIVE_STEP_HOVERING, /* drifting and bobbing on its side */
	DIVE_STEP_DIVE,		/* sets off at the warrior */
	DIVE_STEP_DIVE_STARTED,
	DIVE_STEP_DIVING, /* on arriving it cries, and the warrior turns and jumps
					   */
	DIVE_STEP_WARRIOR_JUMPED,
	DIVE_STEP_RETURN, /* sets off back to its side */
	DIVE_STEP_RETURN_STARTED,
	DIVE_STEP_RETURNING, /* on arriving it slows and stops drifting */
	DIVE_STEP_BACK
} TOLBI_SHIP_DECK_DIVE_STEP;

/* Defines `name`, the update of a sea monster hovering by one warrior's side */
/* of the deck while the monsters attack. */
/* */
/* It drifts between minX and maxX and bobs, at random. One frame in a hundred
 */
/* it dives at the warrior to (diveX, diveZ), 20 pixels up, and the warrior */
/* turns and jumps (sometimes with an emote). Then it returns to its side, */
/* (eastX, eastZ) or (westX, westZ) as sceneByte picks. */
/* */
/* Returns 1 to keep running. */
#define TOLBI_SHIP_DECK_HOVER_AND_DIVE(name, warrior, diveX, diveZ, eastX,     \
									   eastZ, westX, westZ, minX, maxX)        \
	static s32 name(ACTOR *actor) {                                            \
		if(actor->sceneCnt != DIVE_STEP_HOVERING) {                            \
			switch(actor->sceneCnt) {                                          \
			case DIVE_STEP_DIVE:                                               \
				actor->speedLimit = toFIXED(4);                                \
				actor->acceleration = toFIXED(2);                              \
				syFieldActorSetMoveTarget(actor, diveX, 20, diveZ);            \
				actor->sceneCnt++;                                             \
				break;                                                         \
			case DIVE_STEP_DIVE_STARTED:                                       \
				actor->sceneCnt++;                                             \
				break;                                                         \
			case DIVE_STEP_DIVING:                                             \
				if(actor->tgtX == ACTOR_NO_TARGET &&                           \
				   actor->tgtY == actor->tgtX && actor->tgtZ == actor->tgtY) { \
					actor->sceneCnt++;                                         \
					sySndPlay(SE_ENEMY_CRY);                                   \
					if(actor->sceneByte != 0)                                  \
						fdActorFaceDirection(warrior, FACING_NORTH_NORTHEAST,  \
											 0);                               \
					else                                                       \
						fdActorFaceDirection(warrior, FACING_NORTH_NORTHWEST,  \
											 0);                               \
					if(syRandomBelow(4) != 0) {                                \
						fdActorGet(warrior)->velY = toFIXED(2);                \
					}                                                          \
					else {                                                     \
						fdActorShowEmote(                                      \
							warrior, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);    \
						fdActorGet(warrior)->velY = toFIXED(6);                \
					}                                                          \
				}                                                              \
				break;                                                         \
			case DIVE_STEP_WARRIOR_JUMPED:                                     \
				actor->sceneCnt++;                                             \
				break;                                                         \
			case DIVE_STEP_RETURN:                                             \
				if(actor->sceneByte != 0)                                      \
					syFieldActorSetMoveTarget(actor, eastX, 0, eastZ);         \
				else                                                           \
					syFieldActorSetMoveTarget(actor, westX, 0, westZ);         \
				actor->sceneCnt++;                                             \
				break;                                                         \
			case DIVE_STEP_RETURN_STARTED:                                     \
				actor->sceneCnt++;                                             \
				break;                                                         \
			case DIVE_STEP_RETURNING:                                          \
				if(actor->tgtX == ACTOR_NO_TARGET &&                           \
				   actor->tgtY == actor->tgtX && actor->tgtZ == actor->tgtY) { \
					actor->speedLimit = toFIXED(2);                            \
					actor->acceleration = toFIXED(1);                          \
					actor->tmr = 0;                                            \
					actor->userValue = 0;                                      \
					actor->sceneCnt++;                                         \
					actor->sceneWord = 0;                                      \
				}                                                              \
				break;                                                         \
			case DIVE_STEP_BACK:                                               \
				actor->sceneCnt = DIVE_STEP_HOVERING;                          \
				break;                                                         \
			}                                                                  \
		}                                                                      \
		else {                                                                 \
			if(actor->tmr != 0) {                                              \
				actor->sceneWord -= syRandomBelow(4096);                       \
				if(actor->sceneWord < -toFIXED(1) / 4)                         \
					actor->tmr = 0;                                            \
			}                                                                  \
			else {                                                             \
				actor->sceneWord += syRandomBelow(4096);                       \
				if(actor->sceneWord > toFIXED(1) / 4)                          \
					actor->tmr = 1;                                            \
			}                                                                  \
			if(actor->x.fixed > toFIXED(minX) &&                               \
			   actor->x.fixed < toFIXED(maxX))                                 \
				actor->x.fixed += actor->sceneWord;                            \
			if(actor->userValue != 0) {                                        \
				actor->y.fixed =                                               \
					actor->y.fixed - (syRandomBelow(32768)) - toFIXED(1) / 2;  \
				if(actor->y.fixed < 0)                                         \
					actor->userValue = 0;                                      \
			}                                                                  \
			else {                                                             \
				actor->y.fixed =                                               \
					actor->y.fixed + (syRandomBelow(32768)) + toFIXED(1) / 2;  \
				if(actor->y.fixed > toFIXED(8))                                \
					actor->userValue = 1;                                      \
			}                                                                  \
		}                                                                      \
		if(syRandomBelow(100) == 0)                                            \
			actor->sceneCnt = DIVE_STEP_DIVE;                                  \
		return 1;                                                              \
	}

/* A monster hovering on Ouranos's side, diving at him. */
TOLBI_SHIP_DECK_HOVER_AND_DIVE(TolbiShipDeck_HoverAndDiveAtOuranos,
							   ACTOR_SEA_OURANOS, 268, 692, 282, 658, 254, 668,
							   248, 292)

/* A monster hovering on Sean's side, diving at him. */
TOLBI_SHIP_DECK_HOVER_AND_DIVE(TolbiShipDeck_HoverAndDiveAtSean, ACTOR_SEA_SEAN,
							   256, 640, 264, 600, 242, 604, 232, 272)

/* The four strokes' phases, which the oars at the sides follow (65,536 to a */
/* turn). */
static u16 sStrokePhase[4] ALIGNED(16);

/* The deck's rocking up and down (65,536 to a turn). */
static s32 sRockAngleY;
UNUSED static s32 sUnusedAfterRockY; /* Unused: nothing reads or writes it. */

/* Each oar's angle in its swing (65,536 to a turn). */
static u16 sOarAngle[8];

/* The sea layer's scroll across and down, 16.16 pixels within */
/* TOLBI_SHIP_DECK_SEA_WRAP. */
static s32 sSeaScroll[2];

/* The deck's rocking from side to side (65,536 to a turn). */
static s32 sRockAngleX;
UNUSED static s32 sUnusedAfterRockX; /* Unused: nothing reads or writes it. */

/* How fast the sea scrolls across and down, 16.16 pixels a frame. */
static s32 sSeaSpeed[2];

/* The eight oars' strokes (scTolbiShipDeckChooseOarStrokes) and the z each */
/* swings about, in 16.16. */
static u32 sOarStroke[8] ALIGNED(16);
static s32 sOarDepth[8];

/* Bobs the actor between 4 and 12 pixels high by a random amount */
/* each frame. */
static s32 scTolbiShipDeckBob(ACTOR *actor) {
	if(actor->userValue != 0) {
		actor->y.fixed -= (syRandomBelow(32768)) + toFIXED(1) / 2;
		if(actor->y.fixed < toFIXED(4)) {
			actor->userValue = 0;
		}
	}
	else {
		actor->y.fixed += (syRandomBelow(32768)) + toFIXED(1) / 2;
		if(actor->y.fixed > toFIXED(12)) {
			actor->userValue = 1;
		}
	}
	return 1;
}

/* Now and then (two chances in 32) turns the actor to look */
/* north-northeast or north-northwest. */
/* */
/* Returns 1. */
static s32 scTolbiShipDeckLookAround(ACTOR *actor) {
	u32 roll = syRandomBelow(32);

	if(roll == 6)
		actor->facing = FACING_NORTH_NORTHEAST;
	else if(roll == 9)
		actor->facing = FACING_NORTH_NORTHWEST;
	return 1;
}

/* The steps of a monster's leap on deck (TOLBI_SHIP_DECK_LEAP_AT), in timer. */
/* After each jump is started one frame passes before the landing is tested. */
typedef enum TOLBI_SHIP_DECK_LEAP_STEP {
	LEAP_STEP_WAITING, /* one frame in 40 it gets ready */
	LEAP_STEP_READY,
	LEAP_STEP_LEAP, /* jumps at the warrior */
	LEAP_STEP_LEAP_STARTED,
	LEAP_STEP_LEAPING, /* on landing the warrior turns and jumps */
	LEAP_STEP_WARRIOR_JUMPED,
	LEAP_STEP_HOP_AWAY, /* jumps to where it lands */
	LEAP_STEP_HOP_STARTED,
	LEAP_STEP_HOPPING,
	LEAP_STEP_LANDED
} TOLBI_SHIP_DECK_LEAP_STEP;

/* Defines `name`, the update of a monster on deck that, at random, leaps at */
/* a warrior. */
/* */
/* It jumps to him at (atX, atZ), he turns and jumps back (sometimes with */
/* an emote), and it lands. While sceneByte is set he turns to setFacing and */
/* it lands on (setX, setZ), else clearFacing and (clearX, clearZ). */
/* */
/* The actor's timer holds the step (TOLBI_SHIP_DECK_LEAP_STEP). Returns 1. */
#define TOLBI_SHIP_DECK_LEAP_AT(name, warrior, atX, atZ, setFacing,            \
								clearFacing, setX, setZ, clearX, clearZ)       \
	static s32 name(ACTOR *actor) {                                            \
		switch(actor->tmr) {                                                   \
		case LEAP_STEP_WAITING:                                                \
			if((syRandomBelow(40)) == 0) {                                     \
				actor->tmr++;                                                  \
			}                                                                  \
			break;                                                             \
		case LEAP_STEP_READY:                                                  \
			actor->tmr++;                                                      \
			break;                                                             \
		case LEAP_STEP_LEAP:                                                   \
			actor->velY = toFIXED(4);                                          \
			actor->speedLimit = toFIXED(4);                                    \
			actor->acceleration = toFIXED(2);                                  \
			syFieldActorSetMoveTarget(actor, atX, 0, atZ);                     \
			actor->tmr++;                                                      \
			break;                                                             \
		case LEAP_STEP_LEAP_STARTED:                                           \
			actor->tmr++;                                                      \
			break;                                                             \
		case LEAP_STEP_LEAPING:                                                \
			if(actor->tgtX == ACTOR_NO_TARGET && actor->tgtY == actor->tgtX && \
			   actor->tgtZ == actor->tgtY) {                                   \
				actor->tmr++;                                                  \
				sySndPlay(SE_JUMP);                                            \
				if(actor->sceneByte != 0) {                                    \
					fdActorFaceDirection(warrior, setFacing, 0);               \
				}                                                              \
				else {                                                         \
					fdActorFaceDirection(warrior, clearFacing, 0);             \
				}                                                              \
				if(syRandomBelow(4) != 0) {                                    \
					fdActorGet(warrior)->velY = toFIXED(2);                    \
				}                                                              \
				else {                                                         \
					fdActorShowEmote(warrior,                                  \
									 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);    \
					fdActorGet(warrior)->velY = toFIXED(6);                    \
				}                                                              \
			}                                                                  \
			break;                                                             \
		case LEAP_STEP_WARRIOR_JUMPED:                                         \
			actor->tmr++;                                                      \
			break;                                                             \
		case LEAP_STEP_HOP_AWAY:                                               \
			actor->tmr++;                                                      \
			actor->velY = toFIXED(4);                                          \
			actor->speedLimit = toFIXED(2);                                    \
			actor->acceleration = toFIXED(1);                                  \
			if(actor->sceneByte != 0) {                                        \
				syFieldActorSetMoveTarget(actor, setX, 0, setZ);               \
			}                                                                  \
			else {                                                             \
				syFieldActorSetMoveTarget(actor, clearX, 0, clearZ);           \
			}                                                                  \
			break;                                                             \
		case LEAP_STEP_HOP_STARTED:                                            \
			actor->tmr++;                                                      \
			break;                                                             \
		case LEAP_STEP_HOPPING:                                                \
			if(actor->tgtX == ACTOR_NO_TARGET && actor->tgtY == actor->tgtX && \
			   actor->tgtZ == actor->tgtY) {                                   \
				actor->tmr++;                                                  \
			}                                                                  \
			break;                                                             \
		case LEAP_STEP_LANDED:                                                 \
			actor->tmr = LEAP_STEP_WAITING;                                    \
			break;                                                             \
		}                                                                      \
		return 1;                                                              \
	}

/* A monster on deck leaping at Ouranos. */
TOLBI_SHIP_DECK_LEAP_AT(TolbiShipDeck_LeapAtOuranos, ACTOR_SEA_OURANOS, 264,
						662, FACING_NORTH_NORTHWEST, FACING_SOUTH_SOUTHWEST,
						252, 646, 256, 686)
/* A monster on deck leaping at Sean. */
TOLBI_SHIP_DECK_LEAP_AT(TolbiShipDeck_LeapAtSean, ACTOR_SEA_SEAN, 176, 696,
						FACING_NORTH_NORTHEAST, FACING_EAST, 184, 672, 202, 692)

/* Makes the actor hop at random, one frame in 40 on average. */
/* */
/* Returns 1. */
static s32 scTolbiShipDeckRandomlyStartActorHop(ACTOR *actor) {
	if((syRandomBelow(40)) == 0)
		actor->velY = toFIXED(4);
	return 1;
}

/* The update that shrinks an enlarged actor by 1/32 a frame until it is */
/* back to normal size. */
static void scTolbiShipDeckShrinkToNormalSize(ACTOR *actor) {
	if(actor->scaleX > FIXED_ONE) {
		actor->scaleX -= FIXED_ONE / 32;
		actor->scaleY -= FIXED_ONE / 32;
	}
}

/* Turns the actor east, northeast-by-north or southeast-by-south at */
/* random intervals of 80 to 160 frames. */
static s32 scTolbiShipDeckIdleTurn(ACTOR *actor) {
	if(actor->sceneCnt != 0) {
		actor->sceneCnt--;
	}
	else {
		u32 roll = syRandomBelow(300);

		if(roll > 200) {
			actor->facing = FACING_NORTH_NORTHEAST;
		}
		else if(roll > 100) {
			actor->facing = FACING_SOUTH_SOUTHWEST;
		}
		else {
			actor->facing = FACING_EAST;
		}
		actor->sceneCnt = (syRandomBelow(80)) + 80;
	}
	return 1;
}

const SCENE_ENTRANCE *scGetEntrances(void) { return TolbiShipDeckEntrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return TolbiShipDeckRegions; }

const u32 *scGetExits(void) { return TolbiShipDeckExits; }

/* The talk facing of a Kalay actor's row in */
/* TolbiShipDeckActorPlacementsWaiting. */
#define KALAY_TALK_FACING(actorId)                                             \
	STORE_CONST(                                                               \
		TolbiShipDeckActorPlacementsWaiting[(actorId) - ACTOR_FIRST_PLACED]    \
			.talkFacing)

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
		return TolbiShipDeckActorPlacementsArrived;
	if(gmFlagGet(FLAG_TOLBI_SHIP_ROWING))
		return TolbiShipDeckActorPlacementsRowing;
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF))
		return TolbiShipDeckActorPlacementsAtSea;
	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
		if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
			KALAY_TALK_FACING(ACTOR_KALAY_CREW_MEMBER) = TALK_FACING_PARTY;
			KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_WAITING_LONG) =
				TALK_FACING_KEEP;
			KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO) =
				TALK_FACING_KEEP;
		}
		else if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN)) {
			KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_WAITING_LONG) =
				TALK_FACING_PARTY_AND_BACK;
			KALAY_TALK_FACING(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO) =
				TALK_FACING_PARTY_AND_BACK;
		}
		return TolbiShipDeckActorPlacementsWaiting;
	}
	return TolbiShipDeckActorPlacements;
}

#undef KALAY_TALK_FACING

const TRIGGER *scGetTriggers(void) {
	if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
		return TolbiShipDeckTriggersArrived;
	if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND))
		return TolbiShipDeckTriggersAtIsland;
	if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF))
		return TolbiShipDeckTriggersAtSea;
	return TolbiShipDeckTriggers;
}

/* At Kalay, the sailor in a yellow bandana beside the tour guide, south-east */
/* on deck, snaps that they will sail when the replacement ship arrives. */
/* */
/* While the two warriors are off at the captain's quarters he frets */
/* `Somebody stop them...`, and once the anchor charm is back he says they */
/* ought to leave soon, though the passengers may not want to row. */
static void scTolbiShipDeckTalkToCrewMember(void) {
	ACTOR *crewMember;
	s32 frames;

	fdEventStart();
	if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		fdEventSetMessageId(MsgTolbiShipNowWeHaveProtectShip);
		/* "Now that we have you to protect the ship, we ought to be..." */
		fdEventShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
	}
	else {
		if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
			fdActorRunRepeatedMotion(ACTOR_KALAY_CREW_MEMBER, 2);
			fdEventSetMessageId(MsgTolbiShipSomebodyStopThem);
			/* "Somebody stop them..." */
			fdEventShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
			crewMember = fdActorGet(ACTOR_KALAY_CREW_MEMBER);
			frames = syRandomBelow(90) + 60;
			crewMember->tmr = frames;
			fdActorSetScript(ACTOR_KALAY_CREW_MEMBER,
							 TolbiShipDeck_SquashAndStretchActions);
		}
		else {
			fdActorShowEmote(ACTOR_KALAY_CREW_MEMBER,
							 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
			fdActorStartRepeatedMotion(ACTOR_KALAY_CREW_MEMBER, 3);
			fdEventSetMessageId(MsgTolbiShipToldWereLeavingSoonSet);
			/* "I told you, we're leaving soon. We'll set sail when our..." */
			fdEventShowMessage(ACTOR_KALAY_CREW_MEMBER, 0);
		}
	}
	fdEventEnd();
}

/* At Kalay, the tour guide with a pack, south-east on deck, complains that */
/* Colosso will start without his group. */
/* */
/* While the two warriors are off at the captain's quarters he frets that */
/* they can't be planning a mutiny, and once the anchor charm is back he says */
/* the passengers will have to promise to row. */
static void scTolbiShipDeckTalkToTourGuide(void) {
	ACTOR *guide;
	s32 frames;

	fdEventStart();
	if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		fdEventSetMessageId(MsgTolbiShipHaveMakeThemPromiseHelp);
		/* "We'll have to make them promise to help row before we can..." */
		fdEventShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
	}
	else {
		if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
			fdActorRunRepeatedMotion(ACTOR_KALAY_TOUR_GUIDE, 2);
			fdEventSetMessageId(MsgTolbiShipTheyCantPlanningMutiny);
			/* "They can't be planning a mutiny..." */
			fdEventShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
			guide = fdActorGet(ACTOR_KALAY_TOUR_GUIDE);
			frames = syRandomBelow(90) + 60;
			guide->tmr = frames;
			fdActorSetScript(ACTOR_KALAY_TOUR_GUIDE,
							 TolbiShipDeck_SquashAndStretchActions);
		}
		else {
			fdActorShowEmote(ACTOR_KALAY_TOUR_GUIDE,
							 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
			fdActorStartRepeatedMotion(ACTOR_KALAY_TOUR_GUIDE, 3);
			fdEventSetMessageId(MsgTolbiShipIfWeDontLeaveSoon);
			/* "If we don't leave soon, Colosso will start without us! Will..."
			 */
			fdEventShowMessage(ACTOR_KALAY_TOUR_GUIDE, 0);
		}
	}
	fdEventEnd();
}

/* At Tolbi, the sailor in a blue bandana by the gangplank, on the west side */
/* of the deck, asks whether Isaac is headed for Colosso too. */
/* */
/* Yes, and he says most people are at this time of year; no, and he calls */
/* Isaac strange, like the group of six they ferried across a while back. */
static void scTolbiShipDeckAskHeadedForColosso(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiShipHeadedColosso);
	/* "Are you headed for Colosso, too?" */
	/* Yes: "I thought so. Most people coming at this time of year..." */
	/* No: "You came all this way, and you're not going to Colosso? That..." */
	fdEventAskYesNo(ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, 0);
	fdEventEnd();
}

/* The carved door up the ladder at the north end of the deck swings open, */
/* once a visit. */
static void scTolbiShipDeckOpenFirstDoor(void) {
	if(fdEventGetFlag(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN) == 0) {
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridCopyCellsTo(30, 94, 13, 94, 1, 3);
		gmFlagSet(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN);
	}
}

/* The door of the cabin at the south end of the deck swings open, once a */
/* visit. */
static void scTolbiShipDeckOpenSecondDoor(void) {
	if(fdEventGetFlag(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN) == 0) {
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridCopyCellsTo(30, 108, 13, 108, 1, 2);
		gmFlagSet(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN);
	}
}

/* Isaac goes below: through the carved door up the ladder at the north end */
/* of the deck (cell (13, 37)), down the small stairway in the */
/* middle of the deck, walking west (cell (13, 41)), or through the door of */
/* the cabin at the south end (cell (13, 50)), the captain's quarters. */
/* */
/* A door swings open and he walks into it; the stairway plays the exit sound.
 */
/* Each cell's behaviour is its exit's number, which the trigger rows name. */
static void scTolbiShipDeckGoBelow(void) {
	s32 throughDoor;
	FIELD_STATE *state;

	state = FieldStateWork;
	fdEventStart();
	throughDoor = 0;
	switch(state->stepTrigger) {
	case TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD:
		throughDoor = 1;
		scTolbiShipDeckOpenFirstDoor();
		break;
	case TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM:
		throughDoor = 1;
		scTolbiShipDeckOpenSecondDoor();
		break;
	}
	if(throughDoor != 0) {
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 1, -10);
		fdEventWait(10);
	}
	else {
		sySndPlay(SE_MAP_EXIT);
	}
	syImportEventSetExitRequest(state->stepTrigger);
}

#if TBS_PATCH_2
/* In the German, Spanish, French and Italian editions, Isaac goes down the */
/* small stairway in the middle of the deck (cell (13, 41)) as soon as he steps
 */
/* onto it facing west, walked to its middle first; elsewhere he has to keep */
/* walking west on it. */
/* */
/* There the stairway is a plain step trigger, and this checks his facing. */
static void scTolbiShipDeckGoBelowWhileFacingWest(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader->facing > FACING_SOUTHWEST && leader->facing < FACING_NORTHWEST) {
		fdEventStart();
		fdActorWalkTo(ACTOR_PARTY_LEADER, 214, 664);
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		scTolbiShipDeckGoBelow();
	}
}
#endif

/* At Kalay, once the tour group has gone, walking past the stairway in the */
/* middle of the deck brings the camera round to the captain, Kaja and the */
/* crewman in a blue bandana talking over the late replacement ship. */
/* */
/* The captain will sail trusting his anchor charm. Kaja goes off, the captain
 */
/* steps aside, and the crewman slips through the door of the cabin at the */
/* south end. */
static void scTolbiShipDeckWatchCrewDecideToSail(void) {
	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
		fdEventStart();
		fdEndBusyAndReveal();
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_KALAY_CREWMAN, 10);
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(190, CAMERA_KEEP, 708, 1);
		fdCameraWaitForMove();
		fdEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
		fdEventSetMessageId(MsgTolbiShipWonderCouldHaveHappened);
		/* "I wonder what could have happened." */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		fdActorShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
		/* "You mean you still intend to cross this dangerous sea?" */
		scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
		fdActorRunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
		fdActorFaceDirection(ACTOR_KALAY_CAPTAIN, FACING_SOUTH_SOUTHWEST, 0);
		/* "It would be too big a loss to let so many passengers get away..." */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		fdActorRunRepeatedMotion(ACTOR_KALAY_CREWMAN, 1);
		scTolbiShipDeckTurn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHWEST);
		/* "Say, Kaja, how are the oarsmen doing?" */
		scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
		scTolbiShipDeckTurn(ACTOR_KALAY_KAJA, FACING_SOUTH_SOUTHEAST);
		fdActorSetAnimation(ACTOR_KALAY_KAJA, ANIM_NOD);
		/* "They're fearless. There's no problem there. It's just that..." */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
		fdActorShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 40);
		fdActorFaceDirection(ACTOR_KALAY_CAPTAIN, FACING_WEST, 20);
		/* ""It's just that"...what?" */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		scTolbiShipDeckTurn(ACTOR_KALAY_KAJA, FACING_EAST);
		fdActorSetAnimationAndWait(ACTOR_KALAY_KAJA, ANIM_SHAKE_HEAD);
		/* "If a whole bunch of...them appear while we're crossing the sea, I
		 * won't */
		/* be able to handle them alone." */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
		fdActorShowEmote(ACTOR_KALAY_CREWMAN,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
		/* "I'm sorry, but you'll have to. I can't let anyone else fight but
		 * you, */
		/* Kaja." */
		scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
		fdActorSetAnimationAndWait(ACTOR_KALAY_CAPTAIN, ANIM_NOD);
		/* "Remember, the oarsmen will have their hands full rowing. They can't
		 */
		/* fight." */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		scTolbiShipDeckTurn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST);
		fdActorSetAnimation(ACTOR_KALAY_KAJA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_KALAY_CREWMAN, ANIM_NOD);
		fdEventWait(60);
		fdActorShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
						 40);
		scTolbiShipDeckTurn(ACTOR_KALAY_CAPTAIN, FACING_SOUTH_SOUTHWEST);
		fdEventSetMessageId(MsgTolbiShipTheresNothingWeCanDo);
		fdActorStartRepeatedMotion(ACTOR_KALAY_CAPTAIN, 1);
		/* "There's nothing we can do... Let's set sail!" */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		fdActorShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 40);
		fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
		/* "Wha-What are you talking about! If we set sail now, we'll never make
		 * it */
		/* to Tolbi." */
		scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
		fdActorShowEmote(ACTOR_KALAY_CAPTAIN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
		/* "We'll be fine... We have a protective amulet!" */
		fdEventShowMessageThenWait(
			SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE), 0, 20);
		fdActorShowEmote(ACTOR_KALAY_KAJA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		/* "Amulet... Amulet, you say... You mean that little anchor charm?" */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_KAJA, SPEAKER_WINDOW_ABOVE));
		scTolbiShipDeckTurn(ACTOR_KALAY_CAPTAIN, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_KALAY_CAPTAIN, ANIM_NOD);
		/* "The little anchor has always protected us from danger." */
		fdEventShowMessageThenWait(
			SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE), 0, 20);
		fdActorShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
		/* "Captain, get ahold of yourself! That's just foolish superstition!"
		 */
		scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
		scTolbiShipDeckTurn(ACTOR_KALAY_CAPTAIN, FACING_SOUTH_SOUTHWEST);
		fdActorSetAnimation(ACTOR_KALAY_CAPTAIN, ANIM_SHAKE_HEAD);
		/* "It's okay. We'll be fine. Even if we can't find any mercenaries,
		 * that */
		/* little charm will protect us." */
		scTolbiShipDeckSay(ACTOR_KALAY_CAPTAIN);
		fdActorFaceDirection(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_KALAY_KAJA, FACING_SOUTH_SOUTHEAST, 40);
		fdActorFaceDirection(ACTOR_KALAY_KAJA, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST, 20);
		fdActorRunRepeatedMotion(ACTOR_KALAY_CAPTAIN, 2);
		fdEventWait(20);
		/* "Now then, let's hurry up! Prepare to set sail!" */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		fdActorSetAttachedEffect(ACTOR_KALAY_KAJA, ATTACHED_EFFECT_IN_FRONT |
													   ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(ACTOR_KALAY_CREWMAN, ATTACHED_EFFECT_IN_FRONT |
														  ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_KALAY_CAPTAIN, ANIM_NOD);
		/* "This is great... We'll be setting sail soon!" */
		scTolbiShipDeckSay(SPEAKER(ACTOR_KALAY_CAPTAIN, SPEAKER_WINDOW_ABOVE));
		fdCameraSetSpeed(0.8);
		fdCameraMoveTo(182, CAMERA_KEEP, 760, 1);
		fdActorSetSpeed(ACTOR_KALAY_KAJA, 0.8);
		fdActorSetScript(ACTOR_KALAY_KAJA, TolbiShipDeck_KajaLeavesActions);
		fdActorSetSpeed(ACTOR_KALAY_CAPTAIN, 0.8);
		fdActorSetScript(ACTOR_KALAY_CAPTAIN,
						 TolbiShipDeck_CaptainStepsAsideActions);
		fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 0.8);
		fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 182, 760);
		fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
		fdActorShowEmote(ACTOR_KALAY_CREWMAN,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		scTolbiShipDeckTurn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST);
		/* "(Not if I have anything to say about it.)" */
		fdEventShowMessageThenWait(ACTOR_KALAY_CREWMAN, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_KALAY_CREWMAN, ANIM_NOD);
		fdActorJump(ACTOR_KALAY_CREWMAN, 4, 0);
		fdActorFaceDirection(ACTOR_KALAY_CREWMAN, FACING_SOUTH_SOUTHEAST, 40);
		fdCameraSetSpeed(1);
		fdCameraMoveTo(216, CAMERA_KEEP, 790, 1);
		fdActorGet(ACTOR_KALAY_CREWMAN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
		fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 182, 782);
		fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 192, 808);
		fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 808);
		scTolbiShipDeckTurn(ACTOR_KALAY_CREWMAN, FACING_NORTH_NORTHEAST);
		fdActorRunRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
		scTolbiShipDeckOpenSecondDoor();
		fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 798);
		fdActorSetPos(ACTOR_KALAY_CREWMAN, 0, 0);
		fdActorGet(ACTOR_KALAY_CREWMAN)->scaleX = FIXED_ONE;
		fdActorGet(ACTOR_KALAY_CREWMAN)->scaleY = FIXED_ONE;
		gmFlagSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
		fdEventEnd();
	}
}

/* At Kalay, when Isaac comes up to the door of the cabin at the south end */
/* from the west (cells (12, 50) and (12, 51)), the crewman in a blue bandana */
/* bursts out, says `I didn't do anything...`, sidles away east and runs off */
/* up that side of the deck. */
/* */
/* He is next found at the far north end of the deck. */
static void scTolbiShipDeckCatchCrewmanFromWest(void) {
	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
		if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
			fdEventStart();
			fdEndBusyAndReveal();
			scTolbiShipDeckConfrontCrewman();
			fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 0.4);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 232, 816);
			fdEventWait(1);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg |= ACTOR_FACES_MOTION;
			fdEventWait(20);
			fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
			/* "Really, I mean it..." */
			scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
			fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 244, 804);
			fdEventWait(1);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg |= ACTOR_FACES_MOTION;
			fdEventWait(20);
			fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 3.2);
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 248, 778);
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 248, 700);
			fdActorSetPos(ACTOR_KALAY_CREWMAN, 246, 512);
			fdActorFaceDirection(ACTOR_KALAY_CREWMAN, FACING_EAST, 0);
			fdActorShowEmote(ACTOR_PARTY_LEADER,
							 EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
			fdEventEnd();
		}
	}
}

/* At Kalay, when Isaac comes up to the door of the cabin at the south end */
/* from the east (cells (14, 50) and (14, 51)), the crewman in a blue bandana */
/* bursts out, says `I didn't do anything...`, sidles away west and runs off */
/* up that side of the deck. */
/* */
/* He is next found at the far north end of the deck, as when caught from the */
/* west. */
static void scTolbiShipDeckCatchCrewmanFromEast(void) {
	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
		if(gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
			fdEventStart();
			fdEndBusyAndReveal();
			scTolbiShipDeckConfrontCrewman();
			fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 0.4);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 202, 816);
			fdEventWait(1);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg |= ACTOR_FACES_MOTION;
			fdEventWait(20);
			fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
			/* "Really, I mean it..." */
			scTolbiShipDeckSay(ACTOR_KALAY_CREWMAN);
			fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 192, 804);
			fdEventWait(1);
			fdActorGet(ACTOR_KALAY_CREWMAN)->steeringFlg |= ACTOR_FACES_MOTION;
			fdEventWait(20);
			fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 3.2);
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 180, 778);
			fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 180, 700);
			fdActorSetPos(ACTOR_KALAY_CREWMAN, 246, 512);
			fdActorFaceDirection(ACTOR_KALAY_CREWMAN, FACING_EAST, 0);
			fdActorShowEmote(ACTOR_PARTY_LEADER,
							 EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
			fdEventEnd();
		}
	}
}

/* The camera moves down to the cabin door, which opens, and the crewman steps
 */
/* out, shakes his head and starts at the sight of Isaac. Both ways of */
/* catching him start this way. */
static void scTolbiShipDeckConfrontCrewman(void) {
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(216, CAMERA_KEEP, 824, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	scTolbiShipDeckOpenSecondDoor();
	fdMapGridCopyCellsTo(30, 108, 13, 108, 1, 2);
	fdEventWait(10);
	fdActorSetPos(ACTOR_KALAY_CREWMAN, 216, 800);
	fdActorSetSpeed(ACTOR_KALAY_CREWMAN, 1.2);
	fdActorWalkToAndWait(ACTOR_KALAY_CREWMAN, 216, 814);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_KALAY_CREWMAN, 10);
	fdActorSetAnimationAndWait(ACTOR_KALAY_CREWMAN, ANIM_SHAKE_HEAD);
	fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
	fdActorShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 20);
	fdActorFaceActor(ACTOR_KALAY_CREWMAN, ACTOR_PARTY_LEADER, 20);
	fdActorStartRepeatedMotion(ACTOR_KALAY_CREWMAN, 2);
	fdEventSetMessageId(MsgTolbiShipDidntDoAnything);
	/* "I didn't do anything..." */
	fdEventShowMessageThenWait(ACTOR_KALAY_CREWMAN, 0, 20);
	fdActorShowEmote(ACTOR_KALAY_CREWMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	gmFlagSet(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT);
}

/* Boards the deck: the view centred, the sea still, the rocking's angles */
/* drawn at random, and then the voyage's stage and event set up */
/* (scTolbiShipDeckApplyEntryState). */
s32 scInit(void) {
	MAP_GRID_LAYER_SCROLL *sea = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])
									  ->layers[MAPGRID_LAYER_GROUND];

	gmFlagClear(FLAG_WORLD_MAP_LEFT);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	sea->speedY = 0;
	sRockAngleX = (u16)syRandomNext();
	sRockAngleY = (u16)syRandomNext();
	sSeaScroll[0] = 0;
	sSeaScroll[1] = 0;
	sSeaSpeed[0] = 0;
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	scTolbiShipDeckApplyEntryState();
	return 0;
}

/* Redraws the deck for the voyage's stage, starts the oars, the rocking and */
/* the sea as the ship moves, plays the event the entrance names, and */
/* otherwise places the people on deck for the stage. */
static void scTolbiShipDeckApplyEntryState(void) {
	s32 flag;

	gmFlagSet(FLAG_RETREAT_BLOCKED);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		gmFlagClear(FLAG_TOLBI_SHIP_DECK_PASSENGER_HOLD_DOOR_OPEN);
		gmFlagClear(FLAG_TOLBI_SHIP_DECK_CAPTAINS_QUARTERS_DOOR_OPEN);
	}
	if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
		scTolbiShipDeckCopyUnderwayCells();
		scTolbiShipDeckCopyAnchoredCells();
		fdActorSetSprColById(ACTOR_ARRIVED_FIRST_PASSENGER + 1,
							 SPRITE_COLOR_RBG);
	}
	else if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
		scTolbiShipDeckCopyUnderwayCells();
		scTolbiShipDeckCopyAnchoredCells();
	}
	else if((flag = gmFlagGet(FLAG_TOLBI_SHIP_ROWING)) != 0) {
		scTolbiShipDeckCopyUnderwayCells();
		scTolbiShipDeckSetUpOars();
		syTaskAddFrame(scTolbiShipDeckRowOars, TASK_PASS_FRAME);
		sSeaScroll[1] = toFIXED(32);
		sSeaSpeed[1] = toFIXED(6) / 5;
		syTaskAddFrame(scTolbiShipDeckRockDeck, TASK_PASS_FRAME);
	}
	else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
		scTolbiShipDeckCopyUnderwayCells();
		sSeaScroll[1] = flag;
		sSeaSpeed[1] = flag;
		syTaskAddFrame(scTolbiShipDeckRockDeck, TASK_PASS_FRAME);
	}
	if(gmFlagGet(FLAG_TOLBI_SHIP_ROWING) == 0) {
		scTolbiShipDeckRestOars();
	}
	switch(MainRoutineStateWork.entrance) {
	case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST:
		fdActorGet(ACTOR_PARTY_LEADER)->spr->image.pri = 1;
		break;
	case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_READY:
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			scTolbiShipDeckDepartWithParty();
		}
		else {
			scTolbiShipDeckDepartFromKalay();
		}
		return;
	case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_SEA_MONSTERS:
		scTolbiShipDeckSightSeaMonsters();
		return;
	case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD:
		scTolbiShipDeckMonstersBoard();
		return;
	case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK:
		scTolbiShipDeckMonstersAttackCrew();
		return;
	case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK:
		scTolbiShipDeckMonsterLandsOnDeck();
		return;
	case TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_KRAKEN:
		scTolbiShipDeckJumpIntoKrakenBattle();
		return;
	case TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			scTolbiShipDeckSetUpAfterKraken();
		}
		else {
			scTolbiShipDeckThankForKraken();
		}
		return;
	case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED:
		scTolbiShipDeckSightLand();
		return;
	case TOLBI_SHIP_DECK_FROM_WORLD_MAP:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scTolbiShipDeckRowersDisembark();
		}
		return;
	case TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE:
		scTolbiShipDeckSightIsland();
		return;
	}
	if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->minX = toFIXED(65);
	}
	else if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
		scTolbiShipDeckSetUpAtAnchor();
	}
	else if(gmFlagGet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
		scTolbiShipDeckSetUpAfterKraken();
	}
	else if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
		scTolbiShipDeckSetUpUnderSail();
	}
	else if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
		scTolbiShipDeckPlacePassengersAgreedToRow();
	}
	else if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
		scTolbiShipDeckPlaceWaitingPassengers();
	}
}

/* At Kalay while the ship waits, places the passengers by what has happened */
/* so far. */
static void scTolbiShipDeckPlaceWaitingPassengers(void) {
	s32 flag;

	fdActorSetSprPriority(ACTOR_KALAY_PASSENGER_KARAGOL, 1);
	fdActorSetSprPriority(ACTOR_KALAY_KAJA, 1);
	fdActorSetSprPriority(ACTOR_KALAY_CAPTAIN, 1);
	fdActorSetSprPriority(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 1);
	fdActorSetSprPriority(ACTOR_KALAY_TOUR_GUIDE, 1);
	if(gmFlagGet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY) != 0) {
		fdActorSetPos(ACTOR_KALAY_CAPTAIN, 162, 666);
		fdActorGet(ACTOR_KALAY_CAPTAIN)->facing = FACING_WEST;
		fdActorSetPos(ACTOR_KALAY_KAJA, 0, 0);
		fdActorSetPos(ACTOR_KALAY_CREWMAN, 0, 0);
	}
	flag = gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
	if(flag != 0) {
		fdActorSetPos(ACTOR_KALAY_CREW_MEMBER, 264, 702);
		fdActorGet(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_SOUTH_SOUTHWEST;
		fdActorGet(ACTOR_KALAY_CREW_MEMBER)->tmr = SQUASH_AND_STRETCH_PAUSE();
		fdActorSetScript(ACTOR_KALAY_CREW_MEMBER,
						 TolbiShipDeck_SquashAndStretchActions);
		fdActorSetPos(ACTOR_KALAY_TOUR_GUIDE, 248, 680);
		fdActorGet(ACTOR_KALAY_TOUR_GUIDE)->tmr = SQUASH_AND_STRETCH_PAUSE();
		fdActorSetScript(ACTOR_KALAY_TOUR_GUIDE,
						 TolbiShipDeck_SquashAndStretchActions);
		fdActorSetPos(ACTOR_KALAY_CAPTAIN, 0, 0);
	}
	else {
		if(gmFlagGet(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0) {
			fdActorSetPos(ACTOR_KALAY_CREWMAN, 246, 512);
			fdActorGet(ACTOR_KALAY_CREWMAN)->facing = flag;
		}
	}
}

/* At Kalay once the anchor charm is back */
/* (FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED), moves the people on deck. */
static void scTolbiShipDeckPlacePassengersAgreedToRow(void) {
	fdActorSetPos(ACTOR_KALAY_CREW_MEMBER, 262, 706);
	fdActorGet(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_SOUTH_SOUTHWEST;
	fdActorSetPos(ACTOR_KALAY_TOUR_GUIDE, 164, 648);
	fdActorGet(ACTOR_KALAY_TOUR_GUIDE)->facing = FACING_EAST;
	fdActorSetSprPriority(ACTOR_KALAY_TOUR_GUIDE, 1);
	fdActorSetPos(ACTOR_KALAY_PASSENGER_WAITING_LONG, 198, 665);
	fdActorGet(ACTOR_KALAY_PASSENGER_WAITING_LONG)->facing = FACING_WEST;
	fdActorSetSprPriority(ACTOR_KALAY_PASSENGER_WAITING_LONG, 1);
	fdActorSetPos(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 188, 678);
	fdActorGet(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO)->facing =
		FACING_NORTH_NORTHWEST;
	fdActorSetPos(ACTOR_KALAY_PASSENGER_KARAGOL, 186, 635);
	fdActorGet(ACTOR_KALAY_PASSENGER_KARAGOL)->facing = FACING_SOUTH_SOUTHWEST;
	fdActorSetPos(ACTOR_KALAY_CAPTAIN, 0, 0);
	fdActorSetPos(ACTOR_KALAY_KAJA, 0, 0);
	fdActorSetPos(ACTOR_KALAY_CREWMAN, 0, 0);
}

/* Under sail, the crew take their places, the two lookouts start their rounds
 */
/* and, when the party keeps its position, the leader stands on deck. */
static void scTolbiShipDeckSetUpUnderSail(void) {
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_KAJA, 0, 0);
	fdActorSetPos(ACTOR_SEA_CREW_MEMBER, 238, 626);
	fdActorSetPos(ACTOR_SEA_SEAN, 204, 521);
	fdActorGet(ACTOR_SEA_SEAN)->y.fixed = toFIXED(16);
	fdActorGet(ACTOR_SEA_SEAN)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
	fdActorSetSpeed(ACTOR_SEA_SEAN, 0.6);
	fdActorSetScript(ACTOR_SEA_SEAN, TolbiShipDeck_SeanPacesActions);
	fdActorGet(ACTOR_SEA_OURANOS)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 0.8);
	fdActorSetScript(ACTOR_SEA_OURANOS, TolbiShipDeck_OuranosPatrolsActions);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		scTolbiShipDeckPlaceLeaderOnDeck();
	}
	fdEventEnd();
}

/* After the Kraken, the deck is as under sail, with Sean at the helm. */
static void scTolbiShipDeckSetUpAfterKraken(void) {
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_KAJA, 0, 0);
	fdActorSetPos(ACTOR_SEA_CREW_MEMBER, 238, 626);
	fdActorSetPos(ACTOR_SEA_SEAN, 268, 678);
	fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_EAST;
	fdActorSetScript(ACTOR_SEA_SEAN, TolbiShipDeck_SeanLooksAboutActions);
	fdActorGet(ACTOR_SEA_OURANOS)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 0.8);
	fdActorSetScript(ACTOR_SEA_OURANOS, TolbiShipDeck_OuranosPatrolsActions);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		scTolbiShipDeckPlaceLeaderOnDeck();
	}
	fdEventEnd();
}

/* At anchor off the island, the crew stand where they were left, by whether */
/* the party has agreed to scout the island. */
static void scTolbiShipDeckSetUpAtAnchor(void) {
	((MAP_GRID_SCROLL_STATE *)MapGridState[0])->minX = toFIXED(65);
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_KrakenActorPlacements);
	sySynch(1);
	fdActorReleaseById(ACTOR_SEA_KRAKEN);
	fdActorSetPos(ACTOR_SEA_CREW_MEMBER, 238, 626);
	fdActorGet(ACTOR_SEA_CREW_MEMBER)->facing = FACING_SOUTH_SOUTHEAST;
	if(gmFlagGet(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED) != 0) {
		fdActorSetPos(ACTOR_SEA_SEAN, 162, 634);
		fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_SOUTH_SOUTHEAST;
		fdActorSetPos(ACTOR_SEA_OURANOS, 162, 676);
		fdActorGet(ACTOR_SEA_OURANOS)->facing = FACING_NORTH_NORTHEAST;
	}
	else {
		fdActorSetPos(ACTOR_SEA_SEAN, 160, 652);
		fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_SOUTH_SOUTHEAST;
		fdActorSetPos(ACTOR_SEA_OURANOS, 166, 668);
		fdActorGet(ACTOR_SEA_OURANOS)->facing = FACING_NORTH_NORTHWEST;
	}
	if(MainRoutineStateWork.entrance == TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS) {
		scTolbiShipDeckReturnFromIsland();
	}
	fdEventEnd();
}

/* Setting sail from Kalay before the party is on deck, Kaja comes up through */
/* the carved door at the north end and tells the sailor in a yellow bandana */
/* that all is ready. */
static void scTolbiShipDeckDepartFromKalay(void) {
	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_KALAY_CREWMAN, 0, 0);
	fdActorSetPos(ACTOR_KALAY_CAPTAIN, 0, 0);
	fdActorSetPos(ACTOR_KALAY_TOUR_GUIDE, 0, 0);
	fdActorSetPos(ACTOR_KALAY_PASSENGER_WAITING_LONG, 0, 0);
	fdActorSetPos(ACTOR_KALAY_PASSENGER_SEEING_COLOSSO, 0, 0);
	fdActorSetPos(ACTOR_KALAY_PASSENGER_KARAGOL, 0, 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_KALAY_KAJA, 0, 0);
	fdActorGet(ACTOR_KALAY_KAJA)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorSetPos(ACTOR_KALAY_CREW_MEMBER, 232, 650);
	fdActorGet(ACTOR_KALAY_CREW_MEMBER)->facing = FACING_NORTH_NORTHWEST;
	fdCameraMoveTo(232, CAMERA_KEEP, 636, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	scTolbiShipDeckAnnouncePreparationsReady(ACTOR_KALAY_KAJA,
											 ACTOR_KALAY_CREW_MEMBER);
}

/* On later departures, with the party aboard, Kaja comes up through the */
/* carved door at the north end and tells the sailor in a yellow bandana that */
/* all is ready. */
static void scTolbiShipDeckDepartWithParty(void) {
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 232, 636);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	sySynch(1);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorStop(ACTOR_SEA_SEAN);
	fdActorStop(ACTOR_SEA_OURANOS);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_SEAN, 0, 0);
	fdActorSetPos(ACTOR_SEA_OURANOS, 0, 0);
	fdActorSetPos(ACTOR_SEA_KAJA, 0, 0);
	fdActorGet(ACTOR_SEA_KAJA)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorSetPos(ACTOR_SEA_CREW_MEMBER, 232, 650);
	fdActorGet(ACTOR_SEA_CREW_MEMBER)->facing = FACING_NORTH_NORTHWEST;
	sySynch(1);
	scTolbiShipDeckAnnouncePreparationsReady(ACTOR_SEA_KAJA,
											 ACTOR_SEA_CREW_MEMBER);
}

/* The messenger comes up through the carved door at the north end and nods, */
/* and the sailor waiting beside it jumps and calls `Preparations... */
/* Ready!`. */
static void scTolbiShipDeckAnnouncePreparationsReady(s32 messenger,
													 s32 crewMember) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	scTolbiShipDeckOpenFirstDoor();
	fdActorSetPos(messenger, 216, 588);
	fdActorSetSpeed(messenger, 0.8);
	fdActorWalkToAndWait(messenger, 216, 600);
	fdActorWalkToAndWait(messenger, 218, 604);
	fdActorWalkToAndWait(messenger, 234, 604);
	fdActorWalkToAndWait(messenger, 236, 618);
	fdActorFaceDirection(messenger, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(messenger, ANIM_NOD);
	fdEventWait(20);
	scTolbiShipDeckTurn(crewMember, FACING_SOUTH_SOUTHWEST);
	fdActorJump(crewMember, 4, 40);
	fdActorStartRepeatedMotion(crewMember, 2);
	fdEventSetMessageId(MsgTolbiShipPreparationsReady);
	/* "Preparations... Ready!" */
	fdEventShowMessageThenWait(crewMember, 0, 20);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST_COURSE_CLEAR);
}

/* Kaja calls "Aye, Captain! Sea monsters!" and Sean begs the captain to turn */
/* the rudder hard to starboard, which leads to the captain's quarters. */
static void scTolbiShipDeckSightSeaMonsters(void) {
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SEA_KAJA, 1);
	fdEventSetMessageId(MsgTolbiShipAyeCaptainSeaMonsters);
	/* "Aye, Captain! Sea monsters!" */
	fdEventShowMessageThenWait(ACTOR_SEA_KAJA, 0, 10);
	scTolbiShipDeckTurn(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHWEST);
	fdActorJump(ACTOR_SEA_SEAN, 4, 20);
	fdActorStartRepeatedMotion(ACTOR_SEA_SEAN, 2);
	/* "Captain, turn the rudder hard to starboard!" */
	fdEventShowMessageThenWait(
		SPEAKER(ACTOR_SEA_SEAN, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT),
		0, 20);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_TURN);
}

/* Ouranos runs up the deck crying it is too late, three monsters leap over */
/* the rail and go below by the carved door, and Ouranos is heard to cry */
/* "Arrgh!" as the fight moves below. */
static void scTolbiShipDeckMonstersBoard(void) {
	const s32 *shared;

	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_MonstersBoardActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_OURANOS, 248, 728);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 1.6);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 242, 692);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 196, 678);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 182, 654);
	fdActorRunRepeatedMotion(ACTOR_SEA_OURANOS, 2);
	fdEventSetMessageId(MsgTolbiShipNoUseLate);
	/* "It's no use. It's too late!" */
	scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_OURANOS,
							   SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT));
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2.4);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 154, 609);
	sySndPlay(SE_ENEMY_CRY);
	fdActorGet(ACTOR_SEA_FIRST_ATTACK_MONSTER)->tmr = 0;
	fdActorGet(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1)->tmr = 0;
	fdActorGet(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2)->tmr = 0;
	fdActorSetPos(ACTOR_SEA_FIRST_ATTACK_MONSTER, 32, 484);
	fdActorSetPos(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, 84, 496);
	fdActorSetPos(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, 16, 596);
	fdActorSetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER, 2.4);
	fdActorSetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, 2.4);
	fdActorSetSpeed(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, 2.4);
	shared = TolbiShipDeck_MonsterLeapsAboardActions;
	fdActorSetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER, shared);
	fdActorSetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, shared);
	fdActorSetScript(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, shared);
	fdActorSetSprColById(ACTOR_SEA_FIRST_ATTACK_MONSTER, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_FIRST_ATTACK_MONSTER + 1, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_FIRST_ATTACK_MONSTER + 2, SPRITE_COLOR_BGR);
	/* The door opens once the first monster has leapt aboard. */
	for(;;) {
		sySynch(1);
		if(fdActorGet(ACTOR_SEA_FIRST_ATTACK_MONSTER)->tmr != 0) {
			scTolbiShipDeckOpenFirstDoor();
			break;
		}
	}
	fdActorWalkTo(ACTOR_SEA_OURANOS, 196, 612);
	fdActorWaitForScriptEndById(ACTOR_SEA_FIRST_ATTACK_MONSTER);
	fdEventWait(10);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(10);
	fdActorSystemSuspendProjectionAndShowSprs();
	fdMessageSetVoiceFromActor(ACTOR_SEA_OURANOS);
	/* "Arrgh!" */
	mnFieldMessageShowCentered(MsgTolbiShipArrgh, 1, 0);
	fdActorSystemResumeProjectionAndHideLayers();
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_FIRST_ATTACK);
}

/* The monsters harry the crew while more of them climb aboard. */
static void scTolbiShipDeckMonstersAttackCrew(void) {
	fdEventStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_SecondAttackActorPlacements);
	sySynch(1);
	fdActorSetAnimation(ACTOR_SEA_SPLASH, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_SECOND_ATTACK_MONSTER),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1),
					   SPRITE_FLAG_SHADOW);
	fdActorSetPos(ACTOR_SEA_SEAN, 256, 640);
	fdActorSetScript(ACTOR_SEA_SEAN, TolbiShipDeck_LookAroundActions);
	fdActorSetPos(ACTOR_SEA_OURANOS, 268, 692);
	/* XXX: Sean is given this script twice and Ouranos none; whether Ouranos */
	/* keeps still through the second attack would settle whether it is a slip.
	 */
	fdActorSetScript(ACTOR_SEA_SEAN, TolbiShipDeck_LookAroundActions);
	fdActorSetPos(ACTOR_SEA_FIRST_HOVERING_MONSTER, 242, 604);
	fdActorSetPos(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1, 264, 600);
	fdActorSetPos(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2, 254, 668);
	fdActorSetPos(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3, 282, 658);
	fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER)->sceneByte = 0;
	fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1)->sceneByte = 1;
	fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2)->sceneByte = 0;
	fdActorGet(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3)->sceneByte = 1;
	fdActorSetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER,
					 TolbiShipDeck_HoverAndDiveAtSeanActions);
	fdActorSetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1,
					 TolbiShipDeck_HoverAndDiveAtSeanActions);
	fdActorSetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2,
					 TolbiShipDeck_HoverAndDiveAtOuranosActions);
	fdActorSetScript(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3,
					 TolbiShipDeck_HoverAndDiveAtOuranosActions);
	fdActorSetPos(ACTOR_SEA_KAJA, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(400);
	fdActorSetPos(ACTOR_SEA_SECOND_ATTACK_MONSTER, 254, 740);
	fdActorSetPos(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 24, 586);
	fdActorSetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER, 1.6);
	fdActorSetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 1.6);
	fdActorSetDestination(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1, 172, 644);
	fdActorMoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER, 200, 660);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 4);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 174, 620);
	fdActorMoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER, 180, 580);
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER, TolbiShipDeck_BobActions);
	fdActorSetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1,
					 TolbiShipDeck_BobActions);
	sySndPlay(SE_LOG_FALL);
	fdActorSetPos(ACTOR_SEA_SPLASH, 134, 594);
	fdActorSetScript(ACTOR_SEA_SPLASH, TolbiShipDeck_SwellActions);
	fdEventWait(10);
	fdActorSetPos(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 134, 584);
	fdActorSetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 4);
	fdActorGet(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2)->velY = toFIXED(8);
	fdActorMoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 186, 612);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2),
					   SPRITE_FLAG_SHADOW);
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 2);
	fdActorMoveToAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, 216, 600);
	scTolbiShipDeckTurn(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2, FACING_NORTH);
	scTolbiShipDeckOpenFirstDoor();
	fdEventWait(10);
	fdActorSetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER + 2,
					 TolbiShipDeck_MonsterGoesBelowActions);
	fdEventWait(10);
	fdActorSetScript(ACTOR_SEA_SECOND_ATTACK_MONSTER,
					 TolbiShipDeck_MonsterGoesBelowActions);
	fdEventWait(10);
	fdActorRunScriptAndWait(ACTOR_SEA_SECOND_ATTACK_MONSTER + 1,
							TolbiShipDeck_MonsterGoesBelowActions);
	fdEventWait(20);
	sySndPlay(SE_BEAST_CRY);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdActorStop(ACTOR_SEA_FIRST_HOVERING_MONSTER);
	fdActorStop(ACTOR_SEA_FIRST_HOVERING_MONSTER + 1);
	fdActorStop(ACTOR_SEA_FIRST_HOVERING_MONSTER + 2);
	fdActorStop(ACTOR_SEA_FIRST_HOVERING_MONSTER + 3);
	fdEventWait(10);
	fdActorSystemSuspendProjectionAndShowSprs();
	fdMessageSetVoiceFromActor(ACTOR_SEA_OURANOS);
	/* "Arrgh!" */
	mnFieldMessageShowCentered(MsgTolbiShipArrgh, 1, 0);
	fdActorSystemResumeProjectionAndHideLayers();
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SECOND_ATTACK);
}

/* A great shape rises from the sea and a monster drops onto the deck. */
static void scTolbiShipDeckMonsterLandsOnDeck(void) {
	ACTOR *monster;
	u32 frame;
	const s32 *sailorActions;

	fdEventStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_ThirdAttackActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_SEAN, 176, 696);
	fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_NORTH_NORTHEAST;
	fdActorSetPos(ACTOR_SEA_OURANOS, 264, 662);
	fdActorGet(ACTOR_SEA_OURANOS)->facing = FACING_NORTH_NORTHWEST;
	fdActorSetPos(ACTOR_SEA_FIRST_LEAPING_MONSTER, 184, 672);
	fdActorSetPos(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1, 202, 692);
	fdActorSetPos(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2, 252, 646);
	fdActorSetPos(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3, 256, 686);
	fdActorSetPos(ACTOR_SEA_THIRD_ATTACK_MONSTER, 172, 632);
	fdActorSetPos(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, 256, 622);
	fdActorGet(ACTOR_SEA_FIRST_LEAPING_MONSTER)->sceneByte = 0;
	fdActorGet(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1)->sceneByte = 1;
	fdActorGet(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2)->sceneByte = 0;
	fdActorGet(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3)->sceneByte = 2;
	fdActorSetPos(ACTOR_SEA_KAJA, 0, 0);
	fdActorSetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER,
					 TolbiShipDeck_LeapAtSeanActions);
	fdActorSetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1,
					 TolbiShipDeck_LeapAtSeanActions);
	fdActorSetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2,
					 TolbiShipDeck_LeapAtOuranosActions);
	fdActorSetScript(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3,
					 TolbiShipDeck_LeapAtOuranosActions);
	fdActorSetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER, TolbiShipDeck_HopActions);
	fdActorSetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1,
					 TolbiShipDeck_HopActions);
	fdActorSetSprColById(ACTOR_SEA_FIRST_LEAPING_MONSTER, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_THIRD_ATTACK_MONSTER, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, SPRITE_COLOR_BGR);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(80);
	sySndPlay(SE_BEAST_CRY);
	/* The shadow grows where the monster will land, then it drops from 80 */
	/* pixels up. */
	monster = fdActorGet(ACTOR_SEA_FALLING_MONSTER_SHADOW);
	monster->scaleX = FIXED_ONE / 10;
	monster->scaleY = FIXED_ONE / 10;
	monster->x.fixed = toFIXED(194);
	monster->z.fixed = toFIXED(642);
	for(frame = 0; frame < 16; frame++) {
		monster->scaleX += 3 * FIXED_ONE / 50;
		monster->scaleY += 3 * FIXED_ONE / 50;
		sySynch(1);
	}
	monster = fdActorGet(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2);
	monster->scaleX = 11 * FIXED_ONE / 10;
	monster->scaleY = 11 * FIXED_ONE / 10;
	monster->x.fixed = toFIXED(194);
	monster->y.fixed = toFIXED(80);
	monster->z.fixed = toFIXED(642);
	monster->facing = FACING_SOUTH_SOUTHWEST;
	monster->bounce = 2 * FIXED_ONE / 5;
	monster->gravity = toFIXED(2);
	fdEventWait(80);
	sySndPlay(SE_BEAST_CRY);
	fdActorSetPos(ACTOR_SEA_FALLING_MONSTER_SHADOW, 0, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	monster = fdActorGet(ACTOR_PARTY_LEADER);
	monster->motionFlg = 0;
	fdActorSetDestination(ACTOR_PARTY_LEADER, 216, 612);
	fdActorSetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 1.6);
	fdActorMoveToAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 196, 600);
	fdActorMoveToAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, 216, 600);
	fdActorStop(ACTOR_SEA_THIRD_ATTACK_MONSTER);
	sySynch(1);
	fdActorSetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER, 1.6);
	sailorActions = TolbiShipDeck_MonsterGoesBelowActions;
	fdActorSetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER, sailorActions);
	scTolbiShipDeckTurn(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2,
						FACING_NORTH_NORTHEAST);
	scTolbiShipDeckOpenFirstDoor();
	fdEventWait(10);
	fdActorSetScript(ACTOR_SEA_THIRD_ATTACK_MONSTER + 2, sailorActions);
	fdActorStop(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1);
	sySynch(1);
	fdActorSetSpeed(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, 1.6);
	fdActorRunScriptAndWait(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1, sailorActions);
	fdEventWait(20);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdActorStop(ACTOR_SEA_FIRST_LEAPING_MONSTER);
	fdActorStop(ACTOR_SEA_FIRST_LEAPING_MONSTER + 1);
	fdActorStop(ACTOR_SEA_FIRST_LEAPING_MONSTER + 2);
	fdActorStop(ACTOR_SEA_FIRST_LEAPING_MONSTER + 3);
	fdActorStop(ACTOR_SEA_THIRD_ATTACK_MONSTER);
	fdActorStop(ACTOR_SEA_THIRD_ATTACK_MONSTER + 1);
	fdEventWait(10);
	fdActorSystemSuspendProjectionAndShowSprs();
	fdMessageSetVoiceFromActor(ACTOR_SEA_OURANOS);
	/* "Arrgh!" */
	mnFieldMessageShowCentered(MsgTolbiShipArrgh, 1, 0);
	fdActorSystemResumeProjectionAndHideLayers();
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_THIRD_ATTACK);
}

/* The party jumps down to the deck, gathers and charges the Kraken. */
static void scTolbiShipDeckJumpIntoKrakenBattle(void) {
	ACTOR *actor;

	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_KrakenActorPlacements);
	sySynch(1);
	fdCameraFollowActor(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 1);
	sySynch(1);
	fdActorSetAnimation(ACTOR_SEA_OURANOS, ANIM_LYING_FLAT);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_OURANOS), 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_SOUTH;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	scTolbiShipDeckOpenFirstDoor();
	fdEventWait(10);
	fdActorSetPos(ACTOR_PARTY_LEADER, 216, 586);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
	fdEventWait(20);
	scTolbiShipDeckTurn(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
	sySndPlay(SE_BOOM);
	fdMapGridSetShake(4, 4, 1);
	fdEventWait(20);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	fdActorGet(ACTOR_INVISIBLE_SEA_CAMERA_TARGET)->motionFlg = 0;
	fdActorSetSpeed(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 2);
	fdActorSetDestination(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 216, 612);
	sySndPlay(SE_MONSTER_ROAR);
	fdActorSetSprPriority(ACTOR_SEA_SEAN, 2);
	fdActorSetAnimation(ACTOR_SEA_SEAN, ANIM_LYING_FLAT);
	actor = fdActorGet(ACTOR_SEA_SEAN);
	actor->velY = toFIXED(8);
	actor->gravity = toFIXED(7) / 10;
	actor->scaleX = 13 * FIXED_ONE / 8;
	actor->scaleY = 13 * FIXED_ONE / 8;
	actor->update = scTolbiShipDeckShrinkToNormalSize;
	actor->bounce = FIXED_ONE / 2;
	fdActorSetSpeed(ACTOR_SEA_SEAN, 6);
	fdActorMoveToAndWait(ACTOR_SEA_SEAN, 182, 618);
	fdActorSetSprFlags(fdActorGet(ACTOR_SEA_SEAN), 0);
	scTolbiShipDeckTurn(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 80);
	fdActorSetDestination(ACTOR_INVISIBLE_SEA_CAMERA_TARGET, 232, 564);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 250, 584);
	fdActorWalkTo(ACTOR_GARET, 240, 600);
	fdActorWalkTo(ACTOR_IVAN, 254, 600);
	fdActorWalkToAndWait(ACTOR_MIA, 248, 616);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	sySndPlay(SE_MONSTER_ROAR);
	fdEventWait(40);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_IVAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_MIA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 248, 564);
	fdActorWalkTo(ACTOR_GARET, 248, 564);
	fdEventWait(20);
	fdActorWalkTo(ACTOR_IVAN, 248, 564);
	fdActorWalkTo(ACTOR_MIA, 248, 564);
	fdEventWait(20);
	MainRoutineStateWork.restartScene = SCENE_SHIP_ROOMS;
	MainRoutineStateWork.restartEntrance =
		SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
	btSetType(BATTLE_TYPE_SCRIPTED);
	fdEventSetAfterBattleDestination(
		SCENE_TOLBI_SHIP_DECK, TOLBI_SHIP_DECK_AFTER_JUMP_INTO_KRAKEN_BATTLE);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_TOLBI_SHIP, 3);
	fdEventEnd();
}

/* After the Kraken, Kaja thanks Isaac, says they lost another oarsman and */
/* asks him to find someone to row while he checks the hull, and goes below. */
static void scTolbiShipDeckThankForKraken(void) {
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_KAJA, 182, 618);
	fdActorSetPos(ACTOR_SEA_CREW_MEMBER, 238, 626);
	fdActorSetPos(ACTOR_SEA_SEAN, 268, 678);
	fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_EAST;
	fdActorSetScript(ACTOR_SEA_SEAN, TolbiShipDeck_SeanLooksAboutActions);
	fdActorGet(ACTOR_SEA_OURANOS)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 0.8);
	fdActorSetScript(ACTOR_SEA_OURANOS, TolbiShipDeck_OuranosPatrolsActions);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_SEA_KAJA, 1.6);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 182, 548);
	scTolbiShipDeckTurn(ACTOR_SEA_KAJA, FACING_EAST);
	scTolbiShipDeckTurn(ACTOR_PARTY_LEADER, FACING_WEST);
	fdActorRunRepeatedMotion(ACTOR_SEA_KAJA, 1);
	fdEventSetMessageId(MsgTolbiShipThankIsaacDidGoodAgainst);
	/* "Thank you, Isaac. You did good against that Kraken." */
	scTolbiShipDeckSay(ACTOR_SEA_KAJA);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_SEA_KAJA, FACING_SOUTH_SOUTHWEST, 20);
	fdActorShowEmote(ACTOR_SEA_KAJA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "We lost another oarsman, though." */
	fdEventShowMessageThenWait(ACTOR_SEA_KAJA, 0, 40);
	scTolbiShipDeckTurn(ACTOR_SEA_KAJA, FACING_EAST);
	/* "I need to check the hull. Go find someone to row the ship." */
	scTolbiShipDeckSay(ACTOR_SEA_KAJA);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 182, 600);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 216, 600);
	scTolbiShipDeckTurn(ACTOR_SEA_KAJA, FACING_NORTH);
	scTolbiShipDeckOpenFirstDoor();
	fdEventWait(10);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 216, 580);
	fdActorSetPos(ACTOR_SEA_KAJA, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	gmFlagSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
	gmFlagClear(FLAG_TOLBI_SHIP_KRAKEN_ON_DECK);
	fdEventEnd();
}

/* Sean sees land, Ouranos says they have made it, and the ship sails on to */
/* Tolbi on the World Map. */
static void scTolbiShipDeckSightLand(void) {
	fdEventStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_KAJA, 196, 502);
	fdActorGet(ACTOR_SEA_KAJA)->facing = FACING_NORTHWEST;
	fdActorSetPos(ACTOR_SEA_SEAN, 184, 524);
	fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_NORTH_NORTHWEST;
	fdActorSetSprPriority(ACTOR_SEA_OURANOS, 1);
	fdActorSetPos(ACTOR_SEA_OURANOS, 184, 632);
	fdActorGet(ACTOR_SEA_OURANOS)->facing = FACING_NORTH_NORTHWEST;
	/* UB: the field state's pointer is read through a u32 lvalue, which keeps
	 */
	/* the facing store ahead of its load; UBFIX reads it as declared. */
#ifdef UBFIX
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#else
	((FIELD_STATE *)*(u32 *)&FieldStateWork)->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
#endif
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdActorJump(ACTOR_SEA_SEAN, 4, 10);
	fdActorJump(ACTOR_SEA_SEAN, 6, 20);
	fdEventSetMessageId(MsgTolbiShipCanSeeLand);
	/* "Hey, I can see land!" */
	scTolbiShipDeckSay(ACTOR_SEA_SEAN);
	fdActorSetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 3);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 180, 546);
	fdActorFaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHWEST, 40);
	fdActorRunRepeatedMotion(ACTOR_SEA_OURANOS, 1);
	/* "It looks like we made it!" */
	scTolbiShipDeckSay(ACTOR_SEA_OURANOS);
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_WORLD_MAP_REACH_TOLBI);
}

/* What each rower says coming up at Tolbi, named by its message. */
/* */
/* The first four found take the first four replies in the order found. Three */
/* passengers have their own. */
typedef enum TOLBI_SHIP_DECK_ROWER_REPLY {
	ROWER_REPLY_FINALLY_LUCKY = 0,	/* MsgTolbiShipMadeFinallyLucky */
	ROWER_REPLY_OTHERS_ALREADY = 1, /* MsgTolbiShipOtherPassengersAlready */
	ROWER_REPLY_DONE_ROWING = 2,	/* MsgTolbiShipDontRowAnymore */
	ROWER_REPLY_NO_WORKOUT = 3,		/* MsgTolbiShipOhhhHaventWorkout */
	ROWER_REPLY_ROWING_MORE = 10,	/* MsgTolbiShipRowingShipMore */
	ROWER_REPLY_LOST_AT_SEA = 11,	/* MsgTolbiShipTotallyLostOcean */
	ROWER_REPLY_OLD_MAN = 20		/* MsgTolbiShipMakeOldMan */
} TOLBI_SHIP_DECK_ROWER_REPLY;

/* Takes `passenger` as the next to come up, with `reply`, when one of the */
/* four seats holds them. */
#define TOLBI_SHIP_DECK_TAKE_ROWER(passenger, reply)                           \
	do {                                                                       \
		member = (passenger);                                                  \
		for(index = 0; index <= 3; index++) {                                  \
			if(roster[index] == member) {                                      \
				matched[found] = member;                                       \
				category[found] = (reply);                                     \
				found++;                                                       \
				break;                                                         \
			}                                                                  \
		}                                                                      \
	} while(0)

/* At Tolbi, the four passengers who rowed come up from below in turn and */
/* each says something. */
static void scTolbiShipDeckRowersDisembark(void) {
	s32 roster[4];
	s32 matched[4];
	s32 category[4];
	u32 index;
	s32 found;
	s32 actorId;
	s32 member;
	ACTOR *actor;

	found = 0;
	for(index = 0; index <= 3; index++) {
		roster[index] = scTolbiShipDeckGetRowerAtSeat(index);
		category[index] = 0;
	}

	/* The first four in this order take the first four replies; three others */
	/* have their own, for when fewer than four of the first rowed. */
	TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER, found);
	TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 1, found);
	TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 2, found);
	TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 4, found);
	if(found != 4)
		TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 5, found);
	if(found != 4)
		TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 6, found);
	if(found != 4)
		TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 3,
								   ROWER_REPLY_ROWING_MORE);
	if(found != 4)
		TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 7,
								   ROWER_REPLY_LOST_AT_SEA);
	if(found != 4)
		TOLBI_SHIP_DECK_TAKE_ROWER(ACTOR_ARRIVED_FIRST_PASSENGER + 8,
								   ROWER_REPLY_OLD_MAN);

	fdEventStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, actor->x.fixed,
						actor->z.fixed);
	}
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdCameraFollowActor(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 1);

	scTolbiShipDeckOpenFirstDoor();

	fdEventWait(10);

	for(index = 0; index <= 3; index++) {
		actorId = matched[index];
		fdActorSetPos(actorId, 216, 584);
		if(category[index] == ROWER_REPLY_OLD_MAN) {
			fdActorSetSpeed(actorId, 0.8);
		}
		else {
			fdActorSetSpeed(actorId, 1);
		}
		fdActorWalkToAndWait(actorId, 216, 600);
		fdActorWalkToAndWait(actorId, 192, 618);
		fdActorWalkToAndWait(actorId, 192, 656);

		switch(category[index]) {
		case ROWER_REPLY_FINALLY_LUCKY:
			fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
			/* "We made it? Finally! We're lucky to be alive after all those
			 * attacks!" */
			fdEventSetMessageId(MsgTolbiShipMadeFinallyLucky);
			break;
		case ROWER_REPLY_OTHERS_ALREADY:
			scTolbiShipDeckTurn(actorId, FACING_NORTH_NORTHEAST);
			fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
			/* "The other passengers have already disembarked... Didn't even say
			 */
			/* good-bye." */
			fdEventSetMessageId(MsgTolbiShipOtherPassengersAlready);
			break;
		case ROWER_REPLY_DONE_ROWING:
			fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
			/* "We don't have to row anymore? Oh, what a relief!" */
			fdEventSetMessageId(MsgTolbiShipDontRowAnymore);
			break;
		case ROWER_REPLY_NO_WORKOUT:
			fdActorRunRepeatedMotion(actorId, 1);
			/* "Ohhh! I haven't had a workout like that in years!" */
			fdEventSetMessageId(MsgTolbiShipOhhhHaventWorkout);
			break;
		case ROWER_REPLY_ROWING_MORE:
			fdActorSetAnimationAndWait(actorId, ANIM_NOD);
			/* "Rowing the ship was more fun than I expected! I'm pretty buff
			 * now!" */
			fdEventSetMessageId(MsgTolbiShipRowingShipMore);
			break;
		case ROWER_REPLY_LOST_AT_SEA:
			fdActorSetAnimation(actorId, ANIM_SHAKE_HEAD);
			/* "I totally lost out... An ocean cruise, and I didn't get to enjoy
			 * the */
			/* view!" */
			fdEventSetMessageId(MsgTolbiShipTotallyLostOcean);
			break;
		case ROWER_REPLY_OLD_MAN:
			fdActorSetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
			fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
			/* "You make an old man row the ship, then you try to beat him
			 * onshore? */
			/* That's pretty pathetic." */
			fdEventSetMessageId(MsgTolbiShipMakeOldMan);
			break;
		default:
			break;
		}

		scTolbiShipDeckSay(actorId);
		fdActorSetScript(actorId, TolbiShipDeck_RowerLeavesActions);
	}

	fdActorWaitForScriptEndById(actorId);
	fdEventWait(40);
	fdActorSetPos(ACTOR_PARTY_LEADER, 216, 584);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 216, 600);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 190, 612);
	fdEventSetMessageId(MsgTolbiShipThanksHardWork);
	/* "Thanks for all your hard work." */
	scTolbiShipDeckSay(ACTOR_ARRIVED_KAJA);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);

	fdActorGet(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET)->motionFlg = 0;
	fdActorSetSpeed(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 2);
	fdActorSetDestination(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 196, 564);

	fdActorSetSpeed(ACTOR_ARRIVED_KAJA, 0.8);
	fdActorWalkToAndWait(ACTOR_ARRIVED_KAJA, 182, 555);
	fdActorFaceDirection(ACTOR_ARRIVED_KAJA, FACING_SOUTH_SOUTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_ARRIVED_KAJA, 1);
	/* "Your efforts got this ship to shore." */
	scTolbiShipDeckSay(ACTOR_ARRIVED_KAJA);
	fdActorSetAnimationAndWait(ACTOR_ARRIVED_KAJA, ANIM_NOD);
	/* "I thank you again." */
	scTolbiShipDeckSay(ACTOR_ARRIVED_KAJA);
	fdActorFaceDirection(ACTOR_ARRIVED_KAJA, FACING_WEST, 40);
	/* "Hurry, everyone! Colosso is already underway in Tolbi!" */
	scTolbiShipDeckSay(ACTOR_ARRIVED_KAJA);
	scTolbiShipDeckTurn(ACTOR_ARRIVED_KAJA, FACING_SOUTH_SOUTHEAST);
	/* "You'd better get moving to Tolbi, too." */
	scTolbiShipDeckSay(ACTOR_ARRIVED_KAJA);
	fdActorSetAnimationAndWait(ACTOR_ARRIVED_KAJA, ANIM_NOD);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_ARRIVED_KAJA, 188, 512);
	fdActorFaceDirection(ACTOR_ARRIVED_KAJA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetPos(ACTOR_INVISIBLE_ARRIVED_CAMERA_TARGET, 0, 0);

	gmFlagClear(FLAG_SHOW_AREA_NAME);
	fdEventEnd();
}

/* Returns the passenger (ACTOR_ARRIVED_FIRST_PASSENGER and the eight after) */
/* who promised to row at a seat (0 to 3), or 0 if the seat is empty. */
static s32 scTolbiShipDeckGetRowerAtSeat(u32 seat) {
	s32 flag = 0;
	s32 passenger = ACTOR_ARRIVED_FIRST_PASSENGER;
	u32 i;

	switch(seat) {
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
	for(i = 0; i < 9; i++) {
		if(gmFlagGet(flag) != 0)
			return passenger;
		flag++;
		passenger++;
	}
	return 0;
}

/* The sea's pattern repeats every 32 pixels, where its scroll wraps. */
#define TOLBI_SHIP_DECK_SEA_WRAP toFIXED(32)

/* The task that rocks the deck while under sail. */
static void scTolbiShipDeckRockDeck(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	s32 *origin = grid->origin;
	FIXED dx = syCos(sRockAngleX);
	FIXED dy = sySin(sRockAngleY);
	MAP_GRID_LAYER_SCROLL *layer;

	*origin++ += dx >> 1;
	*origin += dy;
	sRockAngleX += syRandomBelow(384);
	sRockAngleY += syRandomBelow(512);
	sRockAngleX &= TRIG_ANGLE_MASK;
	sRockAngleY &= TRIG_ANGLE_MASK;
	layer = &grid->layers[MAPGRID_LAYER_GROUND];
	layer->ofsX = sSeaScroll[0];
	sSeaScroll[0] -= sSeaSpeed[0];
	if(sSeaScroll[0] < 0) {
		sSeaScroll[0] += TOLBI_SHIP_DECK_SEA_WRAP;
	}
	if(sSeaScroll[0] > TOLBI_SHIP_DECK_SEA_WRAP) {
		sSeaScroll[0] -= TOLBI_SHIP_DECK_SEA_WRAP;
	}
	layer->ofsY = sSeaScroll[1];
	sSeaScroll[1] -= sSeaSpeed[1];
	if(sSeaScroll[1] < 0) {
		sSeaScroll[1] += TOLBI_SHIP_DECK_SEA_WRAP;
	}
}

/* Under sail, shows the stroke oars (ACTOR_FIRST_STROKE_OAR) and the oars at */
/* the sides (ACTOR_FIRST_OAR, the right-hand ones mirrored). */
static void scTolbiShipDeckSetUpOars(void) {
	fdActorGet(ACTOR_FIRST_STROKE_OAR)->collisionFlg = 0;
	fdActorGet(ACTOR_FIRST_STROKE_OAR + 1)->collisionFlg = 0;
	fdActorGet(ACTOR_FIRST_STROKE_OAR + 2)->collisionFlg = 0;
	fdActorGet(ACTOR_FIRST_STROKE_OAR + 3)->collisionFlg = 0;
	scTolbiShipDeckShowOar(ACTOR_FIRST_STROKE_OAR);
	scTolbiShipDeckShowOar(ACTOR_FIRST_STROKE_OAR + 1);
	scTolbiShipDeckShowOar(ACTOR_FIRST_STROKE_OAR + 2);
	scTolbiShipDeckShowOar(ACTOR_FIRST_STROKE_OAR + 3);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 1);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 2);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 3);
	sOarDepth[0] = fdActorGet(ACTOR_FIRST_OAR)->z.fixed;
	sOarDepth[1] = fdActorGet(ACTOR_FIRST_OAR + 1)->z.fixed;
	sOarDepth[2] = fdActorGet(ACTOR_FIRST_OAR + 2)->z.fixed;
	sOarDepth[3] = fdActorGet(ACTOR_FIRST_OAR + 3)->z.fixed;
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 4);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 5);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 6);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 7);
	fdActorGet(ACTOR_FIRST_OAR + 4)->scaleX = -FIXED_ONE;
	fdActorGet(ACTOR_FIRST_OAR + 5)->scaleX = -FIXED_ONE;
	fdActorGet(ACTOR_FIRST_OAR + 6)->scaleX = -FIXED_ONE;
	fdActorGet(ACTOR_FIRST_OAR + 7)->scaleX = -FIXED_ONE;
	sOarDepth[4] = fdActorGet(ACTOR_FIRST_OAR + 4)->z.fixed;
	sOarDepth[5] = fdActorGet(ACTOR_FIRST_OAR + 5)->z.fixed;
	sOarDepth[6] = fdActorGet(ACTOR_FIRST_OAR + 6)->z.fixed;
	sOarDepth[7] = fdActorGet(ACTOR_FIRST_OAR + 7)->z.fixed;
	scTolbiShipDeckChooseOarStrokes();
}

/* Shows an oar drawn under the deck, with no collision. */
static void scTolbiShipDeckShowOar(s32 actorId) {
	ACTOR *oar = fdActorGet(actorId);

	if(oar != NULL) {
		fdActorSetSprPriority(actorId, 3);
		fdActorSetSprFlags(oar, 0);
		oar->collisionFlg = 0;
		{
			s32 flags;
			flags = ACTOR_PRIORITY_UNDERFOOT | oar->priFlg;
			oar->priFlg = flags;
		}
	}
}

/* Gives each oar the stroke of its seat's rower. */
static void scTolbiShipDeckChooseOarStrokes(void) {
	u32 *mode = sOarStroke;
	u32 i;

	for(i = 0; i <= 3; i++) {
		if(scTolbiShipDeckGetRowerAtSeat(i) != 0)
			*mode = scTolbiShipDeckGetSeatStroke(i);
		else
			*mode = OAR_STROKE_STEADY;
		mode++;
	}
	if(scTolbiShipDeckGetRowerAtSeat(0) != 0)
		sOarStroke[0] = scTolbiShipDeckGetSeatStroke(0);
	else
		sOarStroke[0] = OAR_STROKE_STEADY;
	if(scTolbiShipDeckGetRowerAtSeat(2) != 0)
		sOarStroke[1] = scTolbiShipDeckGetSeatStroke(2);
	else
		sOarStroke[1] = OAR_STROKE_STEADY;
	sOarStroke[2] = OAR_STROKE_STEADY;
	sOarStroke[3] = OAR_STROKE_STEADY;
	if(scTolbiShipDeckGetRowerAtSeat(1) != 0)
		sOarStroke[4] = scTolbiShipDeckGetSeatStroke(1);
	else
		sOarStroke[4] = OAR_STROKE_STEADY;
	if(scTolbiShipDeckGetRowerAtSeat(3) != 0)
		sOarStroke[5] = scTolbiShipDeckGetSeatStroke(3);
	else
		sOarStroke[5] = OAR_STROKE_STEADY;
	sOarStroke[6] = OAR_STROKE_STEADY;
	sOarStroke[7] = OAR_STROKE_STEADY;
}

/* Returns the stroke (1 to 4) of the rower at a seat, or 0 if the seat is */
/* empty. */
static s32 scTolbiShipDeckGetSeatStroke(u32 seat) {
	s32 base = 0;
	u32 i;

	switch(seat) {
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
	for(i = 0; i < 9; i++) {
		if(gmFlagGet(base + i) != 0)
			return TolbiShipDeckRowerStrokes[i];
	}
	return 0;
}

/* The task that rows, which advances the four strokes and swings the eight */
/* oars at the sides, each following its stroke's phase. */
static void scTolbiShipDeckRowOars(void) {
	scTolbiShipDeckAdvanceStroke(ACTOR_FIRST_STROKE_OAR, 0);
	sStrokePhase[0] = 0;
	scTolbiShipDeckAdvanceStroke(ACTOR_FIRST_STROKE_OAR + 1, 1);
	sStrokePhase[1] += TRIG_TURN / 512;
	scTolbiShipDeckAdvanceStroke(ACTOR_FIRST_STROKE_OAR + 2, 2);
	sStrokePhase[2] += TRIG_TURN / 256;
	scTolbiShipDeckAdvanceStroke(ACTOR_FIRST_STROKE_OAR + 3, 3);
	sStrokePhase[3] += TRIG_TURN / 128;
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR, 0, 0);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 1, 1, 0);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 2, 2, 0);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 3, 3, 0);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 4, 4, OAR_SWING_MIRRORED);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 5, 5, OAR_SWING_MIRRORED);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 6, 6, OAR_SWING_MIRRORED);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 7, 7, OAR_SWING_MIRRORED);
}

/* Advances one stroke's phase, by its oar's angle in the turn: the blade is */
/* in the water from 7/16 to 15/16 of the turn and goes round slowly there, */
/* slowest as it dips in and comes out over the first and last 1/32, and */
/* swings back through the air fastest. */
static void scTolbiShipDeckAdvanceStroke(s32 actorId, s32 slot) {
	u16 value = sOarAngle[slot];

	if(value >= TRIG_TURN * 13 / 32 + 1 && value <= TRIG_TURN * 7 / 16 - 1) {
		sStrokePhase[slot] += TRIG_TURN * 7 / 4096;
		fdActorSetAnimation(actorId, OAR_ANIMATION_DIPPING);
	}
	else if(value >= TRIG_TURN * 29 / 32 + 1 &&
			value <= TRIG_TURN * 15 / 16 - 1) {
		sStrokePhase[slot] += TRIG_TURN * 7 / 2048;
		fdActorSetAnimation(actorId, OAR_ANIMATION_DIPPING);
	}
	else if(value >= TRIG_TURN * 7 / 16 + 1 &&
			value <= TRIG_TURN * 15 / 16 - 1) {
		sStrokePhase[slot] += TRIG_TURN * 7 / 1024;
		fdActorSetAnimation(actorId, OAR_ANIMATION_IN_WATER);
	}
	else {
		sStrokePhase[slot] += TRIG_TURN * 3 / 256;
		fdActorSetAnimation(actorId, OAR_ANIMATION_IN_AIR);
	}
}

/* Swings one oar at the side to its angle, turning it and moving it fore and */
/* aft about its depth; a rowing oar first takes its stroke's angle and frames.
 */
static void scTolbiShipDeckSwingOar(s32 id, s32 slot, s32 flags) {
	ACTOR *actor = fdActorGet(id);
	SPR *sprite = actor->spr;
	FIXED sine;

	if((flags & OAR_SWING_AT_REST) == 0) {
		switch(sOarStroke[slot]) {
		case OAR_STROKE_STILL:
			sOarAngle[slot] = sStrokePhase[0];
			fdActorCopyGraphics(id, ACTOR_FIRST_STROKE_OAR);
			break;
		case OAR_STROKE_SLOW:
			sOarAngle[slot] = sStrokePhase[1];
			fdActorCopyGraphics(id, ACTOR_FIRST_STROKE_OAR + 1);
			break;
		case OAR_STROKE_STEADY:
			sOarAngle[slot] = sStrokePhase[2];
			fdActorCopyGraphics(id, ACTOR_FIRST_STROKE_OAR + 2);
			break;
		case OAR_STROKE_FAST:
			sOarAngle[slot] = sStrokePhase[3];
			fdActorCopyGraphics(id, ACTOR_FIRST_STROKE_OAR + 3);
			break;
		}
	}
	if(flags & OAR_SWING_MIRRORED) {
		sine = sySin(sOarAngle[slot]);
		sprite->rotation = sySin(sOarAngle[slot] + DEGtoANG(180)) >> 5;
		actor->z.fixed = sOarDepth[slot] - (sine << 2) - (sine << 1);
	}
	else {
		sine = sySin(sOarAngle[slot] + DEGtoANG(180));
		sprite->rotation = sySin(sOarAngle[slot]) >> 5;
		actor->z.fixed = sOarDepth[slot] + (sine << 2) + (sine << 1);
	}
}

/* When not under sail, the oars lie at rest, all copying the first stroke */
/* oar. */
static void scTolbiShipDeckRestOars(void) {
	ACTOR *actor;
	u32 i;

	for(i = 0; i <= 7; i++)
		sOarAngle[i] = TRIG_TURN * 3 / 4;
	scTolbiShipDeckShowOar(ACTOR_FIRST_STROKE_OAR);
	fdActorSetPos(ACTOR_FIRST_STROKE_OAR + 1, 0, 0);
	fdActorSetPos(ACTOR_FIRST_STROKE_OAR + 2, 0, 0);
	fdActorSetPos(ACTOR_FIRST_STROKE_OAR + 3, 0, 0);
	fdActorSetPos(ACTOR_FIRST_OAR, 0, 0);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 1);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 2);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 3);
	sOarStroke[0] = 0;
	sOarStroke[1] = 0;
	sOarStroke[2] = 0;
	sOarStroke[3] = 0;
	sOarDepth[0] = fdActorGet(ACTOR_FIRST_STROKE_OAR)->z.fixed;
	sOarDepth[1] = fdActorGet(ACTOR_FIRST_OAR + 1)->z.fixed;
	sOarDepth[2] = fdActorGet(ACTOR_FIRST_OAR + 2)->z.fixed;
	sOarDepth[3] = fdActorGet(ACTOR_FIRST_OAR + 3)->z.fixed;
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 4);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 5);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 6);
	scTolbiShipDeckShowOar(ACTOR_FIRST_OAR + 7);
	fdActorGet(ACTOR_FIRST_OAR + 4)->scaleX = -FIXED_ONE;
	fdActorGet(ACTOR_FIRST_OAR + 5)->scaleX = -FIXED_ONE;
	fdActorGet(ACTOR_FIRST_OAR + 6)->scaleX = -FIXED_ONE;
	fdActorGet(ACTOR_FIRST_OAR + 7)->scaleX = -FIXED_ONE;
	sOarStroke[4] = 0;
	sOarStroke[5] = 0;
	sOarStroke[6] = 0;
	sOarStroke[7] = 0;
	sOarDepth[4] = fdActorGet(ACTOR_FIRST_OAR + 4)->z.fixed;
	sOarDepth[5] = fdActorGet(ACTOR_FIRST_OAR + 5)->z.fixed;
	sOarDepth[6] = fdActorGet(ACTOR_FIRST_OAR + 6)->z.fixed;
	sOarDepth[7] = fdActorGet(ACTOR_FIRST_OAR + 7)->z.fixed;
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL)
		fdActorSetPosFX(ACTOR_FIRST_STROKE_OAR, actor->x.fixed, actor->z.fixed);
	sySynch(1);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 1, ACTOR_FIRST_STROKE_OAR);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 2, ACTOR_FIRST_STROKE_OAR);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 3, ACTOR_FIRST_STROKE_OAR);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 4, ACTOR_FIRST_STROKE_OAR);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 5, ACTOR_FIRST_STROKE_OAR);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 6, ACTOR_FIRST_STROKE_OAR);
	fdActorCopyGraphics(ACTOR_FIRST_OAR + 7, ACTOR_FIRST_STROKE_OAR);
	fdActorGet(ACTOR_FIRST_STROKE_OAR)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 1)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 2)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 3)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 4)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 5)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 6)->retainSprCache = 1;
	fdActorGet(ACTOR_FIRST_OAR + 7)->retainSprCache = 1;
	sySynch(1);
	fdActorSetPos(ACTOR_FIRST_STROKE_OAR, 132, 632);
	sySynch(1);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_STROKE_OAR, 0, OAR_SWING_AT_REST);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 1, 1, OAR_SWING_AT_REST);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 2, 2, OAR_SWING_AT_REST);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 3, 3, OAR_SWING_AT_REST);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 4, 4,
							OAR_SWING_AT_REST | OAR_SWING_MIRRORED);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 5, 5,
							OAR_SWING_AT_REST | OAR_SWING_MIRRORED);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 6, 6,
							OAR_SWING_AT_REST | OAR_SWING_MIRRORED);
	scTolbiShipDeckSwingOar(ACTOR_FIRST_OAR + 7, 7,
							OAR_SWING_AT_REST | OAR_SWING_MIRRORED);
}

/* Redraws a block of the deck's cells and attributes, as it is whenever the */
/* ship has left the Kalay dock. */
static void scTolbiShipDeckCopyUnderwayCells(void) {
	fdMapGridCopyCellsTo(78, 39, 78, 40, 5, 1);
	fdMapGridCopyCellsTo(78, 39, 78, 41, 5, 1);
	fdMapGridCopyCellsTo(78, 39, 79, 42, 4, 1);
	fdMapGridCopyCellsTo(78, 39, 82, 43, 1, 1);
	fdMapGridCopyCellAttributes(17, 38, 5, 2, 17, 40);
}

/* Redraws two more blocks of cells, as they are at anchor. */
static void scTolbiShipDeckCopyAnchoredCells(void) {
	fdMapGridCopyCellsTo(66, 61, 64, 40, 5, 4);
	fdMapGridCopyCellAttributes(0, 0, 5, 4, 5, 39);
}

/* Shows the current message from a speaker and waits 10 frames. */
static void scTolbiShipDeckSay(s32 speaker) {
	fdEventShowMessage(speaker, 0);
	fdEventWait(10);
}

/* Turns an actor to a facing over 10 frames. */
static void scTolbiShipDeckTurn(s32 actorId, s32 facing) {
	fdActorFaceDirection(actorId, facing, 10);
}

/* After the lookout's call, Kaja sees the coast is not Tolbi's and the ship */
/* is off course. */
static void scTolbiShipDeckSightIsland(void) {
	fdEventStart();
	sSeaScroll[0] = toFIXED(4);
	sSeaSpeed[0] = -toFIXED(1) / 2;
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_CrewActorPlacements);
	sySynch(1);
	fdActorSetPos(ACTOR_SEA_OURANOS, 182, 618);
	fdActorGet(ACTOR_SEA_OURANOS)->facing = FACING_NORTH;
	fdActorSetPos(ACTOR_SEA_KAJA, 218, 516);
	fdActorGet(ACTOR_SEA_KAJA)->facing = FACING_NORTH_NORTHWEST;
	fdActorSetPos(ACTOR_SEA_SEAN, 204, 526);
	fdActorGet(ACTOR_SEA_SEAN)->facing = FACING_NORTH_NORTHWEST;
	fdActorSetPos(ACTOR_SEA_CREW_MEMBER, 0, 0);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 0.8);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 182, 532);
	fdActorFaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHWEST, 40);
	fdEventSetMessageId(MsgTolbiShipFinallyReachedTolbi);
	/* "Hmm... So we've finally reached Tolbi, huh?" */
	scTolbiShipDeckSay(ACTOR_SEA_OURANOS);
	fdActorRunRepeatedMotion(ACTOR_SEA_KAJA, 2);
	fdActorSetAnimation(ACTOR_SEA_KAJA, ANIM_SHAKE_HEAD);
	/* "Umm... No. That doesn't look like Tolbi's coast." */
	scTolbiShipDeckSay(
		SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
	fdActorFaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimation(ACTOR_SEA_KAJA, ANIM_NOD);
	/* "We must be off course. I don't recognize this place." */
	scTolbiShipDeckSay(
		SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
	fdActorSetAttachedEffect(ACTOR_SEA_OURANOS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_SEA_SEAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(80);
	fdActorShowEmote(ACTOR_SEA_OURANOS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 1.6);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 194, 500);
	fdActorFaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHWEST, 20);
	fdActorSetSpeed(ACTOR_SEA_SEAN, 0.8);
	fdActorWalkToAndWait(ACTOR_SEA_SEAN, 192, 518);
	fdActorFaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_SEA_KAJA, 1);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 210, 508);
	scTolbiShipDeckTurn(ACTOR_SEA_KAJA, FACING_NORTH_NORTHWEST);
	fdActorRunRepeatedMotion(ACTOR_SEA_OURANOS, 1);
	/* "You're right... That looks like an island." */
	scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_OURANOS,
							   SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT));
	fdActorSetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
	scTolbiShipDeckTurn(ACTOR_SEA_SEAN, FACING_NORTH_NORTHEAST);
	/* "Hmm... Maybe we should stop for a minute." */
	scTolbiShipDeckSay(
		SPEAKER(ACTOR_SEA_SEAN, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT));
	fdActorFaceDirection(ACTOR_SEA_KAJA, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_SEA_KAJA, ANIM_SHAKE_HEAD);
	/* "We need time to check the ship's position... Let's head for that island
	 */
	/* and get our bearings." */
	scTolbiShipDeckSay(
		SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT));
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 204, 536);
	fdActorFaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 182, 548);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 182, 592);
	fdActorWalkTo(ACTOR_SEA_KAJA, 182, 664);
	fdEventWait(40);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_ISLAND);
}

/* At anchor off the island, talking to Sean or Ouranos, the party is asked */
/* to scout the island for the arguing tour group. */
static void scTolbiShipDeckOfferToScoutIsland(void) {
	ACTOR *leader;

	fdEventStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 180, 654);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorWalkTo(ACTOR_GARET, 194, 640);
	fdActorWalkTo(ACTOR_IVAN, 198, 654);
	fdActorWalkToAndWait(ACTOR_MIA, 194, 672);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	scTolbiShipDeckTurn(ACTOR_MIA, FACING_WEST);
	scTolbiShipDeckTurn(ACTOR_SEA_SEAN, FACING_EAST);
	fdEventSetMessageId(MsgTolbiShipIsaacTalkedPassengersDidntTour);
	/* "Isaac, you talked to the passengers, didn't you? The tour group is
	 * arguing */
	/* about whether they should leave the ship." */
	scTolbiShipDeckSay(ACTOR_SEA_SEAN);
	scTolbiShipDeckTurn(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST);
	/* "We'd like to go ashore and see if it's safe out there... But we promised
	 */
	/* we'd protect the ship." */
	fdEventShowMessageThenWait(ACTOR_SEA_OURANOS, 0, 40);
	fdActorShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorRunRepeatedMotion(ACTOR_SEA_SEAN, 1);
	/* "...That's it! Isaac! Can you go check out the island and see if there
	 * are */
	/* any monsters?" */
	fdEventOpenMessage(ACTOR_SEA_SEAN, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		/* "The passengers all seem to be upset, so let's try to do as they
		 * ask." */
		scTolbiShipDeckSay(ACTOR_IVAN);
		scTolbiShipDeckTurn(ACTOR_MIA, FACING_NORTHWEST);
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
		/* "That's right. We have to do something, or there'll be no resolving
		 */
		/* this." */
		scTolbiShipDeckSay(ACTOR_MIA);
		scTolbiShipDeckTurn(ACTOR_GARET, FACING_SOUTHWEST);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		/* "It would be fun to explore the island! What do you say, Isaac?" */
		fdEventOpenMessage(ACTOR_GARET, 0);
		for(;;) {
			if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
				fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
				fdEventSetMessageId(MsgTolbiShipIsaacDontTalkLikeShouldnt);
				/* "Isaac, don't talk like that. Shouldn't we listen to them?"
				 */
				fdEventOpenMessage(ACTOR_IVAN, 0);
			}
			else
				break;
		}
	}
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SEA_SEAN, ANIM_NOD);
	fdEventSetMessageId(MsgTolbiShipSeeYoureGoingGoFor);
	/* "I see, you're going to go for me, aren't you? I'm sure no one will */
	/* complain if we scout out the island." */
	scTolbiShipDeckSay(ACTOR_SEA_SEAN);
	fdActorSetSpeed(ACTOR_SEA_SEAN, 1);
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 1);
	fdActorGet(ACTOR_SEA_SEAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SEA_SEAN, 162, 634);
	fdEventWait(1);
	fdActorGet(ACTOR_SEA_SEAN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SEA_OURANOS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 162, 676);
	fdEventWait(1);
	fdActorGet(ACTOR_SEA_OURANOS)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHEAST, 0);
	scTolbiShipDeckTurn(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST);
	/* "There's no telling what kind of monsters are lurking on this island...
	 * Be */
	/* extremely careful!" */
	scTolbiShipDeckSay(ACTOR_SEA_SEAN);
	fdActorWalkTo(ACTOR_GARET, 180, 654);
	fdActorWalkTo(ACTOR_IVAN, 180, 654);
	fdActorWalkToAndWait(ACTOR_MIA, 180, 654);
	fdActorReleaseById(ACTOR_GARET);
	fdActorReleaseById(ACTOR_IVAN);
	fdActorReleaseById(ACTOR_MIA);
	gmFlagSet(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED);
	fdEventEnd();
}

/* Back on deck from the island, the party is asked whether it explored it. */
static void scTolbiShipDeckReturnFromIsland(void) {
	fdEventStart();
	fdEventAppendActorPlacements(
		(ACTOR_PLACE *)TolbiShipDeck_TourGuideActorPlacements);
	sySynch(1);
	fdEventStartScrTransition();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 148, 656);
	fdActorShowEmote(ACTOR_SEA_SEAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_SEA_SEAN, 1);
	scTolbiShipDeckTurn(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHWEST);
	fdEventSetMessageId(MsgTolbiShipHowWasIsaacDidExplore);
	/* "How was it, Isaac? Did you explore the island thoroughly?" */
	fdEventOpenMessage(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT), 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		/* "If we don't decide if we can go ashore, they'll never stop whining.
		 * ...I */
		/* hate to ask you, but would you go exploring one more time?" */
		scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT));
		fdEventEnd();
		return;
	}
	FieldStateWork->msg++;
	/* "Well... Did you see any monsters out there?" */
	/* Yes: "...I see. Then we can't let the tourists go ashore after all?" */
	/* No: "If there aren't any monsters, I guess we can let the tourists out!"
	 */
	fdEventAskYesNo(SPEAKER(ACTOR_SEA_SEAN, SPEAKER_PORTRAIT_RIGHT), 0);
	scTolbiShipDeckOpenFirstDoor();
	fdActorSetPos(ACTOR_TOUR_GUIDE_AT_ANCHOR, 216, 588);
	fdActorSetSpeed(ACTOR_TOUR_GUIDE_AT_ANCHOR, 1.2);
	fdActorWalkToAndWait(ACTOR_TOUR_GUIDE_AT_ANCHOR, 216, 596);
	fdActorWalkToAndWait(ACTOR_TOUR_GUIDE_AT_ANCHOR, 188, 616);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SEA_OURANOS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SEA_SEAN, FACING_NORTH_NORTHEAST, 0);
	scTolbiShipDeckTurn(ACTOR_TOUR_GUIDE_AT_ANCHOR, FACING_SOUTH_SOUTHWEST);
	fdActorJump(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2, 0);
	fdActorSetAnimation(ACTOR_TOUR_GUIDE_AT_ANCHOR, ANIM_SHAKE_HEAD);
	/* "The passengers are going stir crazy! They're out of control! You're
	 * going */
	/* to have to let the passengers go ashore." */
	fdEventShowMessage(ACTOR_TOUR_GUIDE_AT_ANCHOR, 0);
	fdActorSetPos(ACTOR_SEA_KAJA, 180, 777);
	fdActorSetSpeed(ACTOR_SEA_KAJA, 1);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 180, 664);
	fdActorFaceDirection(ACTOR_SEA_KAJA, FACING_NORTH_NORTHEAST, 0);
	/* "There's no need for that." */
	scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SEA_SEAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_TOUR_GUIDE_AT_ANCHOR,
					 EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_SEA_KAJA, 1);
	/* "We figured out the course to Tolbi." */
	scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
	fdActorStartRepeatedMotion(ACTOR_SEA_OURANOS, 2);
	/* "Really? We can go?" */
	scTolbiShipDeckSay(ACTOR_SEA_OURANOS);
	fdActorFaceDirection(ACTOR_SEA_KAJA, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimation(ACTOR_SEA_KAJA, ANIM_NOD);
	/* "Let's go to Tolbi before the sea gets any rougher. Tell our tourists
	 * they */
	/* can enjoy a nice, long break after we arrive." */
	scTolbiShipDeckSay(
		SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT));
	fdActorJump(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2, 20);
	fdActorSetAnimation(ACTOR_TOUR_GUIDE_AT_ANCHOR, ANIM_SHAKE_HEAD);
	/* "It's too late... Nothing we can say will calm them down." */
	scTolbiShipDeckSay(ACTOR_TOUR_GUIDE_AT_ANCHOR);
	fdActorWalkToAndWait(ACTOR_SEA_KAJA, 182, 640);
	fdActorFaceDirection(ACTOR_SEA_KAJA, FACING_NORTH_NORTHEAST, 0);
	/* "If we hurry, we can make it to Colosso in time." */
	scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_WINDOW_BELOW));
	fdActorShowEmote(ACTOR_TOUR_GUIDE_AT_ANCHOR,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorStartRepeatedMotion(ACTOR_TOUR_GUIDE_AT_ANCHOR, 2);
	/* "I see! If we can get them to Colosso, they'll all be happy." */
	scTolbiShipDeckSay(ACTOR_TOUR_GUIDE_AT_ANCHOR);
	fdActorSetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
	scTolbiShipDeckTurn(ACTOR_SEA_SEAN, FACING_EAST);
	fdActorRunRepeatedMotion(ACTOR_SEA_SEAN, 1);
	/* "All right then, go and tell the tourists." */
	scTolbiShipDeckSay(ACTOR_SEA_SEAN);
	fdActorSetSpeed(ACTOR_SEA_SEAN, 1.6);
	fdActorSetScript(ACTOR_SEA_SEAN, TolbiShipDeck_GoBelowActions);
	fdActorSetSpeed(ACTOR_SEA_OURANOS, 1.6);
	fdActorWalkToAndWait(ACTOR_SEA_OURANOS, 168, 632);
	fdActorSetScript(ACTOR_SEA_OURANOS, TolbiShipDeck_GoBelowActions);
	fdEventWait(80);
	fdActorSetScript(ACTOR_TOUR_GUIDE_AT_ANCHOR, TolbiShipDeck_GoBelowActions);
	fdEventWait(40);
	scTolbiShipDeckTurn(ACTOR_SEA_KAJA, FACING_WEST);
	/* "It's not much farther. I'll ask the oarsmen to work 'til the very end."
	 */
	scTolbiShipDeckSay(SPEAKER(ACTOR_SEA_KAJA, SPEAKER_PORTRAIT_RIGHT));
	scTolbiShipDeckTurn(ACTOR_PARTY_LEADER, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SEA_KAJA, ANIM_NOD);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(TOLBI_SHIP_DECK_TO_SHIP_ROOMS_SWITCH_TEAMS);
}

/* Places the leader on the deck facing south, with the camera on them. */
static void scTolbiShipDeckPlaceLeaderOnDeck(void) {
	fdCameraMoveTo(232, CAMERA_KEEP, 676, 0);
	fdMapGridCenterOnFollowedPoint();
	fdActorSetPos(ACTOR_PARTY_LEADER, 232, 676);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_SOUTH;
	sySynch(1);
}

/* At Kalay, as the crew's talk of the delay ends, Kaja walks off and is */
/* hidden. */
const s32 TolbiShipDeck_KajaLeavesActions[] = {
	SC_ANIMATION(ANIM_WALK),  SC_MOVE(202, 0, 600), SC_SYNC,
	SC_ANIMATION(ANIM_STAND), SC_POSITION(0, 0, 0), SC_END,
};

/* At the same moment the captain steps aside and turns west. */
const s32 TolbiShipDeck_CaptainStepsAsideActions[] = {
	SC_WAIT(20), SC_ANIMATION(ANIM_WALK),  SC_MOVE(162, 0, 666),
	SC_SYNC,	 SC_ANIMATION(ANIM_STAND), SC_SET(SW_FACING, FACING_WEST),
	SC_END,
};

/* The crew member's and the tour guide's impatience at Kalay. */
/* */
/* They squash and stretch on the spot, the wait between in their timer. */
const s32 TolbiShipDeck_SquashAndStretchActions[] = {
	SC_CALL(scSquashAndStretch),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A first-attack monster runs in, leaps over the rail onto the deck and */
/* goes below by the carved door at the north end. */
/* */
/* It is used at TOLBI_SHIP_DECK_FROM_SHIP_ROOMS_MONSTERS_BOARD. */
const s32 TolbiShipDeck_MonsterLeapsAboardActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(140, 0, 586),
	SC_SYNC,
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_MOVE(184, 0, 598),
	SC_SYNC,
	SC_SET(SW_TIMER, 1),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_CALL(scTolbiShipDeckShowSpr),
	SC_MOVE(216, 0, 596),
	SC_SYNC,
	SC_MOVE(216, 0, 586),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

/* Sean on guard at sea, pacing between two spots on the deck and looking */
/* out at each. */
const s32 TolbiShipDeck_SeanPacesActions[] = {
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

/* Ouranos on guard at sea, walking the four corners of a square on the deck */
/* and looking out at each. */
const s32 TolbiShipDeck_OuranosPatrolsActions[] = {
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

/* The second attack's hovering monsters (ACTOR_SEA_FIRST_HOVERING_MONSTER). */
/* */
/* Their scratch fields are cleared, then TolbiShipDeck_HoverAndDiveAtOuranos */
/* or TolbiShipDeck_HoverAndDiveAtSean runs each frame. */
const s32 TolbiShipDeck_HoverAndDiveAtOuranosActions[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_TIMER, 0),
	SC_SET(SW_USER_VALUE, 0),
	SC_SET(SW_SCENE_COUNTER, 0),
	SC_SET(23, 0),
	SC_CALL(TolbiShipDeck_HoverAndDiveAtOuranos),
};

const s32 TolbiShipDeck_HoverAndDiveAtSeanActions[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_TIMER, 0),
	SC_SET(SW_USER_VALUE, 0),
	SC_SET(SW_SCENE_COUNTER, 0),
	SC_SET(23, 0),
	SC_CALL(TolbiShipDeck_HoverAndDiveAtSean),
};

/* The second attack's boarding monsters, bobbing in the air */
/* (scTolbiShipDeckBob). */
const s32 TolbiShipDeck_BobActions[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_USER_VALUE, 0),
	SC_CALL(scTolbiShipDeckBob),
};

const s32 TolbiShipDeck_LookAroundActions[] = {
	SC_CALL(scTolbiShipDeckLookAround),
};

/* ACTOR_SEA_SPLASH swells from 7/10 of its size for 24 frames, then is */
/* hidden. */
const s32 TolbiShipDeck_SwellActions[] = {
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_SCALE_X, 7 * FIXED_ONE / 10),
	SC_SET(SW_SCALE_Y, 7 * FIXED_ONE / 10),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 3932),
	SC_ADD(SW_SCALE_Y, 2 * FIXED_ONE / 25),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

/* A monster on deck goes below by the carved door at the north end and is */
/* hidden. */
const s32 TolbiShipDeck_MonsterGoesBelowActions[] = {
	SC_MOVE(216, 0, 586),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

/* The third attack's leaping monsters (ACTOR_SEA_FIRST_LEAPING_MONSTER). */
/* */
/* Their timer is cleared, then TolbiShipDeck_LeapAtOuranos or */
/* TolbiShipDeck_LeapAtSean runs each frame. */
const s32 TolbiShipDeck_LeapAtOuranosActions[] = {
	SC_SET(SW_TIMER, 0),
	SC_CALL(TolbiShipDeck_LeapAtOuranos),
};

const s32 TolbiShipDeck_LeapAtSeanActions[] = {
	SC_SET(SW_TIMER, 0),
	SC_CALL(TolbiShipDeck_LeapAtSean),
};

/* The third attack's boarding monsters, hopping at random. */
const s32 TolbiShipDeck_HopActions[] = {
	SC_CALL(scTolbiShipDeckRandomlyStartActorHop),
};

/* At Tolbi, a passenger who rowed walks away once they have spoken, and is */
/* hidden. */
const s32 TolbiShipDeck_RowerLeavesActions[] = {
	SC_ANIMATION(ANIM_WALK),  SC_MOVE(82, 0, 656),	SC_SYNC,
	SC_ANIMATION(ANIM_STAND), SC_POSITION(0, 0, 0), SC_END,
};

/* Back from the island, Sean, Ouranos and the tour guide go below by the */
/* carved door at the north end, one after another. */
const s32 TolbiShipDeck_GoBelowActions[] = {
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

/* Sean standing by, turning now and then (scTolbiShipDeckIdleTurn). */
const s32 TolbiShipDeck_SeanLooksAboutActions[] = {
	SC_SET(SW_SCENE_COUNTER, 0),
	SC_CALL(scTolbiShipDeckIdleTurn),
};

const SCENE_ENTRANCE TolbiShipDeckEntrances[] = {
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

/* clang-format off */
const SCENE_EXIT_REGION TolbiShipDeckRegions[] = {
    {208, 24, 634, 224, 40, 650, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_SHIP_MAST},
    {SCENE_TABLE_END},
};

const u32 TolbiShipDeckExits[] = {
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

const ACTOR_PLACE TolbiShipDeckActorPlacements[] = {
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

/* UB: a table the event writes (its talkFacing), defined const and kept with */
/* the constants; UBFIX makes it writable. */
#ifdef UBFIX
ACTOR_PLACE TolbiShipDeckActorPlacementsWaiting[] = {
#else
const ACTOR_PLACE TolbiShipDeckActorPlacementsWaiting[] = {
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

const ACTOR_PLACE TolbiShipDeckActorPlacementsRowing[] = {
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

const ACTOR_PLACE TolbiShipDeckActorPlacementsAtSea[] = {
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

const ACTOR_PLACE TolbiShipDeck_CrewActorPlacements[] = {
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

const ACTOR_PLACE TolbiShipDeck_TourGuideActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

const ACTOR_PLACE TolbiShipDeck_MonstersBoardActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

const ACTOR_PLACE TolbiShipDeck_SecondAttackActorPlacements[] = {
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

const ACTOR_PLACE TolbiShipDeck_ThirdAttackActorPlacements[] = {
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

const ACTOR_PLACE TolbiShipDeck_KrakenActorPlacements[] = {
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

const ACTOR_PLACE TolbiShipDeckActorPlacementsArrived[] = {
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

const TRIGGER TolbiShipDeckTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
    { TRIGGER_EXIT, TOLBI_SHIP_DECK_TO_KALAY_DOCKS, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_KALAY_DOCKS },
    { TRIGGER_TALK, ACTOR_KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckThereAreALotOfPassengers) },
    { TRIGGER_TALK, ACTOR_KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckOhAtTheRiskOfSounding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREWMAN, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, MSGNO(MsgTolbiShipDeckTheReplacementShipFromTolbiShould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KALAY_CREWMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIHidTheAnchorCharmNo) },
    { TRIGGER_TALK, ACTOR_KALAY_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeApologizeForAnyTroubleThat) },
    { TRIGGER_TALK, ACTOR_KALAY_CREW_MEMBER, CONDITION_ALWAYS, EVENT(scTolbiShipDeckTalkToCrewMember) },
    { TRIGGER_TALK, ACTOR_KALAY_TOUR_GUIDE, CONDITION_ALWAYS, EVENT(scTolbiShipDeckTalkToTourGuide) },
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
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SHIP_DECK_BY_CREW, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT(scTolbiShipDeckWatchCrewDecideToSail) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SHIP_DECK_WEST_OF_CABIN_DOOR, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT(scTolbiShipDeckCatchCrewmanFromWest) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SHIP_DECK_EAST_OF_CABIN_DOOR, FLAG_TOLBI_SHIP_CREWMAN_CAUGHT, EVENT(scTolbiShipDeckCatchCrewmanFromEast) },
    { TRIGGER_TABLE_END },
};

const TRIGGER TolbiShipDeckTriggersAtSea[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
    { TRIGGER_TALK, ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering) },
    { TRIGGER_TALK, ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItLooksLikeAnotherOarsmanWas) },
    { TRIGGER_TALK, ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIFeelSorryForTheOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAnOarsmanWasInjuredByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeHaveNoIdeaWhenThose) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUhnnnWhenILookAtThe) },
    { TRIGGER_TABLE_END },
};

const TRIGGER TolbiShipDeckTriggersArrived[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
    { TRIGGER_EXIT, TOLBI_SHIP_DECK_TO_TOLBI_DOCKS, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_TOLBI_DOCKS },
    { TRIGGER_TALK, ACTOR_ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouLookinForSeanAndOuranos) },
    { TRIGGER_TALK, ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, EVENT(scTolbiShipDeckAskHeadedForColosso) },
    { TRIGGER_TALK, ACTOR_ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisGroupThatCameOnOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARRIVED_KAJA, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantSeanAndOuranosTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARRIVED_ASKS_ABOUT_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckImJustGoingToHangOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARRIVED_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWonderWhatHappenedToThat) },
    { TRIGGER_TABLE_END },
};

const TRIGGER TolbiShipDeckTriggersAtIsland[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_PASSENGER_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#if !TBS_PATCH_2
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
#else
    { TRIGGER_STEP, TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CRATE_HOLD, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelowWhileFacingWest) },
#endif
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_SHIP_DECK_TO_SHIP_ROOMS_CAPTAINS_ROOM, CONDITION_ALWAYS, EVENT(scTolbiShipDeckGoBelow) },
    /* At anchor the west gangplank's cells lead to Crossbone Isle. */
    { TRIGGER_EXIT, TOLBI_SHIP_DECK_TO_TOLBI_DOCKS, CONDITION_ALWAYS, TOLBI_SHIP_DECK_TO_WORLD_MAP_CROSSBONE_ISLE },
    { TRIGGER_TALK, ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckKajaAndTheCaptainAreProfessionals) },
    { TRIGGER_TALK, ACTOR_SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT(scTolbiShipDeckOfferToScoutIsland) },
    { TRIGGER_TALK, ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckTheresNoTellingWhatKindOf) },
    { TRIGGER_TALK, ACTOR_SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT(scTolbiShipDeckOfferToScoutIsland) },
    { TRIGGER_TALK, ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeveDoneAGoodJobOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_CREW_MEMBER, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantToBeASailor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_SEAN, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckIGiveUpWhatAreWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_SEAN, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisIslandGivesMeTheCreeps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OURANOS, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckThatsEnoughBickering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OURANOS, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWhatWeCanDoNowIs) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */

/* How hard each passenger rows, from ACTOR_ARRIVED_FIRST_PASSENGER on. */
/* */
/* An empty seat's oars take OAR_STROKE_STEADY */
/* (scTolbiShipDeckChooseOarStrokes). */
const s32 TolbiShipDeckRowerStrokes[] = {
	OAR_STROKE_STEADY, OAR_STROKE_SLOW,	 OAR_STROKE_STEADY,
	OAR_STROKE_STILL,  OAR_STROKE_FAST,	 OAR_STROKE_FAST,
	OAR_STROKE_SLOW,   OAR_STROKE_STILL, OAR_STROKE_STILL};
