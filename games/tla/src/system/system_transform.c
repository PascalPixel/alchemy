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

// gTransform contains twelve s32 values in Q16 (1.0 = 0x10000).
// Entries 0-8 form the 3x3 rotation; entries 9-11 hold the translation.
// Iwram_TransformMatrix multiplies the supplied matrix into the current one.

#define TRANSFORM_DMA DMA_COPY_WORDS(12) ///< DMA for one matrix.

/// The Lost Age obtains cosine by looking up sine ninety degrees ahead.
#define COS(a) Trig_Sin((a) + TRIG_TURN / 4)

/// An empty routine.
void Render_EmptyHook(void) {}

/// Allocates the matrix-save storage and sets the current matrix to identity.
void Transform_Reset(void) {
  gTransformStackTop =
      HeapSlot_AllocEwram(HEAP_SLOT_TRANSFORM_STACK, sizeof(gTransform));
  gTransformStackDepth = 0;
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

/// Resets the save pointer to the start of the workspace.
void Transform_RewindStack(void) {
  gTransformStackTop = Ram_HeapSlots->transformStack;
  gTransformStackDepth = 0;
}

/// Saves gTransform into the save area (one level deep; a second save is
/// ignored).
void Transform_Push(void) {
  if (gTransformStackDepth <= 0) {
    Dma_Set(gTransform, gTransformStackTop, TRANSFORM_DMA, REG_DMA3);
    gTransformStackDepth++;
    gTransformStackTop += sizeof(gTransform);
  }
}

/// Copies gTransform to `dst`.
void Transform_Store(void *dst) {
  Dma_Set(gTransform, dst, TRANSFORM_DMA, REG_DMA3);
}

/// Copies the matrix at `src` into gTransform.
void Transform_Load(const void *src) {
  Dma_Set(src, gTransform, TRANSFORM_DMA, REG_DMA3);
}

/// Restores gTransform from the save area, if Transform_Push saved it.
void Transform_Pop(void) {
  if (gTransformStackDepth > 0) {
    --gTransformStackDepth;
    gTransformStackTop -= sizeof(gTransform);
    Dma_Set(gTransformStackTop, gTransform, TRANSFORM_DMA, REG_DMA3);
  }
}

/// Resets gTransform to identity. Reached through the far-call table.
void Transform_ResetMatrix(void) {
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

/// Rotates gTransform by `angles` about the X, Y and Z axes.
void Transform_ApplyRotation(s32 *angles) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 matrix[TRANSFORM_WORDS];

  sinX = Trig_Sin(angles[0]);

  cosX = COS(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = COS(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = COS(angles[2]);
  SET_VECTOR(matrix, Iwram_MulQ16(cosY, cosZ), Iwram_MulQ16(cosY, sinZ), -sinY);
  matrix[3] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), cosZ) - Iwram_MulQ16(cosX, sinZ);
  matrix[4] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), sinZ) + Iwram_MulQ16(cosX, cosZ);
  matrix[5] = Iwram_MulQ16(sinX, cosY);
  matrix[6] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), cosZ) + Iwram_MulQ16(sinX, sinZ);
  matrix[7] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), sinZ) - Iwram_MulQ16(sinX, cosZ);
  matrix[8] = Iwram_MulQ16(cosX, cosY);
  matrix[TRANSFORM_TRANSLATION_X] = 0;
  matrix[TRANSFORM_TRANSLATION_Y] = 0;
  matrix[TRANSFORM_TRANSLATION_Z] = 0;
  Iwram_TransformMatrix(matrix);
}

/// The identity store of transform.h's Transform_SetIdentity in ⚓️'s order,
/// the 1.0 built before the zeros, as ⚓️'s Transform_ResetCurrent builds it.
static __inline__ void Render_SetIdentity(void *matrix) {
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
void Transform_ApplyPitch(s32 angle) {
  s32 matrix[TRANSFORM_WORDS];
  s32 sine, cosine;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  cosine = COS(angle);
  matrix[4] = cosine;
  matrix[8] = cosine;
  sine = Trig_Sin(angle);
  matrix[5] = sine;
  matrix[7] = -sine;
  Iwram_TransformMatrix(matrix);
}

/// Applies a rotation about the Y axis.
void Transform_ApplyYaw(s32 angle) {
  s32 matrix[TRANSFORM_WORDS];
  s32 sine, cosine;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  cosine = COS(angle);
  matrix[0] = cosine;
  matrix[8] = cosine;
  sine = Trig_Sin(angle);
  matrix[2] = -sine;
  matrix[6] = sine;
  Iwram_TransformMatrix(matrix);
}

/// Makes the current matrix a rotation about the Y axis.
void Transform_SetYaw(s32 angle) {
  s32 *matrix = gTransform;
  s32 sine, cosine;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  cosine = COS(angle);
  matrix[0] = cosine;
  matrix[8] = cosine;
  sine = Trig_Sin(angle);
  matrix[2] = -sine;
  matrix[6] = sine;
}

/// Applies a rotation about the Z axis.
void Transform_ApplyRoll(s32 angle) {
  s32 matrix[TRANSFORM_WORDS];
  s32 sine, cosine;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  cosine = COS(angle);
  matrix[0] = cosine;
  matrix[4] = cosine;
  sine = Trig_Sin(angle);
  matrix[1] = sine;
  matrix[3] = -sine;
  Iwram_TransformMatrix(matrix);
}

/// Applies a translation by position, X, Y and Z in Q16.
void Transform_ApplyPosition(const s32 *position) {
  s32 matrix[TRANSFORM_WORDS];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  matrix[TRANSFORM_TRANSLATION_X] = position[0];
  matrix[TRANSFORM_TRANSLATION_Y] = position[1];
  matrix[TRANSFORM_TRANSLATION_Z] = position[2];
  Iwram_TransformMatrix(matrix);
}

/// Applies a translation by x, y and z in Q16.
void Transform_ApplyTranslation(s32 x, s32 y, s32 z) {
  s32 matrix[TRANSFORM_WORDS];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  matrix[TRANSFORM_TRANSLATION_X] = x;
  matrix[TRANSFORM_TRANSLATION_Y] = y;
  matrix[TRANSFORM_TRANSLATION_Z] = z;
  Iwram_TransformMatrix(matrix);
}

/// Sets the current matrix's translation to position.
void Transform_SetPosition(const s32 *position) {
  gTransform[TRANSFORM_TRANSLATION_X] = position[0];
  gTransform[TRANSFORM_TRANSLATION_Y] = position[1];
  gTransform[TRANSFORM_TRANSLATION_Z] = position[2];
}

/// Applies a scale by scale's X, Y and Z factors in Q16.
void Transform_ApplyScale(const s32 *scale) {
  s32 matrix[TRANSFORM_WORDS];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  matrix[0] = scale[0];
  matrix[4] = scale[1];
  matrix[8] = scale[2];
  Iwram_TransformMatrix(matrix);
}

/// Applies a scale by x, y and z in Q16.
void Transform_ApplyAxisScale(s32 x, s32 y, s32 z) {
  s32 matrix[TRANSFORM_WORDS];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  matrix[0] = x;
  matrix[4] = y;
  matrix[8] = z;
  Iwram_TransformMatrix(matrix);
}

/// Applies the same scale on all three axes.
void Transform_ApplyUniformScale(s32 scale) {
  s32 matrix[TRANSFORM_WORDS];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(matrix);
  matrix[0] = scale;
  matrix[4] = scale;
  matrix[8] = scale;
  Iwram_TransformMatrix(matrix);
}
/// Rotates gTransform by `angles` and translates it by `position` in one
/// matrix. Reached through the far-call table.
void Transform_ApplyRotationTranslation(s32 *angles, s32 *position) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 matrix[TRANSFORM_WORDS];

  sinX = Trig_Sin(angles[0]);

  cosX = COS(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = COS(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = COS(angles[2]);
  SET_VECTOR(matrix, Iwram_MulQ16(cosY, cosZ), Iwram_MulQ16(cosY, sinZ), -sinY);
  matrix[3] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), cosZ) - Iwram_MulQ16(cosX, sinZ);
  matrix[4] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), sinZ) + Iwram_MulQ16(cosX, cosZ);
  matrix[5] = Iwram_MulQ16(sinX, cosY);
  matrix[6] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), cosZ) + Iwram_MulQ16(sinX, sinZ);
  matrix[7] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), sinZ) - Iwram_MulQ16(sinX, cosZ);
  matrix[8] = Iwram_MulQ16(cosX, cosY);
  matrix[TRANSFORM_TRANSLATION_X] = position[0];
  matrix[TRANSFORM_TRANSLATION_Y] = position[1];
  matrix[TRANSFORM_TRANSLATION_Z] = position[2];
  Iwram_TransformMatrix(matrix);
}

/// Applies rotation, scaling by row and translation together.
/// scale: scale factors for rows 1-3 of the rotation matrix.
void Transform_ApplyScaledRotation(s32 *angles, s32 *position, s32 *scale) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 axisScale;
  s32 matrix[TRANSFORM_WORDS];

  sinX = Trig_Sin(angles[0]);
  {
    s32 quarter;

    quarter = TRIG_TURN / 4;
    cosX = Trig_Sin(angles[0] + quarter);
  }
  sinY = Trig_Sin(angles[1]);
  cosY = COS(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = COS(angles[2]);
  axisScale = scale[0];
  SET_VECTOR(matrix, Iwram_MulQ16(axisScale, Iwram_MulQ16(cosY, cosZ)),
             Iwram_MulQ16(axisScale, Iwram_MulQ16(cosY, sinZ)),
             Iwram_MulQ16(axisScale, -sinY));
  axisScale = scale[1];
  matrix[3] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), cosZ) -
                                  Iwram_MulQ16(cosX, sinZ));
  matrix[4] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), sinZ) +
                                  Iwram_MulQ16(cosX, cosZ));
  matrix[5] = Iwram_MulQ16(axisScale, Iwram_MulQ16(sinX, cosY));
  axisScale = scale[2];
  matrix[6] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), cosZ) +
                                  Iwram_MulQ16(sinX, sinZ));
  matrix[7] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), sinZ) -
                                  Iwram_MulQ16(sinX, cosZ));
  matrix[8] = Iwram_MulQ16(axisScale, Iwram_MulQ16(cosX, cosY));
  matrix[TRANSFORM_TRANSLATION_X] = position[0];
  matrix[TRANSFORM_TRANSLATION_Y] = position[1];
  matrix[TRANSFORM_TRANSLATION_Z] = position[2];
  Iwram_TransformMatrix(matrix);
}

/// Square root of a 16.16 fixed-point value, in 16.16. The square root of
/// the raw value is the result in 8.8, so the low eight bits are always 0.
#define FixedSqrt(v) (Iwram_Sqrt(v) << 8)
/// Transform_BuildLookAt's ARM dot product, seven words it copies onto its
/// stack to run.
#define RelocatedQ16DotProduct Math_DotProductQ16Code

/// Builds in `out` the camera matrix of an eye at `eye` looking at `target`.
/// The camera's sideways axis is kept level (no Y component).
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out) {
  /* FAKEMATCH(pin): dot held in r10. */
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
  s32 t;
  s32 w;
  u32 num = 0x80000000; // Dividend 2^31 for calculating the reciprocal.
  /* FAKEMATCH(pin): divide held in r9. */
  register u32 (*divide)(u32 dividend, u32 divisor) asm("r9");
  /* FAKEMATCH(pin): mul held in r11. */
  register s32 (*mul)(s32 a, s32 b) asm("r11");

  // Copy the dot-product routine to the stack and call it there.
  {
    u32 *dst = code;
    dot = (Dot3Fn)dst;
    mul = Iwram_MulQ16;
    divide = Iwram_UnsignedDivideEntry;
    Dma_Set(RelocatedQ16DotProduct, dst, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
            REG_DMA3);
  }

  // Unit viewing direction d, from target to eye.
  dx = target[0] - eye[0];
  dy = target[1] - eye[1];
  dz = target[2] - eye[2];
  t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
  scale = divide(num, t) >> 15;
  scale = -scale;
  /* FAKEMATCH(dead): the normalization in a once-only block. */
  do {
    dx = mul(dx, scale);
    dy = mul(dy, scale);
    dz = mul(dz, scale);
    uz = -dx;
  } while (0);

  // Normalize the horizontal axis u = (dz, 0, -dx).
  w = Q16_ONE - mul(dy, dy);
  scale = Q16_ONE;
  if (w > 0)
    scale = divide(num, FixedSqrt(w)) << 1;
  ux = mul(dz, scale);
  uz = mul(uz, scale);

  // Up axis v is the cross product of d and u.
  vx = mul(dy, uz);
  vy = mul(dz, ux) - mul(dx, uz);
  vz = -mul(dy, ux);
  scale = divide(num, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
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
  out[TRANSFORM_TRANSLATION_X] = -dot(ex, ux, ez, uz, zero, zero);
  out[1] = vx;
  out[4] = vy;
  out[7] = vz;
  out[TRANSFORM_TRANSLATION_Y] = -dot(ex, vx, ey, vy, ez, vz);
  out[2] = dx;
  out[5] = dy;
  out[8] = dz;
  out[TRANSFORM_TRANSLATION_Z] = -dot(ex, dx, ey, dy, ez, dz);
}

/// Makes the camera at `eye` looking at `target` the whole of gTransform.
void Transform_SetLookAt(s32 *eye, s32 *target) {
  Transform_BuildLookAt(eye, target, gTransform);
}

/// Multiplies the camera at `eye` looking at `target` into gTransform.
void Transform_ApplyLookAt(s32 *eye, s32 *target) {
  s32 work[TRANSFORM_WORDS];

  Transform_BuildLookAt(eye, target, work);
  Iwram_TransformMatrix(work);
}

/// Sets the projection from a vertical field of view (1/65536 turns): the focal
/// length, in 16.16 pixels, is 80 (half the screen height) over the tangent of
/// half the angle.
void Projection_SetFieldOfView(s32 fieldOfView, s32 nearDepth, s32 farDepth) {
  s32 halfAngle;
  s32 sine;
  s32 focal;

  halfAngle = fieldOfView / 2;
  sine = Trig_Sin(halfAngle);
  // The cosine, as the sine a quarter turn on: 80 * cos / sin.
  focal = Iwram_RatioMulQ14(sine, Trig_Sin(halfAngle + TRIG_TURN / 4) *
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

/// Transforms `vector` by gTransform into `result`.
static inline void Render_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
}

/// Transforms `point` by gTransform and projects it onto the screen into
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
    screen->z = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = Q16_ONE * 33 / 25; // 1.32
    }
    x = projection->centerX + Iwram_MulQ16(view.x, scale) / Q16_ONE;
    y = projection->centerY - Iwram_MulQ16(view.y, scale) / Q16_ONE;
    screen->x = x;
    screen->y = y;
    return scale;
  }
  *screen = view;
  return 0;
}

#undef TRANSFORM_DMA
#undef COS
#undef FixedSqrt
#undef RelocatedQ16DotProduct
