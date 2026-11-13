/// @file
/// The frame's sprite lists: affine matrices and depth slots.
// Part of system.c.

#include "../system_internal.h"

void Oam_ResetFrame(void) {
  gObjAffineCount = 0;
  Iwram_ClearWordsEntry(gOamBucketMasks, 32);
}

/// Takes the next of the 32 sprite affine matrices and fills it with the
/// rotation and scale of source; returns its index, or 31 when all are taken.
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

/// Link entry at the head of its priority bucket (0-239).
void Func_080140d8(void *entry, s32 slot) {
  u8 *mask;
  void **buckets;
  s32 bit;

  if ((u32)slot > 239) {
    if (slot < 0)
      slot = 0;
    else
      slot = 239;
  }
  mask = &gOamBucketMasks[(u32)slot >> 3];
  bit = slot & 7;
  buckets = gOamBuckets;
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

/// Link entry at the head of its priority bucket.
void Oam_AddSpriteAtDepth(void *entry, s32 slot) {
  u8 *mask;
  void **head;
  s32 bit;

  if (slot > 255)
    slot = 255;
  if (slot < 0)
    slot = 0;
  mask = &gOamBucketMasks[(u32)slot >> 3];
  head = &gOamBuckets[slot];
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
