/* mnWindowSystemUpdate, the same in both games. Both games'
   window/window_borders.c include it where the function stands. */

/* The window system's per-tick event: windows, then messages, then sprite
   animation. */
void mnWindowSystemUpdate(void)
{
	mnWindowUpdateZooms();
	mnMessageUpdateAll();
	mnWindowSprUpdateAll();
}
