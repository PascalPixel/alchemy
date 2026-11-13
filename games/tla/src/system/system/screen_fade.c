/// @file
/// Screen fades through the blend registers.
// Part of system.c.

#include "../system_internal.h"

// ScreenFade_Update advances the fade each VBlank.
// BLDY 0 leaves brightness unchanged; 16 produces solid white or black.
//   gBlendBrighten: 0 fades to black; 1 fades to white.
//   gBlendLayers: BLDCNT target layers (the four helpers below use 0x3e:
//   BG1-3, OBJ, and backdrop). gBlendVBlanksLeft: VBlanks left.

/// Steps the screen fade once a VBlank, called from the VBlank handler: sets
/// BLDCNT to brighten or darken the chosen layers, moves BLDY one step from
/// the start level to the target level and counts gBlendVBlanksLeft down; at
/// the end it clears gBlendDuration, which ScreenFade_Wait waits on.
void ScreenFade_Update(void) {
  if (gBlendDuration != 0) {
    {
      volatile u16 *blendControl;
      u32 control;

      if (gBlendBrighten != 0) {
        control = gBlendLayers | BLDCNT_EFFECT_LIGHTEN;
        blendControl = &REG_BLDCNT;
      } else {
        control = gBlendLayers | BLDCNT_EFFECT_DARKEN;
        blendControl = &REG_BLDCNT;
      }
      *blendControl = control;
    }
    {
      u8 *remaining = &gBlendVBlanksLeft;
      s32 delta;
      s32 level;
      s32 step;

      /* FAKEMATCH(dead): level = target + (start - target) * remaining /
       * duration: BLDY moves in a straight line and reaches the target as
       * remaining reaches 0 The decrement in a once-only block. */
      do {
        (*remaining)--;
      } while (0);
      level = gBlendTargetLevel;
      delta = gBlendStartLevel - gBlendTargetLevel;
      step = *remaining;
      // The division written as a call to __divsi3, which clobbers memory, so
      level += __divsi3(delta * step, gBlendDuration);
      REG_BLDY = level;
      /* FAKEMATCH(dead): the end test in a once-only block. */
      do {
        if (*remaining == 0)
          gBlendDuration = 0;
      } while (0);
    }
  }
}

/// Fades out to black.
/// vblanks: fade duration in VBlanks.
void ScreenFade_ToBlack(s32 vblanks) {
  gBlendBrighten = 0;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 16;
  gBlendDuration = vblanks;
  gBlendVBlanksLeft = gBlendDuration;
}

/// Fades in from black.
void ScreenFade_FromBlack(s32 vblanks) {
  gBlendBrighten = 0;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 0;
  gBlendDuration = vblanks;
  gBlendVBlanksLeft = gBlendDuration;
}

/// Fades out to white.
void ScreenFade_ToWhite(s32 vblanks) {
  gBlendBrighten = 1;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 16;
  gBlendDuration = vblanks;
  gBlendVBlanksLeft = gBlendDuration;
}

/// Fades in from white.
void ScreenFade_FromWhite(s32 vblanks) {
  gBlendBrighten = 1;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 0;
  gBlendDuration = vblanks;
  gBlendVBlanksLeft = gBlendDuration;
}

/// Configures a fade.
/// bright: 0 for black; 1 for white.
/// layer: BLDCNT target layers, in the low six bits.
/// start: initial brightness; values above 16 use the current target
/// brightness. target: target brightness. vblanks: fade duration in VBlanks.
void ScreenFade_Start(s8 bright, s32 layer, u32 start, s32 target,
                      s32 vblanks) {
  gBlendBrighten = bright;
  gBlendLayers = layer & 0x3f;
  if (start > 16U) {
    gBlendStartLevel = gBlendTargetLevel;
  } else {
    gBlendStartLevel = start;
  }
  gBlendTargetLevel = target;
  gBlendVBlanksLeft = (gBlendDuration = vblanks);
}

/// Waits for the fade to finish.
void ScreenFade_Wait(void) {
  if (gBlendDuration != 0) {
    do {
      wait(1);
    } while (gBlendDuration != 0);
  }
}
