#include "types.h"
#include "macros.h"

u16 Func_0803f974(s16 *rgb, s32 scale_r, s32 scale_g, s32 scale_b) {
  s32 r = rgb[0] * scale_r >> 16;
  s32 g = rgb[1] * scale_g >> 16;
  s32 b = rgb[2] * scale_b >> 16;

  CLAMP(r, 0, 31);
  CLAMP(g, 0, 31);
  CLAMP(b, 0, 31);
  r |= b << 10 | g << 5;
  return r;
}
