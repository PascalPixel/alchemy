#include "system.h"
#include "types.h"
#include "io_reg.h"

void Graphics_FadeAlphaOut(void)
{
    s32 alpha_step;

    REG_BLDCNT = 0x2044;
    for (alpha_step = 1; alpha_step <= 16; alpha_step += 2)
    {
        REG_BLDALPHA = 0x1010 - alpha_step;
        WaitFrames(1);
    }
}
