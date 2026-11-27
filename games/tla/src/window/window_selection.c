/* The selection screen, the row of icons a command, item or Psynergy is picked
   from in battle and on the field, and the field menu. Part of window.c. */

#include "field/field_state.h"
#include "field/map_grid.h"

/* The selection row's icons stand 16 pixels apart. */
#define SELECTION_ICON_SPACING 16

/* The row's places on screen, its icons (two more, for those sliding in and
   out) and the icons chosen so far. */
#define SELECTION_ROW_PLACES   5
#define SELECTION_ROW_ICONS	   7
#define SELECTION_CHOSEN_ICONS 5

/* The arrows at the row's ends: their records in SELECTION_MENU_STATE.records,
   and the side mnSelectionMenuSetupArrow and mnSelectionMenuDrawArrow take. */
typedef enum SELECTION_ARROW {
	SELECTION_ARROW_LEFT = 0,
	SELECTION_ARROW_RIGHT = 1
} SELECTION_ARROW;

/* SELECTION_MENU_STATE.verticalScroll as the screen is made: the row's icons
   drawn 32 pixels lower, below the bottom of the screen, and the cursor held
   still, until mnSelectionMenuClear readies the list. */
#define SELECTION_ROW_HIDDEN 32

/* SELECTION_MENU_STATE.locked as the screen is made and once a yes or no window
   has opened: nonzero keeps a yes or no row and its window at the bottom of the
   screen, as mnRunSelectionForValue's 1 does. */
#define SELECTION_YES_NO_AT_BOTTOM 999

/* One frame of an end arrow's animation: an 8x16 sprite of 256 colours. */
#define ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/* One frame of the pointing hand's animation: a 16x16 sprite of 256 colours. */
#define HAND_FRAME_BYTES  (4 * TILE_SIZE_8BPP)

/* Called before their definitions. */
Sint32 mnSelectionMenuFindFreeIcon(Sint32 kind);
void   mnSelectionMenuLoadLargeIcon(void);
void   mnSelectionMenuDraw(void);
void   mnSelectionMenuDrawArrow(SELECTION_MENU_STATE *state, Sint32 side);
void   mnSelectionMenuSetupArrow(SELECTION_MENU_STATE *state, Sint32 idx);
void   mnSelectionMenuStepRight(SELECTION_MENU_STATE *state);
void   mnSelectionMenuStepLeft(SELECTION_MENU_STATE *state);
void   mnSelectionMenuRedrawIcon(SELECTION_MENU_STATE *state, Uint32 rowIdx);
void   mnSelectionMenuHighlightIcon(SELECTION_MENU_STATE *state, Uint32 rowIdx);
void   mnSelectionMenuScroll(SELECTION_MENU_STATE *screen, Sint32 forward);
void   mnSelectionMenuSetupIcon(
	  Uint32 kind, Sint32 base, SELECTION_NODE *node, Sint32 reuse);
Uint32 mnConfirmSelection(SELECTION_MENU_STATE *screen);
void   mnPartyStatOpenAndWait(void);

extern const Uint8 MenuAnimatedCursorTiles[];

/* Allocates and resets the selection screen: the row of icons the player picks
   from, with arrows at each end and a pointing hand over the chosen target,
   whose picture takes a VRAM cache entry. */
void mnSelectionMenuCreate(void)
{
	SELECTION_MENU_STATE *screen;
	SELECTION_SPR		 *visual;
	const Uint8			 *tbl;
	Sint32				  i;
	Uint16				  clear = 0;

	screen = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SELECTION_MENU, sizeof(SELECTION_MENU_STATE));
	screen->head = (SELECTION_NODE *)(Uint32)clear;
	screen->path = (SELECTION_NODE *)(Uint32)clear;
	screen->win = (WINDOW *)(Uint32)clear;
	screen->unread = clear;
	screen->top = clear;
	/* The first list starts from its first entry. */
	screen->cursorIdx = SELECTION_CURSOR_RESET;
	screen->verticalScroll = SELECTION_ROW_HIDDEN;
	screen->cnt = clear;
	screen->locked = SELECTION_YES_NO_AT_BOTTOM;
	i = 0;
	do {
		screen->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
		screen->records[i + SELECTION_RECORD_CHOSEN].kind = ICON_KIND_NONE;
		i++;
	} while(i != SELECTION_CHOSEN_ICONS);
	/* The row has two icons more than the chosen ones. */
	screen->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
	screen->records[i + SELECTION_RECORD_ROW + 1].kind = ICON_KIND_NONE;
	screen->records[SELECTION_ARROW_LEFT].kind = FALSE;
	screen->records[SELECTION_ARROW_RIGHT].kind = FALSE;
	screen->records[SELECTION_ARROW_LEFT].y = 0;
	screen->records[SELECTION_ARROW_RIGHT].y = 0;
	tbl = MenuAnimatedCursorTiles;
	screen->records[SELECTION_RECORD_HAND].slot = syVRAMCacheFindFreeEntry();
	screen->records[SELECTION_RECORD_HAND].tile = syVRAMCacheLoad(
		screen->records[SELECTION_RECORD_HAND].slot, HAND_FRAME_BYTES, tbl);
	screen->records[SELECTION_RECORD_HAND].kind = 0;
	screen->records[SELECTION_RECORD_HAND].scale = 0;
	screen->records[SELECTION_RECORD_LARGE_ICON].kind = ICON_KIND_NONE;
	visual = &screen->records[SELECTION_RECORD_HAND].spr;
	visual->mode = OBJ_MODE_NORMAL;
	visual->mosaic = 0;
	visual->cols = OBJ_COLORS_256;
	visual->affine = OAM_AFFINE_OFF;
	visual->matrix = 0;
	visual->size = 1;
	visual->shape = OBJ_SHAPE_SQUARE;
	visual->pri = 0;
}

/* Empties the selection screen's list of entries; after an entry was chosen
   (SELECTION_CURSOR_RESET) the next list starts again from its first entry. */
void mnSelectionMenuClear(void)
{
	SELECTION_MENU_STATE *screen;

	screen = RamHeapSlots->selectionMenu;
	screen->head = NULL;
	screen->unread = 0;
	if(screen->cursorIdx & SELECTION_CURSOR_RESET) {
		screen->top = 0;
		screen->cursorIdx = 0;
	}
	screen->verticalScroll = 0;
	screen->cnt = 0;
}

/* Adds an entry to the selection screen's list, up to
   SELECTION_MENU_ENTRY_LIMIT. The kinds mnSelectionMenuSetupIcon draws (enum
   ICON_KIND) are ICON_KIND_COMMAND, ICON_KIND_YES_NO, ICON_KIND_ITEM and
   ICON_KIND_ABILITY; base is the command's, item's or ability's number. */
void mnSelectionMenuAddEntry(Uint32 kind, Uint32 base)
{
	SELECTION_MENU_STATE *state = RamHeapSlots->selectionMenu;
	Uint16				  cnt = state->cnt;

	if(cnt != SELECTION_MENU_ENTRY_LIMIT) {
		state->kinds[cnt] = kind;
		state->bases[cnt] = base;
		state->cnt++;
	}
}

/* Makes the icons of the listed entries from top, as many as the row's places,
   in a row along the bottom of the screen, and loads the large icon of the one
   under the cursor. */
void mnSelectionMenuBuildEntries(Uint32 unused)
{
	SELECTION_MENU_STATE *screen = RamHeapSlots->selectionMenu;
	Uint32				  cnt = screen->cnt;
	Uint32				  idx;
	SELECTION_NODE		 *prev = 0;
	SELECTION_NODE		 *node;
	Sint32				  rowPos = 0;

	for(idx = screen->top; idx < cnt; idx++) {
		Sint32 base = screen->bases[idx];
		Uint32 kind = screen->kinds[idx];

		node =
			(SELECTION_NODE *)mnSelectionMenuFindFreeIcon(SELECTION_ICON_ROW);
		if(node == 0)
			break;
		mnSelectionMenuSetupIcon(kind, base, node, 0);
		if(screen->head == 0) {
			screen->head = node;
			node->prev = 0;
		} else {
			prev->next = node;
			node->prev = prev;
		}
		node->next = 0;
		rowPos++;
		prev = node;
		if(rowPos == SELECTION_ROW_PLACES)
			break;
	}

	/* Icons are 16 pixels apart: the row is centred on x 100, at y 140. */
	screen->baseX = 100 - rowPos * SELECTION_ICON_SPACING / 2;
	screen->baseY = 140;
	/* The count is reused as a pixel offset while the row is placed. */
	for(prev = screen->head, rowPos = 0; prev != 0; prev = prev->next) {
		Sint32 x = screen->baseX + rowPos;
		Sint32 y;

		prev->x = x;
		y = screen->baseY;
		prev->y = y;
		prev->tgtX = x;
		prev->tgtY = y;
		/* While locked is 0 a yes or no row sits at the top of the screen. */
		if(prev->kind == ICON_KIND_YES_NO && screen->locked == 0) {
			prev->y = 6;
			prev->tgtY = 6;
		}
		prev->dx = 0;
		prev->dy = 0;
		rowPos += SELECTION_ICON_SPACING;
	}
	mnSelectionMenuLoadLargeIcon();
}

/* Does nothing; reached only through the window stub table. */
void mnSelectionMenuReservedNoOperation(void)
{
}

/* Returns a free selection icon record of `kind` (SELECTION_ICON_GROUP), or
   NULL. */
Sint32 mnSelectionMenuFindFreeIcon(Sint32 kind)
{
	SELECTION_MENU_STATE *state =
		(SELECTION_MENU_STATE *)RamHeapSlots->selectionMenu;
	Sint32 i;

	if(kind != SELECTION_ICON_ROW) {
		for(i = 0; i != SELECTION_CHOSEN_ICONS; i++) {
			if(state->records[i + SELECTION_RECORD_CHOSEN].kind ==
				ICON_KIND_NONE)
				return (Sint32)&state->records[i + SELECTION_RECORD_CHOSEN];
		}
	} else {
		for(i = 0; i != SELECTION_ROW_ICONS; i++) {
			if(state->records[i + SELECTION_RECORD_ROW].kind == ICON_KIND_NONE)
				return (Sint32)&state->records[i + SELECTION_RECORD_ROW];
		}
	}
	return 0;
}

/* Starts drawing the selection screen each tick, in the vertical blank pass
   (EVENT_PASS_TICK). */
void mnSelectionMenuStartDrawing(void)
{
	sySetEvent(mnSelectionMenuDraw, EVENT_PASS_TICK);
}

/* Stops drawing the selection screen. */
void mnSelectionMenuStopDrawing(void)
{
	syCloseEvent(mnSelectionMenuDraw);
}

extern const Sint8 MenuCursorBobOffsets[];

/* The selection screen's per-tick event: slides and draws the row of icons,
   the large icon over the chosen one, the end arrows, the icons already chosen
   and the pointing hand. */
void mnSelectionMenuDraw(void)
{
	SELECTION_MENU_STATE *state = RamHeapSlots->selectionMenu;
	SELECTION_SPR  *cursorEntry = &state->records[SELECTION_RECORD_HAND].spr;
	SELECTION_NODE *cursor = &state->records[SELECTION_RECORD_HAND];
	SELECTION_NODE *largeIcon = &state->records[SELECTION_RECORD_LARGE_ICON];
	SELECTION_NODE *node;
	SELECTION_SPR  *spr;
	SELECTION_SPR  *largeSpr;
	Sint32			place = 0;
	Sint32			depth;
	Sint32			step;
	SCREEN_POINT	pos;

	node = state->head;
	while(node != 0) {
		spr = &node->spr;
		spr->affine = OAM_AFFINE_OFF;
		spr->matrix = 0;
		spr->x = node->x;
		spr->y = node->y;
		depth = OAM_DEPTH_WINDOW;
		if(state->verticalScroll != 0) {
			spr->y += state->verticalScroll;
		} else if(node->x != node->tgtX) {
			if(node->dx > 0) {
				if(node->x + node->dx > node->tgtX) {
					node->x = node->tgtX;
				} else {
					node->x = node->x + node->dx;
				}
			} else {
				if(node->x + node->dx < node->tgtX) {
					node->x = node->tgtX;
				} else {
					node->x = node->x + node->dx;
				}
			}
			spr->x = node->x;
		} else if(place == state->cursorIdx) {
			depth = OAM_DEPTH_WINDOW + 1;
			if(cursor->kind != ICON_KIND_NONE) {
				if(btMotionProjectScaledPosStub(node->base, (Sint32 *)&pos) !=
					-1) {
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
			if(syGameFlagGet(FLAG_IN_BATTLE) != 0) {
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
			syOAMAddSprAtDepth((Sint32 *)spr, depth);
		}
		node = node->next;
		place++;
	}
	if(largeIcon->kind != ICON_KIND_NONE) {
		node = mnSelectionMenuGetCursorNode(state);
		largeSpr = &largeIcon->spr;
		largeSpr->mode = OBJ_MODE_NORMAL;
		largeSpr->affine = OAM_AFFINE_OFF;
		largeSpr->matrix = 0;
		largeSpr->mosaic = 0;
		largeSpr->cols = OBJ_COLORS_256;
		largeSpr->size = 2;
		largeSpr->shape = OBJ_SHAPE_SQUARE;
		largeSpr->pri = 0;
		largeSpr->tile = largeIcon->tile;
		/* A 32x32 sprite over the icon, bobbing by half the hand's bob; while
		   it zooms in, its double-size area moves it 16 pixels up and left. */
		largeSpr->x = node->x - 4;
		largeSpr->y =
			node->y + (MenuCursorBobOffsets[(FrameCount >> 1) & 15] >> 1) - 4;
		if(largeIcon->scale != largeIcon->scaleEnd) {
			state->transform.scaleX = largeIcon->scale;
			state->transform.scaleY = largeIcon->scale;
			state->transform.ang = 0;
			largeSpr->matrix = syOAMMatrixAllocFromTransform(&state->transform);
			largeSpr->affine = OAM_AFFINE_DOUBLE_SIZE;
			largeSpr->x += (Uint16)-16;
			largeSpr->y += (Uint8)-16;
			largeIcon->scale = largeIcon->scale + largeIcon->scaleStep;
		}
		if(syGameFlagGet(FLAG_IN_BATTLE) != 0) {
			largeSpr->mode = OBJ_MODE_SEMI_TRANSPARENT;
		}
		syOAMAddSprAtDepth((Sint32 *)largeSpr, OAM_DEPTH_WINDOW + 8);
	}
	mnSelectionMenuDrawArrow(state, SELECTION_ARROW_LEFT);
	mnSelectionMenuDrawArrow(state, SELECTION_ARROW_RIGHT);
	node = state->path;
	while(node != 0) {
		spr = &node->spr;
		if(node->x != node->tgtX) {
			node->x = node->x + node->dx;
		}
		if(node->y != node->tgtY) {
			node->y = node->y + node->dy;
		}
		spr->x = node->x;
		spr->y = node->y;
		if(node->scale != node->scaleEnd) {
			node->scale = node->scale + node->scaleStep;
			state->transform.scaleX = node->scale;
			state->transform.scaleY = node->scale;
			state->transform.ang = 0;
			spr->matrix = syOAMMatrixAllocFromTransform(&state->transform);
			spr->affine = OAM_AFFINE_DOUBLE_SIZE;
			spr->x += (Uint16)-8;
			spr->y += (Uint8)-8;
		} else {
			spr->affine = OAM_AFFINE_OFF;
			spr->matrix = 0;
		}
		if(syGameFlagGet(FLAG_IN_BATTLE) != 0) {
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
		syOAMAddSprAtDepth((Sint32 *)spr, OAM_DEPTH_WINDOW);
		node = node->next;
	}
	if(cursor->kind != ICON_KIND_NONE) {
		place = (Sint32)MenuAnimatedCursorTiles;
		{
			void *src =
				(void *)(place + ((FrameCount >> 2) & 15) * HAND_FRAME_BYTES);

			cursorEntry->tile =
				syVRAMCacheLoad(cursor->slot, HAND_FRAME_BYTES, src);
		}
		if(cursor->tgtX != cursor->x) {
			step = (cursor->tgtX - cursor->x) >> 1;
			if(step != 0) {
				cursor->x = cursor->x + step;
			} else {
				cursor->x = cursor->tgtX;
			}
		}
		if(cursor->tgtY != cursor->y) {
			if((step = (cursor->tgtY - cursor->y) >> 1) != 0) {
				cursor->y = cursor->y + step;
			} else {
				cursor->y = cursor->tgtY;
			}
		}
		cursorEntry->y =
			cursor->y + MenuCursorBobOffsets[(FrameCount >> 2) & 15] - 32;
		cursorEntry->x = cursor->x - 4;
		if(syGameFlagGet(FLAG_IN_BATTLE) != 0) {
			if(state->records[SELECTION_RECORD_HAND].kind ==
				ICON_KIND_COMMAND) {
				cursorEntry->mode = OBJ_MODE_SEMI_TRANSPARENT;
			} else {
				cursorEntry->mode = OBJ_MODE_NORMAL;
			}
		}
		syOAMAddSprAtDepth((Sint32 *)cursorEntry, OAM_DEPTH_WINDOW + 8);
	}
	state->stat += 1;
}

/* The pictures of the selection row's right and left arrows
   (sprite_cursors.s). */
extern Uint8 MenuCursorActorTiles[], MenuCursorLeftActorTiles[];

/* Draws the selection screen's left or right arrow (SELECTION_ARROW), nudged
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
	if(side != SELECTION_ARROW_LEFT) {
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
	if(syGameFlagGet(FLAG_IN_BATTLE)) {
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
   held (ICON_KIND_ITEM). ICON_KIND_YES_NO is a yes or no question, placed at
   the bottom or top of the screen. count is the name's width in tiles inside
   the tick; 0 keeps the open window as it is. The window stays open until
   mnSelectionMenuClose closes it. */
void mnSelectionMenuOpenNameWindow(Sint32 mode, Uint32 cnt)
{
	SELECTION_MENU_STATE *screen;
	SELECTION_NODE		 *node;
	WINDOW				**slot;
	WINDOW				 *win;

	screen = RamHeapSlots->selectionMenu;
	node = mnSelectionMenuGetCursorNode(screen);
	slot = &screen->win;
	win = *slot;
	if(win == 0) {
		if(mode == ICON_KIND_YES_NO) {
			if(screen->locked != 0) {
				*slot = mnWindowCreate(
					17, 17, 5, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
			} else {
				*slot = mnWindowCreate(
					17, 0, 5, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
			}
			screen->verticalScroll = 0;
			screen->locked = SELECTION_YES_NO_AT_BOTTOM;
		} else {
			*slot = mnWindowCreate(19 + ((9 - cnt) >> 1), 17, cnt + 2, 3,
				WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
		}
		mnWindowSprPrepareForRedraw(screen->win);
	} else {
		if(cnt != 0 && win->width != cnt + 2) {
			mnWindowClose(win, WINDOW_FLAG_INSTANT);
			*slot = mnWindowCreate(19 + ((9 - cnt) >> 1), 17, cnt + 2, 3,
				WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
		}
		mnWindowSprPrepareForRedraw(screen->win);
	}
	if(screen->cnt != 0) {
		mnTextDrawMessageAsTiles(node->msg, screen->win, 0, 0);
	} else {
		switch(mode) {
		case ICON_KIND_ABILITY:
			mnTextDrawMessageAsTiles(
				(Sint32)MsgAbilityNotKnown, screen->win, 0, 0);
			break;
		case ICON_KIND_ITEM:
			mnTextDrawMessageAsTiles((Sint32)MsgItemNotHeld, screen->win, 0, 0);
			break;
		}
	}
}

/* Closes the selection screen and frees its icons, tiles and state. The chosen
   icons go too, so the choices are only in the indexes the loops returned. */
void mnSelectionMenuClose(void)
{
	SELECTION_MENU_STATE *state = RamHeapSlots->selectionMenu;
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
	/* The arrows' entries, taken together by mnSetupSelectionBothSides. */
	if(state->records[SELECTION_ARROW_LEFT].y != 0) {
		syVRAMCacheFreeEntry(state->records[SELECTION_ARROW_LEFT].slot);
		/* DEAD: the left arrow tested again, whose y is always the right one's;
		   the ROM keeps its load and branch. */
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
	SELECTION_MENU_STATE *state;
	SELECTION_NODE		 *node;

	state = RamHeapSlots->selectionMenu;
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
	void *menu = RamHeapSlots->selectionMenu;

	mnSelectionMenuSetupArrow(menu, SELECTION_ARROW_LEFT);
	mnSelectionMenuSetupArrow(menu, SELECTION_ARROW_RIGHT);
}

/* Sets up the left or right arrow of the selection row (SELECTION_ARROW), shown
   when there are entries beyond that end. The first call takes the arrow's VRAM
   cache entry; later calls only move it and decide whether it shows. */
void mnSelectionMenuSetupArrow(SELECTION_MENU_STATE *state, Sint32 side)
{
	SELECTION_SPR *arrow = &state->records[side].spr;
	const Uint8	  *arrowTiles = 0;
	Uint32		   cnt;

	state->records[side].kind = 0;
	if(side != SELECTION_ARROW_LEFT) {
		const Uint8 *right = MenuCursorActorTiles;

		cnt = state->cnt;
		if(state->top != 0)
			cnt -= state->top;
		if(cnt > SELECTION_ROW_PLACES) {
			state->records[side].kind = 1;
			cnt = SELECTION_ROW_PLACES;
		}
		arrowTiles = right;
		/* Just past the last of the icons shown. */
		state->records[SELECTION_ARROW_RIGHT].x =
			state->baseX + SELECTION_ICON_SPACING * (cnt - 1) + 17;
	} else {
		arrowTiles = MenuCursorLeftActorTiles;
		state->records[SELECTION_ARROW_LEFT].x = state->baseX - 9;
		if(state->top != 0)
			state->records[SELECTION_ARROW_LEFT].kind = 1;
	}
	if(state->records[side].y == 0) {
		state->records[side].slot = syVRAMCacheFindFreeEntry();
		state->records[side].tile = syVRAMCacheLoad(
			state->records[side].slot, ARROW_FRAME_BYTES, arrowTiles);
		state->records[side].y = state->baseY;
		state->records[side].base = 0;
		arrow->mode = OBJ_MODE_NORMAL;
		arrow->mosaic = 0;
		arrow->cols = OBJ_COLORS_256;
		arrow->affine = OAM_AFFINE_OFF;
		arrow->matrix = 0;
		arrow->size = 0;
		arrow->shape = OBJ_SHAPE_VERTICAL;
		arrow->pri = 0;
	}
}

/* Returns the icon under the cursor: the row's icon at place cursorIdx. */
SELECTION_NODE *mnSelectionMenuGetCursorNode(SELECTION_MENU_STATE *screen)
{
	SELECTION_NODE *node = screen->head;
	Sint32			i;

	for(i = 0; i != screen->cursorIdx; ++i) {
		node = node->next;
	}
	return node;
}

/* A selection mode that allows neither moving nor choosing, only cancelling
   with B. */
#define SELECTION_FIXED 999

/* The row's last place, the fifth: the cursor stepping onto it with more
   entries to come scrolls the row instead. */
#define SELECTION_LAST_PLACE 4
#define ARROW_SCROLL_TICKS	 8

/* Runs the selection row until the player chooses (the chosen entry's index)
   or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no
   moving or choosing; any mode but 0 allows cancelling. Reached only through a
   bare entry of the window stub table. */
Sint32 mnSelectionMenuRun(Sint32 mode)
{
	SELECTION_MENU_STATE *screen = RamHeapSlots->selectionMenu;

	mnSelectionMenuHighlightIcon(screen, 0);
	for(;;) {
		sySynch(1);
		if(screen->verticalScroll != 0) {
			continue;
		}
		if(mode != SELECTION_FIXED) {
			if(PerWork.repeat & KEY_RIGHT) {
				mnSelectionMenuStepRight(screen);
			} else if(PerWork.repeat & KEY_LEFT) {
				mnSelectionMenuStepLeft(screen);
			} else if(PerWork.push & KEY_A) {
				return mnConfirmSelection(screen);
			}
		}
		if(mode != 0 && (PerWork.push & KEY_B)) {
			return -1;
		}
	}
}

/* As mnSelectionMenuRun, with the menu sounds. */
Uint32 mnSelectionMenuRunWithSounds(Uint32 mode)
{
	SELECTION_MENU_STATE *screen = RamHeapSlots->selectionMenu;
	Uint32				  result;

	for(;;) {
		sySynch(1);
		if(screen->verticalScroll != 0) {
			continue;
		}

		if(mode != SELECTION_FIXED) {
			if(PerWork.repeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				mnSelectionMenuStepRight(screen);
			} else if(PerWork.repeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				mnSelectionMenuStepLeft(screen);
			}

			if(PerWork.push & KEY_A) {
				result = screen->top + screen->cursorIdx;
				if(screen->head->kind == ICON_KIND_YES_NO) {
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

		if(mode != 0 && (PerWork.push & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			return -1;
		}
	}
}

/* Unused: moves the cursor one entry right, scrolling the row at its last
   place; it stops at the end, where mnSelectionMenuStepRight wraps around. */
UNUSED void mnMoveSelectionForward(SELECTION_MENU_STATE *screen)
{
	Uint32 nextEntry = screen->top + screen->cursorIdx + 1;

	if(nextEntry == screen->cnt) {
		return;
	}
	mnSelectionMenuRedrawIcon(screen, screen->cursorIdx);
	screen->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	screen->cursorIdx++;
	if(screen->cursorIdx == SELECTION_LAST_PLACE &&
		nextEntry + 1 < screen->cnt) {
		screen->cursorIdx--;
		screen->records[SELECTION_ARROW_RIGHT].base = ARROW_SCROLL_TICKS;
		screen->top++;
		mnSelectionMenuScroll(screen, TRUE);
		if(screen->top + screen->cursorIdx + 2 == screen->cnt) {
			screen->records[SELECTION_ARROW_RIGHT].kind = FALSE;
		}
		screen->records[SELECTION_ARROW_LEFT].kind = TRUE;
	}
	screen->stat = SELECTION_STATUS_SHOWN;
	mnSelectionMenuHighlightIcon(screen, screen->cursorIdx);
	sySynch(1);
	mnSelectionMenuOpenNameWindow(screen->head->kind, 0);
	sySynch(1);
}

/* Moves the cursor one entry left, scrolling the row at its second place; it
   stops at the beginning, where mnSelectionMenuStepLeft wraps around. */
void mnMoveSelectionBackward(SELECTION_MENU_STATE *screen)
{
	if(screen->top == 0 && screen->cursorIdx == 0) {
		return;
	}
	mnSelectionMenuRedrawIcon(screen, screen->cursorIdx);
	screen->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	if(screen->cursorIdx == 1 && screen->top != 0) {
		screen->records[SELECTION_ARROW_LEFT].base = ARROW_SCROLL_TICKS;
		screen->top--;
		mnSelectionMenuScroll(screen, FALSE);
		if(screen->top == 0) {
			screen->records[SELECTION_ARROW_LEFT].kind = FALSE;
		}
		screen->records[SELECTION_ARROW_RIGHT].kind = 1;
	} else {
		screen->cursorIdx--;
	}
	screen->stat = SELECTION_STATUS_SHOWN;
	mnSelectionMenuHighlightIcon(screen, screen->cursorIdx);
	sySynch(1);
	mnSelectionMenuOpenNameWindow(screen->head->kind, 0);
	sySynch(1);
}

/* Moves the cursor one entry right, scrolling the row at its edge and wrapping
   from the last entry to the first. */
void mnSelectionMenuStepRight(SELECTION_MENU_STATE *state)
{
	SELECTION_NODE *node;
	Uint32			entryIdx;
	Sint32			y;
	Uint32			end;

	mnSelectionMenuRedrawIcon(state, state->cursorIdx);
	state->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	state->cursorIdx++;
	if(state->cnt > SELECTION_ROW_PLACES) {
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
			y = node->x + SELECTION_ICON_SPACING;
			node = node->next;
			while(node != NULL) {
				node->tgtX = y;
				node->dx = 12;
				node = node->next;
				y += SELECTION_ICON_SPACING;
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
	if(state->cnt > SELECTION_ROW_PLACES) {
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
			while(i != state->cnt - SELECTION_ROW_PLACES)
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
	Uint32			output;
	Uint32			value;

	while(rowIdx != 0) {
		rowIdx--;
		node = node->next;
	}
	if(node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
		Uint32 first = node->msg - (Uint32)&MsgCommandName;

		value = node->slot;
		mnIconBuildCommandIconTiles(first, 0, &value, &output, 1);
	}
}

/* Redraws the command icon at row place rowIdx and loads its large icon. The
   small icon is drawn the same as mnSelectionMenuRedrawIcon draws it; the
   highlight is the large icon. */
void mnSelectionMenuHighlightIcon(SELECTION_MENU_STATE *state, Uint32 rowIdx)
{
	SELECTION_NODE *node = state->head;
	Uint32			res;
	Uint32			value;

	while(rowIdx != 0) {
		rowIdx--;
		node = node->next;
	}
	if(node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
		Uint32 id = node->msg - (Uint32)&MsgCommandName;

		value = node->slot;
		mnIconBuildCommandIconTiles(id, 0, &value, &res, 1);
		mnSelectionMenuLoadLargeIcon();
	}
}

/* Stops the battle's highlight of units (btHighlightSetModesStub with mode 0);
   mnConfirmSelection calls it as an entry is chosen. It passes a unit list with
   one place an icon, of which only the end marker (BATTLE_UNIT_LIST_END) is
   written; mode 0 never reads the list. */
void mnClearBattleHighlight(SELECTION_MENU_STATE *screen)
{
	Uint16			data[6];
	SELECTION_NODE *node = screen->head;
	Sint32			cnt = 0;

	while(node != 0) {
		node = node->next;
		cnt++;
	}
	data[cnt] = BATTLE_UNIT_LIST_END;
	btHighlightSetModesStub(data, HIGHLIGHT_OFF);
}

/* Scrolls the row one entry: forward brings an icon in from the right as the
   leftmost shrinks away, and back the reverse; waits until it has moved. The
   icons move 2 pixels a tick, and the arriving and leaving icons zoom by an
   eighth a tick, so a scroll takes 8 ticks. */
void mnSelectionMenuScroll(SELECTION_MENU_STATE *screen, Sint32 forward)
{
	SELECTION_NODE *p;
	SELECTION_NODE *last;
	SELECTION_NODE *first;
	Sint32			base;
	Sint32			kind;
	Uint32			idx;
	Sint32			y;
	Sint32			z;

	if(forward != 0) {
		idx = screen->top + SELECTION_LAST_PLACE;
		base = screen->bases[idx];
		kind = screen->kinds[idx];
		p = (SELECTION_NODE *)mnSelectionMenuFindFreeIcon(SELECTION_ICON_ROW);
		if(p == NULL)
			return;
		mnSelectionMenuSetupIcon(kind, base, p, 0);
		y = screen->baseX;
		p->x = y + (SELECTION_LAST_PLACE + 1) * SELECTION_ICON_SPACING;
		z = screen->baseY;
		p->tgtX = y + SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
		p->y = z;
		p->dx = -2;
		p->tgtY = z;
		p->scaleStep = Q_8_8(1) / 8;
		p->scale = Q_8_8(1) / 8;
		p->scaleEnd = Q_8_8(1);
		last = p;
		p = screen->head;
		p->scaleStep = -Q_8_8(1) / 8;
		p->scaleEnd = 0;
		for(;;) {
			p->tgtX = p->x - SELECTION_ICON_SPACING;
			p->dx = -2;
			if(p->next == NULL)
				break;
			p = p->next;
		}
		p->next = last;
		last->next = NULL;
		last->prev = p;
		p = screen->head;
		do {
			sySynch(1);
		} while(p->scale != 0);
		screen->head = p->next;
		syVRAMCacheFreeEntry(p->slot);
		p->kind = ICON_KIND_NONE;
		p = p->next;
		p->prev = NULL;
	} else {
		idx = screen->top;
		base = screen->bases[idx];
		kind = screen->kinds[idx];
		p = (SELECTION_NODE *)mnSelectionMenuFindFreeIcon(SELECTION_ICON_ROW);
		if(p == NULL)
			return;
		mnSelectionMenuSetupIcon(kind, base, p, 0);
		p->x = screen->baseX - SELECTION_ICON_SPACING;
		p->y = screen->baseY;
		p->tgtY = p->y;
		p->dx = 2;
		p->scale = Q_8_8(1) / 8;
		p->scaleStep = Q_8_8(1) / 8;
		p->tgtX = p->x + SELECTION_ICON_SPACING;
		p->scaleEnd = Q_8_8(1);
		first = p;
		p = screen->head;
		p->prev = first;
		first->next = p;
		first->prev = NULL;
		screen->head = first;
		p = first;
		for(;;) {
			p->tgtX = p->x + SELECTION_ICON_SPACING;
			p->dx = 2;
			if(p->next == NULL)
				break;
			p = p->next;
		}
		p->scaleEnd = 0;
		p->scaleStep = -Q_8_8(1) / 8;
		p = screen->head;
		do {
			sySynch(1);
		} while(p->scale != Q_8_8(1));
		while(p->next != NULL)
			p = p->next;
		syVRAMCacheFreeEntry(p->slot);
		p->kind = ICON_KIND_NONE;
		p->prev->next = NULL;
	}
}

/* Loads the icon of an entry of kind mode (ICON_KIND: a command, a yes or no,
   an item or a Psynergy) into a free sprite tile slot and returns the slot, or
   -1 when none was free or mode is another kind. */
Sint32 mnSelectionIconLoad(Uint32 mode, Sint32 value)
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
	case ICON_KIND_ABILITY:
		mnAbilityBuildIconTiles(value, 1, &result, &output, 0);
		break;
	}
	return result;
}

/* Loads the icon of an entry of kind mode into sprite tile slot result (a free
   slot when -1) and returns the slot, or -1 when none was free. */
Sint32 mnSelectionIconLoadIntoSlot(
	Sint32 mode, Sint32 value, Sint32 result, Sint32 option)
{
	Sint32 output;
	Sint32 original = result;

	if(result == -1) {
		result = syVRAMCacheFindFreeEntry();
		if(result == VRAM_CACHE_ENTRY_COUNT)
			return original;
	}

	switch(mode) {
	case ICON_KIND_COMMAND:
	case ICON_KIND_YES_NO:
		mnIconBuildCommandIconTiles(value, option, &result, &output, 1);
		break;
	case ICON_KIND_ITEM:
		mnItemIconLoadLayersIntoSlot(value,
			ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN | ICON_CURSED,
			result);
		break;
	case ICON_KIND_EQUIPMENT_ITEM:
		mnItemIconLoadLayersIntoSlot(
			value, ICON_COUNT_ABOVE_ONE | ICON_BROKEN | ICON_CURSED, result);
		break;
	case ICON_KIND_ABILITY:
		mnAbilityBuildIconTiles(value, option, &result, &output, 1);
		break;
	case ICON_KIND_PATTERN:
		mnIconBuildMiscIconTiles(value, 0, result);
		break;
	}

	return result;
}

/* Sets up a selection icon for an entry: its icon tiles (in its old slot when
   reuse is set), its name message and its sprite. kind must select a command,
   item or Psynergy with the matching base ID. Items and Psynergy are drawn over
   the icon base; the sprite is 16x16 at full size (Q_8_8(1)). */
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
	spr->cols = OBJ_COLORS_16;
	spr->mosaic = 0;

	spr->size = 1;
	spr->shape = OBJ_SHAPE_SQUARE;
	spr->pal = 0;
	spr->tile = tile;
	spr->pri = 0;
}

/* Chooses the entry under the cursor: the other icons gather into it and
   vanish, and it joins the chosen icons at the top left. */
Uint32 mnConfirmSelection(SELECTION_MENU_STATE *screen)
{
	SELECTION_NODE *node;
	SELECTION_NODE *other = NULL;
	Uint32			place = 0;
	Uint32			idx;
	SELECTION_SPR  *spr;

	idx = screen->top + screen->cursorIdx;
	mnClearBattleHighlight(screen);
	mnSelectionMenuRedrawIcon(screen, screen->cursorIdx);
	screen->stat = SELECTION_STATUS_SCROLLING;
	sySynch(1);
	screen->records[SELECTION_ARROW_LEFT].kind = 0;
	screen->records[SELECTION_ARROW_RIGHT].kind = 0;
	screen->records[SELECTION_RECORD_HAND].kind = 0;
	screen->records[SELECTION_RECORD_HAND].scale = 0;
	mnSelectionMenuFreeLargeIcon();
	node = screen->head;
	for(; node != NULL && screen->cursorIdx != place; place++) {
		node = node->next;
	}
	node->homeX = node->x;
	node->homeY = node->y;
	for(other = screen->head; other != NULL; other = other->next) {
		if(other != node) {
			other->tgtX = node->x;
			other->dx = (node->x - other->x) >> 1;
		}
	}
	sySynch(2);
	for(other = screen->head; other != NULL; other = other->next) {
		if(other != node) {
			syVRAMCacheFreeEntry(other->slot);
			other->kind = ICON_KIND_NONE;
		}
	}
	screen->head = node;
	node->prev = NULL;
	node->next = NULL;
	node->tgtX = 4;
	for(other = screen->path, place = 0; other != NULL; other = other->next) {
		node->tgtX += SELECTION_ICON_SPACING;
		place++;
	}
	screen->pathTop[place] = screen->top;
	screen->pathCursor[place] = screen->cursorIdx;
	node->dx = (node->tgtX - node->x) >> 1;
	screen->unread = 0;
	screen->cursorIdx |= SELECTION_CURSOR_RESET;
	sySynch(2);
	other =
		(SELECTION_NODE *)mnSelectionMenuFindFreeIcon(SELECTION_ICON_CHOSEN);
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
	spr->cols = OBJ_COLORS_16;
	spr->mosaic = 0;
	spr->size = 1;
	spr->shape = OBJ_SHAPE_SQUARE;
	spr->pal = 0;
	spr->tile = other->tile;
	node->kind = ICON_KIND_NONE;
	screen->head = NULL;
	if(screen->path != NULL) {
		node = screen->path;
		while(node->next != NULL)
			node = node->next;
		node->next = other;
		other->prev = node;
		other->next = NULL;
	} else {
		screen->path = other;
		other->prev = NULL;
		other->next = NULL;
	}
	return idx;
}

/* Six functions that do nothing, beside the menu arrows': nothing refers to the
   first, the fourth or the fifth, and the others only fill entries of the
   window stub table, which nothing calls. */

/* Does nothing. */
UNUSED void mnArrowNoOperation1(void)
{
}

/* Does nothing. */
void mnArrowNoOperation2(void)
{
}

/* Does nothing. */
void mnArrowNoOperation3(void)
{
}

/* Does nothing. */
UNUSED void mnArrowNoOperation4(void)
{
}

/* Does nothing. */
UNUSED void mnArrowNoOperation5(void)
{
}

/* Does nothing. */
void mnArrowNoOperation6(void)
{
}

/* Sets up a sprite as the right-pointing arrow, in a free VRAM cache entry
   returned in *slot, which mnArrowRelease frees. */
void mnArrowCreate(MENU_SPR *spr, Sint32 *slot)
{
	const void *data = MenuCursorActorTiles;
	Sint32		cacheSlot = syVRAMCacheFindFreeEntry();

	*slot = cacheSlot;
	spr->oam.f.tile = syVRAMCacheLoad(cacheSlot, ARROW_FRAME_BYTES, data);
	spr->oam.f.mode = OBJ_MODE_NORMAL;
	spr->oam.f.mosaic = 0;
	spr->oam.f.cols = OBJ_COLORS_256;
	spr->oam.f.affine = OAM_AFFINE_OFF;
	spr->oam.f.affineIdx = 0;
	spr->oam.f.size = 0;
	spr->oam.f.shape = OBJ_SHAPE_VERTICAL;
	spr->oam.f.pri = 0;
}

/* Places an arrow sprite at screen pixel (x, y) and queues it for drawing this
   tick, at depth OAM_DEPTH_FRONT - 3; the caller calls it every tick it
   shows. */
void mnArrowSetPos(MENU_SPR *sprite, Sint32 x, Sint32 y)
{
	sprite->oam.f.x = x;
	sprite->oam.f.y = y;
	syOAMAddSprAtDepth(sprite, OAM_DEPTH_FRONT - 3);
}

/* Frees an arrow sprite's tile slot. */
void mnArrowRelease(Sint32 slot)
{
	syVRAMCacheFreeEntry(slot);
}

/* The large icon's zoom: it opens at this scale (in 8.8) and grows by as much
   each tick until six steps, when mnSelectionMenuDraw shows it at its own
   size. */
#define LARGE_ICON_ZOOM_STEP 40

/* Loads the large icon of the command under the cursor, zooming in: 32x32 in
   256 colours (SELECTION_RECORD_LARGE_ICON), its scale growing by
   LARGE_ICON_ZOOM_STEP a tick. Other kinds of entry show none. The icon keeps
   its VRAM cache entry until mnSelectionMenuFreeLargeIcon. */
void mnSelectionMenuLoadLargeIcon(void)
{
	SELECTION_MENU_STATE *state = RamHeapSlots->selectionMenu;
	SELECTION_NODE		 *node = mnSelectionMenuGetCursorNode(state);
	SELECTION_NODE		 *largeIcon;
	ICON_TRANSFER		 *buf;
	Uint8				 *tbl;
	void				 *resource;
	Sint32				  command;

	if(node->kind != ICON_KIND_COMMAND && node->kind != ICON_KIND_YES_NO)
		return;

	buf = (ICON_TRANSFER *)syHeapSlotAlloc(
		HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	largeIcon = &state->records[SELECTION_RECORD_LARGE_ICON];
	command = node->base;
	tbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	{
		Uint8 **dst = &buf->encoded;
		resource = tbl + ((Uint16 *)tbl)[node->base];
		*dst = resource;
	}
	syDecompressByteLZ(resource, buf);

	if(largeIcon->kind == ICON_KIND_NONE)
		largeIcon->slot = syVRAMCacheFindFreeEntry();
	largeIcon->tile = syVRAMCacheLoad(largeIcon->slot, COMMAND_ICON_BYTES, buf);
	largeIcon->kind = ICON_KIND_COMMAND;
	largeIcon->base = command;
	largeIcon->scale = LARGE_ICON_ZOOM_STEP;
	largeIcon->scaleStep = LARGE_ICON_ZOOM_STEP;
	largeIcon->scaleEnd = 6 * LARGE_ICON_ZOOM_STEP;
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Frees the large icon's VRAM cache entry, if it holds one, and marks it
   empty. */
void mnSelectionMenuFreeLargeIcon(void)
{
	SELECTION_NODE *largeIcon =
		&((SELECTION_MENU_STATE *)RamHeapSlots->selectionMenu)
			 ->records[SELECTION_RECORD_LARGE_ICON];

	if(largeIcon->kind != ICON_KIND_NONE) {
		syVRAMCacheFreeEntry(largeIcon->slot);
		largeIcon->kind = ICON_KIND_NONE;
	}
}

Sint32 mnFieldMenuSelectCommand(Sint32);

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
	FIELD_STATE *state;

	state = RamHeapSlots->fieldState;
	selection = 0;

	for(;;) {
		mnPartyStatOpenAndWait();
		selection = mnFieldMenuSelectCommand(selection);
#if FIX_FUNCTION_SIGNATURES
		/* UB: the original reads the void return of mnPartyStatCloseWithMenu,
		   called with no prototype in scope, as The Broken Seal's
		   mnFieldMenuRun does. */
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
			state->examineMetatile = ret;
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

	return ret;
}

/* Opens the party status window that shows with the field menu (flags 1) and
   waits a tick. */
void mnPartyStatOpenAndWait(void)
{
	mnPartyStatCreateWindow(1);
	sySynch(1);
}

/* Closes the party status window that shows with the field menu. */
void mnPartyStatCloseWithMenu(void)
{
	mnPartyStatClose();
}

/* Sets up an empty selection screen and waits a tick. */
void mnSelectionMenuCreateEmpty(void)
{
	mnSelectionMenuCreate();
	mnSelectionMenuClear();
	sySynch(1);
}

/* Runs a selection row with the cursor on entry initIdx, cancellable, and
   returns the entry chosen or -1. */
Uint32 mnRunSelectionForValue(Uint32 initIdx)
{
	SELECTION_MENU_STATE *state = RamHeapSlots->selectionMenu;
	Uint32				  result;

	state->cursorIdx = initIdx;
	state->locked = 1;
	mnSelectionMenuBuildEntries(initIdx);
	mnSetupSelectionBothSides();
	mnSelectionMenuOpenNameWindow(0, 5);
	mnSelectionMenuStartDrawing();
	result = mnSelectionMenuRunWithSounds(1);
	mnSelectionMenuClose();
	return result;
}

#undef SELECTION_FIXED
#undef SELECTION_LAST_PLACE
#undef ARROW_SCROLL_TICKS
