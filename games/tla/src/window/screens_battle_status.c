/* The Status screen. Part of screens.c. */

#include "battle/motobj.h"

/* Room for the help line of an item and of a Djinni, as measured: the item line
   grows from German on, the Djinni line from French on. */
#define SUMMARY_ITEM_HELP_CAPACITY	WHEN(LONG_STATUS_ITEM_HELP, 52, 32)
#define SUMMARY_DJINN_HELP_CAPACITY WHEN(LONG_STATUS_DJINN_HELP, 52, 32)
/* Room for a help line no edition lengthens: no items, an item that cannot be
   used, no Djinn, a Djinni ready to summon. */
#define SUMMARY_HELP_CAPACITY		32

/* Japanese text is shorter: its help lines and Psynergy descriptions take
   smaller buffers, and its stat numbers sit one tile further left. It decodes
   the item, Psynergy and Djinn lines raw where the other languages copy them
   expanded (BATTLE_MENU_MESSAGE_COPY); the lines that take a number are copied
   expanded in every language. */
#define SUMMARY_HELP_TEXT_LENGTH JAPANESE_OR(64, 128)
#define SUMMARY_STAT_VALUE_X	 JAPANESE_OR(176, 184)

/* The condition icons the Status screen shows at most: Normal, or each
   condition the unit has, in a row. */
#define STATUS_ICON_COUNT 11

/* The Status screen's labels, in their message order up to MsgExp
   (OWNER_STATUS_LABEL). */
typedef enum BATTLE_SUMMARY_LABEL {
	OWNER_STATUS_LABEL_ATTACK = 0,
	OWNER_STATUS_LABEL_DEFENSE = 1,
	OWNER_STATUS_LABEL_AGILITY = 2,
	OWNER_STATUS_LABEL_LUCK = 3,
	OWNER_STATUS_LABEL_ATTRIBUTES = 4,	  /* "Attrib.", which nothing draws */
	OWNER_STATUS_LABEL_ELEMENT_LEVEL = 5, /* "Lv" */
	OWNER_STATUS_LABEL_POWER = 6,
	OWNER_STATUS_LABEL_RESIST = 7,
	OWNER_STATUS_LABEL_CLASS = 8, /* "Class", which nothing draws */
	OWNER_STATUS_LABEL_DJINN = 9,
	OWNER_STATUS_LABEL_EXP = 10 /* MsgExp */
} BATTLE_SUMMARY_LABEL;

/* The message of label row (enum above), counted back from MsgExp. */
#define OWNER_STATUS_LABEL(row) \
	((Sint32)MsgExp - OWNER_STATUS_LABEL_EXP + (row))

/* The Status screen's help lines, from MsgOwnerStatusEntryHelp, which the
   cursor stops in status_tables.c name; the status icons' places and the three
   list pages follow. */
typedef enum BATTLE_SUMMARY_HELP {
	/* The name, whose help says how to switch party members with L and R. */
	OWNER_STATUS_HELP_NAME = 0,
	OWNER_STATUS_HELP_LEVEL = 1, /* no cursor stop shows it */
	OWNER_STATUS_HELP_EXPERIENCE = 2,
	OWNER_STATUS_HELP_HP = 3,
	OWNER_STATUS_HELP_PP = 4,
	OWNER_STATUS_HELP_CLASS = 5,
	OWNER_STATUS_HELP_DJINN = 6,
	OWNER_STATUS_HELP_ELEMENT_LEVEL = 7,
	OWNER_STATUS_HELP_POWER = 8,
	OWNER_STATUS_HELP_RESIST = 9,
	OWNER_STATUS_HELP_ATTACK = 10,
	OWNER_STATUS_HELP_DEFENSE = 11,
	OWNER_STATUS_HELP_AGILITY = 12,
	OWNER_STATUS_HELP_LUCK = 13,
	/* The first status icon's place: its help is its status's (MsgNormalStatus
	   + condition). */
	OWNER_STATUS_HELP_CONDITION = 14,
	OWNER_STATUS_HELP_LAST_CONDITION = 21, /* the eighth icon's place */
	OWNER_STATUS_HELP_PSYNERGY_LIST = 22,  /* STATUS_PAGE_PSYNERGY's rows */
	OWNER_STATUS_HELP_DJINN_LIST = 23,	   /* STATUS_PAGE_DJINN's rows */
	OWNER_STATUS_HELP_ITEM_LIST = 24	   /* STATUS_PAGE_ITEMS's rows */
} BATTLE_SUMMARY_HELP;

/* The Status screen's pages, which A turns. */
typedef enum BATTLE_SUMMARY_PAGE {
	STATUS_PAGE_STATS = 0,
	STATUS_PAGE_PSYNERGY = 1,
	STATUS_PAGE_DJINN = 2,
	STATUS_PAGE_ITEMS = 3,
	STATUS_PAGE_COUNT = 4
} BATTLE_SUMMARY_PAGE;

/* The Status screen's cursor groups, each with its table of cursor stops. The
   name, experience, HP, PP, class, Djinn and element rows
   (OwnerStatusStatCursors). */
#define STATUS_CURSOR_GENERAL	   0
/* Attack, Defense, Agility and Luck (OwnerStatusEquipmentCursors). */
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS   2 /* the status icons */
#define STATUS_CURSOR_PSYNERGY	   3 /* the Psynergy page's rows */
#define STATUS_CURSOR_DJINN		   4 /* the Djinn page's grid */
#define STATUS_CURSOR_ITEMS		   5 /* the items page's rows */

/* The general group's stops (OwnerStatusStatCursors): nine for a member with
   Djinn, then the seven of one without. */
#define STATUS_STAT_STOPS				9
#define STATUS_STAT_STOPS_WITHOUT_DJINN 7

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
extern const Uint8 OwnerStatusNivString[];
extern const Uint8 OwnerStatusNvString[];
extern const Uint8 OwnerStatusKpString[];
extern const Uint8 OwnerStatusPvString[];
#endif

void   btObjectSpawnObjectsForListStub(Uint16 *list, Sint32 cnt, Sint32 mode);
Sint32 gmUnitGetExperienceForLevelStub(Sint32 member, Sint32 level);

/* The four list-page scroll marks, one tile per page of four rows (Psynergy and
   items). */
#define SUMMARY_DRAW_PAGE_MARKS(window, count, rowBase) \
	{ \
		for(j = 0; j < ((count) + 3) / 4; j++) { \
			Sint32 tile = PAGE_NUMBER_TILE + j; \
\
			if(j == (rowBase) / 4) \
				tile = PAGE_NUMBER_LIT_TILE + j; \
			mnWindowSetTilemapEntry(window, tile, \
				(window)->width - ((count) + 3) / 4 + j - 10, 7, 0); \
		} \
		mnWindowSetTilemapEntry(window, STATUS_TILE_PAGES_LEFT, \
			(window)->width - ((count) + 3) / 4 - 11, 7, 0); \
		mnWindowSetTilemapEntry( \
			window, STATUS_TILE_PAGES_RIGHT, (window)->width - 10, 7, 0); \
	}

/* Runs the Status screen for party member `member` until B is pressed or the
   link battle's time runs out: the party member's level, experience, HP and PP,
   class, stats, Djinn, elements and statuses, with a help line for the item
   under the cursor. With `listCnt` nonzero, L and R step through the party
   members of `list` (BATTLE_UNIT_LIST_END ends it, BATTLE_UNIT_REMOVED is an
   empty place). The link timer's window is closed while it is open
   (BATTLE_MENU_STATE.tmrHidden), and the party status window is redrawn on the
   way out. Returns 0. mnBattleCollectPartyCommands opens it from the Status
   command and from L or R in a party member's command row. */
Sint32 mnBattleStatScreenRun(Uint16 *list, Sint32 listCnt, Sint32 member)
{
	SUMMARY_OAM		   portraitSpr;
	SUMMARY_OAM		   cursorSpr;
	Sint32			   statSlots[STATUS_ICON_COUNT];
	Sint8			   statKinds[STATUS_ICON_COUNT];
	SUMMARY_OAM		   statSprs[STATUS_ICON_COUNT];
	SUMMARY_OAM		   iconSprs[4];
	SUMMARY_OAM		   nameSpr;
	STAT_CURSOR		   cursor;
	Sint32			   iconSlots[4];
	Uint16			   figureList[2];
	Uint16			   spawnList[2];
	Uint16			   itemText[64];
	Uint16			   psynergyText[64];
	WIN_STATE		  *state;
	WINDOW			  *statWin;
	Sint32			   redraw;
	WINDOW			  *helpWin;
	Sint32			   memberIdx;
	Sint32			   cursorSlot;
	SUMMARY_POINT	   cursorPos;
	Sint32			   portraitSlot;
	Sint32			   statCnt;
	Sint32			   hasDjinn;
	Sint32			   shownMember;
	Sint32			   page;
	Bool			   figureShown;
	Uint32			  *djinnList;
	Sint32			   djinnCnt;
	Sint32			   psynergyCnt;
	Sint32			   itemCnt;
	Sint32			   keys;
	UNIT			  *unit;
	Sint32			   i;
	Sint32			   j;
	BATTLE_MENU_STATE *navigation;
	Sint32			   rowBase;
	Sint32			   row;
	Sint32			   groupPos;
	SUMMARY_OAM		  *spr;

	state = (WIN_STATE *)Ram_HeapSlots->winState;
	redraw = 1;
	memberIdx = -1;
	statCnt = 0;
	hasDjinn = gmPartySumDjinnCountsStub(-1);
	page = 0;
	figureShown = FALSE;
	djinnList = syHeapAllocEWRAM(84 * sizeof(*djinnList));
	itemCnt = psynergyCnt = djinnCnt = 0;
	shownMember = figureList[0] = list[0];
	figureList[1] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(figureList, 1);
	portraitSlot = syVRAMCacheAllocEntry(512);
	navigation = BattleMenuStateWork;
	navigation->tmrHidden = redraw;
	if(navigation->tmrWin != NULL) {
		mnWindowClose(navigation->tmrWin, 1);
		navigation->tmrWin = (void *)page;
	}
	sySndPlayStub(SE_MENU_CONFIRM);

	for(i = 4; i >= 0; i--)
		cursor.groupPositions[i] = 0;
	cursor.helpEntry = 0;
	cursor.drawnHelpEntry = 0;
	cursor.group = STATUS_CURSOR_GENERAL;
	cursorSlot = syVRAMCacheAllocEntry(128);
	for(i = 0; i < STATUS_ICON_COUNT; i++) {
		statSlots[i] = syVRAMCacheAllocEntry(128);
		statKinds[i] = OWNER_CONDITION_EMPTY;
	}
	for(i = 0; i <= 3; i++) {
		iconSlots[i] = syVRAMCacheAllocEntry(128);
		iconSprs[i].oam.f.tile =
			syVRAMCacheReload(iconSlots[i], (const void *)-1);
	}
	if(listCnt != 0) {
		for(i = 0; i <= 5; i++) {
			Sint32 kind = list[i];

			if(kind == BATTLE_UNIT_LIST_END)
				break;
			if(kind != BATTLE_UNIT_REMOVED && kind == member) {
				memberIdx = i;
				break;
			}
		}
	}
	cursorPos.x = 0;
	cursorPos.y = 0;
	statWin =
		mnWindowCreate(0, 0, 30, 20, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	helpWin = mnWindowCreate(
		0, 14, 30, 6, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
	mnWindowMarkVisibleTextTiles();

	for(;;) {
		keys = PerWork.repeat;
		spr = &nameSpr;
		mnBattleStatDrawPageArrows(STATUS_PAGE_ARROW_VRAM);
		if(redraw != 0)
			mnPortraitLoad(member, portraitSlot);
		MENU_SPR_INIT(
			*spr, OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
		spr->oam.f.tile = VramCacheEntries[portraitSlot].ofs >> 5;
		spr->oam.f.x = 8;
		spr->oam.f.y = 24;
		spr->oam.f.pal = PORTRAIT_PALETTE;
		syOAMAddSprAtDepth(spr, OAM_DEPTH_WINDOW);

		switch(cursor.helpEntry) {
		case OWNER_STATUS_HELP_NAME ... OWNER_STATUS_HELP_RESIST:
		default:
			cursor.group = STATUS_CURSOR_GENERAL;
			if(hasDjinn != 0)
				cursor.groupSize = STATUS_STAT_STOPS;
			else
				cursor.groupSize = STATUS_STAT_STOPS_WITHOUT_DJINN;
			break;
		case OWNER_STATUS_HELP_ATTACK ... OWNER_STATUS_HELP_LUCK:
			cursor.group = STATUS_CURSOR_COMBAT_STATS;
			cursor.groupSize = 4;
			break;
		case OWNER_STATUS_HELP_CONDITION ... OWNER_STATUS_HELP_LAST_CONDITION:
			cursor.group = STATUS_CURSOR_CONDITIONS;
			cursor.groupSize = statCnt;
			break;
		case OWNER_STATUS_HELP_PSYNERGY_LIST:
			cursor.group = STATUS_CURSOR_PSYNERGY;
			cursor.groupSize = psynergyCnt;
			break;
		case OWNER_STATUS_HELP_DJINN_LIST:
			cursor.group = STATUS_CURSOR_DJINN;
			cursor.groupSize = djinnCnt;
			break;
		case OWNER_STATUS_HELP_ITEM_LIST:
			cursor.group = STATUS_CURSOR_ITEMS;
			cursor.groupSize = itemCnt;
			break;
		}
		mnWindowHighlightRect(statWin->x, statWin->y, statWin->width,
			statWin->height, WINDOW_PALETTE);

		/* A turns the page; each list page starts its own group at the top. */
		if(PerWork.push & KEY_A) {
			page = (page + 1) % STATUS_PAGE_COUNT;
			/* page + 3 is the list group of the page after the new one, so a
			   list goes back to its top on the turn to the page before it; A
			   turns only forward, so each list still opens at its top. The
			   switch above sets the group again from the help entry. */
			cursor.group = page + 3;
			cursor.groupPositions[page + 3] = 0;
			cursor.groupSize = 1;
			if(page != STATUS_PAGE_STATS)
				cursor.helpEntry = page + OWNER_STATUS_HELP_LAST_CONDITION;
			else
				cursor.helpEntry = page;
			redraw = 1;
			sySndPlayStub(SE_MENU_CONFIRM);
		}

		if(statCnt != 0) {
			if((Uint32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
				groupPos = cursor.groupPositions[cursor.group];
				if(keys & KEY_DOWN) {
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
				} else if(keys & KEY_UP) {
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
				} else if(keys & KEYS_HORIZONTAL) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					cursor.group ^= STATUS_CURSOR_CONDITIONS;
					if(PerWork.repeat & KEY_LEFT)
						groupPos = statCnt - 1;
					else
						groupPos = 0;
				}
				cursor.groupPositions[cursor.group] = groupPos;
			} else if(cursor.group == STATUS_CURSOR_CONDITIONS) {
				groupPos = cursor.groupPositions[cursor.group];
				if(groupPos >= cursor.groupSize)
					groupPos = cursor.groupSize - 1;
				if(groupPos < 0) {
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPos = cursor.groupPositions[STATUS_CURSOR_GENERAL];
				} else if(keys & KEY_RIGHT) {
					keys = 0;
					groupPos++;
					if(groupPos >= cursor.groupSize) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPos = cursor.groupPositions[STATUS_CURSOR_GENERAL];
					}
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				} else if(keys & KEY_LEFT) {
					keys = 0;
					groupPos--;
					if(groupPos < 0) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPos = cursor.groupPositions[STATUS_CURSOR_GENERAL];
					}
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				} else if(keys & KEYS_VERTICAL) {
					keys = 0;
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPos = cursor.groupPositions[STATUS_CURSOR_GENERAL];
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				}
				cursor.groupPositions[cursor.group] = groupPos;
			} else if(cursor.group == STATUS_CURSOR_PSYNERGY ||
				cursor.group == STATUS_CURSOR_ITEMS) {
				/* Four rows a page: up and down step, left and right turn the
				   page. */
				groupPos = cursor.groupPositions[cursor.group];
				if(keys & KEY_DOWN) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPos++;
					if(groupPos >= cursor.groupSize)
						groupPos = 0;
				} else if(keys & KEY_UP) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPos--;
					if(groupPos < 0)
						groupPos = cursor.groupSize - 1;
				} else if(keys & KEY_RIGHT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPos += 4;
					if(groupPos >= cursor.groupSize) {
						if(groupPos / 4 - 1 == (cursor.groupSize - 1) / 4)
							groupPos -= groupPos / 4 * 4;
						else
							groupPos = cursor.groupSize - 1;
					}
				} else if(keys & KEY_LEFT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPos -= 4;
					if(groupPos < 0)
						groupPos =
							cursor.groupSize / 4 * 4 + (groupPos + 4) % 4;
				}
				if(groupPos >= cursor.groupSize)
					groupPos = cursor.groupSize - 1;
				if(groupPos < 0)
					groupPos = 0;
				cursor.groupPositions[cursor.group] = groupPos;
			} else if(cursor.group == STATUS_CURSOR_DJINN) {
				/* Three Djinn a column. */
				Sint32 djinnPos = cursor.groupPositions[cursor.group];

				if(keys & KEY_DOWN) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPos++;
					if(djinnPos >= cursor.groupSize)
						djinnPos = 0;
				} else if(keys & KEY_UP) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPos--;
					if(djinnPos < 0)
						djinnPos = cursor.groupSize - 1;
				} else if(keys & KEY_RIGHT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPos += 3;
					if(djinnPos >= cursor.groupSize)
						djinnPos = djinnPos % 3;
				} else if(keys & KEY_LEFT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPos -= 3;
					if(djinnPos < 0)
						djinnPos =
							cursor.groupSize / 3 * 3 + (djinnPos + 3) % 3;
				}
				if(djinnPos >= cursor.groupSize)
					djinnPos = cursor.groupSize - 1;
				if(djinnPos < 0)
					djinnPos = 0;
				cursor.groupPositions[cursor.group] = djinnPos;
			}
		}

		if(cursor.group == STATUS_CURSOR_GENERAL) {
			const BATTLE_SUMMARY_STAT_CURSOR *stop;
			Sint32 k = cursor.groupPositions[cursor.group];

			if(hasDjinn == 0)
				k += STATUS_STAT_STOPS;
			stop = &OwnerStatusStatCursors[k];
			cursor.helpEntry = stop->entry;
			cursor.column = stop->column;
			cursor.row = stop->row;
			mnWindowHighlightRect(statWin->x + stop->highlightColumn + 1,
				statWin->y + stop->highlightRow + 1, stop->highlightWidth, 1,
				HIGHLIGHT_PALETTE);
		} else if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
			cursor.helpEntry =
				OwnerStatusEquipmentCursors[cursor.groupPositions[cursor.group]]
					.entry;
			cursor.column =
				OwnerStatusEquipmentCursors[cursor.groupPositions[cursor.group]]
					.column;
			cursor.row =
				OwnerStatusEquipmentCursors[cursor.groupPositions[cursor.group]]
					.row;
		} else if(cursor.group == STATUS_CURSOR_CONDITIONS) {
			cursor.helpEntry =
				OwnerStatusConditionCursors[cursor.groupPositions[cursor.group]]
					.entry;
			cursor.column =
				OwnerStatusConditionCursors[cursor.groupPositions[cursor.group]]
					.column;
			cursor.row =
				OwnerStatusConditionCursors[cursor.groupPositions[cursor.group]]
					.row;
			if(statKinds[0] == OWNER_CONDITION_NORMAL)
				mnWindowHighlightRect(
					statWin->x + 15, statWin->y + 1, 10, 1, HIGHLIGHT_PALETTE);
			else
				mnWindowHighlightRect(statWin->x + cursor.column + 1,
					statWin->y + cursor.row + 1, 2, 2, HIGHLIGHT_PALETTE);
		} else if(cursor.group == STATUS_CURSOR_PSYNERGY ||
			cursor.group == STATUS_CURSOR_ITEMS) {
			Sint8 pos = cursor.groupPositions[cursor.group];

			cursor.row = (Sint8)(pos % 4) * 2 + 8;
			if(cursor.group == STATUS_CURSOR_PSYNERGY)
				cursor.column = -2;
			else
				cursor.column = -1;
			mnWindowHighlightRect(statWin->x + 1, statWin->y + cursor.row + 1,
				20, 1, HIGHLIGHT_PALETTE);
		} else {
			Sint32 pos = cursor.groupPositions[STATUS_CURSOR_DJINN];

			cursor.row = (Sint8)(pos % 3) * 2 + 9;
			cursor.column = (Sint8)(pos / 3) * 7 - 2;
			mnWindowHighlightRect(statWin->x + cursor.column + 3,
				statWin->y + cursor.row + 1, 7, 1, HIGHLIGHT_PALETTE);
		}
		if(cursor.drawnHelpEntry != cursor.helpEntry) {
			cursor.drawnHelpEntry = cursor.helpEntry;
			redraw |= 2;
		}
		cursorPos.x = cursor.column * 8;
		cursorPos.y = cursor.row * 8;

		/* The member's battle figure, respawned when L or R changes the
		   member. */
		if(shownMember != member) {
			spawnList[0] = member;
			spawnList[1] = BATTLE_UNIT_LIST_END;
			btObjectDestroySprsStub(shownMember);
			shownMember = member;
			btObjectSpawnObjectsForListStub(spawnList, 1, 0);
			figureShown = FALSE;
		}
		{
			SUMMARY_OAM *face = btObjectGetStub(member)->obj->records;

			MENU_SPR_INIT(portraitSpr,
				OBJ_SIZE_64X64 | OBJ_256_COLORS |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
			if(face != NULL) {
				portraitSpr.oam.f.tile = face->oam.f.tile;
				portraitSpr.oam.f.x = 172;
				portraitSpr.oam.f.y = 56;
				if(figureShown != 0)
					syOAMAddSprAtDepth(&portraitSpr, OAM_DEPTH_WINDOW);
				figureShown = TRUE;
			}
		}

		MENU_SPR_INIT(
			cursorSpr, OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
		cursorSpr.oam.f.tile = syVRAMCacheReload(cursorSlot, PointingHandTiles);
		{
			Sint32 bob = TickCount & 4;
			Sint32 x = statWin->x * 8 + cursorPos.x + 16;

			cursorSpr.oam.f.x =
				page == STATUS_PAGE_STATS ? x - bob / 4 : x + bob / 4;
		}
		cursorSpr.oam.f.y =
			cursorPos.y + statWin->y * 8 - (Sint32)(TickCount & 4) / 4 + 16;
		if(page != STATUS_PAGE_STATS)
			cursorSpr.oam.f.affineIdx = 0;
		else
			cursorSpr.oam.f.affineIdx = 8;
		syOAMAddSprAtDepth(&cursorSpr, OAM_DEPTH_WINDOW + 1);

		if(redraw != 0) {

			unit = syUnitGet(member);
			if(page == STATUS_PAGE_STATS)
				mnTextFillCanvasBackground();

			if(page == STATUS_PAGE_ITEMS) {
				/* Items: those the member can use in battle first. */
				Uint16 *entries = BattleMenuStateWork->listAbilities;
				Sint32	cnt;
				Sint32	idx;
				Sint32	item;
				Sint32	row;

				cnt = 0;
				j = -1;
				for(idx = 0; idx != INVENTORY_SLOTS &&
					(item = unit->inventory[idx]) != 0;
					idx++) {
					gmItemGetDirectStub(item);
					if(mnItemCheckBattleUse(member, item) == 0)
						entries[cnt++] = item;
				}
				for(idx = 0; idx != INVENTORY_SLOTS &&
					(item = unit->inventory[idx]) != 0;
					idx++) {
					gmItemGetDirectStub(item);
					if(mnItemCheckBattleUse(member, item) != 0)
						entries[cnt++] = item;
				}
				entries[cnt] = 0;
				itemCnt = cnt;
				if(cnt <= cursor.groupPositions[5])
					cursor.groupPositions[5] = cnt - 1;
				rowBase = cursor.groupPositions[5] / 4 * 4;
				row = (Sint8)(cursor.groupPositions[5] % 4);
				if(cnt != 0) {
					Uint16 *entry = entries + rowBase + row;

					if(mnItemCheckBattleUse(member, *entry) == 2)
						BATTLE_MENU_MESSAGE_COPY(
							(Sint32)MsgBattleItemUnavailable, itemText,
							SUMMARY_HELP_CAPACITY);
					else
						BATTLE_MENU_MESSAGE_COPY(
							(*entry & INVENTORY_ITEM_ID_MASK) +
								(Sint32)MsgItemDescription,
							itemText, SUMMARY_ITEM_HELP_CAPACITY);
				} else {
					BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleNoItems, itemText,
						SUMMARY_HELP_CAPACITY);
				}
				mnWindowSprRedrawSavedRect(statWin);
				mnWindowMapTextCanvasTiles(
					helpWin->x, helpWin->y, helpWin->width, helpWin->height, 0);
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(itemText, helpWin, 0, 20);
				if(rowBase != j) {
					mnWindowDrawDividerLine(statWin, 0, 16, 29, 16);
					for(j = 0; j <= 3 && (item = entries[rowBase + j]) != 0;
						j++) {
						gmItemGetDirectStub(item);
						mnTextSetCol(TEXT_COLOR_WHITE);
						if(mnItemCheckBattleUse(member, item) != 0)
							mnTextSetCol(TEXT_COLOR_YELLOW);
						else if(item & INVENTORY_BROKEN)
							mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							(item & INVENTORY_ITEM_ID_MASK) +
								(Sint32)MsgItemName,
							statWin, 32, 64 + j * 16);
						mnTextSetCol(TEXT_COLOR_WHITE);
						iconSprs[j].oam.f.tile =
							mnItemIconLoadIntoSlot(item, iconSlots[j]);
					}
					/* FAKEMATCH(dead): a store nothing reads, which keeps the
					 * list loop's exit on the fill loop's test. */
					item = 0;
					for(; j <= 3; j++)
						iconSprs[j].oam.f.tile =
							syVRAMCacheReload(iconSlots[j], (const void *)-1);
				}
				if(cnt > 4)
					SUMMARY_DRAW_PAGE_MARKS(statWin, cnt, rowBase);
			} else if(page == STATUS_PAGE_PSYNERGY) {
				/* Battle Psynergy: grey when it cannot be used in battle, red
				   when the PP are short, another colour while Psynergy is
				   sealed. */
				Uint16 *entries = BattleMenuStateWork->listAbilities;
				Sint32	cnt;
				Sint32	shownRow;
				Sint32	idx;
				Sint32	id;

				cnt = 0;
				j = -1;
				mnWindowSprRedrawSavedRect(statWin);
				shownRow = j;
				for(idx = 0; idx != PSYNERGY_SLOTS &&
					(id = unit->psynergy[idx].encodedAction &
							PSYNERGY_ABILITY_MASK) != 0;
					idx++) {
					if(gmAbilityGetDirectStub(id)->flg &
						ABILITY_USABLE_IN_BATTLE)
						entries[cnt++] = id;
				}
				entries[cnt] = 0;
				psynergyCnt = cnt;
				if(cnt <= cursor.groupPositions[3])
					cursor.groupPositions[3] = cnt - 1;
				rowBase = cursor.groupPositions[3] / 4 * 4;
				row = (Sint8)(cursor.groupPositions[3] % 4);
				if(rowBase != j || row != shownRow) {
					state->holdTilemapUpload = 1;
					mnTextFillCanvasBackground();
					if(cnt != 0)
						BATTLE_MENU_MESSAGE_COPY(
							(entries[rowBase + row] & PSYNERGY_ABILITY_MASK) +
								(Sint32)MsgAbilityDescription,
							psynergyText, BATTLE_MENU_MESSAGE_CAPACITY);
					else
						BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleNoPsynergy,
							psynergyText, BATTLE_MENU_MESSAGE_CAPACITY);
					mnWindowSprRedrawSavedRect(statWin);
					mnWindowMapTextCanvasTiles(helpWin->x, helpWin->y,
						helpWin->width, helpWin->height, 0);
					mnTextDrawWideStringAtOffset(psynergyText, helpWin, 0, 20);
					if(rowBase != j) {
						mnWindowDrawDividerLine(statWin, 0, 16, 29, 16);
						for(j = 0; j <= 3 && (id = entries[rowBase + j]) != 0;
							j++) {
							ABILITY_DEFINITION *ability =
								gmAbilityGetDirectStub(id);
							Sint32 range;
							Sint32 tileFlg;

							mnWindowSetTilemapEntry(statWin,
								WINDOW_TILE_PP_LABEL, 11, j * 2 + 8, 0);
							mnWindowSetTilemapEntry(statWin,
								WINDOW_TILE_PP_LABEL_END, 12, j * 2 + 8, 0);
							iconSprs[j].oam.f.tile =
								mnAbilityIconLoadIntoSlot(id, iconSlots[j]);
							if(!(ability->flg & ABILITY_USABLE_IN_BATTLE))
								mnTextSetCol(TEXT_COLOR_YELLOW);
							else if(ability->ppCost > unit->pp)
								mnTextSetCol(TEXT_COLOR_RED);
							else if(unit->psySeal != 0)
								mnTextSetCol(TEXT_COLOR_PINK);
							tileFlg = 0;
							state->textPal = 5;
							mnTextDrawMessageAsTiles(
								id + (Sint32)MsgAbilityName, statWin, 16,
								64 + j * 16);
							mnTextDrawNumAtOffset(
								ability->ppCost, 2, statWin, 104, 64 + j * 16);
							mnTextSetCol(TEXT_COLOR_WHITE);
							state->textPal = WINDOW_PALETTE;
							if(ability->element != ELEMENT_NONE)
								mnWindowSetTilemapEntry(statWin,
									ELEMENT_ICON_TILE + ability->element, 15,
									j * 2 + 8, tileFlg);
							range = ability->range;
							if(range == ABILITY_RANGE_ALL)
								range = RANGE_ICON_ALL;
							else
								range--;
							mnRangeIconDraw(statWin, 16, j * 2 + 8, range, 0);
						}
						/* FAKEMATCH(dead): a store nothing reads, which keeps
						 * the list loop's exit on the fill loop's test. */
						id = 0;
						for(; j <= 3; j++)
							iconSprs[j].oam.f.tile = syVRAMCacheReload(
								iconSlots[j], (const void *)-1);
					}
					if(cnt > 4)
						SUMMARY_DRAW_PAGE_MARKS(statWin, cnt, rowBase);
					state->dirty = 1;
					state->holdTilemapUpload = 0;
				}
			} else if(page == STATUS_PAGE_DJINN) {
				/* Djinn: each element's set ones, then those on standby or
				   recovering (BATTLE_DJINN_UNSET, the turns left in
				   BATTLE_DJINN_TURNS). */
				Sint32 rowBase = 0;
				Sint32 shownBase = -1;
				Sint32 released = 0;
				Sint32 element;
				Sint32 bit;
				Uint32 entry;

				mnWindowSprRedrawSavedRect(statWin);
				mnWindowMapTextCanvasTiles(
					helpWin->x, helpWin->y, helpWin->width, helpWin->height, 0);
				mnWindowDrawDividerLine(statWin, 0, 16, 29, 16);
				djinnCnt = 0;
				for(element = 0; element < ELEMENT_COUNT; element++) {
					for(bit = 0; bit < DJINN_PER_ELEMENT; bit++) {
						if(unit->djinnSet[element] & (1 << bit)) {
							djinnList[djinnCnt] =
								(element << BATTLE_DJINN_ELEMENT_SHIFT) | bit;
							djinnCnt++;
						} else if(unit->djinnHeld[element] & (1 << bit)) {
							DJINN_RECOVERY_LIST *recovery =
								&((DJINN_RECOVERY_TABLE *)
										gmDjinnGetRecoveryTableStub(
											(Uint32)member > ALLY_MAX))
									 ->list;
							Sint32 k;
							Sint32 value;

							for(k = 0; k < recovery->cnt &&
								!(recovery->entries[k].unitId == member &&
									recovery->entries[k].element == element &&
									recovery->entries[k].idx == bit);
								k++)
								;
							value = (element << BATTLE_DJINN_ELEMENT_SHIFT) |
								bit | BATTLE_DJINN_UNSET;
							djinnList[djinnCnt] = value;
							if(recovery->entries[k].turns > 0)
								djinnList[djinnCnt] = value |
									(recovery->entries[k].turns
										<< BATTLE_DJINN_TURNS_SHIFT);
							djinnCnt++;
						}
					}
				}
				djinnList[djinnCnt] = BATTLE_DJINN_END;
				if(cursor.groupPositions[STATUS_CURSOR_DJINN] >= djinnCnt)
					cursor.groupPositions[STATUS_CURSOR_DJINN] = djinnCnt - 1;
				entry = djinnList[cursor.groupPositions[STATUS_CURSOR_DJINN]];
				state->holdTilemapUpload = 1;
				if(djinnCnt != 0) {
					mnMessageArgumentClearAll();
					if(entry & BATTLE_DJINN_UNSET) {
						if(entry & BATTLE_DJINN_TURNS) {
							mnMessageArgumentPush(
								(entry & BATTLE_DJINN_TURNS) >>
									BATTLE_DJINN_TURNS_SHIFT,
								MESSAGE_ARGUMENT_NUMBER);
							BATTLE_MENU_MESSAGE_COPY(
								BATTLE_DJINNI(entry) + (Sint32)MsgDjinnEffect,
								psynergyText, SUMMARY_DJINN_HELP_CAPACITY);
						} else {
							mnMessageCopyExpanded((Sint32)MsgReadyToSummon,
								psynergyText, SUMMARY_HELP_CAPACITY);
						}
					} else {
						BATTLE_MENU_MESSAGE_COPY(
							BATTLE_DJINNI(entry) + (Sint32)MsgDjinnEffect,
							psynergyText, SUMMARY_DJINN_HELP_CAPACITY);
					}
				} else {
					BATTLE_MENU_MESSAGE_COPY((Sint32)MsgNoDjinn, psynergyText,
						SUMMARY_HELP_CAPACITY);
				}
				state->holdTilemapUpload = released;
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(psynergyText, helpWin, 0, 20);
				if(rowBase != shownBase) {
					Uint32 shown;
					Sint32 column;
					Sint32 tileRow;

					for(j = 0; j <= 8; j++) {
						/* rowBase is this page's 0, as pages 1 and 3 index
						   their lists. */
						shown = djinnList[rowBase + j];
						column = j / 3 * 7;
						tileRow = j % 3 * 2 + 9;
						if(shown == BATTLE_DJINN_END)
							break;
						mnWindowSetTilemapEntry(statWin,
							BATTLE_DJINN_ELEMENT_OF(shown) + ELEMENT_ICON_TILE,
							column, tileRow, 0);
						if(shown & BATTLE_DJINN_TURNS)
							mnTextSetCol(TEXT_COLOR_YELLOW);
						else if(shown & BATTLE_DJINN_UNSET)
							mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							BATTLE_DJINNI(shown) + (Sint32)MsgDjinnName,
							statWin, column * 8 + 8, tileRow * 8);
/* Spanish and Italian leave a recovering Djinni's turns off the list. */
#if !STATUS_DJINN_TURNS_HIDDEN
						if(shown & BATTLE_DJINN_TURNS)
							mnTextDrawNumAtOffset(
								(shown & BATTLE_DJINN_TURNS) >>
									BATTLE_DJINN_TURNS_SHIFT,
								2, statWin, column * 8 + 40, tileRow * 8);
#endif
						mnTextSetCol(TEXT_COLOR_WHITE);
					}
				}
				/* LEFTOVER(template): the scroll marks under a count past */
				/* four, as mnBattleStatScreenRun's item and Psynergy pages draw
				   theirs with SUMMARY_DRAW_PAGE_MARKS; this page's count stays
				   0; as no arm. */
				if(released > 4)
					SUMMARY_DRAW_PAGE_MARKS(statWin, released, rowBase);
				state->dirty = 1;
				state->holdTilemapUpload = 0;
			}

			if(redraw != 0) {
				if(page == STATUS_PAGE_STATS) {
					mnWindowSprRedrawSavedRect(statWin);
					mnWindowMapTextCanvasTiles(helpWin->x, helpWin->y,
						helpWin->width, helpWin->height, page);
					mnWindowDrawDividerLine(statWin, 0, 14, 29, 14);
				}
				mnTextDrawStringAtOffset(unit->name, statWin, 0, 0);
				if(memberIdx > 3)
					mnTextDrawMessageAsTiles(
						(Sint32)MsgInTheBack, statWin, 40, 16);
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
				/* The European editions share one image and choose the level's
				   label, drawing the name again under it, at run time. */
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_FRENCH:
					mnTextDrawStringAtOffset(unit->name, statWin, 0, 0);
					mnTextDrawStringAtOffset(
						OwnerStatusNivString, statWin, 51, 0);
					mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
					break;
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(unit->name, statWin, 0, 0);
					mnTextDrawStringAtOffset(
						OwnerStatusNvString, statWin, 56, 0);
					mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
					break;
				default:
					mnTextDrawStringAtOffset(unit->name, statWin, 0, 0);
					mnTextDrawStringAtOffset(
						OwnerStatusLevelString, statWin, 56, 0);
					mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
					break;
				}
#else
				mnTextDrawStringAtOffset(
					OwnerStatusLevelString, statWin, 56, 0);
				mnTextDrawNumInWindow(unit->level, 2, statWin, 72, 0);
#endif
				mnTextDrawMessageAsTiles((Sint32)MsgExp, statWin, 0, 8);
				mnTextDrawNumInWindow(unit->experience, 8, statWin, 40, 8);
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_GERMAN:
					mnTextDrawStringAtOffset(
						OwnerStatusKpString, statWin, 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_FRENCH:
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(
						OwnerStatusPvString, statWin, 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_ITALIAN:
					mnTextDrawStringAtOffset(
						OwnerStatusHpString, statWin, 40, 24);
					break;
				}
#else
				mnTextDrawStringAtOffset(OwnerStatusHpString, statWin, 40, 24);
#endif
				mnTextDrawNumInWindow(unit->hp, 4, statWin, 56, 24);
				mnTextDrawStringAtOffset(
					OwnerStatusSlashString, statWin, 88, 24);
				mnTextDrawNumInWindow(unit->hpMax, 4, statWin, 96, 24);
				mnTextDrawStringAtOffset(OwnerStatusPpString, statWin, 40, 32);
				mnTextDrawNumInWindow(unit->pp, 4, statWin, 56, 32);
				mnTextDrawStringAtOffset(
					OwnerStatusSlashString, statWin, 88, 32);
				mnTextDrawNumInWindow(unit->ppMax, 4, statWin, 96, 32);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ATTACK), statWin, 136,
					16);
				mnTextDrawNumInWindow(
					unit->attack, 3, statWin, SUMMARY_STAT_VALUE_X, 16);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DEFENSE), statWin,
					136, 24);
				mnTextDrawNumInWindow(
					unit->defense, 3, statWin, SUMMARY_STAT_VALUE_X, 24);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_AGILITY), statWin,
					136, 32);
				mnTextDrawNumInWindow(
					unit->agility, 3, statWin, SUMMARY_STAT_VALUE_X, 32);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_LUCK), statWin, 136,
					40);
				mnTextDrawNumInWindow(
					unit->luck, 3, statWin, SUMMARY_STAT_VALUE_X, 40);
				mnTextDrawMessageAsTiles(
					(Sint32)MsgClassName + unit->classId, statWin, 0, 48);
				if(page == STATUS_PAGE_STATS) {
					if(hasDjinn != 0)
						mnTextDrawMessageAsTiles(
							OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DJINN),
							statWin, 0, 72);
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ELEMENT_LEVEL),
						statWin, 0, 80);
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_POWER), statWin,
						0, 88);
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_RESIST), statWin,
						0, 96);
					for(j = 0; j < ELEMENT_COUNT; j++) {
						Sint32 iconRow = 1;

						if(hasDjinn != 0)
							iconRow = 0;
						mnWindowSetTilemapEntry(statWin, ELEMENT_ICON_TILE + j,
							7 + j * 4, iconRow + 8, 0);
						if(hasDjinn != 0) {
							mnTextDrawNumInWindow(unit->djinnSetCnts[j], 1,
								statWin, 40 + j * 32, 72);
							mnTextDrawStringAtOffset(OwnerStatusSlashString,
								statWin, 48 + j * 32, 72);
							mnTextDrawNumInWindow(unit->djinnHeldCnts[j], 1,
								statWin, 48 + j * 32 + 8, 72);
						}
						mnTextDrawNumInWindow(
							gmUnitGetElementLevelStub(member, j), 2, statWin,
							48 + j * 32, 80);
						mnTextDrawNumInWindow(unit->element[j].power, 3,
							statWin, 40 + j * 32, 88);
						mnTextDrawNumInWindow(unit->element[j].resistance, 3,
							statWin, 40 + j * 32, 96);
					}
				}

				/* The status icons, as The Broken Seal orders them. */
				i = 0;
				if(unit->hp == 0) {
					statKinds[0] = OWNER_CONDITION_DOWNED;
					i = 1;
				}
				do {
					if(unit->curse != 0) {
						statKinds[i++] = OWNER_CONDITION_CURSED_ITEM;
					}
					if(i > 7)
						break;
					if(unit->grimReaper != 0) {
						statKinds[i++] = OWNER_CONDITION_GRIM_REAPER;
					}
					if(i > 7)
						break;
					if(unit->poison == 1) {
						statKinds[i++] = OWNER_CONDITION_POISON;
					}
					if(i > 7)
						break;
					if(unit->poison == 2) {
						statKinds[i++] = OWNER_CONDITION_DEADLY_POISON;
					}
					if(i > 7)
						break;
					if(unit->psySeal != 0) {
						statKinds[i++] = OWNER_CONDITION_PSYNERGY_SEALED;
					}
					if(i > 7)
						break;
					if(unit->stun != 0) {
						statKinds[i++] = OWNER_CONDITION_STUN;
					}
					if(i > 7)
						break;
					if(unit->sleep != 0) {
						statKinds[i++] = OWNER_CONDITION_SLEEP;
					}
					if(i > 7)
						break;
					if(unit->haunt != 0) {
						statKinds[i++] = OWNER_CONDITION_HAUNT;
					}
					if(i > 7)
						break;
					if(unit->delusion != 0) {
						statKinds[i++] = OWNER_CONDITION_DELUSION;
					}
					if(i > 7)
						break;
					if(unit->attackTurns != 0) {
						if(unit->attackStage > 0) {
							statKinds[i++] = OWNER_CONDITION_ATTACK_UP;
						}
						if(unit->attackStage < 0) {
							statKinds[i++] = OWNER_CONDITION_ATTACK_DOWN;
						}
					}
					if(i > 7)
						break;
					if(unit->defenseTurns != 0) {
						if(unit->defenseStage > 0) {
							statKinds[i++] = OWNER_CONDITION_DEFENSE_UP;
						}
						if(unit->defenseStage < 0) {
							statKinds[i++] = OWNER_CONDITION_DEFENSE_DOWN;
						}
					}
					if(i > 7)
						break;
					if(unit->resistanceTurns != 0) {
						if(unit->resistanceStage > 0) {
							statKinds[i++] = OWNER_CONDITION_RESISTANCE_UP;
						}
						if(unit->resistanceStage < 0) {
							statKinds[i++] = OWNER_CONDITION_RESISTANCE_DOWN;
						}
					}
					if(i > 7)
						break;
					if(unit->agilityStage > 0) {
						statKinds[i++] = OWNER_CONDITION_AGILITY_UP;
					}
					if(unit->agilityStage < 0) {
						statKinds[i++] = OWNER_CONDITION_AGILITY_DOWN;
					}
				} while(0);
				for(j = 0; j < i; j++)
					mnMiscIconLoadIntoSlot(statKinds[j], statSlots[j]);
				if(i == 0) {
					statKinds[0] = OWNER_CONDITION_NORMAL;
					i = 1;
				}
				if(i < STATUS_ICON_COUNT) {
					for(j = i; j < STATUS_ICON_COUNT; j++)
						statKinds[j] = OWNER_CONDITION_EMPTY;
				}
				statCnt = i;
				if(statKinds[0] == OWNER_CONDITION_NORMAL)
					mnTextDrawMessageAsTiles(unit->hp != 0
							? (Sint32)MsgNormal
							: (Sint32)MsgCantFight,
						statWin, 112, 0);
			}

			if(page == STATUS_PAGE_STATS) {
				Uint16 *helpText =
					syHeapAlloc(SUMMARY_HELP_TEXT_LENGTH * sizeof(*helpText));

				if((Uint32)cursor.helpEntry >= OWNER_STATUS_HELP_CONDITION) {
					Sint32 statKind = statKinds[cursor.helpEntry -
						OWNER_STATUS_HELP_CONDITION];
					UNIT  *savedUnit;
					Sint32 attackDelta;
					Sint32 defenseDelta;
					Sint32 agilityDelta;
					Sint32 helpValue;

					if(statKind == OWNER_CONDITION_NORMAL && unit->hp == 0)
						statKind = OWNER_CONDITION_DOWNED;
					savedUnit = syHeapAlloc(sizeof(*savedUnit));
					syWordsCopyEntry(savedUnit, unit, sizeof(*savedUnit));
					attackDelta = unit->attack;
					defenseDelta = unit->defense;
					helpValue = 0;
					agilityDelta = unit->agility;
					unit->attackStage = 0;
					unit->defenseStage = 0;
					unit->agilityStage = 0;
					gmUnitCalcStub(member);
					attackDelta -= unit->attack;
					defenseDelta -= unit->defense;
					agilityDelta -= unit->agility;
					syWordsCopyEntry(unit, savedUnit, sizeof(*savedUnit));
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
						helpValue =
							unit->resistanceStage * RESISTANCE_PER_STAGE;
						if(helpValue < 0)
							helpValue = -helpValue;
						break;
					}
					mnMessageArgumentPush(helpValue, MESSAGE_ARGUMENT_NUMBER);
					/* FAKEMATCH(dead): an empty once-only block between the two
					 * calls. */
					do {
					} while(0);
					mnMessageCopyExpanded(statKind + (Sint32)MsgNormalStatus,
						helpText, SUMMARY_HELP_TEXT_LENGTH);
				} else if(cursor.helpEntry == OWNER_STATUS_HELP_EXPERIENCE &&
					unit->level < UNIT_LEVEL_MAX) {
					mnMessageArgumentPush(gmUnitGetExperienceForLevelStub(
											  member, unit->level + 1) -
							unit->experience,
						MESSAGE_ARGUMENT_NUMBER);
					mnMessageCopyExpanded((Sint32)MsgOwnerStatusNextLevel,
						helpText, SUMMARY_HELP_TEXT_LENGTH);
				} else {
					mnMessageCopyExpanded(
						cursor.helpEntry + (Sint32)MsgOwnerStatusEntryHelp,
						helpText, SUMMARY_HELP_TEXT_LENGTH);
				}
				mnTextDrawWideStringAtOffset(helpText, helpWin, 0, 4);
				syHeapRelease(helpText);
			}
			redraw = 0;
			state->dirty = 1;
		}

		{
			VRAM_CACHE_ENTRY *entry;
			Sint32			 *slot;
			Sint32			  iconX;

			spr = statSprs;
			slot = statSlots;
			for(i = 0, entry = VramCacheEntries, iconX = 112;
				i < STATUS_ICON_COUNT; i++) {
				MENU_SPR_INIT(
					*spr, OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				spr->oam.f.tile = entry[*slot++].ofs >> 5;
				spr->oam.f.x = iconX;
				spr->oam.f.y = statWin->y * 8 + 8;
				if(statKinds[i] > OWNER_CONDITION_NORMAL)
					syOAMAddSprAtDepth(spr, OAM_DEPTH_WINDOW);
				iconX += 15;
				spr++;
			}
			if(page == STATUS_PAGE_PSYNERGY || page == STATUS_PAGE_ITEMS) {
				VRAM_CACHE_ENTRY *iconEntry;
				Sint32			  iconRow;

				for(i = 0, iconEntry = VramCacheEntries, spr = iconSprs,
				iconRow = 0;
					i <= 3; i++) {
					MENU_SPR_INIT(*spr,
						OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
					spr->oam.f.tile = iconEntry[iconSlots[i]].ofs >> 5;
					if(page == STATUS_PAGE_ITEMS)
						spr->oam.f.x = 24;
					else
						spr->oam.f.x = 8;
					spr->oam.f.y = (iconRow + statWin->y) * 8 + 68;
					syOAMAddSprAtDepth(spr, OAM_DEPTH_WINDOW);
					spr++;
					iconRow += 2;
				}
			}
		}
		if(BattleMenuStateWork->tmr == 0 || (PerWork.push & KEY_B))
			break;
		if(listCnt != 0) {
			if(keys & KEY_R) {
				memberIdx++;
				if(memberIdx >= listCnt)
					memberIdx = 0;
				member = list[memberIdx];
				redraw = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			} else if(keys & KEY_L) {
				memberIdx--;
				if(memberIdx < 0)
					memberIdx = listCnt - 1;
				member = list[memberIdx];
				redraw = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		sySynch(1);
	}

	{
		Sint32 *slot = iconSlots;

		for(i = 0; i < 4; i++)
			syVRAMCacheFreeEntry(*slot++);
		slot = statSlots;
		for(i = 0; i < STATUS_ICON_COUNT; i++)
			syVRAMCacheFreeEntry(*slot++);
	}
	syVRAMCacheFreeEntry(portraitSlot);
	syVRAMCacheFreeEntry(cursorSlot);
	sySynch(1);
#if BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE
	mnWindowMarkVisibleTextTiles();
#endif
	mnWindowClose(statWin, 1);
	mnWindowClose(helpWin, 1);
	mnPartyStatDraw(Ram_HeapSlots->battleState->partyStat);
	BattleMenuStateWork->tmrHidden = 0;
	btObjectDestroySprsStub(shownMember);
	btObjectPutStub();
	sySynch(1);
	syHeapRelease(djinnList);
	return 0;
}

#undef SUMMARY_ITEM_HELP_CAPACITY
#undef SUMMARY_DJINN_HELP_CAPACITY
#undef SUMMARY_HELP_TEXT_LENGTH
#undef SUMMARY_STAT_VALUE_X
#undef SUMMARY_DRAW_PAGE_MARKS
#undef OWNER_STATUS_LABEL
