// Screen fades through the blend registers. Part of system.c.

enum
{
    // The layers the four plain fades take: all but BG0.
    FADE_GAME_LAYERS = BLDCNT_TGT1_ALL & ~BLDCNT_TGT1_BG0,
    FADE_LAYER_MASK  = BLDCNT_TGT1_ALL, // the layers a fade can take
                                        // BLDY's strongest level: all white or all black; 0 leaves
                                        // the screen as drawn.
    FADE_LEVEL_MAX   = 16
};

// Steps the screen fade once a VBlank, called from the VBlank handler: sets BLDCNT to brighten or
// darken the chosen layers, moves BLDY one step from the start level to the target level and counts
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
                control      = ScreenFadeLayers | BLDCNT_EFFECT_LIGHTEN;
                blendControl = &REG_BLDCNT;
            }
            else
            {
                control      = ScreenFadeLayers | BLDCNT_EFFECT_DARKEN;
                blendControl = &REG_BLDCNT;
            }
            *blendControl = control;
        }
        // BLDY = target + (start - target) * remaining / duration: it moves in a straight line and
        // reaches the target as remaining reaches 0.
        ScreenFadeFramesLeft--;
        syIOWrite16(ScreenFadeTargetLevel
                        + (ScreenFadeStartLevel - ScreenFadeTargetLevel) * ScreenFadeFramesLeft
                              / ScreenFadeFrames,
                    &REG_BLDY);
        if (ScreenFadeFramesLeft == 0)
            ScreenFadeFrames = 0;
    }
}

// Fades out to black over `frames` frames.
void syScreenFadeToBlack(Sint32 frames)
{
    ScreenFadeToWhite     = 0;
    ScreenFadeLayers      = FADE_GAME_LAYERS;
    ScreenFadeStartLevel  = ScreenFadeTargetLevel;
    ScreenFadeTargetLevel = FADE_LEVEL_MAX;
    ScreenFadeFrames      = frames;
    ScreenFadeFramesLeft  = ScreenFadeFrames;
}

// Fades in from black.
void syScreenFadeFromBlack(Sint32 frames)
{
    ScreenFadeToWhite     = 0;
    ScreenFadeLayers      = FADE_GAME_LAYERS;
    ScreenFadeStartLevel  = ScreenFadeTargetLevel;
    ScreenFadeTargetLevel = 0;
    ScreenFadeFrames      = frames;
    ScreenFadeFramesLeft  = ScreenFadeFrames;
}

// Fades out to white.
void syScreenFadeToWhite(Sint32 frames)
{
    ScreenFadeToWhite     = 1;
    ScreenFadeLayers      = FADE_GAME_LAYERS;
    ScreenFadeStartLevel  = ScreenFadeTargetLevel;
    ScreenFadeTargetLevel = FADE_LEVEL_MAX;
    ScreenFadeFrames      = frames;
    ScreenFadeFramesLeft  = ScreenFadeFrames;
}

// Fades in from white.
void syScreenFadeFromWhite(Sint32 frames)
{
    ScreenFadeToWhite     = 1;
    ScreenFadeLayers      = FADE_GAME_LAYERS;
    ScreenFadeStartLevel  = ScreenFadeTargetLevel;
    ScreenFadeTargetLevel = 0;
    ScreenFadeFrames      = frames;
    ScreenFadeFramesLeft  = ScreenFadeFrames;
}

// Starts a fade of any layers, toward white when `bright` is 1 and toward black when 0, from level
// `start` (above FADE_LEVEL_MAX: the level the last fade ended at) to `target` over `frames`
// frames; `layer` holds the BLDCNT target layers. With `frames` 0 nothing is written, but `target`
// still becomes the level a later fade starts from. Reached through the stub call table.
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
