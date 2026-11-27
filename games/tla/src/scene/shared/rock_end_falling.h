/* Ends the falling rocks: removes their task and frees their picture and */
/* state. */
void syRockEndFalling(void) {
	ROCK_STATE *state = Ram_HeapSlots->menuState;

	syFieldTaskRemove(btParticleUpdateFalling);
	syVramCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}
