/* @file */
/* The main routine and scene changes: exits, returns from a sequence, */
/* entering a scene. As The Broken Seal's main_routine.c, in the same order; */
/* in module 04 it follows the field sequence's picture decoders. */

/* This unit declares its own form of fdSequenceRun, below. */
#define OWN_FieldSequence_Run

#include "flags.h"
#include "field/field_sequence.h"
#include "field/scene_tables.h"
#include "sounds.h"
#include "battle/battle_unit.h"
#include "field/field.h"
#include "game/stubs.h"
#include "field/lucky_dice_stubs.h"
#include "window/stubs.h"

/* The ROM's caller here is compiled against this form, which takes the */
/* entrance where the definition takes nothing. */
s32 fdSequenceRun(s32 entrance);
/* The scene overlay's exit table: a header word holding a scene id, then that
 */
/* scene's exits, one packed word each (and a flag word after it when */
/* EXIT_HAS_FLAG is set). Scene id SCENE_EXITS_END ends the exits of a scene and
 */
/* the whole table. */

/* The scene of a table's first exit word, the destination when no exit */
/* matches: nine bits, where EXIT_SCENE_MASK takes twelve. */
#define EXIT_DEFAULT_SCENE_MASK 0x1ff

/* Chooses the scene and entrance to go to when the party leaves the current */
/* scene. exitId is the kind of exit fdSequenceRun ended on; the scene's */
/* exit table gives the first exit of that kind (or of any kind) whose flag, if
 */
/* it has one, is not set. As The Broken Seal's, where a flag word of -1 also */
/* takes the exit. */
void scTakeExit(s32 exitId) {
	s16 currentScene;
	const u32 *exitTable;
	const u32 *firstExit;
	u32 word;
	s32 sceneId;
	s32 destinationScene;
	s32 destinationEntrance;
	s32 entrance;
	s32 kind;
	s32 flag;

	currentScene = MainRoutineStateWork.next;
	exitTable = ((SCENE_ENTRY_TABLE *)OverlayArea)->exits();
	destinationScene = EXIT_KEEP_DESTINATION;
	destinationEntrance = 0;
	if(exitId == EXIT_KEEP_DESTINATION)
		return;

	for(;;) {
		/* Find the header of the current scene: exit words have high bits set.
		 */
		sceneId = *exitTable++;
		if(sceneId & ~EXIT_SCENE_MASK)
			continue;
		sceneId &= EXIT_SCENE_MASK;
		if(sceneId == SCENE_EXITS_END)
			break;
		if(sceneId != currentScene)
			continue;

		firstExit = exitTable;
		for(;;) {
			word = *exitTable++;
			sceneId = word & EXIT_SCENE_MASK;
			entrance = ENTRANCE(word);
			kind = EXIT_ID(word);
			flag = word & EXIT_HAS_FLAG;
			if(flag)
				flag = *exitTable++;
			if(sceneId == SCENE_EXITS_END || kind == 0) {
				destinationScene = *firstExit & EXIT_DEFAULT_SCENE_MASK;
				break;
			}
			if(kind == EXIT_ID_ANY || kind == exitId) {
				if(flag != 0) {
					if(flag != -1) {
						if(syGameFlagGet(flag))
							continue;
					}
				}
				destinationScene = sceneId;
				destinationEntrance = entrance;
				break;
			}
		}
		break;
	}

	/* Commit the chosen exit only after a matching scene was found. */
	if(destinationScene != EXIT_KEEP_DESTINATION) {
		MainRoutineStateWork.next = destinationScene;
		MainRoutineStateWork.entrance = destinationEntrance;
	}
}

/* Chooses where the game continues after a sequence other than the field */
/* sequence (see gmMainRoutineRun). `result` is its result: BATTLE_OUTCOME_LOST
 */
/* when the party was defeated, which revives the leader with 1 HP and returns
 */
/* to the story battle's defeat point (fdEventSetDefeatDestination, */
/* defeatScene) or, when none is set, to the restart point, usually the last */
/* Sanctum (scSetRestartPoint, restartScene); the world map is then told */
/* the scene was left (fdWorldMapSetReturnLeavingScene). As The Broken Seal's,
 */
/* with no childhood healing. */
void scReturnFromSequence(s32 result) {
	UNIT *unit;
	s16 sceneId;
	s16 entrance;

	MainRoutineStateWork.sequenceResult = result;
	if(result == BATTLE_OUTCOME_LOST) {
		/* Revive the leader before choosing the restart point. */
		unit = syUnitGet(MainRoutineStateWork.leaderActorId);
		if(unit->hp == 0) {
			unit->hp = 1;
			gmUnitUpdateGauges(unit);
		}
		sceneId = MainRoutineStateWork.defeatScene;
		entrance = MainRoutineStateWork.defeatEntrance;
		if(sceneId != -1 || entrance != -1) {
			if(sceneId != -1)
				MainRoutineStateWork.next = sceneId;
			else
				MainRoutineStateWork.next = MainRoutineStateWork.returnScene;
			if(entrance != -1)
				MainRoutineStateWork.entrance = entrance;
			else
				MainRoutineStateWork.entrance =
					MainRoutineStateWork.returnEntrance;
		}
		else {
			MainRoutineStateWork.next = MainRoutineStateWork.restartScene;
			MainRoutineStateWork.entrance =
				MainRoutineStateWork.restartEntrance;
		}
		fdWorldMapSetReturnLeavingScene(MainRoutineStateWork.next,
										MainRoutineStateWork.entrance);
	}
	else {
		/* An after-battle destination an event set overrides the return point.
		 */
		sceneId = MainRoutineStateWork.afterBattleScene;
		entrance = MainRoutineStateWork.afterBattleEntrance;
		if(sceneId != -1 || entrance != -1) {
			if(sceneId != -1)
				MainRoutineStateWork.next = sceneId;
			else
				MainRoutineStateWork.next = MainRoutineStateWork.returnScene;
			if(entrance != -1)
				MainRoutineStateWork.entrance = entrance;
			else
				MainRoutineStateWork.entrance =
					MainRoutineStateWork.returnEntrance;
		}
		else {
			MainRoutineStateWork.next = MainRoutineStateWork.returnScene;
			MainRoutineStateWork.entrance = MainRoutineStateWork.returnEntrance;
			syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
		}
	}
}

/* Returns the area sceneId belongs to. The scenes of one area share their */
/* music, area name and the flags kept while inside it. */
s32 scGetArea(s32 sceneId) { return ScenesWork[sceneId].area; }

/* Returns the scene's attributes, the last halfword of its ScenesWork row, */
/* which The Broken Seal leaves 0; the field sequence splits it into two */
/* nibbles. */
s16 gmFieldGetSceneAttributes(s32 sceneId) {
	return ScenesWork[sceneId].attributes;
}

/* The last two 16-colour background palettes, which the minigame runners save
 */
/* and restore around a minigame; palette 15 holds the window colours */
/* (mnWindowColApply). */
enum {
	MINIGAME_PALETTE_FIRST = 14, /* BG palette 14 */
	MINIGAME_PALETTE_BYTES =
		64 /* palettes 14 and 15: 2 x 16 colours x 2 bytes */
};

/* Runs a minigame's sequence, preserving BG palettes 14 and 15 in a 64-byte */
/* syHeapAlloc copy freed afterwards, and stores its start function's result */
/* for scReturnFromSequence. The Broken Seal writes the same as an inline */
/* function for each minigame. */
#define MAIN_ROUTINE_RUN_MINIGAME(start)                                       \
	{                                                                          \
		void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);     \
		void *saved = syHeapAlloc(MINIGAME_PALETTE_BYTES);                     \
                                                                               \
		syDMAXCopy(palette, saved, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4), \
				   REG_DMA3);                                                  \
		entrance = start(entrance);                                            \
		syDMAXCopy(saved, palette, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4), \
				   REG_DMA3);                                                  \
		syHeapRelease(saved);                                                  \
	}

/* The debug start's entrance into Venus Lighthouse's grounds: the path south */
/* of the plaza, which venus_lighthouse_entrance.c calls ENTRANCE_PLAZA_PATH. */
#define DEBUG_VENUS_LIGHTHOUSE_START 10
/* The debug start's entrance into Mt. Aleph's summit, one no stretch of the */
/* ending uses: Alex stands on the summit while B is held, and the staff */
/* credits' opening follows. */
#define DEBUG_MT_ALEPH_START 1

/* The main routine: runs one sequence after another and never returns. mode, */
/* an GAME_START, chooses the first scene. As The Broken Seal's, with this
 */
/* game's four debug starts, rendering re-enabled after DMA 0 stops, and three
 */
/* minigames, Super Lucky Dice the third. */
void gmMainRoutineRun(s32 mode) {
	const SCENE *table;
	const SCENE *record;
	s32 entrance;

	table = ScenesWork;
	/* Choose the first scene before initializing the per-scene runtime. */
	if(DebugMode && mode == GAME_START_DEBUG_VENUS_LIGHTHOUSE) {
		MainRoutineStateWork.next = SCENE_VENUS_LIGHTHOUSE_ENTRANCE;
		MainRoutineStateWork.entrance = DEBUG_VENUS_LIGHTHOUSE_START;
	}
	else if(DebugMode && mode == GAME_START_DEBUG_CLEAR) {
		MainRoutineStateWork.next = SCENE_CLEAR;
		MainRoutineStateWork.entrance = CLEAR_FROM_TITLE;
	}
	else if(DebugMode && mode == GAME_START_DEBUG_MT_ALEPH) {
		MainRoutineStateWork.next = SCENE_MT_ALEPH_SUMMIT;
		MainRoutineStateWork.entrance = DEBUG_MT_ALEPH_START;
	}
	else if(DebugMode && mode == GAME_START_DEBUG_TITLE) {
		MainRoutineStateWork.next = SCENE_TITLE;
		MainRoutineStateWork.entrance = TITLE_STORY_CRAWL;
	}
	else {
		gmMainRoutineStateInitDefaultsStub();
		MainRoutineStateWork.next = SCENE_TITLE;
		MainRoutineStateWork.entrance = TITLE_START;
	}
	/* The window colours from the options: hue, then brightness. */
	mnWindowColApplyStub(MainRoutineStateWork.winHue,
						 MainRoutineStateWork.winBrightness);
	syVramCacheReset();
	syInitEvent();
	syInitEvent();
	for(;;) {
		/* FLAG_SKIP_SCENE_CHANGE_SOUND skips SE_STOP_ACTION_SE (The Broken
		 * Seal's */
		/* SE_STOP_ACTION_SE) for one scene change, then clears itself. */
		if(syGameFlagGet(FLAG_SKIP_SCENE_CHANGE_SOUND))
			syGameFlagClear(FLAG_SKIP_SCENE_CHANGE_SOUND);
		else
			sySndPlayStub(SE_STOP_ACTION_SE);
		record = &table[MainRoutineStateWork.next];
		entrance = MainRoutineStateWork.entrance;

		DMA0_STOP();
		OamCopyEnabled = 1;
		/* Drop the tasks, the HBlank and VCount handlers and every heap block
		 * the */
		/* last scene, battle or minigame left. */
		syInitEvent();
		syIntFunction(IRQ_HBLANK, 0, 0);
		syIntFunction(IRQ_VCOUNT, 0, 0);
		syHeapInit();
		syDebugTextClear();
		syVramCacheReset();
		/* The battle and the three minigames return a result, not an exit: the
		 */
		/* return scene is chosen from the main routine's state, and no overlay
		 * is */
		/* loaded. */
		if(MainRoutineStateWork.next >= SEQUENCE_SUPER_DICE) {
			switch(MainRoutineStateWork.next) {
			case SEQUENCE_BATTLE:
				entrance = btRunStub(entrance);
				break;
			case SEQUENCE_DICE:
				MAIN_ROUTINE_RUN_MINIGAME(fdLuckyDiceStartStub);
				break;
			case SEQUENCE_SLOTS:
				MAIN_ROUTINE_RUN_MINIGAME(fdLuckyWheelsStartStub);
				break;
			case SEQUENCE_SUPER_DICE:
				MAIN_ROUTINE_RUN_MINIGAME(fdSuperLuckyDiceStartStub);
				break;
			}
			scReturnFromSequence(entrance);
			continue;
		}

		/* A scene: set its flags and music, load its code and run it. */
		scArrive(MainRoutineStateWork.next,
				 syGameFlagGet(FLAG_KEEP_PARTY_POSITION));
		scChooseMusic();
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			/* Either flag keeps the music that is playing: FLAG_KEEP_MUSIC_ONCE
			 * for */
			/* this one change, FLAG_KEEP_MUSIC_HELD for as long as it stays
			 * set. */
			if(syGameFlagGet(FLAG_KEEP_MUSIC_ONCE) ||
			   syGameFlagGet(FLAG_KEEP_MUSIC_HELD))
				syGameFlagClear(FLAG_KEEP_MUSIC_ONCE);
			else
				scPlayMusic();
		}
		else if(MainRoutineStateWork.returnMusic != -1) {
			/* Back where the party was: resume the music saved when the Pause
			 * menu */
			/* last opened (field/field_sequence_run.c). */
			sySndPlayStub(MainRoutineStateWork.returnMusic);
		}
		else {
			scPlayMusic();
		}
		MainRoutineStateWork.mapId = record->mapIdx;
		scLoadOverlay(0);
		entrance = fdSequenceRun(entrance);
		scTakeExit(entrance);
	}
}

/* Loads the current scene's overlay code (its resource) into OverlayArea. */
void scLoadOverlay(s32 unused) {
	const SCENE *table;

	table = ScenesWork;
	syOverlayLoad(table[MainRoutineStateWork.next].resourceId, OverlayArea);
}

/* The scene's row of ScenesWork. */
#define MAP_HEADER_AT(id) (&ScenesWork[id])

/* Updates the flags and state that last for one scene or one area, on entering
 */
/* the current scene. As The Broken Seal's, with this game's flags, and */
/* fdWorldMapMarkPlaceVisited in place of the area's visited flag. */
void scArrive(s32 unused, s32 keep) {
	s32 area = MAP_HEADER_AT(MainRoutineStateWork.next)->area;
	s32 flag;

	if(keep == 0) {
		/* Local puzzle flags expire on every normal entry to a scene. */
		for(flag = FLAG_SCENE_TEMP_FIRST; flag <= FLAG_SCENE_TEMP_LAST; flag++)
			syGameFlagClear(flag);
		if(area != MainRoutineStateWork.area) {
			/* Area state and Retreat's destination survive moves within it. */
			for(flag = FLAG_AREA_TEMP_FIRST; flag <= FLAG_AREA_TEMP_LAST;
				flag++)
				syGameFlagClear(flag);
			syGameFlagSet(FLAG_SHOW_AREA_NAME);
			MainRoutineStateWork.encounterSteps = 0;
			MainRoutineStateWork.hazardSteps = 0;
			syGameFlagClear(FLAG_IN_VENUS_LIGHTHOUSE);
			syGameFlagClear(FLAG_IN_MERCURY_LIGHTHOUSE);
			syGameFlagClear(FLAG_IN_MARS_LIGHTHOUSE);
			syGameFlagClear(FLAG_IN_JUPITER_LIGHTHOUSE);
			MainRoutineStateWork.retreatScene = MainRoutineStateWork.next;
			MainRoutineStateWork.retreatEntrance =
				MainRoutineStateWork.entrance;
		}
		/* The Djinn's hiding flags last for one scene, even within an area, */
		/* and so do the flags after them to FLAG_ARRIVAL_CLEARED_LAST. */
		for(flag = FLAG_DJINN_HIDING_FIRST; flag <= FLAG_ARRIVAL_CLEARED_LAST;
			flag++)
			syGameFlagClear(flag);
		syGameFlagClear(FLAG_LINK_BATTLE);
		syGameFlagClear(FLAG_RETREAT_BLOCKED);
		/* FLAG_NO_ENCOUNTERS_IN_AREA stops random battles. */
		syGameFlagClear(FLAG_NO_ENCOUNTERS_IN_AREA);
		syGameFlagClear(FLAG_IN_TOWN);
		syGameFlagClear(FLAG_WORLD_MAP_LEFT);
		syGameFlagClear(FLAG_SAND_CANNOT_END);
		syGameFlagClear(FLAG_CANNOT_SAVE_HERE);
		syGameFlagClear(FLAG_NO_PLACE_TO_SAVE);
		syGameFlagClear(FLAG_IN_BATTLE_ARENA);
		if(MainRoutineStateWork.ownerOverride != 0)
			syGameFlagSet(FLAG_SAILING);
		else
			syGameFlagClear(FLAG_SAILING);
		MainRoutineStateWork.returnMusic = -1;
	}
	MainRoutineStateWork.area = area;
	fdWorldMapMarkPlaceVisited(MainRoutineStateWork.next,
							   MainRoutineStateWork.entrance);
	MainRoutineStateWork.sceneType =
		MAP_HEADER_AT(MainRoutineStateWork.next)->sceneType;
	if(MainRoutineStateWork.sceneType == SCENE_TYPE_TOWN)
		syGameFlagSet(FLAG_IN_TOWN);
	syGameFlagRefreshLureCapStub();
}

/* Starts the current scene's field music, chosen by scChooseMusic. */
void scPlayMusic(void) { sySndPlayStub(MainRoutineStateWork.sceneMusic); }

#undef MAIN_ROUTINE_RUN_MINIGAME
#undef MAP_HEADER_AT
