/// @file
/// The 3D transform, its stack, the look-at matrix and the projection.
// Part of system.c.

#include "system_internal.h"

// The 3D transform. gCurrentMatrix is MATRIX_WORDS s32 in 16.16 fixed point
// (1.0 is FIXED_ONE): [0] to [8] a 3x3 rotation, then the translation.

#define MATRIX_DMA (DMA_COPY_WORDS(MATRIX_WORDS)) ///< one matrix

/// Matrix_BuildLookAt's reciprocals divide FIXED_ONE << RECIPROCAL_SHIFT,
/// 2^31, the most a u32 holds: the quotient shifted right by RECIPROCAL_SHIFT
/// is FIXED_ONE over a whole-number divisor, and doubled it is the 16.16
/// reciprocal of a 16.16 divisor.
#define RECIPROCAL_SHIFT 15

/// Matrix_ProjectPoint's depth drops its low PROJECTION_DEPTH_SHIFT bits,
/// and the focal length gains the rest of FIXED_ONE's, so that the focal
/// length over the depth comes out in 16.16 within 32 bits.
#define PROJECTION_DEPTH_SHIFT 11

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

/// Allocates the one-level save area (heap slot HEAP_SLOT_MATRIX_STACK),
/// clears gMatrixStackDepth and resets gCurrentMatrix to the unit matrix.
void Matrix_Initialize(void) {
  gMatrixStackTop =
      HeapSlot_AllocEwram(HEAP_SLOT_MATRIX_STACK, sizeof(gCurrentMatrix));
  gMatrixStackDepth = 0;
  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetCurrentUnit();
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

/// Rotates gCurrentMatrix by angles[0], angles[1] and angles[2] about the X, Y
/// and Z axes, in that order, with one combined matrix.
void Matrix_Rotate(s32 *angles) {
  FIXED sinX, cosX, sinY, cosY, sinZ, cosZ;
  MATRIX matrix;

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  matrix[0] = Iwram_MulFX(cosY, cosZ);
  matrix[1] = Iwram_MulFX(cosY, sinZ);
  matrix[2] = -sinY;
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

/// Rotates gCurrentMatrix about the X axis by `angle`.
void Matrix_RotateX(s32 angle) {
  MATRIX matrix;
  FIXED sine = Trig_Sin(angle);
  FIXED cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(matrix);
  matrix[4] = cosine;
  matrix[5] = sine;
  matrix[7] = -sine;
  matrix[8] = cosine;
  Iwram_MultiplyMatrix(matrix);
}

/// Rotates gCurrentMatrix about the Y axis by `angle`.
void Matrix_RotateY(s32 angle) {
  MATRIX matrix;
  FIXED sine = Trig_Sin(angle);
  FIXED cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(matrix);
  matrix[0] = cosine;
  matrix[2] = -sine;
  matrix[6] = sine;
  matrix[8] = cosine;
  Iwram_MultiplyMatrix(matrix);
}

/// Rotates gCurrentMatrix about the Z axis by `angle`.
void Matrix_RotateZ(s32 angle) {
  MATRIX matrix;
  FIXED sine = Trig_Sin(angle);
  FIXED cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(matrix);
  matrix[0] = cosine;
  matrix[1] = sine;
  matrix[3] = -sine;
  matrix[4] = cosine;
  Iwram_MultiplyMatrix(matrix);
}

/// Translates gCurrentMatrix by `position` (x, y, z in 16.16).
void Matrix_TranslateVector(const FIXED *position) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(matrix);
  matrix[MATRIX_TRANSLATION_X] = position[0];
  matrix[MATRIX_TRANSLATION_Y] = position[1];
  matrix[MATRIX_TRANSLATION_Z] = position[2];
  Iwram_MultiplyMatrix(matrix);
}

/// Scales gCurrentMatrix by `scale` (x, y, z factors in 16.16).
void Matrix_ScaleVector(const FIXED *scale) {
  MATRIX matrix;

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(matrix);
  matrix[0] = scale[0];
  matrix[4] = scale[1];
  matrix[8] = scale[2];
  Iwram_MultiplyMatrix(matrix);
}

/// Rotates gCurrentMatrix by `angles` and translates it by `position` in one
/// matrix. Reached through the far-call table.
void Matrix_RotateTranslate(s32 *angles, FIXED *position) {
  FIXED sinX, cosX, sinY, cosY, sinZ, cosZ;
  MATRIX matrix;

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  matrix[0] = Iwram_MulFX(cosY, cosZ);
  matrix[1] = Iwram_MulFX(cosY, sinZ);
  matrix[2] = -sinY;
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

/// Rotates by `angles`, scales each rotation row by `scale` and translates by
/// `position`, in one matrix.
void Matrix_RotateTranslateScale(s32 *angles, FIXED *position, FIXED *scale) {
  FIXED sinX, cosX, sinY, cosY, sinZ, cosZ;
  FIXED axisScale;
  MATRIX matrix;

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  axisScale = scale[0];
  matrix[0] = Iwram_MulFX(axisScale, Iwram_MulFX(cosY, cosZ));
  matrix[1] = Iwram_MulFX(axisScale, Iwram_MulFX(cosY, sinZ));
  matrix[2] = Iwram_MulFX(axisScale, -sinY);
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

/// Builds in `out` the camera matrix of an eye at `eye` looking at `target`.
/// The camera's sideways axis is kept level (no Y component).
void Matrix_BuildLookAt(FIXED *eye, FIXED *target, FIXED *out) {
  u32 code[7];
  s32 dx, dy, dz;
  FIXED ux;
  s32 uy = 0; // the sideways axis is kept level
  s32 ex;
  Dot3Fn dot;
  s32 ey;
  FIXED uz;
  FIXED vx, vy, vz;
  s32 scale;
  s32 ez;
  s32 t;
  u32 dividend = (u32)FIXED_ONE << RECIPROCAL_SHIFT;

  // Copy the dot product routine to the stack and call it there.
  dot = (Dot3Fn)code;
  Dma_Set(RelocatedInnerProduct, code, DMA_COPY_WORDS(7), REG_DMA3);

  // The view axis d, pointing from the target back to the eye, normalized:
  // its length in whole pixels, and FIXED_ONE over it, negated.
  dx = target[0] - eye[0];
  dy = target[1] - eye[1];
  dz = target[2] - eye[2];
  t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
  scale = Iwram_UnsignedDivide(dividend, t) >> RECIPROCAL_SHIFT;
  scale = -scale;
  dx = Iwram_MulFX(dx, scale);
  dy = Iwram_MulFX(dy, scale);
  dz = Iwram_MulFX(dz, scale);
  uz = -dx;

  // The sideways axis u = (dz, 0, -dx), normalized, unless d is vertical.
  do {
    t = Iwram_MulFX(dy, dy);
    scale = FIXED_ONE;
    if (scale - t <= 0)
      break;
    scale = Iwram_UnsignedDivide(dividend, Math_SqrtFX(scale - t)) << 1;
  } while (0);
  ux = Iwram_MulFX(dz, scale);
  uz = Iwram_MulFX(uz, scale);

  // The up axis v, the cross product of d and u, normalized.
  vx = Iwram_MulFX(dy, uz);
  vy = Iwram_MulFX(dz, ux) - Iwram_MulFX(dx, uz);
  vz = -Iwram_MulFX(dy, ux);
  scale =
      Iwram_UnsignedDivide(dividend, Math_SqrtFX(dot(vx, vx, vy, vy, vz, vz)))
      << 1;
  vx = Iwram_MulFX(vx, scale);
  vy = Iwram_MulFX(vy, scale);
  vz = Iwram_MulFX(vz, scale);

  // The three axes are the matrix columns; the translation is each axis's
  // dot product with the eye, negated.
  ex = eye[0];
  ey = eye[1];
  ez = eye[2];
  out[0] = ux;
  out[3] = uy;
  out[6] = uz;
  out[MATRIX_TRANSLATION_X] = -dot(ex, ux, ez, uz, uy, uy);
  out[4] = vy;
  out[7] = vz;
  out[1] = vx;
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

/// Sets gProjection from a vertical field of view (1/65536 turns): the focal
/// length, in 16.16 pixels, is 80 (half the screen height) over the tangent of
/// half the angle.
void Projection_SetFieldOfView(u32 fieldOfView, FIXED nearDepth,
                               FIXED farDepth) {
  s32 halfAngle;
  FIXED sine;
  FIXED focal;

  halfAngle = (s32)fieldOfView / 2;
  sine = Trig_Sin(halfAngle);
  // Iwram_RatioMulQ14(d, v) is v * FIXED_ONE / d: here
  // (DISPLAY_HEIGHT / 2) * cos / sin
  focal = Iwram_RatioMulQ14(sine, Trig_Cos(halfAngle) * (DISPLAY_HEIGHT / 2));
  gProjection.near = nearDepth;
  gProjection.focal = focal;
  gProjection.far = farDepth;
}

/// Sets gProjection's focal length and depth range directly.
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth) {
  struct Projection *projection = &gProjection;

  projection->focal = focal;
  projection->near = nearDepth;
  projection->far = farDepth;
}

/// Reads three word-aligned coordinates from `point`, transforms them by
/// gCurrentMatrix and projects them to the screen: screen[0] and screen[1] are
/// its pixel position and screen[2] its whole-unit depth. Returns the scale the
/// point is drawn at, in 16.16, or 0 when it lies outside the projection's
/// depth range and the screen position is not written.
s32 Matrix_ProjectPoint(const void *point, s32 *screen) {
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 result;

  Iwram_TransformVector(point, screen);
  projection = &gProjection;
  // The camera looks down -z, so a point in front of it has a negative z.
  depth = -screen[2];
  result = 0;
  if (depth >= projection->near && depth <= projection->far) {
    screen[2] = FIXEDtoINT(depth);
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> PROJECTION_DEPTH_SHIFT;

      scale = Iwram_UnsignedDivide(
          projection->focal * (FIXED_ONE >> PROJECTION_DEPTH_SHIFT), shifted);
    } else {
      scale = FIXED_ONE * 33 / 25; // 1.32
    }
    screen[0] = projection->centerX + Iwram_MulFX(screen[0], scale) / FIXED_ONE;
    screen[1] = projection->centerY - Iwram_MulFX(screen[1], scale) / FIXED_ONE;
    result = scale;
  }
  return result;
}

#undef MATRIX_DMA
#undef RECIPROCAL_SHIFT
#undef PROJECTION_DEPTH_SHIFT
