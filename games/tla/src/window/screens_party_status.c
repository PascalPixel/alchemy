/* The party status window. Part of screens.c. */

/* The party status window's column for each party member, in tiles. */
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH  5 /* the Djinn column, in battle */

/* Sizes the party status window by the party: a column of six tiles a party
   member (four at most outside battle), one more row with PARTY_STATUS_SHOW_PP
   and the Djinn column with PARTY_STATUS_SHOW_DJINN, against the screen's right
   edge. */
void mnPartyStatBuildLayoutBounds(Sint32 flg)
{
	PARTY_STAT_WINDOW *state = RamHeapSlots->partyStat;
	WIN_STATE		  *winState = (WIN_STATE *)RamHeapSlots->winState;
	Sint32			   height = 4;
	Sint32			   members;
	Sint32			   width;
	Sint32			   x;

	if(winState->battleMode != WINDOW_FIELD) {
		members = btPartyPrepareMembersStub(0);
		height = 3;
	} else {
		members = gmPartyCountMembersStub();
		if((Uint32)members > 4)
			members = 4;
	}
	if(flg & PARTY_STATUS_SHOW_PP)
		height++;
	else
		flg &= ~PARTY_STATUS_SHOW_DJINN;

	members *= PARTY_STATUS_COLUMN_WIDTH;
	width = members + 1;
	if(flg & PARTY_STATUS_SHOW_DJINN)
		width = members + 1 + PARTY_STATUS_DJINN_WIDTH;

	x = DISPLAY_TILE_WIDTH;
	x -= width;
	state->x = x;
	state->y = 0;
	state->width = width;
	state->height = height;
	state->flg = flg;
}

/* Opens the party status window, sized by mnPartyStatBuildLayoutBounds, and
   draws it, holding the tilemap upload meanwhile. */
void mnPartyStatCreateWindow(Sint32 flg)
{
	PARTY_STAT_WINDOW *win;
	WIN_STATE		  *state;

	win =
		syHeapSlotAllocEWRAM(HEAP_SLOT_PARTY_STATUS, sizeof(PARTY_STAT_WINDOW));
	state = (WIN_STATE *)RamHeapSlots->winState;
	state->holdTilemapUpload = 1;
	mnPartyStatBuildLayoutBounds(flg);
	win->win = (WINDOW *)mnWindowCreate(win->x, win->y, win->width, win->height,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	mnPartyStatDraw(flg);
	state->holdTilemapUpload = 0;
}

/* Draws the vertical lines between the party members' columns of the party
   status `win`, with their top and bottom joints, at the offsets listed in
   WindowPartyColumnOffsets (shifted right past the Djinn column, whose own
   line comes first in the list). In battle it also draws the window's bottom
   edge. */
void mnWindowDrawColumnBorders(WINDOW *win, Uint32 flg)
{
	WIN_STATE *winState = (WIN_STATE *)RamHeapSlots->winState;
	Sint32	   first = 1;
	Sint32	   shift = 0;
	Uint32	   lastColumn = win->width - 1;
	Sint32	   i;
	Uint8	   state = winState->battleMode;
	Uint32	   rows = win->height;
	Uint32	   column;
	Uint32	   row;
	Uint16	  *cell;

	if((flg & PARTY_STATUS_SHOW_PP) == 0)
		flg &= ~PARTY_STATUS_SHOW_DJINN;
	if(flg & PARTY_STATUS_SHOW_DJINN) {
		shift = PARTY_STATUS_DJINN_WIDTH;
		first = 0;
	}
	i = first;
	if(WindowPartyColumnOffsets[i] >= 0) {
		for(; WindowPartyColumnOffsets[i] >= 0; i++) {
			column = WindowPartyColumnOffsets[i] + shift;
			if(column < lastColumn) {
				for(row = 0; row != rows; row++) {
					cell = winState->tilemap[win->y + row] + (win->x + column);
					if(row == 0)
						*cell = WINDOW_TILE_BORDER_TOP_TEE;
					else if(row == rows - 1)
						*cell = WINDOW_TILE_BORDER_BOTTOM_TEE;
					else
						*cell = WINDOW_TILE_DIVIDER_VERTICAL;
				}
			}
		}
		state = winState->battleMode;
	}
	{
		/* FAKEMATCH(temporary): shown, a copy of state for the bottom edge's
		 * tests; as state. */
		Uint32 shown = state;

		/* FAKEMATCH(dead): state cleared after its copy, where nothing reads
		 * it. */
		state = 0;
		if(shown) {
			/* FAKEMATCH(dead): a test of shown that is never true, around a
			 * store to column, which the next lines set again. */
			if(shown > 255)
				column = 0;
			cell = winState->tilemap[win->y + win->height - 1] + win->x;
			*cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT;
			for(column = 1; column < lastColumn; column++)
				*cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM;
			*cell = WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT;
		}
	}
	winState->dirty = 1;
}

/* The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile, 4
   bytes a row): the status bars are drawn from a tile's bottom row up. */
#define TILE_ROW_FROM_BOTTOM(tile, up) \
	((Uint32 *)((tile) * 32 - (up) * 4 + ((Sint32)BG_CHAR_BLOCK(0) + 7 * 4)))

/* A party member's HP and PP bars: PARTY_STATUS_BAR_LENGTH pixels across five
   tiles, whose own pixels are drawn in BAR_TILE_COLOR over a shadow of
   BAR_TILE_SHADOW_COLOR. */
#define PARTY_STATUS_BAR_LENGTH 40
#define BAR_TILE_COLOR			14
#define BAR_TILE_SHADOW_COLOR	1
/* The colours the filled part of a bar and the rest of it are drawn in, each
   over its shadow: on the field, the basic sprite colours copied into
   HIGHLIGHT_PALETTE. */
#define BAR_FILLED_COLOR		8
#define BAR_FILLED_SHADOW_COLOR 13
#define BAR_EMPTY_COLOR			2
#define BAR_EMPTY_SHADOW_COLOR	12
/* A 4bpp tile row: eight pixels of four bits, one word. */
#define TILE_ROW_BITS			32

/* Recolours the five bar tiles at (x, y) in `win` to show a bar `length`
   pixels long (0 to PARTY_STATUS_BAR_LENGTH). */
UNREAD_RESULT mnPartyStatDrawBars(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 length)
{
	Sint32	   i;
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;
	Sint32	   fullLength = length;
	Sint32	   row;
	Sint32	   column;
	Uint32	   tile;
	Uint32	   bar;
	Uint32	   shadow;
	Uint32	   pixels;
	Uint32	   recoloured;

	if(state->battleMode == WINDOW_FIELD) {
		syDMAXCopy((const void *)syPaletteGetBasicSprCols(),
			(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
			DMA_COPY_HALFWORDS(16), REG_DMA3);
		BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR) =
			BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	}
	x += win->x;
	y += win->y;
	for(i = 0; i < PARTY_STATUS_BAR_LENGTH / 8; i++, x++) {
		/* This tile's eight pixels: filled up to `length`, empty after. */
		tile = state->tilemap[0][y * BG_SCREEN_WIDTH + x];
		bar = PIXELS_4BPP(BAR_EMPTY_COLOR);
		shadow = PIXELS_4BPP(BAR_EMPTY_SHADOW_COLOR);
		tile &= TILE_NUMBER_MASK;
		if(length > 7) {
			bar = PIXELS_4BPP(BAR_FILLED_COLOR);
			shadow = PIXELS_4BPP(BAR_FILLED_SHADOW_COLOR);
			/* UB: at a length of zero the shift below is by 32, which the GBA
			   takes as zero. */
#if FIX_UNDEFINED_SHIFT
		} else if(length > 0) {
#else
		} else if(length >= 0) {
#endif
			bar <<= length * 4;
			bar |=
				PIXELS_4BPP(BAR_FILLED_COLOR) >> (TILE_ROW_BITS - length * 4);
			shadow <<= length * 4;
			shadow |= PIXELS_4BPP(BAR_FILLED_SHADOW_COLOR) >>
				(TILE_ROW_BITS - length * 4);
		}
		for(row = 0; row <= (fullLength != 0 ? 2 : 0); row++) {
			pixels = *TILE_ROW_FROM_BOTTOM(tile, row);
			recoloured = 0;
			for(column = 0; column <= 7; column++) {
				Uint32 colour = pixels & 0xf;

				if(colour == BAR_TILE_COLOR)
					recoloured |= bar & (0xfU << (column * 4));
				else if(colour == BAR_TILE_SHADOW_COLOR)
					recoloured |= shadow & (0xfU << (column * 4));
				else
					recoloured |= colour << (column * 4);
				pixels >>= 4;
			}
			*TILE_ROW_FROM_BOTTOM(tile, row) = recoloured;
		}
		length -= 8;
	}
}

/* Draws the party status window with `flg` (see PARTY_STATUS_*): each party
   member's name, HP and optionally PP with their bars, and in battle the
   standby Djinn of each element. */
void mnPartyStatDraw(Sint32 flg)
{
	PARTY_STAT_WINDOW *layout = RamHeapSlots->partyStat;
	BATTLE_STATE	  *battle = RamHeapSlots->battleState;
	WINDOW			  *win = layout->win;
	WIN_STATE		  *state = (WIN_STATE *)RamHeapSlots->winState;
	Sint32			   x = 0;
	Uint32			   cnt;
	Sint32			   y = 0;
	Uint32			   i;
	Uint16			   members[5];
	Uint8			   standbyDjinn[4];

	if(state->battleMode != WINDOW_FIELD) {
		cnt = btPartyPrepareMembersStub(0);
		y = -1;
		for(i = 0; i < cnt; i++) {
			members[i] = battle->partyUnits[i];
			if(members[i] == BATTLE_UNIT_LIST_END)
				break;
		}
	} else {
		cnt = gmPartyCountMembersStub();
		if(cnt > 4)
			cnt = 4;
		for(i = 0; i < cnt; i++)
			members[i] = MainRoutineStateWork.party[i];
		members[i] = BATTLE_UNIT_LIST_END;
	}
	cnt = i;

	if(flg == PARTY_STATUS_KEEP_FLAGS)
		flg = layout->flg;
	if(!(flg & PARTY_STATUS_SHOW_PP))
		flg &= ~PARTY_STATUS_SHOW_DJINN;
	if(state->battleMode == WINDOW_FIELD || btDjinnCountStandbyStub(0, 0) == 0)
		flg &= ~PARTY_STATUS_SHOW_DJINN;
	if(flg == PARTY_STATUS_CLOSED) {
		mnWindowEraseBorderRect(
			layout->x, layout->y, layout->width, layout->height);
		return;
	}

	state->holdTilemapUpload = 1;
	if(layout->flg == flg) {
		mnWindowSprRedrawSavedRect(win);
		mnWindowDrawColumnBorders(win, flg);
	} else {
		mnWindowEraseBorderRect(
			layout->x, layout->y, layout->width, layout->height);
		mnPartyStatBuildLayoutBounds(flg);
		win->width = layout->width;
		win->height = layout->height;
		win->x = layout->x;
		mnWindowDrawBorder(layout->x, layout->y, layout->width, layout->height);
		mnWindowDrawColumnBorders(win, flg);
	}

	if(flg & PARTY_STATUS_SHOW_DJINN)
		x = PARTY_STATUS_DJINN_WIDTH;
	for(i = 0; i != cnt; i++) {
		UNIT  *unit = syUnitGet(members[i]);
		Sint32 hp = unit->hp;
		Sint32 maxHp = unit->hpMax;
		Sint32 barLength;
		Sint32 cur;

		if(hp == 0)
			mnTextSetCol(TEXT_COLOR_RED);
		else if(hp <= maxHp / 4)
			mnTextSetCol(TEXT_COLOR_YELLOW);
		else
			mnTextSetCol(TEXT_COLOR_WHITE);
		state->textPal = HIGHLIGHT_PALETTE;
		if(state->battleMode != WINDOW_FIELD)
			state->textPal = BASIC_COLORS_PALETTE;
		mnTextDrawPrefixedNumAtOffset(
			hp, win, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
		state->textPal = WINDOW_PALETTE;
		mnTextDrawStringAtOffset(unit->name, (WINDOW *)win,
			(x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
		mnTextSetCol(TEXT_COLOR_WHITE);
		if(unit->hpMax != 0) {
			cur = unit->hp;
			barLength = (cur * PARTY_STATUS_BAR_LENGTH) / unit->hpMax;
			if(barLength == 0 && cur != 0)
				barLength = 1;
			mnPartyStatDrawBars((WINDOW *)win,
				x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2, barLength);
		}
		if(flg & PARTY_STATUS_SHOW_PP) {
			state->textPal = HIGHLIGHT_PALETTE;
			if(state->battleMode != WINDOW_FIELD)
				state->textPal = BASIC_COLORS_PALETTE;
			mnTextDrawPrefixedNumAtOffset(unit->pp, win,
				(x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 16, 1);
			if(unit->ppMax != 0) {
				cur = unit->pp;
				barLength = (cur * PARTY_STATUS_BAR_LENGTH) / unit->ppMax;
				if(barLength == 0 && cur != 0)
					barLength = 1;
				mnPartyStatDrawBars((WINDOW *)win,
					x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3, barLength);
			}
		}
	}
	state->textPal = WINDOW_PALETTE;

	if(state->battleMode != WINDOW_FIELD && (flg & PARTY_STATUS_SHOW_DJINN)) {
		Sint32 row = y;

		/* FAKEMATCH(dead): i set to a 0 never read. */
		i = 0;
		if(flg & PARTY_STATUS_SHOW_PP)
			row++;
		btDjinnCountStandbyStub(0, standbyDjinn);
		mnWindowSetTilemapEntry(
			win, ELEMENT_ICON_TILE + ELEMENT_VENUS, 0, row, 0);
		mnWindowSetTilemapEntry(
			win, ELEMENT_ICON_TILE + ELEMENT_MERCURY, 2, row, 0);
		mnWindowSetTilemapEntry(
			win, ELEMENT_ICON_TILE + ELEMENT_MARS, 0, row + 1, 0);
		mnWindowSetTilemapEntry(
			win, ELEMENT_ICON_TILE + ELEMENT_JUPITER, 2, row + 1, 0);
		if(standbyDjinn[0] > 9 || standbyDjinn[1] > 9 || standbyDjinn[2] > 9 ||
			standbyDjinn[3] > 9)
			syDMAXCopy(PartyStatusDigitTiles, DJINN_COUNT_TENS_VRAM,
				DMA_COPY_WORDS(9 * TILE_SIZE_4BPP / 4), REG_DMA3);
		mnWindowSetTilemapEntry(
			win, DJINN_COUNT_TILE(standbyDjinn[0]), 1, row, 0);
		mnWindowSetTilemapEntry(
			win, DJINN_COUNT_TILE(standbyDjinn[1]), 3, row, 0);
		mnWindowSetTilemapEntry(
			win, DJINN_COUNT_TILE(standbyDjinn[2]), 1, row + 1, 0);
		mnWindowSetTilemapEntry(
			win, DJINN_COUNT_TILE(standbyDjinn[3]), 3, row + 1, 0);
	}
	state->holdTilemapUpload = 0;
}

/* Closes the party status window and frees its heap block. */
void mnPartyStatClose(void)
{
	mnWindowClose(*(void **)RamHeapSlots->partyStat, 1);
	syHeapSlotRelease(HEAP_SLOT_PARTY_STATUS);
}

#undef TILE_ROW_FROM_BOTTOM
#undef PARTY_STATUS_BAR_LENGTH
#undef BAR_TILE_COLOR
#undef BAR_TILE_SHADOW_COLOR
#undef BAR_FILLED_COLOR
#undef BAR_FILLED_SHADOW_COLOR
#undef BAR_EMPTY_COLOR
#undef BAR_EMPTY_SHADOW_COLOR
#undef TILE_ROW_BITS
#undef PARTY_STATUS_DJINN_WIDTH
#undef PARTY_STATUS_COLUMN_WIDTH
