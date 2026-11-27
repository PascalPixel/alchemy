/* @file */
/* The main routine and scene changes: exits, returns from battle, entering. */

#include "stubs.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/event.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"

/* The scene of a table's first exit word, the destination when no exit */
/* matches: nine bits, where EXIT_SCENE_MASK takes twelve. */
#define EXIT_DEFAULT_SCENE_MASK 0x1ff

/* Chooses the scene and entrance to go to when the party leaves the current */
/* scene. exitId is the kind of exit fdSequenceRun ended on; the scene's */
/* exit table gives the first exit of that kind (or of any kind) whose flag, if
 */
/* it has one, is not set. */
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
	exitTable = OverlayArea.exits();
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
				if(flag == 0 || gmFlagGetStub(flag) == 0) {
					destinationScene = sceneId;
					destinationEntrance = entrance;
					break;
				}
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
/* sequence (see gmMainRoutineRun). result is its result: -1 when the party was
 */
/* defeated, which revives the leader with 1 HP (and, with FLAG_CHILDHOOD, */
/* fully heals Isaac and Garet) and returns to the story battle's defeat point
 */
/* (fdEventSetDefeatDestination) or, when none is set, to the restart point, */
/* usually the last Sanctum (scSetRestartPoint). */
void scReturnFromSequence(s32 result) {
	UNIT *unit;
	s32 i;
	s32 sceneId;
	s32 entrance;

	MainRoutineStateWork.sequenceResult = result;
	if(result == -1) {
		/* Revive the leader before choosing the restart point. */
		unit = gmUnitGetStub(MainRoutineStateWork.leaderActorId);
		if(unit->hp == 0) {
			unit->hp = 1;
			gmUnitUpdateGauges(unit);
		}
		if(gmFlagGetStub(FLAG_CHILDHOOD)) {
			for(i = 0; i < 2; i++) {
				unit = gmUnitGetStub(i);
				unit->hp = unit->hpMax;
				unit->pp = unit->ppMax;
				gmUnitUpdateGauges(unit);
			}
		}
		sceneId = MainRoutineStateWork.defeatScene;
		entrance = MainRoutineStateWork.defeatEntrance;
		if(sceneId == -1 && entrance == -1) {
			MainRoutineStateWork.next = MainRoutineStateWork.restartScene;
			MainRoutineStateWork.entrance =
				MainRoutineStateWork.restartEntrance;
		}
		else {
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
			gmFlagSetStub(FLAG_KEEP_PARTY_POSITION);
		}
	}
}

/* Returns the area sceneId belongs to. The scenes of one area share their */
/* music, area name and the flags kept while inside it. */
s32 scGetArea(s32 sceneId) { return ScenesWork[sceneId].area; }

/* The last two 16-colour background palettes, which the minigame runners save
 */
/* and restore around a minigame; palette 15 holds the window colours */
/* (mnWindowColApply). */
enum {
	MINIGAME_PALETTE_FIRST = 14, /* BG palette 14 */
	MINIGAME_PALETTE_BYTES =
		64 /* palettes 14 and 15: 2 x 16 colours x 2 bytes */
};

/* Each runs a minigame's sequence, preserving BG palettes 14 and 15 in a */
/* 64-byte syHeapAlloc copy freed afterwards, and returns its start function's
 */
/* result, 0, for scReturnFromSequence. */
#define MAIN_ROUTINE_DEFINE_RUN_MINIGAME(name, start)                          \
	static __inline__ s32 name(s32 entrance) {                                 \
		void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);     \
		void *saved = syHeapAlloc(MINIGAME_PALETTE_BYTES);                     \
		s32 result;                                                            \
                                                                               \
		syDMAXCopy(palette, saved, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4), \
				   REG_DMA3);                                                  \
		result = start(entrance);                                              \
		syDMAXCopy(saved, palette, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4), \
				   REG_DMA3);                                                  \
		syHeapRelease(saved);                                                  \
		return result;                                                         \
	}

/* Runs the dice sequence (Lucky Dice). */
MAIN_ROUTINE_DEFINE_RUN_MINIGAME(gmMainRoutineRunDice, fdLuckyDiceStartStub)
/* Runs the slots sequence (Lucky Wheels). */
MAIN_ROUTINE_DEFINE_RUN_MINIGAME(gmMainRoutineRunSlots, fdLuckyWheelsStartStub)

/* The main routine: runs one sequence after another and never returns. mode, */
/* an GAME_START, chooses the first scene. */
void gmMainRoutineRun(s32 mode) {
	const SCENE *table;
	const SCENE *record;
	s32 entrance;
	s32 debug;

	table = ScenesWork;
	/* Choose the first scene before initializing the per-scene runtime. */
	/* Read once: both debug starts test the same value. */
	debug = DebugMode;
	if(debug && mode == GAME_START_DEBUG_VALE) {
		MainRoutineStateWork.next = SCENE_VALE_PLAZA;
		/* Entrance 1, the value mode holds here. */
		MainRoutineStateWork.entrance = mode;
	}
	else if(debug && mode == GAME_START_DEBUG_FILE_SCREEN) {
		MainRoutineStateWork.next = SCENE_FILE_SCREEN;
		MainRoutineStateWork.entrance = FILE_SCREEN_FROM_SANCTUM;
	}
	else {
		gmMainRoutineStateInitDefaultsStub();
		MainRoutineStateWork.next = SCENE_TITLE;
		MainRoutineStateWork.entrance = TITLE_AFTER_RUN;
	}
	/* The window colours from the options: hue, then brightness. */
	mnWindowColApplyStub(MainRoutineStateWork.winHue,
						 MainRoutineStateWork.winBrightness);
	syVramCacheReset();
	syInitEvent();
	/* DEAD: a second reset of the empty task table; the ROM keeps its call. */
	syInitEvent();
	for(;;) {
		/* FLAG_SKIP_SCENE_CHANGE_SOUND skips SE_STOP_ACTION_SE for one scene */
		/* change, then clears itself. */
		if(gmFlagGetStub(FLAG_SKIP_SCENE_CHANGE_SOUND))
			gmFlagClearStub(FLAG_SKIP_SCENE_CHANGE_SOUND);
		else
			sySndPlayStub(SE_STOP_ACTION_SE);
		record = &table[MainRoutineStateWork.next];
		entrance = MainRoutineStateWork.entrance;

		syDma0Stop();
		/* Drop the tasks, the HBlank and VCount handlers (IRQs 1 and 2) and
		 * every */
		/* heap block the last scene, battle or minigame left. */
		syInitEvent();
		syIntFunction(IRQ_HBLANK, 0, 0);
		syIntFunction(IRQ_VCOUNT, 0, 0);
		syHeapInit();
		syDebugTextClear();
		syVramCacheReset();
		/* The battle, dice and slots sequences return a result, not an exit:
		 * the */
		/* return scene is chosen from the main routine's state, and no overlay
		 * is */
		/* loaded. */
		if(MainRoutineStateWork.next >= SEQUENCE_NONE) {
			switch(MainRoutineStateWork.next) {
			case SEQUENCE_BATTLE:
				entrance = btRunStub(entrance);
				break;
			case SEQUENCE_DICE:
				entrance = gmMainRoutineRunDice(entrance);
				break;
			case SEQUENCE_SLOTS:
				entrance = gmMainRoutineRunSlots(entrance);
				break;
			case SEQUENCE_NONE:
				entrance = 0;
				break;
			}
			scReturnFromSequence(entrance);
			continue;
		}

		/* A scene: set its flags and music, load its code and run it. */
		scArrive(MainRoutineStateWork.next,
				 gmFlagGetStub(FLAG_KEEP_PARTY_POSITION));
		scChooseMusic();
		if(!gmFlagGetStub(FLAG_KEEP_PARTY_POSITION)) {
			/* Either flag keeps the music that is playing: FLAG_KEEP_MUSIC_ONCE
			 * for */
			/* this one change, FLAG_KEEP_MUSIC_HELD for as long as it stays
			 * set. */
			if(gmFlagGetStub(FLAG_KEEP_MUSIC_ONCE) ||
			   gmFlagGetStub(FLAG_KEEP_MUSIC_HELD))
				gmFlagClearStub(FLAG_KEEP_MUSIC_ONCE);
			else
				scPlayMusic();
		}
		else if(MainRoutineStateWork.returnMusic != -1) {
			/* Back where the party was: resume the music saved when the Pause
			 * menu */
			/* last opened (field_sequence.c). */
			sySndPlayStub(MainRoutineStateWork.returnMusic);
		}
		else {
			scPlayMusic();
		}
		MainRoutineStateWork.mapId = record->mapIdx;
		scLoadOverlay(0);
		scTakeExit(fdSequenceRun(entrance));
	}
}

/* Loads the current scene's overlay code (its resource) into OverlayArea. */
void scLoadOverlay(s32 unused) {
	const SCENE *table;

	table = ScenesWork;
	syOverlayLoad(table[MainRoutineStateWork.next].resourceId, &OverlayArea);
}

/* The scene id's row of ScenesWork. */
#define SCENE_ROW(id) (&ScenesWork[id])

/* FLAG_AREA_VISITED has a flag for each of 128 areas. */
#define AREA_VISITED_MASK 0x7f

/* Updates the flags and state that last for one scene or one area, on entering
 */
/* the current scene. */
void scArrive(s32 unused, s32 keep) {
	s32 area = SCENE_ROW(MainRoutineStateWork.next)->area;
	s32 flag;

	if(keep == 0) {
		/* Local puzzle flags expire on every normal entry to a scene. */
		for(flag = FLAG_SCENE_TEMP_FIRST; flag <= FLAG_SCENE_TEMP_LAST; flag++)
			gmFlagClearStub(flag);
		if(area != MainRoutineStateWork.area) {
			/* Area state and Retreat's destination survive moves within it. */
			for(flag = FLAG_AREA_TEMP_FIRST; flag <= FLAG_AREA_TEMP_LAST;
				flag++)
				gmFlagClearStub(flag);
			gmFlagSetStub(FLAG_SHOW_AREA_NAME);
			MainRoutineStateWork.encounterSteps = 0;
			MainRoutineStateWork.hazardSteps = 0;
			gmFlagClearStub(FLAG_IN_VENUS_LIGHTHOUSE);
			gmFlagClearStub(FLAG_IN_MERCURY_LIGHTHOUSE);
			gmFlagClearStub(FLAG_IN_MARS_LIGHTHOUSE);
			gmFlagClearStub(FLAG_IN_JUPITER_LIGHTHOUSE);
			MainRoutineStateWork.retreatScene = MainRoutineStateWork.next;
			MainRoutineStateWork.retreatEntrance =
				MainRoutineStateWork.entrance;
		}
		/* The Djinn's hiding flags last for one scene, even within an area, */
		/* and so do the 16 after them to FLAG_ARRIVAL_CLEARED_LAST, which
		 * nothing */
		/* else uses. */
		for(flag = FLAG_DJINN_HIDING_FIRST; flag <= FLAG_ARRIVAL_CLEARED_LAST;
			flag++)
			gmFlagClearStub(flag);
		gmFlagClearStub(FLAG_LINK_BATTLE);
		gmFlagClearStub(FLAG_RETREAT_BLOCKED);
		/* FLAG_NO_ENCOUNTERS_IN_AREA stops random battles */
		/* (fdEncounterCountStepsAndSelectFormation). */
		gmFlagClearStub(FLAG_NO_ENCOUNTERS_IN_AREA);
		gmFlagClearStub(FLAG_IN_TOWN);
		gmFlagClearStub(FLAG_WORLD_MAP_LEFT);
		MainRoutineStateWork.returnMusic = -1;
	}
	MainRoutineStateWork.area = area;
	gmFlagSetStub(FLAG_AREA_VISITED(area & AREA_VISITED_MASK));
	MainRoutineStateWork.sceneType =
		SCENE_ROW(MainRoutineStateWork.next)->sceneType;
	if(MainRoutineStateWork.sceneType == SCENE_TYPE_TOWN)
		gmFlagSetStub(FLAG_IN_TOWN);
	gmFlagRefreshLureCapStub();
}

/* Starts the current scene's field music, chosen by scChooseMusic. */
void scPlayMusic(void) { sySndPlayStub(MainRoutineStateWork.sceneMusic); }

#undef EXIT_DEFAULT_SCENE_MASK
#undef AREA_VISITED_MASK
#undef SCENE_ROW
#undef MAIN_ROUTINE_DEFINE_RUN_MINIGAME
