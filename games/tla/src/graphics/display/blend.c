//--------------------------------------------------------------------------
// blend.c
// Display: configure brightness fades to white or black and wait for
// completion.
//--------------------------------------------------------------------------

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

// Fade_Update advances the fade each VBlank.
// BLDY 0 leaves brightness unchanged; 16 produces solid white or black.
//   gBlendBrighten: 0 fades to black; 1 fades to white.
//   gBlendLayers: BLDCNT target layers (the four helpers below use 0x3e:
//   BG1-3, OBJ, and backdrop). gBlendFramesLeft: remaining frame count.

//--------------------------------------------------------------------------
// Fade out to black.
// frames: fade duration in frames.
//--------------------------------------------------------------------------
void Fade_ToBlack(s32 frames) {
  gBlendBrighten = 0;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 16;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// Fade in from black.
//--------------------------------------------------------------------------
void Fade_FromBlack(s32 frames) {
  gBlendBrighten = 0;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 0;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// Fade out to white.
//--------------------------------------------------------------------------
void Fade_ToWhite(s32 frames) {
  gBlendBrighten = 1;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 16;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// Fade in from white.
//--------------------------------------------------------------------------
void Fade_FromWhite(s32 frames) {
  gBlendBrighten = 1;
  gBlendLayers = 0x3e;
  gBlendStartLevel = gBlendTargetLevel;
  gBlendTargetLevel = 0;
  gBlendDuration = frames;
  gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// Configure a fade.
// bright: 0 for black; 1 for white.
// layer: BLDCNT target layers, in the low six bits.
// start: initial brightness; values above 16 use the current target brightness.
// target: target brightness.
// frames: fade duration in frames.
//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
// Wait for the fade to finish.
//--------------------------------------------------------------------------
void Fade_Wait(void) {
  if (gBlendDuration != 0) {
    do {
      WaitFrames(1);
    } while (gBlendDuration != 0);
  }
}

extern u8 gOamBucketMasks[];

void Render_ResetFrame(void) {
  gObjAffineCount = 0;
  Iwram_ClearWords(gOamBucketMasks, 32);
}

// Takes the next of the 32 sprite affine matrices and fills it with the
// rotation and scale of source; returns its index, or 31 when all are taken.
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
