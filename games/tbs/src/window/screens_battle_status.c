/* The battle's Status screen, which the Status command opens. Part of
   screens.c. */

/* The Status screen's help text buffer, in characters, and the x of the Attack,
   Defense and Agility figures, in pixels. */
#define OWNER_STATUS_TEXT_MAX JAPANESE_OR(64, 128)
#define OWNER_STATUS_VALUE_X  JAPANESE_OR(176, 184)

/* The places of the status icons in a row beside the party member's name, 15
   pixels apart. */
#define OWNER_STATUS_ICON_PLACES 11
/* The status icons shown at most. */
#define OWNER_STATUS_ICONS_MAX	 8

/* The message of a stat's label on the Status screen, by its row. */
#if STATUS_LABELS_FROM_ATTACK
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) \
	(MsgOwnerStatusExp - OWNER_STATUS_LABEL_EXP + (row))
#endif

/* The Status screen's labels, in their message order from MsgOwnerStatusAttack
   (OWNER_STATUS_LABEL). */
typedef enum BATTLE_SUMMARY_LABEL {
	OWNER_STATUS_LABEL_ATTACK = 0,
	OWNER_STATUS_LABEL_DEFENSE = 1,
	OWNER_STATUS_LABEL_AGILITY = 2,
	OWNER_STATUS_LABEL_LUCK = 3,
	OWNER_STATUS_LABEL_ATTRIBUTES = 4, /* "Attributes", which nothing draws */
	OWNER_STATUS_LABEL_ELEMENT_LEVEL = 5, /* "Lv" */
	OWNER_STATUS_LABEL_POWER = 6,
	OWNER_STATUS_LABEL_RESIST = 7,
	OWNER_STATUS_LABEL_CLASS = 8, /* "Class", which nothing draws */
	OWNER_STATUS_LABEL_DJINN = 9,
	OWNER_STATUS_LABEL_EXP = 10 /* MsgOwnerStatusExp */
} BATTLE_SUMMARY_LABEL;

/* What the Status screen redraws on its next tick. */
#define OWNER_STATUS_REDRAW_ALL	 1 /* on opening, and on L or R */
#define OWNER_STATUS_REDRAW_HELP 2 /* only the help line */

/* The Status screen's cursor groups, each with its table of cursor stops. The
   name, experience, HP, PP, class, Djinn and element rows
   (BattleStatusScreenStatCursors). */
#define STATUS_CURSOR_GENERAL	   0
/* Attack, Defense, Agility and Luck (BattleStatusScreenEquipmentCursors). */
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS   2 /* the status icons */

/* The Status screen's cursor: its place in each group, the help entry it shows
   and where it points. */
typedef struct STAT_CURSOR {
	/* The cursor's place in each group. [3] is written for the one tick Left,
	   Right or A in STATUS_CURSOR_COMBAT_STATS leaves the group at a value no
	   branch reads; [4] is cleared at the start and never read. */
	Sint8 groupPositions[5];
	/* The help shown (BATTLE_SUMMARY_HELP). It decides the group. */
	Sint32 helpEntry;
	Sint32 drawnHelpEntry; /* the help drawn last; a change redraws the help */
	Sint32 group;		   /* STATUS_CURSOR_ group */
	Sint32 groupSize;	   /* how many stops the group has */
	Sint32 column;		   /* tiles */
	Sint32 row;			   /* tiles */
} STAT_CURSOR;

/* Moves `cursor` to the stop its group remembers in `stops`: that stop's help
   entry, column and row. */
#define STATUS_CURSOR_TAKE_STOP(cursor, stops) \
	((cursor).helpEntry = \
			(stops)[(cursor).groupPositions[(cursor).group]].entry, \
		(cursor).column = \
			(stops)[(cursor).groupPositions[(cursor).group]].column, \
		(cursor).row = (stops)[(cursor).groupPositions[(cursor).group]].row)

/* Runs the Status screen for party member `member` until B is pressed or the
   link battle's time runs out: the party member's level, experience, HP and PP,
   class, stats, Djinn, elements and statuses, with a help line for the item
   under the cursor. */
Sint32 mnBattleStatusScreenRun(Uint16 *list, Sint32 listCount, Sint32 member)
{
	MENU_SPR						  figureSprite;
	MENU_SPR						  cursorSprite;
	Sint32							  statusSlots[OWNER_STATUS_ICON_PLACES];
	Sint8							  statusKinds[OWNER_STATUS_ICON_PLACES];
	MENU_SPR						  statusSprites[OWNER_STATUS_ICON_PLACES];
	MENU_SPR						  portraitSprite;
	STAT_CURSOR						  cursor;
	WIN_STATE						 *state;
	WINDOW							 *statusWindow;
	WINDOW							 *helpWindow;
	Sint32							  memberIndex;
	Sint32							  cursorSlot;
	MENU_POINT						  cursorPos;
	Sint32							  portraitSlot;
	Sint32							  statusCount;
	Sint32							  hasDjinn;
	Sint32							  keys;
	Uint16							 *helpText;
	Sint32							  statusKind;
	Sint32							 *statusSlot;
	Sint32							  defenseDelta;
	Sint32							  i;
	Sint32							  j;
	Sint32							  attackDelta;
	Sint32							  redraw;
	UNIT							 *unit;
	UNIT							 *savedUnit;
	const BATTLE_SUMMARY_STAT_CURSOR *cursorStop;
	MENU_SPR						 *statusSprite;
	Sint32							  agilityDelta;
	Sint32							  helpValue;
	Sint32							  groupPos;
	BATTLE_MENU_STATE				 *navigation;
	Sint32							  iconX;

	state = WINDOW_STATE;
	redraw = OWNER_STATUS_REDRAW_ALL;
	memberIndex = -1;
	portraitSlot = syVRAMCacheAllocateEntry(512);
	statusCount = 0;
	hasDjinn = gmPartySumDjinnCountsStub(-1);

	navigation = BattleMenuStateWork;
	navigation->timerHidden = 1;
	if(navigation->timerWindow != 0) {
		mnWindowClose(navigation->timerWindow, 1);
		navigation->timerWindow = 0;
	}
	sySoundPlayStub(SE_MENU_CONFIRM);

	for(i = 0; i < 5; i++) {
		cursor.groupPositions[i] = 0;
	}
	cursor.helpEntry = 0;
	cursor.drawnHelpEntry = 0;
	cursor.group = STATUS_CURSOR_GENERAL;

	cursorSlot = syVRAMCacheAllocateEntry(128);
	for(i = 0; i < OWNER_STATUS_ICON_PLACES; i++) {
		statusSlots[i] = syVRAMCacheAllocateEntry(128);
		statusKinds[i] = OWNER_CONDITION_EMPTY;
	}

	/* Find the party member's place in the list, for L and R. */
	if(listCount != 0) {
		for(i = 0; i <= 5; i++) {
			Sint32 kind = list[i];

			if(kind == BATTLE_UNIT_LIST_END) {
				break;
			}
			if(kind != BATTLE_UNIT_REMOVED) {
				if(kind == member) {
					memberIndex = i;
					break;
				}
			}
		}
	}

	cursorPos.x = 0;
	cursorPos.y = 0;
	/* The stat grid and its explanation have separate lifetimes and redraws. */
	statusWindow = mnWindowCreate(0, 0, 30, 20, MENU_WINDOW_FLAGS);
	helpWindow = mnWindowCreate(
		0, 14, 30, 6, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
	mnWindowMarkVisibleTextTiles();

	for(;;) {
		keys = PerRepeat;

		/* The party member's 32x32 portrait at the top left. */
		portraitSprite.oam.word.attr01 =
			OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		portraitSprite.oam.word.attr23 = 0;
		portraitSprite.oam.f.tile = mnPortraitLoad(member, portraitSlot);
		portraitSprite.oam.f.x = 8;
		portraitSprite.oam.f.y = 24;
		portraitSprite.oam.f.palette = PORTRAIT_PALETTE;
		syOAMAddSpriteAtDepth(&portraitSprite, OAM_DEPTH_WINDOW);

		/* The group follows the help shown: the general rows have 9 stops with
		   Djinn, 7 without. */
		switch(cursor.helpEntry) {
		case OWNER_STATUS_HELP_NAME:
		case OWNER_STATUS_HELP_EXPERIENCE ... OWNER_STATUS_HELP_RESIST:
		default:
			cursor.group = STATUS_CURSOR_GENERAL;
			if(hasDjinn != 0) {
				cursor.groupSize = 9;
			} else {
				cursor.groupSize = 7;
			}
			break;
		case OWNER_STATUS_HELP_ATTACK ... OWNER_STATUS_HELP_LUCK:
			cursor.group = STATUS_CURSOR_COMBAT_STATS;
			cursor.groupSize = 4;
			break;
		case OWNER_STATUS_HELP_CONDITION ... OWNER_STATUS_HELP_LAST_CONDITION:
			cursor.group = STATUS_CURSOR_CONDITIONS;
			cursor.groupSize = statusCount;
			break;
		}

		mnWindowHighlightRect(statusWindow->x, statusWindow->y,
			statusWindow->width, statusWindow->height, WINDOW_PALETTE);

		if(statusCount != 0) {
			if((Uint32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
				groupPos = cursor.groupPositions[cursor.group];
				if((keys & KEY_DOWN) != 0) {
					keys = 0;
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
					groupPos++;
					if(groupPos >= cursor.groupSize) {
						groupPos = 0;
						if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
							cursor.group = STATUS_CURSOR_CONDITIONS;
							groupPos = cursor.groupPositions[cursor.group];
						}
					}
				} else if((keys & KEY_UP) != 0) {
					keys = 0;
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
					groupPos--;
					if(groupPos < 0) {
						groupPos = cursor.groupSize - 1;
						if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
							cursor.group = STATUS_CURSOR_CONDITIONS;
							groupPos = cursor.groupPositions[cursor.group];
						}
					}
				} else if((keys & (KEY_A | KEY_RIGHT | KEY_LEFT)) != 0) {
					keys = 0;
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
					/* A, Right or Left toggles between the general group and
					   the statuses. */
					cursor.group ^= STATUS_CURSOR_CONDITIONS;
					if((PerRepeat & KEY_A) != 0) {
						groupPos = cursor.groupPositions[cursor.group];
					} else if((PerRepeat & KEY_LEFT) != 0) {
						groupPos = statusCount - 1;
					} else {
						groupPos = 0;
					}
				}
				cursor.groupPositions[cursor.group] = (Sint8)groupPos;
			} else if(cursor.group == STATUS_CURSOR_CONDITIONS) {
				groupPos = cursor.groupPositions[cursor.group];
				if(groupPos >= cursor.groupSize) {
					groupPos = cursor.groupSize - 1;
				}
				if(groupPos < 0) {
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPos = cursor.groupPositions[0];
				} else if((keys & KEY_RIGHT) != 0) {
					keys = 0;
					groupPos++;
					if(groupPos >= cursor.groupSize) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPos = cursor.groupPositions[0];
					}
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				} else if((keys & KEY_LEFT) != 0) {
					keys = 0;
					groupPos--;
					if(groupPos < 0) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPos = cursor.groupPositions[0];
					}
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				} else if((keys & (KEY_A | KEY_UP | KEY_DOWN)) != 0) {
					/* A, Up or Down go back to the general group. */
					keys = 0;
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPos = cursor.groupPositions[0];
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				}
				cursor.groupPositions[cursor.group] = (Sint8)groupPos;
			}
		}

		if(cursor.group == STATUS_CURSOR_GENERAL) {
			Sint32 k = cursor.groupPositions[cursor.group];

			/* Without Djinn in the party the stops come from the table's second
			   part, which leaves out the class and Djinn stops: 7 stops instead
			   of 9. */
			if(hasDjinn == 0) {
				k += 9;
			}
			cursorStop = &BattleStatusScreenStatCursors[k];
			cursor.helpEntry = cursorStop->entry;
			cursor.column = cursorStop->column;
			cursor.row = cursorStop->row;
			mnWindowHighlightRect(
				statusWindow->x + cursorStop->highlightColumn + 1,
				statusWindow->y + cursorStop->highlightRow + 1,
				cursorStop->highlightWidth, 1, HIGHLIGHT_PALETTE);
		} else if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
			STATUS_CURSOR_TAKE_STOP(cursor, BattleStatusScreenEquipmentCursors);
		} else if(cursor.group == STATUS_CURSOR_CONDITIONS) {
			STATUS_CURSOR_TAKE_STOP(cursor, BattleStatusScreenConditionCursors);
		}

		if(cursor.drawnHelpEntry != cursor.helpEntry) {
			cursor.drawnHelpEntry = cursor.helpEntry;
			redraw = OWNER_STATUS_REDRAW_HELP;
		}
		cursorPos.x = cursor.column * 8;
		cursorPos.y = cursor.row * 8;

		/* The party member's battle figure: its battle object's sprite image's
		   tiles, as a 64x64, 256-colour sprite at (172, 56). */
		{
			SPR *figure = btObjectGetStub(member)->actor->sprite;

			figureSprite.oam.word.attr01 = OBJ_SIZE_64X64 | OBJ_256_COLORS |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			figureSprite.oam.word.attr23 = 0;
			ASM_NOTHING();
			figureSprite.oam.f.tile = figure->image.tile;
			figureSprite.oam.f.x = 172;
			figureSprite.oam.f.y = 56;
			syOAMAddSpriteAtDepth(&figureSprite, OAM_DEPTH_WINDOW);
		}

		cursorSprite.oam.word.attr01 =
			OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		cursorSprite.oam.word.attr23 = 0;
		ASM_NOTHING();
		cursorSprite.oam.f.tile =
			syVRAMCacheReload(cursorSlot, (Sint32)PointingHandTiles);
		cursorSprite.oam.f.x =
			(cursorPos.x + statusWindow->x * 8 - ((TickCount & 4) >> 2)) + 16;
		cursorSprite.oam.f.y =
			(cursorPos.y + statusWindow->y * 8 - ((TickCount & 4) >> 2)) + 16;
		/* 8 sets attribute 1's bit 12, a sprite's horizontal flip when it is
		   not affine: the hand is mirrored. */
		cursorSprite.oam.f.affineIndex = 8;
		syOAMAddSpriteAtDepth(&cursorSprite, OAM_DEPTH_WINDOW + 1);

		if(redraw != 0) {
			unit = gmUnitGetStub(member);
#if BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW
			mnWindowMarkVisibleTextTiles();
#endif
			mnTextFillCanvasBackground();

			if((redraw & OWNER_STATUS_REDRAW_ALL) != 0) {
				mnWindowSpriteRedrawSavedRect(statusWindow);
				mnWindowMapTextCanvasTiles(helpWindow->x, helpWindow->y,
					helpWindow->width, helpWindow->height, 0);
				mnWindowDrawDividerLine(statusWindow, 0, 14, 29, 14);

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_FRENCH:
					mnTextDrawStringAtOffset(
						(Uint8 *)unit->name, statusWindow, 0, 0);
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenNivString, statusWindow, 51,
						0);
					mnTextDrawNumberInWindow(
						unit->level, 2, statusWindow, 72, 0);
					break;
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(
						(Uint8 *)unit->name, statusWindow, 0, 0);
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenNvString, statusWindow, 56,
						0);
					mnTextDrawNumberInWindow(
						unit->level, 2, statusWindow, 72, 0);
					break;
				default:
					mnTextDrawStringAtOffset(
						(Uint8 *)unit->name, statusWindow, 0, 0);
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenLevelString, statusWindow,
						56, 0);
					mnTextDrawNumberInWindow(
						unit->level, 2, statusWindow, 72, 0);
					break;
				}
#else
				mnTextDrawStringAtOffset(
					(Uint8 *)unit->name, statusWindow, 0, 0);
				mnTextDrawStringAtOffset((Uint8 *)BattleStatusScreenLevelString,
					statusWindow, OWNER_STATUS_LEVEL_X, 0);
				mnTextDrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
#endif
				mnTextDrawMessageAsTiles(MsgOwnerStatusExp, statusWindow, 0, 8);
				mnTextDrawNumberInWindow(
					unit->experience, 8, statusWindow, 40, 8);

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_GERMAN:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenKpString, statusWindow, 40,
						24);
					break;
				case CARTRIDGE_LANGUAGE_FRENCH:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenPvString, statusWindow, 40,
						24);
					break;
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenPvString, statusWindow, 40,
						24);
					break;
				case CARTRIDGE_LANGUAGE_ITALIAN:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenHpString, statusWindow, 40,
						24);
					break;
				}
#else
				mnTextDrawStringAtOffset(
					(Uint8 *)BattleStatusScreenHpString, statusWindow, 40, 24);
#endif
				mnTextDrawNumberInWindow(unit->hp, 4, statusWindow, 56, 24);
				mnTextDrawStringAtOffset((Uint8 *)BattleStatusScreenSlashString,
					statusWindow, 88, 24);
				mnTextDrawNumberInWindow(unit->hpMax, 4, statusWindow, 96, 24);
				mnTextDrawStringAtOffset(
					(Uint8 *)BattleStatusScreenPpString, statusWindow, 40, 32);
				mnTextDrawNumberInWindow(unit->pp, 4, statusWindow, 56, 32);
				mnTextDrawStringAtOffset((Uint8 *)BattleStatusScreenSlashString,
					statusWindow, 88, 32);
				mnTextDrawNumberInWindow(unit->ppMax, 4, statusWindow, 96, 32);

				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ATTACK), statusWindow,
					136, 16);
				mnTextDrawNumberInWindow(
					unit->attack, 3, statusWindow, OWNER_STATUS_VALUE_X, 16);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DEFENSE),
					statusWindow, 136, 24);
				mnTextDrawNumberInWindow(
					unit->defense, 3, statusWindow, OWNER_STATUS_VALUE_X, 24);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_AGILITY),
					statusWindow, 136, 32);
				mnTextDrawNumberInWindow(
					unit->agility, 3, statusWindow, OWNER_STATUS_VALUE_X, 32);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_LUCK), statusWindow,
					136, 40);
				mnTextDrawNumberInWindow(
					unit->luck, 3, statusWindow, OWNER_STATUS_VALUE_X, 40);

				mnTextDrawMessageAsTiles(
					MsgClassName + unit->classId, statusWindow, 0, 48);
				if(hasDjinn != 0) {
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DJINN),
						statusWindow, 0, 72);
				}
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ELEMENT_LEVEL),
					statusWindow, 0, 80);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_POWER), statusWindow,
					0, 88);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_RESIST), statusWindow,
					0, 96);

				for(j = 0; j < ELEMENT_COUNT; j++) {
					Sint32 palette;

					palette = 1;
					if(hasDjinn != 0) {
						palette = 0;
					}
					mnWindowSetTilemapEntry(statusWindow,
						LIST_TILE_SUMMON_ELEMENT + j, 7 + j * 4, palette + 8,
						0);
					if(hasDjinn != 0) {
						mnTextDrawNumberInWindow(unit->djinnSetCounts[j], 1,
							statusWindow, 40 + j * 32, 72);
						mnTextDrawStringAtOffset(
							(Uint8 *)BattleStatusScreenSlashString,
							statusWindow, 48 + j * 32, 72);
						mnTextDrawNumberInWindow(unit->djinnHeldCounts[j], 1,
							statusWindow, 48 + j * 32 + 8, 72);
					}
					mnTextDrawNumberInWindow(
						gmUnitGetElementLevelStub(member, j), 2, statusWindow,
						48 + j * 32, 80);
					mnTextDrawNumberInWindow(unit->element[j].power, 3,
						statusWindow, 40 + j * 32, 88);
					mnTextDrawNumberInWindow(unit->element[j].resistance, 3,
						statusWindow, 40 + j * 32, 96);
				}

				/* The status icons, at most OWNER_STATUS_ICONS_MAX, in this
				   order:
				   downed, cursed item, Grim Reaper, poison, deadly poison,
				   Psynergy sealed, stun, sleep, haunt, delusion, then Attack,
				   Defense and Resistance raised or lowered, and Agility.
				   OWNER_CONDITION_NORMAL alone means none. */
				i = 0;
				if(unit->hp == 0) {
					statusKinds[0] = OWNER_CONDITION_DOWNED;
					i = 1;
				}
				do {
					if(unit->curse != 0) {
						statusKinds[i++] = OWNER_CONDITION_CURSED_ITEM;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->grimReaper != 0) {
						statusKinds[i++] = OWNER_CONDITION_GRIM_REAPER;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->poison == 1) {
						statusKinds[i++] = OWNER_CONDITION_POISON;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->poison == 2) {
						statusKinds[i++] = OWNER_CONDITION_DEADLY_POISON;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->psySeal != 0) {
						statusKinds[i++] = OWNER_CONDITION_PSYNERGY_SEALED;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->stun != 0) {
						statusKinds[i++] = OWNER_CONDITION_STUN;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->sleep != 0) {
						statusKinds[i++] = OWNER_CONDITION_SLEEP;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->haunt != 0) {
						statusKinds[i++] = OWNER_CONDITION_HAUNT;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->delusion != 0) {
						statusKinds[i++] = OWNER_CONDITION_DELUSION;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->attackTurns != 0) {
						if(unit->attackStage > 0) {
							statusKinds[i++] = OWNER_CONDITION_ATTACK_UP;
						}
						if(unit->attackStage < 0) {
							statusKinds[i++] = OWNER_CONDITION_ATTACK_DOWN;
						}
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->defenseTurns != 0) {
						if(unit->defenseStage > 0) {
							statusKinds[i++] = OWNER_CONDITION_DEFENSE_UP;
						}
						if(unit->defenseStage < 0) {
							statusKinds[i++] = OWNER_CONDITION_DEFENSE_DOWN;
						}
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->resistanceTurns != 0) {
						if(unit->resistanceStage > 0) {
							statusKinds[i++] = OWNER_CONDITION_RESISTANCE_UP;
						}
						if(unit->resistanceStage < 0) {
							statusKinds[i++] = OWNER_CONDITION_RESISTANCE_DOWN;
						}
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->agilityStage > 0) {
						statusKinds[i++] = OWNER_CONDITION_AGILITY_UP;
					}
					if(unit->agilityStage < 0) {
						statusKinds[i++] = OWNER_CONDITION_AGILITY_DOWN;
					}
				} while(0);

				for(j = 0; j < i; j++) {
					mnStatusIconLoadIntoSlot(statusKinds[j], statusSlots[j]);
				}
				if(i == 0) {
					statusKinds[0] = OWNER_CONDITION_NORMAL;
					i = 1;
				}
				if(i < OWNER_STATUS_ICON_PLACES) {
					for(j = i; j < OWNER_STATUS_ICON_PLACES; j++) {
						statusKinds[j] = OWNER_CONDITION_EMPTY;
					}
				}
				statusCount = i;
				if(statusKinds[0] == OWNER_CONDITION_NORMAL) {
					mnTextDrawMessageAsTiles(unit->hp != 0
							? MsgOwnerStatusNormal
							: MsgOwnerStatusDowned,
						statusWindow, 112, 0);
				}
			}

			helpText = (Uint16 *)syHeapAllocate(
				OWNER_STATUS_TEXT_MAX * sizeof(*helpText));
			if((Uint32)cursor.helpEntry >= OWNER_STATUS_HELP_CONDITION) {
				statusKind =
					statusKinds[cursor.helpEntry - OWNER_STATUS_HELP_CONDITION];
				if(statusKind == OWNER_CONDITION_NORMAL && unit->hp == 0) {
					statusKind = OWNER_CONDITION_DOWNED;
				}

				/* A status's help carries a number: the Grim Reaper's turns
				   left, what the stat stages add (the stats less those
				   recalculated with the stages cleared, after which the unit is
				   restored from a copy), or RESISTANCE_PER_STAGE for each
				   Resistance stage. */
				savedUnit = (UNIT *)syHeapAllocate(sizeof(*unit));
				helpValue = 0;
				syWordsCopy(savedUnit, unit, sizeof(*unit));

				attackDelta = unit->attack;
				defenseDelta = unit->defense;
				agilityDelta = unit->agility;
				unit->attackStage = 0;
				unit->defenseStage = 0;
				unit->agilityStage = 0;
				gmUnitCalculateStub(member);
				attackDelta -= unit->attack;
				defenseDelta -= unit->defense;
				agilityDelta -= unit->agility;
				syWordsCopy(unit, savedUnit, sizeof(*unit));
				syHeapRelease(savedUnit);

				switch(statusKind) {
				case OWNER_CONDITION_GRIM_REAPER:
					helpValue = unit->grimReaper;
					break;
				case OWNER_CONDITION_ATTACK_UP:
					helpValue = attackDelta;
					break;
				case OWNER_CONDITION_ATTACK_DOWN:
					helpValue = -attackDelta;
					break;
				case OWNER_CONDITION_DEFENSE_UP:
					helpValue = defenseDelta;
					break;
				case OWNER_CONDITION_DEFENSE_DOWN:
					helpValue = -defenseDelta;
					break;
				case OWNER_CONDITION_AGILITY_UP:
					helpValue = agilityDelta;
					break;
				case OWNER_CONDITION_AGILITY_DOWN:
					helpValue = -agilityDelta;
					break;
				case OWNER_CONDITION_RESISTANCE_UP:
				case OWNER_CONDITION_RESISTANCE_DOWN:
					helpValue = unit->resistanceStage * RESISTANCE_PER_STAGE;
#if RESISTANCE_DOWN_HELP_NO_MINUS
					if(helpValue < 0) {
						helpValue = -helpValue;
					}
#endif
					break;
				default:
					break;
				}
				mnMessageArgumentPush(helpValue, MESSAGE_ARGUMENT_NUMBER);
				mnMessageCopyExpanded(statusKind + MsgOwnerStatusConditionHelp,
					helpText, OWNER_STATUS_TEXT_MAX);
			} else if(cursor.helpEntry == OWNER_STATUS_HELP_EXPERIENCE &&
				unit->level < UNIT_LEVEL_MAX) {
				/* Experience below level 99: the points still needed for the
				   next. */
				mnMessageArgumentPush(
					gmUnitGetExperienceForLevelStub(member, unit->level + 1) -
						unit->experience,
					MESSAGE_ARGUMENT_NUMBER);
				mnMessageCopyExpanded(
					MsgOwnerStatusNextLevel, helpText, OWNER_STATUS_TEXT_MAX);
			} else {
				mnMessageCopyExpanded(
					cursor.helpEntry + MsgOwnerStatusEntryHelp, helpText,
					OWNER_STATUS_TEXT_MAX);
			}

			mnTextDrawWideStringAtOffset(helpText, (WINDOW *)helpWindow, 0, 4);
			syHeapRelease(helpText);
			redraw = 0;
			state->dirty = 1;
		}

		/* The status icons, 15 pixels apart from x 112 on the first row;
		   OWNER_CONDITION_NORMAL and empty places are not drawn. */
		statusSprite = statusSprites;
		statusSlot = statusSlots;
		{
			VRAM_CACHE_ENTRY *entry;

			for(i = 0, entry = VramCacheEntries, iconX = 112;
				i < OWNER_STATUS_ICON_PLACES; i++) {
				MENU_SPR_INIT(*statusSprite,
					OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				statusSprite->oam.f.tile = entry[*statusSlot++].offset >> 5;
				statusSprite->oam.f.x = iconX;
				statusSprite->oam.f.y = (Uint8)(statusWindow->y * 8 + 8);
				if(statusKinds[i] > OWNER_CONDITION_NORMAL) {
					syOAMAddSpriteAtDepth(statusSprite, OAM_DEPTH_WINDOW);
				}
				iconX += 15;
				statusSprite++;
			}
		}

		if(BattleMenuStateWork->timer == 0) {
			break;
		}
		if((PerPush & KEY_B) != 0) {
			break;
		}

		if(listCount != 0) {
			if((keys & KEY_R) != 0) {
				memberIndex++;
				if(memberIndex >= listCount) {
					memberIndex = 0;
				}
				member = list[memberIndex];
				redraw = OWNER_STATUS_REDRAW_ALL;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			} else if((keys & KEY_L) != 0) {
				memberIndex--;
				if(memberIndex < 0) {
					memberIndex = listCount - 1;
				}
				member = list[memberIndex];
				redraw = OWNER_STATUS_REDRAW_ALL;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		sySynch(1);
	}

	{
		Sint32 *statusSlotCursor = statusSlots;

		for(i = 0; i < OWNER_STATUS_ICON_PLACES; i++) {
			syVRAMCacheFreeEntry(*statusSlotCursor++);
		}
	}
	syVRAMCacheFreeEntry(portraitSlot);
	syVRAMCacheFreeEntry(cursorSlot);
	sySynch(1);
#if BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE
	mnWindowMarkVisibleTextTiles();
#endif
	mnWindowClose(statusWindow, 1);
	mnWindowClose(helpWindow, 1);
	mnPartyStatusDraw(BattleStateWork->partyStatus);
	BattleMenuStateWork->timerHidden = 0;
	sySynch(1);
	return 0;
}
