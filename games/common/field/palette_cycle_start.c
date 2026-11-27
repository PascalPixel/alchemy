/* fdPaletteCycleStart, the same in both games. Both games'
   field/field_tile_animation.c include it where the function stands. */

/* Unused: starts running the palette cycles each tick. */
UNUSED void fdPaletteCycleStart(void)
{
	sySetEvent(fdPaletteCycleStep, EVENT_PASS_TICK);
}
