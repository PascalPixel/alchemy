/* The command icon menus of the field, title and transfer screens. Part of
   screens.c. */

/* How the transfer method's help is drawn ("Use a password to..." and "Use a
   Game Link cable to connect..."), and where its lines go, in pixels. */
#define TRANSFER_MODE_DRAW_HELP \
	GERMAN_OR(mnTextDrawMessageAsTiles, mnTextDrawMessage)
#define TRANSFER_PASSWORD_HELP_X	   GERMAN_OR(32, PASSWORD_HELP_X)
#define TRANSFER_PASSWORD_HELP_FIRST_Y GERMAN_OR(8, 4)
#define TRANSFER_PASSWORD_HELP_LAST_Y  GERMAN_OR(24, 16)
#define TRANSFER_CABLE_HELP_FIRST_Y	   GERMAN_OR(0, 4)
#define TRANSFER_CABLE_HELP_LAST_Y	   GERMAN_OR(32, 28)
/* The y of the cable help's second line, in pixels, in every edition. */
#define TRANSFER_CABLE_HELP_SECOND_Y   16

/* The icon menus' bottom row, in tiles, where most of them stand; a Yes or No
   may stand at the top, row 0, instead. */
#define ICON_MENU_BOTTOM_ROW 17
/* The lines an icon row covers: its label window's three tiles. */
#define ICON_ROW_LINES		 24
/* The most icons a menu shows, and so the row length of
   FileListActionByPosition, which has an action for each place. */
#define ICON_MENU_MAX_ICONS	 ((Sint32)ARRAY_COUNT(IconMenuStateWork->entries))

/* How far the selected icon overhangs its place while it pulses at the 8.8
   `scale`: on each side, half its 24-pixel width grown by the scale, rounded
   up; and past its row, half its 32-pixel height grown by the scale, rounded,
   and a line more. */
#define PULSE_OVERHANG_X(scale) ((((scale) - Q_8_8(1)) * 12 + 255) / 256)
#define PULSE_OVERHANG_LINES(scale) \
	((((scale) - Q_8_8(1)) * 32 + 255) / 512 + 1)

/* Draws the icon menu's icons, an event of the icon menus (mnIconMenuOpen).
   Each icon is a 32x32, 256-colour sprite, and one at x 0 is not drawn; the
   selected one pulses, double-size, in front of the others. While a screen
   transition runs, it widens window 1 over the icon row so the icons show
   through. */
void mnIconMenuDrawIcons(void)
{
	ICON_MENU_STATE *iconMenuState;
	ICON_MENU_ENTRY *entry;
	AFFINE_TRANSFORM transform;
	Sint32			 pulseScale;
	Sint32			 affineIndex;
	Sint32			 span;
	Sint32			 spriteY;
	Uint32			 lastLine;
	Uint32			 index;
	/* UB: edge steps from one line's window 1 edges to the next line's, past
	   the member it points into. */
	Uint16					*edge;
	SCREEN_TRANSITION_STATE *state;

	iconMenuState = IconMenuStateWork;
	entry = iconMenuState->entries;
	pulseScale = WindowSpriteBounceScales[(TickCount * 2) & 31];
	pulseScale = (pulseScale - Q_8_8(1)) / 4 + Q_8_8(1.1875);
	transform.scaleX = pulseScale;
	transform.scaleY = pulseScale;
	transform.angle = 0;
	affineIndex = syOAMMatrixAllocateFromTransform(&transform);
	/* Pulse only the selection; the other icons keep their normal OAM shape. */
	for(index = 0; index < iconMenuState->count; index++, entry++) {
		Sint32	drawLayer;
		Uint32 *word;
		Sint32	x;

		x = entry->x;
		if(x == 0)
			continue;
		if(index == iconMenuState->selection) {
			Sint32 left = x + pulseScale * 7 / 512 - 20;

			/* The icon grows to 32 * scale / Q_8_8(1) pixels in the middle of
			   its 64-pixel double-size box: on the top row its top edge stays
			   at the screen's top, the box starting above it with its y wrapped
			   within OAM's 8 bits; elsewhere it grows mostly upward from its
			   row. */
			if(entry->y != 0)
				spriteY = entry->y + pulseScale * 3 / 256 - 20;
			else
				spriteY = (pulseScale * 15 / 256 - 30) & 255;
			word = entry->sprite;
			*word++ = 0;
			*word++ = OBJ_MATRIX(affineIndex) | spriteY | OBJ_X(left) |
				OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
			*word = VramCacheEntries[entry->slot].offset >> 5;
			drawLayer = OAM_DEPTH_WINDOW + 6;
		} else {
			Uint32 attributes;
			Uint32 tileOffset;

			word = entry->sprite;
			spriteY = *(Sint16 *)&entry->y;
			*word++ = 0;
			attributes = spriteY | OBJ_X(x) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
			tileOffset = VramCacheEntries[entry->slot].offset;
			*word++ = attributes;
			*word = tileOffset >> 5;
			drawLayer = OAM_DEPTH_WINDOW + 5;
		}
		syOAMAddSpriteAtDepth(entry, drawLayer);
	}
	/* Window 1 widens around the selected icon on the lines it overhangs, below
	   it on the top row and above it on the bottom one, then covers the row's
	   ICON_ROW_LINES lines from the first icon, keeping each line's right
	   edge. */
	if(iconMenuState->row == 0) {
		WINDOW_BUFFER *page;
		Sint32		   halfWidth;

		state = ScreenTransitionStateWork;
		if(state == NULL || iconMenuState->count == 0)
			return;
		page = (WINDOW_BUFFER *)state->pages[state->page];
		span = iconMenuState->entries[iconMenuState->selection].x;
		halfWidth = PULSE_OVERHANG_X(pulseScale);
		span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
		lastLine =
			iconMenuState->entries[iconMenuState->selection].y + ICON_ROW_LINES;
		lastLine += PULSE_OVERHANG_LINES(pulseScale);
		edge = &page->lines[ICON_ROW_LINES].win1;
		for(index = ICON_ROW_LINES; index < lastLine; index++, edge += 2)
			*edge = (*edge & 0xff) | span;
		span = iconMenuState->entries[0].x;
		if(iconMenuState->selection == 0)
			span -= PULSE_OVERHANG_X(pulseScale);
		span <<= 8;
		edge = &page->lines[0].win1;
		for(index = 0; index < ICON_ROW_LINES; index++, edge += 2)
			*edge = (*edge & 0xff) | span;
	} else {
		WINDOW_BUFFER *page;
		Sint32		   firstLine;
		Sint32		   halfWidth;

		state = ScreenTransitionStateWork;
		if(state == NULL || iconMenuState->count == 0)
			return;
		page = (WINDOW_BUFFER *)state->pages[state->page];
		span = iconMenuState->entries[iconMenuState->selection].x;
		halfWidth = PULSE_OVERHANG_X(pulseScale);
		span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
		firstLine = iconMenuState->entries[iconMenuState->selection].y;
		firstLine -= PULSE_OVERHANG_LINES(pulseScale);
		edge = &page->lines[firstLine].win1;
		for(index = firstLine; index < ICON_MENU_BOTTOM_ROW * 8;
			index++, edge += 2)
			*edge = (*edge & 0xff) | span;
		span = iconMenuState->entries[0].x;
		if(iconMenuState->selection == 0)
			span -= PULSE_OVERHANG_X(pulseScale);
		span <<= 8;
		edge = &page->lines[ICON_MENU_BOTTOM_ROW * 8].win1;
		for(index = ICON_MENU_BOTTOM_ROW * 8; index < DISPLAY_HEIGHT;
			index++, edge += 2)
			*edge = (*edge & 0xff) | span;
	}
}

/* Allocates and clears the icon menu's state, which lives in
   HEAP_SLOT_ICON_MENU until mnIconMenuClose frees it, and starts the event that
   draws its icons. Every icon menu starts here. */
void *mnIconMenuOpen(void)
{
	void *state;
	state =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_ICON_MENU, sizeof(ICON_MENU_STATE));
	DMA_FILL32(0, state, sizeof(ICON_MENU_STATE));
	sySetEvent(mnIconMenuDrawIcons, EVENT_PASS_TICK - 10);
	return state;
}

/* Stops drawing the icons, closes the label window and releases each loaded
   icon before freeing the icon menu's state (HEAP_SLOT_ICON_MENU), then waits a
   tick. */
void mnIconMenuClose(void)
{
	WINDOW			*labelWindow;
	Sint32			 i;
	ICON_MENU_STATE *state;

	state = IconMenuStateWork;
	syCloseEvent(mnIconMenuDrawIcons);
	labelWindow = state->window;
	if(labelWindow != 0) {
		mnWindowClose(labelWindow, 2);
	}
	for(i = 0; i < state->count; i++) {
		syVRAMCacheFreeEntry(state->entries[i].slot);
	}
	syHeapSlotRelease(HEAP_SLOT_ICON_MENU);
	sySynch(1U);
}

/* Selects an icon position with wrapped directional input. A returns the
   position; B or Start returns -1. Redraws the label only after a move: the
   message labelBase + position when the layout set a labelBase, else the icon's
   command name. */
Sint32 mnIconMenuRun(Sint32 init)
{
	ICON_MENU_STATE *state = IconMenuStateWork;
	Sint32			 resource;

	state->selection = init;
redraw:
	mnWindowSpritePrepareForRedraw(state->window);
	if(state->labelBase != 0)
		resource = state->labelBase + state->selection;
	else
		resource = state->commandIcons[state->selection] + MsgCommandName;
	mnTextDrawMessageAsTiles(resource, state->window, 0, 0);
	for(;;) {
		sySynch(1);
		if(PerPush & KEY_A) {
			sySoundPlayStub(SE_MENU_CONFIRM);
			return state->selection;
		}
		if(PerPush & KEY_B) {
			sySoundPlayStub(SE_MENU_CANCEL);
			return -1;
		}
		if(PerPush & KEY_START) {
			sySoundPlayStub(SE_MENU_CANCEL);
			return -1;
		}
		if((PerRepeat & KEY_LEFT) || (PerRepeat & KEY_UP)) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			state->selection--;
			if(state->selection < 0)
				state->selection = state->count - 1;
		} else if((PerRepeat & KEY_RIGHT) || (PerRepeat & KEY_DOWN)) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			state->selection++;
			if(state->selection >= state->count)
				state->selection = 0;
		} else
			continue;
		goto redraw;
	}
}

/* Returns the distance between lhs and rhs, reusing their supplied
   difference. */
static inline Sint32 mnAbsoluteDifference(
	Sint32 difference, Sint32 lhs, Sint32 rhs)
{
	if(difference >= 0)
		return difference;
	return rhs - lhs;
}

/* Moves the selection from start to goal one icon at a time with the cursor
   sound, redrawing the label each step and waiting
   MenuSelectionStepDelays[icons still to go] ticks at each (12 more at the
   first), then 48 ticks on the goal before the confirm sound: a menu choice
   made for the player to watch (mnAnimateSelectionToEntry). Returns goal. */
Sint32 mnIconMenuAnimateTo(Sint32 start, Sint32 goal)
{
	Sint16			 resourceBase;
	Sint16			 selection;
	Sint32			 difference;
	Sint32			 resourceId;
	Sint32			 distance;
	Sint32			 pos;
	Sint32			 step;
	Sint32			 delay;
	const Uint8		*stepDelays;
	ICON_MENU_STATE *state;

	state = IconMenuStateWork;
	step = 1;
	delay = 12;
	state->selection = start;
	if(goal < start)
		step = -1;
	pos = start;

	for(;;) {
		mnWindowSpritePrepareForRedraw(state->window);
		resourceBase = state->labelBase;
		if(resourceBase != 0) {
			resourceId = resourceBase + state->selection;
		} else {
			resourceId = state->commandIcons[state->selection] + MsgCommandName;
		}
		mnTextDrawMessageAsTiles(resourceId, state->window, 0, 0);

		selection = state->selection;
		stepDelays = MenuSelectionStepDelays;
		difference = selection - goal;
		distance = mnAbsoluteDifference(difference, selection, goal);
		sySynch(stepDelays[distance] + delay);

		if(pos == goal)
			break;

		state->selection += step;
		sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		delay = 0;
		pos += step;
	}

	sySynch(48);
	sySoundPlayStub(SE_MENU_CONFIRM);
	return goal;
}

/* Decodes command icon `index` into the VRAM cache entry `slot`, through a
   block it allocates and frees. */
void mnCommandIconLoadIntoSlot(Sint32 slot, Sint32 index)
{
	Sint32	size = COMMAND_ICON_BYTES;
	void   *buf = syHeapAllocate(size);
	Uint16 *offsetTable = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	syDecompressByteLZ((void *)((Uint32)offsetTable + offsetTable[index]), buf);
	syVRAMCacheLoad(slot, size, buf);
	syHeapRelease(buf);
}

/* Appends a command icon if fewer than ICON_MENU_MAX_ICONS are present. It
   starts on the bottom row; mnIconMenuCenter or mnIconMenuLayoutIcons places
   the row. Its VRAM cache entry is taken unchecked. */
void mnIconMenuAddIcon(Sint32 icon)
{
	ICON_MENU_STATE *state;
	ICON_MENU_ENTRY *entry;
	Sint16			 index;
	Sint32			 slot;
	Sint32			 y = ICON_MENU_BOTTOM_ROW * 8;

	state = IconMenuStateWork;
	index = state->count;
	if(index < ICON_MENU_MAX_ICONS) {
		state->count++;
		entry = &state->entries[index];
		slot = syVRAMCacheFindFreeEntry();
		mnCommandIconLoadIntoSlot(slot, icon);
		entry->x = index * 24 + 32;
		entry->y = y;
		entry->slot = slot;
		state->commandIcons[index] = icon;
	}
}

/* Centres the icons and their label window together on tile row `row`. A
   nonzero resourceBase is the first label message; otherwise each icon is
   labelled by its command name. The icons stand three tiles apart and the label
   window, `width` + 2 tiles wide and 3 tall, follows them; the centring counts
   two thirds of the window's width. */
void mnIconMenuCenter(Sint32 row, Sint32 width, Sint32 resourceBase)
{
	ICON_MENU_STATE *state = IconMenuStateWork;
	Sint32			 x;
	Sint32			 i;
	Sint32			 count;

	state->width = width + 2;
	state->labelBase = resourceBase;
	state->row = row;
	count = state->count;
	x = DISPLAY_TILE_WIDTH / 2 - (count * 3 + state->width * 2 / 3) / 2;
	for(i = 0; i < state->count; i++) {
		ICON_MENU_ENTRY *entry = &state->entries[i];

		entry->x = x * 8;
		entry->y = row * 8;
		x += 3;
	}
	state->window =
		mnWindowCreate(x, row, state->width, 3, WINDOW_FLAG_INSTANT);
}

/* Places the icons three tiles apart from tile (x, y), then opens their label
   window, `width` + 2 tiles wide, just after them. */
void mnIconMenuLayoutIcons(
	Sint32 x, Sint32 y, Sint32 width, Sint32 resourceBase)
{
	ICON_MENU_STATE *state;
	Sint32			 i;
	ICON_MENU_ENTRY *entry;

	state = IconMenuStateWork;
	state->width = width + 2;
	state->labelBase = resourceBase;
	state->row = y;
	for(i = 0; i < state->count; i++) {
		entry = &state->entries[i];
		entry->y = y << 3;
		entry->x = x << 3;
		x += 3;
	}
	state->window = mnWindowCreate(x, y, state->width, 3, WINDOW_FLAG_INSTANT);
}

/* Reads the signed command-position mapping used by the icon menu. */
static __inline__ Sint32 mnIconMenuReadCommandPos(Sint8 *table, Sint32 index)
{
	return table[index];
}

/* The field menu's row of icons: Psynergy, Djinn, Item and Status, without
   Djinn when the party has none. `selection` is the command to start on and the
   result the command chosen (FIELD_MENU_COMMAND), or -1 when cancelled; two
   tables turn commands into icon positions and back. */
Sint32 mnFieldMenuSelectCommand(Sint32 selection)
{
	Sint32 group = 0;
	Sint32 offset;
	Sint32 groupTriple;
	Sint32 result;
	Sint8 *commandPositions;

	if(gmPartySumDjinnCountsStub(-1) == 0) {
		group = 1;
	}

	groupTriple = group * 3;
	commandPositions = FieldMenuPositionByCommand;
	offset = groupTriple << 1;
	selection =
		mnIconMenuReadCommandPos(commandPositions, selection + offset) - 1;
	if(selection < 0) {
		selection = 0;
	}

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_PSYNERGY);
	if(group == 0) {
		mnIconMenuAddIcon(COMMAND_ICON_DJINN);
	}
	mnIconMenuAddIcon(COMMAND_ICON_ITEM);
	mnIconMenuAddIcon(COMMAND_ICON_STATUS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FIELD_MENU_ICON_ROW_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();

	if(result >= 0) {
		result = FieldMenuCommandByPosition[result + offset + 1];
	}

	return result;
}

/* The width of the field command menu's label window, in tiles. */
#define FIELD_COMMAND_LABEL_WIDTH JAPANESE_OR(5, 7)

/* Opens the field command menu and animates from start to the command's icon.
   Returns the final icon position after closing the menu. */
Sint32 mnAnimateSelectionToEntry(Sint32 start, Sint32 command)
{
	mnPartyStatusOpenAndWait();
	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_PSYNERGY);
	mnIconMenuAddIcon(COMMAND_ICON_DJINN);
	mnIconMenuAddIcon(COMMAND_ICON_ITEM);
	mnIconMenuAddIcon(COMMAND_ICON_STATUS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FIELD_COMMAND_LABEL_WIDTH, 0);
	command = mnIconMenuAnimateTo(start, command - 1);
	mnIconMenuClose();
	mnPartyStatusCloseWithMenu();
	return command;
}

/* The File Screen's rows of icons, by the files saved, each a row of
   FileListActionByPosition. */
typedef enum FILE_ACTION_ROW {
	FILE_ROW_SOME = 0, /* one or two files: New Game, Continue, Copy, Erase */
	FILE_ROW_FULL = 1, /* three files: Continue, Erase */
	FILE_ROW_FULL_CLEARED = 2, /* three files, all cleared: Erase */
	/* one or two files, all cleared: New Game, Copy, Erase */
	FILE_ROW_CLEARED = 3
} FILE_ACTION_ROW;

/* The File Screen's row of icons, from the save summaries: New Game, Continue,
   Copy and Erase for one or two files (starting on Continue); no New Game or
   Copy with three; with every file cleared no Continue, and only Erase with
   three such files; then Battle and Send when they are enabled. Returns the
   action chosen (FILE_ACTION) by its position. With no files it returns 0 at
   once, and -1 when cancelled or the backup memory cannot be read. */
Sint32 mnFileListSelectAction(void)
{
	Sint32 fileState;
	Sint32 result;
	Bool   init;
	Uint32 group;

	group = FILE_ROW_SOME;
	init = FALSE;
	fileState = mnSaveScanFileSummaries();
	if(fileState < 0) {
		return -1;
	}
	if(fileState == 0) {
		return 0;
	}
	if(fileState == BUP_FILE_COUNT) {
		group = FILE_ROW_FULL;
	} else if(fileState == SAVE_SCAN_ALL_CLEARED + BUP_FILE_COUNT) {
		group = FILE_ROW_FULL_CLEARED;
	} else if(fileState > SAVE_SCAN_ALL_CLEARED) {
		group = FILE_ROW_CLEARED;
	} else {
		init = TRUE;
	}
	mnIconMenuOpen();
	if((group == FILE_ROW_SOME) || (group == FILE_ROW_CLEARED)) {
		mnIconMenuAddIcon(COMMAND_ICON_NEW_GAME);
	}
	if(group <= (Uint32)FILE_ROW_FULL) {
		mnIconMenuAddIcon(COMMAND_ICON_CONTINUE);
	}
	if((group == FILE_ROW_SOME) || (group == FILE_ROW_CLEARED)) {
		mnIconMenuAddIcon(COMMAND_ICON_COPY);
	}
	mnIconMenuAddIcon(COMMAND_ICON_ERASE);
	if(TitleBattleOptionEnabled != 0) {
		mnIconMenuAddIcon(COMMAND_ICON_BATTLE);
	}
	if(TitleSendOptionEnabled != 0) {
		mnIconMenuAddIcon(COMMAND_ICON_SEND);
	}
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FILE_SCREEN_ICON_ROW_WIDTH, 0);
	result = mnIconMenuRun(init);
	mnIconMenuClose();
	/* With Send shown but not Battle, Send's place maps to SAVE_ACTION_BATTLE:
	   FileListActionByPosition lists Battle before Send in every row. */
	if(result >= 0) {
		result =
			FileListActionByPosition[result + (group * ICON_MENU_MAX_ICONS)];
	}
	return result;
}

/* The x of the list of what a password level carries, and the right edge of the
   area cleared for it, in pixels. */
#define MODE_LABEL_X	 JAPANESE_OR(8, 18)
#define MODE_LABEL_RIGHT JAPANESE_OR(80, 144)

/* An event of the password level menu: when the selection moves, lists what
   that level's password carries, from MsgPasswordLevel on: Gold "Level",
   "Djinn", "Character stats", "Items" and "Coins", Silver the first three,
   Bronze the first two. */
void mnPasswordLevelDrawHelp(void)
{
	ICON_MENU_STATE *state = IconMenuStateWork;

	if(state->drawnSelection != state->selection) {
		state->drawnSelection = state->selection;
		mnWindowClearInteriorTiles(
			state->lowerWindow, 8, 40, MODE_LABEL_RIGHT, 80);

		switch(state->selection) {
		case PASSWORD_LEVEL_GOLD: {
			Sint32 text = MsgPasswordLevel;

			mnTextDrawMessageAsTiles(
				text, state->lowerWindow, MODE_LABEL_X, 40);
			mnTextDrawMessageAsTiles(
				text + 1, state->lowerWindow, MODE_LABEL_X, 48);
			mnTextDrawMessageAsTiles(
				text + 2, state->lowerWindow, MODE_LABEL_X, 56);
			mnTextDrawMessageAsTiles(
				text + 3, state->lowerWindow, MODE_LABEL_X, 64);
			text += 4;
			mnTextDrawMessageAsTiles(
				text, state->lowerWindow, MODE_LABEL_X, 72);
			break;
		}
		case PASSWORD_LEVEL_SILVER: {
			Sint32 text = MsgPasswordLevel;

			mnTextDrawMessageAsTiles(
				text, state->lowerWindow, MODE_LABEL_X, 40);
			mnTextDrawMessageAsTiles(
				text + 1, state->lowerWindow, MODE_LABEL_X, 48);
			text += 2;
			mnTextDrawMessageAsTiles(
				text, state->lowerWindow, MODE_LABEL_X, 56);
			break;
		}
		case PASSWORD_LEVEL_BRONZE:
		default: {
			Sint32 text = MsgPasswordLevel;

			mnTextDrawMessageAsTiles(
				text++, state->lowerWindow, MODE_LABEL_X, 40);
			mnTextDrawMessageAsTiles(
				text, state->lowerWindow, MODE_LABEL_X, 48);
			break;
		}
		}
	}
}

/* The x of the password transfer's help lines, in pixels. */
#define PASSWORD_HELP_X JAPANESE_OR(0, 16)

/* French alone redraws the transfer help its own way: it releases the help's
   window sprites instead of preparing them for a redraw, and draws Cable's help
   only when coming from Password. */
#if TRANSFER_HELP_SKIPS_EMPTY_WINDOW_REDRAW

/* An event of the transfer method menu: when the selection moves between
   Password and Cable, explains the one chosen. */
void mnTransferMethodDrawHelp(void)
{
	ICON_MENU_STATE *state = IconMenuStateWork;

	if(state->drawnSelection != state->selection) {
		if(state->selection == TRANSFER_METHOD_PASSWORD) {
			mnWindowSpriteClearList(state->lowerWindow);
			mnTextDrawMessage(
				MsgPasswordTransferHelp, state->lowerWindow, 16, 4);
			mnTextDrawMessage(
				MsgPasswordTransferHelp + 1, state->lowerWindow, 16, 16);
		} else if(state->drawnSelection == TRANSFER_METHOD_PASSWORD) {
			mnWindowSpriteClearList(state->lowerWindow);
			mnTextDrawMessage(MsgCableTransferHelp, state->lowerWindow, 0, 4);
			mnTextDrawMessage(
				MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
			mnTextDrawMessage(
				MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
		}
		state->drawnSelection = state->selection;
	}
}
#else
/* An event of the transfer method menu: when the selection moves between
   Password and Cable, explains the one chosen. */
void mnTransferMethodDrawHelp(void)
{
	ICON_MENU_STATE *state = IconMenuStateWork;
	Sint16			*shown = &state->drawnSelection;

	if(*shown != state->selection) {
		*shown = state->selection;
		mnWindowSpritePrepareForRedraw(state->lowerWindow);
		if(state->selection == TRANSFER_METHOD_PASSWORD) {
			/* "Use a password to" "pass along your saved quest." */
			TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp, state->lowerWindow,
				TRANSFER_PASSWORD_HELP_X, TRANSFER_PASSWORD_HELP_FIRST_Y);
			TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp + 1,
				state->lowerWindow, TRANSFER_PASSWORD_HELP_X,
				TRANSFER_PASSWORD_HELP_LAST_Y);
		} else {
			/* "Use a Game Link cable to connect" "two Game Boy Advance
			   systems." "Pass along your saved quest." */
			TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp, state->lowerWindow, 0,
				TRANSFER_CABLE_HELP_FIRST_Y);
			TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 1,
				state->lowerWindow, 0, TRANSFER_CABLE_HELP_SECOND_Y);
			TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 2,
				state->lowerWindow, 0, TRANSFER_CABLE_HELP_LAST_Y);
		}
	}
}
#endif

/* Selects either the transfer method or the password level (enum
   RESOURCE_LAYOUT_MODE). */
Sint32 mnIconMenuChooseSendOption(Sint32 mode)
{
	ICON_MENU_STATE *state;
	WINDOW			*window;
	Sint32			 msg;
	WINDOW		   **upper;
	Sint32			 result;

	mnIconMenuOpen();
	state = IconMenuStateWork;
	if(mode == SEND_OPTION_TRANSFER_METHOD) {
		mnIconMenuAddIcon(COMMAND_ICON_PASSWORD);
		mnIconMenuAddIcon(COMMAND_ICON_CABLE);
	} else {
		mnIconMenuAddIcon(COMMAND_ICON_GOLD);
		mnIconMenuAddIcon(COMMAND_ICON_SILVER);
		mnIconMenuAddIcon(COMMAND_ICON_BRONZE);
#if TRANSFER_METHOD_MENU_UNCENTRED
		mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 5, 0);
#endif
	}
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN || \
	LANGUAGE == LANGUAGE_ITALIAN
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 7, 0);
#elif TRANSFER_METHOD_MENU_UNCENTRED
#elif LANGUAGE == LANGUAGE_FRENCH
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 9, 0);
#else
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 5, 0);
#endif
	if(mode != SEND_OPTION_TRANSFER_METHOD) {
		sySetEvent(mnPasswordLevelDrawHelp, EVENT_PASS_TICK - 10);
		state->drawnSelection = -1;
		window = mnWindowCreate(PASSWORD_LEVEL_WINDOW_X, 0,
			PASSWORD_LEVEL_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
		msg = MsgPasswordSelection;
		upper = &state->upperWindow;
		*upper = window;
		/* "Password Selection", then "The data stored in your" "password
		   depends on which" "level you select:" */
		mnTextDrawMessage(msg, window, 0, 4);
#if LANGUAGE != LANGUAGE_JAPANESE
		window = mnWindowCreate(PASSWORD_HELP_WINDOW_X, 4,
			PASSWORD_HELP_WINDOW_WIDTH, 12, WINDOW_FLAG_INSTANT);
		state->lowerWindow = window;
		mnTextDrawMessage(msg + 1, window, 8, 0);
		mnTextDrawMessage(msg + 2, state->lowerWindow, 8, 11);
		msg += 3;
		mnTextDrawMessage(msg, state->lowerWindow, 8, 22);
#else
		window = mnWindowCreate(5, 4, 21, 12, WINDOW_FLAG_INSTANT);
		state->lowerWindow = window;
		mnTextDrawMessage(msg + 1, window, 0, 4);
		msg += 2;
		mnTextDrawMessage(msg, state->lowerWindow, 0, 16);
#endif
	} else {
		sySetEvent(mnTransferMethodDrawHelp, EVENT_PASS_TICK - 10);
		state->drawnSelection = -1;
		window = mnWindowCreate(TRANSFER_METHOD_WINDOW_X, 0,
			TRANSFER_METHOD_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
		upper = &state->upperWindow;
		*upper = window;
		/* "Transfer Method" */
		mnTextDrawMessage(
			MsgTransferMethod, window, TRANSFER_METHOD_LABEL_X, 4);
		state->lowerWindow = mnWindowCreate(TRANSFER_HELP_WINDOW_X, 5,
			TRANSFER_HELP_WINDOW_WIDTH, 7, WINDOW_FLAG_INSTANT);
	}
	result = mnIconMenuRun(0);
	if(mode != SEND_OPTION_TRANSFER_METHOD)
		syCloseEvent(mnPasswordLevelDrawHelp);
	else
		syCloseEvent(mnTransferMethodDrawHelp);
	mnWindowSpritePrepareForRedraw(*upper);
	mnWindowSpritePrepareForRedraw(state->lowerWindow);
	mnWindowClose(*upper, 2);
	mnWindowClose(state->lowerWindow, 2);
	sySynch(1);
	mnIconMenuClose();
	return result;
}

/* Command icons arranged in a row to choose from, such as Yes and No, each a
   picture of RESOURCE_COMMAND_ICONS (COMMAND_ICON). */

/* Selects a shop command (Buy, Sell, Artifacts, Repair), centred on the bottom
   row, starting on position `selection`. */
Sint32 mnSelectShopCommand(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_BUY);
	mnIconMenuAddIcon(COMMAND_ICON_SELL);
	mnIconMenuAddIcon(COMMAND_ICON_ARTIFACTS);
	mnIconMenuAddIcon(COMMAND_ICON_REPAIR);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, SHOP_COMMAND_LABEL_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

/* Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse),
   centred on the bottom row, starting on position `selection`. */
Sint32 mnSelectSanctumService(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_REVIVE);
	mnIconMenuAddIcon(COMMAND_ICON_CURE_POISON);
	mnIconMenuAddIcon(COMMAND_ICON_REPEL_EVIL);
	mnIconMenuAddIcon(COMMAND_ICON_REMOVE_CURSE);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, SANCTUM_SERVICE_LABEL_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

/* Selects Yes or No, returning YES or NO; cancelling returns NO. */
Sint32 mnYesNoMenuRun(
	Sint32 bottom, Sint32 msg, Sint32 labelWidth, Sint32 selection)
{
	Sint32 row;

	row = 0;

	mnIconMenuOpen();

	if(labelWidth == 0) {
		labelWidth = 3;
	}
	if(bottom != 0) {
		row = ICON_MENU_BOTTOM_ROW;
	}

	mnIconMenuAddIcon(COMMAND_ICON_YES);
	mnIconMenuAddIcon(COMMAND_ICON_NO);
	mnIconMenuCenter(row, labelWidth, msg);

	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}

/* Selects Yes or No with the icons from tile (x, y) and the name window after
   them, returning YES or NO; cancelling returns NO. */
Sint32 mnYesNoMenuRunAt(Sint32 x, Sint32 y, Sint32 selection)
{
	Sint32 message = MsgCommandYes;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_YES);
	mnIconMenuAddIcon(COMMAND_ICON_NO);
	mnIconMenuLayoutIcons(x, y, 3, message);
	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}

/* The width of the label window under Coins and Lucky Medals, fitted to each
   language's text. */
#define PAIR_WIDTH BY_LANGUAGE(7, 9, 9, 9, 10, 10)

/* Selects Coins or Lucky Medals, centred on the bottom row, starting on
   position `selection`. Returns the position chosen, or -1 when cancelled. */
Sint32 mnSelectCoinsOrMedals(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_COINS);
	mnIconMenuAddIcon(COMMAND_ICON_LUCKY_MEDALS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, PAIR_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

#undef TRANSFER_CABLE_HELP_SECOND_Y
#undef ICON_MENU_BOTTOM_ROW
#undef ICON_ROW_LINES
#undef ICON_MENU_MAX_ICONS
#undef PULSE_OVERHANG_X
#undef PULSE_OVERHANG_LINES
