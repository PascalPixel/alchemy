/// @file
/// The frame's sprite lists: affine matrices, depth slots and the OAM build.
// Part of system.c.

#include "system_internal.h"

extern u8 IwramClearWords[];

/// Starts a new frame of sprites: frees the affine matrices and empties the 256
/// depth slots sprites are linked into.
void Oam_ResetFrame(void) {
  gOamMatrixCount = 0;
  // IwramClearWords(destination, bytes): 1024 bytes are the 256 slot heads.
  _call_via_r3(((u32)&gOamDepthLists), 1024, ((u32)&gOamMatrixCount),
               (u32)IwramClearWords);
}

typedef void (*OamBuildRoutine)(void *work);

enum {
  OBJ_AFFINE_MATRIX_COUNT = 32, ///< the OAM's rotation-scaling matrices
  RENDER_DEPTH_SLOT_COUNT = 256 ///< depth slots a sprite is linked into
};

/// Hands out the next OAM affine matrix for a sprite drawn with `source`'s
/// scale and rotation and fills it in. Returns the matrix number, 0 to 31.
s32 OamMatrix_AllocFromTransform(struct AffineTransform *source) {
  union OamMatrix *matrix;
  s16 *coefficient;
  s32 xScale;
  s32 yScale;
  s32 angle;
  u8 index;

  index = gOamMatrixCount;
  xScale = (s16)source->scaleX;
  yScale = (s16)source->scaleY;
  angle = source->angle;
  if (index >= OBJ_AFFINE_MATRIX_COUNT)
    return 0;

  matrix = &gOamMatrices[index];
  coefficient = matrix->coefficients;
  if ((xScale == yScale || -xScale == yScale) && angle == 0) {
    // Unrotated and evenly scaled (or mirrored): only the diagonal is set.
    s32 reciprocal;
    // Q16_ONE / scale is the 8.8 inverse of an 8.8 scale.
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(Q16_ONE, yScale);
    xReciprocal = reciprocal;
    if (-xScale == yScale)
      xReciprocal = -reciprocal;

    matrix->rows[0] = (u16)xReciprocal;
    matrix->rows[1] = (u32)reciprocal << 16;
  } else {
    // The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is
    // an 8.8 coefficient
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

  gOamMatrixCount = index + 1;
  return index;
}

/// Links a sprite entry into depth slot `slot` (clamped to 0..255) for this
/// frame.
void Oam_AddSpriteAtDepth(void *slotEntry, s32 slot) {
  void **head;
  void *previousHead;

  if (slot >= RENDER_DEPTH_SLOT_COUNT)
    slot = RENDER_DEPTH_SLOT_COUNT - 1;
  if (slot < 0)
    slot = 0;
  head = &gOamDepthLists[slot];
  previousHead = *head;
  *head = slotEntry;
  *(void **)slotEntry = previousHead;
}

/// Builds this frame's OAM list in `work` from the sprites linked into the
/// depth slots, with a RAM copy of the ARM builder.
void Oam_BuildBuffer(void *work) {
  OamBuildRoutine routine;

  RamCode_Load(routine, Oam_BuildBufferKernel, LoadedRuntime_Size);
  routine(work);
  Heap_Release((void *)routine);
}
