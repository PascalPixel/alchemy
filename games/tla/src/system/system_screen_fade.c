/// @file
/// Screen fades through the blend registers.
// Part of system.c.

#include "system_internal.h"

enum {
  /// The layers the four plain fades take: all but BG0.
  FADE_GAME_LAYERS = BLDCNT_TGT1_ALL & ~BLDCNT_TGT1_BG0,
  FADE_LAYER_MASK = BLDCNT_TGT1_ALL, ///< the layers a fade can take
  /// BLDY's strongest level: all white or all black; 0 leaves the screen as
  /// drawn.
  FADE_LEVEL_MAX = 16
};

/// Steps the screen fade once a VBlank, called from the VBlank handler: sets
/// BLDCNT to brighten or darken the chosen layers, moves BLDY one step from
/// the start level to the target level and counts gScreenFadeVBlanksLeft down;
/// at the end it clears gScreenFadeVBlanks, which ScreenFade_Wait waits on.
void ScreenFade_Update(void) {
  if (gScreenFadeVBlanks != 0) {
    {
      volatile u16 *blendControl;
      u32 control;

      if (gScreenFadeToWhite != 0) {
        control = gScreenFadeLayers | BLDCNT_EFFECT_LIGHTEN;
        blendControl = &REG_BLDCNT;
      } else {
        control = gScreenFadeLayers | BLDCNT_EFFECT_DARKEN;
        blendControl = &REG_BLDCNT;
      }
      *blendControl = control;
    }
    // BLDY = target + (start - target) * remaining / duration: it moves in a
    // straight line and reaches the target as remaining reaches 0.
    gScreenFadeVBlanksLeft--;
    Io_Write16(gScreenFadeTargetLevel +
                   (gScreenFadeStartLevel - gScreenFadeTargetLevel) *
                       gScreenFadeVBlanksLeft / gScreenFadeVBlanks,
               &REG_BLDY);
    if (gScreenFadeVBlanksLeft == 0)
      gScreenFadeVBlanks = 0;
  }
}

/// Fades out to black over `vblanks` VBlanks.
void ScreenFade_ToBlack(s32 vblanks) {
  gScreenFadeToWhite = 0;
  gScreenFadeLayers = FADE_GAME_LAYERS;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = FADE_LEVEL_MAX;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Fades in from black.
void ScreenFade_FromBlack(s32 vblanks) {
  gScreenFadeToWhite = 0;
  gScreenFadeLayers = FADE_GAME_LAYERS;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = 0;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Fades out to white.
void ScreenFade_ToWhite(s32 vblanks) {
  gScreenFadeToWhite = 1;
  gScreenFadeLayers = FADE_GAME_LAYERS;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = FADE_LEVEL_MAX;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Fades in from white.
void ScreenFade_FromWhite(s32 vblanks) {
  gScreenFadeToWhite = 1;
  gScreenFadeLayers = FADE_GAME_LAYERS;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = 0;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Starts a fade of any layers, toward white when `bright` is 1 and toward
/// black when 0, from level `start` (above FADE_LEVEL_MAX: the level the last
/// fade ended at) to `target` over `vblanks` VBlanks; `layer` holds the BLDCNT
/// target layers. With `vblanks` 0 nothing is written, but `target` still
/// becomes the level a later fade starts from. Reached through the far-call
/// table.
void ScreenFade_Start(s8 bright, s32 layer, u32 start, s32 target,
                      s32 vblanks) {
  gScreenFadeToWhite = bright;
  gScreenFadeLayers = layer & FADE_LAYER_MASK;
  if (start > FADE_LEVEL_MAX) {
    gScreenFadeStartLevel = gScreenFadeTargetLevel;
  } else {
    gScreenFadeStartLevel = start;
  }
  gScreenFadeTargetLevel = target;
  gScreenFadeVBlanksLeft = (gScreenFadeVBlanks = vblanks);
}

/// Waits a frame at a time until the running fade has finished.
void ScreenFade_Wait(void) {
  while (gScreenFadeVBlanks != 0) {
    wait(1);
  }
}
