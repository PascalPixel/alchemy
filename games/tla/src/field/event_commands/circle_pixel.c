#include "types.h"
#include "iwram_call.h"
#include "fixed_math.h"

void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color);

/// Plots one pixel of the 128x64 canvas on a circle around its bottom middle
/// (63, 63): a radius (16.16) and an angle, the cosine leftward and the sine
/// upward.
void Func_080e7f94(s32 unused, s32 radius, s32 angle, s32 color) {
  /* FAKEMATCH(pin): the angle fixed in r8, which leaves the multiply's entry in
   * r10; as the plain parameter, 12 of 39 instructions differ in tla-en. */
  register s32 turn asm("r8") = angle;
  s32 center = 63;
  s32 x = Iwram_MulQ16(Trig_Cos(turn), radius);
  s32 y = Iwram_MulQ16(Trig_Sin(turn), radius);

  BattleEffect_PlotCanvasPixel(center - (x >> 16), center - (y >> 16), color);
}
