/* mnTextScrollCanvasUp, the same in both games. Both games'
   window/window_typing.c include it where the function stands. */

/* Scrolls the message canvas up by 6 pixel rows per step, below its top
   CANVAS_KEPT_ROWS, and clears the rows this uncovers. A typing message calls
   it with step 1 on each tick of a line scroll. */
void mnTextScrollCanvasUp(Sint32 step)
{
	Sint32	shift = step * 6;
	Uint32 *base = TEXT_CANVAS_VRAM;
	Sint32	n;

	for(n = 0; n < CANVAS_COLUMNS; n++) {
		Uint32 *column = base + n * CANVAS_COLUMN_WORDS;

		syDMAXCopy(column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
			DMA_COPY_WORDS(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
			REG_DMA3);
		syWordsFill(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
	}
}
