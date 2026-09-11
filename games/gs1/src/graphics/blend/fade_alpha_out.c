#include "types.h"
#include "scene.h"

/* graphics/blend/fade_alpha_out.c */
s32 WaitFrames(s32);
void Graphics_FadeAlphaOut(void)
{
  s32 alpha_step;
  unsigned long long ctrl;
  long long alpha;
  ctrl = 0x04000050;
  *((s16 *)ctrl) = 0x2044;
  alpha = 0x04000052;
  alpha_step = 1;
  do
  {
    do
    {
      *((s16 *)alpha) = 0x1010 - alpha_step;
      alpha_step += 2;
      WaitFrames(1);
    }
    while (0);
  }
  while (alpha_step <= 0x10);
}

/* graphics/blend/fade_alpha_in.c */
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

/* graphics/blend/set_control.c */
/*
 * Blend control setup.  The eight-byte owner at 0x080c0ea8 includes its two
 * trailing pool words, 0x000000bf and 0x04000050.
 */

#define REG_BLDCNT (*(volatile u16 *)0x04000050)

/* Set the blend control bits to 0xbf.  No arguments, no result. */
void Graphics_SetBlendControl(void)
{
    REG_BLDCNT = 0xbf;
}
