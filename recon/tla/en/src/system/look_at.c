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
  /* FAKEMATCH(pin): dot fixed to r10; as a plain local, 175 of 228 instructions
     differ in tla-en. */
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
  /* FAKEMATCH(pin): divide fixed to r9; as a plain local, 141 of 228
     instructions differ in tla-en. */
  register u32 (*divide)(u32 dividend, u32 divisor) asm("r9");
  /* FAKEMATCH(pin): mul fixed to r11; as a plain local, 209 of 228 instructions
     differ in tla-en. */
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
  dx = mul(dx, scale);
  dy = mul(dy, scale);
  dz = mul(dz, scale);
  uz = -dx;
  /* FAKEMATCH(one-pass): an empty one-pass loop after the store; without it, 2
     of 228 instructions differ in tla-en. */
  do {
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
