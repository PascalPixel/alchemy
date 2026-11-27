/* Switching a rectangle of a window's text to another palette. */

/* The main image and the Clear screen's overlay both compile this file. */

/* Like mnWindowSetTextTileBitRect, but ORs all of `palette` into the palette
   number of each entry after clearing only the bit in which WINDOW_PALETTE and
   HIGHLIGHT_PALETTE differ. */
void mnWindowSetTextTileRect(const WINDOW *win, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Uint32 palette)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;

	x += win->x + 1;
	y += win->y + 1;
	palette = BG_TILE_PALETTE(palette);
	if(x < 0) {
		width += x;
		x = 0;
	}
	if(x + width > DISPLAY_TILE_WIDTH - 1) {
		width = DISPLAY_TILE_WIDTH - x;
	}
	if(y < 0) {
		height += y;
		y = 0;
	}
	/* BUG: tests the bottom against the screen's width, so a window reaching
	   below the screen draws into the map's unseen rows. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}
	if(width > 0 && height > 0) {
		do {
			Uint16 *cell = &state->tilemap[y][x];
			Sint32	remaining = width;
			while(remaining != 0) {
				Uint32 entry = *cell;
				entry &= ~BG_TILE_PALETTE(WINDOW_PALETTE ^ HIGHLIGHT_PALETTE);
				entry |= palette;
				remaining--;
				*cell = entry;
				cell++;
			}
			height--;
			y++;
		} while(height != 0);
		state->dirty = 1;
	}
}
