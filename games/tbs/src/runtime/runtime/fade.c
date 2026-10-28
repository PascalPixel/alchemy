/// @file
/// Screen fades through the blend registers.
// Part of runtime.c.

#include "../runtime_internal.h"

enum {
  /// The layers the screen fades take: all but BG0.
  FADE_GAME_LAYERS = BLDCNT_TGT1_ALL & ~BLDCNT_TGT1_BG0,
  FADE_LAYER_MASK = BLDCNT_TGT1_ALL,
  FADE_BRIGHTEN = BLDCNT_EFFECT_LIGHTEN,
  FADE_DARKEN = BLDCNT_EFFECT_DARKEN,
  FADE_LEVEL_MAX = 16 ///< BLDY's strongest level: all white or all black.
};

/// Steps the screen fade once a VBlank, called by System_VBlankHandler: sets
/// BLDCNT to brighten or darken the chosen layers, moves BLDY one step from
/// the start level to the target level and counts gScreenFadeVBlanksLeft down;
/// at the end it clears gScreenFadeVBlanks, which ScreenFade_Wait waits on.
/// While gScreenFadeVBlanks is 0 it writes nothing, leaving BLDCNT and BLDY as
/// they were last written.
void ScreenFade_Update(void) {
  if (gScreenFadeVBlanks != 0) {
    {
      volatile u16 *blendControl;
      u32 control;

      if (gScreenFadeToWhite != 0) {
        control = gScreenFadeLayers | FADE_BRIGHTEN;
        blendControl = &REG_BLDCNT;
      } else {
        control = gScreenFadeLayers | FADE_DARKEN;
        blendControl = &REG_BLDCNT;
      }
      *blendControl = control;
    }
    {
      u8 *remaining = &gScreenFadeVBlanksLeft;
      s32 delta;
      s32 level;
      s32 step;

      // level = target + (start - target) * remaining / duration: BLDY moves
      // in a straight line and reaches the target as remaining reaches 0
      (*remaining)--;
      level = gScreenFadeTargetLevel;
      delta = gScreenFadeStartLevel - gScreenFadeTargetLevel;
      step = *remaining;
      /* FAKEMATCH(wrapper): the division as a call to __divsi3, which clobbers
       * memory, so the zero test reloads *remaining; as delta * step /
       * gScreenFadeVBlanks, 8 of 46 instructions differ in tbs-en. */
      level += __divsi3(delta * step, gScreenFadeVBlanks);
      REG_BLDY = level;
      if (*remaining == 0)
        gScreenFadeVBlanks = 0;
    }
  }
}

// Screen fades. ScreenFade_Update moves the fade one step each VBlank.
// The brightness (BLDY) runs from 0, the picture as drawn, to 16, all white
// or all black.
//   gScreenFadeToWhite     : 0 fades toward black, 1 toward white
//   gScreenFadeLayers      : the BLDCNT layers the fade covers
//   gScreenFadeVBlanksLeft : VBlanks until the fade ends

/// Fades BG1 to BG3, the sprites and the backdrop (0x3e) from the current
/// target brightness to `level` over `vblanks` VBlanks.
#define FADE_DEFINE_BLEND(name, brighten, level)                               \
  void name(s32 vblanks) {                                                     \
    gScreenFadeToWhite = brighten;                                             \
    gScreenFadeLayers = FADE_GAME_LAYERS;                                      \
    gScreenFadeStartLevel = gScreenFadeTargetLevel;                            \
    gScreenFadeTargetLevel = level;                                            \
    gScreenFadeVBlanks = vblanks;                                              \
    gScreenFadeVBlanksLeft = gScreenFadeVBlanks;                               \
  }

/// Fades out to black.
FADE_DEFINE_BLEND(ScreenFade_ToBlack, 0, FADE_LEVEL_MAX)

/// Fades in from black.
FADE_DEFINE_BLEND(ScreenFade_FromBlack, 0, 0)

/// Fades out to white.
FADE_DEFINE_BLEND(ScreenFade_ToWhite, 1, FADE_LEVEL_MAX)

/// Fades in from white.
FADE_DEFINE_BLEND(ScreenFade_FromWhite, 1, 0)

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

/// Waits a frame at a time until the running fade has finished. Called by
/// the Title and the battle, and through the far-call table.
void ScreenFade_Wait(void) {
  if (gScreenFadeVBlanks != 0) {
    do {
      wait(1);
    } while (gScreenFadeVBlanks != 0);
  }
}

#undef FADE_DEFINE_BLEND
