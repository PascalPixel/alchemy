/// @file
/// The frame's sprite lists: affine matrices, depth slots and the OAM build.
// Part of runtime.c.

#include "../runtime_internal.h"

extern u8 IwramClearWords[];

/// Starts a new frame of sprites: frees the affine matrices and empties the
/// 256 depth slots sprites are linked into. Called by WaitFrames after each
/// VBlank.
void Render_ResetFrame(void) {
  gSpriteAffineCount = 0;
  // IwramClearWords(destination, bytes): 0x400 bytes are the 256 slot heads.
  // It zeroes r2 itself, so the third argument is never read.
  _call_via_r3(((u32)&gRenderSlotHeads), 0x400, ((u32)&gSpriteAffineCount),
               (u32)IwramClearWords);
}

typedef void (*OamBuildRoutine)(void *work);

enum {
  OBJ_AFFINE_MATRIX_COUNT = 32, ///< the OAM's rotation-scaling matrices
  RENDER_DEPTH_SLOT_COUNT = 256 ///< depth slots a sprite is linked into
};

/// Hands out the next OAM affine matrix for a sprite drawn with `source`'s
/// scale and rotation and fills it in. Called by the field sprites and the
/// windows' scaled sprites. Returns the matrix number, 0 to 31. When all 32
/// are taken it fills nothing and still returns 0, which a caller cannot tell
/// from matrix 0: the sprite is drawn with whatever matrix 0 holds.
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  s32 xScale;
  s32 yScale;
  s32 angle;
  u8 index;

  index = gSpriteAffineCount;
  xScale = (s16)source->scaleX;
  yScale = (s16)source->scaleY;
  angle = source->angle;
  if (index >= OBJ_AFFINE_MATRIX_COUNT)
    return 0;

  matrix = &gSpriteAffineMatrices[index];
  coefficient = matrix->coefficients;
  if ((xScale == yScale || -xScale == yScale) && angle == 0) {
    // Unrotated and evenly scaled (or mirrored): only the diagonal is set.
    // 0x10000 / scale is the 8.8 inverse of an 8.8 scale.
    s32 reciprocal;
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(0x10000, yScale);
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

  gSpriteAffineCount = index + 1;
  return index;
}

/// Links a sprite entry into depth slot `slot` (clamped to 0..255) for this
/// frame. The entry's first word becomes its link to the next entry. Called
/// by everything that draws a sprite, before WaitFrames builds the OAM list.
void Runtime_PushSlotEntry(void *slotEntry, s32 slot) {
  void **head;
  void *previousHead;

  if (slot >= RENDER_DEPTH_SLOT_COUNT)
    slot = RENDER_DEPTH_SLOT_COUNT - 1;
  if (slot < 0)
    slot = 0;
  head = &gRenderSlotHeads[slot];
  previousHead = *head;
  *head = slotEntry;
  *(void **)slotEntry = previousHead;
}

/// Builds this frame's OAM list in `work` from the sprites linked into the
/// depth slots, with a RAM copy of the ARM builder. Called by WaitFrames into
/// the frame scratch heap slot.
void Render_BuildOamList(void *work) {
  OamBuildRoutine routine;

  RamCode_Load(routine, Render_BuildOamListKernel, LoadedRuntime_Size);
  routine(work);
  Free((void *)routine);
}
