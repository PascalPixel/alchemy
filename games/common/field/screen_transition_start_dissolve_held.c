/* fdScreenTransitionStartDissolveHeld, the same in both games. Both games'
   field/field_mode_screen_transition.c include it where the function stands. */

/* Starts a dissolve of the given mode, its value held at 0, and waits two
   seconds. In The Broken Seal nothing calls it, by name or through its
   stub. */
void fdScreenTransitionStartDissolveHeld(Sint32 mode)
{
	SCREEN_TRANSITION_STATE *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
	DMA_FILL32(0, state, sizeof(*state));
	fdScreenTransitionFillTilemapAndColTile(0);
	state->mode = mode;
	state->value = 0;
	sySetEvent(fdScreenTransitionUpdateDissolve, EVENT_PASS_TICK);
	sySynch(120);
}
