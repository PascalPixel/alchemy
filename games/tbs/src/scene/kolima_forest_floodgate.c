/* @file */
/* Kolima Forest's pool above the floodgate (SceneId_KolimaForestFloodgate), */
/* the fourth of the forest's five scenes. The party comes in from the cliffs */
/* to the west (SceneId_KolimaForestCliff) and goes on by the path north to */
/* Tret (SceneId_KolimaForestTree). */
/* */
/* A river pool fills most of the scene, with five logs floating on it. At */
/* the bottom of the scene, by the floodgate and a sign that reads "Do not */
/* touch Floodgate Switch!", the Floodgate Switch drains the pool and fills it
 */
/* again. Drained, the pool is a grassy riverbed with the logs lying on it, */
/* and walking into a log rolls it ahead until it hits something. Filled, the */
/* logs float where they were left, and Isaac hops across them from log to */
/* log; from the north bank he can jump out onto one. Rolled into the right */
/* places, they make a way across to the chest with a Fur Coat on the */
/* north-east bank. A lone upright log on the grass west of the pool slides */
/* two cells west, or back east, when walked into. The forest keeps its cursed
 */
/* colours until Tret lifts the curse (FLAG_KOLIMA_CURSE_LIFTED). */
/* */
/* The water level, the bank log's place and where each log lies are kept in */
/* SceneLocalState (KolimaForestFloodgate_WaterHigh, */
/* KolimaForestFloodgate_BankLogWest and KolimaForestFloodgate_Logs), so */
/* they survive a battle. */
/* */
/* The cells under a log are CELL_BEHAVIOR_JUMP_FORWARD while it floats, which
 */
/* is what makes the leader hop across it, and CELL_BEHAVIOR_WALL while it */
/* lies on the riverbed (scKolimaForestFloodgateMarkLogCells), which */
/* scKolimaForestFloodgateCheckLogFits also treats as taken. */
/* */
/* Flipping the switch while the pool is full lifts the floodgate: water */
/* churns out through it (SPLASH_ANIMATION_CHURN, SE_WATER_DRAIN) and the pool
 */
/* turns to riverbed a line at a time, at random. Flipping it back drops the */
/* gate in a burst of spray (SPLASH_ANIMATION_BURST) and the water comes back */
/* the same way. */

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/map_grid_scroll.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "scene/kolima_palette.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

/* Unused: eight bytes before the logs' records that nothing reads. */
u8 KolimaForestFloodgate_UnusedBeforeRecords[8] BSS_DATA;
/* The bend's logs in SceneLocalState from byte 4, so they survive a battle. */
struct FLOODGATE_LOG *KolimaForestFloodgate_Logs BSS_DATA;
/* The bank log's place in SceneLocalState's first halfword: 1 west, 0 east. */
u16 *KolimaForestFloodgate_BankLogWest BSS_DATA;
/* The bend's water level in SceneLocalState's second halfword: 1 high (the */
/* logs float), 0 low; the floodgate switch flips it. */
s16 *KolimaForestFloodgate_WaterHigh BSS_DATA;
/* Unused: four bytes nothing reads or writes. */
u8 KolimaForestFloodgate_UnusedAfterMap[4] BSS_DATA;
/* The healthy colours, kept by scKolimaPaletteSaveHealthy: the palette fade's
 */
/* 448 background and sprite colours, in the array's first half. */
u8 KolimaPalette_Healthy[1792] BSS_DATA;
/* The cursed colours, kept by scKolimaPaletteSaveCursed. */
u8 KolimaPalette_Cursed[896] BSS_DATA;
/* Unused: 896 bytes nothing reads or writes. */
u8 KolimaForestFloodgate_UnusedAfterCursed[896] BSS_DATA;
/* BG1 to BG3's scroll, captured each frame and moved down to where the */
/* river's new level is drawn (NEW_LEVEL_OFFSET). */
u32 KolimaForestFloodgate_NewLevelScroll[3] BSS_DATA;
/* The percent chance that a line of the river shows its new level: 0 when the
 */
/* rumble starts (SE_WATER_LEVEL), then a point higher each frame up to 101 */
/* (scKolimaForestFloodgateFlipFloodgateSwitch). */
u32 KolimaForestFloodgate_NewLevelPercent BSS_DATA;

struct ACTOR;

/* A log's saved record: its sprite, first map cell and orientation, then its */
/* actor once created. */
typedef struct FLOODGATE_LOG {
	s16 spr;	  /* sprite number; -1 ends the list */
	s16 x;		  /* map cells */
	s16 z;		  /* map cells */
	s16 vertical; /* nonzero runs north to south; zero west to east */
	ACTOR *actor; /* created for this row; NULL in the defaults */
} FLOODGATE_LOG;

STATIC_ASSERT(sizeof(FLOODGATE_LOG) == 12);
STATIC_ASSERT(offsetof(FLOODGATE_LOG, x) == 2);
STATIC_ASSERT(offsetof(FLOODGATE_LOG, vertical) == 6);
STATIC_ASSERT(offsetof(FLOODGATE_LOG, actor) == 8);

/* UB: the scene calls this before its definition with no prototype, so as a */
/* function returning int. */
#ifdef UBFIX
void scKolimaForestFloodgateMarkLogCells(const FLOODGATE_LOG *records,
										 s32 value);
#endif
extern FLOODGATE_LOG KolimaForestFloodgate_DefaultLogs[];
extern u8 KolimaForestFloodgate_PushAnimations[];
extern s8 KolimaForestFloodgate_PushStepX[];
extern s8 KolimaForestFloodgate_PushStepZ[];
extern SCENE_ENTRANCE KolimaForestFloodgateEntrances[];
extern u32 KolimaForestFloodgateExits[];
extern ACTOR_PLACE KolimaForestFloodgateActorPlacements[];
extern TRIGGER KolimaForestFloodgateTriggers[];

/* The actors, numbering KolimaForestFloodgateActorPlacements' */
/* rows from ACTOR_FIRST_PLACED. */
enum {
	/* The upright log on the grass west of the pool, which slides between */
	/* cell columns 4 and 6. */
	ACTOR_BANK_LOG = ACTOR_FIRST_PLACED,
	/* The water at the floodgate, which churns out as the gate lifts and */
	/* bursts as it drops shut. */
	ACTOR_FLOODGATE_SPRAY,
	ACTOR_SIGN
};

/* The most cells one shove rolls a log. */
#define LOG_ROLL_MAX_CELLS 11

/* The background cell row the river is drawn at, and the row its new level */
/* is drawn at first while the river's lines change to it. */
#define RIVER_PANEL_ROW		19
#define NEW_LEVEL_PANEL_ROW 30
/* How far below the river its new level is drawn, in pixels. */
#define NEW_LEVEL_OFFSET                                                       \
	((NEW_LEVEL_PANEL_ROW - RIVER_PANEL_ROW) * MAPGRID_CELL_PIXELS)
/* The last screen line of the river, the lines that change to its new level. */
#define RIVER_LAST_LINE 53

/* The cells this screen gives a meaning of its own. */
typedef enum KOLIMA_FOREST_FLOODGATE_CELL_BEHAVIOR {
	/* The Floodgate Switch's cell, which a search flips. */
	CELL_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH = 9,
	/* The north bank's edge at cell (10, 7), where the leader jumps out onto */
	/* a floating log; the full pool's cells alone have it. */
	CELL_BEHAVIOR_KOLIMA_FOREST_NORTH_BANK_JUMP = 10,
	/* The drained pool's riverbed, where walking into a log rolls it. */
	CELL_BEHAVIOR_KOLIMA_FOREST_RIVERBED = 11,
	/* West and east of the bank log, cells (3, 9) to (3, 12) and (7, 9) to */
	/* (7, 12), where walking into it shoves it. */
	CELL_BEHAVIOR_KOLIMA_FOREST_WEST_OF_BANK_LOG = 12,
	CELL_BEHAVIOR_KOLIMA_FOREST_EAST_OF_BANK_LOG = 13
} KOLIMA_FOREST_FLOODGATE_CELL_BEHAVIOR;

static void scKolimaForestFloodgateMarkBankLogCells(void);
static void scKolimaForestFloodgateCreateLogs(FLOODGATE_LOG *log);
static void scKolimaForestFloodgateRollLog(FLOODGATE_LOG *logs);

/* The party comes in from the cliffs to the west or down the path from Tret */
/* to the north. */
const SCENE_ENTRANCE *scGetEntrances(void) {
	return KolimaForestFloodgateEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return 0; }

/* The ways back west to the cliffs (SceneId_KolimaForestCliff) and north to */
/* Tret (SceneId_KolimaForestTree). */
const u32 *scGetExits(void) { return KolimaForestFloodgateExits; }

/* The bank log, the floodgate's spray and the sign. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	return KolimaForestFloodgateActorPlacements;
}

/* The logs, the switch, the sign, the jump and the chest. */
const TRIGGER *scGetTriggers(void) { return KolimaForestFloodgateTriggers; }

/* With the pool above Kolima Forest's floodgate drained, walking into a log on
 */
/* the riverbed rolls it ahead until it meets the bank or another log, with */
/* the view following it, and it stays where it stops. Walking anywhere on the
 */
/* riverbed runs this, but only a log right ahead of Isaac moves; the riverbed
 */
/* is then given back its own cells and the logs' cells made solid where they */
/* now lie. */
static void scKolimaForestFloodgatePushLog(void) {
	scKolimaForestFloodgateRollLog(KolimaForestFloodgate_Logs);
	fdMapGridCopyCellAttributes(0, 64, 32, 32, 0, 0);
	scKolimaForestFloodgateMarkLogCells(KolimaForestFloodgate_Logs,
										CELL_BEHAVIOR_WALL);
	scKolimaForestFloodgateMarkBankLogCells();
}

/* With the pool above Kolima Forest's floodgate full, walking south off the */
/* north bank at cell (10, 7) jumps Isaac two cells out onto a floating log, */
/* when one lies there; over open water he does not jump. */
static void scKolimaForestFloodgateJumpSouth(void) { fdTryJumpForward(); }

#include "shared/map_grid_panel.c"

/* Makes the bank log's four cells solid: column 4 at its west place, column 6
 */
/* at its east, rows 9 to 12. */
static void scKolimaForestFloodgateMarkBankLogCells(void) {
	if((s16)*KolimaForestFloodgate_BankLogWest == 1) {
		fdMapGridCopyCellAttributes(0, 0, 1, 4, 4, 9);
	}
	else {
		fdMapGridCopyCellAttributes(0, 0, 1, 4, 6, 9);
	}
}

/* Draws the pool full or drained and each log floating on it or lying on the */
/* riverbed, and gives the pool the cells of that level. */
static void scKolimaForestFloodgateDrawWaterLevel(void) {
	ACTOR *log;
	FLOODGATE_LOG *record = KolimaForestFloodgate_Logs;

	/* The floodgate, shut over the full pool or lifted over the drained one. */
	if(*KolimaForestFloodgate_WaterHigh != 0) {
		fdMapGridCopyCells(65, 53, 2, 1, 79, 29);
		fdMapGridCopyCells(65, 40, 2, 4, 15, 28);
	}
	else {
		fdMapGridCopyCells(65, 50, 2, 5, 79, 25);
	}

	/* The pool's water or its riverbed, on both layers, and its cells. */
	if(*KolimaForestFloodgate_WaterHigh != 0) {
		fdMapGridCopyCells(0, 32, 32, 32, 32, 0);
		fdMapGridCopyCells(32, 32, 32, 32, 64, 0);
		fdMapGridCopyCellAttributes(0, 32, 32, 32, 0, 0);
	}
	else {
		fdMapGridCopyCells(0, 64, 32, 32, 32, 0);
		fdMapGridCopyCells(32, 64, 32, 32, 64, 0);
		fdMapGridCopyCellAttributes(0, 64, 32, 32, 0, 0);
	}

	/* Each log floats or rests, and on high water its four cells are drawn. */
	for(; record->spr != -1; record++) {
		log = record->actor;

		if(*KolimaForestFloodgate_WaterHigh == 1) {
			fdActorSelectAnimation(log, LOG_ANIMATION_FLOAT);
			log->priFlg = ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
			log->motionFlg = 0;
			log->y.fixed = toFIXED(26);

			if(record->vertical != 0) {
				fdMapGridCopyCells(68, 40, 1, 4, record->x + 32, record->z);
			}
			else {
				fdMapGridCopyCells(70, 40, 4, 1, record->x + 32, record->z);
			}
		}
		else {
			fdActorSelectAnimation(log, LOG_ANIMATION_AT_REST);
			log->priFlg = ACTOR_PRIORITY_AUTOMATIC;
			log->motionFlg = ACTOR_FALLS;
			log->y.fixed = 0;
		}
	}

	/* The full pool's own cell (10, 50), which it copies in as cell (10, 18),
	 */
	/* takes cell (70, 42): a wall cell (render probe scene */
	/* KolimaForestFloodgate --cells 8,48,12,52 reads 255 there), so the water
	 */
	/* under the west end of the last log of KolimaForestFloodgate_DefaultLogs
	 */
	/* stays a wall once the log has been shoved away. Its picture shows from
	 * the */
	/* next time the pool fills. */
	/* XXX: what that picture is; a map photo of the full pool with that log */
	/* shoved off (10, 18) would settle it. */
	fdMapGridCopyCells(70, 42, 1, 1, 10, 50);

	if(*KolimaForestFloodgate_WaterHigh == 1) {
		fdMapGridCopyCellAttributes(0, 32, 32, 32, 0, 0);
		scKolimaForestFloodgateMarkLogCells(KolimaForestFloodgate_Logs,
											CELL_BEHAVIOR_JUMP_FORWARD);
	}
	else {
		fdMapGridCopyCellAttributes(0, 64, 32, 32, 0, 0);
		scKolimaForestFloodgateMarkLogCells(KolimaForestFloodgate_Logs,
											CELL_BEHAVIOR_WALL);
	}

	scKolimaForestFloodgateMarkBankLogCells();
}

/* HBlank callback: gives each line of the river, with a chance of */
/* KolimaForestFloodgate_NewLevelPercent in 100, the scroll that shows its new
 */
/* level. The HBlank after line n sets line n + 1's scroll. */
static void scKolimaForestFloodgateChangeRiverLines(void) {
	u16 line = REG_VCOUNT;
	u32 *source = (u32 *)&BgScrollWork[1];
	volatile u32 *destination = (volatile u32 *)&REG_BG1HOFS;

	if(line == DISPLAY_TOTAL_LINES - 1 || line < RIVER_LAST_LINE) {
		if(syRandomBelow(100) < KolimaForestFloodgate_NewLevelPercent) {
			source = KolimaForestFloodgate_NewLevelScroll;
		}
	}

	*destination = *source++;
	destination = (volatile u32 *)&REG_BG2HOFS;
	*destination++ = *source++;
	*destination = *source;
}

/* Captures BG1 to BG3's scroll and moves it down to the river's new level. */
static void scKolimaForestFloodgateCaptureNewLevelScroll(void) {
	const u32 *source = (const u32 *)&BgScrollWork[1];
	u32 *destination = KolimaForestFloodgate_NewLevelScroll;
	BG_SCROLL *newLevel;

	/* BG1 to BG3's offsets, each pair one word. */
	*destination++ = *source++;
	*destination++ = *source++;
	*destination = *source;
	newLevel = (BG_SCROLL *)KolimaForestFloodgate_NewLevelScroll;
	newLevel[0].y += NEW_LEVEL_OFFSET;
	newLevel[1].y += NEW_LEVEL_OFFSET;
	newLevel[2].y += NEW_LEVEL_OFFSET;
}

/* Searching the Floodgate Switch, on the bank by Kolima Forest's floodgate */
/* west of the sign (cell (19, 28)), flips it: "Isaac flipped the switch." The
 */
/* view moves to the gate. If the pool is full, the gate lifts with water */
/* churning out below it and the pool drains away a line at a time; if it is */
/* drained, the gate drops shut in a burst of spray and the pool fills again */
/* the same way. */
static void scKolimaForestFloodgateFlipFloodgateSwitch(void) {
	s32 nextPercent;

	fdEventStart();
	fdCameraSetSpeed(1);
	fdCameraMoveTo(264, CAMERA_KEEP, 448, 1);
	fdCameraWaitForArrival();
	mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
	sySndPlay(SE_CRUNCH);
	/* The switch's lever turns, then the gate drops shut, or lifts, a picture
	 * at */
	/* a time. */
	if(*KolimaForestFloodgate_WaterHigh == 0) {
		fdActorSetPos(ACTOR_FLOODGATE_SPRAY, 256, 462);
		fdMapGridCopyCells(77, 34, 1, 2, 83, 25);
		sySynch(3);
		fdMapGridCopyCells(78, 34, 1, 2, 83, 25);
		sySynch(3);
		fdMapGridCopyCells(79, 34, 1, 2, 83, 25);
		sySynch(30);
		fdMapGridCopyCells(67, 34, 2, 5, 79, 25);
		sySynch(6);
		fdMapGridCopyCells(69, 34, 2, 5, 79, 25);
		fdActorSetAnimation(ACTOR_FLOODGATE_SPRAY, SPLASH_ANIMATION_BURST);
		sySndPlay(SE_LOG_FALL);
		sySynch(6);
		fdMapGridCopyCells(71, 34, 2, 5, 79, 25);
		sySynch(6);
		fdMapGridCopyCells(73, 34, 2, 5, 79, 25);
		fdMapGridCopyCells(75, 38, 2, 1, 79, 29);
		sySynch(4);
		fdMapGridCopyCells(77, 38, 2, 1, 79, 29);
		sySynch(6);
		fdMapGridCopyCells(79, 38, 2, 1, 79, 29);
		sySynch(8);
		fdMapGridCopyCells(65, 53, 2, 1, 79, 29);
		fdMapGridCopyCells(65, 40, 2, 4, 15, 28);
	}
	else {
		fdActorSetPos(ACTOR_FLOODGATE_SPRAY, 256, 480);
		fdMapGridCopyCells(78, 34, 1, 2, 83, 25);
		sySynch(3);
		fdMapGridCopyCells(77, 34, 1, 2, 83, 25);
		sySynch(3);
		fdMapGridCopyCells(76, 34, 1, 2, 83, 25);
		sySynch(30);
		fdMapGridCopyCells(65, 45, 2, 4, 15, 28);
		fdMapGridCopyCells(71, 50, 2, 5, 79, 25);
		fdActorSetAnimation(ACTOR_FLOODGATE_SPRAY, SPLASH_ANIMATION_CHURN);
		sySndPlay(SE_WATER_DRAIN);
		sySynch(6);
		fdMapGridCopyCells(69, 50, 2, 5, 79, 25);
		sySynch(6);
		fdMapGridCopyCells(67, 50, 2, 5, 79, 25);
		sySynch(6);
		fdMapGridCopyCells(65, 50, 2, 5, 79, 25);
		sySynch(30);
	}
	if(*KolimaForestFloodgate_WaterHigh == 0) {
		scMapGridDrawCellPanel(9, 19, 16, 5, *KolimaForestFloodgate_WaterHigh,
							   9, NEW_LEVEL_PANEL_ROW);
		scMapGridDrawCellPanel(9, 51, 16, 5, 1, 9, NEW_LEVEL_PANEL_ROW);
		scMapGridDrawCellPanel(41, 51, 16, 5, 2, 9, NEW_LEVEL_PANEL_ROW);
	}
	else {
		scMapGridDrawCellPanel(9, 19, 16, 5, 0, 9, NEW_LEVEL_PANEL_ROW);
		scMapGridDrawCellPanel(9, 83, 16, 5, 1, 9, NEW_LEVEL_PANEL_ROW);
		scMapGridDrawCellPanel(41, 83, 16, 5, 2, 9, NEW_LEVEL_PANEL_ROW);
	}

	/* Each frame captures the scroll, and the HBlank handler shows the river's
	 */
	/* lines at their new level, more often as the chance climbs to 100%. */
	KolimaForestFloodgate_NewLevelPercent = 0;
	syTaskAddFrame(scKolimaForestFloodgateCaptureNewLevelScroll,
				   TASK_PASS_FRAME);
	sySynch(1);
	syIntFunction(IRQ_HBLANK, 0, scKolimaForestFloodgateChangeRiverLines);
	sySndPlay(SE_WATER_LEVEL);
	KolimaForestFloodgate_NewLevelPercent = 0;
	do {
		sySynch(1);
		nextPercent = KolimaForestFloodgate_NewLevelPercent + 1;
		KolimaForestFloodgate_NewLevelPercent += 1;
	} while(nextPercent <= 100);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	if(*KolimaForestFloodgate_WaterHigh == 0) {
		scMapGridDrawCellPanel(9, 19, 16, 5, *KolimaForestFloodgate_WaterHigh,
							   9, RIVER_PANEL_ROW);
		scMapGridDrawCellPanel(9, 51, 16, 5, 1, 9, RIVER_PANEL_ROW);
		scMapGridDrawCellPanel(41, 51, 16, 5, 2, 9, RIVER_PANEL_ROW);
	}
	else {
		scMapGridDrawCellPanel(9, 19, 16, 5, 0, 9, RIVER_PANEL_ROW);
		scMapGridDrawCellPanel(9, 83, 16, 5, 1, 9, RIVER_PANEL_ROW);
		scMapGridDrawCellPanel(41, 83, 16, 5, 2, 9, RIVER_PANEL_ROW);
	}
	sySynch(1);
	syIntFunction(IRQ_HBLANK, 0, 0);
	sySynch(1);
	syCloseEvent(scKolimaForestFloodgateCaptureNewLevelScroll);
	*KolimaForestFloodgate_WaterHigh ^= 1;
	scKolimaForestFloodgateDrawWaterLevel();
	fdMapGridCenterOnFollowedPoint();
	fdEventEnd();
}

/* Walking west into the upright log on the grass west of Kolima Forest's pool,
 */
/* from its east side (cells (7, 9) to (7, 12)), shoves it two cells west, to */
/* column 4, with Isaac following it a step. */
static void scKolimaForestFloodgatePushBankLogWest(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdEventWait(6);
	sySndPlay(SE_HEAVY_MOVE);
	fdActorSetSpeedAndAcceleration(ACTOR_BANK_LOG, 0.5, 0.2);
	fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_ROLL_EAST);
	fdActorSetDestination(ACTOR_BANK_LOG, 72, 176);
	fdEventWait(6);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.2);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, -8, 0);
	fdEventWait(24);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_BANK_LOG);
	fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_AT_REST);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);
	fdMapGridCopyCellAttributes(5, 9, 1, 4, 6, 9);
	fdMapGridCopyCellAttributes(0, 0, 1, 4, 4, 9);
	*KolimaForestFloodgate_BankLogWest = 1;
	fdEventEnd();
}

/* Walking east into the upright log on the grass west of Kolima Forest's pool,
 */
/* from its west side (cells (3, 9) to (3, 12)), shoves it two cells east, back
 */
/* to column 6, with Isaac following it a step. */
static void scKolimaForestFloodgatePushBankLogEast(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdEventWait(6);
	sySndPlay(SE_HEAVY_MOVE);
	fdActorSetSpeedAndAcceleration(ACTOR_BANK_LOG, 0.5, 0.2);
	fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_ROLL_EAST);
	fdActorSetDestination(ACTOR_BANK_LOG, 104, 176);
	fdEventWait(6);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.2);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
	fdEventWait(24);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_BANK_LOG);
	fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_AT_REST);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);
	fdMapGridCopyCellAttributes(5, 9, 1, 4, 4, 9);
	fdMapGridCopyCellAttributes(0, 0, 1, 4, 6, 9);
	*KolimaForestFloodgate_BankLogWest = 0;
	fdEventEnd();
}

/* How far the curse tints the floodgate until it is lifted: green and blue */
/* lose 1 / FLOODGATE_CURSE_DIVISOR of themselves (scKolimaPaletteCurse). */
#define FLOODGATE_CURSE_DIVISOR 4

/* Sets up the pool on entry: full, with the logs where they start, on a fresh
 */
/* visit, and as the party left it after a battle. */
s32 scInit(void) {
	ACTOR *actor;

	KolimaForestFloodgate_BankLogWest = (u16 *)SceneLocalState;
	KolimaForestFloodgate_WaterHigh = (s16 *)SceneLocalState + 1;
	/* The bank log's place and the water level take the first two halfwords. */
	KolimaForestFloodgate_Logs =
		(FLOODGATE_LOG *)(SceneLocalState + 2 * sizeof(u16));
	fdMapGridCopyCells(32, 0, 64, 32, 0, 64);
	fdMapGridCopyCellAttributes(0, 0, 32, 32, 0, 64);
	fdMapGridCopyCellAttributes(32, 0, 32, 32, 0, 32);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		/* The five logs and the end of the list, six records of three words. */
		syDMAXCopy(KolimaForestFloodgate_DefaultLogs,
				   KolimaForestFloodgate_Logs, DMA_COPY_WORDS(18), REG_DMA3);
		*KolimaForestFloodgate_BankLogWest = 0;
		*KolimaForestFloodgate_WaterHigh = 1;
	}
	scKolimaForestFloodgateCreateLogs(KolimaForestFloodgate_Logs);
	scKolimaForestFloodgateMarkLogCells(KolimaForestFloodgate_DefaultLogs,
										CELL_BEHAVIOR_WALL);
	scKolimaForestFloodgateDrawWaterLevel();
	fdActorSetAnimation(ACTOR_FLOODGATE_SPRAY, 0);
	fdActorGet(ACTOR_FLOODGATE_SPRAY)->motionFlg = 0;
	actor = fdActorGet(ACTOR_SIGN);
	actor->radius = 8;
	actor->scaleX = 3 * FIXED_ONE / 4;
	actor->scaleY = 3 * FIXED_ONE / 4;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
		scKolimaPaletteCurse(FLOODGATE_CURSE_DIVISOR);
	return 0;
}

/* Makes each log at the middle of its four cells, standing on the ground. */
static void scKolimaForestFloodgateCreateLogs(FLOODGATE_LOG *entry) {
	ACTOR *actor;
	FIXED height;
	s32 x;
	s32 z;

	for(; entry->spr != -1; entry++) {
		if(entry->vertical == 0) {
			x = FIELD_CELL_ORIGIN(entry->x) + toFIXED(32);
			z = FIELD_CELL_CENTER(entry->z);
		}
		else {
			x = FIELD_CELL_CENTER(entry->x);
			z = FIELD_CELL_ORIGIN(entry->z) + toFIXED(32);
		}
		actor = syFieldActorCreate(entry->spr, x, 0, z);
		if(actor == NULL)
			return;
		entry->actor = actor;
		fdActorSelectAnimation(actor, LOG_ANIMATION_AT_REST);
		fdActorSetSprFlags(actor, 0);
		actor->collisionFlg = 0;
		actor->radius = 32;
		height = toFIXED(fdMapGridGetTerrainHeight(0, actor->x.part[0].integer,
												   actor->z.part[0].integer));
		actor->y.fixed += height;
		actor->groundHeight = height;
	}
}

/* Writes a cell behavior into the four cells of each log. */
void scKolimaForestFloodgateMarkLogCells(const FLOODGATE_LOG *records,
										 s32 value) {
	const FLOODGATE_LOG *record = records;
	s32 column;
	s32 row;
	s32 i;

	if(record->spr == -1)
		return;
	for(; record->spr != -1; record++) {
		column = record->x;
		row = record->z;
		for(i = 0; i < 4; i++) {
			((MAP_GRID_CELL *)
				 MapGridCellBuffer)[column + (row << MAPGRID_ROW_SHIFT)]
				.behavior = value;
			if(record->vertical == 0)
				column++;
			else
				row++;
		}
	}
}

/* The log lying across the cell, or none. */
static FLOODGATE_LOG *scKolimaForestFloodgateFindLogAt(FLOODGATE_LOG *log,
													   s32 x, s32 y) {
	for(; log->spr != -1; log++) {
		s32 x0 = log->x;
		s32 x1 = x0;
		s32 y0 = log->z;
		s32 y1 = y0;

		if(log->vertical == 0)
			x1 += 3;
		else
			y1 += 3;

		if(x >= x0 && x <= x1 && y >= y0 && y <= y1)
			return log;
	}
	return 0;
}

/* Whether four cells from the one given, across or down, are free for a log: */
/* none is a wall or has a shape other than flat ground. */
static s32 scKolimaForestFloodgateCheckLogFits(s32 x, s32 z, s32 vertical) {
	s32 i;

	for(i = 0; i <= 3; i++) {
		MAP_GRID_CELL *cell =
			&((MAP_GRID_CELL *)MapGridCellBuffer)[x + z * MAPGRID_SIZE];

		if(cell->behavior == CELL_BEHAVIOR_WALL ||
		   *(cell->shape * 4 + MapGridShapes) != 0) {
			return -1;
		}
		if(vertical == 0) {
			x++;
		}
		else {
			z++;
		}
	}
	return 0;
}

/* Finds the log ahead of the leader and rolls it on, a cell at a time, up to */
/* LOG_ROLL_MAX_CELLS cells or until a cell is not free. The leader follows its
 */
/* shove. */
static void scKolimaForestFloodgateRollLog(FLOODGATE_LOG *logs) {
	ACTOR *actor;
	FIELD_STATE *state;
	FLOODGATE_LOG *log;
	s32 point[3];
	s32 facing;
	s32 moved;
	s32 i;
	FIXED x;
	FIXED z;
	s32 quarter;

	moved = 0;
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	facing = FACING_NEAREST_CARDINAL(actor->facing);
	SET_VECTOR(point, (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
			   actor->y.fixed, (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
	syVectorAddPolarOffset(toFIXED(16), facing, point);
	log = scKolimaForestFloodgateFindLogAt(logs, point[0] / toFIXED(16),
										   point[2] / toFIXED(16));
	if(log == NULL)
		return;
	for(i = 0; i < LOG_ROLL_MAX_CELLS; i++) {
		point[0] = FIELD_CELL_ORIGIN(log->x);
		point[2] = FIELD_CELL_ORIGIN(log->z);
		syVectorAddPolarOffset(toFIXED(16), facing, point);
		if(scKolimaForestFloodgateCheckLogFits(point[0] / toFIXED(16),
											   point[2] / toFIXED(16),
											   log->vertical) != 0)
			break;
		moved = 1;
		if(log->vertical == 0) {
			x = point[0] + toFIXED(32);
			z = point[2] + toFIXED(8);
		}
		else {
			x = point[0] + toFIXED(8);
			z = point[2] + toFIXED(32);
		}
		log->x = point[0] / toFIXED(16);
		log->z = point[2] / toFIXED(16);
	}
	if(moved == 0)
		return;
	SET_VECTOR(point, (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
			   actor->y.fixed, (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
	syVectorAddPolarOffset(toFIXED(8), facing, point);
	actor = log->actor;
	quarter = facing / (DEGtoANG(90));
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdEventWait(6);
	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 5;
	sySndPlay(SE_HEAVY_MOVE);
	fdActorSelectAnimation(actor,
						   KolimaForestFloodgate_PushAnimations[quarter]);
	syFieldActorSetMoveTargetFX(actor, x, 0, z);
	fdEventWait(6);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	state = syHeapSlotAllocEwram(HEAP_SLOT_FIELD, sizeof(*state));
	fdActorStartCamera(state->camera, (s32)actor);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.2);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER,
								KolimaForestFloodgate_PushStepX[quarter],
								KolimaForestFloodgate_PushStepZ[quarter]);
	fdEventWait(24);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorWaitForMove(actor);
	fdActorSelectAnimation(actor, LOG_ANIMATION_AT_REST);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);
	fdEventWait(15);
	fdEventEnd();
}

#include "shared/kolima_palette.c"

/* The logs on a first visit, as FLOODGATE_LOG, each in the sprite of the
 */
/* way it runs: one sprite for north to south, another for west to east. */
/* clang-format off */
FLOODGATE_LOG KolimaForestFloodgate_DefaultLogs[] = {
    {SPRITE_LOG_NORTH_SOUTH, 11, 9, 1, NULL},
    {SPRITE_LOG_WEST_EAST, 12, 12, 0, NULL},
    {SPRITE_LOG_WEST_EAST, 17, 12, 0, NULL},
    {SPRITE_LOG_NORTH_SOUTH, 13, 13, 1, NULL},
    {SPRITE_LOG_WEST_EAST, 10, 18, 0, NULL},
    {-1},
};

/* For each quarter turn of the way a log is pushed: its rolling animation, and */
/* the leader's step after it in pixels. */
u8 KolimaForestFloodgate_PushAnimations[] = {
    LOG_ANIMATION_ROLL_EAST,
    LOG_ANIMATION_ROLL_EAST,
    LOG_ANIMATION_ROLL_WEST,
    LOG_ANIMATION_ROLL_WEST,
};
s8 KolimaForestFloodgate_PushStepX[] = {8, 0, -8, 0};
s8 KolimaForestFloodgate_PushStepZ[] = {0, 8, 0, -8};

SCENE_ENTRANCE KolimaForestFloodgateEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 407, 0, 276, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 50, 32, 369, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 162, 32, 84, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 KolimaForestFloodgateExits[] = {
    SCENE_EXITS(KolimaForestFloodgate),
    SCENE_EXIT(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE),
    SCENE_EXIT(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE),
    SCENE_EXITS_END,
};

ACTOR_PLACE KolimaForestFloodgateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        104, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        256, 0, 464, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        348, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
/* clang-format on */

TRIGGER KolimaForestFloodgateTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS,
	 KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS,
	 KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
	 CELL_BEHAVIOR_KOLIMA_FOREST_NORTH_BANK_JUMP, CONDITION_ALWAYS,
	 EVENT(scKolimaForestFloodgateJumpSouth)},
	{TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
	 CELL_BEHAVIOR_KOLIMA_FOREST_RIVERBED, CONDITION_ALWAYS,
	 EVENT(scKolimaForestFloodgatePushLog)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST),
	 CELL_BEHAVIOR_KOLIMA_FOREST_EAST_OF_BANK_LOG, CONDITION_ALWAYS,
	 EVENT(scKolimaForestFloodgatePushBankLogWest)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST),
	 CELL_BEHAVIOR_KOLIMA_FOREST_WEST_OF_BANK_LOG, CONDITION_ALWAYS,
	 EVENT(scKolimaForestFloodgatePushBankLogEast)},
	{TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS,
	 MSGNO(MsgKolimaDoNotTouchFloodgate)},
	{EXAMINE_TRIGGER(EXAMINE_SILENT),
	 CELL_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH, CONDITION_ALWAYS,
	 EVENT(scKolimaForestFloodgateFlipFloodgateSwitch)},
	{EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_KOLIMA_FOREST_FUR_COAT,
	 FIND_ITEM(ITEM_FUR_COAT)},
	{TRIGGER_TABLE_END},
};
