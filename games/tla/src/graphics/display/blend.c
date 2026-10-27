/// @file
/// Display: configure brightness fades to white or black and wait for
/// completion.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "affine.h"
#include "io_reg.h"

// Fade_Update advances the fade each VBlank.
// BLDY 0 leaves brightness unchanged; 16 produces solid white or black.
//   gBlendBrighten: 0 fades to black; 1 fades to white.
//   gBlendLayers: BLDCNT target layers (the four helpers below use 0x3e:
//   BG1-3, OBJ, and backdrop). gBlendFramesLeft: remaining frame count.

/// Steps the screen fade once a frame, called from the VBlank handler: sets
/// BLDCNT to brighten or darken the chosen layers, moves BLDY one step from
/// the start level to the target level and counts gBlendFramesLeft down; at
/// the end it clears gBlendDuration, which Fade_Wait waits on.
void Fade_Update(void) {
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
      /* FAKEMATCH(one-pass): the BLDCNT store in a once-only block, so the
       * frames left's address is loaded after it; as the plain store, 2 of 48
       * instructions differ in tla-en. */
      do {
        *blendControl = control;
      } while (0);
    }
    {
      u8 *remaining = &gBlendFramesLeft;
      s32 delta;
      s32 level;
      s32 step;

      // level = target + (start - target) * remaining / duration: BLDY moves
      // in a straight line and reaches the target as remaining reaches 0
      /* FAKEMATCH(one-pass): the decrement in a once-only block, so the
       * target level's address is loaded after its store; as the plain
       * decrement, 6 of 48 instructions differ in tla-en. */
      do {
        (*remaining)--;
      } while (0);
      level = gBlendTargetLevel;
      delta = gBlendStartLevel - gBlendTargetLevel;
      step = *remaining;
      /* FAKEMATCH(wrapper): the division as a call to __divsi3, which clobbers
       * memory, so the zero test reloads *remaining; as delta * step /
       * gBlendDuration, 21 of 48 instructions differ in tla-en. */
      level += __divsi3(delta * step, gBlendDuration);
      REG_BLDY = level;
      /* FAKEMATCH(one-pass): the end test in a once-only block, so BLDY's
       * address is finished after the quotient is added; as the plain test,
       * 2 of 48 instructions differ in tla-en. */
      do {
        if (*remaining == 0)
          gBlendDuration = 0;
      } while (0);
    }
  }
}

/// Fades out to black.
/// frames: fade duration in frames.
void Fade_ToBlack(s32 frames) {
  gBlendBrighten = 0;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 16;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

/// Fades in from black.
void Fade_FromBlack(s32 frames) {
  gBlendBrighten = 0;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 0;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

/// Fades out to white.
void Fade_ToWhite(s32 frames) {
  gBlendBrighten = 1;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 16;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

/// Fades in from white.
void Fade_FromWhite(s32 frames) {
  gBlendBrighten = 1;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 0;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

/// Configures a fade.
/// bright: 0 for black; 1 for white.
/// layer: BLDCNT target layers, in the low six bits.
/// start: initial brightness; values above 16 use the current target brightness.
/// target: target brightness.
/// frames: fade duration in frames.
void Fade_Start(s8 bright, s32 layer, u32 start, s32 target, s32 frames) {
  gBlendBrighten = bright;
  gBlendLayers = layer & 0x3f;
  if (start > 16U) {
    gBlendStartLevel = gBlendTargetLevel;
  } else {
    gBlendStartLevel = start;
  }
  gBlendTargetLevel = target;
  gBlendFramesLeft = (gBlendDuration = frames);
}

/// Waits for the fade to finish.
void Fade_Wait(void) {
  if (gBlendDuration != 0) {
    do {
      WaitTicks(1);
    } while (gBlendDuration != 0);
  }
}

extern u8 gOamBucketMasks[];

void Render_ResetFrame(void) {
  gObjAffineCount = 0;
  Iwram_ClearWords(gOamBucketMasks, 32);
}

/// Takes the next of the 32 sprite affine matrices and fills it with the
/// rotation and scale of source; returns its index, or 31 when all are taken.
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  s32 xScale;
  s32 yScale;
  s32 angle;
  u8 index;

  xScale = (s16)source->scaleX;
  angle = source->angle;
  yScale = (s16)source->scaleY;
  index = gObjAffineCount;
  if (index > 31)
    return 31;

  matrix = &gObjAffineMatrices[index];
  coefficient = matrix->coefficients;
  gObjAffineCount = index + 1;
  if ((xScale == yScale || xScale == -yScale) && angle == 0) {
    s32 reciprocal;
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(0x10000, yScale);
    xReciprocal = reciprocal;
    if (xScale == -yScale)
      xReciprocal = -reciprocal;

    matrix->rows[0] = (u16)xReciprocal;
    matrix->rows[1] = reciprocal << 16;
  } else {
    s32 sine;
    s32 cosine;

    sine = Trig_Sin(angle);
    cosine = Trig_Cos(angle);
    *coefficient = cosine / xScale;
    coefficient++;
    *coefficient = sine / xScale;
    coefficient++;
    *coefficient = (-sine) / yScale;
    coefficient++;
    *coefficient = cosine / yScale;
  }

  return index;
}
