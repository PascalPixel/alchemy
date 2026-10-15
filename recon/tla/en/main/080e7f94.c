/* Near miss, differ 8 of 39 (2026-10-15; the earlier draft stood at 44 with
 * 35 instructions). The centre is a local set before the calls, as the ROM
 * keeps 63 in r6 (a constant-equivalent pseudo whose set moves down to its
 * uses, which also moves the angle to r8: 44 -> 12), and the multiply is a
 * local pointer, as the ROM keeps it in r10 (12 -> 8). What remains is one
 * load: the ROM loads IwramMulQ16's entry after the cosine call, where this
 * loads it first. Measured: the pointer set after the cosine (43, x reused),
 * the pointer declared after the centre (8), a u16 angle (29). */
#include "types.h"
#include "iwram_call.h"
#include "fixed_math.h"

void BattleFx_PlotCanvasPixel(s32 x, s32 y, s32 color);

// Plots one pixel of the 128x64 canvas on a circle around its bottom middle
// (63, 63): a radius (16.16) and an angle, the cosine leftward and the sine
// upward.
void Func_080e7f94(s32 unused, s32 radius, s32 angle, s32 color) {
  s32 (*mul)(s32, s32) = Iwram_MulQ16;
  s32 center = 63;
  s32 x = mul(Trig_Cos(angle), radius);
  s32 y = mul(Trig_Sin(angle), radius);

  BattleFx_PlotCanvasPixel(center - (x >> 16), center - (y >> 16), color);
}
