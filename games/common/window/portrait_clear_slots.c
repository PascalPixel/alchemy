/* mnPortraitClearSlots, the same in both games. Both games'
   window/window_messages.c include it where the function stands. */

/* Marks both portrait slots free (PORTRAIT_SLOT_EMPTY), as the window system's
   field setup does. */
void mnPortraitClearSlots(void)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   idx;

	for(idx = 0; idx != ARRAY_COUNT(state->portraitPictures); idx++) {
		state->portraitPictures[idx] = PORTRAIT_SLOT_EMPTY;
	}
}
