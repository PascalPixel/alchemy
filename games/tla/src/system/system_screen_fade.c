/* @file */
/* Screen fades through the blend registers. */
/* Part of system.c. */

enum {
	/* The layers the four plain fades take: all but BG0. */
	FADE_GAME_LAYERS = BLDCNT_TGT1_ALL & ~BLDCNT_TGT1_BG0,
	FADE_LAYER_MASK = BLDCNT_TGT1_ALL, /* the layers a fade can take */
	/* BLDY's strongest level: all white or all black; 0 leaves the screen as */
	/* drawn. */
	FADE_LEVEL_MAX = 16
};

/* Steps the screen fade once a VBlank, called from the VBlank handler: sets */
/* BLDCNT to brighten or darken the chosen layers, moves BLDY one step from */
/* the start level to the target level and counts ScreenFadeVBlanksLeft down; */
/* at the end it clears ScreenFadeVBlanks, which syScrFadeWait waits on. */
void syScrFadeUpdate(void) {
	if(ScreenFadeVBlanks != 0) {
		{
			volatile u16 *blendControl;
			u32 control;

			if(ScreenFadeToWhite != 0) {
				control = ScreenFadeLayers | BLDCNT_EFFECT_LIGHTEN;
				blendControl = &REG_BLDCNT;
			}
			else {
				control = ScreenFadeLayers | BLDCNT_EFFECT_DARKEN;
				blendControl = &REG_BLDCNT;
			}
			*blendControl = control;
		}
		/* BLDY = target + (start - target) * remaining / duration: it moves in
		 * a */
		/* straight line and reaches the target as remaining reaches 0. */
		ScreenFadeVBlanksLeft--;
		syIoWrite16(ScreenFadeTargetLevel +
						(ScreenFadeStartLevel - ScreenFadeTargetLevel) *
							ScreenFadeVBlanksLeft / ScreenFadeVBlanks,
					&REG_BLDY);
		if(ScreenFadeVBlanksLeft == 0)
			ScreenFadeVBlanks = 0;
	}
}

/* Fades out to black over `vblanks` VBlanks. */
void syScrFadeToBlack(s32 vblanks) {
	ScreenFadeToWhite = 0;
	ScreenFadeLayers = FADE_GAME_LAYERS;
	ScreenFadeStartLevel = ScreenFadeTargetLevel;
	ScreenFadeTargetLevel = FADE_LEVEL_MAX;
	ScreenFadeVBlanks = vblanks;
	ScreenFadeVBlanksLeft = ScreenFadeVBlanks;
}

/* Fades in from black. */
void syScrFadeFromBlack(s32 vblanks) {
	ScreenFadeToWhite = 0;
	ScreenFadeLayers = FADE_GAME_LAYERS;
	ScreenFadeStartLevel = ScreenFadeTargetLevel;
	ScreenFadeTargetLevel = 0;
	ScreenFadeVBlanks = vblanks;
	ScreenFadeVBlanksLeft = ScreenFadeVBlanks;
}

/* Fades out to white. */
void syScrFadeToWhite(s32 vblanks) {
	ScreenFadeToWhite = 1;
	ScreenFadeLayers = FADE_GAME_LAYERS;
	ScreenFadeStartLevel = ScreenFadeTargetLevel;
	ScreenFadeTargetLevel = FADE_LEVEL_MAX;
	ScreenFadeVBlanks = vblanks;
	ScreenFadeVBlanksLeft = ScreenFadeVBlanks;
}

/* Fades in from white. */
void syScrFadeFromWhite(s32 vblanks) {
	ScreenFadeToWhite = 1;
	ScreenFadeLayers = FADE_GAME_LAYERS;
	ScreenFadeStartLevel = ScreenFadeTargetLevel;
	ScreenFadeTargetLevel = 0;
	ScreenFadeVBlanks = vblanks;
	ScreenFadeVBlanksLeft = ScreenFadeVBlanks;
}

/* Starts a fade of any layers, toward white when `bright` is 1 and toward */
/* black when 0, from level `start` (above FADE_LEVEL_MAX: the level the last */
/* fade ended at) to `target` over `vblanks` VBlanks; `layer` holds the BLDCNT
 */
/* target layers. With `vblanks` 0 nothing is written, but `target` still */
/* becomes the level a later fade starts from. Reached through the stub call */
/* table. */
void syScrFadeStart(s8 bright, s32 layer, u32 start, s32 target, s32 vblanks) {
	ScreenFadeToWhite = bright;
	ScreenFadeLayers = layer & FADE_LAYER_MASK;
	if(start > FADE_LEVEL_MAX) {
		ScreenFadeStartLevel = ScreenFadeTargetLevel;
	}
	else {
		ScreenFadeStartLevel = start;
	}
	ScreenFadeTargetLevel = target;
	ScreenFadeVBlanksLeft = (ScreenFadeVBlanks = vblanks);
}

/* Waits a frame at a time until the running fade has finished. */
void syScrFadeWait(void) {
	while(ScreenFadeVBlanks != 0) {
		sySynch(1);
	}
}
