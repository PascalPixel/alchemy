/* Choosing a battle target. Part of screens.c. */

/* Widths in tiles that fit each language's text: the "Set Djinni" window
   (MsgTargetSetDjinni) and the list of conditions an action cures; and the
   Mystery names' copy, which Japanese decodes raw where the other languages
   copy it expanded. */
#define TARGET_SET_DJINNI_WIDTH		JAPANESE_OR(8, 9)
#define TARGET_CONDITION_LIST_WIDTH JAPANESE_OR(14, 16)
#define TARGET_MYSTERY_NAME_COPY \
	JAPANESE_OR(mnMessageDecodeRaw, mnMessageCopyExpanded)
#define TARGET_MYSTERY_NAME_CAPACITY JAPANESE_OR(10, 14)

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
/* These editions size the windows by the cartridge's language as it runs: wider
   for Italian and Spanish. */
#define TARGET_SET_DJINNI_WIDTH_ITALIAN		10
#define TARGET_SET_DJINNI_WIDTH_SPANISH		11
#define TARGET_CONDITION_LIST_WIDTH_SPANISH 17
#endif

/* The HP and PP window's width, and the one-line windows' of a revival or a
   cure of poison. */
#define TARGET_GAUGE_WIDTH	  13
#define TARGET_ONE_LINE_WIDTH 12

/* The help window's column, `back` tiles left of the aimed party member's; a
   window `windowWidth` tiles wide that would run past the screen's right edge
   stands against it instead. */
#define TARGET_HELP_COLUMN(back, windowWidth) \
	{ \
		Sint32 anchor = selectedPosition.x / 8; \
\
		helpColumn = anchor - (back); \
		if(anchor + (windowWidth) - (back) > DISPLAY_TILE_WIDTH - 1) \
			helpColumn = DISPLAY_TILE_WIDTH - (windowWidth); \
	}

/* The window listing the conditions an action cures, helpRow down and `count`
   lines tall. */
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
#define TARGET_CONDITION_LIST_WINDOW() \
	{ \
		Sint32 anchor = selectedPosition.x / 8; \
\
		helpColumn = anchor - 7; \
		if(syGetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) { \
			if(anchor + TARGET_CONDITION_LIST_WIDTH_SPANISH - 7 > \
				DISPLAY_TILE_WIDTH - 1) \
				helpColumn = \
					DISPLAY_TILE_WIDTH - TARGET_CONDITION_LIST_WIDTH_SPANISH; \
			conditionWindow = mnWindowCreate(helpColumn, helpRow, \
				TARGET_CONDITION_LIST_WIDTH_SPANISH, count + 2, \
				MENU_WINDOW_FLAGS); \
		} else { \
			if(anchor + TARGET_CONDITION_LIST_WIDTH - 7 > \
				DISPLAY_TILE_WIDTH - 1) \
				helpColumn = DISPLAY_TILE_WIDTH - TARGET_CONDITION_LIST_WIDTH; \
			conditionWindow = mnWindowCreate(helpColumn, helpRow, \
				TARGET_CONDITION_LIST_WIDTH, count + 2, MENU_WINDOW_FLAGS); \
		} \
	}
#else
#define TARGET_CONDITION_LIST_WINDOW() \
	{ \
		TARGET_HELP_COLUMN(7, TARGET_CONDITION_LIST_WIDTH); \
		conditionWindow = mnWindowCreate(helpColumn, helpRow, \
			TARGET_CONDITION_LIST_WIDTH, count + 2, MENU_WINDOW_FLAGS); \
	}
#endif

/* The enemy name window, the screen's width from row 12, and the pixels of its
   interior the name is kept within. */
#define TARGET_NAME_WINDOW_ROW	   12
#define TARGET_NAME_INTERIOR_WIDTH ((DISPLAY_TILE_WIDTH - 2) * 8)

/* Where the battle view slides while a target is chosen
   (BATTLE_MENU_STATE.tgtViewOfs): 16 pixels down when aiming at the enemies, 2
   up when aiming at the party or the user. */
#define TARGET_VIEW_OFFSET_ENEMIES 16
#define TARGET_VIEW_OFFSET_PARTY   (-2)

/* The target cursor's markers: the cursor, and two to each side of it for the
   units its range covers. */
#define TARGET_MARKERS 6

/* One of the target cursor's markers: where it stands, its state flags and
   which listed unit it marks. */
typedef struct TARGET_TRACK {
	Uint8 x; /* screen pixels */
	Uint8 y; /* screen pixels */
	/* bits: 7..2 frames to stay put (CURSOR_HOLD_FRAME each) | 1
	   CURSOR_IN_SPREAD | 0 CURSOR_EASING */
	Uint8 flg;
	/* The place it follows from the aimed one; TARGET_MARKERS when unused. */
	Sint8 idx;
} TARGET_TRACK;

/* TARGET_TRACK.flg: easing halfway to its unit each frame, in the current
   spread, and one frame of staying put. */
#define CURSOR_EASING	  (1 << 0)
#define CURSOR_IN_SPREAD  (1 << 1)
#define CURSOR_HOLD_FRAME (1 << 2)

/* What the selection redraws: the spread's targets and their markers, and at
   the start every part. */
#define TARGET_REDRAW_SPREAD 1
#define TARGET_REDRAW_ALL	 0xFFFF

/* Lets the player choose the target of an action of unit `selection`, with a
   cursor over the aimed unit and smaller markers on the others its range
   covers. `mode` is the action's targetMode: ABILITY_TARGET_ALLIES the party,
   ABILITY_TARGET_SELF only `selection` itself, anything else the enemies.
   `spread` is its range: 1 one unit, n the aimed one and n - 1 either side,
   ABILITY_RANGE_ALL every one (the cursor does not move); 0 counts as 1. `kind`
   (BATTLE_TARGET_HINT) is mnBattleClassifyEntryKind's class, or
   TARGET_HINT_SET_DJINNI: on a party member it opens a window of what the
   action would do (its HP or PP, the statuses it cures, "Set Djinni"), and a
   reviving or curing action not aimed at everyone starts on the first party
   member who needs it. Returns the target chosen, PLACE_PARTY | its party slot
   or PLACE_ENEMY | its enemy slot, or -1 when cancelled. */
Sint32 mnBattleTargetRunSelection(
	Sint32 selection, Sint32 mode, Uint32 spread, Uint32 kind)
{
	Uint16			 candidateIds[8];
	SELECTION_SPR	 cursorSprites[TARGET_MARKERS];
	TARGET_TRACK	 cursorTracks[TARGET_MARKERS];
	SCREEN_POINT	 actorPosition;
	Uint16			 spreadActorIds[UNIT_MAX];
	Uint8			 spreadTrackIndices[8];
	SCREEN_POINT	 selectedPosition;
	Uint16			 targetName[UNIT_NAME_SIZE];
	SCREEN_POINT	 namePosition;
	SCREEN_POINT	 markerPosition;
	AFFINE_TRANSFORM effect;
	UNIT			*unit;
	BATTLE_STATE	*battle;
	WINDOW			*window;
	WINDOW			*conditionWindow;
	Sint32			 targetIndex;
	Sint32			 targetCount;
	Sint32			 candidateCount;
	Sint32			 redraw;
	Sint32			 cursorSlot;
	Sint32			 drawCursors;
	Sint32			 matrix;
	Sint32			 found;
	Sint32			 helpColumn;
	Sint32			 helpRow;
	Sint32			 count;
	Sint32			 i;
	Sint32			 j;
	Sint32			 n;
	Sint32			 cursorX;
	Sint32			 cursorY;
	Sint32			 pressed;
	Sint32			 y;
	Sint32			 width;
	Sint32			 repeated;
	Sint32			 result;
	TARGET_TRACK	*track;
	SELECTION_SPR	*marker;
	SELECTION_SPR	*firstSprite;
	UNIT			*target;

	battle = BattleStateWork;
	targetCount = 0;
	redraw = TARGET_REDRAW_ALL;
	cursorSlot = syVRAMCacheAllocEntry(256);
	conditionWindow = NULL;
	if(spread == 0)
		spread = 1;
	if(mode == ABILITY_TARGET_ALLIES || mode == ABILITY_TARGET_SELF)
		BattleMenuStateWork->tgtViewOfs = TARGET_VIEW_OFFSET_PARTY;
	else
		BattleMenuStateWork->tgtViewOfs = TARGET_VIEW_OFFSET_ENEMIES;
	for(i = 0; i < TARGET_MARKERS; i++)
		cursorTracks[i].flg = 0;
	/* The candidates: the party, the acting unit alone, or the enemies. */
	targetIndex = -1;
	if(mode == ABILITY_TARGET_ALLIES) {
		for(i = 0; i <= 5 && battle->partyUnits[i] != BATTLE_UNIT_LIST_END;
			i++) {
			candidateIds[targetCount] = battle->partyUnits[i];
			targetCount++;
		}
	} else if(mode == ABILITY_TARGET_SELF) {
		candidateIds[0] = selection;
		targetCount = 1;
	} else {
		for(i = 0; i <= 5 && battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
			candidateIds[targetCount] = battle->enemyIds[i];
			targetCount++;
		}
	}
	/* Keep the candidate list separate from the markers drawn for a spread. */
	candidateIds[targetCount] = BATTLE_UNIT_LIST_END;
	candidateCount = targetCount;
	/* Restorative actions prefer an ally who benefits from their effect.
	   Otherwise the cursor starts on the acting unit when it is a candidate. */
	if(mode == ABILITY_TARGET_ALLIES) {
		if(spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE) {
			found = 0;
			for(j = 0; j < targetCount; j++) {
				UNIT *candidate;

				if(candidateIds[j] == BATTLE_UNIT_REMOVED)
					continue;
				candidate = gmUnitGetStub(candidateIds[j]);
				switch(kind) {
				case TARGET_HINT_REVIVE:
					if(candidate->hp == 0) {
						selection = candidateIds[j];
						found = 1;
					}
					break;
				case TARGET_HINT_CURE_POISON:
					if(candidate->poison != 0)
						found = 1;
					break;
				case TARGET_HINT_RESTORE:
					if(candidate->delusion != 0 || candidate->stun != 0 ||
						candidate->sleep != 0 || candidate->psySeal != 0 ||
						candidate->grimReaper != 0)
						found = 1;
					break;
				case TARGET_HINT_CURE_ALL:
					if(candidate->delusion != 0 || candidate->stun != 0 ||
						candidate->sleep != 0 || candidate->psySeal != 0 ||
						candidate->grimReaper != 0 || candidate->poison != 0 ||
						candidate->haunt != 0)
						found = 1;
					break;
				}
				if(found != 0) {
					selection = candidateIds[j];
					break;
				}
			}
		}
		for(j = 0; j < targetCount; j++) {
			if(candidateIds[j] == selection)
				break;
		}
		if(j != targetCount)
			targetIndex = j;
	}
	/* Else the middle one; then step back past empty places and, in a link
	   battle, past downed enemies. */
	if(targetIndex < 0)
		targetIndex = (targetCount - 1) / 2;
	while(candidateIds[targetIndex] == BATTLE_UNIT_REMOVED ||
		(gmFlagGetStub(FLAG_LINK_BATTLE) && mode == ABILITY_TARGET_FOES &&
			gmUnitGetStub(candidateIds[targetIndex])->hp == 0)) {
		targetIndex = targetIndex + targetCount - 1;
		targetIndex %= targetCount;
	}
	/* Aimed at enemies or itself, the cursor starts at the acting unit (at y
	   128), stays two frames, then eases to the target. */
	if(mode != ABILITY_TARGET_ALLIES) {
		btMotionProjectCursorPointStub(selection, &actorPosition);
		cursorTracks[0].flg = CURSOR_HOLD_FRAME * 2;
		cursorTracks[0].x = actorPosition.x;
		cursorTracks[0].y = 128;
	}
	window = mnWindowCreate(0, TARGET_NAME_WINDOW_ROW, DISPLAY_TILE_WIDTH, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_OUTLINED_TEXT);
	do {
		/* FAKEMATCH(temporary): firstSprite holds &cursorSprites[0]. */
		firstSprite = cursorSprites;
		drawCursors = 0;
		{
			Sint32 t = targetIndex * 2;

			/* FAKEMATCH(dead): a test that cannot change t. */
			if(t == 0)
				t = 0;
			btMotionProjectCursorPointStub(
				*(Uint16 *)((Uint8 *)candidateIds + t), &selectedPosition);
		}
		/* The cursor: 32 frames (256 bytes each), a frame every four frames,
		   bobbing 2 pixels each way every 16 frames. */
		((Sint32 *)firstSprite)[1] = OBJ_SIZE_16X16 | OBJ_256_COLORS;
		((Sint32 *)firstSprite)[2] = 0;
		firstSprite->tile = syVRAMCacheReload(cursorSlot,
			(((FrameCount >> 2) & 31) << 8) + (Sint32)Menu_AnimatedCursorTiles);
		selectedPosition.y += CURSOR_BOB_Y();
		y = selectedPosition.y;
		/* Ease halfway each frame; the other markers show once the cursor is
		   within 7 pixels of its target. */
		if(cursorTracks[0].flg & CURSOR_EASING) {
			cursorX = (selectedPosition.x + cursorTracks[0].x) / 2;
			cursorY = (y + cursorTracks[0].y) / 2;
			if((selectedPosition.x - cursorX < 0
					   ? -(selectedPosition.x - cursorX)
					   : selectedPosition.x - cursorX) <= 7)
				drawCursors = 1;
			selectedPosition.x = cursorX;
			cursorTracks[0].x = cursorX;
			cursorTracks[0].y = cursorY;
			selectedPosition.y = cursorY;
		} else if(cursorTracks[0].flg < CURSOR_HOLD_FRAME) {
			cursorTracks[0].x = selectedPosition.x;
			selectedPosition.y = y;
			cursorTracks[0].y = y;
			cursorTracks[0].flg = CURSOR_EASING;
		} else {
			selectedPosition.x = cursorTracks[0].x;
			selectedPosition.y = cursorTracks[0].y;
			cursorTracks[0].flg = cursorTracks[0].flg - CURSOR_HOLD_FRAME;
			if(cursorTracks[0].flg < CURSOR_HOLD_FRAME)
				cursorTracks[0].flg = CURSOR_EASING;
		}
		firstSprite->x = selectedPosition.x - 8;
		firstSprite->y = selectedPosition.y - 16;
		syOAMAddSprAtDepth(firstSprite, OAM_DEPTH_WINDOW);
		/* The other markers' scale: 11/16 beside an aimed one, full size when
		   the action takes every unit. */
		if(spread == ABILITY_RANGE_ALL) {
			effect.scaleX = Q_8_8(1);
			effect.scaleY = Q_8_8(1);
		} else {
			effect.scaleX = Q_8_8(11.0 / 16);
			effect.scaleY = Q_8_8(11.0 / 16);
		}
		effect.ang = 0;
#if !CARTRIDGE_LANGUAGE_AT_RUN_TIME
		/* FAKEMATCH(dead): eighteen stores nothing reads, the eighteenth only
		 * for the test above. */
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
		effect.ang = 0;
#endif
		matrix = syOAMMatrixAllocFromTransform(&effect);
		/* The aim moved: list the units the range covers, the aimed one first,
		   mark them, and redraw the window for the aimed one. */
		if(redraw & TARGET_REDRAW_SPREAD) {
			targetCount = 0;
			for(i = 0; i < TARGET_MARKERS; i++)
				cursorTracks[i].flg &= ~CURSOR_IN_SPREAD;
			for(i = 0; i < spread; i++) {
				{
					n = targetIndex + i;
					if(n < candidateCount &&
						candidateIds[n] != BATTLE_UNIT_REMOVED) {
						spreadActorIds[targetCount] = candidateIds[n];
						cursorTracks[i].flg |= CURSOR_IN_SPREAD;
						if(cursorTracks[i].idx != i) {
							cursorTracks[i].flg &= ~CURSOR_EASING;
							cursorTracks[i].idx = i;
						}
						spreadTrackIndices[targetCount] = i;
						targetCount++;
					}
					if(i != 0 && (n = targetIndex - i) >= 0 &&
						candidateIds[n] != BATTLE_UNIT_REMOVED) {
						spreadActorIds[targetCount] = candidateIds[n];
						cursorTracks[TARGET_MARKERS - i].flg |=
							CURSOR_IN_SPREAD;
						if(cursorTracks[TARGET_MARKERS - i].idx != -i) {
							cursorTracks[TARGET_MARKERS - i].flg &=
								~CURSOR_EASING;
							cursorTracks[TARGET_MARKERS - i].idx = -i;
						}
						spreadTrackIndices[targetCount] = TARGET_MARKERS - i;
						targetCount++;
					}
				}
			}
			for(i = 0; i < TARGET_MARKERS; i++) {
				if((cursorTracks[i].flg & CURSOR_IN_SPREAD) == 0)
					cursorTracks[i].idx = TARGET_MARKERS;
			}
			spreadActorIds[targetCount] = BATTLE_UNIT_LIST_END;
			btHighlightSetModesStub(spreadActorIds, HIGHLIGHT_SELECTED);
			/* A party member: the window of what the action does, placed by the
			   party member's tile column and kept on screen. */
			if(candidateIds[targetIndex] <= ALLY_MAX) {
				if(spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE) {
					unit = gmUnitGetStub(candidateIds[targetIndex]);
					btMotionProjectCursorPointStub(
						candidateIds[targetIndex], &selectedPosition);
					if(conditionWindow != 0)
						mnWindowClose(conditionWindow, 1);
					/* FAKEMATCH(dead): cancelling updates of
					 * conditionWindow. */
					conditionWindow++;
					conditionWindow--;
					switch(kind) {
					case TARGET_HINT_SET_DJINNI:
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
					{
						Sint32 anchor = selectedPosition.x / 8;

						helpColumn = anchor - 4;
						if(syGetLanguage() == CARTRIDGE_LANGUAGE_ITALIAN) {
							if(anchor + TARGET_SET_DJINNI_WIDTH_ITALIAN - 4 >
								DISPLAY_TILE_WIDTH - 1)
								helpColumn = DISPLAY_TILE_WIDTH -
									TARGET_SET_DJINNI_WIDTH_ITALIAN;
							conditionWindow = mnWindowCreate(helpColumn, 8,
								TARGET_SET_DJINNI_WIDTH_ITALIAN, 3,
								MENU_WINDOW_FLAGS);
						} else if(syGetLanguage() ==
							CARTRIDGE_LANGUAGE_SPANISH) {
							if(anchor + TARGET_SET_DJINNI_WIDTH_SPANISH - 4 >
								DISPLAY_TILE_WIDTH - 1)
								helpColumn = DISPLAY_TILE_WIDTH -
									TARGET_SET_DJINNI_WIDTH_SPANISH;
							conditionWindow = mnWindowCreate(helpColumn, 8,
								TARGET_SET_DJINNI_WIDTH_SPANISH, 3,
								MENU_WINDOW_FLAGS);
						} else {
							if(anchor + TARGET_SET_DJINNI_WIDTH - 4 >
								DISPLAY_TILE_WIDTH - 1)
								helpColumn = DISPLAY_TILE_WIDTH -
									TARGET_SET_DJINNI_WIDTH;
							conditionWindow = mnWindowCreate(helpColumn, 8,
								TARGET_SET_DJINNI_WIDTH, 3, MENU_WINDOW_FLAGS);
						}
					}
#else
#if !SET_DJINNI_WINDOW_FITS_SCREEN
						/* BUG: placed as the Japanese window, 8 tiles wide;
						   this one is 9, so against the right edge its last
						   column is past the screen's. */
						TARGET_HELP_COLUMN(4, 8);
#else
						TARGET_HELP_COLUMN(4, TARGET_SET_DJINNI_WIDTH);
#endif
						conditionWindow = mnWindowCreate(helpColumn, 8,
							TARGET_SET_DJINNI_WIDTH, 3, MENU_WINDOW_FLAGS);
#endif
						mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							MsgTargetSetDjinni, conditionWindow, 0, 0);
						mnTextSetCol(TEXT_COLOR_WHITE);
						break;
					case TARGET_HINT_HP:
						TARGET_HELP_COLUMN(7, TARGET_GAUGE_WIDTH);
						conditionWindow = mnWindowCreate(helpColumn, 8,
							TARGET_GAUGE_WIDTH, 3, MENU_WINDOW_FLAGS);
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
						switch(syGetLanguage()) {
						case CARTRIDGE_LANGUAGE_GERMAN:
							mnTextDrawStringAtOffset(
								(Uint8 *)BattleStatusScreen_KpString,
								conditionWindow, 0, 0);
							break;
						case CARTRIDGE_LANGUAGE_FRENCH:
							mnTextDrawStringAtOffset(
								(Uint8 *)BattleStatusScreen_PvString,
								conditionWindow, 0, 0);
							break;
						case CARTRIDGE_LANGUAGE_SPANISH:
							mnTextDrawStringAtOffset(
								(Uint8 *)BattleStatusScreen_PvString,
								conditionWindow, 0, 0);
							break;
						case CARTRIDGE_LANGUAGE_ITALIAN:
							mnTextDrawStringAtOffset(
								(Uint8 *)BattleStatusScreen_HpString,
								conditionWindow, 0, 0);
							break;
						}
#else
						mnTextDrawStringAtOffset(
							(Uint8 *)BattleStatusScreen_HpString,
							conditionWindow, 0, 0);
#endif
						mnTextDrawNumInWindow(
							unit->hp, 4, conditionWindow, 16, 0);
						mnTextDrawStringAtOffset(
							(Uint8 *)BattleStatusScreen_SlashString,
							conditionWindow, 48, 0);
						mnTextDrawNumInWindow(
							unit->hpMax, 4, conditionWindow, 56, 0);
						break;
					case TARGET_HINT_PP:
						TARGET_HELP_COLUMN(7, TARGET_GAUGE_WIDTH);
						conditionWindow = mnWindowCreate(helpColumn, 8,
							TARGET_GAUGE_WIDTH, 3, MENU_WINDOW_FLAGS);
						mnTextDrawStringAtOffset(
							(Uint8 *)BattleStatusScreen_PpString,
							conditionWindow, 0, 0);
						mnTextDrawNumInWindow(
							unit->pp, 4, conditionWindow, 16, 0);
						mnTextDrawStringAtOffset(
							(Uint8 *)BattleStatusScreen_SlashString,
							conditionWindow, 48, 0);
						mnTextDrawNumInWindow(
							unit->ppMax, 4, conditionWindow, 56, 0);
						break;
					case TARGET_HINT_REVIVE:
						TARGET_HELP_COLUMN(7, TARGET_ONE_LINE_WIDTH);
						conditionWindow = mnWindowCreate(helpColumn, 8,
							TARGET_ONE_LINE_WIDTH, 3, MENU_WINDOW_FLAGS);
						if(unit->hp != 0)
							goto no_condition;
						mnTextDrawMessageAsTiles(
							MsgTargetDowned, conditionWindow, 0, 0);
						break;
					case TARGET_HINT_CURE_POISON:
						TARGET_HELP_COLUMN(7, TARGET_ONE_LINE_WIDTH);
						conditionWindow = mnWindowCreate(helpColumn, 8,
							TARGET_ONE_LINE_WIDTH, 3, MENU_WINDOW_FLAGS);
						if(unit->poison == 0)
							goto no_condition;
						mnTextDrawMessageAsTiles(
							MsgTargetCurePoison, conditionWindow, 0, 0);
						break;
					case TARGET_HINT_RESTORE:
						count = 0;
						if(unit->delusion != 0)
							count = 1;
						if(unit->stun != 0)
							count++;
						if(unit->sleep != 0)
							count++;
						if(unit->psySeal != 0)
							count++;
						if(unit->grimReaper != 0)
							count++;
						if(count == 0)
							count = 1;
						/* The list ends on row 10, as the one-line windows do,
						   and starts no higher than row 4. */
						helpRow = 9 - count;
						if(helpRow <= 3)
							helpRow = 4;
						TARGET_CONDITION_LIST_WINDOW();
						count = 0;
						if(unit->delusion != 0) {
							mnTextDrawMessageAsTiles(
								MsgTargetCureDelusion, conditionWindow, 0, 0);
							count = 1;
						}
						if(unit->stun != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureStun,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->sleep != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureSleep,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->psySeal != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureSeal,
								conditionWindow, 0, count * 8);
							++count;
						}
						if(unit->grimReaper != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureCurse,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(count == 0)
							goto no_condition;
						break;
					default:
						break;
					case TARGET_HINT_CURE_ALL:
						count = 0;
						if(unit->poison != 0)
							count = 1;
						if(unit->delusion != 0)
							count++;
						if(unit->stun != 0)
							count++;
						if(unit->sleep != 0)
							count++;
						if(unit->psySeal != 0)
							count++;
						if(unit->grimReaper != 0)
							count++;
						if(unit->haunt != 0)
							count++;
						if(count == 0)
							count = 1;
						helpRow = 9 - count;
						if(helpRow <= 3)
							helpRow = 4;
						TARGET_CONDITION_LIST_WINDOW();
						count = 0;
						if(unit->poison != 0) {
							mnTextDrawMessageAsTiles(
								MsgTargetCurePoison, conditionWindow, 0, 0);
							count = 1;
						}
						if(unit->delusion != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureDelusion,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->stun != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureStun,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->sleep != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureSleep,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->psySeal != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureSeal,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->grimReaper != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureCurse,
								conditionWindow, 0, count * 8);
							count++;
						}
						if(unit->haunt != 0) {
							mnTextDrawMessageAsTiles(MsgTargetCureHaunt,
								conditionWindow, 0, 8 * count);
							count++;
						}
						if(count != 0)
							break;
					no_condition:
						mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							MsgTargetNoEffect, conditionWindow, 0, 0);
						mnTextSetCol(TEXT_COLOR_WHITE);
						break;
					}
				}
			} else if(spread != ABILITY_RANGE_ALL) {
				/* An enemy: its name, centred under it and kept within the
				   window; ENEMY_MYSTERY_MAN and ENEMY_MYSTERY_WOMAN are named
				   "Mystery Man" and "Mystery Woman". */
				target = gmUnitGetStub(candidateIds[targetIndex]);
				btMotionProjectCursorPointStub(
					candidateIds[targetIndex], &namePosition);
				namePosition.y += CURSOR_BOB_Y();
				if(target->id == ENEMY_MYSTERY_WOMAN ||
					target->id == ENEMY_MYSTERY_MAN) {
					width = MsgMysteryPersonName;
					if(target->id == ENEMY_MYSTERY_WOMAN)
						width++;
					TARGET_MYSTERY_NAME_COPY(
						width, targetName, TARGET_MYSTERY_NAME_CAPACITY);
				} else {
					for(i = 0; i < UNIT_NAME_SIZE - 1; i++) {
						targetName[i] = n = target->name[i];
						if(!n)
							break;
					}
					targetName[i] = 0;
				}
				width = mnTextGetWideStringWidth(targetName);
				namePosition.x -= width / 2;
				namePosition.x -= 8;
				if(namePosition.x + width > TARGET_NAME_INTERIOR_WIDTH)
					namePosition.x = TARGET_NAME_INTERIOR_WIDTH - width;
				if(namePosition.x < 0)
					namePosition.x = 0;
				mnTextClearCanvas();
				mnTextDrawWideStringAtOffset(
					targetName, window, namePosition.x, 4);
			}
			redraw &= ~TARGET_REDRAW_SPREAD;
		}
		if(drawCursors != 0) {
			for(i = 1; i < targetCount; i++) {
				marker = &firstSprite[i];
				track = &cursorTracks[spreadTrackIndices[i]];
				btMotionProjectCursorPointStub(
					spreadActorIds[i], &markerPosition);
				markerPosition.y = markerPosition.y + CURSOR_BOB_Y();
				*marker = *firstSprite;
				if(track->flg & CURSOR_EASING) {
					Sint32 x = (markerPosition.x + track->x) / 2;
					Sint32 y = (markerPosition.y + track->y) / 2;

					markerPosition.x = x;
					track->x = x;
					markerPosition.y = y;
					track->y = y;
				} else {
					Sint32 x = marker->x;
					Sint32 y = marker->y + 8;

					track->flg = CURSOR_EASING;
					markerPosition.x = x;
					track->x = x;
					markerPosition.y = y;
					track->y = y;
				}
				marker->mode = OBJ_MODE_SEMI_TRANSPARENT;
				marker->x = markerPosition.x - 8;
				marker->y = markerPosition.y - 12;
				if(spread == ABILITY_RANGE_ALL)
					marker->affine = 0;
				else
					marker->affine = 1;
				marker->matrix = matrix;
				syOAMAddSprAtDepth(marker, OAM_DEPTH_WINDOW);
			}
		}
		pressed = PerPush;
		repeated = PerRepeat;
		if(BattleMenuStateWork->tutorialStep) {
			repeated = 0;
			pressed = 0;
			if(BattleMenuStateWork->tutorialDelay == 0) {
				BattleMenuStateWork->tutorialDelay = 60;
				repeated = KEY_A;
				pressed = KEY_A;
			} else {
				BattleMenuStateWork->tutorialDelay--;
			}
		}
		/* A: the aimed unit's side and place; the loop ends after this
		   frame. */
		if(pressed & KEY_A) {
			Sint32 id;
			Sint32 k;

			id = candidateIds[targetIndex];
			redraw = 0;
			result = -1;
			for(k = 0; k <= 5 && battle->partyUnits[k] != BATTLE_UNIT_LIST_END;
				k++) {
				if(battle->partyUnits[k] == id) {
					result = PLACE_PARTY | k;
					break;
				}
			}
			if(result < 0) {
				for(k = 0;
					k <= 5 && battle->enemyIds[k] != BATTLE_UNIT_LIST_END;
					k++) {
					if(battle->enemyIds[k] == id) {
						result = PLACE_ENEMY | k;
						break;
					}
				}
			}
			targetIndex = result;
		} else if(spread != ABILITY_RANGE_ALL) {
			if(repeated & (KEY_DOWN | KEY_RIGHT)) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				do {
					targetIndex++;
					targetIndex %= candidateCount;
				} while(candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
				redraw |= TARGET_REDRAW_SPREAD;
			}
			if(repeated & (KEY_UP | KEY_LEFT)) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				do {
					targetIndex = targetIndex + candidateCount - 1;
					targetIndex %= candidateCount;
				} while(candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
				redraw |= TARGET_REDRAW_SPREAD;
			}
		}
		if(BattleMenuStateWork->tmr == 0 || (pressed & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			targetIndex = -1;
			break;
		}
		sySynch(1);
	} while(redraw != 0);
	sySynch(1);
	syVRAMCacheFreeEntry(cursorSlot);
	if(conditionWindow != 0)
		mnWindowClose(conditionWindow, 1);
	mnWindowClose(window, 1);
	btHighlightSetModesStub(candidateIds, HIGHLIGHT_OFF);
	BattleMenuStateWork->tgtViewOfs = 0;
	sySynch(1);
	return targetIndex;
}

#undef TARGET_SET_DJINNI_WIDTH
#undef TARGET_CONDITION_LIST_WIDTH
#undef TARGET_MYSTERY_NAME_COPY
#undef TARGET_MYSTERY_NAME_CAPACITY
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
#undef TARGET_SET_DJINNI_WIDTH_ITALIAN
#undef TARGET_SET_DJINNI_WIDTH_SPANISH
#undef TARGET_CONDITION_LIST_WIDTH_SPANISH
#endif
#undef TARGET_GAUGE_WIDTH
#undef TARGET_ONE_LINE_WIDTH
#undef TARGET_HELP_COLUMN
#undef TARGET_CONDITION_LIST_WINDOW
#undef TARGET_NAME_WINDOW_ROW
#undef TARGET_NAME_INTERIOR_WIDTH
#undef TARGET_VIEW_OFFSET_ENEMIES
#undef TARGET_VIEW_OFFSET_PARTY
#undef TARGET_MARKERS
#undef CURSOR_EASING
#undef CURSOR_IN_SPREAD
#undef CURSOR_HOLD_FRAME
#undef TARGET_REDRAW_SPREAD
#undef TARGET_REDRAW_ALL
