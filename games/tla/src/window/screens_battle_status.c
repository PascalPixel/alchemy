/* @file */
/* The Status screen. */
/* Part of screens.c. */

#include "battle/motobj.h"

/* Room for the help line of an item and of a Djinni, as measured: the item */
/* line grows from German on, the Djinni line from French on. */
#if TLA_PATCH_2
#define SUMMARY_ITEM_HELP_CAPACITY 52
#else
#define SUMMARY_ITEM_HELP_CAPACITY 32
#endif
#if TLA_PATCH_3
#define SUMMARY_DJINN_HELP_CAPACITY 52
#else
#define SUMMARY_DJINN_HELP_CAPACITY 32
#endif
/* Room for a help line no edition lengthens: no items, an item that cannot */
/* be used, no Djinn, a Djinni ready to summon. */
#define SUMMARY_HELP_CAPACITY 32

/* Japanese text is shorter: its help lines and Psynergy descriptions take */
/* smaller buffers, and its stat numbers sit one tile further left. It decodes
 */
/* the item, Psynergy and Djinn lines raw where the other languages copy them */
/* expanded (BATTLE_MENU_MESSAGE_COPY); the lines that take a number are copied
 */
/* expanded in every language. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define SUMMARY_HELP_TEXT_LENGTH 64
#define SUMMARY_STAT_VALUE_X	 176
#else
#define SUMMARY_HELP_TEXT_LENGTH 128
#define SUMMARY_STAT_VALUE_X	 184
#endif

/* The condition icons the Status screen shows at most: Normal, or each */
/* condition the unit has, in a row. */
#define STATUS_ICON_COUNT 11

/* The Status screen's labels, in their message order up to MsgExp */
/* (OWNER_STATUS_LABEL). */
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
#define OWNER_STATUS_LABEL(row) ((s32)MsgExp - OWNER_STATUS_LABEL_EXP + (row))

/* The Status screen's help lines, from MsgOwnerStatusEntryHelp, which the */
/* cursor stops in status_tables.c name; the status icons' places and the */
/* three list pages follow. */
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
	/* The first status icon's place: its help is its status's */
	/* (MsgNormalStatus + condition). */
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

/* The Status screen's cursor groups, each with its table of cursor stops. */
/* The name, experience, HP, PP, class, Djinn and element rows */
/* (OwnerStatus_StatCursors). */
#define STATUS_CURSOR_GENERAL 0
/* Attack, Defense, Agility and Luck (OwnerStatus_EquipmentCursors). */
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS   2 /* the status icons */
#define STATUS_CURSOR_PSYNERGY	   3 /* the Psynergy page's rows */
#define STATUS_CURSOR_DJINN		   4 /* the Djinn page's grid */
#define STATUS_CURSOR_ITEMS		   5 /* the items page's rows */

/* The general group's stops (OwnerStatus_StatCursors): nine for a member */
/* with Djinn, then the seven of one without. */
#define STATUS_STAT_STOPS				9
#define STATUS_STAT_STOPS_WITHOUT_DJINN 7

#if TLA_PATCH_2
extern const u8 OwnerStatus_NivString[];
extern const u8 OwnerStatus_NvString[];
extern const u8 OwnerStatus_KpString[];
extern const u8 OwnerStatus_PvString[];
#endif

void btObjectSpawnObjectsForListStub(u16 *list, s32 count, s32 mode);
s32 gmUnitGetExperienceForLevelStub(s32 member, s32 level);

/* The four list-page scroll marks, one tile per page of four rows (Psynergy */
/* and items). */
#define SUMMARY_DRAW_PAGE_MARKS(window, count, rowBase)                        \
	{                                                                          \
		for(j = 0; j < ((count) + 3) / 4; j++) {                               \
			s32 tile = PAGE_NUMBER_TILE + j;                                   \
                                                                               \
			if(j == (rowBase) / 4)                                             \
				tile = PAGE_NUMBER_LIT_TILE + j;                               \
			mnWindowSetTilemapEntry(                                           \
				window, tile, (window)->width - ((count) + 3) / 4 + j - 10, 7, \
				0);                                                            \
		}                                                                      \
		mnWindowSetTilemapEntry(window, STATUS_TILE_PAGES_LEFT,                \
								(window)->width - ((count) + 3) / 4 - 11, 7,   \
								0);                                            \
		mnWindowSetTilemapEntry(window, STATUS_TILE_PAGES_RIGHT,               \
								(window)->width - 10, 7, 0);                   \
	}

/* Runs the Status screen for party member `member` until B is pressed or */
/* the link battle's time runs out: the party member's level, experience, HP */
/* and PP, class, stats, Djinn, elements and statuses, with a help line for the
 */
/* item under the cursor. With `listCount` nonzero, L and R step through the */
/* party members of `list` (BATTLE_UNIT_LIST_END ends it, BATTLE_UNIT_REMOVED */
/* is an empty place). The link timer's window is closed while it is open */
/* (BATTLE_MENU_STATE.tmrHidden), and the party status window is redrawn on
 * the
 */
/* way out. Returns 0. mnBattleCollectPartyCommands opens it from the Status */
/* command and from L or R in a party member's command row. */
s32 mnBattleStatScrRun(u16 *list, s32 listCount, s32 member) {
	SUMMARY_OAM portraitSprite;
	SUMMARY_OAM cursorSprite;
	s32 statusSlots[STATUS_ICON_COUNT];
	s8 statusKinds[STATUS_ICON_COUNT];
	SUMMARY_OAM statusSprites[STATUS_ICON_COUNT];
	SUMMARY_OAM iconSprites[4];
	SUMMARY_OAM nameSprite;
	STAT_CURSOR cursor;
	s32 iconSlots[4];
	u16 figureList[2];
	u16 spawnList[2];
	u16 itemText[64];
	u16 psynergyText[64];
	WIN_STATE *state;
	WINDOW *statusWindow;
	s32 redraw;
	WINDOW *helpWindow;
	s32 memberIndex;
	s32 cursorSlot;
	SUMMARY_POINT cursorPosition;
	s32 portraitSlot;
	s32 statusCount;
	s32 hasDjinn;
	s32 shownMember;
	s32 page;
	s32 figureShown;
	u32 *djinnList;
	s32 djinnCount;
	s32 psynergyCount;
	s32 itemCount;
	s32 keys;
	UNIT *unit;
	s32 i;
	s32 j;
	BATTLE_MENU_STATE *navigation;
	s32 rowBase;
	s32 row;
	s32 groupPosition;
	SUMMARY_OAM *sprite;

	state = (WIN_STATE *)Ram_HeapSlots->winState;
	redraw = 1;
	memberIndex = -1;
	statusCount = 0;
	hasDjinn = gmPartySumDjinnCountsStub(-1);
	page = 0;
	figureShown = 0;
	djinnList = syHeapAllocEwram(84 * sizeof(*djinnList));
	itemCount = psynergyCount = djinnCount = 0;
	shownMember = figureList[0] = list[0];
	figureList[1] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(figureList, 1);
	portraitSlot = syVramCacheAllocEntry(512);
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
	cursorSlot = syVramCacheAllocEntry(128);
	for(i = 0; i < STATUS_ICON_COUNT; i++) {
		statusSlots[i] = syVramCacheAllocEntry(128);
		statusKinds[i] = OWNER_CONDITION_EMPTY;
	}
	for(i = 0; i <= 3; i++) {
		iconSlots[i] = syVramCacheAllocEntry(128);
		iconSprites[i].oam.f.tile =
			syVramCacheReload(iconSlots[i], (const void *)-1);
	}
	if(listCount != 0) {
		for(i = 0; i <= 5; i++) {
			s32 kind = list[i];

			if(kind == BATTLE_UNIT_LIST_END)
				break;
			if(kind != BATTLE_UNIT_REMOVED && kind == member) {
				memberIndex = i;
				break;
			}
		}
	}
	cursorPosition.x = 0;
	cursorPosition.y = 0;
	statusWindow =
		mnWindowCreate(0, 0, 30, 20, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	helpWindow = mnWindowCreate(0, 14, 30, 6,
								WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
	mnWindowMarkVisibleTextTiles();

	for(;;) {
		keys = InputWork.repeat;
		sprite = &nameSprite;
		mnBattleStatDrawPageArrows(STATUS_PAGE_ARROW_VRAM);
		if(redraw != 0)
			mnPortraitLoad(member, portraitSlot);
		syMenuSprInit(*sprite,
					  OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
		sprite->oam.f.tile = VramCacheEntries[portraitSlot].ofs >> 5;
		sprite->oam.f.x = 8;
		sprite->oam.f.y = 24;
		sprite->oam.f.pal = PORTRAIT_PALETTE;
		syOamAddSprAtDepth(sprite, OAM_DEPTH_WINDOW);

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
			cursor.groupSize = statusCount;
			break;
		case OWNER_STATUS_HELP_PSYNERGY_LIST:
			cursor.group = STATUS_CURSOR_PSYNERGY;
			cursor.groupSize = psynergyCount;
			break;
		case OWNER_STATUS_HELP_DJINN_LIST:
			cursor.group = STATUS_CURSOR_DJINN;
			cursor.groupSize = djinnCount;
			break;
		case OWNER_STATUS_HELP_ITEM_LIST:
			cursor.group = STATUS_CURSOR_ITEMS;
			cursor.groupSize = itemCount;
			break;
		}
		mnWindowHighlightRect(statusWindow->x, statusWindow->y,
							  statusWindow->width, statusWindow->height,
							  WINDOW_PALETTE);

		/* A turns the page; each list page starts its own group at the top. */
		if(InputWork.pressed & KEY_A) {
			page = (page + 1) % STATUS_PAGE_COUNT;
			/* page + 3 is the list group of the page after the new one, so a
			 * list */
			/* goes back to its top on the turn to the page before it; A turns
			 * only */
			/* forward, so each list still opens at its top. The switch above
			 * sets */
			/* the group again from the help entry. */
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

		if(statusCount != 0) {
			if((u32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
				groupPosition = cursor.groupPositions[cursor.group];
				if(keys & KEY_DOWN) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					groupPosition++;
					if(groupPosition >= cursor.groupSize) {
						groupPosition = 0;
						if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
							cursor.group = STATUS_CURSOR_CONDITIONS;
							groupPosition = cursor.groupPositions[cursor.group];
						}
					}
				}
				else if(keys & KEY_UP) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					groupPosition--;
					if(groupPosition < 0) {
						groupPosition = cursor.groupSize - 1;
						if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
							cursor.group = STATUS_CURSOR_CONDITIONS;
							groupPosition = cursor.groupPositions[cursor.group];
						}
					}
				}
				else if(keys & KEYS_HORIZONTAL) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					cursor.group ^= STATUS_CURSOR_CONDITIONS;
					if(InputWork.repeat & KEY_LEFT)
						groupPosition = statusCount - 1;
					else
						groupPosition = 0;
				}
				cursor.groupPositions[cursor.group] = groupPosition;
			}
			else if(cursor.group == STATUS_CURSOR_CONDITIONS) {
				groupPosition = cursor.groupPositions[cursor.group];
				if(groupPosition >= cursor.groupSize)
					groupPosition = cursor.groupSize - 1;
				if(groupPosition < 0) {
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPosition =
						cursor.groupPositions[STATUS_CURSOR_GENERAL];
				}
				else if(keys & KEY_RIGHT) {
					keys = 0;
					groupPosition++;
					if(groupPosition >= cursor.groupSize) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPosition =
							cursor.groupPositions[STATUS_CURSOR_GENERAL];
					}
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				}
				else if(keys & KEY_LEFT) {
					keys = 0;
					groupPosition--;
					if(groupPosition < 0) {
						cursor.group = STATUS_CURSOR_GENERAL;
						groupPosition =
							cursor.groupPositions[STATUS_CURSOR_GENERAL];
					}
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				}
				else if(keys & KEYS_VERTICAL) {
					keys = 0;
					cursor.group = STATUS_CURSOR_GENERAL;
					groupPosition =
						cursor.groupPositions[STATUS_CURSOR_GENERAL];
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				}
				cursor.groupPositions[cursor.group] = groupPosition;
			}
			else if(cursor.group == STATUS_CURSOR_PSYNERGY ||
					cursor.group == STATUS_CURSOR_ITEMS) {
				/* Four rows a page: up and down step, left and right turn the
				 * page. */
				groupPosition = cursor.groupPositions[cursor.group];
				if(keys & KEY_DOWN) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPosition++;
					if(groupPosition >= cursor.groupSize)
						groupPosition = 0;
				}
				else if(keys & KEY_UP) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPosition--;
					if(groupPosition < 0)
						groupPosition = cursor.groupSize - 1;
				}
				else if(keys & KEY_RIGHT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPosition += 4;
					if(groupPosition >= cursor.groupSize) {
						if(groupPosition / 4 - 1 == (cursor.groupSize - 1) / 4)
							groupPosition -= groupPosition / 4 * 4;
						else
							groupPosition = cursor.groupSize - 1;
					}
				}
				else if(keys & KEY_LEFT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					groupPosition -= 4;
					if(groupPosition < 0)
						groupPosition =
							cursor.groupSize / 4 * 4 + (groupPosition + 4) % 4;
				}
				if(groupPosition >= cursor.groupSize)
					groupPosition = cursor.groupSize - 1;
				if(groupPosition < 0)
					groupPosition = 0;
				cursor.groupPositions[cursor.group] = groupPosition;
			}
			else if(cursor.group == STATUS_CURSOR_DJINN) {
				/* Three Djinn a column. */
				s32 djinnPosition = cursor.groupPositions[cursor.group];

				if(keys & KEY_DOWN) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPosition++;
					if(djinnPosition >= cursor.groupSize)
						djinnPosition = 0;
				}
				else if(keys & KEY_UP) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPosition--;
					if(djinnPosition < 0)
						djinnPosition = cursor.groupSize - 1;
				}
				else if(keys & KEY_RIGHT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPosition += 3;
					if(djinnPosition >= cursor.groupSize)
						djinnPosition = djinnPosition % 3;
				}
				else if(keys & KEY_LEFT) {
					keys = 0;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw |= 2;
					djinnPosition -= 3;
					if(djinnPosition < 0)
						djinnPosition =
							cursor.groupSize / 3 * 3 + (djinnPosition + 3) % 3;
				}
				if(djinnPosition >= cursor.groupSize)
					djinnPosition = cursor.groupSize - 1;
				if(djinnPosition < 0)
					djinnPosition = 0;
				cursor.groupPositions[cursor.group] = djinnPosition;
			}
		}

		if(cursor.group == STATUS_CURSOR_GENERAL) {
			const BATTLE_SUMMARY_STAT_CURSOR *stop;
			s32 k = cursor.groupPositions[cursor.group];

			if(hasDjinn == 0)
				k += STATUS_STAT_STOPS;
			stop = &OwnerStatus_StatCursors[k];
			cursor.helpEntry = stop->entry;
			cursor.column = stop->column;
			cursor.row = stop->row;
			mnWindowHighlightRect(statusWindow->x + stop->highlightColumn + 1,
								  statusWindow->y + stop->highlightRow + 1,
								  stop->highlightWidth, 1, HIGHLIGHT_PALETTE);
		}
		else if(cursor.group == STATUS_CURSOR_COMBAT_STATS) {
			cursor.helpEntry =
				OwnerStatus_EquipmentCursors[cursor
												 .groupPositions[cursor.group]]
					.entry;
			cursor.column =
				OwnerStatus_EquipmentCursors[cursor
												 .groupPositions[cursor.group]]
					.column;
			cursor.row =
				OwnerStatus_EquipmentCursors[cursor
												 .groupPositions[cursor.group]]
					.row;
		}
		else if(cursor.group == STATUS_CURSOR_CONDITIONS) {
			cursor.helpEntry =
				OwnerStatus_ConditionCursors[cursor
												 .groupPositions[cursor.group]]
					.entry;
			cursor.column =
				OwnerStatus_ConditionCursors[cursor
												 .groupPositions[cursor.group]]
					.column;
			cursor.row =
				OwnerStatus_ConditionCursors[cursor
												 .groupPositions[cursor.group]]
					.row;
			if(statusKinds[0] == OWNER_CONDITION_NORMAL)
				mnWindowHighlightRect(statusWindow->x + 15, statusWindow->y + 1,
									  10, 1, HIGHLIGHT_PALETTE);
			else
				mnWindowHighlightRect(statusWindow->x + cursor.column + 1,
									  statusWindow->y + cursor.row + 1, 2, 2,
									  HIGHLIGHT_PALETTE);
		}
		else if(cursor.group == STATUS_CURSOR_PSYNERGY ||
				cursor.group == STATUS_CURSOR_ITEMS) {
			s8 position = cursor.groupPositions[cursor.group];

			cursor.row = (s8)(position % 4) * 2 + 8;
			if(cursor.group == STATUS_CURSOR_PSYNERGY)
				cursor.column = -2;
			else
				cursor.column = -1;
			mnWindowHighlightRect(statusWindow->x + 1,
								  statusWindow->y + cursor.row + 1, 20, 1,
								  HIGHLIGHT_PALETTE);
		}
		else {
			s32 position = cursor.groupPositions[STATUS_CURSOR_DJINN];

			cursor.row = (s8)(position % 3) * 2 + 9;
			cursor.column = (s8)(position / 3) * 7 - 2;
			mnWindowHighlightRect(statusWindow->x + cursor.column + 3,
								  statusWindow->y + cursor.row + 1, 7, 1,
								  HIGHLIGHT_PALETTE);
		}
		if(cursor.drawnHelpEntry != cursor.helpEntry) {
			cursor.drawnHelpEntry = cursor.helpEntry;
			redraw |= 2;
		}
		cursorPosition.x = cursor.column * 8;
		cursorPosition.y = cursor.row * 8;

		/* The member's battle figure, respawned when L or R changes the member.
		 */
		if(shownMember != member) {
			spawnList[0] = member;
			spawnList[1] = BATTLE_UNIT_LIST_END;
			btObjectDestroySprsStub(shownMember);
			shownMember = member;
			btObjectSpawnObjectsForListStub(spawnList, 1, 0);
			figureShown = 0;
		}
		{
			SUMMARY_OAM *face = btObjectGetStub(member)->obj->records;

			syMenuSprInit(portraitSprite,
						  OBJ_SIZE_64X64 | OBJ_256_COLORS |
							  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
			if(face != NULL) {
				portraitSprite.oam.f.tile = face->oam.f.tile;
				portraitSprite.oam.f.x = 172;
				portraitSprite.oam.f.y = 56;
				if(figureShown != 0)
					syOamAddSprAtDepth(&portraitSprite, OAM_DEPTH_WINDOW);
				figureShown = 1;
			}
		}

		syMenuSprInit(cursorSprite,
					  OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
		cursorSprite.oam.f.tile =
			syVramCacheReload(cursorSlot, PointingHandTiles);
		{
			s32 bob = FrameCount & 4;
			s32 x = statusWindow->x * 8 + cursorPosition.x + 16;

			cursorSprite.oam.f.x =
				page == STATUS_PAGE_STATS ? x - bob / 4 : x + bob / 4;
		}
		cursorSprite.oam.f.y = cursorPosition.y + statusWindow->y * 8 -
							   (s32)(FrameCount & 4) / 4 + 16;
		if(page != STATUS_PAGE_STATS)
			cursorSprite.oam.f.affineIdx = 0;
		else
			cursorSprite.oam.f.affineIdx = 8;
		syOamAddSprAtDepth(&cursorSprite, OAM_DEPTH_WINDOW + 1);

		if(redraw != 0) {

			unit = syUnitGet(member);
			if(page == STATUS_PAGE_STATS)
				mnTextFillCanvasBackground();

			if(page == STATUS_PAGE_ITEMS) {
				/* Items: those the member can use in battle first. */
				u16 *entries = BattleMenuStateWork->listAbilities;
				s32 count;
				s32 index;
				s32 item;
				s32 row;

				count = 0;
				j = -1;
				for(index = 0; index != INVENTORY_SLOTS &&
							   (item = unit->inventory[index]) != 0;
					index++) {
					gmItemGetDirectStub(item);
					if(mnItemCheckBattleUse(member, item) == 0)
						entries[count++] = item;
				}
				for(index = 0; index != INVENTORY_SLOTS &&
							   (item = unit->inventory[index]) != 0;
					index++) {
					gmItemGetDirectStub(item);
					if(mnItemCheckBattleUse(member, item) != 0)
						entries[count++] = item;
				}
				entries[count] = 0;
				itemCount = count;
				if(count <= cursor.groupPositions[5])
					cursor.groupPositions[5] = count - 1;
				rowBase = cursor.groupPositions[5] / 4 * 4;
				row = (s8)(cursor.groupPositions[5] % 4);
				if(count != 0) {
					u16 *entry = entries + rowBase + row;

					if(mnItemCheckBattleUse(member, *entry) == 2)
						BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemUnavailable,
												 itemText,
												 SUMMARY_HELP_CAPACITY);
					else
						BATTLE_MENU_MESSAGE_COPY(
							(*entry & INVENTORY_ITEM_ID_MASK) +
								(s32)MsgItemDescription,
							itemText, SUMMARY_ITEM_HELP_CAPACITY);
				}
				else {
					BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleNoItems, itemText,
											 SUMMARY_HELP_CAPACITY);
				}
				mnWindowSprRedrawSavedRect(statusWindow);
				mnWindowMapTextCanvasTiles(helpWindow->x, helpWindow->y,
										   helpWindow->width,
										   helpWindow->height, 0);
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(itemText, helpWindow, 0, 20);
				if(rowBase != j) {
					mnWindowDrawDividerLine(statusWindow, 0, 16, 29, 16);
					for(j = 0; j <= 3 && (item = entries[rowBase + j]) != 0;
						j++) {
						gmItemGetDirectStub(item);
						mnTextSetCol(TEXT_COLOR_WHITE);
						if(mnItemCheckBattleUse(member, item) != 0)
							mnTextSetCol(TEXT_COLOR_YELLOW);
						else if(item & INVENTORY_BROKEN)
							mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							(item & INVENTORY_ITEM_ID_MASK) + (s32)MsgItemName,
							statusWindow, 32, 64 + j * 16);
						mnTextSetCol(TEXT_COLOR_WHITE);
						iconSprites[j].oam.f.tile =
							mnItemIconLoadIntoSlot(item, iconSlots[j]);
					}
					/* FAKEMATCH(dead): a store nothing reads, which keeps the
					 * list loop's exit on the fill loop's test. */
					item = 0;
					for(; j <= 3; j++)
						iconSprites[j].oam.f.tile =
							syVramCacheReload(iconSlots[j], (const void *)-1);
				}
				if(count > 4)
					SUMMARY_DRAW_PAGE_MARKS(statusWindow, count, rowBase);
			}
			else if(page == STATUS_PAGE_PSYNERGY) {
				/* Battle Psynergy: grey when it cannot be used in battle, red
				 * when the */
				/* PP are short, another colour while Psynergy is sealed. */
				u16 *entries = BattleMenuStateWork->listAbilities;
				s32 count;
				s32 shownRow;
				s32 index;
				s32 id;

				count = 0;
				j = -1;
				mnWindowSprRedrawSavedRect(statusWindow);
				shownRow = j;
				for(index = 0; index != PSYNERGY_SLOTS &&
							   (id = unit->psynergy[index].encodedAction &
									 PSYNERGY_ABILITY_MASK) != 0;
					index++) {
					if(btAbilityGet(id)->flg & ABILITY_USABLE_IN_BATTLE)
						entries[count++] = id;
				}
				entries[count] = 0;
				psynergyCount = count;
				if(count <= cursor.groupPositions[3])
					cursor.groupPositions[3] = count - 1;
				rowBase = cursor.groupPositions[3] / 4 * 4;
				row = (s8)(cursor.groupPositions[3] % 4);
				if(rowBase != j || row != shownRow) {
					state->holdTilemapUpload = 1;
					mnTextFillCanvasBackground();
					if(count != 0)
						BATTLE_MENU_MESSAGE_COPY(
							(entries[rowBase + row] & PSYNERGY_ABILITY_MASK) +
								(s32)MsgAbilityDescription,
							psynergyText, BATTLE_MENU_MESSAGE_CAPACITY);
					else
						BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleNoPsynergy,
												 psynergyText,
												 BATTLE_MENU_MESSAGE_CAPACITY);
					mnWindowSprRedrawSavedRect(statusWindow);
					mnWindowMapTextCanvasTiles(helpWindow->x, helpWindow->y,
											   helpWindow->width,
											   helpWindow->height, 0);
					mnTextDrawWideStringAtOffset(psynergyText, helpWindow, 0,
												 20);
					if(rowBase != j) {
						mnWindowDrawDividerLine(statusWindow, 0, 16, 29, 16);
						for(j = 0; j <= 3 && (id = entries[rowBase + j]) != 0;
							j++) {
							ABILITY_DEFINITION *ability = btAbilityGet(id);
							s32 range;
							s32 tileFlags;

							mnWindowSetTilemapEntry(statusWindow,
													WINDOW_TILE_PP_LABEL, 11,
													j * 2 + 8, 0);
							mnWindowSetTilemapEntry(statusWindow,
													WINDOW_TILE_PP_LABEL_END,
													12, j * 2 + 8, 0);
							iconSprites[j].oam.f.tile =
								mnAbilityIconLoadIntoSlot(id, iconSlots[j]);
							if(!(ability->flg & ABILITY_USABLE_IN_BATTLE))
								mnTextSetCol(TEXT_COLOR_YELLOW);
							else if(ability->ppCost > unit->pp)
								mnTextSetCol(TEXT_COLOR_RED);
							else if(unit->psySeal != 0)
								mnTextSetCol(TEXT_COLOR_PINK);
							tileFlags = 0;
							state->textPal = 5;
							mnTextDrawMessageAsTiles(id + (s32)MsgAbilityName,
													 statusWindow, 16,
													 64 + j * 16);
							mnTextDrawNumAtOffset(ability->ppCost, 2,
												  statusWindow, 104,
												  64 + j * 16);
							mnTextSetCol(TEXT_COLOR_WHITE);
							state->textPal = WINDOW_PALETTE;
							if(ability->element != ELEMENT_NONE)
								mnWindowSetTilemapEntry(
									statusWindow,
									ELEMENT_ICON_TILE + ability->element, 15,
									j * 2 + 8, tileFlags);
							range = ability->range;
							if(range == ABILITY_RANGE_ALL)
								range = RANGE_ICON_ALL;
							else
								range--;
							mnRangeIconDraw(statusWindow, 16, j * 2 + 8, range,
											0);
						}
						/* FAKEMATCH(dead): a store nothing reads, which keeps
						 * the list loop's exit on the fill loop's test. */
						id = 0;
						for(; j <= 3; j++)
							iconSprites[j].oam.f.tile = syVramCacheReload(
								iconSlots[j], (const void *)-1);
					}
					if(count > 4)
						SUMMARY_DRAW_PAGE_MARKS(statusWindow, count, rowBase);
					state->dirty = 1;
					state->holdTilemapUpload = 0;
				}
			}
			else if(page == STATUS_PAGE_DJINN) {
				/* Djinn: each element's set ones, then those on standby or
				 * recovering */
				/* (BATTLE_DJINN_UNSET, the turns left in BATTLE_DJINN_TURNS).
				 */
				s32 rowBase = 0;
				s32 shownBase = -1;
				s32 released = 0;
				s32 element;
				s32 bit;
				u32 entry;

				mnWindowSprRedrawSavedRect(statusWindow);
				mnWindowMapTextCanvasTiles(helpWindow->x, helpWindow->y,
										   helpWindow->width,
										   helpWindow->height, 0);
				mnWindowDrawDividerLine(statusWindow, 0, 16, 29, 16);
				djinnCount = 0;
				for(element = 0; element < ELEMENT_COUNT; element++) {
					for(bit = 0; bit < DJINN_PER_ELEMENT; bit++) {
						if(unit->djinnSet[element] & (1 << bit)) {
							djinnList[djinnCount] =
								(element << BATTLE_DJINN_ELEMENT_SHIFT) | bit;
							djinnCount++;
						}
						else if(unit->djinnHeld[element] & (1 << bit)) {
							DJINN_RECOVERY_LIST *recovery =
								&((DJINN_RECOVERY_TABLE *)
									  gmDjinnGetRecoveryTableStub((u32)member >
																  ALLY_MAX))
									 ->list;
							s32 k;
							s32 value;

							for(k = 0;
								k < recovery->cnt &&
								!(recovery->entries[k].unitId == member &&
								  recovery->entries[k].element == element &&
								  recovery->entries[k].idx == bit);
								k++)
								;
							value = (element << BATTLE_DJINN_ELEMENT_SHIFT) |
									bit | BATTLE_DJINN_UNSET;
							djinnList[djinnCount] = value;
							if(recovery->entries[k].turns > 0)
								djinnList[djinnCount] =
									value | (recovery->entries[k].turns
											 << BATTLE_DJINN_TURNS_SHIFT);
							djinnCount++;
						}
					}
				}
				djinnList[djinnCount] = BATTLE_DJINN_END;
				if(cursor.groupPositions[STATUS_CURSOR_DJINN] >= djinnCount)
					cursor.groupPositions[STATUS_CURSOR_DJINN] = djinnCount - 1;
				entry = djinnList[cursor.groupPositions[STATUS_CURSOR_DJINN]];
				state->holdTilemapUpload = 1;
				if(djinnCount != 0) {
					mnMessageArgumentClearAll();
					if(entry & BATTLE_DJINN_UNSET) {
						if(entry & BATTLE_DJINN_TURNS) {
							mnMessageArgumentPush(
								(entry & BATTLE_DJINN_TURNS) >>
									BATTLE_DJINN_TURNS_SHIFT,
								MESSAGE_ARGUMENT_NUMBER);
							BATTLE_MENU_MESSAGE_COPY(
								BATTLE_DJINNI(entry) + (s32)MsgDjinnEffect,
								psynergyText, SUMMARY_DJINN_HELP_CAPACITY);
						}
						else {
							mnMessageCopyExpanded((s32)MsgReadyToSummon,
												  psynergyText,
												  SUMMARY_HELP_CAPACITY);
						}
					}
					else {
						BATTLE_MENU_MESSAGE_COPY(
							BATTLE_DJINNI(entry) + (s32)MsgDjinnEffect,
							psynergyText, SUMMARY_DJINN_HELP_CAPACITY);
					}
				}
				else {
					BATTLE_MENU_MESSAGE_COPY((s32)MsgNoDjinn, psynergyText,
											 SUMMARY_HELP_CAPACITY);
				}
				state->holdTilemapUpload = released;
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(psynergyText, helpWindow, 0, 20);
				if(rowBase != shownBase) {
					u32 shown;
					s32 column;
					s32 tileRow;

					for(j = 0; j <= 8; j++) {
						/* rowBase is this page's 0, as pages 1 and 3 index
						 * their lists. */
						shown = djinnList[rowBase + j];
						column = j / 3 * 7;
						tileRow = j % 3 * 2 + 9;
						if(shown == BATTLE_DJINN_END)
							break;
						mnWindowSetTilemapEntry(statusWindow,
												BATTLE_DJINN_ELEMENT_OF(shown) +
													ELEMENT_ICON_TILE,
												column, tileRow, 0);
						if(shown & BATTLE_DJINN_TURNS)
							mnTextSetCol(TEXT_COLOR_YELLOW);
						else if(shown & BATTLE_DJINN_UNSET)
							mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							BATTLE_DJINNI(shown) + (s32)MsgDjinnName,
							statusWindow, column * 8 + 8, tileRow * 8);
/* Spanish and Italian leave a recovering Djinni's turns off the list. */
#if !TLA_PATCH_4
						if(shown & BATTLE_DJINN_TURNS)
							mnTextDrawNumAtOffset(
								(shown & BATTLE_DJINN_TURNS) >>
									BATTLE_DJINN_TURNS_SHIFT,
								2, statusWindow, column * 8 + 40, tileRow * 8);
#endif
						mnTextSetCol(TEXT_COLOR_WHITE);
					}
				}
				/* LEFTOVER(template): the scroll marks under a count past four,
				 * as mnBattleStatScrRun's item and Psynergy pages draw theirs
				 * with SUMMARY_DRAW_PAGE_MARKS; this page's count stays 0; as
				 * no arm. */
				if(released > 4)
					SUMMARY_DRAW_PAGE_MARKS(statusWindow, released, rowBase);
				state->dirty = 1;
				state->holdTilemapUpload = 0;
			}

			if(redraw != 0) {
				if(page == STATUS_PAGE_STATS) {
					mnWindowSprRedrawSavedRect(statusWindow);
					mnWindowMapTextCanvasTiles(helpWindow->x, helpWindow->y,
											   helpWindow->width,
											   helpWindow->height, page);
					mnWindowDrawDividerLine(statusWindow, 0, 14, 29, 14);
				}
				mnTextDrawStringAtOffset(unit->name, statusWindow, 0, 0);
				if(memberIndex > 3)
					mnTextDrawMessageAsTiles((s32)MsgInTheBack, statusWindow,
											 40, 16);
#if TLA_PATCH_2
				/* The European editions share one image and choose the level's
				 * label, */
				/* drawing the name again under it, at run time. */
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_FRENCH:
					mnTextDrawStringAtOffset(unit->name, statusWindow, 0, 0);
					mnTextDrawStringAtOffset(OwnerStatus_NivString,
											 statusWindow, 51, 0);
					mnTextDrawNumInWindow(unit->level, 2, statusWindow, 72, 0);
					break;
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(unit->name, statusWindow, 0, 0);
					mnTextDrawStringAtOffset(OwnerStatus_NvString, statusWindow,
											 56, 0);
					mnTextDrawNumInWindow(unit->level, 2, statusWindow, 72, 0);
					break;
				default:
					mnTextDrawStringAtOffset(unit->name, statusWindow, 0, 0);
					mnTextDrawStringAtOffset(OwnerStatus_LevelString,
											 statusWindow, 56, 0);
					mnTextDrawNumInWindow(unit->level, 2, statusWindow, 72, 0);
					break;
				}
#else
				mnTextDrawStringAtOffset(OwnerStatus_LevelString, statusWindow,
										 56, 0);
				mnTextDrawNumInWindow(unit->level, 2, statusWindow, 72, 0);
#endif
				mnTextDrawMessageAsTiles((s32)MsgExp, statusWindow, 0, 8);
				mnTextDrawNumInWindow(unit->experience, 8, statusWindow, 40, 8);
#if TLA_PATCH_2
				switch(syGetLanguage()) {
				case CARTRIDGE_LANGUAGE_GERMAN:
					mnTextDrawStringAtOffset(OwnerStatus_KpString, statusWindow,
											 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_FRENCH:
				case CARTRIDGE_LANGUAGE_SPANISH:
					mnTextDrawStringAtOffset(OwnerStatus_PvString, statusWindow,
											 40, 24);
					break;
				case CARTRIDGE_LANGUAGE_ITALIAN:
					mnTextDrawStringAtOffset(OwnerStatus_HpString, statusWindow,
											 40, 24);
					break;
				}
#else
				mnTextDrawStringAtOffset(OwnerStatus_HpString, statusWindow, 40,
										 24);
#endif
				mnTextDrawNumInWindow(unit->hp, 4, statusWindow, 56, 24);
				mnTextDrawStringAtOffset(OwnerStatus_SlashString, statusWindow,
										 88, 24);
				mnTextDrawNumInWindow(unit->hpMax, 4, statusWindow, 96, 24);
				mnTextDrawStringAtOffset(OwnerStatus_PpString, statusWindow, 40,
										 32);
				mnTextDrawNumInWindow(unit->pp, 4, statusWindow, 56, 32);
				mnTextDrawStringAtOffset(OwnerStatus_SlashString, statusWindow,
										 88, 32);
				mnTextDrawNumInWindow(unit->ppMax, 4, statusWindow, 96, 32);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ATTACK), statusWindow,
					136, 16);
				mnTextDrawNumInWindow(unit->attack, 3, statusWindow,
									  SUMMARY_STAT_VALUE_X, 16);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DEFENSE),
					statusWindow, 136, 24);
				mnTextDrawNumInWindow(unit->defense, 3, statusWindow,
									  SUMMARY_STAT_VALUE_X, 24);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_AGILITY),
					statusWindow, 136, 32);
				mnTextDrawNumInWindow(unit->agility, 3, statusWindow,
									  SUMMARY_STAT_VALUE_X, 32);
				mnTextDrawMessageAsTiles(
					OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_LUCK), statusWindow,
					136, 40);
				mnTextDrawNumInWindow(unit->luck, 3, statusWindow,
									  SUMMARY_STAT_VALUE_X, 40);
				mnTextDrawMessageAsTiles((s32)MsgClassName + unit->classId,
										 statusWindow, 0, 48);
				if(page == STATUS_PAGE_STATS) {
					if(hasDjinn != 0)
						mnTextDrawMessageAsTiles(
							OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DJINN),
							statusWindow, 0, 72);
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ELEMENT_LEVEL),
						statusWindow, 0, 80);
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_POWER),
						statusWindow, 0, 88);
					mnTextDrawMessageAsTiles(
						OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_RESIST),
						statusWindow, 0, 96);
					for(j = 0; j < ELEMENT_COUNT; j++) {
						s32 iconRow = 1;

						if(hasDjinn != 0)
							iconRow = 0;
						mnWindowSetTilemapEntry(statusWindow,
												ELEMENT_ICON_TILE + j,
												7 + j * 4, iconRow + 8, 0);
						if(hasDjinn != 0) {
							mnTextDrawNumInWindow(unit->djinnSetCnts[j], 1,
												  statusWindow, 40 + j * 32,
												  72);
							mnTextDrawStringAtOffset(OwnerStatus_SlashString,
													 statusWindow, 48 + j * 32,
													 72);
							mnTextDrawNumInWindow(unit->djinnHeldCnts[j], 1,
												  statusWindow, 48 + j * 32 + 8,
												  72);
						}
						mnTextDrawNumInWindow(
							gmUnitGetElementLevelStub(member, j), 2,
							statusWindow, 48 + j * 32, 80);
						mnTextDrawNumInWindow(unit->element[j].power, 3,
											  statusWindow, 40 + j * 32, 88);
						mnTextDrawNumInWindow(unit->element[j].resistance, 3,
											  statusWindow, 40 + j * 32, 96);
					}
				}

				/* The status icons, as The Broken Seal orders them. */
				i = 0;
				if(unit->hp == 0) {
					statusKinds[0] = OWNER_CONDITION_DOWNED;
					i = 1;
				}
				do {
					if(unit->curse != 0) {
						statusKinds[i++] = OWNER_CONDITION_CURSED_ITEM;
					}
					if(i > 7)
						break;
					if(unit->grimReaper != 0) {
						statusKinds[i++] = OWNER_CONDITION_GRIM_REAPER;
					}
					if(i > 7)
						break;
					if(unit->poison == 1) {
						statusKinds[i++] = OWNER_CONDITION_POISON;
					}
					if(i > 7)
						break;
					if(unit->poison == 2) {
						statusKinds[i++] = OWNER_CONDITION_DEADLY_POISON;
					}
					if(i > 7)
						break;
					if(unit->psySeal != 0) {
						statusKinds[i++] = OWNER_CONDITION_PSYNERGY_SEALED;
					}
					if(i > 7)
						break;
					if(unit->stun != 0) {
						statusKinds[i++] = OWNER_CONDITION_STUN;
					}
					if(i > 7)
						break;
					if(unit->sleep != 0) {
						statusKinds[i++] = OWNER_CONDITION_SLEEP;
					}
					if(i > 7)
						break;
					if(unit->haunt != 0) {
						statusKinds[i++] = OWNER_CONDITION_HAUNT;
					}
					if(i > 7)
						break;
					if(unit->delusion != 0) {
						statusKinds[i++] = OWNER_CONDITION_DELUSION;
					}
					if(i > 7)
						break;
					if(unit->attackTurns != 0) {
						if(unit->attackStage > 0) {
							statusKinds[i++] = OWNER_CONDITION_ATTACK_UP;
						}
						if(unit->attackStage < 0) {
							statusKinds[i++] = OWNER_CONDITION_ATTACK_DOWN;
						}
					}
					if(i > 7)
						break;
					if(unit->defenseTurns != 0) {
						if(unit->defenseStage > 0) {
							statusKinds[i++] = OWNER_CONDITION_DEFENSE_UP;
						}
						if(unit->defenseStage < 0) {
							statusKinds[i++] = OWNER_CONDITION_DEFENSE_DOWN;
						}
					}
					if(i > 7)
						break;
					if(unit->resistanceTurns != 0) {
						if(unit->resistanceStage > 0) {
							statusKinds[i++] = OWNER_CONDITION_RESISTANCE_UP;
						}
						if(unit->resistanceStage < 0) {
							statusKinds[i++] = OWNER_CONDITION_RESISTANCE_DOWN;
						}
					}
					if(i > 7)
						break;
					if(unit->agilityStage > 0) {
						statusKinds[i++] = OWNER_CONDITION_AGILITY_UP;
					}
					if(unit->agilityStage < 0) {
						statusKinds[i++] = OWNER_CONDITION_AGILITY_DOWN;
					}
				} while(0);
				for(j = 0; j < i; j++)
					mnMiscIconLoadIntoSlot(statusKinds[j], statusSlots[j]);
				if(i == 0) {
					statusKinds[0] = OWNER_CONDITION_NORMAL;
					i = 1;
				}
				if(i < STATUS_ICON_COUNT) {
					for(j = i; j < STATUS_ICON_COUNT; j++)
						statusKinds[j] = OWNER_CONDITION_EMPTY;
				}
				statusCount = i;
				if(statusKinds[0] == OWNER_CONDITION_NORMAL)
					mnTextDrawMessageAsTiles(unit->hp != 0 ? (s32)MsgNormal
														   : (s32)MsgCantFight,
											 statusWindow, 112, 0);
			}

			if(page == STATUS_PAGE_STATS) {
				u16 *helpText =
					syHeapAlloc(SUMMARY_HELP_TEXT_LENGTH * sizeof(*helpText));

				if((u32)cursor.helpEntry >= OWNER_STATUS_HELP_CONDITION) {
					s32 statusKind = statusKinds[cursor.helpEntry -
												 OWNER_STATUS_HELP_CONDITION];
					UNIT *savedUnit;
					s32 attackDelta;
					s32 defenseDelta;
					s32 agilityDelta;
					s32 helpValue;

					if(statusKind == OWNER_CONDITION_NORMAL && unit->hp == 0)
						statusKind = OWNER_CONDITION_DOWNED;
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
					mnMessageCopyExpanded(statusKind + (s32)MsgNormalStatus,
										  helpText, SUMMARY_HELP_TEXT_LENGTH);
				}
				else if(cursor.helpEntry == OWNER_STATUS_HELP_EXPERIENCE &&
						unit->level < UNIT_LEVEL_MAX) {
					mnMessageArgumentPush(gmUnitGetExperienceForLevelStub(
											  member, unit->level + 1) -
											  unit->experience,
										  MESSAGE_ARGUMENT_NUMBER);
					mnMessageCopyExpanded((s32)MsgOwnerStatusNextLevel,
										  helpText, SUMMARY_HELP_TEXT_LENGTH);
				}
				else {
					mnMessageCopyExpanded(cursor.helpEntry +
											  (s32)MsgOwnerStatusEntryHelp,
										  helpText, SUMMARY_HELP_TEXT_LENGTH);
				}
				mnTextDrawWideStringAtOffset(helpText, helpWindow, 0, 4);
				syHeapRelease(helpText);
			}
			redraw = 0;
			state->dirty = 1;
		}

		{
			VRAM_CACHE_ENTRY *entry;
			s32 *slot;
			s32 iconX;

			sprite = statusSprites;
			slot = statusSlots;
			for(i = 0, entry = VramCacheEntries, iconX = 112;
				i < STATUS_ICON_COUNT; i++) {
				syMenuSprInit(*sprite, OBJ_SIZE_16X16 |
										   OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				sprite->oam.f.tile = entry[*slot++].ofs >> 5;
				sprite->oam.f.x = iconX;
				sprite->oam.f.y = statusWindow->y * 8 + 8;
				if(statusKinds[i] > OWNER_CONDITION_NORMAL)
					syOamAddSprAtDepth(sprite, OAM_DEPTH_WINDOW);
				iconX += 15;
				sprite++;
			}
			if(page == STATUS_PAGE_PSYNERGY || page == STATUS_PAGE_ITEMS) {
				VRAM_CACHE_ENTRY *iconEntry;
				s32 iconRow;

				for(i = 0, iconEntry = VramCacheEntries, sprite = iconSprites,
				iconRow = 0;
					i <= 3; i++) {
					syMenuSprInit(*sprite,
								  OBJ_SIZE_16X16 |
									  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
					sprite->oam.f.tile = iconEntry[iconSlots[i]].ofs >> 5;
					if(page == STATUS_PAGE_ITEMS)
						sprite->oam.f.x = 24;
					else
						sprite->oam.f.x = 8;
					sprite->oam.f.y = (iconRow + statusWindow->y) * 8 + 68;
					syOamAddSprAtDepth(sprite, OAM_DEPTH_WINDOW);
					sprite++;
					iconRow += 2;
				}
			}
		}
		if(BattleMenuStateWork->tmr == 0 || (InputWork.pressed & KEY_B))
			break;
		if(listCount != 0) {
			if(keys & KEY_R) {
				memberIndex++;
				if(memberIndex >= listCount)
					memberIndex = 0;
				member = list[memberIndex];
				redraw = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			else if(keys & KEY_L) {
				memberIndex--;
				if(memberIndex < 0)
					memberIndex = listCount - 1;
				member = list[memberIndex];
				redraw = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		sySynch(1);
	}

	{
		s32 *slot = iconSlots;

		for(i = 0; i < 4; i++)
			syVramCacheFreeEntry(*slot++);
		slot = statusSlots;
		for(i = 0; i < STATUS_ICON_COUNT; i++)
			syVramCacheFreeEntry(*slot++);
	}
	syVramCacheFreeEntry(portraitSlot);
	syVramCacheFreeEntry(cursorSlot);
	sySynch(1);
#if TLA_PATCH_1
	mnWindowMarkVisibleTextTiles();
#endif
	mnWindowClose(statusWindow, 1);
	mnWindowClose(helpWindow, 1);
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
