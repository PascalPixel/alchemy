/* syScreenFadeStart and syScreenFadeWait, the same in both games. Both
   games' system/system_screen_fade.c include it where the functions stand. */

/* Starts a fade of any layers, toward white when `bright` is 1 and toward black
   when 0, from level `start` (above FADE_LEVEL_MAX: the level the last fade
   ended at) to `tgt` over `frames` frames; `layer` holds the BLDCNT target
   layers. With `frames` 0 nothing is written, but `tgt` still becomes the
   level a later fade starts from. Reached through the stub call table. */
void syScreenFadeStart(
	Sint8 bright, Sint32 layer, Uint32 start, Sint32 tgt, Sint32 frames)
{
	ScreenFadeToWhite = bright;
	ScreenFadeLayers = layer & FADE_LAYER_MASK;
	if(start > FADE_LEVEL_MAX) {
		ScreenFadeStartLevel = ScreenFadeTargetLevel;
	} else {
		ScreenFadeStartLevel = start;
	}
	ScreenFadeTargetLevel = tgt;
	ScreenFadeFramesLeft = (ScreenFadeFrames = frames);
}

/* Waits a tick at a time until the running fade has finished. */
void syScreenFadeWait(void)
{
	while(ScreenFadeFrames != 0) {
		sySynch(1);
	}
}
