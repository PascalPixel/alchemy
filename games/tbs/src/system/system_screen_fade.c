// Screen fades through the blend registers: the screen brightens toward white or darkens toward
// black, BLDY running from 0, the picture as drawn, to FADE_LEVEL_MAX, all white or all black, one
// step each VBlank. Part of system.c.

enum
{
	// The layers the screen fades take: all but BG0.
	FADE_GAME_LAYERS = BLDCNT_TGT1_ALL & ~BLDCNT_TGT1_BG0,
	FADE_LAYER_MASK  = BLDCNT_TGT1_ALL,
	FADE_BRIGHTEN    = BLDCNT_EFFECT_LIGHTEN,
	FADE_DARKEN      = BLDCNT_EFFECT_DARKEN,
	FADE_LEVEL_MAX   = BLEND_FADE_MAX // BLDY's strongest: all white or black
};

// Steps the screen fade once a VBlank, called by syVBlankHandler: sets BLDCNT to brighten or darken
// the chosen layers, moves BLDY one step from the start level to the target level and counts
// ScreenFadeFramesLeft down; at the end it clears ScreenFadeFrames, which syScreenFadeWait waits
// on.
void syScreenFadeUpdate(void)
{
	if (ScreenFadeFrames != 0)
	{
		{
			volatile Uint16 *blendControl;
			Uint32           control;

			if (ScreenFadeToWhite != 0)
			{
				control      = ScreenFadeLayers | FADE_BRIGHTEN;
				blendControl = &REG_BLDCNT;
			}
			else
			{
				control      = ScreenFadeLayers | FADE_DARKEN;
				blendControl = &REG_BLDCNT;
			}
			*blendControl = control;
		}
		{
			Sint32 level;

			// level = target + (start - target) * remaining / duration: BLDY moves in a straight
			// line and reaches the target as remaining reaches 0
			ScreenFadeFramesLeft--;
			level  = ScreenFadeTargetLevel;
			level += (ScreenFadeStartLevel - ScreenFadeTargetLevel) * ScreenFadeFramesLeft
			       / ScreenFadeFrames;
			REG_BLDY = level;
			if (ScreenFadeFramesLeft == 0)
				ScreenFadeFrames = 0;
		}
	}
}

// Fades BG1 to BG3, the sprites and the backdrop from the current target brightness to `level` over
// `frames` frames.
#define FADE_DEFINE_BLEND(name, brighten, level)                                                   \
	void name(Sint32 frames)                                                                       \
	{                                                                                              \
		ScreenFadeToWhite     = brighten;                                                          \
		ScreenFadeLayers      = FADE_GAME_LAYERS;                                                  \
		ScreenFadeStartLevel  = ScreenFadeTargetLevel;                                             \
		ScreenFadeTargetLevel = level;                                                             \
		ScreenFadeFrames      = frames;                                                            \
		ScreenFadeFramesLeft  = ScreenFadeFrames;                                                  \
	}

// Fades out to black.
FADE_DEFINE_BLEND(syScreenFadeToBlack, 0, FADE_LEVEL_MAX)

// Fades in from black.
FADE_DEFINE_BLEND(syScreenFadeFromBlack, 0, 0)

// Fades out to white.
FADE_DEFINE_BLEND(syScreenFadeToWhite, 1, FADE_LEVEL_MAX)

// Fades in from white.
FADE_DEFINE_BLEND(syScreenFadeFromWhite, 1, 0)

// Starts a fade of any layers by setting the ScreenFade globals syScreenFadeUpdate steps. Reached
// through the stub table.
//       bright  : 0 toward black, 1 toward white
//       layer   : the BLDCNT layers to fade (low six bits)
//       start   : the starting brightness; above 16, the current target
//       target  : the brightness to end at
//       frames : how long the fade takes, in frames; with 0 nothing is
//                         written, but `target` still becomes the current
// target
void syScreenFadeStart(Sint8 bright, Sint32 layer, Uint32 start, Sint32 target, Sint32 frames)
{
	ScreenFadeToWhite = bright;
	ScreenFadeLayers  = layer & FADE_LAYER_MASK;
	if (start > FADE_LEVEL_MAX)
	{
		ScreenFadeStartLevel = ScreenFadeTargetLevel;
	}
	else
	{
		ScreenFadeStartLevel = start;
	}
	ScreenFadeTargetLevel = target;
	ScreenFadeFramesLeft  = (ScreenFadeFrames = frames);
}

// Waits a tick at a time until the running fade has finished.
void syScreenFadeWait(void)
{
	while (ScreenFadeFrames != 0)
	{
		sySynch(1);
	}
}

#undef FADE_DEFINE_BLEND
