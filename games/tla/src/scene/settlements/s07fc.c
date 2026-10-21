#include "local.h"

s32 Math_Sine(s32 angle);
u32 Random16Far(void);

/// Moves the object around its orbit's centre (words 14 and 15) and steps the
/// angle by a random amount.
// As ☀️'s Scene_UpdateFloatingNut does.
s32 Func_020007fc(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Math_Sine(p[12]) * 2;

  if (d > 0)
    d = -d;
  p[2] = p[14] + Math_Cosine(p[12]) * 2;
  p[3] = p[15] + d;
  q[9] = Math_Cosine(p[12] + 0x8000) / 8;
  a = Random16Far();
  b = Random16Far();
  {
    u32 x;
    s32 angle;

    /* FAKEMATCH: the barriers keep the first random value's shift after the
     * second call and the angle's load between the two shifts; plainly, 2
     * instructions differ in tla-en. */
    __asm__("");
    x = (u32)a << 9;
    angle = p[12];
    __asm__("");
    p[12] = angle + ((x >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  }
  return 0;
}
