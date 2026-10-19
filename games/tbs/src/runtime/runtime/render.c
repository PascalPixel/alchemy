/// @file
/// Part of runtime.c, the runtime object, compiled only through it.
/// The frame's sprite lists: affine matrices, depth slots and the OAM build.

#include "../runtime_internal.h"

extern u8 IwramClearWords[];

/// Starts a new frame of sprites: frees the affine matrices and empties the
/// 256 depth slots sprites are linked into.
void Render_ResetFrame(void) {
  gObjAffineCount = 0;
  _call_via_r3(((u32)&gRenderSlotHeads), 0x400, ((u32)&gObjAffineCount),
               (u32)IwramClearWords);
}

typedef void (*OamBuildRoutine)(void *work);

enum { OBJ_AFFINE_MATRIX_COUNT = 32, RENDER_DEPTH_SLOT_COUNT = 256 };

/// Hands out the next OAM affine matrix for a sprite drawn with `source`'s
/// scale and rotation and fills it in. Returns the matrix number, or 0 when
/// all 32 are taken.
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  s32 xScale;
  s32 yScale;
  s32 angle;
  u8 index;

  index = gObjAffineCount;
  xScale = (s16)source->scaleX;
  yScale = (s16)source->scaleY;
  angle = source->angle;
  if (index >= OBJ_AFFINE_MATRIX_COUNT)
    return 0;

  matrix = &gObjAffineMatrices[index];
  coefficient = matrix->coefficients;
  if ((xScale == yScale || -xScale == yScale) && angle == 0) {
    // Unrotated and evenly scaled (or mirrored): only the diagonal is set
    s32 reciprocal;
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(0x10000, yScale);
    xReciprocal = reciprocal;
    if (-xScale == yScale)
      xReciprocal = -reciprocal;

    matrix->rows[0] = (u16)xReciprocal;
    matrix->rows[1] = (u32)reciprocal << 16;
  } else {
    // The inverse of rotate-then-scale
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

  gObjAffineCount = index + 1;
  return index;
}

/// Links a sprite entry into depth slot `slot` (clamped to 0..255) for this
/// frame. The entry's first word becomes its link to the next entry.
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
/// depth slots, with a RAM copy of the ARM builder.
void Render_BuildOamList(void *work) {
  OamBuildRoutine routine;

  RamCode_Load(routine, Render_BuildOamListKernel, LoadedRuntime_Size);
  routine(work);
  MemFree((void *)routine);
}
