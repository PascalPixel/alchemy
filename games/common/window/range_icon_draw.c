/* mnRangeIconDraw and mnGraphicsMergeShiftedTileRows, the same in both
   games. Both games' window/screens_menu_titles.c include it where the
   functions stand. */

/* Draws range icon `pattern`, how many targets an ability reaches, across tiles
   (x, y) to (x + 2, y) of `win`'s interior: its end tile mirrored, its
   centre tile and its end tile, in the UI palette. */
UNREAD_RESULT mnRangeIconDraw(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 pattern, Sint32 unused)
{
	Sint32 patternOfs = pattern * 2;
	Sint32 rightTile = patternOfs + WINDOW_TILE(RANGE_ICON_FIRST_TILE + 1);

	mnWindowSetTilemapEntry(win, TILE_HFLIP | rightTile, x, y, 0);
	mnWindowSetTilemapEntry(
		win, patternOfs + WINDOW_TILE(RANGE_ICON_FIRST_TILE), x + 1, y, 0);
	mnWindowSetTilemapEntry(win, rightTile, x + 2, y, 0);
}

/* A 4bpp tile row's eighth pixel, in the top four bits of its word. */
#define ROW_LAST_PIXEL_SHIFT 28

/* Merges two 8x8 4bpp tiles row by row into `output`: `second` moved `shift`
   pixels (right when positive, left when negative), drawn over `first`, which
   shows wherever the moved pixel is colour 0. */
void mnGraphicsMergeShiftedTileRows(
	Uint32 *first, Uint32 *second, Uint32 *output, Sint32 shift)
{
	Sint32 oppositeShift;
	Sint32 rightShift;
	Sint32 leftShift;
	Sint32 row;

	oppositeShift = -shift;
	row = 0;
	rightShift = oppositeShift * 4;
	leftShift = shift * 4;

	for(; row <= 7; row++) {
		Uint32 result;
		Uint32 shifted;
		Uint32 fallback;
		Sint32 pixel;

		shifted = *second++;
		result = 0;
		fallback = *first++;
		if(shift < 0)
			shifted >>= rightShift;
		else
			shifted <<= leftShift;

		for(pixel = 0; pixel < 8; pixel++) {
			result <<= 4;
			if(shifted >= 1u << ROW_LAST_PIXEL_SHIFT)
				result += shifted >> ROW_LAST_PIXEL_SHIFT;
			else
				result += fallback >> ROW_LAST_PIXEL_SHIFT;
			shifted <<= 4;
			fallback <<= 4;
		}

		*output++ = result;
	}
}
