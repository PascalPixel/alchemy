#include "types.h"

s32 WaitFrames(s32);
void Graphics_FadeAlphaIn(void)
{
  unsigned long apply_step;
  s32 alpha_step;
  *((s16 *) 0x04000050) = 0x2044;
  alpha_step = 1;
  do
  {
    apply_step = 1;
    if (apply_step)
    {
      *((s16 *) 0x04000052) = alpha_step + 0x1000;
      alpha_step += 2;
      WaitFrames(1);
    }
  }
  while (alpha_step <= 0x10);
}
