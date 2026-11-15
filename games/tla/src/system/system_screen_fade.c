/// @file
/// Screen fades through the blend registers.
// Part of system.c.

#include "system_internal.h"

enum {
  /// The layers the screen fades take: all but BG0.
  FADE_GAME_LAYERS = BLDCNT_TGT1_ALL & ~BLDCNT_TGT1_BG0,
  FADE_LAYER_MASK = BLDCNT_TGT1_ALL,
  FADE_BRIGHTEN = BLDCNT_EFFECT_LIGHTEN,
  FADE_DARKEN = BLDCNT_EFFECT_DARKEN,
  FADE_LEVEL_MAX = 16 ///< BLDY's strongest level: all white or all black.
};

// ScreenFade_Update advances the fade each VBlank.
// BLDY 0 leaves brightness unchanged; 16 produces solid white or black.
//   gScreenFadeToWhite: 0 fades to black; 1 fades to white.
//   gScreenFadeLayers: BLDCNT target layers (the four helpers below use 0x3e:
//   BG1-3, OBJ, and backdrop). gScreenFadeVBlanksLeft: VBlanks left.

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
    {
      u8 *remaining = &gScreenFadeVBlanksLeft;
      s32 delta;
      s32 level;
      s32 step;

      /* FAKEMATCH(dead): level = target + (start - target) * remaining /
       * duration: BLDY moves in a straight line and reaches the target as
       * remaining reaches 0 The decrement in a once-only block. */
      do {
        (*remaining)--;
      } while (0);
      level = gScreenFadeTargetLevel;
      delta = gScreenFadeStartLevel - gScreenFadeTargetLevel;
      step = *remaining;
      // The division written as a call to __divsi3, which clobbers memory, so
      level += __divsi3(delta * step, gScreenFadeVBlanks);
      REG_BLDY = level;
      /* FAKEMATCH(dead): the end test in a once-only block. */
      do {
        if (*remaining == 0)
          gScreenFadeVBlanks = 0;
      } while (0);
    }
  }
}

/// Fades out to black.
/// vblanks: fade duration in VBlanks.
void ScreenFade_ToBlack(s32 vblanks) {
  gScreenFadeToWhite = 0;
  gScreenFadeLayers = 0x3e;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = 16;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Fades in from black.
void ScreenFade_FromBlack(s32 vblanks) {
  gScreenFadeToWhite = 0;
  gScreenFadeLayers = 0x3e;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = 0;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Fades out to white.
void ScreenFade_ToWhite(s32 vblanks) {
  gScreenFadeToWhite = 1;
  gScreenFadeLayers = 0x3e;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = 16;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Fades in from white.
void ScreenFade_FromWhite(s32 vblanks) {
  gScreenFadeToWhite = 1;
  gScreenFadeLayers = 0x3e;
  gScreenFadeStartLevel = gScreenFadeTargetLevel;
  gScreenFadeTargetLevel = 0;
  gScreenFadeVBlanks = vblanks;
  gScreenFadeVBlanksLeft = gScreenFadeVBlanks;
}

/// Starts a fade of any layers by setting the gBlend globals ScreenFade_Update
/// steps. Reached through the far-call table.
///   bright  : 0 toward black, 1 toward white
///   layer   : the BLDCNT layers to fade (low six bits)
///   start   : the starting brightness; above 16, the current target
///   target  : the brightness to end at
///   vblanks : how long the fade takes, in VBlanks; with 0 nothing is
///             written, but `target` still becomes the current target
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
