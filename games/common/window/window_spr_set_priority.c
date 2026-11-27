/* mnWindowSprSetPriority, the same in both games. Both games'
   window/screens_window_sprites.c include it where the function stands. */

/* Sets the depth of the window sprite `spr` to ~priority, which is
   OAM_DEPTH_FRONT - `pri`: a higher priority draws further back. Does
   nothing when `spr` is NULL. */
void mnWindowSprSetPriority(void *spr, Sint32 pri)
{
	if(spr != NULL)
		((WINDOW_SPR *)spr)->depth = ~pri;
}
