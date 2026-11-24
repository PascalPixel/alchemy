/// @file
/// The frame's sprite lists: affine matrices and depth slots.
// Part of system.c.

#include "system_internal.h"
#include "fixed_math.h"

enum {
  OBJ_AFFINE_MATRIX_COUNT = 32, ///< the OAM's rotation-scaling matrices
  RENDER_DEPTH_SLOT_COUNT = 256 ///< depth slots a sprite is linked into
};

/// Starts a new frame of sprites: frees the affine matrices and clears the
/// masks of the depth slots sprites are linked into, a bit a slot.
void Oam_ResetFrame(void) {
  gOamMatrixCount = 0;
  Iwram_ClearWordsEntry(gOamDepthMasks, RENDER_DEPTH_SLOT_COUNT / 8);
}

/// Hands out the next OAM affine matrix for a sprite drawn with `source`'s
/// scale and rotation and fills it in. Returns the matrix number, or the last
/// (OBJ_AFFINE_MATRIX_COUNT - 1) when all are taken.
s32 OamMatrix_AllocFromTransform(struct AffineTransform *source) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  s32 xScale;
  s32 yScale;
  s32 angle;
  u8 index;

  xScale = (s16)source->scaleX;
  angle = source->angle;
  yScale = (s16)source->scaleY;
  index = gOamMatrixCount;
  if (index >= OBJ_AFFINE_MATRIX_COUNT)
    return OBJ_AFFINE_MATRIX_COUNT - 1;

  matrix = &gOamMatrices[index];
  coefficient = matrix->coefficients;
  gOamMatrixCount = index + 1;
  if ((xScale == yScale || xScale == -yScale) && angle == 0) {
    // Unrotated and evenly scaled (or mirrored): pa and pd are the scale's
    // reciprocals (FIXED_ONE / scale, the 8.8 inverse of an 8.8 scale), pb and
    // pc 0, two halfwords to a row.
    s32 reciprocal;
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(FIXED_ONE, yScale);
    xReciprocal = reciprocal;
    if (xScale == -yScale)
      xReciprocal = -reciprocal;

    matrix->rows[0] = (u16)xReciprocal;
    matrix->rows[1] = (u32)reciprocal << 16;
  } else {
    // The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is an
    // 8.8 coefficient.
    FIXED sine;
    FIXED cosine;

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

/// As Oam_AddSpriteAtDepth, with the depth clamped below OAM_DEPTH_WINDOW, so
/// that the sprite stays behind the windows' sprites.
void Oam_AddSpriteAtDepthClamped(void *entry, s32 slot) {
  u8 *mask;
  void **buckets;
  s32 bit;

  if ((u32)slot > OAM_DEPTH_WINDOW - 1) {
    if (slot < 0)
      slot = 0;
    else
      slot = OAM_DEPTH_WINDOW - 1;
  }
  mask = &gOamDepthMasks[(u32)slot >> 3];
  bit = slot & 7;
  buckets = gOamDepthLists;
  if (((u32)*mask >> bit) & 1) {
    void **head = &buckets[slot];
    void *previous = *head;

    *head = entry;
    *(void **)entry = previous;
  } else {
    *mask |= 1 << bit;
    buckets[slot] = entry;
    *(void **)entry = NULL;
  }
}

/// Links a sprite entry at the head of depth slot `slot`'s list (clamped to the
/// RENDER_DEPTH_SLOT_COUNT slots) for this frame, and marks the slot taken.
void Oam_AddSpriteAtDepth(void *entry, s32 slot) {
  u8 *mask;
  void **head;
  s32 bit;

  if (slot >= RENDER_DEPTH_SLOT_COUNT)
    slot = RENDER_DEPTH_SLOT_COUNT - 1;
  if (slot < 0)
    slot = 0;
  mask = &gOamDepthMasks[(u32)slot >> 3];
  head = &gOamDepthLists[slot];
  bit = slot & 7;
  if (((u32)*mask >> bit) & 1) {
    void *previous = *head;

    *head = entry;
    *(void **)entry = previous;
  } else {
    *mask |= 1 << bit;
    *head = entry;
    *(void **)entry = NULL;
  }
}
