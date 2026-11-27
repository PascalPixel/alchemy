/* The party member chooser and the Status menu: its party member row, status
   page, Psynergy page and equipment page. Part of menu.c. */

#include "menu_internal.h"

/* In every edition but the Japanese the menu resets eight party figure rows
   rather than four, and saves and clears more window tiles for the extra text
   tiles it then uses; the editions outside Japanese also lay the menu out for
   longer words. */
#define MEMBER_CHOOSER_MEMBER_COUNT WHEN(EIGHT_PARTY_FIGURES_RESET, 8, 4)

/* The window tiles the Status menu saves and restores. */
#define STATUS_SAVED_TILE_BYTES WHEN(STATUS_MENU_EXTRA_TEXT_TILES, 8192, 2048)

/* The page modes of mnStatScreenDrawStatPage and mnStatScreenDrawTopRows, in
   the mode's low byte: the party row's summary, or the full status page, with
   the elemental levels, Power and Resist. */
enum { STATUS_PAGE_SUMMARY = 0, STATUS_PAGE_FULL = 1 };

/* A bit over the page mode: draw without clearing the window first, as for a
   window just opened. */
#define STATUS_PAGE_NO_CLEAR  (1 << 8)
#define STATUS_PAGE_MODE_MASK (STATUS_PAGE_NO_CLEAR - 1)

/* The longest status help text, in characters. */
#define STATUS_HELP_TEXT_MAX JAPANESE_OR(64, 128)

/* The stat column of the status page's top rows: three digits from 200 or four
   from 192, both ending at pixel 224. */
#define STATUS_STAT_DIGITS JAPANESE_OR(4, 3)
#define STATUS_STAT_X	   JAPANESE_OR(192, 200)

/* Items to a page of the equipment page's list. */
#define STATUS_EQUIP_PAGE_SIZE JAPANESE_OR(6, 5)

/* The hand cursor on the Psynergy page. */
#define STATUS_PSYNERGY_CURSOR_X JAPANESE_OR(58, 55)
#define STATUS_PSYNERGY_CURSOR_Y JAPANESE_OR(52, 60)

/* The highlight over the equipment window's four slot rows. */
#define STATUS_EQUIP_SLOT_X		JAPANESE_OR(2, 1)
#define STATUS_EQUIP_SLOT_WIDTH JAPANESE_OR(9, 12)

/* The equipment page's item list: its page icons, the item names, and the
   Attack and Defense figures. */
#define STATUS_ITEM_PAGE_ICONS_X JAPANESE_OR(123, 119)
#define STATUS_ITEM_NAME_X		 JAPANESE_OR(136, 128)
#define STATUS_ITEM_STAT_X		 JAPANESE_OR(72, 80)

/* The x of "You have no items." in the equipment page's list, in pixels. */
#define STATUS_ITEM_EMPTY_MESSAGE_X GERMAN_OR(112, 120)

/* The equipment window, and the equipped items' names in it. */
#define STATUS_EQUIP_WINDOW_WIDTH JAPANESE_OR(13, 15)
#define STATUS_EQUIPPED_NAME_X	  JAPANESE_OR(16, 8)

/* The Djinn figures stand in a row 32 pixels apart, at y STATUS_FIGURE_ROW_Y,
   over the status page's Djinn counts. On the full status page those stand
   three tiles higher, and the figures at STATUS_FIGURE_FULL_PAGE_ROW_Y; the
   Psynergy page puts them above the screen. */
#define STATUS_FIGURE_X(index)		  (130 + (index) * 32)
#define STATUS_FIGURE_ROW_Y			  128
#define STATUS_FIGURE_FULL_PAGE_ROW_Y 104
#define STATUS_FIGURE_ABOVE_SCREEN_Y  -16

/* The Djinn list (mnStatScreenShowDjinnList): a column for each element, its
   Djinni figure standing over it. */
#define DJINN_LIST_COLUMN_WIDTH		 56
#define DJINN_LIST_FIGURE_X(element) (32 + (element) * DJINN_LIST_COLUMN_WIDTH)
#define DJINN_LIST_FIGURE_Y			 70
/* The window tile of the first element icon, Venus's; the four elements follow
   it in order. STATUS_ELEMENT_TILE is its map entry in palette 1, as
   TILEMAP_ENTRY_ICON gives it. */
#define ELEMENT_ICON_FIRST_TILE		 1
#define STATUS_ELEMENT_TILE			 (BG_TILE_PALETTE(1) | ELEMENT_ICON_FIRST_TILE)

/* The switch help's two lines, drawn above the status window's interior. */
#define SWITCH_HELP_Y -24

/* The status page's ailments, one a row of 16 pixels from y 40. */
#define STATUS_AILMENT_Y(row) ((row) * 16 + 40)

/* The status page's four element columns, 4 tiles apart from tile 15, where
   each element's icon stands; its figures end left of it. */
#define STATUS_ELEMENT_COLUMN(element) ((element) * 4 + 15)
#define STATUS_ELEMENT_X(element)	   (STATUS_ELEMENT_COLUMN(element) * 8)

/* Lets the player choose one party member, over the field (the Colosso scenes
   use it, through menu.s, and so does the debug test). */
Sint32 mnPartyMemberChooserOpen(void)
{
	STAT_SCREEN_STATE *menu = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	Sint32			   result;
	Sint32			   index;

	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	sySynch(1);
	mnInitState(0);

	menu->partyCnt = gmPartyListMembersStub(menu->memberIds);
	mnStatScreenInit(0, 3, 0, 7);
	menu->selectorWin = mnWindowCreateStub(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	for(index = 0; index < MEMBER_CHOOSER_MEMBER_COUNT; index++)
		menu->memberY[index] = MENU_MEMBER_Y;
	menu->page = STATUS_SCREEN_PAGE_CHOOSER;

	result = mnPartyMemberChooserRun();

	mnWindowSprClearListStub(menu->statWin);
	mnItemMenuClose();
	ActorSystemStateWork->suspended = 0;
	sySynch(1);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	return result;
}

/* The party member chooser's loop. Returns the chosen party member's id, or
   -1. */
Sint32 mnPartyMemberChooserRun(void)
{
	STAT_SCREEN_STATE *state = MenuState;
	Sint32			   result = 0;

	do {
		state->selectedSlots[0] = 0;
		if(mnStatScreenSelectMember(0) == -1) {
			result = -1;
			break;
		}
		result = state->selectedIds[0];
	} while(0);
	return result;
}

/* The Status screen, the field menu's Status (mnFieldMenuRun,
   FIELD_MENU_STATUS), over the field: saves the first two background palettes
   and the window tiles it draws over, sets up the party row and, when the party
   has Djinn, the four Djinn sprites, runs the status pages, then puts
   everything back. */
Sint32 mnStatScreenRun(void)
{
	STAT_SCREEN_STATE *menu = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	void			  *palette = syHeapAllocEWRAM(64);
	void			  *tiles = syHeapAllocEWRAM(STATUS_SAVED_TILE_BYTES);
	Sint32			   result;
	Sint32			   index;

	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	sySynch(1);
	mnSuspendFieldEvents();
	mnInitState(0);
	menu->page = STATUS_SCREEN_PAGE_STATUS;
	menu->partyCnt = gmPartyListMembersStub(menu->memberIds);
	mnStatScreenInit(0, 3, 0, 7);
	syWordsCopy(palette, (void *)BG_PLTT, 64);
#if !STATUS_MENU_EXTRA_TEXT_TILES
	syWordsCopy(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
#endif
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	/* The first sprite palette into background palettes 0 and 1, for the
	   element icons drawn as window tiles: palette 0, for the highlighted row,
	   takes HIGHLIGHT_PALETTE's WINDOW_BACKGROUND_COLOR as its
	   TEXT_BACKGROUND_COLOR, palette 1 WINDOW_PALETTE's
	   (mnStatScreenShowPsynergyRow). */
	syDMAXCopy(
		(void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy(
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
#if STATUS_MENU_EXTRA_TEXT_TILES
	syWordsCopy(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
	syWordsFill(BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES, PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif
	menu->selectorWin = mnWindowCreateStub(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	for(index = 0; index < STATUS_MEMBER_FIGURE_COUNT; index++)
		menu->memberY[index] = MENU_MEMBER_Y;
	if(gmPartySumDjinnCountsStub(-1) != 0)
		mnDjinnFiguresInitTopRow(menu->selectorWin, 0);
	for(index = 0; index < 4; index++) {
		menu->rowX[index] = STATUS_FIGURE_X(index);
		menu->rowY[index] = STATUS_FIGURE_ROW_Y;
	}
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	/* Run the pages. */
	mnStartWatchingStart();
	menu->page = STATUS_SCREEN_PAGE_STATUS;

	result = mnStatScreenRunPages();

	/* Close, and put the palettes and tiles back. */
	mnStopWatchingStart();
	mnWindowSprClearListStub(menu->statWin);
	mnDjinnFiguresReleaseTopRow();
	mnResumeFieldEvents();
	mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	sySynch(1);
#if STATUS_MENU_EXTRA_TEXT_TILES
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	sySynch(1);
#endif
	syWordsCopy((void *)BG_PLTT, palette, 64);
	syWordsCopy(BG_CHAR_BLOCK(1), tiles, STATUS_SAVED_TILE_BYTES);
	syHeapRelease(tiles);
	syHeapRelease(palette);
	WINDOW_STATE->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	ActorSystemStateWork->suspended = 0;
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	WINDOW_STATE->holdTilemapUpload = 0;
	return result;
}

/* The steps of the Status menu (mnStatScreenRunPages). */
typedef enum STAT_SCREEN_STEP {
	STATUS_STEP_MEMBER = 0,	  /* choose a party member */
	STATUS_STEP_STATUS = 1,	  /* their status page */
	STATUS_STEP_PSYNERGY = 2, /* their Psynergy page */
	STATUS_STEP_EQUIPMENT = 3 /* their equipment page */
} STAT_SCREEN_STEP;

/* The Status menu's pages: choose a party member, then their status page, A for
   their Psynergy page and A again for their equipment page, whose A goes back
   round to the status page; B on any page goes back to the party member row. */
Sint32 mnStatScreenRunPages(void)
{
	STAT_SCREEN_STATE *menu = MenuState;
	Sint32			   step = STATUS_STEP_MEMBER;
	Sint32			   finished = step;
	Sint32			   result = 0;

	while(!finished && !gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		switch(step) {
		case STATUS_STEP_MEMBER:
			menu->selectedSlots[0] = finished;
			if(mnStatScreenSelectMember(0) == -1) {
				result = -1;
				finished = 1;
			}
			step = STATUS_STEP_STATUS;
			break;
		case STATUS_STEP_STATUS:
			menu->memberCursors[0]->motion = ICON_HIDDEN;
			result = mnStatScreenRunStatPage();
			step = result == -1 ? STATUS_STEP_MEMBER : STATUS_STEP_PSYNERGY;
			break;
		case STATUS_STEP_PSYNERGY:
			menu->memberCursors[0]->motion = ICON_HIDDEN;
			result = mnStatScreenRunPsynergyPage();
			step = STATUS_STEP_MEMBER;
			if(result != -1)
				step = STATUS_STEP_EQUIPMENT;
			break;
		case STATUS_STEP_EQUIPMENT:
			menu->memberCursors[0]->motion = ICON_HIDDEN;
			result = mnStatScreenRunEquipmentPage();
			if(result == -1)
				step = STATUS_STEP_MEMBER;
			else
				step = STATUS_STEP_STATUS;
			break;
		default:
			finished = 1;
			break;
		}
	}

	if(gmFlagGetStub(FLAG_MENU_START_PRESSED))
		result = -1;

	return result;
}

/* Lets the player choose a party member in slot, from the one chosen last: the
   party member chooser on STATUS_SCREEN_PAGE_CHOOSER, else the Status menu's
   party row. */
Sint32 mnStatScreenSelectMember(Sint32 slot)
{
	STAT_SCREEN_STATE *menu;
	WINDOW_SPR		  *cursor;
	Sint32			   result;
	Sint32			   index;

	menu = MenuState;
	index = menu->memberIdx[slot];
	result = 0;
	cursor = menu->memberCursors[slot];
	cursor->motion = ICON_SHOWN;
	cursor->anmStep = 0;
	mnWindowSprRedrawSavedRectStub(menu->memberWin);
	/* FLAG_LINK_BATTLE_ARRANGED is set while a link battle is arranged in the
	   Battle Arena, in which only the first three party members fight
	   (scBattleArenaPartyContains); the divider falls between the third and
	   fourth figures, which stand three tiles apart. */
	if(gmFlagGetStub(FLAG_LINK_BATTLE_ARRANGED))
		mnWindowDrawDividerLineStub(menu->memberWin, 9, 1, 9, 3);
	if(index == -1)
		menu->memberIdx[slot] = 0;
	else
		mnCursorSlide(MENU_MEMBER_CURSOR_X(index), 16);
	if(menu->page == STATUS_SCREEN_PAGE_CHOOSER)
		result = mnPartyMemberChooserChooseMember();
	else
		result = mnStatScreenRunPartyRow();
	cursor = menu->memberCursors[slot];
	mnIconShow(cursor);
	sySynch(1);
	return result;
}

/* Shows every Djinn the party has found, a column of seven for each element,
   until A, B or Select, then puts the screen back. */
Sint32 mnStatScreenShowDjinnList(void)
{
	DJINN_MENU_STATE *menu;
	WINDOW			 *window;
	Sint32			  element;
	Sint32			  djinn;

	menu = MenuState;
	mnStatScreenHideStatIcons();
	sySndPlayStub(SE_MENU_CONFIRM);
	mnWindowSprRedrawSavedRectStub(menu->helpWin);
	mnTextDrawMessageAsTilesStub(MsgReturnHelp, menu->helpWin, 0, 16);
	menu->cursor->motion = ICON_HIDDEN;
	menu->secondCursor->motion = ICON_HIDDEN;
	sySynch(1);
	window = menu->win;
	{
		Sint32 i;

		for(i = 0; i < 4; i++) {
			menu->slotX[i] = DJINN_LIST_FIGURE_X(i);
			menu->slotY[i] = DJINN_LIST_FIGURE_Y;
			menu->memberY[i] = MENU_MEMBER_Y;
		}
	}
	mnWindowSprRedrawSavedRectStub(window);
	mnWindowDrawDividerLineStub(window, 0, 11, 28, 11);
	mnTextDrawMessageStub(MsgCurrentDjinn, menu->helpWin, -96, 132);
	for(element = 0; element < ELEMENT_COUNT; element++) {
		for(djinn = 0; djinn < 7; djinn++) {
			Sint32 id = element * DJINN_PER_ELEMENT + djinn;

			/* FLAG_DJINN_FIRST on: one per Djinni found, DJINN_PER_ELEMENT to
			   an element. */
			if(gmFlagGetStub(FLAG_DJINNI(id))) {
				mnWindowSetTilemapEntryStub(window,
					STATUS_ELEMENT_TILE + element,
					element * (DJINN_LIST_COLUMN_WIDTH / 8) + 1, djinn + 3,
					TILEMAP_ENTRY_PLAIN);
				mnTextDrawMessageAsTilesStub(MsgDjinnName + id, window,
					element * DJINN_LIST_COLUMN_WIDTH + 16, djinn * 8 + 24);
			}
		}
	}
	WINDOW_STATE->dirty = 1;
	for(;;) {
		if(gmFlagGetStub(FLAG_MENU_START_PRESSED))
			break;
		sySynch(1);
		if(PerPush & (KEY_A | KEY_B | KEY_SELECT))
			break;
	}
	mnWindowSprRedrawSavedRectStub(menu->win);
	mnWindowSprClearListStub(menu->helpWin);
	{
		Sint32 i;

		for(i = 0; i < 4; i++) {
			menu->slotX[i] = STATUS_FIGURE_X(i);
			menu->slotY[i] = STATUS_FIGURE_ROW_Y;
		}
	}
	menu->cursor->motion = ICON_SHOWN;
	menu->secondCursor->motion = ICON_SHOWN;
	sySndPlayStub(SE_MENU_CANCEL);
	/* UB: returns no value. */
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

struct ITEM_MENU_STATE;

/* Lowers the party figures, then raises the currently selected party member. */
static __inline__ void mnStatScreenSetSelectedMemberRow(
	STAT_SCREEN_STATE *menu, Sint32 selectedMember)
{
	Sint32 rowIndex;

	for(rowIndex = 0; rowIndex < STATUS_MEMBER_FIGURE_COUNT; rowIndex++)
		menu->memberY[rowIndex] = MENU_MEMBER_Y;
	menu->memberY[selectedMember] = MENU_MEMBER_RAISED_Y;
}

/* The Status menu's party row: Left and Right choose a party member, showing
   their status; L and R move the party member one place along the party order;
   Select lists the Djinn found (once the first is caught, FLAG_DJINN_FIRST); A
   chooses the party member and B backs out. */
Sint32 mnStatScreenRunPartyRow(void)
{
	STAT_SCREEN_STATE *menu;
	Sint32			   memberIndex;
	Sint32			   memberCount;
	Sint32			   page;
	Bool			   redraw;
	Sint32			   result;
	Sint32			   rowIndex;

	menu = MenuState;
	memberIndex = menu->memberIdx[0];
	memberCount = menu->memberCnt[0];
	redraw = TRUE;
	result = 0;
	page = menu->page;
	gmUnitGetStub(menu->memberIds[memberIndex]);
	for(rowIndex = 0; rowIndex < 4; rowIndex++) {
		menu->rowX[rowIndex] = STATUS_FIGURE_X(rowIndex);
		menu->rowY[rowIndex] = STATUS_FIGURE_ROW_Y;
	}
	/* The highlight palette, and the first sprite palette into background
	   palettes 0 and 1, as mnStatScreenRun sets them. */
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	syDMAXCopy(
		(void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy(
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			redraw = FALSE;
			/* The help lines, and the party member's status and figure. */
			mnWindowSprRedrawSavedRectStub(menu->selectorWin);
			mnTextDrawMessageAsTilesStub(
				MsgRearrangeHelp, menu->selectorWin, 0, 0);
			if(gmFlagGetStub(FLAG_DJINN_FIRST))
				mnTextDrawMessageAsTilesStub(
					MsgDjinnListHelp, menu->selectorWin, 0, 16);
			/* "A: Details" */
			mnTextDrawMessageAsTilesStub(
				MsgRearrangeHelp - 3, menu->selectorWin, 0, 8);
			memberIndex = (memberIndex + memberCount) % memberCount;
			gmUnitGetStub(menu->memberIds[memberIndex]);
			page = (page + 3) % 3;
			mnStatScreenShowMemberStat(menu->memberIds[memberIndex], page);
			mnReloadMemberPortrait(menu, menu->memberIds[memberIndex]);
			mnStatScreenSetSelectedMemberRow(menu, memberIndex);
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(memberIndex), 16);
		sySynch(1);
		if(PerPush & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		/* L and R move the party member along the party order. */
		if(PerRepeat & KEY_R) {
			if(mnStatScreenMovePartyMember(memberIndex, 1)) {
				sySndPlayStub(SE_MENU_CONFIRM);
				memberIndex++;
				mnReleasePartyFigures();
				mnCreatePartyFigures(menu->memberWin, 2, 2, 8, 0);
				mnStatScreenSetSelectedMemberRow(menu, memberIndex);
			} else {
				sySndPlayStub(SE_MENU_ERROR);
			}
			sySynch(1);
		} else if(PerRepeat & KEY_L) {
			if(mnStatScreenMovePartyMember(memberIndex, 0)) {
				sySndPlayStub(SE_MENU_CONFIRM);
				memberIndex--;
				mnReleasePartyFigures();
				mnCreatePartyFigures(menu->memberWin, 2, 2, 8, 0);
				mnStatScreenSetSelectedMemberRow(menu, memberIndex);
			} else {
				sySndPlayStub(SE_MENU_ERROR);
			}
			sySynch(1);
		} else if((PerPush & KEY_SELECT) && gmFlagGetStub(FLAG_DJINN_FIRST)) {
			mnStatScreenShowDjinnList();
			redraw = TRUE;
		} else {
			if(PerRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				if(memberCount > 1) {
					memberIndex--;
					redraw = TRUE;
				}
			}
			if(PerRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				if(memberCount > 1) {
					redraw = TRUE;
					memberIndex++;
				}
			}
		}
	}
	menu->memberIdx[0] = memberIndex;
	menu->selectedMember = menu->memberIds[memberIndex];
	menu->selectedIds[0] = menu->memberIds[memberIndex];
	return result;
}

/* The party member chooser (STATUS_SCREEN_PAGE_CHOOSER): Left and Right choose
   a party member, showing their status and Psynergy icons; A chooses and B
   backs out. */
Sint32 mnPartyMemberChooserChooseMember(void)
{
	PSYNERGY_MENU_STATE *menu = MenuState;
	Sint32				 memberIndex = menu->tabIdx[0];
	Sint32				 memberCount = menu->tabCnts[0];
	Bool				 redraw = TRUE;
	Sint32				 page = menu->flg;
	Sint32				 result;
	Sint32				 rowIndex;

	gmUnitGetStub(menu->memberTbl[memberIndex]);
	mnWindowSprRedrawSavedRectStub(menu->msgWin);
	mnTextDrawMessageAsTilesStub(MsgChooseCharacter, menu->msgWin, 0, 0);
	mnTextDrawMessageAsTilesStub(MsgChooseCharacter + 1, menu->msgWin, 0, 16);
	for(;;) {
		if(redraw) {
			redraw = FALSE;
			memberIndex = (memberIndex + memberCount) % memberCount;
			gmUnitGetStub(menu->memberTbl[memberIndex]);
			page = (page + 3) % 3;
			mnStatScreenShowMemberStat(menu->memberTbl[memberIndex], page);
			mnReloadMemberPortrait(menu, menu->memberTbl[memberIndex]);
			for(rowIndex = 0; rowIndex < STATUS_MEMBER_FIGURE_COUNT; rowIndex++)
				menu->rowPositions[rowIndex] = MENU_MEMBER_Y;
			menu->rowPositions[memberIndex] = MENU_MEMBER_RAISED_Y;
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				gmUnitGetStub(menu->memberTbl[memberIndex]),
				(Uint16 *)menu->psynergies, PSYNERGY_LIST_ALL);
			mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
			mnPlaceEntryIconsInGrid(96, 96, 8);
			mnHideEmptyEntryIcons(menu->psynergies);
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(memberIndex), 16);
		sySynch(1);
		if(PerPush & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(memberCount > 1) {
				memberIndex--;
				redraw = TRUE;
			}
		}
		if(PerRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(memberCount > 1) {
				memberIndex++;
				redraw = TRUE;
			}
		}
	}
	menu->tabIdx[0] = memberIndex;
	menu->selectedMember = menu->memberTbl[memberIndex];
	menu->memberIds[0] = menu->memberTbl[memberIndex];
	return result;
}

/* Moves the party member at selectedIndex one place later (direction 1) or
   earlier (0) in the party order. Returns 1, or 0 when it cannot move. */
Bool mnStatScreenMovePartyMember(Sint32 selectedIndex, Sint32 direction)
{
	STAT_SCREEN_STATE *menu = MenuState;
	Uint32			   reordered[14];
	Sint32			   index;

	if(menu->partyCnt <= 1) {
		return FALSE;
	}
	if(direction == 1) {
		if(selectedIndex == menu->partyCnt - 1) {
			return FALSE;
		}
	} else {
		if(selectedIndex == 0) {
			return FALSE;
		}
	}

	for(index = 0; index < (Sint32)ARRAY_COUNT(reordered); index++) {
		reordered[index] = 0;
	}
	for(index = 0; index < menu->partyCnt; index++) {
		reordered[index] = menu->memberIds[index];
	}

	if(direction == 1) {
		index = reordered[selectedIndex];
		reordered[selectedIndex] = reordered[selectedIndex + 1];
		reordered[selectedIndex + 1] = index;
	} else {
		index = reordered[selectedIndex];
		reordered[selectedIndex] = reordered[selectedIndex - 1];
		reordered[selectedIndex - 1] = index;
	}

	/* Rebuild the party in the new order. */
	for(index = 0; index < menu->partyCnt; index++) {
		gmPartyRemoveMemberStub(menu->memberIds[index]);
	}
	for(index = 0; index < menu->partyCnt; index++) {
		gmPartyAddMemberStub(reordered[index]);
	}
	menu->partyCnt = gmPartyListMembersStub(menu->memberIds);
	return TRUE;
}

/* Sets up the Status menu's first screen: the cursor window with the party's
   figures, no other windows yet, and an 8 by 2 cursor grid. */
void mnStatScreenInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns)
{
	STAT_SCREEN_STATE *menu;

	menu = MenuState;
	mnCreatePartyFigures(mnCursorCreate(menu), 2, 2, 8, 0);
	menu->unopenedWin = 0;
	menu->statWin = 0;
	menu->helpWin = 0;
	menu->actionWin = 0;
	menu->selectedColumn = 0;
	menu->selectedRow = 0;
	menu->columnCnt = 8;
	menu->rowCnt = 2;
}

/* Shows `member`'s status in the status window (0, 5, 30 by 15), opening it
   first with the party member's portrait (kept as the state's statCursor), the
   32 entry icons (the chooser's Psynergy icons) on the chooser page, and the
   eight status icon sprites; then draws the stats and statuses. frame is not
   used. */
void mnStatScreenShowMemberStat(Sint32 member, Sint32 frame)
{
	Sint32			   created;
	WINDOW			  *window;
	WINDOW_SPR		  *cursor;
	STAT_SCREEN_STATE *menu;

	menu = MenuState;
	created = 0;
	window = menu->statWin;
	if(window == 0) {
		created = mnWindowUpdateOrCreate(
			&menu->statWin, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
		window = menu->statWin;
	}
	if(created != 0) {
		cursor = mnPortraitCreateStub(member, 0, 0, window, 0, 0);
		menu->statCursor = cursor;
		/* The portrait goes to depth slot 240, as the status icons do. */
		if((cursor->depth = OAM_DEPTH_WINDOW, menu->page) ==
			STATUS_SCREEN_PAGE_CHOOSER) {
			mnSpawnIconEntries((ITEM_MENU_STATE *)menu, window);
		}
		mnStatScreenCreateStatIcons(window);
		mnStatScreenDrawStatPage(window, member, STATUS_PAGE_NO_CLEAR);
		return;
	}
	mnStatScreenDrawStatPage(window, member, STATUS_PAGE_SUMMARY);
}

/* The party member's status page. */
Sint32 mnStatScreenRunStatPage(void)
{
	STAT_SCREEN_STATE *menu;
	Sint32			   side;
	Sint32			   result;
	Sint32			   selected;
	Sint8			   count;
	Bool			   hasAilments;
	Bool			   done;
	Bool			   hasDjinn;
	Bool			   redraw;
	Sint32			   member;
	Sint32			   djinnTotal;
	Sint32			   i;
	Uint8			   entries[8];

	menu = MenuState;
	side = 0;
	result = 0;
	selected = 0;
	djinnTotal = gmPartySumDjinnCountsStub(-1);
	hasDjinn = FALSE;
	if(djinnTotal != 0)
		hasDjinn = TRUE;
	/* The help window covers the top of the screen. */
	mnWindowUpdateOrCreate(&menu->helpWin, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
	syCloseEvent(mnPlacePartyFigures);
	for(i = 0; i < 4; i++)
		menu->rowY[i] = STATUS_FIGURE_FULL_PAGE_ROW_Y;
	done = FALSE;
	mnCursorSlide(-10, 88);

	/* One party member's page at a time; L and R start the next. */
	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		gmUnitGetStub(menu->selectedIds[0]);
		mnStatScreenDrawStatPage(
			menu->statWin, menu->selectedIds[0], STATUS_PAGE_FULL);
		count = mnStatScreenListStatuses(entries, 1, menu->selectedIds[0]);
		hasAilments = FALSE;
		if(count == 0)
			count = 1;
		else
			hasAilments = TRUE;
		redraw = TRUE;

		while(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
			if(redraw != 0) {
				Sint32 limit;

				redraw = FALSE;
				limit = count;
				side = (side + 2) % 2;
				if(side == 0) {
					selected = (selected + limit) % limit;
					mnWindowSprRedrawSavedRectStub(menu->helpWin);
					if(hasAilments == 0) {
						mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp,
							menu->statWin, SWITCH_HELP_X, SWITCH_HELP_Y);
						mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp + 1,
							menu->statWin, 0, SWITCH_HELP_Y);
					}
				} else {
					/* The right side has eight entries, seven without the Djinn
					   row. */
					mnWindowSprRedrawSavedRectStub(menu->helpWin);
					if(hasDjinn != 0)
						selected = (selected + 8) % 8;
					else
						selected = (selected + 7) % 7;
				}
				mnStatScreenDrawSelectionCursor(side, selected, entries, 0);
				mnWindowSprClearListStub(menu->helpWin);
				sySynch(1);
				if(side == 0)
					mnStatScreenDrawSelectionLabels(
						menu->helpWin, selected, entries);
				else
					mnStatScreenShowMemberProgressMessage(
						menu->helpWin, selected, hasDjinn);
			}
			/* The statuses are 16 pixels apart; the right side's first four
			   rows (name to PP) are 8 apart, the rows below them 8 apart from y
			   112. */
			menu->memberCursors[0]->motion = ICON_SHOWN;
			if(side == 0)
				mnCursorPos(-10, selected * 16 + 88);
			else if(selected <= 3)
				mnCursorPos(24, selected * 8 + 48);
			else
				mnCursorPos(48, selected * 8 + 80);
			sySynch(1);
			/* Take the highlight off before the cursor moves. */
			if(PerRepeat & KEYS_DPAD)
				mnStatScreenDrawSelectionCursor(side, selected, entries, 1);
			if(PerPush & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				done = TRUE;
				result = 1;
				break;
			}
			if(PerPush & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				done = TRUE;
				result = -1;
				break;
			}
			if(PerRepeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				selected--;
			}
			if(PerRepeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				selected++;
			}
			if(PerRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				side++;
			}
			if(PerRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				side--;
			}
			/* L and R: the previous or next party member. */
			if((PerRepeat & KEY_R) || (PerRepeat & KEY_L)) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				member = menu->memberIdx[0];
				if(PerRepeat & KEY_R)
					member++;
				else
					member--;
				member = (member + menu->partyCnt) % menu->partyCnt;
				menu->selectedMember = menu->memberIds[member];
				menu->selectedIds[0] = menu->memberIds[member];
				menu->memberIdx[0] = member;
				mnReloadMemberPortrait(menu, menu->memberIds[member]);
				break;
			}
		}
	}
	mnWindowSprClearListStub(menu->helpWin);
	mnWindowSprRedrawSavedRectStub(menu->helpWin);
	mnWindowClearInteriorTilesStub(menu->statWin, 64, 56, 224, 96);
	sySetEvent(mnPlacePartyFigures, EVENT_PASS_FRAME);
	for(i = 0; i < 4; i++)
		menu->rowY[i] = STATUS_FIGURE_ROW_Y;
	mnStatScreenHideStatIcons();
	return result;
}

/* Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
   status page: on side 0 the selected status row, as wide as its label
   (StatusScreen_CursorWidths); on side 1 one of the first four stat rows or of
   the rows below. */
void mnStatScreenDrawSelectionCursor(
	Sint32 side, Sint32 selected, Uint8 *entries, Sint32 invert)
{
	STAT_SCREEN_STATE *menu = MenuState;
	Sint32			   count;
	Sint32			   index;
	Sint32			   x;
	Sint32			   y;
	Sint32			   palette;
	Uint8			   width;

	if(side == 0) {
		y = selected * 2 + 5;
		x = 0;
		width = 5;
		count = 0;
		for(index = 0; index <= 4; index++) {
			if(entries[index] != 0) {
				if(selected == count) {
					width = StatusScreen_CursorWidths[index];
					break;
				}
				count++;
			}
		}
	} else if(selected <= 3) {
		y = selected;
		x = 5;
		width = 13;
	} else {
		y = selected + 4;
		x = 8;
		width = 20;
	}

	if(invert == 1)
		palette = WINDOW_PALETTE;
	else
		palette = HIGHLIGHT_PALETTE;
	mnWindowSetHighlightRect(menu->statWin, x, y, width, 1, palette);
}

/* Writes the advice for the selected status (two lines, for example how to
   revive a party member who is down) into window target, or that the party
   member is well when they have no status. */
void mnStatScreenDrawSelectionLabels(
	WINDOW *target, Sint32 selected, const Uint8 *entries)
{
	Sint32 labelCount = 0;
	Sint32 labelIndex;

	for(labelIndex = 0; labelIndex <= 4; labelIndex++) {
		if(entries[labelIndex] != 0) {
			if(selected == labelCount) {
				Sint32 messageId = MsgStatusAdvice + labelIndex * 2;
				mnTextDrawMessageStub(messageId, target, 0, -1);
				mnTextDrawMessageStub(messageId + 1, target, 0, 15);
			}
			labelCount++;
		}
	}

	if(labelCount == 0)
		mnTextDrawMessageStub(MsgStatusNormal, target, 0, 0);
}

/* The experience still needed is worked out in 64 bits outside Japanese; the
   message argument takes 32 bits either way. */
#if EXPERIENCE_NEEDED_64_BIT
typedef Uint64 EXPERIENCE_DIFFERENCE;
#else
typedef Uint32 EXPERIENCE_DIFFERENCE;
#endif

/* Writes the help text of the status page's right-hand row `row` into
   destination: the rows skip the Djinn row when the party has no Djinn
   (hasDjinn 0); the experience row says how many points the next level needs,
   or, at level 99, gives the plain experience text. */
void mnStatScreenShowMemberProgressMessage(
	WINDOW *destination, Sint32 row, Sint32 hasDjinn)
{
	STAT_SCREEN_STATE *menu = MenuState;
	void			  *buffer;

	/* Entry 4 is the Djinn row's help: without Djinn the rows below it come one
	   entry earlier. */
	if(hasDjinn == 0 && row > 3) {
		row++;
	}
	if(row == 1) {
		UNIT *member = gmUnitGetStub(menu->selectedIds[0]);

		if(member->level == UNIT_LEVEL_MAX) {
			row = 8; /* "These are your experience points." */
		} else {
			EXPERIENCE_DIFFERENCE remaining =
				gmUnitGetExperienceForLevelStub(
					menu->selectedIds[0], member->level + 1) -
				member->experience;
			mnMessageArgumentPushStub(remaining, MESSAGE_ARGUMENT_NUMBER);
		}
	}

	buffer = syHeapAlloc(STATUS_HELP_TEXT_MAX * 2);
	mnMessageCopyExpandedStub(
		row + MsgProgressHelp, buffer, STATUS_HELP_TEXT_MAX);
	mnTextDrawWideStringAtOffsetStub(buffer, destination, 0, -1);
	syHeapRelease(buffer);
}

/* Draws party member memberId's status page into window: the top rows
   (mnStatScreenDrawTopRows), the status list or "Normal", and except on the
   chooser page the Djinn counts by element and, on STATUS_PAGE_FULL, the
   elemental levels, Power and Resist. */
void mnStatScreenDrawStatPage(WINDOW *window, Sint32 memberId, Sint32 mode)
{
	STAT_SCREEN_STATE *menu = MenuState;
	UNIT			  *unit;
	Sint32			   djinnRow;
	Sint32			   hasDjinn;
	Sint32			   keepDrawn;
	Sint32			   i;
	Uint8			   ailments[8];

	if(gmPartySumDjinnCountsStub(-1) != 0)
		hasDjinn = 1;
	else
		hasDjinn = 0;
	unit = gmUnitGetStub(memberId);
	/* The Djinn row, in tiles: higher on STATUS_PAGE_FULL, which has the
	   elemental rows below it. */
	if((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL)
		djinnRow = 7;
	else
		djinnRow = 10;
	menu->statCursor->motion = ICON_SHOWN;
	mnStatScreenDrawTopRows(window, memberId, mode);
	mnStatScreenListStatuses(ailments, 1, memberId);
	mnStatScreenLoadStatIcons(ailments);
	keepDrawn = mode & STATUS_PAGE_NO_CLEAR;
	if(keepDrawn == 0)
		mnWindowClearInteriorTilesStub(window, 0, 40, 96, 96);
	/* The statuses, one a row, or "Normal". */
	i = 0;
	if(ailments[CHARACTER_DOWN] != 0) {
		mnTextDrawMessageAsTilesStub(
			MsgDownLabel, window, 16, STATUS_AILMENT_Y(0));
		i = 1;
	}
	if(ailments[CHARACTER_POISON] != 0) {
		mnTextDrawMessageAsTilesStub(
			MsgPoisonLabel, window, 16, STATUS_AILMENT_Y(i));
		i++;
	}
	if(ailments[CHARACTER_VENOM] != 0) {
		mnTextDrawMessageAsTilesStub(
			MsgVenomLabel, window, 16, STATUS_AILMENT_Y(i));
		i++;
	}
	if(ailments[CHARACTER_CURSE] != 0) {
		mnTextDrawMessageAsTilesStub(
			MsgCurseLabel, window, 16, STATUS_AILMENT_Y(i));
		i++;
	}
	if(ailments[CHARACTER_HAUNT] != 0) {
		mnTextDrawMessageAsTilesStub(
			MsgHauntLabel, window, 16, STATUS_AILMENT_Y(i));
		i++;
	}
	if(i == 0)
		mnTextDrawMessageAsTilesStub(
			MsgNormalLabel, window, 0, STATUS_AILMENT_Y(0));
	mnStatScreenLoadStatIcons(ailments);
	mnStatScreenShowStatIcons(ailments);
	if(menu->page == STATUS_SCREEN_PAGE_CHOOSER)
		return;
	if(keepDrawn == 0) {
		sySynch(1);
		mnWindowClearInteriorTilesStub(window, 64, 56, 224, 96);
	}
	/* The four element icons, then the Djinn counts and, on STATUS_PAGE_FULL,
	   the elemental levels, Power and Resist. */
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(mode == STATUS_PAGE_FULL || hasDjinn == 1) {
		mnWindowSetTilemapEntryStub(window,
			ELEMENT_ICON_FIRST_TILE + ELEMENT_VENUS,
			STATUS_ELEMENT_COLUMN(ELEMENT_VENUS), djinnRow, TILEMAP_ENTRY_ICON);
		mnWindowSetTilemapEntryStub(window,
			ELEMENT_ICON_FIRST_TILE + ELEMENT_MERCURY,
			STATUS_ELEMENT_COLUMN(ELEMENT_MERCURY), djinnRow,
			TILEMAP_ENTRY_ICON);
		mnWindowSetTilemapEntryStub(window,
			ELEMENT_ICON_FIRST_TILE + ELEMENT_MARS,
			STATUS_ELEMENT_COLUMN(ELEMENT_MARS), djinnRow, TILEMAP_ENTRY_ICON);
		mnWindowSetTilemapEntryStub(window,
			ELEMENT_ICON_FIRST_TILE + ELEMENT_JUPITER,
			STATUS_ELEMENT_COLUMN(ELEMENT_JUPITER), djinnRow,
			TILEMAP_ENTRY_ICON);
	}
	if(hasDjinn)
		mnTextDrawMessageAsTilesStub(
			MsgDjinnLabel, window, 64, (djinnRow + 1) * 8);
	if(mode == STATUS_PAGE_FULL) {
		if(!hasDjinn)
			djinnRow--;
		MENU_DRAW_STATUS_LABEL(Menu_LvString, window, 64, (djinnRow + 2) * 8);
		mnTextDrawMessageAsTilesStub(
			MsgPowerLabel, window, 64, (djinnRow + 3) * 8);
		mnTextDrawMessageAsTilesStub(
			MsgPowerLabel + 1, window, 64, (djinnRow + 4) * 8);
	}
	for(i = 0; i <= 3; i++) {
		if(hasDjinn)
			mnTextDrawNumInWindowStub(unit->djinnHeldCnts[i], 1, window,
				STATUS_ELEMENT_X(i), (djinnRow + 1) * 8);
		if((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL) {
			if(hasDjinn) {
				mnTextDrawNumInWindowStub(unit->djinnSetCnts[i], 1, window,
					STATUS_ELEMENT_X(i) - 16, (djinnRow + 1) * 8);
				mnTextDrawStringInWindowStub(Menu_SlashString, window,
					STATUS_ELEMENT_X(i) - 8, (djinnRow + 1) * 8);
			}
			mnTextDrawNumInWindowStub(gmUnitGetElementLevelStub(memberId, i), 2,
				window, STATUS_ELEMENT_X(i) - 8, (djinnRow + 2) * 8);
			mnTextDrawNumInWindowStub(unit->element[i].power, 3, window,
				STATUS_ELEMENT_X(i) - 16, (djinnRow + 3) * 8);
			mnTextDrawNumInWindowStub(unit->element[i].resistance, 3, window,
				STATUS_ELEMENT_X(i) - 16, (djinnRow + 4) * 8);
		}
	}
}

#if UNUSED_BUSY_WAIT
/* Spins for 256 loop passes (international editions only). Nothing calls it. */
void mnBusyWait(void)
{
	Sint16 count;

	for(count = 0; count < 256; count++) {
	}
}

#endif

/* Stat label `n` of "Attack", "Defense", "Agility" and "Luck" (MsgStatLabel +
   n), counted back from "Exp" (MsgExpLabel), which the status page also
   shows. */
#define STAT_LABEL(n) \
	(MsgExpLabel - (MSGNO(MsgExpLabel) - MSGNO(MsgStatLabel)) + (n))

/* Draws the top rows of party member memberId's status page into window: name,
   class, level, experience, HP and PP on the left, Attack, Defense, Agility and
   Luck on the right. */
void mnStatScreenDrawTopRows(WINDOW *window, Sint32 memberId, Sint32 flags)
{
	ITEM_MENU_STATE *menu;
	UNIT			*unit;

	menu = MenuState;
	unit = gmUnitGetStub(memberId);
	menu->cursor->motion = ICON_SHOWN;
	flags &= STATUS_PAGE_NO_CLEAR;
	if(flags == 0) {
		mnWindowClearInteriorTilesStub(window, 0, 0, 128, 40);
	}
	mnTextDrawStringAtOffsetStub(unit->name, window, 40, 0);
	mnTextDrawMessageAsTilesStub(unit->classId + MsgClassName, window, 0, 32);
	MENU_DRAW_STATUS_LABEL(Menu_LvString, window, 104, 0);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	mnTextDrawNumInWindowStub(unit->level, 2, window, 128, 0);
	MENU_DRAW_STATUS_LABEL(Menu_HpString, window, 40, 16);
	mnTextDrawNumInWindowStub(unit->hp, 4, window, 72, 16);
	mnTextDrawNumInWindowStub(unit->hpMax, 4, window, 112, 16);
	mnTextDrawStringInWindowStub(Menu_SlashString, window, 104, 16);
	MENU_DRAW_STATUS_LABEL(Menu_PpString, window, 40, 24);
	mnTextDrawNumInWindowStub(unit->pp, 4, window, 72, 24);
	mnTextDrawNumInWindowStub(unit->ppMax, 4, window, 112, 24);
	mnTextDrawStringInWindowStub(Menu_SlashString, window, 104, 24);
	mnTextDrawMessageAsTilesStub(MsgExpLabel, window, 40, 8);
	mnTextDrawNumInWindowStub(unit->experience, 7, window, 88, 8);
	if(flags == 0) {
		sySynch(1);
		mnWindowClearInteriorTilesStub(window, 144, 0, 224, 40);
	}
	/* "Attack", "Defense", "Agility" and "Luck". */
	mnTextDrawMessageAsTilesStub(STAT_LABEL(0), window, 152, 0);
	mnTextDrawMessageAsTilesStub(STAT_LABEL(1), window, 152, 8);
	mnTextDrawMessageAsTilesStub(STAT_LABEL(2), window, 152, 16);
	mnTextDrawMessageAsTilesStub(STAT_LABEL(3), window, 152, 24);
	mnTextDrawNumInWindowStub(
		unit->attack, STATUS_STAT_DIGITS, window, STATUS_STAT_X, 0);
	mnTextDrawNumInWindowStub(
		unit->defense, STATUS_STAT_DIGITS, window, STATUS_STAT_X, 8);
	mnTextDrawNumInWindowStub(
		unit->agility, STATUS_STAT_DIGITS, window, STATUS_STAT_X, 16);
	mnTextDrawNumInWindowStub(
		unit->luck, STATUS_STAT_DIGITS, window, STATUS_STAT_X, 24);
}

/* Fills ailments[0..4] (UNIT_STAT) with party member memberId's statuses: down
   (only when requested is 1), poison or venom, curse and haunt. */
Sint32 mnStatScreenListStatuses(
	Uint8 *ailments, Sint32 requested, Sint32 memberId)
{
	UNIT  *unit;
	Sint32 ailmentIndex;
	Sint32 count;
	Sint32 poison;

	unit = gmUnitGetStub(memberId);
	for(ailmentIndex = 0; ailmentIndex < CHARACTER_AILMENT_COUNT;
		ailmentIndex++)
		ailments[ailmentIndex] = 0;

	count = 0;
	if(unit->hp == 0 && requested == 1) {
		ailments[CHARACTER_DOWN] = requested;
		count = 1;
	}

	poison = unit->poison;
	if(poison != 0) {
		if(poison == 1) /* poison; any other value is venom */
			ailments[CHARACTER_POISON] = poison;
		else
			ailments[CHARACTER_VENOM] = 1;
		count++;
	}
	if(unit->curse != 0) {
		ailments[CHARACTER_CURSE] = 1;
		count++;
	}
	if(unit->haunt != 0) {
		ailments[CHARACTER_HAUNT] = 1;
		count++;
	}
	return count;
}

/* Fills result with the item list of the party member in side index for the
   equipment pages: their unit, the menu's item count, and the page and row of
   the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page. */
Sint32 mnPagedListBuild(PAGED_LIST *result, Sint32 index)
{
	Sint32			 unitBits;
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 itemCount;
	Sint32			 row;
	Sint32			 page;
	Sint32			 pageCount;
	Sint32			 selected;

	unitBits = (Sint32)gmUnitGetStub(menu->sideMember[index]);
	itemCount = menu->itemCnt;
	selected = menu->selectedIdxByMember[menu->sideMember[index]];
	if((selected + 1) > itemCount) {
		selected = itemCount - 1;
	}
	if(itemCount == 0) {
		selected = 0;
	}
	page = selected / STATUS_EQUIP_PAGE_SIZE;
	row = selected % STATUS_EQUIP_PAGE_SIZE;
	pageCount = itemCount / STATUS_EQUIP_PAGE_SIZE;
	if(itemCount % STATUS_EQUIP_PAGE_SIZE != 0) {
		pageCount++;
	}
	result->memberState = unitBits;
	result->page = page;
	result->pageCnt = pageCount;
	result->row = row;
	result->entryCnt = itemCount;
	result->selectedIdx = selected;
	return 1;
}

/* The Psynergy range icons' tiles: two sets of RANGE_ICON_PAIRS tile pairs, a
   pair RANGE_ICON_PAIR_WORDS of tile data, a word to a 4bpp tile row.
   RANGE_ICON_ALL, the last pair, stands for ABILITY_RANGE_ALL. */
enum { RANGE_ICON_PAIRS = 6, RANGE_ICON_PAIR_WORDS = 2 * TILE_SIZE_4BPP / 4 };

/* Builds the Psynergy range icons' tiles in the window tile area, each a fill
   of WINDOW_BACKGROUND_COLOR with the rows of Menu_PatternRowMasks XORed in,
   the first set growing row by row and the second skipping its top row. */
void mnBuildPatternTiles(void)
{
	Uint32 *vram = (Uint32 *)(BG_CHAR_BLOCK(0) +
		(RANGE_ICON_TILE & TILE_NUMBER_MASK) * TILE_SIZE_4BPP);
	Sint32	set;
	Sint32	tileIndex;

	for(set = 0; set < 2; set++) {
		for(tileIndex = 0; tileIndex < RANGE_ICON_PAIRS; tileIndex++) {
			Uint32 *tile = vram +
				set * RANGE_ICON_PAIRS * RANGE_ICON_PAIR_WORDS +
				tileIndex * RANGE_ICON_PAIR_WORDS;
			Sint32 rowIndex;

			syWordsFill(tile, 2 * TILE_SIZE_4BPP, TEXT_CANVAS_BACKGROUND_WORD);
			for(rowIndex = 1; rowIndex <= 7; rowIndex++) {
				Sint32 mask = tileIndex;

				if(set == 1 && rowIndex <= 1) {
					continue;
				}
				if(set == 0 && tileIndex > rowIndex - 2) {
					mask = rowIndex - 2;
					if(mask < 0) {
						mask = 0;
					}
				}
				tile[rowIndex] = mnXorWord(
					tile[rowIndex], Menu_PatternRowMasks[mask].firstTile);
				tile[rowIndex + 8] = mnXorWord(
					tile[rowIndex + 8], Menu_PatternRowMasks[mask].secondTile);
			}
		}
	}
}

/* Draws a Psynergy range icon at tile (x, y) of window: three tiles from pair
   range of the range icon tiles (mnBuildPatternTiles), the side tile on both
   ends, mirrored on the left, and the centre tile between. range is the
   ability's range less one (0 for the aimed unit alone), or RANGE_ICON_ALL for
   ABILITY_RANGE_ALL (mnStatScreenDrawPsynergyPage). */
void mnPsynergyMenuDrawRange(
	WINDOW *window, Sint32 x, Sint32 y, Sint32 range, Sint32 unused)
{
	Sint32 iconIndex;
	Sint32 tile;

	(void)unused;

	iconIndex = range * 2;
	tile = iconIndex + RANGE_ICON_TILE + 1;
	mnWindowSetTilemapEntryStub(
		window, TILE_HFLIP | tile, x, y, TILEMAP_ENTRY_PLAIN);
	mnWindowSetTilemapEntryStub(
		window, iconIndex + RANGE_ICON_TILE, x + 1, y, TILEMAP_ENTRY_PLAIN);
	mnWindowSetTilemapEntryStub(window, tile, x + 2, y, TILEMAP_ENTRY_PLAIN);
}

/* Updates the Psynergy page for the row under the cursor: the Psynergy's
   description in the info window, where it can be used (in the field, in
   battle, or anywhere), and the row highlights with the range icons. */
Sint32 mnStatScreenShowPsynergyRow(
	WINDOW *window, Sint32 unused, PAGED_LIST *state)
{
	PSYNERGY_MENU_STATE *menu;
	ABILITY_DEFINITION	*ability;
	Sint32				 row;
	Sint32				 base;

	menu = MenuState;
	state->selectedIdx = state->page * STATUS_LIST_PAGE_ROWS + state->row;
#if LANGUAGE != LANGUAGE_JAPANESE && !STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY
	mnWindowSprRedrawSavedRectStub(menu->infoWin);
#elif STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY
	mnWindowClearInteriorTilesStub(menu->infoWin, 0, 0, 224, 8);
#else
	mnWindowSprClearListStub(menu->infoWin);
#endif
	sySynch(1);
	if(menu->psynergies[state->selectedIdx] != 0) {
		MENU_DRAW_DESCRIPTION(
			(menu->psynergies[state->selectedIdx] & PSYNERGY_ABILITY_MASK) +
				MsgAbilityDescription,
			menu->infoWin, 0, 0);
		ability = gmAbilityGetDirectStub(
			menu->psynergies[state->selectedIdx] & PSYNERGY_ABILITY_MASK);
#if LANGUAGE != LANGUAGE_JAPANESE
		mnWindowClearInteriorTilesStub(window, 0, 96, 224, 104);
		/* row holds where it can be used meanwhile: 2 for the field, 1 for
		   battle. */
		row = 0;
		if(ability->fieldPsynergy != 0 ||
			(ability->flg & ABILITY_USABLE_IN_FIELD) != 0) {
			row = 2;
		}
		if((ability->flg & ABILITY_USABLE_IN_BATTLE) != 0) {
			row |= 1;
		}
		if(row == 3) {
			mnTextDrawMessageAsTilesStub(MsgUsableAnywhere, window, 0, 96);
		} else if(row == 2) {
			mnTextDrawMessageAsTilesStub(MsgUsableInField, window, 0, 96);
		} else if(row == 1) {
			mnTextDrawMessageAsTilesStub(MsgUsableInBattle, window, 0, 96);
		}
#else
		mnWindowClearInteriorTilesStub(window, 0, 72, 64, 96);
		row = 0;
		if(ability->fieldPsynergy != 0 ||
			(ability->flg & ABILITY_USABLE_IN_FIELD) != 0) {
			mnTextDrawMessageAsTilesStub(MsgUsableInField, window, 0, 72);
			row = 1;
		}
		if((ability->flg & ABILITY_USABLE_IN_BATTLE) != 0) {
			mnTextDrawMessageAsTilesStub(
				MsgUsableInBattle, window, 0, row * 8 + 72);
			row++;
		}
		mnTextDrawMessageAsTilesStub(MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
	}

	/* Highlight the cursor's row. */
	base = state->page * STATUS_LIST_PAGE_ROWS;
	for(row = 0; row <= STATUS_LIST_PAGE_ROWS - 1; row++) {
		if(row == state->row) {
			ability = gmAbilityGetDirectStub(
				menu->psynergies[base + row] & PSYNERGY_ABILITY_MASK);
			if(ability->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(window, ability->element + 1, 24,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW,
					TILEMAP_ENTRY_PLAIN);
				mnWindowSetHighlightRect(window, 9,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 15, 1,
					HIGHLIGHT_PALETTE);
				mnWindowSetHighlightRect(window, 25,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 3, 1,
					HIGHLIGHT_PALETTE);
			} else {
				mnWindowSetHighlightRect(window, 9,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 19, 1,
					HIGHLIGHT_PALETTE);
			}
		} else {
			ability = gmAbilityGetDirectStub(
				menu->psynergies[base + row] & PSYNERGY_ABILITY_MASK);
			if(ability->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(window, ability->element + 1, 24,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW,
					TILEMAP_ENTRY_ICON);
				mnWindowSetHighlightRect(window, 9,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 15, 1,
					WINDOW_PALETTE);
				mnWindowSetHighlightRect(window, 25,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 3, 1,
					WINDOW_PALETTE);
			} else {
				mnWindowSetHighlightRect(window, 9,
					row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 19, 1,
					WINDOW_PALETTE);
			}
		}
	}
	sySynch(1);
	return 1;
}

/* Draws one page of the Status menu's Psynergy list into window: up to
   STATUS_LIST_PAGE_ROWS names with their icons, PP costs and range icons, the
   page numbers or "no Psynergy", and the party member's name, class and
   level. */
Sint32 mnStatScreenDrawPsynergyPage(
	WINDOW *window, Sint32 unused, const PAGED_LIST *res)
{
	PSYNERGY_MENU_STATE *menu = MenuState;
	UNIT				*member;
	Uint32				 firstEntry;
	Uint32				 rows;
	Uint8				 row;
	Sint32				 entryIndex;

	(void)unused;

	member = gmUnitGetStub(menu->memberIds[0]);

	mnWindowSprRedrawSavedRectStub(window);

	firstEntry = res->page * STATUS_LIST_PAGE_ROWS;
	rows = (Uint8)(res->entryCnt - firstEntry);
	if(rows > STATUS_LIST_PAGE_ROWS) {
		rows = STATUS_LIST_PAGE_ROWS;
	}

	mnSetPageIcons(STATUS_LIST_PAGE_ROWS, firstEntry, window,
		STATUS_PSYNERGY_LIST_ICONS_X, STATUS_PSYNERGY_LIST_ICONS_Y);
	mnPagedListDrawPageNumbers(
		window, res->entryCnt, STATUS_LIST_PAGE_ROWS, res->page, 28);

	mnTextDrawMessageAsTilesStub(
		MsgPsynergyPp, window, STATUS_PSYNERGY_LIST_PP_X, 0);

	row = 0;
	if(rows > row) {
		entryIndex = firstEntry;
		do {
			ABILITY_DEFINITION *action;
			Sint32				messageId;
			Sint32				y;
			Sint32				range;
			action = gmAbilityGetDirectStub(
				PSYNERGY_ABILITY_MASK & menu->psynergies[entryIndex]);
			messageId = (menu->psynergies[entryIndex] & PSYNERGY_ABILITY_MASK) +
				MsgAbilityName;
			y = row * 16 + STATUS_PSYNERGY_LIST_FIRST_ROW * 8;

			mnTextDrawMessageAsTilesStub(
				messageId, window, STATUS_PSYNERGY_LIST_NAME_X, y);
			mnTextDrawNumAtOffsetStub(
				action->ppCost, 2, window, STATUS_PSYNERGY_LIST_PP_NUMBER_X, y);

			/* Range n takes range icon n - 1. */
			range = action->range;
			if(range == ABILITY_RANGE_ALL) {
				range = RANGE_ICON_ALL;
			} else {
				range--;
			}
			mnPsynergyMenuDrawRange(
				window, 25, row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, range, 0);

			row++;
			entryIndex++;
		} while(rows > row);
	}

	if(menu->psynergyCnt == 0) {
		mnTextDrawMessageAsTilesStub(
			MsgNoPsynergy, window, 96, STATUS_PSYNERGY_LIST_EMPTY_Y);
	}

	mnTextDrawStringAtOffsetStub(member->name, (void *)window, 40, 0);
	mnTextDrawMessageAsTilesStub(member->classId + MsgClassName, window, 0, 32);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
	mnTextDrawStringAtOffsetStub(Menu_LvString, (void *)window, 0, 48);
#else
	mnTextDrawStringInWindowStub(Menu_LvString, window, 0, 48);
#endif
	mnTextDrawNumInWindowStub(member->level, 2, window, 24, 48);

	return 1;
}

/* The Status menu's Psynergy page: the party member's Psynergy,
   STATUS_LIST_PAGE_ROWS to a page, each with its PP cost and range icon; the
   cursor's Psynergy is described in the info window with where it can be
   used. */
Sint32 mnStatScreenRunPsynergyPage(void)
{

	PSYNERGY_MENU_STATE *menu;
	Sint32				 result;
	Bool				 done;
	Bool				 redraw;
	Bool				 listNeedsRedraw;
	Sint32				 navigationResult;
	Sint32				 memberIndex;
	Sint32				 iconIndex;
	PAGED_LIST			 state;

	menu = MenuState;
	result = 0;
	done = FALSE;
	mnBuildPatternTiles();
	mnWindowSprRedrawSavedRectStub(menu->msgWin);
	mnWindowUpdateOrCreate(&menu->infoWin, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
	/* slotY is STAT_SCREEN_STATE's rowY: the four Djinn figures go above the
	   screen while this page is up, until the party member row or the status
	   page puts them back. */
	for(iconIndex = 0; iconIndex < 4; iconIndex++)
		menu->slotY[iconIndex] = STATUS_FIGURE_ABOVE_SCREEN_Y;
	{
		Sint32		 depth = OAM_DEPTH_WINDOW + 5;
		WINDOW_SPR **icons = menu->entryIcons;

		for(iconIndex = 0; iconIndex < MENU_ENTRY_ICONS; iconIndex++) {
			WINDOW_SPR *icon = *icons++;

			if(icon != 0)
				icon->depth = depth;
		}
	}
	syCloseEvent(mnPlacePartyFigures);
	mnSpawnIconEntries((ITEM_MENU_STATE *)menu, menu->msgWin);
	mnTextDrawMessageAsTilesStub(
		MsgSwitchCharacterHelp, menu->statWin, SWITCH_HELP_X, SWITCH_HELP_Y);
	mnTextDrawMessageAsTilesStub(
		MsgSwitchCharacterHelp + 2, menu->statWin, 0, SWITCH_HELP_Y);

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		mnHideEntryIcons();
		mnWindowSprRedrawSavedRectStub(menu->statWin);
		menu->psynergyCnt =
			mnPsynergyMenuCollectActions(gmUnitGetStub(menu->memberIds[0]),
				menu->psynergies, PSYNERGY_LIST_ALL);
		sySynch(1);
		mnPagedListBuild(&state, 0);
		mnPsynergyMenuDrawPreparedPsynergyIcons(
			menu->statWin, menu->memberIds[0]);
		redraw = TRUE;
		listNeedsRedraw = TRUE;

		while(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
			if(redraw != 0) {
				redraw = FALSE;
				if(listNeedsRedraw != 0) {
					listNeedsRedraw = FALSE;
					mnStatScreenDrawPsynergyPage(menu->statWin, 0, &state);
				}
				mnStatScreenShowPsynergyRow(menu->statWin, 0, &state);
				sySynch(1);
			}
			sySynch(1);
			navigationResult = mnPagedListHandleInput(0, state.entryCnt,
				STATUS_LIST_PAGE_ROWS, &state.row, &state.page);
			menu->sideIcon[0]->motion = ICON_SHOWN;
			mnCursorPos(STATUS_PSYNERGY_CURSOR_X,
				state.row * 16 + STATUS_PSYNERGY_CURSOR_Y);
			if(navigationResult == 1) {
				listNeedsRedraw = TRUE;
				redraw = TRUE;
			}
			if(navigationResult == 0)
				redraw = TRUE;
			if(navigationResult == -1)
				redraw = FALSE;
			if(PerPush & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = 1;
				done = TRUE;
				break;
			}
			if(PerPush & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				sySetEvent(mnPlacePartyFigures, EVENT_PASS_FRAME);
				break;
			}
			if((PerRepeat & KEY_R) || (PerRepeat & KEY_L)) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				memberIndex = menu->tabIdx[0];
				menu->selectedIdxByMember[menu->memberTbl[memberIndex]] =
					state.selectedIdx;
				if(PerRepeat & KEY_R)
					memberIndex++;
				else
					memberIndex--;
				memberIndex = (memberIndex + menu->memberCnt) % menu->memberCnt;
				menu->selectedMember = menu->memberTbl[memberIndex];
				menu->memberIds[0] = menu->memberTbl[memberIndex];
				menu->tabIdx[0] = memberIndex;
				mnReloadMemberPortrait(menu, menu->memberTbl[memberIndex]);
				break;
			}
		}
	}
	mnWindowSprClearListStub(menu->infoWin);
	mnWindowSprRedrawSavedRectStub(menu->infoWin);
	mnItemMenuHideAllIcons();
	mnWindowSprClearListStub(menu->msgWin);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	return result;
}

/* Unused: returns 1. */
UNUSED Sint32 mnStatScreenPsynergyPageReturnTrue(void)
{
	return 1;
}

/* Hides every entry icon, then loads the icons of the Psynergy already listed
   in the state (psynergies) into them. memberId is only looked up. */
void mnPsynergyMenuDrawPreparedPsynergyIcons(
	const void *unused, Sint32 memberId)
{
	PSYNERGY_MENU_STATE *menu = MenuState;

	gmUnitGetStub(memberId);
	mnItemMenuHideAllIcons();
	mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
}

/* Unused: two functions that do nothing. */
UNUSED void mnPsynergyMenuPreparedIconsNoOperation(void)
{
}

UNUSED void mnPsynergyMenuPreparedIconsSecondNoOperation(void)
{
}

/* Updates the Status menu's equipment page for the row under the cursor: the
   item's plain name in the info window, the row highlights, and in the
   equipment window the highlight of the slot the item is worn in, if it is. */
Sint32 mnStatScreenShowEquipmentRow(
	WINDOW *window, Sint32 unused, PAGED_LIST *state)
{
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;
	Sint32			 firstEntry;
	Sint32			 rowIndex;

	menu = MenuState;
	firstEntry = state->page * STATUS_LIST_PAGE_ROWS;
	state->selectedIdx = firstEntry + state->row;
#if LANGUAGE != LANGUAGE_JAPANESE && !STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY
	mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
#elif STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY
	mnWindowClearInteriorTilesStub(menu->descriptionWin, 0, 0, 224, 8);
#else
	mnWindowSprClearListStub(menu->descriptionWin);
#endif
	sySynch(1);
	if(menu->items[state->selectedIdx] != 0) {
		MENU_DRAW_DESCRIPTION(
			(menu->items[state->selectedIdx] & INVENTORY_ITEM_ID_MASK) +
				MsgItemPlainName,
			menu->descriptionWin, 0, 0);
	}

	/* An item with an element shows its icon at tile column 27, in palette 0 on
	   the highlighted row and palette 1 (TILEMAP_ENTRY_ICON) elsewhere, and the
	   highlight stops short of it. */
	for(rowIndex = 0; rowIndex < STATUS_LIST_PAGE_ROWS; rowIndex++) {
		if(rowIndex == state->row) {
			item = gmItemGetDirectStub(
				menu->items[state->selectedIdx] & INVENTORY_ITEM_ID_MASK);
			if(item->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(window, item->element + 1, 27,
					rowIndex * 2 + 1, TILEMAP_ENTRY_PLAIN);
				mnWindowSetHighlightRect(
					window, 14, rowIndex * 2 + 1, 13, 1, HIGHLIGHT_PALETTE);
			} else {
				mnWindowSetHighlightRect(
					window, 14, rowIndex * 2 + 1, 14, 1, HIGHLIGHT_PALETTE);
			}
		} else {
			item = gmItemGetDirectStub(
				menu->items[firstEntry + rowIndex] & INVENTORY_ITEM_ID_MASK);
			if(item->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(window, item->element + 1, 27,
					rowIndex * 2 + 1, TILEMAP_ENTRY_ICON);
				mnWindowSetHighlightRect(
					window, 14, rowIndex * 2 + 1, 13, 1, WINDOW_PALETTE);
			} else {
				mnWindowSetHighlightRect(
					window, 14, rowIndex * 2 + 1, 14, 1, WINDOW_PALETTE);
			}
		}
	}

	for(rowIndex = 0; rowIndex <= 3; rowIndex++) {
		mnWindowSetHighlightRect(menu->equipWin, STATUS_EQUIP_SLOT_X,
			rowIndex * 2 + 1, STATUS_EQUIP_SLOT_WIDTH, 1, WINDOW_PALETTE);
	}

	/* The slots' name rows: weapon 1, head 3, shield 5, chest 7, each under its
	   label (mnItemMenuDrawEquipment). Other worn items have no row. */
	if(menu->items[state->selectedIdx] & INVENTORY_EQUIPPED) {
		item = gmItemGetDirectStub(
			menu->items[state->selectedIdx] & INVENTORY_ITEM_ID_MASK);
		switch(item->type) {
		case ITEM_TYPE_WEAPON:
			mnWindowSetHighlightRect(menu->equipWin, STATUS_EQUIP_SLOT_X, 1,
				STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		case ITEM_TYPE_HELM:
			mnWindowSetHighlightRect(menu->equipWin, STATUS_EQUIP_SLOT_X, 3,
				STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		case ITEM_TYPE_SHIELD:
			mnWindowSetHighlightRect(menu->equipWin, STATUS_EQUIP_SLOT_X, 5,
				STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		case ITEM_TYPE_ARMOR:
			mnWindowSetHighlightRect(menu->equipWin, STATUS_EQUIP_SLOT_X, 7,
				STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		}
	}
	sySynch(1);
	return 1;
}

/* Draws one page of the Status menu's equipment page into window: up to
   STATUS_LIST_PAGE_ROWS item names with their icons and the page numbers, or
   "no items", and the party member's name with their Attack and Defense.
   Returns 1. */
Sint32 mnStatScreenDrawEquipmentPage(
	WINDOW *window, Sint32 unused, PAGED_LIST *state)
{
	ITEM_MENU_STATE *menu = MenuState;
	UNIT			*unit;
	Uint16			*inventoryEntry;
	Sint32			 firstEntry;
	Uint32			 rowCount;
	Uint32			 rowIndex;
	Sint32			 message;

	unit = gmUnitGetStub(menu->sideMember[0]);
	mnWindowClearInteriorTilesStub(window, 128, 8, 224, 96);
	firstEntry = state->page * STATUS_LIST_PAGE_ROWS;
	rowCount = (Uint8)(state->entryCnt - firstEntry);
	if(rowCount > STATUS_LIST_PAGE_ROWS)
		rowCount = STATUS_LIST_PAGE_ROWS;
	mnSetPageIcons(STATUS_LIST_PAGE_ROWS, firstEntry, window,
		STATUS_ITEM_PAGE_ICONS_X, 52);
	mnPagedListDrawPageNumbers(
		window, state->entryCnt, STATUS_LIST_PAGE_ROWS, state->page, 28);
	if(menu->itemCnt == 0) {
		mnTextDrawMessageAsTilesStub(
			MsgItemMenuEmpty, window, STATUS_ITEM_EMPTY_MESSAGE_X, 8);
	} else {
		rowIndex = 0;
		if(rowCount > rowIndex) {
			inventoryEntry = &menu->items[firstEntry];
			do {
				mnTextDrawMessageAsTilesStub(
					(*inventoryEntry & INVENTORY_ITEM_ID_MASK) + MsgItemName,
					window, STATUS_ITEM_NAME_X, (rowIndex << 4) + 8);
				rowIndex = (Uint8)(rowIndex + 1);
				inventoryEntry++;
			} while(rowCount > rowIndex);
		}
	}
	mnTextDrawStringAtOffsetStub(unit->name, window, 40, 0);
	message = MsgStatLabel;
	mnTextDrawMessageAsTilesStub(message, window, 32, 16);
	mnTextDrawMessageAsTilesStub(message + 1, window, 32, 24);
	mnTextDrawNumInWindowStub(unit->attack, 3, window, STATUS_ITEM_STAT_X, 16);
	mnTextDrawNumInWindowStub(unit->defense, 3, window, STATUS_ITEM_STAT_X, 24);
	return 1;
}

/* The Status menu's equipment page: the party member's items in the first list
   order (LIST_SORT_EQUIPPED_THEN_ITEMS), STATUS_LIST_PAGE_ROWS to a page,
   beside the equipment window of what they wear. */
Sint32 mnStatScreenRunEquipmentPage(void)
{

	ITEM_MENU_STATE *menu;
	UNIT			*member;
	Sint32			 result;
	Bool			 done;
	Bool			 redraw;
	Bool			 listNeedsRedraw;
	Sint32			 navigationResult;
	Sint32			 memberIndex;
	Sint32			 iconIndex;
	Uint16			 savedInventory[INVENTORY_SLOTS];
	PAGED_LIST		 state;

	menu = MenuState;
	{
		Sint32 memberId = menu->sideMember[0];

		result = 0;
		gmUnitGetStub(menu->memberIds[memberId]);
	}
	mnWindowUpdateOrCreate(&menu->equipWin, 0, 10, STATUS_EQUIP_WINDOW_WIDTH,
		10, WINDOW_FLAG_INSTANT);
	mnHideEntryIcons();

	for(iconIndex = 0; iconIndex < MENU_ENTRY_ICONS; iconIndex++) {
		WINDOW_SPR *icon = menu->entryIcons[iconIndex];

		if(icon != 0)
			icon->depth = OAM_DEPTH_WINDOW;
	}

	syCloseEvent(mnPlacePartyFigures);
	mnTextDrawMessageAsTilesStub(
		MsgSwitchCharacterHelp, menu->statWin, 64, SWITCH_HELP_Y);
	mnTextDrawMessageAsTilesStub(
		MsgSwitchCharacterHelp + 3, menu->statWin, 0, SWITCH_HELP_Y);
	mnHideEntryIcons();
	sySynch(1);
	mnSpawnIconEntries((ITEM_MENU_STATE *)menu, menu->questionWin);

	done = FALSE;
	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		mnHideEntryIcons();
		mnWindowSprRedrawSavedRectStub(menu->statWin);
		member = gmUnitGetStub(menu->sideMember[0]);
		for(iconIndex = 0; iconIndex < INVENTORY_SLOTS; iconIndex++)
			savedInventory[iconIndex] = member->inventory[iconIndex];
		menu->itemCnt = mnItemMenuCollectItems(member, menu->items, 0);
		mnItemMenuSort(menu->items, LIST_SORT_EQUIPPED_THEN_ITEMS);
		mnPagedListBuild(&state, 0);
		mnWindowSprRedrawSavedRectStub(menu->equipWin);
		mnItemMenuDrawEquipment(menu->equipWin, menu->sideMember[0], 1);
		sySynch(1);
		mnItemMenuDrawIcons(menu->items, 0);
		redraw = TRUE;
		listNeedsRedraw = TRUE;

		while(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
			if(redraw != 0) {
				mnWindowSprClearListStub(menu->descriptionWin);
				redraw = FALSE;
				if(listNeedsRedraw != 0) {
					listNeedsRedraw = FALSE;
					mnStatScreenDrawEquipmentPage(menu->statWin, 0, &state);
				}
				mnStatScreenShowEquipmentRow(menu->statWin, 0, &state);
			}
			menu->sideIcons[0]->motion = ICON_SHOWN;
			mnCursorPos(96, state.row * 16 + 52);
			sySynch(1);

			navigationResult = mnPagedListHandleInput(0, state.entryCnt,
				STATUS_LIST_PAGE_ROWS, &state.row, &state.page);
			if(navigationResult == 1) {
				listNeedsRedraw = TRUE;
				redraw = TRUE;
			}
			if(navigationResult == 0)
				redraw = TRUE;
			if(navigationResult == -1)
				redraw = FALSE;

			if(PerPush & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = 1;
				done = TRUE;
				break;
			}
			if(PerPush & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				break;
			}
			if((PerRepeat & KEY_R) || (PerRepeat & KEY_L)) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				memberIndex = menu->sideIdx[0];
				menu->selectedIdxByMember[menu->memberIds[memberIndex]] =
					state.selectedIdx;
				if(PerRepeat & KEY_R)
					memberIndex++;
				else
					memberIndex--;
				/* Copy back the inventory saved as the list was built; only the
				   list was sorted, so this writes what is already there. */
				for(iconIndex = 0; iconIndex < INVENTORY_SLOTS; iconIndex++)
					member->inventory[iconIndex] = savedInventory[iconIndex];
				memberIndex = (memberIndex + menu->partyCnt) % menu->partyCnt;
				menu->selectedMember = menu->memberIds[memberIndex];
				menu->sideMember[0] = menu->memberIds[memberIndex];
				menu->sideIdx[0] = memberIndex;
				mnReloadMemberPortrait(menu, menu->memberIds[memberIndex]);
				break;
			}
		}
	}

	mnWindowSprClearListStub(menu->descriptionWin);
	mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
	mnWindowSprClearListStub(menu->questionWin);
	mnItemMenuHideAllIcons();
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	sySetEvent(mnPlacePartyFigures, EVENT_PASS_FRAME);
	return result;
}

/* Moves through the status grid's irregular rows and returns row * 9 + column.
   Nothing calls it. */
Sint32 mnMoveGridCursor(Sint32 *columnOut, Sint32 *rowOut, Sint32 direction)
{
	Sint32 row;
	Sint32 column;

	column = *columnOut;
	row = *rowOut;
	switch(direction) {
	case KEY_UP:
		row -= 1;
		if(row < 0) {
			row = 5;
		}
		if(row <= 3) {
			if(row == 3) {
				if(column <= 4) {
					column = 0;
				} else {
					column = 1;
				}
			} else if(column > 1) {
				column = 1;
			}
			if((row == 3) && (column == 1)) {
				row = 2;
			}
		}
		break;
	case KEY_DOWN:
		row += 1;
		if(row > 5) {
			row = 0;
		}
		if((row == 3) && (column == 1)) {
			row = 4;
		}
		if(row == 4) {
			column = 0;
		}
		break;
	case KEY_LEFT:
		column -= 1;
		if(row == 3) {
			column += 1;
		} else if(row > 3) {
			if(column < 0) {
				column = 7;
			}
		} else if(column < 0) {
			column = 1;
		}
		break;
	case KEY_RIGHT:
		column += 1;
		if(row == 3) {
			column -= 1;
		} else if(row > 3) {
			if(column > 7) {
				column = 0;
			}
		} else if(column > 1) {
			column = 0;
		}
		break;
	}
	*columnOut = column;
	*rowOut = row;
	return (row * 9) + column;
}

/* Unused: returns 1. */
UNUSED Sint32 mnGridCursorReturnTrue(void)
{
	return 1;
}

/* The worn items' rows in mnItemMenuDrawEquipment's window, 16 pixels apart:
   weapon, helm, shield and armor from the top. A slot's label stands on its
   row, its item's name a line below, and its icon at EQUIPPED_ICON_X on
   screen. */
#define EQUIPPED_ROW_Y(row)	 ((row) * 16)
#define EQUIPPED_ICON_X		 216
#define EQUIPPED_ICON_Y(row) (EQUIPPED_ROW_Y(row) + 32)

/* Writes the four equipment slot labels (Weapon, Head, Shield and Chest, one a
   16-pixel row) and the names of the items worn in them into window; mode 0
   also shows their icons at the right. */
void mnItemMenuDrawEquipment(WINDOW *window, Sint32 memberId, Sint32 mode)
{
	ITEM_MENU_STATE *menu = MenuState;
	Uint16			*items;

	gmUnitGetStub(memberId);
	mnHideEntryIcons();
	mnItemMenuHideAllIcons();
	mnTextDrawMessageAsTilesStub(MsgEquipSlotLabels, window, 0, 0);
	mnTextDrawMessageAsTilesStub(
		MsgEquipSlotLabels + 1, window, 0, EQUIPPED_ROW_Y(2));
	mnTextDrawMessageAsTilesStub(
		MsgEquipSlotLabels + 2, window, 0, EQUIPPED_ROW_Y(1));
	mnTextDrawMessageAsTilesStub(
		MsgEquipSlotLabels + 3, window, 0, EQUIPPED_ROW_Y(3));
	items = menu->items;
	mnItemMenuDrawEquippedItemNames(window, items);
	if(mode == 0) {
		sySynch(1);
		mnItemMenuDrawIcons(items, 1);
		mnItemMenuPlaceEquippedIcons(items);
	}
}

/* Writes the name of each worn weapon, armor, shield and helm under its slot
   label (mnItemMenuDrawEquipment). Other worn items are not listed. */
void mnItemMenuDrawEquippedItemNames(WINDOW *window, Uint16 *items)
{
	Sint32 slotIndex;
	Sint32 itemId;

	for(slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
		if(items[slotIndex] & INVENTORY_EQUIPPED) {
			itemId = items[slotIndex] & INVENTORY_ITEM_ID_MASK;
			switch(gmItemGetDirectStub(itemId)->type) {
			case ITEM_TYPE_WEAPON:
				mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window,
					STATUS_EQUIPPED_NAME_X, EQUIPPED_ROW_Y(0) + 8);
				break;
			case ITEM_TYPE_ARMOR:
				mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window,
					STATUS_EQUIPPED_NAME_X, EQUIPPED_ROW_Y(3) + 8);
				break;
			case ITEM_TYPE_SHIELD:
				mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window,
					STATUS_EQUIPPED_NAME_X, EQUIPPED_ROW_Y(2) + 8);
				break;
			case ITEM_TYPE_HELM:
				mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window,
					STATUS_EQUIPPED_NAME_X, EQUIPPED_ROW_Y(1) + 8);
				break;
			}
		}
	}
}

/* Places the 32 entry icons in a grid of 16x16 cells, columns wide, from
   (originX, originY), and shows them: the party member chooser's Psynergy
   icons. */
void mnPlaceEntryIconsInGrid(Sint32 originX, Sint32 originY, Sint32 columns)
{
	Sint32		 entryIndex;
	WINDOW_SPR	*icon;
	WINDOW_SPR **icons;

	for(entryIndex = 0, icons = ((ITEM_MENU_STATE *)MenuState)->entryIcons;
		entryIndex <= MENU_ENTRY_ICONS - 1; entryIndex++) {
		icon = *icons++;
		if(icon != NULL) {
			mnPlaceEntryIconInGrid(icon, entryIndex, originX, originY, columns);
		}
	}
}

/* The side of a cell of the grids mnPlaceEntryIconInGrid places icons in. */
#define ENTRY_ICON_CELL_SIZE 16

/* Places icon at cell index (an index above 31 is taken as 0) of a grid of
   ENTRY_ICON_CELL_SIZE square cells, columns wide, from (originX, originY), and
   shows it. */
void mnPlaceEntryIconInGrid(WINDOW_SPR *icon, Sint32 index, Sint32 originX,
	Sint32 originY, Sint32 columns)
{
	Sint32 entryIndex;

	entryIndex = index;
	if(entryIndex > MENU_ENTRY_ICONS - 1) {
		entryIndex = 0;
	}
	icon->y = (entryIndex / columns * ENTRY_ICON_CELL_SIZE) + originY;
	icon->x = (entryIndex % columns * ENTRY_ICON_CELL_SIZE) + originX;
	mnIconShow(icon);
}

/* Places the icon of each worn weapon, armor, shield and helm at
   EQUIPPED_ICON_X, on its slot's row; the others stay off screen. */
void mnItemMenuPlaceEquippedIcons(Uint16 *items)
{
	ITEM_MENU_STATE *menu;
	WINDOW_SPR		*icon;
	Sint32			 slotIndex;

	menu = MenuState;
	mnHideEntryIcons();
	for(slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
		if(items[slotIndex] != 0 &&
			(items[slotIndex] & INVENTORY_EQUIPPED) != 0) {
			icon = menu->entryIcons[slotIndex];
			if(icon != 0) {
				switch(gmItemGetDirectStub(
					items[slotIndex] & INVENTORY_ITEM_ID_MASK)
						->type) {
				case ITEM_TYPE_WEAPON:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(0);
					break;
				case ITEM_TYPE_ARMOR:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(3);
					break;
				case ITEM_TYPE_SHIELD:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(2);
					break;
				case ITEM_TYPE_HELM:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(1);
					break;
				}
				mnIconShow(icon);
			}
		}
	}
}

/* Moves all 32 entry icons off screen, to MENU_ICON_HIDDEN_X and _Y, before a
   screen lays them out again. */
void mnHideEntryIcons(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 x = MENU_ICON_HIDDEN_X;
	WINDOW_SPR	   **entry = menu->entryIcons;
	Sint32			 y = MENU_ICON_HIDDEN_Y;
	Sint32			 i;

	for(i = 0; i < MENU_ENTRY_ICONS; i++) {
		WINDOW_SPR *object = *entry++;

		if(object != 0) {
			object->x = x;
			object->y = y;
			mnIconShow(object);
		}
	}
}

/* Creates the eight status icon sprites in window, off screen at
   MENU_ICON_HIDDEN_X and _Y, each on item graphic slot of the same number; the
   Status menu shows the status icons with the first five
   (mnStatScreenLoadStatIcons). */
Sint32 mnStatScreenCreateStatIcons(WINDOW *window)
{
	WINDOW_SPR	   **slot;
	WINDOW_SPR		*icon;
	Sint32			 slotIndex;
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	slotIndex = 0;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		slot = menu->statIcons;
		do {
			icon = mnWindowSprCreateFromResourceStub(
				ICON_KIND_ITEM, slotIndex, window, MENU_ICON_HIDDEN_X, y);
			slotIndex += 1;
			*slot = icon;
			slot += 1;
		} while(slotIndex <= 7);
		return 1;
	}
}

/* Shows the icon of each status that flags marks (mnStatScreenListStatuses)
   down the left edge from y 88, 16 pixels apart, beside the status labels of
   mnStatScreenDrawStatPage. */
void mnStatScreenShowStatIcons(const Uint8 *flags)
{
	ITEM_MENU_STATE *menu;
	WINDOW_SPR	   **slot;
	WINDOW_SPR		*entry;
	Sint32			 index;
	Sint32			 shown;

	menu = MenuState;
	mnStatScreenHideStatIcons();
	for(index = 0, slot = menu->statIcons, shown = 0; index <= 4; index++) {
		entry = *slot++;
		if(entry != 0 && flags[index] != 0) {
			entry->x = 8;
			entry->y = shown * 16 + 88;
			entry->depth = OAM_DEPTH_WINDOW;
			mnIconShow(entry);
			shown++;
		}
	}
}

/* Moves the five status icons off screen, before they are shown again or while
   another view covers them. */
void mnStatScreenHideStatIcons(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 index;

	for(index = 0; index < 5; index++) {
		WINDOW_SPR *object = menu->statIcons[index];

		if(object != 0) {
			object->x = MENU_ICON_HIDDEN_X;
			object->y = MENU_ICON_HIDDEN_Y;
			object->depth = OAM_DEPTH_WINDOW;
			mnIconShow(object);
		}
	}
}

/* Loads each enabled status's icon into its sprite's tile slot: kind is the
   icon's number in Icon_MiscIconPointers, which the battle Status screen shows
   for the same status. */
Sint32 mnStatScreenLoadStatIcons(const Uint8 *enabled)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 index;

	for(index = 0; index <= 4; index++) {
		if(enabled[index] != 0) {
			Sint32 kind;
			switch(index) {
			case CHARACTER_DOWN:
				kind = OWNER_CONDITION_DOWNED;
				break;
			case CHARACTER_POISON:
				kind = OWNER_CONDITION_POISON;
				break;
			case CHARACTER_VENOM:
				kind = OWNER_CONDITION_DEADLY_POISON;
				break;
			case CHARACTER_CURSE:
				kind = OWNER_CONDITION_CURSED_ITEM;
				break;
			case CHARACTER_HAUNT:
				kind = OWNER_CONDITION_HAUNT;
				break;
			default:
				kind = OWNER_CONDITION_NORMAL;
				break;
			}
			mnSelectionIconLoadIntoSlotStub(
				ICON_KIND_PATTERN, kind, (Uint8)menu->statIcons[index]->idx, 0);
		}
	}
	return 1;
}

#undef MEMBER_CHOOSER_MEMBER_COUNT
#undef STATUS_SAVED_TILE_BYTES
#undef STATUS_HELP_TEXT_MAX
#undef STATUS_STAT_DIGITS
#undef STATUS_STAT_X
#undef STATUS_EQUIP_PAGE_SIZE
#undef STATUS_PSYNERGY_CURSOR_X
#undef STATUS_PSYNERGY_CURSOR_Y
#undef STATUS_EQUIP_SLOT_X
#undef STATUS_EQUIP_SLOT_WIDTH
#undef STATUS_ITEM_PAGE_ICONS_X
#undef STATUS_ITEM_NAME_X
#undef STATUS_ITEM_STAT_X
#undef STATUS_ITEM_EMPTY_MESSAGE_X
#undef STATUS_EQUIP_WINDOW_WIDTH
#undef STATUS_EQUIPPED_NAME_X
#undef SWITCH_HELP_Y
#undef STATUS_AILMENT_Y
#undef STATUS_ELEMENT_X
#undef STATUS_ELEMENT_COLUMN
#undef EQUIPPED_ICON_X
#undef EQUIPPED_ICON_Y
#undef EQUIPPED_ROW_Y
#undef STAT_LABEL
