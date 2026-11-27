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
Sint32 mnBattleStatScreenRun(Uint16 *list, Sint32 listCnt, Sint32 member)
{
	MENU_SPR						  figureSpr;
	MENU_SPR						  cursorSpr;
	Sint32							  statSlots[OWNER_STATUS_ICON_PLACES];
	Sint8							  statKinds[OWNER_STATUS_ICON_PLACES];
	MENU_SPR						  statSprs[OWNER_STATUS_ICON_PLACES];
	MENU_SPR						  portraitSpr;
	STAT_CURSOR						  cursor;
	WIN_STATE						 *state;
	WINDOW							 *statWin;
	WINDOW							 *helpWin;
	Sint32							  memberIdx;
	Sint32							  cursorSlot;
	MENU_POINT						  cursorPos;
	Sint32							  portraitSlot;
	Sint32							  statCnt;
	Sint32							  hasDjinn;
	Sint32							  keys;
	Uint16							 *helpText;
	Sint32							  statKind;
	Sint32							 *statSlot;
	Sint32							  defenseDelta;
	Sint32							  i;
	Sint32							  j;
	Sint32							  attackDelta;
	Sint32							  redraw;
	UNIT							 *unit;
	UNIT							 *savedUnit;
	const BATTLE_SUMMARY_STAT_CURSOR *cursorStop;
	MENU_SPR						 *statSpr;
	Sint32							  agilityDelta;
	Sint32							  helpValue;
	Sint32							  groupPos;
	BATTLE_MENU_STATE				 *navigation;
	Sint32							  iconX;

	state = WINDOW_STATE;
	redraw = OWNER_STATUS_REDRAW_ALL;
	memberIdx = -1;
	portraitSlot = syVRAMCacheAllocEntry(512);
	statCnt = 0;
	hasDjinn = gmPartySumDjinnCountsStub(-1);

	navigation = BattleMenuStateWork;
	navigation->tmrHidden = 1;
	if(navigation->tmrWin != 0) {
		mnWindowClose(navigation->tmrWin, 1);
		navigation->tmrWin = 0;
	}
	sySndPlayStub(SE_MENU_CONFIRM);

	for(i = 0; i < 5; i++) {
		cursor.groupPositions[i] = 0;
	}
	cursor.helpEntry = 0;
	cursor.drawnHelpEntry = 0;
	cursor.group = STATUS_CURSOR_GENERAL;

	cursorSlot = syVRAMCacheAllocEntry(128);
	for(i = 0; i < OWNER_STATUS_ICON_PLACES; i++) {
		statSlots[i] = syVRAMCacheAllocEntry(128);
		statKinds[i] = OWNER_CONDITION_EMPTY;
	}

	/* Find the party member's place in the list, for L and R. */
	if(listCnt != 0) {
		for(i = 0; i <= 5; i++) {
			Sint32 kind = list[i];

			if(kind == BATTLE_UNIT_LIST_END) {
				break;
			}
			if(kind != BATTLE_UNIT_REMOVED) {
				if(kind == member) {
					memberIdx = i;
					break;
				}
			}
		}
	}

	cursorPos.x = 0;
	cursorPos.y = 0;
	/* The stat grid and its explanation have separate lifetimes and redraws. */
	statWin = mnWindowCreate(0, 0, 30, 20, MENU_WINDOW_FLAGS);
	helpWin = mnWindowCreate(
		0, 14, 30, 6, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
	mnWindowMarkVisibleTextTiles();

	for(;;) {
		keys = PerRepeat;

		/* The party member's 32x32 portrait at the top left. */
		portraitSpr.oam.word.attr01 =
			OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		portraitSpr.oam.word.attr23 = 0;
		portraitSpr.oam.f.tile = mnPortraitLoad(member, portraitSlot);
		portraitSpr.oam.f.x = 8;
		portraitSpr.oam.f.y = 24;
		portraitSpr.oam.f.pal = PORTRAIT_PALETTE;
		syOAMAddSprAtDepth(&portraitSpr, OAM_DEPTH_WINDOW);

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
			cursor.groupSize = statCnt;
			break;
		}

		mnWindowHighlightRect(statWin->x, statWin->y, statWin->width,
			statWin->height, WINDOW_PALETTE);

		if(statCnt != 0) {
			if((Uint32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
				groupPos = cursor.groupPositions[cursor.group];
				if((keys & KEY_DOWN) != 0) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
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
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
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
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					/* A, Right or Left toggles between the general group and
					   the statuses. */
					cursor.group ^= STATUS_CURSOR_CONDITIONS;
					if((PerRepeat & KEY_A) != 0) {
						groupPos = cursor.groupPositions[cursor.group];
					} else if((PerRepeat & KEY_LEFT) != 0) {
						groupPos = statCnt - 1;
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
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				} else if((keys & KEY_LEFT) != 0) {
					keys = 0;
					groupPos--;
					if(groupPos < 0) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPos = cursor.groupPositions[0];
					}
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				} else if((keys & (KEY_A | KEY_UP | KEY_DOWN)) != 0) {
					/* A, Up or Down go back to the general group. */
					keys = 0;
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPos = cursor.groupPositions[0];
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
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
			mnWindowHighlightRect(statWin->x + cursorStop->highlightColumn + 1,
				statWin->y + cursorStop->highlightRow + 1,
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
			SPR *figure = btObjectGetStub(member)->actor->spr;

			figureSpr.oam.word.attr01 = OBJ_SIZE_64X64 | OBJ_256_COLORS |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			figureSpr.oam.word.attr23 = 0;
			ASM_NOTHING();
			figureSpr.oam.f.tile = figure->image.tile;
			figureSpr.oam.f.x = 172;
			figureSpr.oam.f.y = 56;
			syOAMAddSprAtDepth(&figureSpr, OAM_DEPTH_WINDOW);
		}

		cursorSpr.oam.word.attr01 =
			OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		cursorSpr.oam.word.attr23 = 0;
		ASM_NOTHING();
		cursorSpr.oam.f.tile =
			syVRAMCacheReload(cursorSlot, (Sint32)PointingHandTiles);
		cursorSpr.oam.f.x =
			(cursorPos.x + statWin->x * 8 - ((TickCount & 4) >> 2)) + 16;
		cursorSpr.oam.f.y =
			(cursorPos.y + statWin->y * 8 - ((TickCount & 4) >> 2)) + 16;
		/* 8 sets attribute 1's bit 12, a sprite's horizontal flip when it is
		   not affine: the hand is mirrored. */
		cursorSpr.oam.f.affineIdx = 8;
		syOAMAddSprAtDepth(&cursorSpr, OAM_DEPTH_WINDOW + 1);

		if(redraw != 0) {
			unit = gmUnitGetStub(member);
#if BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW
			mnWindowMarkVisibleTextTiles();
#endif
			mnTextFillCanvasBackground();

			if((redraw & OWNER_STATUS_REDRAW_ALL) != 0) {
				mnWindowSprRedrawSavedRect(statWin);
				mnWindowMapTextCanvasTiles(
					helpWin->x, helpWin->y, helpWin->width, helpWin->height, 0);
				mnWindowDrawDividerLine(statWin, 0, 14, 29, 14);

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_FRENCH:
					mnTextDrawStringAtOffset(
						(Uint8 *)unit->name, statWin, 0, 0);
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenNivString, statWin, 51, 0);
					mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
					break;
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(
						(Uint8 *)unit->name, statWin, 0, 0);
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenNvString, statWin, 56, 0);
					mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
					break;
				default:
					mnTextDrawStringAtOffset(
						(Uint8 *)unit->name, statWin, 0, 0);
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenLevelString, statWin, 56, 0);
					mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
					break;
				}
#else
				mnTextDrawStringAtOffset((Uint8 *)unit->name, statWin, 0, 0);
				mnTextDrawStringAtOffset((Uint8 *)BattleStatusScreenLevelString,
					statWin, OWNER_STATUS_LEVEL_X, 0);
				mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
#endif
				mnTextDrawMessageAsTiles(MsgOwnerStatusExp, statWin, 0, 8);
				mnTextDrawNumInWindow(unit->experience, 8, statWin, 40, 8);

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_GERMAN:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenKpString, statWin, 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_FRENCH:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenPvString, statWin, 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenPvString, statWin, 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_ITALIAN:
					mnTextDrawStringAtOffset(
						(Uint8 *)BattleStatusScreenHpString, statWin, 40, 24);
					break;
				}
#else
				mnTextDrawStringAtOffset(
					(Uint8 *)BattleStatusScreenHpString, statWin, 40, 24);
#endif
				mnTextDrawNumInWindow(unit->hp, 4, statWin, 56, 24);
				mnTextDrawStringAtOffset(
					(Uint8 *)BattleStatusScreenSlashString, statWin, 88, 24);
				mnTextDrawNumInWindow(unit->hpMax, 4, statWin, 96, 24);
				mnTextDrawStringAtOffset(
					(Uint8 *)BattleStatusScreenPpString, statWin, 40, 32);
				mnTextDrawNumInWindow(unit->pp, 4, statWin, 56, 32);
				mnTextDrawStringAtOffset(
					(Uint8 *)BattleStatusScreenSlashString, statWin, 88, 32);
				mnTextDrawNumInWindow(unit->ppMax, 4, statWin, 96, 32);

				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ATTACK), statWin, 136,
					16);
				mnTextDrawNumInWindow(
					unit->attack, 3, statWin, OWNER_STATUS_VALUE_X, 16);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DEFENSE), statWin,
					136, 24);
				mnTextDrawNumInWindow(
					unit->defense, 3, statWin, OWNER_STATUS_VALUE_X, 24);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_AGILITY), statWin,
					136, 32);
				mnTextDrawNumInWindow(
					unit->agility, 3, statWin, OWNER_STATUS_VALUE_X, 32);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_LUCK), statWin, 136,
					40);
				mnTextDrawNumInWindow(
					unit->luck, 3, statWin, OWNER_STATUS_VALUE_X, 40);

				mnTextDrawMessageAsTiles(
					MsgClassName + unit->classId, statWin, 0, 48);
				if(hasDjinn != 0) {
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DJINN), statWin,
						0, 72);
				}
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ELEMENT_LEVEL),
					statWin, 0, 80);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_POWER), statWin, 0,
					88);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_RESIST), statWin, 0,
					96);

				for(j = 0; j < ELEMENT_COUNT; j++) {
					Sint32 pal;

					pal = 1;
					if(hasDjinn != 0) {
						pal = 0;
					}
					mnWindowSetTilemapEntry(statWin,
						LIST_TILE_SUMMON_ELEMENT + j, 7 + j * 4, pal + 8, 0);
					if(hasDjinn != 0) {
						mnTextDrawNumInWindow(
							unit->djinnSetCnts[j], 1, statWin, 40 + j * 32, 72);
						mnTextDrawStringAtOffset(
							(Uint8 *)BattleStatusScreenSlashString, statWin,
							48 + j * 32, 72);
						mnTextDrawNumInWindow(unit->djinnHeldCnts[j], 1,
							statWin, 48 + j * 32 + 8, 72);
					}
					mnTextDrawNumInWindow(gmUnitGetElementLevelStub(member, j),
						2, statWin, 48 + j * 32, 80);
					mnTextDrawNumInWindow(
						unit->element[j].power, 3, statWin, 40 + j * 32, 88);
					mnTextDrawNumInWindow(unit->element[j].resistance, 3,
						statWin, 40 + j * 32, 96);
				}

				/* The status icons, at most OWNER_STATUS_ICONS_MAX, in this
				   order:
				   downed, cursed item, Grim Reaper, poison, deadly poison,
				   Psynergy sealed, stun, sleep, haunt, delusion, then Attack,
				   Defense and Resistance raised or lowered, and Agility.
				   OWNER_CONDITION_NORMAL alone means none. */
				i = 0;
				if(unit->hp == 0) {
					statKinds[0] = OWNER_CONDITION_DOWNED;
					i = 1;
				}
				do {
					if(unit->curse != 0) {
						statKinds[i++] = OWNER_CONDITION_CURSED_ITEM;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->grimReaper != 0) {
						statKinds[i++] = OWNER_CONDITION_GRIM_REAPER;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->poison == 1) {
						statKinds[i++] = OWNER_CONDITION_POISON;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->poison == 2) {
						statKinds[i++] = OWNER_CONDITION_DEADLY_POISON;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->psySeal != 0) {
						statKinds[i++] = OWNER_CONDITION_PSYNERGY_SEALED;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->stun != 0) {
						statKinds[i++] = OWNER_CONDITION_STUN;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->sleep != 0) {
						statKinds[i++] = OWNER_CONDITION_SLEEP;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->haunt != 0) {
						statKinds[i++] = OWNER_CONDITION_HAUNT;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->delusion != 0) {
						statKinds[i++] = OWNER_CONDITION_DELUSION;
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->attackTurns != 0) {
						if(unit->attackStage > 0) {
							statKinds[i++] = OWNER_CONDITION_ATTACK_UP;
						}
						if(unit->attackStage < 0) {
							statKinds[i++] = OWNER_CONDITION_ATTACK_DOWN;
						}
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->defenseTurns != 0) {
						if(unit->defenseStage > 0) {
							statKinds[i++] = OWNER_CONDITION_DEFENSE_UP;
						}
						if(unit->defenseStage < 0) {
							statKinds[i++] = OWNER_CONDITION_DEFENSE_DOWN;
						}
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->resistanceTurns != 0) {
						if(unit->resistanceStage > 0) {
							statKinds[i++] = OWNER_CONDITION_RESISTANCE_UP;
						}
						if(unit->resistanceStage < 0) {
							statKinds[i++] = OWNER_CONDITION_RESISTANCE_DOWN;
						}
					}
					if(i >= OWNER_STATUS_ICONS_MAX) {
						break;
					}
					if(unit->agilityStage > 0) {
						statKinds[i++] = OWNER_CONDITION_AGILITY_UP;
					}
					if(unit->agilityStage < 0) {
						statKinds[i++] = OWNER_CONDITION_AGILITY_DOWN;
					}
				} while(0);

				for(j = 0; j < i; j++) {
					mnStatIconLoadIntoSlot(statKinds[j], statSlots[j]);
				}
				if(i == 0) {
					statKinds[0] = OWNER_CONDITION_NORMAL;
					i = 1;
				}
				if(i < OWNER_STATUS_ICON_PLACES) {
					for(j = i; j < OWNER_STATUS_ICON_PLACES; j++) {
						statKinds[j] = OWNER_CONDITION_EMPTY;
					}
				}
				statCnt = i;
				if(statKinds[0] == OWNER_CONDITION_NORMAL) {
					mnTextDrawMessageAsTiles(unit->hp != 0
							? MsgOwnerStatusNormal
							: MsgOwnerStatusDowned,
						statWin, 112, 0);
				}
			}

			helpText = (Uint16 *)syHeapAlloc(
				OWNER_STATUS_TEXT_MAX * sizeof(*helpText));
			if((Uint32)cursor.helpEntry >= OWNER_STATUS_HELP_CONDITION) {
				statKind =
					statKinds[cursor.helpEntry - OWNER_STATUS_HELP_CONDITION];
				if(statKind == OWNER_CONDITION_NORMAL && unit->hp == 0) {
					statKind = OWNER_CONDITION_DOWNED;
				}

				/* A status's help carries a number: the Grim Reaper's turns
				   left, what the stat stages add (the stats less those
				   recalculated with the stages cleared, after which the unit is
				   restored from a copy), or RESISTANCE_PER_STAGE for each
				   Resistance stage. */
				savedUnit = (UNIT *)syHeapAlloc(sizeof(*unit));
				helpValue = 0;
				syWordsCopy(savedUnit, unit, sizeof(*unit));

				attackDelta = unit->attack;
				defenseDelta = unit->defense;
				agilityDelta = unit->agility;
				unit->attackStage = 0;
				unit->defenseStage = 0;
				unit->agilityStage = 0;
				gmUnitCalcStub(member);
				attackDelta -= unit->attack;
				defenseDelta -= unit->defense;
				agilityDelta -= unit->agility;
				syWordsCopy(unit, savedUnit, sizeof(*unit));
				syHeapRelease(savedUnit);

				switch(statKind) {
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
				mnMessageCopyExpanded(statKind + MsgOwnerStatusConditionHelp,
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

			mnTextDrawWideStringAtOffset(helpText, (WINDOW *)helpWin, 0, 4);
			syHeapRelease(helpText);
			redraw = 0;
			state->dirty = 1;
		}

		/* The status icons, 15 pixels apart from x 112 on the first row;
		   OWNER_CONDITION_NORMAL and empty places are not drawn. */
		statSpr = statSprs;
		statSlot = statSlots;
		{
			VRAM_CACHE_ENTRY *entry;

			for(i = 0, entry = VramCacheEntries, iconX = 112;
				i < OWNER_STATUS_ICON_PLACES; i++) {
				MENU_SPR_INIT(*statSpr,
					OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				statSpr->oam.f.tile = entry[*statSlot++].ofs >> 5;
				statSpr->oam.f.x = iconX;
				statSpr->oam.f.y = (Uint8)(statWin->y * 8 + 8);
				if(statKinds[i] > OWNER_CONDITION_NORMAL) {
					syOAMAddSprAtDepth(statSpr, OAM_DEPTH_WINDOW);
				}
				iconX += 15;
				statSpr++;
			}
		}

		if(BattleMenuStateWork->tmr == 0) {
			break;
		}
		if((PerPush & KEY_B) != 0) {
			break;
		}

		if(listCnt != 0) {
			if((keys & KEY_R) != 0) {
				memberIdx++;
				if(memberIdx >= listCnt) {
					memberIdx = 0;
				}
				member = list[memberIdx];
				redraw = OWNER_STATUS_REDRAW_ALL;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			} else if((keys & KEY_L) != 0) {
				memberIdx--;
				if(memberIdx < 0) {
					memberIdx = listCnt - 1;
				}
				member = list[memberIdx];
				redraw = OWNER_STATUS_REDRAW_ALL;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		sySynch(1);
	}

	{
		Sint32 *statSlotCursor = statSlots;

		for(i = 0; i < OWNER_STATUS_ICON_PLACES; i++) {
			syVRAMCacheFreeEntry(*statSlotCursor++);
		}
	}
	syVRAMCacheFreeEntry(portraitSlot);
	syVRAMCacheFreeEntry(cursorSlot);
	sySynch(1);
#if BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE
	mnWindowMarkVisibleTextTiles();
#endif
	mnWindowClose(statWin, 1);
	mnWindowClose(helpWin, 1);
	mnPartyStatDraw(BattleStateWork->partyStat);
	BattleMenuStateWork->tmrHidden = 0;
	sySynch(1);
	return 0;
}
