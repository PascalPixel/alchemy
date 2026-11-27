/* mnTextClearCanvas and mnTextFillCanvasBackground, the same in both games.
   Both games' window/window_windows.c include it where the functions stand. */

/* Clears the text canvas to transparent. */
Sint32 mnTextClearCanvas(void)
{
	return syWordsFill(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
}

/* Fills the text canvas with the window background colour (4). */
Sint32 mnTextFillCanvasBackground(void)
{
	return syWordsFill(
		TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, TEXT_CANVAS_BACKGROUND_WORD);
}
