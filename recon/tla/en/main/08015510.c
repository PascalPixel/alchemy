// Draft, differ 143 (199 before statements reordered 2026-10-26) of 228: The Broken Seal's Transform_BuildLookAt
// (games/tbs/src/system/system/transform.c) for The Lost Age: the
// dot-product routine copied to the stack is eight words here
// (Math_DotProductQ16Code, frame 68), the square roots are Iwram_Sqrt at its
// fixed entry shifted left 8, and the native holds three routines in
// registers for the whole function, the divide in r9, the
// stack routine in r10 and the multiply in r11, loading only the square root
// entry from the pool at each use; it keeps eye, out, the view vector and the
// side axis on the stack. This form keeps out in r9 and spills the dot pointer,
// so nearly every row after the prologue differs in registers. It has The
// Broken Seal's zero and dividend locals, its one-pass block around the
// normalization, and local pointers for the divide and the multiply set before
// the copy. Measured beside it: the same pointers initialized at their
// declarations with plain statements, with and without those locals and that
// block, 202 to 208 (the form this draft replaces was 205). Not yet tried: the
// locals' declaration order and volatile.
#include "types.h"
#include "transform.h"
#include "dma.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "graphics.h"

#define FixedSqrt(v) (Iwram_Sqrt(v) << 8)
#define RelocatedQ16DotProduct Math_DotProductQ16Code
extern const u32 Math_DotProductQ16Code[];

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out) {
  Dot3Fn dot;
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
  u32 num = 0x80000000; // Dividend 2^31 for calculating the reciprocal.
  u32 (*divide)(u32 dividend, u32 divisor);
  s32 (*mul)(s32 a, s32 b);

  // Copy the dot-product routine to the stack and call it there.
  divide = Iwram_UnsignedDivideEntry;
  mul = Iwram_MulQ16;
  dot = (Dot3Fn)code;
  Dma_Set(RelocatedQ16DotProduct, code, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          REG_DMA3);

  // Unit viewing direction d, from target to eye.
  dx = target[0] - eye[0];
  dy = target[1] - eye[1];
  dz = target[2] - eye[2];
  t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
  /* FAKEMATCH: the routine is called through a local pointer to its fixed
   * entry; a plain call matches The Broken Seal through its IWRAM call layer
   * but not The Lost Age, whose header calls the entry directly (all six of its
   * editions differ). */
  scale = divide(num, t) >> 15;
  scale = -scale;
  /* FAKEMATCH: the normalization keeps its CSE block; without it, the third
   * product spills before the negated axis (four reordered instructions). */
  do {
    dx = mul(dx, scale);
    uz = -dx;
    dy = mul(dy, scale);
    t = mul(dz, scale);
    dz = t;
  } while (0);

  // Normalize the horizontal axis u = (dz, 0, -dx).
  t = mul(dy, dy);
  scale = 0x10000;
  if (scale - t > 0)
    scale = divide(num, FixedSqrt(scale - t)) << 1;
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
  {
    out[0] = ux;
    out[3] = zero;
    out[6] = uz;
    out[9] = -dot(ex, ux, ez, uz, zero, zero);
  }
  out[4] = vy;
  out[7] = vz;
  out[1] = vx;
  out[10] = -dot(ex, vx, ey, vy, ez, vz);
  out[2] = dx;
  out[5] = dy;
  out[8] = dz;
  out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
