/* mnWindowSprAcquireFree and mnWindowSprReleaseFree, the same in both games.
   Both games' window/window_windows.c include it where the functions stand. */

/* Takes the first window sprite from the free list, or returns NULL when every
   sprite is in use. */
WINDOW_SPR *mnWindowSprAcquireFree(void)
{
	WIN_STATE  *state = WINDOW_STATE;
	WINDOW_SPR *out = state->freeSprs.head;

	if(out != NULL) {
		/* Taking the last sprite leaves the list empty. */
		if(out->next == NULL)
			state->freeSprs.tailLink = &state->freeSprs.head;
		state->freeSprs.head = out->next;
		out->next = NULL;
	}
	return out;
}

/* Returns a window sprite to the end of the free list. A sprite from outside
   the window system's pool is ignored. */
void mnWindowSprReleaseFree(WINDOW_SPR *out)
{
	WIN_STATE *state = WINDOW_STATE;

	/* freeSprs follows the pool directly, so its address is the pool's end. */
	if((Uint32)out >= (Uint32)state->sprs &&
		(Uint32)out < (Uint32)&state->freeSprs.head) {
		WINDOW_SPR **tail = state->freeSprs.tailLink;

		state->freeSprs.tailLink = &out->next;
		*tail = out;
		out->next = NULL;
	}
}
