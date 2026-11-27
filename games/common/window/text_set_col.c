/* mnTextSetCol, the same in both games. Both games'
   window/window_text_tiles.c include it where the function stands. */

/* Sets the text colour (TEXT_COL) of drawn text. The menus colour the HP
   figures with it, and set TEXT_COLOR_WHITE back after. */
void mnTextSetCol(Sint32 col)
{
	WIN_STATE *state = WINDOW_STATE;

	state->colour = col & (PALETTE_COLORS - 1);
}
