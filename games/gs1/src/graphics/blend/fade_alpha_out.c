#include "types.h"

s32 WaitFrames(s32);
void Graphics_FadeAlphaOut(void)
{
  s32 alpha_step;
  unsigned long long ctrl;
  long long alpha;
  ctrl = 0x04000050;
  *((s16 *) ctrl) = 0x2044;
  alpha = 0x04000052;
  alpha_step = 1;
  do
  {
    do
    {
      *((s16 *) alpha) = 0x1010 - alpha_step;
      alpha_step += 2;
      WaitFrames(1);
    }
    while (0);
  }
  while (alpha_step <= 0x10);
}
