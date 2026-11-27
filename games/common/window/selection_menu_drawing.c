/* mnSelectionMenuStartDrawing and mnSelectionMenuStopDrawing, the same in
   both games. Both games' window/window_selection.c include it where the
   functions stand. */

/* Starts drawing the selection screen each tick, in the tick pass
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
