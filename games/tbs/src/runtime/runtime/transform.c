/// @file
/// The 3D transform, its stack, the look-at matrix and the projection.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

// The 3D transform. gTransform is twelve s32 in 16.16 fixed point (1.0 is
// 0x10000): [0] to [8] a 3x3 rotation, [9] to [11] a translation.
// Iwram_TransformMatrix multiplies the matrix it is given into gTransform.

#define TRANSFORM_DMA (DMA_COPY_WORDS(12)) ///< one matrix

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

/// Allocates the one-level save area (heap slot HEAP_SLOT_TRANSFORM_STACK),
/// clears gTransformStackDepth and resets gTransform to identity. Called by
/// the battle scene setup and the field camera before a scene's first
/// transform.
void Render_ResetTransformState(void) {
  gTransformStackTop =
      Runtime_AllocateBlock(HEAP_SLOT_TRANSFORM_STACK, sizeof(gTransform));
  gTransformStackDepth = 0;
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

/// Saves gTransform into the save area (one level deep; a second save is
/// ignored). Called by the battle effects that draw a part with its own
/// transform, with Transform_Pop after.
void Transform_Push(void) {
  if (gTransformStackDepth <= 0) {
    Dma_Set(gTransform, gTransformStackTop, TRANSFORM_DMA, REG_DMA3);
    gTransformStackDepth++;
    gTransformStackTop += sizeof(gTransform);
  }
}

/// Copies gTransform to `dst`. Called by BattleFx_RunAtalanta.
void Transform_Store(void *dst) {
  Dma_Set(gTransform, dst, TRANSFORM_DMA, REG_DMA3);
}

/// Copies the matrix at `src` into gTransform. Called by BattleFx_RunAtalanta.
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
void SceneTransform_ResetMatrix(void) {
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

/// Rotates gTransform by angles[0], angles[1] and angles[2] about the X, Y and
/// Z axes, in that order, with one combined matrix. Reached through the
/// far-call table.
void SceneTransform_ApplyRotation(s32 *angles) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 matrix[12];

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  matrix[0] = Iwram_MulQ16(cosY, cosZ);
  matrix[1] = Iwram_MulQ16(cosY, sinZ);
  matrix[2] = -sinY;
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
  matrix[9] = 0;
  matrix[10] = 0;
  matrix[11] = 0;
  Iwram_TransformMatrix(matrix);
}

/// Rotates gTransform about the X axis by `angle`. Called by the battle and
/// field cameras.
void SceneTransform_ApplyPitch(s32 angle) {
  s32 matrix[12];
  s32 sine = Trig_Sin(angle);
  s32 cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[4] = cosine;
  matrix[5] = sine;
  matrix[7] = -sine;
  matrix[8] = cosine;
  Iwram_TransformMatrix(matrix);
}

/// Rotates gTransform about the Y axis by `angle`. Called by the battle and
/// field cameras.
void SceneTransform_ApplyYaw(s32 angle) {
  s32 matrix[12];
  s32 sine = Trig_Sin(angle);
  s32 cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[0] = cosine;
  matrix[2] = -sine;
  matrix[6] = sine;
  matrix[8] = cosine;
  Iwram_TransformMatrix(matrix);
}

/// Rotates gTransform about the Z axis by `angle`. Called by battle effects
/// that spin their picture.
void SceneTransform_ApplyRoll(s32 angle) {
  s32 matrix[12];
  s32 sine = Trig_Sin(angle);
  s32 cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[0] = cosine;
  matrix[1] = sine;
  matrix[3] = -sine;
  matrix[4] = cosine;
  Iwram_TransformMatrix(matrix);
}

/// Translates gTransform by `position` (x, y, z in 16.16). Called by the
/// battle and field cameras and effects.
void SceneTransform_ApplyPosition(const s32 *position) {
  s32 matrix[12];

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[9] = position[0];
  matrix[10] = position[1];
  matrix[11] = position[2];
  Iwram_TransformMatrix(matrix);
}

/// Scales gTransform by `scale` (x, y, z factors in 16.16). Called by a few
/// battle effects (Bind, Astral Blast, Thor).
void SceneTransform_ApplyScale(const s32 *scale) {
  s32 matrix[12];

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[0] = scale[0];
  matrix[4] = scale[1];
  matrix[8] = scale[2];
  Iwram_TransformMatrix(matrix);
}

/// Rotates gTransform by `angles` and translates it by `position` in one
/// matrix. Reached through the far-call table.
void SceneTransform_ApplyRotationTranslation(s32 *angles, s32 *position) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 matrix[12];

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  matrix[0] = Iwram_MulQ16(cosY, cosZ);
  matrix[1] = Iwram_MulQ16(cosY, sinZ);
  matrix[2] = -sinY;
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
  matrix[9] = position[0];
  matrix[10] = position[1];
  matrix[11] = position[2];
  Iwram_TransformMatrix(matrix);
}

/// Rotates by `angles`, scales each rotation row by `scale` and translates by
/// `position`, in one matrix.
void SceneTransform_ApplyScaledRotation(s32 *angles, s32 *position,
                                        s32 *scale) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 axisScale;
  s32 matrix[12];

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  axisScale = scale[0];
  matrix[0] = Iwram_MulQ16(axisScale, Iwram_MulQ16(cosY, cosZ));
  matrix[1] = Iwram_MulQ16(axisScale, Iwram_MulQ16(cosY, sinZ));
  matrix[2] = Iwram_MulQ16(axisScale, -sinY);
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
  matrix[9] = position[0];
  matrix[10] = position[1];
  matrix[11] = position[2];
  Iwram_TransformMatrix(matrix);
}

/// Builds in `out` the camera matrix of an eye at `eye` looking at `target`.
/// The camera's sideways axis is kept level (no Y component). Called by
/// Transform_SetLookAt and Transform_ApplyLookAt.
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out) {
  u32 code[7];
  s32 dx, dy, dz;
  s32 ux;
  s32 zero = 0;
  s32 ex;
  Dot3Fn dot;
  s32 ey;
  s32 uz;
  s32 vx, vy, vz;
  s32 scale;
  s32 ez;
  s32 t;
  u32 num = 0x80000000; // 2^31, the dividend of each reciprocal

  // Copy the dot product routine to the stack and call it there
  dot = (Dot3Fn)code;
  Dma_Set(RelocatedQ16DotProduct, code, DMA_COPY_WORDS(7), REG_DMA3);

  // The view axis d, pointing from the target back to the eye, normalized
  dx = target[0] - eye[0];
  dy = target[1] - eye[1];
  dz = target[2] - eye[2];
  t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
  scale = Iwram_UnsignedDivide(num, t) >> 15;
  scale = -scale;
  /* FAKEMATCH: the normalization keeps its CSE block, so the third product
   * spills after the negated axis; without it, 4 instructions differ in
   * tbs-en. */
  do {
    dx = Iwram_MulQ16(dx, scale);
    dy = Iwram_MulQ16(dy, scale);
    dz = Iwram_MulQ16(dz, scale);
    uz = -dx;
  } while (0);

  // The sideways axis u = (dz, 0, -dx), normalized
  t = Iwram_MulQ16(dy, dy);
  scale = 0x10000;
  if (scale - t > 0)
    scale = Iwram_UnsignedDivide(num, FixedSqrt(scale - t)) << 1;
  ux = Iwram_MulQ16(dz, scale);
  uz = Iwram_MulQ16(uz, scale);

  // The up axis v, the cross product of d and u, normalized
  vx = Iwram_MulQ16(dy, uz);
  vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
  vz = -Iwram_MulQ16(dy, ux);
  scale = Iwram_UnsignedDivide(num, FixedSqrt(dot(vx, vx, vy, vy, vz, vz)))
          << 1;
  vx = Iwram_MulQ16(vx, scale);
  vy = Iwram_MulQ16(vy, scale);
  vz = Iwram_MulQ16(vz, scale);

  // The three axes are the matrix columns; the translation is each axis's
  // dot product with the eye, negated
  ex = eye[0];
  ey = eye[1];
  ez = eye[2];
  out[0] = ux;
  out[3] = zero;
  out[6] = uz;
  out[9] = -dot(ex, ux, ez, uz, zero, zero);
  out[4] = vy;
  out[7] = vz;
  out[1] = vx;
  out[10] = -dot(ex, vx, ey, vy, ez, vz);
  out[2] = dx;
  out[5] = dy;
  out[8] = dz;
  out[11] = -dot(ex, dx, ey, dy, ez, dz);
}

/// Makes the camera at `eye` looking at `target` the whole of gTransform.
/// Called by the battle cameras.
void Transform_SetLookAt(s32 *eye, s32 *target) {
  Transform_BuildLookAt(eye, target, gTransform);
}

/// Multiplies the camera at `eye` looking at `target` into gTransform. Called
/// by the battle and field object cameras.
void Transform_ApplyLookAt(s32 *eye, s32 *target) {
  s32 work[12];

  Transform_BuildLookAt(eye, target, work);
  Iwram_TransformMatrix(work);
}

/// Sets gProjection from a vertical field of view: the focal length is 80
/// (half the screen height) over the tangent of half the angle. Points nearer
/// than `nearDepth` or farther than `farDepth` are not drawn. Reached through
/// the far-call table.
void Projection_SetFieldOfView(u32 fieldOfView, s32 nearDepth, s32 farDepth) {
  s32 halfAngle;
  s32 sine;
  s32 focal;

  halfAngle = (s32)(fieldOfView + (fieldOfView >> 31)) >> 1;
  sine = Trig_Sin(halfAngle);
  focal = Iwram_RatioMulQ14(sine, Trig_Cos(halfAngle) * 80);
  gProjection.near = nearDepth;
  gProjection.focal = focal;
  gProjection.far = farDepth;
}

/// Sets gProjection's focal length and depth range directly. Called by the
/// battle cameras and effects.
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth) {
  struct Projection *projection = &gProjection;

  projection->focal = focal;
  projection->near = nearDepth;
  projection->far = farDepth;
}

/// Reads three word-aligned coordinates from `point`, transforms them by
/// gTransform and projects them to the screen: screen[0] and screen[1] are
/// its pixel position and screen[2] its whole-unit depth. Returns the
/// projection scale (16.16), or 0 when the point is outside the depth range,
/// when only the transformed point is written. Called by the battle and field
/// code that places sprites in 3D.
s32 Render_ProjectPoint(const void *point, s32 *screen) {
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 result;

  Iwram_TransformVector(point, screen);
  projection = &gProjection;
  depth = -screen[2];
  result = 0;
  if (depth >= projection->near && depth <= projection->far) {
    screen[2] = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = 0x151eb; // about 1.32
    }
    screen[0] = projection->centerX + Iwram_MulQ16(screen[0], scale) / 0x10000;
    screen[1] = projection->centerY - Iwram_MulQ16(screen[1], scale) / 0x10000;
    result = scale;
  }
  return result;
}

#undef TRANSFORM_DMA
