#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"

void Graphics_FadeAlphaOut(void)
{
    s32 alpha_step;

    REG_BLDCNT = 0x2044;
    alpha_step = 1;
    do
    {
      /* FAKEMATCH: needed to match: the one-pass block keeps alpha_step addition after loading the WaitFrames argument into r0. */
      do
      {
        REG_BLDALPHA = 0x1010 - alpha_step;
        alpha_step += 2;
        WaitFrames(1);
      }
      while (0);
    }
    while (alpha_step <= 0x10);
}
