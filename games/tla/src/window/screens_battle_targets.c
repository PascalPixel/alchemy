// Choosing a battle target. Part of screens.c.

// Widths in tiles that fit each language's text: the "Set Djinni" window (MsgSetDjinni) and the
// list of conditions an action cures; and the Mystery names' copy, which Japanese decodes raw where
// the other languages copy it expanded.
#define TARGET_SET_DJINNI_WIDTH      JAPANESE_OR(8, 9)
#define TARGET_CONDITION_LIST_WIDTH  JAPANESE_OR(14, 16)
#define TARGET_MYSTERY_NAME_COPY     JAPANESE_OR(mnMessageDecodeRaw, mnMessageCopyExpanded)
#define TARGET_MYSTERY_NAME_CAPACITY JAPANESE_OR(10, 14)

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
// These editions size the "Set Djinni" window by the cartridge's language as it runs: wider for
// Italian and Spanish.
#	define TARGET_SET_DJINNI_WIDTH_ITALIAN 10
#	define TARGET_SET_DJINNI_WIDTH_SPANISH 11
extern const Uint8 OwnerStatusKpString[];
extern const Uint8 OwnerStatusPvString[];
#endif

// The HP and PP window's width, and the one-line windows' of a revival or a cure of poison.
#define TARGET_GAUGE_WIDTH    13
#define TARGET_ONE_LINE_WIDTH 12

// The help window's column, `back` tiles left of the aimed party member's; a window `windowWidth`
// tiles wide that would run past the screen's right edge stands against it instead.
#define TARGET_HELP_COLUMN(back, windowWidth)                                                      \
	{                                                                                              \
		Sint32 anchor = selectedPos.x / 8;                                                         \
		helpColumn    = anchor - (back);                                                           \
		if (anchor + (windowWidth) - (back) > DISPLAY_TILE_WIDTH - 1)                              \
			helpColumn = DISPLAY_TILE_WIDTH - (windowWidth);                                       \
	}

// The window listing the conditions an action cures, helpRow down and `count` lines tall.
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
// These editions widen it by a tile for Spanish as it runs.
#	define TARGET_CONDITION_LIST_WIDTH_SPANISH 17
#	define TARGET_CONDITION_LIST_WINDOW()                                                         \
		{                                                                                          \
			Sint32 anchor = selectedPos.x / 8;                                                     \
			helpColumn    = anchor - 7;                                                            \
			if (syGetLanguage() == CARTRIDGE_LANGUAGE_SPANISH)                                     \
			{                                                                                      \
				if (anchor + TARGET_CONDITION_LIST_WIDTH_SPANISH - 7 > DISPLAY_TILE_WIDTH - 1)     \
					helpColumn = DISPLAY_TILE_WIDTH - TARGET_CONDITION_LIST_WIDTH_SPANISH;         \
				conditionWindow = mnWindowCreate(helpColumn,                                       \
				                                 helpRow,                                          \
				                                 TARGET_CONDITION_LIST_WIDTH_SPANISH,              \
				                                 count + 2,                                        \
				                                 WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);        \
			}                                                                                      \
			else                                                                                   \
			{                                                                                      \
				if (anchor + TARGET_CONDITION_LIST_WIDTH - 7 > DISPLAY_TILE_WIDTH - 1)             \
					helpColumn = DISPLAY_TILE_WIDTH - TARGET_CONDITION_LIST_WIDTH;                 \
				conditionWindow = mnWindowCreate(helpColumn,                                       \
				                                 helpRow,                                          \
				                                 TARGET_CONDITION_LIST_WIDTH,                      \
				                                 count + 2,                                        \
				                                 WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);        \
			}                                                                                      \
		}
#else
#	define TARGET_CONDITION_LIST_WINDOW()                                                         \
		{                                                                                          \
			TARGET_HELP_COLUMN(7, TARGET_CONDITION_LIST_WIDTH);                                    \
			conditionWindow = mnWindowCreate(helpColumn,                                           \
			                                 helpRow,                                              \
			                                 TARGET_CONDITION_LIST_WIDTH,                          \
			                                 count + 2,                                            \
			                                 WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);            \
		}
#endif

// The enemy name window, the screen's width from row 12, and the pixels of its interior the name is
// kept within.
#define TARGET_NAME_WINDOW_ROW     12
#define TARGET_NAME_INTERIOR_WIDTH ((DISPLAY_TILE_WIDTH - 2) * 8)

// Where the battle view slides while a target is chosen (BATTLE_MENU_STATE.targetViewOffset): 16
// pixels down when aiming at the enemies, 2 up when aiming at the party or the user.
#define TARGET_VIEW_OFFSET_ENEMIES 16
#define TARGET_VIEW_OFFSET_PARTY   (-2)

// TARGET_TRACK.flags: easing halfway to its unit each tick, in the current spread, and one tick of
// staying put (bits 7 to 2 count those ticks).
#define CURSOR_EASING              (1 << 0)
#define CURSOR_IN_SPREAD           (1 << 1)
#define CURSOR_HOLD_TICK           (1 << 2)

// What the selection redraws: the spread's targets and their markers, and at the start every part.
#define TARGET_REDRAW_SPREAD       1
#define TARGET_REDRAW_ALL          0xFFFF

// The cursors' bob, a sine turn every 16 ticks scaled down to 2 pixels: the sine divided by
// toFIXED(1) / 2, rounding toward zero.
#define TARGET_BOB(y)                                                                              \
	{                                                                                              \
		Sint32 bob = sySin(TickCount * (TRIG_TURN / 16));                                          \
                                                                                                   \
		if (bob < 0)                                                                               \
			bob += toFIXED(1) / 2 - 1;                                                             \
		(y) += bob >> 15;                                                                          \
	}

// Lets the player choose the target of an action of unit `selection`, with a cursor over the aimed
// unit and smaller markers on the others its range covers. `mode` is the action's targetMode:
// ABILITY_TARGET_ALLIES the party, ABILITY_TARGET_SELF only `selection` itself, anything else the
// enemies. `spread` is its range: 1 one unit, n the aimed one and n - 1 either side,
// ABILITY_RANGE_ALL every one (the cursor does not move); 0 counts as 1. `kind`
// (BATTLE_TARGET_HINT) is mnBattleClassifyEntryKind's class, or TARGET_HINT_SET_DJINNI: on a party
// member it opens a window of what the action would do (its HP or PP, the statuses it cures, "Set
// Djinni"), and a reviving or curing action not aimed at everyone starts on the first party member
// who needs it, TARGET_HINT_LAST_TARGET on the party member last aimed at. Returns the target
// chosen, PLACE_PARTY | its party slot or PLACE_ENEMY | its enemy slot, or -1 when cancelled.
Sint32 mnBattleTargetRunSelection(Sint32 selection, Sint32 mode, Uint32 spread, Uint32 kind)
{
	Uint16           candidateIds[8];
	SELECTION_SPR    cursorSprites[6];
	TARGET_TRACK     cursorTracks[6];
	TARGET_POINT     actorPos;
	Uint16           spreadActorIds[UNIT_MAX];
	Uint8            spreadTrackIndices[8];
	TARGET_POINT     selectedPos;
	Uint16           targetName[UNIT_NAME_SIZE];
	TARGET_POINT     namePos;
	TARGET_POINT     markerPos;
	AFFINE_TRANSFORM effect;
	UNIT            *unit;
	BATTLE_STATE    *battle;
	WINDOW          *window;
	WINDOW          *conditionWindow;
	Sint32           targetIndex;
	Sint32           targetCount;
	Sint32           candidateCount;
	Sint32           redraw;
	Sint32           cursorSlot;
	Sint32           drawCursors;
	Sint32           matrix;
	Sint32           found;
	Sint32           helpColumn;
	Sint32           helpRow;
	Sint32           count;
	Sint32           i;
	Sint32           j;
	Sint32           n;
	Sint32           cursorX;
	Sint32           cursorY;
	Sint32           pressed;
	Sint32           y;
	Sint32           width;
	Sint32           repeated;
	Sint32           result;
	TARGET_TRACK    *track;
	SELECTION_SPR   *marker;
	SELECTION_SPR   *firstSprite;
	UNIT            *target;

	battle          = RamHeapSlots->battleState;
	targetCount     = 0;
	redraw          = TARGET_REDRAW_ALL;
	cursorSlot      = syVRAMCacheAllocateEntry(256);
	conditionWindow = NULL;
	if (spread == 0)
		spread = 1;
	if (mode == ABILITY_TARGET_ALLIES || mode == ABILITY_TARGET_SELF)
		BattleMenuStateWork->targetViewOffset = TARGET_VIEW_OFFSET_PARTY;
	else
		BattleMenuStateWork->targetViewOffset = TARGET_VIEW_OFFSET_ENEMIES;
	for (i = 0; i < 6; i++)
		cursorTracks[i].flags = 0;
	// The candidates: the party, the acting unit alone, or the enemies.
	targetIndex = -1;
	if (mode == ABILITY_TARGET_ALLIES)
	{
		for (i = 0; i <= 5 && battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++)
		{
			candidateIds[targetCount] = battle->partyUnits[i];
			targetCount++;
		}
	}
	else if (mode == ABILITY_TARGET_SELF)
	{
		candidateIds[0] = selection;
		targetCount     = 1;
	}
	else
	{
		for (i = 0; i <= 5 && battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++)
		{
			candidateIds[targetCount] = battle->enemyIds[i];
			targetCount++;
		}
	}
	// Keep the candidate list separate from the markers drawn for a spread.
	candidateIds[targetCount] = BATTLE_UNIT_LIST_END;
	candidateCount            = targetCount;
	// Restorative actions prefer an ally who benefits from their effect, and
	// TARGET_HINT_LAST_TARGET the unit last aimed at. Otherwise the cursor starts on the acting
	// unit when it is a candidate.
	if (mode == ABILITY_TARGET_ALLIES)
	{
		if (spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE)
		{
			found = 0;
			for (j = 0; j < targetCount; j++)
			{
				UNIT *candidate;

				if (candidateIds[j] == BATTLE_UNIT_REMOVED)
					continue;
				candidate = syUnitGet(candidateIds[j]);
				switch (kind)
				{
					case TARGET_HINT_REVIVE:
						if (candidate->hp == 0)
						{
							selection = candidateIds[j];
							found     = 1;
						}
						break;
					case TARGET_HINT_CURE_POISON:
						if (candidate->poison != 0)
							found = 1;
						break;
					case TARGET_HINT_RESTORE:
						if (candidate->delusion != 0 || candidate->stun != 0
						    || candidate->sleep != 0 || candidate->psySeal != 0
						    || candidate->grimReaper != 0)
							found = 1;
						break;
					case TARGET_HINT_CURE_ALL:
						if (candidate->delusion != 0 || candidate->stun != 0
						    || candidate->sleep != 0 || candidate->psySeal != 0
						    || candidate->grimReaper != 0 || candidate->poison != 0
						    || candidate->haunt != 0)
							found = 1;
						break;
					case TARGET_HINT_LAST_TARGET:
						if (BattleMenuStateWork->lastTarget == candidateIds[j])
							found = 1;
						break;
				}
				if (found != 0)
				{
					selection = candidateIds[j];
					break;
				}
			}
		}
		for (j = 0; j < targetCount; j++)
		{
			if (candidateIds[j] == selection)
				break;
		}
		if (j != targetCount)
			targetIndex = j;
	}
	// Else the middle one; then step back past empty places and, in a link battle, past downed
	// enemies.
	if (targetIndex < 0)
		targetIndex = (targetCount - 1) / 2;
	while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED
	       || (syGameFlagGet(FLAG_LINK_BATTLE) && mode == ABILITY_TARGET_FOES
	           && syUnitGet(candidateIds[targetIndex])->hp == 0))
	{
		targetIndex  = targetIndex + targetCount - 1;
		targetIndex %= targetCount;
	}
	// Aimed at enemies or itself, the cursor starts at the acting unit (at y 128), stays two ticks,
	// then eases to the target.
	if (mode != ABILITY_TARGET_ALLIES)
	{
		btMotionProjectCursorPointStub(selection, &actorPos);
		cursorTracks[0].flags = CURSOR_HOLD_TICK * 2;
		cursorTracks[0].x     = actorPos.x;
		cursorTracks[0].y     = 128;
	}
	window =
		mnWindowCreate(0,
	                   TARGET_NAME_WINDOW_ROW,
	                   DISPLAY_TILE_WIDTH,
	                   4,
	                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS | WINDOW_FLAG_OUTLINED_TEXT);

	for (;;)
	{
		// FAKEMATCH(temporary): firstSprite holds &cursorSprites[0].
		firstSprite = cursorSprites;
		drawCursors = 0;
		btMotionProjectCursorPointStub(candidateIds[targetIndex], &selectedPos);
		((Uint32 *)firstSprite)[1] = OBJ_SIZE_16X16 | OBJ_256_COLORS;
		syIOWrite32(0, &((Uint32 *)firstSprite)[2]);
		// The cursor: 32 ticks (256 bytes each), a tick every four ticks, bobbing 2 pixels each way
		// every 16 ticks.
		firstSprite->tile =
			syVRAMCacheReload(cursorSlot, ((TickCount >> 2) & 31) * 256 + MenuAnimatedCursorTiles);
		TARGET_BOB(selectedPos.y);
		y = selectedPos.y;
		// Ease halfway each tick; the other markers show once the cursor is within 7 pixels of its
		// target.
		if (cursorTracks[0].flags & CURSOR_EASING)
		{
			cursorX = (selectedPos.x + cursorTracks[0].x) / 2;
			cursorY = (y + cursorTracks[0].y) / 2;
			if ((selectedPos.x - cursorX < 0 ? -(selectedPos.x - cursorX) : selectedPos.x - cursorX)
			    <= 7)
				drawCursors = 1;
			selectedPos.x     = cursorX;
			cursorTracks[0].x = cursorX;
			selectedPos.y     = cursorY;
			cursorTracks[0].y = cursorY;
		}
		else if (cursorTracks[0].flags < CURSOR_HOLD_TICK)
		{
			cursorTracks[0].x     = selectedPos.x;
			selectedPos.y         = y;
			cursorTracks[0].y     = y;
			cursorTracks[0].flags = CURSOR_EASING;
		}
		else
		{
			selectedPos.x          = cursorTracks[0].x;
			selectedPos.y          = cursorTracks[0].y;
			cursorTracks[0].flags -= CURSOR_HOLD_TICK;
			if (cursorTracks[0].flags < CURSOR_HOLD_TICK)
				cursorTracks[0].flags = CURSOR_EASING;
		}
		firstSprite->x = selectedPos.x - 8;
		firstSprite->y = selectedPos.y - 16;
		syOAMAddSpriteAtDepth(firstSprite, OAM_DEPTH_WINDOW);
		// The other markers' scale: 11/16 beside an aimed one, full size when the action takes
		// every unit.
		if (spread == ABILITY_RANGE_ALL)
		{
			effect.scaleX = Q_8_8(1);
			effect.scaleY = Q_8_8(1);
		}
		else
		{
			effect.scaleX = Q_8_8(11.0 / 16);
			effect.scaleY = Q_8_8(11.0 / 16);
		}
		effect.angle = 0;
		matrix       = syOAMMatrixAllocateFromTransform(&effect);

		// The aim moved: list the units the range covers, the aimed one first, mark them, and
		// redraw the window for the aimed one.
		if (redraw & TARGET_REDRAW_SPREAD)
		{
			targetCount = 0;
			for (i = 0; i < 6; i++)
				cursorTracks[i].flags &= ~CURSOR_IN_SPREAD;
			for (i = 0; i < spread; i++)
			{
				n = targetIndex + i;
				if (n < candidateCount && candidateIds[n] != BATTLE_UNIT_REMOVED)
				{
					spreadActorIds[targetCount]  = candidateIds[n];
					cursorTracks[i].flags       |= CURSOR_IN_SPREAD;
					if (cursorTracks[i].index != i)
					{
						cursorTracks[i].flags &= ~CURSOR_EASING;
						cursorTracks[i].index  = i;
					}
					spreadTrackIndices[targetCount] = i;
					targetCount++;
				}
				if (i != 0 && (n = targetIndex - i) >= 0 && candidateIds[n] != BATTLE_UNIT_REMOVED)
				{
					spreadActorIds[targetCount]  = candidateIds[n];
					cursorTracks[6 - i].flags   |= CURSOR_IN_SPREAD;
					if (cursorTracks[6 - i].index != -i)
					{
						cursorTracks[6 - i].flags &= ~CURSOR_EASING;
						cursorTracks[6 - i].index  = -i;
					}
					spreadTrackIndices[targetCount] = 6 - i;
					targetCount++;
				}
			}
			for (i = 0; i < 6; i++)
			{
				if ((cursorTracks[i].flags & CURSOR_IN_SPREAD) == 0)
					cursorTracks[i].index = 6;
			}
			spreadActorIds[targetCount] = BATTLE_UNIT_LIST_END;
			btHighlightSetModesStub(spreadActorIds, HIGHLIGHT_SELECTED);
			// A party member: the window of what the action does, placed by the party member's tile
			// column and kept on screen. The aimed unit is remembered for TARGET_HINT_LAST_TARGET.
			if ((Uint32)(BattleMenuStateWork->lastTarget = candidateIds[targetIndex]) <= ALLY_MAX)
			{
				if (spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE)
				{
					unit = syUnitGet(candidateIds[targetIndex]);
					btMotionProjectCursorPointStub(candidateIds[targetIndex], &selectedPos);
					if (conditionWindow != NULL)
						mnWindowClose(conditionWindow, 1);
					switch (kind)
					{
						case TARGET_HINT_SET_DJINNI:
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
						{
							Sint32 anchor = selectedPos.x / 8;

							helpColumn    = anchor - 4;
							if (syGetLanguage() == CARTRIDGE_LANGUAGE_ITALIAN)
							{
								if (anchor + TARGET_SET_DJINNI_WIDTH_ITALIAN - 4
								    > DISPLAY_TILE_WIDTH - 1)
									helpColumn =
										DISPLAY_TILE_WIDTH - TARGET_SET_DJINNI_WIDTH_ITALIAN;
								conditionWindow =
									mnWindowCreate(helpColumn,
								                   8,
								                   TARGET_SET_DJINNI_WIDTH_ITALIAN,
								                   3,
								                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							}
							else if (syGetLanguage() == CARTRIDGE_LANGUAGE_SPANISH)
							{
								if (anchor + TARGET_SET_DJINNI_WIDTH_SPANISH - 4
								    > DISPLAY_TILE_WIDTH - 1)
									helpColumn =
										DISPLAY_TILE_WIDTH - TARGET_SET_DJINNI_WIDTH_SPANISH;
								conditionWindow =
									mnWindowCreate(helpColumn,
								                   8,
								                   TARGET_SET_DJINNI_WIDTH_SPANISH,
								                   3,
								                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							}
							else
							{
								if (anchor + TARGET_SET_DJINNI_WIDTH - 4 > DISPLAY_TILE_WIDTH - 1)
									helpColumn = DISPLAY_TILE_WIDTH - TARGET_SET_DJINNI_WIDTH;
								conditionWindow =
									mnWindowCreate(helpColumn,
								                   8,
								                   TARGET_SET_DJINNI_WIDTH,
								                   3,
								                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							}
						}
#else
#	if SET_DJINNI_WINDOW_FITS_SCREEN
							TARGET_HELP_COLUMN(4, TARGET_SET_DJINNI_WIDTH);
#	else
							// BUG: placed as the Japanese window, 8 tiles wide; the English one is
							// 9, so against the right edge its last column is past the screen's.
							TARGET_HELP_COLUMN(4, 8);
#	endif
							conditionWindow =
								mnWindowCreate(helpColumn,
							                   8,
							                   TARGET_SET_DJINNI_WIDTH,
							                   3,
							                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
#endif
							mnTextSetColor(TEXT_COLOR_RED);
							mnTextDrawMessageAsTiles((Sint32)MsgSetDjinni, conditionWindow, 0, 0);
							mnTextSetColor(TEXT_COLOR_WHITE);
							break;
						case TARGET_HINT_HP:
							TARGET_HELP_COLUMN(7, TARGET_GAUGE_WIDTH);
							conditionWindow =
								mnWindowCreate(helpColumn,
							                   8,
							                   TARGET_GAUGE_WIDTH,
							                   3,
							                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
							switch (syGetLanguage())
							{
								case CARTRIDGE_LANGUAGE_GERMAN:
									mnTextDrawStringAtOffset(
										OwnerStatusKpString, conditionWindow, 0, 0);
									break;
								case CARTRIDGE_LANGUAGE_FRENCH:
								case CARTRIDGE_LANGUAGE_SPANISH:
									mnTextDrawStringAtOffset(
										OwnerStatusPvString, conditionWindow, 0, 0);
									break;
								case CARTRIDGE_LANGUAGE_ITALIAN:
									mnTextDrawStringAtOffset(
										OwnerStatusHpString, conditionWindow, 0, 0);
									break;
							}
#else
							mnTextDrawStringAtOffset(OwnerStatusHpString, conditionWindow, 0, 0);
#endif
							mnTextDrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
							mnTextDrawStringAtOffset(
								OwnerStatusSlashString, conditionWindow, 48, 0);
							mnTextDrawNumberInWindow(unit->hpMax, 4, conditionWindow, 56, 0);
							break;
						case TARGET_HINT_PP:
							TARGET_HELP_COLUMN(7, TARGET_GAUGE_WIDTH);
							conditionWindow =
								mnWindowCreate(helpColumn,
							                   8,
							                   TARGET_GAUGE_WIDTH,
							                   3,
							                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							mnTextDrawStringAtOffset(OwnerStatusPpString, conditionWindow, 0, 0);
							mnTextDrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
							mnTextDrawStringAtOffset(
								OwnerStatusSlashString, conditionWindow, 48, 0);
							mnTextDrawNumberInWindow(unit->ppMax, 4, conditionWindow, 56, 0);
							break;
						case TARGET_HINT_REVIVE:
							TARGET_HELP_COLUMN(7, TARGET_ONE_LINE_WIDTH);
							conditionWindow =
								mnWindowCreate(helpColumn,
							                   8,
							                   TARGET_ONE_LINE_WIDTH,
							                   3,
							                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							if (unit->hp != 0)
								goto no_condition;
							mnTextDrawMessageAsTiles(
								(Sint32)MsgEffectDowned, conditionWindow, 0, 0);
							break;
						case TARGET_HINT_CURE_POISON:
							TARGET_HELP_COLUMN(7, TARGET_ONE_LINE_WIDTH);
							conditionWindow =
								mnWindowCreate(helpColumn,
							                   8,
							                   TARGET_ONE_LINE_WIDTH,
							                   3,
							                   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							if (unit->poison == 0)
								goto no_condition;
							mnTextDrawMessageAsTiles(
								(Sint32)MsgEffectCurePoison, conditionWindow, 0, 0);
							break;
						case TARGET_HINT_RESTORE:
						{
							count = 0;
							if (unit->delusion != 0)
								count = 1;
							if (unit->stun != 0)
								count++;
							if (unit->sleep != 0)
								count++;
							if (unit->psySeal != 0)
								count++;
							if (unit->grimReaper != 0)
								count++;
							if (count == 0)
								count = 1;
							// The list ends on row 10, as the one-line windows do, and starts no
							// higher than row 4.
							helpRow = 9 - count;
							if (helpRow <= 3)
								helpRow = 4;
							TARGET_CONDITION_LIST_WINDOW();
							count = 0;
							if (unit->delusion != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgRemoveDelusion, conditionWindow, 0, 0);
								count = 1;
							}
							if (unit->stun != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgRemoveStun, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->sleep != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgWakeFromSleep, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->psySeal != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgBreakPsynergySeal, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->grimReaper != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgDispelGrimReaper, conditionWindow, 0, count * 8);
								count++;
							}
							if (count == 0)
							{
								mnTextSetColor(TEXT_COLOR_RED);
								mnTextDrawMessageAsTiles(
									(Sint32)MsgEffectNone, conditionWindow, 0, 0);
								mnTextSetColor(TEXT_COLOR_WHITE);
							}
							break;
						}
						default: break;
						case TARGET_HINT_CURE_ALL:
						{
							count = 0;
							if (unit->poison != 0)
								count = 1;
							if (unit->delusion != 0)
								count++;
							if (unit->stun != 0)
								count++;
							if (unit->sleep != 0)
								count++;
							if (unit->psySeal != 0)
								count++;
							if (unit->grimReaper != 0)
								count++;
							if (unit->haunt != 0)
								count++;
							// FAKEMATCH(dead): count read back through its own
							// * address.
							if (*&count == 0)
								count = 1;
							helpRow = 9 - count;
							if (helpRow <= 3)
								helpRow = 4;
							TARGET_CONDITION_LIST_WINDOW();
							count = 0;
							if (unit->poison != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgEffectCurePoison, conditionWindow, 0, 0);
								count = 1;
							}
							if (unit->delusion != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgRemoveDelusion, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->stun != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgRemoveStun, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->sleep != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgWakeFromSleep, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->psySeal != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgBreakPsynergySeal, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->grimReaper != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgDispelGrimReaper, conditionWindow, 0, count * 8);
								count++;
							}
							if (unit->haunt != 0)
							{
								mnTextDrawMessageAsTiles(
									(Sint32)MsgDispelEvilSpirit, conditionWindow, 0, count * 8);
								count++;
							}
							if (count != 0)
								break;
						}
						no_condition:
							mnTextSetColor(TEXT_COLOR_RED);
							mnTextDrawMessageAsTiles((Sint32)MsgEffectNone, conditionWindow, 0, 0);
							mnTextSetColor(TEXT_COLOR_WHITE);
							break;
					}
				}
			}
			else if (spread != ABILITY_RANGE_ALL)
			{
				// An enemy: its name, centred under it and kept within the window;
				// ENEMY_MYSTERY_MAN and ENEMY_MYSTERY_WOMAN are named "Mystery Man" and "Mystery
				// Woman".
				target = syUnitGet(candidateIds[targetIndex]);
				btMotionProjectCursorPointStub(candidateIds[targetIndex], &namePos);
				TARGET_BOB(namePos.y);
				if (target->enemyId == ENEMY_MYSTERY_WOMAN || target->enemyId == ENEMY_MYSTERY_MAN)
				{
					width = (Sint32)MsgMysteryMan;
					if (target->enemyId == ENEMY_MYSTERY_WOMAN)
						width++;
					TARGET_MYSTERY_NAME_COPY(width, targetName, TARGET_MYSTERY_NAME_CAPACITY);
				}
				else
				{
					for (i = 0; i < UNIT_NAME_SIZE - 1; i++)
					{
						targetName[i] = n = target->name[i];
						if (!n)
							break;
					}
					targetName[i] = 0;
				}
				width      = mnTextGetWideStringWidth(targetName);
				namePos.x -= width / 2;
				namePos.x -= 8;
				if (namePos.x + width > TARGET_NAME_INTERIOR_WIDTH)
					namePos.x = TARGET_NAME_INTERIOR_WIDTH - width;
				if (namePos.x < 0)
					namePos.x = 0;
				mnTextClearCanvas();
				mnTextDrawWideStringAtOffset(targetName, window, namePos.x, 4);
			}
			redraw &= ~TARGET_REDRAW_SPREAD;
		}

		if (drawCursors != 0)
		{
			for (i = 1; i < targetCount; i++)
			{
				marker = &firstSprite[i];
				track  = &cursorTracks[spreadTrackIndices[i]];
				btMotionProjectCursorPointStub(spreadActorIds[i], &markerPos);
				TARGET_BOB(markerPos.y);
				*marker = *firstSprite;
				if (track->flags & CURSOR_EASING)
				{
					Sint32 x    = (markerPos.x + track->x) / 2;
					Sint32 y    = (markerPos.y + track->y) / 2;

					markerPos.x = x;
					track->x    = x;
					markerPos.y = y;
					track->y    = y;
				}
				else
				{
					Sint32 x     = marker->x;
					Sint32 y     = marker->y + 8;

					track->flags = CURSOR_EASING;
					markerPos.x  = x;
					track->x     = x;
					markerPos.y  = y;
					track->y     = y;
				}
				marker->mode = 1;
				marker->x    = markerPos.x - 8;
				marker->y    = markerPos.y - 12;
				if (spread == ABILITY_RANGE_ALL)
					marker->affine = 0;
				else
					marker->affine = 1;
				marker->matrix = matrix;
				syOAMAddSpriteAtDepth(marker, OAM_DEPTH_WINDOW);
			}
		}

		pressed  = PerWork.push;
		repeated = PerWork.repeat;
		if (BattleMenuStateWork->tutorialStep != 0)
		{
			repeated = 0;
			pressed  = 0;
			if (BattleMenuStateWork->tutorialDelay == 0)
			{
				BattleMenuStateWork->tutorialDelay = 60;
				repeated                           = KEY_A;
				pressed                            = KEY_A;
			}
			else
			{
				BattleMenuStateWork->tutorialDelay--;
			}
		}
		// A: the aimed unit's side and place; the loop ends after this tick.
		if (pressed & KEY_A)
		{
			Sint32 id = candidateIds[targetIndex];
			Sint32 k;

			redraw = 0;
			result = -1;
			for (k = 0; k <= 5 && battle->partyUnits[k] != BATTLE_UNIT_LIST_END; k++)
			{
				if (battle->partyUnits[k] == id)
				{
					result = PLACE_PARTY | k;
					break;
				}
			}
			if (result < 0)
			{
				for (k = 0; k <= 5 && battle->enemyIds[k] != BATTLE_UNIT_LIST_END; k++)
				{
					if (battle->enemyIds[k] == id)
					{
						result = PLACE_ENEMY | k;
						break;
					}
				}
			}
			targetIndex = result;
		}
		else if (spread != ABILITY_RANGE_ALL)
		{
			if (repeated & (KEY_RIGHT | KEY_DOWN))
			{
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				do
				{
					targetIndex++;
					targetIndex %= candidateCount;
				} while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
				redraw |= TARGET_REDRAW_SPREAD;
			}
			if (repeated & (KEY_LEFT | KEY_UP))
			{
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				do
				{
					targetIndex  = targetIndex + candidateCount - 1;
					targetIndex %= candidateCount;
				} while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
				redraw |= TARGET_REDRAW_SPREAD;
			}
		}
		if (BattleMenuStateWork->timer == 0 || (pressed & KEY_B))
		{
			sySoundPlayStub(SE_MENU_CANCEL);
			targetIndex = -1;
			break;
		}
		sySynch(1);
		// FAKEMATCH(dead): cancelling updates of conditionWindow.
		conditionWindow++;
		conditionWindow--;
		if (redraw == 0)
			break;
	}
	sySynch(1);
	syVRAMCacheFreeEntry(cursorSlot);
	if (conditionWindow != NULL)
		mnWindowClose(conditionWindow, 1);
	mnWindowClose(window, 1);
	btHighlightSetModesStub(candidateIds, HIGHLIGHT_OFF);
	BattleMenuStateWork->targetViewOffset = 0;
	sySynch(1);
	return targetIndex;
}

#undef TARGET_SET_DJINNI_WIDTH
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
#	undef TARGET_SET_DJINNI_WIDTH_ITALIAN
#	undef TARGET_SET_DJINNI_WIDTH_SPANISH
#	undef TARGET_CONDITION_LIST_WIDTH_SPANISH
#endif
#undef TARGET_CONDITION_LIST_WIDTH
#undef TARGET_MYSTERY_NAME_COPY
#undef TARGET_MYSTERY_NAME_CAPACITY
#undef TARGET_GAUGE_WIDTH
#undef TARGET_ONE_LINE_WIDTH
#undef TARGET_HELP_COLUMN
#undef TARGET_CONDITION_LIST_WINDOW
#undef TARGET_BOB
#undef TARGET_NAME_WINDOW_ROW
#undef TARGET_NAME_INTERIOR_WIDTH
#undef TARGET_VIEW_OFFSET_ENEMIES
#undef TARGET_VIEW_OFFSET_PARTY
#undef CURSOR_EASING
#undef CURSOR_IN_SPREAD
#undef CURSOR_HOLD_TICK
#undef TARGET_REDRAW_SPREAD
#undef TARGET_REDRAW_ALL
