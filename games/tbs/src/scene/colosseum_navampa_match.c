/* Colosseum Final 3 (SceneIdColosseumNavampaMatch) is the site of the last
   finals match, where Isaac races Navampa of Gondowan through six stages to the
   arena and fights him there.

   Isaac has won the first two matches, and Navampa is the sixth-ranked warrior.

   It works as Colosseum Final 1 does (colosseum_azart_match.c). The guide, who
   did not think Isaac would make it this far, leaves the stages to their
   attendants. The stages run through the Scales of Justice, the Stepping-Stone
   Stage, the Wall Wedge, the Free-Climb Area, the Moving Sidewalk Stage and
   another log-rolling area.

   scInit restores the stages from their temporary flags (FLAG_PLANT_GROWN to
   FLAG_LONG_LOG_IN_PLACE, and the stone's column from FLAG_STONE_COLUMN). Then
   it sets up the visit its entrance stands for.

   - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH, the tour
	 (scColosseumNavampaMatchIntroduceThirdFinals), from the Colosseum's east
	 passage after the second win. The attendants describe their stages and
	 take the friends to cheer, and the guide sends Isaac back to the passage
	 (COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM).
   - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, `Lend a Hand`. Garet, Ivan
	 and Mia in turn may use Psynergy on the stages. Frost freezes the target
	 on the Scales of Justice's eastern ledge so the scales stay tipped. Halt
	 stops the Wall Wedge's operator and with him its pistons. Force knocks the
	 log off the Moving Sidewalk so the belt no longer carries anyone back.
	 Growth grows a plant into a shortcut. Move shifts the weight onto the
	 scales and the stones and the rock on the way. After Mia,
	 COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE starts the race.
   - COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, the race against Navampa,
	 who follows a recorded path, after the lettering `The 3rd Match` and a
	 count of three. At the arena the first to arrive takes the Claymore and
	 the second the Hunter's Sword. The battle is ENCOUNTER_ZONE_COLOSSO's
	 fifth or sixth group, the Navampa with or without the Claymore.

   Stepping onto the Scales of Justice's western ledge tips the scales
   (scColosseumNavampaMatchStepOnScales): the ledge sinks almost to the floor
   with Isaac on it, and the eastern ledge rises. Stepping off swings them back.
   The weight Move sets on the western ledge tips them the same way, and once
   Frost has frozen the puddle on the eastern ledge
   (scColosseumNavampaMatchFreezeOnScales) stepping on no longer moves them.

   Win or lose, the third match ends Colosso.
   COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON and _LOST set FLAG_COLOSSO_OVER, and
   a win also FLAG_WON_COLOSSO.

   In the script that follows (scColosseumNavampaMatchPlayPostBattleReunion),
   Garet, Ivan and Mia run to Isaac in the arena, but he falls asleep,
   exhausted, and they leave him to sleep. Both exits lead to Babi's Palace,
   where he wakes long after the finals. */

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/map_grid.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "scene/colosso_gauge.h"
#include "scene/colosso_guide.h"
#include "scene/colosso_lettering.h"
#include "scene/colosso_push.h"
#include "scene/colosso_setup.h"
#include "scene/colosso_solo.h"
#include "scene/sleep_z.h"
#include "text/msg_ids.h"

/* The match held here, as the lettering `The 3rd Match` reads; the friends'
   turns open with `Lend a Hand!` instead (scColossoShowOpeningLettering). */
enum { FINALS_MATCH = 3 };

/* The stages, as their attendants number them: "This is stage 1 of the third
   finals". */
typedef enum COLOSSEUM_NAVAMPA_MATCH_STAGE {
	STAGE_SCALES_OF_JUSTICE = 1,
	STAGE_STEPPING_STONE = 2,
	STAGE_WALL_WEDGE = 3,
	STAGE_FREE_CLIMB = 4,
	STAGE_MOVING_SIDEWALK = 5,
	STAGE_LOG_ROLLING = 6
} COLOSSEUM_NAVAMPA_MATCH_STAGE;

/* FIELD_STATE.raisedCue with nothing raised. */
enum { RAISED_CUE_NONE = 0 };

/* The Moving Sidewalk Stage's two tileset animations: the belt's, which runs
   while it carries the party west, and the one that runs instead once Force has
   knocked its log off. */
enum { SIDEWALK_RUNNING_ANIMATION = 1, SIDEWALK_STOPPED_ANIMATION = 2 };

/* SPRITE_STANDING_LOG's animations (standing_log.tsv) for the Moving Sidewalk's
   log: it tips over (picture 1) and lies on its side, its cut end facing the
   viewer (picture 2). */
typedef enum STANDING_LOG_ANIMATION {
	STANDING_LOG_ANIMATION_TOPPLE = 2,
	STANDING_LOG_ANIMATION_FALLEN = 3
} STANDING_LOG_ANIMATION;

/* The long log's roll through pictures 10 to 13 (log_north_south.tsv), which it
   plays rolling west, where Vale Cave and Kolima's floodgate play it as
   LOG_ANIMATION_ROLL_EAST. Rolling east it plays the same pictures backwards,
   LOG_ANIMATION_ROLL_WEST. */
/* XXX: which way pictures 10 to 13 turn the log; a recording of the long log
   rolled each way, beside one of Vale Cave's west log, would settle it. */
#define LONG_LOG_ANIMATION_ROLL_WEST 2

/* The Wall Wedge's operator's poses (young_man_with_fair_hair.tsv's held
   pictures 54 to 56), in step with the pistons he works: as the first, third
   and fifth pistons pull in, as they pass half out, and as they push out. */
typedef enum WALL_OPERATOR_ANIMATION {
	WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT = 10,
	WALL_OPERATOR_ANIMATION_ODD_PISTONS_IN = 11,
	WALL_OPERATOR_ANIMATION_ODD_PISTONS_OUT = 12
} WALL_OPERATOR_ANIMATION;

/* Metatiles the stages' changes copy from, kept off the course: the Scales of
   Justice's western ledge, metatile (9, 12), sunk with someone on it, level
   (METATILE_BEHAVIOR_SCALES_LEDGE) and sunk under the weight; and the store at
   (120..127, 60..65) where scInit keeps layer 2's metatiles of the long log's
   lane, (74..81, 60..65), before the log is marked in them. */
typedef enum COLOSSEUM_NAVAMPA_MATCH_SPARE_METATILE {
	SCALES_LEDGE_SUNK_COLUMN = 0,
	SCALES_LEDGE_SUNK_ROW = 24,
	SCALES_LEDGE_LEVEL_COLUMN = 0,
	SCALES_LEDGE_LEVEL_ROW = 25,
	SCALES_LEDGE_WEIGHTED_COLUMN = 1,
	SCALES_LEDGE_WEIGHTED_ROW = 25,
	LOG_LANE_STORE_COLUMN = 120,
	LOG_LANE_STORE_ROW = 60
} COLOSSEUM_NAVAMPA_MATCH_SPARE_METATILE;

Sint32 ColosseumNavampaMatchSleepTick BSS_DATA ALIGNED(16);
/* Unused: twelve bytes after the sleep tick that nothing reads. */
Uint8 ColosseumNavampaMatchUnusedAfterSleepTick[12] BSS_DATA;
/* The shared Colosso code's state, in the order the scene's memory lays it out;
   the shared files that use it say what each holds: the guide's pointer
   (colosso_guide.c), the lettering (colosso_task.c) and where Isaac stood
   before he was shown on a stage (colosso_guide.c). */
Sint16 ColossoPointerStep					  BSS_DATA;
Sint16 ColossoLetteringScaleDuration BSS_DATA ALIGNED(4);
Sint16 ColossoPointerFlip BSS_DATA			  ALIGNED(4);
/* Isaac's z before he is shown on the stage, 16.16 fixed-point pixels. */
FIXED ColossoWarriorStartZ					 BSS_DATA;
Sint16 ColossoPointerEndX					 BSS_DATA;
Sint16 ColossoLetteringParameter BSS_DATA	 ALIGNED(4);
Sint16 ColossoLetteringScaleStart BSS_DATA	 ALIGNED(4);
Sint16 ColossoLetteringMoveDuration BSS_DATA ALIGNED(4);
Sint32 ColossoLetteringPosition				 BSS_DATA;
Sint16 ColossoPointerBlink					 BSS_DATA;
Sint16 ColossoLetteringScaleTarget BSS_DATA	 ALIGNED(4);
Sint16 ColossoLetteringMoveStep BSS_DATA	 ALIGNED(4);
Sint16 ColossoPointerY BSS_DATA				 ALIGNED(4);
Sint16 ColossoLetteringFadeStep BSS_DATA	 ALIGNED(4);
/* Isaac's facing before scColossoFadeInWarrior, which scColossoRestoreWarrior
   puts back. */
Sint32 ColossoWarriorStartAngle				 BSS_DATA;
Sint16 ColossoPointerSteps					 BSS_DATA;
Sint16 ColossoLetteringMode BSS_DATA		 ALIGNED(4);
Sint16 ColossoLetteringFadeTarget BSS_DATA	 ALIGNED(4);
Sint16 ColossoLetteringFadeStart BSS_DATA	 ALIGNED(4);
Sint16 ColossoLetteringTimer BSS_DATA		 ALIGNED(4);
Sint16 *ColossoLetteringScript				 BSS_DATA;
POINTER_COORDINATE ColossoPointerStartX		 BSS_DATA;
Sint16 ColossoLetteringFadeDuration BSS_DATA ALIGNED(4);
Uint8 ColosseumNavampaMatchUnusedAfterBlendDuration[6] BSS_DATA;
/* The blinking pointer's sprite part: the queue link and attributes 0 to 2. */
Sint32									ColossoPointerOam[3] BSS_DATA;
POINTER_COORDINATE ColossoPointerStartY BSS_DATA;
/* The lettering's sprite parts, three words each, written every tick. */
Uint32									   ColossoLetteringSprites[12] BSS_DATA;
Sint16 ColossoLetteringMoveStart		   BSS_DATA;
Sint16 ColossoPointerX BSS_DATA			   ALIGNED(4);
Sint16 ColossoLetteringMoveTarget BSS_DATA ALIGNED(4);
Sint16 ColossoLetteringScaleStep BSS_DATA  ALIGNED(4);
Sint16 ColossoPointerEndY BSS_DATA		   ALIGNED(4);
/* Isaac's x before he is shown on the stage, 16.16 fixed-point pixels. */
FIXED ColossoWarriorStartX BSS_DATA;

Sint32				  scInit(void);
extern const Sint32	  ColosseumNavampaMatchSidewalkLogOffBelt[];
extern const Sint8	  ColosseumNavampaMatchWallWedgeCycle[][6];
extern const Uint16	  ColosseumNavampaMatchFacingAngles[];
extern const Sint32	  ColosseumNavampaMatchSidewalkLogScript[];
extern SCENE_ENTRANCE ColosseumNavampaMatchEntrances[];
extern Uint32		  ColosseumNavampaMatchExits[];
extern ACTOR_PLACE	  ColosseumNavampaMatchActorPlacements[];
extern Sint32		  ColosseumNavampaMatchWallWedgeStep;
extern Uint32		  ColosseumNavampaMatchWallWedgeTimer;
extern CUE			  ColosseumNavampaMatchCues[];
extern Sint32		  ColosseumNavampaMatchOpponentAfterBattle[];
extern Sint32		  ColosseumNavampaMatchLeaderActionA[];
extern Sint32		  ColosseumNavampaMatchLeaderActionB[];
extern Sint32		  ColosseumNavampaMatchLeaderActionC[];
extern Sint32		  ColosseumNavampaMatchLeaderScaleScript[];
extern Sint32		  ColosseumNavampaMatchSleepZScript[];
extern char			  HexDigits[20];

/* UB: this calls a void function through an Sint32 function type, the call the
   bytes show; UBFIX calls it as declared. */
#if FIX_FUNCTION_SIGNATURES
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION fdActorFaceDirectionById
#else
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION \
	((Sint32 (*)())fdActorFaceDirectionById)
#endif

static void scColosseumNavampaMatchPlayPostBattleReunion(Sint32 battleResult);
static void scColosseumNavampaMatchIntroduceThirdFinals(Sint32 guideId);
static void scColossoShowOpeningLettering(Sint32 match);
/* Unused: the third match ends with the party's reunion in the arena
   (scColosseumNavampaMatchPlayPostBattleReunion) instead. */
UNUSED static void scColossoShowMatchResult(Sint32 result);
static void		   scColossoNoOperation(void);
static void		   scColossoWaitForOpponentToFinish(void);
static Sint32	   scColossoTakePrize(Sint32 firstItem, Sint32 secondItem);
static void		   scColossoPushActorAhead(void);

typedef void (*MAP_TASK_ENTRY)(void);

/* The third finals match's actors, numbered in the order of
   ColosseumNavampaMatchActorPlacements from ACTOR_FIRST_PLACED.

   The rows of Isaac, Garet, Ivan and Mia take their own actors.

   By the guide's and attendants' explanations, the Scales of Justice, the
   Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
   Sidewalk Stage and another log-rolling area lead to the arena.

   ACTOR_SCALES_WEIGHT to ACTOR_LOG_AREA_STONE_C are all drawn as the rocks of
   Colosseum Final 1's Rock Challenge. The attendant of this match's log-rolling
   area calls the ones there stones. */
enum {
	/* Navampa, racing Isaac to the arena. */
	ACTOR_OPPONENT = 8,
	/* The puddle Frost freezes on the Scales of Justice's eastern ledge. */
	ACTOR_PUDDLE = 9,
	/* The Scales of Justice's two ledges. */
	ACTOR_SCALES_WEST_LEDGE = 10,
	ACTOR_SCALES_EAST_LEDGE = 11,
	ACTOR_SCALES_WEIGHT = 12,
	ACTOR_STONE = 13,
	/* A stone pillar east of the Wall Wedge's pistons, by the operator. It can
	   be pushed or moved, and nothing else comes of it
	   (scColosseumNavampaMatchJustMoveRock). */
	ACTOR_ROCK = 14,
	/* The log-rolling area's three stone pillars. One pushed or moved off the
	   ledge stays down in the tank as a step
	   (scColosseumNavampaMatchUpdateLogAreaStoneMetatiles,
	   FLAG_FIRST_LOG_AREA_STONE_SETTLED and the two after it). */
	ACTOR_LOG_AREA_STONE_A = 15,
	ACTOR_LOG_AREA_STONE_B = 16,
	ACTOR_LOG_AREA_STONE_C = 17,
	/* The Wall Wedge's five pistons, west to east. Each is drawn by two actors,
	   ACTOR_FIRST_PISTON + n and ACTOR_FIRST_PISTON + PISTON_COUNT + n. */
	ACTOR_FIRST_PISTON = 18,
	ACTOR_LAST_PISTON = 22,
	/* A young man with fair hair who works the Wall Wedge. */
	ACTOR_WALL_OPERATOR = 28,
	/* The plant Growth grows into a shortcut. */
	ACTOR_GROWTH_PLANT = 29,
	/* The log Force knocks off the Moving Sidewalk Stage's conveyor belt. */
	ACTOR_SIDEWALK_LOG = 30,
	/* The log-rolling area's long log. */
	ACTOR_LONG_LOG = 31,
	/* A soldier who explains the third finals match. */
	ACTOR_GUIDE = 32,
	/* The stages' attendants, young men with fair hair in white tunics. */
	ACTOR_SCALES_ATTENDANT = 33,
	ACTOR_STEPPING_STONE_ATTENDANT = 34,
	ACTOR_WALL_WEDGE_ATTENDANT = 35,
	ACTOR_FREE_CLIMB_ATTENDANT = 36,
	ACTOR_MOVING_SIDEWALK_ATTENDANT = 37,
	ACTOR_LOG_ROLLING_ATTENDANT = 38,
	/* The arena's prizes, drawn as item icons at two blank actors
	   (scColossoShowItemIcon). */
	ACTOR_PRIZE_A = 39,
	ACTOR_PRIZE_B = 40,
	/* The first of the three letters Z that rise from the sleeping Isaac after
	   the last match (scColosseumNavampaMatchSpawnSleepZ). */
	ACTOR_FIRST_SLEEP_Z = 41
};

/* The temporary flags that keep what the friends changed on the stages, for the
   race, and the scales' state.

   scInit restores the stages from them. */
enum {
	FLAG_PLANT_GROWN = FLAG_AREA_TEMP_60, /* by Growth, into a shortcut */
	/* by Halt on ACTOR_WALL_OPERATOR */
	FLAG_WALL_WEDGE_HALTED = FLAG_AREA_TEMP_61,
	FLAG_SCALES_FROZEN = FLAG_AREA_TEMP_62, /* by Frost on ACTOR_PUDDLE */
	/* By Force on ACTOR_SIDEWALK_LOG. The belt carries no one any more. */
	FLAG_SIDEWALK_LOG_KNOCKED_OFF = FLAG_AREA_TEMP_63,
	/* Someone stands on the western ledge, which has sunk
	   (scColosseumNavampaMatchStepOnScales). */
	FLAG_ON_SCALES = FLAG_AREA_TEMP_67,
	/* ACTOR_SCALES_WEIGHT is on the western ledge */
	FLAG_WEIGHT_ON_SCALES = FLAG_AREA_TEMP_68,
	FLAG_LONG_LOG_IN_PLACE = FLAG_AREA_TEMP_69, /* at the east end */
	/* (scColosseumNavampaMatchRollLongLog) The byte of flags that keeps
	   ACTOR_STONE's column. */
	FLAG_STONE_COLUMN = FLAG_AREA_TEMP_70,
	/* ACTOR_LOG_AREA_STONE_A has settled. The next two flags are B's and
	   C's. */
	FLAG_FIRST_LOG_AREA_STONE_SETTLED = FLAG_SCENE_TEMP_14
};

/* The Wall Wedge's pistons stand in every other column, 32 pixels apart. */
enum { PISTON_COUNT = 5, PISTON_SPACING = 32 };

/* The Wall Wedge pistons' animations (piston.png). Each piston is two actors:
   its body, and PISTON_COUNT actors on, its tip, which shows below the body
   when it is all the way out and plays the body's animation +
   PISTON_ANIMATION_TIP. */
typedef enum PISTON_ANIMATION {
	PISTON_ANIMATION_IN = 1,
	PISTON_ANIMATION_HALF_OUT = 2,
	PISTON_ANIMATION_OUT = 3,
	PISTON_ANIMATION_PUSH_HALF_OUT = 4, /* from in to half out */
	PISTON_ANIMATION_PUSH_OUT = 5,		/* from half out to all the way out */
	PISTON_ANIMATION_PULL_HALF_IN = 6,	/* from out to half out */
	PISTON_ANIMATION_PULL_IN = 7,		/* from half out to in */
	PISTON_ANIMATION_TIP = 8
} PISTON_ANIMATION;

/* Metatile behaviors the stages' cues answer. */
typedef enum COLOSSEUM_NAVAMPA_MATCH_METATILE_BEHAVIOR {
	/* The Moving Sidewalk Stage's belt, metatiles (62..71, 11) and on. */
	METATILE_BEHAVIOR_MOVING_SIDEWALK = 11,
	/* The Scales of Justice's western ledge, metatile (9, 12), given this
	   behavior from the spare metatile (0, 25) while the scales are level. */
	METATILE_BEHAVIOR_SCALES_LEDGE = 15,
	/* Beside the ledge, metatiles (8..10, 13), where stepping off swings the
	   scales back. */
	METATILE_BEHAVIOR_OFF_SCALES = 16,
	/* The weight's square by the Scales of Justice, round (4..7, 9..11), where
	   walking into the weight pushes it. */
	METATILE_BEHAVIOR_WEIGHT_SQUARE = 20,
	/* The row of the stone pillar by the Wall Wedge's operator, (43..45,
	   11). */
	METATILE_BEHAVIOR_ROCK_ROW = 22,
	/* Where the long log's ride starts (scColosseumNavampaMatchRollLongLog). */
	METATILE_BEHAVIOR_LONG_LOG = 23,
	/* The way into the arena, metatiles (92, 9..14). */
	METATILE_BEHAVIOR_ARENA_ENTRY = 50,
	/* The east end of the log-rolling area, metatile (83, 13). */
	METATILE_BEHAVIOR_LOG_AREA_END = 90,
	/* The east edge of the weight's square, metatile (8, 12). */
	METATILE_BEHAVIOR_WEIGHT_SQUARE_EDGE = 91,
	/* The chests: a Smoke Bomb at (12, 7), a Nut at (44, 6) and a Sleep Bomb at
	   (75, 13), opened facing west. */
	METATILE_BEHAVIOR_SMOKE_BOMB_CHEST = 101,
	METATILE_BEHAVIOR_NUT_CHEST = 102,
	METATILE_BEHAVIOR_SLEEP_BOMB_CHEST = 103
} COLOSSEUM_NAVAMPA_MATCH_METATILE_BEHAVIOR;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return ColosseumNavampaMatchEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return ColosseumNavampaMatchExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ColosseumNavampaMatchActorPlacements;
}

/* The Wall Wedge's event steps its pistons through their cycle, a step every 18
   ticks. */
static void scColosseumNavampaMatchUpdateWallWedge(void)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR		*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32		 row = FIELD_METATILE(actor->z.fixed);
	Sint32		 i;
	Sint32		 kind;

	if(ColosseumNavampaMatchWallWedgeTimer == 0) {
		ColosseumNavampaMatchWallWedgeStep =
			(ColosseumNavampaMatchWallWedgeStep + 1) & 3;
		for(i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
			kind = ColosseumNavampaMatchWallWedgeCycle
				[ColosseumNavampaMatchWallWedgeStep][i - ACTOR_FIRST_PISTON];
			fdActorSetAnimation(i, kind);
			fdActorSetAnimation(i + PISTON_COUNT, kind + PISTON_ANIMATION_TIP);
			/* Each piston's column, (33 + 2n, 11..12), takes the walkway's (32,
			   11..12), and is walled by (74, 12) while the piston is out. */
			fdMapGridCopyMetatileAttributeRect(
				32, 11, 1, 2, (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
			if(kind != PISTON_ANIMATION_PULL_IN) {
				fdMapGridCopyMetatileAttributeRect(
					74, 12, 1, 1, (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
			}
		}
		fdActorSetAnimation(ACTOR_WALL_OPERATOR,
			ColosseumNavampaMatchWallWedgeCycle
				[ColosseumNavampaMatchWallWedgeStep][PISTON_COUNT]);
	} else {
		for(i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
			kind = ColosseumNavampaMatchWallWedgeCycle
				[ColosseumNavampaMatchWallWedgeStep][i - ACTOR_FIRST_PISTON];
			/* The leader stands in the piston's column, which starts 48 pixels
			   west of i * PISTON_SPACING. */
			if((actor->x.fixed - toFIXED(i * PISTON_SPACING) > -toFIXED(50) &&
				   actor->x.fixed - toFIXED(i * PISTON_SPACING) <=
					   -toFIXED(30) - 1)) {
				if(row == 11 && kind == PISTON_ANIMATION_PUSH_HALF_OUT)
					state->raisedCue = kind;
				if(row == 12 && kind == PISTON_ANIMATION_PUSH_OUT)
					state->raisedCue = kind;
			}
		}
	}
	if(++ColosseumNavampaMatchWallWedgeTimer > 17) {
		ColosseumNavampaMatchWallWedgeTimer = 0;
	}
}

/* Resets the Wall Wedge's pistons to the first step of their cycle. */
static void scColosseumNavampaMatchResetWallWedge(void)
{
	MAP_TASK_ENTRY entry;

	ColosseumNavampaMatchWallWedgeTimer = 0;
	entry = scColosseumNavampaMatchUpdateWallWedge;
	ColosseumNavampaMatchWallWedgeStep = 0;
	syCloseEvent(entry);
	entry();
}

/* Resumes cycling the Wall Wedge's pistons. */
static void scColosseumNavampaMatchStartWallWedge(void)
{
	sySetTickEvent(scColosseumNavampaMatchUpdateWallWedge, EVENT_PASS_TICK);
}

/* Waits, at most two seconds, for the step of the Wall Wedge's cycle that its
   attendant's demonstration passes through. */
static void scColosseumNavampaMatchWaitForWallWedgeOpening(void)
{
	Sint32 ticks;

	sySynch(10);

	ticks = 0;
	while(ColosseumNavampaMatchWallWedgeStep != 3 ||
		ColosseumNavampaMatchWallWedgeTimer != 1) {
		sySynch(1);
		ticks++;
		if(ticks > 119) {
			return;
		}
	}
}

/* The Moving Sidewalk Stage's belt carries the party west. */
static void scColosseumNavampaMatchRideMovingSidewalk(void)
{
	FIELD_STATE		   *fieldState = FieldStateWork;
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	Sint32				id = state->leaderActorId;
	ACTOR			   *subject = fieldState->camera;
	ACTOR			   *actor = fdActorGet(id);
	Sint32				z = actor->z.part[0].integer;

	if(z >= 183 && z <= 186) {
		subject->x.fixed -= toFIXED(4) / 5;
		actor->x.fixed -= toFIXED(4) / 5;
	}
}

/* Growth on the sprout in the Free-Climb Area, the fourth stage (metatile (49,
   12)), grows it into a vine, a shortcut that saves the racer time
   (FLAG_PLANT_GROWN). */
static void scColosseumNavampaMatchGrowPlant(void)
{
	gmFlagSet(FLAG_PLANT_GROWN);
	/* Layer 2's wall at (49, 61..64) opens, as the column at (47, 61..64)
	   is. */
	fdMapGridCopyMetatileAttributes(47, 61, 1, 4, 49, 61);
}

/* Moves a scale ledge vertically to a height in whole pixels, at the shared
   speed and acceleration. */
static inline void scColosseumNavampaMatchMoveOnScales(
	ACTOR *ledge, Sint32 height)
{
	ledge->acceleration = toFIXED(2) / 5;
	ledge->speedLimit = toFIXED(4) / 5;
	syFieldActorSetMoveTargetFX(
		ledge, ledge->x.fixed, toFIXED(height), ledge->z.fixed);
}

/* Frost on the target on the Scales of Justice's eastern ledge freezes it. */
static void scColosseumNavampaMatchFreezeOnScales(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_PUDDLE);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);

	scColosseumNavampaMatchMoveOnScales(
		fdActorGet(ACTOR_SCALES_EAST_LEDGE), 32);

	scColosseumNavampaMatchMoveOnScales(fdActorGet(ACTOR_SCALES_WEST_LEDGE), 4);

	gmFlagSet(FLAG_SCALES_FROZEN);
	/* The frozen eastern ledge, (13, 12), and the western, (9, 12), become
	   squares to hop to, as (15, 12) and (14, 12) are. */
	fdMapGridCopyMetatileAttributes(15, 12, 1, 1, 13, 12);
	fdMapGridCopyMetatileAttributes(14, 12, 1, 1, 9, 12);
}

/* Stepping onto the Scales of Justice's western ledge sinks it, unless the
   scales are frozen. */
static void scColosseumNavampaMatchStepOnScales(void)
{
	Sint32 actorId;
	Sint32 lowered;
	ACTOR *log;
	ACTOR *rider;

	actorId = MainRoutineStateWork.leaderActorId;
	lowered = gmFlagGet(FLAG_SCALES_FROZEN);
	if(lowered == 0) {
		log = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
		if(log != NULL) {
			fdActorSetDestination(
				actorId, log->x.part[0].integer, log->z.part[0].integer);
		}
		fdActorWaitForMoveById(actorId);
		log = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
		log->motionFlags = lowered;
		scColosseumNavampaMatchMoveOnScales(log, 32);
		log = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
		log->motionFlags = lowered;
		scColosseumNavampaMatchMoveOnScales(log, 4);
		rider = fdActorGet(actorId);
		rider->motionFlags = lowered;
		scColosseumNavampaMatchMoveOnScales(rider, 4);
		fdActorSetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
		fdActorWaitForMoveById(actorId);
		fdMapGridCopyMetatileAttributes(
			SCALES_LEDGE_SUNK_COLUMN, SCALES_LEDGE_SUNK_ROW, 1, 1, 9, 12);
		sySynch(2);
		fdActorSetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
		rider->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		rider->groundHeight = rider->y.fixed;
		gmFlagSet(FLAG_ON_SCALES);
	}
}

/* Stepping off the Scales of Justice's western ledge swings the scales back. */
static void scColosseumNavampaMatchStepOffScales(void)
{
	ACTOR *actor;

	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);

	actor = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
	actor->motionFlags = 0;
	scColosseumNavampaMatchMoveOnScales(actor, 4);

	actor = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
	actor->motionFlags = 0;
	scColosseumNavampaMatchMoveOnScales(actor, 32);

	fdActorWaitForMoveById(ACTOR_SCALES_WEST_LEDGE);
	fdMapGridCopyMetatileAttributes(
		SCALES_LEDGE_LEVEL_COLUMN, SCALES_LEDGE_LEVEL_ROW, 1, 1, 9, 12);
	sySynch(2);
	gmFlagClear(FLAG_ON_SCALES);
}

/* Move on the weight by the Scales of Justice can set it on the scales. */
static void scColosseumNavampaMatchSetWeightOnScales(void)
{
	ACTOR *probe;
	ACTOR *log;
	ACTOR *upper;
	Sint32 x;
	Sint32 z;

	probe = fdActorGet(ACTOR_SCALES_WEIGHT);
	x = FIELD_METATILE(probe->x.fixed);
	if(x == 9) {
		z = FIELD_METATILE(probe->z.fixed);
		if(z == 12) {
			log = fdActorGet(ACTOR_SCALES_WEIGHT);
			fdActorSetSpriteFlags(log, 0);
			log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			log->motionFlags = 0;
			scColosseumNavampaMatchMoveOnScales(log, 4);
			upper = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
			upper->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			scColosseumNavampaMatchMoveOnScales(upper, 32);
			scColosseumNavampaMatchMoveOnScales(
				fdActorGet(ACTOR_SCALES_WEST_LEDGE), 4);
			gmFlagSet(FLAG_WEIGHT_ON_SCALES);
			fdMapGridCopyMetatileAttributes(15, 12, 1, 1, 13, z);
			fdMapGridCopyMetatileAttributes(SCALES_LEDGE_WEIGHTED_COLUMN,
				SCALES_LEDGE_WEIGHTED_ROW, 1, 1, x, z);
		}
	}
}

/* Walking into the weight by the Scales of Justice pushes it, perhaps onto the
   scales. */
static void scColosseumNavampaMatchPushWeight(void)
{
	scColossoPushActorAhead();
	scColosseumNavampaMatchSetWeightOnScales();
}

/* Move on the stone pillar of the Stepping-Stone Stage, the second stage,
   slides it along its row; pushed east, it gives the racer a step across. The
   same work follows every push.

   The pillar's column is kept in FLAG_STONE_COLUMN, so that a friend's Move
   during Lend a Hand still holds for the race. Row 11 of the stage is made
   plain again, and the pillar's own metatile is given the top of a pillar. */
static void scColosseumNavampaMatchMoveStone(void)
{
	Sint32 x;

	x = FIELD_METATILE(fdActorGet(ACTOR_STONE)->x.fixed);
	syFieldGameFlagSetByte(FLAG_STONE_COLUMN, x);
	/* Row 11 takes the plain floor of (18..20, 10), and the pillar's metatile
	   the square to hop to at (17, 11). */
	fdMapGridCopyMetatileAttributes(18, 10, 3, 1, 18, 11);
	fdMapGridCopyMetatileAttributes(17, 11, 1, 1, x, 11);
}

/* Move on the stone pillar by the Wall Wedge's operator, east of the pistons,
   just slides it; nothing else happens. */
static void scColosseumNavampaMatchJustMoveRock(void)
{
}

/* Walking into the stone pillar by the Wall Wedge's operator, from the row it
   stands in (metatiles (43..45, 11)), pushes it a metatile. */
static void scColosseumNavampaMatchPushRock(void)
{
	scColossoPushActorAhead();
}

/* Move on one of the three stone pillars of the last stage, the log-rolling
   area, slides it a metatile. A pillar that comes down off the ledge into the
   tank stays there, drawn under the party, as a square to hop across, and the
   long log can no longer roll over that metatile.

   Each pillar that has come down is remembered by its flag
   (FLAG_FIRST_LOG_AREA_STONE_SETTLED and the two after it), and its metatile is
   marked in both copies of the map, the second 52 rows further down. */
static void scColosseumNavampaMatchUpdateLogAreaStoneMetatiles(void)
{
	ACTOR *actor;
	Sint32 slot;
	Sint32 x;
	Sint32 z;
	Sint32 x2;
	Sint32 z2;

	for(slot = ACTOR_LOG_AREA_STONE_A; slot <= ACTOR_LOG_AREA_STONE_C; slot++) {
		actor = fdActorGet(slot);
		if(fdMapGridGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) == 0) {
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = 0;
			x = FIELD_METATILE(actor->x.fixed);
			z = FIELD_METATILE(actor->z.fixed);
			/* The metatile takes the behaviour of the area's east end, (83,
			   13). */
			fdMapGridCopyMetatileAttributes(83, 13, 1, 1, x, z);
			x2 = FIELD_METATILE(actor->x.fixed);
			z2 = FIELD_METATILE(actor->z.fixed);
			fdMapGridCopyMetatileAttributes(83, 13, 1, 1, x2, z2 + 52);
			gmFlagSet(slot - ACTOR_LOG_AREA_STONE_A +
				FLAG_FIRST_LOG_AREA_STONE_SETTLED);
		}
	}
}

/* An event for Colosseum Final 3's eastern end. */
static void scColosseumNavampaMatchUpdateEastEndCollision(void)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 z;
	Sint32 behavior;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	x = FIELD_METATILE(actor->x.fixed);
	behavior = METATILE_BEHAVIOR_LONG_LOG;
	z = FIELD_METATILE(actor->z.fixed);
	if(x == 81 && z == 12) {
		if(FACING_EIGHTH(actor->facing) == FACING_SOUTH) {
			behavior = METATILE_BEHAVIOR_CLIMB;
		}
		fdMapGridSetMetatileBehaviorAt(
			0, FIELD_METATILE_ORIGIN(x), FIELD_METATILE_ORIGIN(z), behavior);
	}
}

/* Rejects blocked metatiles and metatiles occupied by one of the log-rolling
   area's three stones. */
static Sint32 scColosseumNavampaMatchCheckLongLogMetatile(Sint32 x, Sint32 z)
{
	ACTOR *actor;

	if(fdMapGridGetMetatileBehaviorAt(0, x, z) == METATILE_BEHAVIOR_WALL) {
		return -2;
	}
	actor = fdActorGet(ACTOR_LOG_AREA_STONE_A);
	x = FIELD_METATILE(x);
	z = FIELD_METATILE(z);
	if(FIELD_METATILE(actor->x.fixed) == x &&
		FIELD_METATILE(actor->z.fixed) == z) {
		return -1;
	}
	actor = fdActorGet(ACTOR_LOG_AREA_STONE_B);
	if(FIELD_METATILE(actor->x.fixed) == x &&
		FIELD_METATILE(actor->z.fixed) == z) {
		return -1;
	}
	actor = fdActorGet(ACTOR_LOG_AREA_STONE_C);
	if(FIELD_METATILE(actor->x.fixed) == x &&
		FIELD_METATILE(actor->z.fixed) == z) {
		return -1;
	}
	return 0;
}

/* Checks all four metatiles covered by the long rolling log. */
static Sint32 scColosseumNavampaMatchCheckLongLogPath(Sint32 x, Sint32 z)
{
	if(scColosseumNavampaMatchCheckLongLogMetatile(x, z - toFIXED(24)) != 0 ||
		scColosseumNavampaMatchCheckLongLogMetatile(x, z - toFIXED(8)) != 0 ||
		scColosseumNavampaMatchCheckLongLogMetatile(x, z + toFIXED(8)) != 0 ||
		scColosseumNavampaMatchCheckLongLogMetatile(x, z + toFIXED(24)) != 0) {
		return -1;
	}
	return 0;
}

/* The long log carries the leader until something stops it. */
static void scColosseumNavampaMatchRollLongLog(void)
{
	ACTOR		*rider;
	ACTOR		*log;
	FIELD_STATE *state;
	Sint32		 angle;
	Sint32		 actorId;
	FIXED		 x;
	FIXED		 z;
	Sint32		 steps;
	FIXED		 nextX;
	Sint32		 column;
	FIXED		 acceleration;
	Sint32		 rollAnimation;
	Sint32		 step;
	Sint32		 ahead[3];

	rider = fdActorGet(MainRoutineStateWork.leaderActorId);
	log = fdActorGet(ACTOR_LONG_LOG);
	steps = 0;
	angle =
		ColosseumNavampaMatchFacingAngles[FACING_EIGHTH_INDEX(rider->facing)];
	actorId = MainRoutineStateWork.leaderActorId;
	SET_VECTOR(ahead, (rider->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
		rider->y.fixed, (rider->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
	syVectorAddPolarOffset(toFIXED(16), angle, ahead);
	x = log->x.fixed;
	z = log->z.fixed;
	/* Only a nearby rider rolls the log; other interactions push the actor
	   ahead. */
	if((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > toFIXED(8) ||
		(ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > toFIXED(32)) {
		goto far;
	}
	if(PerData & KEY_LEFT) {
		rollAnimation = LONG_LOG_ANIMATION_ROLL_WEST;
		step = -8;
		for(;;) {
			nextX = x - toFIXED(16);
			if(scColosseumNavampaMatchCheckLongLogPath(nextX, z) == 0) {
				steps++;
				x = nextX;
			} else {
				break;
			}
		}
	} else {
		if(!(PerData & KEY_RIGHT)) {
			return;
		}
		rollAnimation = LOG_ANIMATION_ROLL_WEST;
		step = 8;
		for(;;) {
			nextX = x + toFIXED(16);
			if(scColosseumNavampaMatchCheckLongLogPath(nextX, z) == 0) {
				steps++;
				x = nextX;
			} else {
				break;
			}
		}
	}
	if(steps == 0) {
		return;
	}
	/* The log's column takes the lane's floor again, (74, 8..11), and layer 2's
	   lane is restored from its store. */
	fdMapGridCopyMetatileAttributes(
		74, 8, 1, 4, FIELD_METATILE(log->x.fixed), 9);
	fdMapGridCopyMetatileAttributes(
		LOG_LANE_STORE_COLUMN, LOG_LANE_STORE_ROW, 8, 5, 74, 60);
	fdScriptStart();
	fdActorSetAnimation(actorId, ANIM_PUSH);
	fdScriptWait(6);
	log->speedLimit = toFIXED(1) / 2;
	acceleration = toFIXED(1) / 5;
	log->acceleration = acceleration;
	fdActorSelectAnimation(log, rollAnimation);
	syFieldActorSetMoveTargetFX(log, x, 0, z);
	fdScriptWait(6);
	fdActorSetAnimation(actorId, ANIM_WALK);
	state = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	fdActorStartCamera(state->camera, (Sint32)log);
	fdActorSetSpeedFX(actorId, toFIXED(1) / 2, acceleration);
	sySoundPlay(SE_HEAVY_MOVE);
	fdActorSelectAnimation(rider, ANIM_WALK);
	syFieldActorSetMoveTargetFX(
		rider, toFIXED(steps * step) + rider->x.fixed, 0, rider->z.fixed);
	fdActorWaitForMove(rider);
	fdActorSelectAnimation(rider, ANIM_STAND);
	fdActorWaitForMove(log);
	/* At the eastern end the log settles at the end of its stage. */
	if(x >= toFIXED(1328)) {
		gmFlagSet(FLAG_LONG_LOG_IN_PLACE);
		fdActorSetAnimation(ACTOR_LONG_LOG, LOG_ANIMATION_ROLL_WEST);
		fdActorSetDestinationOffset(ACTOR_LONG_LOG, 18, 6);
		fdScriptWait(30);
		fdActorSelectAnimation(log, LOG_ANIMATION_DROP_IN);
		fdActorWaitForMove(log);
		log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		/* Fallen in, its metatiles (84, 10..12) are squares to hop across, as
		   (86, 9..11) are. */
		column = 84;
		fdMapGridCopyMetatileAttributes(86, 10, 1, 2, column, 10);
		fdMapGridCopyMetatileAttributes(86, 9, 1, 1, column, 12);
		sySoundPlay(SE_STOP_ACTION_SE);
		sySoundPlay(SE_LOG_FALL);
	} else {
		fdActorSelectAnimation(log, LOG_ANIMATION_AT_REST);
		sySoundPlay(SE_STOP_ACTION_SE);
		sySoundPlay(SE_HEAVY_STOP);
		/* Its new column, on the ground and on layer 2, is the wall of (85,
		   9..12). */
		column = FIELD_METATILE(x);
		fdMapGridCopyMetatileAttributes(85, 9, 1, 4, column, 9);
		fdMapGridCopyMetatileAttributes(85, 9, 1, 4, column, 61);
	}
	fdScriptWait(15);
	fdScriptEnd();
	return;
far:
	scColossoPushActorAhead();
	scColosseumNavampaMatchUpdateLogAreaStoneMetatiles();
}

/* Displays the activated moving sidewalk and plays its arrival sounds. */
static Sint32 scColosseumNavampaMatchShowActivatedSidewalkLayer(
	ACTOR *interpreter)
{
	fdTilesetAnimationPause(SIDEWALK_RUNNING_ANIMATION);
	fdTilesetAnimationResume(SIDEWALK_STOPPED_ANIMATION);
	sySoundPlay(SE_STOP_ACTION_SE);
	sySoundPlay(SE_FLOOR_SWITCH);
	return 0;
}

/* Force on the Moving Sidewalk Stage's log makes it slide away. */
static void scColosseumNavampaMatchForceSidewalkLog(void)
{
	ACTOR *effect;
	FIXED  moveRate;

	effect = fdActorGet(ACTOR_SIDEWALK_LOG);
	effect->motionFlags = 0;
	moveRate = toFIXED(8) / 5;
	effect->acceleration = moveRate;
	effect->speedLimit = moveRate;
	fdActorSelectAnimation(effect, STANDING_LOG_ANIMATION_TOPPLE);
	fdActorStartScript(effect, ColosseumNavampaMatchSidewalkLogScript);
	gmFlagSet(FLAG_SIDEWALK_LOG_KNOCKED_OFF);
}

/* Halt on the Wall Wedge's operator stops the pistons at their next open step
   (FLAG_WALL_WEDGE_HALTED). */
static void scColosseumNavampaMatchHaltWall(void)
{
	fdPsynergyHaltActor(ACTOR_WALL_OPERATOR);
	gmFlagSet(FLAG_WALL_WEDGE_HALTED);
	sySynch(10);
	while(ColosseumNavampaMatchWallWedgeStep != 1 &&
		ColosseumNavampaMatchWallWedgeStep != 3)
		sySynch(1);
	sySynch(1);
	syCloseEvent(scColosseumNavampaMatchUpdateWallWedge);
}

/* A piston of the Wall Wedge, the third stage, punching halfway out while Isaac
   stands before it on the walkway's north row (row 11) shoves him a metatile
   and a half south.

   scColosseumNavampaMatchUpdateWallWedge raises this script with the piston's
   animation number, PISTON_ANIMATION_PUSH_HALF_OUT. */
static void scColosseumNavampaMatchShovedByPiston(void)
{
	ACTOR *actor;
	Sint32 z;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor->acceleration = toFIXED(1);
	actor->speedLimit = toFIXED(2);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
	z = actor->z.fixed & FIELD_METATILE_MASK;
	syFieldActorSetMoveTargetFX(
		actor, actor->x.fixed, actor->y.fixed, z + toFIXED(24));
	fdActorWaitForMove(actor);
}

/* A piston of the Wall Wedge punching all the way out while Isaac stands before
   it on the walkway's south row (row 12) shoves him off the walkway, and he
   drops down the ledge below.

   scColosseumNavampaMatchUpdateWallWedge raises this script with the piston's
   animation number, PISTON_ANIMATION_PUSH_OUT. */
static void scColosseumNavampaMatchShovedOffWalkway(void)
{
	ACTOR *actor;
	Sint32 z;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(actor->x.fixed > toFIXED(664)) {
		actor->x.fixed = toFIXED(664);
	}
	actor->acceleration = toFIXED(1);
	actor->speedLimit = toFIXED(2);
	fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
	z = actor->z.fixed & FIELD_METATILE_MASK;
	syFieldActorSetMoveTargetFX(
		actor, actor->x.fixed, actor->y.fixed, z + toFIXED(12));
	fdActorWaitForMove(actor);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldActorJumpDownLedge(MainRoutineStateWork.leaderActorId, 6, 0);
}

/* Unused: an empty function nothing refers to. */
UNUSED static void scColosseumNavampaMatchUnusedNoOperation(void)
{
}

/* XXX (no evidence): which lines this question was written for; in the Japanese
   catalog, as in the English one, the lines three apart per character from
   MsgColossoDidntThinkBattles are a spectator's thought and the stage
   announcements, and no catalog older than the released ones survives. */
/* Unused: talking to a character at the third finals site would have handed the
   player control of that character.

   Nothing calls it. Arriving at COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH it
   would ask a yes or no question and, on yes, make the character it names the
   one the player moves alone, as the friends' turns do. */
UNUSED static void scColosseumNavampaMatchUnusedAskToControlCharacter(
	Sint32 characterId)
{
	FIELD_STATE *state;
	Sint32		 actorId;
	Sint32		 msgOffset;
	Sint32		 entrance;

	state = FieldStateWork;
	actorId = MainRoutineStateWork.leaderActorId;
	entrance = MainRoutineStateWork.entrance;
	if(entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		fdScriptStart();
		msgOffset = characterId * 3;
		fdScriptSetMessageId(msgOffset + MsgColossoDidntThinkBattles);
		fdScriptOpenMessage(characterId, 0);
		if(fdScriptAskYesNoWithActorAnimation(actorId, 0) == 0) {
			fdScriptSetMessageId(msgOffset + MsgColossoDidntThinkBattles + 1);
			fdScriptShowMessage(characterId, 0);
			state->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
			state->transitionFrame = 15;
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionTicks();
			scColossoControlCharacterAlone(characterId);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
		} else {
			fdScriptSetMessageId(msgOffset + MsgColossoDidntThinkBattles + 2);
			fdScriptShowMessage(characterId, 0);
		}
		fdScriptEnd();
	}
}

/* Reaching Colosseum Final 3's arena, the leader takes a prize and fights
   Navampa. */
static void scColosseumNavampaMatchReachArena(void)
{
	Sint32 opponentFirst;
	Sint32 i;
	Sint32 opponentFirstSlot = 4;
	scColossoNoOperation();
	fdScriptStart();
	opponentFirst = scColossoTakePrize(ITEM_CLAYMORE, ITEM_HUNTERS_SWORD);
	scColossoWaitForOpponentToFinish();
	/* Up to ten waits of 300 ticks while the opponent holds up his prize. */
	for(i = 0; i < 10; i++) {
		fdActorWaitForScriptEndById(ACTOR_OPPONENT);
	}
	fdActorSetSpeed(ACTOR_OPPONENT, 1);
	fdActorWalkTo(ACTOR_OPPONENT, 1528, 192);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 1496, 192);
	fdActorSetAnimation(ACTOR_OPPONENT, ANIM_STAND);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_OPPONENT, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorSetSpeed(ACTOR_OPPONENT, 2);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 1504, 192);
	fdActorWalkToAndWait(ACTOR_OPPONENT, 1520, 192);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
	fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(10);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_COLOSSO, opponentFirstSlot - opponentFirst + 1);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(
		SCENE_COLOSSEUM_NAVAMPA_MATCH, COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON);
	fdScriptSetDefeatDestination(SCENE_COLOSSEUM_NAVAMPA_MATCH,
		COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
}

/* Walking on from the east end of the log-rolling area (metatile (83, 13)) hops
   Isaac forward across the gap, if no stone pillar stands in the way and there
   is somewhere to land. */
static void scColosseumNavampaMatchHopFromLogAreaEnd(void)
{
	if(scColossoFindActorAhead() == 0) {
		fdTryJumpForward();
	}
}

/* Walking on from the east edge of the weight's square by the Scales of Justice
   (metatile (8, 12)) pushes the weight if it stands ahead, perhaps onto the
   scales. With nothing there, Isaac hops forward across the gap where there is
   somewhere to land. */
static void scColosseumNavampaMatchPushWeightOrHopAcross(void)
{
	if(scColossoFindActorAhead() == 0) {
		fdTryJumpForward();
	} else {
		scColosseumNavampaMatchPushWeight();
	}
}

const CUE *scGetCues(void)
{
	return ColosseumNavampaMatchCues;
}

/* Sets up Colosseum Final 3 for the visit its entrance stands for. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 i;
	FIXED  height;
	Sint32 column;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	sySetTickEvent(
		scColosseumNavampaMatchUpdateEastEndCollision, EVENT_PASS_TICK);
	fdMapGridCopyMetatileAttributes(
		74, 60, 8, 6, LOG_LANE_STORE_COLUMN, LOG_LANE_STORE_ROW);

	fdActorSetSpriteFlags(fdActorGet(ACTOR_PUDDLE), 0);

	actor = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
	fdActorSetSpriteFlags(actor, 0);
	actor->motionFlags = 0;
	actor->y.fixed = toFIXED(32);

	actor = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
	fdActorSetSpriteFlags(actor, 0);
	actor->motionFlags = 0;
	actor->y.fixed = toFIXED(4);

	if(gmFlagGet(FLAG_SCALES_FROZEN) != 0) {
		fdActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
		actor = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
		actor->y.fixed = toFIXED(4);
		actor = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
		actor->y.fixed = toFIXED(32);
		fdMapGridCopyMetatileAttributes(15, 12, 1, 1, 13, 12);
	} else {
		actor = fdActorGet(ACTOR_PUDDLE);
		actor->scaleX = toFIXED(3) / 2;
		actor->scaleY = toFIXED(3) / 2;
		/* The metatiles scColosseumNavampaMatchStepOnScales copies as it sets
		   the flag, redrawn on arrival. */
		if(gmFlagGet(FLAG_ON_SCALES) != 0) {
			fdMapGridCopyMetatileAttributes(
				SCALES_LEDGE_SUNK_COLUMN, SCALES_LEDGE_SUNK_ROW, 1, 1, 9, 12);
		} else {
			fdMapGridCopyMetatileAttributes(
				SCALES_LEDGE_LEVEL_COLUMN, SCALES_LEDGE_LEVEL_ROW, 1, 1, 9, 12);
		}
	}

	/* The Scales of Justice are two pans on poles. With the weight, a rock, on
	   the western ledge they have tipped: the west pan hangs low and the east
	   one high, where otherwise the west is high and the east low. The two
	   metatiles under the pans take the ground that goes with it. */
	if(gmFlagGet(FLAG_WEIGHT_ON_SCALES) != 0) {
		fdMapGridCopyMetatileAttributes(15, 12, 1, 1, 13, 12);
		fdMapGridCopyMetatileAttributes(SCALES_LEDGE_WEIGHTED_COLUMN,
			SCALES_LEDGE_WEIGHTED_ROW, 1, 1, 9, 12);
		actor = fdActorGet(ACTOR_SCALES_WEIGHT);
		fdActorSetSpriteFlags(actor, 0);
		actor->motionFlags = 0;
		actor->y.fixed = toFIXED(2);
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
		actor->y.fixed = toFIXED(4);
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
		actor->y.fixed = toFIXED(32);
	}

	column = syFieldGameFlagGetByte(FLAG_STONE_COLUMN);
	if(column == 0) {
		column = 19;
	}
	actor = fdActorGet(ACTOR_STONE);
	actor->x.fixed = FIELD_METATILE_CENTER(column);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(18, 10, 3, 1, 18, 11);
	fdMapGridCopyMetatileAttributes(17, 11, 1, 1, column, 11);

	for(i = ACTOR_LOG_AREA_STONE_A; i <= ACTOR_LOG_AREA_STONE_C; i++) {
		actor = fdActorGet(i);
		height = fdMapGridGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		if(actor->y.fixed == 0 && height == 0) {
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = height;
			fdMapGridCopyMetatileAttributes(83, 13, 1, 1,
				FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
			fdMapGridCopyMetatileAttributes(83, 13, 1, 1,
				FIELD_METATILE(actor->x.fixed),
				(FIELD_METATILE(actor->z.fixed)) + 52);
		}
	}

	/* Halt preserves a stationary wall; otherwise restart its cycle. */
	if(gmFlagGet(FLAG_WALL_WEDGE_HALTED) != 0) {
		for(i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
			actor = fdActorGet(i);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			fdActorSelectAnimation(actor, PISTON_ANIMATION_HALF_OUT);
			actor = fdActorGet(i + PISTON_COUNT);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = 0;
			actor->y.fixed = toFIXED(32);
			fdActorSelectAnimation(
				actor, PISTON_ANIMATION_HALF_OUT + PISTON_ANIMATION_TIP);
			fdMapGridCopyMetatileAttributes(74, 12, 1, 1, i * 2 - 3, 11);
		}
		fdActorSetAnimation(
			ACTOR_WALL_OPERATOR, WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT);
		fdPsynergyHaltActor(ACTOR_WALL_OPERATOR);
	} else {
		for(i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
			actor = fdActorGet(i);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor = fdActorGet(i + PISTON_COUNT);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = 0;
			actor->y.fixed = toFIXED(32);
		}
		sySetTickEvent(scColosseumNavampaMatchUpdateWallWedge, EVENT_PASS_TICK);
	}

	if(gmFlagGet(FLAG_PLANT_GROWN) != 0) {
		fdActorSetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
		fdMapGridCopyMetatileAttributes(47, 61, 1, 4, 49, 61);
	}

	if(gmFlagGet(FLAG_SIDEWALK_LOG_KNOCKED_OFF) != 0) {
		fdTilesetAnimationPause(SIDEWALK_RUNNING_ANIMATION);
		actor = fdActorGet(ACTOR_SIDEWALK_LOG);
		actor->motionFlags = 0;
		actor->x.fixed = toFIXED(1130);
		actor->z.fixed = toFIXED(184);
		fdActorSetSpriteFlags(actor, 0);
		fdActorSelectAnimation(actor, STANDING_LOG_ANIMATION_FALLEN);
		fdActorStartScript(actor, ColosseumNavampaMatchSidewalkLogOffBelt);
	} else {
		fdTilesetAnimationPause(SIDEWALK_STOPPED_ANIMATION);
	}

	if(gmFlagGet(FLAG_LONG_LOG_IN_PLACE) != 0) {
		actor = fdActorGet(ACTOR_LONG_LOG);
		fdActorSelectAnimation(actor, LOG_ANIMATION_DROP_IN);
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(86, 10, 1, 2, 84, 10);
		fdMapGridCopyMetatileAttributes(86, 9, 1, 1, 84, 12);
	} else {
		actor = fdActorGet(ACTOR_LONG_LOG);
		fdMapGridCopyMetatileAttributes(
			85, 9, 1, 4, FIELD_METATILE(actor->x.fixed), 9);
		fdMapGridCopyMetatileAttributes(
			85, 9, 1, 4, FIELD_METATILE(actor->x.fixed), 61);
	}

	actor = fdActorGet(ACTOR_PUDDLE);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	actor = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	actor = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	scColossoShowItemIcon(ACTOR_PRIZE_A, ITEM_CLAYMORE);
	scColossoShowItemIcon(ACTOR_PRIZE_B, ITEM_HUNTERS_SWORD);
	fdActorSetSpriteColorById(ACTOR_OPPONENT, SPRITE_COLOR_RBG);

	/* Entrances: the race, the friends' turns, the tour, and the end of Colosso
	   after a win or a loss. */
	switch(MainRoutineStateWork.entrance) {
	case COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND:
		scColossoStartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 6,
			toFIXED(1512), toFIXED(192), ACTOR_PRIZE_A, ACTOR_PRIZE_B);
		/* The doorway Isaac sets out from, (5, 2..3), takes the two metatiles
		   kept at (127, 0..1) for the race. */
		fdMapGridCopyMetatilesLayered(127, 0, 1, 2, 5, 2);
		fdActorReleaseById(ACTOR_GUIDE);
		fdActorReleaseById(ACTOR_SCALES_ATTENDANT);
		fdActorReleaseById(ACTOR_STEPPING_STONE_ATTENDANT);
		fdActorReleaseById(ACTOR_WALL_WEDGE_ATTENDANT);
		fdActorReleaseById(ACTOR_FREE_CLIMB_ATTENDANT);
		fdActorReleaseById(ACTOR_MOVING_SIDEWALK_ATTENDANT);
		fdActorReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			sySoundPlay(SOUND_FADE_OUT_BGM);
			scColossoControlCharacterAlone(ISAAC);
			scColossoPlaceFriendsAtCheeringSpots();
			fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
			scColossoShowOpeningLettering(FINALS_MATCH);
		}
		fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
		fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
		fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
		scColossoStartOpponent(RESOURCE_NAVAMPA_OPPONENT_PATH);
		break;

	case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH:
		sySetTickEvent(scColossoRaiseTakePlaceOnPsynergy, EVENT_PASS_TICK);
		fdActorReleaseById(ACTOR_PRIZE_A);
		fdActorReleaseById(ACTOR_PRIZE_B);
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			break;
		}
		scColossoPlaceFriendsAtCheeringSpots();
		scColossoControlCharacterAlone(GARET);
		scColossoShowOpeningLettering(LETTERING_LEND_A_HAND);
		break;

	case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
			break;
		}
		scColosseumNavampaMatchIntroduceThirdFinals(ACTOR_GUIDE);
		scColossoClearCheeringSpots();
		break;

	case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON:
		scColosseumNavampaMatchPlayPostBattleReunion(1);
		syImportScriptSetExitRequest(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE);
		gmFlagSet(FLAG_COLOSSO_OVER);
		gmFlagSet(FLAG_WON_COLOSSO);
		break;

	case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST:
		scColosseumNavampaMatchPlayPostBattleReunion(-1);
		syImportScriptSetExitRequest(
			COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST);
		gmFlagSet(FLAG_COLOSSO_OVER);
		break;
	}
	return 0;
}

/* Advances one of the particles that rise from the sleeping Isaac after the
   last match. */
static Sint32 scColosseumNavampaMatchAdvanceSleepZ(ACTOR *particle)
{
	particle->x.fixed +=
		particle->timer * (toFIXED(1) / SLEEP_Z_STEPS_PER_PIXEL);
	particle->y.fixed +=
		particle->userValue * (toFIXED(1) / SLEEP_Z_STEPS_PER_PIXEL);
	particle->scaleX += toFIXED(1) / 40;
	particle->scaleY += toFIXED(1) / 40;
	particle->timer += 5;
	particle->userValue--;
	return 0;
}

/* Shows one of the three particles that rise from the sleeping Isaac, each at
   its point in a three-second cycle. */
static void scColosseumNavampaMatchSpawnSleepZ(void)
{
	ACTOR *particle;
	ACTOR *source;
	FIXED  x;
	FIXED  y;
	Sint32 actorId;
	Sint32 tick;

	particle = fdActorGet(ACTOR_PARTY_LEADER);
	tick = ColosseumNavampaMatchSleepTick + 1;
	actorId = ACTOR_FIRST_SLEEP_Z;
	x = particle->x.fixed;
	y = particle->y.fixed;
	ColosseumNavampaMatchSleepTick = tick;
	switch(tick % 180) {
	case 10:
		break;
	case 20:
		actorId = ACTOR_FIRST_SLEEP_Z + 1;
		break;
	case 30:
		actorId = ACTOR_FIRST_SLEEP_Z + 2;
		break;
	default:
		return;
	}
	particle = fdActorGet(actorId);
	if(particle == NULL) {
		return;
	}
	source = fdActorGet(ACTOR_PARTY_LEADER);
	if(source != NULL) {
		fdActorSetPosFX(actorId, source->x.fixed, source->z.fixed);
	}
	fdActorSetSpriteFlags(fdActorGet(actorId), 0);
	particle->motionFlags = 0;
	particle->scaleX = toFIXED(2) / 5;
	particle->scaleY = toFIXED(2) / 5;
	{
		FIXED height = toFIXED(4);
		particle->x.fixed = x + height;
		height += y;
		particle->y.fixed = height;
		particle->targetY = height;
	}
	/* It drifts east ever faster and rises ever slower, from half a pixel a
	   tick. */
	particle->timer = 25;
	particle->userValue = SLEEP_Z_STEPS_PER_PIXEL / 2;
	fdActorSetScript(actorId, ColosseumNavampaMatchSleepZScript);
}

/* After the last match, Garet, Ivan and Mia run to Isaac in the arena, but he
   falls asleep. */
static void scColosseumNavampaMatchPlayPostBattleReunion(Sint32 battleResult)
{
	const Sint32 *leaderScaleScript;
	EVENT_FUNC	  sleepZTask;

	fdActorReleaseById(ACTOR_PRIZE_A);
	fdActorReleaseById(ACTOR_PRIZE_B);
	gmPartyRestoreHpPp(1);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdScriptStart();
	fdActorSetPos(ACTOR_OPPONENT, 1544, 192);
	if(battleResult < 0) {
		fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
	} else {
		fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
	}
	fdActorSetScript(ACTOR_OPPONENT, ColosseumNavampaMatchOpponentAfterBattle);
	fdActorSetPos(ACTOR_PARTY_LEADER, 1504, 192);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_EAST;
	fdActorSetScript(ACTOR_PARTY_LEADER, ColosseumNavampaMatchLeaderActionA);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetPos(ACTOR_GARET, 1464, 184);
	fdActorSetPos(ACTOR_IVAN, 1464, 200);
	fdActorSetPos(ACTOR_MIA, 1448, 192);
	fdActorGet(ACTOR_GARET)->facing = FACING_EAST;
	fdActorGet(ACTOR_IVAN)->facing = FACING_EAST;
	fdActorGet(ACTOR_MIA)->facing = FACING_EAST;
	sySynch(1);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptSetMessageId(MsgColossoIsaac);
	fdScriptWait(60);
	leaderScaleScript = ColosseumNavampaMatchLeaderScaleScript;
	fdActorSetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
	fdActorGet(ACTOR_PARTY_LEADER)->scaleX = toFIXED(1);
	fdActorGet(ACTOR_PARTY_LEADER)->scaleY = toFIXED(1);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_LEADER_FALL);
	fdActorGet(ACTOR_PARTY_LEADER)->x.fixed += toFIXED(3);
	fdScriptWait(10);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_PARTY_LEADER, ColosseumNavampaMatchLeaderActionB);
	/* Let each companion react before the leader settles back to sleep.
	   "Isaac!!!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_GARET, 1504, 176);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_IVAN, 1488, 176);
	fdActorWalkTo(ACTOR_GARET, 1520, 184);
	fdActorWalkToAndWait(ACTOR_IVAN, 1504, 176);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(10);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(30);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
	fdActorWalkToAndWait(ACTOR_MIA, 1488, 184);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(10);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
	fdScriptWait(20);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorSetScript(ACTOR_PARTY_LEADER, ColosseumNavampaMatchLeaderActionC);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
	fdScriptWait(20);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
	fdScriptWait(20);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
	fdScriptWait(20);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorSetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
	fdScriptWait(60);
	sleepZTask = scColosseumNavampaMatchSpawnSleepZ;
	/* A Z rises from Isaac on the event's first tick (tick 10 of the
	   cycle). */
	ColosseumNavampaMatchSleepTick = 9;
	sySetTickEvent(sleepZTask, EVENT_PASS_TICK);
	fdScriptWait(5);
	syCloseEvent(sleepZTask);
	fdScriptWait(55);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	sySetTickEvent(sleepZTask, EVENT_PASS_TICK);
	fdScriptWait(20);
	syCloseEvent(sleepZTask);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	ColosseumNavampaMatchSleepTick = 9;
	sySetTickEvent(sleepZTask, EVENT_PASS_TICK);
	fdScriptWait(35);
	syCloseEvent(sleepZTask);
	fdScriptWait(25);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	ColosseumNavampaMatchSleepTick = 9;
	sySetTickEvent(sleepZTask, EVENT_PASS_TICK);
	fdScriptWait(35);
	syCloseEvent(sleepZTask);
	fdScriptWait(25);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
	fdScriptWait(60);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
	ColosseumNavampaMatchSleepTick = 9;
	sySetTickEvent(sleepZTask, EVENT_PASS_TICK);
	fdScriptWait(35);
	syCloseEvent(sleepZTask);
	fdScriptWait(25);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
	fdActorStartRepeatedMotion(ACTOR_GARET, 3);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 3);
	fdActorRunRepeatedMotion(ACTOR_MIA, 3);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
	ColosseumNavampaMatchSleepTick = 9;
	sySetTickEvent(sleepZTask, EVENT_PASS_TICK);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(60);
	/* The companions begin to leave, then discuss the sleeping leader
	   off-screen. */
	fdActorWalkTo(ACTOR_MIA, 1464, 200);
	fdScriptWait(5);
	fdActorWalkTo(ACTOR_IVAN, 1368, 184);
	fdScriptWait(3);
	fdActorWalkToAndWait(ACTOR_GARET, 1512, 184);
	fdActorWalkTo(ACTOR_GARET, 1368, 184);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
	fdScriptWait(60);
	fdActorWalkToAndWait(ACTOR_MIA, 1432, 200);
	fdActorWalkTo(ACTOR_MIA, 1368, 184);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(30);
	fdActorSetPos(ACTOR_GARET, 1512, 176);
	fdActorSetPos(ACTOR_IVAN, 1464, 192);
	fdActorSetPos(ACTOR_MIA, 1560, 200);
	fdActorSystemSuspendProjectionAndShowSprites();
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(1);
	fdScriptSetMessageId(MsgColossoIsaacFellAsleep);
	/* "Isaac fell asleep..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	/* "He must be exhausted." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	/* "Let's leave him to sleep for now." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(60);
	fdScriptEnd();
}

/* The guide gives the party the tour of Colosseum Final 3
   (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH). */
static void scColosseumNavampaMatchIntroduceThirdFinals(Sint32 guideId)
{
	Sint32 guideZ;
	Sint32 guideX;
	ACTOR *actor;

	actor = fdActorGet(guideId);
	guideX = actor->x.part[0].integer;
	guideZ = actor->z.part[0].integer;
	fdScriptStart();
	fdActorSetSpeed(guideId, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetPosFX(
		ACTOR_PARTY_LEADER, toFIXED(guideX), (toFIXED(guideZ) - toFIXED(48)));
	fdActorSetPosFX(ACTOR_GARET, toFIXED(guideX) - toFIXED(16),
		toFIXED(guideZ) - toFIXED(40));
	fdActorSetPosFX(ACTOR_IVAN, toFIXED(guideX) + toFIXED(16),
		toFIXED(guideZ) - toFIXED(40));
	fdActorSetPosFX(
		ACTOR_MIA, toFIXED(guideX), (toFIXED(guideZ) - toFIXED(32)));
	fdActorSetPosFX(guideId, toFIXED(guideX), (toFIXED(guideZ) - toFIXED(80)));
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptSetMessageId(MsgColossoSiteThirdFinals);
	fdActorSetAnimationAndWait(guideId, ANIM_NOD);
	/* "This is the site of the third finals match." */
	fdScriptShowMessage(guideId, 0);
	fdActorRunRepeatedMotion(guideId, 2);
	/* "By now, you know that each stage has its own attendant." */
	fdScriptShowMessage(guideId, 0);
	fdActorRunRepeatedMotion(guideId, 2);
	/* "Ask the attendants descriptions of the stages." */
	fdScriptShowMessage(guideId, 0);
	fdActorRunRepeatedMotion(guideId, 2);
	/* "Honestly, Isaac, I didn't think you'd make it this far! I'll..." */
	fdScriptShowMessage(guideId, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWalkToAndWait(guideId, guideX - 16, guideZ - 64);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorWalkToAndWait(guideId, guideX - 16, guideZ - 16);
	fdActorWalkToAndWait(guideId, guideX, guideZ);
	fdActorFaceDirection(guideId, FACING_NORTH, 10);
	fdScriptEnd();
}

/* The Scales of Justice's attendant explains stage 1 of Colosseum Final 3. */
static void scColosseumNavampaMatchTalkToScalesAttendant(Sint32 guideId)
{
	ACTOR *westPan;
	ACTOR *eastPan;
	ACTOR *westPanAgain;
	ACTOR *eastPanAgain;
	Sint32 promptResult;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		scColossoAskDoneCheering(guideId);
		return;
	}
	fdScriptStart();
	promptResult = scColossoGreetAtStage(guideId, STAGE_SCALES_OF_JUSTICE);
	if(promptResult == COLOSSO_GUIDE_DESCRIBE) {
		fdScriptSetMessageId(MsgColossoStageCalledScales);
		fdCameraSetSpeed(3);
		fdCameraMoveTo(152, CAMERA_KEEP, 200, 1);
		fdCameraWaitForMove();
		fdScriptWait(30);
		/* "This stage is called the Scales of Justice." */
		fdScriptShowMessage(guideId, 0);
		scColossoShowPointer(104, 68, 0);
		fdScriptWait(60);
		scColossoMovePointer(168, 96, 10);
		fdScriptWait(70);
		/* "There are two ledges that actually form a scale." */
		fdScriptShowMessage(guideId, 0);
		scColossoHidePointer();
		sySynch(2);
		westPan = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
		westPan->motionFlags = 0;
		scColosseumNavampaMatchMoveOnScales(westPan, 4);
		eastPan = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
		eastPan->motionFlags = 0;
		scColosseumNavampaMatchMoveOnScales(eastPan, 32);
		fdActorWaitForMove(eastPan);
		fdScriptWait(45);
		westPanAgain = fdActorGet(ACTOR_SCALES_WEST_LEDGE);
		westPanAgain->motionFlags = 0;
		scColosseumNavampaMatchMoveOnScales(westPanAgain, 32);
		eastPanAgain = fdActorGet(ACTOR_SCALES_EAST_LEDGE);
		eastPanAgain->motionFlags = 0;
		scColosseumNavampaMatchMoveOnScales(eastPanAgain, 4);
		fdActorWaitForMove(eastPanAgain);
		fdScriptWait(15);
		/* "When one ledge rises, the other falls." */
		fdScriptShowMessage(guideId, 0);
		scColossoShowPointer(104, 68, 0);
		fdScriptWait(30);
		scColossoMovePointer(168, 96, 10);
		fdScriptWait(40);
		scColossoMovePointer(104, 68, 10);
		fdScriptWait(70);
		/* "Use the scales to cross the gaps and race to the next stage." */
		fdScriptShowMessage(guideId, 0);
		scColossoHidePointer();
		sySynch(2);
		fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
		scColossoRepeatStageNumber(guideId, STAGE_SCALES_OF_JUSTICE);
	} else if(promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
		fdScriptSetMessageId(MsgColossoObjectiveGetAcross);
		/* "Here your objective is to get across the gaps as quickly as..." */
		fdScriptShowMessage(guideId, 0);
	}
	scColossoOfferCheeringSpot(promptResult, guideId, STAGE_SCALES_OF_JUSTICE);
	fdScriptEnd();
}

/* The Stepping-Stone Stage's attendant explains stage 2 of Colosseum Final
   3. */
static void scColosseumNavampaMatchTalkToSteppingStoneAttendant(Sint32 guideId)
{
	Sint32 promptResult;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		scColossoAskDoneCheering(guideId);
		return;
	}
	fdScriptStart();
	promptResult = scColossoGreetAtStage(guideId, STAGE_STEPPING_STONE);
	if(promptResult == COLOSSO_GUIDE_DESCRIBE) {
		fdScriptSetMessageId(MsgColossoSteppingStoneStage);
		fdCameraSetSpeed(3);
		fdCameraMoveTo(376, CAMERA_KEEP, 152, 1);
		fdCameraWaitForMove();
		fdScriptWait(30);
		/* "This is the Stepping- Stone Stage." */
		fdScriptShowMessage(guideId, 0);
		scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 280, 200);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 280, 152);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
		fdScriptWait(10);
		fdTryJumpForward();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 15);
		fdTryJumpForward();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
		fdTryJumpForward();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
		/* "You can reach the next stage by jumping from stone to stone." */
		fdScriptShowMessage(guideId, 0);
		scColossoShowPointer(96, 40, 0);
		scColossoMovePointer(128, 40, 10);
		fdScriptWait(30);
		scColossoMovePointer(160, 40, 10);
		fdScriptWait(30);
		scColossoMovePointer(160, 72, 10);
		fdScriptWait(30);
		/* "Study the placement of the stones, then choose the shortest
		   path." */
		fdScriptShowMessage(guideId, 0);
		scColossoHidePointer();
		scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
		fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
		scColossoRepeatStageNumber(guideId, STAGE_STEPPING_STONE);
	} else if(promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
		fdScriptSetMessageId(MsgColossoYourGoalInStageSimple);
		/* "Your goal in this stage is simple: find the shortest path." */
		fdScriptShowMessage(guideId, 0);
	}
	scColossoOfferCheeringSpot(promptResult, guideId, STAGE_STEPPING_STONE);
	fdScriptEnd();
}

/* The Wall Wedge's attendant explains stage 3 of Colosseum Final 3. */
static void scColosseumNavampaMatchTalkToWallWedgeAttendant(Sint32 guideId)
{
	FIELD_STATE *state;
	Sint32		 promptResult;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		scColossoAskDoneCheering(guideId);
	} else {
		fdScriptStart();
		promptResult = scColossoGreetAtStage(guideId, STAGE_WALL_WEDGE);
		if(promptResult == COLOSSO_GUIDE_DESCRIBE) {
			state = FieldStateWork;
			fdScriptSetMessageId(MsgColossoPlaceCalledWall);
			scColosseumNavampaMatchResetWallWedge();
			fdCameraSetSpeed(3);
			fdCameraMoveTo(616, CAMERA_KEEP, 184, 1);
			fdCameraWaitForMove();
			fdScriptWait(30);
			/* "This place is called the Wall Wedge." */
			fdScriptShowMessage(guideId, 0);
			scColosseumNavampaMatchStartWallWedge();
			fdScriptWait(60);
			/* "The pistons move back and forth. Avoid them to reach the next
			   stage." */
			fdScriptShowMessage(guideId, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 504, 200);
			COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
			scColosseumNavampaMatchWaitForWallWedgeOpening();
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			scColossoWalkActorTo(ACTOR_PARTY_LEADER, 680, 200);
			for(promptResult = 0; promptResult < 4 * TICKS_PER_SECOND &&
				state->raisedCue != PISTON_ANIMATION_PUSH_OUT;
				promptResult++)
				sySynch(1);
			scColosseumNavampaMatchShovedOffWalkway();
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
			fdActorShowEmote(
				ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
			/* "If your timing is off, a piston might knock you off the
			   ledge." */
			fdScriptShowMessage(guideId, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNumber(guideId, STAGE_WALL_WEDGE);
			state->raisedCue = RAISED_CUE_NONE;
		} else if(promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
			fdScriptSetMessageId(MsgColossoOperatorWallsCheer);
			/* "The operator of the walls will cheer for you on this stage." */
			fdScriptShowMessage(guideId, 0);
		}
		scColossoOfferCheeringSpot(promptResult, guideId, STAGE_WALL_WEDGE);
		fdScriptEnd();
	}
}

/* The Free-Climb Area's attendant explains stage 4 of Colosseum Final 3. */
static void scColosseumNavampaMatchTalkToFreeClimbAttendant(Sint32 guideId)
{
	Sint32 result;
	ACTOR *actor;
	FIXED  x;
	FIXED  y;
	FIXED  raisedHeight;
	FIXED  shiftedX;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		scColossoAskDoneCheering(guideId);
	} else {
		fdScriptStart();
		result = scColossoGreetAtStage(guideId, STAGE_FREE_CLIMB);
		if(result == COLOSSO_GUIDE_DESCRIBE) {
			fdScriptSetMessageId(MsgColossoPlaceNormallyCalledFreeClimb);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(856, CAMERA_KEEP, 168, 1);
			fdCameraWaitForMove();
			fdScriptWait(30);
			/* "This place is normally called the Free- Climb Area." */
			fdScriptShowMessage(guideId, 0);
			/* "This sheer wall has a bumpy surface." */
			fdScriptShowMessage(guideId, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 816, 200);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 840, 200);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
			fdClimbUp();
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			y = actor->y.fixed;
			x = actor->x.fixed;
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
			raisedHeight = toFIXED(6) + y;
			syFieldActorSetMoveTargetFX(actor, x, raisedHeight, actor->z.fixed);
			fdActorWaitForMove(actor);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_EAST);
			shiftedX = toFIXED(64) + x;
			syFieldActorSetMoveTargetFX(
				actor, shiftedX, raisedHeight, actor->z.fixed);
			fdActorWaitForMove(actor);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
			syFieldActorSetMoveTargetFX(
				actor, shiftedX, y + toFIXED(54), actor->z.fixed);
			fdActorWaitForMove(actor);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_WEST);
			syFieldActorSetMoveTargetFX(
				actor, x + toFIXED(48), y + toFIXED(54), actor->z.fixed);
			fdActorWaitForMove(actor);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLING);
			/* "Using those bumps, feel your way up the wall and climb to
			   the..." */
			fdScriptShowMessage(guideId, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNumber(guideId, STAGE_FREE_CLIMB);
		} else if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
			fdScriptSetMessageId(MsgColossoClearStageMustAbleChange);
			/* "To clear this stage, you must be able to change
			   directions..." */
			fdScriptShowMessage(guideId, 0);
		}
		scColossoOfferCheeringSpot(result, guideId, STAGE_FREE_CLIMB);
		fdScriptEnd();
	}
}

/* Sets an actor running toward (x, z) in pixels, at the height it stands at. */
static void scColosseumNavampaMatchRunActorTo(
	Sint32 selector, Sint32 x, Sint32 z)
{
	ACTOR *actor;

	actor = fdActorLookup(selector);
	if(actor != NULL) {
		fdActorStartIdle(actor);
		fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
		fdActorSetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

/* Runs an actor to (x, z) in pixels and stands it there once it arrives. */
static void scColosseumNavampaMatchRunActorToAndStand(
	Sint32 selector, Sint32 x, Sint32 z)
{
	ACTOR *actor;

	actor = fdActorLookup(selector);
	if(actor != NULL) {
		fdActorStartIdle(actor);
		fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
		fdActorSetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMove(actor);
		fdActorSelectAnimation(actor, ANIM_STAND);
	}
}

/* The Moving Sidewalk Stage's attendant explains stage 5 of Colosseum Final
   3. */
static void scColosseumNavampaMatchTalkToMovingSidewalkAttendant(Sint32 guideId)
{
	Sint32 i;
	Sint32 result;
	ACTOR *actor;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		scColossoAskDoneCheering(guideId);
	} else {
		fdScriptStart();
		result = scColossoGreetAtStage(guideId, STAGE_MOVING_SIDEWALK);
		if(result == COLOSSO_GUIDE_DESCRIBE) {
			fdScriptSetMessageId(MsgColossoCalledMovingSidewalkStage);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(1080, CAMERA_KEEP, 168, 1);
			fdCameraWaitForMove();
			fdScriptWait(30);
			/* "This is called the Moving Sidewalk Stage." */
			fdScriptShowMessage(guideId, 0);
			/* "The conveyor belt moves opposite the direction you need to
			   go." */
			fdScriptShowMessage(guideId, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 984, 184);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			scColosseumNavampaMatchRunActorToAndStand(
				ACTOR_PARTY_LEADER, 992, 184);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.3);
			scColosseumNavampaMatchRunActorTo(ACTOR_PARTY_LEADER, 1120, 184);
			fdScriptWait(120);
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
			fdScriptWait(120);
			scActorResetMotionFields(ACTOR_PARTY_LEADER);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if !SIDEWALK_DEMO_KEEPS_LEADER_EFFECT
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
#endif
			fdActorShowEmote(
				ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			for(i = 0; i < 120; i++) {
				if(actor->x.fixed > toFIXED(992)) {
					actor->x.fixed -= toFIXED(6) / 5;
				}
				sySynch(1);
			}
			fdActorShowEmote(
				ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
			scColosseumNavampaMatchRunActorTo(ACTOR_PARTY_LEADER, 1120, 184);
			/* "Race against it to reach the next stage." */
			fdScriptShowMessage(guideId, 0);
			scActorResetMotionFields(ACTOR_PARTY_LEADER);
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_CLIMB;
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNumber(guideId, STAGE_MOVING_SIDEWALK);
		} else if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
			fdScriptSetMessageId(MsgColossoInStageMustTryOutpace);
			/* "In this stage, you must try to outpace the conveyor belt." */
			fdScriptShowMessage(guideId, 0);
		}
		scColossoOfferCheeringSpot(result, guideId, STAGE_MOVING_SIDEWALK);
		fdScriptEnd();
	}
}

/* The log-rolling area's attendant explains stage 6 of Colosseum Final 3. */
static void scColosseumNavampaMatchTalkToLogRollingAttendant(Sint32 actorId)
{
	Sint32 state;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
		scColossoAskDoneCheering(actorId);
		return;
	}

	fdScriptStart();
	state = scColossoGreetAtStage(actorId, STAGE_LOG_ROLLING);

	if(state == COLOSSO_GUIDE_DESCRIBE) {
		fdScriptSetMessageId(MsgColossoAnotherLogRollingArea);
		fdCameraSetSpeed(3);
		fdCameraMoveTo(1288, CAMERA_KEEP, 152, 1);
		fdCameraWaitForMove();
		fdScriptWait(30);
		/* "This is another log-rolling area." */
		fdScriptShowMessage(actorId, 0);
		scColossoShowPointer(180, 88, 0);
		fdScriptWait(60);
		/* "The obstacle in this stage is the water tank at the exit." */
		fdScriptShowMessage(actorId, 0);
		scColossoMovePointer(32, 84, 10);
		fdScriptWait(30);
		/* "To get across the tank, you'll need to roll the logs." */
		fdScriptShowMessage(actorId, 0);
		scColossoMovePointer(96, 84, 30);
		fdScriptWait(60);
		/* "You can get through quickly by moving the logs and stones." */
		fdScriptShowMessage(actorId, 0);
		scColossoHidePointer();
		fdScriptWait(2);
		fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
		scColossoRepeatStageNumber(actorId, STAGE_LOG_ROLLING);
	} else if(state == COLOSSO_GUIDE_OFFER_CHEERING) {
		fdScriptSetMessageId(MsgColossoHereMustFigureOutHow);
		/* "Here you must figure out how the logs and stones need to be..." */
		fdScriptShowMessage(actorId, 0);
	}

	scColossoOfferCheeringSpot(state, actorId, STAGE_LOG_ROLLING);
	fdScriptEnd();
}

#include "shared/colosso_solo.c"
#include "shared/item_equip.c"
#include "shared/colosso_task.c"

#include "shared/colosso_lettering.c"

#include "shared/colosso_guide.c"
#include "shared/colosso_item_icon.c"

#include "shared/colosso_hex_string.c"
#include "shared/colosso_finish.c"

#include "shared/colosso_path_opponent.c"
#include "shared/colosso_setup.c"
#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_push.c"

/* The chest holding a Smoke Bomb has been opened. */
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_6A
/* The chest holding a Nut has been opened. */
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_NUT_CHEST_OPENED		 FLAG_AREA_TEMP_6B
/* The chest holding a Sleep Bomb has been opened. */
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_SLEEP_BOMB_CHEST_OPENED FLAG_AREA_TEMP_6C

/* The Moving Sidewalk Stage's log where Force has knocked it off the belt,
   bobbing a pixel up and down. */
const Sint32 ColosseumNavampaMatchSidewalkLogOffBelt[] = {
	SC_SET(SW_Y, toFIXED(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The Wall Wedge's cycle of four steps.

   Each holds each piston's animation, west to east, then the operator's. */
const Sint8 ColosseumNavampaMatchWallWedgeCycle[][6] = {
	{PISTON_ANIMATION_PULL_IN, PISTON_ANIMATION_PUSH_OUT,
		PISTON_ANIMATION_PULL_IN, PISTON_ANIMATION_PUSH_OUT,
		PISTON_ANIMATION_PULL_IN, WALL_OPERATOR_ANIMATION_ODD_PISTONS_IN},
	{PISTON_ANIMATION_PUSH_HALF_OUT, PISTON_ANIMATION_PULL_HALF_IN,
		PISTON_ANIMATION_PUSH_HALF_OUT, PISTON_ANIMATION_PULL_HALF_IN,
		PISTON_ANIMATION_PUSH_HALF_OUT,
		WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT},
	{PISTON_ANIMATION_PUSH_OUT, PISTON_ANIMATION_PULL_IN,
		PISTON_ANIMATION_PUSH_OUT, PISTON_ANIMATION_PULL_IN,
		PISTON_ANIMATION_PUSH_OUT, WALL_OPERATOR_ANIMATION_ODD_PISTONS_OUT},
	{PISTON_ANIMATION_PULL_HALF_IN, PISTON_ANIMATION_PUSH_HALF_OUT,
		PISTON_ANIMATION_PULL_HALF_IN, PISTON_ANIMATION_PUSH_HALF_OUT,
		PISTON_ANIMATION_PULL_HALF_IN,
		WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT},
};

/* clang-format off */
/* The way the long log rolls for each eighth of its rider's facing. */
const Uint16 ColosseumNavampaMatchFacingAngles[] = {
	0, 0, FACING_SOUTH, FACING_WEST, FACING_WEST, FACING_WEST, FACING_NORTH, 0,
};

const Sint32 ColosseumNavampaMatchSidewalkLogScript[] = {
	SC_MOVE(1088, 0, 184),
	SC_SYNC,
	SC_MOVE(1130, 0, 184),
	SC_SYNC,
	SC_CALL(scColosseumNavampaMatchShowActivatedSidewalkLayer),
	SC_LABEL(1),
	SC_SET(SW_Y, toFIXED(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

#include "shared/colosso_lettering_tables.c"

char HexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

SCENE_ENTRANCE ColosseumNavampaMatchEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ColosseumNavampaMatchExits[] = {
	SCENE_EXITS(ColosseumNavampaMatch),
	SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE, ColosseumNavampaMatch, COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND),
	SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM, Colosseum, COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
	SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
	SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
	SCENE_EXITS_END,
};

ACTOR_PLACE ColosseumNavampaMatchActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		152, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		216, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		712, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1240, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1272, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1304, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		536, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		568, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		600, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		632, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		664, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		536, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		568, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		600, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		664, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		720, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		792, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STANDING_LOG, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		1080, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		1208, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		600, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		856, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1080, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1304, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1512, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1512, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

/* The Wall Wedge's step in its four-step piston cycle. */
Sint32 ColosseumNavampaMatchWallWedgeStep = 0;

/* The ticks into the Wall Wedge's step, 0 to 17. */
Uint32 ColosseumNavampaMatchWallWedgeTimer = 0;

CUE ColosseumNavampaMatchCues[] = {
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_LOG_AREA_END, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchHopFromLogAreaEnd) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_WEIGHT_SQUARE_EDGE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchPushWeightOrHopAcross) },
	{ CUE_STEP, METATILE_BEHAVIOR_MOVING_SIDEWALK, FLAG_SIDEWALK_LOG_KNOCKED_OFF, SCRIPT_REF(scColosseumNavampaMatchRideMovingSidewalk) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_WEIGHT_SQUARE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchPushWeight) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SCALES_WEIGHT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchSetWeightOnScales) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchMoveStone) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_ROCK_ROW, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchPushRock) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchJustMoveRock) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_LONG_LOG, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchRollLongLog) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_A, FLAG_FIRST_LOG_AREA_STONE_SETTLED, SCRIPT_REF(scColosseumNavampaMatchUpdateLogAreaStoneMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_B, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 1, SCRIPT_REF(scColosseumNavampaMatchUpdateLogAreaStoneMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_C, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 2, SCRIPT_REF(scColosseumNavampaMatchUpdateLogAreaStoneMetatiles) },
	{ CUE_STEP, METATILE_BEHAVIOR_SCALES_LEDGE, FLAG_ON_SCALES, SCRIPT_REF(scColosseumNavampaMatchStepOnScales) },
	{ CUE_STEP, METATILE_BEHAVIOR_OFF_SCALES, IF_FLAG_SET(FLAG_ON_SCALES), SCRIPT_REF(scColosseumNavampaMatchStepOffScales) },
	{ CUE_STEP, METATILE_BEHAVIOR_ARENA_ENTRY, FLAG_COLOSSEUM_ARENA_CLOSED, SCRIPT_REF(scColosseumNavampaMatchReachArena) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_SMOKE_BOMB_CHEST, FLAG_COLOSSEUM_NAVAMPA_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_NUT_CHEST, FLAG_COLOSSEUM_NAVAMPA_MATCH_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
	{ EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | FACING_WEST, METATILE_BEHAVIOR_SLEEP_BOMB_CHEST, FLAG_COLOSSEUM_NAVAMPA_MATCH_SLEEP_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE, FLAG_SCALES_FROZEN, SCRIPT_REF(scColosseumNavampaMatchFreezeOnScales) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, SCRIPT_REF(scColosseumNavampaMatchGrowPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_WALL_OPERATOR, FLAG_WALL_WEDGE_HALTED, SCRIPT_REF(scColosseumNavampaMatchHaltWall) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_ACTS), ACTOR_SIDEWALK_LOG, FLAG_SIDEWALK_LOG_KNOCKED_OFF, SCRIPT_REF(scColosseumNavampaMatchForceSidewalkLog) },
	{ CUE_RAISED, PISTON_ANIMATION_PUSH_HALF_OUT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchShovedByPiston) },
	{ CUE_RAISED, PISTON_ANIMATION_PUSH_OUT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchShovedOffWalkway) },
	{ CUE_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scColossoAskDoneLookingAround) },
	{ CUE_TALK, ACTOR_SCALES_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchTalkToScalesAttendant) },
	{ CUE_TALK, ACTOR_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchTalkToSteppingStoneAttendant) },
	{ CUE_TALK, ACTOR_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchTalkToWallWedgeAttendant) },
	{ CUE_TALK, ACTOR_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchTalkToFreeClimbAttendant) },
	{ CUE_TALK, ACTOR_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchTalkToMovingSidewalkAttendant) },
	{ CUE_TALK, ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumNavampaMatchTalkToLogRollingAttendant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogTheStreetVendorsCornOnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogICantSeeAnythingWhenIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogItsNotVeryExcitingWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogStandingForLongPeriodsOfTime) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogIWasUpAllNightTrying) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogSomeTouristToldMeILook) },
	{ CUE_RAISED, COLOSSO_TAKE_PLACE_CUE, CONDITION_ALWAYS,
	  SCRIPT_REF(scColossoTakePlaceForMatch) },
	{ CUE_TABLE_END },
};

/* Navampa after the last battle */
/* (scColosseumNavampaMatchPlayPostBattleReunion). */
/* */
/* His sprite grows taller and narrower and back, over and over. */
Sint32 ColosseumNavampaMatchOpponentAfterBattle[] = {
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

Sint32 ColosseumNavampaMatchLeaderActionA[] = {
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(6),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(12),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

Sint32 ColosseumNavampaMatchLeaderActionB[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 64),
	SC_WAIT(16),
	SC_ADD(SW_SCALE_Y, -(toFIXED(3) / 64)),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -(toFIXED(3) / 64)),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -(toFIXED(3) / 64)),
	SC_WAIT(32),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

Sint32 ColosseumNavampaMatchLeaderActionC[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 64),
	SC_WAIT(26),
	SC_ADD(SW_SCALE_Y, -(toFIXED(3) / 64)),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -(toFIXED(3) / 64)),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -(toFIXED(3) / 64)),
	SC_WAIT(52),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

Sint32 ColosseumNavampaMatchLeaderScaleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_END,
};

Sint32 ColosseumNavampaMatchSleepZScript[] = {
	SC_CALL(scColosseumNavampaMatchAdvanceSleepZ),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

Sint16 ColossoLetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
/* clang-format on */

Sint16 ColossoPointerSlot = -1;

#include "shared/colosso_finish_scripts.c"
