// Draft, differ 133 of 228, skeleton gap 26 (tla-en, 2026-11-01)
// The Broken Seal's Transform_BuildLookAt (games/tbs/src/system/system/
// transform.c) for The Lost Age: the dot-product routine copied to the stack
// is eight words here (Math_DotProductQ16Code), the square roots are Iwram_Sqrt
// at its fixed entry shifted left 8.
//
// What is left is allocation, and nearly every row differs because of it. The
// listing holds the stack routine in r10, the divide in r9 and the multiply in
// r11 from the top of the function, and keeps eye, out, dx, dy, dz, ux and uz
// on the stack (sp+32 down to sp+8, declaration order). This form cannot reach
// that: those pointers are few references over long lives, so global.c ranks
// them below out (13 refs over 167 insns) and dy, which take the high registers
// first (lreg and greg dumps); the eviction path cannot change it, since every
// pseudo in that group conflicts with everything in block 2.
//
// Measured, not kept: these three as register variables fixed to r10, r9 and
// r11 give every spill slot of the listing at once, skeleton 0, differ 2 of 228
// (the pin form, with this body, the pointers set dot, mul, divide; the last
// difference is the load order of mul(dy, uz)'s arguments). Pins were out of
// bounds for this round (S2), so that is a decision for Pascal; ask whether a
// pin may stand here, or find what else gives r9 to r11 hard conflicts for the
// pseudos above without giving them to the three pointers.
//
// Also measured: the Broken Seal's static inline layer around the fixed entries,
// differ 266; plain long_call calls of IwramMulQ16 and IwramUnsignedDivide, 254
// (no CSE of the address); eye, out, dx, dy, dz, ux and uz as volatile locals,
// 335 (their addresses hoist); pointers set dot, mul, divide without pins, 146.
// `w`, a separate local for 1 - dy * dy, and out[1], out[4], out[7] in that
// order both carry over from the pin form and hold here too. The Broken Seal's
// one-pass block around the normalization gains nothing here and came out.
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
  s32 w;
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
  scale = divide(num, t) >> 15;
  scale = -scale;
  dx = mul(dx, scale);
  uz = -dx;
  dy = mul(dy, scale);
  t = mul(dz, scale);
  dz = t;

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
