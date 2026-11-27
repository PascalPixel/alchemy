/* btBackgroundFree, the same in both games. Both games'
   battle/battle_background.c include it where the function stands. */

/* Frees the zoomed background's scanline state. */
void btBackgroundFree(void)
{
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SCANLINES);
}
