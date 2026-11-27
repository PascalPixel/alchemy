/* fdPaletteFadeRelease, the same in both games. Both games'
   field/field_mode_palette_fade.c include it where the function stands. */

/* Stops the palette fade and frees it. */
void fdPaletteFadeRelease(void)
{
	syCloseEvent(&fdPaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}
