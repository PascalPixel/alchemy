/* Trial Road, the mountain path above the Shaman Village where Moapa tests the
   party before he will hand over the Hover Jade. Felix races Moapa and his two
   knights up four stages, by the left path or the right. Each stage is one of
   eight puzzle rooms (logs to roll, stakes to Pound, weeds to blow away, frozen
   puddles, crumbling pillars, fountains, a sand stream), dealt in an order that
   depends on the letters of the leader's name. At the top of each stage the
   switch in front of the door opens the blue chests beside it, and the door
   opens once each open chest holds a piece of the party's equipment: one chest,
   or two when Moapa got there first. A button at the top offers to give up.
   Moapa's party waits at the summit, or arrives after the party, and fights
   them for the Jade. Long after, Hover on the summit's purple tile reaches a
   cave whose rock Lift raises, and beyond it a pool with weeds hiding a Mad
   Plant, a hidden tile Reveal shows, an Elixir chest and Gasp the Djinni, who
   moves opposite to Felix. */
#include "item_ids.h"
#include "macros.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "scene/ground_effects.h"
#include "scene/scene.h"
#include "system/field_effect.h"
#include "window/window.h"
#include "text/msg_ids.h"

/* The map metatiles the cue tables answer, by the behaviour the map gives them.
   The race's frame, the same in every stage: the stairs at the foot of each
   path and the door at its top, the give-up buttons and the switches before the
   doors (west and east of the map's middle), each with the ring of metatiles
   around it that steps off it. */
typedef enum TRIAL_ROAD_METATILE {
	METATILE_LEFT_PATH_STAIRS_DOWN = 1,
	METATILE_RIGHT_PATH_STAIRS_DOWN = 2,
	METATILE_LEFT_PATH_DOOR = 3,
	METATILE_RIGHT_PATH_DOOR = 4,
	METATILE_AROUND_WEST_GIVE_UP_BUTTON = 10,
	METATILE_WEST_GIVE_UP_BUTTON = 11,
	METATILE_AROUND_EAST_GIVE_UP_BUTTON = 20,
	METATILE_EAST_GIVE_UP_BUTTON = 21,
	METATILE_AROUND_WEST_SWITCH = 30,
	METATILE_WEST_SWITCH = 31,
	METATILE_AROUND_EAST_SWITCH = 40,
	METATILE_EAST_SWITCH = 41,
	/* The pillar stage: the south edge of the upper ledge, over the gap a
	   pillar can be pushed into, and the edge beside it. */
	METATILE_PILLAR_STAGE_GAP = 51,
	METATILE_PILLAR_STAGE_LEDGE = 52,
	/* The torch stage: north and south of each pillar of ice. */
	METATILE_ICE_PILLAR_SIDE = 33,
	/* The weeds stage's ledge, jumped from southward. */
	METATILE_WEEDS_STAGE_LEDGE = 33,
	/* The Pound stage's four stakes, and the ground the pillars stand on. */
	METATILE_WEST_STAKE = 50,
	METATILE_INNER_WEST_STAKE = 51,
	METATILE_INNER_EAST_STAKE = 52,
	METATILE_EAST_STAKE = 53,
	METATILE_STAKE_GROUND = 60,
	/* The crumbling stage: its ledge, the metatiles beside the cracked pillars,
	   and the pillars themselves. */
	METATILE_CRUMBLING_STAGE_LEDGE = 33,
	METATILE_BESIDE_CRUMBLING_PILLAR = 34,
	METATILE_WEST_CRUMBLING_PILLAR = 50,
	METATILE_EAST_CRUMBLING_PILLAR = 51,
	/* The log stage: its ledge over the water, the rows the upper and lower
	   logs roll along, and the two ends of the east log's track. */
	METATILE_LOG_STAGE_LEDGE = 33,
	METATILE_UPPER_LOG_ROWS = 50,
	METATILE_LOWER_LOG_ROWS = 51,
	METATILE_EAST_LOG_NORTH_END = 52,
	METATILE_EAST_LOG_SOUTH_END = 53,
	/* The fountain stage: the column the block can stand in, and the three
	   buried vents. */
	METATILE_BLOCK_TRACK = 33,
	METATILE_WEST_VENT = 50,
	METATILE_MIDDLE_VENT = 51,
	METATILE_EAST_VENT = 52,
	/* The sand stage's stream. */
	METATILE_STREAM = 50,
	/* Gasp's pool: the weeds, and the clump hiding the Mad Plant. */
	METATILE_WEEDS = 33,
	METATILE_WEEDS_HIDING_MAD_PLANT = 34,
	/* The summit: in front of each doorway from the stages, and the metatiles
	   that bring on Moapa. */
	METATILE_SUMMIT_DOORWAY = 33,
	METATILE_LEFT_PATH_SUMMIT = 40,
	METATILE_RIGHT_PATH_SUMMIT = 41,
} TRIAL_ROAD_METATILE;

/* Trial Road's Mad Plant, the last of the five hidden in weeds: its
   FLAG_MAD_PLANT and its column of ENCOUNTER_ZONE_MAD_PLANTS. */
#define MAD_PLANT_TRIAL_ROAD			   4
/* Set once the summit's stone pillar has been moved one metatile east, into
   column 8 of the map, where it stays. */
#define FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED 0x9fc

/* Set while Felix stands up on the pillar stage's upper ledge or a pillar's top
   (scTrialRoadCheckLeaderOnPillars). */
#define FLAG_LEADER_ON_PILLARS				  FLAG_SCENE_TEMP_08
/* Set once the give-up button has asked; stepping off it clears it. */
#define FLAG_GIVE_UP_ASKED					  FLAG_SCENE_TEMP_81
/* Set once both blue chests hold their items and the leader's door opens. */
#define FLAG_STAGE_DOOR_OPEN				  FLAG_SCENE_TEMP_82
/* Set once Felix has stood on the switch at the top of the stage and the blue
   chests have opened (scTrialRoadOpenChests). */
#define FLAG_STAGE_CHESTS_OPENED			  FLAG_SCENE_TEMP_84
/* Set while the inner blue chest stands open for an item. */
#define FLAG_INNER_CHEST_OPEN				  FLAG_SCENE_TEMP_85
/* Set while the outer blue chest stands open for an item: it opens only when
   Moapa reached the door first. */
#define FLAG_OUTER_CHEST_OPEN				  FLAG_SCENE_TEMP_86
/* Set once the doorway Felix came out of at the summit has shut behind him
   (scTrialRoadShutSummitDoorway). */
#define FLAG_SUMMIT_DOORWAY_SHUT			  FLAG_SCENE_TEMP_83
/* Set once Lift has raised the rock at the summit cave's mouth. */
#define FLAG_CAVE_ROCK_LIFTED				  FLAG_SCENE_TEMP_89
/* Set once Gasp has stopped mirroring Felix and hopped away to where he can be
   talked to. */
#define FLAG_GASP_FLED						  FLAG_SCENE_TEMP_88
/* The link race's flags, which no code in the game sets, as nothing chooses
   RIVAL_LINKED: the byte of flags from FLAG_LINK_PLAYER holds this player's
   number in the link, so the rival follows the other player's record; while
   FLAG_LINKED_RIVAL_HALTS is set, a rival whose record has its top byte set
   stops 12 pixels beside the course's corner, 16 pixels north of it with the
   far side's timer if FLAG_LINKED_RIVAL_ON_FAR_SIDE is set and 16 south with
   the near side's if not. */
#define FLAG_LINK_PLAYER					  FLAG_SCENE_TEMP_F8
#define FLAG_LINKED_RIVAL_HALTS				  FLAG_SCENE_TEMP_11
#define FLAG_LINKED_RIVAL_ON_FAR_SIDE		  FLAG_SCENE_TEMP_12
/* Set once Whirlwind has blown away the weeds on the pillar stage's pillar C,
   on the Pound stage's pillar B, and on the sand stage's cliff. */
#define FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY	  FLAG_AREA_TEMP_07
#define FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY	  FLAG_AREA_TEMP_39
#define FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY	  FLAG_AREA_TEMP_74
/* Set while the Pound stage's inner west stake stands raised and the west one
   sunk; clear the other way round. */
#define FLAG_INNER_WEST_STAKE_RAISED		  FLAG_AREA_TEMP_35
/* Set while the Pound stage's east stake stands raised and the inner east one
   sunk; clear the other way round. */
#define FLAG_EAST_STAKE_RAISED				  FLAG_AREA_TEMP_37
/* The four stages' flags, from FLAG_AREA_TEMP_81: each set once Moapa reached
   that stage's door before Felix. */
#define FLAG_RIVAL_FIRST_AT_DOOR_BASE		  FLAG_AREA_TEMP_81
/* Each cracked pillar of the crumbling stage: set once Felix has first stepped
   on it and it has flashed, and once it has crumbled away. */
#define FLAG_WEST_CRUMBLING_PILLAR_STEPPED_ON FLAG_AREA_TEMP_42
#define FLAG_WEST_CRUMBLING_PILLAR_CRUMBLED	  FLAG_AREA_TEMP_43
#define FLAG_EAST_CRUMBLING_PILLAR_STEPPED_ON FLAG_AREA_TEMP_44
#define FLAG_EAST_CRUMBLING_PILLAR_CRUMBLED	  FLAG_AREA_TEMP_45
/* Set once Felix climbs to the summit ahead of Moapa. */
#define FLAG_PARTY_FIRST_AT_PEAK			  FLAG_AREA_TEMP_85
/* Set once the stages' order has been dealt from the leader's name. */
#define FLAG_STAGE_ORDER_DEALT				  FLAG_AREA_TEMP_80

#include "shared/trial_road.h"

/* The actors every stage room places first, numbering the rows of each stage's
   table: Moapa, the four blue chests (two on the left path, two on the right,
   inner and outer) and Moapa's two knights. A stage's own puzzle pieces follow
   from ACTOR_FIRST_STAGE_PIECE. */
typedef enum TRIAL_ROAD_STAGE_ACTOR_ID {
	ACTOR_MOAPA = ACTOR_FIRST_PLACED,
	ACTOR_LEFT_INNER_CHEST,
	ACTOR_LEFT_OUTER_CHEST,
	ACTOR_RIGHT_INNER_CHEST,
	ACTOR_RIGHT_OUTER_CHEST,
	ACTOR_LEFT_KNIGHT,
	ACTOR_RIGHT_KNIGHT,
	ACTOR_FIRST_STAGE_PIECE
} TRIAL_ROAD_STAGE_ACTOR_ID;

/* Kraden, at the summit (TrialRoad_SummitPlacements), after Moapa. */
#define ACTOR_KRADEN (ACTOR_MOAPA + 1)

/* The log stage's three logs (TrialRoad_LogStagePlacements): the upper and
   lower logs lie north to south and roll east or west, and the east log lies
   across them and rolls north or south. */
typedef enum TRIAL_ROAD_LOG_STAGE_ACTOR_ID {
	ACTOR_UPPER_LOG = ACTOR_FIRST_STAGE_PIECE + 1,
	ACTOR_LOWER_LOG,
	ACTOR_EAST_LOG
} TRIAL_ROAD_LOG_STAGE_ACTOR_ID;

/* The Pound stage's four cone-shaped stakes (TrialRoad_PoundStagePlacements),
   west to east at metatiles (13, 26), (15, 26), (17, 26) and (19, 26), in two
   pairs: driving one down raises its partner. */
typedef enum TRIAL_ROAD_POUND_STAGE_ACTOR_ID {
	ACTOR_WEST_STAKE = ACTOR_FIRST_STAGE_PIECE + 3,
	ACTOR_INNER_WEST_STAKE,
	ACTOR_INNER_EAST_STAKE,
	ACTOR_EAST_STAKE
} TRIAL_ROAD_POUND_STAGE_ACTOR_ID;

/* The crumbling stage's two pillars (TrialRoad_CrumblingStagePlacements). */
typedef enum TRIAL_ROAD_CRUMBLING_STAGE_ACTOR_ID {
	ACTOR_WEST_CRUMBLING_PILLAR = ACTOR_FIRST_STAGE_PIECE,
	ACTOR_EAST_CRUMBLING_PILLAR
} TRIAL_ROAD_CRUMBLING_STAGE_ACTOR_ID;

/* The stone pillars most stages stand from ACTOR_FIRST_STAGE_PIECE: five, or
   six in the torch and Pound stages; the log and fountain stages have one, the
   block. */
typedef enum TRIAL_ROAD_PILLAR_ACTOR_ID {
	ACTOR_PILLAR_A = ACTOR_FIRST_STAGE_PIECE,
	ACTOR_PILLAR_B,
	ACTOR_PILLAR_C,
	ACTOR_PILLAR_D,
	ACTOR_PILLAR_E,
	ACTOR_PILLAR_F
} TRIAL_ROAD_PILLAR_ACTOR_ID;

/* The torch stage's two torches burning on stands, after its six pillars
   (TrialRoad_TorchStagePlacements): pushed next to a frozen puddle, a torch
   melts it. */
typedef enum TRIAL_ROAD_TORCH_STAGE_ACTOR_ID {
	ACTOR_WEST_TORCH = ACTOR_FIRST_STAGE_PIECE + 6,
	ACTOR_EAST_TORCH
} TRIAL_ROAD_TORCH_STAGE_ACTOR_ID;

/* The weeds after the pillars: on pillar C in the pillar stage, on pillars A
   and B in the weeds pillar stage. */
enum { ACTOR_WEEDS_A = ACTOR_FIRST_STAGE_PIECE + 5, ACTOR_WEEDS_B };

/* The weeds on pillar B in the Pound stage, after its four stakes. */
enum { ACTOR_POUND_STAGE_WEEDS = ACTOR_EAST_STAKE + 1 };

/* The weeds on the sand stage's cliff, after its two tightrope posts. */
enum { ACTOR_SAND_STAGE_WEEDS = ACTOR_FIRST_STAGE_PIECE + 2 };

/* The pillar at the summit (TrialRoad_SummitPlacements), after Kraden. */
#define ACTOR_PEAK_PILLAR (ACTOR_KRADEN + 1)

/* Gasp, alone in his room (TrialRoad_GaspPlacements). */
enum { ACTOR_GASP = ACTOR_FIRST_PLACED };

/* The round boulder on the ledge of the cave beyond the summit, which Lift
   raises (TrialRoad_CaveRockPlacements). */
enum { ACTOR_CAVE_ROCK = ACTOR_FIRST_PLACED };

/* The two parts of the cave beyond the summit, each way between them leading in
   by the entrance on its far side: the west cave, where the summit's passage
   comes in and the boulder sits on its ledge, and the east passage north to
   Gasp's pool. */
enum {
	TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE = 1,
	TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE = 2
};

/* The entrances, one for each way above. */
enum {
	TRIAL_ROAD_CAVE_ROCK_WEST_CAVE_FROM_EAST_PASSAGE = 1,
	TRIAL_ROAD_CAVE_ROCK_EAST_PASSAGE_FROM_WEST_CAVE = 2
};

/* The fountain stage's five vents (TrialRoad_FountainStagePlacements): three in
   a row west to east, and two south of them. */
typedef enum TRIAL_ROAD_FOUNTAIN_STAGE_ACTOR_ID {
	ACTOR_WEST_VENT = ACTOR_FIRST_STAGE_PIECE + 1,
	ACTOR_MIDDLE_VENT,
	ACTOR_EAST_VENT,
	ACTOR_SOUTHWEST_VENT,
	ACTOR_SOUTHEAST_VENT
} TRIAL_ROAD_FOUNTAIN_STAGE_ACTOR_ID;

/* The stage room's entrances. Every stage is the same room redrawn: the stairs
   down at a stage's foot lead in at the top of the stage below, and the door at
   its top in at the foot of the next. The Shaman Village enters the first stage
   by its left or right path, at
   TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST or _EAST, and the
   summit returns to the last stage the same way, at
   TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST or _EAST; each goes on to the
   stage entrance of its last digit. While Moapa explains the trial's rules
   (scShamanVillageExplainTrialRules), the village sends the party in at
   TRIAL_ROAD_STAGES_FOR_TRIAL_RULES, which deals a course for
   TRIAL_ROAD_RULES_COURSE to show. The summit is entered again after the battle
   with Moapa, won or lost. */
typedef enum TRIAL_ROAD_ENTRANCE {
	TRIAL_ROAD_STAGE_FOOT_LEFT = 1,
	TRIAL_ROAD_STAGE_FOOT_RIGHT = 2,
	TRIAL_ROAD_STAGE_TOP_LEFT = 3,
	TRIAL_ROAD_STAGE_TOP_RIGHT = 4,
	TRIAL_ROAD_SUMMIT_AFTER_WIN = 4,
	TRIAL_ROAD_SUMMIT_AFTER_DEFEAT = 5,
	TRIAL_ROAD_RULES_COURSE = 55
} TRIAL_ROAD_ENTRANCE;

/* The sand stage's two tightrope posts (TrialRoad_SandStagePlacements). */
enum { ACTOR_WEST_POST = ACTOR_FIRST_STAGE_PIECE, ACTOR_EAST_POST };

/* The torch stage's pillars C to F are frozen puddles, pillars of ice; each
   one's flag, FLAG_SCENE_TEMP_14 to FLAG_SCENE_TEMP_17, is set once a torch has
   melted it to a puddle. */
#define FLAG_PUDDLE_MELTED_BASE	   (FLAG_SCENE_TEMP_14 - ACTOR_PILLAR_C)
/* The weeds pillar stage's two clumps of weeds; each one's flag,
   FLAG_AREA_TEMP_27 and FLAG_AREA_TEMP_28, is set once Whirlwind has blown it
   away. */
#define FLAG_WEEDS_BLOWN_AWAY_BASE (FLAG_AREA_TEMP_27 - ACTOR_WEEDS_A)

/* The log stage's logs' flags, FLAG_SCENE_TEMP_53 to FLAG_SCENE_TEMP_55, set
   once each has rolled off the edge into the water. */
#define FLAG_LOG_IN_WATER_BASE	 (FLAG_SCENE_TEMP_53 - ACTOR_UPPER_LOG)
/* The fountain stage's vents' flags, FLAG_AREA_TEMP_63 to FLAG_AREA_TEMP_65,
   set once each has been uncovered. */
#define FLAG_VENT_UNCOVERED_BASE (FLAG_AREA_TEMP_63 - ACTOR_WEST_VENT)

/* A piece Felix cannot push: a pillar weeds hold down, a frozen puddle, a
   stake, or a log (logs roll by their own scripts). fdLeaderCanPushObjectAhead
   refuses a target with this byte set. */
#define PUSH_BLOCKED(object)   *(Uint8 *)&object->sceneCnt
/* The height in pixels of a pillar's top (scTrialRoadAddPillarTop) above the
   pillar: PILLAR_HEIGHT standing, -PILLAR_HEIGHT lowered. */
#define PILLAR_TOP_HEIGHT(top) *(Uint8 *)&top->sceneByte

/* A fountain rises and falls once each FOUNTAIN_PERIOD frames, rising at
   FOUNTAIN_RISE_AT and falling at FOUNTAIN_FALL_AT. */
#define FOUNTAIN_PERIOD	 240
#define FOUNTAIN_RISE_AT 120
#define FOUNTAIN_FALL_AT 150

/* How far a pillar's top stands above it; once the pillar is lowered, its top
   is as far below it. */
#define PILLAR_HEIGHT 16

/* Talking to Gasp starts his battle. */
void scTrialRoadFightGasp(void)
{
	fdRequestDjinnBattle(ACTOR_GASP, DJINNI_GASP);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE TrialRoad_Placements[];
extern ACTOR_PLACE TrialRoad_CaveRockPlacements[];
extern ACTOR_PLACE TrialRoad_GaspPlacements[];
extern ACTOR_PLACE TrialRoad_SummitPlacements[];
extern Sint32	   TrialRoad_StagesPlacements[];

/* The script of each clump of weeds: it sits on the ground at the foot of its
   pillar or cliff, drawn half again as tall as its picture. */
Sint32 scTrialRoadSettleOnGround(ACTOR *actor)
{
	Sint32 height =
		fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) + toFIXED(2);

	actor->groundHeight = height;
	actor->y.fixed = height;
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	actor->scaleY = toFIXED(3) / 2;
	return 0;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint32 pattern = TrialRoadStateWork->pattern;

	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_STAGES)
		return TrialRoad_StagesPlacements[pattern & 7];
	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_CAVE_ROCK)
		return TrialRoad_CaveRockPlacements;
	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_GASP)
		return TrialRoad_GaspPlacements;
	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_SUMMIT)
		return TrialRoad_SummitPlacements;
	return TrialRoad_Placements;
}

/* Whether Moapa is ahead in the race: by the stages each has cleared, then by
   who reached the switch at the top of this stage first; on a tie, fallback. */
Sint32 scTrialRoadRivalIsAhead(Sint32 fallback)
{
	TRIAL_ROAD_STATE *state = TrialRoadStateWork;

	if(state->leaderBest < state->rivalBest)
		return 1;
	if(state->leaderBest > state->rivalBest)
		return 0;
	if(fdScriptGetFlag(FLAG_STAGE_CHESTS_OPENED))
		return 0;
	if(state->rivalTmr != 0)
		return 1;
	return fallback;
}

Bool scTrialRoadOfferToGiveUp(void);

/* Walking down the stairs at the foot of a stage, or through the open door at
   its top. Down from the first stage during the race, Felix is asked whether he
   gives up, and stays if he does not; up through the fourth door, he climbs to
   the summit. */
void scTrialRoadTakeStairs(Sint32 direction)
{
	TRIAL_ROAD_STATE *state = TrialRoadStateWork;

	if(direction == METATILE_LEFT_PATH_STAIRS_DOWN ||
		direction == METATILE_RIGHT_PATH_STAIRS_DOWN) {
		if(state->stage[0] <= 0) {
			if(fdScriptGetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
#if FIX_FUNCTION_SIGNATURES
				if(scTrialRoadOfferToGiveUp() == 0)
#else
				/* UB: scTrialRoadOfferToGiveUp takes no argument, but this call
				   passes 0. */
				if(((Sint32 (*)(Sint32))scTrialRoadOfferToGiveUp)(0) == 0)
#endif
					fdActorWalkByAndWait(
						MainRoutineStateWork.leaderActorId, 0, -16);
				return;
			}
			/* Out to the trial grounds: the stairs' exits to them are numbered
			   four on from the stairs' metatiles. */
			syImportScriptSetExitRequest(direction + 4);
		} else {
			state->stage[0]--;
			syImportScriptSetExitRequest(direction);
		}
		fdScriptPlaySnd(SE_MAP_EXIT);
	}
	if(direction == METATILE_LEFT_PATH_DOOR ||
		direction == METATILE_RIGHT_PATH_DOOR) {
		state->stage[0]++;
		if(state->stage[0] > 3) {
			if(fdScriptGetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT) &&
				scTrialRoadRivalIsAhead(0) == 0)
				fdScriptSetFlag(FLAG_PARTY_FIRST_AT_PEAK);
			fdScriptPlaySnd(SE_STAIRS_UP);
			fdExitStartStairsUp();
			/* Up to the summit, by the exits numbered four on from the
			   doors'. */
			syImportScriptSetExitRequest(direction + 4);
		} else {
			syImportScriptSetExitRequest(direction);
			fdScriptPlaySnd(SE_MAP_EXIT);
		}
		if(state->stage[0] > state->leaderBest)
			state->leaderBest = state->stage[0];
	}
	state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
}

struct ACTOR;
struct FIELD_SPR;
union FIELD_OBJECT;

/* The sprite's priority bits, read as the byte that holds them. */
typedef struct EFFECT_SPR_BITS {
	Uint8 tileHigh : 2; /* the tile number's top two bits */
	Uint8 pri : 2;
} __attribute__((packed)) EFFECT_SPR_BITS;

/* A sprite's attribute 2, as the byte bits view reaches it. */
typedef struct EFFECT_SPR_ATTR {
	Uint8			padding[9];
	EFFECT_SPR_BITS bits;
} EFFECT_SPR_ATTR;

/* What the rival does each frame (scTrialRoadMoveRival). */
typedef enum TRIAL_ROAD_RIVAL_MODE {
	RIVAL_REPLAY = 1, /* walks the recorded path */
	RIVAL_RECORD = 2, /* records the actor's path */
	/* Unused: stands where the linked player's record puts it */
	RIVAL_LINKED = 3,
	RIVAL_MIRROR = 4,  /* follows the leader mirrored across the course */
	RIVAL_WAITING = 9, /* the path is done */
} TRIAL_ROAD_RIVAL_MODE;

/* The rival's state in SceneStateWork. */
typedef struct TRIAL_ROAD_REPLAY {
	Sint16 mode; /* TRIAL_ROAD_RIVAL_MODE */
	Sint16 mirrored;
	Sint16 id;		 /* the rival's actor */
	Sint16 point;	 /* the next x, z pair of the path */
	Sint16 still;	 /* frames the rival has not moved */
	Sint16 resource; /* the recording's resource */
} TRIAL_ROAD_REPLAY;

/* One queued sprite: the three words syOAMAddSprAtDepth adds to OAM. */
typedef struct TRIAL_ROAD_SPR {
	Uint32 words[3];
} TRIAL_ROAD_SPR;

/* The halfwords of the rival's recorded path: x, z pairs, the last pair 0,
   0. */
#define RIVAL_PATH_LENGTH 7200

/* The tiles of the race map's picture (TrialRoad_RaceMapPicture): a stage of
   the road in blue, 16 by 32 pixels with a 16 by 8 foot below it, and the two
   players' 16 by 16 markers, "1P" and "2P". */
typedef enum TRIAL_ROAD_RACE_MAP_TILE {
	RACE_MAP_TILE_STAGE = 0,
	RACE_MAP_TILE_STAGE_FOOT = 8,
	RACE_MAP_TILE_1P = 10,
	RACE_MAP_TILE_2P = 14,
	RACE_MAP_TILE_COUNT = 18
} TRIAL_ROAD_RACE_MAP_TILE;

/* The bytes of the race map's tiles. */
#define RACE_MAP_PICTURE_BYTES (RACE_MAP_TILE_COUNT * TILE_SIZE_4BPP)
/* The sprite palette the race map loads and draws with. */
#define RACE_MAP_PALETTE	   14
/* The pixels between one stage's picture and the next, down the display. */
#define RACE_MAP_STAGE_SPACING 36

/* The heap slot scTrialRoadShowRaceMap takes for the race map of both parties
   and the rival's recorded path. */
typedef struct TRIAL_ROAD_LINK_STATE {
	TRIAL_ROAD_SPR sprs[18]; /* the display's queued sprites */
	Sint16		   vramEntry;
	Sint16		   fade; /* 0 hidden to 2 shown, a step a frame */
	Sint16		   scrollX;
	Sint16		   scrollY;
	Sint16		   holdFade; /* nonzero holds the fade at 2 */
	Sint16		   leaderId; /* the actor the first marker follows */
	Sint16 rivalId; /* the second marker's, which the recording replays */
	/* The rival's timer in the link race, beside the far and the near side;
	   nothing in the overlay writes them. */
	Uint16 rivalTmrs[2];
	Sint16 resource; /* the recorded path's resource, once decoded */
	Sint32 originX;	 /* the course's corner the markers measure from */
	Sint32 originZ;
	Sint16 path[RIVAL_PATH_LENGTH]; /* x, z pairs in pixels, 0, 0 ending them */
} TRIAL_ROAD_LINK_STATE;

Sint32 scTrialRoadIsMirrored(void);

void scTrialRoadSetJumpMetatileUnder(Sint32 id);
void scTrialRoadClearJumpMetatileUnder(Sint32 id);
void scTrialRoadAddPillarTop(Sint32 id);
void scTrialRoadSetPillarTopLift(Sint32 id, Sint32 value);
void scTrialRoadOpenVentMetatile(Sint32 id);
void scTrialRoadCheckLeaderOnPillars(void);
void scTrialRoadCheckStakesCovered(Sint32 unused, Sint32 id);
void scTrialRoadPoundStake(Sint32 id, Sint32 phase, Sint32 frame);
void scTrialRoadStepOntoStake(
	Sint32 id, Sint32 other, Sint32 flag, Sint32 side);
void scTrialRoadPushLog(Sint32 id, Sint32 dx, Sint32 dz, Sint32 axis);
void scTrialRoadUncoverVent(Sint32 id, Sint32 phase, Sint32 frame);
void scLeaderCheckCrumblingStep(const Uint16 *table);
void scTrialRoadCrumblePillar(const void *data, Sint32 id);

void scTrialRoadClearGiveUpAsked(void);

void scTrialRoadBlowAwayPillarStageWeeds(void);

void scTrialRoadJumpAcrossGap(void);

void scTrialRoadJumpMeltedPuddle(void);

void scTrialRoadStepOntoWestStake(void);

void scTrialRoadStepOntoInnerWestStake(void);

void scTrialRoadStepOntoInnerEastStake(void);

void scTrialRoadStepOntoEastStake(void);

void scTrialRoadLandOnPillar(void);

void scTrialRoadCheckCrumblingPillars(void);

void scTrialRoadPushUpperLogWest(void);

void scTrialRoadPushUpperLogEast(void);

void scTrialRoadPushLowerLogWest(void);

void scTrialRoadPushLowerLogEast(void);

void scTrialRoadPushEastLogNorth(void);

void scTrialRoadPushEastLogSouth(void);

void scTrialRoadPushEastLogSouthOrClimbDown(void);

void scTrialRoadStartSand(void);

void scTrialRoadEndSand(void);

void scTrialRoadBlowAwaySandStageWeeds(void);

void scTrialRoadDriftWithStream(void);

void scTrialRoadLiftCaveRock(void);

void scTrialRoadScatterWeeds(void);

void scTrialRoadScatterWeedsHidingMadPlant(void);

void scTrialRoadRevealPoolTiles(void);
void scTrialRoadHidePoolTiles(void);

void scTrialRoadChallengeMoapa(void);

extern ACTOR_PLACE TrialRoad_PillarStagePlacements[];
extern ACTOR_PLACE TrialRoad_TorchStagePlacements[];
extern ACTOR_PLACE TrialRoad_WeedsPillarStagePlacements[];
extern ACTOR_PLACE TrialRoad_PoundStagePlacements[];
extern ACTOR_PLACE TrialRoad_CrumblingStagePlacements[];
extern ACTOR_PLACE TrialRoad_LogStagePlacements[];
extern ACTOR_PLACE TrialRoad_FountainStagePlacements[];
extern ACTOR_PLACE TrialRoad_SandStagePlacements[];
extern CUE		   TrialRoad_PillarStageCues[];
extern CUE		   TrialRoad_TorchStageCues[];
extern CUE		   TrialRoad_WeedsPillarStageCues[];
extern CUE		   TrialRoad_PoundStageCues[];
extern CUE		   TrialRoad_CrumblingStageCues[];
extern CUE		   TrialRoad_LogStageCues[];
extern CUE		   TrialRoad_FountainStageCues[];
extern CUE		   TrialRoad_SandStageCues[];

void scTrialRoadIdle(void);

Sint32 scTrialRoadRivalIsAhead(Sint32 fallback);

extern Sint16 TrialRoad_CrumblingPillars[];

void scTrialRoadReachPeakFirst(void);
void scTrialRoadReachPeakSecond(void);

void scTrialRoadLowerStake(Sint32 id);

void scTrialRoadRaiseStake(Sint32 id);

void scTrialRoadBlockLogs(void);

void scTrialRoadStartFountains(Sint32 unused, Sint32 shape);

extern Uint32 TrialRoad_GaspHopScript[];

/* The summit's colours as the cycle loads them
   (scTrialRoadStartPaletteCycle). */
Uint16 TrialRoad_PaletteCycleColors[16] BSS_DATA;

void scTrialRoadStepPaletteCycle(void);
void scTrialRoadShutSummitDoorway();
void scTrialRoadPlaceMoapaAtPeak(void);
void scTrialRoadBeatMoapa(void);
void scTrialRoadLoseToMoapa(void);
void scTrialRoadShowTraps(void);

void scTrialRoadSetUpCave(void);

void scTrialRoadSetUpPoolRoom(void);

extern Uint32 TrialRoad_RivalRecordings[];
Sint32		  scTrialRoadRivalIsWaiting(void);
void		  scTrialRoadPlayRivalRecording(Sint32 resource);
void		  scTrialRoadDealStageOrder(Uint32 seed);

void scTrialRoadShowRaceMap(Sint32 leaderId, Sint32 rivalId);

/* Opens the door at the top of Felix's stage. */
void scTrialRoadOpenLeaderDoor(Sint32 side, Sint32 phase);

extern Uint32 TrialRoad_LeftKnightEntersScript[];

extern Uint32 TrialRoad_RightKnightEntersScript[];

extern Uint32 TrialRoad_LeftKnightToChestScript[];
extern Uint32 TrialRoad_RightKnightToChestsScript[];

extern Uint32 TrialRoad_RightKnightLeavesScript[];

void scDustScatter(Sint32 value);
void fdLeaderSetMetatileShapeAt(Sint32 offset, Sint32 layer);
void scMagmaRockInsideSpawnDustRing(void);
void fdActorSetMetatileShape(Sint32 level, Sint32 layer, Sint32 id);

void scTrialRoadDriftDust(FIELD_OBJECT *object);

/* The race map's colours. */
extern const Uint16 TrialRoad_RaceMapPalette[];

void scTrialRoadMoveRival(void);
void scTrialRoadSetUpPillarStage(void);
void scTrialRoadSetUpTorchStage(void);
void scTrialRoadSetUpWeedsPillarStage(void);
void scTrialRoadSetUpPoundStage(void);
void scTrialRoadSetUpCrumblingStage(void);
void scTrialRoadSetUpLogStage(void);
void scTrialRoadSetUpFountainStage(void);

void scTrialRoadSetUpSandStage(void);

void scTrialRoadUpdateFountains(void);

/* Defines a function that opens a stage's door at the top: on the left path
   (side 0) the door at column leftX, on the right the one at rightX, redrawn
   open from the spare metatiles, with the upper layer's at upperLeftX or
   upperRightX and the ground of the metatile at column groundLeftX or
   groundRightX of row 127. */
#define DEFINE_TRIAL_ROAD_OPEN_DOOR( \
	name, leftX, upperLeftX, groundLeftX, rightX, upperRightX, groundRightX) \
	void name(Sint32 side, Sint32 phase) \
	{ \
		if(side == 0) { \
			if(phase == 3) \
				fdMapGridCopyMetatileTilesMetatilesRect( \
					0, 120, 1, 2, leftX, phase); \
			fdMapGridCopyMetatilesLayered(127, 3, 1, 2, upperLeftX, 3); \
			fdMapGridCopyMetatileAttributes(groundLeftX, 127, 1, 1, leftX, 8); \
		} else { \
			if(phase == 3) \
				fdMapGridCopyMetatileTilesMetatilesRect( \
					0, 120, 1, 2, rightX, phase); \
			fdMapGridCopyMetatilesLayered(127, 3, 1, 2, upperRightX, 3); \
			fdMapGridCopyMetatileAttributes( \
				groundRightX, 127, 1, 1, rightX, 8); \
		} \
	}

/* Opens the door at the top of Felix's stage: the door at metatile (23, 3) on
   the left path, (9, 3) on the right, is redrawn open and its doorway leads
   on. */
DEFINE_TRIAL_ROAD_OPEN_DOOR(scTrialRoadOpenLeaderDoor, 23, 57, 0, 9, 43, 1)

/* Opens the door at the top of Moapa's stage, beside Felix's: metatile (28, 3)
   when Felix took the left path, (4, 3) when he took the right. */
DEFINE_TRIAL_ROAD_OPEN_DOOR(scTrialRoadOpenRivalDoor, 28, 62, 1, 4, 38, 0)

/* Stepping on the give-up button at the top of a stage during the race
   (metatile (12, 8) on the left path, (20, 8) on the right) asks "Do you want
   to give up?". Felix walks onto the button's middle and faces south; answering
   Yes, he leans back with a drop of sweat, gets the party's equipment back and
   is sent down to the Shaman Village's trial grounds. */
Bool scTrialRoadOfferToGiveUp(void)
{
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 message;

	if(fdScriptGetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId,
			((FIELD_METATILE(leader->x.fixed)) << 4) + 8,
			((FIELD_METATILE(leader->z.fixed)) << 4) + 8);
		fdScriptPlaySnd(SE_SPLASH);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
		message = (Sint32)&MsgTrialRoadGiveUpPrompt;
		/* "Do you want to give up?" */
		mnFieldMessageShow(message, QUESTION_MESSAGE);
		state->choiceMsg = message + 1;
		state->choiceWidth = 4;
		state->choiceSelection = 1;
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 1) == 0) {
			fdActorFaceDirection(
				MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
			fdScriptWait(10);
			fdActorSetAnimation(
				MainRoutineStateWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
			fdScriptWait(10);
			scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
				EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
			fdScriptWait(30);
			gmInventorySnapshotRestore();
			syImportScriptSetExitRequest(
				TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_C);
			return TRUE;
		}
	}
	fdScriptSetFlag(FLAG_GIVE_UP_ASKED);
	return FALSE;
}

/* Stepping off the give-up button lets it ask again next time Felix steps on
   it. */
void scTrialRoadClearGiveUpAsked(void)
{
	syGameFlagClear(FLAG_GIVE_UP_ASKED);
}

/* Standing on the switch in front of the door at the top of a stage (metatile
   (22, 9) on the left path, (10, 9) on the right) opens the blue chest beside
   the door, and the second chest too when Moapa got there first ("it will take
   twice the weight to open the doors"). */
void scTrialRoadOpenChests(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	TRIAL_ROAD_STATE *state = TrialRoadStateWork;
	Sint32			  found;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId,
		((FIELD_METATILE(leader->x.fixed)) << 4) + 8,
		((FIELD_METATILE(leader->z.fixed)) << 4) + 8);
	scTrialRoadIdle();
	found = scTrialRoadRivalIsAhead(0);
	if(found != 0)
		fdScriptSetFlag(state->stage[0] + FLAG_RIVAL_FIRST_AT_DOOR_BASE);
	fdScriptPlaySnd(SE_CHEST_OPEN);
	if(state->mirrored == 0) {
		fdActorSetAnimation(ACTOR_LEFT_INNER_CHEST, 6);
		fdScriptSetFlag(FLAG_INNER_CHEST_OPEN);
		if(found != 0) {
			fdActorSetAnimation(ACTOR_LEFT_OUTER_CHEST, 6);
			fdScriptSetFlag(FLAG_OUTER_CHEST_OPEN);
		}
	} else {
		fdActorSetAnimation(ACTOR_RIGHT_INNER_CHEST, 6);
		fdScriptSetFlag(FLAG_INNER_CHEST_OPEN);
		if(found != 0) {
			fdActorSetAnimation(ACTOR_RIGHT_OUTER_CHEST, 6);
			fdScriptSetFlag(FLAG_OUTER_CHEST_OPEN);
		}
	}
	fdScriptSetFlag(FLAG_STAGE_CHESTS_OPENED);
	fdScriptEnd();
}

/* Choosing a piece of equipment for an open blue chest puts it inside, and the
   message names its owner and the item ("Felix put ...'s ... in the chest.").
   The chest shuts, and once every open chest is shut the door beside them
   opens. The item is taken from its owner's inventory. */
void scTrialRoadPutItemInChest(Sint32 item, Sint32 owner, Sint32 slot)
{
	Sint32			  chest = fdFindNearbyTalkTarget(fdPartyGetLeaderActor());
	TRIAL_ROAD_STATE *state = TrialRoadStateWork;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	mnMessageArgumentPush(owner, MESSAGE_ARGUMENT_ACTOR);
	mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
	/* "Felix put ...'s ... in the chest." */
	mnFieldMessageShow((Sint32)&MsgTrialRoadStoredItem, FIELD_MESSAGE_ZOOM);
	fdScriptPlaySnd(SE_CHEST_CLOSE);
	fdActorSetAnimationAndWait(chest, 5);
	if(chest == ACTOR_LEFT_INNER_CHEST || chest == ACTOR_RIGHT_INNER_CHEST)
		fdScriptClearFlag(FLAG_INNER_CHEST_OPEN);
	else
		fdScriptClearFlag(FLAG_OUTER_CHEST_OPEN);
	if(!fdScriptGetFlag(FLAG_INNER_CHEST_OPEN) &&
		!fdScriptGetFlag(FLAG_OUTER_CHEST_OPEN)) {
		scTrialRoadOpenLeaderDoor(state->mirrored, state->stage[0]);
		fdScriptPlaySnd(SE_ITEM_BREAK);
		fdScriptSetFlag(FLAG_STAGE_DOOR_OPEN);
	}
	gmInventoryClearSlot(owner, slot);
	fdScriptEnd();
}

/* Searching a blue chest before the switch has opened it says "The chest won't
   open. There's something keeping it shut tight." Once it stands open, Felix is
   asked "Place this item in the chest?", and Yes opens the item menu. */
void scTrialRoadExamineChest(Sint32 id)
{
	Sint32 flag;

	if(id == ACTOR_LEFT_INNER_CHEST || id == ACTOR_RIGHT_INNER_CHEST)
		flag = FLAG_INNER_CHEST_OPEN;
	else
		flag = FLAG_OUTER_CHEST_OPEN;
	if(!fdScriptGetFlag(flag)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		/* "The chest won't open. There's something keeping it shut tight." */
		mnFieldMessageShow(
			(Sint32)MsgTrialRoadChestBlocked, FIELD_MESSAGE_ZOOM);
		fdScriptEnd();
	} else {
		/* "Place this item in the chest?" */
		mnFieldMessageShow(
			(Sint32)MsgTrialRoadPlaceItemPrompt, QUESTION_MESSAGE);
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 1) == 0)
			FieldStateWork->fieldMenu = 1;
	}
}

/* Walking around the switch once the chests have opened does nothing. */
void scTrialRoadStepOffSwitch(void)
{
}

/* Lets Felix jump onto the top of the pillar standing at the actor's metatile
   (MAP_METATILE_JUMP). */
void scTrialRoadSetJumpMetatileUnder(Sint32 id)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR	  *actor = fdActorGetById(id);

	if(actor != NULL) {
		Uint8 *metatiles = (Uint8 *)map->layers[0].metatiles;
		Uint8 *metatile = metatiles +
			(((FIELD_METATILE(actor->x.fixed)) +
				 ((FIELD_METATILE(actor->z.fixed)) << 7))
				<< 2);
		Sint32 bit = MAP_METATILE_JUMP;

		metatile[3] = bit | metatile[3];
	}
}

/* Stops Felix jumping onto the metatile the actor is leaving. */
void scTrialRoadClearJumpMetatileUnder(Sint32 id)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR	  *actor = fdActorGetById(id);

	if(actor != NULL) {
		Uint8 *metatiles = (Uint8 *)map->layers[0].metatiles;
		Uint8 *metatile = metatiles +
			(((FIELD_METATILE(actor->x.fixed)) +
				 ((FIELD_METATILE(actor->z.fixed)) << 7))
				<< 2);

		metatile[3] &= (Uint8)~MAP_METATILE_JUMP;
	}
}

/* Keeps an invisible pillar top on its pillar, so many pixels above it. */
Sint32 scTrialRoadFollowPillar(ACTOR *actor)
{
	ACTOR *owner = &actor->watchedActor->actor;

	SET_ACTOR_POSITION(actor, owner->x.fixed,
		owner->y.fixed + toFIXED(PILLAR_TOP_HEIGHT(actor)), owner->z.fixed);
	actor->groundHeight = owner->groundHeight;
	return 1;
}

extern Sint32 TrialRoad_PillarTopScript[];

/* Puts an invisible top on the pillar for Felix to stand on. */
void scTrialRoadAddPillarTop(Sint32 id)
{
	ACTOR *owner = fdActorGetById(id);
	ACTOR *actor;

	if(owner != NULL) {
		actor = scFieldActorCreate(SPRITE_UNSEEN_MARKER, owner->x.fixed,
			owner->y.fixed, owner->z.fixed);
		if(actor != NULL) {
			fdActorSetSprFlags(actor, 0);
			fdActorStartScript(actor, TrialRoad_PillarTopScript);
			actor->watchedActor = (FIELD_OBJECT *)owner;
			actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
			actor->motionFlg = 0;
			actor->collisionFlg = 0;
			PILLAR_TOP_HEIGHT(actor) = PILLAR_HEIGHT;
			owner->watchedActor = (FIELD_OBJECT *)actor;
		}
	}
}

/* Sets how many pixels above the pillar its top stands. */
void scTrialRoadSetPillarTopLift(Sint32 id, Sint32 value)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != NULL) {
		actor = &actor->watchedActor->actor;
		if(actor != NULL)
			PILLAR_TOP_HEIGHT(actor) = value;
	}
}

/* Defines a push or Move cue's script that, while the leader is on foot, calls
   onFoot with the pushed actor. */
#define DEFINE_TRIAL_ROAD_ON_FOOT(name, onFoot) \
	void name(Sint32 unused, Sint32 id) \
	{ \
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork; \
		Uint8				mode; \
\
		mode = party->movementMode; \
\
		if(mode == LEADER_MOVEMENT_WALK) \
			onFoot(id); \
	}

/* As a pillar starts to slide, Felix can no longer jump onto the place it is
   leaving. */
DEFINE_TRIAL_ROAD_ON_FOOT(
	TrialRoad_ClearJumpUnderPushedPillar, scTrialRoadClearJumpMetatileUnder)

/* Once a pushed or Moved pillar stops, Felix can jump onto its top where it
   stands. */
DEFINE_TRIAL_ROAD_ON_FOOT(
	TrialRoad_SetJumpUnderPushedPillar, scTrialRoadSetJumpMetatileUnder)

/* Whirlwind on the weeds at the foot of the pillar stage's overgrown pillar, at
   metatile (15, 24), blows it away, and the pillar can then be pushed. */
void scTrialRoadBlowAwayPillarStageWeeds(void)
{
	PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_C)) = 0;
	fdScriptSetFlag(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY);
}

/* Walking off the edge of a ledge or pillar top toward another one jumps Felix
   across, when there is somewhere to land. */
void scTrialRoadJumpForward(void)
{
	fdTryJumpForward();
}

/* Lets Felix walk into the vent where the actor stands, now that it is dug
   out. */
void scTrialRoadOpenVentMetatile(Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != NULL)
		fdMapGridCopyMetatilesLayered(14, 27, 1, 1,
			FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
}

/* Keeps note, each frame, of whether Felix is up on the pillar stage's upper
   ledge or a pillar's top, 32 pixels up (FLAG_LEADER_ON_PILLARS). */
void scTrialRoadCheckLeaderOnPillars(void)
{
	FIXED y = fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed;

	if(y == toFIXED(32))
		fdScriptSetFlag(FLAG_LEADER_ON_PILLARS);
	else
		fdScriptClearFlag(FLAG_LEADER_ON_PILLARS);
}

/* Walking south off the pillar stage's upper ledge, beside the vine, jumps
   Felix across the gap when the pillar beneath has been pushed into it (row 23
   of the map); otherwise he hops down to the ground in a puff of dust. */
void scTrialRoadJumpAcrossGap(void)
{
	if(FIELD_METATILE(fdActorGetById(ACTOR_PILLAR_B)->z.fixed) == 23)
		fdTryJumpForward();
	else
		fdActorJumpDownLedge(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING, 0);
}

/* As one of the torch stage's two stone pillars starts to slide, Felix can no
   longer jump onto the place it is leaving. */
void scTrialRoadClearJumpUnderTorchStagePillar(Sint32 unused, Sint32 id)
{
	if((Uint32)(id - ACTOR_PILLAR_A) <= ACTOR_PILLAR_B - ACTOR_PILLAR_A)
		scTrialRoadClearJumpMetatileUnder(id);
}

/* Pushing a torch against one of the torch stage's two pillars of ice, at
   metatiles (13, 27) and (19, 27), melts it to a puddle Felix can jump across.
   A stone pillar pushed in the torch stage can be jumped onto where it
   stops. */
void scTrialRoadMeltIceBesideTorch(Sint32 unused, Sint32 id)
{
	Sint32 x;

	if((Uint32)(id - ACTOR_PILLAR_A) <= ACTOR_PILLAR_B - ACTOR_PILLAR_A) {
		scTrialRoadSetJumpMetatileUnder(id);
		return;
	}
	x = (Sint32)fdActorGetById(id);
	x = FIELD_METATILE(((ACTOR *)x)->x.fixed);
	id = 0;
	if(x == 13)
		id = ACTOR_PILLAR_C;
	if(x == 15)
		id = ACTOR_PILLAR_D;
	if(x == 17)
		id = ACTOR_PILLAR_E;
	if(x == 19)
		id = ACTOR_PILLAR_F;
	if(id != 0) {
		Sint32 flag = id + FLAG_PUDDLE_MELTED_BASE;

		if(fdScriptGetFlag(flag) == 0) {
			ACTOR *actor = fdActorGetById(id);

			actor->update = NULL;
			fdActorSetSprColMode(actor, 0);
			fdActorSetAnimation(id, 6);
			scTrialRoadSetPillarTopLift(id, -PILLAR_HEIGHT);
			fdScriptSetFlag(flag);
		}
	}
}

/* Walking north or south into a melted puddle in the torch stage jumps Felix
   across it; a frozen one stops him. */
void scTrialRoadJumpMeltedPuddle(void)
{
	if(fdScriptGetFlag(
		   fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId) +
		   FLAG_PUDDLE_MELTED_BASE))
		fdTryJumpForward();
}

/* Frost on a melted puddle in the torch stage freezes it into a pillar of ice
   again, which Felix can stand on. */
void scTrialRoadFreezePuddle(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	scTrialRoadSetPillarTopLift(id, PILLAR_HEIGHT);
	fdScriptClearFlag(id + FLAG_PUDDLE_MELTED_BASE);
	if(actor != NULL)
		actor->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
}

/* Whirlwind on the weeds at the foot of one of the two overgrown pillars in the
   weeds stage blows it away, and the pillar can then be pushed or Moved. */
void scTrialRoadBlowAwayWeeds(Sint32 unused, Sint32 id)
{
	PUSH_BLOCKED(fdActorGetById(id - (ACTOR_WEEDS_A - ACTOR_PILLAR_A))) = 0;
	fdScriptSetFlag(id + FLAG_WEEDS_BLOWN_AWAY_BASE);
}

/* Sinks a stake into the ground, so that Felix walks over it. */
void scTrialRoadLowerStake(Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != NULL) {
		scTrialRoadSetPillarTopLift(id, -PILLAR_HEIGHT);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSelectAnimation(actor, 6);
	}
}

/* Raises a stake out of the ground, solid, its top 16 pixels up. */
void scTrialRoadRaiseStake(Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != NULL) {
		scTrialRoadSetPillarTopLift(id, PILLAR_HEIGHT);
		actor->collisionFlg = ACTOR_SOLID;
		actor->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
		fdActorSelectAnimation(actor, 5);
	}
}

/* Whirlwind on the weeds at the foot of the Pound stage's middle pillar, at
   metatile (16, 28), blows it away, and the pillar can then be pushed. */
void scTrialRoadBlowAwayPoundStageWeeds(void)
{
	PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_B)) = 0;
	fdScriptSetFlag(FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY);
}

/* The stake each of the pound stage's four stakes raises when it is pounded
   down: they go in pairs. */
extern Sint8 TrialRoad_PoundPartners[4];

/* After Move or a push, notes each stake whose partner has a pillar standing on
   it: stepping on that stake can no longer raise the partner
   (scTrialRoadStepOntoStake). */
void scTrialRoadCheckStakesCovered(Sint32 unused, Sint32 pushedId)
{
	Sint32 id;

	for(id = ACTOR_WEST_STAKE; id <= ACTOR_EAST_STAKE; id++) {
		ACTOR *actor = fdActorGetById(id);
		Sint32 x = FIELD_METATILE(actor->x.fixed);
		Sint32 z = FIELD_METATILE(actor->z.fixed);
		Uint8 *covered = &PILLAR_TOP_HEIGHT(fdActorGetById(
			TrialRoad_PoundPartners[(id - ACTOR_WEST_STAKE) & 3]));
		Sint32 other;

		*covered = 0;
		for(other = ACTOR_PILLAR_A; other <= ACTOR_PILLAR_C; other++) {
			actor = fdActorGetById(other);
			if(x == FIELD_METATILE(actor->x.fixed) &&
				z == FIELD_METATILE(actor->z.fixed)) {
				*covered = 1;
				break;
			}
		}
	}
}

/* Pound on a raised stake in the Pound stage squashes it into the ground and
   pops its partner up (TrialRoad_PoundPartners); Felix, standing where the
   partner rises, is thrown up onto it. */
void scTrialRoadPoundStake(Sint32 id, Sint32 phase, Sint32 frame)
{
	Sint32 other = TrialRoad_PoundPartners[(id - ACTOR_WEST_STAKE) & 3];
	ACTOR *actor = fdActorGetById(id);
	ACTOR *partner = fdActorGetById(other);
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	if(frame <= 10)
		actor->scaleY = toFIXED(1) - frame * (toFIXED(1) / 16);
	else
		actor->scaleY = toFIXED(1);
	if(phase == POUND_STAGE_LANDED) {
		scTrialRoadLowerStake(id);
		scTrialRoadRaiseStake(other);
		switch(id) {
		case ACTOR_WEST_STAKE:
			fdScriptSetFlag(FLAG_INNER_WEST_STAKE_RAISED);
			break;
		case ACTOR_INNER_WEST_STAKE:
			fdScriptClearFlag(FLAG_INNER_WEST_STAKE_RAISED);
			break;
		case ACTOR_INNER_EAST_STAKE:
			fdScriptSetFlag(FLAG_EAST_STAKE_RAISED);
			break;
		case ACTOR_EAST_STAKE:
			fdScriptClearFlag(FLAG_EAST_STAKE_RAISED);
			break;
		}
		if(FIELD_METATILE(partner->x.fixed) ==
				FIELD_METATILE(leader->x.fixed) &&
			FIELD_METATILE(partner->z.fixed) ==
				FIELD_METATILE(leader->z.fixed)) {
			leader->velY = toFIXED(8);
			leader->x.fixed = (leader->x.fixed & -toFIXED(16)) + toFIXED(8);
			leader->z.fixed = (leader->z.fixed & -toFIXED(16)) + toFIXED(8);
		}
	}
	if(frame == 9)
		partner->scaleY = toFIXED(12) / 10;
	else if(frame == 11)
		partner->scaleY = toFIXED(11) / 10;
	else if(frame == 13)
		partner->scaleY = toFIXED(105) / 100;
	else
		partner->scaleY = toFIXED(1);
}

/* Pound on the west stake, metatile (13, 26), drives it down and raises the
   inner west stake. */
void scTrialRoadPoundWestStake(Sint32 phase, Sint32 frame)
{
	scTrialRoadPoundStake(ACTOR_WEST_STAKE, phase, frame);
}

/* Pound on the inner west stake, metatile (15, 26), drives it down and raises
   the west stake. */
void scTrialRoadPoundInnerWestStake(Sint32 phase, Sint32 frame)
{
	scTrialRoadPoundStake(ACTOR_INNER_WEST_STAKE, phase, frame);
}

/* Pound on the inner east stake, metatile (17, 26), drives it down and raises
   the east stake. */
void scTrialRoadPoundInnerEastStake(Sint32 phase, Sint32 frame)
{
	scTrialRoadPoundStake(ACTOR_INNER_EAST_STAKE, phase, frame);
}

/* Pound on the east stake, metatile (19, 26), drives it down and raises the
   inner east stake. */
void scTrialRoadPoundEastStake(Sint32 phase, Sint32 frame)
{
	scTrialRoadPoundStake(ACTOR_EAST_STAKE, phase, frame);
}

/* Stepping onto a raised stake in the Pound stage sinks it slowly under Felix's
   weight while its partner grows out of the ground, unless a pillar stands on
   the partner. */
void scTrialRoadStepOntoStake(Sint32 id, Sint32 other, Sint32 flag, Sint32 side)
{
	ACTOR *actor = fdActorGetById(id);
	ACTOR *partner = fdActorGetById(other);
	FIXED  t;

	if(PILLAR_TOP_HEIGHT(actor) != 0)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	partner->scaleY = toFIXED(1) / 8;
	sySynch(1);
	scTrialRoadRaiseStake(other);
	t = toFIXED(1) / 8;
	do {
		scTrialRoadSetPillarTopLift(
			id, (toFIXED(1) / 2 - t) / (toFIXED(1) / 32));
		actor->scaleY = toFIXED(1) - t;
		partner->scaleY = t;
		sySynch(1);
		t += toFIXED(1) / 8;
	} while(t <= toFIXED(1));
	actor->scaleY = toFIXED(1);
	scTrialRoadLowerStake(id);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	if(side != 0)
		fdScriptSetFlag(flag);
	else
		fdScriptClearFlag(flag);
	fdScriptEnd();
}

/* Stepping onto the west stake sinks it and raises the inner west one. */
void scTrialRoadStepOntoWestStake(void)
{
	scTrialRoadStepOntoStake(ACTOR_WEST_STAKE, ACTOR_INNER_WEST_STAKE,
		FLAG_INNER_WEST_STAKE_RAISED, 1);
}

/* Stepping onto the inner west stake sinks it and raises the west one. */
void scTrialRoadStepOntoInnerWestStake(void)
{
	scTrialRoadStepOntoStake(ACTOR_INNER_WEST_STAKE, ACTOR_WEST_STAKE,
		FLAG_INNER_WEST_STAKE_RAISED, 0);
}

/* Stepping onto the inner east stake sinks it and raises the east one. */
void scTrialRoadStepOntoInnerEastStake(void)
{
	scTrialRoadStepOntoStake(
		ACTOR_INNER_EAST_STAKE, ACTOR_EAST_STAKE, FLAG_EAST_STAKE_RAISED, 1);
}

/* Stepping onto the east stake sinks it and raises the inner east one. */
void scTrialRoadStepOntoEastStake(void)
{
	scTrialRoadStepOntoStake(
		ACTOR_EAST_STAKE, ACTOR_INNER_EAST_STAKE, FLAG_EAST_STAKE_RAISED, 0);
}

/* Walking into a pillar or stake in the Pound stage: from a raised top Felix
   jumps onto it, and from the ground he pushes it, if it can be pushed. */
void scTrialRoadLandOnPillar(void)
{
	FIXED y = fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed;

	if(y >= toFIXED(16)) {
		fdTryJumpForward();
	} else if(fdLeaderCanPushObjectAhead()) {
		fdLeaderPushObjectAhead();
		scTrialRoadCheckStakesCovered(0, 0);
	}
}

/* Hopping onto one of the crumbling stage's two cracked pillars, from the east
   or the west: the first time it flashes, and the second time it crumbles under
   Felix and drops him to the ground. */
void scTrialRoadCheckCrumblingPillars(void)
{
	scLeaderCheckCrumblingStep(TrialRoad_CrumblingPillars);
}

/* Burst on the crumbling stage's west cracked pillar crumbles it away. */
void scTrialRoadCrumbleWestPillar(Sint32 phase)
{
	if(phase == 3)
		scTrialRoadCrumblePillar(
			TrialRoad_CrumblingPillars, ACTOR_WEST_CRUMBLING_PILLAR);
}

/* Burst on the crumbling stage's east cracked pillar crumbles it away. */
void scTrialRoadCrumbleEastPillar(Sint32 phase)
{
	if(phase == 3)
		scTrialRoadCrumblePillar(
			TrialRoad_CrumblingPillars, ACTOR_EAST_CRUMBLING_PILLAR);
}

/* Makes a log solid where it lies: its own metatile and the two beside it along
   its length, north and south of it in mode 0, east and west otherwise
   (METATILE_BEHAVIOR_WALL on the ground and the upper level). */
void scTrialRoadBlockLogMetatiles(Sint32 id, Sint32 mode)
{
	MAP_STATE	 *map = Ram_HeapSlots->mapGrid;
	ACTOR		 *actor = fdActorGetById(id);
	Sint32		  index;
	MAP_METATILE *low;
	MAP_METATILE *high;

	if(actor == 0)
		return;
	index = (FIELD_METATILE(actor->x.fixed)) +
		((FIELD_METATILE(actor->z.fixed)) << 7);
	low = map->layers[0].metatiles + index;
	high = map->layers[2].metatiles + index;
	if(mode == 0) {
		low[-MAPGRID_SIZE].collisionCode = METATILE_BEHAVIOR_WALL;
		low->collisionCode = METATILE_BEHAVIOR_WALL;
		low[MAPGRID_SIZE].collisionCode = METATILE_BEHAVIOR_WALL;
		high[-MAPGRID_SIZE].collisionCode = METATILE_BEHAVIOR_WALL;
		high->collisionCode = METATILE_BEHAVIOR_WALL;
		high[MAPGRID_SIZE].collisionCode = METATILE_BEHAVIOR_WALL;
	} else {
		low[-1].collisionCode = METATILE_BEHAVIOR_WALL;
		low->collisionCode = METATILE_BEHAVIOR_WALL;
		low[1].collisionCode = METATILE_BEHAVIOR_WALL;
		high[-1].collisionCode = METATILE_BEHAVIOR_WALL;
		high->collisionCode = METATILE_BEHAVIOR_WALL;
		high[1].collisionCode = METATILE_BEHAVIOR_WALL;
	}
}

/* Makes the three logs solid where they lie, and lets Felix jump east across a
   log that has fallen into the water. */
void scTrialRoadBlockLogs(void)
{
	fdMapGridCopyMetatileAttributes(11, 64, 11, 8, 11, 22);
	fdMapGridCopyMetatileAttributes(79, 64, 11, 8, 79, 22);
	scTrialRoadBlockLogMetatiles(ACTOR_UPPER_LOG, 0);
	scTrialRoadBlockLogMetatiles(ACTOR_LOWER_LOG, 0);
	scTrialRoadBlockLogMetatiles(ACTOR_EAST_LOG, 1);
	if(fdScriptGetFlag(FLAG_LOG_IN_WATER_BASE + ACTOR_UPPER_LOG))
		fdMapGridCopyMetatileAttributes(0, 48, 1, 3, 19, 23);
	if(fdScriptGetFlag(FLAG_LOG_IN_WATER_BASE + ACTOR_LOWER_LOG))
		fdMapGridCopyMetatileAttributes(0, 48, 1, 3, 19, 26);
}

/* Whether a log rolling into the metatile is stopped there by the block or a
   wall. */
Sint32 scTrialRoadIsLogBlocked(Sint32 x, Sint32 z)
{
	ACTOR	  *actor = fdActorGetById(ACTOR_PILLAR_A);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Uint8	  *metatiles;
	Uint8	  *metatile;
	Sint32	   value;

	if(FIELD_METATILE(actor->x.fixed) == x &&
		FIELD_METATILE(actor->z.fixed) == z)
		return 1;
	metatiles = (Uint8 *)map->layers[0].metatiles;
	metatile = metatiles + ((x + (z << 7)) << 2);
	value = metatile[2];
	return value == METATILE_BEHAVIOR_WALL;
}

/* Felix leans on a log in the log stage and it rolls, up to six metatiles,
   until the block or a wall stops it. Rolled east past column 17 of the map, it
   rolls on to the edge and falls into the water, where it stays and Felix can
   jump across it. */
void scTrialRoadPushLog(Sint32 id, Sint32 dx, Sint32 dz, Sint32 axis)
{
	ACTOR *actor = fdActorGetById(id);
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 fromX = FIELD_METATILE(actor->x.fixed);
	Sint32 fromZ = FIELD_METATILE(actor->z.fixed);
	Sint32 toX = fromX;
	Sint32 toZ = fromZ;
	Sint32 n;
	Sint32 mode = 2;
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	VECTOR3 unused;

	if((dx != 0 && toX != x + dx) || (dz != 0 && toZ != z + dz)) {
		FieldStateWork->blockedSteps = FIELD_PUSH_FRAMES + 1;
		return;
	}
	for(n = 0; n <= 5; n++) {
		x = toX + dx;
		z = toZ + dz;
		if(axis == 0) {
			if(scTrialRoadIsLogBlocked(x, z - 1))
				break;
			if(scTrialRoadIsLogBlocked(x, z))
				break;
			if(scTrialRoadIsLogBlocked(x, z + 1))
				break;
		} else {
			if(scTrialRoadIsLogBlocked(x - 1, z))
				break;
			if(scTrialRoadIsLogBlocked(x, z))
				break;
			if(scTrialRoadIsLogBlocked(x + 1, z))
				break;
		}
		toX = x;
		toZ = z;
	}
	if(fromX == toX && fromZ == toZ)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, 8);
	fdScriptWait(6);
	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 5;
	sySndPlay(SE_HEAVY_MOVE);
	if(dx < 0 || dz < 0)
		mode = 3;
	fdActorSelectAnimation(actor, mode);
	fdActorSetMoveTarget(
		actor, FIELD_METATILE_CENTER(toX), 0, FIELD_METATILE_CENTER(toZ));
	fdScriptWait(6);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(
		MainRoutineStateWork.leaderActorId, 0.3, 0.2);
	fdActorSetDestinationOffsetById(
		MainRoutineStateWork.leaderActorId, dx << 3, dz << 3);
	fdScriptWait(24);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorWaitForMove(actor);
	if(toX <= 17) {
		fdActorSelectAnimation(actor, 1);
		sySndPlay(SE_STOP_EXIT_SE);
		sySndPlay(SE_HEAVY_STOP);
	} else {
		fdActorSelectAnimation(actor, 3);
		fdActorSetMoveTarget(
			actor, toFIXED(312), 0, FIELD_METATILE_CENTER(toZ));
		fdScriptWait(30);
		fdActorSelectAnimation(actor, 8);
		fdActorWaitForMove(actor);
		/* FAKEMATCH(dead): the flags stored through their taken address; as
		 * actor->priFlg. */
		/* FAKEMATCH(temporary): mode reused to hold ACTOR_PRIORITY_UNDERFOOT
		 * for the store; as the constant stored. */
		*&actor->priFlg = mode = ACTOR_PRIORITY_UNDERFOOT;
		sySndPlay(SE_STOP_EXIT_SE);
		sySndPlay(SE_LOG_FALL);
		syGameFlagSet(id + FLAG_LOG_IN_WATER_BASE);
	}
	fdScriptWait(15);
	fdScriptEnd();
	scTrialRoadBlockLogs();
}

/* Walking west into the upper log rolls it west. */
void scTrialRoadPushUpperLogWest(void)
{
	scTrialRoadPushLog(ACTOR_UPPER_LOG, -1, 0, 0);
}

/* Walking east into the upper log rolls it east. */
void scTrialRoadPushUpperLogEast(void)
{
	scTrialRoadPushLog(ACTOR_UPPER_LOG, 1, 0, 0);
}

/* Walking west into the lower log rolls it west. */
void scTrialRoadPushLowerLogWest(void)
{
	scTrialRoadPushLog(ACTOR_LOWER_LOG, -1, 0, 0);
}

/* Walking east into the lower log rolls it east. */
void scTrialRoadPushLowerLogEast(void)
{
	scTrialRoadPushLog(ACTOR_LOWER_LOG, 1, 0, 0);
}

/* Walking north into the east log rolls it north. */
void scTrialRoadPushEastLogNorth(void)
{
	scTrialRoadPushLog(ACTOR_EAST_LOG, 0, -1, 1);
}

/* Walking south into the east log rolls it south. */
void scTrialRoadPushEastLogSouth(void)
{
	scTrialRoadPushLog(ACTOR_EAST_LOG, 0, 1, 1);
}

/* Pressing south at the north end of the east log's track: if Felix is climbing
   there he climbs down, and on foot he rolls the east log south. */
void scTrialRoadPushEastLogSouthOrClimbDown(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->movementMode == LEADER_MOVEMENT_KEYS)
		fdClimbDown();
	else
		scTrialRoadPushLog(ACTOR_EAST_LOG, 0, 1, 1);
}

/* Scoop over one of the fountain stage's three buried vents, at metatiles (13,
   23), (16, 23) and (19, 23), digs it out: the vent shows in the ground, Felix
   can step onto it, and its fountain rises and falls with the others. */
void scTrialRoadUncoverVent(Sint32 id, Sint32 phase, Sint32 frame)
{
	if(frame == 30) {
		ACTOR *actor = fdActorGetById(id);
		Sint32 height;

		actor->x.fixed = toFIXED(actor->tmr);
		actor->z.fixed = toFIXED(actor->userValue);
		height = fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		actor->groundHeight = height;
		actor->y.fixed = height;
		scTrialRoadOpenVentMetatile(id);
		fdScriptSetFlag(id + FLAG_VENT_UNCOVERED_BASE);
	}
}

/* Scoop over the west vent digs it out. */
void scTrialRoadUncoverWestVent(Sint32 phase, Sint32 frame)
{
	scTrialRoadUncoverVent(ACTOR_WEST_VENT, phase, frame);
}

/* Scoop over the middle vent digs it out. */
void scTrialRoadUncoverMiddleVent(Sint32 phase, Sint32 frame)
{
	scTrialRoadUncoverVent(ACTOR_MIDDLE_VENT, phase, frame);
}

/* Scoop over the east vent digs it out. */
void scTrialRoadUncoverEastVent(Sint32 phase, Sint32 frame)
{
	scTrialRoadUncoverVent(ACTOR_EAST_VENT, phase, frame);
}

/* Stepping onto the fountain stage's block from above leaves Felix standing on
   its top, 32 pixels up, rather than inside it. */
void scTrialRoadStandOnBlock(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *block = fdActorGetById(ACTOR_PILLAR_A);

	if(FIELD_METATILE(block->x.fixed) == FIELD_METATILE(leader->x.fixed) &&
		FIELD_METATILE(block->z.fixed) == FIELD_METATILE(leader->z.fixed)) {
		FIXED y = leader->y.fixed;

		if(y < toFIXED(32))
			leader->y.fixed = toFIXED(32);
	}
}

/* Sand at the sand stage lets Felix, sunk into the ground, go up the stream
   that would otherwise carry him back. The game moves him onto the map's upper
   level for as long as he stays sunk. */
void scTrialRoadStartSand(void)
{
	fdActorGetById(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_OVERHEAD;
}

/* When Felix rises out of the sand at the sand stage, he walks on the ground
   level again. */
void scTrialRoadEndSand(void)
{
	fdActorGetById(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_GROUND;
}

/* Whirlwind on the weeds on the sand stage's cliff blows it away and uncovers
   the vine behind it, metatile (19, 21), which Felix can then climb. */
void scTrialRoadBlowAwaySandStageWeeds(void)
{
	fdMapGridCopyMetatileAttributes(12, 21, 1, 1, 19, 21);
	fdScriptSetFlag(FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY);
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_TRIAL_ROAD_OPEN_DOOR
#undef DEFINE_TRIAL_ROAD_ON_FOOT

extern const Sint32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#include "../field/field_effect_advance_motion_priority.c"

/* Walking in the sand stage's stream carries Felix south a pixel a frame,
   throwing up spray, unless he is crossing the tightrope above it. */
void scTrialRoadDriftWithStream(void)
{
	ACTOR		  *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	Sint32		   velocity[3];

	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_TIGHTROPE)
		return;
	leader->z.fixed += toFIXED(1);
	if((FrameCount & 7) != 0)
		return;
	if(leader->velZ == 0 && leader->velX == 0)
		return;
	options.update = fdEffectAdvanceMotionAtLeaderPriority;
	options.startScaleX = toFIXED(8) / 10;
	options.startScaleY = toFIXED(8) / 10;
	velocity[1] = 0;
	velocity[2] = toFIXED(1);
	velocity[0] = 0;
	velocity[0] += (Uint16)(syRandom() * 2) - toFIXED(1);
	velocity[2] += (Uint16)syRandom() - toFIXED(0.5);
	scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
		velocity[0], velocity[1], velocity[2], EFFECT_USE_UPDATE | 1, &options);
}

/* Lift on the rock in the cave beyond the summit raises it out of the way to
   Gasp's pool. */
void scTrialRoadLiftCaveRock(void)
{
	syGameFlagSet(FLAG_CAVE_ROCK_LIFTED);
}

/* Cyclone on the clumps of weeds around Gasp's pool scatters the ones it
   touches. */
void scTrialRoadScatterWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(
		68, 6, MAPGRID_LAYER_MIDDLE, 21, 13, 64, 64);
}

/* Cyclone on the left-hand weeds by the entrance of Gasp's pool, metatile (4,
   9), scatters them, and the Mad Plant hiding there leaps out and attacks. */
void scTrialRoadScatterWeedsHidingMadPlant(void)
{
	syFieldCycloneCopyTouchedMetatiles(
		68, 6, MAPGRID_LAYER_MIDDLE, 21, 13, 64, 64);
	if(!fdScriptGetFlag(FLAG_MAD_PLANT(MAD_PLANT_TRIAL_ROAD))) {
		syImportMadPlantLeapAtLeader(SPRITE_MAD_PLANT, 72, 152);
		MainRoutineStateWork.pendingDjinni =
			IF_FLAG_SET(FLAG_MAD_PLANT(MAD_PLANT_TRIAL_ROAD));
		SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(
			ENCOUNTER_ZONE_MAD_PLANTS, MAD_PLANT_TRIAL_ROAD);
	}
}

/* Unused: always 1. */
UNUSED Sint32 scTrialRoadReturnTrue(void)
{
	return 1;
}

/* Defines a function that sets the three tiles in Gasp's pools, at metatiles
   (10, 11), (18, 7) and (18, 15), to the metatile at column groundX of row 30,
   and the upper layer's to the one at upperX. */
#define DEFINE_TRIAL_ROAD_SET_POOL_TILES(name, groundX, upperX) \
	void name(void) \
	{ \
		fdMapGridCopyMetatiles(groundX, 30, 1, 1, 10, 11); \
		fdMapCopyMetatilesRect(groundX, 30, 1, 1, 18, 7); \
		fdMapCopyMetatilesRect(groundX, 30, 1, 1, 18, 15); \
		fdMapCopyMetatilesRect(upperX, 30, 1, 1, 74, 11); \
		fdMapCopyMetatilesRect(upperX, 30, 1, 1, 82, 7); \
		fdMapCopyMetatilesRect(upperX, 30, 1, 1, 82, 15); \
	}

/* Reveal anywhere in Gasp's pool room shows the hidden tiles in the pools, at
   metatiles (10, 11), (18, 7) and (18, 15). */
DEFINE_TRIAL_ROAD_SET_POOL_TILES(scTrialRoadRevealPoolTiles, 33, 35)

/* When Reveal wears off in Gasp's pool room, the tiles in the pools are hidden
   again. */
DEFINE_TRIAL_ROAD_SET_POOL_TILES(scTrialRoadHidePoolTiles, 34, 36)

/* Felix comes out of the last stage's doorway at the summit during the race and
   steps on the metatile in front of it, metatile (13, 19) on the left path or
   (18, 19) on the right: with the sound of a door, the dark doorway behind him
   shuts, a closed green door like the other path's. The game also gives
   metatile (77, 18) (or (82, 18) on the right) the walking rules of the
   metatile beside it. Coming back to the summit while the race is on, the door
   is shut again, without the sound. */
void scTrialRoadShutSummitDoorway(Sint32 sound)
{
	if(fdScriptGetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
		if(TrialRoadStateWork->mirrored == 0) {
			fdMapGridCopyMetatilesLayered(26, 79, 1, 2, 13, 79);
			fdMapGridCopyMetatileAttributes(78, 18, 1, 1, 77, 18);
		} else {
			fdMapGridCopyMetatilesLayered(26, 79, 1, 2, 18, 79);
			fdMapGridCopyMetatileAttributes(81, 18, 1, 1, 82, 18);
		}
		if(sound)
			fdScriptPlaySnd(SE_ITEM_BREAK);
	}
	fdScriptSetFlag(FLAG_SUMMIT_DOORWAY_SHUT);
}

/* At the summit during the race, stepping onto the top of the peak, metatile
   (13, 16) on the left path or (18, 16) on the right, either side of the round
   tile in the middle of the top, brings on Moapa: he speaks according to who
   arrived first, and the battle with him and his knights begins, chosen by how
   many doors he reached first. (The doorways below, metatiles (13, 19) and (18,
   19), are the two dark openings in the cliff under the top.) */
void scTrialRoadChallengeMoapa(void)
{
	Sint32 count = fdScriptGetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE);

	count += fdScriptGetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE + 1);
	count += fdScriptGetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE + 2);
	count += fdScriptGetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE + 3);
	if(fdScriptGetFlag(FLAG_PARTY_FIRST_AT_PEAK))
		scTrialRoadReachPeakFirst();
	else
		scTrialRoadReachPeakSecond();
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdScriptSetAfterBattleDestination(
		SCENE_TRIAL_ROAD_SUMMIT, TRIAL_ROAD_SUMMIT_AFTER_WIN);
	fdScriptSetDefeatDestination(
		SCENE_TRIAL_ROAD_SUMMIT, TRIAL_ROAD_SUMMIT_AFTER_DEFEAT);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MOAPA, 4 - count);
}

/* Moving or pushing the summit's stone pillar, at metatile (7, 20), one
   metatile east into column 8 settles it there for good. */
void scTrialRoadSetSummitPillar(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	if(FIELD_METATILE(actor->x.fixed) == 8) {
		fdScriptSetFlag(FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED);
		actor->motionFlg = 0;
		actor->y.fixed -= toFIXED(1);
	}
}

extern CUE	  TrialRoad_Cues[];
extern CUE	  TrialRoad_CaveRockCues[];
extern CUE	  TrialRoad_GaspCues[];
extern CUE	  TrialRoad_SummitCues[];
extern Sint32 TrialRoad_StagesCues[];

const CUE *scGetCues(void)
{
	Sint32 pattern = TrialRoadStateWork->pattern;

	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_STAGES)
		return TrialRoad_StagesCues[pattern & 7];
	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_CAVE_ROCK)
		return TrialRoad_CaveRockCues;
	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_GASP)
		return TrialRoad_GaspCues;
	if(MainRoutineStateWork.next == SCENE_TRIAL_ROAD_SUMMIT)
		return TrialRoad_SummitCues;
	return TrialRoad_Cues;
}

/* Readies the pillar stage: the middle pillar cannot be pushed until Whirlwind
   blows its weeds off, and Felix can jump between the pillars' tops. */
void scTrialRoadSetUpPillarStage(void)
{
	if(!fdScriptGetFlag(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY))
		PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_C)) = 1;
	else
		fdActorSetPos(ACTOR_WEEDS_A, 0, 0);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_A);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_B);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_C);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_D);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_E);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_B);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_C);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_D);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_E);
	sySetEvent(scTrialRoadCheckLeaderOnPillars, EVENT_PASS_FRAME);
}

/* Readies the torch stage: its pillars of ice cannot be pushed, and those
   already melted stay puddles. */
void scTrialRoadSetUpTorchStage(void)
{
	scTrialRoadAddPillarTop(ACTOR_PILLAR_A);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_B);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_C);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_D);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_E);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_F);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_A);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_B);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_C);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_D);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_E);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_F);
	PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_C)) = 1;
	PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_D)) = 1;
	PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_E)) = 1;
	PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_F)) = 1;
	if(!fdScriptGetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_C))
		fdActorSetAnimation(ACTOR_PILLAR_C, 5);
	else
		scTrialRoadSetPillarTopLift(ACTOR_PILLAR_C, -PILLAR_HEIGHT);
	if(!fdScriptGetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_D))
		fdActorSetAnimation(ACTOR_PILLAR_D, 5);
	else
		scTrialRoadSetPillarTopLift(ACTOR_PILLAR_D, -PILLAR_HEIGHT);
	if(!fdScriptGetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_E))
		fdActorSetAnimation(ACTOR_PILLAR_E, 5);
	else
		scTrialRoadSetPillarTopLift(ACTOR_PILLAR_E, -PILLAR_HEIGHT);
	if(!fdScriptGetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_F))
		fdActorSetAnimation(ACTOR_PILLAR_F, 5);
	else
		scTrialRoadSetPillarTopLift(ACTOR_PILLAR_F, -PILLAR_HEIGHT);
}

/* Readies the weeds stage: its two overgrown pillars cannot be pushed until
   Whirlwind blows their weeds off. */
void scTrialRoadSetUpWeedsPillarStage(void)
{
	if(!fdScriptGetFlag(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A))
		PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_A)) = 1;
	else
		fdActorSetPos(ACTOR_WEEDS_A, 0, 0);
	if(!fdScriptGetFlag(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B))
		PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_B)) = 1;
	else
		fdActorSetPos(ACTOR_WEEDS_B, 0, 0);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_A);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_B);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_C);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_D);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_E);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_A);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_B);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_C);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_D);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_E);
}

/* Readies the Pound stage: its four stakes cannot be pushed, one of each pair
   stands raised, and the overgrown pillar cannot be pushed until Whirlwind
   blows its weeds off. */
void scTrialRoadSetUpPoundStage(void)
{
	PUSH_BLOCKED(fdActorGetById(ACTOR_WEST_STAKE)) = 1;
	PUSH_BLOCKED(fdActorGetById(ACTOR_INNER_WEST_STAKE)) = 1;
	PUSH_BLOCKED(fdActorGetById(ACTOR_INNER_EAST_STAKE)) = 1;
	PUSH_BLOCKED(fdActorGetById(ACTOR_EAST_STAKE)) = 1;
	if(!fdScriptGetFlag(FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY))
		PUSH_BLOCKED(fdActorGetById(ACTOR_PILLAR_B)) = 1;
	else
		fdActorSetPos(ACTOR_POUND_STAGE_WEEDS, 0, 0);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_A);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_B);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_C);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_D);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_E);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_F);
	scTrialRoadAddPillarTop(ACTOR_EAST_STAKE);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_A);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_B);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_C);
	if(!fdScriptGetFlag(FLAG_INNER_WEST_STAKE_RAISED)) {
		scTrialRoadRaiseStake(ACTOR_WEST_STAKE);
		scTrialRoadLowerStake(ACTOR_INNER_WEST_STAKE);
	} else {
		scTrialRoadRaiseStake(ACTOR_INNER_WEST_STAKE);
		scTrialRoadLowerStake(ACTOR_WEST_STAKE);
	}
	if(!fdScriptGetFlag(FLAG_EAST_STAKE_RAISED)) {
		scTrialRoadRaiseStake(ACTOR_INNER_EAST_STAKE);
		scTrialRoadLowerStake(ACTOR_EAST_STAKE);
	} else {
		scTrialRoadRaiseStake(ACTOR_EAST_STAKE);
		scTrialRoadLowerStake(ACTOR_INNER_EAST_STAKE);
	}
}

void scMagmaRockInsideRestoreCrumblingPillars(const Uint16 *table);

/* Readies the crumbling stage: the cracked pillars already crumbled stay
   gone. */
void scTrialRoadSetUpCrumblingStage(void)
{
	scMagmaRockInsideRestoreCrumblingPillars(TrialRoad_CrumblingPillars);
}

/* Readies the log stage: the logs roll by their own scripts rather than sliding
   like blocks, their metatiles are walls (scTrialRoadBlockLogs), and Felix can
   jump onto the block. */
void scTrialRoadSetUpLogStage(void)
{
	fdMapGridCopyMetatileAttributes(11, 22, 11, 8, 11, 64);
	fdMapGridCopyMetatileAttributes(79, 22, 11, 8, 79, 64);
	fdActorGetById(ACTOR_UPPER_LOG)->sceneCnt = 1;
	fdActorGetById(ACTOR_LOWER_LOG)->sceneCnt = 1;
	fdActorGetById(ACTOR_EAST_LOG)->sceneCnt = 1;
	fdActorGetById(ACTOR_UPPER_LOG)->collisionFlg = 0;
	fdActorGetById(ACTOR_LOWER_LOG)->collisionFlg = 0;
	fdActorGetById(ACTOR_EAST_LOG)->collisionFlg = 0;
	scTrialRoadBlockLogs();
	scTrialRoadAddPillarTop(ACTOR_PILLAR_A);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_A);
}

/* One of the fountain stage's fountains: the vent its water rises from, and the
   platform the leader rides on top of it. */
typedef struct TRIAL_ROAD_FOUNTAIN {
	Sint16 id;		  /* its vent; 0 for an empty slot */
	Sint16 phase;	  /* frames into its period */
	Sint16 height;	  /* how far it has risen, in pixels */
	Sint16 periodFrm; /* frames in one rise and fall */
	Sint16 riseFrm;	  /* the phase it starts to rise at */
	Sint16 fallFrm;	  /* the phase it starts to fall at */
	Sint16 high;
	Sint16 low;
	Sint16
		/* set to 1 as the fountain is added; nothing reads it */
		   unusedAfterLow;
	Sint16 speed; /* pixels a frame, 4 rising and -8 falling */
	Sint16
		/* cleared as the fountain is added; nothing reads it */
		   unusedAfterSpeed;
	Sint16 bob; /* the top's bob, a sine phase */
	ACTOR *top; /* an unseen platform on the water the leader rides */
} TRIAL_ROAD_FOUNTAIN;

/* The fountains the fountain stage's state has room for. */
#define FOUNTAIN_COUNT		  16
/* The sprites the fountains' columns may queue in a frame. */
#define FOUNTAIN_SPRITE_COUNT 80
/* A fountain's sprites sort at their screen row plus this many depth slots,
   four behind an actor's sprite at the same place (field_sprite_draw.c adds
   62), so the leader riding a fountain is drawn in front of its water. */
#define FOUNTAIN_DEPTH_OFFSET 58

/* The fountain stage's state, kept in the inventory snapshot's spare room. */
typedef struct TRIAL_FOUNTAINS_WORK {
	Sint16 vramEntry;
	Sint16 shape; /* the first of the shape grid's values for the fountains */
	TRIAL_ROAD_FOUNTAIN fountains[FOUNTAIN_COUNT];
	Uint32			   *sprs;
} TRIAL_FOUNTAINS_WORK;

/* The fountain stage's fountains, kept in the inventory snapshot's spare
   bytes. */
#define TRIAL_ROAD_FOUNTAINS \
	((TRIAL_FOUNTAINS_WORK *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE))

TRIAL_ROAD_FOUNTAIN *scTrialRoadAddFountain(
	Sint32 slot, Sint32 id, Sint32 timer, Sint32 low, Sint32 high);

/* How far the fountain stage's northern three fountains rise, and its southern
   two. */
#define NORTH_FOUNTAIN_HEIGHT 64
#define SOUTH_FOUNTAIN_HEIGHT 32

/* Readies the fountain stage: its five vents' fountains rise and fall together,
   the northern three buried until Scoop digs them out; Felix rides a risen
   fountain as a step up, and can jump onto the block. */
void scTrialRoadSetUpFountainStage(void)
{
	TRIAL_ROAD_FOUNTAIN *fountain;

	scTrialRoadStartFountains(0, FOUNTAIN_PERIOD);
	fountain =
		scTrialRoadAddFountain(0, ACTOR_WEST_VENT, 0, 0, NORTH_FOUNTAIN_HEIGHT);
	fountain->periodFrm = FOUNTAIN_PERIOD;
	fountain->riseFrm = FOUNTAIN_RISE_AT;
	fountain->fallFrm = FOUNTAIN_FALL_AT;
	fountain = scTrialRoadAddFountain(
		1, ACTOR_MIDDLE_VENT, 0, 0, NORTH_FOUNTAIN_HEIGHT);
	fountain->periodFrm = FOUNTAIN_PERIOD;
	fountain->riseFrm = FOUNTAIN_RISE_AT;
	fountain->fallFrm = FOUNTAIN_FALL_AT;
	fountain =
		scTrialRoadAddFountain(2, ACTOR_EAST_VENT, 0, 0, NORTH_FOUNTAIN_HEIGHT);
	fountain->periodFrm = FOUNTAIN_PERIOD;
	fountain->riseFrm = FOUNTAIN_RISE_AT;
	fountain->fallFrm = FOUNTAIN_FALL_AT;
	fountain = scTrialRoadAddFountain(
		3, ACTOR_SOUTHWEST_VENT, 0, 0, SOUTH_FOUNTAIN_HEIGHT);
	fountain->periodFrm = FOUNTAIN_PERIOD;
	fountain->riseFrm = FOUNTAIN_RISE_AT;
	fountain->fallFrm = FOUNTAIN_FALL_AT;
	fountain = scTrialRoadAddFountain(
		4, ACTOR_SOUTHEAST_VENT, 0, 0, SOUTH_FOUNTAIN_HEIGHT);
	fountain->periodFrm = FOUNTAIN_PERIOD;
	fountain->riseFrm = FOUNTAIN_RISE_AT;
	fountain->fallFrm = FOUNTAIN_FALL_AT;
	if(!fdScriptGetFlag(FLAG_VENT_UNCOVERED_BASE + ACTOR_WEST_VENT))
		fdActorSetPos(ACTOR_WEST_VENT, 0, 0);
	else
		scTrialRoadOpenVentMetatile(ACTOR_WEST_VENT);
	if(!fdScriptGetFlag(FLAG_VENT_UNCOVERED_BASE + ACTOR_MIDDLE_VENT))
		fdActorSetPos(ACTOR_MIDDLE_VENT, 0, 0);
	else
		scTrialRoadOpenVentMetatile(ACTOR_MIDDLE_VENT);
	if(!fdScriptGetFlag(FLAG_VENT_UNCOVERED_BASE + ACTOR_EAST_VENT))
		fdActorSetPos(ACTOR_EAST_VENT, 0, 0);
	else
		scTrialRoadOpenVentMetatile(ACTOR_EAST_VENT);
	fdMapSetUpperLevelWading(Ram_HeapSlots->mapGrid);
	scTrialRoadAddPillarTop(ACTOR_PILLAR_A);
	scTrialRoadSetJumpMetatileUnder(ACTOR_PILLAR_A);
}

/* Readies the sand stage: its tightrope between two posts, and the vine on the
   cliff, climbable once Whirlwind has blown its weeds away. */
void scTrialRoadSetUpSandStage(void)
{
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_WEST_POST, ACTOR_EAST_POST);
	if(!fdScriptGetFlag(FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY))
		fdMapGridCopyMetatileAttributes(18, 21, 1, 1, 19, 21);
	else
		fdActorSetPos(ACTOR_SAND_STAGE_WEEDS, 0, 0);
}

/* The cave beyond the summit needs no setting up. */
void scTrialRoadSetUpCave(void)
{
}

/* Gasp moves opposite to Felix about the hidden tile at metatile (18, 15):
   whatever way Felix steps, Gasp steps the other. Once Felix stands on that
   tile, Gasp cries out and hops away to metatile (21, 16), there to be talked
   to. */
void scTrialRoadGaspFlees(void)
{
	ACTOR *gasp = fdActorGetById(ACTOR_GASP);
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 dx;
	Sint32 dz;
	Sint32 gaspDistanceSquared;
	Sint32 spotDistanceSquared;
	FIXED  y;

	if(fdScriptGetFlag(FLAG_GASP_FLED))
		return;
	dx = FIXEDtoINT(leader->x.fixed - gasp->x.fixed);
	dz = FIXEDtoINT(leader->z.fixed - gasp->z.fixed);
	gaspDistanceSquared = dx * dx + dz * dz;
	dx = (FIXEDtoINT(leader->x.fixed)) - 296;
	dz = (FIXEDtoINT(leader->z.fixed)) - 248;
	spotDistanceSquared = dx * dx + dz * dz;
	if(spotDistanceSquared <= 80 * 80 || gaspDistanceSquared <= 48 * 48)
		fdActorSetMoveTarget(
			gasp, toFIXED(296 - dx), gasp->y.fixed, toFIXED(248 - dz));
	if(spotDistanceSquared >= 4 * 4)
		return;
	fdScriptPlaySnd(SE_ENEMY_CRY);
	fdScriptPlaySnd(SE_DJINNI_FLEES);
	gasp->speedLimit = toFIXED(2);
	gasp->acceleration = toFIXED(2);
	gasp->motionFlg = ACTOR_FALLS;
	gasp->groundHeight = 0;
	gasp->collisionFlg = ACTOR_SOLID;
	gasp->velY = toFIXED(10);
	fdActorSetScript(ACTOR_GASP, TrialRoad_GaspHopScript);
	y = gasp->y.fixed;
	fdActorSetMoveTarget(gasp, toFIXED(344), y, toFIXED(264));
	fdScriptSetFlag(FLAG_GASP_FLED);
}

/* Whether the palette cycle runs; the frame it has reached. */
extern Sint32 TrialRoad_PaletteCycleRunning;

/* Readies Gasp's pool room, unless Gasp has joined the party: he floats over
   the water until he first hops away, and stands on the ground after; each
   frame he mirrors Felix (scTrialRoadGaspFlees). */
void scTrialRoadSetUpPoolRoom(void)
{
	if(!fdScriptGetFlag(FLAG_DJINNI(DJINNI_GASP))) {
		ACTOR *actor = fdActorGetById(ACTOR_GASP);
		Sint32 started = fdScriptGetFlag(FLAG_GASP_FLED);
		Uint8 *flags = &actor->motionFlg;

		if(!started) {
			*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
			actor->y.fixed += toFIXED(17) / 2;
		} else {
			*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			actor->y.fixed = 0;
		}
		sySetEvent(scTrialRoadGaspFlees, EVENT_PASS_FRAME);
	}
}

/* Starts or stops the summit's colour cycle. */
void scTrialRoadSetPaletteCycleRunning(Sint32 value)
{
	TrialRoad_PaletteCycleRunning = value;
}

extern Sint32 TrialRoad_PaletteCycleFrame;

/* Sets the palette cycle's frame counter: the scPaletteCycleSetFrame of the
   Jupiter and Mars Lighthouse rooms, whose palette cycles this one copies (the
   name is ours). */
static inline void scPaletteCycleSetFrame(Sint32 value)
{
	TrialRoad_PaletteCycleFrame = value;
}

/* Each frame: turns the summit's five purples over by one every four frames and
   writes the first four of them to BG palette 5's colours 2 to 5; it pauses
   while a Psynergy other than Hover, Reveal or the menu is up. */
void scTrialRoadStepPaletteCycle(void)
{
	Uint16	colors[16];
	Sint32	phase = (TrialRoad_PaletteCycleFrame / 4) % 5;
	Sint32	i;
	Sint32	offset;
	Uint16 *p;

	if(TrialRoad_PaletteCycleRunning != 0) {
		FIELD_STATE *fieldState = Ram_HeapSlots->fieldState;
		Sint16		*state = &fieldState->psynergy;

		if(*state != 0 && ABILITY_ID(*state) != ABILITY_HOVER)
			return;
		if(fieldState->revealActive != 0)
			return;
		if(fieldState->pauseMenu != 0)
			return;
	}
	i = 0;
	offset = 4;
	do {
		p = colors;
		{
			Sint32 slot = (phase + i) % 5;
			Uint8 *table = (Uint8 *)TrialRoad_PaletteCycleColors;

			colors[slot] = *(Uint16 *)(table + offset);
		}
		i++;
		offset += 2;
	} while(i <= 4);
	syDMAXCopy(p, (void *)&BG_PLTT_COLOR(5, 2), DMA_COPY_WORDS(2), REG_DMA3);
	TrialRoad_PaletteCycleFrame++;
}

/* Starts the summit's colour cycle, loading its colours unless mode is 2; mode
   1 also starts its event. On the summit the purple stripes along the pale
   stone channels that run down the cliff face, and the round purple Hover pad
   at their foot, shimmer: their five shades of purple turn over every four
   frames, so the light seems to pulse along the channels and ripple in the
   pad's rings. */
void scTrialRoadStartPaletteCycle(Sint32 mode)
{
	TrialRoad_PaletteCycleRunning = 1;
	if(mode != 2)
		syWordsCopy(
			TrialRoad_PaletteCycleColors, (void *)&BG_PLTT_COLOR(5, 0), 32);
	if(mode == 1) {
		scPaletteCycleSetFrame(0);
		sySetEvent(scTrialRoadStepPaletteCycle, EVENT_PASS_FRAME);
	}
}

void scTrialRoadLeaveMoapaWaiting(void);

/* Readies the summit: after the battle, Moapa's surrender or his scorn; during
   the race, Moapa and his knights waiting at the top if they got there first,
   or nobody yet. */
void scTrialRoadSetUpPeak(void)
{
	Sint32 entrance = MainRoutineStateWork.entrance;

	if(entrance == TRIAL_ROAD_SUMMIT_AFTER_WIN) {
		gmInventorySnapshotRestore();
		scTrialRoadBeatMoapa();
		syImportScriptSetExitRequest(TRIAL_ROAD_SUMMIT_AFTER_WIN);
	} else if(entrance == TRIAL_ROAD_SUMMIT_AFTER_DEFEAT) {
		gmInventorySnapshotRestore();
		scTrialRoadLoseToMoapa();
		syImportScriptSetExitRequest(TRIAL_ROAD_SUMMIT_AFTER_DEFEAT);
	} else if(fdScriptGetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
		if(fdScriptGetFlag(FLAG_SUMMIT_DOORWAY_SHUT))
			scTrialRoadShutSummitDoorway(0);
		if(fdScriptGetFlag(FLAG_PARTY_FIRST_AT_PEAK))
			scTrialRoadLeaveMoapaWaiting();
		else
			scTrialRoadPlaceMoapaAtPeak();
	}
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	scTrialRoadStartPaletteCycle(1);
	scTrialRoadAddPillarTop(ACTOR_PEAK_PILLAR);
	if(fdScriptGetFlag(FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED))
		fdActorSetPos(ACTOR_PEAK_PILLAR, 136, 328);
}

/* Each stage pattern's set-up, by the pattern's low three bits. */
extern void (*TrialRoad_StageSetUps[])(void);

Sint32 scInit(void)
{
	Sint32 scene = MainRoutineStateWork.next;
	Sint32 entrance = MainRoutineStateWork.entrance;
	Sint32 pattern = TrialRoadStateWork->pattern;

	if(FieldStateWork->exit != 0)
		return 0;
	if(scene == SCENE_TRIAL_ROAD_STAGES) {
		if(entrance <= 7) {
			fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
			Value0((Sint32 (*)())TrialRoad_StageSetUps[pattern & 7]);
		} else if(entrance == TRIAL_ROAD_RULES_COURSE) {
			scTrialRoadShowTraps();
		}
	}
	if(scene == SCENE_TRIAL_ROAD_CAVE_ROCK)
		scTrialRoadSetUpCave();
	if(scene == SCENE_TRIAL_ROAD_GASP)
		scTrialRoadSetUpPoolRoom();
	if(scene == SCENE_TRIAL_ROAD_SUMMIT) {
		fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
		scTrialRoadSetUpPeak();
	}
	return 0;
}

/* Copies a rectangle of map metatiles and their shapes, last row first, so that
   overlapping rectangles copy whole. */
void scTrialRoadCopyMetatilesBackward(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 dstX, Sint32 dstY)
{
	Uint32 *metatiles = (Uint32 *)Ram_MapMetatileBuffer;
	Sint32	dst = (dstY << 7) + dstX;
	Uint8  *dstShapes = MapShapeGrid + dst;
	Sint32	src = (srcY << 7) + srcX;
	Uint8  *srcShapes = MapShapeGrid + src;
	Uint32 *dstMetatiles = metatiles + dst;
	Sint16	row;
	Sint16	col;

	metatiles += src;
	for(row = height - 1; row >= 0; row--) {
		Uint32 *dstMetatile = dstMetatiles + ((row << 7) + width) - 1;
		Uint32 *srcMetatile = metatiles + ((row << 7) + width) - 1;
		Uint8  *dstShape = dstShapes + (row << 7) + width - 1;
		Uint8  *srcShape = srcShapes + (row << 7) + width - 1;

		for(col = width - 1; col >= 0; col--) {
			*dstMetatile-- = *srcMetatile--;
			*dstShape = *srcShape;
			dstShape--;
			srcShape--;
		}
	}
}

void scTrialRoadOpenRivalDoor(Sint32 mirrored, Sint32 stage);

/* Each frame of the race, moves Moapa's party on by its recording, and opens
   his door when he reaches it. */
void scTrialRoadUpdateRival(void)
{
	TRIAL_ROAD_STATE *state = TrialRoadStateWork;
	TRIAL_ROAD_STATE *road = TrialRoadStateWork;
	Sint32			  side;

	if(state->stage[1] > 3)
		return;
	if(scTrialRoadRivalIsWaiting() != 0) {
		if(state->rivalTmr == 0) {
			side = scTrialRoadRivalIsAhead(1) ^ 1;
			scTrialRoadPlayRivalRecording(TrialRoad_RivalRecordings[side + 8]);
			/* Moapa's time at the chests counts from 1 when he reached the
			   switch first, with one chest to fill, and from 601 when Felix
			   did, with two. */
			state->rivalTmr = side * 5 * 120 + 1;
		} else {
			if(state->stage[1] >= state->rivalBest)
				state->rivalBest = state->stage[1] + 1;
			if(++road->stage[1] <= 3) {
				Sint32 pattern = state->order[state->stage[1] * 2 +
					(Sint16)(state->mirrored ^ 1)];

				scTrialRoadPlayRivalRecording(
					TrialRoad_RivalRecordings[pattern & 7]);
				fdActorGetById(ACTOR_MOAPA)->groundHeight = 0;
				fdActorGetById(ACTOR_MOAPA)->y.fixed = 0;
			} else {
				fdActorSetPos(ACTOR_MOAPA, 0, 0);
			}
			state->rivalTmr = 0;
		}
	}
	{
		Uint8 *kind = &fdActorGetById(ACTOR_MOAPA)->sprStorageKind;

		if(state->stage[0] == state->stage[1])
			*kind = 1;
		else
			*kind = 0;
	}
	if(state->rivalTmr == 0)
		return;
	if(syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS))
		return;
	if(state->stage[0] == state->stage[1]) {
		switch(state->rivalTmr) {
		case 1:
		case 601:
			sySndPlay(SE_CHEST_OPEN);
			if(state->mirrored != 0) {
				fdActorSetAnimationById(ACTOR_LEFT_INNER_CHEST, 6);
				if(state->rivalTmr > 600)
					fdActorSetAnimationById(ACTOR_LEFT_OUTER_CHEST, 6);
			} else {
				fdActorSetAnimationById(ACTOR_RIGHT_INNER_CHEST, 6);
				if(state->rivalTmr > 600)
					fdActorSetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 6);
			}
			break;
		case 655:
			sySndPlay(SE_CHEST_CLOSE);
			if(state->mirrored != 0)
				fdActorSetAnimationById(ACTOR_LEFT_OUTER_CHEST, 5);
			else
				fdActorSetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 5);
			break;
		case 653:
		case 708:
			fdActorGetById(ACTOR_MOAPA)->facing = FACING_NORTH;
			break;
		case 50:
		case 710:
			sySndPlay(SE_CHEST_CLOSE);
			if(state->mirrored != 0)
				fdActorSetAnimationById(ACTOR_LEFT_INNER_CHEST, 5);
			else
				fdActorSetAnimationById(ACTOR_RIGHT_INNER_CHEST, 5);
			break;
		case 70:
		case 730:
			scTrialRoadOpenRivalDoor(state->mirrored, state->stage[1]);
			sySndPlay(SE_ITEM_BREAK);
			break;
		}
	}
	road->rivalTmr++;
}

extern Sint16 TrialRoad_StageMapMetatiles[][2];
extern Sint8  TrialRoad_RulesTreasureStages[];
extern Sint8  TrialRoad_TreasureStages[];

void scTrialRoadStartRival(Sint32 mirrored);

/* Places `layer` `column` metatiles into the map: its offset (column << 20),
   its metatiles and terrain from that column on, and its position from the
   view. */
#define SET_LAYER(column, x) \
	words = (Sint32 *)layer; \
	words[2] = FIELD_METATILE_ORIGIN(column); \
	words[3] = row; \
	x = (column); \
	layer->metatiles = (MAP_METATILE *)Ram_MapMetatileBuffer + x; \
	layer->terrainIds = MapShapeGrid + x; \
	words[0] = syMulFX(*viewX, words[4]) + words[2]; \
	words[1] = syMulFX(*viewZ, words[5]) + words[3]

Sint32 scPrepareMap(void)
{
	Sint32 scene = MainRoutineStateWork.next;
	Sint32 entrance = MainRoutineStateWork.entrance;

	if(scene == SCENE_TRIAL_ROAD_STAGES) {
		MAP_STATE		 *map = Ram_HeapSlots->mapGrid;
		TRIAL_ROAD_STATE *state = TrialRoadStateWork;
		ACTOR			 *leader;
		Uint8			 *unit;
		Sint32			  value;
		Sint32			  n;
		Sint32			  px;
		Uint32			  i;

		if((Uint32)(entrance -
			   TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST) <= 1) {
			if(!syGameFlagGet(FLAG_STAGE_ORDER_DEALT)) {
				unit = syUnitGet(MainRoutineStateWork.leaderActorId);
				syWordsClearEntry(state, sizeof(*state));
				value = 0;
				for(n = 0; n < UNIT_NAME_SIZE - 1; n++)
					value += unit[n];
				scTrialRoadDealStageOrder(value);
				syGameFlagSet(FLAG_STAGE_ORDER_DEALT);
			}
			state->mirrored = (entrance - 1) & 1;
			state->stage[0] = 0;
			state->stage[1] = 0;
			state->leaderBest = 0;
			state->rivalBest = 0;
			value = state->order[state->mirrored ^ 1];
			scTrialRoadPlayRivalRecording(
				(Sint32)TrialRoad_RivalRecordings[value & 7]);
			state->pattern =
				state->order[state->stage[0] * 2 + state->mirrored];
			/* From the village, in at the foot of the left or right path. */
			n = entrance % 10;
			fdScriptSetDestination(SCENE_TRIAL_ROAD_STAGES, n);
		} else if((Uint32)(entrance -
					  TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST) <= 1) {
			if(!syGameFlagGet(FLAG_STAGE_ORDER_DEALT)) {
				unit = syUnitGet(MainRoutineStateWork.leaderActorId);
				syWordsClearEntry(state, sizeof(*state));
				value = 0;
				for(n = 0; n < UNIT_NAME_SIZE - 1; n++)
					value += unit[n];
				scTrialRoadDealStageOrder(value);
				syGameFlagSet(FLAG_STAGE_ORDER_DEALT);
			}
			{
				Sint16 side = (entrance - 1) & 1;

				state->mirrored = side;
				state->stage[0] = 3;
				state->stage[1] = 3;
				state->leaderBest = 4;
				state->rivalBest = 4;
				{
					/* The rival runs stage 3 on the other side of the
					   course. */
					Sint32 rivalSlot = 3 * 2 + (side ^ 1);

					value = state->order[rivalSlot];
				}
			}
			scTrialRoadPlayRivalRecording(
				(Sint32)TrialRoad_RivalRecordings[value & 7]);
			state->pattern =
				state->order[state->stage[0] * 2 + state->mirrored];
			/* From the summit, in at the top of the left or right path. */
			n = entrance % 10;
			fdScriptSetDestination(SCENE_TRIAL_ROAD_STAGES, n);
		} else if(entrance == TRIAL_ROAD_STAGES_FOR_TRIAL_RULES) {
			unit = syUnitGet(MainRoutineStateWork.leaderActorId);
			syWordsClearEntry(state, sizeof(*state));
			value = 0;
			for(n = 0; n < UNIT_NAME_SIZE - 1; n++)
				value += unit[n];
			scTrialRoadDealStageOrder(value);
			state->mirrored = 0;
			state->stage[0] = 0;
			state->stage[1] = 0;
			state->leaderBest = 0;
			state->rivalBest = 0;
			state->pattern = state->order[0];
			fdScriptSetDestination(
				SCENE_TRIAL_ROAD_STAGES, TRIAL_ROAD_RULES_COURSE);
		} else {
			Sint32	  *viewX = map->view;
			Sint32	  *viewZ = &map->view[1];
			MAP_LAYER *layer;
			Sint32	  *words;
			Sint32	   row;

			/* The map holds the room three times, for layers 0 to 2, 34
			   metatiles apart, with the course patterns south of them: each
			   redraw below is made on all three. Rows 0 to 12 are the stage's
			   top (the doors, the chests and the switch), rows 13 to 33 the
			   arena that a pattern fills, and rows 34 to 43 its foot. */
			if(entrance == TRIAL_ROAD_RULES_COURSE) {
				Sint32 x1;
				Sint32 x2;

				row = 0;
				layer = &map->layers[1];
				SET_LAYER(34, x1);
				layer = &map->layers[2];
				SET_LAYER(68, x2);
				fdActorSetSprCol(fdActorGetById(ACTOR_LEFT_INNER_CHEST), 3);
				fdActorSetSprCol(fdActorGetById(ACTOR_LEFT_OUTER_CHEST), 3);
				fdActorSetSprCol(fdActorGetById(ACTOR_RIGHT_INNER_CHEST), 3);
				fdActorSetSprCol(fdActorGetById(ACTOR_RIGHT_OUTER_CHEST), 3);
				/* The course Moapa shows: the arena is cleared, each copy of
				   the blank metatile at its west edge carried across it, and
				   the dealt pattern drawn from column 1. */
				fdMapCopyRawMetatilesRect(0, 12, 1, 21, 0, 13);
				fdMapCopyRawMetatilesRect(x1, 12, 1, 21, x1, 13);
				fdMapCopyRawMetatilesRect(x2, 12, 1, 21, x2, 13);
				fdMapCopyRawMetatilesRect(0, 13, 33, 21, 1, 13);
				fdMapCopyRawMetatilesRect(x1, 13, 33, 21, x1 + 1, 13);
				fdMapCopyRawMetatilesRect(x2, 13, 33, 21, x2 + 1, 13);
				value = state->pattern;
				px = TrialRoad_StageMapMetatiles[value][0];
				row = TrialRoad_StageMapMetatiles[value][1];
				fdMapCopyRawMetatilesRect(px, row, 13, 21, 1, 13);
				fdMapCopyRawMetatilesRect(px + 42, row, 13, 21, x1 + 1, 13);
				fdMapCopyRawMetatilesRect(px + 84, row, 13, 21, x2 + 1, 13);
				for(i = 0; i <= 9; i++) {
					if(TrialRoad_RulesTreasureStages[i] != value)
						fdActorSetPos(i + FIELD_TREASURE_ACTOR, 0, 0);
				}
				fdMapGridCenterOnFollowedPoint();
			} else {
				Sint32 x1;
				Sint32 x2;
				Sint32 destinationY;

				row = 0;
				layer = &map->layers[1];
				SET_LAYER(34, x1);
				layer = &map->layers[2];
				SET_LAYER(68, x2);
				value = 1;
				/* The room is drawn for the leader's path: from the left path
				   the top and the foot slide 9 metatiles east, and from the
				   right path 10 metatiles west with the four blue chests
				   following; the strip either leaves is blanked by carrying a
				   blank metatile across it. */
				if(entrance & value) {
					scTrialRoadCopyMetatilesBackward(0, 0, 102, 13, 9, 0);
					fdMapCopyRawMetatilesRect(0, 0, 10, 1, 1, 0);
					fdMapCopyRawMetatilesRect(0, 0, 10, 12, 0, 1);
					fdMapCopyRawMetatilesRect(x1, 0, 10, 12, x1, 1);
					fdMapCopyRawMetatilesRect(x2, 0, 10, 12, x2, 1);
					scTrialRoadCopyMetatilesBackward(0, 34, 102, 10, 9, 34);
					fdMapCopyRawMetatilesRect(0, 33, 10, 10, 0, 34);
					fdMapCopyRawMetatilesRect(x1, 33, 10, 10, x1, 34);
					fdMapCopyRawMetatilesRect(x2, 33, 10, 10, x2, 34);
				} else {
					fdMapCopyRawMetatilesRect(10, 0, 92, 13, 0, 0);
					fdMapCopyRawMetatilesRect(23, 0, 10, 1, 24, 0);
					fdMapCopyRawMetatilesRect(23, 0, 10, 12, 23, 1);
					fdMapCopyRawMetatilesRect(57, 0, 10, 12, 57, 1);
					fdMapCopyRawMetatilesRect(91, 0, 10, 12, 91, 1);
					fdMapCopyRawMetatilesRect(10, 34, 92, 10, 0, 34);
					fdMapCopyRawMetatilesRect(24, 33, 10, 10, 24, 34);
					fdMapCopyRawMetatilesRect(58, 33, 10, 10, 58, 34);
					fdMapCopyRawMetatilesRect(92, 33, 10, 10, 92, 34);
					fdActorSetPos(ACTOR_LEFT_INNER_CHEST, 40, 152);
					fdActorSetPos(ACTOR_LEFT_OUTER_CHEST, 24, 152);
					fdActorSetPos(ACTOR_RIGHT_INNER_CHEST, 184, 152);
					fdActorSetPos(ACTOR_RIGHT_OUTER_CHEST, 200, 152);
				}
				fdActorSetSprCol(fdActorGetById(ACTOR_LEFT_INNER_CHEST), 3);
				fdActorSetSprCol(fdActorGetById(ACTOR_LEFT_OUTER_CHEST), 3);
				fdActorSetSprCol(fdActorGetById(ACTOR_RIGHT_INNER_CHEST), 3);
				fdActorSetSprCol(fdActorGetById(ACTOR_RIGHT_OUTER_CHEST), 3);
				/* The stage's own pattern fills the arena from column 10, and
				   the other stages' treasures are put away. */
				destinationY = 13;
				value = state->pattern;
				px = TrialRoad_StageMapMetatiles[value][0];
				row = TrialRoad_StageMapMetatiles[value][1];
				fdMapCopyRawMetatilesRect(px, row, 13, 21, 10, destinationY);
				fdMapCopyRawMetatilesRect(
					px + 42, row, 13, 21, 44, destinationY);
				fdMapCopyRawMetatilesRect(
					px + 84, row, 13, 21, 78, destinationY);
				for(i = 0; i <= 9; i++) {
					if(TrialRoad_TreasureStages[i] != value)
						fdActorSetPos(i + FIELD_TREASURE_ACTOR, 0, 0);
				}
				fdMapGridCenterOnFollowedPoint();
				leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
				if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
					leader->y.fixed = leader->groundHeight =
						fdMapGetTerrainHeight(
							0, leader->x.fixed, leader->z.fixed);
				if(syGameFlagGet(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
					TRIAL_ROAD_STATE *road;

					syGameFlagSet(FLAG_RETREAT_BLOCKED);
					road = TrialRoadStateWork;
					if(road->stage[0] < road->leaderBest ||
						syGameFlagGet(FLAG_STAGE_DOOR_OPEN)) {
						scTrialRoadOpenLeaderDoor(
							road->mirrored, road->stage[0]);
						syGameFlagSet(FLAG_STAGE_DOOR_OPEN);
						syGameFlagSet(FLAG_STAGE_CHESTS_OPENED);
					}
					if(road->stage[0] < road->rivalBest)
						scTrialRoadOpenRivalDoor(
							road->mirrored, road->stage[0]);
					scTrialRoadShowRaceMap(
						MainRoutineStateWork.leaderActorId, ACTOR_MOAPA);
					scTrialRoadStartRival((Sint16)(state->mirrored ^ 1));
					sySetEvent(scTrialRoadUpdateRival, EVENT_PASS_FRAME);
				} else {
					scTrialRoadOpenLeaderDoor(state->mirrored, state->stage[0]);
					syGameFlagSet(FLAG_STAGE_DOOR_OPEN);
					syGameFlagSet(FLAG_STAGE_CHESTS_OPENED);
					scTrialRoadOpenRivalDoor(state->mirrored, state->stage[0]);
				}
			}
		}
	} else if(scene == SCENE_TRIAL_ROAD_CAVE_ROCK) {
	} else if(scene == SCENE_TRIAL_ROAD_GASP) {
	} else if(scene == SCENE_TRIAL_ROAD_SUMMIT &&
		syGameFlagGet(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT) &&
		!syGameFlagGet(FLAG_PARTY_FIRST_AT_PEAK)) {
		/* During the race, the doorway of the path the party did not take is
		   drawn shut, as scTrialRoadShutSummitDoorway shuts the party's own. */
		if(TrialRoadStateWork->mirrored != 0)
			fdMapGridCopyMetatilesLayered(26, 79, 1, 2, 13, 79);
		else
			fdMapGridCopyMetatilesLayered(26, 79, 1, 2, 18, 79);
	}
	return 0;
}

extern Sint8 TrialRoad_KeyStages[];

/* Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
   eight slots at the start of the stage state, again until both the even and
   the odd slots hold at least one of the four values in TrialRoad_KeyStages;
   the random state is restored after. */
void scTrialRoadDealStageOrder(Uint32 seed)
{
	Sint16 *order = (Sint16 *)TrialRoadStateWork;
	Uint32	saved = RandomSeed;
	Sint32	counts[2];
	Sint32	i;
	Sint32	k;
	Sint32	offset;
	Sint8	value;

	RandomSeed = seed;
	do {
		for(i = 0; i <= 7; i++)
			order[i] = -1;
		for(i = 0; i <= 7; i++) {
			do {
				offset = syRandomBelow(8) * 2;
			} while(*(Sint16 *)((Uint8 *)order + offset) != -1);
			*(Sint16 *)((Uint8 *)order + offset) = i;
		}
		counts[1] = 0;
		counts[0] = 0;
		for(i = 0; i <= 7; i++) {
			for(k = 0; k <= 3; k++) {
				value = TrialRoad_KeyStages[k];
				if(order[i] == value)
					counts[i & 1]++;
			}
		}
	} while(counts[0] == 0 || counts[1] == 0);
	RandomSeed = saved;
}

/* The stage a side of the race has reached: 0 the leader's, 1 Moapa's. */
Sint32 scTrialRoadGetStage(Sint32 side)
{
	TRIAL_ROAD_STATE *state;

	state = TrialRoadStateWork;
	return state->stage[side & 1];
}

/* Whether the leader took the right path, on which the road is mirrored. */
Sint32 scTrialRoadIsMirrored(void)
{
	return TrialRoadStateWork->mirrored;
}

/* A pixel x on the mirrored road, the right path's, when the leader took it. */
Sint32 scTrialRoadMirrorX(Sint32 x)
{
	if(scTrialRoadIsMirrored())
		return 512 - x;
	return x;
}

/* A metatile x, in pixels, on the mirrored road when the leader took it. */
Sint32 scTrialRoadMirrorMetatileX(Sint32 x)
{
	if(scTrialRoadIsMirrored())
		return (64 - x) << 3;
	return x << 3;
}

/* An x offset of so many 8-pixel steps, in pixels, turned round on the mirrored
   road. */
Sint32 scTrialRoadMirrorOffset(Sint32 x)
{
	if(scTrialRoadIsMirrored())
		return -x << 3;
	return x << 3;
}

/* An angle on the mirrored road, turned round when it is. */
Sint32 scTrialRoadMirrorAng(Sint32 angle)
{
	if(scTrialRoadIsMirrored())
		return (DEGtoANG(180) - angle) & TRIG_ANGLE_MASK;
	return angle & TRIG_ANGLE_MASK;
}

/* Places an actor at a pixel position and facing, mirrored when the leader took
   the right path. */
void scTrialRoadPlaceActor(Sint32 id, Sint32 x, Sint32 z, Sint32 angle)
{
	fdActorSetPosAndFacing(id, toFIXED(scTrialRoadMirrorX(x)), toFIXED(z),
		(Uint16)scTrialRoadMirrorAng(angle));
}

/* Felix got to the summit first, so Moapa is not placed yet; he arrives when
   Felix walks up (scTrialRoadReachPeakFirst). */
void scTrialRoadLeaveMoapaWaiting(void)
{
}

/* Places Moapa and his two knights at the summit, waiting for the party. */
void scTrialRoadPlaceMoapaAtPeak(void)
{
	scTrialRoadPlaceActor(ACTOR_MOAPA, 288, 264, FACING_WEST);
	scTrialRoadPlaceActor(ACTOR_LEFT_KNIGHT, 296, 248, FACING_WEST);
	scTrialRoadPlaceActor(ACTOR_RIGHT_KNIGHT, 296, 280, FACING_WEST);
}

/* The camera pans to the summit, and the party walks up to its near side and
   turns to face the far one, where Moapa stands or will land. */
static inline void scTrialRoadGatherAtPeak(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(256, CAMERA_KEEP, 264, 1);
	fdActorPlaceOnActor(ACTOR_JENNA, ACTOR_FELIX);
	fdActorPlaceOnActor(ACTOR_SHEBA, ACTOR_FELIX);
	fdActorPlaceOnActor(ACTOR_PIERS, ACTOR_FELIX);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_SHEBA, 1);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorWalkTo(ACTOR_FELIX, scTrialRoadMirrorMetatileX(28), 272);
	fdActorWalkTo(ACTOR_PIERS, scTrialRoadMirrorMetatileX(28), 256);
	fdActorWalkTo(ACTOR_JENNA, scTrialRoadMirrorMetatileX(27), 248);
	fdActorWalkTo(ACTOR_SHEBA, scTrialRoadMirrorMetatileX(27), 280);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_FELIX, scTrialRoadMirrorAng(FACING_EAST), 0);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorFaceDirection(ACTOR_PIERS, scTrialRoadMirrorAng(FACING_EAST), 0);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorFaceDirection(ACTOR_JENNA, scTrialRoadMirrorAng(FACING_EAST), 0);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorFaceDirection(ACTOR_SHEBA, scTrialRoadMirrorAng(FACING_EAST), 0);
	fdCameraWaitForArrival();
}

/* The party gathers at the summit first, and Moapa and his knights arrive after
   them: "You were quite swift! You may prove a challenge yet!" */
void scTrialRoadReachPeakFirst(void)
{
	ACTOR *actor;

	scTrialRoadGatherAtPeak();
	sySynch(20);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	sySynch(20);
	scTrialRoadPlaceActor(ACTOR_MOAPA, 344, 296, FACING_NORTH);
	actor = fdActorGetById(ACTOR_MOAPA);
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->motionFlg = 0;
	fdActorSetMoveTarget(
		actor, actor->x.fixed, actor->y.fixed + toFIXED(88), actor->z.fixed);
	fdActorSelectAnimation(actor, 2);
	scFieldActorWaitForMove(actor);
	fdActorSelectAnimation(actor, 1);
	actor->motionFlg = ACTOR_FALLS;
	actor->groundHeight = toFIXED(128);
	actor->velY = toFIXED(4);
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	fdActorSetMoveTarget(
		actor, actor->x.fixed, actor->y.fixed, actor->z.fixed - toFIXED(16));
	scFieldActorWaitForMove(actor);
	sySynch(12);
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	scFieldActorSetMoveTargetFX(actor, toFIXED(scTrialRoadMirrorMetatileX(41)),
		actor->y.fixed, toFIXED(264));
	scFieldActorWaitForMove(actor);
	sySynch(6);
	fdScriptPlaySnd(SE_JUMP);
	actor->velY = toFIXED(6);
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	fdActorSetMoveTarget(actor, toFIXED(scTrialRoadMirrorMetatileX(37)),
		actor->y.fixed, toFIXED(264));
	scFieldActorWaitForMove(actor);
	sySynch(6);
	fdActorPlaceOnActor(ACTOR_LEFT_KNIGHT, ACTOR_MOAPA);
	fdActorPlaceOnActor(ACTOR_RIGHT_KNIGHT, ACTOR_MOAPA);
	fdActorSetSpeed(ACTOR_MOAPA, 1);
	fdActorSetSpeed(ACTOR_LEFT_KNIGHT, 1);
	fdActorSetSpeed(ACTOR_RIGHT_KNIGHT, 1);
	fdActorWalkTo(ACTOR_MOAPA, scTrialRoadMirrorMetatileX(36), 264);
	fdActorWalkTo(ACTOR_LEFT_KNIGHT, scTrialRoadMirrorMetatileX(37), 248);
	fdActorWalkTo(ACTOR_RIGHT_KNIGHT, scTrialRoadMirrorMetatileX(37), 280);
	fdActorWaitForMoveById(ACTOR_MOAPA);
	fdActorFaceDirection(ACTOR_MOAPA, scTrialRoadMirrorAng(FACING_WEST), 0);
	fdActorWaitForMoveById(ACTOR_LEFT_KNIGHT);
	fdActorFaceDirection(
		ACTOR_LEFT_KNIGHT, scTrialRoadMirrorAng(FACING_WEST), 0);
	fdActorWaitForMoveById(ACTOR_RIGHT_KNIGHT);
	fdActorFaceDirection(
		ACTOR_RIGHT_KNIGHT, scTrialRoadMirrorAng(FACING_WEST), 0);
	fdScriptSetMessageId((Sint32)MsgTrialRoadSwiftChallenge);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
	/* "You were quite swift! You may prove a challenge yet!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "However, just because you're fast doesn't mean you're strong...." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorRunRepeatedMotion(ACTOR_MOAPA, 2);
	/* "Let the battle begin!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdScriptWait(15);
	fdActorSetSpeed(ACTOR_FELIX, 2);
	fdActorSetSpeed(ACTOR_JENNA, 1.9);
	fdActorSetSpeed(ACTOR_SHEBA, 1.7);
	fdActorSetSpeed(ACTOR_PIERS, 1.8);
	fdActorSetSpeed(ACTOR_MOAPA, 2);
	fdActorSetSpeed(ACTOR_LEFT_KNIGHT, 1.9);
	fdActorSetSpeed(ACTOR_RIGHT_KNIGHT, 1.7);
	fdActorWalkBy(ACTOR_FELIX, scTrialRoadMirrorOffset(3), 0);
	fdActorWalkBy(ACTOR_MOAPA, scTrialRoadMirrorOffset(-3), 0);
	sySynch(1);
	fdActorWalkBy(ACTOR_JENNA, scTrialRoadMirrorOffset(3), 0);
	fdActorWalkBy(ACTOR_LEFT_KNIGHT, scTrialRoadMirrorOffset(-3), 0);
	sySynch(1);
	fdActorWalkBy(ACTOR_PIERS, scTrialRoadMirrorOffset(3), 0);
	fdActorWalkBy(ACTOR_RIGHT_KNIGHT, scTrialRoadMirrorOffset(-3), 0);
	sySynch(1);
	fdActorWalkBy(ACTOR_SHEBA, scTrialRoadMirrorOffset(3), 0);
	sySynch(1);
	fdScriptWait(6);
}

/* Moapa and his knights are already waiting when the party reaches the summit:
   "You may have been slow, but you made it through Trial Road". */
void scTrialRoadReachPeakSecond(void)
{
	scTrialRoadGatherAtPeak();
	fdScriptSetMessageId((Sint32)MsgTrialRoadSlowArrival);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
	/* "You may have been slow, but you made it through Trial Road to the..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "Well, then... The time is nigh for us to decide who is stronger!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorRunRepeatedMotion(ACTOR_MOAPA, 2);
	/* "Let the battle begin!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	/* The same charge as scTrialRoadReachPeakFirst's: written once and shared,
	   it does not compile to the ROM's code. */
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdScriptWait(15);
	fdActorSetSpeed(ACTOR_FELIX, 2);
	fdActorSetSpeed(ACTOR_JENNA, 1.9);
	fdActorSetSpeed(ACTOR_SHEBA, 1.7);
	fdActorSetSpeed(ACTOR_PIERS, 1.8);
	fdActorSetSpeed(ACTOR_MOAPA, 2);
	fdActorSetSpeed(ACTOR_LEFT_KNIGHT, 1.9);
	fdActorSetSpeed(ACTOR_RIGHT_KNIGHT, 1.7);
	fdActorWalkBy(ACTOR_FELIX, scTrialRoadMirrorOffset(3), 0);
	fdActorWalkBy(ACTOR_MOAPA, scTrialRoadMirrorOffset(-3), 0);
	sySynch(1);
	fdActorWalkBy(ACTOR_JENNA, scTrialRoadMirrorOffset(3), 0);
	fdActorWalkBy(ACTOR_LEFT_KNIGHT, scTrialRoadMirrorOffset(-3), 0);
	sySynch(1);
	fdActorWalkBy(ACTOR_PIERS, scTrialRoadMirrorOffset(3), 0);
	fdActorWalkBy(ACTOR_RIGHT_KNIGHT, scTrialRoadMirrorOffset(-3), 0);
	sySynch(1);
	fdActorWalkBy(ACTOR_SHEBA, scTrialRoadMirrorOffset(3), 0);
	sySynch(1);
	fdScriptWait(6);
}

/* After the party beats Moapa at the summit, he admits their strength and gives
   them the Hover Jade for the Shaman's Rod, and the party goes back to the
   Shaman Village to rest. */
void scTrialRoadBeatMoapa(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scTrialRoadPlaceActor(ACTOR_FELIX, 248, 272, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_PIERS, 224, 248, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_JENNA, 232, 264, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_SHEBA, 224, 280, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_KRADEN, 248, 256, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_MOAPA, 272, 264, FACING_SOUTH);
	scTrialRoadPlaceActor(ACTOR_LEFT_KNIGHT, 288, 248, FACING_SOUTH);
	scTrialRoadPlaceActor(ACTOR_RIGHT_KNIGHT, 288, 280, FACING_SOUTH);
	fdActorSetAnimation(ACTOR_MOAPA, 6);
	fdActorSetAnimation(ACTOR_LEFT_KNIGHT, 6);
	fdActorSetAnimation(ACTOR_RIGHT_KNIGHT, 6);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(30);
	fdScriptSetMessageId((Sint32)MsgTrialRoadHeroesProven);
	fdActorStartRepeatedMotion(ACTOR_LEFT_KNIGHT, 2);
	sySynch(4);
	fdActorStartRepeatedMotion(ACTOR_RIGHT_KNIGHT, 2);
	sySynch(10);
	fdActorRunRepeatedMotion(ACTOR_MOAPA, 2);
	/* "All right, all right. You've proven your strength, and you are..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimation(ACTOR_MOAPA, 7);
	fdScriptWait(15);
	fdActorSetAnimation(ACTOR_LEFT_KNIGHT, 7);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_RIGHT_KNIGHT, 7);
	fdScriptWait(5);
	fdActorSetAnimation(ACTOR_MOAPA, 1);
	fdScriptWait(15);
	fdActorFaceDirection(ACTOR_MOAPA, scTrialRoadMirrorAng(FACING_WEST), 0);
	fdActorSetAnimation(ACTOR_LEFT_KNIGHT, 1);
	fdScriptWait(10);
	fdActorFaceDirection(
		ACTOR_LEFT_KNIGHT, scTrialRoadMirrorAng(FACING_WEST), 0);
	fdActorSetAnimation(ACTOR_RIGHT_KNIGHT, 1);
	fdScriptWait(5);
	fdActorFaceDirection(
		ACTOR_RIGHT_KNIGHT, scTrialRoadMirrorAng(FACING_WEST), 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "As we promised, you will receive the Hover Jade, and we will take..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetSpeed(ACTOR_MOAPA, 1);
	fdActorWalkTo(ACTOR_MOAPA, scTrialRoadMirrorMetatileX(32), 264);
	fdActorFaceEachOther(ACTOR_KRADEN, ACTOR_PIERS, 0);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
	fdActorWaitForMoveById(ACTOR_MOAPA);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_FELIX, 0);
	fdScriptWait(20);
	gmPartyInventoryDiscard(ITEM_SHAMANS_ROD);
	gmPartyGiveItem(ITEM_HOVER_JADE, 0);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_MOAPA, 0);
	fdActorGetById(ACTOR_MOAPA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_MOAPA, scTrialRoadMirrorMetatileX(35), 264);
	fdScriptWait(1);
	fdActorGetById(ACTOR_MOAPA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 10);
	/* "After many years, the vow that Hoabna made to Yegelos has at last..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, scTrialRoadMirrorAng(FACING_EAST));
	fdActorFaceActor(ACTOR_LEFT_KNIGHT, ACTOR_MOAPA, 0);
	fdActorFaceActor(ACTOR_RIGHT_KNIGHT, ACTOR_MOAPA, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "I'm tired now. I'll be back in the Shaman Village resting." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimation(ACTOR_LEFT_KNIGHT, 3);
	sySynch(3);
	fdActorSetAnimationAndWait(ACTOR_RIGHT_KNIGHT, 3);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_KRADEN, 0);
	/* "I'm sure you must be exhausted as well. Let's go back to the
	   village..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	sySynch(2);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	sySynch(1);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	sySynch(30);
}

/* After the party loses to Moapa at the summit: "You've lost. Now leave Shaman
   village." */
void scTrialRoadLoseToMoapa(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scTrialRoadPlaceActor(ACTOR_MOAPA, 264, 264, FACING_WEST);
	scTrialRoadPlaceActor(ACTOR_LEFT_KNIGHT, 280, 248, FACING_WEST);
	scTrialRoadPlaceActor(ACTOR_RIGHT_KNIGHT, 280, 280, FACING_WEST);
	scTrialRoadPlaceActor(ACTOR_FELIX, 240, 276, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_PIERS, 216, 254, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_JENNA, 232, 244, FACING_EAST);
	scTrialRoadPlaceActor(ACTOR_SHEBA, 224, 296, FACING_EAST);
	/* Felix alone is left on his feet, the others lying collapsed. */
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_STILL);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_COLLAPSE);
	scFieldActorSetSprRotationById(ACTOR_PIERS, DEGtoANG(90));
	fdActorSetSprFlags(fdActorGetById(ACTOR_FELIX), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_JENNA), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_SHEBA), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_PIERS), 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(30);
	fdScriptSetMessageId((Sint32)MsgTrialRoadLeaveVillage);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "You've lost. Now leave Shaman village." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	sySynch(1);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 2);
	sySynch(2);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorStartRepeatedMotion(ACTOR_PIERS, 2);
	fdScriptWait(15);
}

extern Uint32 TrialRoad_LeftKnightLeavesScript[];

/* While Moapa explains the trial, the camera tours a stage: its traps, the four
   doors, a knight showing how the switch opens the blue chests and how filling
   them opens the door, and the give-up button. */
void scTrialRoadShowTraps(void)
{
	Sint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 1016);
	fdActorSetPos(ACTOR_MOAPA, 1016, 1016);
	for(i = ACTOR_MOAPA + 1; i < FIELD_ACTOR_COUNT; i++) {
		ACTOR *actor = fdActorGetById(i);

		if(actor->x.fixed != 0)
			actor->x.fixed -= toFIXED(144);
	}
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetMovementTypeById(
		ACTOR_LEFT_KNIGHT, TrialRoad_LeftKnightEntersScript);
	fdActorSetMovementTypeById(
		ACTOR_RIGHT_KNIGHT, TrialRoad_RightKnightEntersScript);
	fdScriptStartScreenTransition();
	fdScriptWait(20);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(184, CAMERA_KEEP, 600, 1);
	fdCameraWaitForArrival();
	fdCameraMoveTo(120, CAMERA_KEEP, 552, 1);
	fdCameraWaitForArrival();
	fdCameraMoveTo(120, CAMERA_KEEP, 408, 1);
	fdCameraWaitForArrival();
	fdCameraMoveTo(120, CAMERA_KEEP, 120, 1);
	fdScriptSetMessageId((Sint32)MsgTrialRoadTrapInstructions);
	/* "The room is filled with traps and snares. Use the power of
	   Contigo..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdCameraWaitForArrival();
	fdCameraMoveTo(216, CAMERA_KEEP, 120, 1);
	fdCameraWaitForArrival();
	/* "This road has four doors. To open them, you will need to place
	   items..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	/* "Why?" */
	fdScriptShowMessageAndWait(ACTOR_FELIX, 0);
	fdActorSetMovementTypeById(
		ACTOR_LEFT_KNIGHT, TrialRoad_LeftKnightToChestScript);
	fdActorWaitForScriptEndById(ACTOR_LEFT_KNIGHT);
	fdActorSetAnimationById(ACTOR_LEFT_KNIGHT, 1);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_LEFT_KNIGHT, 0.25);
	fdActorGetById(ACTOR_LEFT_KNIGHT)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(ACTOR_LEFT_KNIGHT, 0, 8);
	fdScriptWait(1);
	fdActorGetById(ACTOR_LEFT_KNIGHT)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_LEFT_KNIGHT, 2);
	fdActorSetAnimationById(ACTOR_LEFT_KNIGHT, 5);
	fdActorSetDestinationOffset(ACTOR_LEFT_KNIGHT, 0, -40);
	fdActorWaitForMoveById(ACTOR_LEFT_KNIGHT);
	sySndPlay(SE_CURSE);
	fdMapGridSetShake(2, 4, 1);
	fdActorGetById(ACTOR_LEFT_KNIGHT)->velY = toFIXED(4);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdActorGetById(ACTOR_LEFT_KNIGHT)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(ACTOR_LEFT_KNIGHT, 0, 16);
	fdScriptWait(1);
	fdActorGetById(ACTOR_LEFT_KNIGHT)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(60);
	fdActorFaceDirectionById(ACTOR_LEFT_KNIGHT, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LEFT_KNIGHT, 4);
	/* "The door is triggered only when the treasure chests are filled to..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_LEFT_KNIGHT, FACING_NORTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_LEFT_KNIGHT, 3);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_LEFT_KNIGHT, 1);
	fdActorWalkByAndWait(ACTOR_LEFT_KNIGHT, -16, -16);
	fdActorFaceDirection(ACTOR_LEFT_KNIGHT, FACING_WEST, 0);
	sySndPlay(SE_CHEST_OPEN);
	fdActorSetAnimationById(ACTOR_LEFT_INNER_CHEST, 6);
	fdScriptWait(30);
	/* "Try putting heavier items, like weapons and tools, into the
	   chests..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdActorWalkToAndWaitById(ACTOR_LEFT_KNIGHT, 200, 168);
	fdActorFaceDirection(ACTOR_LEFT_KNIGHT, FACING_NORTH, 0);
	fdScriptWait(40);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetAnimationById(ACTOR_LEFT_INNER_CHEST, 5);
	fdScriptWait(20);
	sySndPlay(SE_ITEM_BREAK);
	fdMapGridCopyMetatilesLayered(127, 3, 1, 2, 48, 3);
	fdMapCopyMetatileAttributeRect(0, 127, 1, 1, 14, 8);
	fdScriptWait(20);
	fdActorSetMovementTypeById(
		ACTOR_LEFT_KNIGHT, TrialRoad_LeftKnightLeavesScript);
	fdActorWaitForScriptEndById(ACTOR_LEFT_KNIGHT);
	sySndPlay(SE_MAP_EXIT);
	/* "If you reach the door after your opponents do, you will be at a..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdActorSetMovementTypeById(
		ACTOR_RIGHT_KNIGHT, TrialRoad_RightKnightToChestsScript);
	fdCameraMoveTo(328, CAMERA_KEEP, 120, 1);
	fdCameraWaitForArrival();
	/* "You see, it will take twice the weight to open the doors." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	/* "Stand on the switch, and the number of chests you need to fill
	   will..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_RIGHT_KNIGHT, FACING_NORTHEAST, 0);
	fdActorSetAnimationAndWait(ACTOR_RIGHT_KNIGHT, 3);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_RIGHT_KNIGHT, 1);
	fdActorWalkByAndWait(ACTOR_RIGHT_KNIGHT, 16, -16);
	fdActorFaceDirectionById(ACTOR_RIGHT_KNIGHT, FACING_EAST, 0);
	sySndPlay(SE_CHEST_OPEN);
	fdActorSetAnimationById(ACTOR_RIGHT_INNER_CHEST, 6);
	fdActorSetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 6);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_RIGHT_KNIGHT, 350, 168);
	fdActorFaceDirection(ACTOR_RIGHT_KNIGHT, FACING_NORTH, 0);
	fdScriptWait(40);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetAnimationById(ACTOR_RIGHT_INNER_CHEST, 5);
	fdScriptWait(40);
	fdActorWalkToAndWait(ACTOR_RIGHT_KNIGHT, 364, 168);
	fdActorFaceDirectionById(ACTOR_RIGHT_KNIGHT, FACING_NORTH, 0);
	fdScriptWait(40);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 5);
	fdScriptWait(20);
	sySndPlay(SE_ITEM_BREAK);
	fdMapGridCopyMetatilesLayered(127, 3, 1, 2, 53, 3);
	fdMapCopyMetatileAttributeRect(0, 127, 1, 1, 19, 8);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_RIGHT_KNIGHT, FACING_NORTHWEST, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_RIGHT_KNIGHT, 3);
	/* "If you are last to each of the four doors, you will lose eight
	   items..." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdCameraMoveTo(408, CAMERA_KEEP, 120, 1);
	fdCameraWaitForArrival();
	/* "If Trial Road proves too much for you, press this button." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	fdActorSetMovementTypeById(
		ACTOR_RIGHT_KNIGHT, TrialRoad_RightKnightLeavesScript);
	fdActorWaitForScriptEndById(ACTOR_RIGHT_KNIGHT);
	/* "This ends the battle, though. A true hero would not surrender so
	   easily." */
	fdScriptShowMessageAndWait(ACTOR_MOAPA, 0);
	syImportScriptSetExitRequest(
		TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_D);
}

/* Moves the fountains each frame: at its rise phase a fountain starts up 4
   pixels a frame, at its fall phase down 8, until it reaches its high or low
   height; its phase runs round its period while the field engine handles no
   request. Its metatile's collision height follows it; while it is up the
   metatile is water to wade in, and while it also stands still a gap to jump
   onto; its top bobs on the water. */
void scTrialRoadUpdateFountains(void)
{
	TRIAL_FOUNTAINS_WORK *state = TRIAL_ROAD_FOUNTAINS;
	TRIAL_ROAD_FOUNTAIN	 *fountain = state->fountains;
	MAP_STATE			 *map = Ram_HeapSlots->mapGrid;
	Sint32				  shape = state->shape;
	Uint8				 *shapes = map->layers[0].terrainIds;
	Uint8				 *metatiles = (Uint8 *)map->layers[0].metatiles;
	Sint32				 *collision;
	ACTOR				 *actor;
	ACTOR				 *marker;
	Sint32				  metatile;
	Uint8				 *attributes;
	Sint32				  i;

	i = 0;
	for(collision = (Sint32 *)Ram_MapCollision + shape; i < FOUNTAIN_COUNT;
		collision++, i++, fountain++) {
		if(fountain->id == 0)
			continue;
		actor = fdActorGetById(fountain->id);
		if(actor->x.fixed == 0 && actor->z.fixed == 0)
			continue;
		if(fountain->phase == fountain->riseFrm) {
			sySndPlay(SE_CURSE_FALL);
			fountain->speed = 4;
		}
		if(fountain->phase == fountain->fallFrm) {
			sySndPlay(SOUND_FADE_OUT_EXIT_SE);
			fountain->speed = -8;
		}
		if(fountain->speed != 0) {
			fountain->height += fountain->speed;
			if(fountain->height >= fountain->high) {
				fountain->height = fountain->high;
				fountain->speed = 0;
				fountain->bob = 0;
			}
			if(fountain->height <= fountain->low) {
				fountain->height = fountain->low;
				fountain->speed = 0;
				fountain->bob = 0;
			}
		}
		if(!syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS)) {
			if(++fountain->phase >= fountain->periodFrm)
				fountain->phase = 0;
		}
		metatile = (FIELD_METATILE(actor->x.fixed)) +
			((FIELD_METATILE(actor->z.fixed)) << 7);
		shapes[metatile] = (Uint8)state->shape + i;
		*collision = ((actor->y.part[0].integer + fountain->height) / 8) << 8;
		attributes = metatiles + metatile * 4;
		if(fountain->speed != 0)
			attributes[3] = 0;
		else {
			Sint32 bits = MAP_METATILE_JUMP;
			bits |= attributes[3];
			attributes[3] = bits;
		}
		if(fountain->height != 0) {
			Sint32 bits = MAP_METATILE_WATER;
			bits |= attributes[3];
			attributes[3] = bits;
		} else
			attributes[3] = 0;
		marker = fountain->top;
		if(marker != 0) {
			if(fountain->height == 0) {
				marker->y.fixed = actor->y.fixed - toFIXED(16);
			} else {
				FIXED wave;
				fountain->bob += TRIG_TURN / 16;
				wave = syMulFX(toFIXED(2), sySin(fountain->bob)) - toFIXED(14);
				marker->y.fixed =
					actor->y.fixed + toFIXED(fountain->height) + wave;
			}
			marker->x.fixed = actor->x.fixed;
			marker->z.fixed = actor->z.fixed;
		}
	}
}

#undef SET_LAYER

/* Queues the fountains' sprites each frame: each risen fountain's column of
   water, in 16-pixel segments up from its vent, and its spout bobbing on
   top. */
void scTrialRoadDrawFountains(void)
{
	TRIAL_FOUNTAINS_WORK *base = TRIAL_ROAD_FOUNTAINS;
	TRIAL_ROAD_FOUNTAIN	 *fountain = base->fountains;
	Sint32				  i;
	Sint32				  tile;
	Sint32				  frame;
	Sint32				  count;
	Sint32				  viewX;
	Sint32				  viewZ;
	Sint32				  viewY;
	FIXED				  rise;
	MAP_STATE			 *map;
	ACTOR				 *actor;
	Uint32				 *entry;
	Uint32				 *cursor;
	Sint32				  segmentHeight;
	Sint32				  x;
	Sint32				  height;
	FIXED				  z;
	Sint32				  screenY;
	Sint32				  depth;
	FIXED				  bob;

	tile = VramCacheEntries[base->vramEntry].ofs >> 5;
	entry = base->sprs;
	count = 0;
	rise = -toFIXED(2);
	map = Ram_HeapSlots->mapGrid;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	viewY = map->origin[1];

	for(i = 0; i < FOUNTAIN_COUNT; i++, fountain++) {
		if(fountain->id == 0 || fountain->height == 0)
			continue;
		actor = fdActorGetById(fountain->id);
		frame = ((FrameCount % 12) / 3) * 8 + 32;
		segmentHeight = 0;
		if(segmentHeight < fountain->height) {
			do {
				if(count >= FOUNTAIN_SPRITE_COUNT)
					continue;
				x = actor->x.fixed - viewX;
				height = actor->y.fixed + toFIXED(segmentHeight) - viewY;
				z = actor->z.fixed - viewZ - viewY;
				x = FIXEDtoINT(x) - 8;
				screenY = z - height;
				screenY = FIXEDtoINT(screenY) - 16;
				depth = FIXEDtoINT(height + z) + FOUNTAIN_DEPTH_OFFSET;
				if((Uint32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
					continue;
				x &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				cursor = entry;
				*cursor++ = 0;
				*cursor++ =
					screenY | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
				*cursor = (tile + frame) | OBJ_PRIORITY(2);
				syOAMAddSprAtDepth(entry, depth);
				entry += 3;
				count++;
			} while((segmentHeight += 16) < fountain->height);
		}
		if(count >= FOUNTAIN_SPRITE_COUNT)
			continue;
		x = actor->x.fixed - viewX;
		height = actor->y.fixed + toFIXED(fountain->height) - viewY;
		z = actor->z.fixed - viewZ - viewY;
		x = FIXEDtoINT(x) - 8;
		screenY = z - height;
		bob = syMulFX(rise, sySin(fountain->bob));
		screenY = FIXEDtoINT(screenY + bob + rise) - 4;
		depth = FIXEDtoINT(height + z) + FOUNTAIN_DEPTH_OFFSET;
		if((Uint32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
			continue;
		x &= OBJ_X_MASK;
		screenY &= OBJ_Y_MASK;
		cursor = entry;
		*cursor++ = 0;
		*cursor++ = screenY | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		*cursor = (tile + frame - 32) | OBJ_PRIORITY(2);
		syOAMAddSprAtDepth(entry, depth);
		entry += 3;
		count++;
	}
}

/* Allocates the fountains' working buffer. */
static __inline__ void *scPoolAlloc(Sint32 size)
{
	return syHeapAllocEWRAM(size);
}

/* Starts the fountains' state and their event. */
void scTrialRoadStartFountains(Sint32 unused, Sint32 shape)
{
	TRIAL_FOUNTAINS_WORK *state = TRIAL_ROAD_FOUNTAINS;
	void				 *buffer;

	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
		syWordsClear(state, sizeof(*state));
	buffer = scPoolAlloc(2048);
	syDecompressTaggedLZ(Field_GeyserTiles, buffer);
	state->vramEntry = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(state->vramEntry, 2048, buffer);
	syHeapRelease(buffer);
	state->sprs =
		syHeapAllocEWRAM(FOUNTAIN_SPRITE_COUNT * sizeof(TRIAL_ROAD_SPR));
	state->shape = shape;
	sySetEvent(scTrialRoadUpdateFountains, EVENT_PASS_FRAME + 13);
	sySetEvent(scTrialRoadDrawFountains, EVENT_PASS_FRAME);
	fdActorGetById(fdPartyGetLeaderActor())->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}

/* Sets up one fountain on its vent, with its timer and height. */
TRIAL_ROAD_FOUNTAIN *scTrialRoadAddFountain(
	Sint32 slot, Sint32 id, Sint32 timer, Sint32 low, Sint32 high)
{
	TRIAL_FOUNTAINS_WORK *state = TRIAL_ROAD_FOUNTAINS;
	TRIAL_ROAD_FOUNTAIN	 *fountain = &state->fountains[slot];
	ACTOR				 *actor = fdActorGetById(id);
	ACTOR				 *top;

	fdActorGetById(id);
	fdActorSetSprFlags(actor, 0);
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if(high < low) {
			Sint32 swap = low;

			low = high;
			high = swap;
		}
		fountain->id = id;
		fountain->height = 0;
		fountain->phase = timer;
		fountain->periodFrm = 360;
		fountain->high = high;
		fountain->riseFrm = 60;
		fountain->fallFrm = 240;
		fountain->low = low;
		fountain->unusedAfterLow = 1;
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fountain->speed = 0;
		fountain->bob = 0;
		fountain->unusedAfterSpeed = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	}
	top =
		((ACTOR *)syImportActorCreate(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
			actor->x.fixed, actor->y.fixed, actor->z.fixed));
	top->collisionFlg = 0;
	top->motionFlg = 0;
	top->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fountain->top = top;
	return fountain;
}

#include "shared/pillar_restore_raised.h"

#include "shared/leader_check_crumbling_step.h"

/* A cracked pillar in the crumbling stage shakes, crumbles and falls away, as
   Magma Rock's do. */
void scTrialRoadCrumblePillar(const void *data, Sint32 id)
{
	const TRAP_ENTRY *entry = data;
	ACTOR			 *actor = fdActorGetById(id);
	Sint32			  flag;

	while(entry->actor != ACTOR_LIST_END) {
		if(entry->actor != id) {
			entry++;
			continue;
		}
		{
			flag = entry->flg;
			fdScriptStart();
			if(!fdScriptGetFlag(flag + 1)) {
				fdScriptPlaySnd(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(actor, 2);
				fdActorSetMetatileShape(-4, actor->layer, id);
				scDustScatter(1);
				sySynch(16);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				fdActorSetPosAndResetMotion(actor, 0, 0, 0);
				sySynch(2);
				fdScriptPlaySnd(SE_THUMP);
				scMagmaRockInsideSpawnDustRing();
				sySynch(20);
				fdScriptSetFlag(flag);
				fdScriptSetFlag(flag + 1);
			}
			fdScriptEnd();
			break;
		}
	}
}

#include "shared/pillar_take_out_of_play.c"

#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

/* Unused: Magma Rock's trap floor, kept in this overlay: a cracked pillar
   stepped on flashes the first time and gives way under Felix the second, with
   Moapa's actor standing in for the trap's picture. The crumbling stage uses
   scLeaderCheckCrumblingStep instead. */
UNUSED void scTrialRoadStepOnTrap(const Uint16 *table)
{
	ACTOR *leader;
	ACTOR *partner;
	ACTOR *trap;
	FIXED  position[3];
	Sint32 id;
	Sint32 flag;
	Sint32 second;
	Uint8 *motion;

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	partner = fdActorGetById(ACTOR_MOAPA);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	trap = fdActorFindAtMetatile(position, 0);
	if(trap != 0) {
		while(*table != ACTOR_LIST_END) {
			id = table[0];
			if(fdActorGetById(id) != trap) {
				table += 2;
				continue;
			}
			flag = table[1];
			fdScriptStart();
			if(!syGameFlagGet(flag)) {
				fdActorSelectAnimation(partner, 1);
				scActorTakePlaceOf(partner, trap);
				sySynch(1);
				sySndPlay(SE_CHEST_CLOSE);
				fdActorSetSprCol(fdActorGetById(ACTOR_MOAPA), 7);
				sySynch(2);
				fdActorSelectAnimation(partner, 0);
				fdActorCopyGraphics(id, 9);
				fdActorSetSprCol(fdActorGetById(ACTOR_MOAPA), 0);
				sySynch(2);
				fdActorSetSprCol(fdActorGetById(ACTOR_MOAPA), 7);
				sySynch(4);
				fdActorSetSprCol(fdActorGetById(ACTOR_MOAPA), 0);
				scDustScatter(0);
				scActorTakePlaceOf(trap, partner);
				sySynch(1);
				syGameFlagSet(flag);
			} else {
				second = flag + 1;
				if(!syGameFlagGet(second)) {
					fdActorSelectAnimation(partner, 0);
					scActorTakePlaceOf(partner, trap);
					motion = &leader->motionFlg;
					sySynch(1);
					*motion = 0;
					sySndPlay(SE_PUSH);
					fdMapGridSetShake(1, 4, 1);
					scDustScatter(0);
					sySynch(8);
					fdActorSelectAnimation(partner, 2);
					fdLeaderSetMetatileShapeAt(-2, trap->layer);
					scDustScatter(1);
					sySynch(16);
					fdLeaderSetMetatileShapeAt(-2, trap->layer);
					sySynch(4);
					fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
					sySynch(8);
					*motion = 3;
					sySynch(5);
					fdActorSetPosAndResetMotion(partner, 0, 0, 0);
					sySynch(2);
					sySndPlay(SE_THUMP);
					scMagmaRockInsideSpawnDustRing();
					sySynch(20);
					syGameFlagSet(second);
				}
			}
			fdScriptEnd();
			return;
		}
	}
}

/* The dust's motion each frame, as fdEffectAdvanceMotionAtLeaderPriority's: it
   drifts, slows, grows and spins, drawn at the leader's sprite priority. */
void scTrialRoadDriftDust(FIELD_OBJECT *object)
{
	FIELD_EFFECT	   *effect = &object->eff;
	Sint32				velocityX = effect->vel.x;
	Sint32				velocityZ;
	MAIN_ROUTINE_STATE *party;
	FIELD_SPR		   *sprite;
	FIELD_OBJECT	   *leader;
	EFFECT_SPR_ATTR	   *to;
	EFFECT_SPR_ATTR	   *from;
	EFFECT_SPR_BITS		source;
	EFFECT_SPR_BITS		bits;

	effect->x += velocityX;
	effect->y += effect->vel.y;
	velocityZ = effect->vel.z;
	effect->z += velocityZ;
	effect->vel.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
	effect->scaleX += effect->scaleRateX;
	sprite = effect->spr;
	effect->scaleY += effect->scaleRateY;
	sprite->rotation += effect->spin;

	party = &MainRoutineStateWork;

	leader = fdActorGetById(party->leaderActorId);
	to = (EFFECT_SPR_ATTR *)effect->spr;
	from = (EFFECT_SPR_ATTR *)leader->eff.spr;
	source = from->bits;
	bits = to->bits;
	bits.pri = source.pri;
	to->bits = bits;
}

extern Uint32 Dust_ScatterScript[];

/* Eight clouds of dust puff up in a row across the leader's feet; with spread
   set they fly out all round. */
void scDustScatter(Sint32 spread)
{
	ACTOR		 *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	Uint32		  frame = 0;
	Uint32		  i;

	for(i = 0; i <= 7; i++) {
		{
			/* Each piece starts 0 to 13 pixels lower than the leader. */
			FIXED y = leader->y.fixed - ((syRandomBelow(14)) << 16);

			effect = (FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD,
				i * toFIXED(2) + leader->x.fixed - toFIXED(8), y - toFIXED(2),
				leader->z.fixed);
		}
		if(effect != 0) {
			frame = fdSprShareVRAMCacheEntry(effect->spr, frame);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags(effect, 0);
			fdActorSelectAnimation(effect, 2);
			fdActorStartScript((ACTOR *)effect, Dust_ScatterScript);
			effect->scaleY = effect->scaleX = toFIXED(7) / 10;
			effect->spr->pri = 1;
			if(spread != 0) {
				effect->vel.x =
					syMulFX(syCos(i * (TRIG_TURN / 8)), toFIXED(12) / 10);
				effect->vel.z = sySin(i * (TRIG_TURN / 8));
			} else {
				effect->vel.x = spread;
				effect->vel.z = spread;
			}
			effect->vel.y = -(syRandomBelow(16384) + (toFIXED(3) / 8));
			effect->spin = syRandomBelow(512) - (TRIG_TURN / 256);
			effect->scaleRateX = -(toFIXED(1) / 32);
			effect->scaleRateY = -(toFIXED(3) / 128);
			effect->update = scTrialRoadDriftDust;
		}
	}
}

extern Uint32 Dust_BurstScript[];
/* The clouds of dust scMagmaRockInsideSpawnDustRing throws out in a ring. */
#define DUST_RING_PIECES 17

/* Clouds of dust burst out in a ring round the leader as something lands with a
   thump. */
void scMagmaRockInsideSpawnDustRing(void)
{
	ACTOR		 *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	Uint32		  frame;
	Uint32		  i;

	if(leader == 0)
		return;
	frame = 0;
	for(i = 0; i < DUST_RING_PIECES; i++) {
		effect = (FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD,
			leader->x.fixed, leader->y.fixed, leader->z.fixed);
		if(effect != 0) {
			frame = fdSprShareVRAMCacheEntry(effect->spr, frame);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags(effect, 0);
			fdActorSelectAnimation(effect, 2);
			fdActorStartScript((ACTOR *)effect, Dust_BurstScript);
			effect->vel.x = syCos(i * (TRIG_TURN / 16));
			effect->vel.y = 0;
			effect->vel.z = sySin(i * (TRIG_TURN / 16));
			effect->vel.x += effect->vel.x >> 1;
			effect->vel.x += syRandomBelow(toFIXED(3) / 4) - (toFIXED(3) / 8);
			effect->vel.z += syRandomBelow(toFIXED(3) / 8) - (toFIXED(3) / 16);
			effect->spin = syRandomBelow(4096) - (TRIG_TURN / 32);
			effect->scaleRateX = 0;
			effect->scaleRateY = 0;
			effect->update = scTrialRoadDriftDust;
			effect->spr->pri = ((FIELD_SPR *)leader->spr)->pri;
		}
	}
}

#include "shared/actor_set_metatile_shape.h"

#include "shared/leader_set_metatile_shape.h"

extern Uint32 TrialRoad_HexDigits[];

/* Unused: writes value as eight hexadecimal digits (TrialRoad_HexDigits) and a
   terminating null. */
UNUSED void scStringWriteU32AsHex(Uint8 *text, Uint32 value)
{
	Sint32		 i;
	const Uint8 *digits = TrialRoad_HexDigits;

	text += 8;
	*text = 0;
	text--;
	for(i = 0; i < 8; i++) {
		*text = digits[value & 0xf];
		value >>= 4;
		text--;
	}
}

/* Does nothing. */
void scTrialRoadIdle(void)
{
}

/* Stops Moapa's party where it stands, as at the end of its recording. */
void scTrialRoadStopRival(void)
{
	((TRIAL_ROAD_REPLAY *)SceneStateWork)->mode = RIVAL_WAITING;
}

/* Waits until Moapa's party has finished its recording. */
void scTrialRoadWaitForRival(void)
{
	TRIAL_ROAD_REPLAY *state = (TRIAL_ROAD_REPLAY *)SceneStateWork;

	while(state->mode != RIVAL_WAITING) {
		sySynch(1);
	}
}

/* The most the rival turns in a frame: a sixteenth of a turn. */
#define RIVAL_MAX_TURN (TRIG_TURN / 16)

/* Updates the Trial Road rival by its mode: RIVAL_MIRROR follows the leader's
   position mirrored across the course; RIVAL_LINKED places the rival and its
   partner from the linked player's record; RIVAL_REPLAY replays the recorded
   path; RIVAL_RECORD records the actor's position. */
void scTrialRoadMoveRival(void)
{
	TRIAL_ROAD_LINK_STATE *trialRoadLinkState =
		(TRIAL_ROAD_LINK_STATE *)Ram_HeapSlots->trialRoadLink;
	TRIAL_ROAD_REPLAY *state = (TRIAL_ROAD_REPLAY *)SceneStateWork;
	ACTOR			  *rival = fdActorGetById(state->id);
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 x, z, turn;
	FIXED  dx, dz;
	Uint16 heading;

	if(rival == 0)
		return;
	if(state->mode == RIVAL_MIRROR) {
		x = leader->x.fixed;
		z = leader->z.fixed;
		if((Uint32)(z - toFIXED(256)) > toFIXED(304))
			x = toFIXED(528) - (x & FIELD_PIXEL_MASK);
		if(MainRoutineStateWork.entrance & 1)
			x += toFIXED(304);
		else
			x -= toFIXED(304);
		if(rival->x.fixed != x || rival->z.fixed != z) {
			dx = x - rival->x.fixed;
			dz = z - rival->z.fixed;
			heading = syAtan(dz, dx);
			turn = (Sint16)(heading - rival->facing);
			if(turn > RIVAL_MAX_TURN)
				turn = RIVAL_MAX_TURN;
			if(turn < -RIVAL_MAX_TURN)
				turn = -RIVAL_MAX_TURN;
			rival->facing += turn;
			rival->x.fixed = x;
			rival->z.fixed = z;
		}
		if(state->still > 2)
			fdActorSelectAnimation(rival, 1);
		else
			fdActorSelectAnimation(rival, 5);
	} else if(state->mode == RIVAL_LINKED) {
		Sint32	player;
		Sint16 *signature;
		Sint32	angle, link, animation, partner;
		Sint16	packed;

		player = ((Sint32)syGameFlagGetByte(FLAG_LINK_PLAYER)) ^ 1;
		signature = (Sint16 *)LinkPeerSignatures[player];
		if(((LinkStatusWork >> player) & 1) == 0)
			return;
		x = *signature++;
		z = *signature++;
		angle = *signature++;
		packed = *signature++;
		link = packed >> 8;
		animation = packed & 0xff;
		if(syGameFlagGet(dz = FLAG_LINKED_RIVAL_HALTS) && link != 0) {
			state->mode = RIVAL_WAITING;
			fdActorSelectAnimation(rival, 1);
			if(trialRoadLinkState->originX < rival->x.fixed)
				x = trialRoadLinkState->originX + toFIXED(12);
			else
				x = trialRoadLinkState->originX - toFIXED(12);
			if(!syGameFlagGet(FLAG_LINKED_RIVAL_ON_FAR_SIDE)) {
				z = trialRoadLinkState->originZ + toFIXED(16);
				rival->tmr = trialRoadLinkState->rivalTmrs[1];
			} else {
				z = trialRoadLinkState->originZ - toFIXED(16);
				rival->tmr = trialRoadLinkState->rivalTmrs[0];
			}
			rival->acceleration = toFIXED(1) / 4;
			rival->speedLimit = toFIXED(1);
			fdActorSetMoveTarget(rival, x, 0, z);
			fdActorStartIdle(rival);
		} else {
			x <<= 16;
			z <<= 16;
			rival->facing = angle;
			rival->x.fixed = x;
			rival->z.fixed = z;
			fdActorSelectAnimation(rival, animation);
		}
		partner = *signature++;
		x = *signature++;
		z = *signature++;
		angle = *signature++;
		animation = *signature++;
		if(partner == 0)
			return;
		rival = fdActorGetById(partner);
		if(rival == 0)
			return;
		rival->x.fixed = toFIXED(x);
		rival->facing = angle;
		rival->z.fixed = toFIXED(z);
		fdActorSelectAnimation(rival, animation);
	} else if(state->mode == RIVAL_REPLAY) {
		if(state->resource != trialRoadLinkState->resource) {
			syDecompressTaggedLZ(syResourceGetTableEntry(state->resource),
				trialRoadLinkState->path);
			trialRoadLinkState->resource = state->resource;
		}
		x = trialRoadLinkState->path[state->point];
		z = trialRoadLinkState->path[state->point + 1];
		if(!syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS))
			state->point += 2;
		if(x == 0 && z == 0) {
			state->mode = RIVAL_WAITING;
			return;
		}
		x <<= 16;
		z <<= 16;
		if(state->mirrored != 0 && (Uint32)(z - toFIXED(256)) > toFIXED(304))
			x = toFIXED(528) - x;
		if(MainRoutineStateWork.entrance & 1)
			x += toFIXED(304);
		else
			x -= toFIXED(304);
		if(rival->x.fixed != x || rival->z.fixed != z) {
			dx = x - rival->x.fixed;
			dz = z - rival->z.fixed;
			heading = syAtan(dz, dx);
			turn = (Sint16)(heading - rival->facing);
			if(turn > RIVAL_MAX_TURN)
				turn = RIVAL_MAX_TURN;
			if(turn < -RIVAL_MAX_TURN)
				turn = -RIVAL_MAX_TURN;
			rival->facing += turn;
			rival->x.fixed = x;
			rival->z.fixed = z;
			state->still = 0;
		} else {
			state->still++;
		}
		if(state->still > 2)
			fdActorSelectAnimation(rival, 1);
		else
			fdActorSelectAnimation(rival, 5);
	} else if(state->mode == RIVAL_RECORD) {
		x = FIXEDtoINT(rival->x.fixed);
		z = FIXEDtoINT(rival->z.fixed);
		trialRoadLinkState->path[state->point] = x;
		trialRoadLinkState->path[state->point + 1] = z;
		if(!syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS))
			state->point += 2;
		if(state->point == RIVAL_PATH_LENGTH - 2) {
			Uint16 next = state->point + 1;

			trialRoadLinkState->path[state->point] = 0;
			trialRoadLinkState->path[(Sint16)next] = 0;
			state->id = trialRoadLinkState->rivalId;
			state->point = 0;
			state->mode = RIVAL_REPLAY;
		}
	}
}

/* The race map's picture, tagged LZ. */
extern const Uint8 TrialRoad_RaceMapPicture[];

/* The picture's buffer in EWRAM, the size syVRAMCacheLoad copies. */
static inline void *scTrialRoadLinkAllocPicture(void)
{
	return syHeapAllocEWRAM(RACE_MAP_PICTURE_BYTES);
}

/* Where an actor's marker stands on the race map: its place in metatiles, moved
   by `left` or `right` on the entrance's side, and raised by the stage it has
   reached. */
#define MARKER(left, right, which, tileAdd) \
	do { \
		Sint32 lift; \
		Sint32 side = MainRoutineStateWork.entrance & 1; \
		if(side) { \
			x = (actor->x.fixed - state->originX) / toFIXED(16) left; \
			lift = scTrialRoadGetStage(which); \
			y = (actor->z.fixed - state->originZ) / toFIXED(16) + \
				state->scrollY - lift * RACE_MAP_STAGE_SPACING + 4; \
		} else { \
			x = (actor->x.fixed - state->originX) / toFIXED(16) right; \
			lift = scTrialRoadGetStage(which); \
			y = (actor->z.fixed - state->originZ) / toFIXED(16) + \
				state->scrollY - lift * RACE_MAP_STAGE_SPACING + 4; \
		} \
		x &= 511; \
		y &= 255; \
		*cursor++ = 0; \
		y |= OBJ_X(x); \
		y |= markerSize; \
		*cursor++ = y; \
		*cursor++ = (tile + (tileAdd)) | \
			(OBJ_PALETTE(RACE_MAP_PALETTE) | OBJ_PRIORITY(1)); \
		syOAMAddSprAtDepth(entry++, OAM_DEPTH_FRONT); \
	} while(0)

/* Draws the race map each frame, once scTrialRoadShowRaceMap starts it: the
   display slides in from both sides of the screen, and out while a menu or a
   Psynergy is open. Each side shows the road's stages one above the other, and
   on eleven frames of sixteen a marker shows where each party is: "1P" for the
   leader's, "2P" for Moapa's. */
void scTrialRoadDrawRaceMap(void)
{
	TRIAL_ROAD_LINK_STATE *state = Ram_HeapSlots->trialRoadLink;
	Uint32				  *cursor = (Uint32 *)state;
	TRIAL_ROAD_SPR		  *entry = (TRIAL_ROAD_SPR *)state;
	Sint16				  *vramEntry = &state->vramEntry;
	Uint32				   row, i;
	Sint32				   tile = VramCacheEntries[*vramEntry].ofs >> 5;
	Uint32				   flip, word;
	Sint32				   x;
	Sint32				   y;
	Uint32				   markerSize = OBJ_SIZE_16X16;
	FIELD_STATE			  *event = Ram_HeapSlots->fieldState;
	ACTOR				  *actor;

	if(state->holdFade != 0) {
		state->fade = 2;
	} else if(syGameFlagGet(FLAG_MENU_OPEN) || event->psynergy != 0) {
		if(state->fade > 0)
			state->fade--;
	} else if(state->fade <= 1) {
		if(++state->fade == 1) {
			void *buffer;
			syDMAXCopy(TrialRoad_RaceMapPalette,
				(void *)&OBJ_PLTT_COLOR(RACE_MAP_PALETTE, 0),
				DMA_COPY_HALFWORDS(16), REG_DMA3);
			buffer = scTrialRoadLinkAllocPicture();
			syDecompressTaggedLZ(TrialRoad_RaceMapPicture, buffer);
			syVRAMCacheLoad(*vramEntry, RACE_MAP_PICTURE_BYTES, buffer);
			syHeapRelease(buffer);
		}
	}
	if(state->fade == 0) {
		syVRAMCacheReleaseVRAM(state->vramEntry);
		return;
	}
	for(row = 0; row < 2; row++) {
		y = (state->scrollY + 8) & OBJ_Y_MASK;
		if(row == 0) {
			x = state->fade * 6 + state->scrollX - 12;
			x &= OBJ_X_MASK;
			flip = 0;
		} else {
			x = 236 - state->fade * 6 - state->scrollX;
			x &= OBJ_X_MASK;
			flip = OBJ_HFLIP;
		}
		for(i = 0; i < 4 && y < 160; i++) {
			*cursor++ = 0;
			*cursor++ = y | OBJ_X(x) | flip | OBJ_SIZE_16X32;
			*cursor++ = (tile + RACE_MAP_TILE_STAGE) |
				(OBJ_PALETTE(RACE_MAP_PALETTE) | OBJ_PRIORITY(1));
			syOAMAddSprAtDepth(entry++, OAM_DEPTH_FRONT);
			word = y + 32;
			*cursor++ = 0;
			*cursor++ = word | OBJ_X(x) | flip | OBJ_SIZE_16X8;
			*cursor++ = (tile + RACE_MAP_TILE_STAGE_FOOT) |
				(OBJ_PALETTE(RACE_MAP_PALETTE) | OBJ_PRIORITY(1));
			syOAMAddSprAtDepth(entry++, OAM_DEPTH_FRONT);
			y += RACE_MAP_STAGE_SPACING;
		}
	}
	if((FrameCount & 15) < 5)
		return;
	actor = fdActorLookup(state->leaderId);
	if(actor != NULL) {
		MARKER(+state->fade * 6 + state->scrollX - 19,
			-state->fade * 6 - state->scrollX + 232, 0, RACE_MAP_TILE_1P);
	}
	actor = fdActorLookup(state->rivalId);
	if(actor == NULL || actor->x.fixed == 0)
		return;
	/* This side's offset adds scrollX where the first marker's subtracts it.
	   Nothing writes scrollX, and in a render of the race (the stage room by
	   entrance 31 with FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT set) both columns stand
	   at the screen's edges with "1P" and "2P" at their feet: it is 0, so the
	   two agree. */
	MARKER(+state->fade * 6 + state->scrollX + 189,
		-state->fade * 6 + state->scrollX + 23, 1, RACE_MAP_TILE_2P);
}

/* Shows the race map for both sides. */
void scTrialRoadShowRaceMap(Sint32 leaderId, Sint32 rivalId)
{
	TRIAL_ROAD_LINK_STATE *state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_TRIAL_ROAD_LINK, sizeof(TRIAL_ROAD_LINK_STATE));
	void *buffer = syHeapAllocEWRAM(RACE_MAP_PICTURE_BYTES);

	state->leaderId = leaderId;
	state->rivalId = rivalId;
	if(leaderId == MainRoutineStateWork.leaderActorId) {
		state->originX = toFIXED(160);
		state->originZ = toFIXED(-1632);
	} else {
		state->originX = toFIXED(112);
		state->originZ = toFIXED(-1632);
	}
	fdActorGetById(leaderId);
	fdActorGetById(rivalId);
	state->fade = 0;
	state->holdFade = 0;
	syDecompressTaggedLZ(TrialRoad_RaceMapPicture, buffer);
	state->vramEntry = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(state->vramEntry, RACE_MAP_PICTURE_BYTES, buffer);
	sySetEvent(scTrialRoadDrawRaceMap, EVENT_PASS_FRAME - 10);
	syHeapRelease(buffer);
}

/* Starts Moapa's party on its recording, mirrored when the leader went
   right. */
void scTrialRoadStartRival(Sint32 mirrored)
{
	TRIAL_ROAD_LINK_STATE *trialRoadLinkState = Ram_HeapSlots->trialRoadLink;
	TRIAL_ROAD_REPLAY	  *state = (TRIAL_ROAD_REPLAY *)SceneStateWork;

	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
		state->mirrored = mirrored;
		state->id = trialRoadLinkState->rivalId;
		state->still = 0;
	}
	sySetEvent(scTrialRoadMoveRival, EVENT_PASS_FRAME + 5);
}

/* Plays Moapa's recording from the given resource. */
void scTrialRoadPlayRivalRecording(Sint32 resource)
{
	TRIAL_ROAD_REPLAY *state = (TRIAL_ROAD_REPLAY *)SceneStateWork;

	state->resource = resource;
	((TRIAL_ROAD_LINK_STATE *)Ram_HeapSlots->trialRoadLink)->resource = 0;
	state->point = 0;
	state->mode = RIVAL_REPLAY;
	scTrialRoadMoveRival();
}

/* Whether Moapa's party is waiting. */
Sint32 scTrialRoadRivalIsWaiting(void)
{
	Sint32 mode = ((TRIAL_ROAD_REPLAY *)SceneStateWork)->mode;

	return mode == RIVAL_WAITING;
}

/* Unused: does nothing. */
UNUSED void scTrialRoadNoOperation(void)
{
}

Sint32 TrialRoad_SettleScript[] = {
	SC_CALL((Sint32)scTrialRoadSettleOnGround),
	SC_END,
};

Sint32 TrialRoad_PillarTopScript[] = {
	SC_CALL((Sint32)scTrialRoadFollowPillar),
	SC_END,
};

Sint8 TrialRoad_PoundPartners[4] ALIGNED(4) = {ACTOR_INNER_WEST_STAKE,
	ACTOR_WEST_STAKE, ACTOR_EAST_STAKE, ACTOR_INNER_EAST_STAKE};

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* Unused: an empty script nothing starts. */
Uint32 TrialRoad_EmptyScript[] = {
	SC_END,
};

Uint32 TrialRoad_GaspHopScript[] = {
	SC_ADD(SW_FACING, FACING_EAST_SOUTHEAST),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

void (*TrialRoad_StageSetUps[])(void) = {
	scTrialRoadSetUpPillarStage,
	scTrialRoadSetUpTorchStage,
	scTrialRoadSetUpWeedsPillarStage,
	scTrialRoadSetUpPoundStage,
	scTrialRoadSetUpCrumblingStage,
	scTrialRoadSetUpLogStage,
	scTrialRoadSetUpFountainStage,
	scTrialRoadSetUpSandStage,
};

/* The recordings Moapa's party runs by, as resource ids: one for each stage */
/* pattern, in TrialRoad_StageSetUps' order, then his run at the blue chests */
/* when he reached the stage's switch first and when Felix did. */
Uint32 TrialRoad_RivalRecordings[] = {
	RESOURCE_TRIAL_ROAD_PILLAR_RECORDING,
	RESOURCE_TRIAL_ROAD_TORCH_RECORDING,
	RESOURCE_TRIAL_ROAD_WEEDS_RECORDING,
	RESOURCE_TRIAL_ROAD_POUND_RECORDING,
	RESOURCE_TRIAL_ROAD_CRUMBLING_RECORDING,
	RESOURCE_TRIAL_ROAD_LOG_RECORDING,
	RESOURCE_TRIAL_ROAD_FOUNTAIN_RECORDING,
	RESOURCE_TRIAL_ROAD_SAND_RECORDING,
	RESOURCE_TRIAL_ROAD_CHESTS_MOAPA_FIRST_RECORDING,
	RESOURCE_TRIAL_ROAD_CHESTS_FELIX_FIRST_RECORDING,
};

/* Each stage's place in the map, in metatiles. */
Sint16 TrialRoad_StageMapMetatiles[][2] = {
	{1, 48}, {15, 48}, {29, 48}, {1, 70},
	{15, 70}, {29, 70}, {1, 92}, {15, 92},
};

/* The stage pattern each of the ten treasures belongs to: as a stage is */
/* laid, the treasures of the others are put aside at (0, 0). */
Sint8 TrialRoad_RulesTreasureStages[] = {
	0, 1, 2, 3, 4, 4, 4, 5,
	6, 7,
};

/* The same table as TrialRoad_RulesTreasureStages, under another name. */
Sint8 TrialRoad_TreasureStages[] = {
	0, 1, 2, 3, 4, 4, 4, 5,
	6, 7,
};

/* The stages each path's order must hold at least one of: the pillar, weeds */
/* pillar, Pound and sand stages. scTrialRoadDealStageOrder reads only these */
/* first four. */
Sint8 TrialRoad_KeyStages[] = {
	0, 2, 3, 7, 1, 4, 5, 6,
};

Uint32 TrialRoad_RightKnightEntersScript[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_SET(SW_X, toFIXED(312)),
	SC_SET(SW_Z, toFIXED(680)),
	SC_ANIMATION(5),
	SC_MOVE(312, 0, 632),
	SC_SYNC,
	SC_MOVE(344, 0, 616),
	SC_SYNC,
	SC_MOVE(408, 0, 616),
	SC_SYNC,
	SC_MOVE(424, 0, 600),
	SC_SYNC,
	SC_MOVE(424, 0, 456),
	SC_SYNC,
	SC_END,
};

Uint32 TrialRoad_LeftKnightEntersScript[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_SET(SW_X, toFIXED(232)),
	SC_SET(SW_Z, toFIXED(680)),
	SC_ANIMATION(5),
	SC_MOVE(232, 0, 632),
	SC_SYNC,
	SC_MOVE(200, 0, 616),
	SC_SYNC,
	SC_MOVE(136, 0, 616),
	SC_SYNC,
	SC_MOVE(120, 0, 600),
	SC_SYNC,
	SC_MOVE(120, 0, 456),
	SC_SYNC,
	SC_END,
};

Uint32 TrialRoad_LeftKnightToChestScript[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_SET(SW_X, toFIXED(120)),
	SC_SET(SW_Z, toFIXED(296)),
	SC_ANIMATION(5),
	SC_MOVE(120, 0, 216),
	SC_SYNC,
	SC_MOVE(136, 0, 200),
	SC_SYNC,
	SC_MOVE(216, 0, 200),
	SC_SYNC,
	SC_MOVE(232, 0, 184),
	SC_SYNC,
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_END,
};

Uint32 TrialRoad_LeftKnightLeavesScript[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_ANIMATION(5),
	SC_MOVE(216, 0, 168),
	SC_SYNC,
	SC_MOVE(232, 0, 152),
	SC_SYNC,
	SC_MOVE(232, 0, 136),
	SC_SYNC,
	SC_SET(SW_X, 0),
	SC_SET(SW_Z, 0),
	SC_END,
};

Uint32 TrialRoad_RightKnightToChestsScript[] = {
	SC_SPEED(1),
	SC_ACCELERATION(0.5),
	SC_SET(SW_X, toFIXED(344)),
	SC_SET(SW_Z, toFIXED(184)),
	SC_ANIMATION(2),
	SC_MOVE(312, 0, 168),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};

Uint32 TrialRoad_RightKnightLeavesScript[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_ANIMATION(5),
	SC_MOVE(376, 0, 200),
	SC_SYNC,
	SC_MOVE(472, 0, 200),
	SC_SYNC,
	SC_MOVE(488, 0, 184),
	SC_SYNC,
	SC_MOVE(488, 0, 136),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};
/* clang-format on */
#undef MARKER

#undef DEFINE_TRIAL_ROAD_SET_POOL_TILES
#undef SET_LAYER
#undef MARKER

/* clang-format off */
#define DUST_SCRIPT_SECTION LATE_DATA
#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"
/* clang-format on */

void scTrialRoadFightGasp(void);
void scTrialRoadOpenChests(void);
void scTrialRoadJumpForward(void);
void scTrialRoadBlowAwayPoundStageWeeds(void);
void scTrialRoadStandOnBlock(void);
void scTrialRoadShutSummitDoorway(Sint32 sound);
void scTrialRoadStepOffSwitch(void);

/* Unused: the digits 0 to 9 and A to F, four to a word. */
/* clang-format off */
LAST_DATA Uint32 TrialRoad_HexDigits[] = {
	0x33323130, 0x37363534, 0x42413938, 0x46454443, 0,
};

LAST_DATA SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 440, 0, 184, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{9, CONDITION_ALWAYS, 344, 0, 264, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

LAST_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(TrialRoadStages),
	SCENE_EXIT(METATILE_LEFT_PATH_STAIRS_DOWN, TrialRoadStages, TRIAL_ROAD_STAGE_TOP_LEFT),
	SCENE_EXIT(METATILE_RIGHT_PATH_STAIRS_DOWN, TrialRoadStages, TRIAL_ROAD_STAGE_TOP_RIGHT),
	SCENE_EXIT(METATILE_LEFT_PATH_DOOR, TrialRoadStages, TRIAL_ROAD_STAGE_FOOT_LEFT),
	SCENE_EXIT(METATILE_RIGHT_PATH_DOOR, TrialRoadStages, TRIAL_ROAD_STAGE_FOOT_RIGHT),
	SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_A, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_A),
	SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_B, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_B),
	SCENE_EXIT(TRIAL_ROAD_STAGES_TO_TRIAL_ROAD_SUMMIT_WEST_A, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_STAGES_WEST),
	SCENE_EXIT(TRIAL_ROAD_STAGES_TO_TRIAL_ROAD_SUMMIT_EAST_A, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_STAGES_EAST),
	SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_C, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_C),
	SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_D, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_E),
	SCENE_EXITS(TrialRoadCaveRock),
	SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_EAST_PASSAGE_FROM_WEST_CAVE),
	SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_WEST_CAVE_FROM_EAST_PASSAGE),
	SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_CAVE_ROCK),
	SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP, TrialRoadGasp, TRIAL_ROAD_GASP_FROM_TRIAL_ROAD_CAVE_ROCK),
	SCENE_EXITS(TrialRoadGasp),
	SCENE_EXIT(TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_FROM_TRIAL_ROAD_GASP),
	SCENE_EXITS(TrialRoadSummit),
	SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST),
	SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_EAST),
	SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_FROM_TRIAL_ROAD_SUMMIT),
	SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_SHAMAN_VILLAGE_HOUSES_EAST_A, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A),
	SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_SHAMAN_VILLAGE_HOUSES_EAST_B, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_B),
	SCENE_EXITS_END,
};

LAST_DATA ACTOR_PLACE TrialRoad_Placements[] = {
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_PillarStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)TrialRoad_SettleScript, 248, 0, 400, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_TorchStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 456, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_WeedsPillarStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 440, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)TrialRoad_SettleScript, 232, 0, 432, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)TrialRoad_SettleScript, 296, 0, 432, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_PoundStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 456, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)TrialRoad_SettleScript, 264, 0, 464, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_CrumblingStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_LogStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TREE_TRUNK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TREE_TRUNK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STACKED_LOGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 456, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_FountainStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 472, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 440, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 440, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_SandStagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 408, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)TrialRoad_SettleScript, 312, 0, 336, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_CaveRockPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_GaspPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GASP), MOVEMENT_TYPE_STAND, 344, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

LAST_DATA ACTOR_PLACE TrialRoad_SummitPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 328, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* Each stage pattern's actor placements. */
LAST_DATA Sint32 TrialRoad_StagesPlacements[] = {
	(Sint32)TrialRoad_PillarStagePlacements,
	(Sint32)TrialRoad_TorchStagePlacements,
	(Sint32)TrialRoad_WeedsPillarStagePlacements,
	(Sint32)TrialRoad_PoundStagePlacements,
	(Sint32)TrialRoad_CrumblingStagePlacements,
	(Sint32)TrialRoad_LogStagePlacements,
	(Sint32)TrialRoad_FountainStagePlacements,
	(Sint32)TrialRoad_SandStagePlacements,
};

LAST_DATA CUE TrialRoad_Cues[] = {
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_PillarStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_PILLAR_STAGE_GAP, IF_FLAG_SET(FLAG_LEADER_ON_PILLARS), SCRIPT_REF(scTrialRoadJumpAcrossGap)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_PILLAR_STAGE_LEDGE, IF_FLAG_SET(FLAG_LEADER_ON_PILLARS), SCRIPT_REF(scTrialRoadJumpForward)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_C, IF_FLAG_SET(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY), SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_D, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_E, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, IF_FLAG_SET(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY), SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_D, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_E, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_A, FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY, SCRIPT_REF(scTrialRoadBlowAwayPillarStageWeeds)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_TorchStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_ICE_PILLAR_SIDE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpMeltedPuddle)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_ICE_PILLAR_SIDE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpMeltedPuddle)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadClearJumpUnderTorchStagePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadClearJumpUnderTorchStagePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadClearJumpUnderTorchStagePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WEST_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadClearJumpUnderTorchStagePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadClearJumpUnderTorchStagePillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadMeltIceBesideTorch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadMeltIceBesideTorch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadMeltIceBesideTorch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadMeltIceBesideTorch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadMeltIceBesideTorch)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_C, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_C), SCRIPT_REF(scTrialRoadFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_D, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_D), SCRIPT_REF(scTrialRoadFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_E, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_E), SCRIPT_REF(scTrialRoadFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_F, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_F), SCRIPT_REF(scTrialRoadFreezePuddle)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_WeedsPillarStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_WEEDS_STAGE_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpForward)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_A, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A), SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_B, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B), SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_C, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_D, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_E, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A), SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B), SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_D, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_E, CONDITION_ALWAYS, SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_A, FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A, SCRIPT_REF(scTrialRoadBlowAwayWeeds)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_B, FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B, SCRIPT_REF(scTrialRoadBlowAwayWeeds)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_PoundStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadExamineChest)},
	{STEP_CUE_WALKING, METATILE_WEST_STAKE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadLandOnPillar)},
	{STEP_CUE_WALKING, METATILE_INNER_WEST_STAKE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadLandOnPillar)},
	{STEP_CUE_WALKING, METATILE_INNER_EAST_STAKE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadLandOnPillar)},
	{STEP_CUE_WALKING, METATILE_EAST_STAKE, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadLandOnPillar)},
	{CUE_STEP, METATILE_WEST_STAKE, FLAG_INNER_WEST_STAKE_RAISED, SCRIPT_REF(scTrialRoadStepOntoWestStake)},
	{CUE_STEP, METATILE_INNER_WEST_STAKE, IF_FLAG_SET(FLAG_INNER_WEST_STAKE_RAISED), SCRIPT_REF(scTrialRoadStepOntoInnerWestStake)},
	{CUE_STEP, METATILE_INNER_EAST_STAKE, FLAG_EAST_STAKE_RAISED, SCRIPT_REF(scTrialRoadStepOntoInnerEastStake)},
	{CUE_STEP, METATILE_EAST_STAKE, IF_FLAG_SET(FLAG_EAST_STAKE_RAISED), SCRIPT_REF(scTrialRoadStepOntoEastStake)},
	{STEP_CUE_WALKING, METATILE_STAKE_GROUND, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadLandOnPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadCheckStakesCovered)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, IF_FLAG_SET(FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY), SCRIPT_REF(scTrialRoadCheckStakesCovered)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadCheckStakesCovered)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_POUND_STAGE_WEEDS, FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY, SCRIPT_REF(scTrialRoadBlowAwayPoundStageWeeds)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_WEST_STAKE, FLAG_INNER_WEST_STAKE_RAISED, SCRIPT_REF(scTrialRoadPoundWestStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_INNER_WEST_STAKE, IF_FLAG_SET(FLAG_INNER_WEST_STAKE_RAISED), SCRIPT_REF(scTrialRoadPoundInnerWestStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_INNER_EAST_STAKE, FLAG_EAST_STAKE_RAISED, SCRIPT_REF(scTrialRoadPoundInnerEastStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_EAST_STAKE, IF_FLAG_SET(FLAG_EAST_STAKE_RAISED), SCRIPT_REF(scTrialRoadPoundEastStake)},
	{CUE_TABLE_END},
};

/* The crumbling stage's two cracked pillars, west and east, each with the */
/* flag that remembers its first step (scLeaderCheckCrumblingStep), ended by */
/* -1. */
LAST_DATA Sint16 TrialRoad_CrumblingPillars[] = {
	ACTOR_WEST_CRUMBLING_PILLAR, FLAG_WEST_CRUMBLING_PILLAR_STEPPED_ON,
	ACTOR_EAST_CRUMBLING_PILLAR, FLAG_EAST_CRUMBLING_PILLAR_STEPPED_ON,
	-1, 0,
};
/* clang-format on */

LAST_DATA CUE TrialRoad_CrumblingStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_CRUMBLING_STAGE_LEDGE,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_CRUMBLING_STAGE_LEDGE,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_WEST_CRUMBLING_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_EAST_CRUMBLING_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BESIDE_CRUMBLING_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadCheckCrumblingPillars)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BESIDE_CRUMBLING_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadCheckCrumblingPillars)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_WEST_CRUMBLING_PILLAR, FLAG_WEST_CRUMBLING_PILLAR_CRUMBLED,
		SCRIPT_REF(scTrialRoadCrumbleWestPillar)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_EAST_CRUMBLING_PILLAR, FLAG_EAST_CRUMBLING_PILLAR_CRUMBLED,
		SCRIPT_REF(scTrialRoadCrumbleEastPillar)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_LogStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_LOG_STAGE_LEDGE,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_UPPER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushUpperLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_UPPER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushUpperLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_LOWER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushLowerLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_LOWER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushLowerLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_UPPER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogNorth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_UPPER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogSouth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_LOWER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogNorth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_LOWER_LOG_ROWS,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogSouth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_EAST_LOG_NORTH_END,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogNorth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_EAST_LOG_NORTH_END,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogSouthOrClimbDown)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_EAST_LOG_SOUTH_END,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogNorth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_EAST_LOG_SOUTH_END,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadPushEastLogSouth)},
	{CUE_PUSH, 0, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_PILLAR_A, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_FountainStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_STEP, METATILE_BLOCK_TRACK, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadStandOnBlock)},
	{CUE_PUSH, 0, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_PILLAR_A, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_ClearJumpUnderPushedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS,
		SCRIPT_REF(TrialRoad_SetJumpUnderPushedPillar)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_WEST_VENT, FLAG_VENT_UNCOVERED_BASE + ACTOR_WEST_VENT,
		SCRIPT_REF(scTrialRoadUncoverWestVent)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_MIDDLE_VENT, FLAG_VENT_UNCOVERED_BASE + ACTOR_MIDDLE_VENT,
		SCRIPT_REF(scTrialRoadUncoverMiddleVent)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_EAST_VENT, FLAG_VENT_UNCOVERED_BASE + ACTOR_EAST_VENT,
		SCRIPT_REF(scTrialRoadUncoverEastVent)},
	{CUE_TABLE_END},
};

LAST_DATA const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

LAST_DATA CUE TrialRoad_SandStageCues[] = {
	{CUE_STEP, METATILE_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_LEFT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadTakeStairs)},
	{CUE_STEP, METATILE_AROUND_WEST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_EAST_GIVE_UP_BUTTON,
		IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
		SCRIPT_REF(scTrialRoadClearGiveUpAsked)},
	{CUE_STEP, METATILE_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
		SCRIPT_REF(scTrialRoadOfferToGiveUp)},
	{CUE_STEP, METATILE_AROUND_WEST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{CUE_STEP, METATILE_AROUND_EAST_SWITCH,
		IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
		SCRIPT_REF(scTrialRoadStepOffSwitch)},
	{CUE_STEP, METATILE_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
		SCRIPT_REF(scTrialRoadOpenChests)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
		IF_FLAG_SET(FLAG_INNER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
		CUE_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
		IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN),
		SCRIPT_REF(scTrialRoadPutItemInChest)},
	{CUE_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadExamineChest)},
	{CUE_STEP, METATILE_STREAM, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadDriftWithStream)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadStartSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadEndSand)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SAND_STAGE_WEEDS,
		FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY,
		SCRIPT_REF(scTrialRoadBlowAwaySandStageWeeds)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_CaveRockCues[] = {
	{CUE_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE, CONDITION_ALWAYS,
		TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE},
	{CUE_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE, CONDITION_ALWAYS,
		TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE},
	{CUE_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT, CONDITION_ALWAYS,
		TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT},
	{CUE_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP, CONDITION_ALWAYS,
		TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_CAVE_ROCK, FLAG_CAVE_ROCK_LIFTED,
		SCRIPT_REF(scTrialRoadLiftCaveRock)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_GaspCues[] = {
	{CUE_EXIT, TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK, CONDITION_ALWAYS,
		TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadScatterWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_WEEDS_HIDING_MAD_PLANT, CONDITION_ALWAYS,
		SCRIPT_REF(scTrialRoadScatterWeedsHidingMadPlant)},
	{CUE_TALK, ACTOR_GASP, CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadFightGasp)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadRevealPoolTiles)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scTrialRoadHidePoolTiles)},
	{CUE_TABLE_END},
};

LAST_DATA CUE TrialRoad_SummitCues[] = {
	{CUE_STEP, METATILE_SUMMIT_DOORWAY, FLAG_SUMMIT_DOORWAY_SHUT,
		SCRIPT_REF(scTrialRoadShutSummitDoorway)},
	{CUE_STEP, METATILE_LEFT_PATH_SUMMIT,
		IF_FLAG_SET(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT),
		SCRIPT_REF(scTrialRoadChallengeMoapa)},
	{CUE_STEP, METATILE_RIGHT_PATH_SUMMIT,
		IF_FLAG_SET(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT),
		SCRIPT_REF(scTrialRoadChallengeMoapa)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST, CONDITION_ALWAYS,
		TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST, CONDITION_ALWAYS,
		TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST},
	{CUE_EXIT, TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK, CONDITION_ALWAYS,
		TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PEAK_PILLAR,
		FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED,
		SCRIPT_REF(scTrialRoadSetSummitPillar)},
	{CUE_PUSHED, 0, FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED,
		SCRIPT_REF(scTrialRoadSetSummitPillar)},
	{CUE_TABLE_END},
};

LAST_DATA Sint32 TrialRoad_StagesCues[] = {
	(Sint32)TrialRoad_PillarStageCues,
	(Sint32)TrialRoad_TorchStageCues,
	(Sint32)TrialRoad_WeedsPillarStageCues,
	(Sint32)TrialRoad_PoundStageCues,
	(Sint32)TrialRoad_CrumblingStageCues,
	(Sint32)TrialRoad_LogStageCues,
	(Sint32)TrialRoad_FountainStageCues,
	(Sint32)TrialRoad_SandStageCues,
};

/* The summit's colour cycle, in frames; -1 before it starts. */
LAST_DATA Sint32 TrialRoad_PaletteCycleFrame = -1;

LAST_DATA Sint32 TrialRoad_PaletteCycleRunning = 1;
