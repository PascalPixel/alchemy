/* Ends the falling rocks: removes their event and frees their picture and
   state. */
void scRockEndFalling(void)
{
	ROCK_STATE *state = Ram_HeapSlots->menuState;

	syFieldCloseEvent(scParticleUpdateFalling);
	syVRAMCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}
