// Draft, differ 132 of 228, skeleton gap 21 (tla-en, 2026-11-01)
// The Broken Seal's Transform_BuildLookAt (games/tbs/src/system/system/
// transform.c) for The Lost Age: the dot-product routine copied to the stack
// is eight words here (Math_DotProductQ16Code), the square roots are Iwram_Sqrt
// at its fixed entry shifted left 8.
//
// What is left is allocation; the skeleton gap is its reloads, not statements.
// The listing holds the stack routine in r10, the divide in r9 and the multiply
// in r11 from the top of the function, then ey in r9 and ex in r11 once those
// two are dead, and keeps eye, out, dx, dy, dz, ux and uz on the stack (sp+32
// down to sp+8, declaration order). Here out takes r9 and dot goes to the
// stack.
//
// How far the mechanism is understood (lreg and greg dumps, global.c find_reg):
// a local-alloc pseudo in block 2 gives every long pseudo a hard conflict, and
// a set-once call address has its live length doubled (REG_EQUIV). Splitting
// vx, vy, vz into a set-once first value and a set-once normalized value puts
// r9 to r11 in local hands, and the whole stack group of the listing then
// loses, as it should: that is the real lead. With the plain Iwram_MulQ16 and
// Iwram_UnsignedDivide calls CSE makes a temporary for each address, undoubled,
// and the multiply's (14 over 168, ratio 0.083) kicks ux (5 over 62) out of
// r10, so ux lands on the stack too. Both forms then give mul, Iwram_Sqrt or
// divide the high registers but not dot: differ 266, skeleton 27 plain, 275
// with pointer locals, and the wrong one of r9 to r11 each. What is missing is
// why dot, 7 refs over 160 and below out, dy, dz, dx and eye in both priority
// and kick ratio, wins r10: no local can conflict with those and not with dot.
// The next try is whatever raises dot or lowers those five. ex below ey in r11
// and r9 is a second clue, since ex has more references.
//
// Measured, not kept: these three as register variables fixed to r10, r9 and
// r11, the pointers set dot, mul, divide, give every spill slot of the listing,
// differ 11, skeleton 4 on the current compiler. Pins are out of bounds (S2),
// so whether one may stand here is for Pascal.
//
// Also measured, 2026-11-02 grid of 1,536 forms (pointer locals or plain calls,
// axes split per axis, zero as local, late local or literal, dot declared first
// or after ex, the order the pointers are set, `w` or twin's t, `t` for dz): no
// form beats this one's order. uz = -dx after the third product, as the twin
// has it, 145 or 263. The Broken Seal's static inline layer, 266 on the old
// compiler; eye, out, dx, dy, dz, ux and uz as volatile locals, 335 (their
// addresses hoist, and eye's loads in the listing are one reload shared); dot
// as the DMA destination, 133. Setting dot after the copy gains one row. The
// permuter refuses this listing (bounded Thumb only).
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
  Dma_Set(RelocatedQ16DotProduct, code, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          REG_DMA3);
  dot = (Dot3Fn)code;

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
