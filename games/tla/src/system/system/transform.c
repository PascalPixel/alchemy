/// @file
/// The 3D transform, its stack, the look-at matrix and the projection.
// Part of system.c.

#include "../system_internal.h"
#include "projection.h"

struct ProjectedPoint {
  s32 x;
  s32 y;
  s32 z;
};

// gTransform contains twelve s32 values in Q16 (1.0 = 0x10000).
// Entries 0-8 form the 3x3 rotation; entries 9-11 hold the translation.
// Iwram_TransformMatrix multiplies the supplied matrix into the current one.

#define MTX_DMA DMA_COPY_WORDS(12) ///< DMA for one matrix.

/// The Lost Age obtains cosine by looking up sine ninety degrees ahead.
#define COS(a) Trig_Sin((a) + 0x4000)

void Render_EmptyHook(void) {}

/// Allocates the matrix-save storage and sets the current matrix to identity.
void Transform_Reset(void) {
  gTransformStackTop = HeapSlot_AllocEwram(8, sizeof(gTransform));
  gTransformStackDepth = 0;
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

/// Resets the save pointer to the start of the workspace.
void SceneTransform_RewindStack(void) {
  gTransformStackTop = Ram_HeapSlots->transformStack;
  gTransformStackDepth = 0;
}

/// Saves the current matrix (one level deep).
void Transform_Push(void) {
  if (gTransformStackDepth <= 0) {
    Dma_Set(gTransform, gTransformStackTop, MTX_DMA, REG_DMA3);
    gTransformStackDepth++;
    gTransformStackTop += sizeof(gTransform);
  }
}

void Transform_Store(void *dst) { Dma_Set(gTransform, dst, MTX_DMA, REG_DMA3); }

void Transform_Load(const void *src) {
  Dma_Set(src, gTransform, MTX_DMA, REG_DMA3);
}

void Transform_Pop(void) {
  if (gTransformStackDepth > 0) {
    --gTransformStackDepth;
    gTransformStackTop -= sizeof(gTransform);
    Dma_Set(gTransformStackTop, gTransform, MTX_DMA, REG_DMA3);
  }
}

/// Sets the current matrix to identity.
void Transform_ResetMatrix(void) {
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

/// Applies rotations around the X, Y and Z axes.
/// ang: the three axis angles.
void Transform_ApplyRotation(s32 *ang) {
  s32 sx, cx, sy, cy, sz, cz;
  s32 m[12];

  sx = Trig_Sin(ang[0]);

  cx = COS(ang[0]);
  sy = Trig_Sin(ang[1]);
  cy = COS(ang[1]);
  sz = Trig_Sin(ang[2]);
  cz = COS(ang[2]);
  SET_VECTOR(m, Iwram_MulQ16(cy, cz), Iwram_MulQ16(cy, sz), -sy);
  m[3] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz);
  m[4] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz);
  m[5] = Iwram_MulQ16(sx, cy);
  m[6] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz);
  m[7] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz);
  m[8] = Iwram_MulQ16(cx, cy);
  m[9] = 0;
  m[10] = 0;
  m[11] = 0;
  Iwram_TransformMatrix(m);
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
void Transform_ApplyPitch(s32 ang) {
  s32 m[12];
  s32 sin, cos;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  cos = COS(ang);
  m[4] = cos;
  m[8] = cos;
  sin = Trig_Sin(ang);
  m[5] = sin;
  m[7] = -sin;
  Iwram_TransformMatrix(m);
}

/// Applies a rotation about the Y axis.
void Transform_ApplyYaw(s32 ang) {
  s32 m[12];
  s32 sin, cos;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  cos = COS(ang);
  m[0] = cos;
  m[8] = cos;
  sin = Trig_Sin(ang);
  m[2] = -sin;
  m[6] = sin;
  Iwram_TransformMatrix(m);
}

/// Makes the current matrix a rotation about the Y axis.
void Transform_SetYaw(s32 ang) {
  s32 *m = gTransform;
  s32 sin, cos;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  cos = COS(ang);
  m[0] = cos;
  m[8] = cos;
  sin = Trig_Sin(ang);
  m[2] = -sin;
  m[6] = sin;
}

/// Applies a rotation about the Z axis.
void Transform_ApplyRoll(s32 ang) {
  s32 m[12];
  s32 sin, cos;

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  cos = COS(ang);
  m[0] = cos;
  m[4] = cos;
  sin = Trig_Sin(ang);
  m[1] = sin;
  m[3] = -sin;
  Iwram_TransformMatrix(m);
}

/// Applies a translation by pos, X, Y and Z in Q16.
void SceneTransform_ApplyPosition(const s32 *pos) {
  s32 m[12];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  m[9] = pos[0];
  m[10] = pos[1];
  m[11] = pos[2];
  Iwram_TransformMatrix(m);
}

/// Applies a translation by x, y and z in Q16.
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z) {
  s32 m[12];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  m[9] = x;
  m[10] = y;
  m[11] = z;
  Iwram_TransformMatrix(m);
}

/// Sets the current matrix's translation to pos.
void SceneTransform_SetPosition(const s32 *pos) {
  gTransform[9] = pos[0];
  gTransform[10] = pos[1];
  gTransform[11] = pos[2];
}

/// Applies a scale by scale's X, Y and Z factors in Q16.
void SceneTransform_ApplyScale(const s32 *scale) {
  s32 m[12];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  m[0] = scale[0];
  m[4] = scale[1];
  m[8] = scale[2];
  Iwram_TransformMatrix(m);
}

/// Applies a scale by x, y and z in Q16.
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z) {
  s32 m[12];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  m[0] = x;
  m[4] = y;
  m[8] = z;
  Iwram_TransformMatrix(m);
}

/// Applies the same scale on all three axes.
void SceneTransform_ApplyUniformScale(s32 scale) {
  s32 m[12];

  CAMELOT_ASM("the fixed-register identity store above")
  Render_SetIdentity(m);
  m[0] = scale;
  m[4] = scale;
  m[8] = scale;
  Iwram_TransformMatrix(m);
}
void Transform_ApplyRotationTranslation(s32 *ang, s32 *pos) {
  s32 sx, cx, sy, cy, sz, cz;
  s32 m[12];

  sx = Trig_Sin(ang[0]);

  cx = COS(ang[0]);
  sy = Trig_Sin(ang[1]);
  cy = COS(ang[1]);
  sz = Trig_Sin(ang[2]);
  cz = COS(ang[2]);
  SET_VECTOR(m, Iwram_MulQ16(cy, cz), Iwram_MulQ16(cy, sz), -sy);
  m[3] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz);
  m[4] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz);
  m[5] = Iwram_MulQ16(sx, cy);
  m[6] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz);
  m[7] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz);
  m[8] = Iwram_MulQ16(cx, cy);
  m[9] = pos[0];
  m[10] = pos[1];
  m[11] = pos[2];
  Iwram_TransformMatrix(m);
}

/// Applies rotation, scaling by row and translation together.
/// scale: scale factors for rows 1-3 of the rotation matrix.
void Transform_ApplyScaledRotation(s32 *ang, s32 *pos, s32 *scale) {
  s32 sx, cx, sy, cy, sz, cz;
  s32 s;
  s32 m[12];

  sx = Trig_Sin(ang[0]);
  {
    s32 quarter;

    quarter = 0x4000;
    cx = Trig_Sin(ang[0] + quarter);
  }
  sy = Trig_Sin(ang[1]);
  cy = COS(ang[1]);
  sz = Trig_Sin(ang[2]);
  cz = COS(ang[2]);
  s = scale[0];
  SET_VECTOR(m, Iwram_MulQ16(s, Iwram_MulQ16(cy, cz)),
             Iwram_MulQ16(s, Iwram_MulQ16(cy, sz)), Iwram_MulQ16(s, -sy));
  s = scale[1];
  m[3] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) -
                             Iwram_MulQ16(cx, sz));
  m[4] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) +
                             Iwram_MulQ16(cx, cz));
  m[5] = Iwram_MulQ16(s, Iwram_MulQ16(sx, cy));
  s = scale[2];
  m[6] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) +
                             Iwram_MulQ16(sx, sz));
  m[7] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) -
                             Iwram_MulQ16(sx, cz));
  m[8] = Iwram_MulQ16(s, Iwram_MulQ16(cx, cy));
  m[9] = pos[0];
  m[10] = pos[1];
  m[11] = pos[2];
  Iwram_TransformMatrix(m);
}

#define FixedSqrt(v) (Iwram_Sqrt(v) << 8)
#define RelocatedQ16DotProduct Math_DotProductQ16Code

void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out) {
  /* FAKEMATCH(pin): dot held in r10; as a plain local, 175 of 228
   * instructions differ in tla-en. */
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
  /* FAKEMATCH(pin): divide held in r9; as a plain local, 141 of 228
   * instructions differ in tla-en. */
  register u32 (*divide)(u32 dividend, u32 divisor) asm("r9");
  /* FAKEMATCH(pin): mul held in r11; as a plain local, 209 of 228
   * instructions differ in tla-en. */
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
  // The normalization in a once-only block, so the axis is negated before the
  // third product spills; as plain statements, 2 of 228 instructions differ in
  // tla-en.
  do {
    dx = mul(dx, scale);
    dy = mul(dy, scale);
    dz = mul(dz, scale);
    uz = -dx;
  } while (0);

  // Normalize the horizontal axis u = (dz, 0, -dx).
  w = 0x10000 - mul(dy, dy);
  scale = 0x10000;
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
  out[9] = -dot(ex, ux, ez, uz, zero, zero);
  out[1] = vx;
  out[4] = vy;
  out[7] = vz;
  out[10] = -dot(ex, vx, ey, vy, ez, vz);
  out[2] = dx;
  out[5] = dy;
  out[8] = dz;
  out[11] = -dot(ex, dx, ey, dy, ez, dz);
}

void Transform_SetLookAt(s32 *eye, s32 *target) {
  Transform_BuildLookAt(eye, target, gTransform);
}

void Transform_ApplyLookAt(s32 *eye, s32 *target) {
  s32 work[12];

  Transform_BuildLookAt(eye, target, work);
  Iwram_TransformMatrix(work);
}

void Projection_SetFieldOfView(s32 value, s32 param1, s32 param2) {
  s32 half;
  s32 first;
  s32 result;

  half = value / 2;
  first = Trig_Sin(half);
  result = Iwram_RatioMulQ14(first, Trig_Sin(half + 0x4000) * 0x50);
  gCameraSceneParameters[1] = param1;
  gCameraSceneParameters[0] = result;
  gCameraSceneParameters[2] = param2;
}

void Projection_Set(u32 value0, u32 value1, u32 value2) {
  u32 *work = gCameraSceneParameters;
  work[0] = value0;
  work[1] = value1;
  work[2] = value2;
}

// The Broken Seal's typed Iwram_TransformVector inline from iwram_call.h, the
// layer every call goes through there; as a direct call to the fixed entry, 4
// of 82 instructions of Render_ProjectPoint differ in tla-en.
static inline void Render_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
}

s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen) {
  struct ProjectedPoint view;
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 x;
  s32 y;

  Render_TransformVector(point, &view.x);
  depth = -view.z;
  projection = (struct Projection *)gCameraSceneParameters;
  if (depth >= projection->near && depth <= projection->far) {
    screen->z = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = 0x151eb;
    }
    x = projection->centerX + Iwram_MulQ16(view.x, scale) / 0x10000;
    y = projection->centerY - Iwram_MulQ16(view.y, scale) / 0x10000;
    screen->x = x;
    screen->y = y;
    return scale;
  }
  *screen = view;
  return 0;
}

#undef MTX_DMA
#undef COS
#undef FixedSqrt
#undef RelocatedQ16DotProduct
