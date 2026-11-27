/* fdScreenTransitionAllocateAndClearState, the same in both games. Both
   games' field/field_mode_screen_transition.c include it where the function
   stands. */

/* Allocates the screen transition state (HEAP_SLOT_SCREEN_TRANSITION) and
   clears it. */
SCREEN_TRANSITION_STATE *fdScreenTransitionAllocateAndClearState(void)
{
	SCREEN_TRANSITION_STATE *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
	DMA_FILL32(0, state, sizeof(*state));
	return state;
}
