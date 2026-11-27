/* The selection screen's row of icons, and the field menu. Part of window.c. */

#include "field/map_grid.h"

/* The selection row's icons stand 16 pixels apart. */
#define SELECTION_ICON_SPACING	 16
/* The row shows five entries at a time, from its seven icon records; the icons
   chosen so far take five more. */
#define SELECTION_ROW_SHOWN		 5
#define SELECTION_ROW_RECORDS	 7
#define SELECTION_CHOSEN_RECORDS 5

/* The records of the arrows at the row's ends, before the row's
   (SELECTION_MENU_STATE.records): an arrow's kind is TRUE while it shows, and
   its base counts the ticks it stays nudged out after a scroll. */
enum {
	SELECTION_ARROW_LEFT = 0,
	SELECTION_ARROW_RIGHT = 1,
};

/* The flags the name window opens with: WINDOW_FLAG_INSTANT, and a 4 that
   mnWindowCreate drops. A yes or no window opens with its kind,
   ICON_KIND_YES_NO, as its flags, which come to the same. */
#define NAME_WINDOW_FLAGS (WINDOW_FLAG_INSTANT | 4)

/* SELECTION_MENU_STATE.verticalScroll as the screen is made: the row's icons
   drawn 32 pixels lower, below the bottom of the screen, and the cursor held
   still, until mnSelectionMenuClear readies the list. */
#define SELECTION_ROW_HIDDEN 32

/* SELECTION_MENU_STATE.locked as the screen is made and once a yes or no window
   has opened: nonzero keeps a yes or no row and its window at the bottom of the
   screen, as mnRunSelectionForValue's 1 does. */
#define SELECTION_YES_NO_AT_BOTTOM 999

/* The large icon's zoom: it opens at this scale (in 8.8) and grows by as much a
   tick up to six times it. */
#define LARGE_ICON_ZOOM_STEP 40

/* One frame of an end arrow's animation: an 8x16 sprite of 256 colours. */
#define ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/* One frame of the pointing hand's animation: a 16x16 sprite of 256 colours. */
#define HAND_FRAME_BYTES  (4 * TILE_SIZE_8BPP)

/* Allocates and resets the selection screen: the row of icons the player picks
   from, with arrows at each end and a pointing hand over the chosen target. */
void mnSelectionMenuCreate(void)
{
	SELECTION_MENU_STATE *state;
	SELECTION_SPR		 *visual;
	const Uint8			 *tbl;
	Sint32				  i;
	Uint16				  clear = 0;

	state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SELECTION_MENU, sizeof(SELECTION_MENU_STATE));
	state->head = NULL;
	state->path = NULL;
	state->win = NULL;
	state->unread = clear;
	state->top = clear;
	/* The first list starts from its first entry. */
	state->cursorIdx = SELECTION_CURSOR_RESET;
	state->verticalScroll = SELECTION_ROW_HIDDEN;
	state->cnt = clear;
	state->locked = SELECTION_YES_NO_AT_BOTTOM;
	for(i = 0; i != SELECTION_CHOSEN_RECORDS; i++) {
		state->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
		state->records[i + SELECTION_RECORD_CHOSEN].kind = ICON_KIND_NONE;
	}
	/* The row has two icons more than the chosen ones; i is 5. */
	state->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
	state->records[i + SELECTION_RECORD_ROW + 1].kind = ICON_KIND_NONE;
	state->records[SELECTION_ARROW_LEFT].kind = FALSE;
	state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
	state->records[SELECTION_ARROW_LEFT].y = 0;
	state->records[SELECTION_ARROW_RIGHT].y = 0;
	tbl = MenuAnimatedCursorTiles;
	state->records[SELECTION_RECORD_HAND].slot = syVRAMCacheFindFreeEntry();
	state->records[SELECTION_RECORD_HAND].tile = syVRAMCacheLoad(
		state->records[SELECTION_RECORD_HAND].slot, HAND_FRAME_BYTES, tbl);
	state->records[SELECTION_RECORD_HAND].kind = 0;
	state->records[SELECTION_RECORD_HAND].scale = 0;
	state->records[SELECTION_RECORD_LARGE_ICON].kind = ICON_KIND_NONE;
	visual = &state->records[SELECTION_RECORD_HAND].spr;
	visual->mode = OBJ_MODE_NORMAL;
	visual->mosaic = 0;
	visual->cols = 1;
	visual->affine = SPRITE_AFFINE_OFF;
	visual->matrix = 0;
	visual->size = 1;
	visual->shape = SPRITE_SHAPE_SQUARE;
	visual->pri = 0;
}

/* Empties the selection screen's list of entries. The high cursor bit requests
   resetting both the list origin and cursor. */
void mnSelectionMenuClear(void)
{
	SELECTION_MENU_STATE *state;

	state = SelectionMenuStateWork;
	state->head = NULL;
	state->unread = 0;
	if(state->cursorIdx & SELECTION_CURSOR_RESET) {
		state->top = 0;
		state->cursorIdx = 0;
	}
	state->verticalScroll = 0;
	state->cnt = 0;
}

/* Adds an entry (kind and id) to the selection screen's list, up to 16. The
   count starts at 0 when the screen is created or cleared. */
void mnSelectionMenuAddEntry(Uint32 kind, Uint32 base)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	Uint16				  entryCnt = state->cnt;

	if(entryCnt != SELECTION_MENU_ENTRY_LIMIT) {
		state->kinds[entryCnt] = kind;
		state->bases[entryCnt] = base;
		state->cnt++;
	}
}

/* Creates icons for the first five listed entries from top, in a row along the
   bottom of the screen. */
void mnSelectionMenuBuildEntries(Uint32 unused)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	Uint32				  cnt = state->cnt;
	Uint32				  entryIdx;
	SELECTION_NODE		 *prev = 0;
	SELECTION_NODE		 *node;
	Sint32				  rowPos = 0;

	for(entryIdx = state->top; entryIdx < cnt; entryIdx++) {
		Sint32 base = state->bases[entryIdx];
		Uint32 kind = state->kinds[entryIdx];

		node = mnSelectionMenuFindFreeIcon(SELECTION_ICON_ROW);
		if(node == 0)
			break;
		mnSelectionMenuSetupIcon(kind, base, node, 0);
		if(state->head == 0) {
			state->head = node;
			node->prev = 0;
		} else {
			prev->next = node;
			node->prev = prev;
		}
		node->next = 0;
		rowPos++;
		prev = node;
		if(rowPos == SELECTION_ROW_SHOWN)
			break;
	}

	/* The row is centred on x 100, at y 140. */
	state->baseX = 100 - rowPos * SELECTION_ICON_SPACING / 2;
	state->baseY = 140;
	/* rowPos, the row's icon count, now steps an icon's spacing. */
	for(prev = state->head, rowPos = 0; prev != 0; prev = prev->next) {
		Sint32 x = state->baseX + rowPos;
		Sint32 y;

		prev->x = x;
		y = state->baseY;
		prev->y = y;
		prev->tgtX = x;
		prev->tgtY = y;
		/* While locked is 0 a yes or no row sits at the top of the screen. */
		if(prev->kind == ICON_KIND_YES_NO && state->locked == 0) {
			prev->y = 6;
			prev->tgtY = 6;
		}
		prev->dx = 0;
		prev->dy = 0;
		rowPos += SELECTION_ICON_SPACING;
	}
	mnSelectionMenuLoadLargeIcon();
}

#include "window/selection_menu_reserved_no_operation.c"

/* Returns a free selection icon record of `kind` (SELECTION_ICON_GROUP), or
   NULL. */
SELECTION_NODE *mnSelectionMenuFindFreeIcon(Sint32 kind)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	Sint32				  i;

	if(kind != SELECTION_ICON_ROW) {
		for(i = 0; i != SELECTION_CHOSEN_RECORDS; i++) {
			if(state->records[i + SELECTION_RECORD_CHOSEN].kind ==
				ICON_KIND_NONE)
				return &state->records[i + SELECTION_RECORD_CHOSEN];
		}
	} else {
		for(i = 0; i != SELECTION_ROW_RECORDS; i++) {
			if(state->records[i + SELECTION_RECORD_ROW].kind == ICON_KIND_NONE)
				return &state->records[i + SELECTION_RECORD_ROW];
		}
	}
	return NULL;
}

#include "window/selection_menu_drawing.c"

/* The selection screen's per-tick event: slides and draws the row of icons,
   the large icon over the chosen one, the end arrows, the icons already chosen
   and the pointing hand. */
void mnSelectionMenuDraw(void)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	SELECTION_SPR  *cursorEntry = &state->records[SELECTION_RECORD_HAND].spr;
	SELECTION_NODE *cursor = &state->records[SELECTION_RECORD_HAND];
	SELECTION_NODE *largeIcon = &state->records[SELECTION_RECORD_LARGE_ICON];
	SELECTION_NODE *node;
	SELECTION_SPR  *spr;
	SELECTION_SPR  *commandSpr;
	Sint32			entryIdx = 0;
	Sint32			slot;
	Sint32			axisStep;
	SCREEN_POINT	pos;

	node = state->head;
	/* Move and queue the visible row before drawing its overlays. */
	while(node != 0) {
		spr = &node->spr;
		spr->affine = SPRITE_AFFINE_OFF;
		spr->matrix = 0;
		spr->x = node->x;
		spr->y = node->y;
		slot = OAM_DEPTH_WINDOW;
		if(state->verticalScroll != 0) {
			spr->y += state->verticalScroll;
		} else if(node->x != node->tgtX) {
			if(node->dx > 0) {
				if(node->x + node->dx > node->tgtX) {
					node->x = node->tgtX;
				} else {
					node->x += node->dx;
				}
			} else {
				if(node->x + node->dx < node->tgtX) {
					node->x = node->tgtX;
				} else {
					node->x += node->dx;
				}
			}
			spr->x = node->x;
		} else if(entryIdx == state->cursorIdx) {
			slot = OAM_DEPTH_WINDOW + 1;
			if(cursor->kind != ICON_KIND_NONE) {
				if(btMotionProjectScaledPosStub(
					   node->base, syScreenPointCoordinates(&pos)) != -1) {
					cursor->tgtX = pos.x;
					cursor->tgtY = pos.y;
					if(cursor->scale == 0) {
						cursor->x = pos.x;
						cursor->y = pos.y;
						cursor->scale = 1;
					}
				}
			}
		}
		if(node->scale != 0) {
			if(gmFlagGetStub(FLAG_IN_BATTLE) != 0) {
				if(state->records[SELECTION_RECORD_HAND].kind ==
					ICON_KIND_COMMAND) {
					spr->mode = OBJ_MODE_SEMI_TRANSPARENT;
				} else {
					spr->mode = OBJ_MODE_NORMAL;
				}
				if(node->kind == ICON_KIND_COMMAND) {
					spr->mode = OBJ_MODE_SEMI_TRANSPARENT;
				}
			}
			syOAMAddSprAtDepth(spr, slot);
		}
		node = node->next;
		entryIdx++;
	}
	/* The selected command's large icon owns a separate cached tile block. */
	if(largeIcon->kind != ICON_KIND_NONE) {
		node = mnSelectionMenuGetCursorNode(state);
		commandSpr = &largeIcon->spr;
		commandSpr->mode = OBJ_MODE_NORMAL;
		commandSpr->affine = SPRITE_AFFINE_OFF;
		commandSpr->matrix = 0;
		commandSpr->mosaic = 0;
		commandSpr->cols = 1;
		commandSpr->size = 2;
		commandSpr->shape = SPRITE_SHAPE_SQUARE;
		commandSpr->pri = 0;
		commandSpr->tile = largeIcon->tile;
		/* A 32x32 sprite (size 2) over the icon, bobbing by half the cursor's
		   bob; while it zooms in, the double-size area moves it 16 up and left,
		   wrapped as the 9-bit x and the 8-bit y wrap. */
		commandSpr->x = node->x - 4;
		commandSpr->y =
			node->y + (MenuCursorBobOffsets[(FrameCount >> 1) & 15] >> 1) - 4;
		if(largeIcon->scale != largeIcon->scaleEnd) {
			state->transform.scaleX = largeIcon->scale;
			state->transform.scaleY = largeIcon->scale;
			state->transform.ang = 0;
			commandSpr->matrix =
				syOAMMatrixAllocFromTransform(&state->transform);
			commandSpr->affine = SPRITE_AFFINE_DOUBLE_SIZE;
			commandSpr->x += (Uint16)-16;
			commandSpr->y += (Uint8)-16;
			largeIcon->scale += largeIcon->scaleStep;
		}
		if(gmFlagGetStub(FLAG_IN_BATTLE) != 0) {
			commandSpr->mode = OBJ_MODE_SEMI_TRANSPARENT;
		}
		syOAMAddSprAtDepth(commandSpr, OAM_DEPTH_WINDOW + 8);
	}
	mnSelectionMenuDrawArrow(state, 0);
	mnSelectionMenuDrawArrow(state, 1);
	/* Chosen entries gather at the top-left independently of the scrolling
	   row. */
	node = state->path;
	while(node != 0) {
		spr = &node->spr;
		if(node->x != node->tgtX) {
			node->x += node->dx;
		}
		if(node->y != node->tgtY) {
			node->y += node->dy;
		}
		spr->x = node->x;
		spr->y = node->y;
		if(node->scale != node->scaleEnd) {
			node->scale += node->scaleStep;
			state->transform.scaleX = node->scale;
			state->transform.scaleY = node->scale;
			state->transform.ang = 0;
			spr->matrix = syOAMMatrixAllocFromTransform(&state->transform);
			spr->affine = SPRITE_AFFINE_DOUBLE_SIZE;
			spr->x += (Uint16)-8;
			spr->y += (Uint8)-8;
		} else {
			spr->affine = SPRITE_AFFINE_OFF;
			spr->matrix = 0;
		}
		if(gmFlagGetStub(FLAG_IN_BATTLE) != 0) {
			if(state->records[SELECTION_RECORD_HAND].kind ==
				ICON_KIND_COMMAND) {
				spr->mode = OBJ_MODE_SEMI_TRANSPARENT;
			} else {
				spr->mode = OBJ_MODE_NORMAL;
			}
			if(node->kind == ICON_KIND_COMMAND) {
				spr->mode = OBJ_MODE_SEMI_TRANSPARENT;
			}
		}
		syOAMAddSprAtDepth(spr, OAM_DEPTH_WINDOW);
		node = node->next;
	}
	/* Point at the selected battle target, smoothing each axis separately: the
	   hand covers half the distance left each tick. */
	if(cursor->kind != ICON_KIND_NONE) {
		entryIdx = (Sint32)MenuAnimatedCursorTiles;
		{
			void *src = (void *)(entryIdx + (((FrameCount >> 2) & 15) << 8));

			cursorEntry->tile =
				syVRAMCacheLoad(cursor->slot, HAND_FRAME_BYTES, src);
		}
		if(cursor->tgtX != cursor->x) {
			axisStep = (cursor->tgtX - cursor->x) >> 1;
			if(axisStep != 0) {
				cursor->x += axisStep;
			} else {
				cursor->x = cursor->tgtX;
			}
		}
		if(cursor->tgtY != cursor->y) {
			if((axisStep = (cursor->tgtY - cursor->y) >> 1) != 0) {
				cursor->y += axisStep;
			} else {
				cursor->y = cursor->tgtY;
			}
		}
		cursorEntry->y =
			cursor->y + MenuCursorBobOffsets[(FrameCount >> 2) & 15] - 32;
		cursorEntry->x = cursor->x - 4;
		if(gmFlagGetStub(FLAG_IN_BATTLE) != 0) {
			if(state->records[SELECTION_RECORD_HAND].kind ==
				ICON_KIND_COMMAND) {
				cursorEntry->mode = OBJ_MODE_SEMI_TRANSPARENT;
			} else {
				cursorEntry->mode = OBJ_MODE_NORMAL;
			}
		}
		syOAMAddSprAtDepth(cursorEntry, OAM_DEPTH_WINDOW + 8);
	}
	state->stat += 1;
}

/* Draws the selection screen's left (index 0) or right (1) arrow, nudged
   outwards while it flashes after a scroll: the record's base holds the ticks
   left of the nudge (8 after each scroll) and is the nudge in pixels. */
void mnSelectionMenuDrawArrow(SELECTION_MENU_STATE *state, Sint32 side)
{
	Uint32		   anmStep = (FrameCount >> 2) & 7;
	SELECTION_SPR *arrow;
	const Uint8	  *arrowTiles;
	const Uint8	  *tmp;

	if(state->records[side].kind == ICON_KIND_NONE)
		return;
	arrow = &state->records[side].spr;
	arrow->x = state->records[side].x;
	arrow->y = state->records[side].y;
	if(side != 0) {
		arrowTiles = MenuCursorActorTiles;
		if(state->records[SELECTION_ARROW_RIGHT].base != 0)
			arrow->x = arrow->x + state->records[SELECTION_ARROW_RIGHT].base;
	} else {
		arrowTiles = MenuCursorLeftActorTiles;
		if(state->records[SELECTION_ARROW_LEFT].base != 0)
			arrow->x = arrow->x - state->records[SELECTION_ARROW_LEFT].base;
	}
	tmp = arrowTiles + anmStep * ARROW_FRAME_BYTES;
	arrow->tile =
		syVRAMCacheLoad(state->records[side].slot, ARROW_FRAME_BYTES, tmp);
	if(gmFlagGetStub(FLAG_IN_BATTLE)) {
		if(state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND)
			arrow->mode = OBJ_MODE_SEMI_TRANSPARENT;
		else
			arrow->mode = OBJ_MODE_NORMAL;
	}
	syOAMAddSprAtDepth(arrow, OAM_DEPTH_WINDOW - 2);
	if(state->records[side].base != 0)
		state->records[side].base--;
}

/* Opens or refits the name window above the selection row and writes the name
   of the icon under the cursor, or that nothing is known (ICON_KIND_ABILITY) or
   held (ICON_KIND_ITEM). */
void mnSelectionMenuOpenNameWindow(Sint32 mode, Uint32 cnt)
{
	SELECTION_MENU_STATE *state;
	SELECTION_NODE		 *node;
	WINDOW				**slot;
	WINDOW				 *win;

	state = SelectionMenuStateWork;
	node = mnSelectionMenuGetCursorNode(state);
	slot = &state->win;
	win = *slot;
	if(win == 0) {
		/* A yes or no window, 5 by 3 tiles, from column 17 at the bottom, or
		   the top while locked is 0. */
		if(mode == ICON_KIND_YES_NO) {
			if(state->locked != 0) {
				*slot = mnWindowCreate(17, 17, 5, 3, mode);
			} else {
				*slot = mnWindowCreate(17, 0, 5, 3, mode);
			}
			state->verticalScroll = 0;
			state->locked = SELECTION_YES_NO_AT_BOTTOM;
		} else {
			*slot = mnWindowCreate(
				19 + ((9 - cnt) >> 1), 17, cnt + 2, 3, NAME_WINDOW_FLAGS);
		}
		mnWindowSprPrepareForRedraw(state->win);
	} else {
		if(cnt != 0 && win->width != cnt + 2) {
			mnWindowClose(win, WINDOW_FLAG_INSTANT);
			*slot = mnWindowCreate(
				19 + ((9 - cnt) >> 1), 17, cnt + 2, 3, NAME_WINDOW_FLAGS);
		}
		mnWindowSprPrepareForRedraw(state->win);
	}
	if(state->cnt != 0) {
		mnTextDrawMessageAsTiles(node->msg, state->win, 0, 0);
	} else {
		switch(mode) {
		case ICON_KIND_ABILITY:
			mnTextDrawMessageAsTiles(MsgAbilityNotKnown, state->win, 0, 0);
			break;
		case ICON_KIND_ITEM:
			mnTextDrawMessageAsTiles(MsgItemNotHeld, state->win, 0, 0);
			break;
		}
	}
}

/* Closes the selection screen and frees its icons, tiles and state block. */
void mnSelectionMenuClose(void)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	SELECTION_NODE		 *node;

	mnSelectionMenuStopDrawing();
	mnWindowClose(state->win, WINDOW_FLAG_INSTANT);
	sySynch(1);
	node = state->head;
	while(node != 0) {
		if(node->kind != ICON_KIND_NONE) {
			syVRAMCacheFreeEntry(node->slot);
			node->kind = ICON_KIND_NONE;
		}
		node = node->next;
	}
	node = state->path;
	while(node != 0) {
		if(node->kind != ICON_KIND_NONE) {
			syVRAMCacheFreeEntry(node->slot);
			node->kind = ICON_KIND_NONE;
		}
		node = node->next;
	}
	mnSelectionMenuFreeLargeIcon();
	/* An arrow's y is nonzero once mnSelectionMenuSetupArrow has given it a
	   slot. */
	if(state->records[SELECTION_ARROW_LEFT].y != 0) {
		syVRAMCacheFreeEntry(state->records[SELECTION_ARROW_LEFT].slot);
		if(state->records[SELECTION_ARROW_LEFT].y != 0) {
			syVRAMCacheFreeEntry(state->records[SELECTION_ARROW_RIGHT].slot);
		}
	}
	syVRAMCacheFreeEntry(state->records[SELECTION_RECORD_HAND].slot);
	syHeapSlotRelease(HEAP_SLOT_SELECTION_MENU);
}

/* Moves the row of icons to start at pixel (x, y), 16 pixels apart. */
void mnSelectionMenuSetPos(Uint32 x, Uint32 y)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	SELECTION_NODE		 *node;

	state->baseX = x;
	state->baseY = y;
	node = state->head;
	while(node != 0) {
		node->x = x;
		node->tgtX = x;
		node->y = y;
		node->tgtY = y;
		node = node->next;
		x += SELECTION_ICON_SPACING;
	}
}

/* Sets up both end arrows of the selection row. */
void mnSetupSelectionBothSides(void)
{
	SELECTION_MENU_STATE *state;

	state = SelectionMenuStateWork;
	mnSelectionMenuSetupArrow(state, 0);
	mnSelectionMenuSetupArrow(state, 1);
}

/* Sets up the left (index 0) or right (1) arrow of the selection row, shown
   when there are entries beyond that end. */
void mnSelectionMenuSetupArrow(SELECTION_MENU_STATE *state, Sint32 side)
{
	SELECTION_SPR *arrow = &state->records[side].spr;
	const Uint8	  *arrowTiles = 0;
	Uint32		   cnt;

	state->records[side].kind = FALSE;
	if(side != 0) {
		const Uint8 *right = MenuCursorActorTiles;

		cnt = state->cnt;
		if(state->top != 0)
			cnt -= state->top;
		if(cnt > SELECTION_ROW_SHOWN) {
			state->records[side].kind = TRUE;
			cnt = SELECTION_ROW_SHOWN;
		}
		arrowTiles = right;
		/* Just past the last of the at most five icons shown. */
		state->records[SELECTION_ARROW_RIGHT].x =
			state->baseX + SELECTION_ICON_SPACING * (cnt - 1) + 17;
	} else {
		arrowTiles = MenuCursorLeftActorTiles;
		state->records[SELECTION_ARROW_LEFT].x = state->baseX - 9;
		if(state->top != 0)
			state->records[SELECTION_ARROW_LEFT].kind = TRUE;
	}
	if(state->records[side].y == 0) {
		state->records[side].slot = syVRAMCacheFindFreeEntry();
		state->records[side].tile = syVRAMCacheLoad(
			state->records[side].slot, ARROW_FRAME_BYTES, arrowTiles);
		state->records[side].y = state->baseY;
		state->records[side].base = 0;
		arrow->mode = OBJ_MODE_NORMAL;
		arrow->mosaic = 0;
		arrow->cols = 1;
		arrow->affine = SPRITE_AFFINE_OFF;
		arrow->matrix = 0;
		arrow->size = 0;
		arrow->shape = SPRITE_SHAPE_TALL;
		arrow->pri = 0;
	}
}

/* Returns the node at the selection screen's cursor index. cursorIndex is
   relative to head, not the number of queued entries. */
SELECTION_NODE *mnSelectionMenuGetCursorNode(SELECTION_MENU_STATE *state)
{
	SELECTION_NODE *node = state->head;
	Sint32			i;

	for(i = 0; i != state->cursorIdx; ++i) {
		node = node->next;
	}
	return node;
}

/* The selection row's mode that only B leaves: the cursor stays and A chooses
   nothing. */
#define SELECTION_FIXED 999

/* The selection row's status while it scrolls, and once it shows. */
#define SELECTION_STATUS_SCROLLING 33
#define SELECTION_STATUS_SHOWN	   1

/* The row's last place, the fifth: the cursor stepping onto it with more
   entries to come scrolls the row instead. */
#define SELECTION_LAST_PLACE 4
#define ARROW_SCROLL_TICKS	 8

/* Runs the selection row until the player chooses (the chosen entry's index)
   or, when mode allows it, cancels with B (-1). */
Sint32 mnSelectionMenuRun(Sint32 mode)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;

	mnSelectionMenuHighlightIcon(state, 0);
	for(;;) {
		sySynch(1);
		/* No input while the row is pushed down. */
		if(state->verticalScroll != 0) {
			continue;
		}
		if(mode != SELECTION_FIXED) {
			if(PerRepeat & KEY_RIGHT) {
				mnSelectionMenuStepRight(state);
			} else if(PerRepeat & KEY_LEFT) {
				mnSelectionMenuStepLeft(state);
			} else if(PerPush & KEY_A) {
				return mnConfirmSelection(state);
			}
		}
		if(mode != 0 && (PerPush & KEY_B)) {
			return -1;
		}
	}
}

/* mnSelectionMenuRun with the menu's sounds: the cursor's move, a confirm, and
   on a yes or no row a cancel for no; B, when mode allows it, cancels with its
   sound. */
Uint32 mnSelectionMenuRunWithSounds(Uint32 mode)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	Uint32				  result;

	for(;;) {
		sySynch(1);
		if(state->verticalScroll != 0) {
			continue;
		}

		if(mode != SELECTION_FIXED) {
			if(PerRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				mnSelectionMenuStepRight(state);
			} else if(PerRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				mnSelectionMenuStepLeft(state);
			}

			if(PerPush & KEY_A) {
				/* On a yes or no row, entry 0 (yes) sounds as a confirm and the
				   other as a cancel. */
				result = state->top + state->cursorIdx;
				if(state->head->kind == ICON_KIND_YES_NO) {
					if(result == 0)
						sySndPlayStub(SE_MENU_CONFIRM);
					else
						sySndPlayStub(SE_MENU_CANCEL);
				} else {
					sySndPlayStub(SE_MENU_CONFIRM);
				}
				return result;
			}
		}

		if(mode != 0 && (PerPush & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			return -1;
		}
	}
}

/* Moves the cursor one entry right, scrolling the row at its fourth place. This
   variant stops at the end; mnSelectionMenuStepRight also wraps around. */
UNUSED void mnMoveSelectionForward(SELECTION_MENU_STATE *state)
{
	Uint32 nextEntry = state->top + state->cursorIdx + 1;

	if(nextEntry == state->cnt) {
		return;
	}
	mnSelectionMenuRedrawIcon(state, state->cursorIdx);
	state->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	state->cursorIdx++;
	if(state->cursorIdx == SELECTION_LAST_PLACE && nextEntry + 1 < state->cnt) {
		state->cursorIdx--;
		state->records[SELECTION_ARROW_RIGHT].base = ARROW_SCROLL_TICKS;
		state->top++;
		mnSelectionMenuScroll(state, TRUE);
		if(state->top + state->cursorIdx + 2 == state->cnt) {
			state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
		}
		state->records[SELECTION_ARROW_LEFT].kind = TRUE;
	}
	state->stat = SELECTION_STATUS_SHOWN;
	mnSelectionMenuHighlightIcon(state, state->cursorIdx);
	sySynch(1);
	mnSelectionMenuOpenNameWindow(state->head->kind, 0);
	sySynch(1);
}

/* Moves the cursor one entry left, scrolling the row at its second place. This
   variant stops at the beginning; mnSelectionMenuStepLeft also wraps around. */
UNUSED void mnMoveSelectionBackward(SELECTION_MENU_STATE *state)
{
	Uint32 cursorIdx;

	if(state->top == 0 && state->cursorIdx == 0) {
		return;
	}
	mnSelectionMenuRedrawIcon(state, state->cursorIdx);
	state->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	cursorIdx = state->cursorIdx;
	if(cursorIdx == 1 && state->top != 0) {
		state->records[SELECTION_ARROW_LEFT].base = ARROW_SCROLL_TICKS;
		state->top--;
		mnSelectionMenuScroll(state, FALSE);
		if(state->top == 0) {
			state->records[SELECTION_ARROW_LEFT].kind = FALSE;
		}
		state->records[SELECTION_ARROW_RIGHT].kind = cursorIdx;
	} else {
		state->cursorIdx--;
	}
	state->stat = SELECTION_STATUS_SHOWN;
	mnSelectionMenuHighlightIcon(state, state->cursorIdx);
	sySynch(1);
	mnSelectionMenuOpenNameWindow(state->head->kind, 0);
	sySynch(1);
}

/* Moves the cursor one entry right, scrolling the row at its edge and wrapping
   from the last entry to the first. */
void mnSelectionMenuStepRight(SELECTION_MENU_STATE *state)
{
	SELECTION_NODE *node;
	Uint32			entryIdx;
	Sint32			x;
	Uint32			end;

	mnSelectionMenuRedrawIcon(state, state->cursorIdx);
	state->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	state->cursorIdx++;
	/* With more entries than the five shown, stepping past the last gathers the
	   icons at the row's start (12 pixels a tick), refills them from the first
	   entry and fans them out again; stepping onto the fifth place with more to
	   come scrolls the row instead, the cursor staying on the fourth. */
	if(state->cnt > SELECTION_ROW_SHOWN) {
		end = state->top + state->cursorIdx;
		if(end == state->cnt) {
			node = state->head;
			state->records[SELECTION_ARROW_LEFT].kind = FALSE;
			while(node->next != NULL) {
				node->tgtX = state->baseX;
				node->tgtY = state->baseY;
				node->dx = -12;
				node = node->next;
			}
			node->tgtX = state->baseX;
			node->tgtY = state->baseY;
			node->dx = -12;
			while(node->tgtX != node->x)
				sySynch(1);
			node = state->head;
			if(node != NULL) {
				entryIdx = 0;
				do {
					mnSelectionMenuSetupIcon(state->kinds[entryIdx],
						state->bases[entryIdx], node, 1);
					node = node->next;
					entryIdx++;
				} while(node != NULL);
			}
			state->cursorIdx = 0;
			state->top = 0;
			node = state->head;
			x = node->x + SELECTION_ICON_SPACING;
			node = node->next;
			while(node != NULL) {
				node->tgtX = x;
				node->dx = 12;
				node = node->next;
				x += SELECTION_ICON_SPACING;
			}
			state->records[SELECTION_ARROW_RIGHT].kind = TRUE;
		} else if(state->cursorIdx == SELECTION_LAST_PLACE &&
			end + 1 < state->cnt) {
			state->cursorIdx--;
			state->records[SELECTION_ARROW_RIGHT].base = ARROW_SCROLL_TICKS;
			state->top++;
			mnSelectionMenuScroll(state, TRUE);
			if(state->top + state->cursorIdx + 2 == state->cnt)
				state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
			state->records[SELECTION_ARROW_LEFT].kind = TRUE;
		}
	} else if(state->cursorIdx == state->cnt) {
		state->cursorIdx = 0;
	}
	state->stat = SELECTION_STATUS_SHOWN;
	mnSelectionMenuHighlightIcon(state, state->cursorIdx);
	sySynch(1);
	mnSelectionMenuOpenNameWindow(state->head->kind, 0);
	sySynch(1);
}

/* Moves the cursor one entry left, scrolling the row at its edge and wrapping
   from the first entry to the last. */
void mnSelectionMenuStepLeft(SELECTION_MENU_STATE *state)
{
	SELECTION_NODE *node;
	Sint32			i;
	Sint32			kind;

	mnSelectionMenuRedrawIcon(state, state->cursorIdx);
	state->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	/* As mnSelectionMenuStepRight, mirrored: from the second place with entries
	   before it the row scrolls back; from the first entry the icons gather at
	   the row's last place (12 pixels a tick), refill from the last five
	   entries and fan out again, with the cursor on the last. */
	if(state->cnt > SELECTION_ROW_SHOWN) {
		if((state->cursorIdx | state->top) != 0) {
			if(state->cursorIdx == 1 && state->top != 0) {
				state->records[SELECTION_ARROW_LEFT].base = ARROW_SCROLL_TICKS;
				state->top--;
				mnSelectionMenuScroll(state, FALSE);
				if(state->top == 0)
					state->records[SELECTION_ARROW_LEFT].kind = FALSE;
				state->records[SELECTION_ARROW_RIGHT].kind = TRUE;
			} else {
				state->cursorIdx--;
			}
		} else {
			node = state->head;
			i = SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
			/* kind, which later takes the head's kind, carries the 0 first. */
			kind = 0;
			state->records[SELECTION_ARROW_RIGHT].kind = kind;
			while(node->next != NULL) {
				node->tgtX = node->x + i;
				node->dx = 12;
				node = node->next;
				i -= SELECTION_ICON_SPACING;
			}
			node = state->head;
			while(node->x != node->tgtX)
				sySynch(1);
			i = 0;
			while(i != state->cnt - SELECTION_ROW_SHOWN)
				i++;
			node = state->head;
			state->top = i;
			state->cursorIdx = SELECTION_LAST_PLACE;
			while(node != NULL) {
				mnSelectionMenuSetupIcon(
					state->kinds[i], state->bases[i], node, 1);
				node = node->next;
				i++;
			}
			node = state->head;
			i = state->baseX;
			while(node->next != NULL) {
				node->tgtX = i;
				node->dx = -12;
				node = node->next;
				i += SELECTION_ICON_SPACING;
			}
			state->records[SELECTION_ARROW_LEFT].kind = TRUE;
		}
	} else if(state->cursorIdx != 0) {
		state->cursorIdx--;
	} else {
		state->cursorIdx = state->cnt - 1;
	}
	state->stat = SELECTION_STATUS_SHOWN;
	mnSelectionMenuHighlightIcon(state, state->cursorIdx);
	sySynch(1);
	kind = state->head->kind;
	mnSelectionMenuOpenNameWindow(kind, 0);
	sySynch(1);
}

/* Redraws the command icon at row place rowIdx in its plain state, in its own
   VRAM cache entry. Other kinds of icon are left alone. */
void mnSelectionMenuRedrawIcon(SELECTION_MENU_STATE *state, Uint32 rowIdx)
{
	SELECTION_NODE *node = state->head;
	Uint32			tile;
	Uint32			value;

	while(rowIdx != 0) {
		rowIdx--;
		node = node->next;
	}
	if(node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
		Uint32 first = node->msg - MsgCommandName;

		value = node->slot;
		mnIconBuildCommandIconTiles(first, 0, &value, &tile, 1);
	}
}

/* Redraws the command icon at row place rowIdx and loads its large icon. */
void mnSelectionMenuHighlightIcon(SELECTION_MENU_STATE *state, Uint32 rowIdx)
{
	SELECTION_NODE *node = state->head;
	Uint32			tile;
	Uint32			value;

	while(rowIdx != 0) {
		rowIdx--;
		node = node->next;
	}
	if(node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
		Uint32 resource = node->msg - MsgCommandName;

		value = node->slot;
		mnIconBuildCommandIconTiles(resource, 0, &value, &tile, 1);
		mnSelectionMenuLoadLargeIcon();
	}
}

/* Stops the battle engine's highlight of units (btHighlightSetModesStub with
   mode 0); mnConfirmSelection calls it as an entry is chosen. */
void mnClearBattleHighlight(SELECTION_MENU_STATE *state)
{
	Uint16			data[6];
	SELECTION_NODE *node = state->head;
	Sint32			cnt = 0;

	while(node != 0) {
		node = node->next;
		cnt++;
	}
	data[cnt] = BATTLE_UNIT_LIST_END;
	btHighlightSetModesStub(data, 0);
}

/* Scrolls the row one entry: forward brings an icon in from the right as the
   leftmost shrinks away, and back the reverse; waits until it has moved. */
void mnSelectionMenuScroll(SELECTION_MENU_STATE *state, Sint32 forward)
{
	SELECTION_NODE *node;
	SELECTION_NODE *last;
	SELECTION_NODE *first;
	Sint32			base;
	Sint32			kind;
	Uint32			entryIdx;
	Sint32			rowX;
	Sint32			rowY;

	if(forward != 0) {
		entryIdx = state->top + SELECTION_LAST_PLACE;
		base = state->bases[entryIdx];
		kind = state->kinds[entryIdx];
		node = mnSelectionMenuFindFreeIcon(SELECTION_ICON_ROW);
		if(node == NULL)
			return;
		mnSelectionMenuSetupIcon(kind, base, node, 0);
		rowX = state->baseX;
		node->x = rowX + (SELECTION_LAST_PLACE + 1) * SELECTION_ICON_SPACING;
		rowY = state->baseY;
		node->tgtX = rowX + SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
		node->y = rowY;
		node->tgtY = rowY;
		node->scaleStep = Q_8_8(1) / 8;
		node->scale = Q_8_8(1) / 8;
		node->scaleEnd = Q_8_8(1);
		node->dx = -2;
		last = node;
		node = state->head;
		node->scaleStep = -Q_8_8(1) / 8;
		node->scaleEnd = 0;
		for(;;) {
			node->tgtX = node->x - SELECTION_ICON_SPACING;
			node->dx = -2;
			if(node->next == NULL)
				break;
			node = node->next;
		}
		node->next = last;
		last->next = NULL;
		last->prev = node;
		node = state->head;
		do {
			sySynch(1);
		} while(node->scale != 0);
		state->head = node->next;
		syVRAMCacheFreeEntry(node->slot);
		node->kind = ICON_KIND_NONE;
		node = node->next;
		node->prev = NULL;
	} else {
		entryIdx = state->top;
		base = state->bases[entryIdx];
		kind = state->kinds[entryIdx];
		node = mnSelectionMenuFindFreeIcon(SELECTION_ICON_ROW);
		if(node == NULL)
			return;
		mnSelectionMenuSetupIcon(kind, base, node, 0);
		node->x = state->baseX - SELECTION_ICON_SPACING;
		node->y = state->baseY;
		node->tgtY = node->y;
		node->dx = 2;
		node->scale = Q_8_8(1) / 8;
		node->scaleStep = Q_8_8(1) / 8;
		node->tgtX = node->x + SELECTION_ICON_SPACING;
		node->scaleEnd = Q_8_8(1);
		first = node;
		node = state->head;
		node->prev = first;
		first->next = node;
		first->prev = NULL;
		state->head = first;
		node = first;
		for(;;) {
			node->tgtX = node->x + SELECTION_ICON_SPACING;
			node->dx = 2;
			if(node->next == NULL)
				break;
			node = node->next;
		}
		node->scaleEnd = 0;
		node->scaleStep = -Q_8_8(1) / 8;
		node = state->head;
		do {
			sySynch(1);
		} while(node->scale != Q_8_8(1));
		while(node->next != NULL)
			node = node->next;
		syVRAMCacheFreeEntry(node->slot);
		node->kind = ICON_KIND_NONE;
		node->prev->next = NULL;
	}
}

/* Loads the icon of an entry of kind mode (ICON_KIND_COMMAND, ICON_KIND_YES_NO,
   ICON_KIND_ITEM, ICON_KIND_ABILITY or ICON_KIND_TABLE_ENTRY) into a free
   sprite tile slot and returns it, or -1. */
Sint32 mnSelectionIconLoad(Sint32 mode, Sint32 value)
{
	Sint32 output;
	Sint32 result = -1;

	switch(mode) {
	case ICON_KIND_COMMAND:
	case ICON_KIND_YES_NO:
		mnIconBuildCommandIconTiles(value, 0, &result, &output, 0);
		break;
	case ICON_KIND_ITEM:
		result = syVRAMCacheFindFreeEntry();
		if(result == VRAM_CACHE_ENTRY_COUNT)
			return -1;
		mnItemIconLoadLayersIntoSlot(
			value, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN, result);
		break;
	case ICON_KIND_TABLE_ENTRY:
		result = syVRAMCacheFindFreeEntry();
		if(result == VRAM_CACHE_ENTRY_COUNT)
			return -1;
		mnTitleLoadFirstTile(value, 0, result);
		break;
	case ICON_KIND_ABILITY:
		mnAbilityBuildIconTiles(value, 1, &result, &output, 0);
		break;
	}
	return result;
}

/* Loads the icon of entry, of kind kind, into sprite tile slot slot (a free
   slot when -1), drawn from baseIcon where the kind takes one, and returns the
   slot, or -1 when none was free. */
Sint32 mnSelectionIconLoadIntoSlot(
	Sint32 kind, Sint32 entry, Sint32 slot, Sint32 baseIcon)
{
	Sint32 output;
	Sint32 original = slot;

	if(slot == -1) {
		slot = syVRAMCacheFindFreeEntry();
		if(slot == VRAM_CACHE_ENTRY_COUNT)
			return original;
	}

	switch(kind) {
	case ICON_KIND_COMMAND:
	case ICON_KIND_YES_NO:
		mnIconBuildCommandIconTiles(entry, baseIcon, &slot, &output, 1);
		break;
	case ICON_KIND_ITEM:
		mnItemIconLoadLayersIntoSlot(entry,
			ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN | ICON_CURSED,
			slot);
		break;
	case ICON_KIND_EQUIPMENT_ITEM:
		mnItemIconLoadLayersIntoSlot(
			entry, ICON_COUNT_ABOVE_ONE | ICON_BROKEN | ICON_CURSED, slot);
		break;
	case ICON_KIND_ABILITY:
		mnAbilityBuildIconTiles(entry, baseIcon, &slot, &output, 1);
		break;
	case ICON_KIND_PATTERN:
		mnIconBuildMiscIconTiles(entry, 0, slot);
		break;
	case ICON_KIND_TABLE_ENTRY:
		mnTitleLoadFirstTile(entry, 0, slot);
		break;
	}

	return slot;
}

/* Sets up a selection icon for an entry: its icon tiles (in its old slot when
   reuse is set), its name message and its sprite. kind must select a command,
   item or Psynergy with the matching base ID. */
void mnSelectionMenuSetupIcon(
	Uint32 kind, Sint32 base, SELECTION_NODE *node, Sint32 reuse)
{
	Sint32		   cacheSlot;
	Sint32		   tile;
	SELECTION_SPR *spr;

	switch(kind) {
	case ICON_KIND_COMMAND:
	case ICON_KIND_YES_NO:
		if(reuse != 0)
			cacheSlot = node->slot;
		mnIconBuildCommandIconTiles(base, 0, &cacheSlot, &tile, reuse);
		node->msg = base + MsgCommandName;
		break;
	case ICON_KIND_ITEM:
		if(reuse != 0)
			cacheSlot = node->slot;
		mnIconBuildItemIconTiles(base, 1, &cacheSlot, &tile, reuse);
		node->msg = base + MsgItemName;
		break;
	case ICON_KIND_ABILITY:
		if(reuse != 0)
			cacheSlot = node->slot;
		mnAbilityBuildIconTiles(base, 1, &cacheSlot, &tile, reuse);
		node->msg = base + MsgAbilityName;
		break;
	}

	node->base = base;
	node->slot = cacheSlot;
	node->tile = tile;
	node->kind = kind;
	node->scale = Q_8_8(1);
	node->scaleEnd = Q_8_8(1);

	spr = &node->spr;
	spr->mode = OBJ_MODE_NORMAL;
	spr->cols = 0;
	spr->mosaic = 0;

	spr->size = 1;
	spr->shape = SPRITE_SHAPE_SQUARE;
	spr->pal = 0;
	spr->tile = tile;
	spr->pri = 0;
}

/* Chooses the entry under the cursor: the other icons gather into it and
   vanish, and it joins the chosen icons at the top left. */
Uint32 mnConfirmSelection(SELECTION_MENU_STATE *state)
{
	SELECTION_NODE *node;
	SELECTION_NODE *other = NULL;
	Uint32			nodeIdx = 0;
	Uint32			chosenIdx;
	SELECTION_SPR  *spr;

	chosenIdx = state->top + state->cursorIdx;
	mnClearBattleHighlight(state);
	mnSelectionMenuRedrawIcon(state, state->cursorIdx);
	state->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	state->records[SELECTION_ARROW_LEFT].kind = FALSE;
	state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
	state->records[SELECTION_RECORD_HAND].kind = 0;
	state->records[SELECTION_RECORD_HAND].scale = 0;
	mnSelectionMenuFreeLargeIcon();
	node = state->head;
	for(; node != NULL && state->cursorIdx != nodeIdx; nodeIdx++) {
		node = node->next;
	}
	node->homeX = node->x;
	node->homeY = node->y;
	/* The others slide halfway in each of two ticks, then vanish. */
	for(other = state->head; other != NULL; other = other->next) {
		if(other != node) {
			other->tgtX = node->x;
			other->dx = (node->x - other->x) >> 1;
		}
	}
	sySynch(2);
	for(other = state->head; other != NULL; other = other->next) {
		if(other != node) {
			syVRAMCacheFreeEntry(other->slot);
			other->kind = ICON_KIND_NONE;
		}
	}
	state->head = node;
	node->prev = NULL;
	node->next = NULL;
	/* The chosen icons line up from x 4, 16 pixels apart; the new one goes
	   after the last, reaching its place over two ticks, and its record moves
	   to the chosen list. */
	node->tgtX = 4;
	for(other = state->path, nodeIdx = 0; other != NULL; other = other->next) {
		node->tgtX += SELECTION_ICON_SPACING;
		nodeIdx++;
	}
	state->pathTop[nodeIdx] = state->top;
	state->pathCursor[nodeIdx] = state->cursorIdx;
	node->dx = (node->tgtX - node->x) >> 1;
	state->unread = 0;
	state->cursorIdx |= SELECTION_CURSOR_RESET;
	sySynch(2);
	other = mnSelectionMenuFindFreeIcon(SELECTION_ICON_CHOSEN);
	other->kind = node->kind;
	other->msg = node->msg;
	other->base = node->base;
	other->slot = node->slot;
	other->tile = node->tile;
	other->x = node->x;
	other->y = node->y;
	other->tgtX = other->x;
	other->tgtY = other->y;
	other->homeX = node->homeX;
	other->homeY = node->homeY;
	other->dx = 0;
	other->dy = 0;
	other->scale = Q_8_8(1);
	other->scaleEnd = Q_8_8(1);
	spr = &other->spr;
	spr->mode = OBJ_MODE_NORMAL;
	spr->cols = 0;
	spr->mosaic = 0;
	spr->size = 1;
	spr->shape = SPRITE_SHAPE_SQUARE;
	spr->pal = 0;
	spr->tile = other->tile;
	node->kind = ICON_KIND_NONE;
	state->head = NULL;
	if(state->path != NULL) {
		node = state->path;
		while(node->next != NULL)
			node = node->next;
		node->next = other;
		other->prev = node;
		other->next = NULL;
	} else {
		state->path = other;
		other->prev = NULL;
		other->next = NULL;
	}
	return chosenIdx;
}

/* Six empty functions; nothing calls the first, the fourth or the fifth. The
   others are reached only through bare entries of the window stub table. */
UNUSED void mnArrowNoOperationCallback(void)
{
}

void mnArrowNoOperationCallbackBank(void)
{
}

void mnArrowNoOperationCallbackA(void)
{
}

UNUSED void mnArrowNoOperationCallbackB(void)
{
}

UNUSED void mnArrowNoOperationCallbackC(void)
{
}

void mnArrowNoOperationCallbackD(void)
{
}

/* Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
   in *slot). */
void mnArrowCreate(MENU_SPR *spr, Sint32 *slot)
{
	const void *data = MenuCursorActorTiles;
	Sint32		cacheSlot = syVRAMCacheFindFreeEntry();

	*slot = cacheSlot;
	spr->oam.f.tile = syVRAMCacheLoad(cacheSlot, ARROW_FRAME_BYTES, data);
	spr->oam.f.mode = OBJ_MODE_NORMAL;
	spr->oam.f.mosaic = 0;
	spr->oam.f.cols = 1;
	spr->oam.f.affine = SPRITE_AFFINE_OFF;
	spr->oam.f.affineIdx = 0;
	spr->oam.f.size = 0;
	spr->oam.f.shape = SPRITE_SHAPE_TALL;
	spr->oam.f.pri = 0;
}

#include "window/arrow.c"

/* Loads the large icon of the command under the cursor, zooming in: 32x32 in
   256 colours (32 * 32 bytes, record 15), its scale stepping up by
   LARGE_ICON_ZOOM_STEP a tick, after which mnSelectionMenuDraw shows it at its
   own size. */
void mnSelectionMenuLoadLargeIcon(void)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	SELECTION_NODE		 *selected = mnSelectionMenuGetCursorNode(state);
	SELECTION_NODE		 *largeIcon;
	ICON_TRANSFER		 *buf;
	Uint8				 *tbl;
	void				 *resource;
	Sint32				  resourceId;

	if(selected->kind != ICON_KIND_COMMAND &&
		selected->kind != ICON_KIND_YES_NO)
		return;

	buf = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	largeIcon = &state->records[SELECTION_RECORD_LARGE_ICON];
	resourceId = selected->base;
	/* The resource starts with a halfword offset per command icon. */
	tbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	{
		Uint8 **tiles = &buf->encoded;
		resource = tbl + *(Uint16 *)(tbl + selected->base * 2);
		*tiles = resource;
	}
	syDecompressByteLZ(resource, buf);

	if(largeIcon->kind == ICON_KIND_NONE)
		largeIcon->slot = syVRAMCacheFindFreeEntry();
	largeIcon->tile = syVRAMCacheLoad(largeIcon->slot, 32 * 32, buf);
	largeIcon->kind = ICON_KIND_COMMAND;
	largeIcon->base = resourceId;
	largeIcon->scale = LARGE_ICON_ZOOM_STEP;
	largeIcon->scaleStep = LARGE_ICON_ZOOM_STEP;
	largeIcon->scaleEnd = 6 * LARGE_ICON_ZOOM_STEP;
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Frees the large command icon, if it is shown. The large preview owns a cache
   slot independently of the visible row icons. */
void mnSelectionMenuFreeLargeIcon(void)
{
	SELECTION_NODE *largeIcon =
		&SelectionMenuStateWork->records[SELECTION_RECORD_LARGE_ICON];

	if(largeIcon->kind != ICON_KIND_NONE) {
		syVRAMCacheFreeEntry(largeIcon->slot);
		largeIcon->kind = ICON_KIND_NONE;
	}
}

/* Runs the field menu until the player picks something that leaves it, and
   returns the chosen screen's result. Cancelling the menu returns an undefined
   value, the missing result of mnPartyStatCloseWithMenu (0 under UBFIX); its
   one caller, fdModeRun, ignores the result. Its case for command 0, which
   stores the cue tile ahead in the field state's examineMetatile, is
   unreachable. */
Sint32 mnFieldMenuRun(void)
{
	Sint32		 ret;
	Sint32		 selection;
	FIELD_STATE *field;

	field = FieldStateWork;
	selection = 0;

	for(;;) {
		mnPartyStatOpenAndWait();
		selection = mnFieldMenuSelectCommand(selection);
		/* German, Spanish, French and Italian: mark the menu open, which pauses
		   Reveal's sparkles, and, while Reveal is active, clear its
		   sparkles. */
#if FIELD_MENU_STOPS_REVEAL_SPARKLES
		field->menuOpen = 1;
		if(field->revealActive != 0) {
			fdEffectReleaseRevealSparklesStub();
			sySynch(1);
		}
#endif
#if FIX_FUNCTION_SIGNATURES
		/* UB: the original reads a void return. The default result is ignored
		   by the only caller, so give it a defined value after closing the
		   window. */
		mnPartyStatCloseWithMenu();
		ret = 0;
#else
		ret = mnPartyStatCloseWithMenu();
#endif

		/* A screen left without a choice (-1, or nonzero from the Djinn screen)
		   opens the field menu again. */
		switch(selection) {
		case 0:
			ret = fdActorGetCueMetatileAheadOfCurrentStub();
			if(ret == 0)
				ret = METATILE_BEHAVIOR_WALL;
			field->examineMetatile = ret;
			break;
		case FIELD_MENU_PSYNERGY:
			ret = mnPsynergyMenuRunStub();
			if(ret == -1)
				continue;
			break;
		case FIELD_MENU_DJINN:
			ret = mnDjinnMenuRunStub();
			if(ret != 0)
				continue;
			break;
		case FIELD_MENU_ITEM:
			ret = mnItemMenuRunStub();
			if(ret == -1)
				continue;
			break;
		case FIELD_MENU_STATUS:
			ret = mnStatScreenRunStub();
			if(ret == -1)
				continue;
			break;
		default:
			break;
		}
		break;
	}

#if FIELD_MENU_STOPS_REVEAL_SPARKLES
	field->menuOpen = 0;
#endif
	return ret;
}

#include "window/party_stat_open_close.c"

/* Sets up an empty selection screen and waits a tick. */
UNUSED void mnSelectionMenuCreateEmpty(void)
{
	mnSelectionMenuCreate();
	mnSelectionMenuClear();
	sySynch(1);
}

/* Runs a selection row with the cursor on entry initIdx, cancellable, and
   returns the entry chosen or -1. initIdx is a row index; cancellation
   returns -1 encoded as Uint32. */
Uint32 mnRunSelectionForValue(Uint32 initIdx)
{
	SELECTION_MENU_STATE *state = SelectionMenuStateWork;
	Uint32				  result;

	/* locked 1: a yes or no row stays at the bottom. */
	state->cursorIdx = initIdx;
	state->locked = 1;
	mnSelectionMenuBuildEntries(initIdx);
	mnSetupSelectionBothSides();
	mnSelectionMenuOpenNameWindow(0, SELECTION_ROW_SHOWN);
	mnSelectionMenuStartDrawing();
	result = mnSelectionMenuRunWithSounds(1);
	mnSelectionMenuClose();
	return result;
}
