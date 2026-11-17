/// @file
/// The 3D transform, its stack, the look-at matrix and the projection.
// Part of system.c.

#include "system_internal.h"
#include "fixed_math.h"
#include "system/projection.h"

/// A point in view space, and where Render_ProjectPoint puts it on the screen.
struct ProjectedPoint {
  s32 x;
  s32 y;
  s32 z;
};

// gCurrentMatrix contains twelve s32 values in 16.16 (1.0 = 0x10000).
// Entries 0-8 form the 3x3 rotation; entries 9-11 hold the translation.
// Iwram_MultiplyMatrix multiplies the supplied matrix into the current one.

#define MATRIX_DMA DMA_COPY_WORDS(12) ///< DMA for one matrix.

/// The Lost Age obtains cosine by looking up sine ninety degrees ahead.
#define COS(a) Trig_Sin((a) + DEGtoANG(90))

/// An empty routine.
void Render_EmptyHook(void) {}

/// Allocates the matrix-save storage and sets the current matrix to the unit
/// matrix.
void Matrix_Initialize(void) {
  gMatrixStackTop =
      HeapSlot_AllocEwram(HEAP_SLOT_MATRIX_STACK, sizeof(gCurrentMatrix));
  gMatrixStackDepth = 0;
  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetCurrentUnit();
}

/// Resets the save pointer to the start of the workspace.
void Matrix_RewindStack(void) {
  gMatrixStackTop = Ram_HeapSlots->transformStack;
  gMatrixStackDepth = 0;
}

/// Saves gCurrentMatrix into the save area (one level deep; a second save is
/// ignored).
void Matrix_Push(void) {
  if (gMatrixStackDepth <= 0) {
    Dma_Set(gCurrentMatrix, gMatrixStackTop, MATRIX_DMA, REG_DMA3);
    gMatrixStackDepth++;
    gMatrixStackTop += sizeof(gCurrentMatrix);
  }
}

/// Copies gCurrentMatrix to `dst`.
void Matrix_Get(void *dst) {
  Dma_Set(gCurrentMatrix, dst, MATRIX_DMA, REG_DMA3);
}

/// Copies the matrix at `src` into gCurrentMatrix.
void Matrix_Load(const void *src) {
  Dma_Set(src, gCurrentMatrix, MATRIX_DMA, REG_DMA3);
}

/// Restores gCurrentMatrix from the save area, if Matrix_Push saved it.
void Matrix_Pop(void) {
  if (gMatrixStackDepth > 0) {
    --gMatrixStackDepth;
    gMatrixStackTop -= sizeof(gCurrentMatrix);
    Dma_Set(gMatrixStackTop, gCurrentMatrix, MATRIX_DMA, REG_DMA3);
  }
}

/// Resets gCurrentMatrix to the unit matrix. Reached through the far-call
/// table.
void Matrix_ResetUnit(void) {
  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetCurrentUnit();
}

/// Rotates gCurrentMatrix by `angles` about the X, Y and Z axes.
void Matrix_Rotate(s32 *angles) {
  FIXED sinX;
  s32 cosX;
  FIXED sinY;
  s32 cosY;
  FIXED sinZ;
  s32 cosZ;
  MATRIX matrix;

  sinX = Trig_Sin(angles[0]);

  cosX = COS(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = COS(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = COS(angles[2]);
  SET_VECTOR(matrix, Iwram_MulFX(cosY, cosZ), Iwram_MulFX(cosY, sinZ), -sinY);
  matrix[3] =
      Iwram_MulFX(Iwram_MulFX(sinX, sinY), cosZ) - Iwram_MulFX(cosX, sinZ);
  matrix[4] =
      Iwram_MulFX(Iwram_MulFX(sinX, sinY), sinZ) + Iwram_MulFX(cosX, cosZ);
  matrix[5] = Iwram_MulFX(sinX, cosY);
  matrix[6] =
      Iwram_MulFX(Iwram_MulFX(cosX, sinY), cosZ) + Iwram_MulFX(sinX, sinZ);
  matrix[7] =
      Iwram_MulFX(Iwram_MulFX(cosX, sinY), sinZ) - Iwram_MulFX(sinX, cosZ);
  matrix[8] = Iwram_MulFX(cosX, cosY);
  matrix[MATRIX_TRANSLATION_X] = 0;
  matrix[MATRIX_TRANSLATION_Y] = 0;
  matrix[MATRIX_TRANSLATION_Z] = 0;
  Iwram_MultiplyMatrix(matrix);
}

/// The unit matrix store of matrix.h's Matrix_SetUnit in ⚓️'s order,
/// the 1.0 built before the zeros, as ⚓️'s Matrix_SetCurrentUnit builds it.
static __inline__ void Matrix_StoreUnit(void *matrix) {
  CAMELOT_ASM("three stmia no C store or copy compiles to")
  __asm__ volatile(
      "mov r0, %0\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
      "mov r3, #0\n\tmov r4, #0\n\t"
      "stmia r0!, {r1, r2, r3, r4}\n\t"
      "stmia r0!, {r1, r2, r3, r4}\n\t"
      "stmia r0!, {r1, r2, r3, r4}"
      :
      : "r"(matrix)
      : "r0", "r1", "r2", "r3", "r4", "memory");
}

/// Applies a rotation about the X axis.
void Matrix_RotateX(s32 angle) {
  MATRIX matrix;
  FIXED sine;
  s32 cosine;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  cosine = COS(angle);
  matrix[4] = cosine;
  matrix[8] = cosine;
  sine = Trig_Sin(angle);
  matrix[5] = sine;
  matrix[7] = -sine;
  Iwram_MultiplyMatrix(matrix);
}

/// Applies a rotation about the Y axis.
void Matrix_RotateY(s32 angle) {
  MATRIX matrix;
  FIXED sine;
  s32 cosine;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  cosine = COS(angle);
  matrix[0] = cosine;
  matrix[8] = cosine;
  sine = Trig_Sin(angle);
  matrix[2] = -sine;
  matrix[6] = sine;
  Iwram_MultiplyMatrix(matrix);
}

/// Makes the current matrix a rotation about the Y axis.
void Matrix_SetRotateY(s32 angle) {
  FIXED *matrix = gCurrentMatrix;
  FIXED sine;
  s32 cosine;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  cosine = COS(angle);
  matrix[0] = cosine;
  matrix[8] = cosine;
  sine = Trig_Sin(angle);
  matrix[2] = -sine;
  matrix[6] = sine;
}

/// Applies a rotation about the Z axis.
void Matrix_RotateZ(s32 angle) {
  MATRIX matrix;
  FIXED sine;
  s32 cosine;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  cosine = COS(angle);
  matrix[0] = cosine;
  matrix[4] = cosine;
  sine = Trig_Sin(angle);
  matrix[1] = sine;
  matrix[3] = -sine;
  Iwram_MultiplyMatrix(matrix);
}

/// Applies a translation by position, X, Y and Z in 16.16.
void Matrix_TranslateVector(const FIXED *position) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  matrix[MATRIX_TRANSLATION_X] = position[0];
  matrix[MATRIX_TRANSLATION_Y] = position[1];
  matrix[MATRIX_TRANSLATION_Z] = position[2];
  Iwram_MultiplyMatrix(matrix);
}

/// Applies a translation by x, y and z in 16.16.
void Matrix_Translate(FIXED x, FIXED y, FIXED z) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  matrix[MATRIX_TRANSLATION_X] = x;
  matrix[MATRIX_TRANSLATION_Y] = y;
  matrix[MATRIX_TRANSLATION_Z] = z;
  Iwram_MultiplyMatrix(matrix);
}

/// Sets the current matrix's translation to position.
void Matrix_SetTranslation(const FIXED *position) {
  gCurrentMatrix[MATRIX_TRANSLATION_X] = position[0];
  gCurrentMatrix[MATRIX_TRANSLATION_Y] = position[1];
  gCurrentMatrix[MATRIX_TRANSLATION_Z] = position[2];
}

/// Applies a scale by scale's X, Y and Z factors in 16.16.
void Matrix_ScaleVector(const FIXED *scale) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  matrix[0] = scale[0];
  matrix[4] = scale[1];
  matrix[8] = scale[2];
  Iwram_MultiplyMatrix(matrix);
}

/// Applies a scale by x, y and z in 16.16.
void Matrix_Scale(FIXED x, FIXED y, FIXED z) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  matrix[0] = x;
  matrix[4] = y;
  matrix[8] = z;
  Iwram_MultiplyMatrix(matrix);
}

/// Applies the same scale on all three axes.
void Matrix_ScaleUniform(FIXED scale) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store above")
  Matrix_StoreUnit(matrix);
  matrix[0] = scale;
  matrix[4] = scale;
  matrix[8] = scale;
  Iwram_MultiplyMatrix(matrix);
}
/// Rotates gCurrentMatrix by `angles` and translates it by `position` in one
/// matrix. Reached through the far-call table.
void Matrix_RotateTranslate(s32 *angles, FIXED *position) {
  FIXED sinX;
  s32 cosX;
  FIXED sinY;
  s32 cosY;
  FIXED sinZ;
  s32 cosZ;
  MATRIX matrix;

  sinX = Trig_Sin(angles[0]);

  cosX = COS(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = COS(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = COS(angles[2]);
  SET_VECTOR(matrix, Iwram_MulFX(cosY, cosZ), Iwram_MulFX(cosY, sinZ), -sinY);
  matrix[3] =
      Iwram_MulFX(Iwram_MulFX(sinX, sinY), cosZ) - Iwram_MulFX(cosX, sinZ);
  matrix[4] =
      Iwram_MulFX(Iwram_MulFX(sinX, sinY), sinZ) + Iwram_MulFX(cosX, cosZ);
  matrix[5] = Iwram_MulFX(sinX, cosY);
  matrix[6] =
      Iwram_MulFX(Iwram_MulFX(cosX, sinY), cosZ) + Iwram_MulFX(sinX, sinZ);
  matrix[7] =
      Iwram_MulFX(Iwram_MulFX(cosX, sinY), sinZ) - Iwram_MulFX(sinX, cosZ);
  matrix[8] = Iwram_MulFX(cosX, cosY);
  matrix[MATRIX_TRANSLATION_X] = position[0];
  matrix[MATRIX_TRANSLATION_Y] = position[1];
  matrix[MATRIX_TRANSLATION_Z] = position[2];
  Iwram_MultiplyMatrix(matrix);
}

/// Applies rotation, scaling by row and translation together.
/// scale: scale factors for rows 1-3 of the rotation matrix.
void Matrix_RotateTranslateScale(s32 *angles, FIXED *position, FIXED *scale) {
  FIXED sinX, cosX, sinY;
  s32 cosY;
  FIXED sinZ;
  s32 cosZ;
  FIXED axisScale;
  MATRIX matrix;

  sinX = Trig_Sin(angles[0]);
  {
    s32 quarter;

    quarter = DEGtoANG(90);
    cosX = Trig_Sin(angles[0] + quarter);
  }
  sinY = Trig_Sin(angles[1]);
  cosY = COS(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = COS(angles[2]);
  axisScale = scale[0];
  SET_VECTOR(matrix, Iwram_MulFX(axisScale, Iwram_MulFX(cosY, cosZ)),
             Iwram_MulFX(axisScale, Iwram_MulFX(cosY, sinZ)),
             Iwram_MulFX(axisScale, -sinY));
  axisScale = scale[1];
  matrix[3] =
      Iwram_MulFX(axisScale, Iwram_MulFX(Iwram_MulFX(sinX, sinY), cosZ) -
                                 Iwram_MulFX(cosX, sinZ));
  matrix[4] =
      Iwram_MulFX(axisScale, Iwram_MulFX(Iwram_MulFX(sinX, sinY), sinZ) +
                                 Iwram_MulFX(cosX, cosZ));
  matrix[5] = Iwram_MulFX(axisScale, Iwram_MulFX(sinX, cosY));
  axisScale = scale[2];
  matrix[6] =
      Iwram_MulFX(axisScale, Iwram_MulFX(Iwram_MulFX(cosX, sinY), cosZ) +
                                 Iwram_MulFX(sinX, sinZ));
  matrix[7] =
      Iwram_MulFX(axisScale, Iwram_MulFX(Iwram_MulFX(cosX, sinY), sinZ) -
                                 Iwram_MulFX(sinX, cosZ));
  matrix[8] = Iwram_MulFX(axisScale, Iwram_MulFX(cosX, cosY));
  matrix[MATRIX_TRANSLATION_X] = position[0];
  matrix[MATRIX_TRANSLATION_Y] = position[1];
  matrix[MATRIX_TRANSLATION_Z] = position[2];
  Iwram_MultiplyMatrix(matrix);
}

/// Square root of a 16.16 fixed-point value, in 16.16. The square root of
/// the raw value is the result in 8.8, so the low eight bits are always 0.
#define Math_SqrtFX(v) (Iwram_Sqrt(v) << 8)
/// Matrix_BuildLookAt's dot product, a Thumb entry and its ARM code: eight
/// words it copies onto its stack to run.
#define RelocatedInnerProduct Math_InnerProductCode

/// Builds in `out` the camera matrix of an eye at `eye` looking at `target`.
/// The camera's sideways axis is kept level (no Y component).
void Matrix_BuildLookAt(FIXED *eye, FIXED *target, FIXED *out) {
  /* FAKEMATCH(pin): dot fixed to a register. */
  register Dot3Fn dot asm("r10");
  u32 code[8];
  s32 dx, dy, dz;
  s32 ux;
  s32 zero = 0;
  s32 ex;
  s32 ey;
  s32 uz;
  s32 vx, vy, vz;
  s32 scale;
  s32 ez;
  s32 length;
  FIXED levelSquared;
  u32 num = 0x80000000; // Dividend 2^31 for calculating the reciprocal.
  /* FAKEMATCH(pin): divide fixed to a register. */
  register u32 (*divide)(u32 dividend, u32 divisor) asm("r9");
  /* FAKEMATCH(pin): mul fixed to a register. */
  register s32 (*mul)(s32 a, s32 b) asm("r11");

  // Copy the dot-product routine to the stack and call it there.
  {
    dot = (Dot3Fn)code;
    mul = Iwram_MulFX;
    divide = Iwram_UnsignedDivideEntry;
    Dma_Set(RelocatedInnerProduct, code, DMA_COPY_WORDS(8), REG_DMA3);
  }

  // Unit viewing direction d, from target to eye.
  dx = target[0] - eye[0];
  dy = target[1] - eye[1];
  dz = target[2] - eye[2];
  length =
      Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
  scale = divide(num, length) >> 15;
  scale = -scale;
  /* FAKEMATCH(dead): the normalization in a once-only block. */
  do {
    dx = mul(dx, scale);
    dy = mul(dy, scale);
    dz = mul(dz, scale);
    uz = -dx;
  } while (0);

  // Normalize the horizontal axis u = (dz, 0, -dx).
  levelSquared = FIXED_ONE - mul(dy, dy);
  scale = FIXED_ONE;
  if (levelSquared > 0)
    scale = divide(num, Math_SqrtFX(levelSquared)) << 1;
  ux = mul(dz, scale);
  uz = mul(uz, scale);

  // Up axis v is the cross product of d and u.
  vx = mul(dy, uz);
  vy = mul(dz, ux) - mul(dx, uz);
  vz = -mul(dy, ux);
  scale = divide(num, Math_SqrtFX(dot(vx, vx, vy, vy, vz, vz))) << 1;
  vx = mul(vx, scale);
  vy = mul(vy, scale);
  vz = mul(vz, scale);

  // Place the three axes in columns; translation uses dot products with
  // eye.
  ex = eye[0];
  ey = eye[1];
  ez = eye[2];
  out[0] = ux;
  out[3] = zero;
  out[6] = uz;
  out[MATRIX_TRANSLATION_X] = -dot(ex, ux, ez, uz, zero, zero);
  out[1] = vx;
  out[4] = vy;
  out[7] = vz;
  out[MATRIX_TRANSLATION_Y] = -dot(ex, vx, ey, vy, ez, vz);
  out[2] = dx;
  out[5] = dy;
  out[8] = dz;
  out[MATRIX_TRANSLATION_Z] = -dot(ex, dx, ey, dy, ez, dz);
}

/// Makes the camera at `eye` looking at `target` the whole of gCurrentMatrix.
void Matrix_SetLookAt(FIXED *eye, FIXED *target) {
  Matrix_BuildLookAt(eye, target, gCurrentMatrix);
}

/// Multiplies the camera at `eye` looking at `target` into gCurrentMatrix.
void Matrix_LookAt(FIXED *eye, FIXED *target) {
  MATRIX work;

  Matrix_BuildLookAt(eye, target, work);
  Iwram_MultiplyMatrix(work);
}

/// Sets the projection from a vertical field of view (1/65536 turns): the focal
/// length, in 16.16 pixels, is 80 (half the screen height) over the tangent of
/// half the angle.
void Projection_SetFieldOfView(s32 fieldOfView, FIXED nearDepth,
                               FIXED farDepth) {
  s32 halfAngle;
  FIXED sine;
  FIXED focal;

  halfAngle = fieldOfView / 2;
  sine = Trig_Sin(halfAngle);
  // The cosine, as the sine a quarter turn on: 80 * cos / sin.
  focal = Iwram_RatioMulQ14(sine, Trig_Sin(halfAngle + DEGtoANG(90)) *
                                      (DISPLAY_HEIGHT / 2));
  gProjection.near = nearDepth;
  gProjection.focal = focal;
  gProjection.far = farDepth;
}

/// Sets the projection's focal length and depth range directly.
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth) {
  struct Projection *projection = &gProjection;

  projection->focal = focal;
  projection->near = nearDepth;
  projection->far = farDepth;
}

/// Transforms `vector` by gCurrentMatrix into `result`.
static inline void Render_TransformVector(const void *vector, FIXED *result) {
  Iwram_TransformVectorEntry(vector, result);
}

/// Transforms `point` by gCurrentMatrix and projects it onto the screen into
/// `screen`, returning the scale at its depth; a point outside the near and far
/// depths keeps its view coordinates and returns 0.
s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen) {
  struct ProjectedPoint view;
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 x;
  s32 y;

  Render_TransformVector(point, &view.x);
  depth = -view.z;
  projection = &gProjection;
  if (depth >= projection->near && depth <= projection->far) {
    screen->z = FIXEDtoINT(depth);
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = FIXED_ONE * 33 / 25; // 1.32
    }
    x = projection->centerX + Iwram_MulFX(view.x, scale) / FIXED_ONE;
    y = projection->centerY - Iwram_MulFX(view.y, scale) / FIXED_ONE;
    screen->x = x;
    screen->y = y;
    return scale;
  }
  *screen = view;
  return 0;
}

#undef MATRIX_DMA
#undef COS
#undef Math_SqrtFX
#undef RelocatedInnerProduct
